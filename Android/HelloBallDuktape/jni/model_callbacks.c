//
//  model_callbacks.cpp
//  duktape_helloball
//
//  Created by Michele Orsini on 17/02/15.
//  Copyright (c) 2015 Michele Orsini. All rights reserved.
//

#include "model_callbacks.h"
#include <android/log.h>

static JNIEnv *_env;
static jobject _obj;

const char* LOG_TAG = "duktapeJNI";

static duk_ret_t ballCountChanged(duk_context *ctx) {
    
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\n in ballCountChanged ...");
    int ballCount = (int)(duk_require_number(ctx, 0));
    
    jclass cls = (*_env)->GetObjectClass(_env, _obj);
    jmethodID mid = (*_env)->GetMethodID(_env, cls, "onBallCountChanged", "(I)V");
    if (mid == 0) {
        return 0;
    }
    
    (*_env)->CallVoidMethod(_env, _obj, mid, ballCount);
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, " in done \n");
    return 0;
}

//public void displayListChanged(String json)
static duk_ret_t displayListChanged(duk_context *ctx) {
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\n in displayListChanged ...");
    jclass cls = (*_env)->GetObjectClass(_env, _obj);
    jmethodID mid = (*_env)->GetMethodID(_env, cls, "onDisplayListChanged", "(Ljava/lang/String;)V");
    if (mid == 0) {
        return 0;
    }
    
    jstring dlJString = (*_env)->NewStringUTF(_env, duk_require_string(ctx, 0));
    (*_env)->CallVoidMethod(_env, _obj, mid, dlJString);
    (*_env)->DeleteLocalRef(_env, dlJString);
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, " in done \n");
    return 0;
}

//public void eventsPerSecond(int events)
static duk_ret_t eventsPerSecond(duk_context *ctx) {
__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "\n in displayListChanged ...");
    int eventsPerSecond = (int)(duk_require_number(ctx, 0));
    
    jclass cls = (*_env)->GetObjectClass(_env, _obj);
    jmethodID mid = (*_env)->GetMethodID(_env, cls, "onEventsPerSecond", "(I)V");
    if (mid == 0) {
        return 0;
    }
    
    (*_env)->CallVoidMethod(_env, _obj, mid, eventsPerSecond);
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, " in done \n");
    return 0;
}

// public void log(String message)
static duk_ret_t log_message(duk_context *ctx) {

    jclass cls = (*_env)->GetObjectClass(_env, _obj);
    jmethodID mid = (*_env)->GetMethodID(_env, cls, "onLog", "(Ljava/lang/String;)V");
    if (mid == 0) {
        return 0;
    }
    
    (*_env)->CallVoidMethod(_env, _obj, mid, duk_require_string(ctx, 0));

    return 0;
}

/*
 *  Module initialization
 */
static const duk_function_list_entry my_module_funcs[] = {
    { "ballCountChanged", ballCountChanged, 1 /*nargs*/ },
    { "displayListChanged", displayListChanged, 1 /*nargs*/ },
    { "eventsPerSecond", eventsPerSecond, 1 /*nargs*/ },
    { "log", log_message, 1 /*nargs*/ },
    { NULL, NULL, 0 }
};

//static const duk_number_list_entry my_module_consts[] = {
//    { "FLAG_FOO", (double) (1 << 0) },
//    { NULL, 0.0 }
//};

/* Init function name is dukopen_<modname>. */
duk_ret_t dukopen_my_module(duk_context *ctx) {
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, my_module_funcs);
//    duk_put_number_list(ctx, -1, my_module_consts);
    
    /* Return value is the module object.  It's up to the caller to decide
     * how to use the value, e.g. write to global object.
     */
    return 1;
}

void register_model_callbacks(duk_context *ctx, const char* module_name)
{
    duk_push_c_function(ctx, dukopen_my_module, 0 /*nargs*/);
    duk_call(ctx, 0);
    duk_put_global_string(ctx, module_name);
}

void push_env(JNIEnv *env, jobject obj)
{
   _env = env;
   _obj = obj;
}