/*
Integrantes:

De Francesca Joaquin | e-mail: joacoemilianodf@gmail.com
Marchi Agustin | e-mail: agusmarchi1997@gmail.com
Torti Ariel | e-mail: arieltorti14@gmail.com
*/

#ifndef THEOUTSIDER_H_
#define THEOUTSIDER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define VECINOS_INICIAL 5
#define HASH_RATE 4 // 2 or 4
#define COLOR_CERO 0
#define COLOR_UNO 1

typedef unsigned int u32;
typedef struct GrafoSt* Grafo;

struct Vertice{
  bool empty;
  u32 nombre;
  u32* color;
  u32 cant_vecinos;
  u32 total_vecinos;
  u32** colores_vecinos;
  struct Vertice** vecinos;
};

struct GrafoSt{
  /* Si no hay colisiones en la hash table, el grafo es perfecto */
  bool perfect;
  u32 cant_vertices;
  u32 cant_lados;
  u32 cant_colores;
  u32 cant_creados;
  u32* vertices_originales;
  u32* nombre_vertices;
  u32* colores_vertices;
  struct Vertice* vertices;
};

struct qsort_cmp{
  u32 size;
  u32 *vertices_color;   
};

u32 NumeroDeVertices(Grafo g);
u32 NumeroDeLados(Grafo g);
u32 NumeroDeColores(Grafo g);
u32 NombreDelVertice(Grafo g, u32 i);
u32 ColorDelVertice(Grafo g, u32 i);
u32 GradoDelVertice(Grafo g, u32 i);
u32 ColorJotaesimoVecino(Grafo g, u32 i, u32 j);
u32 NombreJotaesimoVecino(Grafo g, u32 i, u32 j);
u32 GradoJotaesimoVecino(Grafo g, u32 i, u32 j);

void OrdenNatural(Grafo g);
void OrdenWelshPowell(Grafo g);
void AleatorizarVertices(Grafo g, u32 s);
void ReordenManteniendoBloqueColores(Grafo, u32 x);

Grafo ConstruccionDelGrafo();
void DestruccionDelGrafo(Grafo grafo);

int Bipartito(Grafo);
u32 NotSoGreedy(Grafo, u32 s);

unsigned int pseudorandom(unsigned int seed);
void set_seed(unsigned int seed);

/* start queue  */
typedef struct Vertice *queue_elem;

typedef struct _queue_t *queue_t;

typedef struct _queue_t {
    u32 start;
    u32 size;
    queue_elem *elems;
    u32 max_queue;
} _queue_t;


queue_t create_queue();
queue_elem first(queue_t q);
bool is_empty(queue_t q);
bool is_full_queue(queue_t q);
void empty_queue(queue_t q);
void enqueue(queue_t q, queue_elem e);
void dequeue(queue_t q);
void destroy_queue(queue_t q);
/* end queue */
#endif