
#include "minishell.h"


void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	is_a_path(char *arg)
{
	char	*tmp;

	tmp = arg;
	while (*tmp != '\0')
	{
		if (*tmp == '/')
			return (1);
		tmp++;
	}
	return (0);
}

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}
char	*find_correct_path(char *cmd, char **directories)
{
	int		len;
	char	*cmd_path;
	int		i;

	i = 0;
	while (directories[i] != NULL)
	{
		len = ft_strlen(directories[i]) + 1 + ft_strlen(cmd) + 1;
		cmd_path = malloc(len);
		if (!cmd_path)
			return (NULL);
		cmd_path[0] = '\0';
		ft_strlcat(cmd_path, directories[i], len);
		ft_strlcat(cmd_path, "/", len);
		ft_strlcat(cmd_path, cmd, len);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		else
			free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*handle_absolute_path(char *cmd_path)
{
	if (access(cmd_path, X_OK) == 0)
		return (ft_strdup(cmd_path));
	return (NULL);
}

char	*handle_relative_path(char *cmd,char **envp)
{
	char	*path;
	char	*cmd_path;
	char	**directories;

	path = get_path_from_env(envp);
	if (!path)
		return (NULL);
	directories = ft_split(path, ':');
	if (!directories)
		return (NULL);
	cmd_path = find_correct_path(cmd, directories);
	free_split(directories);
	return (cmd_path);
}

char	*get_path(char **args, char **envp)
{
	char	*cmd_path;

	if (is_a_path(args[0]))
		cmd_path = handle_absolute_path(args[0]);
	else
		cmd_path = handle_relative_path(args[0], envp);
	return (cmd_path);
}

void execute_external_cmd(char	**args, char **envp)
{
	char *cmd_path;
	//eventualmente vou ter de dar free a este path
	cmd_path = get_path(args, envp);
	printf("Path found for %s : %s\n",args[0],cmd_path);
	execve(cmd_path, args, envp); //talvez nao precise de dar free a path sendo que tudo vai ser destruido
}
