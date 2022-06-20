/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:52:40 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/20 18:53:32 by plouvel          ###   ########.fr       */
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

#define SAMPLE_PPX 3 

static void	generate_ray(t_ray *ray, t_3dpoint viewport_point)
{
	ray->org = vec(0.0, 10.0, 0.0);
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

t_vec	vec_add_scalar(t_vec a, double d)
{
	return (vec(a.x + d, a.y + d, a.z + d));
}

# define L 5e3

t_vec3d	get_reflect_vec(t_vec3d incident, t_vec3d normal)
{
	t_vec3d	reflect_vec;
	double	dot_incident_normal;
	
	incident = vec_normalize(vec_mul_scalar(incident, 1));
	dot_incident_normal = vec_dot(incident, normal);
	reflect_vec = vec_sub(vec_mul_scalar(vec_mul_scalar(normal, 2.), dot_incident_normal), incident);
	
	return (reflect_vec);
}

t_color	get_color_from_texture(t_2dpoint uv, t_texture *texture)
{
	void		*dest;
	uint32_t	color;
	t_2dpoint	xy;
	t_color		dest_color;

	xy.x = uv.x * (texture->width - 1);
	xy.y = uv.y * (texture->height - 1);
	xy.z = 0;
	dest = &texture->img.addr[(int) xy.x * (texture->img.bits_per_pixel / 8)
		+ (int) (texture->height - xy.y - 1) * texture->img.line_length];
	color = *(uint32_t *) dest;
	dest_color.x = color >> 16;
	dest_color.y = 0xFF & color >> 8;
	dest_color.z = 0xFF & color;
	dest_color = vec_mul_scalar(dest_color, 1. / 255);
	return (dest_color);
}

t_2dpoint	get_uv_sphere(t_vec3d p)
{
	return (vec(asin(p.x) / M_PI + 0.5, asin(p.y) / M_PI + 0.5, 0.));
}


t_texture	texture;

t_color	compute_light(t_scene *scene, t_ray *ray, t_rayhit const *rayhit, t_object *obj)
{

	t_color	color = {0};
	double	intensity;
	t_vec3d	intersect_to_light;
	t_vec3d	reflect_vec;
	t_vec	eye_vec;
	double	factor;
	double	dot_reflect_eye;
	double	dot_normal_inter_light;

	color = vec_mul_scalar(scene->ambiant.albedo, scene->ambiant.ratio);
	for (t_list *elem = scene->light; elem; elem = elem->next)
	{
		t_light *light = elem->content;
		if (!is_a_shadow(scene, light, rayhit))
		{
			intersect_to_light = vec_sub(light->pos, rayhit->intersect_p);
			dot_normal_inter_light = max(0, vec_dot(vec_normalize(intersect_to_light), rayhit->normal)); 
			intensity = 6e2 * dot_normal_inter_light / vec_lenght_p(intersect_to_light) * light->ratio;
			color = vec_add(color, vec_mul_scalar(light->albedo, intensity));

			eye_vec = vec_normalize(vec_sub(ray->org, rayhit->intersect_p));
			reflect_vec = get_reflect_vec(intersect_to_light, rayhit->normal);
			dot_reflect_eye = max(0, vec_dot(reflect_vec, eye_vec));
			factor = pow(dot_reflect_eye, 30);
			color = vec_add(color, vec_mul_scalar(light->albedo, factor));
		}
	}
	t_color obj_color;

	if (obj->type == T_SPHERE)
		obj_color = get_color_from_texture(get_uv_sphere(rayhit->normal), &texture);
	else
		obj_color = obj->albedo;

	color = vec_mul(color, obj_color);


	//diffuse = vec_add(diffuse, vec_mul_scalar(light->albedo, light->ratio));

	return (color);
	
	/*n_to_eye = vec_normalize(vec_sub(ray->org, rayhit->intersect_p));
	for (t_list *elem = scene->light; elem; elem = elem->next)
	{
		t_light	*light = elem->content;

		if (is_a_shadow(scene, light, rayhit))
			continue ;
		n_to_l = vec_normalize(vec_sub(light->pos, rayhit->intersect_p));
		n_dot_l = max(0, vec_dot(n_to_l, rayhit->normal));
		light_intensity = vec_add_scalar(light_intensity, light->ratio * n_dot_l);

		l_reflected = vec_sub(vec_mul_scalar(vec_mul_scalar(rayhit->normal, 2.), vec_dot(rayhit->normal, n_to_l)), n_to_l);
		r_dot_l = vec_dot(l_reflected, n_to_eye);
		if (r_dot_l > 0)
			light_intensity = vec_add_scalar(light_intensity, light->ratio * pow(r_dot_l / vec_length(l_reflected) *vec_length(n_to_eye), 100));	}*/
}

uint32_t	get_color(t_color color)
{
	unsigned char r, g, b;

	double	inv = 1. / SAMPLE_PPX;

	color = vec_mul_scalar(color, inv);
	r = min(255., min(1, color.x) * 255.99);
	g = min(255., min(1, color.y) * 255.99);
	b = min(255., min(1, color.z) * 255.99);
	return (r << 16 | g << 8 | b);
}

double	rand_double(void)
{
	return (rand() / (RAND_MAX + 1.));
}



void	render_img(t_minirt *minirt)
{
	t_3dpoint	viewport_point;
	t_ray		ray;
	t_rayhit	rayhit;
	t_object	*obj;
	size_t		i;
	size_t		j;


	minirt->scene.ambiant.albedo = vec(1,1, 1);
	minirt->scene.ambiant.ratio = 0.4;
	/* Ici on definie tout nos objects pour debug. */


	texture.img.img = mlx_xpm_file_to_image(minirt->mlx.ptr, "earth.xpm", &texture.width, &texture.height);

	if (texture.img.img)
	{
		printf("Loaded texture: %d x %d\n", texture.width, texture.height);
		texture.img.addr = mlx_get_data_addr(texture.img.img, &texture.img.bits_per_pixel, &texture.img.line_length, &texture.img.endian);
	}

	add_obj_to_scene(&minirt->scene, new_sphere(
				vec(0, 14, -60),
				15.0,
				0xffffff));
	add_obj_to_scene(&minirt->scene, new_sphere(
				vec(10, 6, -42),
				7.0,
				0xffffff));
	/*add_obj_to_scene(&minirt->scene, new_sphere(
				vec(-10, 4, -30),
				5.0,
				0xffffff));
	add_obj_to_scene(&minirt->scene, new_sphere(
				vec(10, 5, -20),
				6.0,
				0xffffff));*/

	// le sol

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, -1, 0),
				vec(0, 1, 0),
				0xff0000));
