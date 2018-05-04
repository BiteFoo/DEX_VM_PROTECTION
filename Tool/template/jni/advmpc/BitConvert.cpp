#include "stdafx.h"
#include "BitConvert.h"

// ���ֽ�����ת��Ϊ�ַ�����
char* ToString(unsigned char bytes[], size_t size) {
    char* str = (char*) calloc (size + 1, sizeof(char));
    if (NULL == str) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        str[i] = (char)(bytes[i]);
    }
    return str;
}

// ���ֽ�����ת��Ϊ�޷������Ρ�
unsigned int ToUInt(unsigned char bytes[], size_t size) {
    unsigned int ret = 0;
    unsigned  int sss=bytes[0];
    unsigned  int ss1s=bytes[1];
    unsigned  int sss2=bytes[2];
    unsigned  int sss12=bytes[3];
    unsigned  int ss3s=bytes[4];
    for (int i = 0, j = 0; (i < size) && (j < 4); i++, j++) {
        ret |= ((bytes[i] & 0xFF)<<(j*8));
    }
    return ret;
}

// ���ֽ�����ת��Ϊ�޷������Ρ�
unsigned int ToUInt(unsigned char bytes[], size_t size, unsigned int start) {
    unsigned int ret = 0;
    for (int i = start, j = 0; (i < size) && (j < 4); i++, j++) {
        ret |= ((bytes[i] & 0xFF) << j);
    }
    return ret;
}
