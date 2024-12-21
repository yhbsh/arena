#include <stdlib.h>

typedef struct Arena {
    char *memory;
    int capacity;
    int offset;
    struct Arena *parent;
} Arena;

Arena *arena_create(int capacity, Arena *parent) {
    Arena *arena = (Arena *)malloc(sizeof(Arena));
    if (!arena) return NULL;

    if (parent) {
        arena->memory = parent->memory + parent->offset;
        if (parent->offset + capacity > parent->capacity) {
            free(arena);
            return NULL;
        }
        parent->offset += capacity;
    } else {
        arena->memory = (char *)malloc(capacity);
        if (!arena->memory) {
            free(arena);
            return NULL;
        }
    }

    arena->capacity = capacity;
    arena->offset = 0;
    arena->parent = parent;
    return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    if (arena->offset + size > arena->capacity) {
        return NULL;
    }
    void *ptr = arena->memory + arena->offset;
    arena->offset += size;
    return ptr;
}

void arena_reset(Arena *arena) {
    if (!arena->parent) {
        arena->offset = 0;
    }
}

void arena_destroy(Arena *arena) {
    if (!arena->parent) {
        free(arena->memory);
    }
    free(arena);
}
