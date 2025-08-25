/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: movbou <movbou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by movbou            #+#    #+#             */
/*   Updated: 2025/08/25 00:00:00 by movbou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	philo_sleep(t_philo *philo)
{
	if (is_simulation_ended(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->write_mutex);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(&philo->data->write_mutex);
		return ;
	}
	printf("%ld %d is sleeping\n",
		get_current_time() - philo->data->start_simulation_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	if (is_simulation_ended(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->write_mutex);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(&philo->data->write_mutex);
		return ;
	}
	printf("%ld %d is thinking\n",
		get_current_time() - philo->data->start_simulation_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
}
