#include <stdio.h> //注意哦，VC中""是当前路径，<>是系统路径
#include <stdlib.h>
#include <string.h>

const char BASE_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; //base码

//编码，参数：要编码的字符串指针，解码后存放的位置（编码字串长度的4/3），要编码的字符串长度
//->返回结果长度
int fnBase64Encode(char *lpString, char *lpBuffer, int sLen)
//参数：明文，缓冲区，明文长度
{
    register int vLen = 0; //寄存器局部变量，提速
    while (sLen > 0)       //处理整个字符串
    {
        *lpBuffer++ = BASE_CODE[(lpString[0] >> 2) & 0x3F]; //右移两位，与00111111是防止溢出，自加
        if (sLen > 2)                                       //够3个字符
        {
            *lpBuffer++ = BASE_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];
            *lpBuffer++ = BASE_CODE[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];
            *lpBuffer++ = BASE_CODE[lpString[2] & 0x3F];
        } else {
            switch (sLen) //追加“=”
            {
                case 1:
                    *lpBuffer++ = BASE_CODE[(lpString[0] & 3) << 4];
                    *lpBuffer++ = '=';
                    *lpBuffer++ = '=';
                    break;
                case 2:
                    *lpBuffer++ = BASE_CODE[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];
                    *lpBuffer++ = BASE_CODE[((lpString[1] & 0x0F) << 2) | (lpString[2] >> 6)];
                    *lpBuffer++ = '=';
                    break;
            }
        }
        lpString += 3;
        sLen -= 3;
        vLen += 4;
    }
    *lpBuffer = 0;
    return vLen;
}

//子函数 - 取密文的索引
//inline char GetCharIndex(char c) //内联函数可以省去函数调用过程，提速
 char GetCharIndex(char c) //内联函数可以省去函数调用过程，提速
{
    if ((c >= 'A') && (c <= 'Z')) {
        return c - 'A';
    } else if ((c >= 'a') && (c <= 'z')) {
        return c - 'a' + 26;
    } else if ((c >= '0') && (c <= '9')) {
        return c - '0' + 52;
    } else if (c == '+') {
        return 62;
    } else if (c == '/') {
        return 63;
    } else if (c == '=') {
        return 0;
    }
    return 0;
}

//解码，参数：结果，密文，密文长度
int fnBase64Decode(char *lpString, char *lpSrc, int sLen) //解码函数
{
    static char lpCode[4];
    register int vLen = 0;
    if (sLen % 4) //Base64编码长度必定是4的倍数，包括'='
    {
        lpString[0] = '\0';
        return -1;
    }
    while (sLen > 2) //不足三个字符，忽略
    {
        lpCode[0] = GetCharIndex(lpSrc[0]);
        lpCode[1] = GetCharIndex(lpSrc[1]);
        lpCode[2] = GetCharIndex(lpSrc[2]);
        lpCode[3] = GetCharIndex(lpSrc[3]);

        *lpString++ = (lpCode[0] << 2) | (lpCode[1] >> 4);
        *lpString++ = (lpCode[1] << 4) | (lpCode[2] >> 2);
        *lpString++ = (lpCode[2] << 6) | (lpCode[3]);

        lpSrc += 4;
        sLen -= 4;
        vLen += 3;
    }
//    lpString[21]='\0';
    return vLen;
}

char *_remove_encryptWrap(char mystr[], char *encryptResult) {
    memset(encryptResult, 0, sizeof(encryptResult));
    fnBase64Encode(mystr, encryptResult, strlen(mystr));
    return encryptResult;
}

char *decryptWrap(char mystr[], char *decryptResult) {
//    memset(decryptResult, 0, sizeof(decryptResult));
    fnBase64Decode(decryptResult, mystr, strlen(mystr));
    return decryptResult;
}
/*
const char *decryptWrapConst(char mystr[], char *decryptResult) {

    char *str = decryptWrap(mystr, decryptResult);
//    LOGW("descrypt str:%s from %s",str,mystr);
    const char *str_=(const char *)str;

    jstring jstr = QSSQ_Constant::charstoJstring(JniHelper::getEnv(), str_);
    const char* fixStr= QSSQ_Constant::jstringTochars(JniHelper::getEnv(), jstr);
    return fixStr;
}*/

