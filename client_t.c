#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include<time.h>

#define SERVER_KEY 12345
#define MESSAGE_SIZE 256

typedef struct {
    long msgType;
    char msgText[MESSAGE_SIZE];
} Message;

void displayMenu() {
    printf("Menu:\n");
    printf("1. Enter 1 to contact the Ping Server\n");
    printf("2. Enter 2 to contact the File Search Server\n");
    printf("3. Enter 3 to contact the File Word Count Server\n");
    printf("4. Enter 4 if this Client wishes to exit\n");
}

int main() {
    key_t serverKey = ftok(".", 's');
    int serverQueueID = msgget(serverKey, 0666);

    if (serverQueueID == -1) {
        perror("Error accessing message queue");
        exit(EXIT_FAILURE);
    }

    int clientID;  // Assume the client ID is received from the user

    Message requestMsg;
    Message replyMsg;

    // Assume clientID is randomly generated
    // Modify the way you generate the clientID based on user input or other requirements
    srand(time(NULL));
    clientID = rand() % 1000 + 1;

    printf("Enter Client-ID: %d\n", clientID);

    while (1) {
        displayMenu();
        int choice;

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }

        requestMsg.msgType = clientID;

        // Construct message based on the client's choice
        // ...

        // Send the request to the server
        if (msgsnd(serverQueueID, &requestMsg, sizeof(requestMsg.msgText), 0) == -1) {
            perror("Error sending message to server");
            exit(EXIT_FAILURE);
        }

        // Receive and display the server's reply
        if (msgrcv(serverQueueID, &replyMsg, sizeof(replyMsg.msgText), clientID, 0) == -1) {
            perror("Error receiving message from server");
            exit(EXIT_FAILURE);
        }

        printf("Server's reply: %s\n", replyMsg.msgText);
    }

    return 0;
}
