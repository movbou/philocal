/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:48:34 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:48:44 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	is_simulation_ended(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->end_simulation_mutex);
	result = data->end_simulation;
	pthread_mutex_unlock(&data->end_simulation_mutex);
	return (result);
}

void	end_simulation(t_data *data)
{
	pthread_mutex_lock(&data->end_simulation_mutex);
	data->end_simulation = 1;
	pthread_mutex_unlock(&data->end_simulation_mutex);
}

int	is_simulation_active(t_data *data)
{
	return (!is_simulation_ended(data));
}
