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

const int WIDTH = 400;
const int HEIGHT = 400;
const int NUMPOLYGON = 100;
const int NUMNEAREST = 1;
const int NUMTHREAD = 4;
const int maxRadius = 21;

auto RED = cv::Scalar(0, 0, 255, 255);
auto GREEN = cv::Scalar(0, 255, 0, 255);
auto BLUE = cv::Scalar(255, 0, 0, 255);

void findCorner(int& xmin, int& ymin, int& xmax, int& ymax, box& b, int index)
{
    int distance = WIDTH / NUMTHREAD;

    xmin = index * distance;
    ymin = 0;

    xmax = index * distance + distance - 1 + maxRadius;
    if (xmax > WIDTH - 1) xmax = WIDTH - 1;
    ymax = HEIGHT - 1;


    int x = index * distance + distance - 1;
    if (x > WIDTH - 1) x = WIDTH - 1;

    b = box(point(xmin, ymin), point(x, ymax));
}

void DrawPolygon2(int index, RTREE& rtree2, std::vector<polygon>& polygons, int pmap[][HEIGHT])
{
    int xmin, ymin, xmax, ymax;
    box b;
    findCorner(xmin, ymin, xmax, ymax, b, index);

    std::vector<value> result;
    rtree2.query(bgi::intersects(b), std::back_inserter(result));

    RTREE rtree(result.begin(), result.end());

    // std::this_thread::sleep_for(std::chrono::seconds(index));
    // std::cout << bg::wkt<box>(b) << std::endl;


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

    // //random color
    // std::vector<int> colors;
    // int size = polygons.size();
    // for ( int i = 0 ; i < size ; ++i )
    // {
    //     colors.push_back(i % 3 + 1);
    // }

    // //Draw pixel
    // // t1 = std::chrono::high_resolution_clock::now();
    // cv::Mat mat(HEIGHT, WIDTH, CV_8UC4);
    // mat = cv::Scalar(0, 0, 0, 0);


    // for (int i = 0; i < WIDTH; i++)
    //     for (int j = 0; j < HEIGHT; j++)
    //     {
    //         if (pmap[i][j] != -1) {
    //             int c = colors[pmap[i][j]];
    //             auto color = c == 1 ? RED : c == 2 ? GREEN : BLUE;
    //             line( mat, cv::Point(i, j), cv::Point(i, j), color, 2, 8);
    //         }
    //     }

    // line(mat, cv::Point(10, 10), cv::Point(251, 240), cv::Scalar(0, 0, 0, 255), 1, 8);


    // std::vector<int> compression_params;
    // compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(9);

    // std::string filename = "DrawPolygon7_" + std::to_string(index) + ".png";

    // imwrite(filename, mat, compression_params);
}

void Draw(int index, RTREE& rtree, std::vector<polygon>& polygons, int pmap[][HEIGHT])
{
    int distance = WIDTH / NUMTHREAD;
    int x = index * distance + distance - 1;

    if (index == NUMTHREAD - 1) x = WIDTH - 1;

    box b(point(x, 0), point(x, HEIGHT - 1));

    //std::this_thread::sleep_for (std::chrono::seconds(index));

    std::cout << bg::wkt<box>(b) << "......" << std::endl;

    std::vector<value> result;

    rtree.query(bgi::intersects(b), std::back_inserter(result));
    // rtree.query(bgi::intersects(b[1]), std::back_inserter(result[1]));
    // rtree.query(bgi::intersects(b[2]), std::back_inserter(result[2]));
    // rtree.query(bgi::intersects(b[3]), std::back_inserter(result[3]));
    std::cout << result.size() << "..." << std::endl;
}

int main()
{
    int pmap[NUMTHREAD][WIDTH][HEIGHT];
    //fill all member = -1
    memset(pmap, -1, sizeof(pmap) );

    // polygons
    std::vector<polygon> polygons;
    std::vector<value> values;

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

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    // fill the spatial index
    for ( int i = 0 ; i < polygons.size() ; ++i )
    {
        // calculate polygon bounding box
        box b = bg::return_envelope<box>(polygons[i]);
        // insert new value
        values.push_back(std::make_pair(b, i));
    }

    RTREE rtree(values.begin(), values.end());

    std::thread threads[NUMTHREAD];

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

    //concatenate
    int mapresult[WIDTH][HEIGHT];
    memset(mapresult,-1,sizeof(mapresult));

    for (int index = NUMTHREAD - 1; index >= 0; index--)
    {
        int xmin, ymin, xmax, ymax;
        box b;
        findCorner(xmin, ymin, xmax, ymax, b, index);
        for (int i = xmin; i < xmax; i++)
        {
            for (int j = ymin; j < ymax; j++)
            {
                int id = pmap[index][i][j];
                if (id != -1)
                {
                    mapresult[i][j] = id;
                }
            }
        }
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration : " << duration << std::endl;


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
            if (mapresult[i][j] != -1) {            
                int c = colors[mapresult[i][j]];
                auto color = c == 1 ? RED : c == 2 ? GREEN : BLUE;
                line( mat, cv::Point(i, j), cv::Point(i, j), color, 2, 8);
            }
        }

    line(mat, cv::Point(10, 10), cv::Point(251, 240), cv::Scalar(0, 0, 0, 255), 1, 8);


    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    imwrite("DrawPolygon7.png", mat, compression_params);

    return 0;
}
