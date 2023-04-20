#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <string.h>
#include <stdint.h>
#include <stddef.h>

typedef uint32_t rbd_t;

typedef struct {
    size_t s_elem;
    size_t n_elem;
    void *buffer;
} rb_attr_t;

int32_t ring_buffer_initialization( rbd_t *rb, rb_attr_t *attr );
int32_t ring_buffer_put( rbd_t rb, const void *data );
int32_t ring_buffer_get( rbd_t rb, void *data );

#endif // RING_BUFFER_H
