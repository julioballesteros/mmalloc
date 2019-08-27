//Realizado por Julio Ballesteros y Álvaro Reina

#include "Filtro.h"
#define NUMTHREADS 5

int main(int argc, char** argv)
{

	init_mmalloc(malloc(1000000), 1000000);

	BMPImage_t* image = loadBMP("FLAG_B24.bmp");
	BMPImage_t imgResult;

	imgResult.header.signature[0] = 'B';
	imgResult.header.signature[1] = 'M';
	imgResult.header.fileSize = image->infoHeader.width*image->infoHeader.height * 3 + 14 + 40 + 0;
	imgResult.header.reserved = 0;
	imgResult.header.dataOffset = 14 + 40 + 0;
	imgResult.infoHeader.size = 40;
	imgResult.infoHeader.width = image->infoHeader.width;
	imgResult.infoHeader.height = image->infoHeader.height;
	imgResult.infoHeader.planes = 1;
	imgResult.infoHeader.bpp = 24;
	imgResult.infoHeader.compression = 0;
	imgResult.infoHeader.imageSize = 0;
	imgResult.infoHeader.xPixelsPerM = 0;
	imgResult.infoHeader.yPixelsPerM = 0;
	imgResult.infoHeader.colorsUsed = 0;
	imgResult.infoHeader.importantColors = 0;
	imgResult.pixels = applyFilter(image->pixels, mask, image->infoHeader.width, image->infoHeader.height, 3, 3, NUMTHREADS);
	imgResult.palette = NULL;

	writeBMP(&imgResult, "imgResult.bmp");

	m_free(imgResult.pixels);
	m_free(image->pixels);
	end_mmalloc();

	return 0;
}