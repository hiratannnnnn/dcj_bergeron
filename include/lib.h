#ifndef LIB_H
# define LIB_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <limits.h>
# include <time.h>

# include "get_next_line.h"

extern	size_t		mem;
extern	size_t		mem_peak;
extern	clock_t 	proc_start;
extern	clock_t		proc_end;

// lib_free.c
void	free_array_int(int *ptr, int const n);

// lib_memory.c
void	*xmalloc(size_t size);
void	*xcalloc(int n, size_t size_each);
void	xfree(void *ptr, size_t size);

// lib_perm.c
int		*gen_perm(int start, int end, int *n);
int 	*gen_unsigned_perm(int n);
int		*gen_signed_perm(int n);
int 	*gen_id_perm(int n);
void 	shuffle_perm(int *perm, int n);

// lib_print.c
void 	print_array_int(int *arr, int n, int max_width);
void	print_info(void);

#endif
