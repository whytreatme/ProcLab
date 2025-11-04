#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

void CHLD_EXIT(int sign);
void FATH_EXIT(int sign);

int main(int argc, char *argv[]){
    if(argc < 2){
        cerr << "Lack of argument" << '\n'
               << "Usage " << argv[0] << " cmd" << endl;
        return 1;
    }

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler =  CHLD_EXIT;
    act.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act, NULL);
    sigemptyset(&act.sa_mask);
    act.sa_handler = FATH_EXIT;
    sigaction(SIGINT, &act, NULL);

    pid_t pid;

    while(true){
        pid = fork();
        if(pid < 0){
            cerr << "fork error: " << strerror(errno) << endl;
            return -2;
        }
        if(pid > 0){
            sleep(2);
        }
        else{
            execvp(argv[1], &argv[1]);
            cerr << "exec error: " << strerror(errno) << endl;
            //约定127等于命令失败
            _exit(127);
        }
    }
    return 0;
}

void CHLD_EXIT(int sign){
    int sts;
    pid_t pid;

    while((pid = waitpid(-1, &sts ,WNOHANG)) > 0){
        write(STDOUT_FILENO, "child exited\n", 13);
    }
}

void FATH_EXIT(int sign){
    signal(SIGTERM, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    kill(0, SIGTERM);
    
    int st;
    while (true) {
        pid_t r = waitpid(-1, &st, 0);   // 不带 WNOHANG：一直等到没孩子
        if (r > 0) continue;
        if (r < 0) {
            if (errno == EINTR) continue; // 被别的信号打断就继续等
            // ECHILD -> 没有子进程了
            break;
        }
    }
}