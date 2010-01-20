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
/** @file   ImgTool.h
 *  @brief  ImgTool routine declaration
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  @date   05.06.2009
 *
 *  Based on ImgTool.hxx by Daniel Kondermann.
 *
 *  This file contains only the routine declarations.
 *  Implementations of the template functions are in ImgTool.hxx.
 *  In most cases you want to include ImgTool.hxx instead of ImgTool.h.
 */

#ifndef _ImgTool_H
#define _ImgTool_H

#ifndef cimg_strict
    #define cimg_strict
#endif
#include <CImg.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define finite _finite
    #define CHARON_GDI
#endif

#include "Interpolator.h"
#include "Roi.h"

/// Collection of image manipulation routines.
namespace ImgTool {
    /// %Normalize image.
    /// This stretches/shrinks the values linearly so that the maximal
    /// value will be maxVal and the minimal value will be minVal.
    /// @param  img         input image
    /// @param  minVal      lower bound of target range
    /// @param  maxVal      upper bound of target range
    template <typename T>
    void normalizeSlices(cimg_library::CImg<T>& img,
                         T minVal, T maxVal);

    /// Generate integral image.
    /// Result is stored in dst.
    /// Integration is performed over the xy-plane (for each z,v value).
    /**
        \f[
            \text{dst}(x,y,z,v) =
                \sum_{i=0}^x\sum_{j=0}^y\text{src}(i,j,z,v)
        \f]
     */
    /// @param  src         image input
    /// @param  dst         image output (same size as src)
    template <typename T, typename T2>
    void integralImage2D(const cimg_library::CImg<T >& src,
                               cimg_library::CImg<T2>& dst);

    /** Generate histogram.
     *  Target is the image ist, the histogram has nBins bins in the range
     *  from min to max. Subregions of the image can be selected using the
     *  roi. If normalize is true, the resulting histogram is divided by the
     *  considered region volume.
     *  @param  img         image input
     *  @param  hist        histogram output (size: nBins x 1)
     *  @param  nBins       number of histogram bins
     *  @param  min         lower pixel value bound
     *                      (pixels below min are counted in the min bin)
     *  @param  max         upper pixel value bound
     *                      (pixels above max are counted in the max bin)
     *  @param  normalize   division by the roi volume to normalize
     *  @param  roi         Region to consider for histogram generation.
	 *						If no roi given, the whole image will be used.
	 */
    template <typename T>
    void histogram(const cimg_library::CImg<T>& img, cimg_library::CImg<T>& hist,
                   int nBins, T min, T max, bool normalize=false, const Roi<int>* roi=0);

    /// Generate 2D histogram from two image sources.
    /// The number of bins is extracted from the hist size
    /// (nBins = hist.dimx() (= hist.dimy())).
    ///
    /// hist(v1, v2) is the number of pixels with src1(x1,y1)=v1' and
    /// src2(x2,y2)=v2' (v1' and v2' are calculated so that there are
    /// hist.dimx() values between min and max).
    ///
    /// hist has to be a quadratic image (hist.dimx() == hist.dimy()).
    /// @param  src1        first image input
    /// @param  src2        second image input
    /// @param  hist        histogram output (quadratic size)
    /// @param  min         lower pixel value bound
    ///                     (pixels below min are counted in the min bin)
    /// @param  max         upper pixel value bound
    ///                     (pixels above max are counted in the max bin)
    /// @param  roi         region to consider for histogram generation
    /// @param  normalize   division by the roi volume to normalize
    /// @returns            maximal value from hist
    template <typename T>
    T jointHistogram(cimg_library::CImg<T>& src1, cimg_library::CImg<T>& src2,
                     T min, T max, cimg_library::CImg<T>& hist, Roi<int> roi,
                     bool normalize);

