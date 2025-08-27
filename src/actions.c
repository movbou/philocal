/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: movbou <movbou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by movbou            #+#    #+#             */
/*   Updated: 2025/08/25 00:00:00 by movbou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	print_fork_status(t_philo *philo)
{
	if (!is_simulation_ended(philo->data))
	{
		pthread_mutex_lock(&philo->data->write_mutex);
		if (!is_simulation_ended(philo->data))
			printf("%ld %d has taken a fork\n",
				get_current_time() - philo->data->start_simulation_time,
				philo->id);
		pthread_mutex_unlock(&philo->data->write_mutex);
	}
}

int	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		print_fork_status(philo);
		while (!is_simulation_ended(philo->data))
			usleep(1000);
		pthread_mutex_unlock(&philo->left_fork->fork);
		return (0);
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		print_fork_status(philo);
		pthread_mutex_lock(&philo->left_fork->fork);
		print_fork_status(philo);
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		print_fork_status(philo);
		pthread_mutex_lock(&philo->right_fork->fork);
		print_fork_status(philo);
	}
	return (1);
}

void	drop_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_unlock(&philo->left_fork->fork);
		return ;
	}
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_unlock(&philo->right_fork->fork);
}

void	philo_eat(t_philo *philo)
{
	if (!take_forks(philo))
		return ;
	if (is_simulation_ended(philo->data))
	{
		drop_forks(philo);
		return ;
	}
	pthread_mutex_lock(&philo->data->write_mutex);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(&philo->data->write_mutex);
		drop_forks(philo);
		return ;
	}
	printf("%ld %d is eating\n",
		get_current_time() - philo->data->start_simulation_time, philo->id);
	pthread_mutex_unlock(&philo->data->write_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	if (philo->data->must_eat_count != -1 && 
		philo->meal_count >= philo->data->must_eat_count)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	ft_usleep(philo->data->time_to_eat);
	drop_forks(philo);
}
