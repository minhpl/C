#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <chrono>
#include <boost/geometry/index/rtree.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
using namespace std::chrono;

int main()
{
    typedef bg::model::point<float, 2, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
    typedef std::pair<box, unsigned> value;

    // polygons
    std::vector<polygon> polygons;

    // create some polygons
    for ( unsigned i = 0 ; i < 100000 ; ++i )
    {
        // create a polygon
        polygon p;
        for ( float a = 0 ; a < 1.0f ; a += 1.04720f )
        {
            float x = i + int(10*::cos(a))*0.1f;
            float y = i + int(10*::sin(a))*0.1f;
            p.outer().push_back(point(x, y));
        }

        // add polygon
        polygons.push_back(p);
    }

/*
    // display polygons
    std::cout << "generated polygons:" << std::endl;
    BOOST_FOREACH(polygon const& p, polygons)
        std::cout << bg::wkt<polygon>(p) << std::endl;
*/


	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();  

	// create the rtree using default constructor
        bgi::rtree< value, bgi::rstar<16, 4> > rtree;

        // fill the spatial index
	for ( unsigned i = 0 ; i < polygons.size() ; ++i )
	{
		// calculate polygon bounding box
		box b = bg::return_envelope<box>(polygons[i]);
		// insert new value
		rtree.insert(std::make_pair(b, i));
	}        
	  
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now(); 
	
	float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count())/1000;
 	
 	std::cout<<duration<<std::endl;





/*
    // find values intersecting some area defined by a box
    box query_box(point(0, 0), point(5, 5));
    std::vector<value> result_s;
    rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));
*/

    // find 5 nearest values to a point
    std::vector<value> result_n;
    rtree.query(bgi::nearest(point(0, 0), 5), std::back_inserter(result_n));

    // note: in Boost.Geometry the WKT representation of a box is polygon

    // note: the values store the bounding boxes of polygons
    // the polygons aren't used for querying but are printed
/*
    // display results
    std::cout << "spatial query box:" << std::endl;
    std::cout << bg::wkt<box>(query_box) << std::endl;
    std::cout << "spatial query result:" << std::endl;
    BOOST_FOREACH(value const& v, result_s)
        std::cout << bg::wkt<polygon>(polygons[v.second]) << std::endl;
*/
    std::cout << "knn query point:" << std::endl;
    std::cout << bg::wkt<point>(point(0, 0)) << std::endl;
    std::cout << "knn query result:" << std::endl;
    BOOST_FOREACH(value const& v, result_n)
        std::cout << bg::wkt<polygon>(polygons[v.second]) << std::endl;

    return 0;
}

