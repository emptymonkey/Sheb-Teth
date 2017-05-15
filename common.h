
#define _POSIX_C_SOURCE	199309L
#define _XOPEN_SOURCE	500

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// uuid's are 36 chars with dashes. e.g. c8da2132-382e-11e7-9862-507b9d8156b4
#define BUFFER_LEN  36

int main();

// Plain flag, no anti-debug.
int check_flag_0(char *query);

// Obscured flag, no anti-debug.
int check_flag_1(char *query);

// Obscured flag, anti-debug w/check of /proc/self/status TracerPid value.
int check_flag_2(char *query);

// Obscured flag, anti-debug w/fake breakpoints + SIGTERM handler generation of key material.
int check_flag_3(char *query);

// Obscured flag, anti-debug w/ptrace()d child process that constructs the flag.
// Child assembles flag material that is stored in the elf binary's dead space.
int check_flag_4(char *query);

// Obscured flag, anti-debug w/default blind-alley runtime altered by child process ptrace()ing parent. 
// Child points parent to flag material stored in the .text section of the runtime process.
int check_flag_5(char *query);

// This function is itself key material translated to assembly as though it were opcodes. May be interesting to use
// nested C functions (GNU extension) to place this at the beginning of the check_flag_5() function. Could do 
// interesting things to the static analysis tools.
// Then at the start of check_flag_5() actual codepath, do an if statement that the compiler wont optimize out that
// will never be true and call eldritch_function() if it is. e.g. if(*((&argc)++) == NULL)  which is in essence
// if argv[0] is empty, which it shouldn't ever be, but compiler won't know this. 
void eldritch_function();

