//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gapoV.h"
#include "unit1.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TF_gapoV *F_gapoV;
//---------------------------------------------------------------------------
__fastcall TF_gapoV::TF_gapoV(TComponent* Owner)
	: TForm(Owner)
{
	//nastaven� barvy formul��e
	F_gapoTT->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,F_gapoTT,1,1);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky
}
//---------------------------------------------------------------------------
