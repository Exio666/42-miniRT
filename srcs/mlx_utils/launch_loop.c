/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:16:24 by bsavinel          #+#    #+#             */
/*   Updated: 2022/06/14 14:34:46 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_utils.h"
#include "minirt_struct.h"
#include "end.h"
#include "define.h"
#include "mlx.h"

int	select_hook(int hook, t_minirt *backpack)
{
	if (hook == D_ESCAPE)
		good_exit(backpack);
	return (1);
}

void	launch_loop(t_minirt *backpack)
{
	mlx_hook(backpack->mlx.win, 17, 0, good_exit, backpack);
	mlx_key_hook(backpack->mlx.win, select_hook, backpack);
	mlx_loop(backpack->mlx.ptr);
}
