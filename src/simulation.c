/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:48:22 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:48:28 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!is_simulation_ended(philo->data))
	{
		if (philo->is_full)
			break ;
		philo_eat(philo);
		if (is_simulation_ended(philo->data))
			break ;
		philo_sleep(philo);
		if (is_simulation_ended(philo->data))
			break ;
		philo_think(philo);
	}
	return (NULL);
}

static int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]))
		{
			print_error("Error: Failed to create philosopher thread\n");
			return (0);
		}
		i++;
	}
	return (1);
}

static void	join_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor_thread;

	if (!create_philosopher_threads(data))
		return (0);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data))
	{
		print_error("Error: Failed to create monitor thread\n");
		return (0);
	}
	pthread_join(monitor_thread, NULL);
	join_philosopher_threads(data);
	return (1);
}
