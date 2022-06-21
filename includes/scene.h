/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:47:41 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/21 17:23:45 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

#include "minirt_struct.h"

/* object.c */

t_object	*new_sphere(t_3dpoint center, double radius, uint32_t color);
t_object	*new_plan(t_3dpoint point, t_vec3d normal, uint32_t color);
t_object	*new_cylindre(t_3dpoint center, t_vec3d orientation, double rayon, double hauteur, uint32_t color);

/* scene.c */

t_list		*add_obj_to_scene(t_scene *scene, t_object *obj);
void		set_scene_light(t_scene *scene, t_3dpoint point, double ratio);

#endif
