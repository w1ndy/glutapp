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
	BYTE	*data;
	ULONG 	length;
};

struct png_file
{
	FILE *fp;
	png_header	*header;
	png_chunk	*chunks;
};

png_file*	png_open(const char *filename);
void		png_close(png_file **file);

png_data*	png_get_raw_data(png_file *file, const char *field);
void		png_release_raw_data(png_data **data);
png_header*	png_get_header(png_chunk *IHDR, FILE *fp);
void		png_release_header(png_header **header);
png_chunk*	png_read_chunk(FILE *fp);
void		png_destroy_chunk(png_chunk **chunk);

bool		png_check_signature(FILE *fp);
ULONG 		png_calculate_crc32_with_type(const BYTE *type, const BYTE *data, ULONG length);

#endif // __GLUTPNG_H__
