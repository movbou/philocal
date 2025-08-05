#include "../philo.h"
#include <pthread.h>
#include <stdlib.h>

int	init_mutex(t_data *data)
{
	int	i;
	int	error;

	data->forks = malloc(sizeof(t_fork) * data->philo_count);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		error = pthread_mutex_init(data->forks[i].fork, NULL);
		if (!error)
			return (0);
		i++;
	}
}
void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].state = THINKING;
		data->philos[i].meal_count= 0;
		data->philos[i].last_meal_time = data->start_simulation_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_count];
		i++;
	}
}
int	init_data(void)
{
}
