//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Tvlakno_sound.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSound::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSound::TSound(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TSound::Execute()
{
	switch(A)
	{
		case -1: PlaySound(L"Z0.wav", NULL,SND_ASYNC | SND_FILENAME | SND_NODEFAULT);break;
		case 0:  Beep(800,500);break;
		case 1:  PlaySound(L"Z1.wav", NULL,SND_ASYNC | SND_FILENAME | SND_NODEFAULT);break;
	}
	Terminate();
}
//---------------------------------------------------------------------------
