/* mapfile.c, created from mapfile.def. */
#line 23 "./mapfile.def"

#line 56 "./mapfile.def"

#line 64 "./mapfile.def"

#include <config.h>

#include "builtins.h"
#include "posixstat.h"

#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif

#include "bashansi.h"
#include "bashintl.h"

#include <stdio.h>
#include <errno.h>

#include "../bashintl.h"
#include "../shell.h"
#include "common.h"
#include "bashgetopt.h"

#if !defined (errno)
extern int errno;
#endif

#if defined (ARRAY_VARS)

#define DEFAULT_ARRAY_NAME	"MAPFILE"

/* The value specifying how frequently `mapfile'  calls the callback. */
#define DEFAULT_QUANTUM 5000

/* Values for FLAGS */
#define MAPF_CLEARARRAY	0x01
#define MAPF_CHOP	0x02

static int
run_callback(callback, current_index)
     const char *callback;
     unsigned int current_index;
{
  unsigned int execlen;
  char  *execstr;
  int flags;

  execlen = strlen (callback) + 10;
  /* 1 for space between %s and %d,
     another 1 for the last nul char for C string. */
  execlen += 2;
  execstr = xmalloc (execlen);

  flags = SEVAL_NOHIST;
#if 0
  if (interactive)
    flags |= SEVAL_INTERACT;
#endif
  snprintf (execstr, execlen, "%s %d", callback, current_index);
  return parse_and_execute(execstr, NULL, flags);
}

static void
do_chop(line)
     char * line;
{
  int length;

  length = strlen (line);
  if (length && line[length-1] == '\n') 
    line[length-1] = '\0';
}

static int
mapfile (fd, line_count_goal, origin, nskip, callback_quantum, callback, array_name, flags)
     int fd;
     long line_count_goal, origin, nskip, callback_quantum;
     char *callback, *array_name;
     int flags;
{
  char *line;
  size_t line_length;
  unsigned int array_index, line_count;
  SHELL_VAR *entry;
  int unbuffered_read;
  
  line = NULL;
  line_length = 0;
  unbuffered_read = 0;

  /* The following check should be done before reading any lines.  Doing it
     here allows us to call bind_array_element instead of bind_array_variable
     and skip the variable lookup on every call. */
  entry = find_or_make_array_variable (array_name, 1);
  if (entry == 0 || readonly_p (entry) || noassign_p (entry))
    {
      if (entry && readonly_p (entry))
	err_readonly (array_name);
	
      return (EXECUTION_FAILURE);
    }
  else if (array_p (entry) == 0)
    {
      builtin_error (_("%s: not an indexed array"), array_name);
      return (EXECUTION_FAILURE);
    }
      
  if (flags & MAPF_CLEARARRAY)
    array_flush (array_cell (entry));

#ifndef __CYGWIN__
  unbuffered_read = (lseek (fd, 0L, SEEK_CUR) < 0) && (errno == ESPIPE);
#else
  unbuffered_read = 1;
#endif

  zreset ();

  /* Skip any lines at beginning of file? */
  for (line_count = 0; line_count < nskip; line_count++)
    if (zgetline (fd, &line, &line_length, unbuffered_read) < 0)
      break;

  line = 0;
  line_length = 0;    

  /* Reset the buffer for bash own stream */
  interrupt_immediately++;
  for (array_index = origin, line_count = 1; 
       zgetline (fd, &line, &line_length, unbuffered_read) != -1;
       array_index++, line_count++) 
    {
      /* Have we exceeded # of lines to store? */
      if (line_count_goal != 0 && line_count > line_count_goal) 
	break;

      /* Remove trailing newlines? */
      if (flags & MAPF_CHOP)
	do_chop (line);
	  
      /* Has a callback been registered and if so is it time to call it? */
      if (callback && line_count && (line_count % callback_quantum) == 0) 
	{
	  run_callback (callback, array_index);

	  /* Reset the buffer for bash own stream. */
	  if (unbuffered_read == 0)
	    zsyncfd (fd);
	}

      bind_array_element (entry, array_index, line, 0);
    }

  xfree (line);

  if (unbuffered_read == 0)
    zsyncfd (fd);

  interrupt_immediately--;
  return EXECUTION_SUCCESS;
}

