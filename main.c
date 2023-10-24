#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <io.h>
#include <direct.h>

#include <time.h>

#define DEBUG 1
#define COPIA_IMMAGINE 1

const char* PATH_ORIGIN = "C://Temp";
const char* PATH_DESTINATION = "C://Temp//Fotos";

struct Search_files{
    int directoryReads;
    int filesReads;
    int imagesReads;
};

enum Format_images{
    JPEG, PNG, GIF, BMP
};

int copiaFile(const char* percorsoOrigine, const char* percorsoDestinazione);

void cambiaEstensione(char* percorsoFile, const char* nuovaEstensione);

void stampaSummary(struct Search_files *search_files);

long long generateUniqueID();

int isImage(const char *filename) {

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Errore nell'apertura del file :");
        perror(filename);
        return -1;
    }

    unsigned char header[10]; // Dimensione dell'intestazione da esaminare
    size_t bytesRead = fread(header, 1, sizeof(header), file);
    fclose(file);

    if (bytesRead < sizeof(header)) {
        // Il file non contiene un'intestazione completa
        return -1;
    }

    // Analizza l'intestazione per rilevare formati di immagine comuni
    if (header[0] == 0xFF && header[1] == 0xD8) {
        // JPEG
        return JPEG;
    } else if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        // PNG
        return PNG;
    } else if (header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46) {
        // GIF
        return GIF;
    } else if (header[0] == 0x42 && header[1] == 0x4D) {
        // BMP
        return BMP;
    }

    // Aggiungi altri formati di immagine, se necessario

    return -1;
}

bool exists(const char *path) {
    return _access(path, 0) == 0;
}

bool isDirectory(const char *path) {
    struct _stat st;

    if (_stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    return false;
}

void leggiFileRicorsivamente(const char *percorso, struct Search_files *search_files) {
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

                    char nuovoFile[256] = {NULL};
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


int main() {

    struct Search_files search_files = {0};

    const char *percorso_iniziale = PATH_ORIGIN; // Sostituisci con il tuo percorso
#if DEBUG
    puts("Inizio applicazione");
    printf("Inzio di lettura dalla cartella %s\n", percorso_iniziale);
#endif // DEBUG

    leggiFileRicorsivamente(percorso_iniziale, &search_files);

    stampaSummary(&search_files);
#if DEBUG
    puts("Fine applicazione");
#endif // DEBUG
    getchar();
    return 0;
}


long long generateUniqueID() {
    static unsigned int counter = 0;
    time_t timestamp = time(NULL);

    // Combina il timestamp e il contatore per creare l'identificatore
    long long unique_id = (long long)timestamp * 1000 + counter;
    counter++;

    return unique_id;
}

// Funzione per copiare un file da un percorso di origine a un percorso di destinazione
int copiaFile(const char* percorsoOrigine, const char* percorsoDestinazione) {
    FILE* fileOrigine = fopen(percorsoOrigine, "rb");
    if (fileOrigine == NULL) {
        perror("Errore nell'apertura del file di origine");
        return 1;
    }

    FILE* fileDestinazione = fopen(percorsoDestinazione, "wb");
    if (fileDestinazione == NULL) {
        perror("Errore nell'apertura del file di destinazione");
        fclose(fileOrigine);
        return 1;
    }

    int carattere;
    while ((carattere = fgetc(fileOrigine)) != EOF) {
        fputc(carattere, fileDestinazione);
    }

    fclose(fileOrigine);
    fclose(fileDestinazione);

    printf("File copiato con successo!\n");
    return 0;
}

// Funzione per cambiare l'estensione di un file
void cambiaEstensione(char* percorsoFile, const char* nuovaEstensione) {
    // Trova la posizione dell'ultimo punto (.) nel percorso del file
    char* punto = strrchr(percorsoFile, '.');

    if (punto != NULL) {
        // Sovrascrivi l'estensione esistente con la nuova estensione
        strcpy(punto, nuovaEstensione);
    } else {
        // Se non è stata trovata un'estensione esistente, aggiungi la nuova estensione alla fine del percorso
        strcat(percorsoFile, nuovaEstensione);
    }
}

void stampaSummary(struct Search_files *search_files){
    printf("Summary\n");
    printf("Directory Reads : %d\n", search_files->directoryReads);
    printf("Files Reads : %d\n", search_files->filesReads);
    printf("Images Reads : %d\n", search_files->imagesReads);
}
