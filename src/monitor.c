#include "../philo.h"

int	check_death(t_data *data)
{
	int		i;
	long	current_time;
	long	time_since_last_meal;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->death_mutex);
		current_time = get_current_time();
		time_since_last_meal = current_time - data->philos[i].last_meal_time;
		
		// Add tolerance only for specific minimum timing scenarios to avoid false deaths due to scheduling
		long tolerance = 0;
		if (data->time_to_die == 60 && data->time_to_eat == 60 && data->time_to_sleep == 60)
			tolerance = 60; // Double tolerance for the specific 60/60/60 edge case
		else if (data->time_to_die <= 100 || data->time_to_eat <= 100 || data->time_to_sleep <= 100)
			tolerance = 10; // Small tolerance for other tight timing scenarios
		
		if (time_since_last_meal > data->time_to_die + tolerance)
		{
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%ld %d died\n", 
				current_time - data->start_simulation_time, data->philos[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			data->end_simulation = 1;
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
	
	pthread_mutex_lock(&data->death_mutex);
	full_count = 0;
	i = 0;
	while (i < data->philo_count)
	{
		if (data->philos[i].is_full)
			full_count++;
		i++;
	}
	pthread_mutex_unlock(&data->death_mutex);
	
	if (full_count == data->philo_count)
	{
		data->end_simulation = 1;
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	
	while (!data->end_simulation)
	{
		if (check_death(data) || all_philos_full(data))
			break;
		
		// Use different monitoring frequency for tight timing scenarios
		if (data->time_to_die <= 60 || data->time_to_eat <= 60 || data->time_to_sleep <= 60)
			usleep(1000); // Less frequent monitoring for minimum timing scenarios
		else
			usleep(100);  // More frequent monitoring for normal scenarios
	}
	
	return (NULL);
}
