/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarylak <mbarylak@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:24:31 by mbarylak          #+#    #+#             */
/*   Updated: 2022/03/23 18:00:33 by mbarylak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_get_path(t_pipex *pipex)
{
	int		i;

	i = 0;
	while (pipex->env[i])
	{
		if (ft_strncmp(pipex->env[i], "PATH", 4) == 0)
			pipex->path = ft_split(ft_strchr(pipex->env[i], '/'), ':');
		i++;
	}
	i = 0;
	while (pipex->path[i])
	{
		pipex->path[i] = ft_strjoin(pipex->path[i], "/");
		i++;
	}
}

void	ft_get_files(int argc, char **argv, t_pipex *pipex)
{
	pipex->infile = argv[1];
	pipex->outfile = argv[argc - 1];
}

char	*get_right_path(char *cmd, t_pipex *pipex)
{
	char	*path;
	int		i;

	i = 0;
	while (pipex->path[i])
	{
		path = ft_strjoin(pipex->path[i], cmd);
		if (access(path, X_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);
}

void	ft_exe(char **argv, int n, t_pipex *pipex)
{
	char	*path;
	char	**cmd;

	cmd = ft_split(argv[n], ' ');
	if (access(cmd[0], X_OK) == 0)
		if (execve(cmd[0], cmd, pipex->env) == -1)
			ft_error(cmd[0], 0);
	path = get_right_path(cmd[0], pipex);
	if (execve(path, cmd, pipex->env) == -1)
		ft_error(cmd[0], 0);
	free(path);
	ft_free(cmd);
}

void	ft_get_info(int argc, char **argv, char **env, t_pipex *pipex)
{
	pipex->env = env;
	ft_get_path(pipex);
	ft_get_files(argc, argv, pipex);
}
