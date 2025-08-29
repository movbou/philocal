/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achajar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:47:59 by achajar           #+#    #+#             */
/*   Updated: 2025/08/29 02:48:06 by achajar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
	if ((argc == 6 && data->must_eat_count < 0))
	{
		printf("Error: must_eat_count cannot be negative\n");
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
