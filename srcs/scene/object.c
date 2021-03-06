/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:43:00 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/28 11:10:17 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "mlx_utils.h"
#include "matrix.h"
#include "tuple.h"
#include "define.h"
#include "math.h"
#include <stdlib.h>
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
	obj->type = T_SPHERE;
	obj->albedo = get_norm_color(color);
	obj->M = matrix4_mul(matrix4_translate(pos.x, pos.y, pos.z),
			matrix4_scale(radius, radius, radius));
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
	obj->type = T_PLAN;
	obj->albedo = get_norm_color(color);
	return (obj);
}

t_object	*new_cylindre(t_point3 center, t_vec3 orientation, double rayon, double hauteur, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.cylindre.orientation = vec_norm(orientation);
	obj->p.cylindre.center = center;
	obj->p.cylindre.hauteur = hauteur;
	obj->p.cylindre.rayon = rayon;
	obj->fnct = &intersect_cylindre;
	obj->type = T_CYLINDRE;
	obj->M = matrix4_translate(center.x, center.y, center.z);
	obj->M_inv = matrix4_inv(obj->M);
	obj->albedo = get_norm_color(color);
	return (obj);
}

void	len_and_rayon_cone(t_object *obj)
{
	double	radian_angle;

	radian_angle = (obj->p.cone.angle * M_PI) / 180;
	obj->p.cone.len_pente = obj->p.cone.height / cos(radian_angle);
	obj->p.cone.rayon_base = obj->p.cone.height * tan(radian_angle);
}

t_object	*new_cone(t_point3 top, t_vec3 dir, double angle, double height, uint32_t color)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.cone.top = top;
	obj->p.cone.dir = vec_norm(dir);
	obj->p.cone.angle = angle;
	obj->p.cone.height = height;
	obj->fnct = &intersect_cone;
	obj->type = T_CYLINDRE;
	obj->albedo = get_norm_color(color);
	len_and_rayon_cone(obj);
	return (obj);
}

void	apply_obj_transform(t_object *obj, t_matrix4 const M)
{
	obj->M = M;
	obj->M_inv = matrix4_inv(M);
}
