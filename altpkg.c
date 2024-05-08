#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to check if a command exists in PATH
int commandExists(const char* cmd) {
    return access(cmd, X_OK) == 0;
}

// Function to get the command prefix (sudo or doas)
const char* getCommandPrefix() {
    if (commandExists("/usr/bin/sudo"))
        return "sudo";
    else if (commandExists("/usr/bin/doas"))
        return "doas";
    else
        return "";
}

// Print usage information
void printHelp() {
    printf("Usage: AltPkg for Gentoo\n");
    printf("  install <package>: Install a package\n");
    printf("  deselect <package>: Deselect a package (remove from world set)\n");
    printf("  purge <package>: Completely remove a package\n");
    printf("  update: Sync package repositories\n");
    printf("  help: Display package usage information\n");
    printf("  upgrade: Update all installed packages\n");
    printf("  search <package>: Search for a package\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: Missing command. Use 'help' for usage information.\n");
        return 1;
    }

    // Get the command prefix
    const char* cmdPrefix = getCommandPrefix();

    if (strcmp(cmdPrefix, "") == 0) {
        printf("Error: Neither 'sudo' nor 'doas' is available in the system.\n");
        return 1;
    }

    // Get the command
    const char* cmd = argv[1];

    if (strcmp(cmd, "install") == 0) {
        if (argc < 3) {
            printf("Error: Missing package name for 'install' command.\n");
            return 1;
        }
        char command[256];
        snprintf(command, sizeof(command), "%s emerge --ask --verbose --quiet %s", cmdPrefix, argv[2]);
        system(command);
    } else if (strcmp(cmd, "deselect") == 0) {
        if (argc < 3) {
            printf("Error: Missing package name for 'deselect' command.\n");
            return 1;
        }
        char command[256];
        snprintf(command, sizeof(command), "%s emerge --deselect %s", cmdPrefix, argv[2]);
        system(command);
    } else if (strcmp(cmd, "purge") == 0) {
        char command[256];
        snprintf(command, sizeof(command), "%s emerge --depclean --ask", cmdPrefix);
        system(command);
    } else if (strcmp(cmd, "update") == 0) {
        char command[256];
        snprintf(command, sizeof(command), "%s emerge --sync", cmdPrefix);
        system(command);
        system("eix-update");
    } else if (strcmp(cmd, "help") == 0) {
        printHelp();
    } else if (strcmp(cmd, "upgrade") == 0) {
        char command[256];
        snprintf(command, sizeof(command), "%s emerge --ask --verbose --deep --update --newuse --quiet @world", cmdPrefix);
        system(command);
    } else if (strcmp(cmd, "search") == 0) {
        if (argc < 3) {
            printf("Error: Missing package name for 'search' command.\n");
            return 1;
        }
        char command[256];
        snprintf(command, sizeof(command), "eix %s", argv[2]);
        system(command);
    } else {
        printf("Error: Unknown command '%s'. Use 'help' for usage information.\n", cmd);
        return 1;
    }

    return 0;
}
