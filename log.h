#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdint.h>
#include <list>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <time.h>
#include "util.h"
#include "thread.h"

#define PANGTAO_LOG_DEBUG(logger, s)                                                                                                                                          \
    {PangTao::LogEvent::ptr event(new PangTao::LogEvent(logger, PangTao::LogLevel::DEBUG, __FILE__, __LINE__, 0, PangTao::GetThreadId(), PangTao::GetCoroutineId(), time(0))); \
    event->getSS() << s;                                                                                                                                                     \
    logger->log(event);}

#define PANGTAO_LOG_INFO(logger, s)                                                                                                                                          \
    {PangTao::LogEvent::ptr event(new PangTao::LogEvent(logger, PangTao::LogLevel::INFO, __FILE__, __LINE__, 0, PangTao::GetThreadId(), PangTao::GetCoroutineId(), time(0))); \
    event->getSS() << s;                                                                                                                                                    \
    logger->log(event);}

#define PANGTAO_LOG_ERROR(logger, s)                                                                                                                                          \
    {PangTao::LogEvent::ptr event(new PangTao::LogEvent(logger, PangTao::LogLevel::ERROR, __FILE__, __LINE__, 0, PangTao::GetThreadId(), PangTao::GetCoroutineId(), time(0))); \
    event->getSS() << s;                                                                                                                                                     \
    logger->log(event);}

#define PANGTAO_LOG_WARN(logger, s)                                                                                                                                          \
    {PangTao::LogEvent::ptr event(new PangTao::LogEvent(logger, PangTao::LogLevel::WARN, __FILE__, __LINE__, 0, PangTao::GetThreadId(), PangTao::GetCoroutineId(), time(0))); \
    event->getSS() << s;                                                                                                                                                    \
    logger->log(event);}

#define PANGTAO_LOG_FATAL(logger, s)                                                                                                                                          \
    {PangTao::LogEvent::ptr event(new PangTao::LogEvent(logger, PangTao::LogLevel::FATAL, __FILE__, __LINE__, 0, PangTao::GetThreadId(), PangTao::GetCoroutineId(), time(0))); \
    event->getSS() << s;                                                                                                                                                     \
    logger->log(event);}

#define PANGTAO_ROOT_LOGGER PangTao::LoggerManager::getInstance()->getRoot()

namespace PangTao
{
    class Logger;
    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
        static const char *toString(LogLevel::Level level);
    };
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t coroutine_id, uint64_t time);
        const char *getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getCoroutineId() const { return m_coroutineId; }
        uint64_t getTime() const { return m_time; }
        const std::string getContent() const { return m_ss.str(); }
        std::stringstream &getSS() { return m_ss; }
        LogLevel::Level getLevel() const { return m_level; }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

    private:
        const char *m_file = nullptr;
        int32_t m_line;
        uint32_t m_elapse = 0;
        uint32_t m_threadId = 0;
        uint32_t m_coroutineId = 0;
        time_t m_time = 0;
        std::stringstream m_ss;
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
    };

    // class LogEventWrap
    // {
    // public:
    //     LogEventWrap(LogEvent::ptr e);
    //     ~LogEventWrap();
    //     std::stringstream &getSS();

    // private:
    //     LogEvent::ptr m_event;
    // };

    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
        LogFormatter(const std::string &pattern);

        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){};
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };
        void init();
        std::vector<FormatItem::ptr> m_items;
        std::string m_pattern;
    };

    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender(){};
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr formatter);
        LogFormatter::ptr getFormatter();
        Mutex m_mutex;
        //const Mutex& getMutex(){return m_mutex;}
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
        //Mutex m_mutex;
    };

    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string &name = "root");
        void log(LogEvent::ptr event);
        // void debug(LogEvent::ptr event);
        // void info(LogEvent::ptr event);
        // void warn(LogEvent::ptr event);
        // void fatal(LogEvent::ptr event);
        // void error(LogEvent::ptr event);
        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level level) { m_level = level; }
        void setFormatter(const std::string &fomatter);
        const std::string &getName() const { return m_name; }

    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appenders;
        LogFormatter::ptr m_formatter;
        Mutex m_mutex;
    };

    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

    private:
    };
    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string &filename);
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
    class LoggerManager
    {
    public:
        typedef std::shared_ptr<LoggerManager> ptr;
        static ptr getInstance()
        {
            if (instance == nullptr)
            {
                Mutex::Lock lock(m_mutex);
                if (instance == nullptr)
                {
                    instance = std::shared_ptr<LoggerManager>(new LoggerManager());
                }
            }
            return instance;
        }
        Logger::ptr getRoot() const
        {
            return m_root;
        }
        Logger::ptr getLogger(const std::string &str);
        void registerLogger(const std::string &loggerName, Logger::ptr logger);

    private:
        LoggerManager()
        {
            m_root_init();
        };
        LoggerManager(const LoggerManager &) = delete;
        LoggerManager(const LoggerManager &&) = delete;
        LoggerManager &operator=(const LoggerManager &) = delete;
        static std::shared_ptr<LoggerManager> instance;
        std::unordered_map<std::string, Logger::ptr> m_loggers;
        Logger::ptr m_root;
        static Mutex m_mutex;
        void m_root_init()
        {
            m_root = std::shared_ptr<Logger>(new Logger);
            m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));
            m_root->setLevel(LogLevel::DEBUG);
        }
    };
    //void PANGTAO_LOG_DEBUG(Logger::ptr logger, std::string s);
    // void PANGTAO_LOG_INFO(Logger::ptr logger, std::string s);
    // void PANGTAO_LOG_WARN(Logger::ptr logger, std::string s);
    // void PANGTAO_LOG_ERROR(Logger::ptr logger, std::string s);
    // void PANGTAO_LOG_FATAL(Logger::ptr logger, std::string s);
    //Logger::ptr PANGTAO_ROOT_LOGGER();

} // namespace PangTao
