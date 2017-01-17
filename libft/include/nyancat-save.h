/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nyancat.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 23:52:18 by alelievr          #+#    #+#             */
/*   Updated: 2016/11/15 22:43:31 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NYANCAT_H
# define NYANCAT_H

/*
**	Utils:
*/

# define ALIAS(x, y)	typeof(x) y = x;
# define FOR(a, b, c)	0){;} for ((a); (b); (c)
# define FOREACH(x,y) 0){;}int __##x=0;typeof(*x)y;for(y=*x;(y=x[__##x]);__##x++
# define SHORTIFY(c)	((long)c | ((long)c << 8))
# define INTIFY(c)		(SHORTIFY(c) | (SHORTIFY(c) << 16))
# define LONGIFY(c)		(INTIFY(c) | (INTIFY(c) << 32))

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
}				t_untyped_list;

# define LAST_ADDR(x)		(*((void *[1]){0} + x))
# define LIST				t_untyped_list *
# define LIST_INIT(l)		l = malloc(sizeof(t_untyped_list)); l->base = NULL; l->current = NULL; l->end = NULL;
# define LIST_NEXT_(ptr)	*((unsigned long *)ptr)
# define LIST_PREV_(ptr)	*((unsigned long *)ptr + 1)
# define LIST_NEW(e)		*((void *[1]){malloc(sizeof(t_list_links) + sizeof(e))}); \
	printf("addr: %p\n", LAST_ADDR(1));\
	((char *)LAST_ADDR(2))[0] = '0';\
	LIST_NEXT_(LAST_ADDR(3)) = 0llu; \
	LIST_PREV_(LAST_ADDR(4)) = 0llu; \
	memcpy(LAST_ADDR(5) + sizeof(t_list_links), e, sizeof(e)); \
	printf("%p\n", LAST_ADDR(6)); \

# define LIST_PUSH_BACK(l,e)if (!l->base) { l->base = LIST_NEW(e); printf("data: %s\n", l->base + sizeof(t_list_links)); l->end = l->base; l->current = l->base; } else { l->end->next = LIST_NEW(e); l->end = l->end->next; l->current = l->end; }
# define LIST_PUSH_FRONT(l,e)if (!l->base) { l->base = LIST_NEW(e); l->end = l->base; l->current = l->base; }

# define LIST_GET(l, type) (type)(l->current + sizeof(t_list_links))

#endif
