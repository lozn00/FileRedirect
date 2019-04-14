#include "QSSQ_CheckEnvImpl.h"
#include "QSSQ_Constant.cpp"
#include "md5Helper.c"
#include <string>
#include "base6_with_test.c"
#include <sys/types.h>
#include <android/Foundation/ALog.h>

//#define  //LOGW //printf
//#include<Mspthrd.h>
QSSQ_CheckEnvImpl::QSSQ_CheckEnvImpl() {


}


QSSQ_CheckEnvImpl::~QSSQ_CheckEnvImpl() {


}

//#define  //LOGW //printf
jstring getsign(JNIEnv *env, jclass type, jobject context_object) {
    jclass context_class = env->GetObjectClass(context_object);

    char getPackageManagerSignBuffer[600];
    char getPackageManagerMethodBuffer[600];
    jmethodID methodId = env->GetMethodID(context_class, decryptWrap("Z2V0UGFja2FnZU1hbmFnZXI=",
                                                                     getPackageManagerMethodBuffer),
                                          decryptWrap(
                                                  "KClMYW5kcm9pZC9jb250ZW50L3BtL1BhY2thZ2VNYW5hZ2VyOw==",
                                                  getPackageManagerSignBuffer));

    if (methodId == NULL) {
        env->ExceptionClear();
        //LOGW("not found getPackageName");
    }
    jobject package_manager_object = env->CallObjectMethod(context_object, methodId);
    if (package_manager_object == NULL) {
        // LOGI("getPackageManager() Failed!");

        //LOGW("not found getPackageName");
        return NULL;
    }

    char getPackageNameBuffer[60];
    methodId = env->GetMethodID(context_class,
                                decryptWrap("Z2V0UGFja2FnZU5hbWU=", getPackageNameBuffer),
                                "()Ljava/lang/String;");

    if (methodId == NULL) {
        //LOGW("not found getPackageName");
    }
    jstring package_name_string = (jstring) env->CallObjectMethod(context_object, methodId);
    if (package_name_string == NULL) {
        // LOGI("getPackageName() Failed!");
        //LOGW(" packageName null");
        return NULL;
    }
    env->DeleteLocalRef(context_class);
    jclass pack_manager_class = env->GetObjectClass(package_manager_object);
    char buffGetPackageInfoSign[500];
    char getPackageInfoBuffer[500];

    methodId = env->GetMethodID(pack_manager_class,
                                decryptWrap("Z2V0UGFja2FnZUluZm8=", getPackageInfoBuffer),
                                decryptWrap(
                                        "KExqYXZhL2xhbmcvU3RyaW5nO0kpTGFuZHJvaWQvY29udGVudC9wbS9QYWNrYWdlSW5mbzs=",
                                        buffGetPackageInfoSign)//"(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;"
    );

    if (methodId == NULL) {
        //LOGW("getPackageInfo null");
    }
    env->DeleteLocalRef(pack_manager_class);
    jobject package_info_object = env->CallObjectMethod(package_manager_object, methodId,
                                                        package_name_string, 694886);//hook ,not 64
    if (package_info_object == NULL) {
        // LOGI("getPackageInfo() Failed!");
        //LOGW("getPackageInfo call return null");
//    env->Th
        exit(0);//查询不到也给。
//        return NULL;
    }
/*
 *  encrypt [signatures] -> [c2lnbmF0dXJlcw==]
 encrypt [[Landroid/content/pm/Signature;] -> [W0xhbmRyb2lkL2NvbnRlbnQvcG0vU2lnbmF0dXJlOw==]
 */
    env->DeleteLocalRef(package_manager_object);

    jclass package_info_class = env->GetObjectClass(package_info_object);

    char signaturesFieldNameBuffer[200];
    char signatureSignBuffer[200];
    jfieldID fieldId = env->GetFieldID(package_info_class, decryptWrap("c2lnbmF0dXJlcw==",
                                                                       signaturesFieldNameBuffer)//signatures
            , decryptWrap("W0xhbmRyb2lkL2NvbnRlbnQvcG0vU2lnbmF0dXJlOw==",
                          signatureSignBuffer));//[Landroid/content/pm/Signature;
    env->DeleteLocalRef(package_info_class);
    jobjectArray signature_object_array = (jobjectArray) env->GetObjectField(package_info_object,
                                                                             fieldId);
    if (signature_object_array == NULL) {
        LOGE("PackageInfo.signatures[] is null。。");
        exit(0);
//        return NULL;
        //        return NULL;
    }
    jobject signature_object = env->GetObjectArrayElement(signature_object_array, 0);
    env->DeleteLocalRef(signature_object_array);
    env->DeleteLocalRef(package_info_object);
    jclass signature_class = env->GetObjectClass(signature_object);
    char toCharsStringBuffer[300];

    methodId = env->GetMethodID(signature_class,
                                decryptWrap("dG9DaGFyc1N0cmluZw==", toCharsStringBuffer),
                                "()Ljava/lang/String;");
    env->DeleteLocalRef(signature_class);
    jstring signature_string = (jstring) env->CallObjectMethod(signature_object, methodId);
    return signature_string;
}

/**
 * check sign
 * @param env
 */
