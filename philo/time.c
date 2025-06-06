/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:47:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 07:58:21 by albetanc         ###   ########.fr       */
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

int	check_end_cond(t_philo *philo)
{
	int	sim_status;

	pthread_mutex_lock(&philo->program->end_mutex);
	sim_status = philo->program->end_flag;
	pthread_mutex_unlock(&philo->program->end_mutex);
	return (sim_status);
}

//destroy mutex: output, fork, philo, monitor
//free: array of philo
//free:array of forks
//free: array of parse
void	clean_up_program(t_program *data)//check if philo dies if forks need to be "released"
{//this function should be made by the main thread
	int	i;

	i = 0;
	if (data->out_mut_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->output_mutex);//need to includ error check of destroy?
	if (data->end_mutex_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->end_mutex);
	if (data->fork != NULL)
	{
		i = 0;
		while (i < data->total_philo)
		{
			if (data->fork[i].fork_mut_status == MUTEX_INIT)
				pthread_mutex_destroy(&data->fork[i].mutex);//need to includ error check of destroy?
			i++;
		}
		i = count_arr_elements((void **)data->fork);//need to include any handling error?
		free_array((void **)data->fork, i);
	}
	if (data->philo != NULL)
	{
		i = 0;
		while (i < data->total_philo)
		{
			if (data->philo[i].mutex_status_phi == MUTEX_INIT)
				pthread_mutex_destroy(&data->philo[i].philo_mutex);
			i++;
		}
		free_array((void **)data->philo, data->total_philo);
	}
	if (data->parse != NULL && data->parse->arr != NULL)
	{
		i = count_arr_elements((void **)data->parse);
		free_array((void **)data->parse, i);
	}
}

//sim wait until all threads joined and clean-up
void	sim_stop(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		pthread_join(data->philo[i].thread_id, NULL);//check
		i++;
	}
	if (data->total_philo > 0)
	{
		if (data->monitor_thread_id != 0)
			pthread_join(data->monitor_thread_id, NULL);//check
	}
	clean_up_program(data);//check if is working and full and includes monitor's thread
}

void	end_program(t_program *data, char *msg, t_philo *dying_philo)
{
	pthread_mutex_lock(&data->end_mutex);
	if (data->end_flag == PHILO_ALIVED) // Only set if not already terminated
	{
		data->end_flag = PHILO_DIED;
		// pthread_mutex_lock(&data->output_mutex);
		if (dying_philo)
			print_status(dying_philo, msg);//check
		//print debug here total foods
		// pthread_mutex_unlock(&data->output_mutex);
	}
	pthread_mutex_unlock(&data->end_mutex);
}

//if receives program it will only work with philo[0]
//with param philo it can check all
int check_life(t_philo *philo)
{
	long long	since_last_meal;
	int			philo_status;

	philo_status = PHILO_ALIVED;
	pthread_mutex_lock(&philo->philo_mutex);
	since_last_meal = precise_time_ms() - philo->last_meal;
	if (since_last_meal >= philo->program->time_die)
		philo_status = PHILO_DIED;//needs to be in the struct?
	pthread_mutex_unlock(&philo->philo_mutex);//unlock after reading
	return (philo_status);
}
//check every philo
//lock each philo mutex to check # meal
int	meal_control(t_program *data)
{
	int	i;

	if (data->max_meals == MAX_MEALS_DISABLED)
		return(PHILO_ALIVED);
	i = 0;
	while (i < data->total_philo)
	{
		pthread_mutex_lock(&data->philo[i].philo_mutex);
		if (data->philo[i].meal_number < data->max_meals)
		{
			pthread_mutex_unlock(&data->philo[i].philo_mutex);
			return (PHILO_ALIVED);
		}
		pthread_mutex_unlock(&data->philo[i].philo_mutex);
		i++;
	}
	return (PHILO_DIED);
}

//monitores the philo's lifes
//CONNECT WITH THE EAT LOGIC and end condition with the end_flag
//will trigger the programm termination
//check last meal vs time to die
//check if all philos have eaten
//check each philo and max meals of all
//asign the next philo to eat?
//usleep: small initial delay to help ensure all philo threads start
// Loop until the end_flag is set (simulation termination condition met)
// Loop infinitely, breaking out based on end_flag
void	*life_monitor(void *arg)//check why adn how to connect it good with all
{//what if monito fails? or not need to check it?
	int			i;
	t_program	*data;
    int			life_status;

	data = (t_program *) arg;//check if needed usleep
	usleep(1000);
	while (1)
	{
		i = 0;
		life_status = check_end_cond(data->philo);//check if will check all philos and not only philo[0]
		if (life_status == PHILO_DIED)
			return (NULL);//CAN I RETURN AFTER UNLOCK this is to exit monitor thread
		while (i < data->total_philo)// Check if any philosopher has died
		{
			if (check_life(&data->philo[i]) == PHILO_DIED)
			{
				kill_philo(&data->philo[i]);
				return (NULL);//terminates monitor thread
			}
			i++;
		}
		if (data->max_meals != MAX_MEALS_DISABLED && meal_control(data) == PHILO_DIED)
		{
			// pthread_mutex_lock(&data->end_mutex);//check if instead of this i can call the kill function
			// data->end_flag = PHILO_DIED; // Use PHILO_DIED as a general termination signal
			// pthread_mutex_unlock(&data->end_mutex);
			// pthread_mutex_lock(&data->output_mutex);
			// printf("%lld All philosophers have eaten enough meals!\n", precise_time_ms() - data->start_time);//test
			// pthread_mutex_unlock(&data->output_mutex);
			end_program(data, "All philos have eaten all set meals", NULL);//no message asked, can I include some?
			return (NULL);
		}
		usleep(500);//can be modified
	}
	return (NULL);//Should be unreachable if return statements are used inside loop
}
