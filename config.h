// No need to edit anymore; Will auto-select OS.
// (If not on Linux but on another *nix you might
// want to force LINUX however)

#define DOS


/*
Define plain_term for linux and similar, and dos_term for DOS and EMX.
*/
#ifdef linux
  #undef DOS
  #define LINUX
  #define PLAIN_TERM
  #include "linuxlib.h"
#endif  


/*
The first EOL should be used for DOS; most other OSs apparently
use \n
*/
#ifdef DOS
  #define EOL "\n\r"
  #define DOS_TERM
#else
  #define EOL "\n"
#endif


/*
Define MACRO if you want to use the macro patch in macro.cc.
*/
#define MACROS


