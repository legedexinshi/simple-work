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

struct node {
	int free;
	int size;
	node *next, *pre;
};

node *first = NULL;

int main () {
	node a;
	cout << sizeof(a);
}
