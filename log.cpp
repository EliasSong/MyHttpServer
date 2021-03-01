#include "log.h"
namespace PangTao {

// Logger::ptr PANGTAO_ROOT_LOGGER()
// {
//     return LoggerManager::getInstance()->getRoot();
// }
// void PANGTAO_LOG_DEBUG(Logger::ptr logger, std::string s)
// {
//     LogEvent::ptr event(new LogEvent(logger, LogLevel::DEBUG, __FILE__,
//     __LINE__, 0, GetThreadId(), GetCoroutineId(), time(0))); event->getSS()
//     << s; logger->log(event);
// }

// void PANGTAO_LOG_INFO(Logger::ptr logger, std::string s)
// {
//     LogEvent::ptr event(new LogEvent(logger, LogLevel::INFO, __FILE__,
//     __LINE__, 0, GetThreadId(), GetCoroutineId(), time(0))); event->getSS()
//     << s; logger->log(event);
// }

// void PANGTAO_LOG_WARN(Logger::ptr logger, std::string s)
// {
//     LogEvent::ptr event(new LogEvent(logger, LogLevel::WARN, __FILE__,
//     __LINE__, 0, GetThreadId(), GetCoroutineId(), time(0))); event->getSS()
//     << s; logger->log(event);
// }

// void PANGTAO_LOG_FATAL(Logger::ptr logger, std::string s)
// {
//     LogEvent::ptr event(new LogEvent(logger, LogLevel::FATAL, __FILE__,
//     __LINE__, 0, GetThreadId(), GetCoroutineId(), time(0))); event->getSS()
//     << s; logger->log(event);
// }

// void PANGTAO_LOG_ERROR(Logger::ptr logger, std::string s)
// {
//     LogEvent::ptr event(new LogEvent(logger, LogLevel::ERROR, __FILE__,
//     __LINE__, 0, GetThreadId(), GetCoroutineId(), time(0))); event->getSS()
//     << s; logger->log(event);
// }

const char *LogLevel::toString(LogLevel::Level level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
            break;
        case LogLevel::WARN:
            return "WARN";
            break;
        case LogLevel::INFO:
            return "INFO";
            break;
        case LogLevel::ERROR:
            return "ERROR";
            break;
        case LogLevel::FATAL:
            return "FATAL";
            break;
        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}

class MessageFormatItem : public LogFormatter::FormatItem {
   public:
    MessageFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};
class LevelFormatItem : public LogFormatter::FormatItem {
   public:
    LevelFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::toString(level);
    }
};
class ElapseFormatItem : public LogFormatter::FormatItem {
   public:
    ElapseFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};
class UsernameFormatItem : public LogFormatter::FormatItem {
   public:
    UsernameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << logger->getName();
    }
};
class ThreadIdFormatItem : public LogFormatter::FormatItem {
   public:
    ThreadIdFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};
class CoroutineIdFormatItem : public LogFormatter::FormatItem {
   public:
    CoroutineIdFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getCoroutineId();
    }
};
class DateTimeFormatItem : public LogFormatter::FormatItem {
   public:
    DateTimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S") {
        m_format = format;
        if (m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);

        os << buf;
    }

   private:
    std::string m_format;
};
class FilenameFormatItem : public LogFormatter::FormatItem {
   public:
    FilenameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getFile();
    }
};
class LineFormatItem : public LogFormatter::FormatItem {
   public:
    LineFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getLine();
    }
};
class NewLineFormatItem : public LogFormatter::FormatItem {
   public:
    NewLineFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};
class StringFormatItem : public LogFormatter::FormatItem {
   public:
    StringFormatItem(const std::string &str) : m_string(str) {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << m_string;
    }

