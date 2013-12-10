#include <unistd.h>
#include <stdlib.h>

char			*readline()
{
  char			*line = 0;
  static char		buff[100];
  static ssize_t	buff_length;
  static ssize_t	old_index = sizeof(buff);
  size_t		i = 0;
  size_t		length = 0;

  while (1)
  {
    length += buff_length;
    line = realloc(line, length);
    for (; old_index < buff_length; ++old_index)
      {
	line[i++] = buff[old_index];
	if (buff[old_index] == '\n')
	  {
	    line[i - 1] = '\0';
	    ++old_index;
	    return (line);
	  }
      }
    old_index = 0;
    buff_length = read(0, buff, sizeof(buff));
    if (buff_length <= 0)
      {
	return (NULL);
      }
  }
}
