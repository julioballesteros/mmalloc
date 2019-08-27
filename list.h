//Desarrollado por Julio Ballesteros y Álvaro Reina

#pragma once


typedef struct node_t
{
	void* addr;
	long int size;
	struct node_t* next;
}node_t;

typedef struct list_t
{
	struct node_t* begin;
	int size; //numero de nodos
}list_t;

void insert_node(struct list_t* list, struct node_t* nnode);
void delette_node(struct list_t* list, struct node_t* nnode);
struct node_t* search_node(struct list_t* list, void* addrKey);
struct node_t* create_new_node(long int newSize);