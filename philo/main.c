/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/30 14:59:56 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
}

int	safe_write(int fd, const void *buf, size_t count)//check if needed
{
	if (write (fd, buf, count) == -1)
		return (ERROR);
	return (SUCCESS);
}

void	free_array(char **arr, int j)//check if needed
{
	if (!arr)
		return ;
	while (j > 0)
	{
		free (arr[j - 1]);
		j--;
	}
	free (arr);
}

char	*malloc_error(void)//check how can be used in philo
{
	// safe_write(STDERR_FILENO, "Error\n", 6);//check if safe write can be use in philo
	// exit(1);//NOT ALLOWED CHANGE IT
	print_error_msg("Memory allocation failed\n");//new
	return (NULL);//check or if maybe should be int -1
}

int	count_arr_elements(char **arr)//check if needed in philo
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	handle_error(char **arr, char *msg)//check and fix to philo
{
	int	i;

	if (arr)
	{
		i = count_arr_elements(arr);
		free_array(arr, i);
	}
    print_error_msg(msg);//check if possible senf msg from other or if is better a "general msg"
	// safe_write(STDERR_FILENO, "Error\n", 6);//check if safe write can be use in philo
	return (ERROR);//NOT ALLOWED CHANGE IT
}

// static char	*process_sign(char *str, int *sign)
// {
// 	*sign = 1;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			*sign = -1;
// 		str++;
// 	}
// 	return (str);
// }

long long	str_tolongl(char *str, int *status)
{
	long long	n;
	// int			sign;
	int			digit;

	n = 0;
	// str = process_sign(str, &sign);
	*status = CONVERSION_SUCCESS;
	while (*str >= '0' && *str <= '9') 
	{
		digit = *str - '0';
		if (n > LLONG_MAX / 10 || (n == LLONG_MAX / 10 && digit > 7)) 
		// {
			*status = CONVERSION_OVERFLOW;
			// if (sign == 1)
			// 	return (LLONG_MAX);
			// else
			// 	return (LLONG_MIN);
		// }
		n = n * 10 + digit;
		str++;
	}
	// return (n * sign);
	return (n);//new always be positive, check
}

//positive int
int	check_pos_int(char *argv, int status)//check if status needed
{
	int	i;

	i = 0;
	if (!argv || argv[0] == '\0')
	{
		status = CONVERSION_ERROR;//check if needed
		return (ERR_INVALID_ARGS);
	}
	if (argv[i] == '+')
		i++;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
		{
			status = CONVERSION_ERROR;//check if needed
			return (ERR_INVALID_ARGS);
		}
		i++;
	}
	status = CONVERSION_SUCCESS;
	return (SUCCESS);
}

int	check_limits(long long n)
{
	if (n < -2147483648LL || n > 2147483647LL)
		return (ERR_INVALID_ARGS);
	return (SUCCESS);
}

int	get_args(char *argv, int *arr, size_t *count)
{
	long long	n;
	int			status;

	status = CONVERSION_SUCCESS;
	if (!check_pos_int(argv, status))
	{
		free(arr);
		handle_error(NULL, "not positive integer\n");//check
	}
	n = str_tolongl(argv, &status);
	if (status == CONVERSION_ERROR || status == CONVERSION_OVERFLOW)
	{
		free (arr);
		handle_error(NULL, "Invalid argument format\n");//check

	}
	if (!check_limits(n))
	{
		free(arr);
		handle_error(NULL, "Arg value too long\n");//check
	}
	arr[*(count)++] = (int) n;
    return (n);//check
}

//conver str from args to long long
//check if are positive int
// static void	process_get_arg(char *argv, int *arr, size_t *count)

int	process_arg(int argc, char *argv)
{
	int				*arr;//tmp arr to checks
	int		j; //index to loop in args
    size_t  count;

	arr = malloc((argc - 1) * sizeof(int));
	if (!arr)
		malloc_error();//check if this return NULL if is ok or should be a int -1?
	count = 0;
	j = 1;//skiping the program's name
	while (argv[j])
	{
		get_args(argv, arr, &count);
			// process_get_arg(argv[j], arr, &count);//check if count eneded in philo
		j++;
	}
    return (SUCCESS);
}

//number of argc
//get the numbers from args
int	parsing_args(int argc, char *argv)
{
	if (argc < 5)
	{
		printf ("Not enough args to simulate\n");//handle error for printf?
		return (ERR_INVALID_ARGS);
	}
	else if (argc > 6)
	{
		printf ("More args than required for simulation\n"); //handle error for printf?
		return (ERR_INVALID_ARGS);
	}
	else
		process_arg(argc, argv);
	return (SUCCESS);//exit success? set macros to exit succeess and failure
}

//IN PUT:
//number of philo
//time to die
//time to eat
//time to sleep
//optional: # times each philo should eat
int	main(int argc, char **argv)
{
	parsing_args(argc, *argv);//inside handling error if not see if possible end the program if not send a flag
	return (0);
}