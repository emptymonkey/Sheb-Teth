
#include "common.h"

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


char *banner_break_0 = "#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.";
char *welcome_0 = "Welcome to the Sheb-Teth RE challenge.";
char *welcome_1 = "There are six flags. flag_0, flag_1, ..., flag_5.";
char *welcome_2 = "Each flag is in a uuid format. (e.g. \"c8da2132-382e-11e7-9862-507b9d8156b4\")";
char *welcome_3 = "When prompted, enter the flag you would like to test.";
char *welcome_4 = "This program will tell you which flag it matches, if any.";
char *welcome_5 = "Enjoy!";
char *banner_break_1 = ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#";

int main(){

	// Call sheb teth. Native of death. Invite and tremble.
	char *welcome = "uln sheb teth  n'ghaoth  sll'ha throd";

	// ...yet living, [It] sleeps/waits and then acts, we send [our] prayers [to thee], answer [us]!
	char *prompt = "mglw'nafh fhthagn-ngah cf'ayak 'vulgtmm vugtlag'n";

	// Worthless secret.
	char *bad_flag = "mnahn' r'luh";

	// Secret of the brotherhood. Invites eternity. Amen.
	char *good_flag = "r'luh chtenff  sll'ha syha'h  y'hah";

	// Darkn heretic mind. Earthing. <lift spell>
	char *goodbye_fail = "n'ghft hlirgh lloig  shuggoth  zhro";

	// Granted crossing wish. <finish spell>
	char *goodbye_success = "ch'goka gotha  uaaah";

	char *input_buffer;
	char *tmp_ptr = NULL;

	int success = 0;


//  XXX Left in place for testing eldritch_function() replacement.
//	write(1, ((char *) eldritch_function)+4, 36);
//	eldritch_function();


	if((input_buffer = calloc(BUFFER_LEN + 1, sizeof(char))) == NULL){
		error(-1, errno, "calloc(%d, %d)", BUFFER_LEN + 1, (int) sizeof(char));
	}

	printf("\n%s\n", welcome);
	printf("\n%s\n> ", prompt);
	if(fgets(input_buffer, BUFFER_LEN + 1, stdin) == NULL){
		return(-1);
	}
	printf("\n");

	tmp_ptr = strchr(input_buffer, '\n');
	if(tmp_ptr){
		*tmp_ptr = '\0';
	}

	if(check_flag_0(input_buffer)){
		printf("flag 0: Confirmed!\n");
		success = 1;
	}else if(check_flag_1(input_buffer)){
		printf("flag 1: Confirmed!\n");
		success = 1;
	}else if(check_flag_2(input_buffer)){
		printf("flag 2: Confirmed!\n");
		success = 1;
	}else if(check_flag_3(input_buffer)){
		printf("flag 3: Confirmed!\n");
		success = 1;
/*
*/
/*
	}else if(check_flag_4(input_buffer)){
		printf("flag 4: Confirmed!\n");
		success = 1;
	}else if(check_flag_5(input_buffer)){
		printf("flag 5: Confirmed!\n");
		success = 1;
*/
	}

	if(success){
		printf("\n%s\n", good_flag);
		printf("%s\n", goodbye_success);
	}else{
		printf("\n%s\n", bad_flag);
		printf("%s\n", goodbye_fail);
	}


	return(0);
}
