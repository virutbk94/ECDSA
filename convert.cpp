#include <NTL/ZZ.h>
#include <iostream>
#include<cstring>
#include "convert.h"

using namespace std;
using namespace NTL;
char convIntToChar(int a)
{
    switch(toupper(a)){
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'a';
        case 11: return 'b';
        case 12: return 'c';
        case 13: return 'd';
        case 14: return 'e';
        case 15: return 'f';
        default:
            return '0';
    }
}
int convCharToInt(char c)
{
    switch(toupper(c)){
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        default:
            return 0;
    }
}

void conv_hex_to_ZZ(ZZ& a,char* b)
{
    a = 0;
    for(int i=0; i<strlen(b); i++)
    {
        a = a*16 + convCharToInt(b[i]);
    }
}
void conv_ui_to_ZZ(ZZ& a,unsigned int b)
{
    conv(a,b);
}
void conv_ZZ_to_hex(char* a,ZZ b,int n)
{
    for(int i=1;i<=n;i++){
        a[n-i]=convIntToChar(b%16);
        b=b/16;
    }
    a[n]='\0';
}

void conv_ZZ_to_ui(unsigned int& a,ZZ b)
{
    conv(a,b);
}

