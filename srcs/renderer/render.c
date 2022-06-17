/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:52:40 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/17 18:42:30 by plouvel          ###   ########.fr       */
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

#define SAMPLE_PPX 100

static void	generate_ray(t_ray *ray, t_3dpoint viewport_point)
{
	ray->org = vec(0.0, 0.0, 0.0);
	ray->dir = vec_normalize(viewport_point);
}

/* If the spectified ray hit an object in the specified scene, return the first
 * object hitted and fill the rayhit structure. */

t_object	*ray_intersect_scene_objs(t_scene *scene, t_ray *ray,
		t_rayhit *rayhit)
{
	t_list		*elem;
	t_object	*obj;

	elem = scene->objs;
	while (elem)
	{
		obj = elem->content;
		if (obj->fnct(obj, ray, rayhit))
			return (obj);
		elem = elem->next;
	}
	return (NULL);
}

bool	is_a_shadow(t_scene *scene, t_object *primary_obj, t_rayhit const *primary_rayhit)
{
	t_ray		ray;
	t_rayhit	rayhit;
	t_object	*obj;
	double		distance_to_light;
	(void) primary_obj;

	ray.org = vec_add(primary_rayhit->intersect_p, vec_mul_scalar(primary_rayhit->normal, 0.01));
	ray.dir = vec_normalize(vec_sub(scene->light.point,
												primary_rayhit->intersect_p));
	obj = ray_intersect_scene_objs(scene, &ray, &rayhit);
	if (obj)
	{
		distance_to_light = vec_length(
				vec_sub(scene->light.point, primary_rayhit->intersect_p));
		if (rayhit.t < distance_to_light)
			return (true);
	}
	return (false);
}

t_color	compute_light(t_scene *scene, t_object *obj, t_rayhit const *rayhit)
{
	t_color	color = {0};
	double	dot_normal_light;
	t_vec3d	normal_to_light;

	if (is_a_shadow(scene, obj, rayhit) == true)
		return (vec(0., 0., 0.));
	normal_to_light = vec_sub(scene->light.point, rayhit->intersect_p);
	dot_normal_light = max(0, vec_dot(vec_normalize(normal_to_light),
			rayhit->normal));
	color = vec_mul_scalar(obj->albedo,
			1.4e8 * scene->light.ratio * dot_normal_light / vec_lenght_p(normal_to_light));
	return (color);
}

uint32_t	get_color(t_color color)
{
	unsigned char r, g, b;

	r = min(255., pow(color.x, 1/2.2));
	g = min(255., pow(color.y, 1 / 2.2));
	b = min(255., pow(color.z, 1/2.2));
	return (r << 16 | g << 8 | b);
}

void	render_img(t_minirt *minirt)
{
	t_3dpoint	viewport_point;
	t_ray		ray;
	t_rayhit	rayhit;
	t_object	*obj;
	size_t		i;
	size_t		j;


	/* Ici on definie tout nos objects pour debug. */

	add_obj_to_scene(&minirt->scene, new_sphere(
				vec(0, 0, -55),
				10.0,
				0xff0000));
	add_obj_to_scene(&minirt->scene, new_sphere(
				vec(20, 0, -80),
				8.0,
				0x00ff00));

	// le sol

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, -7, 0),
				vec(0, 1, -0.2),
				0xbfbfe0));

	/* Pour l'instant, l'unique lumiere */

	set_scene_light(&minirt->scene,
				vec(10, 47, -35),
				1);


	viewport_point.z = - (WIDTH / (2 * tan(FOV / 2)));
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			for (int s = 0; s < SAMPLE_PPX; s++)
			{
				viewport_point.x = j - (WIDTH / 2.0);
				viewport_point.y = i - (HEIGHT / 2.0);
				generate_ray(&ray, viewport_point);
				obj = ray_intersect_scene_objs(&minirt->scene, &ray, &rayhit);
				if (obj)
				{
					t_color color = compute_light(&minirt->scene, obj, &rayhit);
					//if (color.x != 0 || color.y != 0 || color.z != 0)
						//vec_print(color);
					mlx_pixel_img_put(minirt, j, i, get_color(color));
				}
				else
				{
					unsigned char r, g, b;

					r = 16 + (double) (255 - 16) / HEIGHT * i;
					g = 112 + (double) (255 - 112) / HEIGHT * i;
					b = 222 + (double) (255 - 222) / HEIGHT * i;
					mlx_pixel_img_put(minirt, j, i, 0xFF << 24| r << 16 | g << 8 | b);
				}
				j++;
			}
		}
		i++;
	}
	mlx_put_image_to_window(minirt->mlx.ptr, minirt->mlx.win, minirt->mlx.img.img, 0, 0);
	puts("Rendering done");
}
