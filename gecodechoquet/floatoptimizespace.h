/*
 *  This file is a adaptation of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 * Original file : minimodel.hh
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __GECODE_FLOATOPTIMIZESPACE_HH__
#define __GECODE_FLOATOPTIMIZESPACE_HH__

#include <gecode/minimodel.hh>


namespace Gecode {

  /**
   * \defgroup TaskModelMiniModelOptimize Support for cost-based optimization
   *
   * Provides for minimizing or maximizing the cost value as defined by
   * a cost-member function of a space.
   *
   * \ingroup TaskModelMiniModel
   */
  //@{
  namespace MiniModel {

    /// Baseclass for cost-based optimization
    template<FloatRelType irt>
    class FloatOptimizeSpace : public Space {
    public:
      /// Default constructor
      FloatOptimizeSpace(void);
      /// Constructor for cloning
      FloatOptimizeSpace(bool share, FloatOptimizeSpace& s);
      /// Member function constraining according to cost
      virtual void constrain(const Space& best);
      /// Return variable with current cost
      virtual FloatVar cost(void) const = 0;
    };

  }

  /// Class for minimizing cost
  typedef MiniModel::FloatOptimizeSpace<FRT_LE> FloatMinimizeSpace;

  /// Class for maximizing cost
  typedef MiniModel::FloatOptimizeSpace<FRT_GR> FloatMaximizeSpace;
  //@}

}


#include <floatoptimizespace.hpp>

#endif	/* FLOATOPTIMIZESPACE_H */

