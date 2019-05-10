#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

extern char *base64_decode(const char*,char **);
extern char* base64_decode_wrap(const char*,char*);
extern char *base64_encode(const char*,char **);