/*

	R Project Library

	RChoquetPropagator.h

	Propagator for Choquet integrals - Header.

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
#ifndef RChoquetPropagator_H
#define RChoquetPropagator_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rvector.h>


//-----------------------------------------------------------------------------
// include files for GeCode
#include <gecode/minimodel.hh>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class RTestChoquet;


//-----------------------------------------------------------------------------
/**
 * The RChoquetPropagator class provides a propagator of the constraints linked
 * to the Choquet integrals. In practice, it waits until all the values are
 * fixed and set the cost function.
 * @short Choquet Integrals Constraint Propagator.
 */
class RChoquetPropagator : public Gecode::Propagator
{
protected:

	/**
	 * The tester.
	 */
	RTestChoquet* Test;

	/**
	 * Views on the Shapley indexes to optimize.
	 */
	Gecode::ViewArray<Gecode::Int::IntView> v;

	/**
	 * Views on the interaction indexes to optimize.
	 */
	Gecode::ViewArray<Gecode::Int::IntView> I;

	/**
	 * View on the cost function to maximize.
	 */
	Gecode::Float::FloatView Cost;

public:

	/**
	 * Constructor.
    * @param home            Home space.
    * @param test            Tester.
    * @param V               Shapley indexes to optimize.
    * @param i               Interaction indexes to optimize.
    * @param cost            Cost function to optimize.
    */
	RChoquetPropagator(Gecode::Home home,RTestChoquet* test,Gecode::IntVarArray& V,Gecode::IntVarArray& i,Gecode::Float::FloatView cost);

	/**
	 * Copy constructor.
	 * @param home            Home space.
	 * @param share           Internal parameter for GeCode.
	 * @param p               Propagator to copy.
	 */
	RChoquetPropagator(Gecode::Space& home, bool share, RChoquetPropagator& p);

	/**
	 * Function that post a Choquet integrals constraint propagator.
    * @param home            Home space.
    * @param test            Tester.
    * @param V               Shapley indexes to optimize.
    * @param i               Interaction indexes to optimize.
    * @param cost            Cost function to optimize.
    * @return the status of operation.
    */
	static Gecode::ExecStatus post(Gecode::Home home,RTestChoquet* test,Gecode::IntVarArray& V,Gecode::IntVarArray& i,Gecode::Float::FloatView cost);

	/**
	 * Release the instance.
    * @param home            Home space.
    * @return the amount of memory to dispose.
    */
	virtual size_t dispose(Gecode::Space& home);

	/**
	 * Create a new propagator by copy.
	 * @param home            Home space.
    * @param share           Internal parameter for GeCode.
    * @return a pointer to the propagator to create.
    */
	virtual Gecode::Propagator* copy(Gecode::Space& home, bool share);

	/**
	 * Get the importance of this propagator.
	 * @param home            Home space.
    * @param mod             Internal parameter for GeCode.
    * @return PropCost::binary(PropCost::HI).
    */
	virtual Gecode::PropCost cost(const Gecode::Space& home, const Gecode::ModEventDelta& mod) const;

	/**
	 * Verify if all the variables are assigned. If it is the case, the cost
	 * function is computed.
    * @param home            Home space.
    * @param mod             Internal parameter for GeCode.
    * @return ES_SUBSUMED if all variables are assigned, else ES_NOFIX.
    */
	virtual Gecode::ExecStatus propagate(Gecode::Space& home, const Gecode::ModEventDelta& mod);

	/**
	 * Get the value of a given Shapley index for a particular Choquet integral
	 * for the solution to evaluate.
    * @param id              Identifier of the Choquet integral.
    * @param i               Identifier of the criteria.
    * @return the Shapley index.
    */
	double vi(size_t id,size_t i) const;

	/**
	 * Get the value of an interaction index for a particular Choquet integral
	 * for the solution to evaluate.
    * @param id              Identifier of the Choquet integral.
    * @param i               Identifier of the first criteria.
    * @param j               Identifier of the second criteria.
    * @return the interaction index.
    */
	double Iij(size_t id,size_t i,size_t j) const;

	/**
	 * Compute the Choquet integral for a given identifier.
	 * @param id              Identifier of the Choquet integral.
	 * @param vec             Vector containing the values for the different
	 *                        criteria.
    */
	double ComputeChoquet(size_t choquet,R::RVector& vec) const;

	/**
	 * Destructor.
    */
	virtual ~RChoquetPropagator(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
