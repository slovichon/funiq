/* $Id$ */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#include "funiq.h"

static u_int64_t hashfile(const char *);

static u_int64_t *hashtab;

void
hash_init(int n, const char **argv)
{
	int i;

	if ((hashtab = calloc(n, sizeof(*hashtab))) == NULL)
		err(EX_OSERR, "calloc");
	for (i = 0; i < n; i++)
		hashtab[i] = hashfile(*argv++);
}

__inline int
hash_eq(int a, int b, const char **argv)
{
	return (hashtab[a] == hashtab[b]);
}

void
hash_cleanup(void)
{
	free(hashtab);
}

static u_int64_t
hashfile(const char *s)
{
	unsigned char *p;
	struct stat st;
	u_int64_t hash;
	off_t i;
	int fd;

	if ((fd = open(s, O_RDONLY)) == NULL)
		err(EX_NOINPUT, "open %s", s);
	if (fstat(fd, &st) == -1)
		err(EX_OSERR, "stat %s", s);
	if ((p = mmap(NULL, st.st_size, PROT_READ, MAP_FILE, fd,
	     0)) == NULL)
		err(EX_OSERR, "mmap %s", s);
	for (hash = 0, i = 0; i < st.st_size; i++)
		hash = (((hash * UINT_MAX) + p[i]) % UQUAD_MAX) ^ hash;
	(void)munmap(p, st.st_size);
	(void)close(fd);

	return (hash);
}
