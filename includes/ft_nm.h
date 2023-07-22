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

#endif
