#include "ft_nm.h"

static const char *error_table[] = {
	[ERR_SUCCESS]		= "Success",
	[ERR_ERRNO]			= "Errno is set to indicate the error",
	[ERR_NOFILE]		= "No such file",
	[ERR_FORMAT]		= "file format not recognized"
};

void		error(char *prg_name, char *file, int errcode) {
	if (errcode == ERR_ERRNO) {
		fprintf(stderr, "%s: '%s': %s\n", prg_name, file, strerror(errno));
	} else if (errcode != ERR_SUCCESS &&
		(size_t)errcode < sizeof(error_table) / sizeof(error_table[0])) {
		fprintf(stderr, "%s: '%s': %s\n", prg_name, file, error_table[errcode]);
	} else if (errcode != ERR_SUCCESS) {
		fprintf(stderr, "%s: '%s': %s\n", prg_name, file, "Unknown error");
	}
}
