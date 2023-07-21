#ifndef ERROR_H
# define ERROR_H

enum errors {
	ERR_SUCCESS	= 0,
	ERR_ERRNO	= 1,
	ERR_NOFILE	= 2,
};

void		error(char *prg_name, char *file, int errcode);

#endif
