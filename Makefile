

# ARCH_X64 or ARCH_X86 change the calculation of function prolog length.
ARCH_DEF = ARCH_X64

KEY_SEED_4 = 3132303430333230343200
KEY_5 = 45c2fd0c3993705773660f7b67cf896994cd817514b5e30f55a7bb31c204d97f94f23633

CC = /usr/bin/cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

ECHO = /bin/echo
XXD = /usr/bin/xxd
TR = /usr/bin/tr
SED = /bin/sed
RM = /bin/rm
MV = /bin/mv

XORSCURA_DIR = ../xorscura
LIBS = -lxorscura

OBJS = flag_0.o flag_1.o flag_2.o flag_3.o flag_4.o flag_5.o eldritch.o

all: Sheb-Teth

Sheb-Teth: common.h Sheb-Teth.c $(OBJS) 
	$(CC) $(CFLAGS) -c -o Sheb-Teth.o Sheb-Teth.c
	$(CC) $(CFLAGS) -L$(XORSCURA_DIR) Sheb-Teth.o $(OBJS) -o Sheb-Teth $(LIBS)
	$(ECHO) -n $(KEY_SEED_4) | $(XXD) -p -r >>Sheb-Teth

flag_0.o: flag_0.c
	$(CC) $(CFLAGS) -g -c -o flag_0.o flag_0.c

flag_1.o: flag_1.c
	$(CC) $(CFLAGS) -g -I$(XORSCURA_DIR) -c -o flag_1.o flag_1.c 

flag_2.o: flag_2.c
	$(CC) $(CFLAGS) -g -I$(XORSCURA_DIR) -c -o flag_2.o flag_2.c

flag_3.o: flag_3.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_3.o flag_3.c

flag_4.o: flag_4.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_4.o flag_4.c

flag_5.o: flag_5.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -D$(ARCH_DEF) -c -o flag_5.o flag_5.c

eldritch.o: eldritch.c
	$(CC) $(CFLAGS) -c -o eldritch.o eldritch.c
	$(XXD) -p eldritch.o | $(TR) -d '\n' | $(SED) 's/\(90\)\{36\}/$(KEY_5)/' | $(XXD) -p -r >eldritch.tmp
	$(MV) eldritch.tmp eldritch.o

clean: 
	$(RM) -rf Sheb-Teth $(OBJS)
