/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 08:09:27 by zoukaddo          #+#    #+#             */
/*   Updated: 2022/10/20 06:51:50 by aamoussa         ###   ########.fr       */
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
	gb.input = 0;
	gb.output = 1;
	setup_env(envp);
	// env();
	while (again)
	{
		gb.status = 0;
		signals();
		buffer = readline(getprompt(envp));
		if (!buffer)
			break;
		if (!buffer[0])
			continue ;
		es = &buffer[ft_strlen(buffer)];
		add_history(buffer);
		ps = buffer;
		cmd = parser(&ps, es, envp);
		if (!cmd)
			continue;
		// print_tree(cmd);
		// exit(1);
		if (cmd->type == EXEC)
			execute_builtins(cmd);
		else
		{
			// executer(cmd);
			// pipe_executer(cmd);
			while (waitpid(-1, NULL, 0) != -1)
				;
		}
		// printf("exit status: %d\n", gb.exit_statut);    
		// print_tree(cmd);
	}
	return (0);
}