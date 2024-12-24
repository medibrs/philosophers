/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouras <mbouras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:52:57 by mbouras           #+#    #+#             */
/*   Updated: 2024/12/24 15:49:22 by mbouras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->stop_simulation)
	{
		printf("%lld %d %s\n", current_time() - philo->data->start_time, \
				philo->id, message);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	number;

	i = 0;
	sign = 1;
	number = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + (str[i] - '0');
		i++;
	}
	return (number * sign);
}
