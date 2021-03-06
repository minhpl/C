#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>


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

bool sortByLatitude(Circle& c1,Circle& c2)
{
	return c1.x<c2.x;
}

int main(){

	std::list<Circle> mylist;

	for(int i=0;i<1000000;i++)
	{
		int x = rand()%(WIDTH+1);
		int y = rand()%(HEIGHT+1);
		Circle c;
		c.x = x;
		c.y = y;
		c.radius = RADIUS;
		c.index = i;
		mylist.push_back(c);
	}
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();  
	mylist.sort(sortByLatitude);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now(); 
	
	float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000;
 	
 	std::cout<<duration<<std::endl; 
	/*
	for (std::vector<Circle>::iterator it = myvector.begin(); it != myvector.end(); ++it){
	    std::cout << ' ' << it->x <<','<<it->y<<std::endl;
	}
	*/
	
	return 0;	
}
