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
#include <rmatrix.h>
#include <rlowertriangularmatrix.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for current project
#include <testclustering.h>
#include <testretrieval.h>



//------------------------------------------------------------------------------
void DoTestRetrieval(int pres)
{
	TestRetrieval Retrieval;
	Retrieval.Run(pres);
	Retrieval.Print();
}


//------------------------------------------------------------------------------
void DoTestClustering(int pres,bool sims,bool consolidate)
{
	TestClustering Clustering(sims,consolidate);
	Clustering.Run(pres);
	Clustering.Print();
}


//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	try
	{
		setlocale(LC_ALL, "");
		DoTestClustering(1,true,false);
		//DoTestRetrieval(1);
		cout<<"OK"<<endl;
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
	cout<<"Finish"<<endl;
	return EXIT_SUCCESS;
}
