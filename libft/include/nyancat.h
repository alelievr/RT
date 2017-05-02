/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nyancat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 23:52:18 by alelievr          #+#    #+#             */
/*   Updated: 2017/05/02 17:35:18 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NYANCAT_H
# define NYANCAT_H

# include <stdbool.h>

/*
**	Utils:
*/

# define CONCAT(x, y)	x##y
# define STRINGYFY(x)	#x
# define ALIAS(x, y)	typeof(x) y = x;
# define PALIAS(x, y)	typeof(x) *y = &(x);
# define FOR(a, b, c)	0){;} for ((a); (b); (c)
# define FOREACH(x,y) 0){;}int __##x=0;typeof(*x)y;for(y=*x;(y=x[__##x]);__##x++
# define SHORTIFY(c)	((long)c | ((long)c << 8))
# define INTIFY(c)		(SHORTIFY(c) | (SHORTIFY(c) << 16))
# define LONGIFY(c)		(INTIFY(c) | (INTIFY(c) << 32))
# define GOTO(x)		goto x
# define TER(x, y, z)	((x) ? (y) : (z))

/*
**	Static linked lists:
*/

# define PUSH_FRONT(x, y)	(t_list[]){{x,y}}
# define PUSH_AFTER(x, y)	((!y) ? (0) : (y->next = (t_list[]){{x, y->next}}))
# define PUSH_BACK(x, y)	TO_END(y);PUSH_AFTER(x, t)
# define TO_END(y)			t = y;while((t=t->next)->next);
# define POP_FRONT(y)		y = y->next;
# define POP_BACK(y)		t=y;while((y=y->next)->next->next);y->next=NULL;y=t

/*
**	Non-typed linked lists:
*/

typedef struct	s_list_links
{
	void	*next;
	void	*prev;
}				t_list_links;

typedef struct	s_untyped_list
{
	t_list_links	*base;
	t_list_links	*current;
	t_list_links	*end;
	bool			pointer;
	long			__:48;
}				t_untyped_list;

# define VOID				void
# define LAST_ADDR(x)		(*((VOID *[1]){0} + x))
# define LIST				t_untyped_list *
# define LIST_INIT(l,p)		l = malloc(sizeof(t_untyped_list));l->base = NULL; l->current = NULL; l->end = NULL; l->pointer = p;
# define LIST_NEXT_(ptr)	*((unsigned long *)ptr)
# define LIST_PREV_(ptr)	*((unsigned long *)ptr + 1)
# define LIST_NEW(l, e)		*((VOID *[1]){malloc(sizeof(t_list_links) + sizeof(e))}); \
	LIST_NEXT_(LAST_ADDR(1)) = 0llu; \
	LIST_PREV_(LAST_ADDR(2)) = 0llu; \
	memcpy(LAST_ADDR(3) + sizeof(t_list_links), (l->pointer) ? (VOID *)e : (VOID *)(typeof(e)[]){e}, sizeof(e)); \

# define LIST_PUSH_BACK(l,e)if (!l->base) { l->base = LIST_NEW(l, e); l->end = l->base; l->current = l->base; } else { l->end->next = LIST_NEW(l, e); l->end = l->end->next; l->current = l->end; }
# define LIST_PUSH_FRONT(l,e)if (!l->base) { l->base = LIST_NEW(l, e); l->end = l->base; l->current = l->base; } else { l->base->prev = LIST_NEW(l, e); l->base = l->base->prev; l->current = l->base; }

# define LIST_DATA_(node) ((VOID *)node + sizeof(t_list_links))
# define LIST_GET_(l, type, node) (l->pointer) ? (type)LIST_DATA_(node) : *(type *)LIST_DATA_(node)

# define LIST_CURRENT(l, type) LIST_GET_(l, type, l->current)

# define LIST_AT(l, i)

# define LIST_CONTENT(elem, type) ((type)((VOID *)elem + sizeof(t_list_links))) 
# define LIST_FOREACH(l, type, as) type __##as; t_untyped_list *__iter##as; \
	for ((__##as = LIST_CONTENT(l->base, type), __iter##as = l->begin); \
			__iter##as; (__##as = LIST_CONTENT(__iter##as, type), __iter##as = __iter##as->next))

# define LIST_SWAP(l, i, j)

# define LIST_SORT(l, cmp)

# define LIST_INSERT_AT(l, e)

# define LIST_INSERT_SORT(l, e, cmp)

# define LIST_FIND(l, pred)

# define LIST_FIRST(l, type) LIST_GET_(l, type, l->base)

# define LIST_LAST(l, type) LIST_GET_(l, type, l->end)

#endif
