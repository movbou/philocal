#include "../philo.h"

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long time)
{
	long	start;

	start = get_current_time();
	while (get_current_time() - start < time)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	long	current_time;

	pthread_mutex_lock(&philo->data->write_mutex);
	if (!is_simulation_ended(philo->data))
	{
		current_time = get_current_time() - philo->data->start_simulation_time;
		printf("%ld %d %s\n", current_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	cleanup_data(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->forks[i].fork);
			i++;
		}
		free(data->forks);
	}
	
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->end_simulation_mutex);
	
	if (data->philos)
		free(data->philos);
}

int	is_simulation_ended(t_data *data)
{
	int result;
	
	pthread_mutex_lock(&data->end_simulation_mutex);
	result = data->end_simulation;
	pthread_mutex_unlock(&data->end_simulation_mutex);
	return (result);
}

void	end_simulation(t_data *data)
{
	pthread_mutex_lock(&data->end_simulation_mutex);
	data->end_simulation = 1;
	pthread_mutex_unlock(&data->end_simulation_mutex);
}

void	print_error(char *s)
{
	printf("%s", s);
}
