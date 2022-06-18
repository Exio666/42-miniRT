/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 14:37:51 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/17 14:18:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_parser.h"

void	*set_parser_errcode(t_parser *parser, t_parser_errcode errcode)
{
	parser->errcode = errcode;
	return (NULL);
}

const char	*get_parser_err_msg(t_parser_errcode errcode)
{
	if (errcode == E_INVALID_VALUE)
		return (STR_INVALID_VALUE);
	else if (errcode ==  E_INVALID_RANGE)
		return (STR_INVALID_RANGE);
	else if (errcode == E_EXPECTED_COMMA)
		return (STR_EXPECTED_COMMA);
	else if (errcode == E_EXPECTED_IDENTIFIER)
		return (STR_EXPECTED_IDENTIFIER);
	else if (errcode == E_EXPECTED_VALUE)
		return (STR_EXPECTED_VALUE);
	else if (errcode == E_EXPECTED_NEWLINE)
		return (STR_EXPECTED_NEWLINE);
	else
		return (NULL);
}
