#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combinations.h"

char *add(char *first, char *second) {
	int len1 = strlen(first);
	int len2 = strlen(second);

	char *c = (char *)malloc(sizeof(char)*(len1+len2+1));
	memset(c, '\0', len1+len2+1);
	strncpy(c, first, len1);
	strncpy(c+len1, second, len2);
	return c;
}

llist_st *getCombinations(int start_index, int end_index, int k) {
	if (start_index > end_index) {
		return NULL;
	}

	if (start_index == end_index) {
		if (k == 0) {
			llist_st *list = (llist_st *)malloc(sizeof(llist_st));
			list->c = "0";
			list->len = 1;
			list->next = NULL;
			return list;
		} else if (k == 1) {
			llist_st *list = (llist_st *)malloc(sizeof(llist_st));
			list->c = "1";
			list->len = 1;
			list->next = NULL;
			return list;
		} else {
			return NULL;
		}
	} else if (end_index - start_index +1 == k) {
		llist_st *list = (llist_st *)malloc(sizeof(llist_st));
		list->c = (char *)malloc(sizeof(char)*(k+1));
		int i = 0;
		for (i = 0; i < k; i++) {
			(list->c)[i] = '1';
		}
		(list->c)[k] = '\0';
		list->len = k;
		list->next = NULL;
		return list;
	} else {
		llist_st *head = NULL;
		llist_st *temp_parent = NULL;
		llist_st * list = getCombinations(start_index+1, end_index, k);
		while (list != NULL) {
			char *list_c = list->c;
			int len = list->len;
			llist_st *temp = (llist_st*)malloc(sizeof(llist_st));
			temp->c = add("0", list_c);
			temp->len  = len+1;
			temp->next = NULL;
			if (head == NULL) {
				head = temp;
			} else if (temp_parent != NULL) {
				temp_parent->next = temp;
			}
			temp_parent = temp;
			list = list->next;
		}
		list = getCombinations(start_index+1, end_index, k-1);
		while (list != NULL) {
			char *list_c = list->c;
			int len = list->len;
			llist_st *temp = (llist_st*)malloc(sizeof(llist_st));
			temp->c = add("1", list_c);
			temp->len  = len+1;
			temp->next = NULL;
			if (head == NULL) {
				head = temp;
			} else if (temp_parent != NULL) {
				temp_parent->next = temp;
			}
			temp_parent = temp;
			list = list->next;
		}
		return head;
	}
}

int get(char *c) {
	int len = strlen(c);
	int i = 0;
	int ret = 0;
	while (i < len) {
		ret = 2*ret + (c[i]-'0');	
		i++;
	}
	return ret;
}

void free_list(llist_st *list) {
	while (list != NULL) {
		llist_st *temp = list;
		list = temp->next;
		free(list);
	}
}

void print(llist_st *list) {
	while (list != NULL) {
		printf("%s, ", list->c);
		list = list->next;
	}
	printf("\n");
}

llist_st *reverseList(llist_st *list) {
	llist_st *start = list;
	llist_st *temp1 = list;
	llist_st *temp2 = list->next;
	while (temp1 && temp2) {
		llist_st *local = temp2->next;
		temp2->next = start;
		temp1->next = local;
		start = temp2;
		temp2 = local;
	}

	return start;
}
