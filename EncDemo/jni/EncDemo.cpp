#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <time.h>
#include "charmap.h"
#include "zlib.h"
#include "jni_common.h"
#include "com_example_encdemo_LoadNative.h"
/**
jni JNI_OnLoad(JavaVM* VM, void* reserved){
	JNIEnv* env = NULL;
	jint result = -1;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}
	return JNI_VERSION_1_4;
}**/

#ifdef __cplusplus
extern "C" {
#endif
__attribute__ ((visibility ("default"))) void ijiami(){
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__x86_64__)
   #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
   #define ABI "mips64"
#elif defined(__mips__)
   #define ABI "mips"
#elif defined(__aarch64__)
   #define ABI "arm64-v8a"
#else
   #define ABI "unknown"
#endif
	LOGD("%s",ABI);
}
#ifdef __cplusplus
}
#endif

jstring CharTojstring(JNIEnv* env, char* str)
{
    jsize len = strlen(str);
    jclass clsstring = env -> FindClass("java/lang/String");
    jstring strencode = env -> NewStringUTF("GB2312");
    jmethodID mid = env -> GetMethodID(clsstring,"<init>","([BLjava/lang/String;)V");
    jbyteArray barr = env-> NewByteArray(len);
    env-> SetByteArrayRegion(barr,0,len,(jbyte*)str);
    return (jstring)env -> NewObject(clsstring,mid,barr,strencode);
}

char* jstringToChar(JNIEnv *env, jstring jstr)
{
    char * rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
    jsize alen = env->GetArrayLength(barr);
    LOGI("size:%d",alen);
    jbyte * ba = env->GetByteArrayElements(barr,JNI_FALSE);
    LOGI("ba:%s",ba);
    if(alen > 0)
    {
        rtn = (char*)malloc(alen+1); //new char[alen+1];
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);

    return rtn;
}

void encMap(unsigned char *str, uLong length)
{
  int i;

  int l,c;
  for (i=0; i<length; i++)
    {
      l = (str[i]&0xF0)>>0x04;
      c = str[i]&0x0F;
      str[i] = sBox[l*16 + c]&0X00FF;
    }
}

void decMap(unsigned char *str, uLong length)
{
  int i;

  int l,c;

  for (i=0; i<length; i++)
    {
      l = (str[i]&0xF0)>>0x04;
      c = str[i]&0x0F;
      str[i] = invsBox[l*16 + c]&0X00FF;
    }

}

JNIEXPORT jint JNICALL Java_com_example_encdemo_LoadNative_encrypt(JNIEnv *env, jobject obj, jstring fin){

	double  start, finish;
	char* path = jstringToChar(env,fin);
	LOGI("path:%s",path);
	char* fout = path;
    FILE* file;
    FILE* fileEnc;
    uLong flen;
    unsigned char* fbuf = NULL;
    uLong clen;
    unsigned char* cbuf = NULL;
    if((file = fopen(path, "rb")) == NULL)
    {
        LOGI("Can\'t open %s!\n", path);
        return -1;
    }
    LOGI("here1");
    fseek(file, 0L, SEEK_END);    /* 跳到文件末尾 */
    flen = ftell(file);        /* 获取文件长度 */
    fseek(file, 0L, SEEK_SET);
    if((fbuf = (unsigned char*)malloc(sizeof(unsigned char) * flen)) == NULL)
    {
        LOGI("No enough memory!\n");
        fclose(file);
        return -1;
    }
    LOGI("here2");
    //start = clock();
    LOGI("here2.01");
    LOGI("fbuf:%s, flen:%d, file:%s",fbuf, flen, file);
    fread(fbuf, sizeof(unsigned char), flen, file);
    LOGI("here2.0");
    clen = compressBound(flen);
    LOGI("here2.1");
    if((cbuf = (unsigned char*)malloc(sizeof(unsigned char) * clen)) == NULL)
    {
        LOGI("No enough memory!\n");
        fclose(file);
        return -1;
    }
    LOGI("here3");
    if(compress(cbuf, &clen, fbuf, flen) != Z_OK)
    {
        LOGI("Compress %s failed!\n", path);
        return -1;
    }
    LOGI("here4");
    fclose(file);
    encMap(cbuf,clen);
    //finish = clock();//取结束时间
    //LOGI( "encrypt TIME:%f ms\n",(finish - start) * 1000/ CLOCKS_PER_SEC);//以秒为单位显示
    LOGI("here5");
    strcat(fout,".enc");
    LOGI("here6");
    if((fileEnc = fopen(fout, "wb")) == NULL)
    {
        LOGI("Can\'t create %s!\n", fout);
        return -1;
    }
    LOGI("here7");
    fwrite(&flen, sizeof(uLong), 1, fileEnc);    /* 写入源文件长度 */

    fwrite(&clen, sizeof(uLong), 1, fileEnc);    /* 写入目标数据长度 */
    fwrite(cbuf, sizeof(unsigned char), flen, fileEnc);
    LOGI("here8");
    fclose(fileEnc);
    free(fbuf);
    free(cbuf);
    LOGI("here9");
    LOGI("success encrpt ! Outfile --> \"%s\"\n",fout);
    return 0;
}


JNIEXPORT jint JNICALL Java_com_example_encdemo_LoadNative_decrypt(JNIEnv *env, jobject obj, jstring fin){
	double  start, finish;
	char* path = jstringToChar(env,fin);
	char* fout = path;
    FILE* file;
    FILE* fileDec;
    uLong flen;
    unsigned char* fbuf = NULL;
    uLong ulen;
    unsigned char* ubuf = NULL;
    if((file = fopen(path, "rb")) == NULL)
    {
        LOGI("Can\'t open %s!\n", path);
        return -1;
    }
    start = clock();//取开始时间
    /* 装载源文件数据到缓冲区 */
    fread(&ulen, sizeof(uLong), 1, file);   /* 获取缓冲区大小 */
    fread(&flen, sizeof(uLong), 1, file);   /* 获取数据流大小 */
    if((fbuf = (unsigned char*)malloc(sizeof(unsigned char) * ulen)) == NULL) //申请和原内存大小相同的空间
    {
        LOGI("No enough memory1!\n");
        fclose(file);
        return -1;
    }
    fread(fbuf, sizeof(unsigned char), flen, file);


    if((ubuf = (unsigned char*)malloc(sizeof(unsigned char) * ulen)) == NULL)
    {
        LOGI("No enough memory2!\n");
        fclose(file);
        return -1;
    }

    decMap(fbuf,flen);
    if(uncompress(ubuf, &ulen, fbuf, flen) != Z_OK)
    {
        LOGI("Uncompress %s failed!\n", path);
        return -1;
    }

    fclose(file);
    memcpy(fbuf, ubuf, ulen);
    finish = clock();//取结束时间
    LOGI( "decrypt TIME:%f ms\n",(finish - start) * 1000/ CLOCKS_PER_SEC);//以秒为单位显示
    strcat(fout,".dec");

    if((fileDec = fopen(fout, "wb")) == NULL)
    {
        LOGI("Can\'t create %s!\n", fout);
        return -1;
    }
    /* 保存解压缩后的数据到目标文件 */
    fwrite(fbuf, sizeof(unsigned char), ulen, fileDec);
    fclose(fileDec);

    free(fbuf);
    free(ubuf);
    LOGI("success decrpt ! Outfile --> \"%s\"\n",fout);
    return 0;
}

