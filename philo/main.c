/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/30 13:01:27 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	safe_write(STDERR_FILENO, "Error\n", 6);//check if safe write can be use in philo
	exit(1);//NOT ALLOWED CHANGE IT
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

void	handle_error(char **arr, t_node *stack)//check and fix to philo
{
	int	i;

	if (arr)
	{
		i = count_arr_elements(arr);
		free_array(arr, i);
	}
	if (stack)//not needed
		free_stack(stack);//not needed
	safe_write(STDERR_FILENO, "Error\n", 6);//check if safe write can be use in philo
	exit(1);//NOT ALLOWED CHANGE IT
}

//positive int
int	check_pos_int(char *argv)
{
	int	i;

	i = 0;
	if (!argv || argv[0] == '\0')
		return (0);
	if (argv[i] == '+')
		i++;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_limits(long long n)
{
	if (n < -2147483648LL || n > 2147483647LL)
		return (0);
	return (1);
}

//conver str from args to long long
//check if are positive int
static void	process_get_arg(char *argv, int *arr, size_t *count)
{
	long long		n;
	int				status;

	status = CONVERSION_SUCCESS;
	if (!check_int(argv))
	{
		free(arr);
		handle_error(NULL, NULL);
	}
	n = str_tolongl(argv, &status);
	if (status == CONVERSION_ERROR || status == CONVERSION_OVERFLOW)
	{
		free (arr);
		handle_error(NULL, NULL);
	}
	if (!check_limits(n))
	{
		free(arr);
		handle_error(NULL, NULL);
	}
	arr[(*count)++] = (int) n;
}
//number of argc
//get the numbers from args
int	parsing_args(int argc, char *argv)
{
	int		*arr;//tmp arr to checks
	int		j; //index to loop in args
	size_t	count;//to count the valid args passed (int) CHECK IF NEEDED

	if (argc < 5)
	{
		printf ("Not enough args to simulate\n");//handle error for printf?
		return (0);
	}
	else if (argc > 6)
	{
		printf ("More args than required for simulation\n"); //handle error for printf?
		return (0);
	}
	else
	{
		arr = malloc((argc - 1) * sizeof(int));
		if (!arr)
			malloc_error();
		j = 1;//skiping the program's name
		count = 0;
		while (argv[j])
		{
			process_get_arg(argv[j], arr, &count);//check if count eneded in philo
			j++;
		}
	}
	//free if smt is needed
	return (0);
}

//IN PUT:
//number of philo
//time to die
//time to eat
//time to sleep
//optional: # times each philo should eat
int	main(int argc, char **argv)
{
	parsing_args(argc, argv);//inside handling error if not see if possible end the program if not send a flag
	return (0);
}