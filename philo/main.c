/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/02 08:31:36 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//init mutex for mutex output
//init mutex for sim_over
//return SUCCESS or ERR_MUTEX
int	init_cross_mutex(t_program *data)//do i have to set attributes for philo?
{
	if (pthread_mutex_init(&data->output_mutex, NULL) != SUCCESS)//CAN I USE THE MACRO?
	{
		print_error_msg("Failed to init mutex output\n");//check
		return (ERR_MUTEX);//if success then set conditions to change mutex status to temrinate
	}
	printf ("Mutex initialized correctly\n");//testing
	return (SUCCESS);
}

//i is where the error happened
//will be usefull to destroy clean-up
int	mutex_fork_error(t_program  *data, int i)
{
	int	j;//new

	print_error_msg("Failed to init fork mutex\n");//PENDING DESTROY MUTEX AFTER THE ERROR
	j = 0;
	while (j < i)//new check if needed here or may be while loop in clean-up function
	{
		pthread_mutex_destroy(&data->fork[j].mutex);//check
		j++;
	}
	free(data->fork);//check
	data->fork = NULL;//check sure?
	return (ERR_MUTEX);
}

int	init_forks(t_program *data)
{
	int	i;

	i = 0;
	data->fork = (t_fork *)malloc(sizeof(t_fork) * data->total_philo);
	if (!data->fork)
		return (malloc_error());
	while (i < data->total_philo)
	{
		data->fork[i].fork_id = i + 1;//check if can begin in 1 and not increas +1 every time
		if (pthread_mutex_init(&data->fork[i].mutex, NULL) != SUCCESS)//CHECK IF CAN BE SUCCESS INSTEAD OF 0
			return (mutex_fork_error(data, i));//this return errors how to use it here to finish the ft?
		//pending include destroy mutex for output and sim_over
		i++;
	}
    printf ("Forks initialized correctly\n");//testing
	return (SUCCESS);
}

//check if needs to be init the last meal or when
//needs to calculate start_fork
//Like this: Calculate the 0-indexed array position for 
//this philosopher's corresponding fork.
//THREAD_ID NOT NEED TO INIT FOR NOW
//start_fork will help to find forks and philo position
//the starting fork is left
//philo thread init in 0 to avoid garbage in memory
void	fill_each_philo(t_program *data, int philo_id)
{
	int		start_fork;//check
	t_philo	*philo;

	start_fork = philo_id -1;
	philo = &data->philo[start_fork];//new
	philo->philo_id = philo_id;
	philo->meal_number = 0;
	philo->program = data;
	philo->left_fork = &data->fork[start_fork];//check
	philo->right_fork = &data->fork[start_fork + 1];//check
	philo->thread_id = 0;//TO AVOID ERRORS, CHECK IF IT'S OK
	printf("Este filósofo no tiene hilo creado aún.\n");//test
}

int	init_philo(t_program *data)
{
	int	i;

	i = 0;
	data->philo = (t_philo *)malloc (sizeof(t_philo) * data->total_philo);
	if (!data->philo)
		return (malloc_error());//check if smt needs to free or destroy mutex before
	while (i < data->total_philo)
	{
		data->philo[i].program = data;//check
		fill_each_philo(data, i + 1);//check how is sent philo[i]
		i++;
	}
    printf ("Philos initialized correctly\n");//testing
    //pending include destroy mutex for output and sim_over
	return (SUCCESS);
}

// int	start_threads(t_program *data)
// {

// }

int	setup_simulation(t_program *data)//check if **argv needed or only data?
{
	int	status;

	memset(data, 0, sizeof(t_program));
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
	// status = start_threads(data);
	// if (status != SUCCESS)
	// 	return (status);//clean-up before return
	return (SUCCESS);
}

void	set_default(t_program *data)//check if really needed or only return
{
	data->total_philo = 0;
	data->max_meals = -1;
	data->time_die = 0;
	data->time_eat = 0;
	data->time_sleep = 0;
	data->start_time = 0;
	data->end_flag = 0;
	data->philo = NULL;//CHECK MAY BE NOT NEEDED 
	data->fork = NULL;//CHECK MAY BE NOT NEEDED
	data->parse = NULL;// CHECK IT MAY BE NOT NEEDED
}
//number of philo arr[0]
//time to die arr[1]
//time to eat arr[2]
//time to sleep arr[3]
//optional: # times each philo should eat arr[4]
//mutex will be initialized later

void	init_program(t_program *data)//check if set default really needed
{
	if (!data || !data->parse || !data->parse->arr)
	{
		if (data)
			set_default(data);
		return ;
	}
	if (data->parse->count < 4) // Not enough arguments, set defaults or handle error
	{
		set_default(data);
		return ;
	}
	data->total_philo = data->parse->arr[0];
	data->time_die = data->parse->arr[1];
	data->time_eat = data->parse->arr[2];
	data->time_sleep = data->parse->arr[3];
	data->start_time = 0;
	data->end_flag = 0;
	data->philo = NULL;
	data->fork = NULL;
	if (data->parse->count == 4)
		data->max_meals = MAX_MEALS_DISABLED;
	else
		data->max_meals = data->parse->arr[4];
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