/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 06:46:41 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/16 10:00:34 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_args(char *argv, t_arg_parse *parse)
{
	int			status;

	status = CONVERSION_SUCCESS;
	if (check_pos_int(argv, &status) != SUCCESS)
	{
		parse->msg = "Invalid numeric format";
		return (ERR_INVALID_ARGS);
	}
	parse->n = str_tolongl(argv, &status);
	if (status == CONVERSION_ERROR || status == CONVERSION_OVERFLOW)
	{
		parse->msg = "Numeric value too large or invalid";
		return (ERR_INVALID_ARGS);
	}
	if (check_limits(parse->n) != SUCCESS)
	{
		parse->msg = "Integer must be between 1 and INT MAX";
		return (ERR_INVALID_ARGS);
	}
	parse->arr[parse->count++] = (int) parse->n;
	return (SUCCESS);
}

void	init_arg_parse(t_arg_parse *parse, int *arr)
{
	parse->arr = arr;
	parse->count = 0;
	parse->msg = NULL;
	parse->n = 0;
}

//conver str from args to long long
//check if are positive int
// static void	process_get_arg(char *argv, int *arr, size_t *count)
//arr is a tmp for parsing
//ir arr is ok it will be used later in the program
int	process_arg(int argc, char **argv, t_arg_parse *parse)
{
	int		*arr;
	int		j;

	arr = malloc((argc - 1) * sizeof(int));
	if (!arr)
		return (malloc_error());
	init_arg_parse(parse, arr);
	parse->count = 0;
	j = 1;
	while (argv[j])
	{
		if (get_args(argv[j], parse) != SUCCESS)
		{
			print_error_msg(parse->msg);
			free(arr);
			return (ERROR);
		}
		j++;
	}
	return (SUCCESS);
}

//number of argc
//get the numbers from args
int	parsing_args(int argc, char **argv, t_arg_parse *parse)
{
	if (argc < 5)
	{
		print_error_msg("Not enough args to simulate");
		return (ERR_INVALID_ARGS);
	}
	else if (argc > 6)
	{
		print_error_msg("More args than required for simulation");
		return (ERR_INVALID_ARGS);
	}
	else
		if (process_arg(argc, argv, parse) != SUCCESS)
			return (ERROR);
	return (SUCCESS);
}
