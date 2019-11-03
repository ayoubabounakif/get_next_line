/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabounak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 16:04:44 by aabounak          #+#    #+#             */
/*   Updated: 2019/11/03 23:41:21 by aabounak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	while (*(char*)s != '\0' && *s != (char)c)
		s++;
	if (*(char*)s == (char)c)
		return ((char*)s);
	return (0);
}

/*
 * In this function, we check if k < 0 which means that the number
 * of bytes read is negative (error), we then return -1.
 * We use the ft_strchr function to look for \n in our static variable,
 * if It's not found we look for \0 instead and we set our indicator to 0.
 * (The use of the indicator variable will be clear in a minut).
 * We then allocate enough memory for line (str - *s) + 1
 * If the memory allocation fails we return -1.
 * We copy what's on our static variable to line with len of (str - *s) + 1
 * We then return a string that accepts the whole static variable
 * but starting from the last index of line, we then substract
 * the length of the static variable and *line to give us the remaining.
 * We should free str to avoid memory leaks.
 * We check if indicator == 0 (That means that we got to the EOF)
 * (Remember how we set indicator == 0  when we find \0)
 * If the indicator is indeed == 0 we return 0
 * Else we return 1 as we still have more lines.
 * If BUFFER_SIZE is set to 0 nearly anything of the above will get executed
 * So we return 0.
*/

int		output(char **line, char **s, int k)
{
	char		*str;
	int			indicator;

	indicator = 1;
	if (k < 0)
		return (-1);
	if (!(str = ft_strchr(*s, '\n')))
	{
		str = ft_strchr(*s, '\0');
		indicator = 0;
	}
	str++;
	if (!(*line = malloc(sizeof(char) * (str - *s) + 1)))
		return (-1);
	ft_strlcpy(*line, *s, (str - *s));
	str = *s;
	*s = ft_substr(*s, ft_strlen(*line) + 1,
			ft_strlen(*s) - ft_strlen(*line) + 1);
	free(str);
	if ((indicator == 0) || *line)
	{
		s = 0;
		return ((indicator == 0) ? 0 : 1);
	}
	return (0);
}

/*
 * The static variable is used, so that whenever the function is called,
 * It remembers the previous call.
 * BUFFER_SIZE might be < 0 || fd < 0 (error) ||
 * **line might be null if there is no memory allocated to it.
 * We start by allocating to our static variable s using ft_strdup,
 * and also to avoid segmentation faults after using ft_strjoin.
 * The function read() if successful, returns the number of bytes read.
 * Inside the loop, the number of bytes read works like an index
 * so we use it to put a '\0' on the end of the buffer.
 * We then read more of the line and join them together using a temp string
 * This temporary string will store the data inside of the static variable
 * It is used to keep track of how much is read and then free the unused data.
*/

int		get_next_line(int fd, char **line)
{
	static char		*s;
	char			*buffer;
	char			*temp;
	int				k;

	if (BUFFER_SIZE < 0 || fd < 0 || line == NULL)
		return (-1);
	if (!(buffer = malloc(BUFFER_SIZE + 1)))
		return (-1);
	if (s == NULL)
		s = ft_strdup("");
	while (!ft_strchr(s, '\n') && (k = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[k] = '\0';
		temp = s;
		s = ft_strjoin(s, buffer);
		free(temp);
	}
	free(buffer);
	return (output(line, &s, k));
}

/*
 * 		WARNING !!
 * Do not push with the main function.
 * It's only purpose is testing.
*/

int		main(void)
{
	int		fd;
	char	*buffer;
	int		k;

	buffer = NULL;
	fd = open("lorem_ipsum.txt", O_RDONLY);
	while ((k = get_next_line(fd, &buffer)) > 0)
	{
		printf("%d | %s\n", k, buffer);
		free(buffer);
	}
	printf("%d | %s\n", k, buffer);
	free(buffer);
	return (0);
}
