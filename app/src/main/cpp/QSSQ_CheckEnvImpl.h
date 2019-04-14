#pragma once
#include<jni.h>

class QSSQ_CheckEnvImpl
{
public:
	QSSQ_CheckEnvImpl();
	~QSSQ_CheckEnvImpl();

void checkMemoryLZ(JNIEnv *env,int queryCode);
void	hKillerLZ(JNIEnv* env);
};

