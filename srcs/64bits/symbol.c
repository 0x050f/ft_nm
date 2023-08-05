#include "ft_nm.h"

/*
** Print on stdout a single symbol using its offset, symbol character and name
*/
void		print_64symbol(Elf64_Off offset, char symbol, char *string) {
	if (!offset)
		printf("%016c %c %s\n", ' ', symbol, string);
	else
		printf("%016llx %c %s\n", offset, symbol, string);
}

/*
** Get the name of a symbol_table by looking at '.strtab' section
**
** If such section does not exists or an offset is too far from elf structure
** it returns NULL
*/
char		*get_64symbol_name(void *addr, size_t size, Elf64_Sym *symbol_table) {
	Elf64_Shdr	*symstr;

	symstr = get_64section_by_name(addr, size, ".strtab");
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
char		*get_64symbol_section_name(void *addr, size_t size, Elf64_Sym *symbol_table) {
	Elf64_Shdr		*shstr, *section;

	shstr = get_64section_by_name(addr, size, ".shstrtab");
	if (!shstr)
		return (NULL);
	section = get_64section_by_index(addr, size, symbol_table->st_shndx);
	if (!section)
		return (NULL);
	if (shstr->sh_offset + section->sh_name > size)
		return (NULL);
	return (addr + shstr->sh_offset + section->sh_name);
}

/*
** Get the symbol character representing the symbol_table using its section_name
*/
char		get_64symbol_type(Elf64_Sym *symbol_table, char *section_name) {
	char			symbol;

	symbol = '?';
	if (!symbol_table->st_value)
		symbol = 'U';
	else if (!strncmp(".text", section_name, strlen(section_name) + 1) || !strncmp(".fini", section_name, strlen(section_name) + 1) || !strncmp(".init", section_name, strlen(section_name) + 1))
		symbol = 'T';
	else if (!strncmp(".bss", section_name, strlen(section_name) + 1))
		symbol = 'B';
	else if (!strncmp(".data", section_name, strlen(section_name) + 1) || !strncmp(".dynamic", section_name, strlen(section_name) + 1) || !strncmp(".got.plt", section_name, strlen(section_name) + 1))
		symbol = 'D';
	else if (!strncmp(".rodata", section_name, strlen(section_name) + 1) || !strncmp(".eh_frame_hdr", section_name, strlen(section_name) + 1))
		symbol = 'R';
	else {
		printf("%s\n", section_name);
	}
	if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK) {
		if (symbol_table->st_value)
			symbol = 'W';
		else
			symbol = 'w';
	} else if (ELF64_ST_BIND(symbol_table->st_info) != STB_GLOBAL && symbol >= 'A' && symbol <= 'Z')
		symbol += 'a' - 'A';
	return (symbol);
}
