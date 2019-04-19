#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "image.h"
#include "libtgvoip/client/android/tg_voip_jni.h"
#include "native-lib.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int registerNativeTgNetFunctions(JavaVM *vm, JNIEnv *env);
int videoOnJNILoad(JavaVM *vm, JNIEnv *env);

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env = 0;
    srand(time(NULL));

	if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
	    LOGD("JNI", "JNI_VERSION_1_6");
		return -1;
	}

    if (imageOnJNILoad(vm, env) != JNI_TRUE) {
        LOGD("JNI", "imageOnJNILoad");
        return -1;
    }

    if (videoOnJNILoad(vm, env) != JNI_TRUE) {
        LOGD("JNI", "videoOnJNILoad");
        return -1;
    }

    if (registerNativeTgNetFunctions(vm, env) != JNI_TRUE) {
        LOGD("JNI", "registerNativeTgNetFunctions");
        return -1;
    }

    tgvoipRegisterNatives(env);

	return JNIOnLoad(vm, reserved);
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {

}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_aesIgeEncryption(JNIEnv *env, jclass clazz, jobject buffer, jbyteArray key, jbyteArray iv, jboolean encrypt, jint offset, jint length) {
    jbyte *what = ((jbyte *)env->GetDirectBufferAddress( buffer)) + offset;
    unsigned char *keyBuff = (unsigned char *)env->GetByteArrayElements( key, NULL);
    unsigned char *ivBuff = (unsigned char *)env->GetByteArrayElements( iv, NULL);

    AES_KEY akey;
    if (!encrypt) {
        AES_set_decrypt_key(keyBuff, 32 * 8, &akey);
        AES_ige_encrypt((unsigned char *)what, (unsigned char *)what, length, &akey, ivBuff, AES_DECRYPT);
    } else {
        AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
        AES_ige_encrypt((unsigned char *)what, (unsigned char *)what, length, &akey, ivBuff, AES_ENCRYPT);
    }
    env->ReleaseByteArrayElements( key, (jbyte *)keyBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( iv, (jbyte *)ivBuff, 0);
}

JNIEXPORT jint Java_org_telegramster_messenger_Utilities_pbkdf2(JNIEnv *env, jclass clazz, jbyteArray password, jbyteArray salt, jbyteArray dst, jint iterations) {
    jbyte *passwordBuff = env->GetByteArrayElements( password, NULL);
    size_t passwordLength = (size_t) env->GetArrayLength( password);
    jbyte *saltBuff = env->GetByteArrayElements( salt, NULL);
    size_t saltLength = (size_t) env->GetArrayLength( salt);
    jbyte *dstBuff = env->GetByteArrayElements( dst, NULL);
    size_t dstLength = (size_t) env->GetArrayLength( dst);

    int result = PKCS5_PBKDF2_HMAC((char *) passwordBuff, passwordLength, (uint8_t *) saltBuff, saltLength, (unsigned int) iterations, EVP_sha512(), dstLength, (uint8_t *) dstBuff);

    env->ReleaseByteArrayElements( password, passwordBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( salt, saltBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( dst, dstBuff, 0);

    return result;
}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_aesCtrDecryption(JNIEnv *env, jclass clazz, jobject buffer, jbyteArray key, jbyteArray iv, jint offset, jint length) {
    jbyte *what = ((jbyte *)env->GetDirectBufferAddress( buffer)) + offset;
    unsigned char *keyBuff = (unsigned char *)env->GetByteArrayElements( key, NULL);
    unsigned char *ivBuff = (unsigned char *)env->GetByteArrayElements( iv, NULL);

    AES_KEY akey;
    unsigned int num = 0;
    uint8_t count[16];
    memset(count, 0, 16);
    AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
    AES_ctr128_encrypt((unsigned char *)what, (unsigned char *)what, length, &akey, ivBuff, count, &num);
    env->ReleaseByteArrayElements( key, (jbyte *)keyBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( iv, (jbyte *)ivBuff, JNI_ABORT);
}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_aesCtrDecryptionByteArray(JNIEnv *env, jclass clazz, jbyteArray buffer, jbyteArray key, jbyteArray iv, jint offset, jint length, jint fileOffset) {
    unsigned char *bufferBuff = (unsigned char *) env->GetByteArrayElements( buffer, NULL);
    unsigned char *keyBuff = (unsigned char *) env->GetByteArrayElements( key, NULL);
    unsigned char *ivBuff = (unsigned char *) env->GetByteArrayElements( iv, NULL);

    AES_KEY akey;
    uint8_t count[16];
    AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
    unsigned int num = (unsigned int) (fileOffset % 16);

    int o = fileOffset / 16;
    ivBuff[15] = (uint8_t) (o & 0xff);
    ivBuff[14] = (uint8_t) ((o >> 8) & 0xff);
    ivBuff[13] = (uint8_t) ((o >> 16) & 0xff);
    ivBuff[12] = (uint8_t) ((o >> 24) & 0xff);
    AES_encrypt(ivBuff, count, &akey);

    o = (fileOffset + 15) / 16;
    ivBuff[15] = (uint8_t) (o & 0xff);
    ivBuff[14] = (uint8_t) ((o >> 8) & 0xff);
    ivBuff[13] = (uint8_t) ((o >> 16) & 0xff);
    ivBuff[12] = (uint8_t) ((o >> 24) & 0xff);

    AES_ctr128_encrypt(bufferBuff + offset, bufferBuff + offset, length, &akey, ivBuff, count, &num);

    env->ReleaseByteArrayElements( key, (jbyte *) keyBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( iv, (jbyte *) ivBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( buffer, (jbyte *) bufferBuff, 0);
}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_aesCbcEncryptionByteArray(JNIEnv *env, jclass clazz, jbyteArray buffer, jbyteArray key, jbyteArray iv, jint offset, jint length, jint fileOffset, jint encrypt) {
    unsigned char *bufferBuff = (unsigned char *) env->GetByteArrayElements( buffer, NULL);
    unsigned char *keyBuff = (unsigned char *) env->GetByteArrayElements( key, NULL);
    unsigned char *ivBuff = (unsigned char *) env->GetByteArrayElements( iv, NULL);

    AES_KEY akey;
    if (encrypt) {
        AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
    } else {
        AES_set_decrypt_key(keyBuff, 32 * 8, &akey);

        if (fileOffset != 0) {
            int o = (fileOffset + 15) / 16;
            ivBuff[15] = (uint8_t) (o & 0xff);
            ivBuff[14] = (uint8_t) ((o >> 8) & 0xff);
            ivBuff[13] = (uint8_t) ((o >> 16) & 0xff);
            ivBuff[12] = (uint8_t) ((o >> 24) & 0xff);
        }
    }

    AES_cbc_encrypt(bufferBuff, bufferBuff, length, &akey, ivBuff, encrypt);

    env->ReleaseByteArrayElements( buffer, (jbyte *) bufferBuff, 0);
    env->ReleaseByteArrayElements( key, (jbyte *) keyBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( iv, (jbyte *) ivBuff, JNI_ABORT);
}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_aesCbcEncryption(JNIEnv *env, jclass clazz, jobject buffer, jbyteArray key, jbyteArray iv, jint offset, jint length, jint encrypt) {
    unsigned char *bufferBuff = ((unsigned char *) env->GetDirectBufferAddress( buffer)) + offset;
    unsigned char *keyBuff = (unsigned char *) env->GetByteArrayElements( key, NULL);
    unsigned char *ivBuff = (unsigned char *) env->GetByteArrayElements( iv, NULL);

    AES_KEY akey;
    if (encrypt) {
        AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
    } else {
        AES_set_decrypt_key(keyBuff, 32 * 8, &akey);
    }

    AES_cbc_encrypt(bufferBuff + offset, bufferBuff + offset, length, &akey, ivBuff, encrypt);

    env->ReleaseByteArrayElements( key, (jbyte *) keyBuff, JNI_ABORT);
    env->ReleaseByteArrayElements( iv, (jbyte *) ivBuff, JNI_ABORT);
}

int64_t listdir(const char *fileName, int32_t mode, int32_t docType, int64_t time) {
    int64_t value = 0;
    DIR *dir;
    struct stat attrib;
    if ((dir = opendir(fileName)) != NULL) {
        char buff[4096];
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            char *name = entry->d_name;
            size_t len = strlen(name);
            if (name[0] == '.') {
                continue;
            }
            if ((docType == 1 || docType == 2) && len > 4) {
                if (name[len - 4] == '.' && (
                        ((name[len - 3] == 'm' || name[len - 3] == 'M') && (name[len - 2] == 'p' || name[len - 2] == 'P') && (name[len - 1] == '3')) ||
                        ((name[len - 3] == 'm' || name[len - 3] == 'M') && (name[len - 2] == '4') && (name[len - 1] == 'a' || name[len - 1] == 'A'))
                )) {
                    if (docType == 1) {
                        continue;
                    }
                } else if (docType == 2) {
                    continue;
                }
            }
            strncpy(buff, fileName, 4095);
            strncat(buff, "/", 4095);
            strncat(buff, entry->d_name, 4095);
            if (entry->d_type == DT_DIR) {
                value += listdir(buff, mode, docType, time);
            } else {
                stat(buff, &attrib);
                if (mode == 0) {
                    value += 512 * attrib.st_blocks;
                } else if (mode == 1) {
                    if (attrib.st_atim.tv_sec != 0) {
                        if (attrib.st_atim.tv_sec < time) {
                            remove(buff);
                        }
                    } else {
                        if (attrib.st_mtim.tv_sec < time) {
                            remove(buff);
                        }
                    }
                }
            }
        }
        closedir(dir);
    }
    return value;
}

JNIEXPORT jlong Java_org_telegramster_messenger_Utilities_getDirSize(JNIEnv *env, jclass clazz, jstring path, jint docType) {
    const char *fileName = env->GetStringUTFChars( path, NULL);
    jlong value = listdir(fileName, 0, docType, 0);
    env->ReleaseStringUTFChars( path, fileName);
    return value;
}

JNIEXPORT void Java_org_telegramster_messenger_Utilities_clearDir(JNIEnv *env, jclass clazz, jstring path, jint docType, jlong time) {
    const char *fileName = env->GetStringUTFChars( path, NULL);
    listdir(fileName, 1, docType, time);
    env->ReleaseStringUTFChars( path, fileName);
}

#ifdef __cplusplus
}
#endif
