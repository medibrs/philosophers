#include "philosophers.h"

long long current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void print_status(t_philo *philo, char *message) {
    pthread_mutex_lock(&philo->data->print_lock);
    if (!philo->data->stop_simulation) {
        printf("%lld %d %s\n", current_time() - philo->data->start_time, philo->id, message);
    }
    pthread_mutex_unlock(&philo->data->print_lock);
}

void ft_noob()
{
    
}