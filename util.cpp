#include "util.h"
#include "log.h"
#include <execinfo.h>
namespace PangTao
{
    pid_t GetThreadId()
    {
        return syscall(SYS_gettid);
    }
    uint32_t GetCoroutineId()
    {
        //return Coroutine::GetCoroutineId();
        return 0;
    }
    void Backtrace(std::vector<std::string> &bt, int size, int skip)
    {
        void **array = (void **)malloc((sizeof(void *) * size));
        size_t s = ::backtrace(array, size);
        char **strings = backtrace_symbols(array, s);
        if (!strings)
        {
            PANGTAO_LOG_ERROR(PANGTAO_ROOT_LOGGER, "backtrace synbols");
            return;
        }
        for (int i = skip; i < s; ++i)
        {
            bt.push_back(strings[i]);
        }
        free(strings);
        free(array);
    }
    std::string BacktraceToString(int size,int skip,const std::string& prefix)
    {
        std::vector<std::string> bt;
        Backtrace(bt,size,skip);
        std::stringstream ss;
        for(int i = 0; i<bt.size();++i){
            ss<<prefix<<bt[i]<<"    ";
        }
        return ss.str();
    }
} // namespace PangTao