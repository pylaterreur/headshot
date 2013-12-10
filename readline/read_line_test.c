#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char			*readline();

int			main()
{
  const char		*const str[] = {"abcdefghiklmnopqrstuvwxyz",
					"",
					"1",
					"",
					"234",
					"",
					"",
					"567893982138921783218372183892189312837218937892178932189371289732891738921783921731197981"
  };
  size_t		i;
  const char		new_line_char = '\n';
  pid_t pid;
  int pipefd[2];

  if (-1 == pipe(pipefd))
    {
      fputs("Error : couldn't pipe", stderr);
      return (EXIT_FAILURE);
    }
  pid = fork();
  if (-1 == pid)
    {
      fputs("Error: couldn't fork", stderr);
      return (EXIT_FAILURE);
    }
  if (pid) /* parent */
    {
      close(pipefd[0]);
      if (-1 == dup2(pipefd[1], 1))
	{
	  fputs("Error: couldn't dup2", stderr);
	  return (EXIT_FAILURE);	  
	}
      for (i = 0; i < sizeof(str) / sizeof(*str); ++i)
	{
	  write(1, str[i], strlen(str[i]) * sizeof(*str[i]));
	  write(1, &new_line_char, sizeof(new_line_char));
	}
      wait(NULL);
    }
  else /* child */
    {
      if (-1 == dup2(pipefd[0], 0))
	{
	  fputs("Error: couldn't dup2", stderr);
	  return (EXIT_FAILURE);
	}
      /* read */
      for (i = 0; i < sizeof(str) / sizeof(*str); ++i)
	{
	  char *line = readline();
	  if (!line)
	    {
	      fprintf(stderr, "Awaited result: \"%s\", but got null string\n", str[i]);
	    }
	  else if (strcmp(line, str[i]) != 0)
	    {
	      fprintf(stderr, "Awaited result: \"%s\", but got \"%s\"\n", str[i], line);
	    }
	  else
	    {
	      fprintf(stderr, "Test #%u passed: \"%s\"\n", i, line);
	    }
	  free(line);
	}
    }
  return (EXIT_SUCCESS);
}
