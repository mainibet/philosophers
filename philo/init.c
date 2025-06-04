/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:00:31 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/04 10:25:14 by albetanc         ###   ########.fr       */
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
		data->mutex_status = MUTEX_NO_INIT;//new
		return (ERR_MUTEX);//if success then set conditions to change mutex status to temrinate
	}
	data->mutex_status = MUTEX_INIT;//new
	// printf ("Mutex initialized correctly\n");//testing
	return (SUCCESS);
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
		{
			data->fork[i].mutex_status = MUTEX_NO_INIT;//not initilized
			return (mutex_fork_error(data, i));//this return errors how to use it here to finish the ft?
		//pending include destroy mutex for output and sim_over
		}
		data->fork[i].mutex_status = MUTEX_INIT;
		i++;
	}
    // printf ("Forks initialized correctly\n");//testing
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

	start_fork = philo_id -1;//to make it index
	philo = &data->philo[start_fork];//new
	philo->philo_id = philo_id;
	philo->meal_number = 0;
	philo->program = data;
	philo->left_fork = &data->fork[start_fork];//check
	philo->right_fork = &data->fork[start_fork + 1];//check
	philo->thread_id = 0;//TO AVOID ERRORS, CHECK IF IT'S OK
	// printf("This philo: %d has no thread yet\n", philo->philo_id);//test
}

int	init_philo(t_program *data)
{
	int	i;

	i = 0;
    // printf("Debug: total_philo = %d\n", data->total_philo); // Debug print
	data->philo = (t_philo *)malloc (sizeof(t_philo) * data->total_philo);
	if (!data->philo)
		return (malloc_error());//check if smt needs to free or destroy mutex before
	while (i < data->total_philo)
	{
		data->philo[i].mutex_status_phi = MUTEX_NO_INIT;
		if (pthread_mutex_init(&data->philo[i].philo_mutex, NULL) != SUCCESS)
				return (mutex_fork_error(data, i));//check function's name and call it correctlu with i
		data->philo[i].program = data;//check
		fill_each_philo(data, i + 1);//needs to be i + 1 if not becomes -1 in fill each philo
		// printf("Philosopher %d initialized with ID: %d\n", i + 1, data->philo[i].philo_id);//test
        // fflush(stdout);//test
		i++;
	}
    // printf("All philos initialized\n");//test
    //pending include destroy mutex for output and sim_over
	return (SUCCESS);
}

//number of philo arr[0]
//time to die arr[1]
//time to eat arr[2]
//time to sleep arr[3]
//optional: # times each philo should eat arr[4]
//mutex will be initialized later

void	init_program(t_program *data)//check if set default really needed
{
    // int i;
	data->total_philo = data->parse->arr[0];
	// printf("Debug: Setting total_philo to %d\n", data->total_philo); // Debug print
	data->time_die = data->parse->arr[1];
	data->time_eat = data->parse->arr[2];
	data->time_sleep = data->parse->arr[3];
	data->start_time = precise_time_ms();//chec it was 0
	data->end_flag = 0;
	data->philo = NULL;
	data->fork = NULL;
	if (data->parse->count == 4)
		data->max_meals = MAX_MEALS_DISABLED;
	else
		data->max_meals = data->parse->arr[4];
	// i = count_arr_elements(&data->parse->arr);
	// free_array(&data->parse->arr, i);//check if is ok here
}
