//base64.c
#include <strings.h>
#include "base64.h"

static char base64_table[255];

void base64_tableinit()

{

    int i, j;

    bzero(base64_table, 255);

    for (j = 0, i = 'A'; i <= 'Z'; i++) /*填base64编码表*/

        base64_table[i] = j++;

    for (i = 'a'; i <= 'z'; i++)

        base64_table[i] = j++;

    for (i = '0'; i <= '9'; i++)

        base64_table[i] = j++;

    base64_table['+'] = j++;

    base64_table['/'] = j++;

    base64_table['='] = j;
}

char *base64_decode(const char *cptr, char **rptr)

{

    char *res;

    int clen, len;

    static int init = 0;

    if (cptr == NULL)

        return NULL;

    len = strlen(cptr);

    if (len % 4) /*编了码的字符绝对是4的倍数*/

        return NULL;

    if (!init)

    {

        init = 1;

        base64_tableinit();
    }

    clen = len / 4;

    if ((res =(char*) malloc(len - clen)) == NULL)

        return NULL;

    for (*rptr = res; clen--;)

    {

        *res = base64_table[*cptr++] << 2 & 0xfc; /*cptr后六位移动到最高位*/

        *res++ |= base64_table[*cptr] >> 4; /*跟着下个字符低两位给res低两位*/

        *res = base64_table[*cptr++] << 4 & 0xf0; /*填充res高四位其它清0*/

        *res++ |= base64_table[*cptr] >> 2 & 0x0f; /*字符前六位移到低六位取低四位*/

        *res = base64_table[*cptr++] << 6;

        if (*cptr != '=') /*=号乎略*/

            *res++ |= base64_table[*cptr++];
    }

    return *rptr;
}

char* base64_decode_wrap(const char *str,char* result){
    char* resultFrom;
  base64_decode(str,&resultFrom);
  strcpy(result,resultFrom);
//  LOGW("decrpt %s str -> %s memory %s",str,result,result);
  free(resultFrom);

  return result;
}
char *base64_encode(const char *cptr, char **rptr)

{

    char *res;

    int i, clen, len;

    len = strlen(cptr);

    clen = len / 3;

    if (cptr == NULL || (res =(char*) malloc(clen + 3 * 2 + len)) == NULL)

        return NULL;

    for (*rptr = res; clen--;)

    {

        *res++ = *cptr >> 2 & 0x3f; /*取ptr高6位放入res低6位*/

        *res = *cptr++ << 4 & 0x30; /*移动ptr最低2位到高6位然后清0其 它位*/

        *res++ |= *cptr >> 4; /*取ptr高4位给res低4位*/

        *res = (*cptr++ & 0x0f) << 2; /*取ptr低4位移动到高6位*/

        *res++ |= *cptr >> 6; /*取ptr高2位给res低2位*/

        *res++ = *cptr++ & 0x3f;
    }

    if (i = len % 3) /*处理多余字符只有两种情况多一个或者两个字符*/

    {

        if (i == 1) /*根据base64编码补=号*/

        {

            *res++ = *cptr >> 2 & 0x3f;

            *res++ = *cptr << 4 & 0x30;

            *res++ = '=';

            *res++ = '=';
        }

        else

        {

            *res++ = *cptr >> 2 & 0x3f;

            *res = *cptr++ << 4 & 0x30;

            *res++ |= *cptr >> 4;

            *res++ = (*cptr & 0x0f) << 2;

            *res++ = '=';
        }
    }

    *res = '='; /*保证最后结位为=结束原因是因为base64里有为0的编码*/

    for (res = *rptr; *res != '='; res++)

        *res = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="[*res];

    rptr[0][strlen(*rptr) - 1] = '\0'; /*去掉最后一个=号*/

    return *rptr;
}