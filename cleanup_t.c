#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define SERVER_KEY 12345

typedef struct {
    long msgType;
    char msgText[1];
} Message;

void displayMenu() {
    printf("Do you want the server to terminate? Press Y for Yes and N for No.\n");
}

int main() {
    key_t serverKey = ftok(".", 's');
    int serverQueueID = msgget(serverKey, 0666);

    if (serverQueueID == -1) {
        perror("Error accessing message queue");
        exit(EXIT_FAILURE);
    }

    Message requestMsg;
    char userChoice;

    while (1) {
        displayMenu();

        printf("Enter your choice: ");
        scanf(" %c", &userChoice);

        if (userChoice == 'Y' || userChoice == 'y') {
            // Inform the main server to terminate
            requestMsg.msgType = SERVER_KEY;
            requestMsg.msgText[0] = 'Y';

            // Send the request to the server
            if (msgsnd(serverQueueID, &requestMsg, sizeof(requestMsg.msgText), 0) == -1) {
                perror("Error sending termination request to server");
                exit(EXIT_FAILURE);
            }

            printf("Termination request sent to the server.\n");
            break;
        } else if (userChoice == 'N' || userChoice == 'n') {
            // Continue running
            printf("Continuing to run...\n");
            continue;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
