#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstring>
#include <vector>
#include <time.h>
using namespace std;
#define TREE_WIDTH 256
#define Buildfilter 1
#define Removepro 2
struct Node
{
	char data;
	bool last;
	struct Node *next[TREE_WIDTH];
};
//free resources
void FreeNodeTree(Node *root)
{

	for (int i = 0; i < TREE_WIDTH; i++)
	{
		if (root->next[i] != NULL)
		{
			FreeNodeTree(root->next[i]);
			root->next[i] = NULL;
		}
		else
			delete root;
	}
}

void FreeKeyWordList(list<char *> &list_input)
{
	for (list<char *>::iterator it_key = list_input.begin(); it_key != list_input.end(); ++it_key)
	{
		delete *it_key;
	}
}
void Log2File(const char *outputpath, int dir, int exectime)
{
	ofstream logfile;
	if (dir == Removepro)
	{
		logfile.open(outputpath, ios::app);
		logfile << "ProfanitiesRemove Time:" << exectime << " ms" << endl;
	}
	else
	{
		logfile.open(outputpath, ios::trunc);
		logfile << "BuiildFilter Time:" << exectime << " ms" << endl;
	}
	logfile.close();
}
//build filter
Node *SliceFilterlst(const char *path, const char *outputpath)
{
	//cout << "Please Wait....." << endl;
	clock_t start_buildtree = clock();
	char *delim = new char[1];
	delim[0] = '\n';
	ifstream FilterFileStream;
	stringstream RowFilter;
	FilterFileStream.open(path, ios::in);
	RowFilter << FilterFileStream.rdbuf();
	FilterFileStream.close();
	// read filter file complete
	char *Filter = new char[RowFilter.str().length() + 1];
	std::memset(Filter, 0, sizeof(char) * RowFilter.str().length());
	memcpy(Filter, RowFilter.str().c_str(), RowFilter.str().length());
	list<char *> list_keyword;
	list<char *>::iterator it_keyword;
	// profanities list
	Node *root = new Node();
	root->next[TREE_WIDTH] = 0;
	root->data = 0;
	Node *current = root;
	char *FilterSegment = strtok(Filter, delim);
	while (FilterSegment)
	{
		bool exist = false;
		for (list<char *>::iterator it_keyword = list_keyword.begin(); it_keyword != list_keyword.end(); ++it_keyword)
		{
			if (strcmp(*it_keyword, FilterSegment) == 0)
			{
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			char *pushbuffer = new char[strlen(FilterSegment) + 1];
			memcpy(pushbuffer, FilterSegment, strlen(FilterSegment));
			pushbuffer[strlen(FilterSegment)] = 0;
			list_keyword.push_back(pushbuffer);
			int length = strlen(pushbuffer);
			for (int i = 0; i < length; i++)
			{
				Node *node = current->next[pushbuffer[i]];
				if (node == 0)
				{
					node = new Node();
					node->data = pushbuffer[i];
					node->last = false;
					node->next[TREE_WIDTH] = 0;
					current->next[pushbuffer[i]] = node;
				}
				current = node;
			}

			current->last = true;
			current = root;
		}
		FilterSegment = strtok(NULL, delim);
	}
	FreeKeyWordList(list_keyword);
	clock_t end_buildtree = clock();
	int exectime = end_buildtree - start_buildtree;
	Log2File(outputpath, Buildfilter, exectime);
	return root;
}

void ProfanitiesRemove(char *input, char *outputpath, Node *root)
{
	clock_t ProfanitiesRemove_Start = clock();
	Node *current = root;
	int start = -1;
	char output[512];
	std::memset(output, 0, sizeof(char) * 512);
	int lastNProfanities;
	bool shouldReview = false;
	for (int i = 0; i < strlen(input); i++)
	{

		bool hasfind = false;
		char ch = input[i];

		while (current != 0)
		{
			if (current->next[input[i]] == 0 && current->last == 1)
				hasfind = true;
			current = current->next[input[i]];
			output[i] = '*';
		}

		if (!hasfind)
		{

			output[i] = input[i];
		}
		current = root;
	}

	cout << output << endl;
	clock_t ProfanitiesRemove_End = clock();
	int exectime = ProfanitiesRemove_End - ProfanitiesRemove_Start;
	Log2File(outputpath, Removepro, exectime);
}

void AddFilterlst(const char *path, char *input)
{
	ofstream Modifyfilestream;
	std::memset(input, 0, sizeof(char) * 512);
	cin.getline(input, 512);
	Modifyfilestream.open(path, ios::app);
	input[strlen(input)] = '\n';
	Modifyfilestream.write(input, strlen(input));
	Modifyfilestream.close();
}

int main(int argc, char *argv[])
{

	Node *root;
	root = SliceFilterlst(argv[1], argv[3]);
	ifstream Testcase;
	Testcase.open(argv[2], ios::in);
	stringstream line;
	char input[512];
	std::memset(input, 0, sizeof(char) * 512);
	while (1)
	{
		//memcpy(input, line.str().c_str(), line.str().length());
		Testcase.getline(input, sizeof(input));
		if (strcmp(input, "#terminate") == 0)
		{
			break;
		}
		else if (strcmp(input, "#add") == 0)
		{
			Testcase.getline(input, sizeof(input));
			while (1)
			{
				if (strcmp(input, "#complete") == 0)
				{
					FreeNodeTree(root);
					root = SliceFilterlst(argv[1], argv[3]);
					break;
				}
				else if (strncmp(input, "#", 1))
					AddFilterlst(argv[1], input);
			}
		}
		else if (strncmp(input, "#", 1) == 0)
		{
			cout << "type\"#terminate\" to leave add mode!!" << endl;
			cout << "type\"#add\" to leave add mode!!" << endl;
		}
		else
		{
			ProfanitiesRemove(input, argv[3], root);
		}
		std::memset(input, 0, sizeof(char) * 512);
	}
	Testcase.close();
	FreeNodeTree(root);
	return 0;
}