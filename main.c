/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouras <mbouras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:53:11 by mbouras           #+#    #+#             */
/*   Updated: 2024/12/24 15:47:34 by mbouras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	validate_arguments(int argc, char **argv, int i, int j)
{
	if (argc != 5 && argc != 6)
	{
		printf("wrong args \n");
		exit(1);
	}
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]) && \
				!(j == 0 && (argv[i][j] == '-' || argv[i][j] == '+')))
			{
				printf("Error: Arguments must be positive integers\n");
				exit(1);
			}
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Arguments must be positive integers\n");
			exit(1);
		}
		i++;
	}
}

void	initialize_data(t_data *data, int i)
{
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->stop_lock, NULL);
	i = -1;
	while (++i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
	}
}

void	init_simulation(t_data *data, int argc, char **argv)
{
	validate_arguments(argc, argv, 1, 0);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	data->stop_simulation = 0;
	data->start_time = current_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	initialize_data(data, 0);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->stop_lock);
	free(data->forks);
	free(data->philos);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	i = -1;
	init_simulation(&data, argc, argv);
	threads = malloc(sizeof(pthread_t) * data.num_philos);
	while (++i < data.num_philos)
		if (pthread_create(&threads[i], NULL, \
			philosopher_routine, &data.philos[i]) != 0)
			return (2);
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
		return (2);
	if (pthread_join(monitor, NULL) != 0)
		return (1);
	i = 0;
	while (i < data.num_philos)
		if (pthread_join(threads[i++], NULL) != 0)
			return (1);
	cleanup(&data);
	free(threads);
	return (0);
}
