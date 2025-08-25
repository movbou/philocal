#include "../philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	if (philo->id % 2 == 0)
		ft_usleep(1);
	
	while (!is_simulation_ended(philo->data))
	{
		if (philo->is_full)
			break;
		
		philo_eat(philo);
		
		if (is_simulation_ended(philo->data))
			break;
		
		philo_sleep(philo);
		
		if (is_simulation_ended(philo->data))
			break;
		
		philo_think(philo);
	}
	
	return (NULL);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, 
			philosopher_routine, &data->philos[i]))
		{
			print_error("Error: Failed to create philosopher thread\n");
			return (0);
		}
		i++;
	}
	
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data))
	{
		print_error("Error: Failed to create monitor thread\n");
		return (0);
	}
	
	pthread_join(monitor_thread, NULL);
	
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (1);
}
