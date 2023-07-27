#include "ft_nm.h"

/*
** Get EI_CLASS from ElfN_Ehdr, supposed header size has been verified using
** check_header()
*/
uint8_t		ei_class(Elf32_Ehdr *header) {
	return (header->e_ident[EI_CLASS]);
}

Elf64_Shdr	*get_64section_by_index(
	void	*addr,
	size_t	size,
	size_t	index
) {
	Elf64_Ehdr *header;

	header = addr;
	if (index >= header->e_shnum)
		return (NULL);
	if (header->e_shoff + header->e_shentsize * index > size)
		return (NULL);
	return ((void *)(addr + header->e_shoff + header->e_shentsize * index));
}

int			get_next_idx_64section_by_type(
	void		*addr,
	size_t		size,
	size_t		from,
	Elf64_Word	type
) {
	Elf64_Ehdr *header;
	Elf64_Shdr *section;

	header= addr;
	for (size_t i = from; i < header->e_shnum; i++) {
		if (header->e_shoff + header->e_shentsize * i > size)
			return (-1);
		section = (void *)(addr + header->e_shoff + header->e_shentsize * i);
		if (section->sh_type == type)
			return (i);
	}
	return (-1);
}

Elf64_Shdr	*get_64section_by_name(
	void	*addr,
	size_t	size,
	char	*name
) {
	Elf64_Ehdr	*header;
	Elf64_Shdr	*shstr, *section;

	header = addr;
	shstr = get_64section_by_index(addr, size, header->e_shstrndx);
	for (size_t i = 0; i < header->e_shnum; i++) {
		if (header->e_shoff + header->e_shentsize * i > size)
			return (NULL);
		section = (void *)(addr + header->e_shoff + header->e_shentsize * i);
		if (shstr->sh_offset + section->sh_name < size &&
			!strncmp(addr + shstr->sh_offset + section->sh_name, name, strlen(name) + 1))
			return (section);
	}
	return (NULL);
}

int			handle_elf32(void *addr, size_t size) {
	Elf32_Ehdr *header;

	header = addr;
	(void)header;
	(void)size;
	return (0);
}

int			handle_elf64(void *addr, size_t size) {
	int			index;
	Elf64_Shdr	*section, *tmp, *symstr;
	Elf64_Sym	*symbol_table;

	symstr = get_64section_by_name(addr, size, ".strtab");
	if (symstr == NULL)
		return (-1);
	index = get_next_idx_64section_by_type(addr, size, 0, SHT_SYMTAB);
	while (index >= 0) {
		section = get_64section_by_index(addr, size, index);
		if (!section)
			return (-1);
		size_t offset_symbol = 0;
		while (offset_symbol < section->sh_size) {
			symbol_table = addr + section->sh_offset + offset_symbol;
			if (symbol_table->st_name != 0) {
				tmp = get_64section_by_index(addr, size, symbol_table->st_shndx);
				if (tmp != NULL) {
					char *string = addr + symstr->sh_offset + symbol_table->st_name;
					printf("%d - %s\n", tmp->sh_type, string);
				}
			}
			offset_symbol += section->sh_entsize;
		}
		index = get_next_idx_64section_by_type(addr, size, index + 1, SHT_SYMTAB);
	}
	return (0);
}

int			process_file(char *filename) {
	int				ret;
	int				fd;
	struct stat		statbuf;
	uint8_t			arch_size;
	void			*addr;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		// No such file or directory
		if (errno == ENOENT) {
			errno = 0;
			// Returns custom error 'No such file'
			return (-ERR_NOFILE);
		}
		return (-ERR_ERRNO);
	}
	if (fstat(fd, &statbuf) == -1)
		return (-ERR_ERRNO);
	if S_ISDIR(statbuf.st_mode) {
		// Setting up errno 'Is a directory'
		errno = EISDIR;
		return (-ERR_ERRNO);
	}
	addr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		return (-ERR_ERRNO);
	// Check header MAGIC & EI_CLASS and sections/programs offset in header
	ret = check_elf_header(addr, statbuf.st_size);
	if (ret < 0) {
		munmap(addr, statbuf.st_size);
		return (ret);
	}
	arch_size = ei_class(addr);
	// Handle ELF 64 or 32bits
	if (arch_size == ELFCLASS32)
		ret = handle_elf32(addr, statbuf.st_size);
	else if (arch_size == ELFCLASS64)
		ret = handle_elf64(addr, statbuf.st_size);
	if (ret < 0) {
		munmap(addr, statbuf.st_size);
		return (ret);
	}
	if (munmap(addr, statbuf.st_size) == -1)
		return (-ERR_ERRNO);
	return (0);
}

int			main(int argc, char *argv[]) {
	int	ret;
	int result;

	result = 0;
	if (argc < 2) {
		ret = process_file("a.out");
		if (ret < 0) {
			error(argv[0], "a.out", -ret);
			// Returns 1 on any error
			result = 1;
		}
	}
	for (int i = 1; i < argc; i++) {
		ret = process_file(argv[i]);
		if (ret < 0) {
			error(argv[0], argv[i], -ret);
			// Returns 1 on any error
			result = 1;
		}
	}
	return (result);
}
