#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file;
    char line[256]; // Buffer to hold one line

    // We need variables to store the values.
    // We use 'long' because these numbers can be very large.
    long mem_total = 0;
    long mem_free = 0;
    long mem_available = 0;

    // 1. Open the /proc/meminfo file for reading ("r")
    file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/meminfo");
        return 1; // Exit with an error
    }

    // 2. Read the file line by line
    // fgets reads one line at a time
    while (fgets(line, sizeof(line), file)) {
        
        // 3. Check if the line is one we care about
        // 'sscanf' tries to match a pattern. If it matches,
        // it fills our variables and returns how many items it filled.

        if (sscanf(line, "MemTotal: %ld kB", &mem_total) == 1) {
            // Found MemTotal, loop continues
        } else if (sscanf(line, "MemFree: %ld kB", &mem_free) == 1) {
            // Found MemFree, loop continues
        } else if (sscanf(line, "MemAvailable: %ld kB", &mem_available) == 1) {
            // Found MemAvailable, loop continues
        }

        // Optimization: If we've found all three, we can stop reading
        if (mem_total > 0 && mem_free > 0 && mem_available > 0) {
            break;
        }
    }

    // 4. Close the file
    fclose(file);

    // 5. Print the results!
    // We divide by 1024 to convert from kB (kilobytes) to MB (megabytes)
    // for easier reading.
    printf("--- System Memory ---\n");
    printf("Total:      %ld MB\n", mem_total / 1024);
    printf("Available:  %ld MB\n", mem_available / 1024);
    printf("Free:       %ld MB\n", mem_free / 1024);

    return 0; // Success
}