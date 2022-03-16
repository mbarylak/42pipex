#include "pipex.h"

void	ft_error(void)
{
	ft_putstr_fd("ERROR\n", 2);
	exit(EXIT_FAILURE);
}

void	ft_pipex_s(int n, char **argv, t_pipex *pipex)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		ft_error();
	pid = fork();
	if (pid == -1)
		ft_error();
	if (pid == 0)
	{
		close(fd[READ_END]);
		if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
			ft_error();
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

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;
	int		i;

	if (argc < 5)
		ft_error();
	pipex.env = env;
	ft_get_path(&pipex);
	ft_get_files(argc, argv, &pipex);
	pipex.fd_out = open(pipex.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	pipex.fd_in = open(pipex.infile, O_RDONLY, 0777);
	if (pipex.fd_in == -1 || pipex.fd_out == -1)
		ft_error();
	if (dup2(pipex.fd_in, STDIN_FILENO) == -1)
		ft_error();
	i = 2;
	while (i < argc - 2)
		ft_pipex_s(i++, argv, &pipex);
	if (dup2(pipex.fd_out, STDOUT_FILENO) == -1)
		ft_error();
	ft_exe(argv, argc - 2, &pipex);
	close(pipex.fd_in);
	close(pipex.fd_out);
	return (0);
}
