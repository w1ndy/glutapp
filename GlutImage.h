//
// GlutImage.h
// -----------
// Provide image loading and processing functionality.
//

#ifndef __GLUTIMAGE_H__
#define __GLUTIMAGE_H__

#include "BaseType.h"

enum ImageType
{
	ImageType_A8R8G8B8,
	ImageType_R8G8B8
};

class GlutImage
{
public:
	static GlutImage *fromPNG(const wchar_t *pngFileName);
	void destroy();

private:
	ImageType	_type;
	ULONG		_width, _height;
	BYTE 		*_data;
	ULONG		_datalen;
	BYTE		*_palette;
	ULONG		_palen;

private:
	void _initializeObject();

private:
	GlutImage();
	~GlutImage();

	GlutImage(const GlutImage &);
	GlutImage &operator=(const GlutImage &);
};

#endif // __GLUTIMAGE_H__