	/** Create warped image from src and flow.
	 *  Warped over the first two dimensions.
	 *  Output is dst. The sizes (xyz) of src and flow have to be the same,
	 *  flow is a vector-field (flow.dimv() >= 2).
	 *  The additional parameter weight can be used to invert the flow (set
	 *  to -1) or for warping half in both directions.
		\f[
			\text{dst}(x,y,z,t) = \text{src}(
				x+\text{weight}\cdot\text{flow}_1(x,y,z,t),
				y+\text{weight}\cdot\text{flow}_2(x,y,z,t),z,t)
		\f]
	 *  The image coordinates of src are interpolated with the given
	 *  interpolator.
	 *  @param[in]   src          image input (size: x,y,z,t)
	 *  @param[in]   flow         flow field input (size: x,y,z,t,v>=2)
	 *  @param[out]  dst          output image (will be set to size x,y,z,t)
	 *  @param[in]   interpolator Interpolator to use if non-integer values
	 *                            occur.
	 *  \param[in]   weight       multiply flow with this factor before warping
	 */
    template <typename T>
    void warp2D(
			const cimg_library::CImg<T>& src,
			const cimg_library::CImgList<T>& flow,
			cimg_library::CImg<T>& dst,
			const Interpolator<T>* interpolator,
			float weight=1.f);

    /** Create warped image from src and flow.
     *  Warped over the first two dimensions and accumulate flow over
     *  the z dimension.
     *  Output is dst. The sizes (xy) of src and flow have to be the same,
     *  but in the z dimension, flow.dimz() == src.dimz() - 1 has to be valid.
     *  flow is a vector-field (flow.dimv() >= 2).
     *  The image coordinates of src are interpolated with the given
     *  interpolator.
     *  @param[in]   src          image input (size: x,y,z,t)
     *  @param[in]   flow         flow field input (size: x,y,z,t,v>=2)
     *  @param[out]  dst          output image (will be set to size x,y,z,t)
     *  @param[in]   interpolator Interpolator to use if non-integer values occur.
	 */
    template <typename T>
    void warpToFirstFrame(const cimg_library::CImg<T>& src,
                          const cimg_library::CImgList<T>& flow,
                          cimg_library::CImg<T>& dst,
                          const Interpolator<T>* interpolator);

    /// Use histogram p(x) and integrate to retrieve mean and variance.
    /**
        \f{align*}{
            \text{mean} &= \int x \cdot p(x)~dx \\
            \text{variance} &= \int x^2 \cdot p(x)~dx - \text{mean}^2
        \f}
     */
    /// imgMin and imgMax are used for normalization of x and dx and should
    /// be the same values that have been used for the histogram generation.
    /// @param  hist        hisogram
    /// @param  imgMin      min from histogram generation
    /// @param  imgMax      max from histogram generation
    /// @param  mean        mean output
    /// @param  variance    variance output
    template <typename T>
    void meanVarianceFromHistogram(const cimg_library::CImg<T>& hist,
                                   T imgMin, T imgMax,
                                   T& mean, T& variance);

    /// Calculate covariance of two images from their joint Hisotgram.
    /// imgMin and imgMax are used for normalization of x and dx and should
    /// be the same values that have been used for the histogram generation.
    /**
        \f[
            \text{ret} = \int_{i=0}^n\int_{j=0}^n
                i \cdot j \cdot \text{jointHist}(i,j)~di~dj
                - \text{mean1} \cdot \text{mean2}
        \f]
      */
    /// @param  jointHist   joint hisogram of img1 and img2
    /// @param  imgMin      min from histogram generation
    /// @param  imgMax      max from histogram generation
    /// @param  mean1       mean of img1
    /// @param  mean2       mean of img2
    template <typename T>
    T covariance(const cimg_library::CImg<T>& jointHist,
                 T mean1, T mean2, T imgMin, T imgMax);

    /// Convert image to grayscale image.
    /// Calculate mean along v dimension and reduce v dimension to size 1.
    /// @param  img         input and output image (in place)
    template <typename T>
    void toGray(cimg_library::CImg<T>& img);

    /// Normalize image list.
    /// All images are normalized linearly so that the global minimum pixel is
    /// min and the global maximum pixel is max.
    /// The same transformation is applied to all images.
    /// @param  list        image list to normalize
    /// @param  min         desired global minimum
    /// @param  max         desired global maximum
    template <typename T>
    void coNormalize(cimg_library::CImgList<T>& list, T min, T max);

    /// Move dimension z to v.
    /// \f[ \text{dst}(x,y,0,v) = \text{src}(x,y,v,0) \f]
    /// Only the first v-channel is copied.
    /// @param  src         image source
    /// @param  dst         image target
    template <typename T>
    void ZtoV(const cimg_library::CImg<T>& src, cimg_library::CImg<T>& dst);

    /// Move dimension v to z.
    /// \f[ \text{dst}(x,y,z,0) = \text{src}(x,y,0,z) \f]
    /// Only the first z-channel is copied.
    /// @param  src         image source
    /// @param  dst         image target
    template <typename T>
    void VtoZ(const cimg_library::CImg<T> &src, cimg_library::CImg<T>& dst);

