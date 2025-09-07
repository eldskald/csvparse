# csvparse

Very simple library for parsing csv files. Parses files and returns an array of array of strings.

Just call `csvparse("filepath", &err)` with the filepath and some `int err` pointer for error handling.

```c
int err = 0;
char ***data = csvparse("file/path.csv", &err);
```

Let's say you have the following file:

```
Name,Email,Phone,Age
John Doe,johndoe@email.com,12341234,34
Dont Track Me,throwaway@email.com
```

This csv would get parsed into:

```c
{
    {"Name", "Email", "Phone", "Age"},
    {"John Doe", "johndoe@email.com", "12341234", "34"},
    {"Don't Track Me", "throwaway@email.com"}
}
```

There are also two additional functions to measure the size of the data, `csvrowscount()` to tell you how many rows are in the parsed data and `csvrowlen()` to tell you how many cells are in the passed row. For this file, this is what they would return:

```c
int err = 0;
char ***data = csvparse("file/path.csv", &err);

csvrowscount(data) = 3;
csvrowlen(data[0]) = 4;
csvrowlen(data[1]) = 4;
csvrowlen(data[2]) = 2;
```

Lastly, there is the `csvfree()` function. Call it after you're done processing your csv data.

```c
int err = 0;
char ***data = csvparse("file/path.csv", &err);

{
    // data processing
}

csvfree(data);
```

We need to dynamically allocate the memory to return the arrays of the right sizes. Call `csvfree()` on the data to free the memory, or else the memory will be leaked.

Also, just so you know, internally the array of rows and each array of cells has one extra last space for a `NULL` value. This is so we can calculate their sizes in a similar manner to how we calculate string sizes. This means we are allocating a bit more memory to keep track of sizes, you could technically do `data[3]` on this csv file and it wouldn't seg fault, it would give you `NULL` instead.

## Installation

It is compiled and meant to be statically linked. It uses `clang` on POSIX systems and `MingGW` on Windows. Run `make` and it will generate a `libcsvparse.a` for you to link on your project. The header file is at `src/csvparse.h`.

The library is also very simple, you can just download both `src/csvparse.c` and `src/csvparse.h` files and include them in your project yourself if you don't want to deal with linking. They are very small and shouldn't increase compilation times, and also allows you to modify them if you need more functionalities.

## Examples

Game development example, let's say you want to keep some game design data on spreadsheets so your game designers don't need to look at code to modify it, for example, when loading player stats:

```
Speed,100
Acceleration,400
Friction,600
Jump Height,800
Max Fall Speed,600
Gravity,1200
```

```c
#include <csvparse.h>
#include <stdlib.h>

Player *NewPlayer() {
    int csverror = 0;
    const char ***data = csvparse("player_stats.csv", &csverror);
    if (csverror > 0) {
        // Returns 0 when nothing is wrong
        // read csvparse.h or the last example for more data on error codes
        // Error code explanations might show on your LSP too
        return NULL;
    }

    Player *player = malloc(sizeof(Player));
    player->speed = atof(data[0][1]);
    player->acceleration = atof(data[1][1]);
    player->friction = atof(data[2][1]);
    player->jump_height = atof(data[3][1]);
    player->max_fall_speed = atof(data[4][1]);
    player->gravity = atof(data[5][1]);

    csvfree(data); // DON'T FORGET TO CALL THIS WHEN YOU'RE DONE WITH THE DATA!

    return player;
}
```

Because we are accessing the data with hard coded "magic numbers", this example wouldn't work if we changed the structure of the csv file, like adding new rows, columns or reordering the rows.

Localization example, this one using `csvrowlen()` and `csvrowscount()` and being more resistant to changing the file. Let's say you have the following i18n.csv file:

```
,en,es,fr,pt
YES,Yes,Sí,Oui,Sim
NO,No,No,Non,Não
CLOSE,Close,Cerrar,Fermer,Fechar
```

```c
#include <csvparse.h>
#include <string.h>

#define __BUFFER_SIZE 64

const char *translate(const char *key, const char *language) {
    int csverror = 0;
    const char ***data = csvparse("i18n.csv", &csverror);
    if (csverror > 0) {
        // Returns 0 when nothing is wrong
        // read csvparse.h or the last example for more data on error codes
        // Error code explanations might show on your LSP too
        return NULL;
    }

    int lang_index = 0;
    for (int i = 1; i < csvrowlen(data[0]); i++) {
        if (strcmp(data[0][i], language) == 0) {
            lang_index = i;
            break;
        }
    }

    int key_index = 0;
    for (int i = 1; i < csvrowscount(data); i++) {
        if (strcmp(data[i][0], key) == 0) {
            key_index = i;
            break;
        }
    }

    static char translation[__BUFFER_SIZE];
    strcpy(translation, data[key_index][lang_index]);

    csvfree(data); // DON'T FORGET TO CALL THIS WHEN YOU'RE DONE WITH THE DATA!

    return (const char *)translation;
}
```

This example only relies on the first row being the row that identifies the languages and the first column being the keys. You can add more language and more keys in the future and this function won't break.

Last example, for error handling.

```c
#include <csvparse.h>
#include <stdio.h>

int main() {
    int csverror = 0;
    const char ***data = csvparse("filepath", &csverror);

    switch (csverror) {
    case 1:
        printf("Failure to open file, check errno");
        return 1;
    case 2:
        printf("Cell starts with \" and doesn't end with it");
        return 1;
    case 3:
        printf("Too long cell (1024 character limit)");
        return 1;
    case 4:
        printf("Row with too many cells (128 cells per row maximum)");
        return 1;
    case 5:
        printf("Too many rows (1024 rows limit)");
        return 1;
    }
    // No need to call csvfree(data) if it errors out, but doing so is harmless
    
    // No error found, process data

    csvfree(data); // Call when done if no errors were found
    return 0;
}
```

If you want to process bigger files, you can change the limits yourself on `src/csvparse.c` at the top of the file and recompile.

## Tests

Check the `tests/test.c` file to see what tests are being run. I'm using `--fsanitize=address` to make sure there are no memory leaks.

## License

MIT license. Use it for whatever you want, leaving the notice on the header file would be enough for me.
