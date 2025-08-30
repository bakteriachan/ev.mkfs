#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>
#include <unistd.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <uuid/uuid.h>

#include "ext2_defs.h"


int write_uint16_le(int fd, uint16_t value, void *buff) {
	*(uint16_t*)buff = htole16(value); 

	return write(fd, buff, sizeof(uint16_t));
}

int write_uint32_le(int fd, uint32_t value, void *buff) {
	*(uint32_t*)buff = htole32(value);

	return write(fd, buff, sizeof(uint32_t));
}

uint32_t max(uint32_t a, uint32_t b) {
	if(a > b) return a;
	return b;
}

uint32_t superblock_groups_count(struct ext2_superblock* sb) {
	return sb->s_blocks_count / sb->s_blocks_per_group;
}

void write_superblock(int fd, struct ext2_superblock *buff) {
	off_t file_size = lseek(fd, 0, SEEK_END);
	const uint32_t blocksize = 4096;

	struct ext2_superblock *sb = malloc(sizeof(struct ext2_superblock));
	sb->s_blocks_count = file_size / (long)blocksize;
	
	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_magic = EXT2_SUPER_MAGIC;
	sb->s_log_block_size = 2; // Block size = 4096
	sb->s_log_frag_size = 2;
	sb->s_first_data_block = blocksize > 1024 ? 0 : 1;
	
	sb->s_blocks_per_group = blocksize * 8;
	sb->s_frags_per_group = sb->s_blocks_per_group;

	uint32_t groups_count = superblock_groups_count(sb);
	sb->s_inode_size = 128;
	sb->s_inodes_per_group = blocksize * 2;
	sb->s_inodes_count = sb->s_inodes_per_group * groups_count;

	sb->s_free_blocks_count = 260070;
	sb->s_free_inodes_count = sb->s_inodes_count;

	sb->s_max_mnt_count = 25;

	sb->s_state = EXT2_VALID_FS;
	sb->s_errors = EXT2_ERRORS_CONTINUE;

	sb->s_minor_rev_lever = EXT2_DYNAMIC_REV;

	sb->s_def_resuid = EXT2_DEF_RESUID;
	sb->s_def_resgid = EXT2_DEF_RESGID;

	sb->s_first_ino = 11;

	sb->s_block_group_nr = 0;

	sb->s_feature_compat = 0;
	sb->s_feature_incompat = 0;
	sb->s_feature_ro_compat = 0;

	uuid_generate(sb->s_uuid);



	lseek(fd, 1024, SEEK_SET);

	write(fd, sb, sizeof(struct ext2_superblock));
	memcpy(buff, sb, sizeof(struct ext2_superblock));
}

void backup_superblock(int fd, struct ext2_superblock *sb, struct ext2_block_group_descriptor* group_descriptors) {
	uint32_t groups_count = superblock_groups_count(sb);
	for(int i = 1; i < groups_count; i++) {
		if(i == 1 || i%3 == 0 || i%5 == 0 || i%7 == 0) {
			sb->s_block_group_nr = (1024 << sb->s_log_block_size) * i;
			size_t superblock_position = sb->s_blocks_per_group * (1024 << sb->s_log_block_size) * i;
			lseek(fd, superblock_position, SEEK_SET);
			write(fd, sb, sizeof(struct ext2_superblock));

			lseek(fd, superblock_position + (1024 << sb->s_log_block_size), SEEK_SET);
			for(int i = 0; i < superblock_groups_count(sb); i++) {
				write(fd, &group_descriptors[i], sizeof(struct ext2_block_group_descriptor));
			}
			write(fd, group_descriptors, sizeof(*group_descriptors) * sizeof(struct ext2_block_group_descriptor));
		}
	}
}

struct ext2_block_group_descriptor* build_block_group_table(int fd, struct ext2_superblock* sb) {
	uint32_t filesize = sb->s_blocks_count * (1024 << sb->s_log_block_size);
	uint32_t groups_count = superblock_groups_count(sb);
	size_t bg_table_size = sizeof(struct ext2_block_group_descriptor) * groups_count;
	size_t inode_table_size = sb->s_inode_size * sb->s_inodes_per_group;
	struct ext2_block_group_descriptor *group_descriptors = (struct ext2_block_group_descriptor *)malloc(sizeof(struct ext2_block_group_descriptor) * groups_count);

	struct ext2_block_group_descriptor *bg = malloc(sizeof(struct ext2_block_group_descriptor));

	uint32_t bg_table_blockid = 2;
	uint32_t bg_tablesize_inblocks = max(bg_table_size / (1024 << sb->s_log_block_size), 1);
	uint32_t inode_tablesize_inblocks = max(inode_table_size / (1024 << sb->s_log_block_size), 1);

	bg->bg_block_bitmap = bg_table_blockid + bg_tablesize_inblocks + 1;
	bg->bg_inode_bitmap = bg->bg_block_bitmap + 1;
	bg->bg_inode_table = bg->bg_inode_bitmap + 1;
	
	bg->bg_free_blocks_count = sb->s_blocks_per_group - 3 /* superblock + block_bitmap + inode_bitmap */ - inode_tablesize_inblocks  - bg_tablesize_inblocks;
	
	bg->bg_free_inodes_count = sb->s_inodes_per_group;
	bg->bg_used_dirs_count = 0;

	group_descriptors[0] = *bg;

	for(int i = 1; i < groups_count; ++i) {
		bg->bg_block_bitmap = (i * sb->s_blocks_per_group) + (i==1||i%3==0||1%5==0|i%7==0) * (bg_tablesize_inblocks + 1);
		bg->bg_inode_bitmap = bg->bg_block_bitmap + 1;
		bg->bg_inode_table = bg->bg_inode_bitmap + 1;
		bg->bg_free_blocks_count = sb->s_blocks_per_group - (i==1||i%3==0||1%5==0|i%7==0) * (bg_tablesize_inblocks + 1) - 2 - inode_tablesize_inblocks;

		group_descriptors[i] = *bg;

		fprintf(stdout, "%d\n", (1024<<sb->s_log_block_size) * (bg->bg_block_bitmap));
		fflush(stdout);
		char zerobuffer[4096];
		memset(zerobuffer, 0, sizeof(zerobuffer));
		lseek(fd, (1024<<sb->s_log_block_size) * (bg->bg_block_bitmap), SEEK_SET);
		write(fd, zerobuffer, 4096);
	}

	fprintf(stdout, "%d\n", (1024<<sb->s_log_block_size) * (bg_table_blockid - 1));
	fflush(stdout);
	lseek(fd, (1024<<sb->s_log_block_size) * (bg_table_blockid - 1), SEEK_SET);
	for(int i = 0; i < groups_count; i++) {
		write(fd, &group_descriptors[i], sizeof(struct ext2_block_group_descriptor));
	}

	return group_descriptors;
}

void create_ext2_filesystem(int fd) {
	struct ext2_superblock *superblock = malloc(sizeof(struct ext2_superblock));

	write_superblock(fd, superblock);
	struct ext2_block_group_descriptor *group_descriptors = build_block_group_table(fd, superblock);
	backup_superblock(fd, superblock, group_descriptors);

	free(superblock);
}

#endif
