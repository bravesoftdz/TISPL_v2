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
#pragma link "scModernControls"
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
		 //	rEditNum_sirkavoziku->Visible=true;

			rStringGridEd_tab_dopravniky->Visible=true;
			Button_ADD->Visible=true;
			Button_DEL->Visible=true;

		 //	scRadioGroup_typVoziku->Visible=true;
			//rEditNum_delkavoziku->Visible=true;
			//rHTMLLabel_delka_voziku->Visible=true;


		}
		else          //Klient
		{
			scGPButton_vozik->Caption="   Jig";
			scGPButton_vozik->ImageIndex=19;
			scGPButton_pohon->Caption="   Pohon";

			rHTMLLabel_sirka->Visible=false;
		 //	rEditNum_sirkavoziku->Visible=false;

			rStringGridEd_tab_dopravniky->Visible=true;
			Button_ADD->Visible=true;
			Button_DEL->Visible=true;

		 //	scRadioGroup_typVoziku->Visible=true;
			rEditNum_delkavoziku->Visible=true;
			rHTMLLabel_delka_voziku->Visible=true;

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
	// rStringGridEd_tab_dopravniky->Columns->Items[0]->Visible=false;

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


	 scGPButton_jig->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton_jig->Options->FramePressedColor=Form_parametry_linky->Color;

	 rStringGridEd_tab_dopravniky->Left=1;
	 rStringGridEd_tab_dopravniky->Width=Form_parametry_linky->Width-2;


	 scGPButton_doporucene->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_doporucene->Options->FrameNormalColor=Form_parametry_linky->Color;

	 //nahr�n� hodnot
	 rEditNum_delka_jigu->Text=Form1->d.v.PP.delka_voziku;
	 rEditNum_sirka_jigu->Text=Form1->d.v.PP.sirka_voziku;
	 if(Form1->d.v.PP.typ_voziku==0) scGPSwitch->State=scswOff;
	 else  { scGPSwitch->State=scswOn; }
	 //scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;
	 rEditNum_takt->Text=Form1->d.v.PP.TT;

	 rStringGridEd_tab_dopravniky->SetColumnAutoFit(0);
	 rStringGridEd_hlavicka_tabulky->SetColumnAutoFit(0);

	 rStringGridEd_tab_dopravniky->Cells[0][0]="ID";
	 rStringGridEd_tab_dopravniky->Cells[1][0]="N�zev";
	 rStringGridEd_tab_dopravniky->Cells[2][0]="Rychlost od [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[3][0]="Rychlost do [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[4][0]="Rozte� [mm]";
	 rStringGridEd_tab_dopravniky->Cells[5][0]="Pou��v�n";

	 rStringGridEd_hlavicka_tabulky->Cells[0][0]=rStringGridEd_tab_dopravniky->Cells[0][0];
	 rStringGridEd_hlavicka_tabulky->Cells[1][0]=rStringGridEd_tab_dopravniky->Cells[1][0];
	 rStringGridEd_hlavicka_tabulky->Cells[2][0]=rStringGridEd_tab_dopravniky->Cells[2][0];
	 rStringGridEd_hlavicka_tabulky->Cells[3][0]=rStringGridEd_tab_dopravniky->Cells[3][0];
	 rStringGridEd_hlavicka_tabulky->Cells[4][0]=rStringGridEd_tab_dopravniky->Cells[4][0];
	 rStringGridEd_hlavicka_tabulky->Cells[5][0]=rStringGridEd_tab_dopravniky->Cells[5][0];


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

				 if(Form1->d.v.pohon_je_pouzivan(ukaz->n)){
					rStringGridEd_tab_dopravniky->Cells[5][i]="ano";
				 } else 	rStringGridEd_tab_dopravniky->Cells[5][i]="ne";

				 //ShowMessage(ukaz->n);
			rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
			rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;
			rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od*60;
			rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do*60;
			rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->roztec*1000;

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
		Changes=false;  //obecna zmena = zmena PP ci TT
		Changes_TT=false;    // konkretni zmena TT
		Changes_PP=false;   // konkretni zmena PP
		Ulozit=true;

		//pri zmene TT
		if(Form1->ms.MyToDouble(rEditNum_takt->Text) != Form1->d.v.PP.TT && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		 Changes_TT=true;
		}
		//pri zmene delky voziku
		if(Form1->ms.MyToDouble(rEditNum_delka_jigu->Text) != Form1->d.v.PP.delka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		 Changes_PP=true;
		}
		//pri zmene sirky voziku
			if(Form1->ms.MyToDouble(rEditNum_sirka_jigu->Text) != Form1->d.v.PP.sirka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL){
		 Changes=true;
		 Changes_PP=true;
		}

		//NEW
		//kontrola rozmez� jednotliv�ch pohon�   - je to spravne, cekovat vzdy vuci RD?
		AnsiString T="";
		for(unsigned short i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
		{
			//pr�chod jednotliv�mi objekty, zda je dan� pohon objektu p�i�azen a pokud ano, tak zda je mimo rozsah
			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			while(O!=NULL)
			{
			//	ShowMessage(O->RD*60.0);
			// 	ShowMessage(Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]));

				if(
					O->pohon!=NULL && //kdy� m� objekt p�i�azen pohon a z�rove�
					O->rezim==1 && //je v re�imu kontinu�l a z�rove�
					O->pohon->n==Form1->ms.a2i(rStringGridEd_tab_dopravniky->Cells[0][i]) &&//pokud objekt m� pohon p�i�azen a z�rove�
					(Form1->ms.MyToDouble(O->RD*60.0)<Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]) ||//je mimo rozsah (pod) nebo
					 Form1->ms.MyToDouble(O->RD*60.0)>Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]))//je mimo rozsah (nad)
				)
				{

					T+="Objekt: "+O->name+" Rychlost:"+O->RD*60+" vs. Pohon: "+rStringGridEd_tab_dopravniky->Cells[1][i];
					if(O->predchozi!=O)T+=",";//u posledn�ho prvku nep�id� ��rku
				}
				O=O->dalsi;
			}
			O=NULL;delete O;
		}

		if(T!="")//byly nalezeny objekty mimo rozmez� + v�pis
		{
			Changes=false;Ulozit=false;//zak�e ulo�en�
			Form1->MB("Pozor, nelze ulo�it hodnoty rozmez� pohon�, proto�e n�sleduj�c� objekty maj� rychlost mimo nov� nastaven� rozsah: "+T);
		}

		/////////////volba priority////////////////////////////////////////////////////

		bool volat_aktualizaci=false;
		int aktualizace_id;

		if(Changes_TT)//pri zmene TT + jiz existuje nejaky objekt
		{
			//Form_PL_priority->rStringGridEd_tab->Height=Form_PL_priority->rStringGridEd_tab->Height-Form_PL_priority->rStringGridEd_tab->DefaultRowHeight;
			Form_PL_priority->rStringGridEd_tab->RowCount=3;
			Form_PL_priority->scGPRadioButton4->Visible=false;
			Form_PL_priority->scGPRadioButton3->Visible=false;
			//zustava
			Form_PL_priority->rStringGridEd_tab->Cells[0][1]="Kapacita, D�lka pohonu";
			Form_PL_priority->rStringGridEd_tab->Cells[0][2]="Technologick� �as";
	 //		Form_PL_priority->rStringGridEd_tab->Cells[0][3]="Individu�ln� nastaven�";
			//meni se
			Form_PL_priority->rStringGridEd_tab->Cells[1][1]="Technologick� �as, Rychlost pohonu";
			Form_PL_priority->rStringGridEd_tab->Cells[1][2]="Kapacita, D�lka pohonu, Rychlost pohonu";
	 //		Form_PL_priority->rStringGridEd_tab->Cells[1][3]="Individu�ln� nastaven�";

			Form_PL_priority->rHTMLLabel_text->Caption="Ve formul��i do�lo ke zm�n� parametru <font color=#2b579a>Tak Time</font>, kter� ovliv�uje parametry objekt�.<br><br>Vyberte parametry, jejich� hodnota z�stane na objektech <font color=#2b579a>zachov�na</font>.";

			if(mrOk==Form_PL_priority->ShowModal())
			{
				// volani aktualizacni fce
				if(Form_PL_priority->scGPRadioButton1->Checked) aktualizace_id=1;//Form1->d.v.aktualizace_objektu(1);
				if(Form_PL_priority->scGPRadioButton2->Checked) aktualizace_id=2;//Form1->d.v.aktualizace_objektu(2);
				if(Form_PL_priority->scGPRadioButton3->Checked) aktualizace_id=-1; //Form1->d.v.aktualizace_objektu(-1); //indi nastav

				volat_aktualizaci=true;
				Ulozit=true;
			}
			else
			{
				volat_aktualizaci=false;
				Ulozit=false;
			}
	 }
	 if(Changes_PP)//pri zmene PP + jiz existuje nejaky objekt
	 {
			Form_PL_priority->rStringGridEd_tab->RowCount=4;
			//Form_PL_priority->rStringGridEd_tab->Height=3*Form_PL_priority->rStringGridEd_tab->DefaultRowHeight+2; //2px kosmetika

			Form_PL_priority->scGPRadioButton4->Visible=false;
			Form_PL_priority->scGPRadioButton3->Visible=true;
						 //zustava
			Form_PL_priority->rStringGridEd_tab->Cells[0][1]="Kapacita, Technologick� �as";
			Form_PL_priority->rStringGridEd_tab->Cells[0][2]="Kapacita, Rychlost pohonu";
			Form_PL_priority->rStringGridEd_tab->Cells[0][3]="D�lka pohonu";
		//	Form_PL_priority->rStringGridEd_tab->Cells[0][4]="Individu�ln� nastaven�";
				 //meni se
			Form_PL_priority->rStringGridEd_tab->Cells[1][1]="D�lka pohonu, Rychlost pohonu";
			Form_PL_priority->rStringGridEd_tab->Cells[1][2]="D�lka pohonu, Technologick� �as";
			Form_PL_priority->rStringGridEd_tab->Cells[1][3]="Kapacita, Technologick� �as, Rychlost pohonu";
		//	Form_PL_priority->rStringGridEd_tab->Cells[1][4]="Individu�ln� nastaven�";

			Form_PL_priority->rHTMLLabel_text->Caption="Ve formul��i do�lo ke zm�n�m parametr� <font color=#2b579a>voz�ku</font>, kter� ovliv�uj� parametry objekt�.<br><br>Vyberte parametry, jejich� hodnota z�stane na objektech <font color=#2b579a>zachov�na</font>.";

			if(mrOk==Form_PL_priority->ShowModal())
			{
					// volani aktualizacni fce
				if(Form_PL_priority->scGPRadioButton1->Checked) aktualizace_id=3; //Form1->d.v.aktualizace_objektu(3);
				if(Form_PL_priority->scGPRadioButton2->Checked) aktualizace_id=4; //Form1->d.v.aktualizace_objektu(4);
				if(Form_PL_priority->scGPRadioButton3->Checked) aktualizace_id=5; //Form1->d.v.aktualizace_objektu(5);
			//	if(Form_PL_priority->scGPRadioButton4->Checked) aktualizace_id=0; //Form1->d.v.aktualizace_objektu(0); //indi nastav

				Ulozit=true;   // predat pro M vybrany parametr z radio
				volat_aktualizaci=true;
			}
			else
			{
				volat_aktualizaci=false;
				Ulozit=false;
			}
	 }

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(Form1->d.v.OBJEKTY->dalsi==NULL)Ulozit=true;   // pokud neexistuje zadny objekt, vzdy dovolim delat zmeny a moznost ulozit

		// ukladej
		if (Ulozit)
		{
			Form1->d.v.vymaz_seznam_POHONY();
			Form1->d.v.hlavicka_POHONY();

			 if (Form1->d.v.OBJEKTY->dalsi!=NULL) {  // kdyz ukladam a existuje i nejaky objekt tak udelam aktualizaci obj.
			 //	Form1->d.v.aktualizace_objektu();
			}

			for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
			{
			 double rychlost_od;
			 double rychlost_do;
			 double roztec;
			 UnicodeString nazev;

			if (rStringGridEd_tab_dopravniky->Cells[1][i].IsEmpty())  nazev="nov� pohon";
			else  nazev=rStringGridEd_tab_dopravniky->Cells[1][i];

			if (rStringGridEd_tab_dopravniky->Cells[2][i].IsEmpty())  rychlost_od=0;
			else  rychlost_od=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]/60
			);

			if(rStringGridEd_tab_dopravniky->Cells[3][i].IsEmpty())  rychlost_do=0;
			else 	rychlost_do=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]/60);

			if(rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty())  roztec=0;
			else roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]/1000);

				 Form1->d.v.vloz_pohon (nazev, //nazev
																rychlost_od,        //rychlost od
																rychlost_do,    //rychlost do
																roztec);      //roztec
			}
			//po aktualizaci (resp. smaz�n� a ulo�en�) pohon� mus� n�sledovat aktualizace p�i�azen� pohon� k objektum:
			if (Form1->d.v.OBJEKTY->dalsi!=NULL)Form1->d.v.aktualizace_objektu(-2);


			// docasne - resim pouze rozmery Jigu neporovnamvam tedy vuci voziku
		 //	if(Form1->ms.MyToDouble(rEditNum_delkavoziku->Text) > Form1->ms.MyToDouble(rEditNum_delka_jigu->Text))
		 //	{
		 //	Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
		 //	}
		 //	else
				Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delka_jigu->Text);

			int typ;
			//Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
			Form1->d.v.PP.sirka_voziku=Form1->ms.MyToDouble(rEditNum_sirka_jigu->Text);   //zavadejici docasne reseni
			if(scGPSwitch->State==scswOff) {
			 typ=0;}
			 else {typ=1;}
			Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(typ);
			Form1->d.v.PP.TT=Form1->ms.MyToDouble(rEditNum_takt->Text);

			if(volat_aktualizaci)
			{
				Form1->d.v.aktualizace_objektu(aktualizace_id);
				//doplnil 1.2.2018 M, aktualizace i �asov�ch os a pop�. ROMA, nutn� z d�vodu zm�ny parametr� objektu
				Form1->aktualizace_maro_a_roma();//aktualizace a p�epo�et volan� kv�li �asov�m os�m (maro) a techn.proces�m(roma)
			}

			Form1->DuvodUlozit(true);
			Form_parametry_linky->Close();
		}
		else//stisknul storno - zustavam na PL a nic jsem neulozil
		{
			 Form1->DuvodUlozit(false);
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_ADD_Click(TObject *Sender)
{
	rStringGridEd_tab_dopravniky->RowCount++;
	rStringGridEd_tab_dopravniky->Cols[0]->Add(rStringGridEd_tab_dopravniky->RowCount - 1);

	if (rStringGridEd_tab_dopravniky->RowCount > 2) {

		int i = rStringGridEd_tab_dopravniky->RowCount - 1;

		rStringGridEd_tab_dopravniky->Cells[0][i] = i;
		rStringGridEd_tab_dopravniky->Cells[1][i] = "nov� pohon";//rStringGridEd_tab_dopravniky->Cells[1][i - 1];
		rStringGridEd_tab_dopravniky->Cells[2][i] = "0";//rStringGridEd_tab_dopravniky->Cells[2][i - 1];
		rStringGridEd_tab_dopravniky->Cells[3][i] = "0";//rStringGridEd_tab_dopravniky->Cells[3][i - 1];
		rStringGridEd_tab_dopravniky->Cells[4][i] = "0";//rStringGridEd_tab_dopravniky->Cells[4][i - 1];
		rStringGridEd_tab_dopravniky->Cells[5][i] = "ne";




	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::Button_DEL_Click(TObject *Sender)
{

			if(Form1->d.v.pohon_je_pouzivan(rStringGridEd_tab_dopravniky->RowCount-1))
			{
				AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(rStringGridEd_tab_dopravniky->RowCount-1,true);
						if(mrOk==Form1->MB("Pohon je pou��v�n pro objekty: <b>"+objekty+"</b>. Opravdu m� b�t smaz�n?",MB_OKCANCEL)){

						Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(rStringGridEd_tab_dopravniky->RowCount-1);
						rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

							if(rStringGridEd_tab_dopravniky->RowCount>1)
							{
							rStringGridEd_tab_dopravniky->RowCount--;
							}
						 //	Form1->MB("Smazano");
						}
						else { //storno   - nic se ned�je

								}
				}

			else {  // pohon neni pouzivany, mohu ho smazat cokoliv ze stringgridu

				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

				if(rStringGridEd_tab_dopravniky->RowCount>1)
				{
				 rStringGridEd_tab_dopravniky->RowCount--;
				 }
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

	 //	scExPanel_doporuc_pohony->Left=1;
	 //	scExPanel_doporuc_pohony->Width=Form_parametry_linky->Width;
		scExPanel_doporuc_pohony->Visible=true;
		scGPButton_doporucene->Visible=false;
		scHTMLLabel_doporuc_pohony->Caption=Form1->d.v.navrhni_POHONY();

}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::scExPanel_doporuc_pohonyClose(TObject *Sender)

{
scExPanel_doporuc_pohony->Visible=false;
scGPButton_doporucene->Visible=true;
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

void __fastcall TForm_parametry_linky::rHTMLLabel_delkavozikuClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double delka=0.0;
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�
		delka=rEditNum_delka_jigu->Value*1000.0;
		rHTMLLabel_delka_jigu->Caption="D�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=rEditNum_delka_jigu->Value/1000.0;
		rHTMLLabel_delka_jigu->Caption="D�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_delka_jigu->Value=delka;
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
		sirka=rEditNum_sirka_jigu->Value*1000.0;
		rHTMLLabel_sirka->Caption="���ka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Sirkaunit=MM;
		//delka - p�epo��t�n�
			sirka=rEditNum_sirka_jigu->Value/1000.0;
		rHTMLLabel_sirka->Caption="���ka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_sirka_jigu->Value=sirka;
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
//tla��tko na kop�rov�n� na kop�rov�n� doporu�en�ch pohon� do striggridu, nep�id�v� ale do pohon�
void __fastcall TForm_parametry_linky::scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender)
{
 //ji� se pou��v� z d�vodu storna
 //Form1->d.v.generuj_POHONY();
 //nacti_pohony();

 //nov� konstrukce zaji��uj�c� pouze vlo�en� do stringgridu, o samotn� ulo�en� pohon� se star� a� tla��tko ulo�it
 AnsiString T=scHTMLLabel_doporuc_pohony->Caption;
 while(T.Pos("</br>"))//bude parsovat dokud bude </br>
 {
	//zv��en� po�tu ��dk�
	rStringGridEd_tab_dopravniky->RowCount++;
	unsigned short i=rStringGridEd_tab_dopravniky->RowCount-1;//pouze zkr�cen� z�pisu
	//pln�n� ��dku a parsov�n� daty
	rStringGridEd_tab_dopravniky->Cells[0][i]=i;
	rStringGridEd_tab_dopravniky->Cells[1][i]=Form1->ms.TrimRightFrom(T,",");T=Form1->ms.TrimLeftFromText(T,", ");
	rStringGridEd_tab_dopravniky->Cells[2][i]=Form1->ms.EP(T,"rychlost:"," [");T=Form1->ms.TrimLeftFrom_UTF(T," </br>");
	rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
	rStringGridEd_tab_dopravniky->Cells[5][i]="ne";
	//smaz�n� jednoho ji� nepot�ebn�ho z�znamu
	T=Form1->ms.TrimLeftFromText(T,"</br>");
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key==13)//ENTER
	{
		 if(Button_save->Enabled)Button_saveClick(Sender);//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
	}
	if(Key==27)//ESC
	{
		 Button_stornoClick(Sender);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button1Click(TObject *Sender)
{
	Cvektory:: TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			while (O!=NULL){
	Memo1->Lines->Add(O->pohon->n);

	O=O->dalsi;
		}
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyGetEditStyle(TObject *Sender,
					int Col, int Row, TrStringGridEdEditStyle &EditStyle)
{

//if (Col==1 && Row==5) {

 //	EditStyle=sgbDropDown;   //vyber typu

// SetFocus();



 //	 }

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyCanEdit(TObject *Sender,
          int Col, int Row, bool &CanEdit)
{

if (Col==1 && Row==0) {
	CanEdit=false;
	 }
if (Col==2 && Row==0) {
	CanEdit=false;
	 }
if (Col==3 && Row==0) {
	CanEdit=false;
	 }
if (Col==4 && Row==0) {
	CanEdit=false;
	 }
if (Col==5 && Row==0) {
	CanEdit=false;
	 }
}
//---------------------------------------------------------------------------




void __fastcall TForm_parametry_linky::rEditNum_delkavozikuClick(TObject *Sender)

{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double delka=0.0;
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�
		delka=rEditNum_delkavoziku->Value*1000.0;
		rHTMLLabel_delka_voziku->Caption="d�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=rEditNum_delkavoziku->Value/1000.0;
		rHTMLLabel_delka_voziku->Caption="d�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_delkavoziku->Value=delka;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rHTMLLabel_delka_jiguClick(TObject *Sender)

{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double delka=0.0;
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�
		delka=rEditNum_delka_jigu->Value*1000.0;
		rHTMLLabel_delka_jigu->Caption="d�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=rEditNum_delka_jigu->Value/1000.0;
		rHTMLLabel_delka_jigu->Caption="d�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_delka_jigu->Value=delka;
	input_state=NOTHING;//u� se mohou p�epo��t�vat

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rHTMLLabel_delka_vozikuClick(TObject *Sender)

{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double delka=0.0;
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�
		delka=rEditNum_delkavoziku->Value*1000.0;
		rHTMLLabel_delka_voziku->Caption="d�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=rEditNum_delkavoziku->Value/1000.0;
		rHTMLLabel_delka_voziku->Caption="d�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	rEditNum_delkavoziku->Value=delka;
	input_state=NOTHING;//u� se mohou p�epo��t�vat

}
//---------------------------------------------------------------------------



void __fastcall TForm_parametry_linky::Button_DELMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
//	if(rStringGridEd_tab_dopravniky->RowCount==1) Button_DEL->Enabled=false;
 //		else  Button_DEL->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_delka_jiguKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
	FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_sirka_jiguKeyDown(TObject *Sender,
					WORD &Key, TShiftState Shift)
{
	FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_taktKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
  FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyEnter(TObject *Sender)

{
 rStringGridEd_tab_dopravniky->Width=Form_parametry_linky->Width;
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
 // ShowMessage(rStringGridEd_tab_dopravniky->RowCount);
 	if (rStringGridEd_tab_dopravniky->RowCount<=1) {
	Button_DEL->Enabled=false;
	}  else Button_DEL->Enabled=true;

}
//---------------------------------------------------------------------------




