/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/03 12:16:34 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// --- Maximum value long long ---
# ifndef LLONG_MAX
#  define LLONG_MAX 9223372036854775807LL
# endif

// --- Str long long conversion status ---
# ifndef CONVERSION_SUCCESS
#  define CONVERSION_SUCCESS 0
# endif

# ifndef CONVERSION_ERROR
#  define CONVERSION_ERROR 1
# endif

# ifndef CONVERSION_OVERFLOW
#  define CONVERSION_OVERFLOW 2
# endif

// --- Return Status Codes ---
# ifndef SUCCESS
#  define SUCCESS 0
# endif

# ifndef ERROR
#  define ERROR -1
# endif

// --- Specific error codes if needed ---
# ifndef ERR_INVALID_ARGS
#  define ERR_INVALID_ARGS -2
# endif
# ifndef ERR_MALLOC_FAIL
#  define ERR_MALLOC_FAIL -3
# endif

# ifndef ERR_MUTEX
#  define ERR_MUTEX -4
# endif
//# ifndef ERR_THREAD_CREAT
// # define ERR_THREAD_CREATE -5
//# endif

// --- Configuration values ---
# ifndef MAX_MEALS_DISABLED
#  define MAX_MEALS_DISABLED -1
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
	int				fork_id;//for login and debug purposes CHECK
}	t_fork;

typedef struct s_philo
{
	int			philo_id;//the number i'll give
	int			meal_number;//how many meals the philo has eaten
	long long	last_meal;//when was the last meal, when they finished eating
	t_fork		*left_fork;
	t_fork		*right_fork;
	t_program	*program;
	pthread_t	thread_id;//id when thread created
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
	pthread_mutex_t	output_mutex;//for printf
	pthread_mutex_t	sim_over;//shared state
	t_philo			*philo;
	t_fork			*fork;
    t_arg_parse		*parse;//new
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
void		fill_each_philo(t_program *data, int philo_id);
int			init_philo(t_program *data);

// --- TIME ---
// long long	precise_time_ms(void);
time_t		precise_time_ms(void);
void		*life_cycle(void *arg);

// --- CLEAN-UP & ERROR HANDLING
int			mutex_fork_error(t_program *data, int i);

// --- HELPER FUNCIONTS ---
void		print_error_msg(const char *msg);//MOVE TO OTHER FILE
int			malloc_error(void);//MOVE TO OTHER FILE

#endif