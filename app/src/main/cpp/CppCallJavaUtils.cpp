//
// Created by wanglei55 on 2019/4/30.
//

#include "CppCallJavaUtils.h"
#include "macro.h"

CppCallJavaUtils::CppCallJavaUtils(JavaVM *vm, JNIEnv *env, jobject instance) {
    this->vm = vm;
    this->env = env;
    this->instance = env->NewGlobalRef(instance);

    jclass jclazz = env->GetObjectClass(instance);
    onPrepareMethodID = env->GetMethodID(jclazz,"onPrepare","(I)V");
}

CppCallJavaUtils::~CppCallJavaUtils() {
    env->DeleteGlobalRef(instance);
}

void CppCallJavaUtils::onPrepare(int threadID,int isSuccess) {
    if (threadID==THREAD_MAIN){
        env->CallVoidMethod(instance,onPrepareMethodID);
    } else{
        JNIEnv *env;
        if (vm->AttachCurrentThread(&env,0)!=JNI_OK){
            return;
        }
        env->CallVoidMethod(instance,onPrepareMethodID,isSuccess);
        vm->DetachCurrentThread();
    }
}

