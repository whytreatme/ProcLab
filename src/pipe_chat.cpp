#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <errno.h>
#include <cctype>

using namespace std;

ssize_t write_n(int fd, const void *buf, size_t count);
ssize_t read_n(int fd, void *buf, size_t count);

int main(){
    int p2c[2], c2p[2];
    if(pipe(p2c) == -1 || pipe(c2p) == -1){
        cerr << "pipe error: " << strerror(errno) << endl;
        return -1;
    }
    pid_t pid = fork();
    if(pid < 0){
        cerr << "fork error: " << strerror(errno) << endl;
        return -2;
    }
        
    else if(pid > 0){
            close(p2c[0]);
            close(c2p[1]);
            char msg[6][256]={};
            for(int i = 0; i < 3; i++){
                if(i == 0)
                    strcpy(msg[i], "I need a job\n");
                if(i == 1)
                    strcpy(msg[i], "I need money\n");
                if(i == 2)
                    strcpy(msg[i], "I need sex\n");
            }
            //写入管道
            for(int j = 0; j < 3; j++){
                write_n(p2c[1], msg[j], sizeof(msg[j]));
            }
            close(p2c[1]);

            //从管道读出来
            for(int k = 3; k < 6; k++){
                read_n(c2p[0], msg[k], sizeof(msg[k]));
                cout << msg[k] << endl;
            }
            int sts;
            if(waitpid(-1, &sts, 0) < 0){
                cerr << "alarm, the signal that terminates wait is "
                     << WTERMSIG(sts) << endl;
            }
            return 0;
        }
        else{
            close(p2c[1]);
            close(c2p[0]);
            char info[6][256]={};
    
            for(int m = 0; m < 3; m++){
                //一边读取
                ssize_t n = read_n(p2c[0], info[m], sizeof(info[m]));
                if(n <= 0) break;
                //一边写入
                for(int i = 0; i <= n; ++i)
                    info[m + 3][i] = toupper((unsigned char)info[m][i]);
                write_n(c2p[1], info[m + 3], sizeof(info[m + 3]));               
            }
            _exit(0);
        }
    
}


ssize_t write_n(int fd, const void *buf, size_t count){
    size_t total = 0;
    while(total < count){
        ssize_t n = write(fd, (char*)buf + total, count - total) 
        if(n == -1){
            //信号中断的恢复
            if(errno == EINTR) continue;
            return -1;
        }
        total += n;
    }
    return (ssize_t)total;
}

ssize_t read_n(int fd, void *buf, size_t count){
    ssize_t n = 0;
    size_t total = 0;
    while(total < count){
        ssize_t n = read(fd, (char*)buf + total, count - total)
        if(n < 0){
            if(errno == EINTR) continue;
            return -1;
        }
        total += n;
    }
    return (ssize_t)total;
}
//错误版本，现在这个版本里buf指针没有移动，总是从头开始读，无法跳出循环也读不完
// ssize_t write_n(int fd, const void *buf, size_t count){
//     ssize_t n ;
//     while((n = write(fd, (char*)buf, count) ) > 0);
//     if(n < 0){
//         cerr << "write error: " << strerror(errno) << endl;
//     }
//     return count;
// }

// ssize_t read_n(int fd, void *buf, size_t count){
//     ssize_t n ;
//     while((n = read(fd, (char*)buf, count)) > 0);
//     if(n < 0){
//         cerr << "read error: " << strerror(errno) << endl;
//     }
//     return count;
// }

