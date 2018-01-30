#include <iostream>
#include "AES.h"
#include "base64.h"
#include "jni_common.h"
#include "jni.h"
//#include "com_example_aesbase64enc_MainActivity.h"
using namespace std;

const char g_key[17] = "asdfwetyhjuytrfd";
const char g_iv[17] = "gfdertfghjkuyrtg";
string EncryptionAES(const string& strSrc) //AES鍔犲瘑
{
	size_t length = strSrc.length();
	int block_num = length / BLOCK_SIZE + 1;
	//鏄庢枃
	char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
	memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
	strcpy(szDataIn, strSrc.c_str());

	//杩涜PKCS7Padding濉厖銆�
	int k = length % BLOCK_SIZE;
	int j = length / BLOCK_SIZE;
	int padding = BLOCK_SIZE - k;
	for (int i = 0; i < padding; i++)
	{
		szDataIn[j * BLOCK_SIZE + k + i] = padding;
	}
	szDataIn[block_num * BLOCK_SIZE] = '\0';

	//鍔犲瘑鍚庣殑瀵嗘枃
	char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
	memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

	//杩涜杩涜AES鐨凜BC妯″紡鍔犲瘑
	AES aes;
	aes.MakeKey(g_key, g_iv, 16, 16);
	aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
	string str = base64_encode((unsigned char*) szDataOut,
			block_num * BLOCK_SIZE);
	delete[] szDataIn;
	delete[] szDataOut;
	return str;
}
string DecryptionAES(const string& strSrc) //AES瑙ｅ瘑
{
	string strData = base64_decode(strSrc);
	LOGD("here");
	size_t length = strData.length();
	//瀵嗘枃
	char *szDataIn = new char[length + 1];
	memcpy(szDataIn, strData.c_str(), length+1);
	//鏄庢枃
	char *szDataOut = new char[length + 1];
	memcpy(szDataOut, strData.c_str(), length+1);

	//杩涜AES鐨凜BC妯″紡瑙ｅ瘑
	AES aes;
	aes.MakeKey(g_key, g_iv, 16, 16);
	aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

	//鍘籔KCS7Padding濉厖
	if (0x00 < szDataOut[length - 1] <= 0x16)
	{
		int tmp = szDataOut[length - 1];
		for (int i = length - 1; i >= length - tmp; i--)
		{
			if (szDataOut[i] != tmp)
			{
				memset(szDataOut, 0, length);
				cout << "鍘诲～鍏呭け璐ワ紒瑙ｅ瘑鍑洪敊锛侊紒" << endl;
				break;
			}
			else
				szDataOut[i] = 0;
		}
	}
	string strDest(szDataOut);
	delete[] szDataIn;
	delete[] szDataOut;
	return strDest;
}

/**
jint Jni_OnLoad(int argc, char **argv)
{
	string str1 = "qwertyuisfdlsajdxcvnkhsakfh1332487";
	cout << "鍔犲瘑鍓�:" << str1 << endl;
	string str2 = EncryptionAES(str1);
	cout << "鍔犲瘑鍚�:" << str2 << endl;
	string str3 = DecryptionAES(str2);
	cout << "瑙ｅ瘑鍚�:" << str3 << endl;
}**/


static JNIEnv* env = NULL;

jstring charTojstring(JNIEnv* env, char* str)
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
    //LOGI("size:%d",alen);
    jbyte * ba = env->GetByteArrayElements(barr,JNI_FALSE);
    //LOGI("ba:%s",ba);
    if(alen > 0)
    {
        rtn = (char*)malloc(alen+1); //new char[alen+1];
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);

    return rtn;
}

jstring enc(JNIEnv* env,jobject thiz, jstring jstr){
	char* chardata1 = jstringToChar(env,jstr);
	string str1 = chardata1;
	string str2 = EncryptionAES(str1);
	char* chardata2 = const_cast<char*>(str2.c_str());
	return charTojstring(env,chardata2);
}

jstring decdata(JNIEnv * env, jobject thiz, jstring jstr){
	char* chardata1 = jstringToChar(env,jstr);
	string str1 = chardata1;
	string str2 = DecryptionAES(str1);
	char* chardata2 = const_cast<char*>(str2.c_str());
	return charTojstring(env,chardata2);
}
/**
JNIEXPORT jstring JNICALL Java_com_example_aesbase64enc_MainActivity_encData
  (JNIEnv * env, jobject clazz, jstring jstr){
	char* chardata1 = jstringToChar(env,jstr);
	string str1 = chardata1;
	string str2 = EncryptionAES(str1);
	char* chardata2 = const_cast<char*>(str2.c_str());
	return charTojstring(env,chardata2);
}

JNIEXPORT jstring JNICALL Java_com_example_aesbase64enc_MainActivity_decData
  (JNIEnv * env, jobject clazz, jstring jstr){
	char* chardata1 = jstringToChar(env,jstr);
	string str1 = chardata1;
	string str2 = DecryptionAES(str1);
	char* chardata2 = const_cast<char*>(str2.c_str());
	return charTojstring(env,chardata2);
}**/

static JNINativeMethod gMethodsenc[] = {
		{"encData","(Ljava/lang/String;)Ljava/lang/String;", (void *)enc},
		{"decData","(Ljava/lang/String;)Ljava/lang/String;", (void *)decdata}
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
	jint result = JNI_FALSE;
	LOGE("VM in AESEnc:%p",vm);
	if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}
	if (env == NULL) {
		return result;
	}
	LOGE("ENV IN AESenc:%p",env);
	jclass clazz = env->FindClass("com/example/aesbase64enc/MainActivity");
	if (clazz == NULL) {
		LOGD("find class err");
		return result;
	}
	LOGE("find class success!");
	//娉ㄥ唽鏂规硶
	if (env->RegisterNatives(clazz, gMethodsenc,sizeof(gMethodsenc) / sizeof(gMethodsenc[0])) < 0) {
		return result;
	}
	result = JNI_VERSION_1_4;
	return result;
}

