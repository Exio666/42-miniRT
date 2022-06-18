/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:52:40 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/18 07:27:21 by plouvel          ###   ########.fr       */
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
	ray->org = vec(0.0, 15.0, 0.0);
	ray->dir = vec_normalize(viewport_point);
}

/* If the spectified ray hit an object in the specified scene, return the first
 * object hitted and fill the rayhit structure. */

t_object	*ray_intersect_scene_objs(t_scene *scene, t_ray *ray,
		t_rayhit *rayhit)
{
	t_list		*elem;
	t_object	*obj;
	t_object	*closest_obj;
	t_rayhit	closest_rayhit;

	elem = scene->objs;
	closest_obj = NULL;
	while (elem)
	{
		obj = elem->content;
		if (obj->fnct(obj, ray, rayhit))
		{
			if (!closest_obj || rayhit->t < closest_rayhit.t)
			{
				closest_rayhit = *rayhit;
				closest_obj = obj;
			}
		}
		elem = elem->next;
	}
	*rayhit = closest_rayhit;
	return (closest_obj);
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

double	compute_light(t_scene *scene, t_ray *ray, t_rayhit const *rayhit)
{
	double	light_intensity;
	t_vec3d	n_to_l;
	t_vec3d	l_reflected;
	t_vec3d	n_to_eye;
	double	n_dot_l;
	double	r_dot_l;

	light_intensity = 0.2;
	n_to_eye = vec_normalize(vec_sub(ray->org, rayhit->intersect_p));
	for (t_list *elem = scene->light; elem; elem = elem->next)
	{
		t_light	*light = elem->content;

		if (is_a_shadow(scene, light, rayhit))
			continue ;
		n_to_l = vec_normalize(vec_sub(light->pos, rayhit->intersect_p));
		n_dot_l = vec_dot(n_to_l, rayhit->normal);
		if (n_dot_l > 0)
			light_intensity += 1.5e3 * light->ratio * n_dot_l / vec_lenght_p(vec_sub(light->pos, rayhit->intersect_p));

		l_reflected = vec_sub(vec_mul_scalar(vec_mul_scalar(rayhit->normal, 2.), vec_dot(rayhit->normal, n_to_l)), n_to_l);
		r_dot_l = vec_dot(l_reflected, n_to_eye);
		if (r_dot_l > 0)
			light_intensity += light->ratio * pow(r_dot_l / vec_length(l_reflected) *vec_length(n_to_eye), 500);
	}
	return (light_intensity);
}

uint32_t	get_color(t_color color)
{
	unsigned char r, g, b;

	r = min(255., min(1, color.x) * 255.99);
	g = min(255., min(1, color.y) * 255.99);
	b = min(255., min(1, color.z) * 255.99);
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
				vec(0, 20, -75),
				14.0,
				0x00ffFF));

	// le sol

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, -1, 0),
				vec(0, 1, 0),
				0xff00ff));

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(50, 0, 0),
				vec(-1, 0, -0.2),
				0x00ff00));
	add_obj_to_scene(&minirt->scene, new_plan(
				vec(-50, 0, 0),
				vec(1, 0, -0.2),
				0xff0000));

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, 0, -300),
				vec(0, 0, 1),
				0xffffff));

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, 110, 0),
				vec(0, -1, 0),
				0xffff00));

	/* Pour l'instant, l'unique lumiere */

	add_light_to_scene(&minirt->scene,
			vec(0, 60, -40),
			0xffffff,
			0.9);
	add_light_to_scene(&minirt->scene,
			vec(0, 60, -140),
			0xffffff,
			0.9);


	viewport_point.z = - (WIDTH / (2 * tan(FOV / 2)));
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			viewport_point.x = j - (WIDTH / 2.0);
			viewport_point.y = i - (HEIGHT / 2.0);
			generate_ray(&ray, viewport_point);
			obj = ray_intersect_scene_objs(&minirt->scene, &ray, &rayhit);
			if (obj)
			{
				t_color	color;

				color = vec_mul_scalar(obj->albedo, compute_light(&minirt->scene, &ray, &rayhit));
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
		i++;
	}
	mlx_put_image_to_window(minirt->mlx.ptr, minirt->mlx.win, minirt->mlx.img.img, 0, 0);
	puts("Rendering done");
}
