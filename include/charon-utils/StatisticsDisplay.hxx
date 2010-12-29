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

namespace StatisticsDisplay {

template <typename T>
StatisticsDisplayPlugin<T>::StatisticsDisplayPlugin(const std::string& name) :
		TemplatedParameteredObject<T>("StatisticsDisplay", name,
			"Calculates various statistical properties of input object and exports a QWidget for display"),
			_in(false, true),
			_mask(true, false),
			_writeToSout(true),
			_exportWidget(0)
{
	ParameteredObject::_addInputSlot(
			_in, "in",
			"The vigra::MultiArray<5, T> input.",
			"vigraArray5<T>");

	ParameteredObject::_addInputSlot(
			_mask, "mask",
			"Mask to determine where the statistics should be calculated.<br>"
			"This is not a multislot as there is no possibility to map multiple mask to the corresponding images in \"in\"<br>"
			"The mask will be applied to all images as long as their dimensions match.",
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
StatisticsDisplayPlugin<T>::~StatisticsDisplayPlugin()
{
	//dont't delete _exportWidget, its parent widget will take care of this
}

template <typename T>
void StatisticsDisplayPlugin<T>::execute() {
	//#pragma warning(push)
	//#pragma warning(disable : 4244)
	typedef vigra::MultiArrayView<5, T> Array ;

	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	
	//get pointer to each object in Multislot and the name of the corresponding parent object
	std::map<const Array* const, std::string> parentNames ;
	std::set<AbstractSlot<Array>*>::const_iterator it = _in.begin() ;
	std::set<AbstractSlot<Array>*>::const_iterator end = _in.end() ;

	/*
	size_t index = 0 ;
	for(; it != end ; it++, index++)
	{
		std::string name = (*it)->getParent().getName() ;
		sout << "Target: " << name << std::endl ;
		
		//WARNING: It is not clear that the slot iterators and the operator[] of _in will return their objects in the same order
		//pointer-to-name correspondences may be wrong
		parentNames.insert(std::pair<const Array* const, std::string> ( &(_in[index]),name)) ;
	}*/
	
	for(std::size_t ii = 0 ; (ii < _in.size()) && (it != end) ; ii++, it++)
	{
		std::string name = (*it)->getParent().getName() ;

		accumulator_set<double, stats<tag::min, tag::max, tag::count, tag::sum, tag::mean, tag::variance, tag::median> > acc;

		const Array& img = _in[ii] ;
		if(!_mask.connected())
		{	
			vigra::inspectMultiArray(srcMultiArrayRange(img), acc);
		}
		else
		{
			const Array& mask = _mask() ;
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
				for(int yy = 0 ; yy < mask.size(0) ; yy++)
					for(int zz = 0 ; zz < mask.size(0) ; zz++)
						for(int tt = 0 ; tt < mask.size(0) ; tt++)
							for(int vv = 0 ; vv < mask.size(0) ; vv++)
								if(mask(xx,yy,zz,tt,vv) != 0)
									acc(img(xx,yy,zz,tt,vv)) ;
		}
		


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

		//get name of parent name if present in map
		if(_writeToSout())
		{
			sout << "Statistics for " << name ;
			std::map<std::string,double>::const_iterator it = s.stats.begin() ;
			for(; it != s.stats.end() ; it++)
			{	sout << "\n\t" << it->first << ": " << it->second ;	}
			sout << std::endl ;
		}
	}
	
	createWidget() ;
	//#pragma warning(pop)
}

template <typename T>
void StatisticsDisplayPlugin<T>::createWidget()
{
	if(!_display.connected())
	{	return ;	}

	QTabWidget* tabWidget = new QTabWidget ;
		tabWidget->setUsesScrollButtons(true) ;
		tabWidget->setTabPosition(QTabWidget::East) ;
	_exportWidget = tabWidget ;
	_exportWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum) ;
	_exportWidget->setObjectName(QString("Statistics")) ;
	std::vector<Statistics>::const_iterator it1 = _statistics.begin() ;
	for(; it1 != _statistics.end() ; it1++)
	{
		QWidget* tab = new QWidget ;
		QVBoxLayout* vLayout = new QVBoxLayout ;
		vLayout->setContentsMargins(1,1,1,1) ;
		QGridLayout* layout = new QGridLayout ;
		const Statistics& s = (*it1) ;
		layout->setContentsMargins(1,1,1,1) ;

		std::map<std::string, double>::const_iterator it = s.stats.begin() ;
		int row = 0 ;
		for(; it != s.stats.end() ; it++)
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
		tab->setLayout(vLayout) ;
		tabWidget->addTab(tab, QString::fromStdString(s.origin)) ;
	}
	_display() = _exportWidget ;
}

} // namespace StatisticsDisplay
#endif /* _STATISTICSDISPLAY_HXX_ */
