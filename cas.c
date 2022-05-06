#include<iostream>
#ifndef __cmpswp_h
    #include <mih/cmpswp.h>
#endif

#define ATOMICADD ( var, val, rc ) {              \
    int aatemp1 = (var);                          \
    int aatemp2  = aatemp1 + val;                 \
    while( ! _CMPSWP( &aatemp1, &var, aatemp2 ) ) \
       aatemp2 = aatemp1 + val;                   \
    rc = aatemp2;                                 \
}