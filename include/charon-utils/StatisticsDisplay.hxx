/*  Copyright (C) 2010 Stephan Meister

 * \file StatisticsDisplay.hxx
 *  Implementation of the class StatisticsDisplay.
 *  \author Stephan Meister
 *  \date 18.12.2010
 */

#ifndef _STATISTICSDISPLAY_HXX_
#define _STATISTICSDISPLAY_HXX_

#include <QApplication>
#include <sstream>
#include <vigra/multi_array.hxx>
#include <vigra/multi_iterator.hxx>
#include <vigra/navigator.hxx>
#include <vigra/multi_pointoperators.hxx>
#include <set>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/density.hpp>

#include <StatisticsDisplayWidget.hpp>
#include "StatisticsDisplay.h"

namespace StatisticsDisplay {

typedef boost::iterator_range
	<std::vector<std::pair<double, double> >::iterator> histogram_type;

cimg_library::CImg<unsigned char> histogramPlot(
		const histogram_type& histogram,double mean) {
	using namespace cimg_library ;

	int leftBorder = 50 ;
	int topBorder = 50 ;
	int graphWidth = 700 ;
	int graphHeight = 500 ;
	int imgWidth = 800 ;
	int imgHeight = 600 ;
	unsigned char foreground[] = {255,255,255} ;
	unsigned char textcolor[] = {255,0,0} ;
	unsigned char background[] = {0,0,0} ;

	CImg<unsigned char> target(imgWidth,imgHeight,1,3,0) ;
	
	//draw graph
	CImg<unsigned char> graph(graphWidth, graphHeight, 1, 3, 0) ;
	CImg<double> histdata(histogram.size() - 2,1) ;
	double maxdensity = 0.0 ;
	double minBin = histogram[1].first ; 
	double maxBin = histogram.back().first ;

	std::vector<std::pair<double, double> >::iterator it = histogram.begin() ;
	//start at second bin, the first one is an underflow bin which is not used
	it++ ;
	for(unsigned int ii = 0;it != histogram.end() && ii < histdata.size(); it++, ii++)
	//for(int ii = 0 ; ii < histogram.size() ; ii++)
	{
		double val = it->second ;
		histdata[ii] = val ;
		if(val > maxdensity)
			maxdensity = val ;
	}
	graph.draw_graph(histdata, foreground, 1.0, 3, 1, maxdensity,0.0) ;
	
	//labels
	int res ;
	char minValS[32] ;
	char maxValS[32] ;
	char minCountS[32] ;
	char maxCountS[32] ;
	char middleValS[32] ;
	res = sprintf(minValS, "%.1f", minBin) ;
	res = sprintf(middleValS, "%.1f", (maxBin + minBin) * 0.5) ;
	res = sprintf(maxValS, "%.1f", maxBin) ;
	res = sprintf(minCountS, "0") ;
	res = sprintf(maxCountS, "%.1f", maxdensity) ;
	cimg::unused(res);

	//combine parts
	target.draw_rectangle(leftBorder, topBorder, leftBorder+ graphWidth + 2,
			topBorder + graphHeight + 2, foreground) ;
	target.draw_image(leftBorder + 1, topBorder + 1, 0, 0, graph) ;

	//draw lines at median and mean
	/* //median is only estimated, sometimes badly; 
	   //so there's little sense in showing it
	float medianX = (median - minBin) * graphWidth / (maxBin - minBin) + leftBorder ;
	target.draw_line(medianX, topBorder-10, medianX,topBorder+graphHeight+5, foreground) ;
	target.draw_text(medianX, topBorder - 10, "median", foreground, background) ;
	*/
	float meanX = (mean - minBin) * graphWidth / (maxBin -minBin) + leftBorder ;
	target.draw_line(meanX, topBorder - 18, meanX,
		topBorder+graphHeight+5, foreground) ;
	target.draw_text(meanX, topBorder - 18, "mean", textcolor, background) ;

	//draw axis ticks
	target.draw_text(int(leftBorder *0.5), topBorder, maxCountS,
		textcolor, background) ;
	target.draw_text(int(leftBorder *0.5), topBorder + graphHeight, minCountS, 
		textcolor, background) ;
	target.draw_text(leftBorder, topBorder+ graphHeight + 10, minValS, 
		textcolor, background) ;
	target.draw_line(leftBorder + graphWidth * 0.5, topBorder + graphHeight - 5,
		leftBorder + graphWidth * 0.5, topBorder + graphHeight + 5,foreground) ;
	target.draw_text(leftBorder + graphWidth * 0.5 - 10, 
		topBorder + graphHeight + 10, middleValS, textcolor, background) ;
	target.draw_text(leftBorder + graphWidth - 20, topBorder+ graphHeight + 10,
		maxValS, textcolor, background) ;
	
	//convert to charon color representation
	return target ;
}


template <typename T>
StatisticsDisplayPlugin<T>::StatisticsDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("StatisticsDisplay", name,
			"Calculates various statistical properties of input object "
			"and exports a QWidget for display<br>"
			"Remark: The median value is only an estimate!"),
			_vigraIn(true, true),
			_cimgIn(true, true),
			_vigraMask(true, false),
			_cimgMask(true, false),
			_writeToSout(true),
			_exportWidget(0)
{
	ParameteredObject::_addInputSlot(
			_vigraIn, "vigraIn",
			"The vigra::MultiArray<5, T> input.",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_cimgIn, "cimgIn",
			"The cimg_library::CImgList<T> input.",
			"CImgList<T>");

	ParameteredObject::_addInputSlot(
			_vigraMask, "vigraMask",
			"Mask to determine where the statistics should be calculated.<br>"
			"This is not a multislot as there is no possibility to "
			"map multiple mask to the corresponding images in \"in\"<br>"
			"The mask will be applied to all vigra images "
			"as long as their dimensions match.",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_cimgMask, "cimgMask",
			"Mask to determine where the statistics should be calculated.<br>"
			"This is not a multislot as there is no possibility to "
			"map multiple mask to the corresponding images in \"in\"<br>"
			"The mask will be applied to all cimg images "
			"as long as their dimensions match.",
			"CImgList<T>");

	ParameteredObject::_addOutputSlot(
			_histograms, "histograms",
			"Image showing the distribution of values in each input image<br>"
			"This is a color image of dimension 800x600x1xnx3 "
			"(where n is the number of input images)<br>"
			"Remark: The histogram calculation does currently not work with masks",
			"CImgList<T>");

	ParameteredObject::_addOutputSlot(
			_display, "display",
			"A QWidget displaying all results, "
			"can for example be connected to a ArgosDisplay instance",
			"QWidget*") ;
			
	_display = 0 ;

	ParameteredObject::_addParameter<bool>(_writeToSout, "writeToSout",
		"write results to stdout and status console", true, "bool");
	
	ParameteredObject::_addParameter<size_t>(_numBins, "numberOfBins",
		"number of bins for histogram", 256) ;
	
	if(!qApp)
	{
		sout << "StatisticsDisplay::No QApplication found! " 
				"StatisticsDisplay can only be used in a Qt GUI Application!"
			 << std::endl ;
		return ;
	}
	

	_exportWidget = new StatisticsDisplayWidget() ;
	_display = _exportWidget ;
}

template <typename T>
StatisticsDisplayPlugin<T>::~StatisticsDisplayPlugin()
{
	delete _exportWidget ;
}

template <typename T>
void StatisticsDisplayPlugin<T>::execute() {
	using namespace boost::accumulators;
	using namespace cimg_library ;

	typedef vigra::MultiArray<5, T> Array ;

	_statistics.clear() ;
	_histograms().assign(3) ;

	//get pointer to each object in Multislot and the name of the corresponding parent object
	std::map<const Array* const, std::string> parentNames ;
	typename std::set<AbstractSlot<Array>*>::const_iterator it = _vigraIn.begin() ;
	typename std::set<AbstractSlot<Array>*>::const_iterator end = _vigraIn.end() ;

	for(std::size_t ii = 0 ; (ii < _vigraIn.size()) && (it != end) ; ii++, it++)
	{
		std::string name = (*it)->getParent().getName() + "." + (*it)->getName();
		sout << "Calculating statistics for " << name << std::endl ;
		
		const Array& img = _vigraIn[ii] ;
		
		//create accumulators for each image, add tags as needed
		accumulator_set<double, stats<
			tag::min, tag::max, tag::count, 
			tag::sum, tag::mean, tag::variance, 
			tag::median, tag::density
		> > acc(tag::density::num_bins = _numBins(),tag::density::cache_size = img.size());

		//accumulate all pixel values
		if(!_vigraMask.connected())
		{
				vigra::inspectMultiArray(srcMultiArrayRange(img), acc);
		}
		else
		{		//size checks
				const Array& mask = _vigraMask() ;
				if(mask.size(0) != img.size(0) ||
						mask.size(1) != img.size(1) ||
						mask.size(2) != img.size(2) ||
						mask.size(3) != img.size(3) ||
						mask.size(4) != img.size(4))
				{	sout << "Dimensions of mask and input array do not match" << std::endl ;
						continue ;
				}
				//can we do this with iterators? or with OpenMP if boost::accumulator is thread-safe?
				for(int xx = 0 ; xx < mask.size(0) ; xx++)
						for(int yy = 0 ; yy < mask.size(1) ; yy++)
								for(int zz = 0 ; zz < mask.size(2) ; zz++)
										for(int tt = 0 ; tt < mask.size(3) ; tt++)
												for(int vv = 0 ; vv < mask.size(4) ; vv++)
														if(mask(xx,yy,zz,tt,vv) != 0)
																acc(img(xx,yy,zz,tt,vv)) ;
		}

		Statistics s ;

		typedef std::pair<std::string, double> StatPair ;
		s.stats.insert(StatPair("dim0 : width",img.size(0))) ;
		s.stats.insert(StatPair("dim1 : height",img.size(1))) ;
		s.stats.insert(StatPair("dim2 : depth",img.size(2))) ;
		s.stats.insert(StatPair("dim3 : slices",img.size(3))) ;
		s.stats.insert(StatPair("dim4 : spectrum",img.size(4))) ;
		s.stats.insert(StatPair("number of pixels",count(acc))) ;
		s.stats.insert(StatPair("sum",sum(acc))) ;
		s.stats.insert(StatPair("min",min(acc))) ;
		s.stats.insert(StatPair("max",max(acc))) ;
		s.stats.insert(StatPair("mean",mean(acc))) ;
		s.stats.insert(StatPair("variance",variance(acc))) ;
		s.stats.insert(StatPair("stddev",sqrt(variance(acc)))) ;
		s.stats.insert(StatPair("median",median(acc))) ;
		s.origin = name ;

		_statistics.push_back(s) ;

		if(_writeToSout())
		{
				sout << "Statistics for " << name ;
				std::map<std::string,double>::const_iterator it = s.stats.begin() ;
				for(; it != s.stats.end() ; it++)
				{	sout << "\n\t" << it->first << ": " << it->second ;	}
				sout << std::endl ;
		}
		
		if(_histograms.connected())
		{
			CImg<unsigned char> histplot = histogramPlot(density(acc), mean(acc)) ;
			_histograms()[0].append(histplot.get_shared_channel(0),'c') ;
			_histograms()[1].append(histplot.get_shared_channel(1),'c') ;
			_histograms()[2].append(histplot.get_shared_channel(2),'c') ;
		}

	}


	//do statistics calculation for CImgLists
	
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cit = _cimgIn.begin() ;
	typename std::set<AbstractSlot<cimg_library::CImgList<T> >*>
			::const_iterator cend = _cimgIn.end() ;
	
	for(std::size_t ii = 0 ; (ii < _cimgIn.size()) && (cit != cend) ; ii++, cit++)
	{
		std::string name = (*cit)->getParent().getName() + "." + (*cit)->getName();
		sout << "Calculating statistics for " << name << std::endl ;
		//create accumulators for each image, add tags as needed
		const cimg_library::CImgList<T>& cimg = _cimgIn[ii] ;

		size_t pixCount  = 0 ;
		cimglist_for(cimg,l)
		{
			pixCount += cimg(l).size() ;
		}
		accumulator_set<double, stats<
			tag::min, tag::max, tag::count, 
			tag::sum, tag::mean, tag::variance, 
			tag::median, tag::density
		> > acc(tag::density::num_bins = _numBins(),tag::density::cache_size = pixCount);

		if(!_cimgMask.connected())
		{
			cimglist_for(cimg,l)
			{
				const cimg_library::CImg<T>& img = cimg(l) ;
				cimg_forXYZC(img, x,y,z,c)
				{
					acc(img(x,y,z,c)) ;
				}
			}
		}
		else
		{		//size checks
			const cimg_library::CImgList<T>& mask = _cimgMask() ;
			if(mask.size() != cimg.size() ||
					mask(0).width() != cimg(0).width() ||
					mask(0).height() != cimg(0).height() ||
					mask(0).depth() != cimg(0).depth() ||
					mask(0).spectrum() != cimg(0).spectrum())
			{	sout << "Dimensions of mask and input array do not match, skipping calculation" << std::endl ;
					continue ;
			}
			cimglist_for(cimg,l)
			{
				const cimg_library::CImg<T>& img = cimg(l) ;
				cimg_forXYZC(img, x,y,z,c)
				{
					if(mask(l,x,y,z,c))
						acc(img(x,y,z,c)) ;
				}
			}
		}

		Statistics s ;

		typedef std::pair<std::string, double> StatPair ;
		s.stats.insert(StatPair("dim0 : width",cimg(0).width())) ;
		s.stats.insert(StatPair("dim1 : height",cimg(0).height())) ;
		s.stats.insert(StatPair("dim2 : depth",cimg(0).depth())) ;
		s.stats.insert(StatPair("dim3 : slices",cimg(0).spectrum())) ; // <- this is no mixup, we designate dimension 5 as spectrum
		s.stats.insert(StatPair("dim4 : spectrum",cimg.width())) ;
		s.stats.insert(StatPair("number of pixels",count(acc))) ;
		s.stats.insert(StatPair("sum",sum(acc))) ;
		s.stats.insert(StatPair("min",min(acc))) ;
		s.stats.insert(StatPair("max",max(acc))) ;
		s.stats.insert(StatPair("mean",mean(acc))) ;
		s.stats.insert(StatPair("variance",variance(acc))) ;
		s.stats.insert(StatPair("stddev",sqrt(variance(acc)))) ;
		s.stats.insert(StatPair("median",median(acc))) ;
		s.origin = name ;

		_statistics.push_back(s) ;

		if(_writeToSout())
		{
				sout << "Statistics for " << name ;
				std::map<std::string,double>::const_iterator it = s.stats.begin() ;
				for(; it != s.stats.end() ; it++)
				{	sout << "\n\t" << it->first << ": " << it->second ;	}
				sout << std::endl ;
		}
	
		if(_histograms.connected())
		{
			CImg<unsigned char> histplot = histogramPlot(density(acc), mean(acc)) ;
			_histograms()[0].append(histplot.get_shared_channel(0),'c') ;
			_histograms()[1].append(histplot.get_shared_channel(1),'c') ;
			_histograms()[2].append(histplot.get_shared_channel(2),'c') ;
		}
	}

	if(_display.connected() && _exportWidget)
	{	
		//_exportWidget->setTitle(this->getName()) ;
		_exportWidget->updateStats(_statistics) ;
	}
}



} // namespace StatisticsDisplay
#endif /* _STATISTICSDISPLAY_HXX_ */
