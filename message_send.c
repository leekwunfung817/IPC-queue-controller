#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSGSZ 128

void ipcq_s(int keyi,int mtype, char txt[]);
char* ipcq_r(int keyi,int mtype,char* txt);

typedef struct msgbuf {
	long mtype;
	char mtext[MSGSZ];
} message_buf;

message_buf rbuf;
int main(int argc, char* argv[]) 
{
	int key;
	int mtype;
	sscanf(argv[1],"%d",&key);
	sscanf(argv[2],"%d",&mtype);
	if (argc==4) {
		printf("%s: key %d mtype %d (optional: message)\n", argv[0], argc,key,mtype);
		printf("Sent message:[%s]",argv[3]);
		ipcq_s(key,mtype,argv[3]);
	} else if (argc==3) {
		char txt[MSGSZ];
		printf("%s",ipcq_r(key,mtype,txt));
		// printf("[%s][%s]\n",txt,rbuf.mtext);
	}
}

void ipcq_s(int keyi,int mtype, char txt[])
{
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	key_t key;
	message_buf sbuf;
	size_t buf_length;

	key = keyi + 0;
	if ((msqid = msgget(key, msgflg)) < 0) {
		perror("msgget");
	} else {
	}
	sbuf.mtype = mtype;
	(void) strcpy(sbuf.mtext, txt);
	buf_length = strlen(sbuf.mtext) + 1;
	if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0){
		printf("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
		perror("msgsnd");
	} else {
		printf("Message: [%s] Sent\n", sbuf.mtext);
	}
}

char* ipcq_r(int keyi,int mtype,char* txt)
{
        int msqid;
        key_t key;
        key = keyi;
        if ((msqid = msgget(key, 0666)) < 0) {
                perror("msgget");
        }
        if (msgrcv(msqid, &rbuf, MSGSZ, mtype, 0) < 0) {
                perror("msgrcv");
        }
	strcpy( txt , rbuf.mtext);
        // printf("%s\n",txt);
	return txt;
}

