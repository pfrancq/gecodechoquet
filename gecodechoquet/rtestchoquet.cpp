/*

	GecodeChoquet Library

	RTestChoquet.cpp

	Test the parameters for Choquet integrals - Implementation.

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
// include files for ANSI C/C++
#include <cstdio>
#include <stdexcept>


//-----------------------------------------------------------------------------
// include files for GeCode
#include <rtestchoquet.h>
using namespace R;
using namespace Gecode;
using namespace Gecode::Int;



//-----------------------------------------------------------------------------
//
// struct RTestChoquet::Choquet
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct RTestChoquet::Choquet
{
	size_t NbCriteria;
	size_t vIdx;
	size_t IIdx;
	Choquet(void) : NbCriteria(0), vIdx(0), IIdx(0) {}
};



//-----------------------------------------------------------------------------
//
// class RTestChoquet
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTestChoquet::RTestChoquet(size_t nbchoquets)
	: NbChoquets(nbchoquets), Precision(0), Domain(0), Choquets(0), MustInit(true), CurSol(0), Search(0)
{
	Choquets=new Choquet[NbChoquets];
}


//------------------------------------------------------------------------------
void RTestChoquet::SetNbCriteria(size_t choquet,size_t nb)
{
	if(choquet>=NbChoquets)
	{
		char str[200];
		sprintf(str,"TestChoquet::SetNbCriteria : Choquet index must be in [0,%ld]",NbChoquets-1);
		throw std::invalid_argument(str);
	}
	Choquets[choquet].NbCriteria=nb;
}


//------------------------------------------------------------------------------
size_t RTestChoquet::GetNbCriteria(size_t choquet) const
{
	if(choquet>=NbChoquets)
	{
		char str[200];
		sprintf(str,"TestChoquet::SetNbCriteria : Choquet index must be in [0,%ld]",NbChoquets-1);
		throw std::invalid_argument(str);
	}
	return(Choquets[choquet].NbCriteria);
}


//------------------------------------------------------------------------------
size_t RTestChoquet::GetviId(size_t id,size_t i) const
{
	if(id>=NbChoquets)
	{
		char str[200];
		sprintf(str,"TestChoquet::vi : Choquet index must be in [0,%ld]",NbChoquets-1);
		throw std::invalid_argument(str);
	}
	const Choquet& integral(Choquets[id]);
	if(i>=integral.NbCriteria)
	{
		char str[200];
		sprintf(str,"TestChoquet::vi : vi index must be in [0,%ld]",Choquets[id].NbCriteria);
		throw std::invalid_argument(str);
	}
	size_t idx(integral.vIdx+i);
	return(idx);
}


//------------------------------------------------------------------------------
size_t RTestChoquet::GetIijId(size_t id,size_t i,size_t j) const
{
	if(id>=NbChoquets)
	{
		char str[200];
		sprintf(str,"TestChoquet::SetNbCriteria : Choquet index must be in [0,%ld]",NbChoquets-1);
		throw std::invalid_argument(str);
	}
	const Choquet& integral(Choquets[id]);
	if(i>=integral.NbCriteria)
	{
		char str[200];
		sprintf(str,"TestChoquet::vi : vi index must be in [0,%ld]",Choquets[id].NbCriteria);
		throw std::invalid_argument(str);
	}
	if(j>=integral.NbCriteria)
	{
		char str[200];
		sprintf(str,"TestChoquet::vi : vi index must be in [0,%ld]",Choquets[id].NbCriteria);
		throw std::invalid_argument(str);
	}
	size_t idx(integral.IIdx);
	if(i>j)
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}
	for(size_t k=0;k<i;k++)
		idx+=(4-k-1);
	idx=idx+j-i-1;
	return(idx);
}


//------------------------------------------------------------------------------
ROptimizeChoquet* RTestChoquet::GetBestSol(void) const
{
	return(CurSol);
}


//------------------------------------------------------------------------------
double RTestChoquet::GetComputedTime(void) const
{
	return(ElapsedSecs);
}


//------------------------------------------------------------------------------
size_t RTestChoquet::GetNbNodesEvaluated(void) const
{
	if(!Search)
		return(0);
	return(Search->statistics().node);
}


//------------------------------------------------------------------------------
void RTestChoquet::Init(void)
{

}


//------------------------------------------------------------------------------
void RTestChoquet::AddConstraints(ROptimizeChoquet&)
{

}


//------------------------------------------------------------------------------
void RTestChoquet::Run(int dom,int precision)
{
	Domain=dom;
	Max=std::pow(10,Domain);
	Precision=std::pow(10,precision);
	if(MustInit)
	{
		// Compute the number of parameters
		Nbv=0;
		NbI=0;
		for(size_t i=0;i<NbChoquets;i++)
		{
			Choquets[i].vIdx=Nbv;
			Nbv+=Choquets[i].NbCriteria;
			Choquets[i].IIdx=NbI;
			NbI+=((Choquets[i].NbCriteria*Choquets[i].NbCriteria-1)/2)-1;
		}

		Init();
		MustInit=false;
	}
	if(CurSol)
	{
		delete CurSol;
		CurSol=0;
	}
	if(Search)
	{
		delete Search;
		Search=0;
	}

	clock_t begin = clock();
	ROptimizeChoquet* m(new ROptimizeChoquet(this,Nbv,NbI,Max,Precision));
	Search=new BAB<ROptimizeChoquet>(m);
	delete m;
	while(ROptimizeChoquet* s=Search->next())
	{
		if(CurSol)
			delete CurSol;
		CurSol=s;
	}
	clock_t end = clock();

	// Print solution
	ElapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
}


//------------------------------------------------------------------------------
void RTestChoquet::Print(void) const
{
	if((!CurSol)||(!Search))
		return;
	std::cout<<"Cost="<<static_cast<double>(CurSol->Cost.val())/static_cast<double>(Precision)<<std::endl;
	for(size_t c=0;c<NbChoquets;c++)
	{
		std::cout<<"  Choquet "<<c<<std::endl<<"  ";

		for(size_t i=0;i<4;i++)
		{
			char tmp[6];
			sprintf(tmp,"%1.4f",CurSol->vi(c,i));
			std::cout<<"  v"<<i+1<<"="<<tmp<<"  ";
		}
		std::cout<<std::endl;
		for(size_t i=0;i<Choquets[c].NbCriteria-1;i++)
		{
			std::cout<<"  ";
			for(size_t j=i+1;j<Choquets[c].NbCriteria;j++)
			{
				char tmp[6];
				sprintf(tmp,"%1.4f",CurSol->Iij(c,i,j));
				std::cout<<"  I"<<i+1<<j+1<<"="<<tmp<<" ";
			}
			std::cout<<std::endl;
		}
	}
	std::cout<<ElapsedSecs<<" sec ("<<Search->statistics().node<<" nodes expanded)"<<std::endl<<std::endl;
}


//------------------------------------------------------------------------------
RTestChoquet::~RTestChoquet(void)
{
	if(Choquets)
	{
		delete[] Choquets;
		Choquets=0;
	}
	if(Search)
	{
		delete Search;
		Search=0;
	}
	if(CurSol)
	{
		delete CurSol;
		CurSol=0;
	}
}
