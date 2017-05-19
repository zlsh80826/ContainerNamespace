#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<sys/errno.h>
#include<errno.h>

#include"msg_helper.h"

int main(int argc,char **argv)
{
	int msgqid, rc;
	struct msg_buf msg;
	
	if (argc > 1) {
		msgqid = msgget(atoi(argv[1]), 0);
	} else {
		msgqid = msgget(MAGIC, 0);
	}
	if (msgqid < 0) {
		perror(strerror(errno));
		printf("failed to create message queue with msgqid = %d\n", msgqid);
		return 1;
	}

   
    while (scanf("%s", msg.mtext) != EOF) {
	    msg.mtype = 1;
        rc = msgsnd(msgqid, &msg, sizeof(msg.mtext), 0);
        if (rc < 0) {
            perror( strerror(errno) );
            printf("msgsnd failed, rc = %d\n", rc);
            return 1;
        }
    }

	return 0;
}
