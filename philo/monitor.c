/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:00:55 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/16 11:48:29 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//if receives program it will only work with philo[0]
//with param philo it can check all
//check time from last meal
static int	check_life(t_philo *philo)
{
	long long	since_last_meal;
	int			philo_status;

	philo_status = PHILO_ALIVED;
	pthread_mutex_lock(&philo->philo_mutex);
	since_last_meal = precise_time_ms() - philo->last_meal;
	if (since_last_meal >= philo->program->time_die)
		philo_status = PHILO_DIED;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (philo_status);
}

//check every philo
//lock each philo mutex to check # meal
static int	meal_control(t_program *data)
{
	int	i;

	if (data->max_meals == MAX_MEALS_DISABLED)
		return (PHILO_ALIVED);
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

static int	check_all_philos(t_program *data)
{
	int	i;

	i = 0;
	while (i < data->total_philo)
	{
		if (check_life(&data->philo[i]) == PHILO_DIED)
		{
			end_program(data, "died", &data->philo[i]);
			return (PHILO_DIED);
		}
		i++;
	}
	return (PHILO_ALIVED);
}

//only set if not already terminated
//kills the philo in the simulation
//handles directly the output of died message
//printf_status handles the status during the simulation
void	end_program(t_program *data, char *msg, t_philo *dying_philo)
{
	long long	current_time;

	pthread_mutex_lock(&data->end_mutex);
	if (data->end_flag == PHILO_ALIVED) 
	{
		data->end_flag = PHILO_DIED;
		data->sim_status = SIM_STOP;
		pthread_mutex_lock(&data->output_mutex);
		current_time = precise_time_ms() - data->start_time;
		if (dying_philo)
			printf("%lld %d %s\n", current_time, dying_philo->philo_id, msg);
		else
			printf("%lld %s\n", current_time, msg);
		pthread_mutex_unlock(&data->output_mutex);
	}
	pthread_mutex_unlock(&data->end_mutex);
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
//when return (NULL) is ending the monitor thread
void	*life_monitor(void *arg)
{
	t_program	*data;
	int			life_status;

	data = (t_program *) arg;
	usleep(1000);
	while (1)
	{
		life_status = check_end_cond(&data->philo[0]);
		if (life_status == PHILO_DIED)
			return (NULL);
		if (check_all_philos(data) == PHILO_DIED)
			return (NULL);
		if (data->max_meals != MAX_MEALS_DISABLED 
			&& meal_control(data) == PHILO_DIED)
		{
			end_program(data, "All philos have eaten all set meals", NULL);
			return (NULL);
		}
		usleep(100);
	}
}
