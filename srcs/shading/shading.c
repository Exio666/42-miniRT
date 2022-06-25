/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 20:16:01 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/25 20:39:12 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "tuple.h"
#include "math_utils.h"

# define L_POWER 1e3

static inline void apply_diffuse_coeff(t_light *light, t_vec3 lightv,
		t_vec3 normal, t_color *color)
{
	double	coeff;

	coeff = max(0, L_POWER * vec_dot(vec_norm(lightv), normal)
			/ vec_norm_sqr(lightv) * light->intensity);
	*color = tadd(*color, tmul_scalar(light->color, coeff));
}

t_color	get_shade(t_scene *scene, t_object *obj, t_rayhit *rayhit)
{
	t_list	*elem;
	t_light	*light;
	t_color	pix_color;
	t_vec3	lightv;

	pix_color = tmul_scalar(scene->amb_light.color, scene->amb_light.intensity);
	elem = scene->light;
	while (elem)
	{
		light = elem->content;
		lightv = tsub(light->pos, rayhit->intersect_p);
		apply_diffuse_coeff(light, lightv, rayhit->normal, &pix_color);
		elem = elem->next;
	}
	pix_color = tmul(pix_color, obj->albedo);
	return (pix_color);
}
