#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <thread>

#include "MakePolygon.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
typedef std::pair<box, unsigned> value;
typedef bgi::rtree< value, bgi::rstar<16, 4> > RTREE;

const int WIDTH = 300;
const int HEIGHT = 300;
const int NUMPOLYGON = 100000;
const int NUMNEAREST = 1;
const int NUMTHREAD = 4;
const int maxRadius = 0;

auto RED = cv::Scalar(0, 0, 255, 255);
auto GREEN = cv::Scalar(0, 255, 0, 255);
auto BLUE = cv::Scalar(255, 0, 0, 255);

void findCorner(int& xmin, int& ymin, int& xmax, int& ymax, int index)
{
    int midWidth = WIDTH >> 1;//int midWidth = WIDTH / 2;
    // std::cout<<midWidth<<"_______________"<<std::endl;
    int midHeight = HEIGHT >> 1; //int midHeight = HEIGHT / 2;
    // std::cout<<midHeight<<"_______________"<<std::endl;
    if (index == 0)
    {
        xmin = 0; xmax = midWidth + maxRadius;
        ymin = 0; ymax = midHeight + maxRadius;
    }
    else if (index == 1)
    {
        // b[0] = box(point(0, 0), point(midWidth, midHeight));
        // b[1] = box(point(0, midHeight + 1), point(midWidth, HEIGHT));
        // b[2] = box(point(midWidth + 1, midHeight + 1), point(WIDTH, HEIGHT));
        // b[3] = box(point(midWidth + 1, 0), point(WIDTH, midHeight));
        xmin = 0;
        ymin = midHeight + 1;

        xmax = midWidth+maxRadius;
        ymax = HEIGHT - 1;
    }
    else if (index == 2)
    {
        xmin = midWidth + 1;
        ymin = midHeight + 1;

        xmax = WIDTH - 1;
        ymax = HEIGHT - 1;
    }
    else if (index == 3)
    {
        xmin = midWidth + 1;
        ymin = 0;

        xmax = WIDTH - 1;
        ymax = midHeight + maxRadius;
    }
}

void DrawPolygon2(int index, RTREE& rtree, std::vector<polygon>& polygons, int pmap[][HEIGHT])
{
    // RTREE rtree(values.begin(), values.end());

    int xmin, ymin, xmax, ymax;
    findCorner(xmin, ymin, xmax, ymax, index);

    // std::cout<<"index = "<<index<<"min_corner = "<<xmin<<","<<ymin<<"  max_corner = "<<xmax<<","<<ymax<<std::endl;
    // std::cout<<"polygon size: "<<polygons.size()<<std::endl;  

    for (int i = xmin; i <= xmax; i++)
        for (int j = ymin; j <= ymax; j++)
        {
            if (pmap[i][j] == -1)
            {
                point sought = point(i, j);
                std::vector<value> result_n;

                for ( RTREE::const_query_iterator it = qbegin(rtree, bgi::nearest(sought, 100)) ;
                        it != qend(rtree) ; ++it )
                {
                    int id = it->second;
                    auto& poly = polygons[id];
                    auto box = it->first;

                    if (!bg::within(sought, box))
                        break;

                    if ( bg::within(sought, poly) ) {
                        // std::cout<<"**********************\n";
                        // pmap[i][j] = id;

                        point pmin = box.min_corner();
                        point pmax = box.max_corner();

                        int xmin2 = pmin.get<0>();
                        int ymin2 = pmin.get<1>();
                        int xmax2 = pmax.get<0>();
                        int ymax2 = pmax.get<1>();

                        for (int w = xmin2; w <= xmax2; w++) {
                            for (int h = ymin2; h <= ymax2; h++)
                            {
                                if (w < xmin || h < ymin || w >= xmax || h >= ymax || pmap[w][h] >= 0)
                                    continue;
                                if (bg::within(point(w, h), poly))
                                {
                                    pmap[w][h] = id;
                                }
                            }
                        }

                        break;
                    }
                }

            }
        }



    //random color
    std::vector<int> colors;
    int size = polygons.size();
    for ( int i = 0 ; i < size ; ++i )
    {
        colors.push_back(i % 3 + 1);
    }

    //Draw pixel
    // t1 = std::chrono::high_resolution_clock::now();
    cv::Mat mat(HEIGHT, WIDTH, CV_8UC4);
    mat = cv::Scalar(0, 0, 0, 0);


    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
        {
            if (pmap[i][j] != -1) {
                int c = colors[pmap[i][j]];
                auto color = c == 1 ? RED : c == 2 ? GREEN : BLUE;
                line( mat, cv::Point(i, j), cv::Point(i, j), color, 2, 8);
            }
        }

    line(mat, cv::Point(10, 10), cv::Point(251, 240), cv::Scalar(0, 0, 0, 255), 1, 8);

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    std::string filename = "DrawPolygon_"+std::to_string(index)+".png";

    imwrite(filename, mat, compression_params);
}

