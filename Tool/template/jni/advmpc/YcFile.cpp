#include "stdafx.h"
#include "log.h"
#include "BitConvert.h"
#include "io.h"
#include "Globals.h"
#include "unzip.h"
#include "YcFile.h"


//////////////////////////////////////////////////////////////////////////
// YcFile

YcFile::YcFile() : mFilePath(NULL) {}

YcFile::YcFile(const char* filePath)
    : mFilePath(NULL)
{
    mFilePath = strdup(filePath);
}

YcFile::~YcFile() {
    if (NULL != mFilePath) {
        free(mFilePath);
    }
}


// 解析NISL文件。
bool YcFile::parse(unsigned char* ycData, size_t dataSize) {
    unsigned char* p = ycData;
    unsigned char* end = ycData + dataSize - 1;

    // 校验魔术字。
    char *magic=ToString(p,8);
    //char* magic = ToString(p, 6);
    if (0 != strcmp(magic, MAGIC)) {
        MY_LOG_WARNING("invalid magic");
        free(magic);
        return false;
    }
    free(magic);

    p += 8;

    mYcFormat.header.magic = MAGIC;

    mYcFormat.header.size = ToUInt(p, dataSize);//将字节数组转换成无符号整形，文件头的大小
    MY_LOG_INFO("header.size:%u", mYcFormat.header.size);
    p += sizeof(mYcFormat.header.size);

    mYcFormat.header.methodSize = ToUInt(p, dataSize);
    MY_LOG_INFO("header.methodSize:%u", mYcFormat.header.methodSize);
    p += sizeof(mYcFormat.header.methodSize);

    mYcFormat.header.methodOffset = ToUInt(p, dataSize);
    MY_LOG_INFO("header.methodOffset:%u", mYcFormat.header.methodOffset);
    p += sizeof(mYcFormat.header.methodOffset);

    mYcFormat.header.separatorDataSize = ToUInt(p, dataSize);
    MY_LOG_INFO("header.separatorDataSize:%u", mYcFormat.header.separatorDataSize);
    p += sizeof(mYcFormat.header.separatorDataSize);

    mYcFormat.header.separatorDataOffset = ToUInt(p, dataSize);
    MY_LOG_INFO("header.separatorDataOffset:%u", mYcFormat.header.separatorDataOffset);
    p += sizeof(mYcFormat.header.separatorDataOffset);


    mYcFormat.header.stringItemSize=ToUInt(p,dataSize);
    MY_LOG_INFO("header.stringItemSize:%u", mYcFormat.header.stringItemSize);
    p += sizeof(mYcFormat.header.stringItemSize);

    mYcFormat.header.stringItemOffest=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.stringItemOffest:%u",mYcFormat.header.stringItemOffest );
    p+= sizeof(mYcFormat.header.stringItemOffest);

    mYcFormat.header.typeItemSize=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.typeItemSize:%u",mYcFormat.header.typeItemSize);
    p+= sizeof(mYcFormat.header.typeItemSize);

    mYcFormat.header.typeItemOffest=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.typeItemOffest：%u",mYcFormat.header.typeItemOffest);
    p+= sizeof(mYcFormat.header.typeItemOffest);


    mYcFormat.header.methodItemSize=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.methodItemSize:%u",mYcFormat.header.methodItemSize);
    p+= sizeof(mYcFormat.header.methodItemSize);

    mYcFormat.header.methodItemOffest=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.methodItemOffest: %u",mYcFormat.header.methodItemOffest);
    p+= sizeof(mYcFormat.header.methodItemOffest);

    //11月30新增
    mYcFormat.header.fieldItemSize=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.fieldItemSize: %u",mYcFormat.header.fieldItemSize);
    p+= sizeof(mYcFormat.header.fieldItemSize);

    mYcFormat.header.fieldItemOffset=ToUInt(p,dataSize);
    MY_LOG_INFO("mYcFormat.header.fieldItemOffset: %u",mYcFormat.header.fieldItemOffset);
    p+= sizeof(mYcFormat.header.fieldItemOffset);

    MY_LOG_INFO("Header file information read out!");


    unsigned int methodSize = mYcFormat.header.methodSize;
    if (0 != methodSize) {
        mYcFormat.methods = new AdvmpMethod*[methodSize];
        p = ycData + mYcFormat.header.methodOffset;
        for(int i=0;i<methodSize;i++)
        {
            mYcFormat.methods[i]=(AdvmpMethod*)calloc(1, sizeof(AdvmpMethod));
            mYcFormat.methods[i]->methodIndex=ToUInt(p,dataSize);
            p+=sizeof(mYcFormat.methods[i]->methodIndex);
            MY_LOG_INFO("%d. methodIndex:%u", i, mYcFormat.methods[i]->methodIndex);

            mYcFormat.methods[i]->size=ToUInt(p,dataSize);
            p+= sizeof(mYcFormat.methods[i]->size);
            MY_LOG_INFO("%d.  current method Size:%u", i, mYcFormat.methods[i]->size);

            mYcFormat.methods[i]->accessFlag=ToUInt(p,dataSize);
            p+=sizeof(mYcFormat.methods[i]->accessFlag);
            MY_LOG_INFO("%d. accessFlag:%u", i, mYcFormat.methods[i]->accessFlag);

            unsigned int tmpDefiningClassSize, tmpNameSize,tmpSigSize;
            tmpDefiningClassSize=ToUInt(p,dataSize);
            MY_LOG_INFO("%d. tmpDefiningClassSize is :%u", i, tmpDefiningClassSize);
            p+= sizeof(tmpDefiningClassSize);
            mYcFormat.methods[i]->definingClass=ToString(p,tmpDefiningClassSize);
            p+=tmpDefiningClassSize;
            MY_LOG_INFO("%d. definingClass:%s", i, mYcFormat.methods[i]->definingClass);

            tmpNameSize=ToUInt(p,dataSize);
            MY_LOG_INFO("%d. tmpNameSize is :%u", i, tmpNameSize);
            p+= sizeof(tmpNameSize);
            mYcFormat.methods[i]->name=ToString(p,tmpNameSize);
            p+=tmpNameSize;
            MY_LOG_INFO("%d. method name:%s", i, mYcFormat.methods[i]->name);

            tmpSigSize=ToUInt(p,dataSize);
            MY_LOG_INFO("%d. tmpSigSize= is :%u", i, tmpSigSize);
            p+= sizeof(tmpSigSize);
            mYcFormat.methods[i]->sig=ToString(p,tmpSigSize);
            p+=tmpSigSize;
            MY_LOG_INFO("%d. registerSize:%s", i, mYcFormat.methods[i]->sig);
        }
    }
//开始解析引用数据
   if(0!=mYcFormat.header.stringItemSize)
   {
       mYcFormat.mStringItem=new char*[mYcFormat.header.stringItemSize];
       p=mYcFormat.header.stringItemOffest+ycData;
       int str_size[mYcFormat.header.stringItemSize];
       //读取每个字符串的大小
       for(int i=0;i<mYcFormat.header.stringItemSize;i++)
       {
           str_size[i]=ToUInt(p,dataSize);
           p+= sizeof(unsigned int);
       }
       //为每个字符串赋值
       for(int i=0;i<mYcFormat.header.stringItemSize;i++)
       {
           mYcFormat.mStringItem[i]=ToString(p,str_size[i]);
           p+= str_size[i];
       }
       for(int i=0;i<mYcFormat.header.stringItemSize;i++)
       {
          MY_LOG_INFO("%s", mYcFormat.mStringItem[i]);
       }

   }
    if(0!=mYcFormat.header.typeItemSize)
    {
        mYcFormat.mTypeItem=new int[mYcFormat.header.typeItemSize];
        p=mYcFormat.header.typeItemOffest+ycData;
        for(int i=0;i<mYcFormat.header.typeItemSize;i++)
        {
            mYcFormat.mTypeItem[i]=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);
        }
    }
    if(0!=mYcFormat.header.methodItemSize)
    {
        mYcFormat.mMethodItem=new  MethodItem[mYcFormat.header.methodItemSize];
        p=mYcFormat.header.methodItemOffest+ycData;
        for(int i=0;i<mYcFormat.header.methodItemSize;i++)
        {
            mYcFormat.mMethodItem[i].classnameIdx=ToUInt(p,dataSize);
            p+=sizeof(unsigned int);
            mYcFormat.mMethodItem[i].protoIdx=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);
            mYcFormat.mMethodItem[i].methodnameIdx=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);
        }
        for(int i=0;i<mYcFormat.header.methodItemSize;i++)
        {
            MY_LOG_INFO("mYcFormat.mMethodItem[i].classnameIdx %d",mYcFormat.mMethodItem[i].classnameIdx);
            MY_LOG_INFO("mYcFormat.mMethodItem[i].protoIdx %d", mYcFormat.mMethodItem[i].protoIdx);
            MY_LOG_INFO("mYcFormat.mMethodItem[i].methodnameIdx %d", mYcFormat.mMethodItem[i].methodnameIdx);

        }
    }
    if(0!=mYcFormat.header.fieldItemSize)
    {
        mYcFormat.mFieldItem=new FieldItem[mYcFormat.header.fieldItemSize];
        p=mYcFormat.header.fieldItemOffset+ycData;
        for (int i = 0; i <mYcFormat.header.fieldItemSize ; ++i) {
            mYcFormat.mFieldItem[i].classNameIdx=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);
            mYcFormat.mFieldItem[i].typeIdx=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);
            mYcFormat.mFieldItem[i].fieldNameIdx=ToUInt(p,dataSize);
            p+= sizeof(unsigned int);

        }
        for(int i=0;i<mYcFormat.header.methodItemSize;i++)
        {
            MY_LOG_INFO("mYcFormat.mFieldItem[i].classNameIdx %d",mYcFormat.mFieldItem[i].classNameIdx);
            MY_LOG_INFO("mYcFormat.mFieldItem[i].typeIdx %d", mYcFormat.mFieldItem[i].typeIdx);
            MY_LOG_INFO("mYcFormat.mFieldItem[i].fieldNameIdx %d", mYcFormat.mFieldItem[i].fieldNameIdx);

        }

    }
    unsigned int separatorDataSize = mYcFormat.header.separatorDataSize;
    // 解析抽离数据。
    if (0 != separatorDataSize) {
        mYcFormat.separatorDatas = new SeparatorData*[separatorDataSize];
        p = ycData + mYcFormat.header.separatorDataOffset;

        for (int i = 0; i < separatorDataSize; i++) {
            mYcFormat.separatorDatas[i] = (SeparatorData*) calloc(1, sizeof(SeparatorData));
            
            mYcFormat.separatorDatas[i]->methodIndex = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->methodIndex);
            MY_LOG_INFO("%d. methodIndex:%u", i, mYcFormat.separatorDatas[i]->methodIndex);

            mYcFormat.separatorDatas[i]->size = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->size);
            MY_LOG_INFO("%d. size:%u", i, mYcFormat.separatorDatas[i]->size);

            mYcFormat.separatorDatas[i]->accessFlag = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->accessFlag);
            MY_LOG_INFO("%d. accessFlag:%u", i, mYcFormat.separatorDatas[i]->accessFlag);

            mYcFormat.separatorDatas[i]->paramSize = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->paramSize);
            MY_LOG_INFO("%d. paramSize:%u", i, mYcFormat.separatorDatas[i]->paramSize);

            mYcFormat.separatorDatas[i]->registerSize = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->registerSize);
            MY_LOG_INFO("%d. registerSize:%u", i, mYcFormat.separatorDatas[i]->registerSize);

            mYcFormat.separatorDatas[i]->paramShortDesc.size = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->paramShortDesc.size);
            MY_LOG_INFO("%d. paramShortDesc.size:%u", i, mYcFormat.separatorDatas[i]->paramShortDesc.size);

            mYcFormat.separatorDatas[i]->paramShortDesc.str = (unsigned char*) calloc(1, mYcFormat.separatorDatas[i]->paramShortDesc.size);
            memcpy(mYcFormat.separatorDatas[i]->paramShortDesc.str, p, mYcFormat.separatorDatas[i]->paramShortDesc.size);
            MY_LOG_INFO("mYcFormat.separatorDatas[i]->paramShortDesc.str=%s", mYcFormat.separatorDatas[i]->paramShortDesc.str);
            MY_LOG_INFO("mYcFormat.separatorDatas[i]->paramShortDesc.str[0]=%c",mYcFormat.separatorDatas[i]->paramShortDesc.str[0]);
            p += mYcFormat.separatorDatas[i]->paramShortDesc.size;

            mYcFormat.separatorDatas[i]->instSize = ToUInt(p, dataSize);
            p += sizeof(mYcFormat.separatorDatas[i]->instSize);
            MY_LOG_INFO("%d. instSize:%u", i, mYcFormat.separatorDatas[i]->instSize);

            size_t instByteSize = mYcFormat.separatorDatas[i]->instSize * sizeof(unsigned short);
            MY_LOG_INFO("instByteSize:%zd", instByteSize);
            mYcFormat.separatorDatas[i]->insts = new unsigned short[mYcFormat.separatorDatas[i]->instSize];
            memcpy(mYcFormat.separatorDatas[i]->insts, p, instByteSize);
            p += instByteSize;
            for(int j=0;j<mYcFormat.separatorDatas[i]->instSize;j++)
            {
                MY_LOG_INFO("mYcFormat.separatorDatas[i]->insts[%d]=%d",j,mYcFormat.separatorDatas[i]->insts[j]);
            }
        }
    }

    MY_LOG_INFO("end:%p, p:%p", end, p);

    return true;
}

