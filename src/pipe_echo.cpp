#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

int main(){
    int fd[2];
    if(pipe(fd) == -1){
        cerr << "pipe error: " << strerror(errno) << endl;
        return -1;
    }
    pid_t pid = fork();
    if(pid < 0){
        cerr << "fork error: " << strerror(errno) << endl;
        return -2;
    }
    else if(pid > 0){
        close(fd[0]);
        ssize_t n = write(fd[1], "hello from parent\n", 18);
        if(n < 0){
            cerr << "write error: " << strerror(errno) << endl;
        }
        close(fd[1]);
        waitpid(-1, NULL, 0);
    }
    else{
        close(fd[1]);
        if(dup2(fd[0], STDIN_FILENO) == -1){
            cerr << "dup2 error: " <<strerror(errno) << endl;
            _exit(127);
        }
        execlp("/bin/cat", "cat", (char*)nullptr);
        cerr << "exec error: " << strerror(errno) << endl;
        _exit(127);
    }

    return 0;
}