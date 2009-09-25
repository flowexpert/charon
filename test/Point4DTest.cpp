#include "Point4D.h"
#include <iostream>

int main() {
	Point4D<int> p1(-2,1,5,-7);
	//check constructor
	if((p1.x != -2) || (p1.y != 1) || (p1.z != 5) || (p1.t != -7)) {
		std::cout<<"Point4D<int> constructor failed"<<std::endl;
		return -1;
	}
	Point4D<unsigned int> p2(5,7,12,8);
	//check constructor
	if((p2.x != 5) || (p2.y != 7) || (p2.z != 12) || (p2.t != 8)) {
		std::cout<<"Point4D<unsigned int> constructor failed"<<std::endl;
		return -1;
	}
	Point4D<int> p3;
	//check empty constructor
	if((p3.x != 0) || (p3.y != 0) || (p3.z != 0) || (p3.t != 0)) {
		std::cout<<"Point4D<int> empty constructor failed"<<std::endl;
		return -1;
	}
	p3 = Point4D<int>(p2);
	//check cast and assignment
	if((p3.x != 5) || (p3.y != 7) || (p3.z != 12) || (p3.t != 8)) {
		std::cout<<"cast Point4D<unsigned int> -> Point4D<int> or assignment failed"<<std::endl;
		return -1;
	}
	p3 = Point4D<unsigned int>(p1);
	//check cast and assignment, loss of sign
	if((p3.x != 2) || (p3.y != 1) || (p3.z != 5) || (p3.t != 7)) {
		std::cout<<"cast Point4D<int> -> Point4D<unsigned int> or assignment failed"<<std::endl;
		return -1;
	}
	
	//Operators
	//check comparison operator
	if(p3 == p2) {
		std::cout<<"comparison operator failed"<<std::endl;
		return -1;
	}
	
	//check < and > operator
	if(p3 > p2) {
		std::cout<<"> operator failed"<<std::endl;
		return -1;
	}
	if(p2 < p3) {
		std::cout<<"< operator failed"<<std::endl;
		return -1;
	}
	
	//check addition
	Point4D<int> p4(12,7,3,9);
	Point4D<int> p5(-7,5,4,-8);
	Point4D<int> p6;
	p6 = p4 + p5;
	if ((p6.x != 5) || (p6.y != 12) || (p6.z != 7) || (p6.t != 1)) {
		std::cout<<"addition failed"<<std::endl;
		return -1;
	}
	
	//check subtraction
	p6 = p4 - p5;
	if ((p6.x != 19) || (p6.y != 2) || (p6.z != -1) || (p6.t != 17)) {
		std::cout<<"subtraction failed"<<std::endl;
		return -1;
	}
	
	//check volume
	int v = p4.volume();
	if (v != 2268) {
		std::cout<<"volume failed"<<std::endl;
		return -1;
	}
	
	return 0;
}