/// \file MixtureOfGaussians.h
/// Declaration of class MixtureOfGaussians
/// \author Jens-Malte Gottfried
/// \date 16.09.2009

#ifndef MIXTURE_OF_GAUSSIANS_H_
#define MIXTURE_OF_GAUSSIANS_H_

#include <charon/Functions/Diff2Function.h>
#include <charon/Functions/Diff2LinFunction.h>
#include <charon/CDFitFunction.h>

#ifdef HANDLE_DLL
#ifdef mixtureofgaussians_EXPORTS
#define mixtureofgaussians_DECLDIR __declspec(dllexport)
#else
#define mixtureofgaussians_DECLDIR __declspec(dllimport)
#endif /*Export or import*/
#else /* No DLL handling or GCC */
#define mixtureofgaussians_DECLDIR
#endif

/// Mixture of gaussians.
/** The probability function is given by the sum of some gaussians:
 *  \f[f(x)=\sum_{i=0}^kw_i~\mathcal{N}(x,0,\sigma_i)\f]
 *  Where \f$\mathcal{N}\f$ is the normal distribution given by
 *  \f[\mathcal{N}(x,\mu,\sigma)=\frac{1}{\sigma\sqrt{2\pi}}
		\exp\left(-\frac {1}{2}\left(\frac{x-\mu}{\sigma}\right)^2\right)
	\f]
 *
 *  The weights \f$\{w_i\}\f$ are the parameters to fit, the widths
 *  \f$\{\sigma_i\}\f$ are fixed.
 */
class mixtureofgaussians_DECLDIR MixtureOfGaussians : public CDFitFunction {
private:
	/// Gaussian function weights, reference to CDFitFunction::fitParameters.
	std::vector<double>& _weights;
	/// Gaussian function widths
	std::vector<double> _sigmas;

	/// Probability function used by MixtureOfGaussians
	class ProbabilityFunction : public Diff2Function {
		friend class MixtureOfGaussians;
	private:
		const std::vector<double>& _weights; ///< reference to weights
		const std::vector<double>& _sigmas;  ///< reference to sigmas
		/// default constructor
		/** \param weights, sigmas   initialization of private members*/
		ProbabilityFunction(
				const std::vector<double>& weights,
				const std::vector<double>& sigmas
		);

	public:
		/// Normal distribution
		/** \f[\mathcal{N}(x,\mu,\sigma)=\frac{1}{\sigma\sqrt{2\pi}}
				\exp\left(-\frac {1}{2}\left(\frac{x-\mu}{\sigma}\right)^2\right)
			\f]
		 *  \param x, mu, s          \f$x,\mu,\sigma\f$ */
		static double normPdf(double x, double mu, double s);

		/// \copybrief Function::calculate()
		/** The probability function is given by the sum of some gaussians:
		 *  \f[p(x)=\sum_{k=0}^Kw_k~\mathcal{N}(x,0,\sigma_k)\f]
		 *  Where \f$\mathcal{N}\f$ is the normal distribution given by
		 *  normPdf() and \f$K\f$ is the number of gaussian scales.
		 *
		 *  \copydetails Function::calculate()
		 */
		virtual double calculate(std::vector<double> x) const;

		/// \copybrief DiffFunction::diff()
		/** \f[p'(x)=\sum_{k=0}^Kw_k\left(-\frac{x}{\sigma_k^2}\right)
			~\mathcal{N}(x,0,\sigma_k)\f]
		 *
		 *  Since we are handling a one-dimensional function, i is ignored.
		 *
		 *  \copydetails DiffFunction::diff()
		 */
		virtual double diff(
				std::vector<double> x,
				std::vector<double>::size_type i = 0
		) const;

		/// \copybrief Diff2Function::diff2()
		/** \f[p''(x)=\sum_{k=0}^Kw_k\left(\frac{x^2}{\sigma_k^4}
			-\frac{1}{\sigma_k^2}\right)~\mathcal{N}(x,0,\sigma_k)\f]
		 *
		 *  Since we are handling a one-dimensional function, i and j are
		 *  ignored.
		 *
		 *  \copydetails Diff2Function::diff2()
		 */
		virtual double diff2(
				std::vector<double> x,
				std::vector<double>::size_type i = 0,
				std::vector<double>::size_type j = 0
		) const;
		/// \copybrief Function::dims()
		/** \copydetails Function::dims()
		 *  Here, the number of dimensions is fixed to one.
		 */
		virtual std::vector<double>::size_type dims() {
			return 1u;
		}
	};

