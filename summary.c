#include <stdio.h>

#include "summary.h"

void stampaSummary(struct Search_files *search_files){
    printf("Summary\n");
    printf("Directory Reads : %d\n", search_files->directoryReads);
    printf("Files Reads : %d\n", search_files->filesReads);
    printf("Images Reads : %d\n", search_files->imagesReads);
}
