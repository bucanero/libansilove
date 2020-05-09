/*
 * loadfile.c
 * libansilove 1.2.1
 * https://www.ansilove.org
 *
 * Copyright (c) 2011-2020 Stefan Vogt, Brian Cassidy, and Frederic Cambus
 * All rights reserved.
 *
 * libansilove is licensed under the BSD 2-Clause License.
 * See LICENSE file for details.
 */

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ansilove.h"

int
ansilove_loadfile(struct ansilove_ctx *ctx, char *input)
{
	int fd;
	struct stat st;

	if (ctx == NULL || input == NULL) {
		if (ctx)
			ctx->error = ANSILOVE_INVALID_PARAM;

		return -1;
	}

	fd = open(input, O_RDONLY);
	if (fd == -1) {
		ctx->error = ANSILOVE_FILE_READ_ERROR;
		return -1;
	}

	if (fstat(fd, &st) == -1) {
		ctx->error = ANSILOVE_FILE_READ_ERROR;
		close(fd);
		return -1;
	}

	ctx->maplen = ctx->length = st.st_size;

	/* mmap input file into memory */
	ctx->buffer = malloc(ctx->length);
	if (ctx->buffer == NULL) {
		ctx->error = ANSILOVE_MEMORY_ERROR;
		close(fd);
		return -1;
	}
	read(fd, ctx->buffer, ctx->length);

	close(fd);

	return 0;
}
