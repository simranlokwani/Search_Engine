#include <bits/stdc++.h>
#include<iostream>
#include<cstring>
#include<dirent.h>
#include <ctime>
#include<conio.h> // for getch()
#include<stdlib.h>
//limited the number of words that the program can handle
#define MAX_SIZE 100000

//limited the number of files the program can handle
#define MAX_FILES 10

using namespace std;

//function used for sorting ====================================================
/*int comparator(const void *p,const void *q)
{
	const int *a = (const int *)p;
	const int *b = (const int *)q;
	return(a[1]-b[1]);
}*/
void mergesort(int arr[][2],int low,int mid,int high)
{
    int i,j,k,m = mid-low+1,n=high-mid;
    int first_half[m][2],second_half[n][2];
    for(i=0;i<m;i++)
        {
           first_half[i][0]=arr[low+i][0];
           first_half[i][1]=arr[low+i][1];
        }
    for(i=0;i<n;i++)
        {
          second_half[i][0]=arr[mid+i+1][0];
          second_half[i][1]=arr[mid+i+1][1];
        }
    i=0;j=0;
    k=low;
    while(i<m || j<n)
    {
       if(i>=m)
       {
           arr[k][0]=second_half[j][0];
           arr[k][1]=second_half[j][1];
           k++;j++;
           continue;
       }
       if(j>=n)
       {
           arr[k][0]=first_half[i][0];
           arr[k][1]=first_half[i][1];
           k++;i++;
           continue;
       }
       if(first_half[i][1]<=second_half[j][1])
        {
            arr[k][1]=first_half[i][1];
            arr[k][0]=first_half[i][0];
            k++;i++;
        }
       else
        {
            arr[k][0]=second_half[j][0];
            arr[k][1]=second_half[j][1];
            k++;j++;
            //counts = counts + mid - low + i+1;
            //printf(" counts %d low %d mid %d high %d",counts,low,mid,high);
        }
    }
}

void divide(int arr[][2],int low,int high)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        divide(arr,low,mid);
        divide(arr,mid+1,high);
        mergesort(arr,low,mid,high);
    }
}
//==============================================================================


//basic structure of the linked list ===========================================
typedef class node{ public:
	char *data;
	int freq[MAX_FILES][2];// 0 index for file , 1 for frequency
    node *next;
}node;
//==============================================================================


node table[MAX_SIZE];
string stored[MAX_SIZE];

//Directory handling variables =================================================
DIR *dpdf;
struct dirent *epdf;
//==============================================================================

long int foo=0;			//to keep count of total number of words ===============

int words=0,currentfile=0;

//hash function ================================================================
long int hash(char *st)
{
	long int hashvalue=0;
	int c;
	char *str = &st[0];
	while(c=*str)
	{
		hashvalue = c + hashvalue<<6 + hashvalue<<16 - hashvalue;
		str++;
	}
	return hashvalue%MAX_SIZE;
}
//==============================================================================


