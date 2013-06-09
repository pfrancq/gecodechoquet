/*

	TestClustering.cpp

	Parametrize the Choquet integral for a clustering problem - Implementation.

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
// include files for project
#include <testclustering.h>
using namespace Gecode;
using namespace Gecode::Int;



//------------------------------------------------------------------------------
//
// TestClustering
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TestClustering::TestClustering(bool add,bool sims,bool consolidate)
	: RTestChoquet(1), NbCourses(4), AvgSim(2,4), MinSim(2), MaxSim(2), PrintSims(sims),
	  PrintConsolidate(consolidate), Add(add)
{
	SetNbCriteria(0,NbCourses);
	MinSim.InsertPtr(new RLowerTriangularMatrix(4));
	MinSim.InsertPtr(new RLowerTriangularMatrix(4));
	MaxSim.InsertPtr(new RLowerTriangularMatrix(4));
	MaxSim.InsertPtr(new RLowerTriangularMatrix(4));
}


//------------------------------------------------------------------------------
bool TestClustering::SameGrp(size_t i,size_t j)
{
	size_t Sep(NbStudents/2);
	return(((i<Sep)&&(j<Sep))||((i>=Sep)&&(j>=Sep)));
}


//------------------------------------------------------------------------------
double TestClustering::SimScores(double score1,double score2)
{
/*	if((note1<4.0)&&(note2<4.0))
		return(0.0);
	double Sim(1.0-(fabs(note1-note2)/10));*/
/*	double Sim;
	if(    (score1<=5.0)&&(score2<=5.0) ||   (score1>5.0)&&(score2>5.0)   )
		Sim=((5.0-fabs(score1-score2))/10.0)+0.5;
	else
		Sim=(10.0-fabs(score1-score2))/20.0;
	return(Sim);*/
	double Sim;
	if( ((score1<5.0)&&(score2>=5.0)) || ((score1>=5.0)&&(score2<5.0)) )
		Sim=(10.0-fabs(score1-score2))/20.0;
	else if((score1>=5.0)&&(score2>=5.0))
		Sim=0.5+(score1+score2-10)/20.0;
	else
		Sim=0.0;
	return(Sim);
}


//------------------------------------------------------------------------------
void TestClustering::SetScore(size_t idx,RMatrix& scores,...)
{
	va_list ap;
	va_start(ap,scores);
	for(size_t i=0;i<NbCourses;i++)
	{
		int note(va_arg(ap,int));
		scores(idx,i)=note;
	}
	va_end(ap);
}


//------------------------------------------------------------------------------
void TestClustering::Init(void)
{
	// Initilize the similarities
	NbStudents=8;
	RMatrix Scores(NbStudents,NbCourses);
	SetScore(0,Scores,7,7,7,3);
	SetScore(1,Scores,8,4,5,7);
	SetScore(2,Scores,4,7,4,4);
	SetScore(3,Scores,6,7,4,7);
	SetScore(4,Scores,6,3,7,6);
	SetScore(5,Scores,4,7,4,7);
	SetScore(6,Scores,5,3,3,3);
	SetScore(7,Scores,4,3,3,6);

	// Compute SimIntra and SimInter;
	AvgSim.Init(0.0);
	MinSim[0]->Init(0.0);
	MinSim[1]->Init(0.0);
	MaxSim[0]->Init(0.0);
	MaxSim[1]->Init(0.0);
	size_t NbIntra(0);
	size_t NbInter(0);
	double* Score=new double[NbCourses];
	for(size_t i=0;i<NbStudents-1;i++)
		for(size_t j=i+1;j<NbStudents;j++)
		{
			if(i==j)
				continue;
			size_t idx;
			if(SameGrp(i,j))
			{
				idx=0;
				NbInter++;
			}
			else
			{
				idx=1;
				NbIntra++;
			}

			// Compute Scores and AvgSim
			for(size_t l=0;l<NbCourses;l++)
			{
				Score[l]=SimScores(Scores(i,l),Scores(j,l));
				AvgSim(idx,l)+=Score[l];
			}

			// Compute Min and Max
			for(size_t l=0;l<NbCourses-1;l++)
				for(size_t m=l+1;m<NbCourses;m++)
				{
					(*MinSim[idx])(m,l)+=min(Score[l],Score[m]);
					(*MaxSim[idx])(m,l)+=max(Score[l],Score[m]);
				}
		}
	delete[] Score;
	(*AvgSim[0])/=NbInter;
	(*AvgSim[1])/=NbIntra;
	(*MinSim[0])/=NbInter;
	(*MaxSim[0])/=NbInter;
	(*MinSim[1])/=NbIntra;
	(*MaxSim[1])/=NbIntra;
	if(PrintSims)
	{
		for(size_t l=0;l<NbCourses;l++)
		{
			RMatrix Sim(NbStudents,NbStudents);
			for(size_t i=0;i<NbStudents;i++)
				for(size_t j=0;j<NbStudents;j++)
				{
					if(i==j)
						Sim(i,j)=1.0;
					else
						Sim(i,j)=SimScores(Scores(i,l),Scores(j,l));
				}
			Sim.Print(cout,"Sim"+RString::Number(l),"%1.4f",6);
		}
	}
	if(PrintConsolidate)
	{
		AvgSim.Print(cout,"AvgSim ");
		MinSim[0]->Print(cout,"MinSim0");
		MinSim[1]->Print(cout,"MinSim1");
		MaxSim[0]->Print(cout,"MaxSim0");
		MaxSim[1]->Print(cout,"MaxSim1");
	}
}


