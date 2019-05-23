#ifndef image_h
#define image_h

#include <jni.h>

#ifdef __cplusplus
extern "C" jint imageOnJNILoad(JavaVM *vm, JNIEnv *env, jclass *jclass_AnimatedFileDrawableStream, jmethodID *jclass_AnimatedFileDrawableStream_read, jmethodID *jclass_AnimatedFileDrawableStream_cancel);
#else
extern jint imageOnJNILoad(JavaVM *vm, JNIEnv *env, jclass *jclass_AnimatedFileDrawableStream, jmethodID *jclass_AnimatedFileDrawableStream_read, jmethodID *jclass_AnimatedFileDrawableStream_cancel);
#endif

#endif
