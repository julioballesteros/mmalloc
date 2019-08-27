//Desarrollado por Julio Ballesteros y Álvaro Reina

#include "m_malloc.h"


struct mmalloc_header_t* header = 0x00;

void init_mmalloc(void* addr, long int size)
{
	header = (mmalloc_header_t*)addr;
	header->addr = addr;
	header->addrSize = size;
	header->freeMem = size;
	header->blocksFree.begin = 0x00;
	header->blocksFree.size = 0;
	header->blocksOccp.begin = 0x00;
	header->blocksOccp.size = 0;

	((long int)header->addr) += sizeof(mmalloc_header_t);
	header->freeMem -= sizeof(mmalloc_header_t);

	//Crear nuevo nodo de memoria libre
	struct node_t* nnode = (struct node_t*)header->addr;
	header->freeMem -= sizeof(struct node_t);
	((long int)header->addr) += sizeof(struct node_t);
	nnode->addr = header->addr;
	nnode->size = header->freeMem;
	//insertar en la lista de nodos libres;
	insert_node(&(header->blocksFree), nnode);
}

void end_mmalloc() {
	free(header);
}

void* m_malloc(long int size)
{
	struct node_t* nnode = 0x00;
	nnode = create_new_node(size);
	if (nnode == 0x00)
	{
		//control errores
		printf("ERROR: No hay memoria suficiente\n");
	}
	else
	{
		insert_node(&(header->blocksOccp), nnode);
		return nnode->addr;
	}
	return 0x00;
}

void m_free(void* addr)
{
	node_t* node = 0x00;
	//Buscar el nodo en ocupados
	node = search_node(&(header->blocksOccp), addr);
	//si no encontrado
	if (node == 0x00)
	{
		//error
		printf("ERROR: El puntero pasado como argumento no es valido\n");
	}
	//else
	else
	{
		//Borrar el nodo de ocupados
		delette_node(&(header->blocksOccp), node);
		//Meter nodo en libres
		insert_node(&(header->blocksFree), node);
	}
}

void* m_realloc(void* addr, long int size)
{
	node_t* node = 0x00;
	int newSize = 0;
	int i = 0;
	char* oldBlock = 0x00;
	char* newBlock = 0x00;

	//Se reserva la nueva memoria de tamaño indicado
	newBlock = (char*)m_malloc(size);
	if (newBlock == 0x00)
	{
		//error
		return 0x00;
	}
	//Se copian los datos a la nueva memoria
	node = search_node(&(header->blocksOccp), addr);
	if (node == 0x00)
	{
		//error
		printf("ERROR: El puntero pasado como argumento no es valido\n");
		return 0x00;
	}
	oldBlock = (char*)node->addr;
	newSize = size < node->size ? size : node->size;
	for(i = 0; i < newSize; i++)
	{
		newBlock[i] = oldBlock[i];
	}


	//Se libera la memoria anterior
	m_free(addr);
	//Se devuelve el puntero al nuevo bloque de memoria
	return newBlock;
}

//Codigo sacado de https://github.com/embeddedartistry/embedded-resources/blob/master/examples/c/malloc_aligned.c


//Macro para alinear la direccion de memoria a la potencia de dos indicada
#ifndef align_up
#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))
#endif

//Convenience macro for memalign, the linux API
#define memalign(align, size) aligned_malloc(align, size)

//Tipo usado para almacenar el offset
typedef uint16_t offset_t;
//Numero de bytes usado para el offset
#define PTR_OFFSET_SZ sizeof(offset_t)

void * m_aligned_malloc(size_t align, size_t size)
{
	void * ptr = 0x00;

	//Aseguramos que align sea una potencia de dos
	assert((align & (align - 1)) == 0);

	//Si align y size no son 0, si lo son devuelve NULL
	if (align && size)
	{
		//Almacenamos el numero de bytes que utilizaremos como offset
		uint32_t hdr_size = PTR_OFFSET_SZ + (align - 1);
		//Reservamos memoria para el tamaño pedido mas el offset
		void * p = m_malloc(size + hdr_size);

		//Si se ha hecho la reserva, si no devuelve NULL
		if (p)
		{
			//Se añade el tamaño del offset al puntero (siempre se añade)
			//Se llama a align_up con el resultado y el valor de alineado
			ptr = (void *)align_up(((uintptr_t)p + PTR_OFFSET_SZ), align);

			//Calcular el offset y guardarlo antes del puntero
			*((offset_t *)ptr - 1) = (offset_t)((uintptr_t)ptr - (uintptr_t)p);

		}
	}

	//Se devuelve el puntero alineado
	return ptr;
}

void m_aligned_free(void * ptr)
{
	//Comprobamos que ptr no es NULL
	assert(ptr);

	//Obtenemos el offset guardado en la reserva, que está guardado antes del puntero
	offset_t offset = *((offset_t *)ptr - 1);

	//Con el offset, restamos al puntero alineado para obtener el puntero original
	void * p = (void *)((uint8_t *)ptr - offset);
	//Se llama a free con el puntero original
	m_free(p);
}