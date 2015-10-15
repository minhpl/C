#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>
#include <stdlib.h>

const int WIDTH = 256;
const int HEIGHT = 256;
const int RADIUS = 10;

struct Circle
{
	int x;
	int y;
	int radius;
	int index;
};

int distance(int x,int y)
{
	return abs(x-y);
}

std::vector<int> a(const int& x,const std::vector<int>& v,const int& radius,int& min, int& max)
{
	
}

bool sortByLatitude(Circle& c1,Circle& c2)
{
	return c1.x<c2.x;
}

bool sortByLongitude(Circle& c1,Circle& c2)
{
	return c1.y<c2.y;
}

int main(){

	std::vector<Circle> myvector;
	myvector.reserve(1000000);

	for(int i=0;i<1000000;i++)
	{
		int x = rand()%(WIDTH+1);
		int y = rand()%(HEIGHT+1);
		Circle c;
		c.x = x;
		c.y = y;
		c.radius = RADIUS;
		c.index = i;
		myvector.push_back(c);
	}
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();  	
	std::sort(myvector.begin(),myvector.end(),sortByLatitude);
	std::sort(myvector.begin(),myvector.end(),sortByLongitude);	
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();  	
	float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000;
 	
 	std::cout<<"sort duration: "<<duration<<std::endl;
		
	return 0;	
}
