/* $Id$ */

#define HASH_THRES 20

struct act {
	void (*a_init)(int, const char **);
	void (*a_cleanup)(void);
	int  (*a_eq)(int, int, const char **);
};

void hash_init(int, const char **);
void hash_cleanup(void);
int  hash_eq(int, int, const char **);

void mem_init(int, const char **);
void mem_cleanup(void);
int  mem_eq(int, int, const char **);
