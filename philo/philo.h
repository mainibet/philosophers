/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 12:13:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// --- STANDARD HEADERS ---

# include <string.h> //memset CHECK IF USED
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free
# include <unistd.h> //usleep CHECK
# include <sys/time.h> //gettimeofday
# include <pthread.h> //for threads and mutex

// --- MY HEADERS ---

# include "macros.h"
# include "colors.h"

// -----------------------------------------//
//                 STRUCTS                  //
// -----------------------------------------//

typedef struct s_program	t_program;

typedef struct s_arg_parse
{
	int			*arr;//tmp array
	size_t		count;//# of parse args stored so far
	char		*msg;//error msg
	long long	n;//parse number from the current arg
}	t_arg_parse;

typedef struct s_fork
{
	pthread_mutex_t	mutex;//to control access to the fork
	int				fork_mut_status;//flag to check
	int				fork_id;//for login and debug purposes CHECK
}	t_fork;

typedef struct s_philo
{
	int				philo_id;//the number i'll give
	int				meal_number;//how many meals the philo has eaten
	long long		last_meal;//when was the last meal, when they finished eating
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_program		*program;
	pthread_mutex_t	philo_mutex;//Mutex to protect each philo data
	int				mutex_status_phi;//new flag
	pthread_t		thread_id;//used to create thread per philo
}	t_philo;

typedef struct s_program
{
	int				total_philo;
	int				max_meals; //-1 if not set (optional)
	long long		time_die;
	long long		time_eat;
	long long		time_sleep;
	long long		start_time;//reference to begin simulation
	int				end_flag;//to terminates the simulation
	pthread_mutex_t	end_mutex;//mutex to protect end_flag
	int				end_mutex_status;//Flag to track initialization status of end_mutex
	pthread_mutex_t	output_mutex;//for printf
	int				out_mut_status;//flag to check CHANGE THE NAME TO OUTPUT_MUT_STAT
	pthread_t       monitor_thread_id;
	t_philo			*philo;
	t_fork			*fork;
	t_arg_parse		*parse;
}	t_program;

// -----------------------------------------//
//                PROTOTYPES                //
// -----------------------------------------//

// --- ARGUMENT PARSING ---
int			get_args(char *argv, t_arg_parse *parse);
void		init_arg_parse(t_arg_parse *parse, int *arr);
int			process_arg(int argc, char **argv, t_arg_parse *parse);
int			parsing_args(int argc, char **argv, t_arg_parse *parse);
long		str_tolongl(char *str, int *status);
int			check_pos_int(char *argv, int *status);
int			check_limits(long long n);

// --- INITIALIZATION ---
void		init_program(t_program *data);
int			init_cross_mutex(t_program *data);//PENDING CHECK IF ANY FUNCTION IS STATIC
int			init_forks(t_program *data);
int			init_philo(t_program *data);

// --- TIME & THREADS ---
long long	precise_time_ms(void);
void		*life_cycle(void *arg);
int			check_end_cond(t_philo *philo);
void		*life_monitor(void *arg);
int			check_end_cond(t_philo *philo);

// --- ACTIONS & LIFE CYCLE---
void		philo_think(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_eat(t_philo *philo);
// void		kill_philo(t_philo *philo);
void		*life_cycle(void *arg);

// --- SIMULATION CONTROL ---
void		sim_stop(t_program *data);
void		end_program(t_program *data, char *msg, t_philo *dying_philo);

// --- CLEAN-UP & ERROR HANDLING ---
// int			mutex_fork_error(t_program *data, int i);
// int			mutex_fork_error(t_program *data);
void		clean_up_program(t_program *data);

//  --OUTPUT ---
void		print_error_msg(const char *msg);
void		print_status(t_philo *philo, const char *msg);

// --- HELPER FUNCIONTS ---
int			malloc_error(void);//MOVE TO OTHER FILE
void		free_array(void **arr, int j);
int			count_arr_elements(void **arr);

#endif