#include "ft_nm.h"

/*
** Check elf32 headers offset and size of each program or section headers
** Also check if strndx is a valid index for number of section
*/
int			check_elf32_header(Elf32_Ehdr *header, size_t size) {
	if (header->e_phoff != 0 && // program header table's offset could be 0
		(size_t)header < (size_t)header->e_phoff &&
		(size_t)header->e_phoff > (size_t)header + size)
		return (-ERR_FORMAT);
	if (header->e_shoff != 0 && // section header table's offset could be 0
		(size_t)header < (size_t)header->e_shoff &&
		(size_t)header->e_shoff > (size_t)header + size)
		return (-ERR_FORMAT);
	// Check if any problem in size or num value of headers
	if (header->e_phentsize * header->e_phnum +
		header->e_shentsize * header->e_shnum +
		(size_t)header->e_ehsize > size) {
		return (-ERR_FORMAT);
	}
	// Check if shstrndx has a possible value
	if (header->e_shstrndx != SHN_UNDEF && header->e_shstrndx > header->e_shnum)
		return (-ERR_FORMAT);
	return (0);
}

/*
** Check elf64 headers offset and size of each program or section headers
** Also check if strndx is a valid index for number of section
*/
int			check_elf64_header(Elf64_Ehdr *header, size_t size) {
	if (header->e_phoff != 0 && // program header table's offset could be 0
		(size_t)header < (size_t)header->e_phoff &&
		(size_t)header->e_phoff > (size_t)header + size)
		return (-ERR_FORMAT);
	if (header->e_shoff != 0 && // section header table's offset could be 0
		(size_t)header < (size_t)header->e_shoff &&
		(size_t)header->e_shoff > (size_t)header + size)
		return (-ERR_FORMAT);
	// Check if any problem in size or num value of headers
	if (header->e_phentsize * header->e_phnum +
		header->e_shentsize * header->e_shnum +
		(size_t)header->e_ehsize > size) {
		return (-ERR_FORMAT);
	}
	// Check if shstrndx has a possible value
	if (header->e_shstrndx != SHN_UNDEF && header->e_shstrndx > header->e_shnum - 1)
		return (-ERR_FORMAT);
	return (0);
}

/*
** Check elf32 or 64bits magic bytes and class headers
** Also check for offset and size of section and program headers
*/
int			check_elf_header(void *addr, size_t size) {
	// Use 32bit header: EI_NIDENT + 8 first bytes are identicals in both
	// 32 or 64bits
	Elf32_Ehdr *header;

	if (size < sizeof(Elf32_Ehdr))
		return (-ERR_FORMAT);
	header = addr;
	if (header->e_ident[EI_MAG0] != ELFMAG0 ||
		header->e_ident[EI_MAG1] != ELFMAG1 ||
		header->e_ident[EI_MAG2] != ELFMAG2 ||
		header->e_ident[EI_MAG3] != ELFMAG3)
		return (-ERR_FORMAT);
	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		return (check_elf32_header(addr, size));
	else if (header->e_ident[EI_CLASS] == ELFCLASS64)
		return (check_elf64_header(addr, size));
	else // ELFCLASSNONE or ANY
		return (-ERR_FORMAT);
	return (0);
}
