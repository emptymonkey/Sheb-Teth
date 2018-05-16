
#include "common.h"
#include "libxorscura.h"

#include <sys/ptrace.h>


int check_flag_2(char *query){

	//	char *flag_2_plaintext = "9148387e-3835-11e7-954d-ef18177e9fe9";
	unsigned int flag_2_key_seed = 3426233066;
	unsigned char flag_2_ciphertext[] = {0x95,0xc5,0xb5,0x15,0xbb,0xd8,0x4b,0x55,0x35,0x03,0x2b,0x40,0x29,0xc3,0xde,0x11,0x91,0x78,0xb0,0x5c,0xe2,0xee,0xb8,0x7f,0xcd,0xa5,0x0b,0x66,0xd7,0xa3,0x73,0x48,0xd7,0xd7,0x74,0x66};

	int match = 0;
	struct xod *xor_data;
	int fd;
	int flag;

	// kadishtu ylloig  ->  understand my own mind
	char kadishtu_ylloig;
	
	/*
		 Homebrew debugger check. Examine /proc/self/status for the TracerPid variable. If non-zero, we are in a debugger.
		 No error checking here. If anything goes wrong... decend into madness.

		 We will perform this check in a way that is not straight forward.
	*/

	fd = open("/proc/self/status", O_RDONLY);

	// Six newlines til the TracerPid line.
	// XXX BUG
	// As correctly indentified by @iximeow, this is an off-by-one error.
	// The TracerPid line is on the seventh line. This is 6 newlines from the start.
	// I am leaving it in the original form that appeared in the CTF and only updating these comments.
	/*
	  I must have put a decimal point in the wrong place or something. 
	  Shit! 
	  I always do that. 
	  I always mess up some mundane detail.
	    - Michael Bolton, Office Space
	*/
	// Turns out the issue is that the TracerPid line isn't set. It's line 5 on some systems, 6 on others, 7 on still others...
	// Best way to do this is actually watch for "TracerPid" instead of newlines.
	flag = 7;
	while(flag){
		read(fd, &kadishtu_ylloig, 1);
		if(kadishtu_ylloig == '\n'){
			flag--;
		}
	}

	// Find the tab break.
	flag = 1;
	while(flag){
		read(fd, &kadishtu_ylloig, 1);
		if(kadishtu_ylloig == '\t'){
			flag--;
		}
	}

	// Next character is the leading digit. This will only be nonzero if we are in a debugger.
	// There are more chars to this digit, but the display is not zero left padded, so there won't
	// ever be a leading zero unless the entire number itself is zero.
	read(fd, &kadishtu_ylloig, 1);
	close(fd);

	// We are being debugged. Decend into madness.
	if(atoi(&kadishtu_ylloig)){
		eldritch_function();
	}


	if((xor_data = (struct xod *) calloc(1, sizeof(struct xod))) == NULL){
		error(-1, errno, "calloc(1, %d)", (int) sizeof(struct xod));
	}

	xor_data->buf_count = BUFFER_LEN;
	xor_data->seed = flag_2_key_seed;
	xor_data->ciphertext_buf = flag_2_ciphertext;

	// Doing this as a decrypt + strcmp so the string ends up in memory for the analyst.
	// In the future we will use xorscura_compare() directly and avoid this.
	if(xorscura_decrypt(xor_data) == -1){
		error(-1, errno, "xorscura_compare(%lx)", (unsigned long) xor_data);
	}

	if(!strncmp(query, (char *) xor_data->plaintext_buf, xor_data->buf_count)){
		match = 1;
	}

	xorscura_free_xod(xor_data);
	free(xor_data);

	return(match);
}
