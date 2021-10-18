#include <iostream>
#include <cstdio>
#include <cstring>


class Rectangle
{	
public:
	Rectangle();
	~Rectangle();
	/*Rectangle(std::string name , int lenght , int width)
	{
		setName(name);
		setLenght(lenght);
		setWidth(width);
	}*/
	void setLenght(int lenght);
	void setWidth(int lenght);
	int getLenght();
	int getWidth();
	void setValue(int length , int width);
	int getPerimetr();
	int getArea();
	void print();
	void callArea();
	void setName(std::string name);
	std::string getName();
	
private:
	int area ;	
	int lenght ;
	int width ;
	std::string name;
};
Rectangle::Rectangle() : lenght(4) , width(40)
{
	callArea();
	printf("Rectangle constructor\n");
}

Rectangle::~Rectangle()
{
	printf("Rectangle destructor\n");
}

void Rectangle::setName(std::string name)
{
	this->name = name;
}

std::string Rectangle::getName()
{
	return this->name;
}

void Rectangle::callArea()
{
	this->area = this->lenght * this->width;
}

void Rectangle::setLenght(int lenght)
{
	this->lenght = lenght;
	this->callArea();
}

void Rectangle::setWidth(int lenght)
{
	this->width = lenght;
	this->callArea();
}

int Rectangle::getLenght()
{
	return this->lenght;
}

int Rectangle::getWidth()
{
	return this->width;
}

void Rectangle::setValue(int length , int width)
{
	this->lenght = length;	
	this->width = width;
	this->callArea();
}

int Rectangle::getPerimetr()
{
	return (lenght + width) * 2;
}
	
int Rectangle::getArea()
{
	return this->area;
}
	
void Rectangle::print()
{
	printf("Name: %s , Area = %d , Perimetr = %d\n" , getName().c_str() , getArea() , getPerimetr());
}

class Square: public Rectangle
{
public:
	Square() 
	{
		printf("Square constructor\n");
	}
	~Square()
	{
		printf("Square destructor\n");	
	}
	/*Square(std::string name , int lenght)
	: Rectangle(name , lenght , lenght)
	{}*/
	void setLenght(int lenght);
	void setName(std::string name);
private:
	int hoho;
}; 

void Square::setName(std::string name)
{
	Rectangle::setName(name);
}

void Square::setLenght(int lenght)
{	
	Rectangle::setWidth(lenght);
	Rectangle::setLenght(lenght);	
}



int main()
{
	//Rectangle rectangle("Rectangle" , 26 , 30);
	Rectangle* rectangle = new Rectangle();
	printf("Size of rectangle = %d --------------------\n" , sizeof(Rectangle));
	Rectangle* square = new Square();
	printf("Size of square = %d --------------------\n" , sizeof(Square));
	rectangle->print();
	delete(rectangle);
	(*square).print();
	delete(square);
	return 0;
}

