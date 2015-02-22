#include <string.h>
#include <string>
#include <iostream>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "v8JNI.h"

using namespace v8;

class ModelObserver
{
protected:
    std::string _displayList;
    int _ballCount;
    int _eventsPerSecond;

public:
    const std::string& display_list()
    {
        return _displayList;
    }

    void ball_count(int bc)
    {
        _ballCount = bc;
    }
    
    int ball_count()
    {
        return _ballCount;
    }

    int events_per_second()
    {
        return _eventsPerSecond;
    }
    
    void events_per_second(int evt)
    {
        _eventsPerSecond = evt;
    }
    
    void display_list(const std::string& dl)
    {
        _displayList = dl;
    }
};

void ballCountChanged(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());
    
    if (args.Length() < 1) {
        return;
    }
    
    int A = args[0]->Int32Value();
    
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<ModelObserver*>(ptr)->ball_count(A);
    
    
    std::cout << "ballCountChanged:" << A << std::endl;
}

void displayListChanged(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());
    
    if (args.Length() < 1) {
        return;
    }
    v8::String::Utf8Value param1(args[0]->ToString());
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    
    // convert it to string
    std::string foo = std::string(*param1);
    
    static_cast<ModelObserver*>(ptr)->display_list(foo);
    
    std::cout << "displayListChanged:" << foo << std::endl;
}

void eventsPerSecond(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());
    
    if (args.Length() < 1) {
        return;
    }
    int A = args[0]->Int32Value();
    
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    static_cast<ModelObserver*>(ptr)->events_per_second(A);

    std::cout << "eventsPerSecond:" << A << std::endl;
}

void log(const FunctionCallbackInfo<Value>& args)
{
    EscapableHandleScope scope(args.GetIsolate());
    
    if (args.Length() < 1) {
        return;
    }
    Local<String> dl = args[0]->ToString();
    String::Utf8Value dlValue(dl);
    std::cout << "log:" << *dl << std::endl;
}


class v8JNI
{
protected:
    Platform* platform;
    Isolate* isolate;
    JNIEnv *_env;
    jobject _obj;
    ModelObserver *model;
    Handle<Context> context;
public:
    v8JNI()
    {
        V8::InitializeICU();
        platform = platform::CreateDefaultPlatform();
        V8::InitializePlatform(platform);
        V8::Initialize();

        // Create a new Isolate and make it the current one.
        isolate = Isolate::New();
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);

        model = new ModelObserver();

        // This creates a new global template which manages our context and our customizations
        Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
        global->SetInternalFieldCount(1);
        global->Set(String::NewFromUtf8(isolate, "ballCountChanged"), FunctionTemplate::New(isolate, ballCountChanged));
        global->Set(String::NewFromUtf8(isolate, "displayListChanged"), FunctionTemplate::New(isolate, displayListChanged));
        global->Set(String::NewFromUtf8(isolate, "eventsPerSecond"), FunctionTemplate::New(isolate, eventsPerSecond));
        global->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, log));

        // Create a new context.
        context = Context::New(isolate, NULL, global);

        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        Local<Object> obj = global->NewInstance();
        obj->SetInternalField(0, External::New(isolate, model));
        context->Global()->Set(String::NewFromUtf8(isolate, "ModelObserver"), obj);
    }
    
    ~v8JNI()
    {
    // TODO???
//        context.Dispose();
        isolate->Dispose();
        V8::Dispose();
        V8::ShutdownPlatform();
        delete platform;
        delete model;
    }
    
    void push_env(JNIEnv *env, jobject obj)
    {
       _env = env;
       _obj = obj;
    }
    
    void loadJS(const char* javascript)
    {
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);
        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        // Compile the source code.
        TryCatch try_catch;
        Local<String> scriptName = String::NewFromUtf8(isolate, "model.js");
        ScriptOrigin origin = ScriptOrigin(scriptName);


        //compile script to binary code - JIT
        Handle<Script> script = Script::Compile(String::NewFromUtf8(isolate, javascript), &origin);

        //check if we got problems on compilation
        if (script.IsEmpty()) {
            // TODO???
        }
        else
        {
            //no errors , let's continue
            script->Run();
        }    
    }
    
    void startModel(double width, double height)
    {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Context::Scope context_scope(context);
        TryCatch try_catch;

        Local<Object> globalObject = context->Global();
        Local<Value> value = globalObject->Get(String::NewFromUtf8(isolate, "modelStart"));
        Local<Function> func = v8::Handle<v8::Function>::Cast(value);
        Local<Value> args[2];
        Local<Value> js_result;

        args[0] = Number::New(isolate, width);
        args[1] = Number::New(isolate, height);

        js_result = func->Call(globalObject, 2, args);
    }
    
    void callMouseMethod(const char *mouseXXXMethod, double x, double y)
    {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Context::Scope context_scope(context);
        TryCatch try_catch;
        
        Local<Value> js_result;
        Local<Object> globalObject = context->Global();
        Local<Object> ModelObserver = Local<Object>::Cast(globalObject->Get(String::NewFromUtf8(isolate, "ModelObserver")));
        Local<Object> appController = Local<Object>::Cast(ModelObserver->Get(String::NewFromUtf8(isolate, "appController")));
        Local<Function> func1 = Local<Function>::Cast(appController->Get(String::NewFromUtf8(isolate, mouseXXXMethod)));
        Local<Value> args1[2];
        args1[0] = Number::New(isolate, x);
        args1[1] = Number::New(isolate, y);
        js_result = func1->Call(globalObject, 2, args1);
    }
};

v8JNI *v8Wrapper;


JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_loadJS(JNIEnv *env, jobject obj, jstring js) {
    v8Wrapper->push_env(env, obj);
    jboolean copy;
    const char* javascript = env->GetStringUTFChars(js, &copy);
    v8Wrapper->loadJS(javascript);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_startModel
  (JNIEnv *env, jobject obj, jdouble width, jdouble height)
{
    v8Wrapper->push_env(env, obj);
    v8Wrapper->startModel(width, height);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_initialize
(JNIEnv *env, jobject obj) {
    v8Wrapper = new v8JNI();
    v8Wrapper->push_env(env, obj);
}

void internalCallToMouseXXX(const char *mouseXXXMethod, double x, double y)
{
    v8Wrapper->callMouseMethod(mouseXXXMethod, x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_onMouseDown(JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    v8Wrapper->push_env(env, obj);
    internalCallToMouseXXX("mouseDown", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_onMouseMove
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    v8Wrapper->push_env(env, obj);
    internalCallToMouseXXX("mouseMove", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_onMouseUp
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    v8Wrapper->push_env(env, obj);
    internalCallToMouseXXX("mouseUp", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelWrapper_stopModel
(JNIEnv *env, jobject obj) {
    v8Wrapper->push_env(env, obj);
    delete v8Wrapper;
}