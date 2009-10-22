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
/** @file   PdeBoundaryHandler.hxx
 *  @brief  Implementations for class PdeBoundaryHandler
 *  @author <a href="mailto:daniel.kondermann@iwr.uni-heidelberg.de">
 *      Daniel Kondermann</a>
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Interface is in PdeBoundaryHandler.h.
 *  Include PdeBoundaryHandler.h, if you only need the class and
 *  function declarations (prototypes).
 */

#ifndef _PdeBoundaryHandler_HXX
#define _PdeBoundaryHandler_HXX

#include "PdeBoundaryHandler.h"
#include <cassert>
#include <cfloat>
#include <cmath>

template <typename T>
PdeBoundaryHandler<T>::Position::Position(int xn, int yn, int zn, int tn) :
        x(xn), y(yn), z(zn), t(tn) {
}

template <typename T>
const typename PdeBoundaryHandler<T>::Position&
    PdeBoundaryHandler<T>::Position::operator= (
        const typename PdeBoundaryHandler<T>::Position& B) {
    x = B.x;
    y = B.y;
    z = B.z;
    t = B.t;
}

template <typename T>
const T* PdeBoundaryHandler<T>::getC(int x, int y, int z, int t,
                                   const cimg_library::CImg<T*>* data) const {
    return (const_cast<PdeBoundaryHandler*>(this)->get(x, y, z, t,data));
}

template <typename T>
PdeBoundaryHandler<T>::PdeBoundaryHandler() :
        delta(0),
        unknownsSourceOverride(false) {
}

template <typename T>
const Roi<int>& PdeBoundaryHandler<T>::getRoi() const {
    return roi;
}

template <typename T>
const Roi<int>& PdeBoundaryHandler<T>::getExtendedRoi() const {
    return extendedRoi;
}

template <typename T>
int PdeBoundaryHandler<T>::getStencilWidthX() const {
    return sWidthX;
}

template <typename T>
int PdeBoundaryHandler<T>::getStencilWidthY() const {
    return sWidthY;
}

template <typename T>
int PdeBoundaryHandler<T>::getStencilWidthZ() const {
    return sWidthZ;
}

template <typename T>
int PdeBoundaryHandler<T>::getStencilWidthT() const {
    return sWidthT;
}

template <typename T>
void PdeBoundaryHandler<T>::setUnknownsSource(bool useOriginalUnknowns) {
    if (!unknownsSourceOverride) {
        if (!useOriginalUnknowns && delta)
            unknowns = &pyramidUnknowns;
        else
            unknowns = &originalUnknowns;
    }
    else
        unknowns = &pyramidUnknowns;
}

template <typename T>
void PdeBoundaryHandler<T>::setUnknownsSourceOverride(
        bool unknSrcOverrd) {
    unknownsSourceOverride = unknSrcOverrd;
}

template <typename T>
int PdeBoundaryHandler<T>::getDimension() const {
    if (is1d)
        return 1;
    else if (is2d)
        return 2;
    else if (is3d)
        return 3;
    else
        return 4;
}

template <typename T>
void PdeBoundaryHandler<T>::setCurrentDelta(cimg_library::CImgList<T>* d) {
    assert(d->size == (unsigned int)nDof);
    delta = d;
}

template <typename T>
T* PdeBoundaryHandler<T>::at(T*** unknowns, int x) const {
    assert(!dimY);
    return &((T*)unknowns)[nDof*x];
}

template <typename T>
T* PdeBoundaryHandler<T>::at(T*** unknowns, int x, int y) const {
    assert(!dimZ);
    return &((T**)unknowns)[y][nDof*x];
}

template <typename T>
T* PdeBoundaryHandler<T>::at(T*** unknowns, int x, int y, int z) const {
    assert(!dimT);
    return &((T***)unknowns)[z][y][nDof*x];
}

template <typename T>
T* PdeBoundaryHandler<T>::at(T*** unknowns, int x, int y, int z, int t) const {
    assert(dimT);
    return &((T***)unknowns)[z][y][nDof*dimT*x+dimT*t];
}

template <typename T>
const T* PdeBoundaryHandler<T>::getU(int x, int y, int z, int t) const {
    return getC(x, y, z, t, unknowns);
}

