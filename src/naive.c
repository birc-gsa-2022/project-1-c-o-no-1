#include <stdio.h>
#include <string.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include "file_reader.h"
#include "naive.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "%s genome reads", argv[0]);
        return 1;
    }
    const char *genome_fname = argv[1];
    const char *reads_fname = argv[2];

    char *fasta_str = read_file(genome_fname);
    char *reads_str = read_file(reads_fname);

    struct Fasta **fastas = parse_fasta(fasta_str);

    // As we parse the reads, we pattern match the read against our above-parsed FASTA sequences
    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        char *pattern = read_fastq_pattern(&reads_str);
        int pattern_len = (int) strlen(pattern);

        struct Fasta **start_of_fastas = fastas;
        while (*fastas != NULL) {
            char *header = (*fastas)->fasta_head;
            char *sequence = (*fastas)->fasta_sequence;

            // get_position_of_next_match should eventually output -1. At that point, we know no more matches occur.
            int match_index = 0;
            do {
                // Get match index
                match_index = naivly_get_position_of_next_match(sequence, pattern, match_index, pattern_len);
                if (match_index < 0) continue;
                printf("%s\t%s\t%d\t%dM\t%s\n", fastq_header, header, match_index, pattern_len, pattern);
            } while (match_index != -1);

            fastas++;
        }
        fastas = start_of_fastas;
    }

    return 0;
}
