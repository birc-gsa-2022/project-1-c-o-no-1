#include <stdio.h>
#include <string.h>
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

    struct Fasta **fastas = parse_fasta(fasta_str);

    // As we parse the reads, we pattern match the read against our above-parsed FASTA sequences
    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        char *pattern = read_fastq_pattern(&reads_str);
        unsigned long pattern_len = strlen(pattern);

        struct Fasta **start_of_fastas = fastas;
        while (*(*fastas)->fasta_head != '\0') {
            char *header = (*fastas)->fasta_head;
            char *sequence = (*fastas)->fasta_sequence;

            // Try to find instances of pattern in sequence
            for (int i = 0; i < strlen(sequence); i++) {
                int j = 0;
                for (; j < pattern_len; j++) {
                    if (sequence[i+j] != pattern[j]) break;
                }
                if (j == pattern_len) {
                    printf("%s\t%s\t%d\t%luM\t%s\n", fastq_header, header, i+1, pattern_len, pattern);
                }
            }
            fastas++;
        }
        fastas = start_of_fastas;
    }

    return 0;
}
