/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:43:00 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/27 23:10:29 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "mlx_utils.h"
#include "texture.h"
#include "matrix.h"
#include "tuple.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

t_object	*new_sphere(t_point3 pos, double radius, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.sphere.pos = pos;
	obj->p.sphere.radius = radius;
	obj->fnct = &intersect_sphere;
	obj->uvmap_fnct = &get_spherical_map;
	obj->type = T_SPHERE;
	obj->albedo = get_norm_color(color);
	obj->M = matrix4_translate(pos.x, pos.y, pos.z);
	obj->M = matrix4_mul(obj->M, matrix4_scale(radius, radius, radius));
	obj->M = matrix4_mul(obj->M, matrix4_rotate_z(M_PI/6));
	obj->M = matrix4_mul(obj->M, matrix4_rotate_y(0));
	obj->M = matrix4_mul(obj->M, matrix4_rotate_x(-M_PI/4));
	obj->M_inv = matrix4_inv(obj->M);
	obj->M_inv_trans = matrix4_trans(obj->M_inv);
	return (obj);
}

t_object	*new_plan(t_point3 pos, t_vec3 normal, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.plan.pos = pos;
	obj->p.plan.normal = normal;
	obj->fnct = &plane_intersection;
	obj->uvmap_fnct = &get_planar_map;
	obj->type = T_PLAN;
	obj->albedo = get_norm_color(color);
	return (obj);
}

void	apply_obj_transform(t_object *obj, t_matrix4 const M)
{
	obj->M = M;
	obj->M_inv = matrix4_inv(M);
}
