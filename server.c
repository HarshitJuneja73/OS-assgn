#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#define MAX_Client_Id_Range 10
#define MAX_TEXT 50


struct connect_msg{
    long int msg_type;
    char some_text[BUFSIZ];
};
int main(){
    int running =1;
    int msgid;
    struct connect_msg some_data;
    long int msg_to_rcv = 0;
    msgid = msgget((key_t)12345,0666|IPC_CREAT);
    while(running){
        msgrcv(msgid,(void *)&some_data,BUFSIZ,msg_to_rcv,0);
        printf("Data received %s\n",some_data.some_text);
        if(some_data.msg_type==4)running=0;
    }
    msgctl(msgid,IPC_RMID,0);
}