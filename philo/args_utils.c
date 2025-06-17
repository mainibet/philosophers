/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 06:47:11 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/17 11:41:32 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	str_tolongl(char *str, int *status)
{
	long long	n;
	int			digit;

	n = 0;
	*status = CONVERSION_SUCCESS;
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9') 
	{
		digit = *str - '0';
		if (n > LLONG_MAX / 10 || (n == LLONG_MAX / 10 && digit > 7)) 
			*status = CONVERSION_OVERFLOW;
		n = n * 10 + digit;
		str++;
	}
	return (n);
}

static int	check_digits(const char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (ERR_INVALID_ARGS);
		str++;
	}
	return (SUCCESS);
}

int	check_pos_int(char *argv, int *status)
{
	int	i;

	i = 0;
	if (!argv || argv[0] == '\0')
	{
		*status = CONVERSION_ERROR;
		return (ERR_INVALID_ARGS);
	}
	if (argv[i] == '+')
		i++;
	if (argv[i] == '\0')
	{
		*status = CONVERSION_ERROR;
		return (ERR_INVALID_ARGS);
	}
	if (check_digits (argv + i) != SUCCESS)
	{
		*status = CONVERSION_ERROR;
		return (ERR_INVALID_ARGS);
	}
	*status = CONVERSION_SUCCESS;
	return (SUCCESS);
}

int	check_limits(long long n)
{
	if (n < 1 || n > 2147483647LL)
		return (ERR_INVALID_ARGS);
	return (SUCCESS);
}
