#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

void set_cgroup_limit(const char *path, const char *file, const char *value);

int main() {
    const char *cgroup_path = "/sys/fs/cgroup/bumbum_cgroup";

    // Create cgroup directory
    if (mkdir(cgroup_path, 0755) < 0){
        perror("Failed to create cgroup directory");
        exit(errno);
    }

    // Set CPU and memory limits
    set_cgroup_limit(cgroup_path, "cpu.max", "50000 100000");
    set_cgroup_limit(cgroup_path, "memory.max", "134217728");

    // Add current process to cgroup
    set_cgroup_limit(cgroup_path, "cgroup.procs", "0");

    printf("Cgroup created and configured. Run 'top' to monitor.\n");

    // Simulate workload
    while (1){
        printf("Running in cgroup...\n");
        sleep(1);
    }

    exit(0);
}

void set_cgroup_limit(const char *path, const char *file, const char *value) {
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, file);

    int fd = open(full_path, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open cgroup file");
        exit(errno);
    }

    if (write(fd, value, strlen(value)) < 0){
        perror("Failed to write to cgroup file");
        close(fd);
        exit(errno);
    }

    close(fd);
}
