#ifndef CSEMP_H_
#define CSEMP_H_

class cesemp{
private:
    union semum{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    };

    int semid;

    short sem_flg;

    csemp(const csemp &) = delete;
    cesemp &operator=(const csemp &) = delete;

public:
    csemp():sem_id(-1){}

    bool init(unsigned short value = 1, short sem_flg = SEM_UNDO);
    bool wait(short value = -1);
    bool post(short value = 1);
    bool destroy();
    ~csemp();
};

#endif