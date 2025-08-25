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
		
		// NO TOLERANCE - strict timing as per problem requirements
		if (time_since_last_meal > data->time_to_die)
		{
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%ld %d died\n", 
				current_time - data->start_simulation_time, data->philos[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			end_simulation(data);
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
		pthread_mutex_lock(&data->death_mutex);
		if (data->philos[i].is_full)
			full_count++;
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	
	if (full_count == data->philo_count)
	{
		end_simulation(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	
	while (!is_simulation_ended(data))
	{
		if (check_death(data) || all_philos_full(data))
			break;
		
		// Higher frequency monitoring for better precision
		// Especially important for short time_to_die values
		if (data->time_to_die <= 100)
			usleep(200);  // Check every 0.2ms for very short death times
		else
			usleep(500);  // Check every 0.5ms for normal cases
	}
	
	return (NULL);
}
