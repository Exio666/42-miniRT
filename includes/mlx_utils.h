/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 13:11:59 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/20 20:19:23 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTILS_H
# define MLX_UTILS_H

# include "minirt_struct.h"

/*
 *	mlx_struct_manage.c
 */

int				init_mlx_struct(t_mlx *mlx);
int				destruct_mlx(t_mlx *mlx);

/*
 * launch_loop.c
 */

void			launch_loop(t_minirt *backpack);

/*
 *	mlx_utils.c
 */

unsigned int	set_color(unsigned char r, unsigned char g, unsigned char b);
t_color			get_normalized_color(uint32_t color);
void			mlx_pixel_img_put(t_minirt *minirt, int x, int y, int color);

#endif
