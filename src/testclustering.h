/*

	TestClustering.h

	Parametrize the Choquet integral for a clustering problem - Header.

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
#ifndef TestClustering_H
#define TestClustering_H


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <cstdarg>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmatrix.h>
#include <rlowertriangularmatrix.h>
#include <rtestchoquet.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
/**
 * The TestClustering class provides a tester for a Choquet integral applied to
 * a clustering problem.
 * @short Clustering Tester.
 */
class TestClustering : public RTestChoquet
{
	/**
	 * Number of students.
	 */
	size_t NbStudents;

	/**
	 * Number of courses followed by the students.
	 */
	size_t NbCourses;

	/**
	 * Matrix containing the average intra- (first line) and inter-cluster
	 * (second line) similarities.
	 */
	RMatrix AvgSim;

	/**
	 * Matrix containing the minimum intra- (first line) and inter-cluster
	 * (second line) similarity for each pair of criteria.
	 */
	RContainer< RLowerTriangularMatrix,true,false> MinSim;

	/**
	 * Matrix containing the maximum intra- (first line) and inter-cluster
	 * (second line) similarity for each pair of criteria.
	 */
	RContainer< RLowerTriangularMatrix,true,false> MaxSim;

	/**
	 * Should the similarity matrix be printed.
	 */
	bool PrintSims;

	/**
	 * Should the average, minimum and maximum matrices be printed.
	 */
	bool PrintConsolidate;

public:

	/**
	 * Constructor.
    * @param sims            Print the similarity matrix?
    * @param consolidate     Print the average, minimum and maximum matrices?
    */
	TestClustering(bool sims,bool consolidate);

	/**
	 * Verify if two students are in the same group.
    * @param i               Identifier of the first student.
    * @param j               Identifier of the second student.
    * @return true if they are in the same group.
    */
	bool SameGrp(size_t i,size_t j);

	/**
	 * Compute an agreement ratio between scores given by two students for a
	 * given course.
    * @param score1          First score.
    * @param score2          Second score.
    * @return a similarity.
    */
	double SimScores(double score1,double score2);

	/**
	 * Set the scores of a student for the different courses.
    * @param idx             Identifier of the student.
    * @param scores          Matrix of the scores.
    * @param ...             The scores.
    */
	void SetScore(size_t idx,RMatrix& scores,...);

	/**
	 * Initialize the problem. First, the scores are set. Secondly, the different
	 * similarity matrices are computed.
    */
	virtual void Init(void);

	/**
	 * Verify that a given similarity is in [0,+1].
    * @param sim             Similarity to test.
    * @return the similarity tested.
    */
	double Sim(double sim) const;

	/**
	 * Compute a ratio to maximize between the intra- and inter-cluster
	 * similarities for a given solution.
    * @param prop            Solution to evaluate.
    * @return a ratio in [-1,+1].
    */
	virtual double Compute(RChoquetPropagator& prop);
};


//-----------------------------------------------------------------------------
#endif

