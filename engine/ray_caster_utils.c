/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_caster_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:57:47 by deddara           #+#    #+#             */
/*   Updated: 2020/08/16 17:59:37 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "engine.h"

int getpixelcolor(t_data *img, int x, int y)
{
	if (!img->width || !img->height)
		return (0);
	x = abs(x);
	y = abs(y);
	if (x > img->width || y > img->height || x < 0 || y < 0)
		return (0);
	return (*(int*)(img->addr + ((x + (y * img->width)) * (img->bits_per_pixel / 8))));
}

int skipper(t_map *map, int y, int x)
{
	if (map->map[y][x] == ' ' || map->map[y][x] == '0' || map->map[y][x] == '2' ||
		map->map[y][x] == 'N' || map->map[y][x] == 'S' || map->map[y][x] == 'W'
		|| map->map[y][x] == 'E')
		return (1);
	return (0);
}