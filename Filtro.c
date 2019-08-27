//Realizado por Julio Ballesteros y Álvaro Reina

#include "Filtro.h"
#include <string.h>
#include <process.h>
#include <intrin.h>
#include <string.h>

float mask[] = {
	0.1,0.1,0.1,
	0.1,0.1,0.1,
	0.1,0.1,0.1
};

uint8* applyFilter(uint8* img, float* filtro, uint32 imgW, uint32 imgH, uint32 filW, uint32 filH, uint32 numThreads)
{
	uint32 i = 0, j = 0, rowsPerThread = 0, row = 0;

	uint8* imgResult = (uint8*)m_malloc(3 * imgW*imgH);
	memset(imgResult, 0, 3 * imgW*imgH);

	uint8* amplifiedImg = (uint8*)m_malloc(3 * (imgW + 2)*(imgH + 2));
	memset(amplifiedImg, 0, 3 * (imgW + 2)*(imgH + 2));

	for (i = 1; i <= imgH; i++)
		for (j = 1; j <= imgW; j++) {
			((pixel_t*)amplifiedImg)[j + i * (imgW + 2)] = ((pixel_t*)img)[(j - 1) + (i - 1)*imgW];
		}

	HANDLE* hThread = (HANDLE*)m_malloc(sizeof(HANDLE)*numThreads);

	hParams_t* hParams = (hParams_t*)m_malloc(sizeof(hParams_t)*numThreads);

	rowsPerThread = imgH / numThreads;

	for (i = 0; i < numThreads; i++, row += rowsPerThread) {


		hParams[i].img = amplifiedImg;
		hParams[i].imgResult = imgResult;
		hParams[i].filtro = filtro;
		hParams[i].imgW = imgW;
		hParams[i].imgH = imgH;
		hParams[i].filW = filH;
		hParams[i].filH = filW;
		hParams[i].iInitial = row;
		hParams[i].iFinal = row + rowsPerThread;

		if (i == numThreads - 1) hParams[i].iFinal = imgH;

		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, &threadApplyFilter, &hParams[i], 0, NULL);

	}

	for (i = 0; i < numThreads; i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
	}

	m_free(amplifiedImg);
	m_free(hThread);
	m_free(hParams);

	return imgResult;
}

unsigned __stdcall threadApplyFilter(void* params)
{
	hParams_t* hparams = (hParams_t*)params;

	int i = 0, j = 0, x = 0, y = 0, z = 0;

	uint8 *xmm_pixOrig = (uint8*)m_aligned_malloc(sizeof(uint8) * 16, 16);
	uint8 *xmm_pixResult = (uint8*)m_aligned_malloc(sizeof(uint8) * 16, 16);

	for (i = hparams->iInitial; i < hparams->iFinal; i++)
		for (j = 0; j < hparams->imgW * 3; j += 15)
		{

			if (j + 15 > hparams->imgW * 3)	j = hparams->imgW * 3 - 15;

			memset(xmm_pixOrig, 0, 16);
			memset(xmm_pixResult, 0, 16);

			for (y = -1; y < 2; y++)
				for (x = -1; x < 2; x++)
				{
					memcpy(xmm_pixOrig, &hparams->img[(y + i + 1)*(hparams->imgW + 2) * 3 + j + 3 + x * 3], 15);

					for (z = 0; z < 15; z++) {
						xmm_pixOrig[z] = (uint8)(xmm_pixOrig[z] * hparams->filtro[(x + 1) + ((y + 1) * 3)]);
					}

					__asm
					{
						mov edx, [xmm_pixResult]
						movaps xmm1, [edx]

						mov edx, [xmm_pixOrig]
						movaps xmm2, [edx]

						paddb xmm1, xmm2

						mov edx, [xmm_pixResult]
						movaps[edx], xmm1
					}
				}

			memcpy(&hparams->imgResult[i*hparams->imgW * 3 + j], xmm_pixResult, 15);
		}

	m_aligned_free(xmm_pixOrig);
	m_aligned_free(xmm_pixResult);

	_endthreadex(0);
	return 0;
}