int main6() //主函数，测试函数
{

    char text[] = "abcd", encryptResult[2048];
    char decryptResult[2048];

    fnBase64Encode(text, encryptResult, strlen(text));
    printf("encryptResult %s\n", encryptResult);

    fnBase64Decode(decryptResult, encryptResult, strlen(encryptResult));
    printf("decryptResult: %s ", decryptResult);
    //------------------------------
    printf("\n---------------------\n");
    memset(encryptResult, 0, sizeof(encryptResult));
    memset(decryptResult, 0, sizeof(decryptResult));
    char temp[] = "dbcd";
    // char temp[] = "30820284308201eda00302010202044a447343300d06092a864886f70d01010b05003075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f7265301e170d3136313130373135333432395a170d3434303332343135333432395a3075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f726530819f300d06092a864886f70d010101050003818d0030818902818100850bdefebdf1040d15adf4a1370796ddd9816fbb414ddca4585a9ae186ef3aa17344849b8834616b7d57c0781a2edc4eb1390dba10db0c686f9860f5d6655161d6a7acea5da7a861ad52509bf5e28bdb7e521818e3209797c6a5e92ce56b3a6e94d0981dde29b82bde57dafb74a231a1e70cb1241ddff6ce40ab47e3f37b63bd0203010001a321301f301d0603551d0e04160414602a55d56209c16ebb013236938db93823d085d8300d06092a864886f70d01010b0500038181005fe284d71999b64d8d47c00be4eeecb8edf0844a03c2c409fd1d2cb7a3c0ba74bd59583eb4609a084765fea260fbd7c943363f125cb620c5dfcd7d656fab3b20798f91aba2a4f243c8156b9e76b70ae6ca41260f802721364ec5e19a498db285189de2e991ebeabbfd47677fdd155b3d3b4ea673a3f4b411fe561c2bbc203a09";

    fnBase64Encode(temp, encryptResult, strlen(temp));
    printf("encryptResult %s\n", encryptResult);

    fnBase64Decode(decryptResult, encryptResult, strlen(encryptResult));
    printf("\n\n");
    printf("decryptResult: %s \n ", decryptResult);

    char buffer[1024];
    char tempSign[] = "30820284308201eda00302010202044a447343300d06092a864886f70d01010b05003075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f7265301e170d3136313130373135333432395a170d3434303332343135333432395a3075310b3009060355040613023836310f300d06035504080c06e5b9bfe4b89c310f300d06035504070c06e6b7b1e59cb331153013060355040a0c0ce9bb91e7958ce7bd91e7bb9c31153013060355040b0c0ce68385e99a8fe4ba8be8bf81311630140603550403130d717373712e6b657973746f726530819f300d06092a864886f70d010101050003818d0030818902818100850bdefebdf1040d15adf4a1370796ddd9816fbb414ddca4585a9ae186ef3aa17344849b8834616b7d57c0781a2edc4eb1390dba10db0c686f9860f5d6655161d6a7acea5da7a861ad52509bf5e28bdb7e521818e3209797c6a5e92ce56b3a6e94d0981dde29b82bde57dafb74a231a1e70cb1241ddff6ce40ab47e3f37b63bd0203010001a321301f301d0603551d0e04160414602a55d56209c16ebb013236938db93823d085d8300d06092a864886f70d01010b0500038181005fe284d71999b64d8d47c00be4eeecb8edf0844a03c2c409fd1d2cb7a3c0ba74bd59583eb4609a084765fea260fbd7c943363f125cb620c5dfcd7d656fab3b20798f91aba2a4f243c8156b9e76b70ae6ca41260f802721364ec5e19a498db285189de2e991ebeabbfd47677fdd155b3d3b4ea673a3f4b411fe561c2bbc203a09";
    _remove_encryptWrap(tempSign, buffer);

    printf("encrypt sign result %s", buffer);
    char aa[] = "tempSign";
    _remove_encryptWrap("sign", buffer);
    // encryptWrap(aa, buffer);
    printf("encrypt sign result %s", buffer);


}