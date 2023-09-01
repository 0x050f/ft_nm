#include "ft_nm.h"

t_lst	*lst_new(void *elem) {
	t_lst	*node;

	node = malloc(sizeof(t_lst));
	if (!node)
		return (NULL);
	node->elem = elem;
	node->next = NULL;
	return node;
}

t_lst	*lst_add(t_lst **lst, void *elem) {
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
		tmp->next = node;
	}
	return (node);
}

void	*lst_del(t_lst **lst, t_lst *node) {
	void	*elem;
	t_lst	*prev;
	t_lst	*tmp;

	elem = node->elem;
	if (!*lst) {
		free(node);
		return (elem);
	}
	prev = NULL;
	tmp = *lst;
	while (tmp && tmp != node) {
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return (NULL);
	else if (!prev)
		*lst = node->next;
	else
		prev->next = node->next;
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