    /// Draw chessBoard with fieldSizeX x fiedlSizeY rectangles.
    /// Black color has value 0 and white color has value 1.
    /// Only the 2D xy-plane is written.
    /// @param  fieldSizeX  rect size in x-dim
    /// @param  fieldSizeY  rect size in y-dim
    /// @param  dst         output image
    template <typename T>
    void chessBoard2D(int fieldSizeX, int fieldSizeY,
                      cimg_library::CImg<T>& dst);

    /// Load image sequence from multiple files.
    /// \f[ \text{seq}(x, y, i, v) = \text{file}[i](x,y,0,v) \f]
    /// All images have to be of the same size. If some file contains multiple
    /// images, only the first one is used.
    /// @param  files       image filename list
    /// @param  seq         sequence output
    template <typename T>
    void loadSequence(const std::vector<std::string>& files,
                      cimg_library::CImg<T>& seq);

    /// Load image sequence from multiple image files.
    /// The filename is generated using the given filename string which will
    /// be interpreted in the printf-style. The must be some "%d" tag in
    /// the string to be replaced by the image number.
    /// @param  fileName    name pattern of the files to load
    /// @param  startFrame  smallest image number
    /// @param  endFrame    last image to load
    /// @param  sequence    sequence output
    template <typename T>
    void loadSequence(std::string fileName, int startFrame, int endFrame,
                      cimg_library::CImg<T>& sequence);

    /// Load image sequence from multiple raw image files.
    /// The filename is generated using the given filename string which will
    /// be interpreted in the printf-style. The must be some "%d" tag in
    /// the string to be replaced by the image number.
    /// @param  fileName    name pattern of the files to load
    /// @param  startFrame  smallest image number
    /// @param  endFrame    last image to load
    /// @param  sizeX       image size in x dimesion
    /// @param  sizeY       image size in y dimesion
    /// @param  sizeV       image size in v dimesion
    /// @param  sequence    sequence output
    template <typename T>
    void loadSequenceRaw(std::string fileName, int startFrame, int endFrame,
                         int sizeX, int sizeY, int sizeV,
                         cimg_library::CImg<T>& sequence);

    /// Save image sequence to multiple image files.
    /// The filename is generated using <tt>fileName####.fileType</tt>
    /// where "####" is replaced by the position in the image sequence.
    /// @param  fileName    basename of the generated files
    /// @param  fileType    file extension
    /// @param  sequence    sequence to write
    template <typename T>
    void saveSequence(std::string fileName, std::string fileType,
                      const cimg_library::CImg<T>& sequence);

