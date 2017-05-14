
#include "common.h"


int check_flag_0(char *query){

	char *flag_0_plaintext = "f36c454e-3831-11e7-a8b5-3fb45ccb41a5";

	if(!strncmp(flag_0_plaintext, query, BUFFER_LEN)){
		return(1);
	}

	return(0);
}
