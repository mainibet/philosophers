/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:47:19 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/04 10:29:01 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//HELPER FUNCTION
//to print all the msg in order
void	print_status(t_philo *philo, const char *msg)//check if needs to handled any error
{
	long long	current_time;

	current_time = precise_time_ms() - philo->program->start_time;
	pthread_mutex_lock(&philo->program->output_mutex);//after this includ checks of termination conditions with a flag
    //also the specific printing to finish lock and unlock
	printf("%lld %d %s\n", current_time, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->program->output_mutex);
}

void	print_format(t_philo *philo, const char *msg)
{
	long long	current_time;

	current_time = precise_time_ms() - philo->program->start_time;
	printf("%lld %d %s\n", current_time, philo->philo_id, msg);
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
	long long			all_sec;
	long long			rem_microsec;
	long long			total_milisec;

	gettimeofday(&tv, NULL); 
	all_sec = tv.tv_sec * 1000LL;
	rem_microsec = tv.tv_usec / 1000;
	total_milisec = all_sec + rem_microsec;
	return (total_milisec);
}

//this should be after sleep
//usleep is in microsecs
void	philo_think(t_philo *philo)
{
	long long		current_time;
	long long		think_time;

	current_time = precise_time_ms () - philo->program->start_time;//check why atart time
//set if cond to stop if terminates condition reach
	print_status(philo, "is thinking");
	// usleep(100);
    // Calculate think time: use time between eat and die to prevent death
	think_time = (philo->program->time_die - philo->program->time_eat - philo->program->time_sleep) / 2;//check
	if (think_time > 0)
		usleep(think_time * 1000);
}
//this should be after release forks
//time_t is in <sys/time.h> for time values
void	philo_sleep(t_philo *philo)
{
	long long		wake_up;
	// time_t		current_time;

	wake_up = precise_time_ms() + philo->program->time_sleep;//check
	// current_time = precise_time_ms () - philo->program->start_time;//check may be can be defined in the life_cycle
	print_status(philo, "is sleeping");
	while (precise_time_ms() < wake_up)
	{
		//condition to stop in termination cond
		usleep(100);
	}
}