void QSSQ_CheckEnvImpl::checkMemoryLZ(JNIEnv *env) {


    //string tempjs = Constant::charstostringFix(env, new char[15]{ "bbbb" });
    //MessageBoxA(0, tempjs.c_str(), "消息", 3);
    //	const char* temp = tempjs->c_str();
    //activityThreadClass=	env->FindClass("Landroid/app/ActivityThread;");
    //MessageBoxA(0, temp, "消息", 3);
    // Constant::javaClass2ClassSign(charsActivityThread).c_str();
    char activityThreadClassBuffer[70];
    const char *charsActivityThread = decryptWrap("YW5kcm9pZC5hcHAuQWN0aXZpdHlUaHJlYWQ=",
                                                  activityThreadClassBuffer);// "android.app.ActivityThread";
    jclass activityThreadClass = (jclass) QSSQ_Constant::classForName(env, NULL,
                                                                      charsActivityThread);
    if (activityThreadClass == NULL) {
        std::string tempstring = QSSQ_Constant::javaClass2ClassSign(charsActivityThread);
        const char *temp = tempstring.c_str();
        charsActivityThread = temp;
        //printf(" replace after  %s  ", charsActivityThread);
        activityThreadClass = env->FindClass(charsActivityThread);
    }
    if (activityThreadClass == NULL) {
        LOGE("a t null");
        exit(0);
        //printf("	not found athread ");
        return;
    }

    char currentApplicationMethodBuffer[170];
    char currentApplicationMethodSignBuffer[400];

    /*
     *
     *
     */
    jmethodID currentApplicationId = env->GetStaticMethodID(activityThreadClass,
                                                            decryptWrap("Y3VycmVudEFwcGxpY2F0aW9u",
                                                                        currentApplicationMethodBuffer),
//                                                            decryptWrap("KClMYW5kcm9pZC9hcHAvQXBwbGljYXRpb247",currentApplicationMethodSignBuffer))
                                                            "()Landroid/app/Application;");
    if (currentApplicationId == NULL) {

        //printf("	没有找到application method");
        LOGE("app me e  null");
        env->ExceptionClear();
        return;

    }

    jobject jobjectApplication = env->CallStaticObjectMethod(activityThreadClass,
                                                             currentApplicationId);
    if (jobjectApplication == NULL) {
        env->ExceptionClear();//currentApcation empty
//        LOGE("call rt  null");
        return;

    }
    jstring signJstring = getsign(env, NULL, jobjectApplication);
    const char *signstr = env->GetStringUTFChars(signJstring, NULL);
    char result[1204] = {'\0'};
    unsigned char key[1024];
    strcpy(reinterpret_cast<char *>(key), signstr);
//    env->DeleteLocalRef(jobjectApplication);
    md5Encrypt(key, result);
    bool iscrack;
    if (ENABLEDEBUG) {
//    strcpy(result, "262e5a7782950b5579d0a68e42638ee6");//模拟为正确的签名。

    }
    iscrack =
            strcmp(reinterpret_cast<const char *>(result), "262e5a7782950b5579d0a68e42638ee6") != 0;

    if (iscrack) {

        LOGE("PackageInfo.signatures[] is crack .........");
        LOGE("PackageInfo.signatures[] is crackx %s ,signstr:%s", result,signstr);
        exit(0);

    } else {
        LOGW("sign_right ......... %s ", result);
    }

    env->ReleaseStringUTFChars(signJstring, signstr);
}

/**
 * 检查hook proxy
 * @param env
 */

void QSSQ_CheckEnvImpl::hKillerLZ(JNIEnv *env) {
    char buffer[200];
    const char *charsActivityThread = "android.app.ActivityThread";
    jclass activityThreadClass = (jclass) QSSQ_Constant::classForName(env, NULL,
                                                                      charsActivityThread);
    if (activityThreadClass == NULL) {
        std::string tempstring = QSSQ_Constant::javaClass2ClassSign(charsActivityThread);
        const char *temp = tempstring.c_str();
        charsActivityThread = temp;
        //printf(" replace after  %s  ", charsActivityThread);
        activityThreadClass = env->FindClass(charsActivityThread);
    }

    if (activityThreadClass == NULL) {

        LOGE("atcl  null");
        exit(0);
        return;
    }

    //env->AllocObject(activityThreadClass);
    jmethodID currentActivityThreadId = env->GetStaticMethodID(activityThreadClass,
                                                               "currentActivityThread",
                                                               "()Landroid/app/ActivityThread;");
    if (currentActivityThreadId == NULL) {
        LOGE("currentActivityThreadId  null");
        exit(0);
    }

    jfieldID packageManagerField = env->GetStaticFieldID(activityThreadClass, "sPackageManager",
                                                         "Landroid/content/pm/IPackageManager;");

    jobject packageManager = env->GetStaticObjectField(activityThreadClass, packageManagerField);
    env->DeleteLocalRef(activityThreadClass);
    if (packageManager == NULL) {
        LOGE("P null");
        //printf("not found packageManager ");
        return;
    }
    std::string str = QSSQ_Constant::getObjClassName(env, packageManager);
    env->DeleteLocalRef(packageManager);
    //printf("当前包管理器类名 %s: ", str.c_str());

    //printf("是否相等  %d: ", strcmp(str.c_str(), "android.content.pm") == 0);
    //printf("寻找位置 %zd: ", str.find("android.content.pm"));
    /* if (str.find("Proxy") != std::string::npos) {//android.content.pm.IPackageManager$Stub$Proxy
         LOGE("px_ %s",str.c_str());
         //biaoshi找到了。代理类破解。
         exit(0);
     }*/
    ///如果没找到，返回一个特别的标志c++中用npos表示  !=0yejiushi 也 不是 特定开头。 不是以这些开头就结束。
    if (str.find("android.content.pm") != 0 && str.find("com.zte.ZTESecurity") != 0 &&
        str.find("qssq666") == std::string::npos &&
        str.find("android.content.pm.IPackageManager") != 0
        && str.find("android.content.pm.IPackageManager$Stub$Proxy") != 0
        && str.find("com.mi") != 0) {
        LOGE("x_r");
        exit(0);
        abort();
    } else {
        LOGE("v_v");
    }
    /*
     * jni内存泄漏规范 https://blog.csdn.net/ccm_oliver/article/details/12781319
     */

}
