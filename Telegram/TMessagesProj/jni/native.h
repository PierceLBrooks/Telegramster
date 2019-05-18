
#ifndef TELEGRAMSTER_NATIVE_H
#define TELEGRAMSTER_NATIVE_H

#include <jni.h>

#ifdef __cplusplus
extern "C" jint JNILoad(JavaVM *pjvm, void *reserved);
#else
extern jint JNILoad(JavaVM *pjvm, void *reserved);
#endif

#endif