//take first the one with the lower ID
void	take_forks(t_philo *philo)
{
	t_fork	*first_fork;//check type
	t_fork	*second_fork;//check type

	//set cond to avoid attempt to take forks
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	// if (pthread_mutex_lock(&first_fork->mutex) != 0) //handling error already lock
	// 	return 	;
	// print_status(philo, msg);
	// if (pthread_mutex_lock(&second_fork->mutex) != 0)//handling error already lock
	// {
	// 	pthread_mutex_unlock(&first_fork->mutex);  // Release first fork if can't get second
	// 	return ;
	// }
	pthread_mutex_lock(&first_fork->mutex);
	print_status(philo, "has taken a fork");
    //check if simulation hasn't stop
	pthread_mutex_lock(&second_fork->mutex);
	print_status(philo, "has taken a fork");
}
//only to mutex unlock
void	release_forks(t_philo *philo)//no message check and check if order is relevant
{
	t_fork	*first_fork;
	t_fork	*second_fork;

	// same order logic as take_forks
	if (philo->left_fork->fork_id < philo->right_fork->fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
    // Release in reverse order of acquisition
	pthread_mutex_unlock(&second_fork->mutex);
	pthread_mutex_unlock(&first_fork->mutex);
}

void	philo_eat(t_philo *philo)
{
	long long	eat_end_time;

	take_forks(philo);
    //check simulation is running
	philo->last_meal = precise_time_ms();
	philo->meal_number++;//increment meal count applies for optional arg
	print_status(philo, "is eating");
	eat_end_time = precise_time_ms () + philo->program->time_eat;
	while (precise_time_ms() < eat_end_time)
	{
		//set if condition if simulation stops
		usleep(100);//test
	}
	release_forks(philo);//after eating
}

void	philo_routine_odd(t_philo *philo)
{
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
}

void	philo_routine_even(t_philo *philo)
{
	philo_think(philo);
	philo_eat(philo);
	philo_sleep(philo);
}

//destroy mutex: output, fork
//free: array of philo
//free:array of forks
//free: array of parse
void	clean_up(t_program *data)//include philo_mutex
{
	int	i;

	i = 0;
	if (data->mutex_status == MUTEX_INIT)
		pthread_mutex_destroy(&data->output_mutex);//need to includ error check of destroy?
	if (data->fork != NULL)
	{
		i = 0;
		while (i < data->total_philo)
		{
			if (data->fork[i].mutex_status == MUTEX_INIT)
				pthread_mutex_destroy(&data->fork[i].mutex);//need to includ error check of destroy?
			i++;
		}
		i = count_arr_elements((void **)data->fork);
		free_array((void **)data->fork, i);
	}
	if (data->philo != NULL)
		free_array((void **)data->philo, data->total_philo);
	if (data->parse != NULL)
	{
		i = count_arr_elements((void **)data->parse);
		free_array((void **)data->parse, i);
	}
}

//sim wait until all threads joined and clean-up
void	sim_stop(t_program *data)//bolean?
{
	int    i;

	i = 0;
	while (i < data->philo->program->total_philo)
	{
		pthread_join(data->philo[i].thread_id, NULL);//check
		i++;
	}
    if (data->total_philo > 1)
        pthread_join(data->end_flag, NULL);//check
    //call there the clean-up function
}

//takes the dying philo as argument
void	kill_philo(t_philo *philo)//check
{
	pthread_mutex_lock(&philo->program->end_mutex);
	philo->program->end_flag = PHILO_DIED;
	pthread_mutex_unlock(&philo->program->end_mutex);
	print_status(philo, "died");//after this call the clean_up
	sim_stop(philo->program);
}

//flag to know if a philo has died or if they reach the amount of eats
//A philo will die if doesn't eat in the max time set
//it terminates if the number of eats is reached when it was included
void	end_condition(t_program *data)//this will be updated by monitor
{
	int	status;//flag or status?
	int	current_meals;//check

	status = PHILO_ALIVED;//connect this with monitor?
	current_meals = data->philo->meal_number;
	if (data->max_meals != MAX_MEALS_DISABLED)
	{
		if (status == PHILO_DIED || current_meals == data->max_meals)
		{
			kill_philo(data->philo);
			return ;
		}
	}
	if (status == PHILO_DIED)
	{
		kill_philo(data->philo);
		return ;
	}
	return ;//check if needed
}

int check_life(t_program *data)
{
	long long	since_last_meal;
	int			philo_status;

	philo_status = PHILO_ALIVED;
	pthread_mutex_lock(&data->philo->philo_mutex);//what's this mutex?
	since_last_meal = precise_time_ms() - data->philo->last_meal;
	if (since_last_meal >= data->time_die)
		philo_status = PHILO_DIED;//needs to be in the struct?
	pthread_mutex_lock(&data->philo->philo_mutex);
	return (philo_status);
}

int	meal_control(t_program *data)
{
	int	i;

	if (data->max_meals == MAX_MEALS_DISABLED)
		return(FALSE);//CHECK MACRO
	i = 0;
	while (i < data->total_philo)//lock each philo mutex to check # meal
	{
		pthread_mutex_lock(&data->philo[i].philo_mutex);
		return (FALSE);
	}
	pthread_mutex_unlock(&data->philo[i].philo_mutex);
	i++;
	return (TRUE);
}

//monitores the philo's lifes
//CONNECT WITH THE EAT LOGIC and end condition with the end_flag
//will trigger the programm termination
//check last meal vs time to die
//check if all philos have eaten
//check each philo and max meals of all
//asign the next philo to eat?
void	life_monitor(void *arg)//check why
{
	int			i;
	t_program	*data;

	i = 0;
	data = (t_program *)arg;//check if needed usleep
	usleep(1000);//check
	pthread_mutex_lock(&data->philo[i].philo_mutex);
	while (data->end_flag == PHILO_ALIVED)//check if needed mutex before
	{
		if (check_life(data) == PHILO_DIED)
			end_condition(data);//check
		i++;
	}
}

//each will be a status
//eating, thinking, and sleeping.
//prototype given by pthread_create
//void *(*start_routine)(void *), void *arg
//thread logic
//odd: eat-sleep-think
//even: think-eat-sleep
void	*life_cycle(void *arg)
{
	t_philo	*philo;//check if can be use this struct
	int		i;
	int		wait_time;

	philo = (t_philo *)arg;
	if (philo->philo_id % 2 == 0)//even small delay CHECK
    // Calculate dynamic wait time based on position and eating time
	{
		wait_time = philo->program->time_eat;//check
		usleep(wait_time * 1000);
	}
	// current_time = precise_time_ms() - philo->program->start_time;
	//pending to set a while loop with a cond to stop the loop term condition
	i = 0;
	// while (i < philo->program->total_philo)
    while (1)
	{
		if (philo->philo_id % 2 != 0)//odd
			philo_routine_odd(philo);
		else
			philo_routine_even(philo);
		i++;//afer include a check for the end_flag
	}
	return (NULL);
}
