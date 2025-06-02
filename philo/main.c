/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/02 11:57:18 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//takes the current hour
//makes the equivalent to the starting moment I set
//make it precise milisec
//gettimeofday is very precise microsec
//* 1000LL convert sec to milisec
// /1000 convert microsec to milisec
//tv.tv_sec = seconds passed utils the moment is called
//tv.tv_usec =  microsec that additional to the seconds have passed
//conver microsec to milisec (tv.tv_usec / 1000)
long long	precise_time_ms(void)//NEW
{
	struct timeval	tv;
	long long		all_sec;
	long long		rem_microsec;
	long long		total_milisec;

	gettimeofday(&tv, NULL); // No timezone
	all_sec = tv.tv_sec * 1000LL;
	rem_microsec = tv.tv_usec / 1000;
	total_milisec = all_sec + rem_microsec;
	return (total_milisec);
}
// int set_initial_timestamps(t_program *program_data)//new
// {
//     int i;

//     program_data->start_time = get_current_time_ms();
//     if (program_data->start_time == 0) // Basic error check for time function (rare)
//     {
//         print_error_msg("Failed to get start time.");
//         return (ERROR); // A generic error, or define a specific one if time functions can fail
//     }

//     i = 0;
//     while (i < program_data->total_philo)
//     {
//         program_data->philo[i].last_meal = program_data->start_time;
//         i++;
//     }
//     return (SUCCESS);
// }

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create
//void *(*start_routine)(void *), void *arg
//thread logic
void	*life_cycle(void *arg)
{
	t_philo *philo;//check if can be use this struct

	philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->data->output_mutex);//connect good data in philo
    //print smt for testing
    //then unlock for the next philo
	printf("Philosopher %d is alive!\n", philo->philo_id);//test life cycle
	sleep(1);  // Simulate some work
    //pending to set a while loop with a cond to stop the loop term condition
	// take_forks(philo);//function with pthread_mutex_lock. 
    //check how to indicate take right and left if is odd or even
	printf("Philosopher %d is done.\n", philo->philo_id);//test only thread creation
	// eat(philo);//reacord time of last meal usleep(time_to_eat)
	// release_fork(philo); ///function to unlock mutex needs to be per each fork
	// sleep(philo); //usleep(time_to_sleep)
	// think(philo);//dynamic how should be set?
	return (NULL);
}

int	start_threads(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		pthread_create(&data->philo[i].thread_id, NULL, 
			life_cycle, &data->philo[i]);
		printf("Thread created for philo ID: %d\n", data->philo[i].philo_id);
		i++;
	}//after include a while loop to join threads then all will be working
	return (SUCCESS);
}

int	setup_simulation(t_program *data)//check if **argv needed or only data?
{
	int	status;

	if (!data->end_flag)
		data->end_flag = 0;//just to initialite the flag
	status = init_cross_mutex(data);
	if (status != SUCCESS)
		return (status);//check and clean-up before return
	status = init_forks(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	status = init_philo(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	status = start_threads(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	return (SUCCESS);
}

//IN PUT:
//number of philo argv[1]
//time to die argv[2]
//time to eat argv[3]
//time to sleep argv[4]
//optional: # times each philo should eat argv[6]
//t_program is in the memory stack, not need to be in the heap (not malloc needed)
int	main(int argc, char **argv)
{
	t_arg_parse	parse;
	t_program	data;

	if (parsing_args(argc, argv, &parse) != SUCCESS)
		return (EXIT_FAILURE);//define in stdlib.h
	printf("Ready to continue\n");//test
	data.parse = &parse;//memory allocated in parsing_args
	init_program(&data);
	if (setup_simulation(&data) != SUCCESS)
		return (EXIT_FAILURE);//DEFINE IN STDLIB.H
	// if (start_simulation() != SUCCESS)
	// 	return (1);
    //join philo threads
    // free (parse.arr);//'cause was used to feed the program and clean up all
	return (EXIT_SUCCESS);//DEFINE IN STDLIB.H
}