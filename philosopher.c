#include "philosophers.h"

void smart_fork_pickup(t_philo *philo) {
    if (philo->id % 2 == 0) {
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a  fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    } else {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a  fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a  fork");
    }
}

void *philosopher_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 == 0) {
        usleep(10000); // Pause initiale pour éviter un conflit immédiat
    }

    while (1) {
        // Vérifier si la simulation doit s'arrêter
        pthread_mutex_lock(&philo->data->stop_lock);
        if (philo->data->stop_simulation) {
            pthread_mutex_unlock(&philo->data->stop_lock);
            break;
        }
        pthread_mutex_unlock(&philo->data->stop_lock);

        // Prendre les fourchettes
        smart_fork_pickup(philo);

        // Manger
        pthread_mutex_lock(&philo->data->stop_lock);
        print_status(philo, "is eating");
        philo->last_meal_time = current_time();
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->data->stop_lock);
        usleep(philo->data->time_to_eat * 1000);

        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        // Dormir
        print_status(philo, "is sleeping");
        usleep(philo->data->time_to_sleep * 1000);

        // Penser
        print_status(philo, "is thinking");
    }
    return NULL;
}
