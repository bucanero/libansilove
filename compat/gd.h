#ifndef GD_WRAPPER_H
#define GD_WRAPPER_H

#define PACK_RGB565(r,g,b) \
    ((((unsigned short)r & 0xf8) << 8) | (((unsigned short) g & 0xfc) << 3) | ((unsigned short) b >> 3))

#define _PACK4(v) ((v * 0xF) / 0xFF)
#define PACK_ARGB4444(a,r,g,b) (_PACK4(a) << 12) | (_PACK4(r) << 8) | (_PACK4(g) << 4) | (_PACK4(b))
#define PACK_ARGB8888(a,r,g,b) ( ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF) )
#define PACK_ARGB1555(a,r,g,b) \
    (((unsigned short)(a > 0) << 15) | (((unsigned short) r >> 3) << 10) | (((unsigned short)g >> 3) << 5) | ((unsigned short)b >> 3))


enum 
{
    GD_TEX_FORMAT_RGB565,
    GD_TEX_FORMAT_ARGB1555,
    GD_TEX_FORMAT_ARGB4444,
    GD_TEX_FORMAT_ARGB8888,
};

typedef struct
{
	int sx;
	int sy;
	unsigned char tex_type;
	unsigned char* data;
} gdImage_t;

typedef gdImage_t* gdImagePtr;

int gdImageColorAllocate (	gdImagePtr 	im, int 	r, int 	g, int 	b	);
gdImagePtr gdImageCreate (	int 	sx, int 	sy	);
gdImagePtr gdImageCreateTrueColor (	int 	sx, int 	sy	);
void gdImageFill(	gdImagePtr 	im, int 	x, int 	y, int 	nc	);
void gdImageFilledRectangle (	gdImagePtr 	im, int 	x1, int 	y1, int 	x2, int 	y2, int 	color	);
void gdImageSetPixel (	gdImagePtr 	im, int 	x, int 	y, int 	color	);
void gdImageDestroy (	gdImagePtr 	im	);
void * gdImagePngPtr (	gdImagePtr 		im, int 	*	size	, int * sx, int * sy);
void gdImageColorTransparent (	gdImagePtr 	im, int 	color	);
void gdFree (	void 	*	ptr	);

void gdImageCopyResized (gdImagePtr 	dst, gdImagePtr 	src, int 	dstX, int 	dstY, int 	srcX, int 	srcY, int 	dstW, int 	dstH, int 	srcW, int 	srcH);
void gdImageCopyResampled (gdImagePtr 	dst, gdImagePtr 	src, int 	dstX, int 	dstY, int 	srcX, int 	srcY, int 	dstW, int 	dstH, int 	srcW, int 	srcH);

#endif /* GD_WRAPPER_H */
