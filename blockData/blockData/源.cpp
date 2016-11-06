#include "block.h"
#include "blockList.h"
#include "windows.h"
#include <iostream>
#include <algorithm>

using namespace std;

int s[50];

int main() {
	for (int i = 0; i < 50; i++) s[i] = i;
	block<int> a(150, s, s + 50);
	blockList<int> b(100), c(10);
	c.insert(c.begin(), s, s + 50);
	c.insert(c.begin(), c.begin(), c.end());
	c.erase(c.begin()+10, c.begin()+60);
	c.clear();
	c.insert(c.begin(), s + 12, s + 28);
	c.insert(c.begin(), c.begin(), c.end());
	c.compress(5);
	for (blockList<int>::iterator i = c.begin(); i < c.end(); i++) cout << *i << ' ';
	system("pause");
}