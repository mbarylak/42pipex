/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarylak <mbarylak@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:22:53 by mbarylak          #+#    #+#             */
/*   Updated: 2022/03/17 19:43:42 by mbarylak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(int n)
{
	if (n == 0)
		ft_putstr_fd("System Error\n", 2);
	else if (n == 1)
		ft_putstr_fd("Input Error\n", 2);
	exit(EXIT_FAILURE);
}

void	ft_pipex_s(int n, char **argv, t_pipex *pipex)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		ft_error(0);
	pid = fork();
	if (pid == -1)
		ft_error(0);
	if (pid == 0)
	{
		close(fd[READ_END]);
		if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
			ft_error(0);
		close(fd[WRITE_END]);
		ft_exe(argv, n, pipex);
	}
	else
		ft_pipex_p(fd, pid);
}

void	ft_pipex_p(int *fd, int pid)
{
	int	status;

	close(fd[WRITE_END]);
	dup2(fd[READ_END], STDIN_FILENO);
	close(fd[READ_END]);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == EXIT_FAILURE)
		exit(1);
}

void	ft_free(char **m)
{
	int	i;

	i = 0;
	while (m[i] != NULL)
	{
		free(m[i]);
		i++;
	}
	free(m);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;
	int		i;

	if (argc < 5 || !env[0])
		ft_error(1);
	pipex.env = env;
	ft_get_path(&pipex);
	ft_get_files(argc, argv, &pipex);
	pipex.fd_out = open(pipex.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	pipex.fd_in = open(pipex.infile, O_RDONLY, 0777);
	if (pipex.fd_in == -1 || pipex.fd_out == -1)
		ft_error(1);
	if (dup2(pipex.fd_in, STDIN_FILENO) == -1)
		ft_error(0);
	i = 2;
	while (i < argc - 2)
		ft_pipex_s(i++, argv, &pipex);
	if (dup2(pipex.fd_out, STDOUT_FILENO) == -1)
		ft_error(0);
	ft_exe(argv, argc - 2, &pipex);
	close(pipex.fd_in);
	close(pipex.fd_out);
	return (0);
}
