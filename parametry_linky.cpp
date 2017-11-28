//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "parametry_linky.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scExtControls"
#pragma link "scHtmlControls"
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
	Form1->m.designButton(Button_save,Form_parametry_linky,1,2);
	Form1->m.designButton(Button_storno,Form_parametry_linky,2,2);


}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{

		if(Form1->d.v.OBJEKTY->dalsi==NULL){
			scGPButton_doporucene->Visible=false;
			rHTMLLabel_doporuc_pohony->Caption=""; // neexistuj� ��dn� objekty -> neum�m spo��tat doporu�. rychlosti
		}
		else {

				scGPButton_doporucene->Visible=true;
				rHTMLLabel_doporuc_pohony->Caption="Doporu�en� rychlosti pohon�";
		}

		if(Form1->STATUS==Form1->NAVRH)    //Architekt
		{
			scGPButton_vozik->Caption="   Jig";
			//scGPButton_obecne->Caption="   Takt time";
			rHTMLLabel_takt->Visible=true;
			rEditNum_takt->Visible=true;
			rHTMLLabel_sirka->Visible=true;
			rEditNum_sirkavoziku->Visible=true;

			rStringGridEd_tab_dopravniky->Visible=true;
			Button_ADD->Visible=true;
			Button_DEL->Visible=true;

			scRadioGroup_typVoziku->Visible=true;
			rEditNum_delkavoziku->Visible=true;
			rHTMLLabel_delka->Visible=true;


		}
		else          //Klient
		{
			scGPButton_vozik->Caption="   Voz�k";
			scGPButton_pohon->Caption="   Pohon";

			rHTMLLabel_sirka->Visible=false;
			rEditNum_sirkavoziku->Visible=false;

			rStringGridEd_tab_dopravniky->Visible=true;
			Button_ADD->Visible=true;
			Button_DEL->Visible=true;

			scRadioGroup_typVoziku->Visible=true;
			rEditNum_delkavoziku->Visible=true;
			rHTMLLabel_delka->Visible=true;

			scGPButton_obecne->Visible=false;
			rHTMLLabel_takt->Visible=false;
			rEditNum_takt->Visible=false;


		}

		nacti_pohony();

		if(!data_nalezena)
		{
		rStringGridEd_tab_dopravniky->RowCount=3;    //defaultn� po�et ��dk� - hlavi�ka, hl.dopravn�k,vedl.dopravn�k
		}

		//	Form1->d.v.vymaz_seznam_POHONY();

	 Form_parametry_linky->Color=(TColor)RGB(240,240,240);//RGB(43,87,154);
	 rStringGridEd_tab_dopravniky->Columns->Items[0]->Visible=false;
	 scHTMLLabel_doporuc_pohony->Color=(TColor)RGB(230,230,230);


	 scGPButton_vozik->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_vozik->Options->FramePressedColor=Form_parametry_linky->Color;

	 scGPButton_pohon->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_pohon->Options->FramePressedColor=Form_parametry_linky->Color;

	 scGPButton_obecne->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_obecne->Options->FramePressedColor=Form_parametry_linky->Color;

	 rStringGridEd_tab_dopravniky->Left=1;
	 rStringGridEd_tab_dopravniky->Width=Form_parametry_linky->Width-2;


	 scGPButton_doporucene->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_doporucene->Options->FrameNormalColor=Form_parametry_linky->Color;



	 //nahr�n� hodnot
	 rEditNum_delkavoziku->Text=Form1->d.v.PP.delka_voziku;
	 rEditNum_sirkavoziku->Text=Form1->d.v.PP.sirka_voziku;
	 scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;
	 rEditNum_takt->Text=Form1->d.v.PP.TT;
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::nacti_pohony (){

   data_nalezena=false;
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

	 if (ukaz!=NULL) {

				rStringGridEd_tab_dopravniky->RowCount = Form1->d.v.POHONY->predchozi->n + 1;
				data_nalezena=true; //pokud jsou ve spojaku nejaka data, nastavit na true
				 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)  {

				 //ShowMessage(ukaz->n);
			rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
			rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;
			rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od;
			rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do;
			rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->roztec;

				ukaz = ukaz->dalsi;
				 }

	}
	else {  //pokud je spoj�k pr�zdn�, zobraz�m tyto p�ednastaven� hodnoty

	data_nalezena=false; // default se nepou��v� resp. po soubor nov� se okam�it� vlo�� do spoj�ku def.pohon
//na��t� se stejn� z unit1
//   rStringGridEd_tab_dopravniky->Cells[0][1]="1";
//	 rStringGridEd_tab_dopravniky->Cells[1][1]="Hlavn� dopravn�k";
//	 rStringGridEd_tab_dopravniky->Cells[2][1]="";
//	 rStringGridEd_tab_dopravniky->Cells[3][1]="";
//	 rStringGridEd_tab_dopravniky->Cells[4][1]="";
	}
}


