
#ifndef __ODOMETRY_HH
#define __ODOMETRY_HH

typedef struct {
    double x, y, theta;
} position_t;

class Odometry
{
public:
    Odometry(double);
    ~Odometry();

    void init();
    void init(position_t);
    void update(double, double);
    position_t getCurrentPosition() { return m_currentPosition; };
    
private:
    position_t m_currentPosition;
    double m_prevDistLeftWheel;
    double m_prevDistRightWheel;
    double m_wheelSpacing;
};

#endif /* __ODOMETRY_HH */
