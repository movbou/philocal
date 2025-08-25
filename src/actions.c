#include "../philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		print_status(philo, "has taken a fork");
		return;
	}
	
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->fork);
		print_status(philo, "has taken a fork");
	}
}

void	drop_forks(t_philo *philo)
{
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_unlock(&philo->left_fork->fork);
		return;
	}
	
	pthread_mutex_unlock(&philo->left_fork->fork);
	pthread_mutex_unlock(&philo->right_fork->fork);
}

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	
	if (philo->data->philo_count == 1)
	{
		// Single philosopher can only take one fork, waits forever trying to get the second
		// They will never actually eat and will eventually die
		while (!is_simulation_ended(philo->data))
			ft_usleep(1);
		drop_forks(philo);
		return;
	}
	
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->must_eat_count != -1 && 
		philo->meal_count >= philo->data->must_eat_count)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}