    /// Extend the image content in the given region to the entire image using
    /// constant boundary conditions.
    /// The image values at the borders are copied to fill up the image
    /// region outside the roi.
    /// @param  roi         region to keep
    /// @param  img         image to manipulate (in place)
    template <typename T>
    void extendRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img);

    /// Fill up image outside the roi by zero padding.
    /// @param  roi         region to keep
    /// @param  img         image to manipulate (in place)
    template <typename T>
    void zeroRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img);

    /// Fill up image outside the using reflected boundary conditions.
    /// @param  roi         region to keep
    /// @param  img         image to manipulate (in place)
    template <typename T>
    void mirrorRoi2D(const Roi<int>& roi, cimg_library::CImg<T>& img);

    /// Create 2D image with sinus waves.
    /**
        \f[
            \text{res}(x,y) = \sin(k_x \cdot (x+\phi_x))
                + \sin(k_y \cdot (y+\phi_y))
        \f]
     */
    /// @param  frequencyX  \f$k_x\f$
    /// @param  frequencyY  \f$k_y\f$
    /// @param  phaseX      \f$\phi_x\f$
    /// @param  phaseY      \f$\phi_y\f$
    /// @param  sizeX       image size (x-dim)
    /// @param  sizeY       imgae size (y-dim)
    /// @returns            \f$\text{res}\f$ (size sizeX \f$\times\f$ sizeY)
    template <typename T>
    cimg_library::CImg<T> sinus2D(T frequencyX, T frequencyY,
                                  T phaseX, T phaseY,
                                  int sizeX, int sizeY);
    /// Convert image list into image sequence.
    /// You have to make sure that in each image of the list dimz == 1.
    /// \f[\text{dst}(x, y, i, v) = \text{list}[i](x, y, 0, v)\f]
    /// @param  list        image sequence input
    /// @param  dst         single image output
    template <typename T>
    void toSequence(const cimg_library::CImgList<T>& list,
                          cimg_library::CImg<T>&     dst);

    /// Copy single image into sequence slice.
    /// \f[\text{dst}(x, y, \text{sliceIndex}, v) = \text{slice}(x, y, 0, v)\f]
    /// \param  slice       source slice
    /// \param  dst         target sequence
    /// \param  sliceIndex  index of the slice in the sequence to overwrite
    template <typename T>
    void toSlice(const cimg_library::CImg<T>& slice,
                       cimg_library::CImg<T>& dst,
                 int sliceIndex);

    /// Copy single image into image plane.
    /// \f[\text{dst}(x, y, z, \text{planeIndex}) = \text{plane}(x, y, z, 0)\f]
    /// \param  plane       source plane
    /// \param  dst         target sequence
    /// \param  planeIndex  index of the plane in the sequence to overwrite
    template <typename T>
    void toPlane(const cimg_library::CImg<T>& plane,
                 cimg_library::CImg<T>& dst,
                 int planeIndex);

    /// Calculate root of the local variance in an image window of given size.
    /// Window size is \f$\text{maskSize}^2\f$ if maskSize is odd, otherwise,
    /// maskSize is increased by one.
    /**
        \f[
            \text{dst}(x,y) = \sqrt{\int_{x'=x}^{x+m}\int_{y'=y}^{y+m}
                \left(\text{src}(x',y')-\mu\right)^2~dx'~dy'}
        \f]
     */
    /// where \f$\mu\f$ is the mean over the image window.
    /// @param  maskSize    \f$m\f$
    /// @param  src         source image
    /// @param  dst         outut image of variances
    template <typename T>
    void localVarianceRoot2D(int maskSize,
                             const cimg_library::CImg<T>& src,
                                   cimg_library::CImg<T>& dst);

    /// Local normalization.
    /// Values in an image window of size \f$\text{maskSize}^2\f$ are
    /// normalized from 0 to 1, then the normalized value of the pixel in
    /// this subwindow is stored in dst and after all dst is rescaled between
    /// vMin and vMax.
    /// @param  maskSize    mask size (like in localVarianceRoot2D())
    /// @param  vMin        normalization offset
    /// @param  vMax        normalization upper bound
    /// @param  src         source image
    /// @param  dst         result output
    template <typename T>
    void localNormalization2D(int maskSize, T vMin, T vMax,
                              const cimg_library::CImg<T>& src,
                                    cimg_library::CImg<T>& dst);

    /// Apply sigmoid threshold.
    /**
        \f[
            \text{dst}(x, y, z, v) = \frac{1}
                {1 + \exp\Bigl(-\sigma\cdot\bigl(
                    \text{src}(x, y, z, v)-\mu\bigr)\Bigr)}
        \f]
     */
    /// @param  center      \f$\mu\f$
    /// @param  sharpness   \f$\sigma\f$
    /// @param  src         image input
    /// @param  dst         result output
    template <typename T>
    void sigmoidThreshold(T center, T sharpness,
                          const cimg_library::CImg<T>& src,
                                cimg_library::CImg<T>& dst);

    /// Copy image into another.
    /// The target image can/should be larger thant the source image.
    /// The target position can be adjusted.
    /// \f[\text{to}(x+l, y+t, z+f, v) = \text{from}(x, y, z, v)\f]
    /// @param  from        copy source
    /// @param  to          target image
    /// @param  left        \f$l\f$
    /// @param  top         \f$t\f$
    /// @param  front       \f$f\f$
    template <typename T>
    void copy(const cimg_library::CImg<T>& from,
                    cimg_library::CImg<T>& to,
              int left = 0, int top = 0, int front = 0);

    /// Generate random pattern with noise in different scales.
    /// @param  dst         result output
    /// @param  dimx        result size in x dimension
    /// @param  dimy        result size in y dimension
    /// @param  eta         noise amplitude
    /// @param  maxLevels   maximal scale number (levels of the used pyramid)
    template <typename T>
    void createAllScaleRandomPattern(cimg_library::CImg<T>& dst,
                                     int dimx, int dimy, float eta,
                                     int maxLevels);

