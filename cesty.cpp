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
}
//---------------------------------------------------------------------------

void __fastcall TForm_cesty::cesta_stornoClick(TObject *Sender)
{
	Form_cesty->Close();
}
//---------------------------------------------------------------------------

void TForm_cesty::nactiNastaveni (){

     	TColor light_gray=(TColor)RGB(240,240,240);
			TColor active_blue=(TColor)RGB(43,87,154);

		 Form_cesty->Color=light_gray;//RGB(43,87,154);
		 scGPButton2->Options->NormalColor=Form_cesty->Color;
		 scGPButton2->Options->FocusedColor=Form_cesty->Color;
		 scGPButton2->Options->HotColor=Form_cesty->Color;
		 scGPButton2->Options->PressedColor=Form_cesty->Color;
		 scGPButton2->Options->FrameNormalColor=Form_cesty->Color;



	Form_definice_zakazek->Color=light_gray;


}

void __fastcall TForm_cesty::FormShow(TObject *Sender)
{

	nactiNastaveni();
//
			// rStringGridEd_cesty->Columns[1].
		//	rStringGridEd_cesty->Columns[1].Items->PickList->Add('kuk');
		//rStringGridEd_cesty->Columns[1].Items->PickList->Add("hu");

		//  rStringGridEd_cesty->Columns[1][2]->PickList->Add("HOJ");

					rStringGridEd_cesty->BoolCells(1,2);


//zobraz data

	/*Cvektory::TObjekt *ukaz=Form1->d.v.OBJEKTY->dalsi;

	int i=0;

	while(ukaz!=NULL){
					i++;

			rStringGridEd_cesty->Cells[0][i]=ukaz->id;
			rStringGridEd_cesty->Cells[1][i]=ukaz->name;
			rStringGridEd_cesty->Cells[2][i]=ukaz->delka_dopravniku;


		 rStringGridEd_cesty->RowCount++;

		 ukaz=ukaz->dalsi;
     	}
   */



}
//---------------------------------------------------------------------------



void __fastcall TForm_cesty::vypis_objektyClick(TObject *Sender)
{
		Cvektory::TZakazka *zakazka=Form1->d.v.vrat_temp_zakazku(1);//inicializace
		Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//pøeskoèí hlavièku, jde rovnou na první segment cesty
		Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace

			while(ukaz!=NULL)
			{
				Memo1->Lines->Add
				(
						AnsiString(ukaz->n)+","+
						AnsiString(ukaz->objekt->short_name)+","+
						AnsiString(ukaz->CT)+","+
						AnsiString(ukaz->RD)+","+
						AnsiString(ukaz->Tc)+","+
						AnsiString(ukaz->Tv)
				);
				ukaz=ukaz->dalsi;
			}


}
//---------------------------------------------------------------------------




