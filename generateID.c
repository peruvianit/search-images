/**
 * @file generateID.c
 * @brief Funzioni per la generazione di un univoco
 *
 * @date 25/10/2023
 * @version 1.0
 * @since 25/10/2023
 * @autor Peruvianit
 */

#include <time.h>

#include "generateID.h"

/**
 * @brief Questa funzione genera un numero univoco.
 *
 */
long long generateUniqueID() {
    static unsigned int counter = 0;
    time_t timestamp = time(NULL);

    // Combina il timestamp e il contatore per creare l'identificatore
    long long unique_id = (long long)timestamp * 1000 + counter;
    counter++;

    return unique_id;
}
