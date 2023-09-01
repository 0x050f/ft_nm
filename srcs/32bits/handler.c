#include "ft_nm.h"

int			handle_elf32(void *addr, size_t size) {
	int			index;
	char		*symbol_name;
	Elf32_Shdr	*section, *tmp;
	Elf32_Sym	*symbol_table;
	t_lst		*symbols;

	symbols = NULL;
	index = get_next_idx_32section_by_type(addr, size, 0, SHT_SYMTAB);
	while (index >= 0) {
		section = get_32section_by_index(addr, size, index);
		if (!section)
			return (-1);
		size_t offset_symbol = 0;
		while (offset_symbol < section->sh_size) {
			symbol_table = addr + section->sh_offset + offset_symbol;
			if (symbol_table->st_name != 0) {
				tmp = get_32section_by_index(addr, size, symbol_table->st_shndx);
				symbol_name = get_32symbol_name(addr, size, symbol_table);
				if (tmp && symbol_name) {
					t_symbol32 *symbol = get_symbol32(
						symbol_table->st_value,
						get_32symbol_type(symbol_table, tmp),
						symbol_name
					);
					if (!symbol) {}
						// TODO
					if (!lst_add(&symbols, symbol)) {}
						// TODO
				}
			}
			offset_symbol += section->sh_entsize;
		}
		index = get_next_idx_32section_by_type(addr, size, index + 1, SHT_SYMTAB);
	}
	lst_apply(symbols, print_symbol32);
	lst_drop(&symbols, true);
	return (0);
}
