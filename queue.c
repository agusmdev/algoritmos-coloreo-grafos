#include "TheOutsider.h"

queue_t create_queue(Grafo g) {
    queue_t q = (queue_t)malloc(sizeof(_queue_t));
    q->elems = malloc(g->cant_vertices*sizeof(struct Vertice*));
    q->max_queue = g->cant_vertices;
    empty_queue(q);
    return (q);
}

void empty_queue(queue_t q) {
    q->start = 0;
    q->size = 0;
}

void enqueue(queue_t q, queue_elem e) {
    assert(!is_full_queue(q));
    q->elems[(q->start + q->size) % (q->max_queue)] = e;
    q->size++;
}

queue_elem first(queue_t q) {
    assert(!is_empty(q));
    return q->elems[q->start];
}

void dequeue(queue_t q) {
    assert(!is_empty(q));
    q->size--;
    q->start = (q->start + 1) % (q->max_queue);
}

bool is_empty(queue_t q) {
    return q->size == 0;
}

bool is_full_queue(queue_t q) {
    return q->size == (q->max_queue);
}

void destroy_queue(queue_t q) {
    free(q->elems);
    free(q);
    q = NULL;
}