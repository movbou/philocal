#include "../philo.h"

int main(int argc, char **argv)
{
	t_data data;
	
	if (argc == 5 || argc == 6)
	{
		if (!parse_arguments(argc, argv, &data))
			return (1);
		if (!init_data(&data))
		{
			exit_error("Error: Failed to initialize data\n");
			return (1);
		}
		start_simulation(&data);
		cleanup_data(&data);
	}
	else
	{
		exit_error("Usage: ./philo philo_count time_to_die time_to_eat time_to_sleep [must_eat_count]\n");
		return (1);
	}
	return (0);
}
