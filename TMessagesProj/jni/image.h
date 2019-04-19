#ifndef image_h
#define image_h

#include <jni.h>


#ifdef __cplusplus
extern "C" {
#endif

jint imageOnJNILoad(JavaVM *vm, JNIEnv *env);

#ifdef __cplusplus
}
#endif

#endif
