/*

	GecodeChoquet Library

	RTestChoquet.h

	Test the parameters for Choquet integrals - Header.

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
#ifndef RTestChoquet_H
#define RTestChoquet_H


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdexcept>


//-----------------------------------------------------------------------------
// include files for GeCode
#include <gecode/search.hh>
#include <roptimizechoquet.h>
#include <rchoquetpropagator.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RTestChoquet class allows to find the parameters of one or more Choquet
 * integrals that optimize a given cost function.
 * @short Choquet Integrals Tester
 */
class RTestChoquet
{
	// Internal structure
	struct Choquet;

	/**
	 * Number of Choquet integrals to optimize.
	 */
	size_t NbChoquets;

	/**
	 * Structure containing information on each Choquet integral.
	 */
	Choquet* Choquets;

	/**
	 * Precision of the cost function.
	 */
	int Precision;

	/**
	 * Domain of the parameters. A domain of 2 supposes each parameters may have
	 * two digits after the comma.
	 */
	int Domain;

	/**
	 * Maximum value of the parameters.
	 */
	int Max;

	/**
	 * Specify if the class was already initialized or not.
	 */
	bool MustInit;

	/**
	 * Number of elapsed seconds of the last search.
	 */
	double ElapsedSecs;

	/**
	 * Current optimal solution found.
	 */
	ROptimizeChoquet* CurSol;

	/**
	 * Search engine used to navigate in the tree.
	 */
	Gecode::BAB<ROptimizeChoquet>* Search;

	/**
	 * Total number of Shapley indexes to find.
	 */
	size_t Nbv;

	/**
	 * Total number of interaction indexes to find.
    */
	size_t NbI;

public:

	/**
	 * Constructor.
    * @param nbchoquets      Number of Choquet integrals to optimize.
    */
	RTestChoquet(size_t nbchoquets);

	/**
	 * Set the number of criteria for a given Choquet integral.
    * @param choquet         Identifier of the Choquet integral.
    * @param nb              Number of parameters.
    */
	void SetNbCriteria(size_t choquet,size_t nb);

	/**
	 * Get the number of criteria for a given Choquet integral.
    * @param choquet         Identifier of the Choquet integral.
    * @return the number of criteria.
    */
	size_t GetNbCriteria(size_t choquet) const;

	/**
	 * Get the identifier of a given Shapley index for a particular Choquet integral.
    * @param choquet         Identifier of the Choquet integral.
	 * @param i               Identifier of the criteria.
    * @return the identifier.
    */
	size_t GetviId(size_t id,size_t i) const;

	/**
	 * Get the identifier of an interaction index for a particular Choquet integral.
    * @param choquet         Identifier of the Choquet integral.
	 * @param i               Identifier of the first criteria.
	 * @param j               Identifier of the second criteria.
    * @return the identifier.
    */
	size_t GetIijId(size_t id,size_t i,size_t j) const;

	/**
	 * Get the best solution computed.
    * @return a pointer.
    */
	ROptimizeChoquet* GetBestSol(void) const;

	/**
	 * Get the computed time of the last run.
    * @return the elapsed time in seconds.
    */
	double GetComputedTime(void) const;

	/**
	 * Get the number of nodes evaluated during the last run.
    * @return the number of nodes.
    */
	size_t GetNbNodesEvaluated(void) const;

private:

	/**
	 * Initialize the tester.
    */
	virtual void Init(void);

	/**
	 * Compute the cost function to optimize for a given solution of parameter.
	 * This cost must be in [0,1].
	 *
	 * This function must be overridden by inherited classes.
    * @param prop            Propagator that holds the solution to evaluate.
    * @return the value of the cost function.
    */
	virtual double Compute(RChoquetPropagator& prop)=0;

	/**
	 * Add eventually some constraints between the criteria.
    * @param home            Space to optimize.
    */
	virtual void AddConstraints(ROptimizeChoquet& home);

public:

	/**
	 * Run the tester.
    * @param dom             Domain of the parameter.
    * @param precision       Precision of the solution (By default, 4 number
	 *                        after the comma).
    */
	void Run(int dom,int precision=4);

	/**
	 * Print the parameter values of the current solution.
    */
	void Print(void) const;

	/**
	 * Destructor.
    */
	~RTestChoquet(void);

	friend class RChoquetPropagator;
	friend class ROptimizeChoquet;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
