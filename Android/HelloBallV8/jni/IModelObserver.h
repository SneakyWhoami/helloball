#ifndef IMODELOBSERVER_INCLUDE
#define IMODELOBSERVER_INCLUDE

#include <string>

class IModelObserver
{
public:
    virtual const std::string& display_list() = 0;
    virtual void ball_count(int bc) = 0;
    virtual int ball_count() = 0;
    virtual int events_per_second() = 0;
    virtual void events_per_second(int evt) = 0;
    virtual void display_list(const std::string& dl) = 0;
};

#endif //IMODELOBSERVER_INCLUDE