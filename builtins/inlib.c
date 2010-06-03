/* inlib.c, created from inlib.def. */
#line 22 "./inlib.def"
#include <config.h>

#include <stdio.h>
#include "../shell.h"

#line 43 "./inlib.def"

#if defined (apollo)

#include <apollo/base.h>
#include <apollo/loader.h>

inlib_builtin (list)
     WORD_LIST *list;
{
  status_$t status;
  int return_value;
  short len;

  if (!list)
    {
      builtin_usage ();
      return (EX_USAGE);
    }

  return_value = EXECUTION_SUCCESS;

  while (list)
    {
      len = (short)strlen (list->word->word);
      loader_$inlib (list->word->word, len, &status);

      if (status.all != status_$ok)
	{
	  builtin_error (_("%s: inlib failed"), list->word->word);
	  return_value = EXECUTION_FAILURE;
	}

      list = list->next;
    }

  return (return_value);
}
#endif /* apollo */