/*
	add_obj_to_scene(&minirt->scene, new_plan(
				vec(50, 0, 0),
				vec(-1, 0, -0.2),
				0x00ff00));
	add_obj_to_scene(&minirt->scene, new_plan(
				vec(-50, 0, 0),
				vec(1, 0, -0.2),
				0x0000ff));

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, 0, -300),
				vec(0, 0, 1),
				0xffffff));

	add_obj_to_scene(&minirt->scene, new_plan(
				vec(0, 110, 0),
				vec(0, -1, 0),
				0xff00ff));
*/
	/* Pour l'instant, l'unique lumiere */

	add_light_to_scene(&minirt->scene,
			vec(0, 30, -40),
			0xffffff,
			0.2);


	

	
	viewport_point.z = - (WIDTH / (2 * tan(FOV / 2)));
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{

			t_vec3d color = vec(0, 0, 0);
			for (int s = 0; s < SAMPLE_PPX; s++)
			{
				viewport_point.x = (j + rand_double()) - (WIDTH / 2.0);
				viewport_point.y = (i + rand_double()) - (HEIGHT / 2.0);
				generate_ray(&ray, viewport_point);
				obj = ray_intersect_scene_objs(&minirt->scene, &ray, &rayhit);
				if (obj)
				{

					color = vec_add(color, compute_light(&minirt->scene, &ray, &rayhit, obj));
					//if (color.x != 0 || color.y != 0 || color.z != 0)
						//vec_print(color);
				}
				else
				{
					unsigned char r, g, b;

					r = 16 + (double) (255 - 16) / HEIGHT * i;
					g = 112 + (double) (255 - 112) / HEIGHT * i;
					b = 222 + (double) (255 - 222) / HEIGHT * i;
					mlx_pixel_img_put(minirt, j, i, 0xFF << 24| r << 16 | g << 8 | b);
				}
			}

			mlx_pixel_img_put(minirt, j, i, get_color(color));
			j++;
		}
		i++;
	}
	
	mlx_put_image_to_window(minirt->mlx.ptr, minirt->mlx.win, minirt->mlx.img.img, 0, 0);
	puts("Rendering done");
}
