#ifndef UTILS_H
#define UTILS_H
#include <chrono>

class Utils
{
public:
    static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).
            count();

    }
    static int getCurTimestamp_second()// 获取秒级时间戳（10位）
    {
        int64_t curTimestamp = getCurTimestamp();

        return int(curTimestamp/1000);
    }

};

#endif // UTILS_H
