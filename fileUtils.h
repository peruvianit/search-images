#ifndef FILEUTILS_H_INCLUDED
#define FILEUTILS_H_INCLUDED

#include <stdbool.h>

enum Format_images{
    JPEG, PNG, GIF, BMP
};

int copiaFile(const char*, const char*);

int isImage(const char*);

bool exists(const char*);

bool isDirectory(const char*);

#endif // FILEUTILS_H_INCLUDED
