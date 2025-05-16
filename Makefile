ev.mkfs: main.c thirdparty/flag.h
	cc -o ev.mkfs -Ithirdparty main.c
