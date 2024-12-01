#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); })

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)    

static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

struct fox {
    unsigned long tail_length;
    unsigned long weight;
    int is_fantastic;
    struct list_head list;
};

struct fox red_fox = {
    .tail_length = 40,
    .weight = 10,
    .is_fantastic = 0,
    .list = LIST_HEAD_INIT(red_fox.list)
};

static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next) { \
        struct fox *f = container_of(pos, struct fox, list); \
        printf("@@fox: %d, %d %d\n", f->tail_length, f->weight, f->is_fantastic); \
    }

int main(void) {
    printf("size: %d, %d, %d, %d\n", sizeof(struct fox), sizeof(unsigned long), sizeof(int), sizeof(struct list_head));
    printf("red_fox: %d, %d %d \n", red_fox.tail_length, red_fox.weight, red_fox.is_fantastic);
    printf("red_fox: %p, %p %p %p %p\n", &red_fox, &red_fox.tail_length, &red_fox.weight, &red_fox.is_fantastic, &red_fox.list);
    printf("container_of: %p\n", container_of(&red_fox.list, struct fox, list));

    struct fox *blue_fox = malloc(sizeof(struct fox));
    blue_fox->tail_length = 50;
    blue_fox->weight = 15;
    blue_fox->is_fantastic = 1;
    INIT_LIST_HEAD(&blue_fox->list);
    list_add(&blue_fox->list, &red_fox.list);
    
    struct fox *blue_fox2 = malloc(sizeof(struct fox));
    blue_fox2->tail_length = 1500;
    blue_fox2->weight = 150;
    blue_fox2->is_fantastic = 0;
    INIT_LIST_HEAD(&blue_fox2->list);
    list_add(&blue_fox2->list, &red_fox.list);

    struct list_head *pos;
    list_for_each(pos, &red_fox.list) 
}