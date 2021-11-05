#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>


class Document
{
public:
	
	Document();
	~Document();	
	void slowDownload(std::string key);
	void slowUpload(std::string key, int value);
	void fastDownload(std::string key);
	void fastUpload(std::string key, int value);
	void set_keyValue();	
	std::map <std::string, int> get_keyValue();	
	void print();
private:	
	
	std::map <std::string, int> keyValue;	
};

Document::Document()
{	
	this -> set_keyValue();
	std::cout << "\n" <<"document created" << "\n";
}

Document::~Document()
{
	std::cout << "\n" <<"document deleted" << "\n";
}

void Document::set_keyValue()
{	
	std::map <std::string, int> m_keyValue;
	this -> keyValue = m_keyValue;	
}


std::map <std::string, int> Document::get_keyValue()
{
	return this -> keyValue;
}

void Document::slowDownload(std::string key)
{		
	int value = 0;
	this -> keyValue.insert({key, value});	
}

void Document::print()
{
	int str;
	std::map <std::string, int> thisDocument = this -> get_keyValue();
	std::cout << "If you want to print document-- enter 0" << "\n";
	std::cout << "If you want to print key -- enter 1" << "\n";
	std::cout << "If you want to print value -- enter 2" << "\n";
	std::cin >> str;
	if(str == 0)
		{
			std::cout << "Document" << std::endl;
			std::cout << "-----------------------------" << std::endl;			
			for(auto myIterator : thisDocument)					
				std::cout << myIterator.first << "\t\t\t" << myIterator.second << std::endl;				
			std::cout << "-----------------------------" << std::endl;
		}
	else if(str == 1)
		{
			std::cout << "Key" << std::endl;
			std::cout << "-----------------------------" << std::endl;			
			for(auto myIterator : thisDocument)					
				std::cout << myIterator.first << std::endl;				
			std::cout << "-----------------------------" << std::endl;	
		}
	else if(str == 2)
		{
			std::cout << "Value" << std::endl;
			std::cout << "-----------------------------" << std::endl;			
			for(auto myIterator : thisDocument)					
				std::cout << myIterator.second << std::endl;				
			std::cout << "-----------------------------" << std::endl;		
		}			
	
}


void Document::fastDownload(std::string key) 
{
	this -> slowDownload(key);
	
}

void Document::slowUpload(std::string key, int value)
{
	this -> keyValue.insert({key, value});
}

void Document::fastUpload(std::string key, int value)
{
	this -> slowUpload(key, value);
	this -> keyValue.value_comp();
}

int main()
{
	Document* a = new Document();			
	int randNumber = 0;
	std::string str;
	std::string ptrStr = "";
	std::ifstream in("Users.txt"); 
	if (in.is_open())
		{
			while (getline(in, str))
				{            		
					randNumber = rand() % 1000 + 1;
					a -> fastUpload(str, randNumber);
				}
		}
    	in.close();	
	while(true)
		{						
			std::cout << "Enter key to add a new user in the document" << std::endl;			
			std::cin >> str;
			randNumber = rand() % 1000 + 1;
			a -> fastUpload(str, randNumber);
			a -> print();				
		}		
	a -> ~Document();
	return 0;
}
