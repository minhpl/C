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

const int WIDTH = 400;
const int HEIGHT = 400;
const int NUMPOLYGON = 100;
const int NUMNEAREST = 1;
const int maxsize = 20;

auto RED = cv::Scalar(0, 0, 255, 255);
auto GREEN = cv::Scalar(0, 255, 0, 255);
auto BLUE = cv::Scalar(255, 0, 0, 255);


std::vector<cv::Point> vertexs;


template <typename Point>
void list_coordinates(Point const& p)
{
    using boost::geometry::get;
    vertexs.push_back(cv::Point(get<0>(p), get<1>(p)));
    std::cout << "x = " << get<0>(p) << " y = " << get<1>(p) << std::endl;
};

void drawPolygon(std::vector<cv::Point>& vertexs, cv::Mat& mat)
{
    int size = vertexs.size();
    for (int i = 0; i < size; i++)
    {
        std::cout << "dsdsdsdsdsds";
        cv::Point p1 = vertexs[i];
        cv::Point p2 = vertexs[(i + 1) % size];
        line( mat, p1, p2, cv::Scalar(0, 0, 0, 255), 2, 8);
    }
    vertexs.clear();
}

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
    // for ( int i = 0 ; i < 10 ; ++i )
    // {
    //     // create a polygon
    //     polygon p;
    //     for ( float a = 0 ; a < 6.28316f ; a += 1.04720f )
    //     {
    //         int x = 30 * i + int(20 *::cos(a));
    //         int y = 30 * i + int(20 *::sin(a));
    //         p.outer().push_back(point(x, y));
    //     }
    //     // add polygon
    //     bg::correct(p);
    //     polygons.push_back(p);
    // }

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

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    RTREE rtree(values.begin(), values.end());
    //map pixel with polygon
    
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
        {
            if (pmap[i][j] == -1)
            {
                point sought = point(i, j);
                std::vector<value> result_n;

                for ( RTREE::const_query_iterator it = qbegin(rtree, bgi::nearest(sought, 100)) ;
                        it != qend(rtree) ; ++it )
                {
                    //do something with value
                    int id = it->second;
                    auto& poly = polygons[id];
                    auto box = it->first;

                    if (!bg::within(sought, box))
                        break;

                    if ( bg::within(sought, poly) ) {

                        point pmin = box.min_corner();
                        point pmax = box.max_corner();                    

                        int xmin = pmin.get<0>();
                        int ymin = pmin.get<1>();
                        int xmax = pmax.get<0>();
                        int ymax = pmax.get<1>();

                        for (int w = xmin; w <= xmax; w++) {
                            for (int h = ymin; h <= ymax; h++)
                            {
                                if (w < 0 || h < 0 || w >= WIDTH || h >= HEIGHT || pmap[w][h] >= 0)
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

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration : " << duration << std::endl;

    //random color
    std::vector<int> colors;
    for ( int i = 0 ; i < polygons.size() ; ++i )
    {
        colors.push_back(i % 3 + 1);
    }

    //Draw pixel
    t1 = std::chrono::high_resolution_clock::now();
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

    t2 = std::chrono::high_resolution_clock::now();
    duration = (float)(std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count()) / 1000;
    std::cout << "duration DrawPolygon: " << duration << std::endl;

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

// find 5 nearest values to a point
    std::vector<value> result_n;
    point pointtest(251, 240);
    rtree.query(bgi::nearest(pointtest, 1), std::back_inserter(result_n));

    int id = result_n[0].second;
    auto& poly = polygons[id];
    bg::for_each_point(poly, list_coordinates<point>);

    drawPolygon(vertexs, mat);

    imwrite("DrawPolygon3.png", mat, compression_params);

    return 0;
}
