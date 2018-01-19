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

	rStringGridEd_cesty->SetColumnAutoFit(0);
	rStringGridEd_cesty->SetColumnAutoFit(2);
	rStringGridEd_cesty->SetColumnAutoFit(3);
	rStringGridEd_cesty->SetColumnAutoFit(4);
	rStringGridEd_cesty->SetColumnAutoFit(5);
	rStringGridEd_cesty->SetColumnAutoFit(6);
	rStringGridEd_cesty->SetColumnAutoFit(7);

	Form_cesty->Width=rStringGridEd_cesty->Width;

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
		rStringGridEd_cesty->Columns->Items[4]->TitleCaption="�as v�m�ny barev [min]";
		rStringGridEd_cesty->Columns->Items[5]->TitleCaption="�as �i�t�n� [min]";
		scGPButton_min_sec->Caption="v�e na sec";

		for(int i=1;i<=rStringGridEd_cesty->RowCount;i++){

		try{

				rStringGridEd_cesty->Cells[2][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[2][i]/60.0);
				rStringGridEd_cesty->Cells[4][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[4][i]/60.0);
				rStringGridEd_cesty->Cells[5][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[5][i]/60.0);

				}

			catch(...){;}  //p�etypov�n� string - double, do�e�it

		}

}

		else {

		 CasoveJednotky=S;
		 rStringGridEd_cesty->Columns->Items[2]->TitleCaption="CT [s]";
		 rStringGridEd_cesty->Columns->Items[4]->TitleCaption="�as v�m�ny barev [s]";
		 rStringGridEd_cesty->Columns->Items[5]->TitleCaption="�as �i�t�n� [s]";

		 scGPButton_min_sec->Caption="v�e na min";

      	for(int i=1;i<=rStringGridEd_cesty->RowCount;i++){

				try{

					rStringGridEd_cesty->Cells[2][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[2][i]*60.0);
					rStringGridEd_cesty->Cells[4][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[4][i]*60.0);
					rStringGridEd_cesty->Cells[5][i]=Form1->ms.MyToDouble(rStringGridEd_cesty->Cells[5][i]*60.0);

					}

			catch(...){;}

		}

	}

	rStringGridEd_cesty->SetColumnAutoFit(0);
	rStringGridEd_cesty->SetColumnAutoFit(2);
	rStringGridEd_cesty->SetColumnAutoFit(3);
	rStringGridEd_cesty->SetColumnAutoFit(4);
	rStringGridEd_cesty->SetColumnAutoFit(5);
	rStringGridEd_cesty->SetColumnAutoFit(6);
	rStringGridEd_cesty->SetColumnAutoFit(7);


}
//---------------------------------------------------------------------------




void __fastcall TForm_cesty::rStringGridEd_cestyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
 if(Key==13)//ENTER
 {
		 if(Button_ulozit->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_cesty->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_cesty->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_cesty->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_cesty->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------

