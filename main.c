#include "philosophers.h"

void init_simulation(t_data *data, int argc, char **argv) {
    int i = 0;

    if (argc != 5 && argc != 6) {
        printf("Usage: ./philosophers num_philos time_to_die time_to_eat time_to_sleep [meals_required]\n");
        exit(1);
    }

    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->meals_required = (argc == 6) ? atoi(argv[5]) : -1;
    data->stop_simulation = 0;
    data->start_time = current_time();

    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    data->philos = malloc(sizeof(t_philo) * data->num_philos);

    while (i < data->num_philos) {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }

    pthread_mutex_init(&data->print_lock, NULL);
    pthread_mutex_init(&data->stop_lock, NULL);

    i = 0;
    while (i < data->num_philos) {
        data->philos[i].id = i + 1;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = current_time();
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        data->philos[i].data = data;
        i++;
    }
}
void cleanup(t_data *data) {
    int i = 0;

    while (i < data->num_philos) {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }

    pthread_mutex_destroy(&data->print_lock);
    pthread_mutex_destroy(&data->stop_lock);
    free(data->forks);
    free(data->philos);
}

int main(int argc, char **argv) {
    t_data data;
    pthread_t *threads;
    pthread_t monitor;
    int i = 0;

    init_simulation(&data, argc, argv);

    threads = malloc(sizeof(pthread_t) * data.num_philos);
    while (i < data.num_philos) {
        pthread_create(&threads[i], NULL, philosopher_routine, &data.philos[i]);
        i++;
    }

    pthread_create(&monitor, NULL, monitor_routine, &data);
    pthread_join(monitor, NULL);

    i = 0;
    while (i < data.num_philos) {
        pthread_join(threads[i], NULL);
        i++;
    }

    cleanup(&data);
    free(threads);
    return 0;
}

