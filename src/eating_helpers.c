/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 06:42:00 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 06:42:00 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	clear_eating_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating_mutex);
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->eating_mutex);
}

int	start_eating_sequence(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating_mutex);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(&philo->eating_mutex);
		drop_forks(philo);
		return (0);
	}
	philo->is_eating = 1;
	pthread_mutex_unlock(&philo->eating_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	if (philo->data->must_eat_count != -1
		&& philo->meal_count >= philo->data->must_eat_count)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (1);
}
