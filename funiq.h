/* $Id$ */

#define HASH_THRES 20

struct act {
	void (*a_init)(int, char **);
	void (*a_cleanup)(void);
	int  (*a_eq)(int, int, char **);
};

void hash_init(int, char **);
void hash_cleanup(void);
int  hash_eq(int, int, char **);

void mem_init(int, char **);
void mem_cleanup(void);
int  mem_eq(int, int, char **);
