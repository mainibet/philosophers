/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 10:30:30 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//join makes the main thread to wai for the execution of the others
//create thread per philo
//create monitor thread
int	start_threads(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if(pthread_create(&data->philo[i].thread_id, NULL, 
			life_cycle, &data->philo[i]) != 0)//check if 0 or other
		{
			clean_up_program(data);//new
			return (ERROR);
		}
		// printf("Thread created for philo ID: %d\n", data->philo[i].philo_id);//test
		i++;
	}
	if (data->total_philo > 0)
	{
		if (pthread_create(&data->monitor_thread_id, NULL, life_monitor, data) != 0)
		{
			clean_up_program(data);//new
			return (ERROR);
		}
	}
	return (SUCCESS);
}

int	setup_simulation(t_program *data)//check if **argv needed or only data?
{
	int	status;

	if (!data->end_flag)
		data->end_flag = PHILO_ALIVED;
	status = init_cross_mutex(data);
	if (status != SUCCESS)
	{
		clean_up_program(data);//new
		return (status);
	}
	status = init_forks(data);
	if (status != SUCCESS)
		return (status);
	status = init_philo(data);
	if (status != SUCCESS)
	{
		clean_up_program(data);//new
		return (status);
	}
	status = start_threads(data);
	if (status != SUCCESS)
	{
		clean_up_program(data);//new
		return (status);
	}
	return (SUCCESS);
}

//IN PUT:
//number of philo argv[1]
//time to die argv[2]
//time to eat argv[3]
//time to sleep argv[4]
//optional: # times each philo should eat argv[6]
//EXIT_SUCCESS and EXIT_FAILURE define in stdlib.h
//t_program is in the memory stack, not need to be in the heap (not malloc needed)
int	main(int argc, char **argv)
{
	t_arg_parse	parse;
	t_program	data;

	if (parsing_args(argc, argv, &parse) != SUCCESS)
		return (EXIT_FAILURE);//define in stdlib.h include clean-up before exit
	// printf("Ready to continue\n");//test
	data.parse = &parse;//memory allocated in parsing_args
	init_program(&data);
	if (setup_simulation(&data) != SUCCESS)
		return (EXIT_FAILURE);//DEFINE IN STDLIB.H//include clean-up before exit
	sim_stop(&data); //if succed can i called clean_up_program to finish? needs to join philos at the end before finishing and free all
	return (EXIT_SUCCESS);
}