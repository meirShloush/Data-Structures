#ifndef DYN_VEC_H
#define DYN_VEC_H
#include <stddef.h> /* size_t */

typedef struct dyn_vec dyn_vec_t;

dyn_vec_t *DynVecCreate(size_t item_size, size_t num_items);
void DynVecDestroy(dyn_vec_t *vec);
size_t DynVecSize(const dyn_vec_t *vec);
int DynVecPushBack(dyn_vec_t *vec, const void *item);
void DynVecPopBack(dyn_vec_t *vec);
void *DynVecGetItemAddress(const dyn_vec_t *vec, size_t index);
size_t DynVecCapacity(const dyn_vec_t *vec);
int DynVecReserve(dyn_vec_t *vec, size_t new_capacity);

#endif /* DYN_VEC_H */
