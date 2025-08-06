#include "../philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	if (philo->id % 2 == 0)
		ft_usleep(1);
	
	while (!philo->data->end_simulation)
	{
		if (philo->is_full)
			break;
		
		philo_eat(philo);
		
		if (philo->data->end_simulation)
			break;
		
		philo_sleep(philo);
		
		if (philo->data->end_simulation)
			break;
		
		philo_think(philo);
	}
	
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]))
		{
			exit_error("Error: Failed to create philosopher thread\n");
			return;
		}
		i++;
	}
	
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data))
	{
		exit_error("Error: Failed to create monitor thread\n");
		return;
	}
	
	pthread_join(monitor_thread, NULL);
	
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}