//main function to execute the program =========================================
int main()
{
	clock_t begin = clock();
	int i,val,len,j,num;
	string locationname[MAX_FILES];
	string userstring;
	char *pointertostring,c;
	cout<<endl;
	cout<<endl;
	std::cout<<"*********Getting all files in the desired directory**********\n";
	
	cout<<""<<endl;
	

	//opening the current directory
	dpdf = opendir("./");
	vector<string> tobeopen;
	vector<string> filenames;

	//storing all file names in a vector =======================================
	if (dpdf != NULL)
	{
		//cout<<"gg"<<endl;
   		while (epdf = readdir(dpdf))
   		{
     	 	filenames.push_back(epdf->d_name);
     	 //	cout<<"name: "<<epdf->d_name<<endl;
   		}
	}
	num = filenames.size();
	cout<<" NUMBER OF FILES = "<<num<<endl;
	i=0;
	size_t foundvar,temp;
	//==========================================================================
	
	//storing file names having '.txt' format in another vector ================
	while(i<num)
	{
		string fname = filenames[i];
		foundvar = filenames[i].find(".txt");
		temp = filenames[i].find(".txt~");
		if((foundvar!=string::npos)&&!(temp!=string::npos))
		{
			//cout<<fname<<" ";
			tobeopen.push_back(fname);
		}
		/*len = strlen((char *)&fname[0]);
		j=0;
		while(j+3<len)
		{
			if(fname[j]=='.'&&fname[j+1]=='t'&&fname[j+2]=='x'&&fname[j+3]=='t'&&fname[j+4]!='~')
			{
				tobeopen.push_back(fname);
			}
			j++;
		}*/
		i++;
	}
	//==========================================================================
	
	
	cout<<" Total .txt files = "<< tobeopen.size()<<endl;

	for(i=0;i<MAX_SIZE;i++)
		table[i].next=NULL;
	
	//**************************************************************************
	//start of while loop to read files one by one from directory ==============
	
	while(currentfile<(tobeopen.size()))
	{		
		// for reading file from the directory vector: tobeopen is used
		string finame = tobeopen[currentfile];
		//cout<<endl<<finame<<"   ";
		ifstream fin;
		fin.open(finame.c_str(),ios::in);
		bool check=false;
		//while loop for reading the characters in a file ----------------------
		while(!fin.eof())
		{
			string g;
            		check = false;
			fin.get(c);
			while((c>=97&&c<=122)||(c>=65&&c<=90)||(c>=48&&c<=57)&&!fin.eof())
			{
				if(c>=65&&c<=90) c+=32;
				check=true;
				g+=c;
				fin.get(c);
			}
			if(check)
			{
				g+='\0';
				foo++;
				stored[words]=g;
				len = strlen(&g[0]);
				val = hash((char *)&g[0])%MAX_SIZE;
				pointertostring = (char *)&stored[words][0];

				//form a new node if the place corresponding to the hash value
				//obtained for the word is empty
				if(table[val].next==NULL)
				{
					table[val].freq[currentfile][0] = currentfile;
					table[val].freq[currentfile][1] = 1;
					table[val].data = pointertostring;
					node* temp = new node;
					temp->next=NULL;
					table[val].next=temp;
					words++;	//go to next word of the file
				}

				//go to the corresponding node of the linked list if there
				//already exists a word corresponding to the obtained hash value
				else
				{
					node *f;
					bool check=true;
					f = &table[val];

					//check if the word is already present in the linked list
					//by traversing it from the root

					//if the is word already present in the linked list
					//corresponding to the hash value, increase the frequency
					//corresponding to the file name that is being traversed
					while(f->next!=NULL)
					{
						if(strcmp(f->data,pointertostring)==0)
						{
							f->freq[currentfile][0]= currentfile;
							f->freq[currentfile][1]++;
							check = false;
						}
						f=f->next;
					}

					//if the word is not present then create another node in
					//the same linked list with the new word and update its
					//frequency corresponding to the file being traversed
					if(check)
					{
						f->data = pointertostring;
						f->freq[currentfile][0] = currentfile;
						f->freq[currentfile][1] = 1;
						node* temp = new node;
						temp->next=NULL;
						f->next = temp;
						words++;	//go to next word of the file
					}
				}
			}
		}
		// end of while loop for reading characters ----------------------------

		//go to next file of the directory
		currentfile++;
	}
	//end of while loop for reading files from the directory ====================

//******************************************************************************
	
	clock_t end = clock();
  	double elapsed_secs1 = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"\n Total time elapsed in hashing all files is = "<<elapsed_secs1<<endl;
	//cout<<" size of total files "<<currentfile<<endl;
	char choice;
	bool check;

//==============================================================================
	cout<<" Total words extracted = "<<foo<<endl;
	while(1)
	{
		clock_t begin1=clock();
		cout<<" Do you Want to enter string? (y/n)  \n";
		cin>>choice;
		if(choice=='n'||choice=='N')
			break;

		//getting keyword from user
		cout<<" Enter the Word to search for : "<<endl;
		cin>>userstring;
		transform(userstring.begin(), userstring.end(), userstring.begin(), ::tolower);
		cout<<endl;
		cout<<"========================================================="<<endl;
		//assigning a pointer to the variable in which user input is stored
		pointertostring = (char *)(&userstring[0]);
		val = hash(pointertostring)%MAX_SIZE ;
		node *f = &table[val];
		check = true;
		while(f->next!=NULL)
		{
			if(strcmp(f->data,pointertostring)==0)
			{
				cout<<" '"<<f->data<<"'"<<" found \n";
				cout<<"-------------------------------"<<endl;
				cout<<" { Increasing order of frequency is } \n";
				divide(f->freq,0,currentfile-1);
				//qsort(f->freq,currentfile,2*sizeof(int),comparator);
				for(i=0;i<currentfile;i++)
				{
					if(f->freq[i][1]>0)
						cout<<" File name is "<<tobeopen[f->freq[i][0]]<<" , Frequency is = "<<f->freq[i][1]<<endl;
				}
				check = false;
				break;
			}
			f=f->next;
		}

		//give an output when no file containes the searched keyword
		if(check)
		{
			cout<<" *Word NOT Found* \n";
		}
		cout<<"=========================================================="<<endl;
		clock_t end1 = clock();
	  	double elapsed_secs = double(end1 - begin1) / CLOCKS_PER_SEC;
		cout<<endl;
		//printf("time elapsed to search for this string is %.9f\n",elapsed_secs);
	    //cout<<" Time elapsed to search for this string is : "<<elapsed_secs<<endl;
		
		cout<<" Press any key to continue "<<endl;
		getch();
	//system("clear");
	system("cls");
	}
//==============================================================================
	return 0;
}

/*############################## END OF CODE #################################*/
