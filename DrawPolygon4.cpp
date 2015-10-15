//

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "MakePolygon.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <vector>
#include <chrono>

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

auto RED = cv::Scalar(0, 0, 255, 255);
auto GREEN = cv::Scalar(0, 255, 0, 255);
auto BLUE = cv::Scalar(255, 0, 0, 255);

int main()
{
    int pmap[WIDTH][HEIGHT];
    //fill all member = -1
    memset(pmap, -1, sizeof(pmap) );

    // for(int i=0;i<WIDTH;i++)
    // {
    //     for(int j=0;j<HEIGHT;j++)
    //     {
    //         std::cout<<pmap[i][j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }

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

    RTREE rtree(values.begin(), values.end());
    // int count = 0;
    //map pixel with polygon
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
        {
            if (pmap[i][j] == -1)
            {
                point sought = point(i, j);
                std::vector<value> result_n;

                rtree.query(bgi::nearest(sought, NUMNEAREST), std::back_inserter(result_n));

                for (int k = 0; k < NUMNEAREST; k++)
                {
                    int id = result_n[k].second;
                    auto& poly = polygons[id];

                    if (bg::within(sought, poly))
                    {
                        // printf("-------------------\n");
                        // printf("id = %d\n", id);

                        pmap[i][j] = id;
                        auto box = result_n[0].first;
                        // std::cout << "BOX = " << bg::wkt(box) << std::endl;
                        point pmin = box.min_corner();
                        point pmax = box.max_corner();

                        // std::cout << bg::wkt(poly) << std::endl;

                        int xmin = pmin.get<0>();
                        int ymin = pmin.get<1>();
                        int xmax = pmax.get<0>();
                        int ymax = pmax.get<1>();

                        // std::cout << bg::wkt<polygon>(polygons[id]) << std::endl;
                        // std::cout << xmin << ", " << ymin << "    " << xmax << ", " << ymax<<std::endl;


                        // std::cout << pmin.get<0>() << ", " << pmin.get<1>() << "    " << pmax.get<0>() << ", " << pmax.get<1>()
                        // << std::endl;                        

                        for (int w = xmin; w <= xmax; w++)
                            for (int h = ymin; h <= ymax; h++)
                            {
                                if (w < 0 || h < 0 || w >= WIDTH || h >= HEIGHT)
                                    continue;
                                if (bg::within(point(w, h), poly))
                                {
                                    pmap[w][h] = id;
                                }
                            }

                        break;    
                    }                    
                }
            }
        }

    // std::cout << count << "......" << std::endl;

    //random color
    std::vector<int> colors;
    for ( int i = 0 ; i < polygons.size() ; ++i )
    {
        colors.push_back(i % 3 + 1);
    }

    //Draw pixel
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    cv::Mat mat(HEIGHT, WIDTH, CV_8UC4);
    mat = cv::Scalar(0, 0, 0, 0);


    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
        {
            if (pmap[i][j] != -1) {
                int c = colors[pmap[i][j]];
                auto color = c == 1 ? RED : c == 2 ? GREEN : BLUE;
                line( mat, cv::Point(i, j), cv::Point(i, j), color, 1, 8);
            }
        }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration DrawPolygon: " << duration << std::endl;

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);


    imwrite("DrawPolygon4.png", mat, compression_params);


// find 5 nearest values to a point
    std::vector<value> result_n;
    rtree.query(bgi::nearest(point(0, 0), 1), std::back_inserter(result_n));

// note: in Boost.Geometry the WKT representation of a box is polygon

// note: the values store the bounding boxes of polygons
// the polygons aren't used for querying but are printed


    std::cout << "knn query point:" << std::endl;
    std::cout << bg::wkt<point>(point(0, 0)) << std::endl;
    std::cout << "knn query result:" << std::endl;
    BOOST_FOREACH(value const & v, result_n)
    std::cout << bg::wkt<polygon>(polygons[v.second]) << " - " << v.second << std::endl;

    return 0;
}
