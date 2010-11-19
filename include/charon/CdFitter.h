/// \file CdFitter.h
/// Declaration of parametered class DcFitter
/// \author Jens-Malte Gottfried
/// \date 17.09.2009

#ifndef CD_FITTER_H_
#define CD_FITTER_H_

#include <charon-core/ParameteredObject.h>
#include <charon-utils/CImg.h>

class McmcSampler;
class Function;
class CDFitFunction;

#ifdef HANDLE_DLL
#ifdef cdfitter_EXPORTS
#define cdfitter_DECLDIR __declspec(dllexport)
#else
#define cdfitter_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define cdfitter_DECLDIR
#endif

/** Function fitter using contrastive divergence algorithm.
 *
 *  You can connect as many input slots as training data
 *  as needed. Activate addMirrored to make sure that
 *  the training data are symmetric (this adds the trainig
 *  data inverted).
 */
template <typename T>
class cdfitter_DECLDIR CdFitter : public TemplatedParameteredObject<T> {
public:
	/// standard constructor
	/// \param name		instance name
	CdFitter(const std::string& name = "");

	/// training data input
	InputSlot<cimg_library::CImgList<T> > training;
	/// function to train
	InputSlot<CDFitFunction*> toFit;

	/// add mirrored training data
	Parameter<bool> addMirrored;
	/// control update cycle
	Parameter<double> agressiveness;
	/// stop threshold for agressiveness
	Parameter<double> agStop;
	/// agressiveness reduction ratio
	Parameter<double> cooldown;
	/// number of oscillations for detection
	Parameter<unsigned int> oscis;
	/// mcmc noise sigma
	Parameter<double> mcmcSigma;
	/// mcmc sweeps during one update cycle
	Parameter<unsigned int> mcmcSweeps;
	/// number of dc cycles
	Parameter<unsigned int> runs;
	/// results output
	Parameter<std::string> results;
	/// progress display using gnuplot
	Parameter<bool> showProgress;
	/// show histogram and current status
	Parameter<bool> showHistogram;
	/// gnuplot call
	Parameter<std::string> gnuplotCall;

	/// perform DC algorithm
	/// \copydetails ParameteredObject::execute
	virtual void execute();

private:
	/** Update progress information.
	 *
	 *  This creates a progress graph using gnuplot
	 *  and displays it in a CImgDisplay.
	 *  There are two temporary files created during plotting,
	 *  an image file (.pgm) and a gnuplot script (.plt).
	 *  The filenames are generated using the output filename
	 *  changing the file extension.
	 *  \param run		current iteration
	 *	\param histData	training data for histogram generation
	 */
	void _updateProgress(
			unsigned int run,
			const cimg_library::CImgList<T>& histData);
};

#endif // DC_FITTER_H_
