/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/31 13:35:10 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

	int			total_philo;
	int			max_meals; //-1 if not set (optional)
	long long	time_die;
	long long	time_eat;
	long long	time_sleep;
	long long	start_time;//reference to begin simulation
	int			end_flag;//to terminates the simulation
	t_philo		*philo;
	t_fork		*fork;

int	init_mutex(t_program *data)//do i have to set attributes for philo?
{
	if (pthread_mutex_init(&data->output_mutex, NULL) != SUCCESS)//CAN I USE THE MACRO?
		return (ERR_MUTEX);//if success then set conditions to change mutex status to temrinate
	data->end_flag = 0;//just to initialite the flag
	printf ("Mutex initialized correctly");//testing
	return (SUCCESS);
}

int	init_forks(t_program *data)
{

}

int	init_philo(t_program *data)
{

}

int	start_threads(t_program *data)
{

}

int	setup_simulation(int argc, char **argv, t_program *data)
{
	int	status;

	memset(data, 0, sizeof(t_program));
	status = init_mutex(data);
	if (status != SUCCESS)
		return (status);//check and clean-up before return
	status = init_forks(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	status = init_philo(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	status = start_threads(data);
	if (status != SUCCESS)
		return (status);//clean-up before return
	return (SUCCESS);
}

//IN PUT:
//number of philo
//time to die
//time to eat
//time to sleep
//optional: # times each philo should eat
int	main(int argc, char **argv)
{
	if (parsing_args(argc, argv) != SUCCESS)
		return (1);//exit failure macro?
	printf("Ready to continue\n");//test
	if (setup_simulation(argc, argv) != SUCCESS)
		return (1);//exit failure macro?
	// if (start_simulation() != SUCCESS)
	// 	return (1);
    //join philo threads
    // free (parse.arr);//'cause was used to feed the program and clean up all
	return (0);//exit success?
}