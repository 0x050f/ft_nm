#include "ft_nm.h"

/*
** Get EI_CLASS from ElfN_Ehdr, supposed header size has been verified using
** check_header()
*/
uint8_t		ei_class(Elf32_Ehdr *header) {
	return (header->e_ident[EI_CLASS]);
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
