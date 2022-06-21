/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:43:00 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/21 17:23:03 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "ray_intersection.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

t_object	*new_sphere(t_3dpoint center, double radius, uint32_t color)
{
	t_object	*obj;
	uint8_t	r, g, b;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.sphere.center = center;
	obj->p.sphere.radius = radius;
	obj->fnct = &intersect_sphere;
	obj->type = T_SPHERE;
	r = color >> 16;
	g = color >> 8;
	b = 0xFF & color;
	obj->albedo = vec(r / 255.,
					g / 255.,
					b / 255.);
	return (obj);
}

t_object	*new_plan(t_3dpoint point, t_vec3d normal, uint32_t color)
{
	t_object	*obj;
	uint8_t	r, g, b;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.plan.point = point;
	obj->p.plan.normal = normal;
	obj->fnct = &plane_intersection;
	obj->type = T_PLAN;
	r = color >> 16;
	g = color >> 8;
	b = 0xFF & color;
	obj->albedo = vec(r / 255.,
					g / 255.,
					b / 255.);
	return (obj);
}

t_object	*new_cylindre(t_3dpoint center, t_vec3d orientation, double rayon, double hauteur, uint32_t color)
{
	t_object	*obj;
	uint8_t	r, g, b;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->p.cylindre.orientation = orientation;
	obj->p.cylindre.center = center;
	obj->p.cylindre.hauteur = hauteur;
	obj->p.cylindre.rayon = rayon;
	obj->fnct = &intersect_cylindre;
	obj->type = T_CYLINDRE;
	r = color >> 16;
	g = color >> 8;
	b = 0xFF & color;
	obj->albedo = vec(r / 255.,
					g / 255.,
					b / 255.);
	return (obj);
}