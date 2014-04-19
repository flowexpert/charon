/*  This file is part of Charon.

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
/** @file   ImgTool.hxx
 *  @brief  ImgTool routine implementations
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   05.06.2009
 *
 *  This file contains the routine implementation.
 *  Interface is in ImgTool.h.
 *  Include ImgTool.h, if you only need the function declarations (prototypes).
 */

#ifndef _ImgTool_HXX
#define _ImgTool_HXX

#include "ImgTool.h"

#include <cassert>
#include <limits>
#include <cfloat>
#include <iomanip>
#include <climits>
#include "Roi.h"
#include "InterpolatorLinear.h"

/// circle number \f$\pi\f$
#define PI 3.14159265358979323846

#undef max
#undef min

template <typename T>
void ImgTool::normalizeSlices(cimg_library::CImg<T> &img, T minVal, T maxVal) {
    cimg_forZ(img, z) {
        T iMin =  std::numeric_limits<T>::max();
        T iMax = -std::numeric_limits<T>::max();

        cimg_forXYC(img, x, y, v) {
            if (img(x, y, z, v) > iMax) iMax = img(x, y, z, v);
            if (img(x, y, z, v) < iMin) iMin = img(x, y, z, v);
        }

        cimg_forXYC(img, x, y, v)
            img(x, y, z, v) = minVal+(img(x, y, z, v)-iMin)/(iMax-iMin)*maxVal;
    }
}

template <typename T, typename T2>
void ImgTool::integralImage2D(const cimg_library::CImg<T >& src,
                                    cimg_library::CImg<T2>& dst) {
    dst.assign(src.width(), src.height(), src.depth(), src.spectrum());
    cimg_forZC(src, z, v) {
        dst(0, 0, z, v) = src(0, 0, z, v);

        for (int x = 1; x < src.width(); ++x)
            dst(x, 0, z, v) = dst(x-1, 0, z, v) + src(x, 0, z, v);

        for (int y = 1; y < src.height(); ++y)
            dst(0, y, z, v) = dst(0, y-1, z, v) + src(0, y, z, v);

        for (int x = 1; x < src.width(); ++x)
            for (int y = 1; y < src.height(); ++y)
                dst(x, y, z, v) =   dst(x-1, y, z, v)   + dst(x, y-1, z, v)
                                  - dst(x-1, y-1, z, v) + src(x, y, z, v);
    }
}

template <typename T>
void ImgTool::histogram(const cimg_library::CImg<T>& img,
                        cimg_library::CImg<T>& hist,
                        int nBins, T minVal, T maxVal,
                        bool normalize, const Roi<int>* roi) {
	// check preconditions
    assert (maxVal > minVal);
    assert (nBins > 0);

    hist.assign(nBins, 1);
    hist.fill(T(0));
    double factor = 1.f / ((double)maxVal - (double)minVal) * (nBins-1.f);
	
	if (roi) {
		cimg_for_inXYZC(img,roi->xBegin(),roi->yBegin(),roi->zBegin(),roi->tBegin(),
			roi->xEnd()-1,roi->yEnd()-1,roi->zEnd()-1,roi->tEnd()-1,x,y,z,v)
		{
			int bin = (int)floor((img(x,y,z,v) - minVal)*factor);

			// if max and min are not the actual max and min,
			// we need to truncate the histogram
			bin = (bin < 0 ? 0 : bin);
			bin = (bin >= nBins ? nBins - 1 : bin);
			hist(bin)++;
		}
		if (normalize)
			// normalize histogram
			hist /= roi->getVolume();
	}
	else {
		cimg_for(img,ptr,T) {
			int bin = (int)floor((*ptr - minVal)*factor);

			// if max and min are not the actual max and min,
			// we need to truncate the histogram
			bin = (bin < 0 ? 0 : bin);
			bin = (bin >= nBins ? nBins - 1 : bin);
			hist(bin)++;
		}
		if (normalize)
			hist /= (T) img.size();
	}
}

template <typename T> T ImgTool::jointHistogram(
                cimg_library::CImg<T>& src1, cimg_library::CImg<T>& src2,
                T min, T max,
                cimg_library::CImg<T>& hist,
                Roi<int> roi,
                bool normalize) {
    //image should be quaradatic (same resolution for both intensity ranges)
    assert(hist.width() == hist.height());

    // preconditions
    assert(min < max);
    assert(roi.xBegin() < roi.xEnd() && roi.yBegin() < roi.yEnd());
    assert(roi.xBegin() >= 0 && roi.xEnd() <= src1.width());
    assert(roi.yBegin() >= 0 && roi.yEnd() <= src1.height());
    assert(src2.height() == src1.height() && src1.width() == src2.width());

    int xBin, yBin, nBins = hist.width();
    T factor = 1.0f / (max - min) * (nBins-1);
    hist.fill(0);
    T resmax = 0;

	forRoiXY(roi,x,y) {
       xBin = (int)floor((src1(x, y) - min)*factor);
       yBin = (int)floor((src2(x, y) - min)*factor);
       // if max and min are not the actual max and min,
       // we need to truncate the histogram
       xBin = (xBin < 0)      ? 0       : xBin;
       xBin = (xBin >= nBins) ? nBins-1 : xBin;
       yBin = (yBin < 0)      ? 0       : yBin;
       yBin = (yBin >= nBins) ? nBins-1 : yBin;

       ++hist(xBin, yBin);
       resmax = (hist(xBin, yBin) > resmax) ? hist(xBin, yBin) : resmax;
    }

    if (normalize) {
        hist   /= roi.getVolume();
        resmax /= roi.getVolume();
    }

    return resmax;
}

template <typename T>
void ImgTool::warp2D(
		const cimg_library::CImg<T>& src,
		const cimg_library::CImgList<T>& flow,
		cimg_library::CImg<T>& dst,
		const Interpolator<T>* interpolator,
		float weight) {
	// check preconditions
	assert(flow.size() >= 2);
	assert(flow[0].is_sameXYZC(src));
	assert(flow[1].is_sameXYZC(src));
	if(!dst.is_sameXYZC(src))
		dst.assign(src.width(), src.height(), src.depth(), src.spectrum());

	cimg_forXYZC(src,x,y,z,t) {
		float cx = float(T(x) + weight*flow[0](x,y,z,t));
		float cy = float(T(y) + weight*flow[1](x,y,z,t));
		T res = interpolator->interpolate(src, cx, cy, int(z), int(t));
		dst(x,y,z,t) = res;
	}
}

template <typename T>
void ImgTool::warpToFirstFrame(const cimg_library::CImg<T>& src,
                               const cimg_library::CImgList<T>& flow,
                               cimg_library::CImg<T>& dst,
                               const Interpolator<T>* interpolator) {
    // check preconditions
    assert(flow.size >= 2);
    assert(flow[0].is_sameXYZ(src));
    assert(flow[1].is_sameXYZ(src));
    assert(flow[0].spectrum() == flow[1].spectrum());
    if(!(dst.is_sameXYZ(src) || dst.spectrum() != flow[0].spectrum()+1))
        dst.assign(src.width(), src.height(), src.depth(), flow[0].spectrum()+1);

    cimg_library::CImg<T> accFlow(2), temp(2);
    cimg_forXYZ(dst,x,y,z) {
        //set first image to source image
        dst(x,y,z,0) = src(x,y,z,0);
        //accumulates flow over all images
        accFlow.fill(0);
        for(int t = 0; t < flow[0].spectrum(); t++) {
            for(int v = 0; v < 2; v++)
                temp(v) = interpolator->interpolate(flow[v],
                    (float)(x+accFlow(0)), (float)(y+accFlow(1)), z, t);

            accFlow += temp;
            dst(x,y,z,t+1) = interpolator->interpolate(src,
                (float)(x+accFlow(0)), (float)(y+accFlow(1)), z, 0);
        }
    }
}

