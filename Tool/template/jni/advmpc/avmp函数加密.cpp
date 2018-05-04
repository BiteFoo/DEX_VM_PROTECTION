#include "stdafx.h"
#include "Common.h"
#include "InterpC.h"
#include "Globals.h"
#include "Utils.h"
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>
typedef struct _funcInfo{
    Elf32_Addr st_value;//57605
    Elf32_Word st_size;//90364
}funcInfo;

////��������
void init_getString()__attribute__((constructor));
static unsigned elfhash(const char *_name)
{
    const unsigned char *name = (const unsigned char *) _name;
    unsigned h = 0, g;

    while(*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}
static unsigned int getLibAddr(){
    unsigned int ret = 0;
    char name[] = "libDexvmp.so";
    char buf[4096], *temp;
    int pid;
    FILE *fp;
    pid = getpid();
    sprintf(buf, "/proc/%d/maps", pid);
    fp = fopen(buf, "r");
    if(fp == NULL)
    {
        puts("open failed");
        goto _error;
    }
    while(fgets(buf, sizeof(buf), fp)){
        if(strstr(buf, name)){
            temp = strtok(buf, "-");
            ret = strtoul(temp, NULL, 16);
            break;
        }
    }_error:
    fclose(fp);
    return ret;
}

static char getTargetFuncInfo(unsigned long base, const char *funcName, funcInfo *info){
    char flag = -1, *dynstr;
    int i;
    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdr;
    Elf32_Off dyn_vaddr;
    Elf32_Word dyn_size, dyn_strsz;
    Elf32_Dyn *dyn;
    Elf32_Addr dyn_symtab, dyn_strtab, dyn_hash;
    Elf32_Sym *funSym;
    unsigned funHash, nbucket,nchain;
    unsigned *chain;
    int *bucket;

    ehdr = (Elf32_Ehdr *)base;
    phdr = (Elf32_Phdr *)(base + ehdr->e_phoff);
    for (i = 0; i < ehdr->e_phnum; ++i) {
        if(phdr->p_type ==  PT_DYNAMIC){
            flag = 0;
            MY_LOG_VERBOSE("Find .dynamic segment");
            break;
        }
        phdr ++;
    }
    if(flag)
        return -1;
    dyn_vaddr = phdr->p_vaddr + base;//ֱ�Ӷ�λ������ͷ�ĵ�ַ�ʹ�С��
    dyn_size = phdr->p_filesz;
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "dyn_vadd =  0x%x, dyn_size =  0x%x", dyn_vaddr, dyn_size);
    flag = 0;
    for (i = 0; i < dyn_size / sizeof(Elf32_Dyn); ++i) {
        dyn = (Elf32_Dyn *)(dyn_vaddr + i * sizeof(Elf32_Dyn));
        if(dyn->d_tag == DT_SYMTAB){
            dyn_symtab = (dyn->d_un).d_ptr;
            flag += 1;
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find .dynsym section, addr = 0x%x\n", dyn_symtab);
        }
        if(dyn->d_tag == DT_HASH){
            dyn_hash = (dyn->d_un).d_ptr;
            flag += 2;
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find .hash section, addr = 0x%x\n", dyn_hash);
        }
        if(dyn->d_tag == DT_STRTAB){
            dyn_strtab = (dyn->d_un).d_ptr;
            flag += 4;
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find .dynstr section, addr = 0x%x\n", dyn_strtab);
        }
        if(dyn->d_tag == DT_STRSZ){
            dyn_strsz = (dyn->d_un).d_val;
            flag += 8;
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find strsz size = 0x%x\n", dyn_strsz);
        }
    }
    if((flag & 0x0f) != 0x0f){
        MY_LOG_VERBOSE("Find needed .section failed\n");
        return -1;
    }
    dyn_symtab += base;
    dyn_hash += base;
    dyn_strtab += base;
    dyn_strsz += base;

    funHash = elfhash(funcName);
    funSym = (Elf32_Sym *) dyn_symtab;
    dynstr = (char*) dyn_strtab;
    nbucket = *((int *) dyn_hash);//521
    nchain=*((int *)(dyn_hash)+1);//577
    bucket = (int *)(dyn_hash + 8);
    chain = (unsigned int *)(dyn_hash + 4 * (2 + nbucket));//ָ������nbucket����ʼλ��

    flag = -1;
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "hash = 0x%x, nbucket = 0x%x\n", funHash, nbucket);
    int mod = (funHash % nbucket);
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "mod = %d\n", mod);
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "i = 0x%d\n", bucket[mod]);
//    int k=0;
//    while(k++<=nchain)
//    {
//        __android_log_print(ANDROID_LOG_INFO, "JNITag","st_name = %s\n",dynstr + (funSym + chain[k])->st_name);
//        if(strcmp(dynstr + (funSym + chain[k])->st_name, funcName) == 0){
//            flag = 0;
//            info->st_value = (funSym + k)->st_value;
//            info->st_size = (funSym + k)->st_size;
//            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find %s\n", funcName);
//            __android_log_print(ANDROID_LOG_INFO, "JNITag", "st_value = %d, st_size = %d", info->st_value, info->st_size);
//            break;
//        }
//    }

    for(i = bucket[mod]; i != 0; i = chain[i]){
        __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find index = %d\n", i);
        __android_log_print(ANDROID_LOG_INFO, "JNITag","st_name = %s\n", dynstr + (funSym + i)->st_name);
        if(strcmp(dynstr + (funSym + i)->st_name, funcName) == 0){
            flag = 0;
            info->st_value = (funSym + i)->st_value;
            info->st_size = (funSym + i)->st_size;
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "Find %s\n", funcName);
            __android_log_print(ANDROID_LOG_INFO, "JNITag", "st_value = %d, st_size = %d", info->st_value, info->st_size);
            break;
        }
    }
    if(flag)
        return -1;
    return 0;
}
void init_getString(){
    const char target_fun[] = "_Z23NISLvmInterpretPortableP6YcFileiP7_JNIEnvP8_jobjectz";
    funcInfo info;
    int i;
    unsigned int npage, base = getLibAddr();

    __android_log_print(ANDROID_LOG_INFO, "JNITag", "base addr =  0x%x", base);
    if(getTargetFuncInfo(base, target_fun, &info) == -1){
        MY_LOG_VERBOSE("Find Java_com_example_shelldemo2_MainActivity_getString failed");
        return ;
    }
    npage = info.st_size / PAGE_SIZE + ((info.st_size % PAGE_SIZE == 0) ? 0 : 1);
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "npage =  0x%d", npage);
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "npage =  0x%d", PAGE_SIZE);

    if(mprotect((void *) ((base + info.st_value) / PAGE_SIZE * PAGE_SIZE), 4096*npage, PROT_READ | PROT_EXEC | PROT_WRITE) != 0){
        MY_LOG_VERBOSE("mem privilege change failed");
    }

    for(i=0;i< info.st_size - 1; i++){
        char *addr = (char*)(base + info.st_value -1 + i);
        *addr = ~(*addr);
    }

    if(mprotect((void *) ((base + info.st_value) / PAGE_SIZE * PAGE_SIZE), 4096*npage, PROT_READ | PROT_EXEC) != 0){
        MY_LOG_VERBOSE("mem privilege change failed");
    }

}


