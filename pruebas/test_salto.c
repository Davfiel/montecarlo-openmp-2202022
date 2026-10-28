#include <stdio.h>
#include <stdint.h>
#define LCG_A 6364136223846793005ULL
#define LCG_C 1442695040888963407ULL
static uint64_t sig(uint64_t s){return LCG_A*s+LCG_C;}
static uint64_t saltar(uint64_t s,uint64_t k){uint64_t aa=1,ca=0,a=LCG_A,c=LCG_C;
 while(k){if(k&1){aa*=a;ca=ca*a+c;}c=(a+1)*c;a*=a;k>>=1;}return aa*s+ca;}
int main(void){uint64_t s=20261018ULL;int fallos=0;
 for(uint64_t k=0;k<=1000000;k++){ if(saltar(20261018ULL,k)!=s){fallos++;} s=sig(s);}
 printf("salto verificado para k=0..1e6, fallos=%d\n",fallos);return fallos;}
