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


    msqid = msgget(CLAVE, IPC_CREAT | 0666);


    if (msqid < 0) {
        perror("msgget");
        return 1;
    }


    printf("Servidor iniciado. Esperando a dos clientes...\n");


    for (int i = 0; i < 2; i++) {


        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0) < 0) {
            perror("msgrcv");
            return 1;
        }


        printf("Servidor recibe del cliente %d: %s\n",
               msg.cliente, msg.mtext);


        int cliente = msg.cliente;


        msg.mtype = cliente + 1;
        snprintf(msg.mtext, MSGSZ,
                 "Hola, cliente %d. Mensaje recibido.", cliente);


        if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            return 1;
        }


        printf("Servidor responde al cliente %d.\n", cliente);
    }


    printf("Servidor finalizo.\n");


    return 0;
}
