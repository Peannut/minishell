/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_cleaner.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:13:53 by aamoussa          #+#    #+#             */
/*   Updated: 2022/10/29 14:56:29 by aamoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*grep_name(char *s)
{
	char *tmp;
	char *name;
	int  i;

	i = 0;
	tmp = s;
	if (*s == '?')
	{
		return (ft_strdup("?"));
	}
	if (!*s)
		return (s);
	name = NULL;
	while (*tmp)
	{
		if (!ft_isalpha(*tmp) && *tmp!='_')
			break;
		tmp++;
		i++;
	}
	name = (char *)malloc(i + 1);
	name = ft_strncpy(name, s, i);
	name[i] = 0;
	// name[i] = '=';
	return (name);
}

char	*grep_variable(char *str)
{
	char	*name;
	char	*variabl;
	t_senv	*tmp;

	tmp = NULL;
	tmp = g_gb.env;
	name = grep_name(str);
	if (!*name)
		return (name);
	variabl = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, name))
			break ;
		tmp = tmp->next;
	}
	if (*name == '?')
	{	
		variabl = ft_itoa(g_gb.exit_statut);
		return (variabl);
	}
	if (tmp)
		variabl = tmp->value;
	put_zero_in_null(&variabl);
	return (variabl);
}

void	word_len(char *arg, int *i, int *len)
{
	while (arg[*i] && arg[*i] != '$')
	{	
		(*len)++;
		(*i)++;
	}	
}

bool	clen_arguments_helper(t_execcmd *execcmd)
{
	if (execcmd->args)
	{	
		make_quotes(execcmd->args);
		if (g_gb.status)
			return (true);
		convert_list_to_args(execcmd);
	}	
	return (false);
}

void	clean_arguments(t_cmd *cmd)
{
	t_execcmd	*execcmd;
	t_pipecmd	*pipecmd;

	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		execcmd = (t_execcmd *)cmd;
		clen_arguments_helper(execcmd);
	}
	if (cmd->type == PIPE)
	{
		pipecmd = (t_pipecmd *)(cmd);
		clean_arguments(pipecmd->left);
		if (g_gb.status)
			return ;
		clean_arguments(pipecmd->right);
		if (g_gb.status)
			return ;
	}
}