template <typename T>
void ImgTool::meanVarianceFromHistogram(
        const cimg_library::CImg<T>& hist,
        T imgMin, T imgMax,
        T& mean, T& variance) {

    T iVal, mVal;
    mean = 0;
    variance = 0;
    cimg_forX(hist, i) {
        iVal      = imgMin + (imgMax-imgMin) / hist.width() * i;
        mVal      = hist(i) * iVal;
        mean     += mVal;
        variance += mVal * iVal;
    }
    variance -= mean * mean;
}

template <typename T>
T ImgTool::covariance(const cimg_library::CImg<T>& jointHist,
                      T mean1, T mean2, T imgMin, T imgMax) {
    // preconditions
    assert(jointHist.width() == jointHist.height());
    assert(jointHist.depth() == 1);

    int nBins = jointHist.width();
    T v12     = 0;

    for(int i = 0; i < nBins; ++i)
        for(int j =0; j < nBins; ++j) {
            // take values as center of bin and assume that hist(0 -> imgMin)
            T iVal = imgMin + (imgMax-imgMin) / jointHist.width() * i;
            T jVal = imgMin + (imgMax-imgMin) / jointHist.height() * j;
            v12 += iVal * jVal * jointHist(i, j);
        }

    v12 -= mean1 * mean2;

    return v12;
}

template <typename T>
void ImgTool::toGray(cimg_library::CImg<T>& img) {
    if (img.spectrum() != 1) {
        cimg_forXYZ(img, x, y, z) {
            double tmp = 0;
            cimg_forC(img, v)
                tmp += (double) img(x, y, z, v);
            img(x, y, z) = (T) (tmp/img.spectrum());
        }
        img.resize(img.width(), img.height(), img.depth(), 1, 0);
    }
}

template <typename T>
void ImgTool::coNormalize(cimg_library::CImgList<T>& list, T min, T max) {
    T offset = -(list.min()) + min;
    T factor = max / (list.max()-list.min());
    for (int i = 0; i < list.size; ++i) {
        list[i] += offset;
        list[i] *= factor;
    }
}

template <typename T>
void ImgTool::loadSequence(const std::vector<std::string>& files,
                           cimg_library::CImg<T>& seq) {
    cimg_library::CImg<T> file;
    for(int i = 0; i < files.size(); ++i) {
        //load single image
        file.load(files[i].c_str());

        if (!i)
            seq.assign(file.width(), file.height(), files.size(), file.spectrum());
        else
            //assure that all images are same size as first image
            assert(file.is_sameXY(seq) && file.is_sameV(seq));

        //copy image into sequence buffer
        cimg_forXYC(file, x, y, v) {
            seq(x, y, i, v) = file(x, y, 0, v);
        }
    }
}

template <typename T>
void ImgTool::ZtoV(const cimg_library::CImg<T>& src,
                   cimg_library::CImg<T>& dst) {
    dst.assign(src.width(), src.height(), 1, src.depth());
    cimg_forXY(src, x, y)
        cimg_forC(dst, v) {
            dst(x, y, 0, v) = src(x, y, v, 0);
        }
}

template <typename T>
void ImgTool::VtoZ(const cimg_library::CImg<T>& src,
                   cimg_library::CImg<T>& dst) {
    dst.assign(src.width(), src.height(), src.spectrum(), 1);
    cimg_forXY(src, x, y)
        cimg_forZ(dst, z) {
            dst(x, y, z, 0) = src(x, y, 0, z);
        }
}

template <typename T>
void ImgTool::chessBoard2D(int fieldSizeX, int fieldSizeY,
                           cimg_library::CImg<T>& dst) {
    int nx = dst.width() / fieldSizeX + 1;
    int ny = dst.height() / fieldSizeY + 1;
    T white = 1;
    T black = 0;
    T color;
    for(int y = 0; y < ny; ++y)
        for(int x = 0; x < nx; ++x) {
            if ((x+y)%2)
                color = white;
            else
                color = black;
            dst.draw_rectangle(    x  * fieldSizeX,    y  * fieldSizeY,
                                (x+1) * fieldSizeX, (y+1) * fieldSizeY,
                                &color);
        }
}

template <typename T>
void ImgTool::saveSequence(std::string fileName, std::string fileType,
                           const cimg_library::CImg<T>& sequence) {
    for(int i = 0; i < sequence.depth(); ++i) {
        cimg_library::CImg<T> slice = sequence.get_slice(i);
        std::ostringstream fname;
        fname << fileName;
        fname << std::setw(4) << std::setfill('0') << i;
        fname << "." << fileType;
        slice.save(fname.str().c_str());
    }
}

template <typename T>
void ImgTool::loadSequence(std::string fileName, int startFrame, int endFrame,
                           cimg_library::CImg<T>& sequence) {
    char *filename = new char[fileName.size()+5];

    assert(startFrame <= endFrame);
    assert(startFrame >= 0 && startFrame < 9999);
    assert(endFrame   >= 0 && endFrame   < 9999);

    for(int i = startFrame; i <= endFrame; ++i) {
        sprintf(filename, fileName.c_str(), i);

        cimg_library::CImg<T> slice;
        slice.load(filename);
        if(i == startFrame)
            sequence.assign(slice.width(), slice.height(),
                            endFrame-startFrame+1, slice.spectrum());

        cimg_forXYC(slice, x, y, v)
            sequence(x, y, i-startFrame, v) =slice(x, y, 0, v);
    }
    delete filename;
}

template <typename T>
void ImgTool::loadSequenceRaw(std::string fileName,
                              int startFrame, int endFrame,
                              int sizeX, int sizeY, int sizeV,
                              cimg_library::CImg<T>& sequence) {
    char *filename = new char[fileName.size()+5];
    assert(startFrame >= 0 && startFrame < 9999);
    assert(endFrame   >= 0 && endFrame   < 9999);

    for (int i = startFrame; i <= endFrame; ++i) {
        sprintf(filename, fileName.c_str(), i);

        cimg_library::CImg<T> slice;
        slice.load_raw(filename, sizeX, sizeY, 1, sizeV);
        if (i == startFrame)
            sequence.assign(slice.width(), slice.height(),
                            endFrame-startFrame+1, slice.spectrum());

        cimg_forXYC(slice, x, y, v)
            sequence(x, y, i-startFrame, v) = slice(x, y, 0, v);
    }

    delete filename;
}

template <typename T>
void ImgTool::extendRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img) {
    cimg_forZC(img, z, v) {
        // top
        T tl = img(roi.xBegin(), roi.yBegin(), z, v);
        T tr = img(roi.xEnd()-1, roi.yBegin(), z, v);
        for (int y = 0; y < roi.yBegin(); ++y) {
            for (int x = 0; x < roi.xBegin(); ++x)
                img(x, y, z, v) = tl;
            for (int x = roi.xBegin(); x < roi.xEnd(); ++x)
                img(x, y, z, v) = img(x, roi.yBegin(), z, v);
            for (int x = roi.xEnd(); x < img.width(); ++x)
                img(x, y, z, v) = tr;
        }

        // bottom
        T bl = img(roi.xBegin(), roi.yEnd()-1);
        T br = img(roi.xEnd()-1, roi.yEnd()-1);
        for (int y = roi.yEnd(); y < img.height(); ++y) {
            for(int x = 0; x < roi.xBegin(); ++x)
                img(x, y, z, v) = bl;
            for(int x = roi.xBegin(); x < roi.xEnd(); ++x)
                img(x, y, z, v) = img(x, roi.yEnd()-1, z, v);
            for(int x = roi.xEnd(); x < img.width(); ++x)
                img(x, y, z, v) = br;
        }

        // left && right
        for(int y = roi.yBegin(); y < roi.yEnd(); ++y) {
            T l = img(roi.xBegin(), y, z, v);
            T r = img(roi.xEnd()-1, y, z, v);
            for(int x = 0; x < roi.xBegin(); ++x)
                img(x, y, z, v) = l;
            for(int x = roi.xEnd(); x < img.width(); ++x)
                img(x, y, z, v) = r;
        }
    }
}

