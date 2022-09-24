#include <stdio.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include "file_reader.h"

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

    while (fasta_str[0] != '\0') {
        char *header = read_fasta_head(&fasta_str);
        printf("%s\t", header);

        char *sequence = read_fasta_sequence(&fasta_str);
        printf("%s\n", sequence);
    }

    while (reads_str[0] != '\0') {
        char *header = read_fastq_head(&reads_str);
        printf("%s\t", header);

        char *sequence = read_fastq_pattern(&reads_str);
        printf("%s\n", sequence);
    }


    return 0;
}
