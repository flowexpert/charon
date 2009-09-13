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
/** @file   PdeBoundaryHandler.h
 *  @brief  Declaration of convolution routines.
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   15.06.2009
 *
 *  Based on PdeBoundaryHandler.hxx by Daniel Kondermann.
 *
 *  This file contains only the routine declarations.
 *  Implementations of the template functions are in PdeBoundaryHandler.hxx.
 *  In most cases you want to include PdeBoundaryHandler.hxx instead of
 *  PdeBoundaryHandler.h.
 */

#ifndef _PdeBoundaryHandler_H
#define _PdeBoundaryHandler_H

#ifndef cimg_strict
    #define cimg_strict
#endif
#include "CImg.h"
#include "Roi.h"
#include <vector>

/// Handling of PDE boundaries.
/// Internally all maps are stored in mask coordinates.
/// The real unknowns coordinates depend
/// on the stencil size stored in sWidth!
template <typename T>
class PdeBoundaryHandler {
private:
    /// 4D Coordinates.
    class Position {
    public:
        /// @name   coordinates
        //  @{
        int x, y, z, t;
        //  @}
        /// Initialize with given values.
        Position(int x=0, int y=0, int z=0, int t=0);
        /// Assign values of another Position.
        const typename PdeBoundaryHandler<T>::Position& operator= (
                const typename PdeBoundaryHandler<T>::Position& B);
    };

    /// the mask being true inside the boundary
    cimg_library::CImg<bool> boundaryMask;

    /// @name   Unknowns
    /// - All unknowns points from
    ///     -# current PETSC-Result and
    ///     -# PETSC-Result plus PyramidResult
    //  @{
    cimg_library::CImg<T*> originalUnknowns;
    cimg_library::CImg<T*> pyramidUnknowns;
    //  @}

    /// Contains pointers to an array where the residuals are stored.
    /// This is either managed by PETSC or by this class,
    /// depending on createInternalPointers
    cimg_library::CImg<T*> residuals;

    /// A list of all boundary positions.
    std::vector<Position> boundaryPositions;
    /// @todo   add content
    cimg_library::CImgList<int> getValuePositions;
    /// @todo   add content
    cimg_library::CImgList<T>* delta;
    /// @todo   add content
    unsigned int iteratorPos;

    /// @name   Widths and dimensions
    /// This is only stored in three values because problem dimension can be
    /// 1, 2 or 3 and access in is*() should be fast.
    //  @{
    int sWidthX, sWidthY, sWidthZ, sWidthT;
    bool is1d, is2d, is3d;
    int dimX, dimY, dimZ, dimT, nDof;
    //  @}

    /// If false, pointers have to be supplied by some other object,
    /// e.g. a PETSC solver
    bool createInternalPointers;
    /// @todo   add content
    bool unknownsSourceOverride;

    /// @name   used for grid computing
    //  @{
    Roi<int> roi, extendedRoi;
    //  @}

    /// Retrieve the pointer to an array of size nDof at node (x,y,z,t)
    /// of any node-CImg by correcting the coordinates wrt the stencilWidths
    /// @param  x,y,z,t         value position coordinates
    /// @param  data            image to read from
    T* get(int x, int y, int z, int t,
           const cimg_library::CImg<T*>* data);

    /// Const version of get(int, int, int, int, cimg_library::CImg<T*>*).
    /// @param  x,y,z,t         value position coordinates
    /// @param  data            image to read from
    const T* getC(int x, int y, int z, int t,
                  const cimg_library::CImg<T*>* data) const;

    /// Retrieves the next boundary position beginning at an arbitrary
    /// location. This function can be used to set constant boundary values
    /// or initial boundary values.
    /// The function returns false if no more boundary points exist.
    /// @param  position        current position output
    bool getNextBoundaryPosition(Position& position);

    /// Resets the boundary iterator to the first position.
    void resetBoundaryPositionIterator();

    /// Update delta with new results.
    /// Call this after changing extendedRoi or unknowns.
    void updateDelta();

public:
    PdeBoundaryHandler();
    ~PdeBoundaryHandler();

    /// Reset original unknowns, pyramid unknowns and residuals.
    void clearUnknowns();

    /// The current pointer to the unknowns which is used.
    /// (Depends on setUnknownsSource())
    cimg_library::CImg<T*>* unknowns;

    /// Read access to roi.
    const Roi<int>& getRoi() const;

    /// Read access to extendedRoi.
    const Roi<int>& getExtendedRoi() const;

    /// @name   Stencil width getters.
    //  @{
    int getStencilWidthX() const;
    int getStencilWidthY() const;
    int getStencilWidthZ() const;
    int getStencilWidthT() const;
    //  @}

