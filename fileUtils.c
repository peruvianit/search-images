#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "fileUtils.h"

struct Search_files{
    unsigned int directoryReads;
    unsigned int filesReads;
    unsigned int imagesReads;
};

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
