#include "TheOutsider.h"

void print_help(){
  fprintf(stderr, "El grafo esta mal formateado\n"
                  "Este programa solo acepta grafos con formato DIMACS\n"
                  "http://prolland.free.fr/works/research/dsat/dimacs.html\n");
  exit(1);
}

void malloc_fail(){
  fprintf(stderr, "No se pudo alocar memoria\n");
  exit(1);
}

struct Vertice* obtener_vertice(Grafo grafo, u32 nombre){
  u32 mask = grafo->cant_vertices*HASH_RATE;
  u32 indice = nombre % mask;

  struct Vertice* v = grafo->vertices+indice;

  if(grafo->perfect)
    return v;

  for(;;v = (grafo->vertices+indice)){
    if (v->nombre == nombre){
      return v;
    } else {
      indice++; /* Probing lineal */
      indice %= mask;
    }
  }

  return v;
}

u32 NumeroDeVertices(Grafo g) {
  return g->cant_vertices;
}

u32 NumeroDeLados(Grafo g) {
  return g->cant_lados;
}

u32 NumeroDeColores(Grafo g) {
  return g->cant_colores;
}

u32 NombreDelVertice(Grafo g, u32 i){
  return g->nombre_vertices[i];
}

u32 ColorDelVertice(Grafo g, u32 i){
  return *obtener_vertice(g, g->nombre_vertices[i])->color;
}

u32 GradoDelVertice(Grafo g, u32 i){
  return obtener_vertice(g, g->nombre_vertices[i])->cant_vecinos;
}

u32 ColorJotaesimoVecino(Grafo g, u32 i, u32 j){
  return *(obtener_vertice(g, g->nombre_vertices[i])->vecinos[j]->color);
}

u32 NombreJotaesimoVecino(Grafo g, u32 i, u32 j){
  return obtener_vertice(g, g->nombre_vertices[i])->vecinos[j]->nombre;
}

u32 GradoJotaesimoVecino(Grafo g, u32 i, u32 j){
  return obtener_vertice(g, g->nombre_vertices[i])->vecinos[j]->cant_vecinos;
}

void DestruccionDelGrafo(Grafo grafo){
  struct Vertice* v;

  for(u32 i = 0; i < NumeroDeVertices(grafo); i++){
    v = grafo->vertices+i;
    if(!v->empty) {
      free(v->vecinos);
      free(v->colores_vecinos);
    }
  }

  free(grafo->vertices_originales);
  free(grafo->nombre_vertices);
  free(grafo->vertices);
  free(grafo);
}

int Bipartito(Grafo g){
  struct Vertice *v;
  bool bipartito = true;
  /* Variable temporal para guardar un vertice */
  struct Vertice *p;
  /* Numero de componentes conexas */
  int k = 0;
  /* Numero de vertices coloreados */ 
  u32 c = 0;
  queue_t queue = create_queue(g);
  /* Coloreo con 0 todo el grafo, (representa no tener color) */
  memset(g->colores_vertices, COLOR_CERO, g->cant_vertices*sizeof(u32));
  
  while (c < NumeroDeVertices(g)) {
    for(u32 j = 0; j < NumeroDeVertices(g); ++j) {
      v = obtener_vertice(g, g->nombre_vertices[j]);
      if (COLOR_CERO == *(v->color)) {
        *(v->color) = COLOR_UNO;
        enqueue(queue, v);
        c++;
        k++;
        while(!is_empty(queue)) {
          p = first(queue);
          dequeue(queue);
          for(u32 i = 0; i < p->cant_vecinos; ++i) {
            if (COLOR_CERO == *(p->vecinos[i]->color)) {
              enqueue(queue, p->vecinos[i]);
              *(p->vecinos[i]->color) = 3 - *(p->color);
              c++;
            } else if(*(p->color) == *(p->vecinos[i]->color)) {
              bipartito = false;
            }
          }
        }
      }
    }
  }
  
  destroy_queue(queue);

  if (bipartito){
      g->cant_colores = 2;
      return k;
    }

  /* Si no es bipartito coloreamos el grafo con un coloreo propio */
  for (u32 i = 0; i < NumeroDeVertices(g); ++i){
    *(obtener_vertice(g, g->nombre_vertices[i])->color) = i+1;
  }
  g->cant_colores = NumeroDeVertices(g);
  return -k;
}

u32 NotSoGreedy(Grafo g, u32 s){
  u32 colores_totales = 1;
  u32 colores_usados = 0;
  u32 cant_vertices = g->cant_vertices;
  u32 cant_vecinos;

  struct Vertice* v;
  u32* colores = calloc(cant_vertices, sizeof(u32));

  if(colores == NULL)
    malloc_fail();

  memset(g->colores_vertices, 0, g->cant_vertices*sizeof(u32));

  *(obtener_vertice(g, g->nombre_vertices[0])->color) = 1;

  for(u32 i = 1; i < cant_vertices; i++){
    v = obtener_vertice(g, g->nombre_vertices[i]);
    cant_vecinos = v->cant_vecinos;

    for(u32 j = 0; j < cant_vecinos; j++){
      u32 ncolor = *(v->colores_vecinos[j]);
      if(ncolor && colores[ncolor] != i){
        colores[ncolor] = i;
        colores_usados++;
      }
    }

    if (colores_usados >= colores_totales){
      colores_totales++;
      *(v->color) = colores_totales;
    } else {
      u32 choice = pseudorandom(s) % (colores_totales - colores_usados);
      for(u32 j = 1; j < cant_vertices; j++){
        if (colores[j] != i && choice-- == 0){
          *(v->color) = j;
          break;
        }
      }
    }

    colores_usados = 0;
  }

  free(colores);
  g->cant_colores = colores_totales;
  return colores_totales;
}

