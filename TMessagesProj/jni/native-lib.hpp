
#ifndef TELEGRAMSTER_NATIVE_LIB_HPP
#define TELEGRAMSTER_NATIVE_LIB_HPP

#include <jni.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

// SFML
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Log.hpp"

#ifdef __cplusplus
extern "C" {
#endif

jint JNIOnLoad(JavaVM *pjvm, void *reserved);
jobject newString(JNIEnv *env, const char *string);
jclass findClass(const char *name);
jclass findClassWithEnv(JNIEnv *env, const char *name);
JNIEnv *getEnv();
JavaVM *getJvm();
int attach(JavaVM** jvm, JNIEnv** env);
int detach(JavaVM* jvm, int attachedHere);
int vibrate(sf::Time duration);
int play(const std::string& song);
int pause();
bool checkFlag(jobject name);

#ifdef __cplusplus
};
#endif

#endif