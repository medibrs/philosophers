/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouras <mbouras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:52:54 by mbouras           #+#    #+#             */
/*   Updated: 2024/12/24 16:06:56 by mbouras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	check_philosopher_death(t_data *data, int *finished_philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->stop_lock);
		if ((current_time() - data->philos[i].last_meal_time) > \
			data->time_to_die)
		{
			print_status(&data->philos[i], "died");
			pthread_mutex_lock(&data->print_lock);
			data->stop_simulation = 1;
			pthread_mutex_unlock(&data->print_lock);
			pthread_mutex_unlock(&data->stop_lock);
			return ;
		}
		if (data->meals_required > 0 && \
			data->philos[i].meals_eaten >= data->meals_required)
			(*finished_philos)++;
		pthread_mutex_unlock(&data->stop_lock);
		i++;
	}
}

void	check_all_philosophers_finished(t_data *data, int finished_philos)
{
	if (data->meals_required > 0 && finished_philos == data->num_philos)
	{
		pthread_mutex_lock(&data->stop_lock);
		pthread_mutex_lock(&data->print_lock);
		data->stop_simulation = 1;
		pthread_mutex_unlock(&data->print_lock);
		pthread_mutex_unlock(&data->stop_lock);
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		finished_philos;

	data = (t_data *)arg;
	while (1)
	{
		finished_philos = 0;
		check_philosopher_death(data, &finished_philos);
		if (data->stop_simulation)
			return (NULL);
		check_all_philosophers_finished(data, finished_philos);
		if (data->stop_simulation)
			return (NULL);
		usleep(2000);
	}
	return (NULL);
}
