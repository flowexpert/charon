#ifndef CD_FITTER_HXX
#define CD_FITTER_HXX

#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <cassert>
#include <charon-core/ParameteredObject.hxx>
#include <charon-core/FileTool.h>
#include <charon-utils/ImgTool.hxx>
#include <charon/CDFitFunction.h>
#include <charon/McmcSampler.h>
#include "CdFitter.h"

#ifdef HANDLE_DLL
#ifdef cdfitter_EXPORTS
#define roi_EXPORTS
#define interpolatorlinear_EXPORTS
#define interpolator_EXPORTS
#endif
#endif

#ifdef WIN32
// for creation of gnuplot processes
#include <windows.h>
#endif

template<typename T>
CdFitter<T>::CdFitter(const std::string& name) :
		TemplatedParameteredObject<T>("cdfitter", name,
			"fitter using contrastive divergence"),
		training(false,true)
{
	ParameteredObject::_addInputSlot(training, "training",
			"training data input", "CImgList<T>");
	ParameteredObject::_addInputSlot(toFit, "toFit",
			"function to fit", "CDFitFunction*");
	ParameteredObject::_addParameter(addMirrored, "addMirrored",
			"add inverted training data to make PDF symmetric", false);
	ParameteredObject::_addParameter(agressiveness, "agressiveness",
			"conversion cycle control", 100.);
	ParameteredObject::_addParameter(agStop, "agStop",
			"stop threshold for agressiveness, 0=disable", 0.);
	ParameteredObject::_addParameter(cooldown, "cooldown",
			"agressiveness reduction ratio", 2.);
	ParameteredObject::_addParameter(oscis, "oscis",
			"number of oscillations for detection, 0=disable", 3u);
	ParameteredObject::_addParameter(mcmcSigma, "mcmcSigma",
			"mcmc noise sigma", 0.05);
	ParameteredObject::_addParameter(mcmcSweeps, "mcmcSweeps",
			"mcmc sweeps during one upate cycle", 1u);
	ParameteredObject::_addParameter(runs, "runs",
			"number dc cycles, 0=infinite", 100u);
	ParameteredObject::_addParameter<std::string>(results, "results",
			"filename to write results", "results.txt", "filename");
	ParameteredObject::_addParameter(showProgress, "showProgress",
			"show progress using gnuplot", true);
	ParameteredObject::_addParameter(showHistogram, "showHistogram",
			"show histogram and current status", true);
	ParameteredObject::_addParameter<std::string>(gnuplotCall, "gnuplotCall",
			"gnuplot call (absolute path in Windows)", "gnuplot", "filename");
}

template<typename T>
void CdFitter<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();

	CDFitFunction& fun = *(toFit());

	// initialize _sampler
	static McmcSampler sampler;
	sampler.sigma = mcmcSigma();
	sampler.steps = mcmcSweeps();
	sampler.pdf.disconnect();
	sampler.pdf.connect(fun.probability);

#ifndef NDEBUG
	// check image dimensions
	int dx  = training()[0].width();
	int dy = training()[0].height();
	int dt = training()[0].spectrum();

	for(unsigned int ii=0; ii<training.getTargets().size();ii++)
		cimglist_for(training[ii],kk)
			assert(training[ii][kk].is_sameXYC(dx,dy,dt));
