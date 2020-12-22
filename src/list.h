/* Embedded doubly-linked lists.
 *
 * From-scratch impl inspired by the Linux kernel source's list.h
 * Copyright (C) 2002 Gordon McNutt
 */

#ifndef list_h
#define list_h

#include <stddef.h>

#define list_for_each(head, ptr) \
        for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)
#define list_init(list) { (list)->next = (list); (list)->prev = (list); }
#define list_empty(list) ((list)->next == (list) && (list)->prev == (list))
#define list_entry(ptr, type, field) \
        ((type*)((char*)(ptr)-(unsigned long)(&((type *)0)->field)))

struct list {
        struct list *next;
        struct list *prev;
};

static inline void list_add_aux(struct list *before, struct list *after,
                                struct list *between)
{
        between->next = after;
        between->prev = before;
        before->next = between;
        after->prev = between;
}

static inline void list_add(struct list *head, struct list *ptr)
{
        list_add_aux(head, head->next, ptr);
}

static inline void list_add_tail(struct list *head, struct list *ptr)
{
        list_add_aux(head->prev, head, ptr);
}

static inline void list_remove(struct list *list)
{
        list->prev->next = list->next;
        list->next->prev = list->prev;
        list_init(list);
}

static inline void list_switch_adjacent(struct list *first, struct list *second)
{
        first->prev->next = second;
        second->prev = first->prev;
        first->prev = second;
        first->next = second->next;
        second->next->prev = first;
        second->next = first;
}

static inline void list_replace(struct list *orig, struct list *repl)
{
        if (orig == repl)
                return;

        *repl = *orig;
        orig->next->prev = repl;
        orig->prev->next = repl;
}

static inline void list_switch(struct list *e1, struct list *e2)
{
        struct list tmp;

        list_replace(e1, &tmp);
        list_replace(e2, e1);
        list_replace(&tmp, e2);
}

static inline size_t list_len(struct list *head)
{
        struct list *list = head->next;
        size_t n = 0;
        while (list != head) {
                list = list->next;
                n++;
        }
        return n;
}

static inline void list_move(struct list *new_head, struct list *old_head)
{
        if (list_empty(old_head)) {
                list_init(new_head);
                return;
        }

        *new_head = *old_head;
        new_head->next->prev = new_head;
        new_head->prev->next = new_head;

        list_init(old_head);
}

static inline void list_clear(struct list *head)
{
        struct list *next = head->next;
        while (next != head) {
                struct list *cur = next;
                next = cur->next;
                list_remove(cur);
        }
}

#endif
