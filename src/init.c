/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:47:37 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:47:40 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <pthread.h>
#include <stdlib.h>

static int	init_fork_mutexes(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->philo_count)
	{
		error = pthread_mutex_init(&data->forks[i].fork, NULL);
		if (error)
			return (0);
		error = pthread_mutex_init(&data->philos[i].eating_mutex, NULL);
		if (error)
			return (0);
		data->forks[i].fork_id = i;
		i++;
	}
	return (1);
}

static int	init_other_mutexes(t_data *data)
{
	int	error;

	error = pthread_mutex_init(&data->write_mutex, NULL);
	if (error)
		return (0);
	error = pthread_mutex_init(&data->death_mutex, NULL);
	if (error)
		return (0);
	error = pthread_mutex_init(&data->end_simulation_mutex, NULL);
	if (error)
		return (0);
	return (1);
}

int	init_mutex(t_data *data)
{
	data->forks = malloc(sizeof(t_fork) * data->philo_count);
	if (!data->forks)
		return (0);
	if (!init_fork_mutexes(data))
		return (0);
	if (!init_other_mutexes(data))
		return (0);
	return (1);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].state = THINKING;
		data->philos[i].meal_count = 0;
		data->philos[i].last_meal_time = data->start_simulation_time;
		data->philos[i].is_full = 0;
		data->philos[i].is_eating = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_count];
		data->philos[i].data = data;
		i++;
	}
}

int	init_data(t_data *data)
{
	data->end_simulation = 0;
	data->start_simulation_time = get_current_time();
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (0);
	if (!init_mutex(data))
	{
		free(data->philos);
		return (0);
	}
	init_philosophers(data);
	return (1);
}
