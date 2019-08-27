//Realizado por Julio Ballesteros y Álvaro Reina

#ifndef _FILTRO_H_
#define _FILTRO_H_
#include "BMPImageLoader.h"
#include <Windows.h>

typedef struct pixel_t
{
	uint8 R;
	uint8 G;
	uint8 B;
}pixel_t;

typedef struct hParams_t
{
	uint8* img;
	uint8* imgResult;
	float* filtro;
	uint32 imgW;
	uint32 imgH;
	uint32 filW;
	uint32 filH;
	uint32 iInitial;
	uint32 iFinal;
}hParams_t;

extern float mask[];

uint8* applyFilter(uint8* img, float* filtro, uint32 imgW, uint32 imgH, uint32 filW, uint32 filH, uint32 numThreads);

unsigned __stdcall threadApplyFilter(void*);

#endif