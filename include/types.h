#ifndef _types_h_
#define _types_h_
#include <string>

#if __cplusplus > 201103L
#define MOVE &&
#else
#define MOVE 
#endif



typedef unsigned char          uchar;
typedef unsigned short         ushort;
typedef unsigned int           uint;
typedef unsigned long long int ullong;
typedef signed   long long int  llong;

#if __cplusplus > 201103L
typedef char32_t point;
#else
typedef int      point;
#endif

typedef std::basic_string<point> utf_string;



#endif

