#!/bin/sh

# generates the header file for JNI
javah -o jni/DuktapeJNI.h -classpath out/production/HelloBall/ com.balsamiq.HelloBall.JavaModelWrapper

# this, instead, generates signatures for callbacks
#javap -p -s -classpath out/production/HelloBall/ com.balsamiq.HelloBall.JavaModelWrapper