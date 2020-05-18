#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <limits.h>
#include <algorithm>
using namespace std;

typedef struct TreeNode
{
	vector<TreeNode *> ptr;
	vector<int> data;
	bool isleaf;
} TreeNode;

TreeNode* root;

vector<string> split(char *phrase, string delimiter = " ")
{
	vector<string> list;
	string s = string(phrase);
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos)
	{
		token = s.substr(0, pos);
		list.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	list.push_back(s);
	return list;
}

void insert(TreeNode* node, TreeNode* parent, int x)
{
	if(node->isleaf)
	{
		node->data.push_back(x);
		sort(node->data.begin(), node->data.end());
		
		if(node->data.size() > 3)
		{
			TreeNode* temp = new TreeNode;
			
			temp->data.push_back(node->data[2]);
			temp->data.push_back(node->data[3]);
			
			temp->isleaf = true;

			if(node->ptr.size())
				temp->ptr.push_back(node->ptr[0]);

			node->data.pop_back();
			node->data.pop_back();
			
			if(node->ptr.size())
				node->ptr[0] = temp;
			
			else
				node->ptr.push_back(temp);
				
			if(parent)
			{
				parent->data.push_back(temp->data[0]);
				parent->ptr.push_back(temp);
			}
			
			else
			{
				TreeNode* newRoot = new TreeNode;
				newRoot->isleaf = false;

				newRoot->data.push_back(temp->data[0]);
				newRoot->ptr.push_back(node);
				newRoot->ptr.push_back(temp);

				root = newRoot;
			}
		}
	}
	
	else
	{
		int i;

		for(i = 0; i < node->data.size(); i++)
			if(x < node->data[i])
				break;
		
		insert(node->ptr[i], node, x);
		
		for(i = node->data.size()-1; i > 0; i--)
		{
			if(node->data[i] < node->data[i-1])
			{
				swap(node->data[i-1], node->data[i]);
				swap(node->ptr[i], node->ptr[i+1]);
			}
		}

		if(node->data.size() > 3)
		{
			int mid = node->data[2];

			TreeNode* temp = new TreeNode;
			temp->isleaf = false;
			temp->data.push_back(node->data[3]);
			temp->ptr.push_back(node->ptr[3]);
			temp->ptr.push_back(node->ptr[4]);
			
			node->data.pop_back();
			node->data.pop_back();
			
			node->ptr.pop_back();
			node->ptr.pop_back();
			
			if(parent)
			{
				parent->data.push_back(mid);
				parent->ptr.push_back(temp);
			}
			
			else
			{
				TreeNode* newRoot = new TreeNode;
				newRoot->isleaf = false;

				newRoot->data.push_back(mid);
				newRoot->ptr.push_back(node);
				newRoot->ptr.push_back(temp);
				
				root = newRoot;
			}
		}
	}
}

void find(TreeNode* t, int x)
{
	if(!t)
	{
		cout << "NO\n";
		return;
	}

	if(t->isleaf)
	{
		TreeNode* curr = t;
		
		while(true)
		{
			for(int i = 0; i < curr->data.size(); i++)
			{
				if(x == curr->data[i])
				{
					cout << "YES\n";
					return;
				}
				
				if(x < curr->data[i])
				{
					cout << "NO\n";
					return;
				}
			}
			
			if(curr->ptr.empty())
			{
				cout << "NO\n";
				return;
			}
			
			else
				curr = curr->ptr[0];
		}
	}
	
	else
	{
		int i;
		for (i = 0; i < t->data.size(); i++)
		{
			if(x == t->data[i])
			{
				cout << "YES\n";
				return;
			}

			if(x < t->data[i])
				break;
		}

		find(t->ptr[i], x);
	}
	
}

void range(TreeNode* t, int lo, int hi)
{
	if(!t)
	{
		cout << "0\n";
		return;
	}

	if(t->isleaf)
	{
		TreeNode* curr = t;
		int count = 0;

		while(true)
		{
			for(int i = 0; i < curr->data.size(); i++)
			{
				if(lo <= curr->data[i] && curr->data[i] <= hi)
					count++;

				if(curr->data[i] > hi)
				{
					cout << count << "\n";
					return;
				}
			}
			
			if(curr->ptr.empty())
			{
				cout << count << "\n";
				return;
			}
			
			else
				curr = curr->ptr[0];
		}
	}

	else
	{
		int i;
		for (i = 0; i < t->data.size(); i++)
		{
			if(lo <= t->data[i])
				break;
		}

		range(t->ptr[i], lo, hi);
	}
	
}

void traverse(TreeNode* t, int d)
{
	if(!t)
		return;
	if(t->isleaf)
		cout << 'L';
	for(int i = 0; i<d; i++)
		cout <<'*';

	for(int i = 0; i<t->data.size(); i++)
		cout << t->data[i] <<' ';
	cout << '\n';

	if(t->isleaf == false)
		for(int i = 0; i<t->ptr.size(); i++)
			traverse(t->ptr[i], d+1);
}

int main(int argc, char const *argv[])
{
	// Root of B+ Tree
	root = new TreeNode;
	root->isleaf = true;

	ifstream input;
	input.open(argv[1]);

	string line; // Temp variable

	while (input)
	{
		getline(input, line);
		if(line.size() == 0) continue;
		vector<string> result_split = split((char *)line.c_str());
		int n1 = atoi((const char*)result_split[1].c_str());
		
		if(result_split[0] == "INSERT")
			insert(root, NULL, n1);
		else if(result_split[0] == "FIND")
			find(root, n1);
		else if(result_split[0] == "COUNT")
			range(root, n1, n1);
		else if(result_split[0] == "RANGE")
		{
			int n2 = atoi((const char*)result_split[2].c_str());
			range(root, n1, n2);
		}

		else
			continue;
	}

	return 0;
}
