/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:47:41 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 21:28:19 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

#include "minirt_struct.h"

/* object.c */

t_object	*new_sphere(t_3dpoint center, double radius, uint32_t color);
t_object	*new_plan(t_3dpoint point, t_vec3d normal, uint32_t color);

/* scene.c */

t_object	*add_obj_to_scene(t_scene *scene, t_object *obj);
t_list		*add_light_to_scene(t_scene *scene, t_3dpoint point, uint32_t color,
		double intensity);
void	set_scene_ambiant_light(t_scene *scene, uint32_t color,
		double intensity);
t_object	*ray_intersect_scene_objs(t_scene *scene, t_ray *ray,
		t_rayhit *rayhit);

#endif