int main()
{
    int pmap[4][WIDTH][HEIGHT];
    //fill all member = -1
    memset(pmap, -1, sizeof(pmap) );

    // polygons
    std::vector<polygon> polygons;
    std::vector<value> values;

    // create some polygons
    for ( int i = 0 ; i < 10 ; ++i )
    {
        // create a polygon
        polygon p;
        for ( float a = 0 ; a < 6.28316f ; a += 1.04720f )
        {
            int x = 30 * i + int(20 *::cos(a));
            int y = 30 * i + int(20 *::sin(a));
            p.outer().push_back(point(x, y));
        }
        // add polygon
        bg::correct(p);
        polygons.push_back(p);
    }

    for (int i = 0; i < NUMPOLYGON; i++)
    {
        polygon p = makePolygon(WIDTH, HEIGHT);
        bg::correct(p);
        polygons.push_back(p);
    }

    //std::cout<<polygons.size()<<std::endl;

    // display polygons
    // std::cout << "generated polygons:" << std::endl;
    // BOOST_FOREACH(polygon const & p, polygons)
    // std::cout << bg::wkt<polygon>(p) << std::endl;

    // fill the spatial index
    for ( int i = 0 ; i < polygons.size() ; ++i )
    {
        // calculate polygon bounding box
        box b = bg::return_envelope<box>(polygons[i]);
        // insert new value
        values.push_back(std::make_pair(b, i));
    }

    // for(int i=0;i<values.size();i++)
    // {
    //     std::cout<<values[i].second<<".\n";
    // }

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    RTREE rtree(values.begin(), values.end());

    int midWidth = WIDTH / 2;
    int midHeight = HEIGHT / 2;

    box b[4];
    b[0] = box(point(0, 0), point(midWidth, midHeight));
    b[1] = box(point(0, midHeight + 1), point(midWidth, HEIGHT));
    b[2] = box(point(midWidth + 1, midHeight + 1), point(WIDTH, HEIGHT));
    b[3] = box(point(midWidth + 1, 0), point(WIDTH, midHeight));

    std::vector<value> result[4];
    
    rtree.query(bgi::intersects(b[0]), std::back_inserter(result[0]));
    rtree.query(bgi::intersects(b[1]), std::back_inserter(result[1]));
    rtree.query(bgi::intersects(b[2]), std::back_inserter(result[2]));
    rtree.query(bgi::intersects(b[3]), std::back_inserter(result[3]));
    

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration : " << duration << std::endl;

    // std::cout<<result[0].size()<<"........................."<<std::endl;
    // std::cout<<result[1].size()<<"........................."<<std::endl;
    // std::cout<<result[2].size()<<"........................."<<std::endl;
    // std::cout<<result[3].size()<<"........................."<<std::endl;

    std::thread threads[NUMTHREAD];

    t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUMTHREAD; i++)
    {
        threads[i] = std::thread([i, &rtree, &polygons, &pmap]() {
            DrawPolygon2(i, rtree, polygons, pmap[i]);
        });
    }


    for (int i = 0; i < NUMTHREAD; i++)
    {
        threads[i].join();
    }

    t2 = std::chrono::high_resolution_clock::now();
    duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration : " << duration << std::endl;

    return 0;
}
