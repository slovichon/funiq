/* $Id$ */

#include "funiq.h"

static u_int64_t hashfile(char *);

static u_int64_t *hashtab;

void
hash_init(int n, char **argv)
{
	int i, j;

	if ((hashtab = malloc(n * sizeof(*hashtab))) == NULL)
		err(EX_OSERR, "malloc");
	for (i = 0; i < n; i++)
		hashtab[i] = hashfile(*argv++);
}

__inline int
hash_eq(int a, int b, char **argv)
{
	return (hashtab[a] == hashtab[b])
}

void
hash_cleanup(void)
{
	free(hashtab);
}

static u_int64_t
hashfile(char *s)
{
	struct stat st;
	u_int64_t hash;
	off_t i;
	void *p;
	int fd;

	if ((fd = open(s, O_RDONLY)) == NULL)
		err(EX_NOINPUT, "open %s", s);
	if (fstat(fd, &st) == -1)
		err(EX_OSERR, "stat %s", s);
	if ((p = mmap(NULL, st.st_size, PROT_READ, MAP_FILE, fd,
	     0)) == NULL)
		err(EX_OSERR, "mmap %s", s);
	for (hash = 0, i = 0; i < st.st_size; i++)
		hash = (((hash * UINT_MAX) + p[i]) % UINT64_MAX) ^ hash;
	(void)munmap(p);
	(void)close(fd);

	return (hash);
}
