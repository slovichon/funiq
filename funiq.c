/* $Id$ */
/*
 *	ncmp = (n * (n - 1)) / 2
 *
 *	+---+---------+
 *	|n=4| a b c d |
 *	+---+---------+
 *	| a | x 1 2 3
 *	| b | x x 4 5
 *	| c | x x x 6
 *	| d | x x x x
 *	+---+
 *
 *	i | j
 *	--+--
 *	0 | 1
 *	0 | 2
 *	0 | 3
 *	1 | 2
 *	1 | 3
 *	2 | 3
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "funiq.h"

static int prdups = 0;
static int pruniq = 0;

#define TF_DUP (1<<0)

static __dead void usage(void);

int
main(int argc, char *argv[])
{
	int c, i, j;
	int *tab;
	struct act acts[] = {
		{ hash_init, hash_cleanup, hash_eq },
		{ mem_init, mem_cleanup, mem_eq }
	}, *pact;

	while ((c = getopt(argc, argv, "du")) != -1) {
		switch (c) {
		case 'd':
			prdups = 1;
			break;
		case 'u':
			pruniq = 1;
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	if (prdups && pruniq) {
		warnx("-d and -u cannot be used together");
		usage();
	}
	argc -= optind;
	argv += optind;

	if ((tab = calloc(argc, sizeof(*tab))) == NULL)
		err(EX_OSERR, "malloc");

	if (argc > HASH_THRES)
		pact = &acts[0];
	else
		pact = &acts[1];

	pact->a_init(argc, argv);
	for (i = 0; i < argc - 1; i++)
		for (j = i + 1; j < argc; j++)
			if (pact->a_eq(i, j, argv)) {
				tab[i] |= TF_DUP;
				tab[j] |= TF_DUP;
			}
	pact->a_cleanup();

	if (prdups) {
		/* Only print duplicates. */
		for (i = 0; i < argc; i++)
			if (tab[i])
				(void)printf("%s", argv[i]);
	} else if (pruniq) {
		/* Only print unique files. */
		for (i = 0; i < argc; i++)
			if ((tab[i] & TF_DUP) == 0)
				(void)printf("%s", argv[i]);
	} else {
		/* Print a list of unique files. */
		for (i = 0; i < argc; i++)
			if (tab[i])
				(void)printf("%s", argv[i]);
	}
	free(tab);
	exit(EXIT_SUCCESS);
}

static __dead void
usage(void)
{
	extern char *__progname;

	(void)fprintf(stderr, "usage: %s [-du] file ...\n", __progname);
	exit(EXIT_SUCCESS);
}