int
mapfile_builtin (list)
     WORD_LIST *list;
{
  int opt, code, fd, clear_array, flags;
  intmax_t intval;
  long lines, origin, nskip, callback_quantum;
  char *array_name, *callback;

  clear_array = 1;
  fd = 0;
  lines = origin = nskip = 0;
  flags = MAPF_CLEARARRAY;
  callback_quantum = DEFAULT_QUANTUM;
  callback = 0;

  reset_internal_getopt ();
  while ((opt = internal_getopt (list, "u:n:O:tC:c:s:")) != -1)
    {
      switch (opt)
	{
	case 'u':
	  code = legal_number (list_optarg, &intval);
	  if (code == 0 || intval < 0 || intval != (int)intval)
	    {
	      builtin_error (_("%s: invalid file descriptor specification"), list_optarg);
	      return (EXECUTION_FAILURE);
	    }
	  else
	    fd = intval;

	  if (sh_validfd (fd) == 0)
	    {
	      builtin_error (_("%d: invalid file descriptor: %s"), fd, strerror (errno));
	      return (EXECUTION_FAILURE);
	    }
	  break;	  

	case 'n':
	  code = legal_number (list_optarg, &intval);
	  if (code == 0 || intval < 0 || intval != (unsigned)intval)
	    {
	      builtin_error (_("%s: invalid line count"), list_optarg);
	      return (EXECUTION_FAILURE);
	    }
	  else
	    lines = intval;
	  break;

	case 'O':
	  code = legal_number (list_optarg, &intval);
	  if (code == 0 || intval < 0 || intval != (unsigned)intval)
	    {
	      builtin_error (_("%s: invalid array origin"), list_optarg);
	      return (EXECUTION_FAILURE);
	    }
	  else
	    origin = intval;
	  flags &= ~MAPF_CLEARARRAY;
	  break;
	case 't':
	  flags |= MAPF_CHOP;
	  break;
	case 'C':
	  callback = list_optarg;
	  break;
	case 'c':
	  code = legal_number (list_optarg, &intval);
	  if (code == 0 || intval <= 0 || intval != (unsigned)intval)
	    {
	      builtin_error (_("%s: invalid callback quantum"), list_optarg);
	      return (EXECUTION_FAILURE);
	    }
	  else
	    callback_quantum = intval;
	  break;
	case 's':
	  code = legal_number (list_optarg, &intval);
	  if (code == 0 || intval < 0 || intval != (unsigned)intval)
	    {
	      builtin_error (_("%s: invalid line count"), list_optarg);
	      return (EXECUTION_FAILURE);
	    }
	  else
	    nskip = intval;
	  break;
	default:
	  builtin_usage ();
	  return (EX_USAGE);
	}
    }
  list = loptend;

  if (list == 0) 
    array_name = DEFAULT_ARRAY_NAME;
  else if (list->word == 0 || list->word->word == 0)
    {
      builtin_error ("internal error: getting variable name");
      return (EXECUTION_FAILURE);
    }
  else if (list->word->word[0] == '\0')
    {
      builtin_error (_("empty array variable name"));
      return (EX_USAGE);
    } 
  else
    array_name = list->word->word;
  
  if (legal_identifier (array_name) == 0 && valid_array_reference (array_name) == 0)
    {
      sh_invalidid (array_name);
      return (EXECUTION_FAILURE);
    }

  return mapfile (fd, lines, origin, nskip, callback_quantum, callback, array_name, flags);
}

#else

int
mapfile_builtin (list)
     WORD_LIST *list;
{
  builtin_error (_("array variable support required"));
  return (EXECUTION_FAILURE);
}

#endif  /* ARRAY_VARS */
