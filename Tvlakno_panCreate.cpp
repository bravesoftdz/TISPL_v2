//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Tvlakno_panCreate.h"
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
//      void __fastcall vlakno_panCreate::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall vlakno_panCreate::vlakno_panCreate(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall vlakno_panCreate::Execute()
{
	F->pan_create2();
}
//---------------------------------------------------------------------------

