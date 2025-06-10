/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 08:00:55 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/10 13:48:28 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//if receives program it will only work with philo[0]
//with param philo it can check all
//check time from last meal
static int check_life(t_philo *philo)
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
static int	meal_control(t_program *data)
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
		life_status = check_end_cond(&data->philo[0]);//check if will check all philos and not only philo[0]
		if (life_status == PHILO_DIED)
			return (NULL);//CAN I RETURN AFTER UNLOCK this is to exit monitor thread
		while (i < data->total_philo)// Check if any philosopher has died
		{
			if (check_life(&data->philo[i]) == PHILO_DIED)
			{
				end_program(data, "died", &data->philo[i]);
				return (NULL);//terminates monitor thread
			}
			i++;
		}
		if (data->max_meals != MAX_MEALS_DISABLED && meal_control(data) == PHILO_DIED)
		{
			end_program(data, "All philos have eaten all set meals", NULL);//no message asked, can I include some?
			return (NULL);
		}
		usleep(100);//can be modified 1000
	}
	return (NULL);//Should be unreachable if return statements are used inside loop
}