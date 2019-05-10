//
// Created by Administrator on 2019/4/15.
//

#include "bytesencrypt.h"
void decodeStrFromBytes(int *bytes, char *result) {
//    memset(re)
    int pwd = bytes[0];
    int i = 1;
    // char result[3000];
    while (bytes[i] != -999) {
        int descryptCode = bytes[i] ^pwd;
        // printf(" char:%d ,current char %c \n", bytes[i], descryptCode);
        result[i - 1] = (unsigned char) descryptCode; //from 0 start
        i++;
    }
//    LOGW("decodeStrFromBytes %s",result);

}