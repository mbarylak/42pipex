#include "pipex.h"

void	ft_error(void)
{
	ft_putstr_fd("ERROR\n", 2);
	exit(EXIT_FAILURE);
}

void	ft_pipex_s(int fd_in, int fd_out, char **argv, t_pipex *pipex)
{
	int	pid;
	int	fd[2];
	int	status;

	if (pipe(fd) == -1)
		ft_error();
	pid = fork();
	if (pid == -1)
		ft_error();
	if (pid == 0)
	{
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		ft_exe(argv, 2, pipex);
		close(fd_in);
	}
	else
	{
		waitpid(pid, &status, 0);
		ft_pipex_p(fd, fd_out, argv, pipex);
	}
}

void	ft_pipex_p(int *fd, int fd_out, char **argv, t_pipex *pipex)
{
	close(fd[WRITE_END]);
	dup2(fd[READ_END], STDIN_FILENO);
	close(fd[READ_END]);
	dup2(fd_out, STDOUT_FILENO);
	ft_exe(argv, 3, pipex);
	close(fd_out);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;
	int		fd_in;
	int		fd_out;

	if (argc < 5)
		ft_error();
	pipex.env = env;
	ft_get_path(&pipex);
	ft_get_files(argc, argv, &pipex);
	fd_out = open(pipex.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fd_in = open(pipex.infile, O_RDONLY, 0777);
	if (fd_in == -1 || fd_out == -1)
		ft_error();
	dup2(fd_in, STDIN_FILENO);
	ft_pipex_s(fd_in, fd_out, argv, &pipex);
	return (0);
}
