//
// GlutPNG.h
// ---------
// Read PNG file.
//

#ifndef __GLUTPNG_H__
#define __GLUTPNG_H__

#include "BaseType.h"
#include <cstdio>

struct png_chunk
{
	ULONG	length;
	BYTE	type[4];
	ULONG	fpos;
	ULONG	crc32;

	png_chunk *next;
};

struct png_header
{
	ULONG	width;
	ULONG	height;
	BYTE	depth;
	BYTE	color_type;
	BYTE	compression_method;
	BYTE	filter_method;
	BYTE	interlace_method;
};

struct png_data
{
	BYTE 	*data;
	ULONG 	size;
};

struct png_file
{
	FILE *fp;
	png_header	*header;
	png_chunk	*chunks;
};

png_file*	png_open(const char *filename);
void		png_close(png_file **file);

png_data*	png_get_raw_data(const char *field);
void		png_release_raw_data(png_data **data);

bool		png_check_signature(FILE *fp);
ULONG 		png_calculate_crc32(BYTE *data, ULONG length);
png_header*	png_resolve_header(png_chunk *IHDR);
png_chunk*	png_read_next_chunk(FILE *fp);

#endif // __GLUTPNG_H__
