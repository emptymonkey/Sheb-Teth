
#include "common.h"
#include "libxorscura.h"

#include <signal.h>

unsigned int elder_sign = 0;

void trap(int dark_sign){
	elder_sign += 379408146 + dark_sign;
}

int check_flag_3(char *query){

	//	char *flag_3_plaintext = "c4e0b7c8-3836-11e7-b5a1-8b9900be9d58";
	//	unsigned int flag_3_seed = 2276448906;
	unsigned char flag_3_ciphertext[] = {0x0e,0x45,0x9c,0x33,0x98,0xaf,0x49,0x28,0x1c,0x03,0x03,0x62,0xee,0xb7,0x87,0x20,0x90,0x41,0x8e,0x44,0xe1,0xed,0x4e,0x08,0x48,0xed,0xe6,0x1a,0x28,0x48,0xb6,0x37,0xac,0x33,0x95,0x07};

	int i;
	int retval;
	int match = 0;
	struct xod *xor_data;


	/*
		 And now for some abstraction of the key + homebrew debug checking...

		 The elder_sign variable will need to be 2276448906 for this to work.
			2276448906 = 6 * (379408146 + 5)
			5 = dark_sign (SIGTERM)

			We set up six calls to the debugger. If no debugger is there, we will
			receive SIGTRAP. The handler for SIGTRAP will increment the elder_sign.

			If there is a debugger, you won't get SIGTRAP. 
	 */
	for(i = 0; i < 6; i++){
		signal(SIGTRAP, trap);
		__asm__(
				"int3 \n\t"
				);
	}

	if((xor_data = (struct xod *) calloc(1, sizeof(struct xod))) == NULL){
		error(-1, errno, "calloc(1, %d)", (int) sizeof(struct xod));
	}

	xor_data->buf_count = BUFFER_LEN;
	xor_data->seed = elder_sign;
	xor_data->ciphertext_buf = flag_3_ciphertext;
	xor_data->plaintext_buf = (unsigned char *) query;

	if((retval = xorscura_compare(xor_data)) == -1){
		error(-1, errno, "xorscura_compare(%lx)", (unsigned long) xor_data);
	}

	if(!retval){
		match = 1;
	}

	xorscura_free_xod(xor_data);
	free(xor_data);

	return(match);
}
