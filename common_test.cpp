//  see if we can link to common_funcs.cpp without needing any extra libraries.
#include <windows.h>
#include <stdio.h>

#include "common.h"

int main(void)
{
   printf("current time is %s\n", get_dtimes_str(NULL));
   return 0 ;
}