hw9: word.c word.h hash.c hash.h main.c
	clang -Wall -o hash_table word.c hash.c main.c -g
testhw9: word.c word.h hash.c hash.h testmain.c
	clang -Wall -o testhw9 word.c hash.c testmain.c -g
