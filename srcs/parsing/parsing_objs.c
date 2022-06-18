/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_objs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 11:21:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/18 14:19:56 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_parser.h"
#include "minirt_types.h"
#include <stdlib.h>

void	*check_rgb_component(t_parser *parser, char c, char *value,
		uint32_t *rgb)
{
	uint16_t	component_value;

	if (ft_strlen(value) <= 3 && ft_stronly_digits(value))
	{
		component_value = ft_atoi(value);
		if (component_value >= 0 && component_value <= 255)
		{
			if (c == 'R')
				*rgb = component_value;
			else if (c == 'G')
				*rgb |= component_value << 8;
			else if (c == 'B')
				*rgb |= component_value << 16;
		}
		else
			return (set_parser_errcode(parser, E_INVALID_RANGE));
	}
	else
		return (set_parser_errcode(parser, E_INVALID_VALUE));
	return (parser);
}

t_range	set_range(double r1, double r2)
{
	t_range	range;
	range.r1 = r1;
	range.r2 = r2;
	return (range);
}

void	*check_double_value(t_parser *parser, char *str, double *value,
		t_range range)
{
	bool	is_a_dot;
	size_t	i;

	is_a_dot = false;
	i = 0;
	if (str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (set_parser_errcode(parser, E_INVALID_VALUE));
	while (str[i] != '\0')
	{
		if (str[i] == '.')
		{
			if (is_a_dot == false)
				is_a_dot = true;
			else
				return (set_parser_errcode(parser, E_INVALID_VALUE));
		}
		else if (!(str[i] >= '0' && str[i] <= '9'))
			return (set_parser_errcode(parser, E_INVALID_VALUE));
		str++;
	}
	*value = strtod(str, NULL);
	if ((range.r1 != range.r2) && !(*value >= range.r1 && *value <= range.r2))
		return (set_parser_errcode(parser, E_INVALID_RANGE));
	return (parser);
}

void	*check_type(t_parser *parser, t_token_type type, char **tkn_value,
		bool do_consume)
{
	if (parser->curr_tkn->type == type)
	{
		if (do_consume)
		{
			if (tkn_value)
				*tkn_value = parser->curr_tkn->value;
			consume(parser, 1);
		}
		return (parser);
	}
	if (type == T_VALUE)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (type == T_COMMA)
		return (set_parser_errcode(parser, E_EXPECTED_COMMA));
	if (type == T_NEWLINE)
		return (set_parser_errcode(parser, E_EXPECTED_NEWLINE));
	return (NULL);
}

void	*parse_rgb(t_parser *parser, uint32_t *rgb)
{
	char	*tkn_value;
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_rgb_component(parser, 'R', tkn_value, rgb) == NULL)
		return (NULL);
	if (check_type(parser, T_COMMA, &tkn_value, true) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_rgb_component(parser, 'G', tkn_value, rgb) == NULL)
		return (NULL);
	if (check_type(parser, T_COMMA, &tkn_value, true) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_rgb_component(parser, 'B', tkn_value, rgb) == NULL)
		return (NULL);
	return (parser);
}

void	*parse_coords(t_parser *parser, t_coord3d *coord, t_range range)
{
	char	*tkn_value;

	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_double_value(parser, tkn_value, &coord->x, range) == NULL)
		return (NULL);
	if (check_type(parser, T_COMMA, &tkn_value, true) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_double_value(parser, tkn_value, &coord->y, range) == NULL)
		return (NULL);
	if (check_type(parser, T_COMMA, &tkn_value, true) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL
			|| check_double_value(parser, tkn_value, &coord->y, range) == NULL)
		return (NULL);
	return (parser);
}

bool	is_an_identifier(t_parser *parser)
{
	t_token_type	type;

	type = parser->curr_tkn->type;
	if (type == T_AMBIANT_LIGHT || type == T_CAMERA
		|| type == T_LIGHT || type == T_SPHERE
		|| type == T_PLAN || type == T_CYLINDER)
	{
		consume(parser, 1);
		return (true);
	}
	else
		return (false);
}

