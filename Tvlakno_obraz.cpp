//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Tvlakno_obraz.h"
#include "vektory.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Tvlakno_obraz::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall Tvlakno_obraz::Tvlakno_obraz(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall Tvlakno_obraz::Execute()
{
	for(i=0;!Terminated && i<1000000;i++) //for jen pro example
	{
		Synchronize(&test);  // nutne volani odkazem
		Sleep(10);  //pro example
	}
}
//---------------------------------------------------------------------------
void __fastcall Tvlakno_obraz::test()
{
	Cvektory v;
	v.test_vlakna(i);    //volani metod
}