void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{

	Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::KonecClick(TObject *Sender)
{
	Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_saveClick(TObject *Sender)
{
		Form1->d.v.vymaz_seznam_POHONY();
		Form1->d.v.hlavicka_POHONY();

		for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
		{
				 Form1->d.v.vloz_pohon (rStringGridEd_tab_dopravniky->Cells[1][i], //nazev
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]),        //rychlost od
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]),    //rychlost do
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]));      //roztec
		}

		Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
		Form1->d.v.PP.sirka_voziku=Form1->ms.MyToDouble(rEditNum_sirkavoziku->Text);
		Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(scRadioGroup_typVoziku->ItemIndex);
		Form1->d.v.PP.TT=Form1->ms.MyToDouble(rEditNum_takt->Text);

		Form1->DuvodUlozit(true);
		Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_ADDClick(TObject *Sender)
{
	rStringGridEd_tab_dopravniky->RowCount++;
	rStringGridEd_tab_dopravniky->Cols[0]->Add(rStringGridEd_tab_dopravniky->RowCount - 1);

	if (rStringGridEd_tab_dopravniky->RowCount > 2) {

		int i = rStringGridEd_tab_dopravniky->RowCount - 1;

		rStringGridEd_tab_dopravniky->Cells[0][i] = i;
		rStringGridEd_tab_dopravniky->Cells[1][i] = rStringGridEd_tab_dopravniky->Cells[1][i - 1];
		rStringGridEd_tab_dopravniky->Cells[2][i] = rStringGridEd_tab_dopravniky->Cells[2][i - 1];
		rStringGridEd_tab_dopravniky->Cells[3][i] = rStringGridEd_tab_dopravniky->Cells[3][i - 1];
		rStringGridEd_tab_dopravniky->Cells[4][i] = rStringGridEd_tab_dopravniky->Cells[4][i - 1];

	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::Button_DELClick(TObject *Sender)
{
			//	rStringGridEd_tab_dopravniky->RowCount - 1;
				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

				if(rStringGridEd_tab_dopravniky->RowCount>=3)
				{
				 rStringGridEd_tab_dopravniky->RowCount--;

				 }
				 else {

				 Form1->MB("Linka mus� obsahovat alespo� jeden pohon.");
				 }

	 //	for (long i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
	 //	rStringGridEd_tab_dopravniky->Cells[0][i] = i;

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Vypis_pohonyClick(TObject *Sender)
{
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

				 while (ukaz!=NULL) {

				 ShowMessage(ukaz->n);
				 ShowMessage(ukaz->name);
				 ShowMessage(ukaz->roztec);

				 	ukaz = ukaz->dalsi;

				 }
}
//---------------------------------------------------------------------------




void __fastcall TForm_parametry_linky::scGPButton_doporuceneClick(TObject *Sender)

{

		scExPanel_doporuc_pohony->Visible=true;
		scHTMLLabel_doporuc_pohony->Caption=Form1->d.v.navrhni_POHONY();

}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::scExPanel_doporuc_pohonyClose(TObject *Sender)

{
scExPanel_doporuc_pohony->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_taktChange(TObject *Sender)
{
 //	ShowMessage("Se zm�nou taktu linky a n�sledn�m ulo�en� budou nov� p�epo��t�ny parametry objekt�");
}
//---------------------------------------------------------------------------

