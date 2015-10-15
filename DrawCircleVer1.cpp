#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> value;
typedef bgi::rtree< value, bgi::rstar<32> > RTREE;

#include <chrono>
#include <iostream>
#include <vector>

const int WIDTH = 2560;
const int HEIGHT = 2560;
const int RADIUS = 10;
const int NUMCIRCLE = 100;

using namespace cv;
using namespace std;

/**
 * @function main
 * @brief Main function
 */


auto RED = Scalar( 0, 0, 255, 255 );
auto GREEN = Scalar( 0, 255 , 0, 255  );
auto BLUE = Scalar( 255, 0, 0, 255  );

struct Cycle
{
	int x;
	int y;
	Scalar color;
	int index;
};

struct Pixel
{
	int x;
	int y;
	Scalar color;
};

bool mycomp (value& v1, value& v2) { return (v1.second < v2.second); }

int main( void )
{
	vector<Cycle> cycles;
	vector<value> points;
	vector<Pixel> pixels;

//make circle list
	for (int i = 0; i < NUMCIRCLE; i++) {
		Cycle c;
		c.x = rand() % (WIDTH + 1);
		c.y = rand() % (HEIGHT + 1);
		c.color = (i % 3 == 0) ? RED : (i % 3 == 1) ? GREEN : BLUE;
		c.index = i;
		cycles.emplace_back(c);

		point p = point(c.x, c.y);
		points.emplace_back(std::make_pair(p, i));
	}

	//insert points to rtree
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	RTREE rtree( points.begin(), points.end() );
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
	std::cout << "duration insert to rtree: " << duration << std::endl;

	//
	std::vector<value> returned_values;
	int count = 0;

	t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
		{
			point sought = point(i, j);

			for ( RTREE::const_query_iterator it = rtree.qbegin(bgi::nearest(sought, NUMCIRCLE) ) ;
			        it != rtree.qend() ; ++it )
			{

				if (bg::distance(it->first, sought) > RADIUS)
				{
					//count++;
					//cout<<returned_values.size()<<"...."<<count<<endl;
					break;
				}

				returned_values.push_back(*it);
			}

			int size = returned_values.size();
			if (size > 0)
			{
				//cout<<returned_values.size()<<endl;
				std::sort(returned_values.begin(), returned_values.end(), mycomp);
				Cycle ctopLayer = cycles[returned_values[size - 1].second];

				Pixel p;
				p.x = i;
				p.y = j;
				p.color = ctopLayer.color;

				pixels.emplace_back(p);
				returned_values.clear();
			}
		}
	std::cout << "count" << count << endl;
	std::cout << pixels.size() << "........\n";

	t2 = std::chrono::high_resolution_clock::now();
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
	std::cout << "duration : " << duration << std::endl;



	Mat mat(WIDTH, HEIGHT, CV_8UC4);
	mat = cv::Scalar(0, 0, 0, 0);

	t1 = std::chrono::high_resolution_clock::now();
	for (std::vector<Pixel>::iterator it = pixels.begin() ; it != pixels.end(); ++it)
	{
		Vec4b& rgba = mat.at<Vec4b>(it->x, it->y);
		line( mat, Point(it->x, it->y ), Point(it->x, it->y ), it->color ,  1, 8 );
	}

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);


	imwrite("DrawCircleVer1.png", mat, compression_params);

	t2 = std::chrono::high_resolution_clock::now();
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
	std::cout << "duration draw: " << duration << std::endl;

	return 0;

}
