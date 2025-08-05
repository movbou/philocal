#include "../philo.h"
#include <stdio.h>
#include <stdlib.h>

void exit_error(char *s)
{
	printf("%s", s);
	exit(1);
}

long	ft_atol(const char *str)
{
	int		i;
	long	n;
	int number_counter = 0;

	i = 0;
	n = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		exit_error("Error: arguments cant be negative");
	if (!(str[i] >= '0' && str[i] <= '9'))
		exit_error("Error: arguments cant be not a number");

	while (str[i] >= '0' && str[i] <= '9')
	{
		n = (n * 10) + (str[i++] - '0');
		number_counter++;
	}
	if(n > INT_MAX)
		exit_error("Error: the input value is too big");
	return (n);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s philo_count time_to_die time_to_eat time_to_sleep [must_eat_count]\n", argv[0]);
		return (0);
	}
	data->philo_count = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atol(argv[5]);
	else
		data->must_eat_count = -1;
	if (data->time_to_die < 60000 || data->time_to_eat < 60000
		|| data->time_to_sleep < 60000 || (argc == 6 && data->must_eat_count < 60000))
	{
		printf("Error: arguments must be at least 60000 ms\n");
		return (0);
	}

	// if (data->philo_count == -1 || data->time_to_die == -1
	// 	|| data->time_to_eat == -1 || data->time_to_sleep == -1 || (argc == 6
	// 		&& data->must_eat_count == -1))
	// {
	// 	printf("Error: Invalid arguments\n");
	// 	return (0);
	// }
	if (data->philo_count > MAX_PHILOS)
	{
		printf("Error: Too many philosophers (max %d)\n", MAX_PHILOS);
		return (0);
	}
	return (1);
}
