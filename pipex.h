#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

# define READ_END 0
# define WRITE_END 1

typedef struct	s_pipex
{
	int		fd_in;
	int		fd_out;
	char	*infile;
	char	*outfile;
	char	**env;
	char	**path;
}	t_pipex;

/* TOOLS */

char	**ft_split(const char *s, char c);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);

/* PROG */

void	ft_get_path(t_pipex *pipex);
void	ft_get_files(int argc, char **argv, t_pipex *pipex);
char	*get_right_path(char *cmd, t_pipex *pipex);
void	ft_exe(char **argv, int n, t_pipex *pipex);
void	ft_pipex_s(int n, char **argv, t_pipex *pipex);
void	ft_pipex_p(int *fd, int pid);

/* ERROR MNGT */

void	ft_error(void);

#endif
