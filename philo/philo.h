/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:53 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/31 12:37:30 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// --- MAximum value long long ---
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
# define SUCCESS 0
# define ERROR -1

// --- Specific error codes if needed ---
# define ERR_INVALID_ARGS -2
# define ERR_MALLOC_FAIL  -3
// # define ERR_MUTEX_INIT   -4
// # define ERR_THREAD_CREATE -5

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

// --- ARGUMENT PARSING ---
int		get_args(char *argv, t_arg_parse *parse);
void	init_arg_parse(t_arg_parse *parse, int *arr);
int		process_arg(int argc, char **argv);
int		parsing_args(int argc, char **argv);
long	str_tolongl(char *str, int *status);
int		check_pos_int(char *argv, int *status);
int		check_limits(long long n);

// --- HELPER FUNCIONTS ---
void	print_error_msg(const char *msg);

#endif