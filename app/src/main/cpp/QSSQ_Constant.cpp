//

// Created by luozheng on 2018/1/29.
//

#include <iostream>
#include <jni.h>
#include <stdlib.h>
#pragma once










#include <android/log.h>
#ifndef INT_ARR_IMPORT
#define INT_ARR_IMPORT
#include"QSSQ_INT_ARR.h"
#endif // !INT_ARR_IMPORT

using namespace std;


static bool SHOW_LOG = true;
//use Android's log API

#define  LOG_TAG    "QSSQ_KILLER"
//#define  AUTH_Illegal CRACK  "软件已被篡改,请联系QQ694886526下载最新版";
#define  LOGI_(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW_(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

//#define LOGXI(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
//#define LOGXW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
//#define  LOGEE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//#define  printf(...) LOGI(...)

 class QSSQ_Constant {
	//class中 只能直接初始化static const int类型的值 所以 类里面不能声明静态常量 。或者常量的数组 直接 使用头文件进行抽取好了，然后使用include 导入。
public:

    /**
     * 返回之后一定要定义变量接受，否则就翻车。
     */
     static  string javaClass2ClassSign(const char* className) {
         std::string stringClassName = string(className);
         stringClassName = stringClassName.insert(0, "L");
         int value;
         int count = 0;

         while ((value = stringClassName.find("\.")) != -1) {
             printf(" replace from %d  ->. ", value);
             stringClassName.replace(value, 1, "/");
             count++;
             if (count > 10) {
                 break;
             }
         }
         stringClassName = stringClassName + ";";

         //printf("replace before, %sreplace after  %s  ", className, stringClassName.c_str());
         return stringClassName;
         //s.replace(pos, n, s1)    用s1替换s中从pos开始（包括0）的n个字符的子串

     }

	static jstring charstoJstring(JNIEnv *env, const char *str) {
		jclass strClass = env->FindClass("java/lang/String");
		jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
		jbyteArray bytes = env->NewByteArray(strlen(str));
		env->SetByteArrayRegion(bytes, 0, strlen(str), (jbyte *)str);
		jstring encoding = env->NewStringUTF("utf-8");
		return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
	}

	static string charstostring(JNIEnv *env, char *str) {
		return str;
	}



	static const char *jstringTochars(JNIEnv *env, jstring jstr) {
		if (jstr == NULL) {
			return "excep";
		}
		char *rtn = NULL;
		jclass clsstring = env->FindClass("java/lang/String");
		jstring strencode = env->NewStringUTF("utf-8");
		jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
		jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
		jsize alen = env->GetArrayLength(barr);
		jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
		if (alen > 0) {
			rtn = (char *)malloc(alen + 1);
			memcpy(rtn, ba, alen);
			rtn[alen] = 0;
		}
		env->ReleaseByteArrayElements(barr, ba, 0);
		return rtn;
	}


	static jstring getObjectClassName(JNIEnv *env, jobject obj) {

		if (obj == NULL) {
			return charstoJstring(env, "null");
		}
		else {
			// env->GetMethodID(obj->)

			jclass jclassName = env->GetObjectClass(obj);
			jmethodID toStringMethodId = env->GetMethodID(env->GetObjectClass(jclassName),
				"getName", "()Ljava/lang/String;");
			jstring getName = static_cast<jstring>(env->CallObjectMethod(jclassName,
				toStringMethodId));

			return getName;
		}


	}

	static int charToInt(char *address) {
		//_itoa
		return atoi(address);
	}

	static char *intToChar(int intvalue) {
		//        int i=0;
		//        char itc[10];
		//        sprintf(itc,"%d.bmp",i);e
		char *s;
		sprintf(s, "%d", intvalue); //产生"123"
	}

	static const char *pointToChar(void *intvalue) {
		//        int i=0;
		//        char itc[10];
		//        char *s;
		//        sprintf(s, "%d", &intvalue); //产生"123"
		return "unknown";
	}

	static const char *objtoString(JNIEnv *env, jobject obj) {

		if (obj == NULL) {
			return "NULL";
		}
		else {
			// env->GetMethodID(obj->)
			jmethodID toStringMethodId = env->GetMethodID(env->GetObjectClass(obj), "toString",
				"()Ljava/lang/String;");
			char addresStr[1024] = { 0 };

			sprintf(addresStr, "%p", obj);

			//            string address=charstostring(env,addresStr);
			jstring toString = static_cast<jstring>(env->CallObjectMethod(obj, toStringMethodId));
			std::string source = jstringTostring(env, getObjectClassName(env, obj)) + "-" +
				jstringTostring(env, toString) + " address " +
				charstostring(env, addresStr);

			//            source += charstostring(env, Constant::pointToChar(&obj));
			return source.c_str();
		}



	}

	static std::string getObjClassName(JNIEnv *env, jobject obj) {

		if (obj == NULL) {
			return std::string("");
		}
		else {
			// env->GetMethodID(obj->)
			jmethodID toStringMethodId = env->GetMethodID(env->GetObjectClass(obj), "toString",
				"()Ljava/lang/String;");
			char addresStr[1024] = { 0 };
			jstring toString = static_cast<jstring>(env->CallObjectMethod(obj, toStringMethodId));
			std::string source = jstringTostring(env, getObjectClassName(env, obj));
			return source;
		}

	}
	/**
	 *  存在问题,
	 * @param env
	 * @param defaultName
	 * @param obj
	 * @return
	 */
	static const char *objtoStringBy(JNIEnv *env, char *defaultName, jobject obj) {

		if (obj == NULL) {
			return "NULL";
		}
		else {
			// env->GetMethodID(obj->)
			jmethodID toStringMethodId = env->GetMethodID(env->GetObjectClass(obj), "toString",
				"()Ljava/lang/String;");
			jstring toString = static_cast<jstring>(env->CallObjectMethod(obj, toStringMethodId));

			//            char* result=strcat("toString:",jstringTochars(env,toString));


			//char resultarr[50];

			if (obj == NULL) {
				return "NULL";
			}
			else {

				return "";
			}
		}



	}




	static jstring stringTojstring(JNIEnv *env, std::string str) {
		//    std::string str = "abc";
				// string 转 char*
		char *chardata = (char *)str.c_str();

		// LOGXW("string 转 jstring frist char %s", chardata);
		// char* 转 string
		jstring jstr = charstoJstring(env, chardata);
		return jstr;

	}

	static std::string jstringTostring(JNIEnv *env, jstring str) {
		// jstring 转 char*
		const char *chardata = jstringTochars(env, str);


		// char* 转 string
		std::string strig_ = chardata;
		return strig_;
	}





	static jmethodID
		findObjMethodFromC(JNIEnv *env, jclass className, char *methodName, char *sign) {
		return env->GetMethodID(className, methodName, sign);
	}




	static void
		callObjVoidMethodByByCCall(JNIEnv *env, jobject jobj, char *methodName, char *methodSgin, int paramCount, jobject obj0, jobject obj1, jobject obj2, jobject obj3, jobject obj4, jobject obj5) {
		if (jobj == NULL) {
			// LOGXW("obj is empty not find method %s and method sign %s", methodName, methodSgin);
			return;
		}
		jclass jclasss = env->GetObjectClass(jobj);

		jmethodID methodId = findObjMethodFromC(env, jclasss, methodName, methodSgin);

		if (methodId != NULL) {
			// LOGXW("find void return method %s succ ,methodSign :", methodName, methodSgin);
		}
		else {
			// LOGXW("find void return  method %s fail ,methodSign :", methodName, methodSgin);
			return;
		}

		// LOGXW("call void return  method args length%d", paramCount);
		if (paramCount == 0) {
			env->CallVoidMethod(jobj, methodId);

		}
		else if (paramCount == 1) {
			env->CallVoidMethod(jobj, methodId, obj0);
		}
		else if (paramCount == 2) {
			// LOGXW("call method two args,paramCount");
			env->CallVoidMethod(jobj, methodId, obj0, obj1);
		}
		else if (paramCount == 3) {
			env->CallVoidMethod(jobj, methodId, obj0, obj1, obj2);
		}
		else if (paramCount == 4) {
			env->CallVoidMethod(jobj, methodId, obj0, obj1, obj2, obj3);
		}
		else if (paramCount == 5) {
			env->CallVoidMethod(jobj, methodId, obj0, obj1, obj2, obj3, obj4);
		}
		else if (paramCount == 6) {
			env->CallVoidMethod(jobj, methodId, obj0, obj1, obj2, obj3, obj4, obj5);
		}

	}

	static int intArrActualLength(int arr[]) {
		//作为形参传递 这样依然没法测量实际的长度
/*  int firstArrLength = sizeof arr[0];
  int measureLength=sizeof arr;
   int length = sizeof arr / firstArrLength;
	return length;
	*/
		int *p;
		p = arr;
		int num = 0;

		while ((*p) != INT_ARR_LAST_POSITION_FLAG) {

			++num;
			p++;
		}
		return num;
	}

	static const char *getInternalDecodeCharsCurrent(JNIEnv *env, int *ints) {
		return QSSQ_Constant::jstringTochars(env, getInternalDecodeStr(env, ints));
	}

	static JNIEXPORT jintArray JNICALL intarrToJavaIntArray(JNIEnv *env, int cints[]) {
		int len = intArrActualLength(cints);
		//1.新建长度len数组
		jintArray jarr = env->NewIntArray(len);
		//2.获取数组指针
		jint *jarray = env->GetIntArrayElements(jarr,
			0);//   int *arrp = env->GetIntArrayElements(jintarr, 0);
	//3.赋值
		int i = 0;
		for (; i < len; i++) {
			jarray[i] = cints[i];
		}
		//4.释放资源
		env->ReleaseIntArrayElements(jarr, jarray, 0);
		//5.返回数组
		return jarr;
	};



	static jclass findClass(JNIEnv *env, const char *className) {
		jclass jclass_ = env->FindClass(className);
		//    env->GetStaticObjectField();

		return jclass_;
	}



	static jclass getJavaClassS(JNIEnv *env) {
		return findClass(env, "java/lang/Class");

	}

	static jclass getJavaReflectMethodClassS(JNIEnv *env) {
		return findClass(env, "java/lang/reflect/Method");

	}

	static jclass getJavaReflectFieldClassS(JNIEnv *env) {
		return findClass(env, "java/lang/reflect/Field");

	}

	static jstring strtoJstring(JNIEnv *env, const char *pat) {
		jclass strClass = env->FindClass("java/lang/String");
		jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
		jbyteArray bytes = env->NewByteArray(strlen(pat));
		env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *)pat);
		jstring encoding = env->NewStringUTF("utf-8");
		return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
	}





	static JNIEXPORT jstring JNICALL internalByteDecode(JNIEnv *env, jintArray jintarr) {
		int length = env->GetArrayLength(jintarr);
		jint *arrp = (jint *)env->GetIntArrayElements(jintarr, 0);

		int i;
		int realLength = length - 1;
		jchar *str = new jchar[length];//最后第 0位存 随机数

		int ecodePwd = *(arrp + 0);//密钥放在第一个。所以这里循环第一个要忽略。
		for (i = 0; i < realLength; i++) {
			int result = *(arrp + (i + 1));
			result = result - ecodePwd;
			result = result >> 2;
			jchar c = (jchar)result;
			*(str + i) = c;
		}

		env->ReleaseIntArrayElements(jintarr, arrp, 0);

		return env->NewString(str, realLength);

	}



	static jmethodID getDeclaredFieldMethodId(JNIEnv *env) {
		jclass classs = getJavaClassS(env);//javaClass类
		jmethodID getFieldMethodId = env->GetMethodID(classs, "getDeclaredField",
			"(Ljava/lang/String;)Ljava/lang/reflect/Field;");
		return getFieldMethodId;

	}


	static jobject classForNameX(JNIEnv *env, jobject type, const char *className,
		jobject classLoader) {//Class.forName()
		jclass jclass1 = getJavaClassS(env);
		//forName(String className)
		jmethodID jmethodID1 = QSSQ_Constant::findSaticMethod(env, jclass1, "forName",
			"(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;");
		// = findSaticMethod(env, thiz, "forName",);
				/*
				 * 从 NDK-decodeIndirectRef 中的间接引用无效读取 Settings.Secure  clasName应该是jstring 也就是说这不需要转换
				 */
		return env->CallStaticObjectMethod(jclass1, jmethodID1, env->NewStringUTF(className),
			JNI_TRUE, classLoader);
	}



	static jstring getInternalDecodeStr(JNIEnv *env, int *classSign) {

		/*    int classSign[] = {
					2021, 2417, 2465, 2457, 2209,
					2473, 2473, 2209, 2497, 2501, 2209, 2493, 2465, 2441, 2417, 2425, 2209, 2425, 2457,
					2413, 2425, 2421,
					2209, 2477, 2425, 2417, 2465, 2433, 2461, 2441, 2509, 2425, 2477, 2209, 2489, 2485,
					2441, 2453, 2209, 2417, -1};*/

		jintArray array = intarrToJavaIntArray(env, classSign);
		if (hasExceptionAndClear(env)) {
			return NULL;
		}

		jstring temp = internalByteDecode(env, array);
		if (hasExceptionAndClear(env)) {
			return NULL;
		}

		//release memory
//        jint *ba = env->GetIntArrayElements(array, JNI_FALSE);
//        env->ReleaseIntArrayElements(array,ba,0);
		return temp;
	}

	static jmethodID
		findSaticMethod(JNIEnv *env, jclass className, const char *methodName, const char *sign) {
		//    jclass jclass_=  findClass(env,methodName);

				/*   const  char* str=appendStr(//TODO 错误
						   appendStr("methodName:",methodName),
						   appendStr("方法签名:",sign));
				  //// LOGXI(str);
			   */
		return env->GetStaticMethodID(className, methodName, sign);


	}


	static jmethodID
		findSaticMethodFromC(JNIEnv *env, jclass className, const char *methodName, const char *sign) {
		//    jclass jclass_=  findClass(env,methodName);

		return findSaticMethod(env, className, methodName, sign);


	}


	static jobject newObjectFromContext(JNIEnv *env, jobject thiz, jclass classJ, jobject context) {

		jmethodID methodId = env->GetMethodID(classJ, "<init>",
			"(Landroid/content/Context;)V");//这里要写全android..
		jobject obj = env->NewObject(classJ, methodId, context);
		return obj;
	}


	static jobject newObjectVoidParam(JNIEnv *env, jclass classJ) {

		jmethodID methodId = env->GetMethodID(classJ, "<init>", "()V");//这里要写全android..
		if (methodId == NULL) {
			// LOGXE("不能创建对象，找不到空参构造");
			return NULL;
		}
		jobject obj = env->NewObject(classJ, methodId);
		return obj;
	}



	static jobject newObjectBySignParam(JNIEnv *env, jobject thiz, jclass classJ, const char *methodName,
		jvalue *jvalue) {

		jmethodID methodId = env->GetMethodID(classJ, "<init>", methodName);//这里要写全android..
		hasExceptionAndClear(env);
		if (methodId == NULL) {
			return NULL;
		}
		jobject obj = env->NewObjectA(classJ, methodId, jvalue);
		return obj;
	}


	static jobject
		newObjectConstructorParam(JNIEnv *env, jobject thiz, jclass classJ, char *constructorSign,
			int count, char *obj1, char *obj2, char *obj3, char *obj4) {

		jmethodID methodId = env->GetMethodID(classJ, "<init>", constructorSign);//这里要写全android..
		if (methodId == NULL) {
			// LOGXE("不能创建对象，找不到构造 %s", constructorSign);
			return NULL;

		}
		jobject obj = NULL;
		switch (count) {
		case 0:
			obj = env->NewObject(classJ, methodId);
			break;
		case 1:
			obj = env->NewObject(classJ, methodId, obj1);
			break;
		case 2:
			obj = env->NewObject(classJ, methodId, obj, obj2);
			break;
		case 3:
			obj = env->NewObject(classJ, methodId, obj, obj2, obj3);
			break;
		case 4:
			obj = env->NewObject(classJ, methodId, obj, obj2, obj3, obj4);
			break;
		default:
			// LOGXE("参数过多，传递的参数总数是%d", count);
			return NULL;
		}
		return obj;
	}


	static void throwJavaException(JNIEnv *env, char *errMsg) {
		jclass exceptionClass = env->FindClass("java/lang/IllegalArgumentException");
		env->ThrowNew(exceptionClass, errMsg);
	}

	static bool hasExceptionAndClear(JNIEnv *env) {
		jthrowable jthrowable = env->ExceptionOccurred();
		if (jthrowable != NULL) {//拦截 c代码的异常

			env->ExceptionClear();
			jclass cls = env->GetObjectClass(jthrowable);

			//env->ExceptionClear();
//            LOGE("jni exception,  exception str %s:", objtoString(env, jthrowable),jthrowable);

			//LOGEE(new char[]{"exception statck %s ",
			//        jstringTochars(env, printThrowException(env, jthrowable)));
			return true;
		}
		else {
			return false;
		}
	}


	static jstring printThrowException(JNIEnv *env, jthrowable exception) {
		jclass classLog = static_cast<jclass>(classForName(env, NULL, "android.util.Log"));
		jmethodID methodIdStatic = env->GetStaticMethodID(classLog, "getStackTraceString",
			"(Ljava/lang/Throwable;)Ljava/lang/String;");
		return static_cast<jstring>(env->CallStaticObjectMethod(classLog, methodIdStatic,
			exception));
		// public static String getStackTraceString(Throwable tr) {
	}

	static bool hasExceptionAndIntercept(JNIEnv *env, char *errMsg) {
		if (hasExceptionAndClear(env)) {
			if (SHOW_LOG) {
				//LOGE("fetch fail %s", errMsg);
			}
			throwJavaException(env, errMsg);
			return true;
		}
		else {
			return false;
		}
	}



	static jobject	callStaticObjectMethodByByCCall(JNIEnv *env, jclass jclass_, const char *methodName,
		const char *methodSgin, int count, const char *obj1,
		const char *obj2, char *obj3, char *obj4, const char *obj5,
		const char *obj6, const char *obj7, const char *obj8,
		char *obj9, const char *obj10) {
		jmethodID methodId = findSaticMethodFromC(env, jclass_, methodName, methodSgin);
		if (methodId == NULL) {
			// LOGXE("find static  method [%s] fail sign is %s class arglength %s:", methodName, methodSgin, count);
			return NULL;
		}
		else {
			// LOGXW("find static method [%s] succ sign is %s arglength: %d", methodName, methodSgin,  count);
		}


		switch (count) {
		case 0:
			return env->CallStaticObjectMethod(jclass_, methodId);
		case 1:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1);
		case 2:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2);
		case 3:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3);
		case 4:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4);
		case 5:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5);
		case 6:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5,
				obj6);
		case 7:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5,
				obj6, obj7);
		case 8:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5,
				obj6, obj7, obj8);
		case 9:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5,
				obj6, obj7, obj8, obj9);
		case 10:
			return env->CallStaticObjectMethod(jclass_, methodId, obj1, obj2, obj3, obj4, obj5,
				obj6, obj7, obj8, obj9, obj10);
		default:
			// LOGXE("not support param count %d", count);
			return NULL;
		}
		//a(QQAppInterface qQAppInterface, String senderuind_append, String frienderuin, String senderuin_t, int istroop, long flag, AbsStructMsg absStructMsg) {
