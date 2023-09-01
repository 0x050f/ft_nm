#include "ft_nm.h"

int			handle_elf64(void *addr, size_t size) {
	int			index;
	char		*symbol_name;
	Elf64_Shdr	*section, *tmp;
	Elf64_Sym	*symbol_table;

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
				symbol_name = get_64symbol_name(addr, size, symbol_table);
				if (tmp && symbol_name) {
					print_64symbol(
						symbol_table->st_value,
						get_64symbol_type(symbol_table, tmp),
						symbol_name
					);
				}
			}
			offset_symbol += section->sh_entsize;
		}
		index = get_next_idx_64section_by_type(addr, size, index + 1, SHT_SYMTAB);
	}
	return (0);
}