t_ambiant_light	*parse_ambiant_light(t_parser *parser)
{
	t_ambiant_light	*ambiant_light;
	t_ambiant_light	obj;
	char			*tkn_value;

	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (check_double_value(parser, tkn_value, &obj.ratio, set_range(0, 1))
			== NULL)
		return (NULL);
	if (parse_rgb(parser, &obj.rgb) == NULL)
		return (NULL);
	ambiant_light = malloc (sizeof(t_ambiant_light));
	if (!ambiant_light)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*ambiant_light = obj;
	return (ambiant_light);
}

t_camera	*parse_camera(t_parser *parser)
{
	t_camera	*camera;
	t_camera	obj;
	char		*tkn_value;

	if (parse_coords(parser, &obj.coord, set_range(0.0, 0.0)) == NULL)
		return (NULL);
	if (parse_coords(parser, &obj.vec, set_range(-1.0, 1.0)) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (NULL);
	if (ft_stronly_digits(tkn_value))
	{
		obj.fov = ft_atoi(tkn_value);
		if (!(obj.fov >= FOV_MIN && obj.fov <= FOV_MAX))
			return (set_parser_errcode(parser, E_INVALID_RANGE));
	}
	else
		return (set_parser_errcode(parser, E_INVALID_VALUE));
	camera = malloc(sizeof(t_camera));
	if (!camera)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*camera = obj;
	return (camera);
}

t_light	*parse_light(t_parser *parser)
{
	t_light	*light;
	t_light	obj;
	char	*tkn_value;

	if (parse_coords(parser, &obj.coord, set_range(0.0, 0.0)) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (check_double_value(parser, tkn_value, &obj.ratio, set_range(0, 1))
			== NULL)
		return (NULL);
	light = malloc(sizeof(t_light));
	if (!light)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*light = obj;
	return (light);
}

t_sphere	*parse_sphere(t_parser *parser)
{
	t_sphere	*sphere;
	t_sphere	obj;
	char		*tkn_value;

	if (parse_coords(parser, &obj.coord, set_range(0.0, 0.0)) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (check_double_value(parser, tkn_value, &obj.diameter, set_range(0, 0))
			== NULL)
		return (NULL);
	if (parse_rgb(parser, &obj.rgb) == NULL)
		return (NULL);
	if (obj.diameter < 0)
		return (set_parser_errcode(parser, E_INVALID_RANGE));
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*sphere = obj;
	return (sphere);
}

t_plan	*parse_plan(t_parser *parser)
{
	t_plan	*plan;
	t_plan	obj;

	if (parse_coords(parser, &obj.coord, set_range(0.0, 0.0)) == NULL)
		return (NULL);
	if (parse_coords(parser, &obj.vec, set_range(-1.0, 1.0)) == NULL)
		return (NULL);
	if (parse_rgb(parser, &obj.rgb) == NULL)
		return (NULL);
	plan = malloc(sizeof(t_plan));
	if (!plan)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*plan = obj;
	return (plan);
}

t_cylinder	*parse_cylinder(t_parser *parser)
{
	t_cylinder	*cylinder;
	t_cylinder	obj;
	char		*tkn_value;

	if (parse_coords(parser, &obj.coord, set_range(0.0, 0.0)) == NULL)
		return (NULL);
	if (parse_coords(parser, &obj.vec, set_range(-1.0, 1.0)) == NULL)
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (!check_double_value(parser, tkn_value, &obj.diameter, set_range(0, 0)))
		return (NULL);
	if (check_type(parser, T_VALUE, &tkn_value, true) == NULL)
		return (set_parser_errcode(parser, E_EXPECTED_VALUE));
	if (check_double_value(parser, tkn_value, &obj.height, set_range(0, 0))
			== NULL)
		return (NULL);
	if (parse_rgb(parser, &obj.rgb) == NULL)
		return (NULL);
	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		return (set_parser_errcode(parser, E_MALLOC)); 
	*cylinder = obj;
	return (cylinder);
}
