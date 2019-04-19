
#include "native.h"
#include "native-lib.hpp"

jint JNILoad(JavaVM *pjvm, void *reserved)
{
    return JNIOnLoad(pjvm, reserved);
}
