ev.mkfs: main.c thirdparty/flag.h fs/ext2/ext2.h fs/ext2/ext2_defs.h
	cc -g -o ev.mkfs -Ithirdparty -Ifs -Ifs/ext2 main.c -luuid

DEBUG:
	cc -g -DDEBUG -o ev.mkfs -Ithirdparty -Ifs -Ifs/ext2 main.c -luuid