/*

	R Project Library

	RChoquetPropagator.cpp

	Propagator for Choquet integrals - Implementation.

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
#include <rchoquetpropagator.h>
#include <rtestchoquet.h>
#include <rvector.h>
using namespace R;
using namespace Gecode;
using namespace Gecode::Int;



//-----------------------------------------------------------------------------
//
// class RChoquetPropagator
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RChoquetPropagator::RChoquetPropagator(Home home,RTestChoquet* test,IntVarArray& V,IntVarArray& i,IntView cost)
	: Propagator(home), Test(test), v(home,V.size()), I(home,i.size()),Cost(cost)
{
	for(size_t j=0;j<V.size();j++)
		v[j]=IntView(V[j]);
	for(size_t j=0;j<i.size();j++)
		I[j]=IntView(i[j]);
	v.subscribe(home,*this,PC_GEN_ASSIGNED);
	I.subscribe(home,*this,PC_GEN_ASSIGNED);
}


//-----------------------------------------------------------------------------
RChoquetPropagator::RChoquetPropagator(Space& home, bool share, RChoquetPropagator& p)
: Propagator(home,share,p), Test(p.Test), v(home,p.v.size()), I(home,p.I.size())
{
	v.update(home,share,p.v);
	I.update(home,share,p.I);
	Cost.update(home,share,p.Cost);
}


//-----------------------------------------------------------------------------
ExecStatus RChoquetPropagator::post(Home home,RTestChoquet* test,IntVarArray& V,IntVarArray& i,IntView cost)
{
	(void) new (home) RChoquetPropagator(home,test,V,i,cost);
	return ES_OK;
}


//-----------------------------------------------------------------------------
size_t RChoquetPropagator::dispose(Space& home)
{
	v.cancel(home,*this,PC_GEN_ASSIGNED);
	I.cancel(home,*this,PC_GEN_ASSIGNED);
	(void) Propagator::dispose(home);
	return sizeof(*this);
}


//-----------------------------------------------------------------------------
Propagator* RChoquetPropagator::copy(Space& home, bool share)
{
	return new (home) RChoquetPropagator(home,share,*this);
}


//-----------------------------------------------------------------------------
PropCost RChoquetPropagator::cost(const Space&, const ModEventDelta&) const
{
	return PropCost::binary(PropCost::HI);
}


//-----------------------------------------------------------------------------
ExecStatus RChoquetPropagator::propagate(Space& home, const ModEventDelta&)
{
	if((!v.assigned())||(!I.assigned()))
		return (ES_NOFIX);
	double Res(Test->Compute(*this));
	if(Res<-1.0||Res>1.0)
		throw std::range_error("Cost function must be in [-1,1]");
	if(Cost.eq(home,static_cast<long long int>(Res*Test->Precision))==Int::ME_INT_FAILED)
		return ES_FAILED;
	return home.ES_SUBSUMED(*this);
}


//-----------------------------------------------------------------------------
double RChoquetPropagator::vi(size_t id,size_t i) const
{
	return(static_cast<double>(v[Test->GetviId(id,i)].val())/static_cast<double>(Test->Max));
}


//-----------------------------------------------------------------------------
double RChoquetPropagator::Iij(size_t id,size_t i,size_t j) const
{
	return(static_cast<double>(I[Test->GetIijId(id,i,j)].val())/static_cast<double>(Test->Max));
}


//-----------------------------------------------------------------------------
double RChoquetPropagator::ComputeChoquet(size_t choquet,RVector& vec) const
{
	size_t NbCriteria(Test->GetNbCriteria(choquet));
	if(vec.GetNb()!=NbCriteria)
	{
		char str[200];
		sprintf(str,"RChoquetPropagator::ComputeChoquet : The size of the vector for the Choquet integral %ld must be %ld",choquet,NbCriteria);
		throw std::invalid_argument(str);
	}

	double Choquet(0.0);

	// Go first through each interaction
	for(size_t i=0;i<NbCriteria-1;i++)
		for(size_t j=i+1;j<NbCriteria;j++)
	{
			if(Iij(choquet,i,j)>0)
				Choquet+=Iij(choquet,i,j)*fmin(vec[i],vec[j]);
			else
				Choquet+=fabs(Iij(choquet,i,j))*fmax(vec[i],vec[j]);
	}

	// Go trough each criteria
	for(size_t i=0;i<NbCriteria;i++)
	{
		double tmp(0);
		for(size_t j=0;j<NbCriteria;j++)
			if(i!=j)
				tmp+=fabs(Iij(choquet,i,j));
		Choquet+=(vec[i]*(vi(choquet,i)-0.5*tmp));
	}

	return(Choquet);

}


//-----------------------------------------------------------------------------
RChoquetPropagator::~RChoquetPropagator(void)
{
}
