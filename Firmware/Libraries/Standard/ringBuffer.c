#include "ringbuffer.h"
#include <xc.h> // For INTCON2bits.GIE

// Optional critical section using GIE flag
static inline uint16_t rb_int_disable(bool protect) {
    if (protect) {
        uint16_t gie_state = INTCON2bits.GIE;
        INTCON2bits.GIE = 0;
        return gie_state;
    }
    return 0xFFFF;
}

static inline void rb_int_restore(uint16_t prev) {
    if (prev != 0xFFFF) {
        INTCON2bits.GIE = prev;
    }
}

// Check power of two (runtime check for init)
static inline bool is_power_of_two(uint16_t x) {
    return x && !(x & (x - 1));
}

bool ringbuffer_init(RingBuffer* rb, uint8_t* storage, uint16_t size, bool protect) {
    if (!is_power_of_two(size)) {
        return false;
    }

    rb->buffer = storage;
    rb->size = size;
    rb->mask = size - 1;
    rb->head = 0;
    rb->tail = 0;
    rb->protect = protect;
    return true;
}

void ringbuffer_clear(RingBuffer* rb) {
    uint16_t gie = rb_int_disable(rb->protect);
    rb->head = 0;
    rb->tail = 0;
    rb_int_restore(gie);
}

bool ringbuffer_empty(const RingBuffer* rb) {
    return rb->head == rb->tail;
}

bool ringbuffer_full(const RingBuffer* rb) {
    return ((rb->head - rb->tail) & rb->mask) == rb->mask;
}

bool ringbuffer_push(RingBuffer* rb, uint8_t value) {
    uint16_t gie = rb_int_disable(rb->protect);

    uint16_t next_head = (rb->head + 1) & rb->mask;
    if (next_head == (rb->tail & rb->mask)) {
        rb_int_restore(gie);
        return false; // Full
    }

    rb->buffer[rb->head & rb->mask] = value;
    rb->head = next_head;

    rb_int_restore(gie);
    return true;
}

bool ringbuffer_pop(RingBuffer* rb, uint8_t* value) {
    uint16_t gie = rb_int_disable(rb->protect);

    if (rb->head == rb->tail) {
        rb_int_restore(gie);
        return false; // Empty
    }

    *value = rb->buffer[rb->tail & rb->mask];
    rb->tail = (rb->tail + 1) & rb->mask;

    rb_int_restore(gie);
    return true;
}

bool ringbuffer_peek(RingBuffer* rb, uint8_t* value) {
    uint16_t gie = rb_int_disable(rb->protect);

    if (rb->head == rb->tail) {
        rb_int_restore(gie);
        return false;
    }

    *value = rb->buffer[rb->tail & rb->mask];

    rb_int_restore(gie);
    return true;
}
