
#include "common.h"

/**********************************************************************************************************************
 *	
 *	Sheb-Teth
 *	Devourer of Souls
 *	
 *	@emptymonkey
 *	2017-03-10
 *
 *	Counter-forensic RE challenge for LayerOne 2017 CTF.
 *
 **********************************************************************************************************************/


char *welcome_00 = "#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.";
char *welcome_01 = ". @emptymonkey            https://github.com/emptymonkey            2017-03-10 #";
char *welcome_02 = "#                                                                              .";
char *welcome_03 = ". Welcome to the LayerOne 2017 CTF \"Sheb-Teth\" RE challenge.                 #";
char *welcome_04 = "# There are six flags. flag_0, flag_1, ..., flag_5.                            .";
char *welcome_05 = ". Each flag is in a uuid format. e.g. c8da2132-382e-11e7-9862-507b9d8156b4     #";
char *welcome_06 = "# When prompted, enter the flag you would like to test.                        .";
char *welcome_07 = ". This program will tell you if the flag you entered is a valid flag.          #";
char *welcome_08 = "# (You know how to speak R'lyehian, right?!)                                   .";
char *welcome_09 = ". Enjoy!                                                                       #";
char *welcome_10 = ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#";

int main(){

	// Call sheb teth. Native of death. Invite and tremble.
	char *welcome = "uln sheb teth  n'ghaoth  sll'ha throd";

	// ...yet living, [It] sleeps/waits and then acts, we send [our] prayers [to thee], answer [us]!
	char *prompt = "mglw'nafh fhthagn-ngah cf'ayak 'vulgtmm vugtlag'n";

	// Worthless secret.
	char *bad_flag = "mnahn' r'luh";

	// Secret of the brotherhood. Invites eternity. Amen.
	char *good_flag = "r'luh chtenff  sll'ha syha'h  y'hah";

	// Dark heretic mind. Earthing. <lift spell>
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

	}else if(check_flag_4(input_buffer)){
		printf("flag 4: Confirmed!\n");
		success = 1;

/*
*/

/*
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
