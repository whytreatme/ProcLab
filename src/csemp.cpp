#include "csemp.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <iostream>
#include <cstring>

bool init(unsigned short value,short sem_flg){
    if(sem_id != -1) return false;

    key_t key = ftok("/home/arthurlee/ProcLab/src/csemp", 'S');
    if((sem_id = semget(key, 1, 0666)) == -1){
        if(errno == ENOENT){
            if((sem_id  = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) == -1){
                if(errno == EEXIST){
                    if((sem_id = semget(key, 1, 0666)) == -1){
                        std::cerr << "semget error: " << strerror(errno) << std::endl;
                        return false;
                    }
                }
                else return false;
            }
            union semum sem_union;
            sem_union.val = value;
            if(semctl(sem_id, 0, SETVAL, sem_union) < 0){
                std::cerr << "semctl error: " << strerror(errno) << '\n';
                return false;
            }
        }
        else { 
            std::cerr << "semget error" << strerror(errno) << '\n';
            return false;
        }   
    }
    return true;
}

bool csemp::wait(short value){
    if(sem_id == -1) return false;

    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = value;
    sem_b.sem_flg = sem_flg;
    if(semop(sem_id, &sem_b, 1) == -1){
        std::cerr << "P error: " << strerror(errno) << '\n';
        return false;
    }
    return true;
}

bool csemp::post(short value){
    if(sem_id == -1) return false;

    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = value;
    sem_b.sem_flg = sem_flg;
    if(semop(sem_id, &sem_b, 1) == -1){
        std::cerr << "V error: " << strerror(errno) << '\n';
        return false;
    }
    return true;
}

bool cesemp::destroy(){
    if(sem_id == -1) return false;

    if(semctl(sem_id, 0, IPC_RMID) == -1){
        std::cerr << "destroy error: " << strerror(errno) <<'\n';
        return false;
    }
    return true;
}

csemp::~csemp(){}

