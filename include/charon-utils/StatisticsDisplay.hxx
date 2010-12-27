/*  Copyright (C) 2010 Stephan Meister

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
/** \file StatisticsDisplay.hxx
 *  Implementation of the class StatisticsDisplay.
 *  \author Stephan Meister
 *  \date 18.12.2010
 */

#ifndef _STATISTICSDISPLAY_HXX_
#define _STATISTICSDISPLAY_HXX_

#include <charon-utils/StatisticsDisplay.h>
#include <sstream>
#include <vigra/multi_array.hxx>
#include <vigra/multi_iterator.hxx>
#include <vigra/navigator.hxx>
#include <vigra/multi_pointoperators.hxx>
#include <QtGui>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/count.hpp>
using namespace boost::accumulators;


template <typename T>
StatisticsDisplay<T>::StatisticsDisplay(const std::string& name) :
		TemplatedParameteredObject<T>("StatisticsDisplay", name,
			"Calculates various statistical properties of input object and extports a QWidget for display"),
			_in(false, true),
			_writeToSout(true),
			_exportWidget(0)
{
	ParameteredObject::_addInputSlot(
			_in, "in",
			"The vigra::MultiArray<5, T> input.",
			"vigraArray5<T>");

	ParameteredObject::_addOutputSlot(
			_display, "display",
			"A QWidget displaying all results, can for example be connected to a ArgosDisplay instance",
			"QWidget*") ;
			
	ParameteredObject::_addParameter<bool>(_writeToSout, "writeToSout",
		"write results to stdout and status console", true, "bool");
	
	/*
	if(!qApp)
	{
		sout << "StatisticsDisplay::No QApplication found! " 
			<< "ArgosDisplay can only be used in a Qt GUI Application!" << std::endl ;
		return ;
	}
	*/
}

template <typename T>
StatisticsDisplay<T>::~StatisticsDisplay()
{
	//dont't delete _exportWidget, its parent widget will take care of this
}

template <typename T>
void StatisticsDisplay<T>::execute() {
	//#pragma warning(push)
	//#pragma warning(disable : 4244)
	typedef vigra::MultiArrayView<5, T> Array ;

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	
	//get pointer to each object in Multislot and the name of the corresponding parent object
	std::map<const Array* const, std::string> parentNames ;
	std::set<AbstractSlot<Array>*>::const_iterator it = _in.begin() ;
	std::set<AbstractSlot<Array>*>::const_iterator end = _in.end() ;

	size_t index = 0 ;
	for(; it != end ; it++, index++)
	{
		std::string name = (*it)->getParent().getName() ;
		sout << "Target: " << name << std::endl ;
		
		//WARNING: It is not clear that the slot iterators and the operator[] of _in will return their objects in the same order
		//pointer-to-name correspondences may be wrong
		parentNames.insert(std::pair<const Array* const, std::string> ( &(_in[index]),name)) ;
	}
	
	for(std::size_t ii = 0 ; ii < _in.size() ; ii++)
	{
		accumulator_set<double, stats<tag::min, tag::max, tag::count, tag::sum, tag::mean, tag::variance, tag::median> > acc;

		const Array& img = _in[ii] ;
		vigra::inspectMultiArray(srcMultiArrayRange(img), acc);

		/*
		typedef vigra::MultiArray<5, T>  Array;
		typedef vigra::MultiArrayNavigator<Array::const_traverser, 5> Navigator;

		Navigator nav(img.traverser_begin(), img.shape(), 0) ;	
        for(; nav.hasMore(); nav++)
        {
			// inner loop: linear iteration over current subset
			//             d == {0, 1, 2}: interate along {x, y, z}-axis respectively
			Navigator::iterator i = nav.begin(), end = nav.end();
			for(; i != end; ++i)
			{	acc(*i) ;	}
		}
		*/
		_statistics.push_back(StatMap()) ;
		StatMap& statistics = _statistics.back() ;
		typedef std::pair<std::string, double> StatPair ;
		statistics.insert(StatPair("number of pixels",count(acc))) ;
		statistics.insert(StatPair("sum",sum(acc))) ;
		statistics.insert(StatPair("min",min(acc))) ;
		statistics.insert(StatPair("max",max(acc))) ;
		statistics.insert(StatPair("mean",mean(acc))) ;
		statistics.insert(StatPair("variance",variance(acc))) ;
		statistics.insert(StatPair("stddev",sqrt(variance(acc)))) ;
		statistics.insert(StatPair("median",median(acc))) ;

		//get name of parent name if present in map
		sout << "Statistics for " << ((parentNames.count(&img) == 1) ? parentNames[&img] : "unknown") ;
		std::map<std::string,double>::const_iterator it = statistics.begin() ;
		for(; it != statistics.end() ; it++)
		{	sout << "\n\t" << it->first << ": " << it->second ;	}
		sout << std::endl ;
	}
	
	createWidget() ;
	//#pragma warning(pop)
}

template <typename T>
void StatisticsDisplay<T>::createWidget()
{
	if(!_display.connected())
	{	return ;	}
	_exportWidget = new QWidget ;
	_exportWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum) ;
	_exportWidget->setObjectName(QString("Statistics")) ;
	QVBoxLayout* vLayout = new QVBoxLayout ;
	QGridLayout* layout = new QGridLayout ;
	const StatMap& statistics = _statistics[0] ;
	layout->setContentsMargins(1,1,1,1) ;

	StatMap::const_iterator it = statistics.begin() ;
	int row = 0 ;
	for(; it != statistics.end() ; it++)
	{
		QLabel* label = new QLabel(QString::fromStdString(it->first), _exportWidget) ;
		QLineEdit* line = new QLineEdit(QString("%2").arg(it->second), _exportWidget) ;
		line->setReadOnly(true) ;
		line->setFrame(false) ;
		layout->addWidget(label, row, 0) ;
		layout->addWidget(line, row++, 1) ;
	}
	vLayout->addLayout(layout) ;
	vLayout->addStretch() ;
	_exportWidget->setLayout(vLayout) ;

	_display() = _exportWidget ;
}

#endif /* _STATISTICSDISPLAY_HXX_ */
