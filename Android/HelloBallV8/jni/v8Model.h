#ifndef V8_MODEL_INCLUDE
#define V8_MODEL_INCLUDE

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "IModelObserver.h"

using namespace v8;

class v8Model
{
protected:
    Platform* platform;
    Isolate* isolate;
    IModelObserver *model;
    Persistent<Context> context_;
    
    //    Local<Object> globalObject;
public:
    v8Model(IModelObserver *observer);
    ~v8Model();
        
    void loadJS(const char* javascript);
    void startModel(double width, double height);
    void callMouseMethod(const char *mouseXXXMethod, double x, double y);
    
    const std::string& display_list();
    int ball_count();
    int events_per_second();
};

#endif // V8_MODEL_INCLUDE