/*  Copyright (C) 2011 Jens-Malte Gottfried

	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file dataPassing.cpp
/// Tests for Slot data passing mechanisms.

#include <cstdlib>
#include <iostream>

#include <limits>
#include <charon-utils/CImg.h>
#include <vigra/multi_array.hxx>
#include <charon-core/ParameteredObject.hxx>
#include <charon-core/ExceptionHandler.h>
#include <charon-utils/DataManagerCImgList.hxx>
#include <charon-utils/DataManagerVigraArray.hxx>

/// sample ParameteredObject class with CImg and vigra outputs
class Outputgen : public ParameteredObject {

public:
	/// sample integer output slot
	OutputSlot< cimg_library::CImgList<float> > out1;
	/// sample float output slot
	OutputSlot< vigra::MultiArray<5, float> >  out2;

	/// create a new sample object
	/// @param name             Object name
	Outputgen(const std::string& name = "") :
			ParameteredObject("outputgen", name,
				"class to provide data output")
	{
		// slots
		_addOutputSlot(out1, "out1", "cimg output slot", "CImgList<float>");
		_addOutputSlot(out2, "out2", "vigra output slot", "vigraArray5<float>");
	}

protected:
	virtual void execute() {
		out1().assign(1,5,4,3,2,234.f);
		out2().reshape(vigra::MultiArrayShape<5>::type(5,4,3,2,1),234.f);
	}
};

/// sample ParameteredObject class with CImg and vigra inputs
class Reader : public ParameteredObject {
public:
	/// sample integer input slot
	InputSlot< cimg_library::CImgList<float> > in1;
	/// sample float input slot
	InputSlot< vigra::MultiArray<5, float> > in2;

	/// create a new sample object
	/// @param name             Object name
	Reader(const std::string& name = "") :
			ParameteredObject("Reader", name, "read slot data")
	{
		_addInputSlot (in1,  "in1",  "cimg input slot", "CImgList<float>");
		_addInputSlot (in2,  "in2",  "vigra input slot", "vigraArray5<float>");
	}

protected:
	virtual void execute() {
		sout << "(II) \tRead slot data dimensions: \n(II) \t";
		cimg_library::CImgList<float> i1 = in1();
		vigra::MultiArray<5, float> i2 = in2();
		sout << "in1: " << i1[0].width() << "x" << i1[0].height() << "x"
			 << i1[0].depth() << "x" << i1[0].spectrum() << "x"
			 << i1.size() << std::endl;
		if (!i1.is_sameNXYZC(1,5,4,3,2)) {
			ParameteredObject::raise("in1 has unexpected shape");
		}
		vigra::MultiArrayShape<5>::type i2s = i2.shape();
		vigra::MultiArrayShape<5>::type expected(5,4,3,2,1);
		sout << "(II) \tin2: " << i2s[0] << "x" << i2s[1] << "x"
			 << i2s[2] << "x" << i2s[3] << "x" << i2s[4] << std::endl;
		if (i2s != expected) {
			ParameteredObject::raise("in2 has unexpected shape");
		}
	}
};


void testDataManagerCImg() {
	Outputgen outGen("outputgen_dataManC");
	Slot::DataManager< cimg_library::CImgList<float> >* man =
			Slot::DataManagerFactory<
				cimg_library::CImgList<float> >::getManager(outGen.out1);
	if (!man) {
		throw "Factory cannot handle int CImg list data";
	}
	const std::string& conf = man->getConfig();
	sout << "(II) cimg  slot config: " << conf << std::endl;
	const cimg_library::CImgList<float> testData(1u,5u,4u,3u,2u,127.f);
	man->setData(testData);
	const float tol = std::numeric_limits<float>::min();
	cimg_library::CImgList<float> readData1 = man->getData();
	if (!readData1.is_sameNXYZC(testData)) {
		throw "read data dimension mismatch (1)";
	}
	readData1[0] -= testData[0];
	readData1[0].abs();
	float max1 = readData1[0].max();
	if (max1 > tol) {
		throw "read data content mismatch (1)";
	}
	delete man;
	man = Slot::DataManagerFactory<
			cimg_library::CImgList<float> >::getManager(outGen.out1, conf);
	cimg_library::CImgList<float> readData2 = man->getData();
	if (!readData2.is_sameNXYZC(testData)) {
		throw "read data dimension mismatch (2)";
	}
	readData2.at(0) -= testData[0];
	readData2[0].abs();
	float max2 = readData2[0].max();
	if (max2 > tol) {
		throw "read data content mismatch (2)";
	}
	delete man;
}

void testDataManagerVigra() {
	Outputgen outGen("outputgen_dataManV");
	Slot::DataManager< vigra::MultiArray<5,float> >* man =
			Slot::DataManagerFactory<
				vigra::MultiArray<5,float> >::getManager(outGen.out2);
	if (!man) {
		throw "Factory cannot handle int Vigra Array data";
	}
	const std::string& conf = man->getConfig();
	sout << "(II) vigra slot config: " << conf << std::endl;
	const vigra::MultiArrayShape<5>::type testShape(5u,4u,3u,2u,1u);
	const vigra::MultiArray<5,float> testData(testShape, 127.f);
	man->setData(testData);
	const float tol = std::numeric_limits<float>::min();
	vigra::MultiArray<5,float> readData1 = man->getData();
	if (readData1.shape()!=testShape) {
		throw "read data shape mismatch (1)";
	}
	readData1 -= testData;
	const float& max1 = readData1.norm(0);
	if (max1 > tol) {
		throw "read data content mismatch (1)";
	}
	delete man;
	man = Slot::DataManagerFactory<
			vigra::MultiArray<5,float> >::getManager(outGen.out2, conf);
	vigra::MultiArray<5,float> readData2 = man->getData();
	if (readData2.shape()!=testShape) {
		throw "read data shape mismatch (2)";
	}
	readData2 -= testData;
	const float& max2 = readData2.norm(0);
	if (max2 > tol) {
		throw "read data content mismatch (2)";
	}
	delete man;
}

void testMem() {
	sout << "\ndry run (memory, as usual):" << std::endl;
	Outputgen outGen;
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	reader.run();
}

void testCacheTypeInvalid() {
	sout << "\nwith invalid cache (should fail):" << std::endl;
	Outputgen outGen;
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	outGen.out1.setCacheType(Slot::CACHE_INVALID);
	outGen.out2.setCacheType(Slot::CACHE_INVALID);
	reader.run();
}

void testCacheTypeManagedCImg() {
	sout << "\nwith managed cache (cimg):" << std::endl;
	Outputgen outGen("outputgen_dataManCR");
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	outGen.out1.setCacheType(Slot::CACHE_MANAGED);
	reader.run();
}

void testCacheTypeManagedVigra() {
	sout << "\nwith managed cache (vigra):" << std::endl;
	Outputgen outGen("outputgen_dataManVR");
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	outGen.out2.setCacheType(Slot::CACHE_MANAGED);
	reader.run();
}

void testCacheTypeManagedBoth() {
	sout << "\nwith managed cache (both):" << std::endl;
	Outputgen outGen("outputgen_dataManCV");
	Reader reader;
	reader.in1.connect(outGen.out1);
	reader.in2.connect(outGen.out2);
	outGen.out1.setCacheType(Slot::CACHE_MANAGED);
	outGen.out2.setCacheType(Slot::CACHE_MANAGED);
	reader.run();
}

int main() {
	sout.assign(std::cout);
	int ret = EXIT_SUCCESS;
	ret |= ExceptionHandler::run(testDataManagerCImg);
	ret |= ExceptionHandler::run(testDataManagerVigra);
	ret |= ExceptionHandler::run(testMem);
	ret |= ExceptionHandler::checkRaise(testCacheTypeInvalid);
	ret |= ExceptionHandler::run(testCacheTypeManagedCImg);
	ret |= ExceptionHandler::run(testCacheTypeManagedVigra);
	ret |= ExceptionHandler::run(testCacheTypeManagedBoth);
	return ret;
}

