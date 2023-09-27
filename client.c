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
    char some_text[MAX_TEXT];
};
//note that without wait -- we can have either all parent process /all child process / or  any kind of overlapping between the 2
int main(){
   
    pid_t f1;
    f1=fork();//parent and child will have different values of f1
    if(f1<0) printf("Error");
    else if(f1==0){//child process - instance creation
         int id;
        printf("client - client pid:%d\n",getpid());
        printf("client -  server pid%d\n",getppid());

        //printf("\n");
         if (scanf("%d", &id) == 1) {
            printf("Child received:\n");
        } else {
            perror("Error reading input\n");
        }
        if(id<=0) printf("enter a positive integer as client id\n");
        
        else{
        // arr[id]+=1;//here u cannot add a global comparison detail hence IPC connection will be used i.e message queue analysis
        // if(arr[id]!=1) printf("error: please enter unique client id");
        // else{
        //     printf("%d  %d\n",arr[id],id);
        // }
        printf("Enter 1 to contact the Ping Server\n");
        printf("Enter 2 to contact the File Search Server\n");
        printf("Enter 3 to contact the File Word Count Server\n");
        printf("Enter 4 if this Client wishes to exit\n");
        struct connect_msg some_data;
        int msgid;
        char buffer[20];
        int running=1;
        
         msgid = msgget((key_t)12345,0666|IPC_CREAT);
        if(msgid==-1) printf("error is creating message queue\n ");
        while(running){
            long int mtype;
            printf("enter some id  ");
            scanf("%ld\n",&mtype);
            
            // if(mtype==1)continue;
            // if(mtype==4) running =0;
            // if(mtype==2 || mtype==3){
                printf("enter the file name  ");
                scanf("%s\n",buffer);
           // }
             some_data.msg_type = mtype;
            strcpy(some_data.some_text,buffer);
            if(msgsnd(msgid,(void *)&some_data,MAX_TEXT,0)==-1) printf("message not sent\n");
           if(mtype==4) running=0;
        }

        }

    }
    else{
        wait(NULL);
        printf("server - server pid%d\n",getpid());
        printf("server - child pid%d\n",f1);
    }
    return 0;
}