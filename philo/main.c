/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:39:37 by albetanc          #+#    #+#             */
/*   Updated: 2025/05/31 12:38:03 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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