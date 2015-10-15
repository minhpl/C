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
#include <map>

const int WIDTH = 256;
const int HEIGHT = 256;
const int RADIUS = 10;
const int NUMCIRCLE = 100;

// typedef int[HEIGHT][WIDTH] PointMap;

using namespace cv;
using namespace std;

/**
 * @function main
 * @brief Main function
 */

auto RED = Scalar(0, 0, 255, 255);
auto GREEN = Scalar(0, 255, 0, 255);
auto BLUE = Scalar(255, 0, 0, 255);

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
	int id;
	Scalar color;
};

struct Point2d
{
	int x;
	int y;
};

int main( void )
{
	vector<Cycle> cycles;
	vector<value> points;
	vector<Pixel> pixels;

	int pmap[WIDTH][HEIGHT];
	memset(pmap, -1, sizeof(pmap) );

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

	t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
		{
			if (pmap[i][j] != -1) {
				Pixel p;
				p.x = i;
				p.y = j;
				int id = pmap[i][j];
				Cycle& ctopLayer = cycles[id];
				p.color = ctopLayer.color;
				pixels.emplace_back(p);
				continue;
			}
			else{}
			point sought = point(i, j);

			for ( RTREE::const_query_iterator it = rtree.qbegin(bgi::nearest(sought, 1) ) ;
			        it != rtree.qend() ; ++it )
			{
				if (bg::distance(it->first, sought) > RADIUS)
				{
					break;
				}
				returned_values.push_back(*it);
			}

			int size = returned_values.size();
			if (size > 0)
			{
				int id = returned_values[0].second;
				Cycle& ctopLayer = cycles[id];

				Pixel p;
				p.x = i;
				p.y = j;
				p.color = ctopLayer.color;

				pixels.emplace_back(p);
				returned_values.clear();
				pmap[i][j] = id;

				for (int w = i+1; w < ctopLayer.x + RADIUS; w++)
					for (int h = j+1; h < ctopLayer.y + RADIUS; h++)
					{
						if (w>=WIDTH || h>=HEIGHT) continue;
						if(bg::distance(point(w,h),point(ctopLayer.x,ctopLayer.y)) <= RADIUS)
						{
							pmap[w][h] = id;
						}
					}
				//
			}
		}

	std::cout << pixels.size() << "........\n";

	t2 = std::chrono::high_resolution_clock::now();
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
	std::cout << "duration : " << duration << std::endl;


	//DrawImage
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


	imwrite("DrawCricleVer2.png", mat, compression_params);

	t2 = std::chrono::high_resolution_clock::now();
	duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
	std::cout << "duration draw: " << duration << std::endl;

	return 0;

}
