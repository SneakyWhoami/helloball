#include <string.h>
#include <string>
#include <iostream>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "v8Model.h"

using namespace v8;

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
    static_cast<IModelObserver*>(ptr)->ball_count(A);
    
    
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
    
    static_cast<IModelObserver*>(ptr)->display_list(foo);
    
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
    static_cast<IModelObserver*>(ptr)->events_per_second(A);

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

v8Model::v8Model(IModelObserver *observer)
{
    V8::InitializeICU();
    platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();
    
    // Create a new Isolate and make it the current one.
    isolate = Isolate::New();
    isolate->Enter();
    
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);
    
    model = observer;
    
    // This creates a new global template which manages our context and our customizations
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    global->SetInternalFieldCount(1);
    global->Set(String::NewFromUtf8(isolate, "ballCountChanged"), FunctionTemplate::New(isolate, ballCountChanged));
    global->Set(String::NewFromUtf8(isolate, "displayListChanged"), FunctionTemplate::New(isolate, displayListChanged));
    global->Set(String::NewFromUtf8(isolate, "eventsPerSecond"), FunctionTemplate::New(isolate, eventsPerSecond));
    global->Set(String::NewFromUtf8(isolate, "log"), FunctionTemplate::New(isolate, log));
    
    // Create a new context.
    v8::Handle<v8::Context> context = Context::New(isolate, NULL, global);
    context_.Reset(isolate, context);
    
    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);
    Local<Object> obj = global->NewInstance();
    obj->SetInternalField(0, External::New(isolate, model));
    context->Global()->Set(String::NewFromUtf8(isolate, "ModelObserver"), obj);
}

v8Model::~v8Model()
{
    // TODO???
    isolate->Exit();
    context_.Reset();
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
}

void v8Model::loadJS(const char* javascript)
{
    HandleScope handle_scope(isolate);
    Local<Context> context = Local<Context>::New(isolate, context_);
    
    // Enter this processor's context so all the remaining operations
    // take place there
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

void v8Model::startModel(double width, double height)
{
    HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = Local<Context>::New(isolate, context_);
    
    // Enter this processor's context so all the remaining operations
    // take place there
    Context::Scope context_scope(context);
    
    TryCatch try_catch;
    Local<Value> value = context->Global()->Get(String::NewFromUtf8(isolate, "modelStart"));
    Local<Function> func = v8::Handle<v8::Function>::Cast(value);
    Local<Value> args[2];
    Local<Value> js_result;

    args[0] = Number::New(isolate, width);
    args[1] = Number::New(isolate, height);

    js_result = func->Call(context->Global(), 2, args);
}

void v8Model::callMouseMethod(const char *mouseXXXMethod, double x, double y)
{
    HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = Local<Context>::New(isolate, context_);
    
    // Enter this processor's context so all the remaining operations
    // take place there
    Context::Scope context_scope(context);
    TryCatch try_catch;
    
    Local<Value> js_result;
    Local<Object> ModelObserver = Local<Object>::Cast(context->Global()->Get(String::NewFromUtf8(isolate, "ModelObserver")));
    Local<Object> appController = Local<Object>::Cast(ModelObserver->Get(String::NewFromUtf8(isolate, "appController")));
    Local<Function> func1 = Local<Function>::Cast(appController->Get(String::NewFromUtf8(isolate, mouseXXXMethod)));
    Local<Value> args1[2];
    args1[0] = Number::New(isolate, x);
    args1[1] = Number::New(isolate, y);
    js_result = func1->Call(context->Global(), 2, args1);
}


