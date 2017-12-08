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

	Delkaunit=MM;
	Sirkaunit=Delkaunit;
	Taktunit=S;


}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{
		input_state=NOTHING;//nutnost
		scExPanel_doporuc_pohony->Visible=false;


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
			scGPButton_vozik->Caption="   Voz�k";
			scGPButton_vozik->ImageIndex=21;
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
			scGPButton_vozik->Caption="   Jig";
			scGPButton_vozik->ImageIndex=19;
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
		rStringGridEd_tab_dopravniky->RowCount=1;    //defaultn� po�et ��dk� - hlavi�ka
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


	 rStringGridEd_tab_dopravniky->Cells[1][0]="N�zev";
	 rStringGridEd_tab_dopravniky->Cells[2][0]="Rychlost od [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[3][0]="Rychlost do [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[4][0]="Rozte� [mm]";
	 rStringGridEd_tab_dopravniky->Cells[5][0]="Pou��v�n";

	//	rStringGridEd_tab_dopravniky->Columns->

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
			rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od*60;
			rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do*60;
			rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->roztec;

				ukaz = ukaz->dalsi;
				 }

	}
	else {  //pokud je spoj�k pr�zdn�, zobraz�m tyto p�ednastaven� hodnoty

	data_nalezena=false; // default se nepou��v� resp. po soubor nov� se okam�it� vlo�� do spoj�ku def.pohon
//nevytvari se zadny default pohon nikde
	}

}

