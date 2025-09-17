
#ifdef HEADER_ONLY
#define CSVPARSE_IMPLEMENTATION
#endif
#include <csvparse.h>
#include <stdio.h>
#include <string.h>

#define GREEN  "\x1b[32m"
#define RED    "\x1b[31m"
#define YELLOW "\x1b[33m"
#define RESET  "\x1b[0m"

void expect(char expectation, int *errors) {
    if (expectation) {
        printf(GREEN "Passed\n" RESET);
    } else {
        printf(RED "Failed\n" RESET);
        (*errors)++;
    }
}

void test1(int *errors) {
    printf("--- 01.csv with comma and empty cell ------------------\n");

    int err = 0;
    const char ***data = csvparse("tests/01.csv", &err);

    printf("Got err = %d, expected 0 ", err);
    expect(err == 0, errors);

    printf("Got csvrowscount(data) = %d, expected 2 ", csvrowscount(data));
    expect(csvrowscount(data) == 2, errors);

    printf(
           "Got csvrowlen(data[0]) = %d, expected 4 ", csvrowlen(data[0]));
    expect(csvrowlen(data[0]) == 4, errors);

    printf("Got csvrowlen(data[1]) = %d, expected 4 ", csvrowlen(data[1]));
    expect(csvrowlen(data[1]) == 4, errors);

    printf("\nData points obtained:\n%s, %s, %s, %s\n%s, %s, %s, %s",
           data[0][0], data[0][1], data[0][2], data[0][3],
           data[1][0], data[1][1], data[1][2], data[1][3]);
    printf("\nExpected:\n, test1, 123, with comma, at end of row"
           "\ntest2, , with, comma, aaa\n");
    expect(!strcmp(data[0][0], "") &&
           !strcmp(data[0][1], "test1") &&
           !strcmp(data[0][2], "123") &&
           !strcmp(data[0][3], "with comma, at end of row") &&
           !strcmp(data[1][0], "test2") &&
           !strcmp(data[1][1], "") &&
           !strcmp(data[1][2], "with, comma") &&
           !strcmp(data[1][3], "aaa"),
           errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test2(int *errors) {
    printf("--- 02.csv with empty rows and cells ------------------\n");

    int err = 0;
    const char ***data = csvparse("tests/02.csv", &err);

    printf("Got err = %d, expected 0 ", err);
    expect(err == 0, errors);

    printf("Got csvrowscount(data) = %d, expected 5 ", csvrowscount(data));
    expect(csvrowscount(data) == 5, errors);

    printf("Got csvrowlen(data[0]) = %d, expected 1 ", csvrowlen(data[0]));
    expect(csvrowlen(data[0]) == 1, errors);

    printf("Got csvrowlen(data[1]) = %d, expected 3 ", csvrowlen(data[1]));
    expect(csvrowlen(data[1]) == 3, errors);

    printf("Got csvrowlen(data[2]) = %d, expected 1 ", csvrowlen(data[2]));
    expect(csvrowlen(data[2]) == 1, errors);

    printf("Got csvrowlen(data[3]) = %d, expected 1 ", csvrowlen(data[3]));
    expect(csvrowlen(data[3]) == 1, errors);

    printf("Got csvrowlen(data[4]) = %d, expected 4 ", csvrowlen(data[4]));
    expect(csvrowlen(data[4]) == 4, errors);

    printf("\nData points obtained:\n%s\n%s, %s, %s\n%s\n%s\n%s, %s, %s, %s",
           data[0][0],
           data[1][0], data[1][1], data[1][2],
           data[2][0],
           data[3][0],
           data[4][0], data[4][1], data[4][2], data[4][3]);
    printf(
           "\nExpected:\n\nnon, empty, column\n\n\nnon, , empty, again\n");
    expect(!strcmp(data[0][0], "") &&
           !strcmp(data[1][0], "non") &&
           !strcmp(data[1][1], "empty") &&
           !strcmp(data[1][2], "column") &&
           !strcmp(data[2][0], "") &&
           !strcmp(data[3][0], "") &&
           !strcmp(data[4][0], "non") &&
           !strcmp(data[4][1], "") &&
           !strcmp(data[4][2], "empty") &&
           !strcmp(data[4][3], "again"),
           errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test3(int *errors) {
    printf("--- 03.csv error due to quotation marks ---------------\n");

    int err = 0;
    const char ***data = csvparse("tests/03.csv", &err);

    printf("Got error = %d, expected 2 ", err);
    expect(err == 2, errors);

    printf("Returned data expected to be null ");
    expect(data == NULL, errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test4(int *errors) {
    printf("--- 04.csv error due to cell beig too long ------------\n");

    int err = 0;
    const char ***data = csvparse("tests/04.csv", &err);

    printf("Got error = %d, expected 3 ", err);
    expect(err == 3, errors);

    printf("Returned data expected to be null ");
    expect(data == NULL, errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test5(int *errors) {
    printf("--- 05.csv error due to row having too many cells -----\n");

    int err = 0;
    const char ***data = csvparse("tests/05.csv", &err);

    printf("Got error = %d, expected 4 ", err);
    expect(err == 4, errors);

    printf("Returned data expected to be null ");
    expect(data == NULL, errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test6(int *errors) {
    printf("--- 06.csv error due to too many rows -----------------\n");

    int err = 0;
    const char ***data = csvparse("tests/06.csv", &err);

    printf("Got error = %d, expected 5 ", err);
    expect(err == 5, errors);

    printf("Returned data expected to be null ");
    expect(data == NULL, errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

void test7(int *errors) {
    printf("--- inexistent.csv error to open file -----------------\n");

    int err = 0;
    const char ***data = csvparse("inexistent.csv", &err);

    printf("Got error = %d, expected 1 ", err);
    expect(err == 1, errors);

    printf("Returned data expected to be null ");
    expect(data == NULL, errors);

    printf("-------------------------------------------------------\n\n");
    csvfree(data);
}

int main() {
    int errors = 0;
    
#ifdef HEADER_ONLY
    printf(YELLOW "HEADER-ONLY VERSION\n" RESET);
#else
    printf(YELLOW "STATICALLY LINKED VERSION\n" RESET);
#endif
    printf(YELLOW "RUNNING TESTS...\n\n" RESET);
    test1(&errors);
    test2(&errors);
    test3(&errors);
    test4(&errors);
    test5(&errors);
    test6(&errors);
    test7(&errors);
    printf(YELLOW "RESULTS: %d failure" RESET, errors);
    if (errors != 1) printf(YELLOW "s" RESET);
    if (errors > 0) printf(RED "\nFAILED\n" RESET);
    else printf(GREEN "\nPASSED\n" RESET);

    return errors > 0;
}
