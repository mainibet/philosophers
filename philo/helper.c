/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:35:21 by albetanc          #+#    #+#             */
/*   Updated: 2025/06/06 07:40:48 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_array(void **arr, int j)//check if needed
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

int	count_arr_elements(void	**arr)//check if needed in philo
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	malloc_error(void)
{
	print_error_msg("Memory allocation failed");//check if smt to free
	return (ERR_MALLOC_FAIL);
}