//------------------------------------------------------------------------------
double TestClustering::Sim(double sim) const
{
	if(fabs(sim)<1e-9)
		sim=0.0;
	if((fabs(sim)>=1) && (fabs(sim)<=1+1e-9))
		sim=1.0;

	if(sim<0.0)
	{
		cout<<"Problem with a similarity: "<<sim<<endl;
		sim=0.0;
	}
	else if(sim>1.0)
	{
		cout<<"Problem with a similarity: "<<sim<<endl;
		sim=1.0;
	}
	return(sim);
}


//------------------------------------------------------------------------------
void TestClustering::AddConstraints(ROptimizeChoquet& home)
{
	// Avoid multiple combinations giving the same cost function
	for(size_t i=0;i<3;i++)
		for(size_t j=i+1;j<4;j++)
		{
			BoolVar tmp[8];
			size_t idx(0);

			// Constraints on I_ij and v_i and v_j
			tmp[idx++]=expr(home,((home.Iij(0,i,j,0)!=0)&&(home.Iij(0,i,j,0)!=2)&&(2*home.vi(0,i,0)==home.Iij(0,i,j,0))&&(2*home.vi(0,j,0)==home.Iij(0,i,j,0))));

			// Constraints on v_n with n<>i and n<>j
			for(size_t n=0;n<4;n++)
			{
				if((n==i)||(n==j))
					continue;
				tmp[idx]=expr(home,tmp[idx-1]&&(home.vi(0,n,0)==0));
				idx++;
			}

			for(size_t l=0;l<3;l++)
			{
				for(size_t m=l+1;m<4;m++)
				{
					if((l==i)&&(m==j))
						continue;
					tmp[idx]=expr(home,tmp[idx-1]&&(home.Iij(0,l,m,0)==0));
						idx++;
				}
			}
			rel(home,!tmp[7]);
		}


	if(!Add) return;
	rel(home,home.vi(0,2,0)<=home.vi(0,0,0));   // ve<vm
	rel(home,home.vi(0,2,0)<=home.vi(0,1,0));   // ve<vp
	rel(home,home.vi(0,2,0)<=home.vi(0,3,0));   // ve<vl
}


//------------------------------------------------------------------------------
double TestClustering::Compute(RChoquetPropagator& prop)
{
	double Intra(0.0);
	double Inter(0.0);

	// Compute Min/Max
	for(size_t l=0;l<3;l++)
		for(size_t m=l+1;m<4;m++)
		{
			if(prop.Iij(0,m,l)>0.0)
			{
				Inter+=prop.Iij(0,m,l)*((*MinSim[0])(m,l));
				Intra+=prop.Iij(0,m,l)*((*MinSim[1])(m,l));
			}
			else if(prop.Iij(0,m,l)<0.0)
			{
				Inter-=prop.Iij(0,m,l)*((*MaxSim[0])(m,l));
				Intra-=prop.Iij(0,m,l)*((*MaxSim[1])(m,l));
			}
		}

	// Compute Criteria
	for(size_t j=0;j<4;j++)
	{
		double sum(0);
		for(size_t k=0;k<4;k++)
		{
			if(k==j)
				continue;
			sum+=abs(prop.Iij(0,j,k));
		}
		Intra+=AvgSim(0,j)*(prop.vi(0,j)-0.5*sum);
		Inter+=AvgSim(1,j)*(prop.vi(0,j)-0.5*sum);
	}

	// Make the final computation
	Intra=Sim(Intra);
	Inter=Sim(Inter);
	double Res;
	if(Intra>Inter)
	{
		if(Intra!=0.0)
			Res=(Intra-Inter)/Intra;
		else
			Res=0.1;
	}
	else
	{
		if(Inter!=0.0)
			Res=(Intra-Inter)/Inter;
		else
			Res=-1.0;
	}
	if(fabs(Res)>2.0)
	{
		cout<<"Problem Res="<<Res<<endl;
		cout<<"Debug"<<endl;
		if(Res>2.0)
			Res=2.0;
		else
			Res=-2.0;
	}
	return(Res);
}
