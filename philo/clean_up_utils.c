/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 07:58:59 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/16 08:20:40 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

//This function will ONLY destroy fork mutexes.
static void	clean_mutex_forks(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (data->fork[i].fork_mut_status == MUTEX_INIT)
			pthread_mutex_destroy(&data->fork[i].fork_mutex);
		i++;
	}
}

// Good practice: nullify after freeing at the end
void	clean_fork(t_program *data)
{
	if (data->fork != NULL)
	{
		clean_mutex_forks(data);
		free(data->fork);
		data->fork = NULL;
	}
}

//This function will ONLY destroy philo mutexes.
static void	clean_mutex_philo(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (data->philo[i].mutex_status_phi == MUTEX_INIT)
			pthread_mutex_destroy(&data->philo[i].philo_mutex);
		i++;
	}
}

void	clean_philo(t_program *data)
{
	if (data->philo != NULL)
	{
		clean_mutex_philo(data);
		free(data->philo);
		data->philo = NULL;
	}
}

void	clean_parse(t_program *data)
{
	if (data->parse != NULL)//check if the struct no need to be freed
	{
		if (data->parse->arr != NULL)//check the array inside
		{
			free(data->parse->arr);//free arr
			data->parse->arr = NULL;//check if is good
		}
	}
}
