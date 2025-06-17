/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 11:39:58 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	start_threads(t_program *data)
{
	int	status;

	pthread_mutex_lock(&data->start_mutex);
	if (data->total_philo == 1)
	{
		status = handle_one_philo(data);
		if (status != SUCCESS)
			return (handle_creation_error(data, status));
	}
	else
	{
		status = handle_many_philos(data);
		if (status != SUCCESS)
			return (handle_creation_error(data, status));
	}
	status = pthread_create(&data->monitor_thread_id, NULL, life_monitor, data);
	if (status != SUCCESS)
	{
		pthread_mutex_unlock(&data->start_mutex);
		clean_up_program(data);
		return (ERROR);
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_arg_parse	parse;
	t_program	*data;

	data = (t_program *)malloc(sizeof(t_program));
	if (!data)
		return (malloc_error());
	memset(data, 0, sizeof(t_program));
	if (parsing_args(argc, argv, &parse) != SUCCESS)
	{
		free(data);
		return (EXIT_FAILURE);
	}
	data->parse = &parse;
	init_program(data);
	if (setup_simulation(data) != SUCCESS)
	{
		clean_up_program(data);
		return (EXIT_FAILURE);
	}
	start_simulation(data);
	if (joining_threads(data) != SUCCESS)
		return (EXIT_FAILURE);
	clean_up_program(data);
	return (EXIT_SUCCESS);
}
