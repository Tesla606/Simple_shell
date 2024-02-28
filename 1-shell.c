#include "shell.h"

/**
 * Main function of the shell with command lines handling arguments
 */
int main(void) {
    char buffer[BUFFER_SIZE];
    size_t len = 0;

    while (1) {
        print_prompt();

        // Read the command from the user
        if (getline(&buffer, &len, stdin) == -1) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        // Remove the newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Execute the command with arguments
        execute_command(buffer);
    }

    return 0;
}
