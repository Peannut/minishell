/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 14:09:58 by aamoussa          #+#    #+#             */
/*   Updated: 2022/10/29 14:14:14 by aamoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_input_redir(t_ends_of_buff *buff, t_ends_of_tok *str,
		t_execcmd *exec, int tok)
{
	char	*file;
	int		fd;

	if (tok != '<')
		return (false);
	tok = gettoken(buff->ps, buff->es, &(str)->q, &(str)->eq);
	if (ft_strchr(FORBIDEN_REDIR, tok))
	{	
		raise_error("syntax error near unexpected token", 258, tok);
		return (true);
	}
	file = ft_substr((str)->q, 0, ((str)->eq - (str)->q));
	fd = open(file, O_RDWR, 0644);
	if (fd == -1)
		printf("%s : No such file or directory\n", file);
	if (exec->input != 0)
		close(exec->input);
	exec->input = fd;
	free(file);
	return (false);
}

bool	parse_output_redir(t_ends_of_buff *buff, t_ends_of_tok *str,
		t_execcmd *exec, int tok)
{
	char	*file;
	int		fd;

	if (tok != '>')
		return (false);
	tok = gettoken(buff->ps, buff->es, &(str)->q, &(str)->eq);
	if (ft_strchr(FORBIDEN_REDIR, tok))
	{	
		raise_error("syntax error near unexpected token", 258, tok);
		return (true);
	}
	file = ft_substr((str)->q, 0, ((str)->eq - (str)->q));
	fd = open(file, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		printf("%s : No such file or directory\n", file);
	if (exec->output != 1)
		close(exec->output);
	exec->output = fd;
	free(file);
	return (false);
}

bool	parse_heredoc(t_ends_of_buff *buff, t_ends_of_tok *str,
		t_execcmd *exec, int tok)
{
	char	*here_doc_lim;
	int		her_doc_len;

	if (tok != '-')
		return (false);
	here_doc_lim = NULL;
	tok = gettoken(buff->ps, buff->es, &(str)->q, &(str)->eq);
	if (ft_strchr(FORBIDEN_REDIR, tok))
	{	
		raise_error("syntax error near unexpected token", 258, tok);
		return (true);
	}
	her_doc_len = ((str)->eq - (str)->q);
	here_doc_lim = ft_substr((str)->q, 0, her_doc_len);
	exec->input = here_doc(here_doc_lim);
	ft_free(&here_doc_lim);
	return (false);
}

bool	parse_output_append(t_ends_of_buff *buff, t_ends_of_tok *str,
		t_execcmd *exec, int tok)
{
	char	*file;
	int		fd;

	if (tok != '+')
		return (false);
	tok = gettoken(buff->ps, buff->es, &(str)->q, &(str)->eq);
	if (ft_strchr(FORBIDEN_REDIR, tok))
	{	
		raise_error("syntax error near unexpected token", 1, tok);
		return (true);
	}
	file = ft_substr((str)->q, 0, ((str)->eq - (str)->q));
	fd = open(file, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		printf("%s No such file or directory\n", file);
	if (exec->output != 1)
		close(exec->output);
	exec->output = fd;
	free(file);
	return (false);
}

t_cmd	*parseredirec(char **ps, char *es, t_cmd *cmd)
{
	int				tok;
	t_ends_of_tok	q_and_eq;
	t_ends_of_buff	buff;
	t_execcmd		*exec;

	buff.ps = (ps);
	buff.es = es;
	exec = (t_execcmd *)cmd;
	while (skip_and_find(buff.ps, buff.es, "<>"))
	{
		tok = gettoken(buff.ps, buff.es, 0, 0);
		if (parse_input_redir(&buff, &q_and_eq, exec, tok))
			return (NULL);
		if (parse_output_redir(&buff, &q_and_eq, exec, tok))
			return (NULL);
		if (parse_heredoc(&buff, &q_and_eq, exec, tok))
			return (NULL);
		if (parse_output_append(&buff, &q_and_eq, exec, tok))
			return (NULL);
	}
	return ((t_cmd *)(exec));
}