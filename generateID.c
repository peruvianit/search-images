#include <time.h>

#include "generateID.h"

long long generateUniqueID() {
    static unsigned int counter = 0;
    time_t timestamp = time(NULL);

    // Combina il timestamp e il contatore per creare l'identificatore
    long long unique_id = (long long)timestamp * 1000 + counter;
    counter++;

    return unique_id;
}
