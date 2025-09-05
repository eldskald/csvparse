#include <csvparse.h>

int main() {
    int size = 0;
    csvparse("test/test1.csv", &size);
    return 0;
}
