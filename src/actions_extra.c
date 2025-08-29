/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:47:14 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:47:25 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	update_meal_data(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	if (philo->data->must_eat_count != -1
		&& philo->meal_count >= philo->data->must_eat_count)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
}

int	print_eating_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(&philo->data->write_mutex);
		return (0);
	}
	printf("%ld %d is eating\n",
		get_current_time() - philo->data->start_simulation_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	return (1);
}

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
	usleep(1000);
}
