/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:47:52 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:47:53 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_death(t_data *data)
{
	int		i;
	long	current_time;
	long	time_since_last_meal;

	current_time = get_current_time();
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->philos[i].eating_mutex);
		if (data->philos[i].is_eating)
		{
			pthread_mutex_unlock(&data->philos[i].eating_mutex);
			i++;
			continue;
		}
		pthread_mutex_unlock(&data->philos[i].eating_mutex);
		pthread_mutex_lock(&data->death_mutex);
		time_since_last_meal = current_time - data->philos[i].last_meal_time;
		if (time_since_last_meal > data->time_to_die)
		{
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%ld %d died\n",
				current_time - data->start_simulation_time, data->philos[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			end_simulation(data);
			return (1);
		}
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	return (0);
}

int	all_philos_full(t_data *data)
{
	int	i;
	int	full_count;

	if (data->must_eat_count == -1)
		return (0);
	full_count = 0;
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->philos[i].is_full)
			full_count++;
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	if (full_count == data->philo_count)
	{
		end_simulation(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	usleep(1000);
	while (!is_simulation_ended(data))
	{
		if (check_death(data))
			return (NULL);
		if (all_philos_full(data))
		{
			end_simulation(data);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
