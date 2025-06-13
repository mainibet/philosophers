/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 07:26:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/13 17:12:41 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

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

// # ifndef ERR_THREAD_CREAT//pending to use it
// #  define ERR_THREAD_CREATE -5
// # endif

// --- Bolean values ---
# ifndef FALSE//CHECK IF IS CORRECT AND NEEDED or if is in any lib
#  define FALSE 0
# endif

# ifndef TRUE
#  define TRUE 1
# endif

// --- Configuration values ---
# ifndef MAX_MEALS_DISABLED
#  define MAX_MEALS_DISABLED -1
# endif

# ifndef MUTEX_NO_INIT
#  define MUTEX_NO_INIT 0
# endif

# ifndef MUTEX_INIT
#  define MUTEX_INIT 1
# endif

# ifndef PHILO_ALIVED
#  define PHILO_ALIVED 0
# endif

# ifndef PHILO_DIED
#  define PHILO_DIED 1
# endif

# ifndef SIM_RUNNING
#  define SIM_RUNNING 1
# endif

# ifndef SIM_STOP
#  define SIM_STOP 0
# endif

# ifndef TAKEN_FORK
#  define TAKEN_FORK 1
# endif

# ifndef NO_TAKEN
#  define NO_TAKEN 0
# endif

#endif
