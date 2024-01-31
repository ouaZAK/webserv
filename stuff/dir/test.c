#include <stdio.h>
#include <sys/stat.h>

int main() {
    const char *path = "./dr";  // Replace with the actual path

    struct stat fileStat;

    // Use stat to get information about the file
    if (stat(path, &fileStat) == 0)
	{
        // Check if it is a directory
        if (S_ISDIR(fileStat.st_mode)) {
            printf("%s is a directory.\n", path);
        } else {
            printf("%s is not a directory.\n", path);
        }
    }
	else
	{
        perror("Error in stat");
    }

    return 0;
}

