/**
 * Trig.h header file for trig calculations
 * Pass in one or two beacons seen, the distance to them and the offset from the centre in radians
 */

#pragma once

#include <geometry_msgs/PoseWithCovariance.h>
#include <assign1_2013/beacons.h>
class trig {
private:
	geometry_msgs::PoseWithCovariance prev;
	void getPoint(geometry_msgs::Point *ret, geometry_msgs::Point left, long rLeft,
	geometry_msgs::Point right, long rRight);
	void getOrientation(geometry_msgs::Quaternion *ret, geometry_msgs::Point *point,
	geometry_msgs::Point left, float aLeft);
	boost::array<double, 36ul> setCovariance(double val);
	boost::array<double, 36ul> increaseCov(boost::array<double, 36ul> prev);

	public:
		trig();
		void getVoPose(geometry_msgs::PoseWithCovariance *ret, comp3431::Beacon *left, long dLeft, float aLeft,
	comp3431::Beacon *right, long dRight, float aRight);
};