// 获得Separator数据
const SeparatorData * YcFile::GetSeparatorData(int index) {
    return mYcFormat.separatorDatas[index];
}

//////////////////////////////////////////////////////////////////////////
// 

YcFormat::YcFormat() : methods(NULL), separatorDatas(NULL)
{
    memset(&header, 0, sizeof(YcHeader));
}

YcFormat::~YcFormat() {
    if (NULL != methods) {
        unsigned int size = header.methodSize;
        for (int i = 0; i < size; i++) {
            free(methods[i]);
        }
        delete[] methods;
    }
    if (NULL != separatorDatas) {
        unsigned int size = header.separatorDataSize;
        for (int i = 0; i < size; i++) {
            if (NULL != separatorDatas[i]->insts) {
                delete[] separatorDatas[i]->insts;
            }
            free(separatorDatas[i]);
        }
        delete[] separatorDatas;
    }
}

//////////////////////////////////////////////////////////////////////////

// 打开并解析yc文件。
// bool OpenAndParseYc(JNIEnv* env) {
//     YcFile* ycFile = new YcFile(gAdvmp.ycFilePath);
//     if (ycFile->parse()) {
//         gAdvmp.ycFile = ycFile;
//     } else {
//         MY_LOG_ERROR("打开&解析yc文件失败！");
//         return false;
//     }
// }