template <typename T>
void ImgTool::zeroRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img) {
    cimg_forZC(img, z, v) {
        //top
        for(int y = 0; y < roi.yBegin(); ++y)
            for(int x = 0; x < img.width(); ++x)
                img(x, y, z, v) = 0;

        //bottom
        for(int y = roi.yEnd(); y < img.height(); ++y)
            for(int x = 0; x < img.width(); ++x)
                img(x, y, z, v) = 0;

        //left && right
        for(int y = roi.yBegin(); y < roi.yEnd(); ++y) {
            for(int x =0; x < roi.xBegin(); ++x)
                img(x, y, z, v) = 0;
            for(int x =roi.xEnd(); x < img.width(); ++x)
                img(x, y, z, v) = 0;
        }
    }
}

template <typename T>
void ImgTool::mirrorRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img) {
    int x      = roi.xBegin();
    int y      = roi.yBegin();
    int width  = roi.getWidth();
    int height = roi.getHeight();

    // Valid input parameters
    assert(x >= 0 && y >= 0 && width > 0 && height > 0);

    // Correct ROI setting
    assert(img.width() >= x + width);
    assert(img.height() >= y + height);   // roi within the image
    assert(x <= width && y <= height);  // enough ROI data for left/top padding

    int i,j,k;
    int offsetX = x;
    int offsetY = y;

    // First mirror left/right margin (excluding upper/bottom corner)
    for(i=offsetY; i < offsetY+height; i++) {
        // left side
        for(j=0, k=2*offsetX-1; j<offsetX; k--, j++)
            cimg_forZC(img, z, v)
                img(j, i, z, v) = img(k, i, z, v);

        // right side
        for(j=width+offsetX, k=width+offsetX-1; j<img.width(); k--, j++)
            cimg_forZC(img, z, v)
                img(j, i, z, v) = img(k, i, z, v);
    }

    // Then mirror top/bottom margin (including 4 corners)
    // top side
    for(i=0, k=2*offsetY-1; i<offsetY; k--, i++)
        cimg_forZC(img, z, v)
            for(int x = 0; x < img.width(); ++x)
                img(x, i, z, v) = img(x, k, z, v);

    // bottom side
    for(i=height+offsetY, k=height+offsetY-1; i<img.height(); k--, i++)
        cimg_forZC(img, z, v)
            for(int x =0; x < img.width(); ++x)
                img(x, i, z, v) =img(x, k, z, v);
}

template <typename T>
cimg_library::CImg<T> ImgTool::sinus2D(T frequencyX, T frequencyY,
                                       T phaseX, T phaseY,
                                       int sizeX, int sizeY) {
    cimg_library::CImg<T> res(sizeX, sizeY);
    cimg_forXY(res, x, y)
        res(x, y) =  (T) sin(frequencyX * (x+phaseX))
                    +(T) sin(frequencyY * (y+phaseY));

    return res;
}

template <typename T>
void ImgTool::toSequence(const cimg_library::CImgList<T>& list,
                         cimg_library::CImg<T>& dst) {
    dst.assign(list[0].width(), list[0].height(), list.size);
    for (unsigned int i = 0; i < list.size; ++i) {
        assert(list[i].width() == dst.width());
        assert(list[i].height() == dst.height());
        assert(list[i].depth() == 1);
        assert(list[i].spectrum() == dst.spectrum());

        cimg_forXYC(dst, x, y, v)
            dst(x, y, i, v) = list[i](x, y, 0, v);
    }
}

template <typename T>
void ImgTool::toSlice(const cimg_library::CImg<T>& slice,
                      cimg_library::CImg<T>& dst,
                      int sliceIndex) {
    assert(slice.spectrum() == dst.spectrum());
    assert(slice.width() == dst.width());
    assert(slice.height() == dst.height());
    assert(sliceIndex < dst.depth());
    assert(sliceIndex >= 0);
    assert(slice.depth() == 1);

    cimg_forXYC(slice, x, y, v)
        dst(x, y, sliceIndex, v) = slice(x, y, 0, v);
}

template <typename T>
void ImgTool::toPlane(const cimg_library::CImg<T>& plane,
                      cimg_library::CImg<T>& dst,
                      int planeIndex) {
    assert(plane.depth() == dst.depth());
    assert(plane.width() == dst.width());
    assert(plane.height() == dst.height());
    assert(planeIndex   <  dst.spectrum());
    assert(planeIndex   >= 0);
    assert(plane.spectrum() == 1);

    cimg_forXYZ(plane, x, y, z)
        dst(x, y, z, planeIndex) = plane(x, y, z, 0);
}

template <typename T>
void ImgTool::localVarianceRoot2D(int maskSize,
                                  const cimg_library::CImg<T>& src,
                                  cimg_library::CImg<T>& dst) {
    if(dst.width() != src.width() || dst.height() != src.height())
        dst.assign(src.width(), src.height());

    if(maskSize % 2 == 0) maskSize++;
    //mask size in one direction (4 -> mask==9)
    int ms = maskSize/2;
    T size = (T) (2*ms+1)*(2*ms+1);

    cimg_forXY(src, x, y) {
        cimg_library::CImg<T> tmp((2*ms+1), (2*ms+1));

        for(int wx = -ms; wx <= +ms; ++wx)
            for(int wy = -ms; wy <= +ms; ++wy)
                tmp(wx+ms, wy+ms) = src.atXY(x+wx, y+wy);

        T w = (T)tmp.variance();
        dst(x, y) = (T)sqrt(w);
    }
}

template <typename T>
void ImgTool::localNormalization2D(int maskSize,
                                   T vMin, T vMax,
                                   const cimg_library::CImg<T>& src,
                                         cimg_library::CImg<T>& dst) {
    if(dst.width() != src.width() || dst.height() != src.height())
        dst.assign(src.width(), src.height());

    if(maskSize % 2 == 0) maskSize++;

    //mask size in one direction (4 -> mask==9)
    int ms = maskSize/2;
    T size = (T) (2*ms+1)*(2*ms+1);

    cimg_forXY(src, x, y) {
        T min =  FLT_MAX;
        T max = -FLT_MAX;
        for(int wx =-ms; wx <= +ms; ++wx)
            for(int wy =-ms; wy <= +ms; ++wy) {
                T v = src.atXY(x+wx, y+wy);
                if(v < min) min = v;
                if(v > max) max = v;
            }

        if (max == min)
            dst(x, y) = 0;
        else
            //local normalization to 0.0f..1.0f
            dst(x, y) = (src(x, y)-min)/(max-min);
    }

    dst.normalize(vMax, vMin);
}

template <typename T>
void ImgTool::sigmoidThreshold(T center, T sharpness,
                               const cimg_library::CImg<T>& src,
                               cimg_library::CImg<T>& dst) {
    if(dst.width() != src.width() || dst.height() != src.height()
            || dst.depth() != src.depth() || dst.spectrum() != src.spectrum())
        dst.assign(src.width(), src.height(), src.depth(), src.spectrum());

    cimg_forXYZC(src, x, y, z, v)
        dst(x, y, z, v)=1/(1+(T)exp(-sharpness*(src(x, y, z, v)-center)));
}

