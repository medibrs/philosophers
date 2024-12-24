/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouras <mbouras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:50:08 by mbouras           #+#    #+#             */
/*   Updated: 2024/12/24 15:50:09 by mbouras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	single_philosopher(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		if (philo->data->stop_simulation)
		{
			pthread_mutex_unlock(&philo->data->stop_lock);
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		pthread_mutex_unlock(&philo->data->stop_lock);
		usleep(1000);
	}
}

void	smart_fork_pickup(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		single_philosopher(philo);
		return ;
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		usleep(1000);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

void	perform_actions(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	print_status(philo, "is eating");
	philo->last_meal_time = current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->stop_lock);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	print_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	print_status(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		if (philo->data->stop_simulation)
		{
			pthread_mutex_unlock(&philo->data->stop_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->stop_lock);
		smart_fork_pickup(philo);
		perform_actions(philo);
	}
	return (NULL);
}
