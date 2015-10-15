// sort algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <chrono>       

bool myfunction (int i,int j) { return (i<j); }

int main () {
  
  std::vector<int> myvector;
  myvector.reserve(1000000);
  
  for(int i=0;i<1000000;i++)
  {
  	myvector.push_back(rand());
  }
  
  /*
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it<<std::endl;
  std::cout << '\n';
  */
  
  // using function as comp
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();  

  std::sort (myvector.begin(), myvector.end(), myfunction); 
  
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now(); 
	
	float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000;
 	
 	std::cout<<duration<<std::endl; 
//  std::sort (myvector.begin(), myvector.end(), myfunction); 

  return 0;
}
