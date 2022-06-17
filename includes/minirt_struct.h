/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:46:37 by plouvel           #+#    #+#             */
/*   Updated: 2022/06/16 16:55:26 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_STRUCT_H
# define MINIRT_STRUCT_H

# include "libft.h"
# include <stdbool.h>

/* Enum */


/* Math related structure */

typedef struct e_vec
{
	double	x;
	double	y;
	double	z;
}				t_vec;

typedef t_vec	t_vec3d;
typedef t_vec	t_3dpoint;
typedef t_vec	t_2dpoint;

/* minilibx and program structure */

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image;

typedef struct s_mlx
{
	t_image		img;
	void		*ptr;
	void		*win;
}	t_mlx;

typedef struct s_minirt
{
	t_mlx	mlx;
}	t_minirt;

/* Related ray structure */

typedef struct e_ray
{
	t_3dpoint	org;
	t_vec3d		dir;
}			t_ray;

typedef struct s_rayhit
{
	double		t;
	t_3dpoint	intersect_p;
	t_vec3d		normal;
}	t_rayhit;

/* Primitive geometry structure */

typedef struct e_sphere
{
	t_3dpoint	center;
	double		radius;
}				t_sphere;

typedef struct e_plan
{
	t_3dpoint	point;
	t_vec3d		normal;
}				t_plan;

typedef struct e_disk
{
	t_3dpoint	center;
	t_3dpoint	rayon;
	t_vec3d		normal;
}				t_disk;

typedef struct e_object	t_object;

typedef bool (*t_intersect_fnct)(t_object *, t_ray *, t_rayhit *);

struct e_object
{
	union
	{
		t_sphere	sphere;
		t_plan		plan;
		t_disk		disk;
	} p;
	
	t_intersect_fnct	fnct;
};

typedef struct e_scene
{
	t_list	*objs;
}				t_scene;

#endif