   private:
    std::string m_string;
};
class TabFormatItem : public LogFormatter::FormatItem {
   public:
    TabFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << "\t";
    }

   private:
    std::string m_string;
};
LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
                   const char *file, int32_t line, uint32_t elapse,
                   uint32_t thread_id, uint32_t coroutine_id, uint64_t time) {
    m_file = file;
    m_line = line;
    m_elapse = elapse;
    m_threadId = thread_id;
    m_coroutineId = coroutine_id;
    m_time = time;
    m_logger = logger;
    m_level = level;
}
Logger::Logger(const std::string &name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter("%d%T%t[%F]%T[%p] %f [%c]:%l%T%m %n"));
}
void Logger::addAppender(LogAppender::ptr appender) {
    Mutex::Lock lock(m_mutex);
    if (!appender->getFormatter()) {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender) {
    Mutex::Lock lock(m_mutex);
    for (auto i = m_appenders.begin(); i != m_appenders.end(); ++i) {
        if (*i = appender) {
            m_appenders.erase(i);
            break;
        }
    }
}
void Logger::log(LogEvent::ptr event) {
    Mutex::Lock lock(m_mutex);
    if (event->getLevel() >= m_level) {
        auto self = shared_from_this();
        for (auto &i : m_appenders) {
            i->log(self, event->getLevel(), event);
           
        }
    }
}
void Logger::setFormatter(const std::string &formatter) {
    Mutex::Lock lock(m_mutex);
    m_formatter.reset(new LogFormatter(formatter));
    for (auto &i : m_appenders) {
        i->setFormatter(m_formatter);
    }
}
void LogAppender::setFormatter(LogFormatter::ptr formatter) {
    Mutex::Lock lock(m_mutex);
    m_formatter = formatter;
}
LogFormatter::ptr LogAppender::getFormatter() {
    // Mutex::Lock lock(m_mutex);
    return m_formatter;
}

FileLogAppender::FileLogAppender(const std::string &filename)
    : m_filename(filename) {
    reopen();
}
void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                          LogEvent::ptr event) {
    if (level >= m_level) {
        Mutex::Lock lock(m_mutex);
        m_filestream << m_formatter->format(logger, level, event)<<std::flush;
    }
};
bool FileLogAppender::reopen() {
    Mutex::Lock lock(m_mutex);
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
void StdoutLogAppender::log(std::shared_ptr<Logger> logger,
                            LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        Mutex::Lock lock(m_mutex);
        std::cout << m_formatter->format(logger, level, event);
    }
};

LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern) {
    init();
}
std::string LogFormatter::format(std::shared_ptr<Logger> logger,
                                 LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto &i : m_items) {
        i->format(ss, logger, level, event);
    }
    return ss.str();
}
void LogFormatter::init() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (int i = 0; i < m_pattern.size(); ++i) {
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        if ((i + 1) < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }
        int n = i + 1;
        int fmt_status = 0;
        int fmt_begin = 0;

        std::string str;
        std::string fmt;
        while (n < m_pattern.size()) {
            if (!fmt_status && !isalpha(m_pattern[n]) && m_pattern[n] != '{' &&
                m_pattern[n] != '}') {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1;
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            }
            if (fmt_status == 1) {
                if (m_pattern[n] = '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if (n == m_pattern.size()) {
                if (str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }
        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        } else if (fmt_status == 1) {
            std::cout << "Pattern parse error: " << m_pattern << " - "
                      << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<Pattern Error>>", fmt, 0));
        }
    }
    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::unordered_map<
        std::string, std::function<FormatItem::ptr(const std::string &str)>>
        s_format_items = {
            {"m",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new MessageFormatItem(fmt));
             }},
            {"p",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new LevelFormatItem(fmt));
             }},
            {"r",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new ElapseFormatItem(fmt));
             }},
            {"c",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new UsernameFormatItem(fmt));
             }},
            {"t",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new ThreadIdFormatItem(fmt));
             }},
            {"n",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new NewLineFormatItem(fmt));
             }},
            {"d",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new DateTimeFormatItem(fmt));
             }},
            {"f",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new FilenameFormatItem(fmt));
             }},
            {"l",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new LineFormatItem(fmt));
             }},
            {"T",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new TabFormatItem(fmt));
             }},
            {"F",
             [](const std::string &fmt) {
                 return FormatItem::ptr(new CoroutineIdFormatItem(fmt));
             }},
        };
    for (auto &i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(
                FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(
                    "<<error format %" + std::get<0>(i) + ">>")));
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
        // std::cout << std::get<0>(i) << " - " << std::get<1>(i) << " - " <<
        // std::get<2>(i) << std::endl;
    }
}
Logger::ptr LoggerManager::getLogger(const std::string &str) {
    Mutex::Lock lock(m_mutex);
    if (m_loggers.find(str) == m_loggers.end()) {
        std::cerr << "Logger unexist" << std::endl;
        return nullptr;
    }
    return m_loggers[str];
}
void LoggerManager::registerLogger(const std::string &loggerName,
                                   Logger::ptr logger) {
    Mutex::Lock lock(m_mutex);
    if (m_loggers.find(loggerName) != m_loggers.end()) {
        std::cerr << "Logger existed" << std::endl;
    }
    m_loggers[loggerName] = logger;
}
Mutex LoggerManager::m_mutex;
LoggerManager::ptr LoggerManager::instance = nullptr;

}  // namespace PangTao
