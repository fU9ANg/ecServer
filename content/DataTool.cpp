/*
 * DataTool.cpp
 *
 *  Created on: 2013-5-2
 *      Author: sxkj1
 */

#include "DataTool.h"
#include <string>
using namespace std;

DataTool::DataTool(long long data) {
	m_data = data;
	m_sdata = getString();
	m_length = m_sdata.length();
	//m_pic=getPicture();
}

DataTool::DataTool(Picture pic) :
		m_pic(pic), m_data(0) {

}

DataTool::~DataTool() {

}

float DataTool::getX() {
	float x;
	stringstream ss;
	ss << string(m_sdata, m_length - 3, 3);
	ss >> x;
	return x;
}
float DataTool::getY() {
	float y;
	stringstream ss;
	ss << string(m_sdata, m_length - 6, 3);
	ss >> y;
	return y;
}

float DataTool::getAngle() {
	float angle;
	stringstream ss;
	ss << string(m_sdata, m_length - 9, 3);
	ss >> angle;
	return angle;
}
float DataTool::getScale() {
	float scale;
	stringstream ss;
	ss << string(m_sdata, m_length - 11, 2);
	ss >> scale;
	scale /= 10;
	return scale;
}
short DataTool::getName() {
   short name;
	stringstream ss;
	ss << string(m_sdata, m_length - 15, 4);
	ss >> name;
	return name;
}
int DataTool::getLayer() {
	int layer;
	stringstream ss;
	ss << string(m_sdata, m_length - 17, 2);
	ss >> layer;
	return layer;
}
int DataTool::getStudentId() {
	int i;
	stringstream ss;
	ss << string(m_sdata, m_length - 19, 2);
	ss >> i;
	return i;
}
Picture DataTool::getPicture() {
	Picture pic;
	pic.angle = getAngle();
	pic.layer = getLayer();
	pic.name = getName();
	pic.scale = getScale();
	pic.x = getX();
	pic.y = getY();
	pic.studentId=getStudentId();
	return pic;
}
string DataTool::getString() {
	stringstream ss;
	ss << m_data;
	string sData;
	ss >> sData;
	return sData;
}

void DataTool::setData() {
	m_data += (int) m_pic.x;
	m_data += ((int) m_pic.y) * 1000;
	m_data += ((int) m_pic.angle) * 1000000;
	m_data += (long long) (m_pic.scale * 10) * 1000000000;

    m_data += m_pic.name * 100000000000;
	m_data += m_pic.layer *   1000000000000000;
	m_data +=m_pic.studentId*100000000000000000;
}
long long DataTool::getData() {
	setData();
	return m_data;
}
