/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: movbou <movbou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by movbou            #+#    #+#             */
/*   Updated: 2025/08/25 00:00:00 by movbou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_PHILOS 200
#define THINKING 0
#define EATING 1
#define SLEEPING 2
#define DEAD 3

/*
	fork
*/
typedef struct s_fork
{
	pthread_mutex_t		fork;
	int					fork_id;
}				t_fork;

/*
 philosopher
*/
typedef struct s_philo
{
	int					id;
	int					state;
	long				meal_count;
	long				last_meal_time;
	int					is_full;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_t			thread;
	struct s_data		*data;
}				t_philo;

/*
 data
*/
typedef struct s_data
{
	long				philo_count;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					must_eat_count;
	long				start_simulation_time;
	int					end_simulation;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		end_simulation_mutex;
	t_fork				*forks;
	t_philo				*philos;
}				t_data;

/* parsing.c */
int		parse_arguments(int argc, char **argv, t_data *data);
long	ft_atol(const char *str);
int		validate_numeric_input(const char *str, int *i);
long	convert_to_number(const char *str, int i);
void	print_error(char *s);

/* init.c */
int		init_data(t_data *data);
int		init_mutex(t_data *data);
void	init_philosophers(t_data *data);

/* utils.c */
long	get_current_time(void);
void	ft_usleep(long time);
void	print_status(t_philo *philo, char *status);
void	cleanup_data(t_data *data);
int		is_simulation_ended(t_data *data);
void	end_simulation(t_data *data);

/* simulation.c */
int		start_simulation(t_data *data);
void	*philosopher_routine(void *arg);

/* actions.c */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
int		take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

/* monitor.c */
void	*monitor_routine(void *arg);
int		check_death(t_data *data);
int		all_philos_full(t_data *data);
