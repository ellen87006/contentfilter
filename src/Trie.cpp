#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstring>
#include <vector>
#include <time.h>
using namespace std;
#define TREE_WIDTH 257
#define Buildfilter 1
#define Removepro 2
struct FilterNode
{
	char data;
	bool last;
	struct FilterNode *next[TREE_WIDTH];
};
//free resources
void FreeNodeTree(FilterNode *root)
{

	for (int i = 0; i < TREE_WIDTH; i++)
	{
		if (root->next[i] != NULL)
		{
			FreeNodeTree(root->next[i]);
			root->next[i] = NULL;
		}
	}
			delete root;
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
FilterNode *SliceFilterlst(const char *path, const char *outputpath)
{
	clock_t start_buildtree = clock();
	char *delim = new char[1];
	delim[0] = '\n';
	ifstream FilterFileStream;
	stringstream RowFilter;
	FilterFileStream.open(path, ios::in);
	RowFilter << FilterFileStream.rdbuf();
	FilterFileStream.close();
	char *Filter = new char[RowFilter.str().length() + 1];
	std::memset(Filter, 0, sizeof(char) * RowFilter.str().length());
	memcpy(Filter, RowFilter.str().c_str(), RowFilter.str().length());
	// profanities list
	struct FilterNode *root = new FilterNode;
	root->next[TREE_WIDTH] = 0;
	std::memset(root->next, 0, sizeof(struct FilterNode *) * TREE_WIDTH);
	root->data = 0;
	FilterNode *current = root;
	char *FilterSegment = strtok(Filter, delim);

	while (FilterSegment)
	{	
		char output[strlen(FilterSegment)];
		std::memset(output, 0, sizeof(char) * strlen(FilterSegment));
		int length = strlen(FilterSegment);
		for (int i = 0; i < length; i++)
		{
			int index = (unsigned char)FilterSegment[i];
			if (index > 0 && index <= 255)
			{
				struct FilterNode *node = current->next[index];
				if (!node)
				{
					node = new FilterNode;
					node->data = FilterSegment[i];
					node->last = false;
					std::memset(node->next, 0, sizeof(struct FilterNode *) * TREE_WIDTH);
					current->next[index] = node;
				}
				current = node;
			}
			else
			{
			}
		}
		current->last = true;
		current = root;
		FilterSegment = strtok(NULL, delim);
	}
	clock_t end_buildtree = clock();
	int exectime = end_buildtree - start_buildtree;

	Log2File(outputpath, Buildfilter, exectime);

	return root;
}

void ProfanitiesRemove(char *input, char *outputpath, struct FilterNode *root)
{
	clock_t ProfanitiesRemove_Start = clock();

	int start = -1;
	char output[512];
	std::memset(output, 0, sizeof(char) * 512);
	int lastNProfanities;
	bool shouldReview = false;
	bool hasfind = false, finding = false;
	for (int i = 0; i < strlen(input);)
	{	
		bool hit=false;
		struct FilterNode *current = root;
		int index = (unsigned char)input[i];
		start=i;
		while(current->next[index])
		{	
			current=current->next[index];
			output[start]='*';
			start++;
			index = (unsigned char)input[start];
			
			if(!current->next[index])
			{
				if(current->last)
				{
					hit=true;
					output[start]='*';
					i=start;

				}
				break;
			}
		}
		output[i]=input[i];
		i++;
	}
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

	struct FilterNode *filterroot;
	filterroot = SliceFilterlst(argv[1], argv[3]);
	ifstream Testcase;
	Testcase.open(argv[2], ios::in);
	stringstream line;
	char input[512];
	std::memset(input, 0, sizeof(char) * 512);
	while (1)
	{
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
					FreeNodeTree(filterroot);
					filterroot = SliceFilterlst(argv[1], argv[3]);
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
			ProfanitiesRemove(input, argv[3], filterroot);
		}
		std::memset(input, 0, sizeof(char) * 512);
	}
	Testcase.close();
	FreeNodeTree(filterroot);
	return 0;
}