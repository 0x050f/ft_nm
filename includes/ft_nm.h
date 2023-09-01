#ifndef FT_NM_H
# define FT_NM_H

# include <fcntl.h>
# include <elf.h>
# include <errno.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/stat.h>

# include "error.h"
# include "utils.h"

typedef struct	s_symbol32 {
	Elf32_Off	offset;
	char		symbol;
	char		*name;
}				t_symbol32;

typedef struct	s_symbol64 {
	Elf64_Off	offset;
	char		symbol;
	char		*name;
}				t_symbol64;

typedef struct		s_lst {
	void			*elem;
	struct s_lst	*prev;
	struct s_lst	*next;
}					t_lst;

/* check.c */
int			check_elf_header(void *addr, size_t size);

/* lst.c */
t_lst	*lst_new(void *elem);
t_lst	*lst_append(t_lst **lst, void *elem);
t_lst	*lst_prepend(t_lst **lst, void *elem);
void	*lst_del(t_lst **lst, t_lst *node);
void	lst_drop(t_lst **lst, bool drop_elem);
void	lst_apply(t_lst *lst, void (*fn)(void *));
size_t	lst_len(t_lst *lst);
void	lst_sort(t_lst **lst, int (*cmp)(void *, void *));

/* 32bits/handler.c */
int			handle_elf32(void *addr, size_t size);

/* 32bits/section.c */
Elf32_Shdr	*get_32section_by_index(void *addr, size_t size, size_t index);
Elf32_Shdr	*get_32section_by_name(void *addr, size_t size, char *name);
int			get_next_idx_32section_by_type(void *addr, size_t size, size_t from, Elf32_Word type);

/* 32bits/symbol.c */
t_symbol32	*get_symbol32(Elf32_Off offset, char symbol, char *name);
int			cmp_symbol32(void *elem1, void *elem2);
void		print_symbol32(void *elem);
char		*get_32symbol_name(void *addr, size_t size, Elf32_Sym *symbol_table);
char		*get_32symbol_section_name(void *addr, size_t size, Elf32_Sym *symbol_table);
char		get_32symbol_type(Elf32_Sym *symbol_table, Elf32_Shdr *section);

/* 64bits/handler.c */
int			handle_elf64(void *addr, size_t size);

/* 64bits/section.c */
Elf64_Shdr	*get_64section_by_index(void *addr, size_t size, size_t index);
Elf64_Shdr	*get_64section_by_name(void *addr, size_t size, char *name);
int			get_next_idx_64section_by_type(void *addr, size_t size, size_t from, Elf64_Word type);

/* 64bits/symbol.c */
t_symbol64	*get_symbol64(Elf64_Off offset, char symbol, char *name);
int			cmp_symbol64(void *elem1, void *elem2);
void		print_symbol64(void *elem);
char		*get_64symbol_name(void *addr, size_t size, Elf64_Sym *symbol_table);
char		*get_64symbol_section_name(void *addr, size_t size, Elf64_Sym *symbol_table);
char		get_64symbol_type(Elf64_Sym *symbol_table, Elf64_Shdr *section);

#endif
