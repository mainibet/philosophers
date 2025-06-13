/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:35:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:46:32 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handling_mutex_init(pthread_mutex_t *my_mutex, int *mutex_status, char *msg)//new check
{
	if (pthread_mutex_init(my_mutex, NULL) != SUCCESS)
	{
		print_error_msg(msg);
		*mutex_status = MUTEX_NO_INIT;//new check
		return (ERR_MUTEX);
	}
	*mutex_status = MUTEX_INIT;
	return (SUCCESS);
}

//takes the current hour
//makes the equivalent to the starting moment I set
//make it precise milisec
//gettimeofday is very precise microsec
//* 1000LL convert sec to milisec
// /1000 convert microsec to milisec
//tv.tv_sec = seconds passed utils the moment is called
//tv.tv_usec =  microsec that additional to the seconds have passed
//conver microsec to milisec (tv.tv_usec / 1000)
// No timezone
long long	precise_time_ms(void)
{
	struct timeval	tv;
	long long		all_sec;
	long long		rem_microsec;
	long long		total_milisec;

	gettimeofday(&tv, NULL); 
	all_sec = tv.tv_sec * 1000LL;
	rem_microsec = tv.tv_usec / 1000;
	total_milisec = all_sec + rem_microsec;
	return (total_milisec);
}

int	count_arr_elements(void	**arr)//check if needed in philo
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}


int	ft_strlen(const char *str)
{
	int	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

void	order_forks(t_philo *philo, t_fork **first, t_fork **second)
{
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}


