#pragma once
#include<jni.h>

class QSSQ_CheckEnvImpl
{
public:
	QSSQ_CheckEnvImpl();
	~QSSQ_CheckEnvImpl();

void checkMemoryLZ(JNIEnv *env);
void	hKillerLZ(JNIEnv* env);
};

