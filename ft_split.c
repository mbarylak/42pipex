/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarylak <mbarylak@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:22:22 by mbarylak          #+#    #+#             */
/*   Updated: 2022/03/17 18:22:33 by mbarylak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_word_nbr(const char *s, char c)
{
	int	i;
	int	w_nbr;

	i = 0;
	w_nbr = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			w_nbr++;
		i++;
	}
	return (w_nbr);
}

static int	get_word_len(const char *s, char c, int i)
{
	int	w_len;

	w_len = 0;
	while (s[i])
	{
		if (s[i] != c)
			w_len++;
		else
			break ;
		i++;
	}
	return (w_len);
}

static void	*free_mem(char **split, int i)
{
	while (--i >= 0)
		free(split[i]);
	free(split);
	return (NULL);
}

static char	**fill_split(const char *s, char c, int w_nbr, char **split)
{
	int	i;
	int	j;
	int	k;
	int	w_len;

	i = 0;
	j = 0;
	while (i < w_nbr)
	{
		while (s[j] == c)
			j++;
		w_len = get_word_len(s, c, j);
		split[i] = (char *)malloc((w_len + 1) * sizeof(char));
		if (!split[i])
			return (free_mem(split, i));
		k = 0;
		while (k < w_len)
			split[i][k++] = s[j++];
		split[i][k] = '\0';
		i++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(const char *s, char c)
{
	char	**split;
	int		w_nbr;

	if (!s)
		return (NULL);
	w_nbr = get_word_nbr(s, c);
	split = (char **) malloc(sizeof (char *) * (w_nbr + 1));
	if (!split)
		return (NULL);
	return (fill_split(s, c, w_nbr, split));
}