template <typename T>
const T* PdeBoundaryHandler<T>::getR(int x, int y, int z, int t) const {
    return getC(x, y, z, t, &residuals);
}

template <typename T>
void PdeBoundaryHandler<T>::setU(int x, int y, int z, int t, int p, T v) {
    get(x, y, z, t, unknowns)[p] = v;
}

template <typename T>
void PdeBoundaryHandler<T>::setR(int x, int y, int z, int t, int p, T v) {
    get(x, y, z, t, &residuals)[p] = v;
}

template <typename T>
T* PdeBoundaryHandler<T>::get(int x, int y, int z, int t,
                              const cimg_library::CImg<T*>* data) {
    x += sWidthX;
    y += sWidthY;
    z += sWidthZ,
    t += sWidthT;
    int px = getValuePositions[0](x, y, z, t);
    int py = getValuePositions[1](x, y, z, t);
    int pz = getValuePositions[2](x, y, z, t);
    int pt = getValuePositions[3](x, y, z, t);
    assert(data->atXYZV(px, py, pz, pt));
    return data->atXYZV(px, py, pz, pt);
}

template <typename T>
bool PdeBoundaryHandler<T>::isBoundary(int x, int y, int z, int t) const {
    x += sWidthX;
    y += sWidthY;
    z += sWidthZ;
    t += sWidthT;
    return boundaryMask(getValuePositions[0](x, y, z, t),
                        getValuePositions[1](x, y, z, t),
                        getValuePositions[2](x, y, z, t),
                        getValuePositions[3](x, y, z, t));
}

template <typename T>
PdeBoundaryHandler<T>::~PdeBoundaryHandler(void) {
    clearUnknowns();
}

template <typename T>
void PdeBoundaryHandler<T>::clearUnknowns() {
    if (!createInternalPointers) {
        // in originalUnknowns we only need to clear boundary points,
        // because the rest is handled by petsc
        resetBoundaryPositionIterator();
        Position pos;
        while (getNextBoundaryPosition(pos)) {
            if (originalUnknowns(pos.x, pos.y, pos.z, pos.t)) {
                //there might be several occurances
                delete originalUnknowns(pos.x, pos.y, pos.z, pos.t);
                originalUnknowns(pos.x, pos.y, pos.z, pos.t) = 0;
            }
        }
        boundaryPositions.clear();
        originalUnknowns.assign();
    }
    else {
        //clear everything because it was created in this class
        cimg_forXYZV(originalUnknowns, x, y, z, v)
            delete originalUnknowns(x, y, z, v);
        originalUnknowns.assign();
    }

    //in pyramidUnknowns we need to clear all points
    cimg_forXYZV(pyramidUnknowns, x, y, z, v)
        delete pyramidUnknowns(x, y, z, v);

    //sout << "Pyramid delete removed!" << std::endl;
    pyramidUnknowns.assign();

    //in residuals we need to clear nothing (handled by solver)
    residuals.assign();
}

