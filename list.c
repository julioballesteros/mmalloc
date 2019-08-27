//Desarrollado por Julio Ballesteros y Álvaro Reina

#include "list.h"
#include "m_malloc.h"


void insert_node(struct list_t* list, struct node_t* nnode) {
	//buscar último nodo
	int i = 0;
	int listSize = list->size;
	struct node_t* lastNode = 0x00;//NULL

	if (listSize == 0)
	{
		list->begin = nnode;
		nnode->next = 0x00;
		list->size++;
	}
	else
	{
		lastNode = list->begin;
		for (int i = 1; i < listSize; i++)
		{
			lastNode = lastNode->next;
		}
		//añadir nodo al último
		if (lastNode != 0x00)
		{
			lastNode->next = nnode;
			nnode->next = 0x00;
			list->size++;
		}
	}
}

void delette_node(struct list_t* list, struct node_t* nnode)
{
	//buscar el padre del nodo dado
	int i = 0;
	int listSize = list->size;
	struct node_t* parentNode = 0x00;

	if (listSize == 0)
	{
		//error
		printf("ERROR: No se encuentra el nodo para borrar");
	}
	else
	{
		parentNode = list->begin;
		if (parentNode->addr != nnode->addr)
			while ((i < (listSize - 1)) && (parentNode->next->addr != nnode->addr))
			{
				parentNode = parentNode->next;
				i++;
			}
		//borrar
		if (i != listSize - 1)
		{
			parentNode->next = nnode->next;
			list->size--;
		}
		else {
			//error
			printf("ERROR: No se encuentra el nodo para borrar");
		}
	}
}

struct node_t* search_node(struct list_t* list, void* addrKey) {
	//buscar nodo
	int i = 0;
	int listSize = list->size;
	struct node_t* foundNode = 0x00;

	if (listSize == 0)
	{
		return 0x00;
	}
	else
	{
		foundNode = list->begin;
		while ((foundNode->addr != addrKey) && (i < listSize))
		{
			foundNode = foundNode->next;
			i++;
		}
		//devolver nodo
		if (i != listSize)
		{
			return foundNode;
		}
		else {
			return 0x00;
		}
	}
}

extern struct mmalloc_header_t* header;

struct node_t* create_new_node(long int newSize)
{
	struct node_t* nnode;
	struct node_t* foundNode;
	long int sizeNNode = sizeof(struct node_t) + newSize;
	//buscar nodo con tamaño suficiente
	if (sizeNNode > header->freeMem)
	{
		//error
		return 0x00;
	}
	else
	{
		int i = 0;
		foundNode = header->blocksFree.begin;
		while ((foundNode->size < sizeNNode) && (i < header->blocksFree.size))
		{
			foundNode = foundNode->next;
			i++;
		}

		//dividir nodo
		if (i != (header->blocksFree.size))
		{
			nnode = (struct node_t*) foundNode->addr;
			((long int)foundNode->addr) += sizeNNode;
			foundNode->size -= sizeNNode;

			nnode->addr = (void*)(((long int)foundNode->addr) - newSize);
			nnode->next = 0x00;
			nnode->size = newSize;
			return nnode;
		}
		else
		{
			//error
			return 0x00;
		}
	}

}