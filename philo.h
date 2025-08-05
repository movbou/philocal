#include <limits.h>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

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
	int			fork_id;
}				t_fork;

/*
 philosopher
*/
typedef struct s_philo
{
	int			id;
	int			state;
	long		meal_count;
	long		last_meal_time;
	int			is_full;
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread;

}				t_philo;
/*
 data
*/
typedef struct s_data
{
	long		philo_count;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	int			must_eat_count;
	long		start_simulation_time;
	int			end_sumulation;
	t_fork		*forks;
	t_philo		*philos;

}				t_data;



int	parse_arguments(int argc, char **argv, t_data *data);
void exit_error(char *s);
