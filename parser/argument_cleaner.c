/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_cleaner.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:13:53 by aamoussa          #+#    #+#             */
/*   Updated: 2022/11/03 04:23:36 by aamoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_count_name_len(char *tmp, int *i)
{
	while (*tmp)
	{
		if (*tmp == '_' || ft_isalnum(*tmp))
		{
			tmp++;
			(*i)++;
		}
		else
			break ;
	}
}

char	*grep_name(char *s)
{
	char	*tmp;
	char	*name;
	int		i;

	i = 0;
	tmp = s;
	if (*s == '?')
	{
		return (ft_strdup("?"));
	}
	if (!*s)
		return (ft_strdup(s));
	name = NULL;
	ft_count_name_len(tmp, &i);
	name = (char *)malloc(i + 1);
	name = ft_strncpy(name, s, i);
	name[i] = 0;
	return (name);
}

char	*find_value(t_senv	**tmp, char *name, int counter)
{
	char	*variabl;

	variabl = NULL;
	while ((*tmp))
	{
		if (!ft_strcmp((*tmp)->key, name))
			break ;
		(*tmp) = (*tmp)->next;
	}
	if (*name == '?')
	{	
		if (counter == 0)
			variabl = ft_itoa(g_gb.exit_statut);
		else
			variabl = ft_itoa(0);
		ft_free(&name);
		return (variabl);
	}
	return (variabl);
}

char	*grep_variable(char *str, int counter)
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
	variabl = find_value(&tmp, name, counter);
	if (variabl)
		return (variabl);
	if (tmp && tmp->value)
		variabl = ft_strdup(tmp->value);
	put_zero_in_null(&variabl);
	ft_free(&name);
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

bool	clen_arguments_helper(t_execcmd *execcmd, int counter)
{
	if (execcmd->args)
	{	
		make_quotes(execcmd->args, true, counter);
		if (g_gb.status)
			return (true);
		convert_list_to_args(execcmd);
	}
	return (false);
}

void	clean_arguments(t_cmd *cmd, int *counter)
{
	t_execcmd	*execcmd;
	t_pipecmd	*pipecmd;

	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		execcmd = (t_execcmd *)cmd;
		clen_arguments_helper(execcmd, *counter);
	}
	if (cmd->type == PIPE)
	{
		pipecmd = (t_pipecmd *)(cmd);
		clean_arguments(pipecmd->left, counter);
		if (g_gb.status)
			return ;
		clean_arguments(pipecmd->right, counter);
		if (g_gb.status)
			return ;
	}
	(*counter)++;
}
