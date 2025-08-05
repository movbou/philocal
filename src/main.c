#include "../philo.h"
int main(void)
{
	t_data data;
	if(ac==5 || ac == 6)
	{
		parse_arguments();
		init_data(&data);
		start(&data);
		clean(&data);

	}
	else {
		exit_error("Usage: ./philo philo_count time_to_die time_to_eat time_to_sleep [must_eat_count]\n");
	
	}
}
