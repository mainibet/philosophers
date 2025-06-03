/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:35:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/03 14:53:06 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//pending destroy mytex if an error occure after init mutex
void	print_error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
}

int	malloc_error(void)
{
	print_error_msg("Memory allocation failed");//check if smt to free
	return (ERR_MALLOC_FAIL);
}

// int	safe_write(int fd, const void *buf, size_t count)//check if needed
// {
// 	if (write (fd, buf, count) == -1)
// 		return (ERROR);
// 	return (SUCCESS);
// }

// void	free_array(int **arr, int j)//check if needed
// {
// 	if (!arr)
// 		return ;
// 	while (j > 0)
// 	{
// 		free (arr[j - 1]);
// 		j--;
// 	}
// 	free (arr);
// }

// int	count_arr_elements(int	**arr)//check if needed in philo
// {
// 	int	i;

// 	i = 0;
// 	if (!arr)
// 		return (0);
// 	while (arr[i])
// 		i++;
// 	return (i);
// }

// int	clean_up(char **arr)//check and fix to philo
// {
// 	int	i;

// 	if (arr)
// 	{
// 		i = count_arr_elements(arr);
// 		free_array(arr, i);
// 	}
// 	return (ERROR);
// }
