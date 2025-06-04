/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:02:26 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/04 10:14:14 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//i is where the error happened
//will be usefull to destroy clean-up
int	mutex_fork_error(t_program  *data, int i)//DOES THIS NEED TO UNLOCK IF SOMETHING IS LOCK?
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
}//can this function work also for other mutex error?
