#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstring>
#include <vector>
#include <time.h>
using namespace std;
#define Buildfilter 1
#define Removepro 2
struct Node
{
	char data;
	bool last;
	Node *next_level;
	Node *same_level;
};
//free resources
void FreeNodeTree(Node *root)
{
	if (root != 0)
	{
		FreeNodeTree(root->next_level);
		FreeNodeTree(root->same_level);
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
//node search
Node *SearchNode(Node *current, char data)
{
	while (current != 0)
	{
		if (current->data == data)
		{
			return current;
		}
		current = current->same_level;
	}
	return 0;
}

void Log2File(const char *outputpath,int dir,int exectime)
{
	ofstream logfile;
	if(dir==Removepro)
	{
		logfile.open(outputpath,ios::app);
		logfile << "ProfanitiesRemove Time:" << exectime << " ms" << endl;
	}
	else
	{
		logfile.open(outputpath,ios::trunc);
		logfile << "BuiildFilter Time:" << exectime << " ms" << endl;
	}
	logfile.close();
}
//build filter
Node *SliceFilterlst(const char *path,const char *outputpath)
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
	root->next_level = 0;
	root->same_level = 0;
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
				Node *node = SearchNode(current->next_level, pushbuffer[i]);
				if (node == 0)
				{
					node = new Node();
					node->data = pushbuffer[i];
					node->last = false;
					node->next_level = 0;
					node->same_level = 0;
					if (current->next_level == 0)
					{
						current->next_level = node;
					}
					else
					{
						Node *temp = current->next_level;
						while (temp->same_level != 0)
						{
							temp = temp->same_level;
						}
						temp->same_level = node;
					}
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
	int exectime=end_buildtree - start_buildtree;
	Log2File(outputpath,Buildfilter,exectime);
	return root;
}

void ProfanitiesRemove(char *input,char *outputpath, Node *root)
{
	clock_t ProfanitiesRemove_Start = clock();

	Node *current = root->next_level;
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
			if (current->data == ch)
			{
				hasfind = true;
				if (start == -1)
				{
					start = i;
				}
				if (current->last && current->next_level == 0)
				{
					start = -1;
					current = root->next_level;
					shouldReview = false;
				}
				else if (current->last)
				{
					start = i + 1;
					current = current->next_level;
					shouldReview = true;
				}
				else
				{
					current = current->next_level;
				}
				output[i] = '*';
				break;
			}
			current = current->same_level;
		}

		if (shouldReview && !hasfind)
		{
			i = start - 1;
			shouldReview = false;
			start = -1;
			current = root->next_level;
		}
		else if (!hasfind)
		{
			if (start > -1)
			{
				i = start;
			}
			lastNProfanities -= i;
			output[i] = input[i];
			lastNProfanities = i;
			start = -1;
			current = root->next_level;
		}
	}

	if (start > -1 && !shouldReview)
	{
		for (int j = start; j < strlen(input); j++)
		{
			output[j] = input[j];
		}
	}
	//cout << output << endl;
	clock_t ProfanitiesRemove_End = clock();
	int exectime=ProfanitiesRemove_End - ProfanitiesRemove_Start;
	Log2File(outputpath,Removepro,exectime);
}

void AddFilterlst(const char *path,char *input)
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
	root = SliceFilterlst(argv[1],argv[3]);
	ifstream Testcase;
	Testcase.open(argv[2], ios::in);
  stringstream line;
	char input[512];
	std::memset(input, 0, sizeof(char) * 512);
	while(1)
	{
    //memcpy(input, line.str().c_str(), line.str().length());
    Testcase.getline(input,sizeof(input));
		if(strcmp(input, "#terminate") == 0)
		{
			break;
		}
		else if (strcmp(input, "#add") == 0)
		{
     Testcase.getline(input,sizeof(input));
     while(1){
		 if (strcmp(input, "#complete") == 0){
  			FreeNodeTree(root);
			  root = SliceFilterlst(argv[1],argv[3]);
        break;
     }
     else if (strncmp(input, "#", 1))
			AddFilterlst(argv[1],input);
     }
		}
		else if (strncmp(input, "#", 1)==0)
		{
		  cout << "type\"#terminate\" to leave add mode!!" << endl;
			cout << "type\"#add\" to leave add mode!!" << endl;
		}
		else
		{
       ProfanitiesRemove(input,argv[3], root);
		}
		std::memset(input, 0, sizeof(char) * 512);
	}
  Testcase.close();
	FreeNodeTree(root);
	return 0;
}