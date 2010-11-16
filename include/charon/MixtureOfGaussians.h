/// \file MixtureOfGaussians.h
/// Declaration of class MixtureOfGaussians
/// \author Jens-Malte Gottfried
/// \date 16.09.2009

#ifndef MIXTURE_OF_GAUSSIANS_H_
#define MIXTURE_OF_GAUSSIANS_H_

#include <charon/Functions/Diff2Function.h>
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

/// mixture of gaussians
///
/// The result is given by the sum of some gaussian functions.
/// \f[f(x)=\sum_{i=0}^kw_i~\mathcal{N}(x,0,\sigma_i)\f]
/// Where \f$\mathcal{N}\f$ is the normal distribution given by
/** \f[\mathcal{N}(x,\mu,\sigma)=\frac{1}{\sigma\sqrt{2\pi}}
		\exp\left(-\frac {1}{2}\left(\frac{x-\mu}{\sigma}\right)^2\right)
	\f]
 */
/// The weights \f$\{w_i\}\f$ are the parameters to fit, the widths
/// \f$\{\sigma_i\}\f$ are fixed.
class mixtureofgaussians_DECLDIR MixtureOfGaussians : public CDFitFunction {
private:
	/// Gaussian function weights
	std::vector<double> _sigmas;
	/// Gaussian function widths
	const std::vector<double>& _weights;

	/// implementation of the probability function used by MixtureOfGaussians
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
		/// normal distribution
		/** \param x, mu, s          \f$x,\mu,\sigma\f$ */
		static double _normPdf(double x, double mu, double s);
	public:
		virtual double calculate(std::vector<double> x) const;
		virtual double diff(
				std::vector<double> x,
				std::vector<double>::size_type i = 0
		) const;
		virtual double diff2(
				std::vector<double> x,
				std::vector<double>::size_type i = 0,
				std::vector<double>::size_type j = 0
		) const;
		virtual std::vector<double>::size_type dims() {
			return 1u;
		}
	} _probability; ///< probabilty function instance

	/// implementation of the energy function used by MixtureOfGaussians
	/** passes its calls to the probability function and calculates
	 *  the energy using logarithm etc.
	 */
	class EnergyFunction : public Diff2Function {
		friend class MixtureOfGaussians;
	private:
		/// reference to probability function
		const Diff2Function& _prob;
		/// standart constructor
		/** \param pf   probability function to use */
		EnergyFunction(const Diff2Function& pf);
	public:
		virtual double calculate(std::vector<double> x) const;
		virtual double diff(
				std::vector<double> x,
				std::vector<double>::size_type i = 0
		) const;
		virtual double diff2(
				std::vector<double> x,
				std::vector<double>::size_type i = 0,
				std::vector<double>::size_type j = 0
		) const;
		virtual std::vector<double>::size_type dims() {
			return 1u;
		}
	} _energy;  ///< energy function instance

public:
	/// standard constructor
	/// \param name     instance name
	MixtureOfGaussians(const std::string& name = "");
	virtual ~MixtureOfGaussians();
	virtual void normalize();

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

	virtual double diffLog(
			std::vector<double> x,
			std::vector<double>::size_type i) const;
};

#endif // MIXTURE_OF_GAUSSIANS_H_
