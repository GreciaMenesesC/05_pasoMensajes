#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define CLAVE 5678
#define MSGSZ 128


typedef struct {
    long mtype;
    int cliente;
    char mtext[MSGSZ];
} message_buf;


int main(void) {
    int msqid;
    message_buf msg;


    msqid = msgget(CLAVE, 0666);


    if (msqid < 0) {
        perror("msgget");
        return 1;
    }


    msg.mtype = 1;
    msg.cliente = 1;
    strcpy(msg.mtext, "Hola, soy el cliente 1");


    if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) < 0) {
        perror("msgsnd");
        return 1;
    }


    printf("Cliente 1 envia: %s\n", msg.mtext);


    if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 2, 0) < 0) {
        perror("msgrcv");
        return 1;
    }


    printf("Cliente 1 recibe: %s\n", msg.mtext);


    return 0;
}
