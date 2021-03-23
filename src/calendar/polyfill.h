#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/*
 *
 * gcc gets warning: 
 *  implicit declaration of function ‘strdup’; did you mean ‘strcmp’? [-Wimplicit-function-declaration]
 *  and then gets a runtime segfault, presumeably strdup is not in the gnu library. 
 *  So why does it compile? Don't know, I generally use clang.
 *
 * @see https://stackoverflow.com/questions/19641460/using-strdup-in-c11/19641749
 * @see https://stackoverflow.com/questions/5573775/strdup-error-on-g-with-c0x
 *
 * rather than continue a debate on which one is correct, here is a polyfill:
 */

#ifndef strdup
char *strdup(const char *str);
#endif 
