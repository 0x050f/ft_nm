#include "ft_nm.h"/*
** Get allocated symbol structure from offset, symbol and name
**
** Returns NULL when malloc fail
*/
t_symbol64	*get_symbol64(Elf64_Off offset, char symbol, char *name, uint64_t size) {
	t_symbol64	*sym;

	sym = malloc(sizeof(t_symbol64));
	if (!sym)
		return (NULL);
	sym->offset = offset;
	sym->symbol = symbol;
	sym->name = name;
	sym->size = size;
	return (sym);
}
/*
** Compare 64bytes symbols on LC_ALL=C
*/
int			cmp_symbol64(void *elem1, void *elem2) {
	char *name1;
	char *name2;

	name1 = ((t_symbol64 *)elem1)->name;
	name2 = ((t_symbol64 *)elem2)->name;
	if (!ft_strcmp(name2, name1))
		return ((t_symbol64 *)elem2)->offset - ((t_symbol64 *)elem1)->offset;
	return (ft_strcmp(name2, name1));
}

/*
** Print on stdout a single symbol using its offset, symbol character and name
*/
void		print_symbol64(void *elem) {
	t_symbol64	*sym;

	sym = elem;
	if ((!sym->offset && !sym->size) || sym->symbol == 'U' || sym->symbol == 'w')
		printf("%016c %c %s\n", ' ', sym->symbol, sym->name);
	else
		printf("%016llx %c %s\n", sym->offset, sym->symbol, sym->name);
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
** Get the symbol character representing the symbol_table using its section
*/
char		get_64symbol_type(Elf64_Sym *symbol_table, Elf64_Shdr *section) {
	char			symbol;

	symbol = '?';
	if (ELF64_ST_BIND(symbol_table->st_info) == STB_GNU_UNIQUE)
		symbol = 'u';
	else if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK && ELF64_ST_TYPE(symbol_table->st_info) == STT_OBJECT) {
		symbol = 'V';
		if (symbol_table->st_shndx == SHN_UNDEF)
			symbol = 'v';
	} else if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK) {
		symbol = 'W';
		if (symbol_table->st_shndx == SHN_UNDEF)
			symbol = 'w';
	} else if (symbol_table->st_shndx == SHN_UNDEF)
		symbol = 'U';
	else if (symbol_table->st_shndx == SHN_ABS)
		symbol = 'A';
	else if (symbol_table->st_shndx == SHN_COMMON)
		symbol = 'C';
	else if (section->sh_type == SHT_NOBITS
			&& section->sh_flags & SHF_ALLOC && section->sh_flags & SHF_WRITE)
		symbol = 'B';
	else if (section->sh_type == SHT_DYNAMIC)
		symbol = 'D';
	else if (section->sh_flags == SHF_ALLOC)
		symbol = 'R';
	else if (section->sh_flags == (SHF_ALLOC | SHF_WRITE))
		symbol = 'D';
	else if (section->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		symbol = 'T';
	if (ELF64_ST_BIND(symbol_table->st_info) == STB_LOCAL && symbol != '?')
		symbol += 'a' - 'A';
	return (symbol);
}
