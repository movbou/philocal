/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: movbou <movbou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by movbou            #+#    #+#             */
/*   Updated: 2025/08/25 00:00:00 by movbou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	print_error(char *s)
{
	printf("%s", s);
}

int	validate_numeric_input(const char *str, int *i)
{
	if (str[*i] != '+' && (str[*i] < '0' || str[*i] > '9'))
	{
		print_error("Error: argument must be a positive number\n");
		return (0);
	}
	if (str[*i] == '+')
		(*i)++;
	if (str[*i] < '0' || str[*i] > '9')
	{
		print_error("Error: invalid number format\n");
		return (0);
	}
	return (1);
}

long	convert_to_number(const char *str, int i)
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
