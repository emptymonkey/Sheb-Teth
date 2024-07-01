
#include "common.h"
#include "libxorscura.h"

#include <sys/ptrace.h>

// A bitmask that will match \n. It will match other things too, but that is fine in our case.
// Not being a simple "if(cb == '\n')" is another obfuscation.
#define NL_MASK 0x55

// This value matches the 32 bit fnv1a hash of the string"TracerPid:\t0".
// That string matches a process that is not being debugged.
#define TRACER_PID_FNV1A_HASH 0x1c18b478

// The clue in this challenge is that these values are well-known for being related to the fnv1a hash algorithm.
#define FNV1A_OB 0x811C9DC5
#define FNV1A_PRIME 0x01000193

int check_flag_2(char *query){

	//	char *flag_2_plaintext = "9148387e-3835-11e7-954d-ef18177e9fe9";
	unsigned int flag_2_key_seed = 3426233066;
	unsigned char flag_2_ciphertext[] = {0x95,0xc5,0xb5,0x15,0xbb,0xd8,0x4b,0x55,0x35,0x03,0x2b,0x40,0x29,0xc3,0xde,0x11,0x91,0x78,0xb0,0x5c,0xe2,0xee,0xb8,0x7f,0xcd,0xa5,0x0b,0x66,0xd7,0xa3,0x73,0x48,0xd7,0xd7,0x74,0x66};

	int match = 0;
	struct xod *xor_data;
	int fd;

	// cb - character buffer
	unsigned char cb = 0;

	// ycthoth nglui hai - "The silence I see now"
	// We are using the fnv1a hash to search, or "see",  the relevant line and confirm its value is 0, or "silent". 
	unsigned int ycthoth_nglui_hai = FNV1A_OB;
	// This is our choice for the 32 bit fnv1a prime number, or "first" silence.
	// ynyth cthoth - "The first silence"
	unsigned int ynyth_cthoth = FNV1A_PRIME;

	/*
		Homebrew debugger check. Examine /proc/self/status looking for evidence the TracerPid variable is 0. If it isn't, 
		we are in a debugger. This check is performed in a way that is not straight forward.
	*/
	fd = open("/proc/self/status", O_RDONLY);

	// Calculate the 32 bit fnv1a hash of each line, excluding the newline.
	while(read(fd, &cb, 1)){

		// Masking cb with the NL_MASK will match more than \n, but that doesn't matter. The other characters aren't in the
		// only line we care about, so improperly handling them just creates other hashes that won't match. So, this mask
		// matches \n successfully when it needs to, allowing reasonable parsing of the file for our needs.
		if(!(cb & NL_MASK)){

			// Repurpose the fnv1a prime to mark a successful match by setting it to 0.
			// This "turns off" the hash, but without stopping the processing, so a successful match is slightly less noticible.
			if(ycthoth_nglui_hai == TRACER_PID_FNV1A_HASH)
				ynyth_cthoth = 0;

			ycthoth_nglui_hai = FNV1A_OB;
			continue;
		}
		ycthoth_nglui_hai ^= cb;
		ycthoth_nglui_hai *= ynyth_cthoth;

	}
	close(fd);

	// If the fnv1a prime is not 0, then we are being debugged.
	// Decend into madness.
	if(ynyth_cthoth){
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
