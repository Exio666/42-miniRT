/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:47:41 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/28 11:16:47 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

#include "minirt_struct.h"

/* object.c */

t_object	*new_cone(t_point3 top, t_vec3 direction, double angle, double hauteur, uint32_t color);
t_object	*new_sphere(t_point3 center, double radius, uint32_t color);
t_object	*new_plan(t_point3 point, t_vec3 normal, uint32_t color);
t_object	*new_cylindre(t_point3 center, t_vec3 orientation, double rayon, double hauteur, uint32_t color);
t_object	*ray_intersect_scene_objs(t_scene *scene, t_ray *ray,
		t_rayhit *rayhit);

/* scene.c */

t_list		*add_obj_to_scene(t_scene *scene, t_object *obj);
t_list		*add_light_to_scene(t_scene *scene, t_point3 pos, uint32_t color,
		double intensity);
void		set_ambiant_light(t_scene *scene, uint32_t color, double intensity);

/* shading.c */

t_color	get_shade(t_scene *scene, t_object *obj, t_rayhit *rayhit);

#endif