void agregar_vecino(struct Vertice* v, struct Vertice* n){
  /* Si el vertice no tiene ningun vecino, reservamos memoria para guardalos. */
  if(v->vecinos == NULL){
    v->vecinos = malloc(v->total_vecinos*sizeof(struct Vertice*));
    v->colores_vecinos = malloc(v->total_vecinos*sizeof(u32*));

    if(v->vecinos == NULL || v->colores_vecinos == NULL)
      malloc_fail();
  } else if (v->total_vecinos <= v->cant_vecinos) {
    v->total_vecinos *= 2;
    v->vecinos = realloc(v->vecinos, v->total_vecinos*sizeof(struct Vertice*));
    v->colores_vecinos = realloc(v->colores_vecinos, v->total_vecinos*sizeof(u32*));

    if(v->vecinos == NULL || v->colores_vecinos == NULL)
      malloc_fail();
  }

  v->vecinos[v->cant_vecinos] = n;
  v->colores_vecinos[v->cant_vecinos] = n->color;
  v->cant_vecinos++;
}

struct Vertice* crear_vertice(Grafo grafo, u32 nombre){
  u32 mask = grafo->cant_vertices*HASH_RATE;
  u32 indice = nombre % mask;

  struct Vertice* v = grafo->vertices+indice;

  for(;;v = grafo->vertices+indice){
    if(v->empty){
      v->empty = false;
      v->cant_vecinos = 0;
      v->color = &grafo->colores_vertices[grafo->cant_creados];
      v->nombre = nombre;
      v->total_vecinos = VECINOS_INICIAL;
      v->vecinos = NULL;
      grafo->nombre_vertices[grafo->cant_creados] = nombre;
      grafo->vertices_originales[grafo->cant_creados] = nombre;
      grafo->colores_vertices[grafo->cant_creados] = grafo->cant_creados;
      grafo->cant_creados++;

      return v;
    } else if (v->nombre == nombre){
      return v;
    } else {
      grafo->perfect = false;
      indice++;
      indice %= mask;
    }
  }
}

void agregar_lado(Grafo grafo, u32 x, u32 y){
  struct Vertice* v = crear_vertice(grafo, x);
  struct Vertice* n = crear_vertice(grafo, y);

  agregar_vecino(v,n);
  agregar_vecino(n,v);
}

void parsear_lado(FILE *input, u32* x, u32* y){
    char linea[80];
    if (fgets(linea, 80, input) == NULL)
      print_help();

    if (sscanf(linea, "e %u %u", x, y) != 2)
      print_help();
}

Grafo crear_grafo(u32 cant_lados, u32 cant_vertices){
  Grafo grafo = malloc(sizeof(struct GrafoSt));
  struct Vertice* vertices = calloc(HASH_RATE*cant_vertices, sizeof(struct Vertice));

  if (grafo == NULL || vertices == NULL)
    malloc_fail();

  for(u32 i = 0; i < HASH_RATE*cant_vertices; i++){
    vertices[i].empty = true;
  }

  grafo->perfect = true;
  grafo->cant_creados = 0;
  grafo->cant_vertices = grafo->cant_colores = cant_vertices;
  grafo->nombre_vertices = malloc(cant_vertices*sizeof(u32));
  /* Arreglo auxiliar que vamos a utilizar en el momento de ordenar los vertices aleatoriamente. */
  grafo->vertices_originales = malloc(cant_vertices*sizeof(u32));
  grafo->colores_vertices = malloc(cant_vertices*sizeof(u32));

  if (grafo->nombre_vertices == NULL || grafo->colores_vertices == NULL 
                                     || grafo->vertices_originales == NULL)
    malloc_fail();

  grafo->cant_lados = cant_lados;
  grafo->vertices = vertices;

  return grafo;
}

void parsear_grafo(FILE *input, u32* x, u32* y){
    char linea[80];

    if (fgets(linea, 80, input) == NULL){
      print_help();
    }
    while(linea[0] == 'c') {
      if (fgets(linea, 80, input) == NULL)
        print_help();
    }

    if (sscanf(linea, "p edge %u %u", x, y) != 2)
      print_help();
}

Grafo ConstruccionDelGrafo(){
  u32 cant_vertices, cant_lados, x, y;
  Grafo grafo;

  parsear_grafo(stdin, &cant_vertices, &cant_lados);
  grafo = crear_grafo(cant_lados, cant_vertices);

  for(u32 i = 0; i < cant_lados; i++){
    parsear_lado(stdin, &x, &y);
    agregar_lado(grafo, x, y);
  }

  return grafo;
}