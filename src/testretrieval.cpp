/*

	TestRetrieval.cpp

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
// include files for current project
#include <testretrieval.h>


//------------------------------------------------------------------------------
//
// ResultItem
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ResultItem::ResultItem(size_t docid,bool rel)
	: DocId(docid), Relevant(rel), Rank(0)
{
}


//------------------------------------------------------------------------------
int ResultItem::Compare(const ResultItem& res) const
{
	if(Rank==res.Rank)
		return(0);
	else if(Rank>res.Rank)
		return(-1);
	else
		return(1);
}



//------------------------------------------------------------------------------
//
// TestRetrieval
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TestRetrieval::TestRetrieval(bool add)
	: RTestChoquet(1), NbDocs(7), NbCriteria(4), Scores(NbDocs,NbCriteria),
	  Res1(5), Res2(5), Add(add)
{
	SetNbCriteria(0,NbCriteria);
}


//------------------------------------------------------------------------------
void TestRetrieval::SetScore(size_t docid,...)
{
	va_list ap;
	va_start(ap,docid);
	for(size_t i=0;i<NbCriteria;i++)
	{
		double note(va_arg(ap,double));
		Scores(docid,i)=note;
	}
	va_end(ap);
}


//------------------------------------------------------------------------------
void TestRetrieval::Init(void)
{
	// Set the scores for the documents
	SetScore(0,0.4,0.3,0.9,0.03);
	SetScore(1,0.5,0.6,0.5,0);
	SetScore(2,0.8,0.3,0.1,0.02);
	SetScore(3,0.3,0.7,0.8,0.01);
	SetScore(4,0.2,0.1,0.3,0.1);
	SetScore(5,0.6,0.4,0.4,0.1);
	SetScore(6,0.4,0.5,0.6,0);

	// Set the results
	Res1.InsertPtr(new ResultItem(0,true));
	Res1.InsertPtr(new ResultItem(1,true));
	Res1.InsertPtr(new ResultItem(2,false));
	Res1.InsertPtr(new ResultItem(3,true));
	Res1.InsertPtr(new ResultItem(4,false));
	Res2.InsertPtr(new ResultItem(0,true));
	Res2.InsertPtr(new ResultItem(3,false));
	Res2.InsertPtr(new ResultItem(4,false));
	Res2.InsertPtr(new ResultItem(5,true));
	Res2.InsertPtr(new ResultItem(6,true));
}


//------------------------------------------------------------------------------
void TestRetrieval::AddConstraints(ROptimizeChoquet& home)
{
	if(!Add) return;
	if(!Add) return;
	rel(home,home.vi(0,0,0)>home.vi(0,1,0));   // vr>vf
	rel(home,home.vi(0,0,0)>home.vi(0,2,0));   // vr>vn
	rel(home,home.vi(0,0,0)>home.vi(0,3,0));   // vr>vh
}


//------------------------------------------------------------------------------
double TestRetrieval::ComputeF3(RContainer<ResultItem,true,false>& cont) const
{
	double Recall(0.0);     // Recall at position 3
	double Precision(0.0);  // Precision at position 3;

	RCursor<ResultItem> Cur(cont);
	size_t Pos;
	for(Cur.Start(),Pos=0;(!Cur.End())&&(Pos<3);Cur.Next(),Pos++)
	{
		if(!Cur()->Relevant)
			continue;

		// Compute the number of relevant document in the first 3 positions
		if(Pos<3)
			Precision+=1.0;

		// Compute the number of relevant documents
		Recall+=1.0;
	}
	Recall=Precision/Recall;
	Precision/=3.0;

	return((2*Recall*Precision)/(Recall+Precision));
}


//------------------------------------------------------------------------------
double TestRetrieval::Compute(RChoquetPropagator& prop)
{
	// Compute the ranks
	RVector Ranks(NbDocs);
	for(size_t i=0;i<NbDocs;i++)
		Ranks[i]=prop.ComputeChoquet(0,*Scores[i]);

	// Assign the ranks and reorder the containers
	RCursor<ResultItem> Cur(Res1);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Rank=Ranks[Cur()->DocId];
	Res1.ReOrder();
	Cur.Set(Res2);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Rank=Ranks[Cur()->DocId];
	Res2.ReOrder();

	// Compute the average of the F measure at position 3
	return((ComputeF3(Res1)+ComputeF3(Res2))/2.0);
}