template <typename T>
void ImgTool::copy(const cimg_library::CImg<T>& from,
                         cimg_library::CImg<T>& to,
                   int left, int top, int front) {
    assert(to.width() >= from.width()+left);
    assert(to.height() >= from.height()+top);
    assert(to.depth() >= from.depth()+front);
    assert(to.spectrum() == from.spectrum());

    cimg_forXYZC(from, x, y, z, v)
        to(x+left, y+top, z+front, v) = from(x, y, z, v);
}

template <typename T>
void ImgTool::createAllScaleRandomPattern(
		cimg_library::CImg<T>& dst,
		int width, int height, float eta, int levels) {
	assert(eta > 0 && eta <= 1.0f);
	int sx = width;
	int sy = height;
	dst.assign(sx,sy,1,1,0);

	for (int ii=0; ii<levels; ii++) {
		float factor = std::pow(2.f, (float)ii*eta);
		int nx = (int)ceilf(sx/factor);
		int ny = (int)ceilf(sy/factor);
		cimg_library::CImg<T> img(nx,ny,1,1,0);
		img.rand(0.f,1.f);
		cimg_library::CImg<T> tmp = img.get_resize(sx,sy,1,1,5);
		dst += tmp;
	}
	dst.blur(0.7f).normalize(0, 255);
}

#ifdef CHARON_GDI
template <typename T>
void ImgTool::fromWindowBMPHeader(LPBITMAPINFOHEADER pbmih,
                                  cimg_library::CImg<T>& dst) {
    int height  = pbmih->biHeight;
    int width   = pbmih->biWidth;
    long stride = (width * pbmih->biBitCount + 31) / 32 * 4;

    dst.assign(width, height, 1, 3, 0);

    //find dib bits
    WORD dibBits;
    /* calculate the size required by the palette */
    WORD wBitCount;  // DIB bit count
    WORD colors;
    /*  If this is a Windows-style DIB, the number of colors in the
     *  color table can be less than the number of bits per pixel
     *  allows for (i.e. lpbi->biClrUsed can be set to some value).
     *  If this is the case, return the appropriate value.
     */
#ifndef IS_WIN30_DIB
#define IS_WIN30_DIB(lpbi)   ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#endif
    if(IS_WIN30_DIB(pbmih)) {
        DWORD dwClrUsed;
        dwClrUsed = ((LPBITMAPINFOHEADER)pbmih)->biClrUsed;
        if (dwClrUsed != 0) {
            colors = (WORD) dwClrUsed;
            dibBits =((WORD) (colors * sizeof(RGBQUAD)));
        }
    }
    /*  Calculate the number of colors in the color table based on
     *  the number of bits per pixel for the DIB.
     */
    wBitCount = ((LPBITMAPINFOHEADER) pbmih)->biBitCount;
    colors = 1 << wBitCount;
    if(IS_WIN30_DIB(pbmih))
        dibBits =((WORD) (colors * sizeof(RGBQUAD)));
    else
        dibBits =((WORD) (colors * sizeof(RGBTRIPLE)));

    //setup pointers to bitmap data
    unsigned char* pData = (unsigned char*)(((char*)pbmih + *(LPDWORD) pbmih
                                             + dibBits)); // the image data
    unsigned char* pDataPointer;
    RGBQUAD* pColorTable = (RGBQUAD*) ((char *)pbmih + (*(LPDWORD) pbmih));
    assert(pData && pColorTable);

    int i,j;
    if(pbmih->biBitCount == 8) {
        // Require color look up table
        T table[256][3];
        for (i =0; i < 256; ++i) {
            table[i][0] = (T)pColorTable[i].rgbRed;
            table[i][1] = (T)pColorTable[i].rgbGreen;
            table[i][2] = (T)pColorTable[i].rgbBlue;
        }

        for(i =height-1; i >= 0; --i) {
            pDataPointer = pData + (height - 1 - i) * stride;
            for(j = 0; j < width; ++j) {
                for(int p =0; p < 3; ++p)
                    dst(j, i, 0, p) = table[*pDataPointer][p];
                ++pDataPointer;
            }
        }
    }
    else if (pbmih->biBitCount == 24 || pbmih->biBitCount == 32) {
        // True color bitmap
        int inc = 0;
        if (pbmih->biBitCount == 32) inc = 1;
        for(i =0; i < height; ++i) {
            pDataPointer = pData + (height - 1 - i) * stride;
            for(j = 0; j < width; ++j) {
                for(int p =0; p < 3; ++p)
                    dst(j, i, 0, 2-p) =(T)(*pDataPointer++);
                pDataPointer +=inc; // if 4 bytes per pixel
            }
        }
    }
}
#endif //CHARON_GDI

template <typename T>
void ImgTool::grayToHsv(const cimg_library::CImg<T>& gray,
                              cimg_library::CImg<T>& hsv,
                              bool drawColorBar) {
    assert(gray.spectrum() == 1);
    int add = drawColorBar ? 20 : 0;
    hsv.assign(gray.width()+add, gray.height(), gray.depth(), 3, 0);
    cimg_library::CImg<T> tmp = gray.get_normalize(0.2f,1);

    cimg_forXYZ(tmp, x, y, z) {
        hsv(x, y, z, 0) = (T)((tmp(x, y, z) * 355));
        hsv(x, y, z, 1) = 1;
        hsv(x, y, z, 2) = 1;
    }

    if (drawColorBar) {
        cimg_forYZ(hsv, y, z)
            for(int x = gray.width()+5; x < hsv.width(); ++x) {
                hsv(x, y, z, 0) = (T)(((1.0f-(y/(float)hsv.height()))
                                       * (1 - 0.2) + 0.2) * 355);
                hsv(x, y, z, 1) = 1;
                hsv(x, y, z, 2) = 1;
            }
    }
    hsv.HSVtoRGB();
}

template <typename T>
void ImgTool::drawHsvCircle(cimg_library::CImg<T>& rgbImage,
                            const Roi<int>& region) {
    int size = region.getWidth() > region.getHeight() ?
               region.getHeight() : region.getWidth();

    cimg_library::CImg<T> box(size, size, 1, 3, 0);

    cimg_forXY(box, x, y) {
        float px        = x-size/2.0f;
        float py        = y-size/2.0f;
        float len       = sqrtf(px*px+py*py);
        box(x, y, 0, 0) = (T)acosf(px/len);
        if(py > 0)
            box(x, y, 0, 0) = (T)(2*PI - box(x, y, 0, 0));
        box(x, y, 0, 1) = 1;
        box(x, y, 0, 2) = 1;
    }

    cimg_library::CImg<T> circle(size, size, 1, 1, 0);
    T white[] = {(T)1.0, (T)1.0, (T)1.0};
    circle.draw_circle(size/2, size/2, size/2, white);
    box.mul(circle);
    box.HSVtoRGB();
    rgbImage.draw_image(box, region.xBegin(), region.yBegin());
}

template <typename T>
void ImgTool::drawHsvFrame(cimg_library::CImg<T>& rgbImage, int imPos) {
    int w = 3;

    // starting point for frame
    // (if mask is shown no black boundary between hsv frame
    // and image should be shown)
    int s = 0;
    if (imPos < w)
        s = 0;
    else
        s = imPos - w;

    cimg_library::CImg<T> box(rgbImage.width(), rgbImage.height(), 1, 3, 0);
    cimg_forXY(rgbImage, x, y)
        if (x < s + w || y < s + w || x >= rgbImage.width() - s - w
                || y >= rgbImage.height() - s - w) {

            float px  = x-rgbImage.width()/2.0f;
            float py  = y-rgbImage.height()/2.0f;
            float len = sqrtf(px*px+py*py);
            box(x, y, 0, 0) = (T)acosf(px/len);
            if(py > 0) box(x, y, 0, 0) = (T)(2*PI - box(x, y, 0, 0));
            box(x, y, 0, 0) *= (T)(180/PI);
            box(x, y, 0, 1) = 1;
            box(x, y, 0, 2) = 1;
        }

    box.HSVtoRGB();

    cimg_forXY(rgbImage, x, y)
        if (x < s + w || y < s + w || x >= rgbImage.width() - s - w
                || y >= rgbImage.height() - s - w) {

            rgbImage(x, y, 0) = box(x, y, 0);
            rgbImage(x, y, 1) = box(x, y, 1);
            rgbImage(x, y, 2) = box(x, y, 2);
        }
}

