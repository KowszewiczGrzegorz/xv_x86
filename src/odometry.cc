#include <math.h>
#include "odometry.hh"

Odometry::Odometry(double wheelSpacing)
{
    m_wheelSpacing = wheelSpacing;
    init();
}

Odometry::~Odometry()
{}

void Odometry::init(void)
{
    position_t pos;
    pos.x = pos.y = pos.theta = 0.0;
    init(pos);
}

void Odometry::init(position_t pos)
{
    m_currentPosition = pos;
}

void Odometry::update(double distLeftWheel, double distRightWheel)
{
    double diffLeftWheel = distLeftWheel - m_prevDistLeftWheel;
    double diffRightWheel = distRightWheel - m_prevDistRightWheel;

    double dTheta = (diffLeftWheel - diffRightWheel) / m_wheelSpacing;
    double dDelta = (diffLeftWheel + diffRightWheel) / 2.0;

    double dX = dDelta * cos(m_currentPosition.theta + (dTheta/2.0));
    double dY = dDelta * sin(m_currentPosition.theta + (dTheta/2.0));

    m_currentPosition.x += dX;
    m_currentPosition.y += dY;
    m_currentPosition.theta += dTheta;

    m_prevDistLeftWheel = distLeftWheel;
    m_prevDistRightWheel = distRightWheel;
}

