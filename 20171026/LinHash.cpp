#include <iostream>
#include <fstream> 
#include <vector>
#include <limits.h>
using namespace std;

float LOAD_MAX = 0.75;
int hashN = 2;

int entries = 0;
int nBuckets = 0;

typedef struct Bucket{
	int data[250]; // 1 KB buckets
	Bucket* next;
}Bucket;

Bucket* init()
{
	Bucket* temp = new Bucket;
	temp->next = NULL;
	
	for(int i = 0; i < 250; i++)
		temp->data[i] = INT_MIN;

	nBuckets++;
	return temp;
}

vector<Bucket*> hashTable(2);

bool search(int n, Bucket* hBuck)
{
	Bucket* cur = hBuck;
	while(cur)
	{
		for(int i = 0; i < 250; i++)
			if(cur->data[i] == n)
				return true;
		cur = cur->next;
	}
	return false;
}

void insertrecord(int n, Bucket* hBuck)
{
	while(true)
	{
		for(int i = 0; i < 250; i++)
			if(hBuck->data[i] == INT_MIN)
			{
				hBuck->data[i] = n;
				return;
			}
			
		if(!hBuck->next)
		{
			hBuck->next = init();
			hBuck->next->data[0] = n;
			return;
		}
		
		else hBuck = hBuck->next;
	}
}

int nearestpow2(int k)
{
	int n = 1;
	while(2*n <= k) n*=2;
	return n;
}

void splitbucket()
{
	int sz = hashTable.size();
	int bucketToBeSplit = sz-nearestpow2(sz);
	hashTable.resize(sz+1);
	
	if(hashTable.size() > hashN)
		hashN*=2;

	hashTable[sz] = init();
	
	Bucket* sBuck = hashTable[bucketToBeSplit];
	while(sBuck)
	{
		for(int i = 0; i < 250; i++)
		{
			int u = sBuck->data[i];
			if(u != INT_MIN)
			{
				if(u%hashN == bucketToBeSplit)
					continue;
				else
					insertrecord(u, hashTable[sz]);
			}
		}
		sBuck = sBuck->next;
	}
	
}

void insert(int n)
{
	int hashBucket = n%hashN;

	if(hashBucket < 0)
		hashBucket += hashN;

	if(hashBucket >= hashTable.size())
		hashBucket -= hashN/2;

	Bucket* currB = hashTable[hashBucket];

	if(search(n, currB))
		return;

	insertrecord(n, currB);
	entries++;
	cout << n <<'\n';
	
	float util = (float)entries/(250*(float)nBuckets);

	if(util > LOAD_MAX)
		splitbucket();		
}

void printTable()
{
	cout << "***\n";
	for(int i = 0; i < hashTable.size(); i++)
	{
		Bucket* hBuck = hashTable[i];
		while(hBuck)
		{
			for(int j = 0; j < 250; j++)
				if(hashTable[i]->data[j] != INT_MIN)
				cout << hashTable[i]->data[j] << ' ';
		
			hBuck = hBuck->next;
		}
		cout << '\n';
	}

	cout << "***\n";	
}

int main(int argc, char const *argv[])
{v
	ifstream input;
	input.open(argv[1]);

	string line; // Temp variable
	hashTable[0] = init();
	hashTable[1] = init();
	
	while (input)
	{
		getline(input, line);
		if(line.size())
		{
			int n1 = atoi(line.c_str());
			insert(n1);
		}
	}
	
	return 0;
}
