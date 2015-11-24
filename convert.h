#include<NTL/ZZ.h>

using namespace NTL;

int convCharToInt(char c);
char convIntToChar(int a);
void conv_hex_to_ZZ(ZZ& a,char* b);
void conv_ui_to_ZZ(ZZ& a,unsigned int b);
void conv_ZZ_to_hex(char* a,ZZ b,int n);
void conv_ZZ_to_ui(unsigned int& a,ZZ b);
