/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:52:40 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/18 13:14:58 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_struct.h"
#include "define.h"
#include "mlx.h"
#include "mlx_utils.h"
#include "vector.h"
#include "scene.h"
#include <math.h>
#include "math_utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_PPX 12

static inline void	launch_ray(t_3dpoint viewport_point)
{
	t_ray		ray;
	t_rayhit	rayhit;

	ray.org = vec(0.0, 15.0, 0.0);
	ray->dir = vec_normalize(viewport_point);
}

bool	is_a_shadow(t_scene *scene, t_light *light, t_rayhit const *primary_rayhit)
{
	t_ray		ray;
	t_rayhit	rayhit;
	t_object	*obj;
	double		distance_to_light;

	ray.org = vec_add(primary_rayhit->intersect_p, vec_mul_scalar(primary_rayhit->normal, 1e-7));
	ray.dir = vec_normalize(vec_sub(light->pos,
												primary_rayhit->intersect_p));
	obj = ray_intersect_scene_objs(scene, &ray, &rayhit);
	if (obj)
	{
		distance_to_light = vec_length(
				vec_sub(light->pos, primary_rayhit->intersect_p));
		if (rayhit.t < distance_to_light)
			return (true);
	}
	return (false);
}

t_vec	vec_mul(t_vec a, t_vec b)
{
	return (vec(a.x * b.x, a.y * b.y, a.z * b.z));
}

# define L 5e3

t_vec	vec_add_scalar(t_vec a, double scalar)
{
	return (vec(a.x + scalar, a.y + scalar, a.z + scalar));
}

t_color	compute_light(t_scene *scene, t_ray *ray, t_rayhit const *rayhit)
{
	t_vec3d	n_to_l;
	double	n_dot_l;
	double	r_dot_l;
	t_vec3d	l_reflected;
	t_vec3d	n_to_eye;
	t_color	intensity;

	(void) ray;
	intensity = vec_mul_scalar(vec(1, 1, 1.), 0.4);
	n_to_eye = vec_normalize(vec_sub(ray->org, rayhit->intersect_p));
	for (t_list *elem = scene->light; elem; elem = elem->next)
	{
		t_light	*light = elem->content;

		if (is_a_shadow(scene, light, rayhit))
			continue ;
		n_to_l = vec_normalize(vec_sub(light->pos, rayhit->intersect_p));
		n_dot_l = max(0, vec_dot(n_to_l, rayhit->normal));
		intensity = vec_add_scalar(intensity, 1e3 * light->ratio * n_dot_l / vec_lenght_p(vec_sub(light->pos, rayhit->intersect_p)));

		l_reflected = vec_sub(vec_mul_scalar(vec_mul_scalar(rayhit->normal, 2.), vec_dot(rayhit->normal, n_to_l)), n_to_l);
		r_dot_l = vec_dot(l_reflected, n_to_eye);
		if (r_dot_l > 0)
			intensity = vec_add_scalar(intensity, light->ratio * pow(r_dot_l, 170));
	}
	return (intensity);
}

double	rand_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

uint32_t	get_color(t_color color)
{
	unsigned char r, g, b;
	double scale = 1. / SAMPLE_PPX;
	color = vec_mul_scalar(color, scale);

	r = min(255., min(1, color.x) * 255.99);
	g = min(255., min(1, color.y) * 255.99);
	b = min(255., min(1, color.z) * 255.99);
	return (r << 16 | g << 8 | b);
}

static inline	t_vec3d	get_canvas_coord(size_t j, size_t i)
{
	return (vec(j - (WIDTH / 2.),
				i - (HEIGHT / 2.),
}

void	render_img(t_minirt *minirt)
{
	t_color		intensity;
	t_3dpoint	viewport_p;
	size_t		i;
	size_t		j;

	viewport_point.z = - (WIDTH / (2 * tan(FOV / 2)));
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			viewport_p.x = j - (WIDTH / 2.0);
			viewport_p.y = i - (HEIGHT / 2.0);
			launch_ray(&ray, viewport_p);
			write_pixel(minirt, j, i, intensity);
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(minirt->mlx.ptr, minirt->mlx.win,
			minirt->mlx.img.img, 0, 0);
}
