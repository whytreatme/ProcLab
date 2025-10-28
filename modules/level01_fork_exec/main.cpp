#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <erron.h>
#include <string.h>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]){
    if(argc == 1){
        cerr << "Usage: ./" << argv[0] << " <command>" << endl;
        exit(-1);
    }

    pid_t pid;
    FILE *fp = fopen()
    while(true){
        pid = fork();
        if(pid > 0) continue;
        else if(pid < 0){
            cerr << "fork error :" << stderror(erron) << endl;
            exit(-1);
        }
        else{
            if((execvp(argv[1], &argv[1])) == -1){
                
            }
        }
    }
}