#include "ft_nm.h"

/*
** Print on stdout a single symbol using its offset, symbol character and name
*/
void		print_32symbol(Elf32_Off offset, char symbol, char *string) {
	if (!offset)
		printf("%08c %c %s\n", ' ', symbol, string);
	else
		printf("%08lx %c %s\n", offset, symbol, string);
}

/*
** Get the name of a symbol_table by looking at '.strtab' section
**
** If such section does not exists or an offset is too far from elf structure
** it returns NULL
*/
char		*get_32symbol_name(void *addr, size_t size, Elf32_Sym *symbol_table) {
	Elf32_Shdr	*symstr;

	symstr = get_32section_by_name(addr, size, ".strtab");
	if (!symstr)
		return (NULL);
	if (symstr->sh_offset + symbol_table->st_name > size)
		return (NULL);
	return (addr + symstr->sh_offset + symbol_table->st_name);
}

/*
** Get the section name of a symbol_table by looking at '.shstrtab' section
**
** If such section does not exists or an offset is too far from elf structure
** it returns NULL
*/
char		*get_32symbol_section_name(void *addr, size_t size, Elf32_Sym *symbol_table) {
	Elf32_Shdr		*shstr, *section;

	shstr = get_32section_by_name(addr, size, ".shstrtab");
	if (!shstr)
		return (NULL);
	section = get_32section_by_index(addr, size, symbol_table->st_shndx);
	if (!section)
		return (NULL);
	if (shstr->sh_offset + section->sh_name > size)
		return (NULL);
	return (addr + shstr->sh_offset + section->sh_name);
}

/*
** Get the symbol character representing the symbol_table using its section
*/
char		get_32symbol_type(Elf32_Sym *symbol_table, Elf32_Shdr *section) {
	char			symbol;

	symbol = '?';
	if (ELF32_ST_BIND(symbol_table->st_info) == STB_GNU_UNIQUE)
		symbol = 'u';
	else if (ELF32_ST_BIND(symbol_table->st_info) == STB_WEAK) {
		symbol = 'W';
		if (symbol_table->st_shndx == SHN_UNDEF)
			symbol = 'w';
	} else if (ELF32_ST_BIND(symbol_table->st_info) == STB_WEAK && ELF32_ST_TYPE(symbol_table->st_info) == STT_OBJECT) {
		symbol = 'V';
		if (symbol_table->st_shndx == SHN_UNDEF)
			symbol = 'v';
	} else if (symbol_table->st_shndx == SHN_UNDEF)
		symbol = 'U';
	else if (symbol_table->st_shndx == SHN_ABS)
		symbol = 'A';
	else if (symbol_table->st_shndx == SHN_COMMON)
		symbol = 'C';
	else if (section->sh_type == SHT_NOBITS
			&& section->sh_flags == (SHF_ALLOC | SHF_WRITE))
		symbol = 'B';
	else if (section->sh_type == SHT_DYNAMIC)
		symbol = 'D';
	else if (section->sh_flags == SHF_ALLOC)
		symbol = 'R';
	else if (section->sh_flags == (SHF_ALLOC | SHF_WRITE))
		symbol = 'D';
	else if (section->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		symbol = 'T';
	if (ELF32_ST_BIND(symbol_table->st_info) == STB_LOCAL && symbol != '?')
		symbol += 'a' - 'A';
	return (symbol);
}
