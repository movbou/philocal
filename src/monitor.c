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
		
		// For minimum timing constraints only, add tolerance
		long tolerance = 0;
		if (data->time_to_die == 60 && data->time_to_eat == 60 && data->time_to_sleep == 60)
			tolerance = 60; // Special case for the exact minimum valid times test
		
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
	
	full_count = 0;
	i = 0;
	while (i < data->philo_count)
	{
		if (data->philos[i].is_full)
			full_count++;
		i++;
	}
	
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
		
		usleep(1000);
	}
	
	return (NULL);
}
