#include "loggingconfig.h"
#include "logwriter.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

LogWriter::LogWriter()
    : mutex()
    , msg_queue_master(new MsgQueueType)
    , msg_queue_standby(new MsgQueueType)
    , ofs(loggingconfig::log_output_file)
    , is_valid(false)
{
    if (!ofs.is_open()) {
        std::cerr << "--- [FATAL] Cannot open log_output_file [" 
                  << loggingconfig::log_output_file << "]." << std::endl;
        exit(-1);
    }
    log_writer_thread = std::thread(&LogWriter::run, this);
    is_valid = true;
}

LogWriter::~LogWriter() {
    // 设置有效标志位为无效
    if (is_valid)
        is_valid = false;
    // 等待线程结束
    if (log_writer_thread.joinable())
        log_writer_thread.join();
    // 如果备用队列还有数据，将备用队列倒空
    dump_queue(msg_queue_standby);
    // 关闭文件输出流
    ofs.close();
    // 销毁主备日志队列
    delete msg_queue_standby;
    delete msg_queue_master;
}

LogWriter& LogWriter::instance() {
    static LogWriter writer;
    return writer;
}

void LogWriter::run() {
    do {
        // 等一个处理间隔后再启动
        std::this_thread::sleep_for(std::chrono::milliseconds(loggingconfig::log_writer_cycle));
        
        {
            // 获取消息队列同步锁
            std::unique_lock<std::mutex> lock_standby(mutex);
            // 交换主备队列(此时备用队列有数据，待输出；主用队列在上一轮写日志已经清空)
            std::swap(msg_queue_master, msg_queue_standby);
        }

        // 将主用队列倒空
        dump_queue(msg_queue_master);
    } while (is_valid);
}

// 日志项入队
void LogWriter::enqueue(const MsgItem& msg) {
    // 入队时永远向备用队列入队
    std::unique_lock<std::mutex> lock(mutex);
    msg_queue_standby->push(msg);
}

// 输出一条队列中的所有记录
void LogWriter::dump_queue(MsgQueueType* q) {
    while (!q->empty()) {
        ofs << q->top().content << std::endl;
        q->pop();
    }
}