#endif

	// collect training input
	std::vector<double>::size_type dims = fun.probability()->dims();
	assert(dims > 0);
	cimg_library::CImgList<T> trainCollection(training());
	if(trainCollection.size() != dims) {
		std::ostringstream msg;
		msg << __FILE__ << ":" << __LINE__ << "\n\t";
		msg << "Training data have wrong dimensions!\n\t";
		msg << "List should contain " << dims << " elements, but ";
		msg << "given one has " << trainCollection.size() << "!";
		throw std::invalid_argument(msg.str());
	}
	for(unsigned int ii=1; ii<training.getTargets().size();ii++) {
		const cimg_library::CImgList<T>& cur = training[ii];
		assert(cur.size() == dims);
		cimglist_for(cur, kk)
			trainCollection[kk].append(cur[kk],'z');
	}

	std::vector<double>::size_type s = trainCollection[0].size();
	sout << "\tTraining data content (" << dims << " dim):\n\t\t"
		<< s << " values" << "\n\t\t"
		<< "dimx : " << trainCollection[0].width() << "\n\t\t"
		<< "dimy : " << trainCollection[0].height() << "\n\t\t"
		<< "dimz : " << trainCollection[0].depth() << "\n\t\t"
		<< "dimt : " << trainCollection[0].spectrum() << std::endl;


	// we need a copy of the input data (preallocate memory)
	cimg_library::CImgList<T> modified(trainCollection);

	std::vector<double>::size_type NN = fun.fitParameters.size();
	assert(NN > 0);
	double* diff  = new double[NN];
	// for oscillation detection
	unsigned int oscillations = 0;
	double* diff2 = new double[NN]; // previous value
	double* diff3 = new double[NN];	// value 2 iterations before

	// write header and initial data to results file
	std::ofstream resOut;
	resOut.open(results().c_str(), std::ios::trunc);
	if(resOut.fail())
		throw std::ios_base::failure("file " + results()
			+ " could not be opened for writing.");
	resOut << "# CdFitter results file" << std::endl;
	resOut << "#" << std::endl;
	resOut << "# Each column represents one fit parameter." << std::endl;
	resOut << "# First column is the iteration number." << std::endl;
	resOut << "# Last column is agressiveness." << std::endl;
	resOut << "# Initial fitparameters are \"iteration 0\"" << std::endl << "0";
	for(std::size_t ii=0; ii<NN;ii++)
		resOut << "\t" << std::setprecision(8) << std::setw(12) << std::fixed
			<< fun.fitParameters[ii];
	resOut << "\t" << agressiveness() << std::endl;
	resOut.close();

	_updateProgress(0, trainCollection);

	for(unsigned int run=1; !runs() || run <= runs(); run++) {
		modified = trainCollection;
		std::vector<double>::size_type kk;
		std::vector<double>& cur = sampler.seed();
		cimg_forXYZC(modified[0],x,y,z,t) {
			for(kk=0;kk<dims;kk++)
				cur[kk]=modified(kk,x,y,z,t);
			sampler.sample();
			for(kk=0;kk<dims;kk++)
				modified(kk,x,y,z,t)=cur[kk];
		}

		// update weights
		for(std::size_t ii=0; ii < NN; ii++) {
			diff3[ii] = diff2[ii];
			diff2[ii] = diff [ii];
			diff [ii] = 0;
			assert(modified.is_sameNXYZC(trainCollection));
			std::vector<double> tval(dims,0.), mval(dims,0.);

			// iterate over all pixels and calculate update values
			cimg_forXYZC(modified[0],x,y,z,t) {
				for(kk=0;kk<dims;kk++) {
					tval[kk]=trainCollection(kk,x,y,z,t);
					mval[kk]=modified(kk,x,y,z,t);
				}
				diff[ii] += (fun.diffLog(tval, ii)
					- fun.diffLog(mval, ii)) / s;
			}
			if(run > 3)
				// perform oscillation detection
				if((diff[ii]-diff2[ii])*(diff2[ii]-diff3[ii]) < 0)
					oscillations++;
		}
		for(std::size_t ii=0; ii < NN; ii++)
			fun.fitParameters[ii] += agressiveness() * diff[ii];

		// normalize weights
		fun.normalize();

		// write results
		resOut.open(results().c_str(), std::ios::app);
		resOut << run;
		for(std::size_t ii=0; ii<NN;ii++)
			resOut << "\t" << std::setprecision(8)
				<< std::setw(12) << std::fixed
				<< fun.fitParameters[ii];
		resOut << "\t" << agressiveness() << std::endl;
		resOut.close();

		// decrease agressiveness if oscillating too much
		if (oscis() && oscillations >= oscis()*NN) {
			agressiveness() /= cooldown();
			oscillations = 0;
		}

		// check stop criterion
		if((agStop() > std::numeric_limits<double>::min())
				&& (agressiveness() < agStop()))
		{
			sout << "\tstopped by agressiveness threshold" << std::endl;
			sout << "\tthreshold: " << agStop() << std::endl;
			sout << "\tcurrent  : " << agressiveness() << std::endl;
			break;
		}

		// update progress information
		try {
			_updateProgress(run, trainCollection);
		}
		catch (std::runtime_error msg) {
			sout << "\tstopped by runtime exception" << std::endl;
			sout << "\texception message:" << std::endl;
			sout << "\t\t" << msg.what() << std::endl;
			break;
		}
	}

	resOut.open(results().c_str(), std::ios::app);
	resOut << "# final parameters:" << std::endl;
	resOut << "#     fitParameters\t";
	for(std::vector<double>::size_type kk=0;kk<fun.fitParameters.size();kk++)
		resOut << fun.fitParameters[kk] << ";";
	resOut << "\n#     agressiveness\t" << agressiveness << std::endl;
	resOut.close();

	fun.printResults();
	sout << "\t\tagressiveness\t" << agressiveness << std::endl;

	delete[] diff;
}

