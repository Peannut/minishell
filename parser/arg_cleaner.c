/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_cleaner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoussa <aamoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 14:35:19 by aamoussa          #+#    #+#             */
/*   Updated: 2022/11/03 04:05:43 by aamoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	convert_list_to_args(t_execcmd *execcmd)
{
	size_t	size;
	int		i;
	t_list	*args;

	i = 0;
	size = ft_lstsize(execcmd->args);
	execcmd->argument = malloc(sizeof(char *) * (size + 1));
	execcmd->argument[size] = NULL;
	while (execcmd->args)
	{	
		if (!execcmd->args->content)
		{
			args = execcmd->args;
			execcmd->args = execcmd->args->next;
			free(args);
			continue ;
		}
		execcmd->argument[i] = execcmd->args->content;
		args = execcmd->args;
		execcmd->args = execcmd->args->next;
		free(args);
		i++;
	}
	execcmd->argument[i] = NULL;
	free(execcmd->args);
}

char	*merge(t_list *tmp)
{
	char	*next;
	t_list	*del;
	char	*content;

	content = NULL;
	put_zero_in_null(&content);
	if (tmp)
	{	
		while (tmp->next)
		{	
			next = (tmp->next)->content;
			(tmp->next)->content = ft_strjoin(tmp->content,
					(tmp->next)->content);
			ft_free(&(next));
			del = tmp;
			tmp = tmp->next;
			ft_free(&del->content);
			free(del);
		}
		ft_free(&content);
		content = ft_strdup(tmp->content);
		ft_free(&tmp->content);
		free(tmp);
	}
	return (content);
}

char	*merge_list(t_list **head)
{
	t_list	*lst;
	t_list	*tmp;
	char	*content;

	lst = *head;
	tmp = lst;
	content = NULL;

	return (merge(tmp));
}

bool	check_sq(t_list **tmp)
{
	if ((*tmp)->state == SQ)
	{
		*tmp = (*tmp)->next;
		return (true);
	}
	return (false);
}

void	split_dollar(t_list *args, int counter)
{
	t_list	*tmp;
	t_list	*lst_of_dollar;
	int		i;
	char	*arg;
	char	*hold;

	tmp = args;
	lst_of_dollar = NULL;
	while (tmp)
	{
		i = 0;
		if (check_sq(&tmp))
			continue ;
		arg = tmp->content;
		while (arg[i])
		{	
			collect_var(&lst_of_dollar, &i, arg, tmp);
			expand_lst(lst_of_dollar, counter);
		}
		hold = tmp->content;
		tmp->content = merge_list(&lst_of_dollar);
		ft_free(&hold);
		lst_of_dollar = NULL;
		tmp = tmp->next;
	}
}

void	collect_sq_and_dq(t_list **split_args, char *line, char q)
{
	char	*content;
	int		i;

	i = 0;
	while (line[i])
	{
		collect_word(split_args, line, &i, q);
		if (line[i] && (line[i] == '\'' || line[i] == '\"'))
		{
			q = line[i];
			content = ft_substr(line, i + 1, count_len(i, line, line[i]));
			ft_lstadd_back(split_args, ft_lstnew(content, q));
			i += count_len(i, line, line[i]) + 1;
		}
		if (!line[i])
			break ;
		i++;
	}
}

void	make_quotes(t_list	*args, bool i, int counter)
{
	t_list		*split_args;
	char		q;
	char		*line;
	t_list		*tmp;

	tmp = args;
	while (tmp)
	{
		split_args = NULL;
		line = tmp->content;
		if (ft_strlen(line) == 1 && line[0] == '$')
		{	
			tmp = tmp->next;
			continue ;
		}
		q = '\"';
		collect_sq_and_dq(&split_args, line, q);
		if (i)
			split_dollar(split_args, counter);
		ft_free(&tmp->content);
		tmp->content = merge_list(&split_args);
		tmp = tmp->next;
	}
}
