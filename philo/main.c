/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/31 11:13:30 by albetanc         ###   ########.fr       */
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

// int	malloc_error(void)
// {
// 	print_error_msg("Memory allocation failed\n");//new
// 	return (ERR_MALLOC_FAIL);//check or if maybe should be int -1
// }

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

int	clean_up(char **arr)//check and fix to philo
{
	int	i;

	if (arr)
	{
		i = count_arr_elements(arr);
		free_array(arr, i);
	}
    // print_error_msg(msg);//check if possible senf msg from other or if is better a "general msg"
	// safe_write(STDERR_FILENO, "Error\n", 6);//check if safe write can be use in philo
	return (ERROR);//NOT ALLOWED CHANGE IT
}

long long	str_tolongl(char *str, int *status)
{
	long long	n;
	int			digit;

	n = 0;
	*status = CONVERSION_SUCCESS;
	while (*str >= '0' && *str <= '9') 
	{
		digit = *str - '0';
		if (n > LLONG_MAX / 10 || (n == LLONG_MAX / 10 && digit > 7)) 
			*status = CONVERSION_OVERFLOW;
		n = n * 10 + digit;
		str++;
	}
	return (n);//new always be positive, check
}

//positive int
int	check_pos_int(char *argv, int *status)//check if status needed
{
	int	i;

	i = 0;
	if (!argv || argv[0] == '\0')
	{
		*status = CONVERSION_ERROR;//check if needed
		return (ERR_INVALID_ARGS);
	}
	if (argv[i] == '+')
		i++;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
		{
			*status = CONVERSION_ERROR;//check if needed
			return (ERR_INVALID_ARGS);
		}
		i++;
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

// int	get_args(char *argv, int *arr, size_t *count, char **msg)
int get_args(char *argv, t_arg_parse *parse)
{
	// long long	n;
	int			status;

	status = CONVERSION_SUCCESS;
	if (check_pos_int(argv, &status) != SUCCESS)
	{
		// free(arr);
		parse->msg = "Not positive integer";//check
		return (ERR_INVALID_ARGS);
	}
	parse->n = str_tolongl(argv, &status);
	if (status == CONVERSION_ERROR || status == CONVERSION_OVERFLOW)
	{
		// free (arr);
		parse->msg = "Invalid argument format";
		return (ERR_INVALID_ARGS);
	}
	if (check_limits(parse->n) != SUCCESS)
	{
		// free(arr);
		parse->msg = "Integer beyond MAX INT or below 1";
		return (ERR_INVALID_ARGS);
	}
	parse->arr[parse->count++] = (int) parse->n;
    // return (n);//check
    return (SUCCESS);
}

void	init_arg_parse(t_arg_parse *parse, int *arr)
{
	parse->arr = arr;//check
	parse->count = 0;
	parse->msg = NULL;
	parse->n = 0;
}

//conver str from args to long long
//check if are positive int
// static void	process_get_arg(char *argv, int *arr, size_t *count)

int	process_arg(int argc, char **argv)
{
	int		*arr;//tmp arr to checks
	int		j; //index to loop in args

	arr = malloc((argc - 1) * sizeof(int));
	if (!arr)
	{
		print_error_msg("Memory allocation failed");//check if smt to free
		return (ERROR);
	}
	t_arg_parse parse;
	init_arg_parse(&parse, arr);
	parse.count = 0;
	j = 1;//skiping the program's name
	while (argv[j])
	{
		if (get_args(argv[j], &parse) != SUCCESS)
		{
			print_error_msg(parse.msg);
			free(arr);//new
			return (ERROR);
		}
		j++;
	}
	free(arr);//check NEW
	return (SUCCESS);
}

//number of argc
//get the numbers from args
int	parsing_args(int argc, char **argv)
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
		if (process_arg(argc, argv) != SUCCESS)
		// {
            // print_error_msg("Invalid input arguments");
            return (ERROR);
        // }
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
	if (parsing_args(argc, argv) != SUCCESS)//inside handling error if not see if possible end the program if not send a flag
		return(1);
    printf("Ready to continue\n");//test
	return (0);
}