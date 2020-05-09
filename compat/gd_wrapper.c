#include <stdlib.h>
#include <string.h>
#include "gd.h"

#define RGB_COLOR(R, G, B)			PACK_ARGB8888(0xFF, R, G, B)
#define MAX_COLORS_PALETTE			16

static uint32_t rgb_colors[MAX_COLORS_PALETTE];
static uint8_t color_idx = 0;

int gdImageColorAllocate (gdImagePtr im, int r, int g, int 	b)
{
	if (color_idx >= MAX_COLORS_PALETTE)
		return 0;

	rgb_colors[color_idx] = RGB_COLOR(r, g, b);
//	printf("i=%d cA = %08X\n", color_idx, rgb_colors[color_idx]);
	return (color_idx++);
}

uint8_t _pixelBytes(uint8_t type)
{
	switch(type)
	{
		case GD_TEX_FORMAT_RGB565:
		case GD_TEX_FORMAT_ARGB1555:
		case GD_TEX_FORMAT_ARGB4444:
			return 2;
		case GD_TEX_FORMAT_ARGB8888:
			return 4;
	}
	return 4;
}

void _initPalette()
{
	for (int i = 0; i < MAX_COLORS_PALETTE; i++)
		rgb_colors[i] = 0xFF000000;

	color_idx = 0;
}

gdImagePtr gdImageCreate (int sx, int sy)
{
//	printf("iC = %d x %d\n", sx, sy);
	gdImagePtr ptr = malloc(sizeof(gdImage_t));
	
	if (!ptr)
		return NULL;
		
	ptr->sx = sx;
	ptr->sy = sy;
	ptr->tex_type = GD_TEX_FORMAT_ARGB8888;
	ptr->data = malloc(sx * sy * _pixelBytes(ptr->tex_type));
	
	if (!ptr->data)
	{
		free(ptr);
		ptr = NULL;
	}

	// init the 16-color palette
	_initPalette();

	// init the image canvas with a black background (no alpha)
	gdImageFill(ptr, 0, 0, 0);
	
	return ptr;
}

gdImagePtr gdImageCreateTrueColor (int sx, int sy)
{
	gdImagePtr ptr = gdImageCreate(sx, sy);
	color_idx = 0xFF;

	return ptr;
}

void gdImageFill(gdImagePtr im, int x, int y, int nc)
{
	int i, j;
//	printf("iF = %08X\n", nc);
	for (i = x; i < im->sx; i++)
		for (j = y; j < im->sy; j++)
			gdImageSetPixel(im, i, j, nc);
}

void gdImageFilledRectangle (gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
	int i, j;
//	printf("fR = %08X\n", color);
	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++)
			gdImageSetPixel(im, i, j, color);
}

void gdImageSetPixel (gdImagePtr im, int x, int y, int 	color)
{
	// skip if pixel is out of the image
	if ((x >= im->sx) || (y >= im->sy))
		return;

	// handle a true-color pixel
	if (color_idx == 0xFF)
		color |= 0xFF000000;

	// handle a 16-color palette pixel
	else if ((color >= 0) && (color < MAX_COLORS_PALETTE))
		color = rgb_colors[color];

	// skip if color is out of the palette
	else
		return;

	memcpy(im->data + (x + im->sx*y) * _pixelBytes(im->tex_type), &color, _pixelBytes(im->tex_type));
}

void gdImageDestroy (gdImagePtr im)
{
//	free(im->data);
	free(im);
}

void gdFree (void *ptr)
{
	free(ptr);
}

void * gdImagePngPtr (gdImagePtr im, int * size, int *sx, int *sy)
{
	*sx = im->sx;
	*sy = im->sy;
	*size = im->sx * im->sy * _pixelBytes(im->tex_type);

	return im->data;
}

void gdImageColorTransparent (gdImagePtr im, int color)
{
	int i, j, *pixel;
	color |= 0xFF000000;
//	printf("cT = %08X\n", color);
	for (i = 0; i < im->sx; i++)
		for (j = 0; j < im->sy; j++)
		{
			pixel = (int*)(im->data + (i + im->sx*j) * _pixelBytes(im->tex_type));
			if (*pixel == color)
				*pixel &= 0x00FFFFFF;
		}
}

void gdImageCopyResized (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int dstW, int dstH, int srcW, int srcH)
{
	dst->sx = src->sx;
	dst->sy = src->sy;
	dst->data = src->data;
}

void gdImageCopyResampled (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int dstW, int dstH, int srcW, int srcH)
{
	dst->sx = src->sx;
	dst->sy = src->sy;
	dst->data = src->data;
}
