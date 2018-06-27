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
	F_gapoV->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,F_gapoV,1,1);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormActivate(TObject *Sender)
{
	//zde nevolat d�l� probl�my
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormShow(TObject *Sender)
{
 //
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormPaint(TObject *Sender)
{
 	mGrid->Show();//vykresl� tabulku
	F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoV::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoV::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoV::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_OKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
}
//---------------------------------------------------------------------------