#ifdef CHARON_GDI
    /// Generate image from window bmp header.
    /// @param  pbmih       window bmp header
    /// @param  dst         result output
    template <typename T>
    void fromWindowBMPHeader(LPBITMAPINFOHEADER pbmih,
                             cimg_library::CImg<T>&    dst);
#endif

    /// Convert grayscale image into pseudocolors.
    /// Gray value is interpreted as hue in the hsv color space (val=sat=1).
    /// @param  gray        grayscale image input
    /// @param  hsv         result output
    /// @param  drawColorBar enable drawing of a color bar
    template <typename T>
    void grayToHsv(const cimg_library::CImg<T>&        gray,
                         cimg_library::CImg<T>&        hsv,
                   bool drawColorBar = true);

    /// Draw HSV circle into some image region.
    /// @param  rgbImage    rgb image to draw in
    /// @param  region      target region for HSV color circle
    template <typename T>
    void drawHsvCircle(cimg_library::CImg<T>& rgbImage,
                       const Roi<int>& region);

    /// Create HSV frame in some image.
    /// If imPos >= 3 a black border of 3 pixels is drawn around the hsv box.
    /// @param  rgbImage    rgb image to draw in
    /// @param  imPos       distance from the image borders
    template <typename T>
    void drawHsvFrame(cimg_library::CImg<T>& rgbImage, int imPos = 0);

    /// Convert flow field into an color image.
    /// At each pixel, the value of the flow vector is used to set the
    /// hue value to the vector angle and the val value to the vector length.
    /// Saturation is one.
    /// @todo               drawColorBar not implemented
    /// @param  flow        flow field to visualize
    /// @param  hsv         rgb image output
    /// @param  drawColorBar enable drawing of a color bar
    template <typename T>
    void flowToHsv(const cimg_library::CImg<T>&        flow,
                         cimg_library::CImg<T>&        hsv,
                   bool drawColorBar = true);

    /// Draw a quiver with thick lines.
    /// @param  dst         result output
    /// @param  flow        flow field to visualize
    /// @param  colors      arrow colors
    /// @param  thickness   line/arrow thickness
    /// @param  sampling    distance between arrow tails (grid constant)
    /// @param  factor      scaling factor for arrow length
    /// @param  quiver_type 0: arrows 1: lines
    /// @param  opacity     line/arrow opacity
    template <typename T, typename T2>
    void drawQuiverThick(cimg_library::CImg<T>&        dst,
                         const cimg_library::CImg<T2>& flow,
                         const cimg_library::CImg<T >&  colors,
                         const float                   thickness   =   3,
                         const unsigned int            sampling    =  25,
                         const float                   factor      = -20,
                         const int                     quiver_type =   0,
                         const float                   opacity     =   1);

    /// Draw a quiver with thick lines.
    /// @param  dst         result output
    /// @param  flow        flow field to visualize
    /// @param  color       arrow color(s)
    /// @param  thickness   line/arrow thickness
    /// @param  sampling    distance between arrow tails (grid constant)
    /// @param  factor      scaling factor for arrow length
    /// @param  quiver_type 0: arrows 1: lines
    /// @param  opacity     line/arrow opacity
    template <typename T, typename T2>
    void drawQuiverThick(      cimg_library::CImg<T >& dst,
                         const cimg_library::CImg<T2>& flow,
                         const T* const                color,
                         const float                   thickness   =   3,
                         const unsigned int            sampling    =  25,
                         const float                   factor      = -20,
                         const int                     quiver_type =   0,
                         const float                   opacity     =   1);

    /// Draw an arrow with thick lines.
    /// @param  dst         result output
    /// @param  x0          arrow tail (x-coordinate)
    /// @param  y0          arrow tail (y-coordinate)
    /// @param  x1          arrow head (x-coordinate)
    /// @param  y1          arrow head (x-coordinate)
    /// @param  color       arrow color
    /// @param  thickness   arrow thickness
    /// @param  angle       arrow head angle
    /// @param  length      arrow head length(>0: pixels; <0: percent)
    /// @param  pattern     arrow line pattern (if thickness == 0)
    /// @param  opacity     arrow opacity
    template <typename T>
    void drawArrowThick(cimg_library::CImg<T>&         dst,
                        const int x0, const int y0,
                        const int x1, const int y1,
                        const T* const                 color,
                        const float                    thickness   =   3,
                        const float                    angle       =  30,
                        const float                    length      = -10,
                        const unsigned int             pattern     = ~0L,
                        const float                    opacity     =   1);

    /// Find zero crossings in the input image.
    /// Type of output image can differ from input image type.
    /// @tparam T           input image data type
    /// @tparam T2          output image data type (must only contain 0,1)
    /// @param  src         image input
    /// @param  zeroCrossings result output
    template <typename T, typename T2>
    void zeroCrossings(const cimg_library::CImg<T >&   src,
                             cimg_library::CImg<T2>&   zeroCrossings);

    /// Save raw image with given header to file.
    /// @param  src         image source
    /// @param  fileName    name of the file to write (binary)
    /// @param  header      header to write before the image data
    template <typename T>
    void saveRawWithHeader(const cimg_library::CImg<T>& src,
                           std::string fileName, std::string header);

    /// Write image values as csv table to file.
    /// @param  src         image source
    /// @param  fileName    name of the csv file to write
    /// @param  header      write first row with image dimensions
    /// @param  rowDelimiter row separator (usually newline or ';')
    /// @param  colDelimiter col separator (usually ',', ';', TAB, etc.)
    /// @throws std::string error message on write errors
    template <typename T>
    void saveCsv(const cimg_library::CImg<T>& src,
                 std::string fileName, bool header = true,
                 char rowDelimiter = '\n', char colDelimiter =',');

    /// Read image values from an csv table.
    /// Set dimX etc. to -1 to read dimensions from csv header.
    /// @param  dst         image output
    /// @param  fileName    name of the csv file to read
    /// @param  dimX        size in x dimension
    /// @param  dimY        size in y dimension
    /// @param  dimZ        size in z dimension
    /// @param  dimV        size in v dimension
    /// @param  rowDelimiter row separator (usually newline or ';')
    /// @param  colDelimiter col separator (usually ',', ';', TAB, etc.)
    /// @param  maxLineLength maximum number of lines to read
    /// @remarks            Line length is limited by 1024!
    /// @throws std::string error message on read errors
    template <typename T>
    void loadCsv(cimg_library::CImg<T>& dst, std::string fileName,
                 int dimX = -1, int dimY = -1, int dimZ = -1, int dimV = -1,
                 char rowDelimiter = '\n', char colDelimiter = ',',
                 int maxLineLength = 1024);

    /// Create a plot of vector values given in the data array.
    /// Each vector is plotted as a point in a 2d image.
    /// @param  data        plot point coordinates (\f$2\times n \times 1\f$)
    /// @param  dimx        size of output image (x dimension)
    /// @param  dimy        size of output image (y dimension)
    /// @param  normalizationType 1: normalize to
    ///                                  \f$f_\text{std}\cdot\text{std}\f$ \n
    ///                           2: normalize to quantile range
    /// @param  dst         output image (
    ///              \f$(\text{dimx}+1)\times(\text{dimy}+1)\times1\times3\f$)
    /// @param  stdFactor   \f$f_\text{std}\f$
    template <typename T>
    void scatterPlot(const cimg_library::CImg<T>& data,
                     int dimx, int dimy, char normalizationType,
                     cimg_library::CImg<T>& dst, float stdFactor = 3.0f);

    /// Set values of the input image which are not finite to the given value.
    /// Finite values remain unchanged.
    /// @param  data        values to manipulate
    /// @param  value       replace infinite values with this
    template <typename T>
    void nonFiniteToConstant(cimg_library::CImg<T>& data, T value = 0);

    /// Calculate variance of 2d square blocks of given size.
    /// dst(x,y) is the variance of a \f$n\times n\f$ where
    /// \f$n=2\cdot\text{halfBlockWidth}+1\f$.
    /// @param  src         image source
    /// @param  dst         output image (same size as src)
    /// @param  halfBlockWidth half of the block size
    /// @param  outsideValue value to use outside the image borders
    template <typename T>
    void blockVariance2d(const cimg_library::CImg<T>& src,
                               cimg_library::CImg<T>& dst,
                         int halfBlockWidth, T outsideValue = -100);

    /// Calculate entropy of 2d square blocks of given size.
    /// dst(x,y) is the entropy of a \f$n\times n\f$ where
    /// \f$n=2\cdot\text{halfBlockWidth}+1\f$.\n
    /// The entropy is defined as
    /**
        \f[
            \sum_\text{block}-p(x)\cdot\log_2(p(x)).
        \f]
     */
    /// @param  src         image source
    /// @param  dst         output image (same size as src)
    /// @param  halfBlockWidth half of the block size
    /// @param  outsideValue value to use outside the image borders
    template <typename T>
    void blockEntropy2d(const cimg_library::CImg<T>& src,
                              cimg_library::CImg<T>& dst,
                        int halfBlockWidth, T outsideValue = -100);

    /// Calculate Baroon/Fleet (angular) error.
    /// Interpret the two v channels as x/y component and calculate
    /// the result for each pixel.\n
    /// The result is defined as:
    /**
        \f[
            \text{res} = \text{acos}~\frac{\vec{x}\cdot\vec{y}+1}
                {(\vec{x}^2+1)\cdot(\vec{y}^2+1)}
        \f]
     */
    /// @param  flow        \f$x\f$
    /// @param  gtFlow      \f$y\f$
    /// @param  result      image output (same size as flow/gtFlow)
    template <typename T>
    void barronFleetError(const cimg_library::CImg<T>& flow,
                          const cimg_library::CImg<T>& gtFlow,
                                cimg_library::CImg<T>& result);

    /// Calculate amplitude error.
    /// Interpret the two v channels as x/y component and calculate
    /// the result for each pixel.\n
    /// The result is defined as:
    /**
        \f[
            \text{res} = \left\vert\sqrt{\vec{x}^2}-\sqrt{\vec{y}^2}\right\vert
        \f]
     */
    /// @param  flow        \f$x\f$
    /// @param  gtFlow      \f$y\f$
    /// @param  result      image output (same size as flow/gtFlow)
    /// @param  relative    divide result by \f$\sqrt{\vec{x}^2}\f$
    template <typename T>
    void amplitudeError(const cimg_library::CImg<T>& flow,
                        const cimg_library::CImg<T>& gtFlow,
                              cimg_library::CImg<T>& result,
                        bool relative);

    /// Calculate angular error.
    /// Interpret the two v channels as x/y component and calculate
    /// the result for each pixel.\n
    /// The result is defined as:
    /**
        \f[
            \text{res} = \text{acos}~\frac{\vec{x}\cdot\vec{y}}
                {\sqrt{\vec{x}^2\cdot\vec{y}^2}}
        \f]
     */
    /// @param  flow        \f$x\f$
    /// @param  gtFlow      \f$y\f$
    /// @param  result      image output (same size as flow/gtFlow)
    template <typename T>
    void angleError(const cimg_library::CImg<T>& flow,
                    const cimg_library::CImg<T>& gtFlow,
                          cimg_library::CImg<T>& result);

    /// Calculate endpoint error.
    /// Interpret the two v channels as x/y component and calculate
    /// the result for each pixel.\n
    /// The result is defined as:
    /**
        \f[
            \text{res} = \left\vert\vec{x}-\vec{y}\right\vert_2
        \f]
     */
    /// @param  flow        \f$x\f$
    /// @param  gtFlow      \f$y\f$
    /// @param  result      image output (same size as flow/gtFlow)
    template <typename T>
    void endPointError(const cimg_library::CImg<T>& flow,
                       const cimg_library::CImg<T>& gtFlow,
                             cimg_library::CImg<T>& result);

    /// Calculate Baroon/Fleet (angular) error.
    /// The result is defined as:
    /**
        \f[
            \text{res} = \text{acos}~\frac{\vec{x}\cdot\vec{y}+1}
                {(\vec{x}^2+1)\cdot(\vec{y}^2+1)}
        \f]
     */
    /// @param  fx          \f$\vec{x}_1\f$
    /// @param  fy          \f$\vec{x}_2\f$
    /// @param  gtFx        \f$\vec{y}_1\f$
    /// @param  gtFy        \f$\vec{y}_2\f$
    template <typename T>
    T barronFleetError(const T& fx,   const T& fy,
                       const T& gtFx, const T& gtFy);

    /// Calculate amplitude error.
    /// The result is defined as:
    /**
        \f[
            \text{res} = \left\vert\sqrt{\vec{x}^2}-\sqrt{\vec{y}^2}\right\vert
        \f]
     */
    /// @param  fx          \f$\vec{x}_1\f$
    /// @param  fy          \f$\vec{x}_2\f$
    /// @param  gtFx        \f$\vec{y}_1\f$
    /// @param  gtFy        \f$\vec{y}_2\f$
    /// @param  relative    divide result by \f$\sqrt{\vec{x}^2}\f$
    template <typename T>
    T amplitudeError(const T& fx,   const T& fy,
                     const T& gtFx, const T& gtFy,
                     bool relative);

    /// Calculate angular error.
    /// The result is defined as:
    /**
        \f[
            \text{res} = \text{acos}~\frac{\vec{x}\cdot\vec{y}}
                {\sqrt{\vec{x}^2\cdot\vec{y}^2}}
        \f]
     */
    /// @param  fx          \f$\vec{x}_1\f$
    /// @param  fy          \f$\vec{x}_2\f$
    /// @param  gtFx        \f$\vec{y}_1\f$
    /// @param  gtFy        \f$\vec{y}_2\f$
    template <typename T>
    T angleError(const T& fx,   const T& fy,
                 const T& gtFx, const T& gtFy);

    /// Calculate endpoint error.
    /// The result is defined as:
    /**
        \f[
            \text{res} = \left\vert\vec{x}-\vec{y}\right\vert_2
        \f]
     */
    /// @param  fx          \f$\vec{x}_1\f$
    /// @param  fy          \f$\vec{x}_2\f$
    /// @param  gtFx        \f$\vec{y}_1\f$
    /// @param  gtFy        \f$\vec{y}_2\f$
    template <typename T>
    T endPointError(const T& fx,   const T& fy,
                    const T& gtFx, const T& gtFy);

    /// Calculate masked mean value.
    /// Usual mean value for all pixels with mask != 0.
    /// @param  src         image input
    /// @param  mask        image mask
    /// @param  mean        mean value output
    /// @param  std         standart deviation output
    template <typename T>
    void maskedMeanStd(const cimg_library::CImg<T>& src,
                       const cimg_library::CImg<unsigned char>& mask,
                       T& mean, T& std);

    /// Select frames from an image sequence.
    /// Pick out each n-th frame starting at a given number.
    /// @param  src         image source (sequence)
    /// @param  dst         image output (sequence)
    /// @param  step        stepsize for image picking
    /// @param  start       first image to pick
    template <typename T>
    void nThFrame(const cimg_library::CImg<T>& src,
                        cimg_library::CImg<T>& dst,
                  int step, int start = 0);

    /// Min values of a image sequence.
    /// Look for minimal value of each pixel in an image sequence.
    /**
        \f[
            \text{res}(x,y)=\min_z~\text{src}(x,y,z)
        \f]
     */
    /// @param  src         image source
    /// @param  dst         result output
    template <typename T>
    void minImageZ(const cimg_library::CImg<T>& src,
                         cimg_library::CImg<T>& dst);

    /// Max values of a image sequence.
    /// Look for maximal value of each pixel in an image sequence.
    /**
        \f[
            \text{res}(x,y)=\max_z~\text{src}(x,y,z)
        \f]
     */
    /// @param  src         image source
    /// @param  dst         result output
    template <typename T>
    void maxImageZ(const cimg_library::CImg<T>& src,
                         cimg_library::CImg<T>& dst);

    /// 2d image crop.
    /// @param  src         image to crop
    /// @param  roi         crop borders (only 2d is used)
    template <typename T, typename T2>
    void crop2d(cimg_library::CImg<T>& src, Roi<T2> roi);

    /// 3d image crop.
    /// @param  src         image to crop
    /// @param  roi         crop borders
    template <typename T, typename T2>
    void crop3d(cimg_library::CImg<T>& src, Roi<T2> roi);

	/// get string with image size information
	/// \param  img			image input
	template <typename T>
	std::string sizeString(const cimg_library::CImg<T>& img);

	/// get string with image data information
	/// \param  img			image input
	template <typename T>
	std::string dataString(const cimg_library::CImg<T>& img);

	/// get string containing image content
	/** the image content is printed in a matlab-like
	 *  matrix representation.
	 *  \param  img         image input
	 *  \param  maxEntries  maximal number of matrix rows/cols to print
	 */
	template <typename T>
	std::string contentString(
			const cimg_library::CImg<T>& img,
			unsigned int maxEntries = 5u);

	/// print image information
	template <typename T>
	void printInfo(
		std::ostream& os                 /**[in,out] output stream to print to*/,
		const cimg_library::CImg<T>& img /**[in]     input image*/,
		const std::string& prefix = ""   /**[in]     prefix to print on new lines first*/);
}

#endif // _ImgTool_H
