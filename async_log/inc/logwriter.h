#pragma once
#include <queue>
#include <fstream>
#include <thread>
#include <mutex>

struct MsgItem {
	std::time_t timestamp;
	std::string content;
};

struct MsgItemComparator {
	bool operator()(const MsgItem& a, const MsgItem& b) {
		return a.timestamp > b.timestamp;
	}
};

class LogWriter {
public:
    using MsgQueueType = std::priority_queue<MsgItem, std::vector<MsgItem>, MsgItemComparator>;

    // 对象销毁
    ~LogWriter();

    // 获取单例对象
    static LogWriter& instance();    
    
    // 线程运行函数
    void run();

    // 日志项入队，进入备用队列
    void enqueue(const MsgItem& msg);

    // 输出一条队列中的所有记录
    void dump_queue(MsgQueueType* q);

private:
    // 同步锁，确保对消息队列的访问是线程安全的
	std::mutex mutex;

	// 消息队列内容，采用主备队列，以减小线程同步的开销
    // 用优先队列存储，以保证出队数据是按时间有序的
	MsgQueueType *msg_queue_master, *msg_queue_standby;

    // 输出文件
    std::ofstream ofs;

    // 写日志线程
    std::thread log_writer_thread;

    // 有效标志
    bool is_valid;

    // 单例类，隐藏其构造函数
    LogWriter();
    LogWriter(const LogWriter&) = delete;
    LogWriter(LogWriter&&) = delete;
};