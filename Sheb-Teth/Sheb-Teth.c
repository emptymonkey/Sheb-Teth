#include <stdio.h>


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


int xor_decrypt_and_compare(char *query, char *cipher, char *key);

void check_flag_0(char *query);
void check_flag_1(char *query);
void check_flag_2(char *query);
void check_flag_3(char *query);

// This function is itself key material translated to assembly as though it were opcodes. May be interesting to use
// nested C functions (GNU extension) to place this at the beginning of the check_flag_3() function. Could do 
// interesting things to the static analysis tools.
// Then at the start of check_flag_3() actual codepath, do an if statement that the compiler wont optimize out that
// will never be true and call eldritch_function() if it is. e.g. if(*((&argc)++) == NULL)  which is in essence
// if argv[0] is empty, which it shouldn't ever be, but compiler won't know this. 
void eldritch_function();


int main(int argc, char **argv, char **envp){

	// start with a root user check. Warn the user if you aren't running as root then exit().

	printf("hello, world\n");

	return(0);
}
