#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define SERVER_KEY 12345
#define MESSAGE_SIZE 256

typedef struct {
    long msgType;
    char msgText[MESSAGE_SIZE];
} Message;

// Function to handle Ping Server
void handlePing(Message *request, Message *reply) {
    // Perform actions for "Ping Server" command
    strcpy(reply->msgText, "hello");
}

// Function to handle File Search Server
void handleFileSearch(Message *request, Message *reply, char *filename) {
    // Perform actions for "File Search Server" command
    // ...
}

// Function to handle File Word Count Server
void handleFileWordCount(Message *request, Message *reply, char *filename) {
    // Perform actions for "File Word Count Server" command
    // ...
}

int main() {
    key_t serverKey = ftok(".", 's');
    int serverQueueID = msgget(serverKey, IPC_CREAT | 0666);

    if (serverQueueID == -1) {
        perror("Error creating message queue");
        exit(EXIT_FAILURE);
    }

    printf("Main server started. Listening for client requests...\n");

    Message requestMsg;
    Message replyMsg;

    while (1) {
        // Receive request from client
        if (msgrcv(serverQueueID, &requestMsg, sizeof(requestMsg.msgText), 0, 0) == -1) {
            perror("Error receiving message from client");
            exit(EXIT_FAILURE);
        }

        // Handle client's request based on the message type
        switch (requestMsg.msgType) {
            case 1:
                handlePing(&requestMsg, &replyMsg);
                break;
            case 2:
                handleFileSearch(&requestMsg, &replyMsg, "filename.txt");
                break;
            case 3:
                handleFileWordCount(&requestMsg, &replyMsg, "filename.txt");
                break;
            default:
                fprintf(stderr, "Invalid command\n");
                continue;
        }

        // Send reply to the client
        if (msgsnd(serverQueueID, &replyMsg, sizeof(replyMsg.msgText), 0) == -1) {
            perror("Error sending message to client");
            exit(EXIT_FAILURE);
        }
    }

    // Cleanup (not reached in this example)
    msgctl(serverQueueID, IPC_RMID, NULL);

    return 0;
}
