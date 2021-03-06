
#include "common.h"
#include "libxorscura.h"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#ifdef ARCH_X64
# define FUNCTION_PROLOG_LEN	4
#elif ARCH_X86
# define FUNCTION_PROLOG_LEN	3
#endif

int check_flag_5(char *query){

	//	char *flag_5_plaintext = "828fe9e4-38da-11e7-80b0-1f8ba5f4d330";
	//	unsigned int flag_5_seed = 924028117;
	//	unsigned char flag_5_key[] = {0x45,0xc2,0xfd,0x0c,0x39,0x93,0x70,0x57,0x73,0x66,0x0f,0x7b,0x67,0xcf,0x89,0x69,0x94,0xcd,0x81,0x75,0x14,0xb5,0xe3,0x0f,0x55,0xa7,0xbb,0x31,0xc2,0x04,0xd9,0x7f,0x94,0xf2,0x36,0x33};

	// fake key
	unsigned char flag_5_key[] = {0xcd,0x2a,0x8b,0x3f,0x9d,0x8a,0x8d,0x3e,0x64,0x5a,0x83,0x4f,0x7c,0x50,0xb9,0x32,0xd2,0xfe,0x3b,0x1a,0x3c,0x98,0x6d,0x3b,0xb4,0xeb,0x60,0x72,0xf8,0xf5,0x46,0x35,0xa6,0xce,0x70,0x6c};
	unsigned char flag_5_ciphertext[] = {0x7d,0xf0,0xc5,0x6a,0x5c,0xaa,0x15,0x63,0x5e,0x55,0x37,0x1f,0x06,0xe2,0xb8,0x58,0xf1,0xfa,0xac,0x4d,0x24,0xd7,0xd3,0x22,0x64,0xc1,0x83,0x53,0xa3,0x31,0xbf,0x4b,0xf0,0xc1,0x05,0x03};

	int retval;
	int match = 0;
  struct xod *xor_data;

	int ppid, cpid;
	unsigned char *tmp_ptr __attribute__ ((aligned (__WORDSIZE)));


	tmp_ptr = flag_5_key;

	/*
		 The key is stored in the .txt area of memory as the eldritch_function.
		 The parent doesn't know this. The child does. The child will now connect to the
		 parent and place the pointer to the key in the right place in memory. 

		 If we are being debugged, the child will fail to connect and silently exit.
		 In this failure mode, the parent will move forward with the fake key, doing the wrong thing.
	 */

	ppid = getpid();
	cpid = fork();

	if(!cpid){
		ptrace(PTRACE_ATTACH, ppid, 0, 0);
		wait(NULL);
		ptrace(PTRACE_POKEDATA, ppid, &tmp_ptr, (char *) &eldritch_function + FUNCTION_PROLOG_LEN);
		ptrace(PTRACE_DETACH, ppid, 0, 0);
		exit(0);
	}

	wait(&cpid);

	if((xor_data = (struct xod *) calloc(1, sizeof(struct xod))) == NULL){
		error(-1, errno, "calloc(1, %d)", (int) sizeof(struct xod));
	}

	xor_data->buf_count = BUFFER_LEN;
	xor_data->key_buf = tmp_ptr;
	xor_data->ciphertext_buf = flag_5_ciphertext;
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
