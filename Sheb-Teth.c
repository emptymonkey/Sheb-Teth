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

// GUID's are 36 chars with dashes. e.g. 4DEF757C-321A-4007-84FE-5AF5EA607BD4
#define BUFFER_LEN	36

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

int xor_decrypt_and_compare(char *query, char *cipher, char *key){


	return(0);
}


void eldritch_function(){
/*
	 $ head -c 36 </dev/urandom | ./udcli -att
	 0000000000000000 90               nop                     
	 0000000000000001 fb               sti                     
	 0000000000000002 1c7b             sbb $0x7b, %al          
	 0000000000000004 7503             jnz 0x9                 
	 0000000000000006 9ab5c2835401c0   call $0xc001, $0x5483c2b5
	 000000000000000d 44               inc %esp                
	 000000000000000e ca8a8f           lret  $0x8f8a           
	 0000000000000011 a36276cf23       mov %eax, 0x23cf7662    
	 0000000000000016 3b3e             cmp (%esi), %edi        
	 0000000000000018 0b39             or (%ecx), %edi         
	 000000000000001a 7778             ja 0x94                 
	 000000000000001c 2b4c1ae8         sub -0x18(%edx,%ebx), %ecx
	 0000000000000020 d7               xlatb                   
	 0000000000000021 97               xchg %eax, %edi         
	 0000000000000022 b7d1             mov $0xd1, %bh
 */


	// key -> \x90\xfb\x1c\x7b\x75\x03\x9a\xb5\xc2\x83\x54\x01\xc0\x44\xca\x8a\x8f\xa3\x62\x76\xcf\x23\x3b\x3e\x0b\x39\x77\x78\x2b\x4c\x1a\xe8\xd7\x97\xb7\xd1

	__asm__ __volitile__("
			nop
			sti
			sbb $0x7b, %%al
			jnz 0x9
			call $0xc001, $0x5483c2b5
			inc %%esp
			lret  $0x8f8a
			mov %%eax, 0x23cf7662
			cmp (%%esi), %%edi
			or (%%ecx), %%edi
			ja 0x94
			sub -0x18(%%edx,%%ebx), %%ecx
			xlatb
			xchg %%eax, %%edi
			mov $0xd1, %%bh
			");
}

