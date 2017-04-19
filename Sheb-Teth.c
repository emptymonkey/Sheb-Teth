
#include <stdio.h>
#include <stdlib.h>


/**********************************************************************************************************************
 *	
 *	Sheb-Teth
 *	Devourer of Souls
 *	
 *	@emptymonkey
 *	2017-03-10
 *
 *
 *	Counter-forensic RE challenge for LayerOne 2017 CTF.
 *
 *	Design concepts:
 *	- In a normal run, the program will query the user for a flag and confirm if it is valid.
 *	- All flags will be GUID style strings. 
 *	- All flags will be stored in memory as binary blob ciphertexts.
 *	- All flag keys will be stored as binary blobs, though they may be hidden differently at different levels.
 *	- All flags will be decrypted with a simple xor decryption function that takes the cipher and key blobs.
 *	- Four flags at various levels of difficulty.
 *	- Challenges focus on two areas of counter-forensics:
 *		* Hiding key material. 
 *		* Frustrating runtime debuggers (generally through the use of ptrace()).
 *	- Code will have a blind-alley codepath by default. If debugged, the later flags will always return false as a 
 *		result of this blind alley.
 *	- Code will fork() a child which will then ptrace() the parent. The child determines the location of the valid key
 *		and ptrace() pushes it to the parent at an anticipated location. This diverts the codepath from the blind-alley.
 *	- One key will be hidden in .txt segment.
 *	
 *	Challenges include (in increased order of difficulty):
 *	- Recover flag from two blobs in the same stack frame. No anti-debugging.
 *	- Recover flag from two blobs in the same stack frame. Basic anti-debugging PTRACE_TRACEME check that needs to be
 *		circumvented. (Errors out with the message "Debugger sind verboten!".)
 *	- Recover flag. Key blob lives inside ELF section dead space. No additional anti-debugging at this point.
 *		e.g.	head -c 1024 /dev/urandom >>./hello_world
 *		Abuse STREAM buffering mechanics.
 *	- Recover flag by skipping blind-alley after ptrace() work done by child. Key is in the .txt segment. Fake key is 
 *		in the blind-alley stack frame.
 *
 *
 **********************************************************************************************************************/

// GUID's are 36 chars with dashes. e.g. 4DEF757C-321A-4007-84FE-5AF5EA607BD4
#define BUFFER_LEN	36

// Plain flag, no anti-debug.
void check_flag_0(char *query);

// Obscured flag, no anti-debug.
void check_flag_1(char *query);

// Obscured flag, anti-debug w/PTRACE_TRACEME.
void check_flag_2(char *query);

// Obscured flag, anti-debug w/breakpoint detection, false disassembly, and blind-alley redirection.
void check_flag_3(char *query);

// Obscured flag, anti-debug w/ptrace()d child process that constructs the flag.
// Child function for assembling the flag has many false breakpoints to frustrate debugger on the parent.
// Child assembles flag material that is stored in the elf binary's dead space.
void check_flag_4(char *query);

// Obscured flag, anti-debug w/default blind-alley runtime altered by child process ptrace()ing parent. 
// Child points parent to flag material stored in the .text section of the runtime process.
// Same child as in check_flag_4(). They simply switch roles.
void check_flag_5(char *query);


// This function is itself key material translated to assembly as though it were opcodes. May be interesting to use
// nested C functions (GNU extension) to place this at the beginning of the check_flag_5() function. Could do 
// interesting things to the static analysis tools.
// Then at the start of check_flag_5() actual codepath, do an if statement that the compiler wont optimize out that
// will never be true and call eldritch_function() if it is. e.g. if(*((&argc)++) == NULL)  which is in essence
// if argv[0] is empty, which it shouldn't ever be, but compiler won't know this. 
void eldritch_function();


int main(int argc, char **argv, char **envp){

	// start with a root user check. Warn the user if you aren't running as root then exit().

	write(1, ((char *) eldritch_function)+4, 36);

	return(0);
}


void eldritch_function(){


	/*

		Generate key:
		head -c 36 /dev/urandom | xxd -p -c 64

		key -> \x1d\x92\x9d\x5f\xaa\xf6\xc4\x83\xad\xed\x61\x9d\xf0\xb2\x17\xd4\x4d\x86\x1a\xfd\x30\xed\x72\x82\x54\xed\x9d\x9f\x3a\xaf\xfa\x01\xe0\xcc\x06\x94

		Then replace nops with key as part of Makefile. :)

		empty@monkey:~/code/Sheb-Teth$ xxd -c 64 -p Sheb-Teth | sed 's/\(90\)\{36\}/1d929d5faaf6c483aded619df0b217d44d861afd30ed728254ed9d9f3aaffa01e0cc0694/' | xxd -p -r >Sheb-Teth.mod
		empty@monkey:~/code/Sheb-Teth$ chmod 755 Sheb-Teth.mod 
		empty@monkey:~/code/Sheb-Teth$ ./Sheb-Teth.mod

	*/

	__asm__
		__volatile__
		(

		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 "nop \n\t"
		 ); 

}

