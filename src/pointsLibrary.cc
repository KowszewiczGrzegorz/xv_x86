#include <stdint.h>
#include "pointsLibrary.hh"

static const double PI_180 = 3.1415926535/180.0;

PointsLibrary::PointsLibrary() {}
PointsLibrary::~PointsLibrary() {};


point_t PointsLibrary::p2c (int angle, int distance)
{
    point_t p;
    p.x = distance * cos(angle*PI_180);
    p.y = distance * sin(angle*PI_180);
    return p;
}


double PointsLibrary::distance2line(line_t l, point_t p)
{
    return (fabs(l.a*p.x + l.b*p.y + l.c) / sqrt(l.a*l.a + l.b*l.b));
}


line_t PointsLibrary::getLineEquation(point_t a, point_t b)
{
    line_t l;
    l.a = b.y - a.y;
    l.b = b.x - a.x;
    l.c = (a.x*b.y - b.x*a.x);
    return l;
}


vector<line_t> PointsLibrary::ropeAlgorithm(vector<point_t> points)
{
    return ropeAlgorithm(points, 0, points.size()-1, 0);
}

void PointsLibrary::crl(uint32_t rl) 
{
    for (uint32_t i = 0; i < rl; i++)
        cerr << "    ";
}


vector<line_t> PointsLibrary::ropeAlgorithm(vector<point_t> points, uint32_t start, uint32_t end, uint32_t rl)
{
    vector<line_t> lines;
    line_t l;
    int maxDistanceIndex = 0;
    uint32_t i;
    double distance;
    double maxDistance = 0.0;

    if (rl > 0)
        return lines;
    
    crl(rl); cerr << "ropeAlgorithm(points, " << start << ", " << end << ")" << endl;

    l = getLineEquation(points[start], points[end]);
    crl(rl); cerr << "line equation : " << l.a << "*x + " << l.b << "*y + " << l.c << endl;
    for (i = start+1; i < end-1; i++) {
        distance = distance2line(l, points[i]);
        crl(rl); cerr << "[" << points[i].x << ", " << points[i].y << "] has distance = " << distance << endl;
        if (distance > maxDistance) {
            maxDistance = distance;
            maxDistanceIndex = i;
        }
    }

    crl(rl); cerr << "got MaxDistance=" << maxDistance << " @  " << maxDistanceIndex << endl;
             
    if (maxDistance > m_maxDistanceRopeAlgorithm) {
        vector<line_t> newLines;
        if (maxDistanceIndex-1-start > 3) {
            crl(rl); cerr << "--- start recursion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
            lines = ropeAlgorithm(points, start, maxDistanceIndex-1, rl+1);
            crl(rl); cerr << "--- end recursion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
        } else {
            crl(rl); cerr << "no recurion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
        }
        if (end-maxDistanceIndex-1 > 3) {
            crl(rl); cerr << "--- start recursion on [" << (maxDistanceIndex+1) << "," << end << "]" << endl;
            newLines = ropeAlgorithm(points, maxDistanceIndex+1, end, rl+1);
            crl(rl); cerr << "--- end recursion on [" << (maxDistanceIndex+1) << "," << end << "]" << endl;
            lines.insert(lines.end(), newLines.begin(), newLines.end());
        } 
    } else {
        lines.push_back(l);
        crl(rl); cerr << "pushing Line " << i << " = " << lines[i].a << "*x + " << lines[i].b << "*y + " << lines[i].c << endl;
    }

    crl(rl); cerr << "end of ropeAlgorithm(points, " << start << ", " << end << ")" << endl;

    return lines;
}

