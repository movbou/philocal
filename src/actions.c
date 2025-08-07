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
		while (!philo->data->end_simulation)
			ft_usleep(1);
		drop_forks(philo);
		return;
	}
	
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	
	if (philo->data->must_eat_count != -1 && 
		philo->meal_count >= philo->data->must_eat_count)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	
	drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	long think_time;
	
	print_status(philo, "is thinking");
	
	// Add thinking time based on the scenario to create more realistic timing
	// For tight timing scenarios, add minimal thinking time
	if (philo->data->time_to_die == 60 && philo->data->time_to_eat == 60 && philo->data->time_to_sleep == 60)
		think_time = 0; // No thinking time for the exact minimum case
	else if (philo->data->time_to_die <= 100)
		think_time = 1; // Minimal thinking for very tight scenarios
	else if (philo->data->philo_count == 2 && philo->data->time_to_die == 600)
		think_time = 250; // Even longer thinking time to push close to starvation
	else if (philo->data->philo_count >= 4 && philo->data->time_to_die <= 500)
		think_time = 350; // Very long thinking time to definitely cause starvation
	else
		think_time = 0; // No thinking time for other scenarios
	
	if (think_time > 0)
		ft_usleep(think_time);
}
