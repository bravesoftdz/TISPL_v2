//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Tvlakno_obraz.h"
#include "unit1.h"
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
//	for(i=0;!Terminated && i<1000000;i++) //for jen pro example
//	{
//		Synchronize(&test);  // nutne volani odkazem
//		Sleep(2000);
//	}
	F->d.v.vlakno_obraz();//provádí operace s obrazem na vláknì
}
//---------------------------------------------------------------------------
void __fastcall Tvlakno_obraz::test()
{
	F->d.v.vlakno_obraz();//provádí operace s obrazem na vláknì
}