template <typename T>
void PdeBoundaryHandler<T>::init(
        int dX, int dY, int dZ, int dT, int nDf,
        int sWX, int sWY, int sWZ, int sWT,
        bool createIntPointers) {
    //rest override if it was set
    setUnknownsSourceOverride(false);
    //if there was a previous call to this function, reset all unknowns
    clearUnknowns();

    createInternalPointers = createIntPointers;
    nDof = nDf;
    dimX = dX;
    dimY = dY;
    dimZ = dZ;
    dimT = dT;

    if(createIntPointers) {
        // no roi used if own pointers (non-PETSC) are used
        roi.xBegin = 0;
        roi.yBegin = 0;
        roi.zBegin = 0;
        roi.xEnd = dX;
        roi.yEnd = dY;
        roi.zEnd = dZ;

        extendedRoi = roi;
    }

    is1d = false;
    is2d = false;
    is3d = false;

    if(!dY)
        is1d = true;
    else if(!dZ)
        is2d = true;
    else if(!dT)
        is3d = true;

    if (is1d) {
        sWidthX = sWX;
        sWidthY = 0;
        sWidthZ = 0;
        sWidthT = 0;
    }
    else if (is2d) {
        sWidthX = sWX;
        sWidthY = sWY;
        sWidthZ = 0;
        sWidthT = 0;
    }
    else if(is3d) {
        sWidthX = sWX;
        sWidthY = sWY;
        sWidthZ = sWZ;
        sWidthT = 0;
    }
    else {
        sWidthX = sWX;
        sWidthY = sWY;
        sWidthZ = sWZ;
        sWidthT = sWT;
    }

    cimg_library::CImg<bool> stencil, mask;

    //add stencilWidth pixel padding to the mask with value false
    if (is1d) {
        mask.assign(dX+2*sWX, 1, 1, 1, true);

        for (int i=0, j=mask.dimx()-1; i < sWX; ++i, j--)
            mask(i) = mask(j) = false;

        stencil.assign(2*sWX+1);
        stencil(stencil.dimx()/2+1) = false;

        for(int i=0, j=stencil.dimx()-1; i <= sWX; ++i, j--)
            stencil(i) = stencil(j) = true;
    }
    else if (is2d) {
        mask.assign(dX+2*sWX, dY+2*sWY, 1, 1, true);

        // left and right padding
        for(int i =0, j =mask.dimx()-1; i < sWX; ++i, j--)
            cimg_forY(mask, y)
                mask(i, y) = mask(j, y) = false;

        //upper and lower padding
        for(int i =0, j =mask.dimy()-1; i < sWY; ++i, j--)
            cimg_forX(mask, x)
                mask(x, i) = mask(x, j) = false;

        stencil.assign(2*sWX+1, 2*sWY+1, 1, 1, true);
        stencil(sWX, sWY) = false;
    }
    else if(is3d) {
        mask.assign(dX+2*sWX, dY+2*sWY, dZ+2*sWZ, 1, true);

        //left and right padding
        for(int i =0, j =mask.dimx()-1; i < sWX; ++i, j--)
            cimg_forYZ(mask, y, z)
                mask(i, y, z) = mask(j, y, z) = false;

        //upper and lower padding
        for(int i =0, j =mask.dimy()-1; i < sWY; ++i, j--)
            cimg_forXZ(mask, x, z)
                mask(x, i, z) = mask(x, j, z) = false;

        //front and back padding
        for(int i =0, j =mask.dimz()-1; i < sWZ; ++i, j--)
            cimg_forXY(mask, x, y)
                mask(x, y, i) = mask(x, y, j) = false;

        stencil.assign(2*sWX+1, 2*sWY+1, 2*sWZ+1, 1, true);
        stencil(sWX, sWY, sWZ) = false;
    }
    else {
        mask.assign(dX+2*sWX, dY+2*sWY, dZ+2*sWZ, dT+2*sWT, true);

        //left and right padding
        for(int i =0, j =mask.dimx()-1; i < sWX; ++i, j--)
            cimg_forYZV(mask, y, z, v)
                mask(i, y, z, v) = mask(j, y, z, v) = false;

        //upper and lower padding
        for(int i =0, j =mask.dimy()-1; i < sWY; ++i, j--)
            cimg_forXZV(mask, x, z, v)
                mask(x, i, z, v) = mask(x, j, z, v) = false;

        //front and back padding
        for(int i =0, j =mask.dimz()-1; i < sWZ; ++i, j--)
            cimg_forXYV(mask, x, y, v)
                mask(x, y, i, v) = mask(x, y, j, v) = false;

        //start and end padding
        for(int i =0, j =mask.dimv()-1; i < sWT; ++i, j--)
            cimg_forXYZ(mask, x, y, z)
                mask(x, y, z, i) = mask(x, y, z, j) = false;

        stencil.assign(2*sWX+1, 2*sWY+1, 2*sWZ+1, 2*sWT+1, true);
        stencil(sWX, sWY, sWZ, sWT) =false;
    }

    //get the mask containing the boundary points

    //init empty boundary image
    boundaryMask.assign(    mask.dimx(), mask.dimy(),
                            mask.dimz(), mask.dimv(), false);
    originalUnknowns.assign(mask.dimx(), mask.dimy(),
                            mask.dimz(), mask.dimv());
    pyramidUnknowns.assign( mask.dimx(), mask.dimy(),
                            mask.dimz(), mask.dimv());
    residuals.assign(       mask.dimx(), mask.dimy(),
                            mask.dimz(), mask.dimv());
    // init unknowns array with null pointers (if done in constructor,
    // this causes Img-typecasting-compiler-error due to type==pointer!)
    cimg_for(originalUnknowns, ptr, T*)
        *ptr =0;
    // create all points for pyramidUnknowns
    cimg_for(pyramidUnknowns, ptr, T*)
        *ptr =new T[nDf];
    cimg_for(residuals, ptr, T*)
        *ptr =0;

    // default unknowns source is original unknowns
    unknowns = &originalUnknowns;

    cimg_forXYZV(mask, x, y, z, t) {
        //if inside domain, add boundary to originalUnknowns if neccessary
        if (mask(x, y, z, t))
            cimg_forXYZV(stencil, sx, sy, sz, st) {
                int ox, oy, oz, ot;
                if (is1d) {
                    ox = x+sx-sWX;
                    oy = oz = ot = 0;
                }
                else if (is2d) {
                    ox = x+sx-sWX;
                    oy = y+sy-sWY;
                    oz = ot =0;
                }
                else if (is3d) {
                    ox = x+sx-sWX;
                    oy = y+sy-sWY;
                    oz = z+sz-sWZ;
                    ot =0;
                }
                else {
                    ox = x+sx-sWX;
                    oy = y+sy-sWY;
                    oz = z+sz-sWZ;
                    ot = t+st-sWT;
                }

                // if stencil is set, but position is not inside the domain
                // here we are always inside mask because all padding is set
                // to false
                if(stencil(sx, sy, sz, st)
                    && !mask(ox, oy, oz, ot)
                    && !originalUnknowns(ox, oy, oz, ot)) {
                    //create a boundary point
                    boundaryMask(ox, oy, oz, ot) = true;
                    originalUnknowns(ox, oy, oz, ot) = new T[nDf];
                    boundaryPositions.push_back(Position(ox, oy, oz, ot));
                }
            }
    }

    getValuePositions.assign(4, mask.dimx()+sWX, mask.dimy()+sWY,
                             mask.dimz()+sWZ, mask.dimv()+sWT, 0);

    // calculate array positions from real unknowns positions
    // stored in x, y, z
    if (is1d)
        cimg_forXYZV(mask, x, y, z, v) {
            getValuePositions[0](x, y, z, v) =x;
            getValuePositions[1](x, y, z, v) =0;
            getValuePositions[2](x, y, z, v) =0;
            getValuePositions[3](x, y, z, v) =0;
        }
    else if (is2d)
        cimg_forXYZV(mask, x, y, z, v) {
            getValuePositions[0](x, y, z, v) =x;
            getValuePositions[1](x, y, z, v) =y;
            getValuePositions[2](x, y, z, v) =0;
            getValuePositions[3](x, y, z, v) =0;
        }
    else if (is3d)
        cimg_forXYZV(mask, x, y, z, v) {
            getValuePositions[0](x, y, z, v) =x;
            getValuePositions[1](x, y, z, v) =y;
            getValuePositions[2](x, y, z, v) =z;
            getValuePositions[3](x, y, z, v) =0;
        }
    else
        cimg_forXYZV(mask, x, y, z, v) {
            getValuePositions[0](x, y, z, v) =x;
            getValuePositions[1](x, y, z, v) =y;
            getValuePositions[2](x, y, z, v) =z;
            getValuePositions[3](x, y, z, v) =v;
        }

    if (createIntPointers)
        cimg_forXYZV(mask, x, y, z, t)
            if(mask(x, y, z, t)) {
                // if inside domain, add boundary to originalUnknowns
                // if neccessary
                assert(!originalUnknowns(x, y, z, t));
                originalUnknowns(x, y, z, t) = new T[nDf];
            }

    resetBoundaryPositionIterator();
}

