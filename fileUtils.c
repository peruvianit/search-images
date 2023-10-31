/**
 * @file fileUtils.c
 * @brief Funzioni per la gestione dei files e Directory
 *
 * @date 25/10/2023
 * @version 1.0
 * @since 25/10/2023
 * @autor Peruvianit
 */


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

/**
 * @brief verifica se � permesso accedere al file/directory.
 *
 * @param path path del file/directory
 * @return true se � permesso accedere al file/directory
 */
bool exists(const char *path) {
    return _access(path, 0) == 0;
}

/**
 * @brief verifica se � una directory.
 *
 * @param path path del file/directory
 * @return true se � una directory
 */
bool isDirectory(const char *path) {
    struct _stat st;

    if (_stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    return false;
}

/**
 * @brief verifica se � una immagine.
 *
 * La verifica � fatta leggendo il file binario e prendendo il descriptor o e bytes iniziali
 * e verificanddo che tipo di fil, sapendo la secuenza de bytes.
 *
 * @param filename path del file
 * @return true se � una immagine
 */
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

/**
 * @brief copia un file in un'altra destinazione.
 *
 * Funzione per copiare un file da un percorso di origine a un percorso di destinazione.
 *
 * @param percorsoOrigine path originale
 * @return percorsoDestinazione path dove se voglia copiare il file
 */
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

    return 0;
}
