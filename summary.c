/**
 * @file summary.c
 * @brief Funzioni per la gestione di un summary quando finisce l'esecuzione
 * del programma.
 *
 * @date 25/10/2023
 * @version 1.0
 * @since 25/10/2023
 * @autor Peruvianit
 */

#include <stdio.h>

#include "summary.h"

/**
 * @brief Questa funzione che stampa il summary dell'applicazione.
 *
 * @param search_files struttura che contiene l'informazione per essere
 *        visualizzata alla fine del programma.
 * @return
 */
void stampaSummary(struct Search_files *search_files){
    puts("\n###########################################");
    printf("SUMMARY\n");
    printf("-------------------------------------------\n");
    printf("Directory Reads \t: %d\n", search_files->directoryReads);
    printf("Files Reads \t\t: %d\n", search_files->filesReads);
    printf("Images Reads \t\t: %d\n", search_files->imagesReads);
    printf("Images Copied \t\t: %d\n", search_files->imagesCopied);
    puts("###########################################");
}