template <typename T>
void PdeBoundaryHandler<T>::updateDelta() {
    if (delta) {
        if (extendedRoi.xBegin <= 0)
            extendedRoi.xBegin = 0;
        if (extendedRoi.xEnd >= dimX)
            extendedRoi.xEnd = dimX;
        if (extendedRoi.yBegin <= 0)
            extendedRoi.yBegin = 0;
        if (extendedRoi.yEnd >= dimY)
            extendedRoi.yEnd = dimY ? dimY : 1;
        if (extendedRoi.zBegin <= 0)
            extendedRoi.zBegin = 0;
        if (extendedRoi.zEnd >= dimZ)
            extendedRoi.zEnd = dimZ ? dimZ : 1;

        cimglist_for(*delta, d) {
            assert((*delta)[d].dimx() == dimX);
            assert((*delta)[d].dimy() == dimY ? dimY : 1);
            assert((*delta)[d].dimz() == dimZ ? dimZ : 1);
            assert((*delta)[d].dimv() == dimT ? dimT : 1);
            forRoiXYZ(extendedRoi, x, y, z)
                for(int t =0; t < (dimT?dimT:1); ++t)
                    get(x, y, z, t, &pyramidUnknowns)[d] =
                            get(x, y, z, t, &originalUnknowns)[d]
                            + (*delta)[d](x, y, z, t);
        }
    }
}