template<typename T>
void CdFitter<T>::_updateProgress(unsigned int run,
		const cimg_library::CImgList<T>& histData)
{
	std::string prefix = results().substr(0, results().find_last_of("."));
	std::string progImgFile = prefix + "_prog.pgm";
	std::string progPltFile = prefix + "_prog.plt";
	std::string histImgFile = prefix + "_hist.pgm";
	std::string histPltFile = prefix + "_hist.plt";
	std::string histDatFile = prefix + "_hist.dat";
	std::string histValFile = prefix + "_vals.dat";

	const CDFitFunction& fun = *(toFit());

	// histogram display
	if(showHistogram()) {
		if(histData.size() > 1)
			throw std::invalid_argument(
					"CdFitter: showProgress only implemented "
					"for 1D functions!");
		cimg_library::CImg<T> hist;
		static T range = 0;
		assert(fun.rangeHint() > 0);
		if(fun.rangeHint() != range) {
			// generate histogram and save values to file
			range = (T) fun.rangeHint();
			unsigned int bins = 60u;
			sout << "\tgenerating histogram\n\t\trange: " << range
				<< "\n\t\tbins : " << bins << std::endl;
			if(addMirrored())
				ImgTool::histogram(histData[0].get_append(
						-histData[0],'z'), hist,2*bins+1,-range,range);
			else
				ImgTool::histogram(histData[0],hist,2*bins+1,-range,range);
			hist /= hist.sum(); // normalize (sum up to one)
			std::ofstream histWriter(histDatFile.c_str(), std::ios::trunc);
			if(histWriter.bad())
				throw std::ios_base::failure("file " + progPltFile
					+ " could not be opened for writing.");
			histWriter
				<< "# CdFitter Training data histogram\n"
				<< "# range = " << range << "\n"
				<< "# bins  = " << 2*bins+1 << std::endl;
			for(unsigned int i=0;i<(2*bins+1);i++)
				histWriter
					<< range*((double)i-(double)bins)/(double)(bins)
					<< "\t" << hist[i] << std::endl;
			histWriter.close();

			// generate histogram plot script
			std::ofstream pltWriter(histPltFile.c_str(), std::ios::trunc);
			if(pltWriter.bad())
				throw std::ios_base::failure("file " + histPltFile
					+ " could not be opened for writing.");
			pltWriter
				<< "#!/usr/bin/gnuplot\n"
				<< "# gnuplot script for CdFitter histogram\n"
				<< "set terminal pbm small color size 500,400\n"
				<< "set output \"" << histImgFile << "\"\n"
				<< "set title \"CdFitter Status\"\n"
				<< "set xrange [" << -range << ":" << range << "]\n"
				<< "set yrange [*:*]\n"
				<< "set xlabel \"function value\"\n"
				<< "set ylabel \"probability\"\n"
				<< "set logscale y\n"
				<< "plot\t\""<< histDatFile<<"\" using 1:($2*"
				<< (2*bins+1)/(2*range)
				<< ") w histeps t \"histogram\",\\\n"
				<< "\t\t\""<<histValFile<<"\" w l t \"fitted\"" << std::endl;
			pltWriter.close();
		}
		// write current fitFunction values
		std::ofstream valWriter(histValFile.c_str(), std::ios::trunc);
		if(valWriter.bad())
			throw std::ios_base::failure("file " + histValFile
				+ " could not be opened for writing.");
		valWriter << "# fittet function values" << std::endl;
		std::vector<double> cur(1u);
		for(cur[0] = -range; cur[0] < range; cur[0] += range/200)
			valWriter << cur[0] << "\t" << fun.probability()->calculate(cur)
				<< std::endl;

		// call gnuplot and load generated image
#ifdef WIN32
		PROCESS_INFORMATION info;
		std::string::size_type pos;
		pos = histPltFile.find_last_of("/");
		std::string dir;
		std::string file = histPltFile;
		if (pos != std::string::npos) {
			dir = histPltFile.substr(0, pos);
			file = histPltFile.substr(pos+1);
		}
		else
			dir = FileTool::getCurrentDir();
		std::string call = "\"" + gnuplotCall() + "\" \"" + file + "\"";
		std::string::size_type len = call.length()+1;
		char* comLine = new char[len];
		call.copy(comLine, std::string::npos);
		comLine[len-1] = 0;
		CreateProcess(0, comLine, 0, 0, false, CREATE_NO_WINDOW,
			0, dir.c_str(), &STARTUPINFO(), &info);
		WaitForSingleObject(info.hProcess, INFINITE);
		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
		delete[] comLine;
#else
		system((gnuplotCall() + " \"" + histPltFile + "\"").c_str());
#endif
		cimg_library::CImg<unsigned short> img;
		img.load(histImgFile.c_str());

		// setup image display, if needed
		static cimg_library::CImgDisplay histDisp(
				500, 400, "CdFitter Status", 1);

		// show generated image
		histDisp.display(img);
		histDisp.show();
	}

	// progress display
	if(showProgress()) {
		std::ofstream pltWriter(progPltFile.c_str(), std::ios::trunc);
		if(pltWriter.bad())
			throw std::ios_base::failure("file " + progPltFile
				+ " could not be opened for writing.");
		pltWriter
			<< "#!/usr/bin/gnuplot\n"
			<< "# gnuplot script for CdFitter progress report\n"
			<< "set terminal pbm small color size 800,600\n"
			<< "set output \"" << progImgFile << "\"\n"
			<< "set title \"CdFitter Progress\"\n"
			<< "set key left top Left\n"
			<< "set xrange [0: ";
		if (runs())
			pltWriter << runs();
		else
			pltWriter << "*";
		pltWriter << "]\n"
			<< "set yrange [*:*]\n"
			<< "set xlabel \"iteration (" << run;
		if(runs())
			pltWriter << "/" << runs();
		pltWriter << " done)\"\n"
			<< "set ylabel \"weight values\"\n"
			<< "set y2label \"agressiveness\"\n"
			<< "set ytics nomirror\n"
			<< "set y2tics\n"
			<< "set logscale y2\n"
			<< "set y2range [*:*]\n";
		for (unsigned int ii=0; ii < fun.fitParameters.size(); ii++) {
			pltWriter << (!ii ? "plot " : "\t");
			pltWriter << "\t\"" << results() << "\" using 1:" << ii + 2
					<< " w lp title \"weight " << ii << " ("
					<< std::setprecision(8) << std::setw(10) << std::fixed
					<< fun.fitParameters[ii] << ")\",\\\n";
		}
		pltWriter << "\t\"" << results() << "\" using 1:"
				<< fun.fitParameters.size()+2
				<< " axes x1y2 w l title \"agressiveness\"" << std::endl;

		// call gnuplot and load generated image
#ifdef WIN32
		PROCESS_INFORMATION info;
		std::string::size_type pos;
		pos = progPltFile.find_last_of("/");
		std::string dir;
		std::string file = progPltFile;
		if (pos != std::string::npos) {
			dir = progPltFile.substr(0, pos);
			file = progPltFile.substr(pos+1);
		}
		else
			dir = FileTool::getCurrentDir();
		std::string call = "\"" + gnuplotCall() + "\" \"" + file + "\"";
		std::string::size_type len = call.length()+1;
		char* comLine = new char[len];
		call.copy(comLine, std::string::npos);
		comLine[len-1] = 0;
		CreateProcess(0, comLine, 0, 0, false, CREATE_NO_WINDOW,
			0, dir.c_str(), &STARTUPINFO(), &info);
		WaitForSingleObject(info.hProcess, INFINITE);
		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
		delete[] comLine;
#else
		system((gnuplotCall() + " \"" + progPltFile + "\"").c_str());
#endif
		cimg_library::CImg<unsigned short> img;
		img.load(progImgFile.c_str());

		// setup image display, if needed
		static cimg_library::CImgDisplay progDisp(
				800, 600, "CdFitter Progress - close to abort", 1);
		if(run && progDisp.is_closed())
			throw std::runtime_error("display closed by user");

		// show generated image
		progDisp.display(img);
		progDisp.show();
		if(run == runs())
			progDisp.wait(5000);
	}
}

#endif // CD_FITTER_HXX