//////////////////////////////////////////////////////////////////////////

// 释放NISL文件。
uLong ReleaseYcFile(const char* zipPath, unsigned char** buffer) {
    MY_LOG_INFO("zip file:%s", zipPath);
    ZipReader zipReader(zipPath);
    if (!zipReader.Open()) {
        MY_LOG_WARNING("open zip file fail%s", zipPath);
        return false;
    }

    char* filePathInZip = (char*) calloc(strlen(gYcFileName) + strlen("assets") + 1, sizeof(char));
    sprintf(filePathInZip, "assets/%s", gYcFileName);
    MY_LOG_INFO("filePathInZip=%s", filePathInZip);
    uLong fileSize = zipReader.GetFileSizeInZip(filePathInZip);

    uLong bRet = 0;
    if (0 == fileSize) {
        MY_LOG_WARNING("NISL file size is 0.");
        goto _ret;
    }

    MY_LOG_INFO("NISL file size:%lu", fileSize);
    
    *buffer = (unsigned char*) calloc(sizeof(unsigned char), fileSize);
    if (!zipReader.ReadBytes(filePathInZip, *buffer, fileSize)) {
        MY_LOG_WARNING("read NISL file fail.");
        goto _ret;
    }

    bRet = fileSize;

_ret:
    if (NULL != filePathInZip) {
        free(filePathInZip);
    }
    return bRet;
}
