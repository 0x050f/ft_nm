#include "ft_nm.h"

t_lst	*lst_new(void *elem) {
	t_lst	*node;

	node = malloc(sizeof(t_lst));
	if (!node)
		return (NULL);
	node->elem = elem;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

t_lst	*lst_prepend(t_lst **lst, void *elem) {
	t_lst	*node;

	node = lst_new(elem);
	if (!node)
		return (NULL);
	if (!*lst) {
		*lst = node;
	} else {
		(*lst)->prev = node;
		node->next = *lst;
		*lst = node;
	}
	return (node);
}

t_lst	*lst_append(t_lst **lst, void *elem) {
	t_lst	*tmp;
	t_lst	*node;

	node = lst_new(elem);
	if (!node)
		return (NULL);
	if (!*lst) {
		*lst = node;
	} else {
		tmp = *lst;
		while (tmp->next) {
			tmp = tmp->next;
		}
		node->prev = tmp;
		tmp->next = node;
	}
	return (node);
}

void	*lst_del(t_lst **lst, t_lst *node) {
	void	*elem;

	elem = node->elem;
	if (!*lst) {
		free(node);
		return (elem);
	}
	if (node->prev)
		node->prev->next = node->next;
	else
		*lst = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node);
	return (elem);
}

void	lst_drop(t_lst **lst, bool drop_elem) {
	t_lst	*next;
	t_lst	*tmp;

	tmp = *lst;
	while (tmp) {
		next = tmp->next;
		if (drop_elem)
			free(tmp->elem);
		free(tmp);
		tmp = next;
	}
	*lst = NULL;
}

void	lst_apply(t_lst *lst, void (*fn)(void *)) {
	t_lst	*tmp;

	tmp = lst;
	while (tmp) {
		fn(tmp->elem);
		tmp = tmp->next;
	}
}

size_t	lst_len(t_lst *lst) {
	size_t	len;
	t_lst	*tmp;

	len = 0;
	tmp = lst;
	while (tmp) {
		len += 1;
		tmp = tmp->next;
	}
	return (len);
}

void	lst_swap(t_lst *node1, t_lst *node2) {
	t_lst *tmp;

	tmp = node1->next;
	node1->next = node2->next;
	node2->next = tmp;
	if (node1->next)
		node1->next->prev = node1;
	if (node2->next)
		node2->next->prev = node2;
	tmp = node1->prev;
	node1->prev = node2->prev;
	node2->prev = tmp;
	if (node1->prev)
		node1->prev->next = node1;
	if (node2->prev)
		node2->prev->next = node2;
}

/*
** Sort a list with a comparaison function using bubble sort algorithm
**
** The comparaison function is comparing two elements
*/
void	lst_sort(t_lst **lst, int (*cmp)(void *, void *)) {
	t_lst	*tmp;
	t_lst	*tmp2;
	t_lst	*min;

	if (!*lst)
		return ;
	tmp = *lst;
	while (tmp) {
		min = tmp;
		tmp2 = tmp->next;
		while (tmp2) {
			if (cmp(min->elem, tmp2->elem) < 0)
				min = tmp2;
			tmp2 = tmp2->next;
		}
		if (min != tmp) {
			lst_swap(min, tmp);
			if (tmp == *lst)
				*lst = min;
		}
		tmp = min->next;
	}
}
