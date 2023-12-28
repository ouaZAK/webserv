#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    fd_set readfds;
    struct timeval timeout;
    int x = open("lala", O_CREAT | O_RDWR , 0600);
    write(x, "haha", 4);
    // Initialize the set and timeout
    FD_ZERO(&readfds);
    FD_SET(x, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    // Use select to monitor stdin for reading
    int result = select(3 + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
    } else if (result == 0) {
        printf("No data within 5 seconds.\n");
    } else {
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            printf("Data is available to read from stdin.\n");
            // Perform read operation from stdin
        }
        if (FD_ISSET(x, &readfds)) {
            printf("Data is available to read from file.\n");
            // Perform read operation from stdin
        }
    }
    return 0;
}
