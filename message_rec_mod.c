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

    // 1er mensaje recibido
    if (msgrcv(msqid, &msg, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Se recibio el mensaje: %s\n", msg.mtext);

    // Respuesta enviada
    msg.mtype = 2;
    strcpy(msg.mtext, "Loud and clear");

    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Se envio el mensaje: %s\n", msg.mtext);

    // 2do mensaje enviado
    if (msgrcv(msqid, &msg, MSGSZ, 3, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Se recibio el mensaje: %s\n", msg.mtext);

    msgctl(msqid, IPC_RMID, NULL);

    return 0;
}