template <typename T>
void PdeBoundaryHandler<T>::updateUnknowns(T*** unknowns,
                                           T*** residuals,
                                           Roi<int>& roi) {
    this->roi =roi;
    this->extendedRoi = Roi<int>(
		roi.xBegin()-sWidthX, roi.xEnd()+sWidthX,
		roi.yBegin()-sWidthY, roi.yEnd()+sWidthY,
		roi.zBegin()-sWidthZ, roi.zEnd()+sWidthZ);

    if (extendedRoi.xBegin() <= 0)
        extendedRoi.xBegin() = 0;
    if (extendedRoi.xEnd() >= dimX)
        extendedRoi.xEnd() = dimX;
    if (extendedRoi.yBegin() <= 0)
        extendedRoi.yBegin() = 0;
    if (extendedRoi.yEnd() >= dimY)
        extendedRoi.yEnd() = dimY ? dimY : 1;
    if (extendedRoi.zBegin() <= 0)
        extendedRoi.zBegin() = 0;
    if (extendedRoi.zEnd() >= dimZ)
        extendedRoi.zEnd() = dimZ ? dimZ : 1;

    cimg_library::CImg<float> test(boundaryMask.dimx(), boundaryMask.dimy(),
                                   boundaryMask.dimz(), 2, 0);
    cimg_forXYZV(boundaryMask, x, y, z, t) {
        // if inside domain, we might need to update the residuals pointers
        if(!boundaryMask(x, y, z, t)) {
            // (iteration over the mask is slow if many processors are used!)

            // The mask is padded with the stencilWidth.
            // Thus it is necessary, to calculate the
            // position in the original unknowns array.
            int dx, dy, dz, dt;
            if (is1d) {
                dx = x-sWidthX;
                dy = dz = dt = 0;
            }
            else if (is2d) {
                dx = x-sWidthX;
                dy = y-sWidthY;
                dz = dt = 0;
            }
            else if (is3d) {
                dx = x-sWidthX;
                dy = y-sWidthY;
                dz = z-sWidthZ;
                dt = 0;
            }
            else {
                dx = x-sWidthX;
                dy = y-sWidthY;
                dz = z-sWidthZ;
                dt =t-sWidthT;
            }

            // ghost point "detection"
            // idea: extend the roi so that all petsc-ghost-points lie inside
            // the roi but not the ghost-points at the boundary of the domain

            /*
            // ghost point "detection": check if one of the eight corners of
            // a sWidth*-sized cube is inside domain always assume block
            // stencils here. if stencil is star-shaped some rubbish will be
            // inserted into this->unknowns (which will not be used, anyway)
            // remember: depending on dimension, sWidth* is set to zero, so
            // we can handle all dimensions at once (the 4th dimension can
            // well be ignored because Petsc-ROIs are 3d at max
            bool ghost =false;
            if(        roi.isInside(dx+sWidthX, dy+sWidthY, dz+sWidthZ)
                    || roi.isInside(dx-sWidthX, dy+sWidthY, dz+sWidthZ)
                    || roi.isInside(dx+sWidthX, dy-sWidthY, dz+sWidthZ)
                    || roi.isInside(dx-sWidthX, dy-sWidthY, dz+sWidthZ)
                    || roi.isInside(dx+sWidthX, dy+sWidthY, dz-sWidthZ)
                    || roi.isInside(dx-sWidthX, dy+sWidthY, dz-sWidthZ)
                    || roi.isInside(dx+sWidthX, dy-sWidthY, dz-sWidthZ)
                    || roi.isInside(dx-sWidthX, dy-sWidthY, dz-sWidthZ))
                ghost =true;
            */

            //the roi has to be  saved in unknowns coordinates
            if (extendedRoi.isInside(dx, dy, dz)) {
                //store unknowns vector from solver (copies pointers)
                if (is1d)
                    originalUnknowns(x,y,z,t) = at(unknowns,dx);
                else if (is2d)
                    originalUnknowns(x,y,z,t) = at(unknowns,dx,dy);
                else if (is3d)
                    originalUnknowns(x,y,z,t) = at(unknowns,dx,dy,dz);
                else
                    originalUnknowns(x,y,z,t) = at(unknowns,dx,dy,dz,dt);
            }
            //the roi has to be saved in residuals coordinates
            if (roi.isInside(dx, dy, dz)) {
                if (is1d)
                    this->residuals(x,y,z,t) = at(residuals,dx);
                else if (is2d)
                    this->residuals(x,y,z,t) = at(residuals,dx,dy);
                else if (is3d)
                    this->residuals(x,y,z,t) = at(residuals,dx,dy,dz);
                else
                    this->residuals(x,y,z,t) = at(residuals,dx,dy,dz,dt);
            }
        }
    }
}


