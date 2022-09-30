#ifndef NAIVE_H
#define NAIVE_H

// Checks sequence[seq_index:] for pattern. Returns index when exact pattern is found. Otherwise -1.
int naivly_get_position_of_next_match(char *sequence, char *pattern, int seq_index, int pattern_len) {
    // We define, that the empty string can be found everywhere
    if (pattern_len == 0) return seq_index+1;
    // Try to find instances of pattern in sequence
    for (int i = seq_index; i < strlen(sequence); i++) {
        int j = 0;
        for (; j < pattern_len; j++) {
            if (sequence[i+j] != pattern[j]) break;
        }
        if (j == pattern_len) {
            return i+1;
        }
    }
    return -1;
}

#endif //NAIVE_H
