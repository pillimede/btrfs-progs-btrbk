/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#define _XOPEN_SOURCE 500
#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "kerncompat.h"
#include "ctree.h"
#include "disk-io.h"
#include "print-tree.h"
#include "transaction.h"
#include "list.h"
#include "version.h"
#include "utils.h"

static void print_usage(void)
{
	fprintf(stderr, "usage: btrfs-zero-log dev\n");
	fprintf(stderr, "%s\n", BTRFS_BUILD_VERSION);
	exit(1);
}

int main(int ac, char **av)
{
	struct btrfs_root *root;
	int ret;

	if (ac != 2)
		print_usage();

	radix_tree_init();

	if((ret = check_mounted(av[1])) < 0) {
		fprintf(stderr, "Could not check mount status: %s\n", strerror(-ret));
		return ret;
	} else if(ret) {
		fprintf(stderr, "%s is currently mounted. Aborting.\n", av[1]);
		return -EBUSY;
	}

	root = open_ctree(av[1], 0, 1);

	if (root == NULL)
		return 1;

	btrfs_set_super_log_root(&root->fs_info->super_copy, 0);
	btrfs_set_super_log_root_level(&root->fs_info->super_copy, 0);
	close_ctree(root);
	return ret;
}
