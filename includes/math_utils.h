/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:44:52 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/24 18:00:56 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

#include "minirt_struct.h"

double		min(double a, double b);
double		max(double a, double b);

bool		solve_quadratic(double a, double b, double c, double t[2]);
t_ray		ray_transform(t_ray old_ray, t_matrix4 const M);
t_point3	get_ray_point(t_ray ray, double t);

#endif
