#include "TheOutsider.h"

/*
  Las funciones de ordenamiento Reorden_bloque_0 y OrdenWelshPowell utilizan la misma
  logica pero comparan distintas variables.
  Reference_ordering es una abstraccion para evitar la repeticion de codigo.
  Recibe una funcion en referencia a la cual ordenara el arreglo dado.
 */
void reference_ordering(Grafo g, 
                        u32 (*cmp_getter)(Grafo g, u32 i));
void reorden_bloque_0(Grafo g);
void reorden_bloque_1(Grafo g);
void reorden_bloque_aleatorio(Grafo g, u32 i);
struct qsort_cmp* llenar_bloques(Grafo g);
void actualizar_vertices(Grafo g, struct qsort_cmp *block);

int criterio_natural(const void *a, const void *b){
    u32 x = *(u32*) a;
    u32 y = *(u32*) b;
    if (x > y) return  1;
    if (y < x) return -1;
    return 0;
}

int criterio_multiproposito(const void *a, const void *b){
  u32 x = **((u32**) a);
  u32 y = **((u32**) b);
  if (x < y) return  1;
  if (y > x) return -1;
  return 0;
}

int criterio_reorden_bloque_1(const void *a, const void *b){
  u32 x = (*(struct qsort_cmp*)a).size;
  u32 y = (*(struct qsort_cmp*)b).size;  
  if (x > y) return  1;
  if (y < x) return -1;
  return 0;
}

void OrdenNatural(Grafo g){
  qsort(g->nombre_vertices, NumeroDeVertices(g), sizeof(u32), criterio_natural);
}

void AleatorizarVertices(Grafo g, u32 s){
  u32 cant_vertices = g->cant_vertices;
  u32 random, tmp;

  memcpy(g->nombre_vertices, g->vertices_originales, sizeof(u32)*g->cant_vertices);

  for(u32 i = 0; i < cant_vertices; i++){
    random = pseudorandom(i+s) % (cant_vertices-1);
    tmp = g->nombre_vertices[i];
    g->nombre_vertices[i] = g->nombre_vertices[random];
    g->nombre_vertices[random] = tmp;
  }
}

void OrdenWelshPowell(Grafo g){
  reference_ordering(g, &GradoDelVertice);
}

void ReordenManteniendoBloqueColores(Grafo g, u32 x){
  switch(x){
    case 0:
      reorden_bloque_0(g);
      break;
    case 1:
      reorden_bloque_1(g);
      break;
    default:
      reorden_bloque_aleatorio(g, x);
      break;
  }
}

void reference_ordering(Grafo g, 
                        u32 (*cmp_getter)(Grafo g, u32 i)){
  u32 size = g->cant_vertices;
  u32 **cmp = malloc(size*sizeof(u32*));

  for (u32 i = 0; i < size; i++){
    cmp[i] = malloc(2*sizeof(u32));
    cmp[i][1] = NombreDelVertice(g, i);
    cmp[i][0] = (*cmp_getter)(g, i);
  }
  qsort(cmp, size, sizeof(u32*), criterio_multiproposito);

  for (u32 i = 0; i < size; i++){
    g->nombre_vertices[i] = cmp[i][1];
    free(cmp[i]);
  }
  free(cmp);
}

void reorden_bloque_0(Grafo g) {
  reference_ordering(g, &ColorDelVertice);
}

void reorden_bloque_1(Grafo g) {
  struct qsort_cmp *block = llenar_bloques(g);
  qsort(block, g->cant_colores, sizeof(struct qsort_cmp), criterio_reorden_bloque_1);
  actualizar_vertices(g, block);
}

struct qsort_cmp* llenar_bloques(Grafo g) {
  u32 size = NumeroDeVertices(g);
  struct qsort_cmp *colors_matrix = malloc(sizeof(struct qsort_cmp)*g->cant_colores);
  u32 **vertcolor = malloc(sizeof(u32*)*size);
  u32 count_bloques = 0; //vamos contando por bloques de colores
  u32 i_vertcolor = 0; //iterar vertcolor
  u32 mem = 0; // memoria para saber donde quedo el recorrido del vertcolor
  for (u32 i = 0; i < size; i++) {
    vertcolor[i] = malloc(2*sizeof(u32));
    vertcolor[i][1] = NombreDelVertice(g, i);
    vertcolor[i][0] = ColorDelVertice(g, i);
  }
  qsort(vertcolor, NumeroDeVertices(g), sizeof(u32*), criterio_multiproposito);
  // hasta aca tengo vertcolor ordenado de mayor a menor segun colores
  // ahora hacemos un for que guarde en cada colors_matrix la cant de vertices que tienen el color X
  //segun el orden que dejo el qsort en vertcolor
  for (u32 i = 0; i < g->cant_colores; i++) {
    while (i_vertcolor < size && vertcolor[i_vertcolor][0] == vertcolor[mem][0]) {
        i_vertcolor++;
      }
      colors_matrix[i].size = i_vertcolor - mem;
      colors_matrix[i].vertices_color = malloc(colors_matrix[i].size*sizeof(u32)); //guardamos cada qsort_cmp
      for (u32 j = 0; j < colors_matrix[i].size; j++){
        colors_matrix[i].vertices_color[j] = vertcolor[mem+j][1]; //guardamos cada qsort_cmp
      }
      mem = i_vertcolor;
      count_bloques++; //suma 1 despues de llenar cada qsort_cmp, tiene que ser al final = g->cant color
   }

  for (u32 i = 0; i < size; i++) {
    free(vertcolor[i]);
  }

  free(vertcolor);
  assert(count_bloques == g->cant_colores);
  return colors_matrix;

}

void actualizar_vertices(Grafo g, struct qsort_cmp *block) {
  u32 z = 0;
  for (u32 i = 0; i < g->cant_colores; i++) {
   for (u32 j = 0; j < block[i].size; j++) {
    g->nombre_vertices[z] = block[i].vertices_color[j];
    z++;
   }
  }
  for (u32 i = 0; i < g->cant_colores; i++) {
    free(block[i].vertices_color);
  }
  free(block);

}

void ordenar_bloques_aleatorio(struct qsort_cmp *block, u32 size, u32 s) {
    u32 random;

    struct qsort_cmp tmp;
    for (u32 i = 0; i < size; ++i){
        random = pseudorandom(i+s) % (size);
        tmp = block[i];
        block[i] = block[random];
        block[random] = tmp;
    }
}

void reorden_bloque_aleatorio(Grafo g, u32 x) {
  set_seed(x);
  struct qsort_cmp *block = llenar_bloques(g);
  ordenar_bloques_aleatorio(block, g->cant_colores, x);
  actualizar_vertices(g, block);
}