/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 15:10:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 20:17:12 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "define.h"
#include "vector.h"

unsigned int	set_color(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 16 | g << 8 | b);
}

t_color	get_normalized_color(uint32_t color)
{
	t_color	normalized_color;

	normalized_color = vec(color >> 16,
			0xFF & color >> 8,
			0xFF & color);
	vec_mul_scalar_p(&normalized_color, 1 / 255.);
	return (normalized_color);
}

void	mlx_pixel_img_put(t_minirt *minirt, int x, int y, int color)
{
	char	*dst;

	dst = minirt->mlx.img.addr + ((HEIGHT - y - 1) * minirt->mlx.img.line_length
			+ x * (minirt->mlx.img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
