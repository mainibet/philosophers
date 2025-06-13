/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:07:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//join each philo
//join monitor
int	joining_threads(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (pthread_join(data->philo[i].thread_id, NULL) != 0)
		{
			clean_up_program(data);
			return (ERROR);
		}
		i++;
	}
	if (data->total_philo > 0)
	{
		if (pthread_join(data->monitor_thread_id, NULL) != 0)
		{
			clean_up_program(data);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

static int	handle_one_philo(t_program *data)
{
	if (pthread_create(&data->philo[0].thread_id, NULL,
			life_one_philo, &data->philo[0]) != SUCCESS)
	{
		clean_up_program(data);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	handle_many_philos(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (pthread_create(&data->philo[i].thread_id, NULL, 
				life_cycle, &data->philo[i]) != SUCCESS)
		{
			clean_up_program(data);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

//join makes the main thread to wai for the execution of the others.HOW THIS WORKS NOW?
//create thread per philo
//create monitor thread
int	start_threads(t_program *data)
{
	int	status;

	if (data->total_philo == 1)
	{
		status = handle_one_philo(data);
		if (status != SUCCESS)
			return (status);
	}
	else
	{
		status = handle_many_philos(data);
		if (status != SUCCESS)
			return (status);
	}
	status = pthread_create(&data->monitor_thread_id, NULL, life_monitor, data);
	if (status != SUCCESS)
	{
		clean_up_program(data);//new
		return (ERROR);
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
	t_program	*data;

	data = (t_program *)malloc(sizeof(t_program));//new
	if (!data)
		return (malloc_error());
	memset(data, 0, sizeof(t_program));//new
	if (parsing_args(argc, argv, &parse) != SUCCESS)
	{
		free(data);
		return (EXIT_FAILURE);//define in stdlib.h include clean-up before exit
	}
	// printf("Ready to continue\n");//test
	data->parse = &parse;//memory allocated in parsing_args
	init_program(data);//NEEDED TO INITIALIZEDD THE MUTEX OF START
	if (setup_simulation(data) != SUCCESS)
	{
		clean_up_program(data);
		return (EXIT_FAILURE);//DEFINE IN STDLIB.H//include clean-up before exit
	}
	pthread_mutex_lock(&data->start_mutex);
	data->start_time = precise_time_ms();
	pthread_mutex_lock(&data->end_mutex);//new
	data->sim_status = SIM_RUNNING;
	pthread_mutex_unlock(&data->end_mutex);//new
	pthread_mutex_unlock(&data->start_mutex);
	if (joining_threads(data) != SUCCESS)
		return (EXIT_FAILURE);
	clean_up_program(data);
	return (EXIT_SUCCESS);
}
