/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 22:11:16 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/25 23:38:00 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"

/* uv_mapping.c */

t_uv	get_planar_map(t_point3 intersect_p);

/* texture.c */

t_texture	create_checkered_texture(int width, int height, uint32_t a,
		uint32_t b);
t_color	get_checker_color(t_texture texture, t_uv uv);
void	apply_obj_texture(t_object *obj, t_texture texture);