#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int seconds = 10; // Default sleep time, can be changed to 60 for a minute

    if (argc > 1) {
        seconds = atoi(argv[1]); // Get sleep time from command line argument
    }

    printf("Sleeping for %d seconds...\n", seconds);
    sleep(seconds);
    printf("Done sleeping for %d seconds.\n", seconds);
    return 0;
}

