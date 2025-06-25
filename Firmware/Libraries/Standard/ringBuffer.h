#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * RingBuffer structure for uint8_t data.
 * Uses uint16_t indices and power-of-two sized buffers.
 */
typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    uint8_t* buffer;
    uint16_t size;   // Must be power of two
    uint16_t mask;   // size - 1
    bool protect;    // true = use GIE locking
} RingBuffer;

// API
bool ringbuffer_init(RingBuffer* rb, uint8_t* storage, uint16_t size, bool protect);
void ringbuffer_clear(RingBuffer* rb);
bool ringbuffer_empty(const RingBuffer* rb);
bool ringbuffer_full(const RingBuffer* rb);
bool ringbuffer_push(RingBuffer* rb, uint8_t value);
bool ringbuffer_pop(RingBuffer* rb, uint8_t* value);
bool ringbuffer_peek(RingBuffer* rb, uint8_t* value);

// Macro: static ring buffer + pointer
#define _IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x)-1)) == 0))

// Compile-time assert macro that works in C99 or older:
#define RINGBUFFER_STATIC_ASSERT(cond, msg) \
    typedef char static_assertion_##msg[(cond) ? 1 : -1]

#define RINGBUFFER_DEFINE(name, size_pow2, enable_protect)             \
    RINGBUFFER_STATIC_ASSERT(_IS_POWER_OF_TWO(size_pow2), size_must_be_power_of_two); \
    static uint8_t name##_storage[size_pow2];                          \
    static RingBuffer name##_instance = {                              \
        .head = 0,                                                     \
        .tail = 0,                                                     \
        .buffer = name##_storage,                                      \
        .size = size_pow2,                                             \
        .mask = (size_pow2 - 1),                                       \
        .protect = enable_protect                                      \
    };                                                                \
    static RingBuffer* name = &name##_instance

#endif // RINGBUFFER_H
