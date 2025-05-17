ev.mkfs: main.c thirdparty/flag.h fs/ext2.h
	cc -o ev.mkfs -Ithirdparty -Ifs main.c
