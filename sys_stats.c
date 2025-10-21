#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h> 

// --- ANSI Color Codes ---
#define CLR_RESET   "\033[0m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_YELLOW  "\033[1;33m"
#define CLR_GREEN   "\033[0;32m"
#define CLR_BLUE    "\033[1;34m"   

// Gauge Drawing Function 
/**
 * Prints a text-based progress bar.
 * @param label The label for the gauge.
 * @param percentage A value from 0.0 to 1.0 (or higher for load).
 * @param width The total width of the bar in characters.
 */
// To use Unicode block characters ---
void print_gauge(const char* label, double percentage, int width) {
    // Clamp percentage
    double clamped_perc = percentage > 1.0 ? 1.0 : (percentage < 0.0 ? 0.0 : percentage);
    int filled_width = (int)(clamped_perc * width);

    printf(CLR_GREEN "%-11s " CLR_RESET, label);
    
    printf("[");
    printf(CLR_BLUE); // Set bar color
    
    // Print the filled part of the bar
    for (int i = 0; i < filled_width; i++) {
        // "█" is the Unicode Full Block character
        printf("█"); 
    }
    
    printf(CLR_RESET); // Reset color for empty part
    
    // Print the empty part of the bar
    for (int i = 0; i < (width - filled_width); i++) {
        // "░" is the Unicode Light Shade character
        printf("░");
    }
    printf("] ");

    printf("%.1f%%\n", percentage * 100.0);
}


void get_load_avg() {
    FILE *file;
    double load_1, load_5, load_15;
    int num_cores = get_nprocs(); // Get number of logical CPUs

    file = fopen("/proc/loadavg", "r");
    if (file == NULL) {
        perror("Error opening /proc/loadavg");
        return;
    }

    if (fscanf(file, "%lf %lf %lf", &load_1, &load_5, &load_15) == 3) {
        printf(CLR_CYAN "--- System Load (relative to %d cores) ---\n" CLR_RESET, num_cores);
        // Calculate percentage based on core count
        print_gauge("Load (1m)",   load_1 / num_cores, 40);
        print_gauge("Load (5m)",   load_5 / num_cores, 40);
        print_gauge("Load (15m)",  load_15 / num_cores, 40);
    } else {
        fprintf(stderr, "Error reading load averages.\n");
    }

    fclose(file);
}


void get_cpu_info() {
    FILE *file;
    char line[256];
    
    file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/cpuinfo");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "model name")) {
            char *model = strchr(line, ':');
            if (model) {
                model += 2; 
                model[strcspn(model, "\n")] = '\0';
                printf(CLR_CYAN "\n--- CPU Info ---\n" CLR_RESET);
                printf(CLR_GREEN "Model: " CLR_RESET "%s\n", model);
            }
            break; 
        }
    }
    fclose(file);
}

// To calculate percentage and use gauge 
void get_memory_info() {
    FILE *file;
    char line[256];
    long mem_total = 0, mem_available = 0;

    file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/meminfo");
        return; 
    }

    while (fgets(line, sizeof(line), file)) {
        // We only *really* need Total and Available for a good percentage
        if (sscanf(line, "MemTotal: %ld kB", &mem_total) == 1) {
        } else if (sscanf(line, "MemAvailable: %ld kB", &mem_available) == 1) {
        }
        if (mem_total > 0 && mem_available > 0) {
            break; // Found what we need
        }
    }
    fclose(file);

    if (mem_total > 0) {
        long mem_used = mem_total - mem_available;
        double mem_perc = (double)mem_used / (double)mem_total;

        printf(CLR_CYAN "--- System Memory ---\n" CLR_RESET);
        print_gauge("Mem Used", mem_perc, 40); // The gauge!

        // Also print the text details for context
        printf(CLR_GREEN "Total:     " CLR_RESET "%9ld MB\n", mem_total / 1024);
        printf(CLR_GREEN "Available: " CLR_RESET "%9ld MB\n", mem_available / 1024);
        printf(CLR_GREEN "Used:      " CLR_RESET "%9ld MB\n", mem_used / 1024);

    } else {
        fprintf(stderr, "Error reading memory info.\n");
    }
}


int main() {
    get_cpu_info();
    printf("\n"); 

    get_memory_info();
    printf("\n"); 
    
    get_load_avg();
    
    return 0;
}