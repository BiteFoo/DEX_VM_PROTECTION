#pragma once

#include <jni.h>
#include "YcFile.h"

/**
 * �ֽ����������
 * @param[in] Separator ���ݡ�
 * @param[in] env JNI������
 * @param[in] thiz ��ǰ����
 * @param[in] ...
 * @return 
 */
jvalue NISLvmInterpretPortable(YcFile* ycFile,int i, JNIEnv* env, jobject thiz, ...);
