#ifndef SUMMARY_H_INCLUDED
#define SUMMARY_H_INCLUDED

struct Search_files{
    unsigned int directoryReads;
    unsigned int filesReads;
    unsigned int imagesReads;
    unsigned int imagesCopied;
};

void stampaSummary(struct Search_files* );

#endif // SUMMARY_H_INCLUDED
