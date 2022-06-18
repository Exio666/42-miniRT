/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:32:57 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/18 05:21:28 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "vector.h"
#include <stdlib.h>

t_list	*add_obj_to_scene(t_scene *scene, t_object *obj)
{
	t_list	*elem;

	elem = ft_lstnew(obj);
	if (!elem)
		return (NULL);
	ft_lstadd_back(&scene->objs, elem);
	return (elem);
}

t_list	*add_light_to_scene(t_scene *scene, t_3dpoint point, uint32_t color,
		double ratio)
{
	t_list	*elem;
	t_light	*light;
	unsigned char r, g, b;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->pos = point;
	r = color >> 16;
	g = color >> 8;
	b = color & 0xFF;
	light->albedo = vec(r / 255., g / 255., b / 255.);
	light->ratio = ratio;
	elem = ft_lstnew(light);
	if (!elem)
	{
		free(light);
		return (NULL);
	}
	ft_lstadd_back(&scene->light, elem);
	return (elem);
}
