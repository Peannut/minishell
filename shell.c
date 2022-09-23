/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoukaddo <zoukaddo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 08:09:27 by zoukaddo          #+#    #+#             */
/*   Updated: 2022/09/23 23:22:05 by zoukaddo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*getprompt(char **envp)
{
	return ("Minishell$ ");
}

pid_t	fork_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Failed to fork process");
		exit(1);
	}
	return (pid);
}

int	main(int ac, char **argv, char **envp)
{
	char	*buffer;
	t_cmd	*cmd;
	bool	again;
	char	*ps;
	char	*es;
	pid_t	pid;
	again = true;
	gb.envp = 0;
	int j = 0;
	gb.status = 0;
	setup_env(envp);
	// env();
	while (again)
	{
		buffer = readline(getprompt(envp));
		if (!buffer[0])
			continue ;
		es = &buffer[ft_strlen(buffer)];
		add_history(buffer);
		ps = buffer;
		cmd = parser(&ps, es, envp);
		// print_tree(cmd);
		// exit(1);
		if (!cmd)
			continue;
		if (cmd->type == REDIR || cmd->type == EXEC)
		{	
			execute_builtins(cmd);
		}
		else 
		{	
			executer(cmd);
			while (waitpid(-1, NULL, 0) != -1)
				;

		}
		// printf("exit status: %d\n", gb.exit_statut);    
		// print_tree(cmd);
	}
	return (0);
}