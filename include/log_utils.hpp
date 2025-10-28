#pragma once //c++头文件中防止重复包含的语法
#include <string>
#include <sys/types.h>

namespace plog{

// 初始化日志系统：推导出 <工程根>/logs/ 并确保存在
    void init();

// 返回当前日志目录路径 (例如 /home/arthurlee/ProcLab/logs)
    std::string get_log_dir();

// 生成 task 日志文件路径 (task-<pid>.log)
    std::string task_log_path(pid_t pid);

//  将 stdout / stderr 重定向到该任务日志文件
    void redirect_stdio_to_task(pid_t pid);

// 写 execvp 失败的错误日志
    void write_exec_fail(const char *cmd, int saved_erron);

// 通用快速日志接口 (写入 logs/system.log)
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

}