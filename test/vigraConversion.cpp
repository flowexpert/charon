#ifdef NDEBUG
#undef NDEBUG
#endif

#include <charon-utils/VigraMultiArray2CImg.hxx>
#include <charon-utils/CImg2VigraMultiArray.hxx>
#include <charon-utils/FileReader.hxx>

#ifndef PENGUINFILE
#define PENGUINFILE
#error no PENGUINFILE given
#endif

int main() {
	// read penguin file and convert it to vigra array and back
	sout.assign(std::cout);
	sout << "Testing using PENGUINFILE" << std::endl;
	FileReader<double> reader;
	reader.filename = PENGUINFILE;

	CImg2VigraMultiArray<double> conv1;
	VigraMultiArray2CImg<double> conv2;

	reader.out.connect(conv1.in);
	conv1.out.connect(conv2.in);
	conv2.execute();
	assert(conv2.out().size() == 1u);

	// read penguin file directly
	cimg_library::CImg<double> reference(PENGUINFILE);
	const cimg_library::CImgList<double>& result = conv2.out();

	// check if results are the same
	assert(reference.is_sameXYZC(result[0]));
	assert((reference - result[0]).abs().sum()
				<= std::numeric_limits<double>::min());

	// check intermediate step
	const vigra::MultiArrayView<5, double>& intermediate = conv1.out();
	assert(intermediate.size(0) == reference.width());
	assert(intermediate.size(1) == reference.height());
	assert(intermediate.size(2) == reference.depth());
	assert(intermediate.size(3) == reference.spectrum());
	assert(intermediate.size(4) == 1u);

	cimg_forXYZC(reference, x, y, z, t) {
		assert(reference(x,y,z,t) == intermediate(x,y,z,t,0));
	}

	sout << "Testing using 5D random data" << std::endl;
	cimg_library::CImgList<double>& testData = reader.out();
	const int dx = 13u, dy = 17u, dz = 5u, dt = 3u, dv = 2u;
	testData.assign(dv, dx, dy, dz, dt);
	cimglist_for(testData, k) {
		testData[k].rand(-1.,1.);
	}
	conv1.resetExecuted();
	conv2.execute();

	// check if original data untouched
	assert(testData.size() == unsigned(dv));
	assert(testData[0].width() == dx);
	assert(testData[0].height() == dy);
	assert(testData[0].depth() == dz);
	assert(testData[0].spectrum() == dt);

	// check if results are the same
	assert(testData.is_sameNXYZC(result));
	cimglist_for(testData, k) {
		assert((testData[k] - result[k]).abs().sum()
				<= std::numeric_limits<double>::min());
	}

	// check intermediate step
	assert(intermediate.size(0) == dx);
	assert(intermediate.size(1) == dy);
	assert(intermediate.size(2) == dz);
	assert(intermediate.size(3) == dt);
	assert(intermediate.size(4) == dv);

	cimglist_for(testData, k) {
		cimg_forXYZC(testData[k], x, y, z, t) {
			assert(testData(k,x,y,z,t) == intermediate(x,y,z,t,k));
		}
	}

	return 0;
}