template <typename T>
void ImgTool::flowToHsv(const cimg_library::CImg<T>& flow,
                              cimg_library::CImg<T>& hsv,
                        bool drawColorBar) {
    // if(!mask.size())
    //     mask.assign(flow.width(), flow.height(), flow.depth(), 1, 1);

    assert(flow.spectrum() == 2);
    int add = drawColorBar ? 20 : 0;
    hsv.assign(flow.width()+add, flow.height(), flow.depth(), 3, 0);

    cimg_library::CImg<T> tmp(flow.width(), flow.height(), flow.depth(), 3, 0);
    T max = 0;
    cimg_forXYZ(tmp, x, y, z) {
        // get angle between unit x vector (1, 0)
        // and flow vector (fx, fy): acos(fx*1+fy*0) = acos(fx)
        // and upgrade from 0..PI to 0..2PI
        T len = sqrt(flow(x, y, z, 0) * flow(x, y, z, 0)
                     + flow(x, y, z, 1) * flow(x, y, z, 1));
        hsv(x, y, z, 0) = (T)acos(flow(x, y, z, 0)/len);
        if (flow(x, y, z, 1) > 0)
            hsv(x, y, z, 0) = (T)(2*PI - hsv(x, y, z, 0));
        hsv(x, y, z, 0) *= (float)(180/PI);

        //set saturation to one
        hsv(x, y, z, 1) =1;

        //get length of vector as value
        hsv(x, y, z, 2) = sqrt(flow(x, y, z, 0) * flow(x, y, z, 0)
                               + flow(x, y, z, 1) * flow(x, y, z, 1));

        max = hsv(x, y, z, 2) > max ? hsv(x, y, z, 2) : max;
    }

    cimg_library::CImg<T> val = hsv.get_shared_channel(2);
    val /= max; //normalize value to one
    val += 0.05;
    val /= 1.05;
    hsv.HSVtoRGB();

    /*
    if(drawColorBar) {
        Roi<int> region(0, flow.width()+1, add, hsv.width());
        drawHsvCircle(hsv, region);
    }

    if(true)
        drawHsvFrame(hsv);
    */
}

template <typename T, typename T2>
void ImgTool::drawQuiverThick(      cimg_library::CImg<T >& dst,
                              const cimg_library::CImg<T2>& flow,
                              const T *const color,
                              const float thickness,
                              const unsigned int sampling,
                              const float factor,
                              const int quiver_type,
                              const float opacity) {
    cimg_library::CImg<T> colors(dst);
    cimg_forXYZC(colors, x, y, z, v)
        colors(x, y, z, v) = color[v];

    ImgTool::drawQuiverThick<T>(dst, flow, colors, thickness, sampling,
                                factor, quiver_type, opacity);
}

template <typename T, typename T2>
void ImgTool::drawQuiverThick(      cimg_library::CImg<T >& dst,
                              const cimg_library::CImg<T2>& flow,
                              const cimg_library::CImg<T >& colors,
                              const float thickness,
                              const unsigned int sampling,
                              const float factor,
                              const int quiver_type,
                              const float opacity) {
    assert(!dst.is_empty() && flow.spectrum() == 2);
    assert(colors.is_sameXYZ(dst) && sampling > 0);

    float vmax, fact;
    if (factor <= 0) {
        cimg_library::CImg<T> tmp = flow.get_pointwise_norm(2);
        vmax = (float)cimg_library::cimg::max(fabs((float)tmp.min()),
                                              fabs((float)tmp.max()));
        fact = -factor;
    }
    else
        fact = factor; vmax = 1;

    for (unsigned int y = sampling/2; y < dst.height; y += sampling)
        for (unsigned int x = sampling/2; x < dst.width; x += sampling) {
            const unsigned int X = x*flow.width/dst.width;
            const unsigned int Y = y*flow.height/dst.height;
            float u = (float)flow(X,Y,0,0)*fact/vmax;
            float v = (float)flow(X,Y,0,1)*fact/vmax;
            if (!quiver_type) {
                const int xx = x+(int)u, yy = y+(int)v;
                ImgTool::drawArrowThick<T,T2>(dst,x,y,xx,yy,
                    colors.get_crop(x, y, 0, 0, x, y, 0,colors.spectrum()-1).ptr(),
                    thickness,45.0f,thickness+2.0f,~0L,opacity);
            }
            else
                dst.draw_line((int)(x-0.5*u),(int)(y-0.5*v),
                    (int)(x+0.5*u), (int)(y+0.5*v),
                    colors.get_crop(x, y, 0, 0,x, y, 0,colors.spectrum()-1).ptr(),
                    opacity,~0L);
        }
}

/*
// Draw an arrow with thick lines
template <typename T>
void ImgTool::drawArrowThick(
        const int x0, const int y0,
        const int x1, const int y1,
        const T *const color, const float thickness=3,
        const float angle=30, const float length=-10,
        const unsigned int pattern=~0L,
        const float opacity=1) {

    if (!is_empty()) {
        const float u = (float)(x0-x1);
        const float v = (float)(y0-y1);
        const float sq = u*u+v*v;
        const float deg = (float)(angle*cimg_library::cimg::PI/180);
        const float ang = (sq>0)?(float)std::atan2(v,u):0.0f;
        const flaot l = (length>=0)?length:-length*(float)std::sqrt(sq)/100;
        if (sq>0) {
            const double cl = std::cos(ang-deg);
            const double sl = std::sin(ang-deg);
            const double cr = std::cos(ang+deg);
            const double sr = std::sin(ang+deg);
            const int xl = x1+(int)(l*cl);
            const int yl = y1+(int)(l*sl);
            const int xr = x1+(int)(l*cr);
            const int yr = y1+(int)(l*sr);
            const int xc = x1+(int)((l+1)*(cl+cr))/2;
            const int yc = y1+(int)((l+1)*(sl+sr))/2;
            if (thickness>1) {
                const float m = 0.5f*thickness;
                const int xl0 = x0+(int)(m*cl);
                const int yl0 = y0+(int)(m*sl);
                const int xr0 = x0+(int)(m*cr);
                const int  yr0 = y0+(int)(m*sr);
                const int xl1 = x1+(int)(m*cl);
                const int yl1 = y1+(int)(m*sl);
                const int xr1 = x1+(int)(m*cr);
                const int yr1 = y1+(int)(m*sr);
                draw_triangle(xl0,yl0,xl1,yl1,xr1,yr1,color,opacity).
                    draw_triangle(xl0,yl0,xr1,yr1,xr0,yr0,color,opacity);
            }
            else
                draw_line(x0,y0,xc,yc,color,pattern,opacity);

            draw_triangle(x1,y1,xl,yl,xr,yr,color,opacity);
        }
        else
            draw_point(x0,y0,color,opacity);
    }
    return *this;
}
*/

