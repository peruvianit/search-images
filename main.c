/**
 * @file main.c
 * @brief Programma che cerca le immagine nel file system a partire di
 *        un path di origine.
 *
 * @date 31/10/2023
 * @version 1.0.0
 * @since 25/10/2023
 * @autor Peruvianit
 */

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

#include "fileUtils.h"
#include "summary.h"
#include "generateID.h"

static const char *EXCLUDE_EXTENSION_IMAGES_FORMATS[] = {".JPEG", ".JPG", ".PNG", ".BMP", ".GIF"};

struct ConfigApp{
    char *originPath;
    char *destinationPath;

    int copyFlag;
    int exludeImagesFlag;
    int verboseFlag;
} configApp;

static void leggiFileRicorsivamente(const char*, struct Search_files*);

static bool exludeFile(const char *[], const char*);

static void leggeOpzioni(struct ConfigApp*, int, char *[]);

int main(int argc, char *argv[]) {

    leggeOpzioni(&configApp, argc, argv);

    struct Search_files search_files = {0};

    if (configApp.verboseFlag) {
        puts("Inizio applicazione");
        printf("Inzio di lettura dalla cartella %s\n", configApp.originPath);
    }

    leggiFileRicorsivamente(configApp.originPath, &search_files);

    stampaSummary(&search_files);
    if (configApp.verboseFlag) {
        puts("Fine applicazione");
    }

    puts("\n\n\Pressiona un carattere per uscire >>>>");
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

            if (configApp.verboseFlag) {
                printf("[%s]\n", path_file);
            }

            if (isDirectory(path_file)) {
                search_files->directoryReads++;
                leggiFileRicorsivamente(path_file, search_files);

            } else {
                search_files->filesReads++;

                long long unique_id = generateUniqueID();

                char str_unique_id[50]; // Dichiaro una stringa di destinazione

                // Converto il valore long long in una stringa e lo salvo in 'str'
                snprintf(str_unique_id, sizeof(str_unique_id), "%lld", unique_id);

                //printf("%s\n", str_unique_id);

                int immagine = isImage(path_file);

                if (immagine >= 0){
                    search_files->imagesReads++;

                    if (configApp.exludeImagesFlag && exludeFile(EXCLUDE_EXTENSION_IMAGES_FORMATS, entry->d_name)){
                        continue;
                    }

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

                    strcpy(path_file_nuovo, configApp.destinationPath);
                    strcat(path_file_nuovo, "//");
                    strcat(path_file_nuovo, nuovoFile);

                    if (configApp.copyFlag) {
                        copiaFile(path_file, path_file_nuovo);
                        search_files->imagesCopied++;
                    }

                    if (configApp.verboseFlag) {
                        printf("File %s copiato \n", path_file_nuovo);
                    }

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
                if (configApp.verboseFlag) {
                    printf("file da escludere %s \n", value);
                }
                return true;
            }
        }
    }

    return false;
}

static void leggeOpzioni(struct ConfigApp *configApp, int argc, char *argv[]){

    int opt;

    int copyFlag = 1;
    int exludeImagesFlag = 0;
    int verboseFlag = 0;
    int helpFlag = 0;

    char *originPath = NULL;
    char *destinationPath = NULL;

    puts("");

    while ((opt = getopt(argc, argv, "o:d:nevh")) != -1) {
        switch (opt) {
            case 'o':
                originPath = optarg;
                printf(">> Percorso di origine: %s\n", originPath);
                break;
            case 'd':
                destinationPath = optarg;
                printf(">> Percorso di destinazione: %s\n", destinationPath);
                break;
            case 'n':
                printf(">> Opzione -n attivata.\n");
                copyFlag = 0;
                break;
            case 'e':
                printf(">> Opzione -e attivata.\n");
                exludeImagesFlag = 1;
                break;
            case 'v':
                printf(">> Opzione -v attivata.\n");
                verboseFlag = 1;
                break;
            case 'h':
                helpFlag = 1;
                break;
            default:
                fprintf(stderr, "Utilizzo: %s -o <PATH ORIGINE> -d <PATH DESTINAZIONE> [-e] [-n] [-v] [-h]\n", argv[0]);
                return 1;
        }
    }

    if (helpFlag && (copyFlag + exludeImagesFlag + verboseFlag) == 1) {
        printf("Programma per la ricerca di immagini da un path di origine e vengono copiate su una cartella destinazione, il nome delle immagini sono generate con un nome univoco.\n");
        printf("-o\t path di origine dove se vuole iniziare a cercare l'immagine\n");
        printf("-d\t path destinazione dove saranno copiate l'immagine se non 'e attiva l'opzione -n\n");
        printf("-e\t Esclude l'immgini con estensione de un formato immagine valido e cerca su file con estensione diversa o senza estensione\n");
        printf("-n\t Non copia i file trovati, solo mostra il Summary\n");
        printf("-v\t Verbose da quello che sta processando il programma\n");
        printf("-h\t Help\n\n");

        printf("Utilizzo: %s -o <PATH ORIGINE> -d <PATH DESTINAZIONE> [-e] [-n] [-v] [-h]\n", argv[0]);
        exit(0);

    }

    if (originPath == NULL || destinationPath == NULL){
        perror("E obbligatorio il path di origine e de destinazione");
        exit(1);
    }

    configApp->originPath = originPath;
    configApp->destinationPath = destinationPath;

    configApp->copyFlag = copyFlag;
    configApp->exludeImagesFlag = exludeImagesFlag;
    configApp->verboseFlag = verboseFlag;

}