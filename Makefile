
KEY = 1d929d5faaf6c483aded619df0b217d44d861afd30ed728254ed9d9f3aaffa01e0cc0694

CC = /usr/bin/cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g

XORSCURA_DIR = ../xorscura

XXD = /usr/bin/xxd
TR = /usr/bin/tr
SED = /bin/sed
CHMOD = /bin/chmod
RM = /bin/rm
MV = /bin/mv

OBJS = flag_0.o flag_1.o flag_2.o flag_3.o flag_4.o flag_5.o eldritch.o
LIBS = -lxorscura

all: Sheb-Teth

Sheb-Teth: common.h Sheb-Teth.c $(OBJS) 
	$(CC) $(CFLAGS) -c -o Sheb-Teth.o Sheb-Teth.c
	$(CC) $(CFLAGS) -L$(XORSCURA_DIR) Sheb-Teth.o $(OBJS) -o Sheb-Teth $(LIBS)

flag_0.o: flag_0.c
	$(CC) $(CFLAGS) -c -o flag_0.o flag_0.c

flag_1.o: flag_1.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_1.o flag_1.c 

flag_2.o: flag_2.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_2.o flag_2.c

flag_3.o: flag_3.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_3.o flag_3.c

flag_4.o: flag_4.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_4.o flag_4.c

flag_5.o: flag_5.c
	$(CC) $(CFLAGS) -I$(XORSCURA_DIR) -c -o flag_5.o flag_5.c

eldritch.o: eldritch.c
	$(CC) $(CFLAGS) -c -o eldritch.o eldritch.c
	$(XXD) -p eldritch.o | $(TR) -d '\n' | $(SED) 's/\(90\)\{36\}/$(KEY)/' | $(XXD) -p -r >eldritch.tmp
	$(MV) eldritch.tmp eldritch.o

clean: 
	$(RM) -rf Sheb-Teth $(OBJS)