template <typename T>
bool PdeBoundaryHandler<T>::getNextBoundaryPosition(Position& position) {
    if (iteratorPos >= boundaryPositions.size())
        return false;
    else {
        position = boundaryPositions[iteratorPos++];
        return true;
    }
}

template <typename T>
void PdeBoundaryHandler<T>::resetBoundaryPositionIterator() {
    iteratorPos = 0;
}

template <typename T>
void PdeBoundaryHandler<T>::updateBoundaries() {
    // use delta-data which is set at the start of a pyramid level
    updateDelta();
    // go through the whole boundary region
    for(unsigned int i=0; i < boundaryPositions.size(); ++i) {
        Position& pos = boundaryPositions[i];
        // get true coordinates of this position
        // t stands for "true" meaning the actual location
        // inside the problem domain
        int tx, ty, tz, tt;
        if (is1d) {
            tx = pos.x-sWidthX;
            ty = tz = tt = 0;
        }
        else if (is2d) {
            tx = pos.x-sWidthX;
            ty = pos.y-sWidthY;
            tz = tt = 0;
        }
        else if (is3d) {
            tx = pos.x-sWidthX;
            ty = pos.y-sWidthY;
            tz = pos.z-sWidthZ;
            tt = 0;
        }
        else {
            tx = pos.x-sWidthX;
            ty = pos.y-sWidthY;
            tz = pos.z-sWidthZ;
            tt = pos.t-sWidthT;
        }

        // If the roi was set by MPI so that a given boundary is not touched,
        // we should ignore these values.
        // Idea: extend current roi by its stencil widths and check whether
        // the boundary point is inside this region
        Roi<int> extended(
            roi.xBegin()-sWidthX, roi.xEnd()+sWidthX,
			roi.yBegin()-sWidthY, roi.yEnd()+sWidthY,
            roi.zBegin()-sWidthZ, roi.zEnd()+sWidthZ);

        if (extended.isInside(tx, ty, tz)) {
            // the source position for von-Neumann boundary conditions is the
            // nearest node inside the domain hence, the source is at the
            // original positions with negative coordinates set to zero
            // (in mask coordinates) and those with values larger than the
            // dimensions to the maximum value
            Position src;

            if (tx < 0)
                src.x = sWidthX;
            else if (tx > roi.xEnd() -1)
                src.x = sWidthX+roi.xEnd() -1;
            else
                src.x = tx+sWidthX;

            if (ty < 0)
                src.y = sWidthY;
            else if (dimY && ty > roi.yEnd()-1)
                src.y = sWidthY+roi.yEnd()-1;
            else
                src.y =ty+sWidthY;

            if (tz < 0)
                src.z = sWidthZ;
            else if (dimZ && tz > roi.zEnd()-1)
                src.z = sWidthZ+roi.zEnd()-1;
            else
                src.z =tz+sWidthZ;

            if(tt < 0)
                src.t = sWidthT;
            else if(dimT && tt > dimT-1)
                src.t = sWidthT+dimT-1;
            else
                src.t = tt+sWidthT;

            assert(originalUnknowns(pos.x, pos.y, pos.z, pos.t));
            assert(originalUnknowns(src.x, src.y, src.z, src.t));

            for(int k =0; k < nDof; ++k) {
                originalUnknowns(pos.x, pos.y, pos.z, pos.t)[k] =
                        originalUnknowns(src.x, src.y, src.z, src.t)[k];
                //also update boundary from pyramid unknowns -
                // take care to call updateDelta() first!
                pyramidUnknowns(pos.x, pos.y, pos.z, pos.t)[k] =
                        pyramidUnknowns(src.x, src.y, src.z, src.t)[k];
            }
        }
    }
}