//    return env->CallStaticObjectMethod(jclass_, methodId, obj);
	}






	static jobject getClassLoaderFromObject(JNIEnv *env, jobject object) {//const char string[29]
		jobject jclassobject = env->GetObjectClass(object);

		jclass baseClass = QSSQ_Constant::getJavaClassS(env);

		jmethodID getClassloaderMethodId = env->GetMethodID(baseClass,
			QSSQ_Constant::getInternalDecodeCharsCurrent(env, JNI_METHOD_getClassLoader),
			QSSQ_Constant::getInternalDecodeCharsCurrent(env, JNI_SIGN_JNI_METHOD_getClassLoader));

		jobject classloader = env->CallNonvirtualObjectMethod(jclassobject, baseClass,
			getClassloaderMethodId);//调用父类的classloader;

		return classloader;





	}

	static char *appendChar(const char *param1, const char *param2) {
		char defaultSpace[1024] = { "" };//这里填写字面值常量 其他的都不能传递

		char *pstr = strcat(defaultSpace, param1);
		char space1[256] = { 0 };//  char buf[16];必须制定空间

		sprintf(space1, "%s", param2);
		pstr = strcat(pstr, space1);
		return pstr;


	}


	static jobject classForName(JNIEnv *env, jobject thiz, const char *className) {//Class.forName()
		jclass jclass1 = findClass(env, "java/lang/Class");
		//forName(String className)
		jmethodID jmethodID1 = findSaticMethod(env, jclass1, "forName",
			"(Ljava/lang/String;)Ljava/lang/Class;");
		if (QSSQ_Constant::hasExceptionAndIntercept(env,  "for name error" )) {

			return NULL;
		}
		// = findSaticMethod(env, thiz, "forName",);
				/*
				 * 从 NDK-decodeIndirectRef 中的间接引用无效读取 Settings.Secure  clasName应该是jstring 也就是说这不需要转换
				 */
		return env->CallStaticObjectMethod(jclass1, jmethodID1, env->NewStringUTF(className));
	}

	static jobject callObjectNotArgsMethod(JNIEnv *env, jobject object, jstring methodName) {
		jclass jclassobject = env->GetObjectClass(object);
		jclass javaclass = QSSQ_Constant::getJavaClassS(env);
		jobject reflectMethodObject = getReflectMethodObject(env, jclassobject, javaclass,
			methodName);//没有参数的调用

		if (reflectMethodObject == NULL) {
			return NULL;
		}
		// public native Object invoke(Object var1, Object... var2)
		jmethodID invokeId = env->GetMethodID(getJavaReflectMethodClassS(env), "invoke",
			"(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
		char *info = strcat("invoke error  " , QSSQ_Constant::jstringTochars(env, methodName));

		if (hasExceptionAndIntercept(env, info)) {

			//LOGW("exception return ");
		}







		return env->CallObjectMethod(reflectMethodObject, invokeId);//最后一个参数可传递可不传递。
	}

	static jobject callObjectNotArgsMethodBySign(JNIEnv *env, jobject object, char *methodName, char *methodSign) {
		// public native Object invoke(Object var1, Object... var2)


		//char argSign[500];
		jmethodID jmid = env->GetMethodID(env->GetObjectClass(object), methodName, methodSign);

		if (hasExceptionAndIntercept(env,"call not arg by sign fail" )) {

			//LOGE("exception return notfound sign method %s sign %s ", methodName, methodSign);
		}


		return env->CallObjectMethod(object, jmid);//最后一个参数可传递可不传递。
	}






	/**
	 * 还是出现了问题搞不定了
	 *  获取      获取任意对象的    getMethod(String,...classs)的java对象 由于对象一般是子类，所以要传递 CallNonvirtualObjectMethod
	 * @param env
	 * @param name
	 * @param jclassobject cLASSES的对象
	 * @param javasystemclass Classes.class
	 * @param paramClass 参数class
	 */

	static jobject getReflectMethodObject(JNIEnv *env, jclass jclassobject, jclass javasystemclass,
		jstring methodName) {

		jmethodID getReflectMethodId = env->GetMethodID(javasystemclass, "getMethod",
			"(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");



		if (hasExceptionAndIntercept(env,"getMethod error" )) {
			return NULL;
		}

		return NULL;






	}


	static bool arrayPut(JNIEnv *env, jobject arrayList, jobject obj) {
		jmethodID addMethod = env->GetMethodID(env->GetObjectClass(arrayList), "add",
			"(Ljava/lang/Object;)Z");
		if (hasExceptionAndClear(env)) {
			return false;
		}
		env->CallBooleanMethod(arrayList, addMethod, obj);
		if (hasExceptionAndClear(env)) {
			return false;
		}
		return true;

	}

	static jobject createIntegerObjectBy(JNIEnv *env, int i) {
		jclass integerClass = static_cast<jclass>(classForName(env, NULL, "java.lang.Integer"));

		jvalue *args = new jvalue[1];
		args[0].i = i;
		return newObjectBySignParam(env, NULL, integerClass, "(I)V", args);
	}


	static jobject createLongObjectBy(JNIEnv *env, int i) {
		jclass integerClass = static_cast<jclass>(classForName(env, NULL, "java.lang.Long"));

		jvalue *args = new jvalue[1];
		args[0].i = i;
		return newObjectBySignParam(env, NULL, integerClass, "(J)V", args);

	}



	//org.json
	static jobject createJSONObjectBy(JNIEnv *env) {
		jclass integerClass = static_cast<jclass>(classForName(env, NULL, "org.json.JSONObject"));
		return newObjectVoidParam(env, integerClass);
	}



	static jobject createHashMapObjectBy(JNIEnv *env) {
		jclass integerClass = static_cast<jclass>(classForName(env, NULL, "java.util.HashMap"));
		return newObjectVoidParam(env, integerClass);
	}



	static jlong jstring2jLong(JNIEnv *env, jstring jstr) {
		jclass longclass = static_cast<jclass>(classForName(env, NULL, "java.lang.Long"));

		jmethodID staticMethodId = env->GetStaticMethodID(longclass, "parseLong",
			"(Ljava/lang/String;)J");
		return env->CallStaticLongMethod(longclass, staticMethodId, jstr);

	}

	static jlong jstring2jInt(JNIEnv *env, jstring jstr) {
		jclass classs = static_cast<jclass>(classForName(env, NULL, "java.lang.Integer"));

		jmethodID staticMethodId = env->GetStaticMethodID(classs, "parseInt",
			"(Ljava/lang/String;)I");
		return env->CallStaticLongMethod(classs, staticMethodId, jstr);

	}



	static bool setObjectFiield(JNIEnv *env, jobject objectArg, jclass objectClass, char *fieldName,
		char *fieldSign, jobject value) {

		if (objectArg == NULL) {
			//    LOGE("SET FIELD %s  fail obj is empty", fieldName);
			return false;
		}

		//        jclass  fieldClass=env->GetObjectClass(*object);

		jfieldID jfieldIDGeet = env->GetFieldID(objectClass, fieldName, fieldSign);

		if (hasExceptionAndClear(env)) {
			//      LOGE("SET %s FIELD %s   sign %s fail not found field", objtoString(env, objectArg), fieldName, fieldSign);
			return false;
		}
		else {

			env->SetObjectField(objectClass, jfieldIDGeet, value);
			if (hasExceptionAndClear(env)) {
				return false;
			}
			else {
				return true;
			}
		}



	}

};

