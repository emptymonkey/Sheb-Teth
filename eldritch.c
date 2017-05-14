
void eldritch_function(){


  /*

     Generate key:
     head -c 36 /dev/urandom | xxd -p -c 64

     key -> \x1d\x92\x9d\x5f\xaa\xf6\xc4\x83\xad\xed\x61\x9d\xf0\xb2\x17\xd4\x4d\x86\x1a\xfd\x30\xed\x72\x82\x54\xed\x9d\x9f\x3a\xaf\xfa\x01\xe0\xcc\x06\x94

     Then replace nops with key as part of Makefile. :)

     empty@monkey:~/code/Sheb-Teth$ xxd -c 64 -p Sheb-Teth | sed 's/\(90\)\{36\}/1d929d5faaf6c483aded619df0b217d44d861afd30ed728254ed9d9f3aaffa01e0cc0694/' | xxd -p -r >Sheb-Teth.mod
     empty@monkey:~/code/Sheb-Teth$ chmod 755 Sheb-Teth.mod 
     empty@monkey:~/code/Sheb-Teth$ ./Sheb-Teth.mod

   */

  __asm__
    __volatile__
    (

     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     "nop \n\t"
     );

}
