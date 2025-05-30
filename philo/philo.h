/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/30 12:44:51 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H


# ifndef CONVERSION_SUCCESS
#  define CONVERSION_SUCCESS 0
# endif

# ifndef CONVERSION_ERROR
#  define CONVERSION_ERROR 1
# endif

# ifndef CONVERSION_OVERFLOW
#  define CONVERSION_OVERFLOW 2
# endif

# include <string.h> //memset CHECK IF USED
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free
# include <unistd.h> //usleep CHECK
# include <sys/time.h> //gettimeofday
# include <pthread.h> //for threads and mutex

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//

typedef struct fork
{
	pthread_mutex_t	mutex;//to control access to the fork
	int				fork_id;//for login and debug purposes CHECK
}	t_fork;

typedef struct philo
{
	int			philo_id;//the number i'll give
	int			meal_number;//how many meals the philo has eaten
	long long	last_meal;//when was the last meal, when they finished eating
	t_fork		*left_fork;
	t_fork		*right_fork;
	t_program	*program;
	pthread_t	thread_id;//id when thread created
}	t_philo;

typedef struct program
{
	int			total_philo;
	int			max_meals; //-1 if not set (optional)
	long long	time_die;
	long long	time_eat;
	long long	time_sleep;
	long long	start_time;//reference to begin simulation
	int			end_flag;//to terminates the simulation
	t_philo		*philo;
	t_fork		*fork;
}	t_program;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//


#endif