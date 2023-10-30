/**
 * @file main.c
 * @brief Programma che cerca le immagine nel file system a partire di
 *        un path di origine.
 *
 * @date 25/10/2023
 * @version 1.0
 * @since 25/10/2023
 * @autor Peruvianit
 */

#include <stdio.h>
#include <dirent.h>

#include "fileUtils.h"
#include "summary.h"
#include "generateID.h"

#define DEBUG 0
#define COPIA_IMMAGINE 1

static const char *PATH_ORIGIN = "C://Temp//images";
static const char *PATH_DESTINATION = "C://Temp//Foto";
static const char *EXCLUDE_EXTENSION_IMAGES_FORMATS[] = {".JPEG", ".JPG", ".PNG", ".BMP", ".GIF"};
static const int EXCLUDE_IMAGES = 0;

static void leggiFileRicorsivamente(const char*, struct Search_files*);

static bool exludeFile(const char *[], const char*);

int main() {

    struct Search_files search_files = {0};

#if DEBUG
    puts("Inizio applicazione");
    printf("Inzio di lettura dalla cartella %s\n", PATH_ORIGIN);
#endif // DEBUG

    leggiFileRicorsivamente(PATH_ORIGIN, &search_files);

    stampaSummary(&search_files);
#if DEBUG
    puts("Fine applicazione");
#endif // DEBUG
    getchar();
    return 0;
}

static void leggiFileRicorsivamente(const char *percorso, struct Search_files *search_files) {
    if (exists(percorso)) {
        struct dirent *entry;
        DIR *dp = opendir(percorso);

        if (!dp) {
            printf("Errore nell'apertura della directory -> %s \n", percorso);
            return;
        }

        while ((entry = readdir(dp)) != NULL) {
            char path_file[256];
            strcpy(path_file, percorso);
            strcat(path_file, "//");
            strcat(path_file, entry->d_name);

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;  // Ignora le directory speciali "." e ".."
            }

            if (EXCLUDE_IMAGES && exludeFile(EXCLUDE_EXTENSION_IMAGES_FORMATS, entry->d_name)){
                continue;
            }
#if DEBUG
            printf("[%s]\n", path_file);
#endif
            if (isDirectory(path_file)) {
                search_files->directoryReads++;
                leggiFileRicorsivamente(path_file, search_files);

            } else {
                long long unique_id = generateUniqueID();

                char str_unique_id[50]; // Dichiaro una stringa di destinazione

                // Converto il valore long long in una stringa e lo salvo in 'str'
                snprintf(str_unique_id, sizeof(str_unique_id), "%lld", unique_id);

                //printf("%s\n", str_unique_id);

                search_files->filesReads++;
                int immagine = isImage(path_file);

                if (immagine >= 0){
                    search_files->imagesReads++;

                    char nuovoFile[256];
                    strcpy(nuovoFile, "");

                    strcat(nuovoFile, str_unique_id);

                    switch (immagine){
                        case JPEG:
                            strcat(nuovoFile, ".JPEG");
                            break;
                        case PNG:
                            strcat(nuovoFile, ".PNG");
                            break;
                        case GIF:
                            strcat(nuovoFile, ".GIF");
                            break;
                        case BMP:
                            strcat(nuovoFile, ".BMP");
                            break;
                    }

                    char path_file_nuovo[256];

                    strcpy(path_file_nuovo, PATH_DESTINATION);
                    strcat(path_file_nuovo, "//");
                    strcat(path_file_nuovo, nuovoFile);

#if COPIA_IMMAGINE
                    copiaFile(path_file, path_file_nuovo);
#endif

#if DEBUG
                    printf("File %s copiato \n", path_file_nuovo);
#endif
                }

            }
        }

        closedir(dp);
    }else {
        printf("Percorso non esiste: %s\n", percorso);
    }
}

static bool exludeFile(const char *filter[], const char* value){
    const char *punto = strrchr(value, '.');

    // Trova la posizione dell'ultimo punto (.) nel percorso
    if (punto) {

        // Estrai l'estensione dal percorso
        const char *estensione = punto;

        int lunghezzaDelVettore = sizeof(filter) / sizeof(filter[0]);

        int exclude = 0;

        for (int i = 0; i < lunghezzaDelVettore; i++) {
            if (strcasecmp(estensione, filter[i]) == 0) {
                printf("file da escludere %s \n", value);
                return true;
            }
        }
    }

    return false;
}