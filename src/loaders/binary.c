//
//  binary.c
//  AnsiLove/C
//
//  Copyright (c) 2011-2018 Stefan Vogt, Brian Cassidy, and Frederic Cambus.
//  All rights reserved.
//
//  This source code is licensed under the BSD 2-Clause License.
//  See the LICENSE file for details.
//

#include <stddef.h>
#include <stdint.h>
#include "../../include/ansilove.h"
#include "../config.h"
#include "../drawchar.h"
#include "../fonts.h"
#include "../output.h"
#include <gd.h>

int ansilove_binary(struct ansilove_ctx *ctx, struct ansilove_options *options)
{
	if (ctx == NULL || options == NULL) {
		if (ctx)
			ctx->error = ANSILOVE_INVALID_PARAM;

		return -1;
	}

	// some type declarations
	struct fontStruct fontData;

	// font selection
	alSelectFont(&fontData, options->font);

	// libgd image pointers
	gdImagePtr canvas;

	// Default to 160 columns if columns option wasn't set
	options->columns = options->columns ? options->columns : 160;

	// allocate buffer image memory
	canvas = gdImageCreate(options->columns * options->bits,
	    ((ctx->length / 2) / options->columns * fontData.height));

	if (!canvas) {
		ctx->error = ANSILOVE_GD_ERROR;
		return -1;
	}

	// allocate black color
	gdImageColorAllocate(canvas, 0, 0, 0);

	// allocate color palette
	uint32_t colors[16];

	for (int i = 0; i < 16; i++) {
		colors[i] = gdImageColorAllocate(canvas, binary_palette[i*3],
		    binary_palette[i*3+1],
		    binary_palette[i*3+2]);
	}

	// process binary
	uint32_t character, attribute, background, foreground;
	uint32_t loop = 0;
	int32_t column = 0, row = 0;

	while (loop < ctx->length) {
		if (column == options->columns) {
			column = 0;
			row++;
		}

		character = ctx->buffer[loop];
		
		if (ctx->length > loop) {
			attribute = ctx->buffer[loop+1];

			background = (attribute & 240) >> 4;
			foreground = (attribute & 15);

			if (background > 8 && !options->icecolors)
				background -= 8;

			drawchar(canvas, fontData.font_data, options->bits,
			    fontData.height, column, row, colors[background],
			    colors[foreground], character);

			column++;
			loop += 2;
		}
	}

	// create output image
	return output(ctx, options, canvas);
}
