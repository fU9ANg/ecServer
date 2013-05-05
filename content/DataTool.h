/*
 * DataTool.h
 *
 *  Created on: 2013-5-2
 */

#ifndef DATATOOL_H_
#define DATATOOL_H_
#include <sstream>
struct Picture {
	float x; //X坐标
	float y; //Y坐标
	float angle; //顺时针旋转角度值
	float scale; //缩放比例
	std::string name; //图片名称
	int layer; //层次与编号统一，不大于99
	int studentId; //学生编号，用于身份识别
};

/*                                     long long        (反向）  1-3 X     4-6 Y    7-9 angle    10_11  scale   12-15 name   16-17 layer  18-19 studentId
 *
 *
 *                           name规则  第一位代表从左到右的种类编号 后三位代表去掉.png的编号
 *
 *
 */

class DataTool {
	Picture m_pic;
	std::string m_sdata;
	long long m_data;
	int m_length;
	std::string getString();
	void setData();
	float getX();
	int getStudentId();
	float getY();
	float getAngle();

	float getScale();
	std::string getName();

	int getLayer();
public:

	DataTool(long long);
	DataTool(Picture);

	Picture getPicture();
	long long getData();
	virtual ~DataTool();

};

#endif /* DATATOOL_H_ */
