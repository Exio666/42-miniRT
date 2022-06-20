/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:43:00 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 20:22:18 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include "mlx.h"
#include "mlx_utils.h"
#include <stdlib.h>
#include <stdio.h>

t_object	*new_sphere(t_3dpoint center, double radius, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.sphere.center = center;
	obj->p.sphere.radius = radius;
	obj->fnct = &intersect_sphere;
	obj->type = T_SPHERE;
	obj->albedo = get_normalized_color(color);
	return (obj);
}

t_object	*new_plan(t_3dpoint point, t_vec3d normal, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.plan.point = point;
	obj->p.plan.normal = normal;
	obj->fnct = &plane_intersection;
	obj->type = T_PLAN;
	obj->albedo = get_normalized_color(color);
	return (obj);
}
