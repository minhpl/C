#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <boost/geometry/index/rtree.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <vector>



namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
typedef std::pair<box, unsigned> value;
typedef bgi::rtree< value, bgi::rstar<16, 4> > RTREE;

const int WIDTH = 256;
const int HEIGHT = 256;

auto RED = cv::Scalar(0, 0, 255, 255);
auto GREEN = cv::Scalar(0, 255, 0, 255);
auto BLUE = cv::Scalar(255, 0, 0, 255);

void printPoint(int * point)
{
	std::cout<<"("<<point[0]<<","<<point[1]<<")"<<std::endl;
}

void push(polygon& p, int * pt)
{
	p.outer().push_back(point(pt[0],pt[1]));
}

polygon makePolygon(int width, int height) {
	//ramdom num point
	int num = rand() % 2 + 3; //3 hoac 4 diem

	polygon p;
		
	int first[2];
	first[0] = rand() % width;
	first[1] = rand() % height;

	push(p,first);

	int second[2];
	second[0] = first[0] + 10 % rand() + 10;
	second[1] = first[1] + 10 % rand() + 10;

	push(p,second);

	int third[2];
	third[0] = first[0] - 10 % rand() - 10;
	third[1] = first[1] + 10 % rand() + 10;

	push(p,third);

	printPoint(first);
	printPoint(second);
	printPoint(third);

	if (num == 4);
	{
		int forth[2];
		forth[0] = third[0] - 10 % rand() + 0;
		forth[1] = third[1] + 10 % rand() + 10;
		push(p,forth);
	}

	std::cout << bg::wkt<polygon>(p) << std::endl;

	return p;
}

int _main()
{
	makePolygon(256, 256);
}