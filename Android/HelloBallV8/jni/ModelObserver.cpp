#include <string.h>
#include <string>
#include <iostream>
#include "jni.h"
#include "ModelObserver.h"

void ModelObserver::push_env(JNIEnv *env, jobject obj)
{
   _env = env;
   _obj = obj;
}

const std::string& ModelObserver::display_list()
{
    return _displayList;
}

void ModelObserver::ball_count(int bc)
{
    _ballCount = bc;
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onBallCountChanged", "(I)V");
    if (mid == 0) {
        return;
    }

    _env->CallVoidMethod(_obj, mid, _ballCount);
}

int ModelObserver::ball_count()
{
    return _ballCount;
}

int ModelObserver::events_per_second()
{
    return _eventsPerSecond;
}

void ModelObserver::events_per_second(int evt)
{
    _eventsPerSecond = evt;
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onEventsPerSecond", "(I)V");
    if (mid == 0) {
        return;
    }

    _env->CallVoidMethod(_obj, mid, _eventsPerSecond);
}

void ModelObserver::display_list(const std::string& dl)
{
    _displayList = dl;
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onDisplayListChanged", "(Ljava/lang/String;)V");
    if (mid == 0) {
        return;
    }

    jstring dlJString = _env->NewStringUTF(_displayList.c_str());
    _env->CallVoidMethod(_obj, mid, dlJString);
    _env->DeleteLocalRef(dlJString);
}

void ModelObserver::phase(double phase_value)
{
    _phase = phase_value;
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onPhaseChanged", "(D)V");
    if (mid == 0) {
        return;
    }
    
    _env->CallVoidMethod(_obj, mid, phase_value);
}

double ModelObserver::phase()
{
    return _phase;
}

void ModelObserver::log(const std::string& message)
{
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onLog", "(Ljava/lang/String;)V");
    if (mid == 0) {
        return;
    }

    jstring dlJString = _env->NewStringUTF(message.c_str());
    _env->CallVoidMethod(_obj, mid, dlJString);
    _env->DeleteLocalRef(dlJString);
}

float ModelObserver::measureText(const std::string& fontName, int fontSize, bool bold, bool italic, const std::string& text)
{
    jclass cls = _env->GetObjectClass(_obj);
    jmethodID mid = _env->GetMethodID(cls, "onTextSize", "(Ljava/lang/String;IZZLjava/lang/String;)F");
    if (mid == 0) {
        return 0;
    }

    jstring jfont = _env->NewStringUTF(fontName.c_str());
    jstring jtext = _env->NewStringUTF(text.c_str());
    float width = _env->CallFloatMethod(_obj, mid, jfont, fontSize, bold, italic, jtext);
    _env->DeleteLocalRef(jfont);
    _env->DeleteLocalRef(jtext);
    return width;
}


