#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_philo {
    int id;
    int meals_eaten;
    long long last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_data *data;
} t_philo;

typedef struct s_data {
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int meals_required;
    int stop_simulation;
    long long start_time;
    pthread_mutex_t print_lock;
    pthread_mutex_t stop_lock;
    pthread_mutex_t *forks;
    t_philo *philos;
} t_data;

long long current_time(void);
void print_status(t_philo *philo, char *message);
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);
void init_simulation(t_data *data, int argc, char **argv);
void cleanup(t_data *data);

#endif
