#include "DuktapeJNI.h"
#include "duktape.h"
#include "model_callbacks.h"

static duk_context *ctx = NULL;

static void internalStop() {
    duk_destroy_heap(ctx);
    ctx = NULL;
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_loadJS(JNIEnv *env, jobject obj, jstring js) {
    register_model_callbacks(ctx, "ModelObserver");
    push_env(env, obj);
    jboolean copy;
    const char* javascript = (*env)->GetStringUTFChars(env, js, &copy);
    if (duk_peval_string(ctx, javascript) != 0) {
        printf("Failed to load javascript.\n");
        internalStop();
        return;
    }

    duk_pop(ctx); // ignore result 
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_startModel
  (JNIEnv *env, jobject obj, jdouble width, jdouble height)
{
    push_env(env, obj);
    duk_get_prop_string(ctx, -1, "modelStart");
    duk_push_number(ctx, width);
    duk_push_number(ctx, height);
    if (duk_pcall(ctx, 2) != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        internalStop();
        return;
    }
    
    duk_pop(ctx); // ignore result 
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_initialize
(JNIEnv *env, jobject obj) {
    push_env(env, obj);
    ctx = duk_create_heap_default();
    if (!ctx) {
        printf("Failed to create a Duktape heap.\n");
        internalStop();
        return;
    }

    duk_push_global_object(ctx);
}

void internalCallToMouseXXX(const char *mouseXXXMethod, double x, double y)
{
    duk_get_prop_string(ctx, -1, "ModelObserver");
    duk_get_prop_string(ctx, -1, "appController");
    duk_get_prop_string(ctx, -1, mouseXXXMethod);
    duk_push_number(ctx, x);
    duk_push_number(ctx, y);
    if (duk_pcall(ctx, 2) != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        internalStop();
        return;
    }
    duk_pop(ctx);  /* ignore result */
    duk_pop(ctx); // restore duk_get_prop_string(ctx, -1, "appController");
    duk_pop(ctx); // restore duk_get_prop_string(ctx, -1, "ModelObserver");   
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_onMouseDown(JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    push_env(env, obj);
    internalCallToMouseXXX("mouseDown", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_onMouseMove
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    push_env(env, obj);
    internalCallToMouseXXX("mouseMove", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_onMouseUp
  (JNIEnv *env, jobject obj, jdouble x, jdouble y)
{
    push_env(env, obj);
    internalCallToMouseXXX("mouseUp", x, y);
}

JNIEXPORT void JNICALL Java_com_balsamiq_HelloBallDuktape_JavaModelWrapper_stopModel
(JNIEnv *env, jobject obj) {
    push_env(env, obj);
    internalStop();
}