    /// If set to true, unknowns is retrieved and set using originalUnknowns
    /// (for unknowns terms), else unknowns points to pyramidUnknowns
    /// (for regularizer terms)
    void setUnknownsSource(bool useOriginalUnknowns);

    /// Toggle for ignoring any calls to setUnknownsSource().
    /// Will be reset to false in any call to init().
    /// Used to enforce the use pyramidUnknowns, so that even after the solver
    /// returned the solution for the problem, the solution can still be
    /// accessed via the BoundaryHandler.
    ///
    /// <b>Applications:</b>\n
    /// FlowDR::debugEnergies (the PdeTerm is not allowed to switch
    /// setUnknownsSource because data in originalUnknowns is already lost)
    /// @param  unknownsSourceOverride
    ///                         new value
    void setUnknownsSourceOverride(bool unknownsSourceOverride);

    /// Initialization.
    /// Stores all known unknowns points to a local unknowns array which is
    /// being used to handle boundary conditions. Therefore the stencilWidth
    /// of the solver setup is used to create some artificial pointers to
    /// T arrays for the boundary values.
    /// Initializes all masks and arrays need to handle a problem
    /// of given dimensions.
    /// @param  dimX,dimY,dimZ,dimT,nDof
    ///                         problem dimensions
    /// @param  sWidthX,sWidthY,sWidthZ,sWidthT
    ///                         stencil widhts
    /// @param  createInternalPointers
    ///                         internal pointer switch
    void init(int dimX, int dimY, int dimZ, int dimT, int nDof,
              int sWidthX, int sWidthY, int sWidthZ, int sWidthT,
              bool createInternalPointers);

    /// Get number of dimensions (1-4).
    int getDimension() const;

    /// Add deltas to unknowns.
    /// This function takes originalUnknowns, adds delta and saves result to
    /// pyramidUnknowns on which regularizers run, when a pyramid scheme
    /// is used. Hence, this function should be called after originalUnknowns
    /// has been updated
    /// @remarks
    ///     To be called by pyramid solver AFTER init() has been called
    /// @param  delta           pointer to new delta values
    void setCurrentDelta(cimg_library::CImgList<T>* delta);

    /// Update unknowns and residuals.
    /// Only callable if createInternalPointers == false (for PETSC-Wrapper)
    /// @param  unknowns,residuals,roi
    ///                         new values
    void updateUnknowns(T*** unknowns, T*** residuals, Roi<int>& roi);

    /// Update boundaries.
    /// Uses the mask and the stored T* pointers to set all boundaries to the
    /// correct values using von Neumann boundary conditions.
    /// If you want to use Dirichlet boundary conditions you can set all
    /// boundary points using the function getNextBoundaryPosition()
    void updateBoundaries();

    /// Convert from CImg to 3D-Array.
    T*** fromCImg(const cimg_library::CImg<T>& src);

    /// Convert from 3D-Array to CImg.
    /// Will delete src.
    /// @param  src             3D-Array input
    /// @param  dst             image output
    void toCimg(T*** src, cimg_library::CImg<T>& dst);

    /// @name   Element selection.
    /// These functions can be used to access any 3D-Array that is stored in
    /// the format assumed by this class.
    //  @{
    T* at(T*** unknowns, int x) const;
    T* at(T*** unknowns, int x, int y) const;
    T* at(T*** unknowns, int x, int y, int z) const;
    T* at(T*** unknowns, int x, int y, int z, int t) const;
    //  @}

    /// @name   Element access with boundary handling.
    /// Returns the T * structure at location (x,y,z)
    /// and automatically handles boundaries.
    //  @{
    /// Return unknowns array of size nDof.
    /// @param  x,y,z,t         position coordinates
    const T* getU(int x, int y=0, int z=0, int t=0) const;
    /// Get residuals at given position
    /// @param  x,y,z,t         position coordinates
    /// @todo check if really needed
    const T* getR(int x, int y=0, int z=0, int t=0) const;
    /// Set unknowns.
    /// @param  x,y,z,t         position coordinates
    /// @param  p               position in unknowns array
    /// @param  v               new value
    void setU(int x, int y, int z, int t, int p, T v);
    /// Set residuals at given position.
    /// @param  x,y,z,t         position coordinates
    /// @param  p               position in residuals array
    /// @param  v               new value
    void setR(int x, int y, int z, int t, int p, T v);
    /// Get value of the boundaryMask at the stored value position.
    /// @param x,y,z,t          position coordinates
    bool isBoundary(int x, int y, int z, int t) const;
    //  @}

    /// Visualize unknowns as image using cimg_library::CImgDisplay.
    void display();
};

#endif // _PdeBoundaryHandler_H
