#pragma once

#include <jni.h>
#include "YcFile.h"

/**
 * 字节码解释器。
 * @param[in] Separator 数据。
 * @param[in] env JNI环境。
 * @param[in] thiz 当前对象。
 * @param[in] ...
 * @return 
 */
jvalue NISLvmInterpretPortable(YcFile* ycFile,int i, JNIEnv* env, jobject thiz, ...);
