#ifndef MODELOBSERVER_INCLUDE
#define MODELOBSERVER_INCLUDE

#include <string.h>
#include <string>
#include <iostream>
#include "jni.h"
#include "IModelObserver.h"

class ModelObserver : public IModelObserver
{
protected:
    std::string _displayList;
    int _ballCount;
    int _eventsPerSecond;
    JNIEnv *_env;
    jobject _obj;
    double _phase;

public:
    void push_env(JNIEnv *env, jobject obj);
    const std::string& display_list();
    void ball_count(int bc);
    int ball_count();
    int events_per_second();
    void events_per_second(int evt);
    void display_list(const std::string& dl);
    
    void phase(double phase_value);
    double phase();

    // debug only
    void log(const std::string& message);
};

#endif //MODELOBSERVER_INCLUDE