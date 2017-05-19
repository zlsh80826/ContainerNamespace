#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

void GetContainerPid(int* server_pid, int* client_pid) {
    
    std::ifstream ifs_server("../common/server.pid", std::ifstream::in);
    ifs_server >> *server_pid;
    std::cout << "Server Container PID is " << *server_pid << std::endl;
    ifs_server.close();

    std::ifstream ifs_client("../common/client.pid", std::ifstream::in);
    ifs_client >> *client_pid;
    std::cout << "Client Container PID is " << *client_pid << std::endl;
    ifs_client.close();
}

int main(int argc, char** argv) {
    int server_pid;
    int client_pid;

    GetContainerPid(&server_pid, &client_pid);


    return 0;
}
