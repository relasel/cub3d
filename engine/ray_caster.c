/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_caster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 14:46:39 by deddara           #+#    #+#             */
/*   Updated: 2020/08/27 17:51:01 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "main.h"

void		dir_calc(t_raycast *ray, t_map *map)
{
	ray->dir_x = 0;
	ray->dir_y = 0;
	ray->plane_x = 0;
	ray->plane_y = 0;
	if (map->player_pos == 'N')
	{
		ray->dir_y = -1;
		ray->plane_x = tan(M_PI * ANGLE / 360);
	}
	else if (map->player_pos == 'S')
	{
		ray->dir_y = 1;
		ray->plane_x = -tan(M_PI * ANGLE / 360);
	}
	else if (map->player_pos == 'W')
	{
		ray->dir_x = -1;
		ray->plane_y = -tan(M_PI * ANGLE / 360);
	}
	else if (map->player_pos == 'E')
	{
		ray->dir_x = 1;
		ray->plane_y = tan(M_PI * ANGLE / 360);
	}
}

static void	step_side_calc(t_raycast *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->dist_x = (ray->player_x - ray->map_x) * ray->dlt_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->dist_x = (1.0 + ray->map_x - ray->player_x) * ray->dlt_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->dist_y = (ray->player_y - ray->map_y) * ray->dlt_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->dist_y = (1.0 + ray->map_y - ray->player_y) * ray->dlt_dist_y;
	}
}

static void	check_wall(t_raycast *ray, t_map *map)
{
	ray->hit = 0;
	while (!ray->hit)
	{
		if (ray->dist_x < ray->dist_y)
		{
			ray->dist_x += ray->dlt_dist_x;
			ray->map_x += ray->step_x;
			ray->wall_side = (ray->ray_dir_x < 0) ? 2 : 3;
		}
		else
		{
			ray->dist_y += ray->dlt_dist_y;
			ray->map_y += ray->step_y;
			ray->wall_side = (ray->ray_dir_y < 0) ? 0 : 1;
		}
		if (!skipper(map, ray->map_y, ray->map_x))
			ray->hit = 1;
	}
	if (ray->wall_side > 1)
		ray->wall_dist = ((ray->map_x - ray->player_x + (1 - ray->step_x) \
				/ 2) / ray->ray_dir_x);
	else
		ray->wall_dist = ((ray->map_y - ray->player_y + (1 - ray->step_y) \
				/ 2) / ray->ray_dir_y);
}

void		ray_caster(t_map *map, t_data *img, t_raycast *ray)
{
	int			x;

	x = 0;
	while (x < map->x)
	{
		ray->camera_x = 2 * x / (double)map->x - 1;
		ray->ray_dir_x = ray->dir_x + ray->plane_x * ray->camera_x;
		ray->ray_dir_y = ray->dir_y + ray->plane_y * ray->camera_x;
		if (!ray->ray_dir_y)
			ray->dlt_dist_x = 0;
		else
			ray->dlt_dist_x = (!ray->ray_dir_x) ? 1 : fabs(1 / ray->ray_dir_x);
		if (!ray->ray_dir_x)
			ray->dlt_dist_y = 0;
		else
			ray->dlt_dist_y = (!ray->ray_dir_y) ? 1 : fabs(1 / ray->ray_dir_y);
		ray->map_x = (int)ray->player_x;
		ray->map_y = (int)ray->player_y;
		step_side_calc(ray);
		check_wall(ray, map);
		paint_map(ray, map, img, x);
		ray->x_buffer[x++] = ray->wall_dist;
	}
	sprite_handler(ray);
}
