#include <iostream>
#include <chrono>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> value;

const int WIDTH = 256;
const int HEIGHT = 256;
const int RADIUS = 10;

int main(int argc, char *argv[])
{

//
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();  

    std::vector<value> points;
    // create some values
    for ( unsigned i = 0 ; i < 1000000 ; ++i )
    {	
        point p = point(0, i);
	points.emplace_back(std::make_pair(p, i));
    }    

    bgi::rtree< value, bgi::rstar<32> > rtree( points.begin(),points.end() );

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now(); 	
    float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000; 	
    std::cout<<"duration insert to rtree: "<<duration<<std::endl; 


    std::vector<value> returned_values;
    point sought = point(0, 0);
	
//test distance search

	t1 = std::chrono::high_resolution_clock::now();  
    rtree.query(bgi::satisfies([&](value const& v) {return bg::distance(v.first, sought) <= 2;}),
                std::back_inserter(returned_values));	
        t2 = std::chrono::high_resolution_clock::now(); 	
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000; 	
    std::cout<<"duration distance search: "<<duration<<std::endl; 
	
    std::cout<<returned_values.size()<<std::endl;

	t1 = std::chrono::high_resolution_clock::now();  
 

//test nerest neighbours queries
	returned_values.clear();
 	rtree.query(bgi::nearest(point(0, 0), 100), std::back_inserter(returned_values));
	
	t2 = std::chrono::high_resolution_clock::now(); 	
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000; 	
        std::cout<<"duration knn search: "<<duration<<std::endl; 
	
	
    // print returned values

    value to_print_out;
    for (size_t i = 0; i < returned_values.size(); i++) {
        to_print_out = returned_values[i];
        float x = to_print_out.first.get<0>();
        float y = to_print_out.first.get<1>();
        std::cout << "Select point: " << to_print_out.second << std::endl;
        std::cout << "x: " << x << ", y: " << y << std::endl;
    }

    return 0;
}
