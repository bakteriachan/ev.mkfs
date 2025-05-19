#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>
#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ext2_defs.h"


int write_uint16_le(int fd, uint16_t value, void *buff) {
	*(uint16_t*)buff = htole16(value); 

	return write(fd, buff, sizeof(uint16_t));
}

int write_uint32_le(int fd, uint32_t value, void *buff) {
	*(uint32_t*)buff = htole32(value);

	return write(fd, buff, sizeof(uint32_t));
}

void write_superblock(int fd, struct ext2_superblock *buff) {
	void *write_buff = malloc(64);
	uint32_t blocksize = 1024;

	struct ext2_superblock *sb = malloc(sizeof(struct ext2_superblock));
	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_magic = EXT2_SUPER_MAGIC;
	sb->s_log_block_size = 2; // Block size = 4096
	sb->s_first_data_block = blocksize > 1024 ? 0 : 1;

	uint32_t filesystem_size = lseek(fd, 0, SEEK_END);
	sb->s_blocks_count = filesystem_size / blocksize;


	sb->s_inode_size = 256;

	lseek(fd, 1024, SEEK_SET);
	write_uint32_le(fd, sb->s_inodes_count, write_buff);

	write_uint32_le(fd, sb->s_blocks_count, write_buff);


	memcpy(buff, sb, sizeof(sb));
	free(write_buff);
}

void create_ext2_filesystem(int fd) {
	struct ext2_superblock *superblock = malloc(sizeof(struct ext2_superblock));

	write_superblock(fd, superblock);


	free(superblock);
}

#endif
