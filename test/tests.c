#include "minunit.h"
#include "../src/naive.h"
#include "../src/kmp.h"

void test_setup(void) {
    /* Nothing */
}

void test_teardown(void) {
    /* Nothing */
}

int (*test_fun)(char*, char*, int, int);

MU_TEST(test_exact) {
    char *sequence = "ACGT";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(1, match);
}

MU_TEST(test_almost_match) {
    char *sequence = "AC*GT";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(-1, match);
}

MU_TEST(test_middle) {
    char *sequence = "----ACGT----";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(5, match);
}

MU_TEST(test_end) {
    char *sequence = "--------ACGT";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(9, match);
}

MU_TEST(test_almost_match_end) {
    char *sequence = "---------ACG";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(-1, match);
}

MU_TEST(test_match_twice) {
    char *sequence = "--ACGTACGT";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(3, match);
    seq_index = match;
    int match2 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(7, match2);
}

MU_TEST(test_match_twice_spaced_out) {
    char *sequence = "-GG-ACGT----ACGT";
    char *pattern = "ACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(5, match);
    seq_index = match;
    int match2 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(13, match2);
}

MU_TEST(test_match_empty_string) {
    char *sequence = "ACGTTAACGT";
    char *pattern = "";

    // Tests the string all the way through. Including before and after the string.
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(1, match);
    seq_index = 1;
    int match2 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(2, match2);
    seq_index = 2;
    int match3 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(3, match3);
    seq_index = 9;
    int match4 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(10, match4);
    seq_index = 10;
    int match5 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(11, match5);
    seq_index = 11;
    int match6 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(12, match6);
    seq_index = -1;
    int match7 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(0, match7);
}

MU_TEST(test_pattern_longer_than_sequence) {
    char *sequence = "ACGT";
    char *pattern = "ACGTTAACGT";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(-1, match);
}

MU_TEST(test_multiple_matches) {
    char *sequence = "CACACACACTTTCTTAC";
    char *pattern = "AC";
    int seq_index = 0;
    int pattern_len = (int) strlen(pattern);
    int match = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(2, match);
    seq_index = match;
    int match2 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(4, match2);
    seq_index = match2;
    int match3 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(6, match3);
    seq_index = match3;
    int match4 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(8, match4);
    seq_index = match4;
    int match5 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(16, match5);
    seq_index = match5;
    int match6 = test_fun(sequence, pattern, seq_index, pattern_len);
    mu_assert_int_eq(-1, match6);
}

MU_TEST(test_border_array_ississippi) {
    char *pattern = "ississippi";
    int expected_border_array[] = {0, 0, 0, 1, 2, 3, 4, 0, 0, 1};
    int pattern_len = (int) strlen(pattern);
    int *ba = compute_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], ba[i]);
    }
}

MU_TEST(test_border_array_increasing) {
    char *pattern = "aaaaaaaa";
    int expected_border_array[] = {0, 1, 2,3,4,5,6,7};
    int pattern_len = (int) strlen(pattern);
    int *ba = compute_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], ba[i]);
    }
}

MU_TEST(test_border_array_short) {
    char *pattern = "a";
    int expected_border_array[] = {0};
    int pattern_len = (int) strlen(pattern);
    int *ba = compute_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], ba[i]);
    }
}

MU_TEST(test_border_array_empty_string) {
    char *pattern = "";
    int pattern_len = (int) strlen(pattern);
    int *ba = compute_border_array(pattern, pattern_len);
    mu_assert(ba==NULL, "Expected null pointer");
}

MU_TEST(test_restricted_border_array_ississippi) {
    char *pattern = "ississippi";
    int expected_border_array[] = {0, 0, 0, 0, 0, 0, 4, 0, 0, 1};
    int pattern_len = (int) strlen(pattern);
    int *bax = compute_restricted_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], bax[i]);
    }
}

MU_TEST(test_restricted_border_array_aaaba) {
    char *pattern = "aaaba";
    int expected_border_array[] = {0, 0, 2, 0, 1};
    int pattern_len = (int) strlen(pattern);
    int *bax = compute_restricted_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], bax[i]);
    }
}

MU_TEST(test_restricted_border_array_aaba) {
    char *pattern = "aaba";
    int expected_border_array[] = {0, 1, 0, 1};
    int pattern_len = (int) strlen(pattern);
    int *bax = compute_restricted_border_array(pattern, pattern_len);

    for (int i = 0; i < sizeof expected_border_array/sizeof (int); i++) {
        mu_assert_int_eq(expected_border_array[i], bax[i]);
    }
}
MU_TEST(test_restricted_border_array_empty_string) {
    char *pattern = "";
    int pattern_len = (int) strlen(pattern);
    int *bax = compute_restricted_border_array(pattern, pattern_len);
    mu_assert(bax==NULL, "Expected null pointer");
}

void run_all_tests() {
    MU_RUN_TEST(test_exact);
    MU_RUN_TEST(test_almost_match);
    MU_RUN_TEST(test_middle);
    MU_RUN_TEST(test_end);
    MU_RUN_TEST(test_almost_match_end);
    MU_RUN_TEST(test_match_twice);
    MU_RUN_TEST(test_match_twice_spaced_out);
    MU_RUN_TEST(test_match_empty_string);
    MU_RUN_TEST(test_pattern_longer_than_sequence);
    MU_RUN_TEST(test_multiple_matches);
}

void run_kmp_relevant_tests() {
    MU_RUN_TEST(test_border_array_ississippi);
    MU_RUN_TEST(test_border_array_increasing);
    MU_RUN_TEST(test_border_array_short);
    MU_RUN_TEST(test_border_array_empty_string);
    MU_RUN_TEST(test_restricted_border_array_ississippi);
    MU_RUN_TEST(test_restricted_border_array_aaaba);
    MU_RUN_TEST(test_restricted_border_array_aaba);
    MU_RUN_TEST(test_restricted_border_array_empty_string);
}

MU_TEST_SUITE(naive_test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    // Run tests for the naive impl.
    test_fun = &naivly_get_position_of_next_match;
    run_all_tests();

    // Run tests for the linear impl.
    test_fun = &kmp_get_position_of_next_match_testing;
    run_kmp_relevant_tests();
    run_all_tests();

}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(naive_test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}