#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

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
    int server_pid;
    int client_pid;

    GetContainerPid(&server_pid, &client_pid);

    std::string client_path = "/proc/" + std::to_string(client_pid) + "/ns/ipc";
    std::string server_path = "/proc/" + std::to_string(server_pid) + "/ns/mnt";

    if (setns(open(client_path.c_str(), O_RDONLY), CLONE_NEWIPC)) {
        std::cerr << "Setting ipc same as " + client_path << " met error\n";
    } else {
        std::cerr << "Setting ipc same as " + client_path << " success\n";
    }
    
    if (setns(open(server_path.c_str(), O_RDONLY), CLONE_NEWNS)) {
        std::cerr << "Setting mnt same as " + server_path << " met error\n";
    } else {
        std::cerr << "Setting mnt same as " + server_path << " success\n";
    }

    return 0;
}
