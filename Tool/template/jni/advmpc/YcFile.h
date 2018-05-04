#pragma once

#include <stdio.h>
#include "unzip.h"

#define MAGIC "NISLFILE"

typedef struct _StringItem {
    /**
     * �ַ������ַ�������
     */
    unsigned int size;

    /**
     * �ַ����顣
     */
    unsigned char* str;
} StringItem;

typedef struct _YcHeader {
    /**
     * ħ���֡�
     */
    const char* magic;

    /**
     * �ļ�ͷ���ȡ�
     */
    unsigned int size;

    /**
     * Method�ṹ�ĸ�����
     */
    unsigned int methodSize;

    /**
     * Method���ݾ����ļ���ʼ��ƫ�ơ�
     */
    unsigned int methodOffset;

    /**
     * SeparatorData�ṹ�ĸ�����
     */
    unsigned int separatorDataSize;

    /**
     * SeparatorData���ݾ����ļ���ʼ��ƫ�ơ�
     */
    unsigned int separatorDataOffset;
    /**
     * StringItem���ܴ�С����ռ�����ֽ�
    */
    unsigned int stringItemSize;
    /**
     * StringItem��ƫ��
     */
    unsigned int stringItemOffest;
    /**
     * TypeItem�ṹ�Ĵ�С
     */
    unsigned int typeItemSize;
    /**
     * TypeItem �ṹ��ƫ��
     */
    unsigned int typeItemOffest;
    /**
     * method�ṹ�Ĵ�С
     */
    unsigned int methodItemSize;
    /**
     * methodItem�ṹ��ƫ��
     */
    unsigned  int methodItemOffest;
    /**
     * field�ṹ�Ĵ�С
     */
    unsigned int fieldItemSize;
    /**
     * fieldItem�ṹ��ƫ��
     */
    unsigned int fieldItemOffset;

} YcHeader;

typedef struct _AdvmpMethod {

    /**
     * ������method_id_item�е�������
     */
    int methodIndex;

    unsigned int size;

    /**
     * �����ķ��ʱ�־��
     */
    unsigned int accessFlag;

    /**
     * ���������ࡣ
     * ��"Ljava/lang/System;"�����ĸ�ʽ���档
     */
     char* definingClass;

    /**
     * ��������
     */
     char* name;

    /**
     * ����ǩ����
     */
     char* sig;
} AdvmpMethod;

typedef struct _SeparatorData {

    /**
     * ���������ʾ��ǰSeparatorData�ṹ��
     * SeparatorData�ṹ�����е�������
     */
    unsigned int methodIndex;

    /**
     * ��ǰ�ṹ�Ĵ�С��
     */
    unsigned int size;

    /**
     * �����ķ��ʱ�־��
     */
    unsigned int accessFlag;

    /**
     * ����������
     */
    unsigned int paramSize;

    /**
     * �Ĵ���������
     */
    unsigned int registerSize;

    /**
     * �����Ķ�����������
     */
    StringItem paramShortDesc;

    /**
     * ָ������Ԫ�ظ�����
     */
    unsigned int instSize;

    /**
     * ��ȡ�����ķ���ָ�
     */
    unsigned short* insts;

    /**
     * ������try...catch�顣
     */
    //public List<TryBlock> tryBlocks;

    /**
     * ������debug��Ϣ��
     */
    //public List<DebugItem> debugItems;

} SeparatorData;

struct MethodItem
{
    unsigned int classnameIdx;
    unsigned int protoIdx;
    unsigned int methodnameIdx;
};
struct FieldItem
{
    unsigned int classNameIdx;
    unsigned int fieldNameIdx;
    unsigned int typeIdx;

};

class YcFormat {
public:
    /**
     * �ļ�ͷ��
     */
    YcHeader header;

    /**
     * ����һ��ָ�����顣
     */
    AdvmpMethod** methods;

    char ** mStringItem;

    int *mTypeItem;
    struct MethodItem *mMethodItem;
    struct FieldItem *mFieldItem;

    /**
     * ���������ݡ�
     * ����һ��ָ�����顣
     */
    SeparatorData** separatorDatas;

    YcFormat();
    ~YcFormat();
};

//////////////////////////////////////////////////////////////////////////

class YcFile {
public:
    YcFile();
    YcFile(const char* filePath);

    ~YcFile();

    /**
     * ����Yc�ļ���
     * @return true�������ɹ���false������ʧ�ܡ�
     */
    //bool parse();

    /**
     * ����Yc�ļ���
     * @param[in] ycData yc�ļ����ݡ�
     * @param[in] dataSize ���ݳ��ȡ�
     * @return true�������ɹ���false������ʧ�ܡ�
     */
    bool parse(unsigned char* ycData, size_t dataSize);

    /**
     * �����ڴ��е�Yc�ļ���
     * @param[in] �������ڴ��е�Yc�ļ����ݡ�
     * @return true�������ɹ���false������ʧ�ܡ�
     */
    bool parse(unsigned char* bytes);

    YcFormat mYcFormat;

    /**
     * ���Separator���ݡ�
     * @param[in] index SeparatorData�����е�������
     * @return ����Separator���ݡ�
     */
    const SeparatorData * GetSeparatorData(int index);

private:
    char* mFilePath;
};

/**
 * �ͷ�NISL�ļ���
 * @param[in] filePath zip�ļ�·����
 * @param[out] buffer NISL�ļ������ݡ�
 * @return �ɹ�������NISL�ļ��ĳ��ȡ�ʧ�ܡ�����0��
 * @note buffer�������Ҫ��free�����ͷ��ڴ档
 */
uLong ReleaseYcFile(const char* zipPath, unsigned char** buffer);

/**
 * �򿪲�����yc�ļ���
 * @return true���ɹ���false��ʧ�ܡ�
 */
//bool OpenAndParseYc();
