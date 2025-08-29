/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:47:46 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:47:48 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	data.end_simulation = 0;
	if (!parse_arguments(argc, argv, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	if (!start_simulation(&data))
		return (1);
	cleanup_data(&data);
	return (0);
}