#if 0
jint separatorTest (JNIEnv *env, jobject thiz, jint a) {
    jvalue result = BWdvmInterpretPortable(gAdvmp.ycFile->GetSeparatorData(0), env, thiz, a);
    return result.i;
}
bool registerNatives0(JNIEnv* env) {
    const char* classDesc = "com/example/lz/dexvmp/MainActivity";
    const JNINativeMethod methods[] = {
            { "separatorTest", "(I)I", (void*)separatorTest },
    };
    jclass clazz = env->FindClass(classDesc);

    if (!clazz) { MY_LOG_ERROR("can't find class:%s!", classDesc); return false; }
    bool bRet = false;
    if ( JNI_OK == env->RegisterNatives(clazz, methods, array_size(methods)) ) { bRet = true; }
    else { MY_LOG_ERROR("classDesc:%s, register method fail.", classDesc); }
    env->DeleteLocalRef(clazz); return bRet;
}

void registerFunctions(JNIEnv* env) {
    if (!registerNatives0(env)) { MY_LOG_ERROR("register method fail."); return; }

#else

jint separatorTest (JNIEnv *env, jobject thiz) {
    jvalue result = NISLvmInterpretPortable(gAdvmp.ycFile,0 , env, thiz);
    return result.i;}


bool registerNatives0(JNIEnv* env) {
    const char* classDesc = "com/example/lz/advmptesteasy/MainActivity";
    const JNINativeMethod methods[] = {
            { "separatorTest", "()I", (void*)separatorTest },
    };
    jclass clazz = env->FindClass(classDesc);
    if (!clazz) { MY_LOG_ERROR("can't find class:%s!", classDesc); return false; }
    bool bRet = false;
    if ( JNI_OK == env->RegisterNatives(clazz, methods, array_size(methods)) ) { bRet = true; }
    else { MY_LOG_ERROR("classDesc:%s, register method fail.", classDesc); }
    env->DeleteLocalRef(clazz); return bRet; }
void registerFunctions(JNIEnv* env) {
    if (!registerNatives0(env)) { MY_LOG_ERROR("register method fail."); return; }

}
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_ERR;
    }

    // ע�᱾�ط�����
    registerFunctions(env);

    // ����apk·����
    gAdvmp.apkPath = GetAppPath(env);
    MY_LOG_INFO("apk path��%s", gAdvmp.apkPath);

    // �ͷ�yc�ļ���
    gAdvmp.ycSize = ReleaseYcFile(gAdvmp.apkPath, &gAdvmp.ycData);
    if (0 == gAdvmp.ycSize) {
        MY_LOG_WARNING("release Yc file fail!");
        goto _ret;
    }

    // ����yc�ļ���
    gAdvmp.ycFile = new YcFile;
    if (!gAdvmp.ycFile->parse(gAdvmp.ycData, gAdvmp.ycSize)) {
        MY_LOG_WARNING("parse Yc file fail.");
        goto _ret;
    }

_ret:
    return JNI_VERSION_1_4;
}
