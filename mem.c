/* $Id$ */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "funiq.h"

void
mem_init(int argc, char **argv)
{
}

void
mem_cleanup(void)
{
}

int
mem_eq(int a, int b, char **argv)
{
	struct stat sta, stb;
	char *fna, *fnb;
	void *pa, *pb;
	int fda, fdb;
	off_t siz;
	int same;

	fna = argv[a];
	fnb = argv[b];

	/* Test 1: check file sizes. */
	if (stat(fna, &sta) == -1)
		err(EX_NOINPUT, "stat %s", fna);
	if (stat(fnb, &stb) == -1)
		err(EX_NOINPUT, "stat %s", fnb);
	if (sta.st_size != stb.st_size)
		return (0);

	siz = sta.st_size;

	/* Test 2: compare file contents. */
	if ((fda = open(fna, O_RDONLY)) == -1)
		err(EX_NOINPUT, "open %s", fna);
	if ((pa = mmap(NULL, siz, PROT_READ, MAP_FILE, fda,
	     0)) == NULL)
		err(EX_OSERR, "mmap %s", fna);

	if ((fdb = open(fnb, O_RDONLY)) == -1)
		err(EX_NOINPUT, "open %s", fnb);
	if ((pb = mmap(NULL, siz, PROT_READ, MAP_FILE, fdb,
	     0)) == NULL)
		err(EX_OSERR, "mmap %s", fnb);

	same = memcmp(pa, pb, siz);

	(void)munmap(pa, siz);
	(void)munmap(pb, siz);
	(void)close(fda);
	(void)close(fdb);

	return (same == 0);
}
