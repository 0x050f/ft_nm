#include "ft_nm.h"

/*
** Get a Elf64_Shdr by index from an elf structure
**
** The function checks for number of section and size of the whole structure to
** avoid reading to far, returning NULL in that case
*/
Elf64_Shdr	*get_64section_by_index(void *addr, size_t size, size_t index) {
	Elf64_Ehdr *header;

	header = addr;
	if (index >= header->e_shnum)
		return (NULL);
	if (header->e_shoff + header->e_shentsize * index > size)
		return (NULL);
	return ((void *)(addr + header->e_shoff + header->e_shentsize * index));
}

/*
** Get a Elf64_Shdr by a string name from an elf structure
**
** The function checks for number of section and size of the whole structure to
** avoid reading to far, returning NULL in that case
*/
Elf64_Shdr	*get_64section_by_name(void *addr, size_t size, char *name) {
	Elf64_Ehdr	*header;
	Elf64_Shdr	*shstr, *section;

	header = addr;
	shstr = get_64section_by_index(addr, size, header->e_shstrndx);
	for (size_t i = 0; i < header->e_shnum; i++) {
		if (header->e_shoff + header->e_shentsize * i > size)
			return (NULL);
		section = (void *)(addr + header->e_shoff + header->e_shentsize * i);
		if (shstr->sh_offset + section->sh_name < size &&
			!ft_strncmp(addr + shstr->sh_offset + section->sh_name, name, ft_strlen(name) + 1))
			return (section);
	}
	return (NULL);
}

/*
** Get next index of a certain type of section from an elf structure
**
** The function takes in parameter the index from where to start and the type
** to search
**
** The function checks for number of section and size of the whole structure to
** avoid reading to far, returning -1 in that case
*/
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
