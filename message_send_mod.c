#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSZ 128

typedef struct {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    message_buf msg;

    msqid = msgget(2345, IPC_CREAT | 0666);

    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    // 1er mensaje enviado
    msg.mtype = 1;
    strcpy(msg.mtext, "Are you hearing me?");

    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Se envio el mensaje: %s\n", msg.mtext);

    // Respuesta recibida
    if (msgrcv(msqid, &msg, MSGSZ, 2, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Se recibio el mensaje: %s\n", msg.mtext);

    // 2do mensaje enviado
    msg.mtype = 3;
    strcpy(msg.mtext, "I can hear you too");

    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Se envio el mensaje: %s\n", msg.mtext);

    return 0;
}
