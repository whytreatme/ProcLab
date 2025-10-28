#include "log_utils.hpp"
#include <unistd.h>
#include <filesystem>
#include <cstdio>
#include <fcntl.h>


namespace plog{
    static std::string g_log_dir;

    //---------------------------------------------------
    // 推导<工程根>/logs/ 路径
    //---------------------------------------------------------------
    
    //这个函数仅供cpp内部使用，不需要暴露实现细节
    std::string detect_log_dir(){
        char buf[1024];
        ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if(n <= 0) return "logs";
        buf[n] = '\0';
        std::filesystem::path exe_path(buf);
        auto root = exe_path.parent_path().parent_path();
        return (root/"logs").string();
    }

//----------------------------------------------------------
    void init(){
        g_log_dir = detect_log_dir();
        std::error_code ec;
        std::filesystem::create_directories(g_log_dir, ec);
    }

//---------------------------------------------------------------
    std::string get_log_dir(){
        if(g_log_dir.empty())  init();
        return g_log_dir;
    }

//---------------------------------------------------------------------
    std::string task_log_path(pid_t pid){
        char filename[64];
        //snprintf让你格式化编写字符串
        snprintf(filename, sizeof(filenaame), "task-%d.log",pid);
        return get_log_dir() + "/" + filename;
    }

//------------------------------------------------------------
    void redirect_stdio_to_task(pid_t pid){
        auto path = task_log_path(pid);
        int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(fd >= 0){
            //任务执行时的echo、printf都属于标准输入的内容要全部重定型
            dup2(fd, STDERR_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
    }

//--------------------------------------------------------------------
    void write_exec_fail(const char *cmd, int saved_erron){
        char path[256];
        snprintf(path, sizeof(path), "%s/exec-<%d>.err.log",get_log_dir().c_str()
                , getpid());
        int fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(fd > 0){
            dprintf(fd, "[exec failed] cmd=%s erron=%d (%s)\n",
                    cmd, saved_erron, stderror(saved_erron));
            close(fd);
        }
        dprintf(STDERR_FILENO, "[exec filed] cmd=%s erron=%d (%s)\n",
                    cmd, saved_erron, stderror(saved_erron));
        
    }

//---------------------------------------------------------------------
    void info(const std::string& msg){
        std::string path = get_log_dir() + "/system.log";
        int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(fd >= 0){
            dprintf(fd, "[INFO] %s\n", msg.c_str());
            close(fd);
        } 
    }

//------------------------------------------------------------
    void warn(const std::string& msg){
        std::string path = get_log_dir() + "/system.log";
        int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(fd >= 0){
            dprintf(fd, "[WARN] %s\n", msg.c_str());
            close(fd);
        } 
    }

    void error(const std::string& msg){
        std::string path = get_log_dir() + "/system.log";
        int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(fd >= 0){
            dprintf(fd, "[ERROR] %s\n", msg.c_str());
            close(fd);
        } 
    }

//------------------------------------------------------------
    void signal_safe_write(const char* msg){
        int fd = open((get_log_dir() + "/signal.log").c_str(),
            O_CREAT | O_WRONLY | O_APPEND, 0644
        );

        if(fd >= 0){
            write(fd, msg, strlen(msg));
            close(fd);
        }
    }
}