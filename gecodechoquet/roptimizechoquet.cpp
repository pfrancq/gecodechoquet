/*

	R Project Library

	ROptimizeChoquet.cpp

	Brand and Bound Search for the parameters of Choquer integrals - Implementation.

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
// include files for GeCode
#include <roptimizechoquet.h>
#include <rtestchoquet.h>
using namespace R;
using namespace Gecode;
using namespace Gecode::Int;
using namespace Gecode::Float;



//-----------------------------------------------------------------------------
void choquetPropagator(Home home,RTestChoquet* test,IntVarArray& v,IntVarArray& i,FloatView cost)
{
	if(home.failed())
		return;
	GECODE_ES_FAIL(RChoquetPropagator::post(home,test,v,i,cost));
}



//-----------------------------------------------------------------------------
//
// class ROptimizeChoquet
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ROptimizeChoquet::ROptimizeChoquet(RTestChoquet* test,size_t nbc,size_t nbi,int max)
	: Test(test), v(*this,nbc,0,max), I(*this,nbi,-max,max), Cost(*this,-1.0,1.0)
{
	// Maximal value <= 1
	rel(*this,max>=sum(v));

	// Minimal value >= 0
	for(size_t i=0;i<4;i++)
	{
		IntVar tmp[3];
		size_t idx(0);
		for(size_t j=0;j<4;j++)
		{
			if(i==j)
				continue;
			if(idx)
				tmp[idx]=expr(*this,tmp[idx-1]-abs(I[Test->GetIijId(0,i,j)]));
			else
				tmp[idx]=expr(*this,-abs(I[Test->GetIijId(0,i,j)]));
			idx++;
		}
		rel(*this, 0<=2*v[Test->GetviId(0,i)]+tmp[idx-1]);
	}

	// Add new constraints
	Test->AddConstraints(*this);

	// Choquet
	choquetPropagator(*this,Test,v,I,Cost);

	// Make the branch
	branch(*this,v,INT_VAR_SIZE_MIN(),INT_VAL_MIN());
	branch(*this,I,INT_VAR_SIZE_MIN(),INT_VAL_MIN());
}


//-----------------------------------------------------------------------------
ROptimizeChoquet::ROptimizeChoquet(bool share, ROptimizeChoquet& s)
	: FloatMaximizeSpace(share,s), Test(s.Test)
{
	v.update(*this,share,s.v);
	I.update(*this,share,s.I);
	Cost.update(*this,share,s.Cost);
}


//-----------------------------------------------------------------------------
Gecode::IntVar& ROptimizeChoquet::vi(size_t id,size_t i,size_t)
{
	return(v[Test->GetviId(id,i)]);
}


//-----------------------------------------------------------------------------
Gecode::IntVar& ROptimizeChoquet::Iij(size_t id,size_t i,size_t j,size_t)
{
	return(I[Test->GetIijId(id,i,j)]);
}


//-----------------------------------------------------------------------------
double ROptimizeChoquet::vi(size_t id,size_t i) const
{
	return(static_cast<double>(v[Test->GetviId(id,i)].val())/Test->Max);
}


//-----------------------------------------------------------------------------
double ROptimizeChoquet::Iij(size_t id,size_t i,size_t j) const
{
	return(static_cast<double>(I[Test->GetIijId(id,i,j)].val())/Test->Max);
}


//-----------------------------------------------------------------------------
double ROptimizeChoquet::GetCost(void) const
{
	return(Cost.med());
}


//-----------------------------------------------------------------------------
Space* ROptimizeChoquet::copy(bool share)
{
	return(new ROptimizeChoquet(share,*this));
}


//-----------------------------------------------------------------------------
FloatVar ROptimizeChoquet::cost(void) const
{
	return(Cost);
}


//-----------------------------------------------------------------------------
ROptimizeChoquet::~ROptimizeChoquet(void)
{
}
