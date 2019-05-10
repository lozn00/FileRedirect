#include <stdlib.h>
#include <stdio.h>
#include "base64.c"
int main(){

char *str;
base64_decode("Y3VycmVudEFjdGl2aXR5VGhyZWFk",&str);
char str1[500];
base54_decode_wrap("Y3VycmVudEFjdGl2aXR5VGhyZWFk",str1);
printf("address str %s\nstr1:%s",str,str1);
free(str);
}