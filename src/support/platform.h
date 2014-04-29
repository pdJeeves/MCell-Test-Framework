#ifndef _platform_h_
#define _platform_h_

#ifndef _Windows
 #if defined(_WIN32)
  #define _Win32
  #define _Windows
 #elif defined(WIN32)
  #define _Win32
  #define _Windows
 #elif defined(__WIN32__)
  #define _Win32
  #define _Windows
 #elif defined(__Win32__)
  #define _Win32
  #define _Windows
 #elif defined(_WINDOWS)
  #define _Windows
 #elif defined(__INTEL__) && defined(__MWERKS__)
 // Metrowerks CodeWarrior doesn't build anything other than Win32 on INTEL, no DOS
  #define _Windows
  #define _Win32
 #endif
#else
 #if defined __Win32__ || defined _WIN32
  #ifndef _Win32
   #define _Win32
  #endif
 #endif
#endif

#ifdef _Windows
#include <windows.h>
#include <io.h>
typedef HANDLE p_data;
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

typedef pid_t p_data;
#define _pipe	pipe
#define _popen	_popen
#define _pclose	pclose
#define _dup	dup
#define _dup2	dup2
#define _close	close
#define _fileno	fileno

#endif

#ifndef _MSDOS
 #ifdef _Windows
  #define _MSDOS
 #elif defined(MSDOS)
  #define _MSDOS
 #elif defined(__MSDOS__)
  #define _MSDOS
 #endif
#endif

#endif
