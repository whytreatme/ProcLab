#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include "csemp.h"

using namespace std;


int *counter;
int main(){
    key_t key = ftok("/home/arthurlee/ProcLab/src/shm_counter", 'A');
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
    if( shmid == -1){
        cerr << "shmget failed." << endl;
        return -1;
    }
    void *p = shmat(shmid, 0, 0);
    if(p == (void*) -1){
        cerr << "shmat failed." << endl;;
        return -1;
    }
    counter = (int *)p;
    *counter = 0;

    csemp mutex; mutex.init();
    pid_t pid = fork();

    if(pid < 0){
        cerr << "fork error: " << strerror(errno) << endl;
        _exit(127);
    }

   
    for(int i = 0; i < 100; i++){
        if(pid > 0){
            mutex.wait();
            cout << "The value of counter is " << *counter << endl;
            mutex.post();
            usleep(5 * 1000); 
        }
        else{
            mutex.wait();
            (*counter)++;
            mutex.post();
            usleep(20 * 1000);
            if(i == 99){
                shmdt(counter);
                 _exit(0);
            }
        }
    }

    if((pid = waitpid(-1, NULL, 0)) < 0){
        cout << "waitpid error: " << endl;
    }
    shmdt(counter);
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        cout << "shmctl failed\n";
        return -1;
    }
    if(!mutex.destroy()) return -1;
    return 0;
}

    