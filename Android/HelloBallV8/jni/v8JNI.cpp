#include <string.h>
#include <string>
#include <iostream>
#include <android/log.h>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "jni.h"
#include "v8Model.h"
#include "ModelObserver.h"
#include "v8JNI.h"

using namespace v8;

v8Model *v8Wrapper;
ModelObserver *observer;

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_loadJS(JNIEnv *env, jobject obj, jstring js, jstring jsName) {
    observer->push_env(env, obj);
    jboolean copy;
    const char* javascript = env->GetStringUTFChars(js, &copy);
    const char* javascriptName = env->GetStringUTFChars(jsName, &copy);
    v8Wrapper->loadJS(javascript, javascriptName);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_startModel
  (JNIEnv *env, jobject obj, jdouble width, jdouble height)
{
    observer->push_env(env, obj);
    v8Wrapper->startModel(width, height);
}


JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_initialize
(JNIEnv *env, jobject obj) {
    observer = new ModelObserver();
    v8Wrapper = new v8Model(observer);
    observer->push_env(env, obj);
}

void internalCallToMouseXXX(const char *mouseXXXMethod, double x, double y)
{
    v8Wrapper->callMouseMethod(mouseXXXMethod, x, y);
}

extern "C" {

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_onMouseDown(JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    observer->push_env(env, obj);
    internalCallToMouseXXX("mouseDown", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_onMouseMove
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    observer->push_env(env, obj);
    internalCallToMouseXXX("mouseMove", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_onMouseUp
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    observer->push_env(env, obj);
    internalCallToMouseXXX("mouseUp", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBall_JavaModelV8_stopModel
(JNIEnv *env, jobject obj) {
    observer->push_env(env, obj);
    delete v8Wrapper;
}
}
