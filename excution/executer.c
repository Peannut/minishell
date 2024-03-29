/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 17:08:23 by zoukaddo          #+#    #+#             */
/*   Updated: 2022/11/03 06:03:13 by aamoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./utils/execution.h"
#include "../parser/parser.h"

int	checkbult(t_execcmd *exec)
{
	if (ft_strcmp(exec->argument[0], "echo") == 0)
		ft_echo(exec->argument);
	else if (ft_strcmp(exec->argument[0], "export") == 0)
	{		
		ft_export(exec->argument);
		get_envp();
	}
	else if (ft_strcmp(exec->argument[0], "unset") == 0)
		unset_cmd(exec->argument);
	else if (ft_strcmp(exec->argument[0], "exit") == 0)
		g_gb.exit_statut = exit_cmd(exec->argument);
	else if (ft_strcmp(exec->argument[0], "env") == 0)
		env(exec->argument);
	else if (ft_strcmp(exec->argument[0], "pwd") == 0)
		built_in_pwd(exec->argument, g_gb.curent);
	else
		return (1);
	return (0);
}

int	checifbuiltin(t_execcmd *exec)
{
	char	*rev;

	if (checkbuiltprotection(exec))
		return (0);
	if (ft_strcmp(exec->argument[0], "cd") == 0)
	{
		if (cd_cmd(exec->argument) == 0)
		{
			if (g_gb.curent)
				free(g_gb.curent);
			g_gb.curent = getcwd(NULL, 0);
		}
		else
		{
			rev = ft_strjoin(g_gb.curent, "/.");
			if (g_gb.curent)
				free(g_gb.curent);
			g_gb.curent = rev;
		}
	}
	else if (checkbult(exec) == 0)
		;
	else
		return (1);
	return (0);
}

char	**get_paths(void)
{
	int		i;
	t_senv	*tmp;
	char	**paths;

	i = 0;
	tmp = g_gb.env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
		{	
			paths = ft_split(tmp->value, ':');
			if (paths)
			{
				while (paths[i])
				{
					paths[i] = ft_strjoin(paths[i], "/");
					i++;
				}
				return (paths);
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	check_access(char **paths, t_execcmd *cmd)
{
	int		i;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], cmd->argument[0]);
		if (!access(tmp, X_OK))
		{
			cmd->argument[0] = tmp;
			return ;
		}
		i++;
	}
}

void	execute_cmd(t_execcmd *cmd)
{
	char	**paths;

	if (!cmd->argument[0])
		exit(g_gb.exit_statut = 0);
	if (cmd->argument[0][0] == '\0')
	{
		ft_putstr_fd("Minishell :", 2);
		ft_putstr_fd(cmd->argument[0], 2);
		ft_putstr_fd(" :command not found\n", 2);
		g_gb.exit_statut = 127;
		exit(g_gb.exit_statut);
	}
	paths = get_paths();
	if (!paths)
	{
		ft_putstr_fd("Minishell :", 2);
		ft_putstr_fd(cmd->argument[0], 2);
		ft_putstr_fd(" No such file or directory\n", 2);
		g_gb.exit_statut = 127;
		exit(g_gb.exit_statut = 127);
	}
	check_access(paths, cmd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(cmd->argument[0], cmd->argument, g_gb.envp) == -1)
		error_displayer(cmd);
}
