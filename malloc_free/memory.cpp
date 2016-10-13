#include <iostream>
#include <thread>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <set>
#include <queue>

using namespace std;
#define P(i) cout<<i<<endl;
#define BLOCK_SIZE 40

struct node {
	int free;
	int padding; // memory align
	size_t size;
	node *next, *prev;
	void *ptr;
};
///*
node *first = NULL;

int cnt = 0;
node *create_block(node *prev, size_t size) {	
	node *p =(node *) sbrk(0);
	//cnt+=size;P(cnt)P(sbrk(0)) 
	if (sbrk(BLOCK_SIZE + size) == (void *) -1) exit(4);
	p -> free = 0;
	p -> size = size;
	p -> prev = prev;
	p -> next = NULL;
	p -> ptr = p + 1;
	if (prev != NULL) prev -> next = p;
	return p;
}

node *find_free(node **plas, size_t size) {
	node *p = first;
	while (p != NULL && !(p -> free && p -> size >= size)) {
		*plas = p;
		p = p -> next;
	}
	return p;
}

void split_block(node *p, size_t size) {
	if (p -> size - size > BLOCK_SIZE + 8) {
		char *s = (char *)p;
		node *ne = (node *)(s + BLOCK_SIZE + size);
		ne -> free = 1;
		ne -> prev = p;
		ne -> next = p -> next;
		ne -> size = p -> size - size - BLOCK_SIZE;
		ne -> ptr = ne + 1;
		if (p -> next) p -> next -> prev = ne;
		p -> next = ne;
	}
	p -> free = 0;
	p -> size = size;
}

size_t align_size(size_t size) {
	if (size & 0x7) size += 8 - size % 8;
	return size;
}

void *malloc(size_t size) {
	size = align_size(size);
	node *b;
	node *las = NULL;
	node *res = find_free(&las, size);
	if (res == NULL) {
		b = create_block(las, size);
	}
	else {
		b = res;
		split_block(res, size);
	}
	if (first == NULL) first = b;
	if (b == NULL) exit(1);
	return b -> ptr;
}

void merge_block(node *p) {
	if (p -> free && p -> next != NULL && p -> next -> free) {
		p -> size += p -> next -> size + BLOCK_SIZE;
		p -> next = p -> next -> next;
		if (p -> next != NULL) 
			p -> next -> prev = p;	
	}
}

bool valid_addr(void *p) {
	if (first != NULL && p >= first + 1 && p < sbrk(0)) {
		return ((node *)((char *)p - BLOCK_SIZE)) -> ptr == p;	
	}
	return false;
}

void free(void *p) {
	if (valid_addr(p)) {
		node *addr = (node *)((char *)p - BLOCK_SIZE);
		if (addr -> free = 0) exit(2);
		if (addr -> prev != NULL && addr -> prev -> free) 
			merge_block(addr = addr -> prev);		
		if (addr -> next != NULL)
			merge_block(addr);
		else {
			if (addr -> prev != NULL) addr -> prev -> next = NULL;
			else first = NULL;
			brk(addr);
		}
	}
	else exit(3);
}

void exam() {
	node *p = first;
	P("Begin")
	while(p) {
		cout << p->prev << ' '<<p<<' '<<p->next<<' '<<p->size<<' '<<p->free<<' '<<p->ptr<<endl;
		p=p->next;
	}
}
//*/

int main () {
/*
	void *p = malloc(100);
	exam();
	void *pp = malloc(100);
	exam();
	free(p);
	exam();
	//void *ppp = malloc(100);
	free(pp);
	return 0;
*/
	char *ch =(char *) malloc(6000000);
	cout << ch<<endl;
	P((size_t)sbrk(0));
	struct rlimit *limit = (struct rlimit *)malloc(sizeof(struct rlimit));
    getrlimit(RLIMIT_AS, limit);
    printf("soft limit: %ld, hard limit: %ld\n", limit->rlim_cur, limit->rlim_max-(size_t)(sbrk(0)));
	return 0;
	srand((size_t)sbrk(0));
	P(sbrk(0)) 
		//srand(0x109c19000);
	map<int, void *>Map;
	int tot = 0, ci = 0;;
	while (1) {
		if (rand() % 2 && Map.size()) {
			cout<<"free  :  "<<Map.begin()->first<<endl;
			free(Map.begin()->second);
			tot -= Map.begin()->first;
			Map.erase(Map.begin());
		}
		else {
			int cnt =rand() % 100 + 1;
			cout<<"malloc:  "<<cnt<<endl;
			tot += cnt;
			if (Map.find(cnt) == Map.end()) {
				Map[cnt] = malloc(cnt);
			}
		}
		cout<<"TOOT:  "<<tot<<"  ci: "<<ci++<<endl;
	}
}
