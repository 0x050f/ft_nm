#ifndef FT_NM_H
# define FT_NM_H

# include <fcntl.h>
# include <elf.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/stat.h>

# include "error.h"

/* check.c */
int			check_elf_header(void *addr, size_t size);

/* 32bits/handler.c */
int			handle_elf32(void *addr, size_t size);

/* 32bits/section.c */
Elf32_Shdr	*get_32section_by_index(void *addr, size_t size, size_t index);
Elf32_Shdr	*get_32section_by_name(void *addr, size_t size, char *name);
int			get_next_idx_32section_by_type(void *addr, size_t size, size_t from, Elf32_Word type);

/* 32bits/symbol.c */
void		print_32symbol(Elf32_Off offset, char symbol, char *string);
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
void		print_64symbol(Elf64_Off offset, char symbol, char *string);
char		*get_64symbol_name(void *addr, size_t size, Elf64_Sym *symbol_table);
char		*get_64symbol_section_name(void *addr, size_t size, Elf64_Sym *symbol_table);
char		get_64symbol_type(Elf64_Sym *symbol_table, Elf64_Shdr *section);

#endif
