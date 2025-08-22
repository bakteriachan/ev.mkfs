#ifndef EXT2_DEFS_H
#define EXT2_DEFS_H

#define EXT2_SUPER_MAGIC 0xEF53

#define EXT2_VALID_FS 1
#define EXT2_ERROR_FS 2

#define EXT2_ERRORS_CONTINUE 1 /* Continue as is nothing happened */
#define EXT2_ERRORS_RO 2 /* Remount Read-Only */
#define EXT2_ERRORS_PANIC 3 /* Cause a kernel panic */

#define EXT2_OS_LINUX 0
#define EXT2_OS_HURD 1
#define EXT2_OS_MASIX 2
#define EXT2_OS_FREEBSD 3
#define EXT2_OS_LITES 4

#define EXT2_GOOD_OLD_REV 0
#define EXT2_DYNAMIC_REV 1

#define EXT2_DEF_RESUID 0 /* default user id for reserved blocks */
#define EXT2_DEF_RESGID 0

#define EXT2_GOOD_OLD_FIRST_INO 11 /* first inode index for Revision 0 */

/* s_feature_compat values */
#define EXT2_FEATURE_COMPAT_DIR_PREALLOC 	0x0001 /* Block pre-allocation for new directories */
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES 	0x0002
#define EXT2_FEATURE_COMPAT_HAS_JOURNAL 	0x0004 /* An ext3 journal exists */
#define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008 /* Extended inodes attributes are present */
#define EXT2_FEATURE_COMPAT_RESIZE_INO		0x0010 /* Non-standard inode size are used */
#define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020 /* Directory indexing (HTree) */

/* s_feature_incompat values */
#define EXT2_FEATURE_INCOMPAT_COMPRESSION 	0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYE		0x0002
#define EXT2_FEATURE_INCOMPAT_RECOVER		0x0004
#define EXT2_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008
#define EXT2_FEATURE_INCOMPAT_META_BG		0x0010


/* s_feature_ro_compat */
#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004

/* s_algo_bitmap */
#define EXT2_LZV1_ALG		0x0001
#define EXT2_LSRW3A_ALG		0x0002
#define EXT2_GZIP_ALG		0x0004
#define EXT2_BZIP2_ALGO		0x0008
#define EXT2_LZO_ALG		0x0010


struct ext2_superblock {
	uint32_t s_inodes_count;
	uint32_t s_blocks_count;
	uint32_t s_r_blocks_count;
	uint32_t s_free_blocks_count;
	uint32_t s_free_inodes_count;
	uint32_t s_first_data_block; /* Always 0 when block size larger than 1KiB, 1 if blocksize is 1KiB */
	uint32_t s_log_block_size;
	uint32_t s_log_frag_size;
	uint32_t s_blocks_per_group;
	uint32_t s_frags_per_group;
	uint32_t s_inodes_per_group;
	uint32_t s_mtime; /* unix time of the last time the filesystem was mounted */
	uint32_t s_wtime; /* unix time of the last write to the filesystem */

	uint16_t s_mnt_count; /* how many times the filesystem was
							 mounted since last fully verified */
	uint16_t s_max_mnt_count; /* maximum number of times the filesystem
								 may be mounted before a full check is performed */
	uint16_t s_magic; /* EXT2 filesystem identifier */
	uint16_t s_state; /* When filesystem is mounted this value is set to 
						 EXT2_ERROR_FS, when is succesfully unmounted is
						 set to EXT2_VALID_FS */
	uint16_t s_errors; /* this value indicates the driver what to do in
						  in case of errors */
	uint16_t s_minor_rev_lever;

	uint32_t s_lastcheck; /* unix time of the last check */
	uint32_t s_checkinterval; /* maximum unix time interval allowed between
								 file system checks */
	uint32_t s_creator_os;
	uint32_t s_rev_level;
	uint16_t s_def_resuid;
	uint16_t s_def_resgid;

	/* EXT2 Dynamic Revision */
	uint32_t s_first_ino;
	uint16_t s_inode_size;
	uint16_t s_block_group_nr;

	uint32_t s_feature_compat;
	uint32_t s_feature_incompat; /* the file system implementation should refuse to mount
									if feature is unsupported */
	uint32_t s_feature_ro_compat; /* read-only features. file system implementation should mount as
									 read-only if any feature is unsupported */


	uint8_t	s_uuid[16];
	char 	s_volume_name[16];

	char 	s_last_mounted[64];

	uint32_t s_algo_bitmap;

	uint8_t s_prealloc_blocks;
	uint8_t s_prealloc_dir_blocks;


	uint16_t s_journal_uuid;
	uint32_t s_journal_inum;

	uint32_t s_last_orphan;
	uint32_t s_hash_seed[4];

	uint8_t s_def_hash_version;

	uint32_t s_default_mount_options;
	uint32_t s_first_meta_bg;
};

struct ext2_block_group_descriptor {
};

#endif /* EXT2_DEFS_H */
