#include "philosophers.h"

void *monitor_routine(void *arg) {
    t_data *data = (t_data *)arg;
    int i;
    int finished_philos;

    while (1) {
        i = 0;
        finished_philos = 0;

        while (i < data->num_philos) {
            pthread_mutex_lock(&data->stop_lock);

            // Vérifier si un philosophe est mort
            if ((current_time() - data->philos[i].last_meal_time) > data->time_to_die) {
                print_status(&data->philos[i], "died");
                data->stop_simulation = 1;
                pthread_mutex_unlock(&data->stop_lock);
                return NULL;
            }

            // Compter les philosophes ayant atteint meals_required
            if (data->meals_required > 0 && data->philos[i].meals_eaten >= data->meals_required)
                finished_philos++;

            pthread_mutex_unlock(&data->stop_lock);
            i++;
        }

        // Arrêter si tous les philosophes ont assez mangé
        if (data->meals_required > 0 && finished_philos == data->num_philos) {
            pthread_mutex_lock(&data->stop_lock);
            data->stop_simulation = 1;
            pthread_mutex_unlock(&data->stop_lock);
            return NULL;
        }

        usleep(2000); // Pause pour réduire la charge CPU
    }
    return NULL;
}