template <typename T>
void ImgTool::drawArrowThick(cimg_library::CImg<T>& dst,
                             const int x0, const int y0,
                             const int x1, const int y1,
                             const T *const color,
                             const float thickness,
                             const float angle, const float length,
                             const unsigned int pattern, const float opacity) {
    assert(!dst.is_empty());

    const float u   = (float)(x0-x1);
    const float v   = (float)(y0-y1);
    const float sq  = u*u+v*v;
    const float deg = (float)(angle*PI/180);
    const float ang = (sq>0) ? (float)std::atan2(v,u) : 0.0f;
    const float l   = (length>=0) ? length : -length*(float)std::sqrt(sq)/100;

    if (sq>0) {
        const double cl = std::cos(ang-deg);
        const double sl = std::sin(ang-deg);
        const double cr = std::cos(ang+deg);
        const double sr = std::sin(ang+deg);
        const int xl = x1+(int)(l*cl);
        const int yl = y1+(int)(l*sl);
        const int xr = x1+(int)(l*cr);
        const int yr = y1+(int)(l*sr);
        const int xc = x1+(int)((l+1)*(cl+cr))/2;
        const int yc = y1+(int)((l+1)*(sl+sr))/2;

        if (thickness>1) {
            const float m = 0.5f*thickness;
            const int xl0 = x0+(int)(m*cl);
            const int yl0 = y0+(int)(m*sl);
            const int xr0 = x0+(int)(m*cr);
            const int yr0 = y0+(int)(m*sr);
            const int xl1 = x1+(int)(m*cl);
            const int yl1 = y1+(int)(m*sl);
            const int xr1 = x1+(int)(m*cr);
            const int yr1 = y1+(int)(m*sr);
            dst.draw_triangle(xl0,yl0,xl1,yl1,xr1,yr1,color,opacity);
            dst.draw_triangle(xl0,yl0,xr1,yr1,xr0,yr0,color,opacity);
        }
        else
            dst.draw_line(x0,y0,xc,yc,color,opacity,pattern);
        dst.draw_triangle(x1,y1,xl,yl,xr,yr,color,opacity);
    }
    // else
    //     draw_point(x0,y0,color,opacity);
}

template <typename T, typename T2>
void ImgTool::zeroCrossings(const cimg_library::CImg<T >& src,
                                  cimg_library::CImg<T2>& zeroCrossings) {
    zeroCrossings.assign(src.width(), src.height(), src.depth(), src.spectrum(), 0);
    Roi<int> roi(1, 1, src.height()-1, src.width()-1, 0, src.depth());

    cimg_forC(src, v)
        forRoiXYZ(roi, x, y, z)
            if (       (src(x-1, y, z, v) < 0 && src(x, y, z, v) > 0)
                    || (src(x+1, y, z, v) < 0 && src(x, y, z, v) > 0)
                    || (src(x, y-1, z, v) < 0 && src(x, y, z, v) > 0)
                    || (src(x, y+1, z, v) < 0 && src(x, y, z, v) > 0))
                zeroCrossings(x, y, z, v) = 1;
}

template <typename T>
void ImgTool::saveRawWithHeader(const cimg_library::CImg<T>& src,
                                std::string fileName, std::string header) {
    FILE* file = fopen(fileName.c_str(), "wb");
    fprintf(file, header.c_str());
    src.save_raw(file);
    fclose(file);
}

template<typename T>
void ImgTool::saveCsv(const cimg_library::CImg<T>& src,
                      std::string fileName, bool header,
                      char rowDelimiter, char colDelimiter) {
    std::ofstream f(fileName.c_str());
    if(f.fail())
        throw std::string("ImgTool::saveCsv() failed because of I/O error ")
                + "using filename '" + fileName + "'!";

    if (header)
        f   << src.width() << colDelimiter
            << src.height() << colDelimiter
            << src.depth() << colDelimiter
            << src.spectrum() << rowDelimiter;

    cimg_forYZC(src, y, z, v) {
        f   << src(0, y, z, v);
        for (int x = 1; x < src.width(); ++x)
            f << colDelimiter << src(x, y, z, v);
        f << rowDelimiter;
    }
}

template<typename T>
void ImgTool::loadCsv(cimg_library::CImg<T> &dst,
                      std::string fileName,
                      int width, int height, int depth, int spectrum,
                      char rowDelimiter, char colDelimiter,
                      int maxLineLength) {
    std::ifstream f(fileName.c_str());
    if(f.fail())
        throw "ImgTool::loadCsv() failed because file '"
                + fileName + "' was not found!";

    //maximum line length is 1024!
    char* line = new char[maxLineLength];

    if(width == -1)
    {
        //get first line which should contain width,height,depth,spectrum
        f.getline(line, maxLineLength, rowDelimiter);
        if(f.fail())
            throw std::string("ImgTool::loadCsv() failed because line length ")
                    + "exceeds 'maxLineLength' characters!";

        std::stringstream strm;
        std::string str(line);
        std::vector<int> dims;
        strm << str;
        while(!strm.eof()) {
            strm.getline(line, maxLineLength, colDelimiter);
            std::stringstream istrm;
            istrm << line;
            int dim;
            istrm >> dim;
            dims.push_back(dim);
        }

        if(dims.size() != 4)
            throw std::string("ImgTool::loadCsv() failed because first line")
                + "is not 'width,height,depth,spectrum'!";
        dst.assign(dims[0], dims[1], dims[2], dims[3], 0);
    }
    else
        dst.assign(width, height, depth, spectrum, 0);

    T value;
    cimg_forYZC(dst, y, z, v) {
        for(int x =0; x < dst.width()-1; ++x) {
            f.getline(line, maxLineLength, colDelimiter);
            if(f.eof())
                throw std::string("ImgTool::loadCsv() failed because ")
                    + "end of file is reached!";
            if(f.fail())
                throw std::string("ImgTool::loadCsv() failed because ")
                    + "line length exceeds 'maxLineLength' characters!";
            std::stringstream istrm;
            istrm << line;
            istrm >> value;
            dst(x, y, z, v) =value;
        }
        f.getline(line, maxLineLength, rowDelimiter);
        if(f.eof())
            throw std::string("ImgTool::loadCsv() failed because ")
                + "end of file is reached!";
        if(f.fail())
            throw std::string("ImgTool::loadCsv() failed because ")
                + "line length exceeds 'maxLineLength' characters!";
        std::stringstream istrm;
        istrm << line;
        istrm >> value;
        dst(dst.width()-1, y, z, v) =value;
    }

    delete line;
}

template<typename T>
void ImgTool::scatterPlot(const cimg_library::CImg<T>& data,
                          int width, int height, char normalizationType,
                          cimg_library::CImg<T>& dst,
                          float stdFactor) {
    //assume data has two values per row for x/y-coordinate in plot
    assert(data.width() == 2);
    assert(data.depth() == 1);
    assert(data.spectrum() == 1);
    cimg_library::CImg<T> dx = data.get_crop(0, 0, 0, data.height()-1);
    cimg_library::CImg<T> dy = data.get_crop(1, 0, 1, data.height()-1);

    if (normalizationType == 1) {
        //normalize to stdFactor*std
        dx.cut((T)(dx.mean()-stdFactor*sqrt(dx.variance())),
               (T)(dx.mean()+stdFactor*sqrt(dx.variance())));
        dy.cut((T)(dy.mean()-stdFactor*sqrt(dy.variance())),
               (T)(dy.mean()+stdFactor*sqrt(dy.variance())));
    }

    else if(normalizationType == 2) {
        //normalize to quantile range
        std::vector<T> xvals(dx.height());
        std::vector<T> yvals(dy.height());

        cimg_forY(dx, y) {
            xvals[y] = dx(0, y);
            yvals[y] = dy(0, y);
        }

        sort(xvals.begin(), xvals.end());
        sort(yvals.begin(), yvals.end());

        //take quantile-range denoted by stdFactor
        dx.cut((T)(xvals[(int)(stdFactor*dx.height())]),
               (T)(xvals[(int)((1-stdFactor)*dx.height()-1)]));

        //take quantile-range denoted by stdFactor
        dy.cut((T)(yvals[(int)(stdFactor*dy.height())]),
               (T)(yvals[(int)((1-stdFactor)*dy.height()-1)]));
    }

    //normalize to min/max
    dx -= dx.min();
    dy -= dy.min();
    dx.normalize(0, 1);
    dy.normalize(0, 1);

    dst.assign(width+1, height+1, 1, 3, 1);

    //draw ordered labels
    cimg_forY(data, y)
        if (data(2, y)) {
            //zero-Label
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 0) = 0;
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 1) = 0;
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 2) = 1;
        }

    cimg_forY(data, y)
        if (!data(2, y)) {
            //zero-Label
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 0) = 1;
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 1) = 0;
            dst((int)(dx(0, y)*width), (int)(dy(0, y)*height), 2) = 0;
        }
}

