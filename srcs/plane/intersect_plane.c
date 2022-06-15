/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 17:17:47 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/15 19:20:56 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "vector.h"

bool	plane_intersection(t_object *object, t_ray *ray, t_rayhit *rayhit)
{
	if (vec_dot(object->p.plan.normal, ray->dir) == 0)
		return (false);
	rayhit->normal = object->p.plan.normal;
	rayhit->t = vec_dot(object->p.plan.normal,
			vec_sub(object->p.plan.point, ray->org))
			/ vec_dot(object->p.plan.normal, ray->dir);
	rayhit->intersect_p = vec_add(ray->org, vec_mul_scalar(ray->dir, rayhit->t));
	return (true);
}