	/// Energy function used by MixtureOfGaussians.
	/** Calls probability function and calculates the energy using the
	 *  negative logarithm of the result.
	 */
	class EnergyFunction : public Diff2Function, public Diff2LinFunction {
		friend class MixtureOfGaussians;
	private:
		const Diff2Function& _prob; ///< reference to probability function
		const std::vector<double>& _sigmas;  ///< reference to sigmas
		/// standart constructor
		/** \param pf      probability function to use
		 *  \param sigmas  reference to sigma array
		 */
		EnergyFunction(
				const Diff2Function& pf, const std::vector<double>& sigmas);
	public:
		/// \copybrief Function::calculate()
		/** The energy is calculated as negarive logarithm of the
		 *  probability given by MixtureOfGaussians::ProbabilityFunction:
		 *  \f[E(x)=-\log p(x)\f]
		 *  \copydetails Function::calculate()
		 */
		virtual double calculate(std::vector<double> x) const;
		/// \copybrief DiffFunction::diff()
		/** \f[E'(x)=\frac{d}{dx}-\log p(x)=-\frac{p'(x)}{p(x)}\f]
		 *  \copydetails DiffFunction::diff()
		 */
		virtual double diff(
				std::vector<double> x,
				std::vector<double>::size_type i = 0
		) const;
		/// \copybrief Diff2Function::diff2()
		/** \f[E''(x)=\frac{d}{dx}-\frac{p'(x)}{p(x)}=
			\left(\frac{p'(x)}{p(x)}\right)^2-\frac{p''(x)}{p(x)}\f]
		 *  \copydetails Diff2Function::diff2()
		 */
		virtual double diff2(
				std::vector<double> x,
				std::vector<double>::size_type i = 0,
				std::vector<double>::size_type j = 0
		) const;

		/// \copybrief Function::dims()
		/** \copydetails Function::dims()
		 *  Here, the number of dimensions is fixed to one.
		 */
		virtual std::vector<double>::size_type dims() {
			return 1u;
		}
		/// \copybrief Diff2LinFunction::diff2Linearized()
		/** \copydetails Diff2LinFunction::diff2Linearized()
		 *
		 *  Here, for large values of \f$x_j\f$, where the first derivative
		 *  gets very small, ths sum of the gaussian functions is approximated
		 *  by a single gaussian function with the largest sigma value.
		 *
		 *  This implementation uses second derivative for small values
		 *  of \f$x_j\f$ to avoid division by zero.
		 */
		virtual double diff2Linearized(
				std::vector<double> x,
				std::vector<double>::size_type i = 0,
				std::vector<double>::size_type j = 0);
	};

	/// probabilty function instance
	ProbabilityFunction _probability;

	/// energy function instance
	EnergyFunction _energy;

public:
	/// standard constructor
	/// \param name     instance name
	MixtureOfGaussians(const std::string& name = "");

	virtual ~MixtureOfGaussians();
	virtual void normalize();
	virtual void printResults();

	/// \copybrief CDFitFunction::rangeHint()
	/** \copydetails CDFitFunction::rangeHint()
	 *
	 *  Using this functions, histogram range should be 3 times the
	 *  width of the broadest gaussian component.
	 */
	virtual double rangeHint(std::vector<double>::size_type i = 0) const;

	/// Gaussian function weights
	ParameterList<double> weights;

	/// Gaussian function widths
	ParameterList<double> sigmas;

	/// \copybrief ParameteredObject::execute()
	/** Copy content of the parameter lists weights and sigmas to the
	 *  corresponding double vectors used for fitting.
	 *
	 *  \copydetails ParameteredObject::execute()
	 */
	virtual void execute();

	/// \copybrief CDFitFunction::diffLog()
	/** \copydetails CDFitFunction::diffLog()
	 *
	 *  \f[\frac{\partial}{\partial w_i}\log p(x)=
		\frac{1}{p(x)}\frac{\partial}{\partial w_i} p(x)=
		\frac{\mathcal{N}(x,0,\sigma_i)}
			{\sum_{k=0}^Kw_k~\mathcal{N}(x,0,\sigma_k)}\f]
	 */
	virtual double diffLog(
			std::vector<double> x,
			std::vector<double>::size_type i) const;
};

#endif // MIXTURE_OF_GAUSSIANS_H_
