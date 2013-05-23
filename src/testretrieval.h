/*

	TestRetrieval.h

	Parametrize the Choquet integral for a retrieval problem - Header.

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
#ifndef TestRetrieval_H
#define TestRetrieval_H


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <cstdarg>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmatrix.h>
#include <rtestchoquet.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
/**
 * The ResultItem class provides a result for a given query.
 */
class ResultItem
{
	/**
	 * Identifier of the document.
	 */
	size_t DocId;

	/**
	 * Define if the retrieved document is relevant.
    */
	bool Relevant;

	/**
	 * The rank of the document.
	 */
	double Rank;

public:

	/**
	 * Constructor.
    * @param docid           Identifier of the document.
    * @param rel             Is it relevant?
    */
	ResultItem(size_t docid,bool rel);

	/**
	 * Compare two items such as the one with the highest rank is appears first in
	 * a container.
    * @param res             Item to compare with.
    * @return a -1,0 or +1 depending on the ranks of the items compared.
    */
	int Compare(const ResultItem& res) const;

	friend class TestRetrieval;
};


//------------------------------------------------------------------------------
/**
 * The TestRetrieval class provides a tester for a Choquet integral applied to
 * a retrieval problem.
 * @short Retrieval Tester.
 */
class TestRetrieval : public RTestChoquet
{
	/**
	 * Number of documents.
	 */
	size_t NbDocs;

	/**
	 * Number of criteria defined for each document.
	 */
	size_t NbCriteria;

	/**
	 * Scores of the different documents for each criteria.
	 */
	RMatrix Scores;

	/**
	 * Results for a first query.
	 */
	RContainer<ResultItem,true,false> Res1;

	/**
	 * Results for a second query.
	 */
	RContainer<ResultItem,true,false> Res2;

	/**
	 *	Additional constraints ?
    */
	bool Add;

public:

	/**
	 * Constructor.
	 * @param add             Add some constraints to the problem ?
    */
	TestRetrieval(bool add);
	
	/**
	 * Toggle the status of the additional constraints.
	 */
	void ToggleAdd(void) {Add=!Add;}

	/**
	 * Set the scores of a document for the different courses.
    * @param docid           Identifier of the document.
    * @param ...             The scores.
    */
	void SetScore(size_t docid,...);

	/**
	 * Initialize the problem. In practice, it sets the scores and defines the
	 * two query results.
    */
	virtual void Init(void);

	/**
	 * Add eventually some constraints between the criteria.
    * @param home            Space to optimize.
    */
	virtual void AddConstraints(ROptimizeChoquet& home);

	/**
	 * Compute the F measure at the third position for a given query result.
    * @param cont            Documents retrieved in descending order of their
	 * rank
    * @return the F measure.
    */
	double ComputeF3(RContainer<ResultItem,true,false>& cont) const;

	/**
	 * Compute the average F measure at the third position for a given solution.
    * @param prop            Solution to evaluate.
    * @return a measure in [0,+1].
    */
	virtual double Compute(RChoquetPropagator& prop);
};


//-----------------------------------------------------------------------------
#endif
