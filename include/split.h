#ifndef _SPLIT_H_
#define _SPLIT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


typedef struct SPLIT
{
	char *body;
	char *all_body;
	struct SPLIT *next;
} SPLIT;

void SplitShow(SPLIT *head);
void SplitFree(SPLIT *head);
int SplitArrayLen(SPLIT *head);
int Split(char *body, char *trigger, SPLIT *resolve);
void *SplitArrayAt(SPLIT *head, int at_len);

#endif
