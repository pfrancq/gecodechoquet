/*
 *   Copyright (C) 2013 by Pascal Francq                                   *
 *   pascal@francq.info                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <cstdarg>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rapplication.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for current project
#include <testclustering.h>
#include <testretrieval.h>



//------------------------------------------------------------------------------
//
// class TestChoquet
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class TestChoquet : public RApplication
{
	/**
	 * Output file.
	 */
	RTextFile* Output;

	/**
	 * Precision
    */
	int Precision;

	/**
	 * Additional constraints ?
    */
	bool Add;

	/**
	 * Retrieval Task ?
    */
	bool RetrievalTask;

	/**
	 * Number of runs.
    */
	size_t NbRuns;

public:
	TestChoquet(int argc, char** argv);
	virtual void Run(void);
	~TestChoquet(void);
};


//------------------------------------------------------------------------------
TestChoquet::TestChoquet(int argc, char** argv)
	: RApplication("TestChoquet",argc,argv), Output(0)
{
	RString Val;
	if(GetParamValue("o",Val))
	{
		Output=new RTextFile(Val);
		Output->Open(RIO::Append);
	}
	if(GetParamValue("p",Val))
		Precision=Val.ToInt();
	else
		Precision=1;
	Add=GetParamValue("a",Val);
	RetrievalTask=GetParamValue("r",Val);
}


//------------------------------------------------------------------------------
void TestChoquet::Run(void)
{
	if(RetrievalTask)
	{
		TestRetrieval Retrieval(Add);
		Retrieval.Run(Precision);
		if(Add)
			Retrieval.Print("Retrieval task (constrained)",Output,true);
		else
			Retrieval.Print("Retrieval task",Output,true);
	}
	else
	{
		// Test Clustering
		TestClustering Clustering(Add,true,false);
		Clustering.Run(Precision);
		if(Add)
			Clustering.Print("Clustering task (constrained)",Output,true);
		else
			Clustering.Print("Clustering task",Output,true);
	}
}


//------------------------------------------------------------------------------
TestChoquet::~TestChoquet(void)
{
	if(Output)
	{
		delete Output;
		Output=0;
	}
}



//------------------------------------------------------------------------------
//
// Main
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	try
	{
		TestChoquet App(argc,argv);
		App.Execute();
	}
	catch(RException& e)
	{
		cout<<"Exception: "<<e.GetMsg()<<endl;
	}
	catch(exception& e)
	{
		cout<<"Standard C++: "<<e.what()<<endl;
	}
	catch(...)
	{
		cout<<"Unknown error"<<endl;
	}
	return(EXIT_SUCCESS);
}