template<typename T>
void ImgTool::nonFiniteToConstant(cimg_library::CImg<T>& data, T value) {
    cimg_for(data, ptr, T)
        if(!finite(*ptr))
            *ptr = value;
}

template<typename T>
void ImgTool::blockVariance2d(const cimg_library::CImg<T>& src,
                                    cimg_library::CImg<T>& dst,
                              int halfBlockWidth,
                              T outsideValue) {
    Roi<int> roiImg(halfBlockWidth, halfBlockWidth,
                    src.height()-halfBlockWidth, src.width()-halfBlockWidth);
    Roi<int> roi(-halfBlockWidth, -halfBlockWidth,
                 halfBlockWidth+1, halfBlockWidth+1);
    cimg_library::CImg<T> tmp(2*halfBlockWidth+1, 2*halfBlockWidth+1);
    dst.assign(src.width(), src.height(), src.depth(), src.spectrum(), outsideValue);

    cimg_forZC (src, z, v) {
        forRoiXY (roiImg, x, y) {
            forRoiXY (roi, dx, dy)
                tmp(dx+halfBlockWidth, dy+halfBlockWidth) =
                    src(x+dx, y+dy, z, v);
            dst(x, y, z, v) = (T)tmp.variance();
        }
    }
}

template<typename T>
void ImgTool::blockEntropy2d(const cimg_library::CImg<T>& src,
                                   cimg_library::CImg<T>& dst,
                             int halfBlockWidth, T outsideValue) {
    Roi<int> roiImg(halfBlockWidth, halfBlockWidth,
                    src.height()-halfBlockWidth, src.width()-halfBlockWidth);
    Roi<int> roi(-halfBlockWidth, -halfBlockWidth,
                 halfBlockWidth+1, halfBlockWidth+1);
    cimg_library::CImg<T> tmp(2*halfBlockWidth+1, 2*halfBlockWidth+1);
    cimg_library::CImg<T> hist;

    dst.assign(src.width(), src.height(), src.depth(), src.spectrum(), outsideValue);
    cimg_forZC(src, z, v) {
        forRoiXY(roiImg, x, y) {
            forRoiXY(roi, dx, dy)
                tmp(dx+halfBlockWidth, dy+halfBlockWidth) =
                    src(x+dx, y+dy, z, v);
            hist = tmp.get_histogram(256);
            hist /= hist.sum();
            dst(x, y, z, v) = 0;
            cimg_forX(hist, h)
                if(hist(h))
                    dst(x, y, z, v) += hist(h)*log(hist(h))/log((T)2);
            dst(x, y, z, v) *= -1;
        }
    }
}

template<typename T>
void ImgTool::barronFleetError(const cimg_library::CImg<T>& flow,
                               const cimg_library::CImg<T>& gtFlow,
                                     cimg_library::CImg<T>& result) {
    assert(flow.is_sameXYZ(gtFlow));
    assert(flow.spectrum() == 2);
    assert(gtFlow.spectrum() == 2);

    cimg_library::CImg<T> xFlw     = flow.get_channel(0);
    cimg_library::CImg<T> yFlw     = flow.get_channel(1);
    cimg_library::CImg<T> xRealFlw = gtFlow.get_channel(0);
    cimg_library::CImg<T> yRealFlw = gtFlow.get_channel(1);
    cimg_library::CImg<T> ones(xFlw.width(), xFlw.height(), 1, 1, 1);

    // barron/fleet angular error
    cimg_library::CImg<T> angAmpFlw =
            (xFlw.get_pow(2) + yFlw.get_pow(2) + ones).sqrt();
    cimg_library::CImg<T> angAmpRealFlw =
            (xRealFlw.get_pow(2) + yRealFlw.get_pow(2) + ones).sqrt();

    result = (xFlw.get_mul(xRealFlw) + yFlw.get_mul(yRealFlw) + ones).div(
            angAmpFlw.get_mul(angAmpRealFlw));
    result.cut(-1, 1);

    cimg_for(result, ptr, T) {
        *ptr = (T)(acos(*ptr)/(2*PI)*360);
        if (!finite(*ptr))
            *ptr =0;
    }
}

template<typename T>
void ImgTool::amplitudeError(const cimg_library::CImg<T>& flow,
                             const cimg_library::CImg<T>& gtFlow,
                                   cimg_library::CImg<T>& result,
                                   bool relative) {
    assert(flow.is_sameXYZ(gtFlow));
    assert(flow.spectrum() == 2);
    assert(gtFlow.spectrum() == 2);

    cimg_library::CImg<T> xFlw     = flow.get_channel(0);
    cimg_library::CImg<T> yFlw     = flow.get_channel(1);
    cimg_library::CImg<T> xRealFlw = gtFlow.get_channel(0);
    cimg_library::CImg<T> yRealFlw = gtFlow.get_channel(1);

    //amplitude error
    cimg_library::CImg<T> ampFlw =
            (xFlw.get_pow(2)+yFlw.get_pow(2)).sqrt();
    cimg_library::CImg<T> ampRealFlw =
            (xRealFlw.get_pow(2)+yRealFlw.get_pow(2)).sqrt();

    if (relative)
        result = (ampRealFlw-ampFlw).abs().div(ampRealFlw);
    else
        result = (ampFlw-ampRealFlw).abs();
}

template<typename T>
void ImgTool::angleError(const cimg_library::CImg<T>& flow,
                         const cimg_library::CImg<T>& gtFlow,
                               cimg_library::CImg<T> &result) {
    assert(flow.is_sameXYZ(gtFlow));
    assert(flow.spectrum() == 2);
    assert(gtFlow.spectrum() == 2);

    cimg_library::CImg<T> xFlw       = flow.get_channel(0);
    cimg_library::CImg<T> yFlw       = flow.get_channel(1);
    cimg_library::CImg<T> xRealFlw   = gtFlow.get_channel(0);
    cimg_library::CImg<T> yRealFlw   = gtFlow.get_channel(1);
    cimg_library::CImg<T> ampFlw     = (xFlw.get_pow(2)
                                            + yFlw.get_pow(2)).sqrt();
    cimg_library::CImg<T> ampRealFlw = (xRealFlw.get_pow(2)
                                            + yRealFlw.get_pow(2)).sqrt();

    //real angular error
    result = (xFlw.get_mul(xRealFlw)+yFlw.get_mul(yRealFlw))
             .div(ampFlw.get_mul(ampRealFlw));
    result.cut(-1, 1);

    cimg_for (result, ptr, T) {
        *ptr =(T)(acos(*ptr)/(2*PI)*360);
        if(!finite(*ptr))
            *ptr =0;
    }
}

