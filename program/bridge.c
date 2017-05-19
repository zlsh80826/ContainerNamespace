#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/inotify.h>
#include <errno.h>

#define MSGPERM 0600    
#define MSGTXTLEN 128 

struct msg_buf {
	long mtype;
	char mtext[MSGTXTLEN];
};

void GetContainerPid(int* server_pid, int* client_pid) {
    
    std::ifstream ifs_server("../common/server.pid", std::ifstream::in);
    ifs_server >> *server_pid;
    std::cerr << "Server Container PID is " << *server_pid << std::endl;
    ifs_server.close();

    std::ifstream ifs_client("../common/client.pid", std::ifstream::in);
    ifs_client >> *client_pid;
    std::cerr << "Client Container PID is " << *client_pid << std::endl;
    ifs_client.close();
}

int main(int argc, char** argv) {
    int magic = 5566;
    if (argc > 1) {
        magic = std::stoi(argv[1]);
    } 

    int server_pid;
    int client_pid;

    GetContainerPid(&server_pid, &client_pid);

    std::string client_path = "/proc/" + std::to_string(client_pid) + "/ns/ipc";
    std::string server_path = "/proc/" + std::to_string(server_pid) + "/ns/mnt";

    if (setns(open(client_path.c_str(), O_RDONLY), CLONE_NEWIPC)) {
        std::cerr << "Setting ipc same as " + client_path << " met error\n";
        return 1;
    } else {
        std::cerr << "Setting ipc same as " + client_path << " success\n";
    }
    
    if (setns(open(server_path.c_str(), O_RDONLY), CLONE_NEWNS)) {
        std::cerr << "Setting mnt same as " + server_path << " met error\n";
        return 1;
    } else {
        std::cerr << "Setting mnt same as " + server_path << " success\n";
    }

    int msgqid, rc;
    msg_buf msg;
    msgqid = msgget(magic, MSGPERM | IPC_CREAT | IPC_EXCL);

    if (msgqid < 0) {
        perror(strerror(errno));
        fprintf(stderr, "Failed to create message queue with msgqid = %d\n", msgqid);
        return 1;
    }
    
    rc = msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0);
    if (rc < 0) {
        perror(strerror(errno));
        fprintf(stderr, "msgrecv failed, rc = %d\n", rc);
    }
    printf("Redirecting: %s ... \n", msg.mtext);

    rc = msgctl(msgqid, IPC_RMID, NULL);
    if (rc < 0) {
        perror(strerror(errno));
        fprintf(stderr, "msgctl (return queue) failed, rc = %d\n", rc);
        return 1;
    }

    int inotifyFd, wd;

    inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        perror(strerror(errno));
        fprintf(stderr, "inotifyFd error\n");
        return 1;
    }

    wd = inotify_add_watch(inotifyFd, getcwd(NULL, 0), IN_DELETE);
    if (wd == -1) {
        perror(strerror(errno));
        fprintf(stderr, "inotify_add_watch error\n");
        return 1;
    }
    
    FILE *fp = fopen("message", "w");
    fputs(msg.mtext, fp);
    fputc('\n', fp);
    fclose(fp);
    return 0;
}
