//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "cesty.h"
#include "superform.h"
#include "unit1.h"
//#include "dopravniky.h"
#include "jig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rStringGridEd"
#pragma link "RzButton"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_cesty *Form_cesty;
//---------------------------------------------------------------------------
__fastcall TForm_cesty::TForm_cesty(TComponent* Owner)
	: TForm(Owner)
{
	nactiNastaveni();
	CasoveJednotky=S;



}
//---------------------------------------------------------------------------
void TForm_cesty::nactiNastaveni ()
{
	TColor light_gray=(TColor)RGB(240,240,240);
	//TColor active_blue=(TColor)RGB(43,87,154);

	Form_cesty->Color=light_gray;//RGB(43,87,154);
	scGPButton2->Options->NormalColor=Form_cesty->Color;
	scGPButton2->Options->FocusedColor=Form_cesty->Color;
	scGPButton2->Options->HotColor=Form_cesty->Color;
	scGPButton2->Options->PressedColor=Form_cesty->Color;
	scGPButton2->Options->FrameNormalColor=Form_cesty->Color;

	Form_definice_zakazek->Color=light_gray;

	Form1->m.designButton(Button_ulozit,Form_cesty,1,2);
	Form1->m.designButton(cesta_storno,Form_cesty,2,2);

}
//---------------------------------------------------------------------------
void __fastcall TForm_cesty::FormShow(TObject *Sender)
{

//cinnost obsluhuje superform.cpp

	//rStringGridEd_cesty->Columns->Items[2]->ReadOnly=true;
	//rStringGridEd_cesty->Columns->Items[2]->Color=(TColor)RGB(230,230,230);
}
//---------------------------------------------------------------------------
void __fastcall TForm_cesty::vypis_objektyClick(TObject *Sender)
{
		Cvektory::TZakazka *zakazka=Form1->d.v.vrat_temp_zakazku(1);//inicializace
		Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//p�esko�� hlavi�ku, jde rovnou na prvn� segment cesty
		//Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace

		while(ukaz!=NULL)
		{
			Memo1->Lines->Add
			(
					AnsiString(ukaz->n)+","+
					AnsiString(ukaz->objekt->short_name)+","+
					AnsiString(ukaz->CT)+","+
					AnsiString(ukaz->RD)+","+
					AnsiString(ukaz->Tc)+","
			);
			ukaz=ukaz->dalsi;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_cesty::cesta_stornoClick(TObject *Sender)
{
	Form_cesty->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_cesty::KonecClick(TObject *Sender)
{
	Form_cesty->Close();
}
//---------------------------------------------------------------------------



void __fastcall TForm_cesty::scGPButton_min_secClick(TObject *Sender)
{

if (CasoveJednotky==S) {  //*60

		CasoveJednotky=MIN;
		rStringGridEd_cesty->Columns->Items[2]->TitleCaption="CT [min]";
		scGPButton_min_sec->Caption="v�e na sec";

		for(int i=1;i<=rStringGridEd_cesty->RowCount;i++){

		try{

				rStringGridEd_cesty->Cells[2][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[2][i]/60.0);

				}

			catch(...){;}  //p�etypov�n� string - double, do�e�it

		}

}

		else {

		 CasoveJednotky=S;
		 rStringGridEd_cesty->Columns->Items[2]->TitleCaption="CT [s]";
		 scGPButton_min_sec->Caption="v�e na min";

      	for(int i=1;i<=rStringGridEd_cesty->RowCount;i++){

				try{

					rStringGridEd_cesty->Cells[2][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[2][i]*60.0);

					}

			catch(...){;}

		}

	}


}
//---------------------------------------------------------------------------

