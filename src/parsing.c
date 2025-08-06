#include "../philo.h"
#include <stdio.h>
#include <stdlib.h>

static int	validate_numeric_input(const char *str, int *i)
{
	if (str[*i] == '+')
		(*i)++;
	else if (str[*i] == '-')
	{
		print_error("Error: arguments cant be negative");
		return (0);
	}
	if (!(str[*i] >= '0' && str[*i] <= '9'))
	{
		print_error("Error: arguments cant be not a number");
		return (0);
	}
	return (1);
}

static long	convert_to_number(const char *str, int i)
{
	long	n;

	n = 0;
	while (str[i] >= '0' && str[i] <= '9')
		n = (n * 10) + (str[i++] - '0');
	if (n > INT_MAX)
	{
		print_error("Error: the input value is too big");
		return (LONG_MIN);
	}
	return (n);
}

long	ft_atol(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (!validate_numeric_input(str, &i))
		return (LONG_MIN);
	return (convert_to_number(str, i));
}

static int	parse_basic_args(char **argv, t_data *data)
{
	data->philo_count = ft_atol(argv[1]);
	if (data->philo_count == LONG_MIN)
		return (0);
	data->time_to_die = ft_atol(argv[2]);
	if (data->time_to_die == LONG_MIN)
		return (0);
	data->time_to_eat = ft_atol(argv[3]);
	if (data->time_to_eat == LONG_MIN)
		return (0);
	data->time_to_sleep = ft_atol(argv[4]);
	if (data->time_to_sleep == LONG_MIN)
		return (0);
	return (1);
}

static int	parse_optional_arg(int argc, char **argv, t_data *data)
{
	long	temp_value;

	if (argc == 6)
	{
		temp_value = ft_atol(argv[5]);
		if (temp_value == LONG_MIN)
			return (0);
		data->must_eat_count = (int)temp_value;
	}
	else
		data->must_eat_count = -1;
	return (1);
}

static int	validate_args(int argc, t_data *data)
{
	if (data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60 || (argc == 6 && data->must_eat_count < 0))
	{
		printf("Error: arguments must be at least 60 ms\n");
		return (0);
	}
	if (data->philo_count > MAX_PHILOS)
	{
		printf("Error: Too many philosophers (max %d)\n", MAX_PHILOS);
		return (0);
	}
	if (data->philo_count <= 0)
	{
		printf("Error: Number of philosophers must be positive\n");
		return (0);
	}
	return (1);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s philo_count time_to_die time_to_eat ", argv[0]);
		printf("time_to_sleep [must_eat_count]\n");
		return (0);
	}
	if (!parse_basic_args(argv, data))
		return (0);
	if (!parse_optional_arg(argc, argv, data))
		return (0);
	return (validate_args(argc, data));
}
