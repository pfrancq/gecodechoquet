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
}


//------------------------------------------------------------------------------
void TestChoquet::Run(void)
{
	// Test Clustering
	cout<<"Init Clustering"<<endl;
	TestClustering Clustering(false,false,false);
	for(int i=1;i<3;i++)
	{
		cout<<"  Run ("<<i<<")"<<endl;
		Clustering.Run(i);
		Clustering.Print("Clustering task",Output,Output==0);
		Clustering.ToggleAdd();
		cout<<"  Run ("<<i<<",constrained)"<<endl;
		Clustering.Run(i);
		Clustering.Print("Clustering task (constrained)",Output,Output==0);
	}

	// Retrieval Task
	cout<<"Init Retrieval"<<endl;
	TestRetrieval Retrieval(false);
	for(int i=1;i<3;i++)
	{
		cout<<"  Run ("<<i<<")"<<endl;
		Retrieval.Run(i);
		Retrieval.Print("Retrieval task",Output,Output==0);
		Retrieval.ToggleAdd();
		cout<<"  Run ("<<i<<",constrained)"<<endl;
		Retrieval.Run(i);
		Retrieval.Print("Retrieval task (constrained)",Output,Output==0);
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
