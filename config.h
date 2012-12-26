// No need to edit anymore; Will auto-select OS.
// (If not on Linux but on another *nix you might
// want to force LINUX however)

/*
Define plain_term for linux and similar.
*/
#ifdef linux
  #define LINUX
  #define PLAIN_TERM
  #include "linuxlib.h"
#endif  


#define EOL "\n"


/*
Define MACRO if you want to use the macro patch in macro.cc.
*/
#define MACROS


