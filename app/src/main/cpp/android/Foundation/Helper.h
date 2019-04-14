//
// VirtualApp Native Project
//

#ifndef NDK_LOG_H
#define NDK_LOG_H

#include "JniHelper.h"
#include "ALog.h"

#include <jni.h>

#define NATIVE_METHOD(func_ptr, func_name, signature) { func_name, signature, reinterpret_cast<void*>(func_ptr) }

class ScopeUtfString {
public:
    ScopeUtfString(JNIEnv *env, jstring j_str) : _j_str(j_str),
                                                 _c_str(env->GetStringUTFChars(j_str, NULL)) {
        LOGW("创建了字符串");
    }

    const char *c_str() {
        return _c_str;
    }

    ~ScopeUtfString() {
        JniHelper::getEnv()->ReleaseStringUTFChars(_j_str, _c_str);
        LOGW("释放了字符串");
    }

private:
    jstring _j_str;
    const char *_c_str;
};

#endif //NDK_LOG_H
