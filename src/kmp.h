#ifndef KMP_H
#define KMP_H

#include <malloc.h>

int *compute_border_array(char *pattern, int pattern_len) {
    if (pattern_len == 0) return NULL;
    int *ba = malloc(sizeof(int)*pattern_len);
    ba[0] = 0;
    for (int i = 1; i < pattern_len; i++) {
        // Test if we can extend border. Otherwise, reset.
        if (pattern[i] == pattern[ba[i-1]]) {
            ba[i] = ba[i-1]+1;
        } else {
            ba[i] = 0;
        }
    }
    return ba;
}

int *compute_restricted_border_array(char *pattern, int pattern_len) {
    // Compute and change the border array.
    // Could be done faster (and uglier), but we assume pattern_len is not long.
    int *bax = compute_border_array(pattern, pattern_len);
    for (int i = 1; i < pattern_len; i++) {
        if (bax[i] != 0 && pattern[i+1] == pattern[bax[i]]) {
            // Next char after border must be different
            bax[i] = bax[bax[i]-1];
        }
    }
    return bax;
}

// Checks sequence[seq_index:] for pattern. Returns index when exact pattern is found. Otherwise -1.
int kmp_get_position_of_next_match(char *sequence, char *pattern, int seq_index, int pattern_len, int *bax) {
    // We define, that the empty string can be found everywhere
    if (pattern_len == 0) return seq_index+1;
    int seq_len = (int) strlen(sequence);

    int i = seq_index;
    int j = 0;

    while (i <= seq_len-pattern_len+j) {
        // Keep increasing i and j on matches
        while (j < pattern_len && sequence[i] == pattern[j]) { i++; j++; }

        // If we have matched all the way; return.
        if (j == pattern_len) return i-pattern_len+1;

        // On mismatches, we can use bax to skip
        if (sequence[i] != pattern[j]) {
            if (j == 0) {
                i++;
            } else {
                j = bax[j - 1];
            }
        }
    }
    return -1;
}

// Calls the original function with a precomputed restricted border array
int kmp_get_position_of_next_match_testing(char *sequence, char *pattern, int seq_index, int pattern_len) {
    int *bax = compute_restricted_border_array(pattern, pattern_len);
    return kmp_get_position_of_next_match(sequence, pattern, seq_index, pattern_len, bax);
}

#endif //KMP_H
