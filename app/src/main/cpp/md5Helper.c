#include "md5.c"
#include <stdlib.h>
#include <stdio.h>

// need release str

char *appendStrByMalloc(char *first, char *second) {
    char *name = (char *) malloc(strlen(first) + strlen(second));
    sprintf(name, "%s%s", first, second);
    return name;
}

char *appendStrMethod2(char *first, char *second, char *appendResult) {
    sprintf(appendResult, "%s%s", first, second);
    return appendResult;
}

char *appendStrMethod1(char *first, char *second, char *appendResult) {
    strcpy(appendResult, first);
    strcat(appendResult, second);

    return appendResult;
}

void md5Encrypt(unsigned char *str, char *encryptResult) {
    unsigned char decrypt[16];
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, str, strlen((char *) str));
    MD5Final(&md5, decrypt);
    int i;
    // char result[32]={};
    for (i = 0; i < 16; i++) {
        char temp[10];
        sprintf(temp, "%02x", decrypt[i]);
        if (i == 0) {
            strcpy(encryptResult, temp);
        } else {
            appendStrMethod1(encryptResult, temp, encryptResult);
        }
    }

    // printf("加密前:%s\n加密后:", encrypt);
}

int mainx() {
    int i;
    unsigned char encrypt[] = "admin"; //21232f297a57a5a743894a0e4a801fc3
    unsigned char decrypt[16];
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, encrypt, strlen((char *) encrypt));
    MD5Final(&md5, decrypt);
    printf("加密前:%s\n加密后: ...\n", encrypt);
    for (i = 0; i < 16; i++) {
        printf("%02x", decrypt[i]);
    }
    printf("\n");
    //262e5a7782950b5579d0a68e42638ee6 
    char result[32] = {'\0'};
    unsigned char key[] = "30820284308201eda00302010202044a447343300d06092a864886f70d01010b05003075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f7265301e170d3136313130373135333432395a170d3434303332343135333432395a3075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f726530819f300d06092a864886f70d010101050003818d0030818902818100850bdefebdf1040d15adf4a1370796ddd9816fbb414ddca4585a9ae186ef3aa17344849b8834616b7d57c0781a2edc4eb1390dba10db0c686f9860f5d6655161d6a7acea5da7a861ad52509bf5e28bdb7e521818e3209797c6a5e92ce56b3a6e94d0981dde29b82bde57dafb74a231a1e70cb1241ddff6ce40ab47e3f37b63bd0203010001a321301f301d0603551d0e04160414602a55d56209c16ebb013236938db93823d085d8300d06092a864886f70d01010b0500038181005fe284d71999b64d8d47c00be4eeecb8edf0844a03c2c409fd1d2cb7a3c0ba74bd59583eb4609a084765fea260fbd7c943363f125cb620c5dfcd7d656fab3b20798f91aba2a4f243c8156b9e76b70ae6ca41260f802721364ec5e19a498db285189de2e991ebeabbfd47677fdd155b3d3b4ea673a3f4b411fe561c2bbc203a09";
    // unsigned char key[] = "admin";
    md5Encrypt(key, result);
    printf("md5 : %s", result);
    // getchar();

    return 0;
}