template<typename T>
void ImgTool::endPointError(const cimg_library::CImg<T>& flow,
                            const cimg_library::CImg<T>& gtFlow,
                                  cimg_library::CImg<T>& result) {
    assert(flow.is_sameXYZ(gtFlow));
    assert(flow.spectrum() == 2);
    assert(gtFlow.spectrum() == 2);

    result = flow-gtFlow;

    //two-norm of difference vector
    result.pointwise_norm(2);
}

template<typename T>
T ImgTool::barronFleetError(const T& fx,   const T& fy,
                            const T& gtFx, const T& gtFy) {
    // Barron/Fleet angular error
    T angAmpFlw     = sqrt(fx*fx+fy*fy+1);
    T angAmpRealFlw = sqrt(gtFx*gtFx+gtFy*gtFy+1);

    T result        = (fx*gtFx+fy*gtFy+1)/(angAmpFlw*angAmpRealFlw);
    if(result < -1)
        result = -1;
    else if(result > 1)
        result =  1;
    result = (T)(acos(result)/(2*PI)*360);
    if(!finite(result))
        result = 0;
    return result;
}

template<typename T>
T ImgTool::amplitudeError(const T& fx,   const T& fy,
                          const T& gtFx, const T& gtFy,
                          bool relative) {
    //amplitude error
    T ampFlw     = sqrt(fx*fx+fy*fy);
    T ampRealFlw = sqrt(gtFx*gtFx+gtFy*gtFy);
    if(relative)
        return abs((ampRealFlw-ampFlw))/(ampRealFlw);
    else
        return abs(ampFlw-ampRealFlw);
}

template<typename T>
T ImgTool::angleError(const T& fx,   const T& fy,
                      const T& gtFx, const T& gtFy) {
    //real angular error
    T ampFlw     = sqrt(fx*fx+fy*fy);
    T ampRealFlw = sqrt(gtFx*gtFx+gtFy*gtFy);

    T result     = (fx*gtFx+fy*gtFy)/(ampFlw*ampRealFlw);
    if(result < -1)
        result = -1;
    else if(result > 1)
        result =  1;
    result = (T)(acos(result)/(2*PI)*360);
    if(!finite(result))
        result =0;
    return result;
}

template<typename T>
T ImgTool::endPointError(const T& fx,   const T& fy,
                         const T& gtFx, const T& gtFy) {
    return sqrt((fx-gtFx)*(fx-gtFx)+(fy-gtFy)*(fy-gtFy));
}

template<typename T>
void ImgTool::maskedMeanStd(const cimg_library::CImg<T>& src,
                            const cimg_library::CImg<unsigned char>& mask,
                            T& mean, T& std) {
    assert(src.is_sameXYZ(mask));
    mean =0;
    std =0;
    unsigned int count =0;

    //same code as in "else" only mask(x, y, z) != mask(x, y, z, v)
    if (mask.spectrum() == 1 && src.spectrum() > 1) {
        cimg_forXYZC(src, x, y, z, v) {
            if(mask(x, y, z)) {
                mean += src(x, y, z, v);
                ++count;
            }
        }
        if (count)
            mean /= (T)count;

        cimg_forXYZC(src, x, y, z, v) {
            if (mask(x, y, z))
                std += (src(x, y, z, v)-mean)*(src(x, y, z, v)-mean);
        }
        if (count)
            std = sqrt(std/(T)count);
    }
    else {
        assert (mask.is_sameC(src));
        cimg_forXYZC(src, x, y, z, v) {
            if (mask(x, y, z, v)) {
                mean += src(x, y, z, v);
                ++count;
            }
        }
        if (count)
            mean /= (T)count;

        cimg_forXYZC(src, x, y, z, v)
            if(mask(x, y, z, v))
                std += (src(x, y, z, v)-mean)*(src(x, y, z, v)-mean);
        if(count)
            std = sqrt(std/(T)count);
    }
}

template<typename T>
void ImgTool::nThFrame(const cimg_library::CImg<T> &src,
                             cimg_library::CImg<T> &dst,
                             int step, int start) {
    int depth = (int)ceil((src.depth()-start)/(float)step);
    dst.assign(src.width(), src.height(), depth, src.spectrum());

    for(int z = 0; z < depth; ++z)
        cimg_forXYC(src, x, y, v)
        dst(x, y, z, v) = src(x, y, start+z*step, v);
}

template<typename T>
void ImgTool::minImageZ(const cimg_library::CImg<T> &src,
                              cimg_library::CImg<T> &dst) {
    assert(src.spectrum() == 1);
    dst = src.get_slice(0);
    cimg_forXYZ(src, x, y, z)
        if(dst(x, y) > src(x, y, z))
            dst(x, y) = src(x, y, z);
}

template<typename T>
void ImgTool::maxImageZ(const cimg_library::CImg<T> &src,
                              cimg_library::CImg<T> &dst) {
    assert(src.spectrum() == 1);
    dst = src.get_slice(0);
    cimg_forXYZ(src, x, y, z)
        if(dst(x, y) < src(x, y, z))
            dst(x, y) = src(x, y, z);
}

template<typename T, typename T2>
void ImgTool::crop2d(cimg_library::CImg<T> &src, Roi<T2> roi) {
    src.crop(roi.left, roi.top,
             roi.right-1, roi.bottom-1);
}

template<typename T, typename T2>
void ImgTool::crop3d(cimg_library::CImg<T> &src, Roi<T2> roi) {
    src.crop(roi.left, roi.top,
             roi.front, roi.right-1,
             roi.bottom-1, roi.back-1);
}

template <typename T>
std::string ImgTool::sizeString(const cimg_library::CImg<T>& img) {
	std::ostringstream res;
	res << "(" << img.width() << "," << img.height()
		<< "," << img.depth() << "," << img.spectrum()
		<< ") [";
	float s = float(img.size()*sizeof(T));
	if (s < 1024)
		res << s << " B";
	if ((s >= 1024) && ((s /= 1024) < 1024))
		res << s << " KiB";
	if ((s >= 1024) && ((s /= 1024) < 1024))
		res << s << " MiB";
	if ((s >= 1024) && ((s /= 1024) < 1024))
		res << s << " GiB";
	res << "]";
	return res.str();
}

template <typename T>
std::string ImgTool::dataString(const cimg_library::CImg<T>& img) {
	std::ostringstream res;
	res << "(" << cimg_library::cimg::type<T>::string() << "*)";
	res << (void*)img._data;
	res << " (" << (img._is_shared ? "shared" : "not shared") << ")";
	return res.str();
}

template <typename T>
std::string ImgTool::contentString(
		const cimg_library::CImg<T>& img,
		unsigned int mE)
{
	if(img.is_empty())
		return "[ ]";

	std::ostringstream res;
	res << "[ ";
	cimg_for_inY(img,0,mE,y) {
		cimg_for_inX(img,0,mE,x) {
			res << cimg_library::cimg::type<T>::format(img(x,y,0,0)) << " ";
		}
		if (img.width() > int(mE))
			res << "... ";
		res << ";\n  ";
	}
	if (img.height() > int(mE))
		res << "... ";
	res << "]";
	if ((img.depth() > 1) || (img.depth() > 1))
		res << "\n ... and more slices ...";
	return res.str();
}

template <typename T>
void ImgTool::printInfo(std::ostream& os, const cimg_library::CImg<T>& img,
		const std::string& prefix) {
	os << prefix << "pointer = " << (void*)&img << "," << std::endl;
	os << prefix << "size    = " << ImgTool::sizeString(img) << "," << std::endl;
	os << prefix << "data    = " << ImgTool::dataString(img) << std::endl;
	std::istringstream cont(ImgTool::contentString(img));
	std::string cur;
	std::getline(cont, cur);
	assert(cont);
	os << prefix << "content = " << cur << std::endl;
	while (std::getline(cont, cur))
		os << prefix << "          " << cur << std::endl;
}
#endif // _ImgTool_HXX
