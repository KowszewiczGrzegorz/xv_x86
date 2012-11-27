
#ifndef __POINTS_LIBRARY_H__
#define __POINTS_LIBRARY_H__

#include <vector>
#include <math.h>
#include "xv25.hh"


typedef struct {
    double x, y;
} point_t;

typedef struct {
    double a, b, c;
} line_t;

class PointsLibrary {
public:
    PointsLibrary();
    ~PointsLibrary();
    
    point_t p2c(int, int);

    line_t linearRegression(vector<point_t> points);
    vector <vector<point_t> > scanSegmentation(vector<point_t> points, double threshold);

    void setMaxDistanceRopeAlgorithm (double value) { m_maxDistanceRopeAlgorithm = value; };
    vector<line_t> ropeAlgorithm(vector<point_t>);
    vector<line_t> ropeAlgorithm(vector<point_t>, uint32_t, uint32_t, uint32_t);

private:
    double distance2line(line_t, point_t);
    line_t getLineEquation(vector<point_t>, double);
    void crl(uint32_t);
    
    double m_maxDistanceRopeAlgorithm;
};


#endif /* __POINTS_LIBRARY_H__ */