template <typename T>
void PdeBoundaryHandler<T>::display()
{
    cimg_library::CImg<float> dm(unknowns.dimx(), unknowns.dimy(),
                                 unknowns.dimz(), nDof*(dimT ? dimT : 1), 0);
    cimg_library::CImg<float> dmask(unknowns.dimx(), unknowns.dimy(),
                                    unknowns.dimz(), 1);

    cimg_forXYZ(unknowns, x, y, z) {
        if(unknowns(x, y, z))
            for(int k =0; k < dm.dimv(); ++k) {
                if(!finite(unknowns(x, y, z)[k]))
                    dm(x, y, z, k) = 255;
                else
                    dm(x, y, z, k) = (float)unknowns(x, y, z)[k];
            }

        if(boundaryMask(x, y, z))
            dmask(x, y, z) = 255;
        else
            dmask(x, y, z) = 0;
    }

    cimg_library::CImgDisplay dsp(dmask);
    dm.display("current parameter map");
    dmask.display("domain");
}

template <typename T>
T*** PdeBoundaryHandler<T>::fromCImg(const cimg_library::CImg<T> &src) {
    T*** res = 0;
    // assert(src.dimx() == dimX && src.dimv() == nDOF);
    if (is1d) {
        //assert(src.dimy() == 1 && src.dimz() == 1);
        res = (T***) new T[src.dimx()*src.dimv()];
        cimg_forXV(src, x, v)
            ((T*)res)[x*src.dimv()+v] = src(x, 0, 0, v);
    }
    else if(is2d) {
        //assert(src.dimy() == dimY && src.dimz() == 1);
        res = (T***) new T*[src.dimy()];
        cimg_forY(src, y)
            res[y] = (T**) new T[src.dimx()*src.dimv()];
        cimg_forXYV(src, x, y, v)
            ((T**)res)[y][x*src.dimv()+v] = src(x, y, 0, v);
    }
    // 4d and 3d are handled alike.
    else {
        // assert(src.dimy() == dimY && src.dimz() == dimZ);
        res = new T**[src.dimz()];
        cimg_forZ(src, z) {
            res[z] = (T**) new T*[src.dimy()];
            cimg_forY(src, y)
                res[z][y] = (T*) new T[src.dimx()*src.dimv()];
        }
        cimg_forXYZV(src, x, y, z, v)
            res[z][y][x*src.dimv()+v] = src(x, y, z, v);
    }

    return res;
}

template <typename T>
void PdeBoundaryHandler<T>::toCimg(T*** src, cimg_library::CImg<T>& dst) {
    dst.assign(dimX,
               dimY ? dimY : 1,
               dimZ ? dimZ : 1,
               dimT ? nDof*dimT : nDof,
               0);

    if(is1d)
        cimg_forXV(dst, x, v)
            dst(x, 0, 0, v) = at(src, x)[v];       // ((T*)src)[x*nDOF+v];
    else if(is2d) {
        cimg_forXYV(dst, x, y, v)
            dst(x, y, 0, v) = at(src, x, y)[v];    // ((T**)src)[y][x*nDOF+v];
        cimg_forY(dst, y)
            delete src[y];
    }
    else {
        cimg_forXYZV(dst, x, y, z, v)
            dst(x, y, z, v) = at(src, x, y, z)[v]; // src[z][y][x*nDOF+v];
        cimg_forZ(dst, z) {
            cimg_forY(dst, y)
                delete src[z][y];
            delete src[z];
        }
    }
    delete src;
}

#endif // _PdeBoundaryHandler_HXX
