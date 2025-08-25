/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: movbou <movbou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by movbou            #+#    #+#             */
/*   Updated: 2025/08/25 00:00:00 by movbou           ###   ########.fr       */
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
