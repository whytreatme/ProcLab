#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main(){
    cout << "Hello ProcLab!" << endl;
    //输出自身以及父进程的进程编号
    cout << "PID= " << getpid()
         << "  ,PPID= " << getppid() << endl;
    return 0;
}
