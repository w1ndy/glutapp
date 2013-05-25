//
// PngTest.cpp
//

#include <iostream>
#include "GlutPNG.h"
using namespace std;

int main(int argc, char **argv)
{
	if(argc != 2) {
		cout << "Usage: " << argv[0] << " [PNG FileName]" << endl;
		return 0;
	}

	png_file *file;
	file = png_open(argv[1]);

	fseek(file->fp, 0, SEEK_END);
	printf("PNG %ld bytes OK\n", ftell(file->fp));
	puts("");

	puts(" -- brief info");
	printf("\twidth = %ld\n\theight = %ld\n\tdepth = %d\n",
			file->header->width, file->header->height, file->header->depth);
	printf("\tcolor type = %d\n\tcompression method = %d\n\tfilter method = %d\n",
			file->header->color_type, file->header->compression_method,
			file->header->filter_method);
	printf("\tinterlace method = %d\n", file->header->interlace_method);
	puts("");

	puts(" -- valid chunk list");
	png_chunk *p = file->chunks;
	while(p) {
		printf(" --- %.*s\n\tlength = %ld bytes\n\tstart at = %ld bytes\n\tcrc32 = 0x%08X\n",
				4, p->type, p->length, p->fpos, p->crc32);
		p = p->next;
	}

	png_close(&file);
	return 0;
}
