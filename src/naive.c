#include <stdio.h>
#include <string.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include "file_reader.h"

struct Fasta {
    char* fasta_head;
    char* fasta_sequence;
};

struct Fasta **get_fastas(char *fasta_str) {
    struct Fasta **fastas = malloc(10*sizeof (char*));
    while (fasta_str[0] != '\0') {
        char *header = read_fasta_head(&fasta_str);
        char *sequence = read_fasta_sequence(&fasta_str);
        struct Fasta *f = malloc(sizeof(struct Fasta));
        f->fasta_head = header;
        f->fasta_sequence = sequence;
        *fastas = f;
        fastas++;
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "%s genome reads", argv[0]);
        return 1;
    }
    const char *genome_fname = argv[1];
    const char *reads_fname = argv[2];

    printf("Search in %s for reads from %s\n", genome_fname, reads_fname);

    char *fasta_str = read_file(genome_fname);
    char *reads_str = read_file(reads_fname);

    /*
     * for read in reads
     *  for fasta in fastas
     *      print(read, fasta, position)
     */

    struct Fasta **fastas = get_fastas(fasta_str);

    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        //printf("%s\t", fastq_header);

        char *pattern = read_fastq_pattern(&reads_str);
        unsigned long pattern_len = strlen(pattern);
        //printf("%s\n", pattern);

        char *fasta_str_start = fasta_str;
        while (fasta_str[0] != '\0') {
            char *header = read_fasta_head(&fasta_str);
            //printf("%s\t", header);

            char *sequence = read_fasta_sequence(&fasta_str);
            //printf("%s\n", sequence);

            // Try to find instances of pattern in sequence
            for (int i = 0; i < strlen(sequence); i++) {
                int j = 0;
                for (; j < strlen(pattern); j++) {
                    if (sequence[i+j] != pattern[j]) break;
                }
                if (j == 3) {
                    printf("%s\t%s\t%d\t%luM\t%s\n", fastq_header, header, i+1, pattern_len, pattern);
                }
            }
            // printf("%s\t%s\t%luM\t%s\n", fastq_header, header, pattern_len, pattern);
        }
        fasta_str = fasta_str_start;
    }


    return 0;
}
