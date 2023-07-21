#include "ft_nm.h"

int			check_file(void *addr) {
	uint8_t magic[4] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	if (memcmp(addr, magic, sizeof(magic)))
		return (-ERR_FORMAT);
	return (0);
}

int			process_file(char *filename) {
	int			ret;
	int			fd;
	struct stat	statbuf;
	void		*addr;

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
	ret = check_file(addr);
	if (ret < 0)
		return (ret);
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
