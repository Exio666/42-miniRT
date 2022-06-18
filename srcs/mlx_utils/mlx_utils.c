/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 15:10:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/18 13:02:02 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "define.h"
#include "math_utils.h"

unsigned int	set_color(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 16 | g << 8 | b);
}

void	mlx_pixel_img_put(t_minirt *minirt, int x, int y, int color)
{
	char	*dst;

	dst = minirt->mlx.img.addr + ((HEIGHT - y - 1) * minirt->mlx.img.line_length
			+ x * (minirt->mlx.img.bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	write_pixel(t_minirt *minirt, size_t j, size_t i, t_color const *color)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	r = min(255., min(1, color->x) * 255.99);
	g = min(255., min(1, color->y) * 255.99);
	b = min(255., min(1, color->z) * 255.99);
	mlx_pixel_img_put(minirt, j, i, r << 16 | g << 8 | b);
}
