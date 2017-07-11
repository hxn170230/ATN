#ifndef __COMBINATIONS_H
#define __COMBINATIONS_H

typedef struct list {
	char *c;
	int len;
	struct list *next;
}llist_st;

extern llist_st *getCombinations(int start, int end, int k);
extern void free_list(llist_st *);
extern void print(llist_st *);
extern llist_st *reverseList(llist_st *);
#endif
