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

line_t PointsLibrary::linearRegression(vector<point_t> points)
{
    line_t line;

    double x_ = 0; // moyenne des x
    double y_ = 0; // moyenne des y 
    double x2_ = 0; // moyenne des carres x
    double y2_ = 0; // moyenne des carres x
    double xy_ = 0;

    double s2x = 0; // variance
    double s2y = 0;
    double sxy = 0; // covariance
    double sx = 0;
    double sy = 0;

    double k = 0;
    uint32_t n = points.size();
    
    for (uint32_t i = 0; i < n; i++) {
        x_ += points[i].x;
        y_ += points[i].y;
        x2_ += points[i].x * points[i].x;
        y2_ += points[i].y * points[i].y;
        xy_ += points[i].x * points[i].y;            
    }
    k = 1.0 / n;

    x_ *= k;
    y_ *= k;
    x2_ *= k;
    y2_ *= k;
    xy_ *= k;

    s2x = x2_ - x_*x_;
    sxy = xy_ - x_*y_;
    s2y = y2_ - y_*y_;
    sx = sqrt(s2x);
    sy = sqrt(s2y);
        
    if (s2x != 0 && 0 != (sx*sy)) {
        double a = sxy / s2x;
        double b = y_ - a*x_;

        line.a = a;
        line.b = -1;
        line.c = b;
    }

    return line;
}


line_t PointsLibrary::getLineEquation(vector<point_t> points, double threshold)
{
    line_t l;
    uint32_t nb;

    /*
    point_t a = points[0];
    point_t b = points[points.size()-1];

    l.a = b.y - a.y;
    l.b = b.x - a.x;
    l.c = (a.x*b.y - b.x*a.x);
    */

    if (points.size() > 5) {
        l = linearRegression(points);
        nb = 0;

        vector<point_t> linRegPoints;
        for (uint32_t i = 0; i < points.size(); i++) 
            if (distance2line(l, points[i]) < threshold) {
                linRegPoints.push_back(points[i]);
                nb++;
            }
    
        if (nb != points.size() && linRegPoints.size() > 5)
            l = linearRegression(linRegPoints);
    }

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

    if (rl > 0 || points.size() < 5)
        return lines;
    
    crl(rl); cerr << "ropeAlgorithm(points, " << start << ", " << end << ")" << endl;

    l = getLineEquation(points, m_maxDistanceRopeAlgorithm);
    // crl(rl); cerr << "line equation : " << l.a << "*x + " << l.b << "*y + " << l.c << endl;
    for (i = start+1; i < end-1; i++) {
        distance = distance2line(l, points[i]);
        // crl(rl); cerr << "[" << points[i].x << ", " << points[i].y << "] has distance = " << distance << endl;
        if (distance > maxDistance) {
            maxDistance = distance;
            maxDistanceIndex = i;
        }
    }

    // crl(rl); cerr << "got MaxDistance=" << maxDistance << " @  " << maxDistanceIndex << endl;
             
    if (maxDistance > m_maxDistanceRopeAlgorithm) {
        vector<line_t> newLines;
        if (maxDistanceIndex-1-start > 3) {
            // crl(rl); cerr << "--- start recursion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
            lines = ropeAlgorithm(points, start, maxDistanceIndex-1, rl+1);
            // crl(rl); cerr << "--- end recursion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
        } else {
            // crl(rl); cerr << "no recurion on [" << start << "," << (maxDistanceIndex-1) << "]" << endl;
        }
        if (end-maxDistanceIndex-1 > 3) {
            // crl(rl); cerr << "--- start recursion on [" << (maxDistanceIndex+1) << "," << end << "]" << endl;
            newLines = ropeAlgorithm(points, maxDistanceIndex+1, end, rl+1);
            // crl(rl); cerr << "--- end recursion on [" << (maxDistanceIndex+1) << "," << end << "]" << endl;
            lines.insert(lines.end(), newLines.begin(), newLines.end());
        } 
    } else {
        lines.push_back(l);
        crl(rl); cerr << "pushing Line " << i << " = " << lines[i].a << "*x + " << lines[i].b << "*y + " << lines[i].c << endl;
    }

    crl(rl); cerr << "end of ropeAlgorithm(points, " << start << ", " << end << ")" << endl;

    return lines;
}


vector <vector<point_t> > PointsLibrary::scanSegmentation(vector<point_t> points, double threshold)
{
    vector <vector<point_t> > clusters;
    vector<point_t> cluster;
    uint32_t cp = 0;
    double distance;

    while (cp < points.size()) {
        cluster.clear();
        cluster.push_back(points[cp++]);

        while (cp < points.size()) {
            distance = sqrt(pow((points[cp].x - points[cp-1].x), 2)
                            + pow((points[cp].y - points[cp-1].y), 2));

            if (distance <= threshold) {
                cluster.push_back(points[cp++]);
            } else {
                clusters.push_back(cluster);
                break;
            }
        }
    }

    return clusters;
}
