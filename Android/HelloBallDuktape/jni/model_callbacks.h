//
//  model_callbacks.h
//  duktape_helloball
//
//  Created by Michele Orsini on 17/02/15.
//  Copyright (c) 2015 Michele Orsini. All rights reserved.
//

#ifndef __duktape_helloball__model_callbacks__
#define __duktape_helloball__model_callbacks__

#include "jni.h"
#include "duktape.h"

void register_model_callbacks(duk_context *ctx, const char* module_name);
void push_env(JNIEnv *env, jobject obj);

#endif /* defined(__duktape_helloball__model_callbacks__) */
