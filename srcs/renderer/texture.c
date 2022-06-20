/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 19:43:03 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 21:28:18 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "vector.h"
#include "mlx_utils.h"
#include <stdio.h>
#include <mlx.h>
#include <unistd.h>

/* Apply a texture on a given object. */

void	*apply_texture_on_obj(void *mlx_ptr, t_object *obj, char *path)
{
	obj->textured = false;
	printf("Loading texture '%s'...", path);
	obj->texture.img.img = mlx_xpm_file_to_image(mlx_ptr,
			path,
			&obj->texture.width,
			&obj->texture.height);
	if (obj->texture.img.img)
	{
		obj->textured = true;
		printf(" W:%d H:%d ok.\n", obj->texture.width, obj->texture.height);
		obj->texture.img.addr = mlx_get_data_addr(obj->texture.img.img,
				&obj->texture.img.bits_per_pixel,
				&obj->texture.img.line_length,
				&obj->texture.img.endian);
	}
	else
		ft_putstr_fd(" failed, using default color.\n", STDERR_FILENO);
	return (obj->texture.img.img);
}

/* Output a normalized color for a given UV coordinate. */

t_color	get_color_from_texture(t_2dpoint uv, t_texture *texture)
{
	void		*dest;
	t_2dpoint	xy;

	xy.x = uv.x * (texture->width - 1);
	xy.y = uv.y * (texture->height - 1);
	xy.z = 0;
	dest = &texture->img.addr[(int) xy.x * (texture->img.bits_per_pixel / 8)
		+ (int) (texture->height - xy.y - 1) * texture->img.line_length];
	return (get_normalized_color(*(uint32_t *) dest));
}
