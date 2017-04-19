
KEY = 1d929d5faaf6c483aded619df0b217d44d861afd30ed728254ed9d9f3aaffa01e0cc0694

CC = /usr/bin/cc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g

XXD = /usr/bin/xxd
SED = /bin/sed
CHMOD = /bin/chmod
RM = /bin/rm


all: Sheb-Teth

Sheb-Teth: Sheb-Teth.c
	$(CC) $(CFLAGS) -o Sheb-Teth.tmp Sheb-Teth.c
	$(XXD) -c 64 -p Sheb-Teth.tmp | $(SED) 's/\(90\)\{36\}/$(KEY)/' | $(XXD) -p -r >Sheb-Teth
	$(CHMOD) 700 Sheb-Teth
	$(RM) Sheb-Teth.tmp


clean: 
	$(RM) Sheb-Teth
