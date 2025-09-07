#include "csvparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define __CSV_MAX_COLUMNS   128
#define __CSV_MAX_ROWS      1024
#define __CSV_MAX_CELL_SIZE 1024

unsigned int csvrowlen(const char **row) {
    if (row == NULL) return 0;
    unsigned int length = 0;
    while (row[length] != NULL) {
        length++;
    }
    return length;
}

unsigned int csvrowscount(const char ***data) {
    if (data == NULL) return 0;
    unsigned int count = 0;
    while (data[count] != NULL) {
        count++;
    }
    return count;
}

void _storedatapoint(char *currCell, const char **currRow) {
    char *datapoint = malloc((strlen(currCell) + 1) * sizeof(char));
    strcpy(datapoint, currCell);
    currRow[csvrowlen(currRow)] = (const char *)datapoint;
    strcpy(currCell, "");
}

void _storerow(const char **currRow, const char ***result) {
    int rowSize = csvrowlen(currRow);
    const char **row = malloc((rowSize + 1) * sizeof(void *));
    for (int i = 0; i < rowSize; i++) {
        row[i] = currRow[i];
        currRow[i] = NULL;
    }
    row[rowSize] = NULL;
    result[csvrowscount(result)] = row;
}

const char ***_getfinalresult(const char ***result) {
    int rowsCount = csvrowscount(result);
    const char ***final = malloc((rowsCount + 1) * sizeof(void *));
    for (int i = 0; i < rowsCount; i++) {
        final[i] = result[i];
    }
    final[rowsCount] = NULL;
    return final;
}

void _error(FILE *file, const char ***result, const char **currRow) {
    fclose(file);
    int rowsCount = csvrowscount(result);
    int currRowLength = csvrowlen(currRow);
    for (int i = 0; i < rowsCount; i++) {
        int rowLength = csvrowlen(result[i]);
        for (int j = 0; j < rowLength; j++) {
            free((void *)result[i][j]);
        }
        free(result[i]);
    }
    for (int i = 0; i < currRowLength; i++) {
        free((void *)currRow[i]);
    }
}

void csvfree(const char ***data) {
    if (data == NULL) return;
    int rowsCount = csvrowscount(data);
    for (int i = 0; i < rowsCount; i++) {
        int rowLength = csvrowlen(data[i]);
        for (int j = 0; j < rowLength; j++) {
            free((void *)data[i][j]);
        }
        free(data[i]);
    }
    free(data);
}

const char ***csvparse(const char *filepath, int *err) {
    *err = 0;

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        *err = 1;
        return NULL;
    }

    const char **result[__CSV_MAX_ROWS] = { NULL };
    const char *currRow[__CSV_MAX_COLUMNS] = { NULL };
    char currCell[__CSV_MAX_CELL_SIZE] = { '\0' };
    char lastChar = '\0';

    bool currCellStartsWithQuot = false;
    bool justEndedQuotCell = false;

    char buffer[2];
    while (fread(buffer, sizeof(char), 1, file) > 0) {
        buffer[1] = '\0';

        // Checking size limits
        if (strlen(currCell) >= __CSV_MAX_CELL_SIZE - 1) {
            _error(file, result, currRow);
            *err = 3;
            return NULL;
        }
        if (csvrowlen(currRow) >= __CSV_MAX_COLUMNS - 1) {
            _error(file, result, currRow);
            *err = 4;
            return NULL;
        }
        if (csvrowscount(result) >= __CSV_MAX_ROWS - 1) {
            _error(file, result, currRow);
            *err = 5;
            return NULL;
        }

        // Dealing with quotation marks.
        if (strlen(currCell) == 0 && buffer[0] == '"') {
            currCellStartsWithQuot = true;
            lastChar = '"';
            continue;
        }
        if (currCellStartsWithQuot && buffer[0] == '"') {
            justEndedQuotCell = true;
            lastChar = '"';
            continue;
        }
        if (justEndedQuotCell && buffer[0] != ',' && buffer[0] != '\n') {
            _error(file, result, currRow);
            *err = 2;
            return NULL;
        }

        // Dealing with commas.
        if (buffer[0] == ',' && !(currCellStartsWithQuot && !justEndedQuotCell)) {
            _storedatapoint(currCell, currRow);
            justEndedQuotCell = false;
            currCellStartsWithQuot = false;
            lastChar = ',';
            continue;
        }

        // Dealing with linebreaks.
        if (buffer[0] == '\n') {

            // Skip empty lines.
            if (lastChar == '\n') continue;

            if (lastChar != ',') _storedatapoint(currCell, currRow);
            _storerow(currRow, result);
            justEndedQuotCell = false;
            currCellStartsWithQuot = false;
            lastChar = '\n';
            continue;
        }

        // Dealing with every other char.
        strcat(currCell, buffer);
        lastChar = buffer[0];
        continue;
    }

    fclose(file);
    return _getfinalresult(result);
}
