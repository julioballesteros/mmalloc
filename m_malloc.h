//Desarrollado por Julio Ballesteros y Álvaro Reina

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct mmalloc_header_t
{
	void* addr; //Dirección de memoria de comienzo del array de reserva
	long int addrSize; //Almacena el tamaño del array de reserva
	long int freeMem;
	struct list_t blocksOccp; //lista de bloques ocupados
	struct list_t blocksFree; //Lista de bloques libres
}mmalloc_header_t;

void init_mmalloc(void* addr, long int size);
void end_mmalloc();
void* m_malloc(long int size);
void* m_realloc(void* addr, long int size);
void m_free(void* addr);

void * m_aligned_malloc(size_t align, size_t size);
void m_aligned_free(void * ptr);
