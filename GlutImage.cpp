//
// GlutImage.cpp
//

#include "GlutImage.h"
#include "GlutPNG.h"

GlutImage *GlutImage::fromPNG(const wchar_t *pngFileName)
{
	// load png file here.
	return NULL;
}

void GlutImage::destroy()
{
	if(_data) ::operator delete(_data);
	if(_palette) ::operator delete(_palette);
	delete this;
}

void GlutImage::_initializeObject()
{
	_type = ImageType_A8R8G8B8;
	_width = 0;
	_height = 0;
	_data = NULL, _datalen = 0;
	_palette = NULL, _palen = 0;
}
