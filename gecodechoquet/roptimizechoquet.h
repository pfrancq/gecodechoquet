/*

	R Project Library

	ROptimizeChoquet.h

	Brand and Bound Search for the parameters of Choquer integrals - Header.

	Copyright 2012-2013 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef ROptimizeChoquet_H
#define ROptimizeChoquet_H


//-----------------------------------------------------------------------------
// include files for GeCode
#include <floatoptimizespace.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class RTestChoquet;


//-----------------------------------------------------------------------------
/**
 * The ROptimizeChoquet provides a research space when some Choquet integral
 * must be optimized.
 * @short Choquet Integrals Space
 */
class ROptimizeChoquet : public Gecode::FloatMaximizeSpace
{
protected:

	/**
	 * The tester.
	 */
	RTestChoquet* Test;

	/**
	 * The Shapley indexes to optimize.
	 */
	Gecode::IntVarArray v;

	/**
	 * The interaction indexes to optimize.
	 */
	Gecode::IntVarArray I;

	/**
	 * Cost to maximize.
	 */
	Gecode::FloatVar Cost;

public:

	/**
	 * Constructor.
	 * @param test            Tester.
	 * @param nbc             Total number of Shapley indexes to find.
	 * @param nbi             Total number of interaction indexes to find.
	 * @param max             Maximum value of the parameters.
	 */
	ROptimizeChoquet(RTestChoquet* test,size_t nbc,size_t nbi,int max);

	/**
	 * Copy constructor.
    * @param share           Internal parameter for GeCode.
    * @param s               Solution to copy.
    */
	ROptimizeChoquet(bool share, ROptimizeChoquet& s);

	/**
	 * Get the value of a given Shapley index for a particular Choquet integral.
    * @param id              Identifier of the Choquet integral
    * @param i               Identifier of the criteria.
	 * @param dummy           This parameter is just used to distinct methods.
	 *                        Its values has no importance.
    * @return the Shapley index.
    */
	Gecode::IntVar& vi(size_t id,size_t i,size_t dummy);

	/**
	 * Get the value of an interaction index for a particular Choquet integral.
    * @param id              Identifier of the Choquet integral
    * @param i               Identifier of the first criteria.
    * @param j               Identifier of the second criteria.
	 * @param dummy           This parameter is just used to distinct methods.
	 *                        Its values has no importance.
    * @return the interaction index.
    */
	Gecode::IntVar& Iij(size_t id,size_t i,size_t j,size_t dummy);

	/**
	 * Get the value of a given Shapley index for a particular Choquet integral.
    * @param id              Identifier of the Choquet integral
    * @param i               Identifier of the criteria.
    * @return the Shapley index.
    */
	double vi(size_t id,size_t i) const;

	/**
	 * Get the value of an interaction index for a particular Choquet integral.
    * @param id              Identifier of the Choquet integral
    * @param i               Identifier of the first criteria.
    * @param j               Identifier of the second criteria.
    * @return the interaction index.
    */
	double Iij(size_t id,size_t i,size_t j) const;

	/**
	 * Get the value of the cost function to optimize.
    * @return the cost value.
    */
	double GetCost(void) const;

	/**
	 * Copy function.
    * @param share           Internal parameter for GeCode.
    * @return a pointer to the space created.
    */
	virtual Gecode::Space* copy(bool share);

	/**
	 * Get the cost function to optimize.
    * @return Cost.
    */
	virtual Gecode::FloatVar cost(void) const;

	/**
	 * Destructor.
    */
	virtual ~ROptimizeChoquet(void);

	void print(void) const;

	friend class RTestChoquet;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