void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{

		for(int i=1;i<=rStringGridEd_tab_dopravniky->RowCount;i++){
			rStringGridEd_tab_dopravniky->Rows[i]->Clear();
			rStringGridEd_tab_dopravniky->RowCount--;
			}
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
	bool Changes=false;
	bool Ulozit=true;


	//pri zmene TT
		if(Form1->ms.MyToDouble(rEditNum_takt->Text) != Form1->d.v.PP.TT && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		}
		//pri zmene delky voziku
		if(Form1->ms.MyToDouble(rEditNum_delkavoziku->Text) != Form1->d.v.PP.delka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		}
		//pri zmene sirky voziku
			if(Form1->ms.MyToDouble(rEditNum_sirkavoziku->Text) != Form1->d.v.PP.sirka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		}

		Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;
		 int i=1;
		while(ukaz!=NULL){

		if(Form1->ms.MyToDouble(ukaz->rychlost_od*60.0)!=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]))
		{  Changes=true;  }
		if(Form1->ms.MyToDouble(ukaz->rychlost_do*60.0)!=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]))
		{  Changes=true;  }
		if(Form1->ms.MyToDouble(ukaz->roztec)!=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]))
		{  Changes=true;  }
		 	i++;
			ukaz=ukaz->dalsi;
		}

		if(Changes){    //pri zmene + jiz existuje nejaky objekt

			if(mrOk==Form1->MB("Pozor, p�i zm�n� parametr� linky dojde k p�epo��t�n� parametr� objekt�.",MB_OKCANCEL))
			{  // OK souhlas se zmenou parametru
				Ulozit=true;
			}
			else { Ulozit=false;} // cancel - data nebudu ukladat
	}

			if (Form1->d.v.OBJEKTY->dalsi==NULL) {  // pokud neexistuje zadny objekt, vzdy dovolim delat zmeny a moznost ulozit
					Ulozit=true;
			}

		if (Ulozit) {  // ukladej

		Form1->d.v.vymaz_seznam_POHONY();
		Form1->d.v.hlavicka_POHONY();

			 if (Form1->d.v.OBJEKTY->dalsi!=NULL) {  // kdyz ukladam a existuje i nejaky objekt tak udelam aktualizaci obj.
				Form1->d.v.aktualizace_objektu();
		 }

		for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
		{
				 Form1->d.v.vloz_pohon (rStringGridEd_tab_dopravniky->Cells[1][i], //nazev
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]/60),        //rychlost od
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]/60),    //rychlost do
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]));      //roztec
		}

		Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
		Form1->d.v.PP.sirka_voziku=Form1->ms.MyToDouble(rEditNum_sirkavoziku->Text);
		Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(scRadioGroup_typVoziku->ItemIndex);


		Form1->d.v.PP.TT=Form1->ms.MyToDouble(rEditNum_takt->Text);

		Form1->DuvodUlozit(true);
		Form_parametry_linky->Close();

		}

		else {     //stisknul storno - zustavam na PL a nic jsem neulozil

		   Form1->DuvodUlozit(false);


		}
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

				if(rStringGridEd_tab_dopravniky->RowCount>1)
				{

				 rStringGridEd_tab_dopravniky->RowCount--;
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

      //Sleep(3000);    - sice po�k�, ale do editu nejde nic ps�t


	 //	scLabel_TT_change_hint->Visible=true;
	 //	scLabel_TT_change_hint->Caption="Pozor, p�i zm�n� TT dojde k p�epo��t�n� parametr� objekt�.";

	 //	rEditNum_takt->ShowHint=true;
	 //	rEditNum_takt->Hint="Pozor, p�i zm�n� TT dojde k p�epo��t�n� parametr� objekt�.";



   // scLabel_TT_change_hint->Visible=false;

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rHTMLLabel_delkaClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double delka=0.0;
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�
		delka=rEditNum_delkavoziku->Value*1000.0;
		rHTMLLabel_delka->Caption="D�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=rEditNum_delkavoziku->Value/1000.0;
		rHTMLLabel_delka->Caption="D�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_delkavoziku->Value=delka;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rHTMLLabel_sirkaClick(TObject *Sender)
{

	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double sirka=0.0;
	if(Sirkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Sirkaunit=M;
		//delka - p�epo��t�n�
		sirka=rEditNum_sirkavoziku->Value*1000.0;
		rHTMLLabel_sirka->Caption="���ka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Sirkaunit=MM;
		//delka - p�epo��t�n�
			sirka=rEditNum_sirkavoziku->Value/1000.0;
		rHTMLLabel_sirka->Caption="���ka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_sirkavoziku->Value=sirka;
	input_state=NOTHING;//u� se mohou p�epo��t�vat

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rHTMLLabel_taktClick(TObject *Sender)
{
//

	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double takt=0.0;
	if(Taktunit==MIN)//pokud je v MM, tak p�epne na metry
	{
		Taktunit=S;
		//delka - p�epo��t�n�
		takt=rEditNum_takt->Value*60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[s]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Taktunit=MIN;
		//delka - p�epo��t�n�
			takt=rEditNum_takt->Value/60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_takt->Value=takt;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender)
{
 Form1->d.v.generuj_POHONY();
 nacti_pohony();

//vykopirovana konstrukce z ADD pohon - kopiruje hodnoty z predchoziho radku
//	rStringGridEd_tab_dopravniky->RowCount++;
//	rStringGridEd_tab_dopravniky->Cols[0]->Add(rStringGridEd_tab_dopravniky->RowCount - 1);
//
//	if (rStringGridEd_tab_dopravniky->RowCount > 2) {
//
//		int i = rStringGridEd_tab_dopravniky->RowCount - 1;
//
//		rStringGridEd_tab_dopravniky->Cells[0][i] = i;
//		rStringGridEd_tab_dopravniky->Cells[1][i] = rStringGridEd_tab_dopravniky->Cells[1][i - 1];
//		rStringGridEd_tab_dopravniky->Cells[2][i] = rStringGridEd_tab_dopravniky->Cells[2][i - 1];
//		rStringGridEd_tab_dopravniky->Cells[3][i] = rStringGridEd_tab_dopravniky->Cells[3][i - 1];
//		rStringGridEd_tab_dopravniky->Cells[4][i] = rStringGridEd_tab_dopravniky->Cells[4][i - 1];
//
//	}



}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
if(Key==13)//ENTER
 {
		 if(Button_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_parametry_linky->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_parametry_linky->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_parametry_linky->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_parametry_linky->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------




