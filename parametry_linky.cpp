//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "parametry_linky.h"
#include "Unit1.h"
#include "kabina_schema.h"
#include "MyMessageBox.h"
#include "parametry.h"
#include "TT_kalkulator.h"
#include "parametry_vozik.h"
#include "Unit2.h"
#include "gapoR.h"
#include "gapoTT.h"
#include "gapoV.h"
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
#pragma link "scGPExtControls"
#pragma link "rHintWindow"
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
	////designové záležitosti
	Form_parametry_linky->Color=(TColor)RGB(240,240,240); //nastavení barvy formuláøe
	Form1->m.designButton(Button_save,Form_parametry_linky,1,2);
	Form1->m.designButton(Button_storno,Form_parametry_linky,2,2);
	zobrazitFrameForm=false;

	 rStringGridEd_tab_dopravniky->SetColumnAutoFit(0);
	 rStringGridEd_hlavicka_tabulky->SetColumnAutoFit(0);

	//pop-up menu
	//pozadí
	clBg=(TColor)RGB(240,240,240); //pasive
	clAcBg=(TColor)RGB(212,208,200);//active
	clAcBg2=(TColor)RGB(195,192,184);//active køížku
	//výplò glyphu
	clGlyph=(TColor)RGB(43,87,154);//pasive
	clAcGlyph=(TColor)RGB(0,128,255);//active
	PopUPmenu->Color=clBg;//nastavení pozadí barvy formuláøe
	pasiveColor();//nastaví všechny položky na pasivní resp. default barvu
	//------

	////jednotky
	//Delkaunit=MM;
	//Sirkaunit=Delkaunit;
	 Taktunit=S;
	 Runit=M;
	 Rzunit=M;
	//roletka_data=0;

}
//---------------------------------------------------------------------------
void TForm_parametry_linky::pasiveColor()//nastaví všechny položky pop-up na pasivní resp. default barvu
{
	Item_zobrazit_parametry->FillColor=clBg;
	Item_nastavit_parametry->FillColor=clBg;
	Item_kopirovat->FillColor=clBg;
	Item_smazat->FillColor=clBg;
	Item_smazat_nepouzite->FillColor=PopUPmenu->Color;

	GlyphButton_close->Options->NormalColor=clAcBg;
	GlyphButton_close->Options->HotColor=clRed;
	GlyphButton_close->Options->FocusedColor=clAcBg;
	GlyphButton_close->Options->FrameNormalColor=clAcBg;
	GlyphButton_close->Options->FrameHotColor=clRed;
	GlyphButton_close->Options->FrameFocusedColor=clAcBg;
	GlyphButton_zobrazit_parametry->Options->NormalColor=clGlyph;
	GlyphButton_zobrazit_parametry->GlyphOptions->NormalColor=clWhite;
	GlyphButton_zobrazit_parametry->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_nastavit_parametry->Options->NormalColor=clGlyph;
	GlyphButton_nastavit_parametry->GlyphOptions->NormalColor=clWhite;
	GlyphButton_nastavit_parametry->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_kopirovat->Options->NormalColor=clGlyph;
	GlyphButton_kopirovat->GlyphOptions->NormalColor=clWhite;
	GlyphButton_kopirovat->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_smazat->Options->NormalColor=clGlyph;
	GlyphButton_smazat->GlyphOptions->NormalColor=clWhite;
	GlyphButton_smazat->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_smazat_nepouzite->Options->NormalColor=clGlyph;
	GlyphButton_smazat_nepouzite->GlyphOptions->NormalColor=clWhite;
	GlyphButton_smazat_nepouzite->GlyphOptions->NormalColorAlpha=200;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{

		//Delkaunit=MM;
		// GLOBAL naètení délka, šíøka, podvozek
//	if (Form1->readINI("nastaveni_form_parametry_linky", "rozmery") == "1")
//		{
//				rHTMLLabel_delka_jiguClick(this);
//				rHTMLLabel_sirkaClick(this);
//				rEditNum_delkavozikuClick(this);
//		}
	 input_state = NOTHING;   //výchozí nastavení stavu
	 Nastav_zamky(empty_klik_ico,empty_klik);
	 vypis(""); VID=-1;



		scExPanel_doporuc_pohony->Visible=false;
		PopUPmenu->Visible=false;
		//Button_save->SetFocus();
		rMemoEx_Nazev->SetFocus();
		zobrazOramovani=false;

		if(scGPSwitch->State==0) {rHTMLLabel_podvozek_zaves->Caption="Podvozek";   rHTMLLabel_podvozek_zaves->Left=34;  }
		else  { rHTMLLabel_podvozek_zaves->Caption="Závìs";  rHTMLLabel_podvozek_zaves->Left=56; }

		//provizorní ošetøení, pøijde celé smazat, až nahodíme aktualizaci
//		if(Form1->d.v.OBJEKTY->dalsi!=NULL || Form1->d.v.POHONY->dalsi!=NULL)
//		{
//			rEditNum_takt->Enabled=false;
//			rEditNum_delka_jigu->Enabled=false;
//			rEditNum_sirka_jigu->Enabled=false;
//			scGPNumericEdit_delka_podvozku->Enabled=false;
//		}
//		else
//		{
//			rEditNum_takt->Enabled=true;
//			rEditNum_delka_jigu->Enabled=true;
//			rEditNum_sirka_jigu->Enabled=true;
//			scGPNumericEdit_delka_podvozku->Enabled=true;
//		}


		if(Form1->d.v.navrhni_POHONY()=="")
		{
			scGPButton_doporucene->Visible=false;
			rHTMLLabel_doporuc_pohony->Caption=""; // neexistují žádné objekty -> neumím spoèítat doporuè. rychlosti
		}
		else
		{
			scGPButton_doporucene->Visible=true;
			rHTMLLabel_doporuc_pohony->Caption="Navržené pohony pro objekty bez pøiøazených pohonù:";
		}

		if(Form1->STATUS==Form1->NAVRH)    //Architekt
		{
			scGPButton_vozik->Caption="   Vozík";
			scGPButton_vozik->ImageIndex=21;
			//scGPButton_obecne->Caption="   Takt time";
			rHTMLLabel_takt->Visible=true;
			rEditNum_takt->Visible=true;
			rHTMLLabel_sirka_jig->Visible=true;
		 //	rEditNum_sirkavoziku->Visible=true;

			rStringGridEd_tab_dopravniky->Visible=true;
			//Button_ADD->Visible=true;
			//Button_DEL->Visible=true;

		 //	scRadioGroup_typVoziku->Visible=true;
			//rEditNum_delkavoziku->Visible=true;
			//rHTMLLabel_delka_voziku->Visible=true;
		}
		else          //Klient
		{
			scGPButton_vozik->Caption="   Jig";
			scGPButton_vozik->ImageIndex=19;
			scGPButton_pohon->Caption="   Pohon";

			rHTMLLabel_sirka_jig->Visible=false;
		 //	rEditNum_sirkavoziku->Visible=false;

			rStringGridEd_tab_dopravniky->Visible=true;
			Button_ADD->Visible=true;
			Button_DEL->Visible=true;

		 //	scRadioGroup_typVoziku->Visible=true;

			scGPButton_obecne->Visible=false;
			rHTMLLabel_takt->Visible=false;
			rEditNum_takt->Visible=false;
		}

		nacti_pohony();

		if(!data_nalezena)
		{
			rStringGridEd_tab_dopravniky->RowCount=1;    //defaultní poèet øádkù - hlavièka
		}

		//pro vytvoøení zálohy zrušených pøíøazení - vyfikundace z dùvodu možného storna
		//musí být umístìno až za nacti_pohony
		zrusena_prirazeni_PID_size=rStringGridEd_tab_dopravniky->RowCount-1;//velikost staèí jako poèet øádkù/pohonu po naètení, více jich být pøiøazeno do nového naètení formu být nemùže
		zrusena_prirazeni_PID=new bool[zrusena_prirazeni_PID_size];
		for(unsigned int PID=0;PID<zrusena_prirazeni_PID_size;PID++)zrusena_prirazeni_PID[PID]=false;


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

	 scGPButton_zamek_aRD->Options->NormalColor=clWhite;
	 scGPButton_zamek_aRD->Options->FocusedColor=clWhite;
	 scGPButton_zamek_aRD->Options->HotColor=clWhite;
	 scGPButton_zamek_aRD->Options->PressedColor=clWhite;
	 scGPButton_zamek_aRD->Options->FrameNormalColor=clWhite;
	 scGPButton_zamek_aRD->Options->PressedColor=clWhite;
	 scGPButton_zamek_aRD->Options->FramePressedColor=clWhite;

	 scGPButton_zamek_roztec->Options->NormalColor=clWhite;
	 scGPButton_zamek_roztec->Options->FocusedColor=clWhite;
	 scGPButton_zamek_roztec->Options->HotColor=clWhite;
	 scGPButton_zamek_roztec->Options->PressedColor=clWhite;
	 scGPButton_zamek_roztec->Options->FrameNormalColor=clWhite;
	 scGPButton_zamek_roztec->Options->PressedColor=clWhite;
	 scGPButton_zamek_roztec->Options->FramePressedColor=clWhite;

	 scGPButton_zamek_Rz->Options->NormalColor=clWhite;
	 scGPButton_zamek_Rz->Options->FocusedColor=clWhite;
	 scGPButton_zamek_Rz->Options->HotColor=clWhite;
	 scGPButton_zamek_Rz->Options->PressedColor=clWhite;
	 scGPButton_zamek_Rz->Options->FrameNormalColor=clWhite;
	 scGPButton_zamek_Rz->Options->PressedColor=clWhite;
	 scGPButton_zamek_Rz->Options->FramePressedColor=clWhite;

	 scGPButton_zamek_Rx->Options->NormalColor=clWhite;
	 scGPButton_zamek_Rx->Options->FocusedColor=clWhite;
	 scGPButton_zamek_Rx->Options->HotColor=clWhite;
	 scGPButton_zamek_Rx->Options->PressedColor=clWhite;
	 scGPButton_zamek_Rx->Options->FrameNormalColor=clWhite;
	 scGPButton_zamek_Rx->Options->PressedColor=clWhite;
	 scGPButton_zamek_Rx->Options->FramePressedColor=clWhite;

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

	 //nahrání hodnot
	 if(Delkaunit == MM)
	 {
			scGPNumericEdit_delka_jig->Value=Form1->d.v.PP.delka_jig*1000;
			scGPNumericEdit_sirka_jig->Value=Form1->d.v.PP.sirka_jig*1000;
			scGPNumericEdit_delka_podvozek->Value=Form1->d.v.PP.delka_podvozek*1000;
	 }
	 else // jsem v Metrech, naètu uložená data (vždy v metrech)
	 {
			scGPNumericEdit_delka_jig->Value=Form1->d.v.PP.delka_jig;
			scGPNumericEdit_sirka_jig->Value=Form1->d.v.PP.sirka_jig;
			scGPNumericEdit_delka_podvozek->Value=Form1->d.v.PP.delka_podvozek;
	 }


	 if(Form1->d.v.PP.typ_voziku==0) scGPSwitch->State=scswOff;
	 else  { scGPSwitch->State=scswOn; }
	 //scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;


	 if(Taktunit==MIN)
	 {
	 rEditNum_takt->Value=Form1->d.v.PP.TT/60.0;
	 } else rEditNum_takt->Value=Form1->d.v.PP.TT;

	 //rStringGridEd_tab_dopravniky->Cells[0][0]="ID";
	 //rStringGridEd_tab_dopravniky->Cells[1][0]="Název";
	 rStringGridEd_tab_dopravniky->Cells[2][0]="od";
	 rStringGridEd_tab_dopravniky->Cells[3][0]="do";
	 rStringGridEd_tab_dopravniky->Cells[4][0]="aktuální";
	 rStringGridEd_tab_dopravniky->Cells[5][0]="";
	 rStringGridEd_tab_dopravniky->Cells[6][0]="vzdálenost [m]";
	 rStringGridEd_tab_dopravniky->Cells[7][0]="každý n-tý palec";
	 //rStringGridEd_tab_dopravniky->Cells[8][0]="pøiøazen";

	 rStringGridEd_hlavicka_tabulky->Cells[0][0]="";
	 rStringGridEd_hlavicka_tabulky->Cells[1][0]="";
	 rStringGridEd_hlavicka_tabulky->Cells[2][0]="";
	 //rStringGridEd_hlavicka_tabulky->Cells[3][0]="Rychlost [m/min]";
	 rStringGridEd_hlavicka_tabulky->Cells[4][0]="";
	 rStringGridEd_hlavicka_tabulky->Cells[5][0]="";
	// rStringGridEd_hlavicka_tabulky->Cells[6][0]="Rozestup";
	 rStringGridEd_hlavicka_tabulky->Cells[7][0]="";
	 rStringGridEd_hlavicka_tabulky->Cells[8][0]="";

	//	rStringGridEd_tab_dopravniky->Columns->

	 if(rStringGridEd_tab_dopravniky->RowCount>1 && rStringGridEd_tab_dopravniky->ColCount>=5){rStringGridEd_tab_dopravniky->Row=1;rStringGridEd_tab_dopravniky->Col=5;}//rStringGridEd_tab_dopravniky->FinishEditing();//ukonèí editaci a tím odstraní nepøíjemný vizuální efekt

	 //pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	 pozice_scGPGlyphButton_hint();

	 //testuje zda existují nepoužíté pohony a je tedy vhodné nabídku na smazání nepoužitých zobrazovat
	 if(existuji_nepouzivane_pohony())scGPGlyphButton_DEL_nepouzite->Visible=true;
	 else scGPGlyphButton_DEL_nepouzite->Visible=false;

	 rMemoEx_ID->Text="";rMemoEx_Nazev->Text="";rMemoEx1_rychlost->Text="";
	 rMemoEx2_prirazen->Text="";rMemoEx1_rozestup->Text="";rMemoEx1_roztec->Text="";
	 rMemoEx1_rozestup_akt_unas->Text="";



	 rMemoEx_ID->Lines->Add("    ID");
	 rMemoEx_Nazev->Lines->Add("    Název");
	 rMemoEx1_rychlost->Lines->Add("   Rychlost [m/min]");
	 if(Runit==M)  rMemoEx1_roztec->Lines->Add("    Rozteè [m]");
	 else rMemoEx1_roztec->Lines->Add("    Rozteè [mm]");
	 rMemoEx1_rozestup->Lines->Add("   Palce");
	 rMemoEx1_rozestup_akt_unas->Lines->Add("rozestup aktivní unašeèe");
	 rMemoEx2_prirazen->Lines->Add("   Pøiøazen");
}
//---------------------------------------------------------------------------
//
void TForm_parametry_linky::nacti_pohony ()
{

   data_nalezena=false;
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

	 if (ukaz!=NULL)
	 {
				rStringGridEd_tab_dopravniky->RowCount = Form1->d.v.POHONY->predchozi->n + 1;
				data_nalezena=true; //pokud jsou ve spojaku nejaka data, nastavit na true
				 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
				 {
						//pùvodní if(Form1->d.v.pohon_je_pouzivan(ukaz->n))rStringGridEd_tab_dopravniky->Cells[5][i]="ano";else 	rStringGridEd_tab_dopravniky->Cells[5][i]="ne";
						AnsiString OBJEKTY_POUZIVAJICI_POHON=Form1->d.v.vypis_objekty_vyuzivajici_pohon(ukaz->n);
						if(OBJEKTY_POUZIVAJICI_POHON!="")rStringGridEd_tab_dopravniky->Cells[8][i]=OBJEKTY_POUZIVAJICI_POHON;
						else rStringGridEd_tab_dopravniky->Cells[8][i]="nepoužíván";

					 //	ShowMessage(ukaz->roztec*1000.0);

						//	 ShowMessage(ukaz->rychlost_do*60.0);
						rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
						rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;

					 //	if(ukaz->rychlost_od==0)  	rStringGridEd_tab_dopravniky->Cells[2][i] = "";
					//	else
						 rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od*60.0;

						if(ukaz->rychlost_do==0)   rStringGridEd_tab_dopravniky->Cells[3][i] ="";
						else rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do*60.0;

						if(ukaz->aRD==0)  	rStringGridEd_tab_dopravniky->Cells[4][i] = "";
						else rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->aRD*60.0;

						if(ukaz->roztec==0) rStringGridEd_tab_dopravniky->Cells[5][i]="";
						if(Runit==MM) rStringGridEd_tab_dopravniky->Cells[5][i] = ukaz->roztec*1000.0;
						else    rStringGridEd_tab_dopravniky->Cells[5][i] = ukaz->roztec;

						if(ukaz->Rz==0) rStringGridEd_tab_dopravniky->Cells[6][i]="";
						else rStringGridEd_tab_dopravniky->Cells[6][i] = ukaz->Rz;

						if(ukaz->Rx==0) rStringGridEd_tab_dopravniky->Cells[7][i]="";
						else rStringGridEd_tab_dopravniky->Cells[7][i] = ukaz->Rx;

						ukaz = ukaz->dalsi;
				 }
				 F_gapoR->pohony_zmena=new TPoint[F->d.v.POHONY->predchozi->n+1]; //alokace o jednièku vyšší, nultý index není totiž využíván
				 for(int i=0; i<=F->d.v.POHONY->predchozi->n;i++){F_gapoR->pohony_zmena[i].X=false;F_gapoR->pohony_zmena[i].Y=false;}





	}
	else {  //pokud je spoják prázdný, zobrazím tyto pøednastavené hodnoty

	data_nalezena=false; // default se nepoužívá resp. po soubor nový se okamžitì vloží do spojáku def.pohon
//nevytvari se zadny default pohon nikde
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{
	for(int i=1;i<=rStringGridEd_tab_dopravniky->RowCount;i++)
	{
		rStringGridEd_tab_dopravniky->Rows[i]->Clear();
		rStringGridEd_tab_dopravniky->RowCount--;
	}
	//M toto tu nesmí být:Form_parametry_linky->Close();
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::KonecClick(TObject *Sender)
{
	Button_stornoClick(Sender);//stejná funkcionalita jako u storna
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_saveClick(TObject *Sender)
{
		Changes=false;  //obecna zmena = zmena PP ci TT
		Changes_TT=false;    // konkretni zmena TT
		Changes_PP=false;   // konkretni zmena PP
		Changes_roztec=false;
		Changes_prirazen=false; //pokud odpøiøadím používaný pohon
		Changes_aRD=false;
		Changes_Rz=false;
		Changes_Rx=false;

		Ulozit=true;
		double delka_jig;
		double sirka_jig;
		double delka_podvozek;
		double Takt;
		bool volat_aktualizaci=false;
		int aktualizace_id;

		//zobrazení formu gapoR
		bool zobrazGAPO_R=false;
		for(int i=1; i<=F->d.v.POHONY->predchozi->n;i++)
		{
			if(F_gapoR->pohony_zmena[i].X) zobrazGAPO_R=true;
			//	ShowMessage(AnsiString(i)+" "+AnsiString((short)(F_gapoR->pohony_zmena[i])));
		}
		if(zobrazGAPO_R)//pokud byl nìjaký používaný pohon zmìnìn
		{
			F_gapoR->Left=Left;F_gapoR->Top=Top;
			F_gapoR->ShowModal();
			mGrid->Delete();
		}

		//NEW
		//kontrola rozmezí jednotlivých pohonù   - je to spravne, cekovat vzdy vuci RD?
		AnsiString T="";
		for(short i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)

		{
			//prùchod jednotlivými objekty, zda je daný pohon objektu pøiøazen a pokud ano, tak zda je mimo rozsah
			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			while(O!=NULL)
			{

				if(
					O->pohon!=NULL && //když má objekt pøiøazen pohon a zároveò
					O->rezim==1 && //je v režimu kontinuál a zároveò
					O->pohon->n==Form1->ms.a2i(rStringGridEd_tab_dopravniky->Cells[0][i]) &&//pokud objekt má pohon pøiøazen a zároveò
					(Form1->ms.MyToDouble(O->RD)*60.0<Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]) ||//je mimo rozsah (pod) nebo
					 Form1->ms.MyToDouble(O->RD)*60.0>Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]))//je mimo rozsah (nad)
				)
				{

					T+="Objekt: "+O->name+" Rychlost:"+O->RD*60.0+" vs. Pohon: "+rStringGridEd_tab_dopravniky->Cells[1][i];
					if(O->predchozi!=O)T+=",";//u posledního prvku nepøidá èárku
				}
				O=O->dalsi;
			}
			O=NULL;delete O;
		}

		if(T!="")//byly nalezeny objekty mimo rozmezí + výpis
		{
			Changes=false;Ulozit=false;//zakáže uložení
			Form1->MB("Pozor, nelze uložit hodnoty rozmezí pohonù, protože následující objekty mají rychlost mimo novì nastavený rozsah: "+T);
		}


		////////////////////////////////////////////////////////////////////////
		//Pri zmene roztece  - volani zmìny rozteèe - pokud dojde ke zmìnì rozteèe u používaného pohonu - pøedám status pro zobrazení PL_priority


			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
			while(P!=NULL)
			{
			     double roztec;
					 if(Runit==M) roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n]);
					 else 				roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n])/1000.0;
					if(rStringGridEd_tab_dopravniky->Cells[8][P->n]!="nepoužíván"  && roztec!=P->roztec)
					{
					if(rStringGridEd_tab_dopravniky->Cells[5][P->n]!="")  //osetreni situace kdyz odmazu pohon a N je prazdne
					{
							Changes_roztec=true;
						 //	Memo2->Lines->Add(rStringGridEd_tab_dopravniky->Cells[5][P->n]);
						 //	Memo3->Lines->Add(P->roztec);
						}
					}
					P=P->dalsi;
				 }
			P=NULL;delete P;


			//////////////////////////////////////////////////

			// Hlídaní následujících zmìn, pokud je pohon používaný

				Cvektory::TPohon *p_prirazen=Form1->d.v.POHONY->dalsi;
			while(p_prirazen!=NULL)
			{
						bool pohon_pouzivan=false;
						AnsiString pohon_prirazen=Form1->d.v.vypis_objekty_vyuzivajici_pohon(p_prirazen->n);
						if(pohon_prirazen!="")pohon_pouzivan=true;
						else   pohon_pouzivan=false;

				 //zmìna odpøiøazení
					if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]=="nepoužíván"  && pohon_pouzivan)
					{
							Changes_prirazen=true;
							zrusena_prirazeni_PID[getPID(p_prirazen->n)-1]=true;

					}
				 //zmìna aRD
						if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepoužíván"  && rStringGridEd_tab_dopravniky->Cells[4][p_prirazen->n]!=p_prirazen->aRD*60.0)
					{
							Changes_aRD=true;

					}

					//zmìna Rz
							if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepoužíván"  && rStringGridEd_tab_dopravniky->Cells[6][p_prirazen->n]!=p_prirazen->Rz)
					{
							Changes_Rz=true;

					}
					 //zmìna Rx
								if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepoužíván"  && rStringGridEd_tab_dopravniky->Cells[7][p_prirazen->n]!=p_prirazen->Rx)
					{
							Changes_Rx=true;

					}

					p_prirazen=p_prirazen->dalsi;
				 }


			p_prirazen=NULL;delete p_prirazen;




//	 if(Changes_roztec)
//	 {
//
//	 ShowMessage("nejaka zmena roztece");
//
//	 }
//		if(Changes_prirazen)
//	 {
//
//	 ShowMessage("Odpøiøazení pohonu");
//
//	 }
//
//			if(Changes_aRD)
//	 {
//
//	 ShowMessage("Zmìna RD pohonu");
//
//	 }
//
//			if(Changes_Rz)
//	 {
//
//	 ShowMessage("Zmìna Rz pohonu");
//
//	 }
//
//				if(Changes_Rx)
//	 {
//
//	 ShowMessage("Zmìna Rx pohonu");
//
//	 }

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(Form1->d.v.OBJEKTY->dalsi==NULL)Ulozit=true;   // pokud neexistuje zadny objekt, vzdy dovolim delat zmeny a moznost ulozit

		// ukladej
		if (Ulozit)
		{
			zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum();
			Form1->d.v.vymaz_seznam_POHONY();
			Form1->d.v.hlavicka_POHONY();

			for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
			{
				double rychlost_od;
				double rychlost_do;
				double roztec;
				double aRD;
				double Rz;
				double Rx;
				UnicodeString nazev;

				if (rStringGridEd_tab_dopravniky->Cells[1][i].IsEmpty()) nazev="nový pohon";
				else  nazev=rStringGridEd_tab_dopravniky->Cells[1][i];

				if (rStringGridEd_tab_dopravniky->Cells[2][i].IsEmpty()) rychlost_od=0;
				else  rychlost_od=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i])/60.0;

				if(rStringGridEd_tab_dopravniky->Cells[3][i].IsEmpty()) rychlost_do=0;
				else 	rychlost_do=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i])/60.0;

				if(rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty()) aRD=0;
				else aRD=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i])/60.0;

				if(rStringGridEd_tab_dopravniky->Cells[5][i].IsEmpty()) roztec=0;
				if(Runit==MM) roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i])/1000.0;
				else roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i]);


				if(rStringGridEd_tab_dopravniky->Cells[6][i].IsEmpty()) Rz=0;
				else Rz=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i]);

				if(rStringGridEd_tab_dopravniky->Cells[7][i].IsEmpty()) Rx=0;
				else Rx=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][i]);

				//uložení pohonu do spojáku
				Form1->d.v.vloz_pohon (nazev,rychlost_od,rychlost_do,aRD,roztec,Rz,Rx);

				//všem objektùm, které mìly pøiøazen pohon s oldN(oldID), pøiøadí pohon s newN(newID), podle toho, jak jsou ukládány novì do spojáku, dùležité, pokud dojde k narušení poøadí ID resp n pohonù a poøadí jednotlivých øádkù ve stringridu, napø. kopirováním, smazáním, zmìnou poøadí øádkù atp., øeší i pro pøípad napø. 2->3,3->4 pomocí atributu objektu probehla_aktualizace_prirazeni_pohonu (aby prvnì nebyl pøiøezn pohon s id 2 na 3 a potom všechny pohony s id 3 na pohon 4, protože mìly být pøiøazený jen nìkteré...)
				Form1->d.v.aktualizace_prirazeni_pohonu_k_objektum(getPID(i),i);
			}
			//po dokonèení aktualizace pøiøazení pohonu (pøi ukládání pohonu na PL) vrátí atribut probehla_aktualizace_prirazeni_pohonu všech objektù na false, aby bylo pøipraveno k dalšímu opìtovnému užítí, nepøímo spolupracuje s metodou výše uvedenou aktualizace_prirazeni_pohonu_k_objektum
			Form1->d.v.aktualizace_prirazeni_pohonu_dokoncena();


			// docasne - resim pouze rozmery Jigu neporovnamvam tedy vuci voziku
			//	if(Form1->ms.MyToDouble(rEditNum_delkavoziku->Text) > Form1->ms.MyToDouble(rEditNum_delka_jigu->Text))
			//	{
			//	Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
			//	}
			//	else
			if(Delkaunit==MM)
			{
				F->d.v.PP.delka_jig=F->ms.MyToDouble(scGPNumericEdit_delka_jig->Value)/1000.0;
				F->d.v.PP.sirka_jig=F->ms.MyToDouble(scGPNumericEdit_sirka_jig->Value)/1000.0;
				F->d.v.PP.delka_podvozek=scGPNumericEdit_delka_podvozek->Value/1000.0;
			}
			else  //Metry
			{
				F->d.v.PP.delka_jig=F->ms.MyToDouble(scGPNumericEdit_delka_jig->Value);
				F->d.v.PP.sirka_jig=F->ms.MyToDouble(scGPNumericEdit_sirka_jig->Value);
				F->d.v.PP.delka_podvozek=scGPNumericEdit_delka_podvozek->Value;
			}
			int typ;
			if(scGPSwitch->State==scswOff){typ=0;}
			else {typ=1;}
			Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(typ);

			double Takt=0;
			if(Taktunit==MIN)  Takt=rEditNum_takt->Value*60.0; else Takt=rEditNum_takt->Value;
			Form1->d.v.PP.TT=Takt;

			if(volat_aktualizaci)
			{
				Form1->d.v.aktualizace_objektu(aktualizace_id);
				//doplnil 1.2.2018 M, aktualizace i èasových os a popø. ROMA, nutná z dùvodu zmìny parametrù objektu
				//zvážit zda neimplementovat rovnou do aktualizace objektu, ale zatím nejde z dùvodu, že se volá ve Form1 z dùvodu "falešného" pøekreslední èasových os pøed ROMA
				Form1->aktualizace_maro_a_roma();//aktualizace a pøepoèet volaná kvùli èasovým osám (maro) a techn.procesùm(roma)
			}

			Form1->DuvodUlozit(true);
			//M toto tu nesmí být:Form_parametry_linky->Close();
		}
		//M - následující vìtev nechápu
//		else//stisknul storno - zustavam na PL a nic jsem neulozil
//		{
//			 Form1->DuvodUlozit(false);
//		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_ADD_Click(TObject *Sender)
{
	//navýší poèet øádkù
	rStringGridEd_tab_dopravniky->RowCount++;
	//zobrazOramovani=false;

	//zaène plnit jednotlivé øádky
	int i = rStringGridEd_tab_dopravniky->RowCount - 1;//poøadí øádku o jednièku nižší než poèet øádkù
	rStringGridEd_tab_dopravniky->Cells[0][i] = getMaxPID()+1;
	rStringGridEd_tab_dopravniky->Cells[1][i] = "nový pohon "+AnsiString(getMaxPID());//rStringGridEd_tab_dopravniky->Cells[1][i - 1];
	rStringGridEd_tab_dopravniky->Cells[2][i] = "";//rStringGridEd_tab_dopravniky->Cells[2][i - 1];
	rStringGridEd_tab_dopravniky->Cells[3][i] = "";//rStringGridEd_tab_dopravniky->Cells[3][i - 1];
	rStringGridEd_tab_dopravniky->Cells[4][i] = "";//rStringGridEd_tab_dopravniky->Cells[4][i - 1];
	rStringGridEd_tab_dopravniky->Cells[5][i] = "";
	rStringGridEd_tab_dopravniky->Cells[6][i] = "";
	rStringGridEd_tab_dopravniky->Cells[7][i] = "";
	rStringGridEd_tab_dopravniky->Cells[8][i] = "nepoužíván";

	//doplní min Rz
	show_min_Rz();

	//pøesune focus na poslední øádek
	rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;

	//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	pozice_scGPGlyphButton_hint();

	//existuje nepoužívaný pohon a je tedy vhodné nabídku na smazání nepoužitých zobrazovat
	scGPGlyphButton_DEL_nepouzite->Visible=true;
 // input_clicked_edit=od_klik;
	INPUT(2,i);
	OUTPUT(0,2,i);
	rStringGridEd_tab_dopravniky->Invalidate();
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//smaže poslední øádek - již se nepoužívá, ale nechvám
void __fastcall TForm_parametry_linky::Button_DEL_Click(TObject *Sender)
{
	if(Form1->d.v.pohon_je_pouzivan(rStringGridEd_tab_dopravniky->RowCount-1))
	{
		AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(rStringGridEd_tab_dopravniky->RowCount-1),true);
				if(mrOk==Form1->MB("Pohon je používán pro objekty: <b>"+objekty+"</b>. Opravdu má být pohon smazán?",MB_OKCANCEL)){

				//pùvodní zakomentovaná konstrukcenefunguje správnì pro pøípad storna, proto øeší následující øádek, Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(rStringGridEd_tab_dopravniky->RowCount-1));
				zrusena_prirazeni_PID[getPID(rStringGridEd_tab_dopravniky->RowCount-1)-1]=true;
				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

					if(rStringGridEd_tab_dopravniky->RowCount>1)
					{
					rStringGridEd_tab_dopravniky->RowCount--;
					}
				 //	Form1->MB("Smazano");
				}
				else { //storno   - nic se nedìje

						}
	}
	else// pohon neni pouzivany, mohu ho smazat cokoliv ze stringgridu
	{
		rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();
		if(rStringGridEd_tab_dopravniky->RowCount>1)
		{
			rStringGridEd_tab_dopravniky->RowCount--;
		}
	}

	 //	for (long i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
	 //	rStringGridEd_tab_dopravniky->Cells[0][i] = i;

	//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	pozice_scGPGlyphButton_hint();
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
//zobrazí panel se navrženými pohony
void __fastcall TForm_parametry_linky::scGPButton_doporuceneClick(TObject *Sender)
{
		scExPanel_doporuc_pohony->Visible=true;
		scGPButton_doporucene->Visible=false;
		if(F->pom==NULL)//pokud je voláno PL pøímo                        //zajistí zobrazení ve stejných jednotkách jako na PO
		scHTMLLabel_doporuc_pohony->Caption=F->d.v.navrhni_POHONY("</br>",F->ms.a2i(F->readINI("nastaveni_form_parametry", "RDt")));
		else// pokud je PL voláno z PO                                    //zajistí zobrazení ve stejných jednotkách jako na PO
		scHTMLLabel_doporuc_pohony->Caption=F->d.v.navrhni_POHONY("</br>",Form_parametry->RDunitT);
		if(scHTMLLabel_doporuc_pohony->Caption=="")
		{
			scHTMLLabel_doporuc_pohony->Caption="Nejsou k dispozici žádné navržené pohony";
			scGPGlyphButton_add_mezi_pohony;
			scGPGlyphButton_add_mezi_pohony->Visible=false;
		}
		else
		{
			scGPGlyphButton_add_mezi_pohony->Visible=true;
		}
		//šíøka komponenty dle aktuálnì zobrazeného textu
		//Canvas->Font=scExPanel_doporuc_pohony->Font;
		//scExPanel_doporuc_pohony->Width=Canvas->TextWidth(Form1->ms.TrimLeftFrom_UTF(scHTMLLabel_doporuc_pohony->Caption," </br>"));
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scExPanel_doporuc_pohonyClose(TObject *Sender)

{
scExPanel_doporuc_pohony->Visible=false;
scGPButton_doporucene->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_takt_Change(TObject *Sender)
{

	if(input_state==NOTHING && input_clicked_edit==TT_klik)
	{

	input_TT();

	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_delkavozikuClick(TObject *Sender)
{
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_sirka_jigClick(TObject *Sender)
{
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_taktClick(TObject *Sender)
{
//

	if(Taktunit==MIN)//pokud je v MM, tak pøepne na metry
	{
		Taktunit=S;
		//delka - pøepoèítání
		rEditNum_takt->Value=rEditNum_takt->Value*60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[s]</font>";
	}
	else//metrech tak se pøepne na MM
	{
		Taktunit=MIN;
		//delka - pøepoèítání
		rEditNum_takt->Value=rEditNum_takt->Value/60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[m]</font>";
	}

}
//---------------------------------------------------------------------------
//tlaèítko na kopírování doporuèených pohonù do striggridu, nepøidává ale do pohonù
void __fastcall TForm_parametry_linky::scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender)
{
	 //najde max použité ID pohonu (protože ID nejsou seøazena, nutno hledat, nikoliv vzít id z posledního øádku)
	 unsigned int ID=getMaxPID();

	 //nová konstrukce zajišující pouze vložení do stringgridu, o samotné uložení pohonù se stará až tlaèítko uložit
	 AnsiString T=scHTMLLabel_doporuc_pohony->Caption;

	 while(T.Pos("</br>"))//bude parsovat dokud bude </br>
	 {
	 	//zvýšení poètu øádkù
   	rStringGridEd_tab_dopravniky->RowCount++;
   	unsigned int i=rStringGridEd_tab_dopravniky->RowCount-1;//pouze zkrácení zápisu
	 	//plnìní øádku a parsování daty
		rStringGridEd_tab_dopravniky->Cells[0][i]=++ID;//ID
		rStringGridEd_tab_dopravniky->Cells[1][i]="Navržený pohon pro"+Form1->ms.EP(T,":",".");//název pohonu a vyèeštìní øetìzce pro další užití
		rStringGridEd_tab_dopravniky->Cells[2][i]=Form1->ms.EP(T,"Navržený pohon s rychlostí"," [");//T=Form1->ms.TrimLeftFrom_UTF(T," </br>");
		rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
		//pro aRD až bude sloupec: rStringGridEd_tab_dopravniky->Cells[?][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
		rStringGridEd_tab_dopravniky->Cells[8][i]="nepoužíván";
		//smazání jednoho již nepotøebného záznamu
		T=Form1->ms.TrimLeftFromText(T,"</br>");
	 }
	 //smazání již nahraných a skrytí panelu
	 scHTMLLabel_doporuc_pohony->Caption="Nejsou k dispozici žádné navržené pohony";//nepoužívá se tento výpis viditelnì, situace nenastane
	 scExPanel_doporuc_pohony->Visible=false;
	 scGPButton_doporucene->Visible=true;
	 //pøesune focus na poslední øádek
	 rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;
	 //pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	 pozice_scGPGlyphButton_hint();
	 //existují urèitì nepoužíté pohony a je tedy vhodné nabídku na smazání nepoužitých zobrazovat
	 scGPGlyphButton_DEL_nepouzite->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key==13)//ENTER
	{
		 if(Button_save->Enabled)Button_saveClick(Sender);//pokud jsou zároveò splnìny podmínky pro stisk OK
		 else MessageBeep(0);//pípnutím upozorní, že nelze
	}
	if(Key==27)//ESC
	{
		 if(PopUPmenu->Visible)//pokud je zobrazeno pop-up, tak skryje to
		 PopUPmenu->Visible=false;
		 else //jinak ukonèení formuláøe
		 Button_stornoClick(Sender);
	}
	if(Key==123)//F12
	{

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
	if(Col==5) Roletka_roztec(Row);

	 // pokud je pohon ve stavu používán, vygeneruji roletku která umožní provést zmìnu na nepoužíván

		if(Col==8 && rStringGridEd_tab_dopravniky->Cells[8][Row]!="nepoužíván") {

			 EditStyle=sgbDropDown;
			 rStringGridEd_tab_dopravniky->Columns->Items[8]->PickList->Clear();
			 rStringGridEd_tab_dopravniky->Columns->Items[8]->PickList->Add("nepoužíván");
		}



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
		if (Col==6 && Row==0) {
			CanEdit=false;
			 }
		if (Col==7 && Row==0) {
			CanEdit=false;
			 }
		if (Col==8 && Row==0) {
			CanEdit=false;
			 }

		//Pokud je pohon používán, nastavím položky rychlost, rozteè atd jako readonly - nelze je mìnit.
//	 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
//	 {
//		 if(rStringGridEd_tab_dopravniky->Cells[5][i]=="" && rStringGridEd_tab_dopravniky->Cells[8][i]=="nepoužíván" )
//		 {
//			// if(Row==i && Col==6) CanEdit=false;      //nakonec je vždy povoleno editovat
//		 //	 if(Row==i && Col==7) CanEdit=false;
//		 }
//
//	 }

			for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
			 {
					 //aRD
					 if(Row==i && Col==4  && scGPButton_zamek_aRD->ImageIndex==37)CanEdit=false;
					 if(Row==i && Col==4  && scGPButton_zamek_aRD->ImageIndex==38)CanEdit=true;

					 //R
					 if(Row==i && Col==5  && scGPButton_zamek_roztec->ImageIndex==37)CanEdit=false;
					 if(Row==i && Col==5  && scGPButton_zamek_roztec->ImageIndex==38)CanEdit=true;

					 //Rz
					 if(Row==i && Col==6  && scGPButton_zamek_Rz->ImageIndex==37)CanEdit=false;
					 if(Row==i && Col==6  && scGPButton_zamek_Rz->ImageIndex==38)CanEdit=true;

					 //Rx
					 if(Row==i && Col==7  && scGPButton_zamek_Rx->ImageIndex==37)CanEdit=false;
					 if(Row==i && Col==7  && scGPButton_zamek_Rx->ImageIndex==38)CanEdit=true;

//					 if(Row==i && Col==4 &&  scGPButton_zamek_Rz->ImageIndex==37)
//					 {scGPButton_zamek_Rz->Visible=true; /*ShowMessage("kuk"); */  }

			 }


			 			 if(VID==1 || VID==2 || VID==3 || VID==23 || VID==4 || VID==5 || VID==6 || VID==7){

			 //pokud se nejedná o øádek, kde právì dochází k validaci a zároveò vynechám nultý øádek (hlavièka)
			 //tak do všech sloupcù dám šedou barvu

			 if(Row!=Row_validace && Row!=0 && Col==1)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==2)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==3)  	  CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==4)  	  CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==5)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==6)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==7)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==8)  		CanEdit=false;


			 }




}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rEditNum_delkavozikuClick(TObject *Sender)
{
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_delka_jigClick(TObject *Sender)
{
	if(Delkaunit==MM)//pokud je v MM, tak pøepne na metry
	{
		Delkaunit=M;
		//delka - pøepoèítání

		rHTMLLabel_delka_jig->Caption="délka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value/1000.0;


		rHTMLLabel_sirka_jig->Caption="šíøka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value/1000.0;

		rHTMLLabel_delka_podvozek->Caption="délka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_podvozek->Value=scGPNumericEdit_delka_podvozek->Value/1000.0;

	}
	else//metrech tak se pøepne na MM
	{
		Delkaunit=MM;

		rHTMLLabel_delka_jig->Caption="délka <font color=#2b579a>[mm]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value*1000.0;


		rHTMLLabel_sirka_jig->Caption="šíøka <font color=#2b579a>[mm]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value*1000.0;

		rHTMLLabel_delka_podvozek->Caption="délka <font color=#2b579a>[mm]</font>";
		scGPNumericEdit_delka_podvozek->Value=scGPNumericEdit_delka_podvozek->Value*1000.0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_delka_vozikuClick(TObject *Sender)
{
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_DELMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
//	if(rStringGridEd_tab_dopravniky->RowCount==1) Button_DEL->Enabled=false;
 //		else  Button_DEL->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rEditNum_delka_jigu_oldKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
	FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rEditNum_sirka_jigu_OLDKeyDown(TObject *Sender,
					WORD &Key, TShiftState Shift)
{
	FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rEditNum_takt_OLDKeyDown(TObject *Sender, WORD &Key,
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
void __fastcall TForm_parametry_linky::FormPaint(TObject *Sender)
{

	//F->d.vykresli_packy_PL(Canvas,input_state,scGPButton_zamek_aRD->ImageIndex,scGPButton_zamek_roztec->ImageIndex,scGPButton_zamek_Rz->ImageIndex,scGPButton_zamek_Rx->ImageIndex);
	show_min_Rz();
	if(zobrazitFrameForm)Form1->m.frameForm(Form_parametry_linky,clWebOrange,1);

	if(VID==-1) scGPGlyphButton_ADD->Enabled=true;
	else        scGPGlyphButton_ADD->Enabled=false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry_linky::show_min_Rz()
{
// double min_mezera_voziky;
// double rotace;
// scGPGlyphButton_hint_Rz->Enabled=true;
//// if(Delkaunit=M) jednotky*1000.0; else jednotky*1.0;
//
// for(int i=1;i<=rStringGridEd_tab_dopravniky->RowCount;i++)
// {
//	 if(!rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty())
//	 {
//	 min_mezera_voziky= Form1->m.mezera_mezi_voziky(rEditNum_delka_jigu->Value*1000,rEditNum_sirka_jigu->Value*1000,0,Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]),0);
//
//	// ShowMessage(min_mezera_voziky);
//
//		if(rEditNum_delka_jigu->Value<rEditNum_sirka_jigu->Value)
//		{
//		rotace=0;
//		scGPGlyphButton_hint_Rz->Hint="Min. Rz = minimální rozteèová vzdálenost,poèítaná na základì zadané rozteèe a šíøky vozíku.";
//		}
//		else
//		{
//		rotace=90;
//		scGPGlyphButton_hint_Rz->Hint="Min. Rz = minimální rozteèová vzdálenost,poèítaná na základì zadané rozteèe a délky vozíku.";
//		}
//
//	// rStringGridEd_tab_dopravniky->Cells[6][i]= Form1->m.Rz(rEditNum_delka_jigu->Value,rEditNum_sirka_jigu->Value,rotace,min_mezera_voziky/1000);
//
//	 }
// }
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPNumericEdit_sirka_jigChange(TObject *Sender)
{
	show_min_Rz();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPNumericEdit_delka_jigChange(TObject *Sender)
{
	show_min_Rz();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zavolá náhled kabiny, pøípadnì v budoucnu info a o požadovaných parametrech
void __fastcall TForm_parametry_linky::scGPGlyphButton_infoClick(TObject *Sender)
{
		bool zFFtemp=false;if(zobrazitFrameForm){zFFtemp=true;zobrazitFrameForm=false;Invalidate();}//pokud je orámování, tak zruší, aby mohlo mít orámování jen na formu kabina_schema, ale zapamatuje si stav pro následné navrácení
		// formuláø na støed
		if(!Form_objekt_nahled->Visible)
		{
			Form_objekt_nahled->Left = Form1->ClientWidth / 2 - Form_objekt_nahled->Width / 2;
			Form_objekt_nahled->Top = Form1->ClientHeight / 2 - Form_objekt_nahled->Height / 2;
			// zobrazeni formuláøe
			Form_objekt_nahled->scLabel_titulek->Visible=false;
			Form_objekt_nahled->zobrazitFrameForm=true;
			Form_objekt_nahled->ShowModal();
			if(zFFtemp)zobrazitFrameForm=true;//pokud bylo orámování, tak vrátí
		}
		else  Form_objekt_nahled->Show();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vrátí ID pohonu na daném øádku
unsigned int TForm_parametry_linky::getPID(int ROW)
{
	try
	{
		return rStringGridEd_tab_dopravniky->Cells[0][ROW].ToInt();
	}
	catch(...)
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
//najde max použité ID pohonu (protože ID nejsou seøazena,nelze vzít index posledního øádku)
unsigned int TForm_parametry_linky::getMaxPID()
{
	unsigned int ID=0;//id
	for(unsigned i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	if(ID<getPID(i))ID=getPID(i);
	return ID;
}
//---------------------------------------------------------------------------
void TForm_parametry_linky::zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum()
{   //pøeindexovat nesmazané nebo jim dat nový odkaz, nebo mazat jen konkrétní
	for(unsigned PID=0;PID<zrusena_prirazeni_PID_size;PID++)
	{
		if(zrusena_prirazeni_PID[PID])
		Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(PID+1);
	}
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
}
//---------------------------------------------------------------------------
//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
void TForm_parametry_linky::pozice_scGPGlyphButton_hint()
{

}
//---------------------------------------------------------------------------
//testuje zda existují nepoužíté pohony, pokud ano,vrací true jinak false
//musí být zde, nikoliv ve vektorech, protože zde mohou vznikat novéh návrhy na pohony, které nejsou ještì ve spojáku POHONY
bool TForm_parametry_linky::existuji_nepouzivane_pohony()
{
	 bool RET=false;
	 for(unsigned int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)//prochází všechny pohany a pokud je pohon nepoužíván, smažeho
	 {
		if(!Form1->d.v.pohon_je_pouzivan(getPID(i)))//pohon není používaný
		{
			RET=true;
			break;//staèí najít jeden
		}
	 }
	 return RET;
}
//---------------------------------------------------------------------------
//-------------------------POP-UP MENU---------------------------------------
//---------------------------------------------------------------------------
//volání pop-up menu
void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyMouseDown(TObject *Sender,
					TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 if(Button==mbRight && rStringGridEd_tab_dopravniky->Row>0)//je stisknuto pravé tlaèítko myši ve stringridu, tzn. volat popupmenu
	 {
			PopUPmenu->Height=34*2;//zobrazené dvì položky
			PopUPmenu->Left=X;PopUPmenu->Top=Y+rStringGridEd_tab_dopravniky->Top;//pozice
			//nastávení textu polože
			scLabel_kopirovat->Caption="  Kopírovat "+rStringGridEd_tab_dopravniky->Cells[1][rStringGridEd_tab_dopravniky->Row];
			scLabel_smazat->Caption="  Smazat "+rStringGridEd_tab_dopravniky->Cells[1][rStringGridEd_tab_dopravniky->Row];
			//testuje zda existují nepoužíté pohony a je tedy vhodné nabídku na smazání nepoužitých zobrazovat
			if(existuji_nepouzivane_pohony())
			{
				PopUPmenu->Height=34*3;//zobrazené již tøi položky
				Item_smazat_nepouzite->Visible=true;
				scGPGlyphButton_DEL_nepouzite->Visible=true;
			}
			else
			{
				Item_smazat_nepouzite->Visible=false;
				scGPGlyphButton_DEL_nepouzite->Visible=false;
			}
			//ošetøení, pokud je mimo obrazovku + 5 px okraj
			if(PopUPmenu->Left>=ClientWidth-PopUPmenu->Width)//nastala situace že je mimo obraz (nebo èásteènì)
			PopUPmenu->Left=ClientWidth-PopUPmenu->Width-5;
			if(PopUPmenu->Top>=rStringGridEd_tab_dopravniky->Top+rStringGridEd_tab_dopravniky->Height-PopUPmenu->Height)
			PopUPmenu->Top=rStringGridEd_tab_dopravniky->Top+rStringGridEd_tab_dopravniky->Height-PopUPmenu->Height-5;
			PopUPmenu->Visible=true;
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//køížek, který skryje pop-up menu
void __fastcall TForm_parametry_linky::GlyphButton_closeClick(TObject *Sender)
{
	PopUPmenu->Visible=false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//hlídání horní pozice, je-li daná komponenta horní kvùli nastavení køížku
void TForm_parametry_linky::top_positon(int top)
{
	if(top==0)
	{
		GlyphButton_close->Options->NormalColor=clAcBg2;
		GlyphButton_close->Options->FocusedColor=clAcBg2;
		GlyphButton_close->Options->FrameFocusedColor=clAcBg2;
		GlyphButton_close->Options->FrameNormalColor=clAcBg2;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajišuje zkopírování pohonu, vloží na konec, nevkládáme za kopírovaný objekt, protože podle poøadí zùstává pøiøaøení k jednotlivým objektùm
void __fastcall TForm_parametry_linky::scLabel_kopirovatClick(TObject *Sender)
{
	int ROW=rStringGridEd_tab_dopravniky->Row;
	rStringGridEd_tab_dopravniky->InsertRowEx(ROW+1,false);
	//zkopírování øádku za kopírovaný pohon
	rStringGridEd_tab_dopravniky->Rows[ROW+1]=rStringGridEd_tab_dopravniky->Rows[ROW];
	//název pøejmenování
	rStringGridEd_tab_dopravniky->Cells[1][ROW+1]=rStringGridEd_tab_dopravniky->Cells[1][ROW]+" - kopie";
	//nepoužíván
	rStringGridEd_tab_dopravniky->Cells[8][ROW+1]="nepoužíván";
	//indexace
	rStringGridEd_tab_dopravniky->Cells[0][ROW+1]=getMaxPID()+1;
	//pøesune focus na poslední øádek
	rStringGridEd_tab_dopravniky->Row=ROW+1;
	//skrytí pop-up menu
	PopUPmenu->Visible=false;
	//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	pozice_scGPGlyphButton_hint();
	//existuje nepoužívaný pohon a je tedy vhodné nabídku na smazání nepoužitých zobrazovat
	scGPGlyphButton_DEL_nepouzite->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_kopirovatMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_kopirovat->FillColor=clAcBg;
	GlyphButton_kopirovat->Options->NormalColor=clAcBg;
	GlyphButton_kopirovat->Options->HotColor=clAcBg;
	GlyphButton_kopirovat->Options->FocusedColor=clAcBg;
	GlyphButton_kopirovat->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_kopirovat->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_kopirovat->Top);//hlídání horní pozice, je-li daná komponenta horní kvùli nastavení køížku
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_kopirovatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_kopirovatMouseEnter(TObject *Sender)
{
	scLabel_kopirovatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_kopirovatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajišuje smázání pohonu na vybraném øádku
void __fastcall TForm_parametry_linky::scLabel_smazatClick(TObject *Sender)
{
	int ROW=rStringGridEd_tab_dopravniky->Row;
	bool smazat=false;
	if(Form1->d.v.pohon_je_pouzivan(getPID(ROW)))//pohon je používaný
	{
			AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(ROW),true);
			myMessageBox->zobrazitFrameForm=true;//zajistí orámování MB
			if(mrYes==Form1->MB("Pohon je používán objekty: <b>"+objekty+"</b>. Opravdu má být pohon smazán?",MB_YESNO))
			{
				//Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(ROW)); pùvodní pøímé smazání, ale nereflektovalo by pøípadné storno
				//pozor není pøipraveno na situaci, pokud by bylo možné pøímo v PL pøiøazovan pohony a potom zase odpøiøazovat (muselo by se navýšit pole zrusena_prirazeni_PID)
				zrusena_prirazeni_PID[getPID(ROW)-1]=true;//nahrazeno novou filozofii, z dùvodu možného storna formu
				smazat=true;
			}
			myMessageBox->zobrazitFrameForm=false;//zajistí odorámování MB - kvùli dalšímu použití
	}
	else//pohon není používaný a mùžeme tedy smazat rovnou
	{
		smazat=true;
	}
	//samotné smazání øádku + zajistí snížení poètu øádkù + nesmí se pøeindexovávat!!! kvùli metodám, které sahají do spojáku POHONY
	if(smazat)rStringGridEd_tab_dopravniky->DeleteRowEx(ROW);
	//skrytí pop-up menu
	PopUPmenu->Visible=false;
	//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	pozice_scGPGlyphButton_hint();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazatMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_smazat->FillColor=clAcBg;
	GlyphButton_smazat->Options->NormalColor=clAcBg;
	GlyphButton_smazat->Options->HotColor=clAcBg;
	GlyphButton_smazat->Options->FocusedColor=clAcBg;
	GlyphButton_smazat->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_smazat->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_smazat->Top);//hlídání horní pozice, je-li daná komponenta horní kvùli nastavení køížku
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazatMouseEnter(TObject *Sender)
{
	scLabel_smazatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//prochází všechny pohany a pokud je pohon nepoužíván, smaže ho
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteClick(TObject *Sender)
{
	for(int j=1;j<rStringGridEd_tab_dopravniky->RowCount;j++)//prochází všechny pohany a pokud je pohon nepoužíván, smažeho
	{
		if(Form1->d.v.pohon_je_pouzivan(getPID(j))==false)//pohon není používaný
		{
			//samotné smazání øádku + zajistí snížení poètu øádkù + nesmí se pøeindexovávat!!! kvùli metodám, které sahají do spojáku POHONY
			rStringGridEd_tab_dopravniky->DeleteRowEx(j);
			j--;//musí po smazání nutnì snížit index
		}
	}
	//skrytí pop-up menu
	PopUPmenu->Visible=false;
	rStringGridEd_tab_dopravniky->FinishEditing();//ukonèí editaci a tím odstraní nepøíjemný vizuální efekt
	pozice_scGPGlyphButton_hint();//pozice info tlaèítka - asi je tlaèítko stejnì provizorní
	//neexistuje nepoužívaný pohon a je tedy vhodné nabídku na smazání nepoužitých nezobrazovat
	scGPGlyphButton_DEL_nepouzite->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_smazat_nepouzite->FillColor=clAcBg;
	GlyphButton_smazat_nepouzite->Options->NormalColor=clAcBg;
	GlyphButton_smazat_nepouzite->Options->HotColor=clAcBg;
	GlyphButton_smazat_nepouzite->Options->FocusedColor=clAcBg;
	GlyphButton_smazat_nepouzite->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_smazat_nepouzite->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_smazat_nepouzite->Top);//hlídání horní pozice, je-li daná komponenta horní kvùli nastavení køížku
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazat_nepouziteMouseEnter(TObject *Sender)
{
	scLabel_smazat_nepouziteMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazat_nepouziteMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyGetCellParams(TObject *Sender,
					int Col, int Row, TFont *AFont, TColor &Background, bool &Highlight)
{
//Memo3->Lines->Add(VID);
 // vynechá první øádek a obarví sloupec tak jak má - pokud je zamèen zámek, je šedivý sloupec a nastaví se,
 // že není možné sloupec editovat - to se nastavuje v metodì rStringGridEd_tab_dopravnikyCanEdit

				for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
			 {
					 //aRD
					 if(Row==i && Col==4  && scGPButton_zamek_aRD->ImageIndex==37)Background=(TColor)RGB(212,212,212);
					 if(Row==i && Col==4  && scGPButton_zamek_aRD->ImageIndex==38)Background=(TColor)RGB(255,255,255);

					 //R
					 if(Row==i && Col==5  && scGPButton_zamek_roztec->ImageIndex==37)Background=(TColor)RGB(212,212,212);
					 if(Row==i && Col==5  && scGPButton_zamek_roztec->ImageIndex==38)Background=(TColor)RGB(255,255,255);

					 //Rz
					 if(Row==i && Col==6  && scGPButton_zamek_Rz->ImageIndex==37)Background=(TColor)RGB(212,212,212);
					 if(Row==i && Col==6  && scGPButton_zamek_Rz->ImageIndex==38)Background=(TColor)RGB(255,255,255);

					 //Rx
					 if(Row==i && Col==7  && scGPButton_zamek_Rx->ImageIndex==37)Background=(TColor)RGB(212,212,212);
					 if(Row==i && Col==7  && scGPButton_zamek_Rx->ImageIndex==38)Background=(TColor)RGB(255,255,255);


				 }

				 if(VID==1 || VID==2 || VID==3 || VID==23 || VID==4 || VID==5 || VID==6 || VID==7)
				 {

			 //pokud se nejedná o øádek, kde právì dochází k validaci a zároveò vynechám nultý øádek (hlavièka)
			 //tak do všech sloupcù dám šedou barvu

			 if(Row!=Row_validace && Row!=0 && Col==1)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==2)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==3)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==4)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==5)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==6)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==7)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==8)  			 Background=(TColor)RGB(212,212,212);


       }

}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scHTMLLabel_doporuc_pohonyClick(TObject *Sender)
{
		// zavolá funkcionalitu tlaèítka na kopírování navržených pohonù do striggridu, nepøidává ale do pohonù
	scGPGlyphButton_add_mezi_pohonyClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPSwitchChangeState(TObject *Sender)
{
if(scGPSwitch->State==0) {rHTMLLabel_podvozek_zaves->Caption="Podvozek";rHTMLLabel_podvozek_zaves->Left=34;}
else  {rHTMLLabel_podvozek_zaves->Caption="Závìs"; rHTMLLabel_podvozek_zaves->Left=56; }
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::FormClose(TObject *Sender, TCloseAction &Action)

{
	//ShowMessage(Delkaunit);
	Form1->writeINI("nastaveni_form_parametry_linky", "rozmery", Delkaunit);
	Form1->writeINI("nastaveni_form_parametry_linky", "TT", Taktunit);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void TForm_parametry_linky::vypis(UnicodeString text,bool red,bool link)
{
		Button_save->Enabled=true;
		Button_save->Caption = "Uložit";
		if (text != "") // zobrazí a vypíše
		{
				rHTMLHint1->ToString()=text;//natežení do hintu zajišuje zobrazení celého textu, nepoužívá se klasický hint
				//prodllužení formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl pøedtím popisek skrytý + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnutí podtrženého písma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
						Button_save->Enabled=false;
						rHTMLLabel_InfoText->Font->Color = clRed;
				}
				else
				{
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
				rHTMLLabel_InfoText->Left = scGPGlyphButton_ADD->Left+ scGPGlyphButton_ADD->Width + 2;
				rHTMLLabel_InfoText->Top = Form_parametry_linky->Height - 74;
				rHTMLLabel_InfoText->Caption = text;
				rHTMLLabel_InfoText->Visible = true;
		}
		else // skryje
		{
				//zkrácení formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
				rHTMLLabel_InfoText->Visible = false;
		}


}


void __fastcall TForm_parametry_linky::rEditNum_taktClick(TObject *Sender)
{
//
		 input_clicked_edit=TT_klik;
		 input_clicked_icon=empty_klik_ico;
		 Nastav_zamky(empty_klik_ico,TT_klik);

		Form_TT_kalkulator->Left=Form1->ClientWidth/2-Form_TT_kalkulator->Width/2;
		Form_TT_kalkulator->Top=Form1->ClientHeight/2-Form_TT_kalkulator->Height/2;

		 //pøekreslení podbarvení sloupcù
		 rStringGridEd_tab_dopravniky->Visible=false;
		 rStringGridEd_tab_dopravniky->Visible=true;

	


		 vypis("Pozor, pøi zmìnì taktu dojde pøi uložení ke zmìnì hodnot aktuální rychlosti pohonu nebo rozteèové vzdálenosti a dalších parametrù dle nastavených zámkù v tabulce pohonù. ",false);
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::Nastav_zamky(Tinput_clicked_icon I,Tinput_clicked_edit E)
{

 if(I==empty_klik_ico && E==empty_klik) //výchozí nastavení zámkù - pøi formshowformu
 {

		scGPButton_zamek_Rz->Visible=true;

		scGPButton_zamek_aRD->ImageIndex=37; // zamknu aRD
		scGPButton_zamek_Rz->ImageIndex=37; // zamknu Rz - jsou v korelaci

		//odemèeno  Rx a Rz
		scGPButton_zamek_Rx->ImageIndex=38;
		scGPButton_zamek_roztec->ImageIndex=38;
 }

 ///////////////////////////////////////////////

	if(I==empty_klik_ico && E==TT_klik) // nastavení zámkù pøi kliku do TT
 {
		//	 ShowMessage("nastav");
		scGPButton_zamek_aRD->ImageIndex=37; // zamknu aRD
		scGPButton_zamek_Rz->ImageIndex=38; // odemknu Rz

		//odemèeno  Rx a Rz
		scGPButton_zamek_Rx->ImageIndex=38;
		scGPButton_zamek_roztec->ImageIndex=38;
 }

	if(I==aRD_klik_ico && E==aRD_klik) // nastavení zámkù pøi kliku do aRD s pøedchozím vstupem do aRD - souvisí s formshow
 {
		//	 ShowMessage("nastav");
		if(scGPButton_zamek_aRD->ImageIndex==37)
		{
				scGPButton_zamek_aRD->ImageIndex=38;
				scGPButton_zamek_Rx->ImageIndex=38;
				scGPButton_zamek_roztec->ImageIndex=37;



		} else
		{

				scGPButton_zamek_aRD->ImageIndex=37;
				scGPButton_zamek_Rx->ImageIndex=38;
				scGPButton_zamek_roztec->ImageIndex=37;

		}
 }

 ///////////////////////////////////////////////////////////////

 if(I==aRD_klik_ico && E==TT_klik) {  //nastavení zámkù pøi kliku do TT  a zaènu mìnit zámek aRD


 	if(scGPButton_zamek_aRD->ImageIndex==37) //odemèeno
			{
				scGPButton_zamek_aRD->ImageIndex=38; // odemknu aRD
				scGPButton_zamek_Rz->ImageIndex=37; // zamknu Rz

				 //odemèeno roztec
				scGPButton_zamek_roztec->ImageIndex=38; //odemèeno
				scGPButton_zamek_Rx->ImageIndex=38;
			}
			else
			{
			//odemèeno
			 scGPButton_zamek_aRD->ImageIndex=37;  //zamknu aRD
			 scGPButton_zamek_Rz->ImageIndex=38;   //odemknu Rz

				 //odemceno  roztec   a Rx
				scGPButton_zamek_roztec->ImageIndex=38;
				scGPButton_zamek_Rx->ImageIndex=38;


			}
 }

 ///////////////////////////////////////////////////////////////////////


	if(I==aRD_klik_ico && E==empty_klik)   //obecné nastavení zámku aRD bez ovlivnìní vstupu TT
	{


			if(scGPButton_zamek_aRD->ImageIndex==37) //zamèeno
			{
				scGPButton_zamek_aRD->ImageIndex=38; // odemknu aRD
				scGPButton_zamek_Rz->ImageIndex=38; // odemknu Rz - jsou v korelaci

				scGPButton_zamek_roztec->ImageIndex=37; //zamèeno
				scGPButton_zamek_Rx->ImageIndex=38;      //odemèeno
			}
			else
			{
			//zamknu
			 scGPButton_zamek_aRD->ImageIndex=37;
			 scGPButton_zamek_Rz->ImageIndex=37;

				 //odemknu  roztec   a zamknu Rx
				scGPButton_zamek_roztec->ImageIndex=38;     //odemèeno
				scGPButton_zamek_Rx->ImageIndex=37;       //zamèeno


			}

	}
		 // nastavení chování zámku Rx, v pøípadì, že pøedtím došlo ke vstupu do buòky aRD

	//Nastav_zamky(Rx_klik_ico,aRD_klik)

	if(E==aRD_klik) {

					 //Rz není vidìt a lze jej mìnit
				// scGPButton_zamek_Rz->ImageIndex=38;
			 //	 scGPButton_zamek_Rz->Visible=false;

	}
//////////////////////////////////////////////////////////////////
																				 //	Nastav_zamky(Rx_klik_ico,aRD_klik)
	if(I==Rx_klik_ico && E==aRD_klik)
	{

			 if(scGPButton_zamek_Rx->ImageIndex==37)
			 {

				 scGPButton_zamek_Rx->ImageIndex=38;
				 scGPButton_zamek_roztec->ImageIndex=37;  //zamknu R

				 //Rz není vidìt a lze jej mìnit
				 scGPButton_zamek_Rz->ImageIndex=38;
				// scGPButton_zamek_Rz->Visible=false;

			 } else

			 {

				 scGPButton_zamek_Rx->ImageIndex=37;
				 scGPButton_zamek_roztec->ImageIndex=38;  //odemknu R

				 //Rz není vidìt a lze jej mìnit
				 scGPButton_zamek_Rz->ImageIndex=38;
			 //	 scGPButton_zamek_Rz->Visible=false;
			 }


	}


		if(I==R_klik_ico && E==aRD_klik)
	{

			 if(scGPButton_zamek_roztec->ImageIndex==37)
			 {

				 scGPButton_zamek_roztec->ImageIndex=38;
				 scGPButton_zamek_Rx->ImageIndex=37;  //zamknu Rx

				 //Rz není vidìt a lze jej mìnit
				 scGPButton_zamek_Rz->ImageIndex=38;
				// scGPButton_zamek_Rz->Visible=false;

			 } else

			 {

				 scGPButton_zamek_Rx->ImageIndex=38;
				 scGPButton_zamek_roztec->ImageIndex=37;  //odemknu R

				 //Rz není vidìt a lze jej mìnit
				 scGPButton_zamek_Rz->ImageIndex=38;
			 //	 scGPButton_zamek_Rz->Visible=false;
			 }


	}
			 if(I==aRD_klik_ico && E==R_klik)
			 {
				 //ShowMessage("ted");
				if(scGPButton_zamek_aRD->ImageIndex==38)
				{

						scGPButton_zamek_aRD->ImageIndex=37;
						scGPButton_zamek_Rz->ImageIndex=37;

						scGPButton_zamek_Rx->ImageIndex=38;

				}   else

				{
						scGPButton_zamek_aRD->ImageIndex=38;
						scGPButton_zamek_Rz->ImageIndex=38;

						scGPButton_zamek_Rx->ImageIndex=37;

				}

			 }

//				 if(I==Rz_klik_ico && E==Rz_klik) //asi k nicemu kdyby byl zamek po kliku pryc
//			 {
//
//				if(scGPButton_zamek_Rz->ImageIndex==37)
//				{
//						scGPButton_zamek_Rz->ImageIndex=38;
//
//						if(scGPButton_zamek_roztec->ImageIndex==37){
//							 scGPButton_zamek_Rx->ImageIndex=38;
//						 }
//						 else
//						 {
//							scGPButton_zamek_roztec->ImageIndex=38;
//							scGPButton_zamek_Rx->ImageIndex=37;
//						 }
//
//
//				}
//				else
//
//				{
//					scGPButton_zamek_Rz->ImageIndex=37;
//
//          	if(scGPButton_zamek_roztec->ImageIndex==37){
//							 scGPButton_zamek_Rx->ImageIndex=38;
//						 }
//						 else
//						 {
//							scGPButton_zamek_roztec->ImageIndex=38;
//							scGPButton_zamek_Rx->ImageIndex=37;
//						 }
//
//				}
//
//			 }

       if(I==R_klik_ico && E==Rx_klik)
			 {

				if(scGPButton_zamek_roztec->ImageIndex==37)
				{
						scGPButton_zamek_roztec->ImageIndex=38;

						scGPButton_zamek_aRD->ImageIndex=37;
						scGPButton_zamek_Rz->ImageIndex=37;

				}   else

				{
						scGPButton_zamek_roztec->ImageIndex=37;

						scGPButton_zamek_Rz->ImageIndex=38;
						scGPButton_zamek_aRD->ImageIndex=38;
				}

			 }

			//  Nastav_zamky(Rx_klik_ico,Rz_klik);
			//Nastav_zamky(R_klik_ico,Rz_klik);

			 if(I==Rx_klik_ico && E==Rz_klik)
			 {

				if(scGPButton_zamek_roztec->ImageIndex==37)
				{
						scGPButton_zamek_roztec->ImageIndex=38;
						scGPButton_zamek_Rx->ImageIndex=37;


				}   else

				{
						scGPButton_zamek_Rx->ImageIndex=38;
						scGPButton_zamek_roztec->ImageIndex=37;

				}

			 }

			 //-------------------------------------------------------------
				// if(input_clicked_edit==Rx_klik)  Nastav_zamky(aRD_klik_ico, Rx_klik);

				 if(I==aRD_klik_ico && E==Rx_klik)
			 {

				if(scGPButton_zamek_Rx->ImageIndex==37)
				{
						scGPButton_zamek_roztec->ImageIndex=37;
						scGPButton_zamek_Rx->ImageIndex=38;

				}   else

				{
						scGPButton_zamek_roztec->ImageIndex=38;
						scGPButton_zamek_Rx->ImageIndex=37;

				}

			 }



//-----------------------------------------------------
			 if(I==R_klik_ico && E==Rz_klik)
			 {

				if(scGPButton_zamek_roztec->ImageIndex==37)
				{
						scGPButton_zamek_roztec->ImageIndex=38;
						scGPButton_zamek_Rx->ImageIndex=37;


				}   else

				{
						scGPButton_zamek_Rx->ImageIndex=38;
						scGPButton_zamek_roztec->ImageIndex=37;

				}

			 }


			 //Rz nastavení
		 if(I==Rz_klik_ico && E==R_klik)
			 {

				if(scGPButton_zamek_aRD->ImageIndex==37)
				{
						scGPButton_zamek_aRD->ImageIndex=38;
						scGPButton_zamek_Rz->ImageIndex=38;

						scGPButton_zamek_Rx->ImageIndex=37;



				}   else

				{
						scGPButton_zamek_aRD->ImageIndex=37;
						scGPButton_zamek_Rz->ImageIndex=37;

						scGPButton_zamek_Rx->ImageIndex=38;

				}

			 }
	//*************************************************************************
				 if(I==Rz_klik_ico && E==Rx_klik)    //vstup z Rx editace do zamku Rz
			 {

				if(scGPButton_zamek_roztec->ImageIndex==38)
				{
						scGPButton_zamek_aRD->ImageIndex=38;
						scGPButton_zamek_Rz->ImageIndex=38;

						scGPButton_zamek_roztec->ImageIndex=37;



				}   else

				{
						scGPButton_zamek_aRD->ImageIndex=37;
						scGPButton_zamek_Rz->ImageIndex=37;

						scGPButton_zamek_roztec->ImageIndex=38;

				}

			 }
			 Invalidate();
			 FormPaint(this);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPButton_zamek_aRDClick(TObject *Sender)

{

		 //pokud pøed klikem do aRD bylo kliknuto do TT, volá se jiné chování zámkù - aRD a Rz nemusí být provázány
	 input_clicked_icon=aRD_klik_ico;

	 if(input_clicked_edit==TT_klik) Nastav_zamky(aRD_klik_ico,TT_klik);
	 if(input_clicked_edit==R_klik)  Nastav_zamky(aRD_klik_ico, R_klik);
	 if(input_clicked_edit==Rx_klik)  Nastav_zamky(aRD_klik_ico, Rx_klik);
	 if(input_clicked_edit==aRD_klik) Nastav_zamky(aRD_klik_ico,aRD_klik);

	 if(input_clicked_edit==empty_klik) Nastav_zamky(aRD_klik_ico, empty_klik);


		//	ShowMessage(input_clicked_edit);  //6
	 //	ShowMessage(input_clicked_icon);   //0


		//workaround
		//tato konstukce zaøídí, že se okamžitì nastaví šedé pozadí pro zamèený zámek
		// pokud by to zde nebylo, tak se šedé pozadí vykreslí až pøi pøejetí myší pøes zamèený sloupec, což je pozdì
		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::input_TT()

{
	input_state=TT;
	INPUT(0,0);   // pøi volání INPUT z TT je souèástí rovnou i volání OUTPUT + volání výpoèetního modelu
	input_state=NOTHING;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TForm_parametry_linky::INPUT(double Sloupec, double Radek)
{
		 Memo2->Lines->Add("INPUT"+ AnsiString(input_state));
	 //Nastavení zámkù
	 if(scGPButton_zamek_aRD->ImageIndex==37)  pm.aRD_locked=true;    //zamèen aRD
	 else  pm.aRD_locked=false;

	 if(scGPButton_zamek_roztec->ImageIndex==37)  pm.R_locked=true;    //zamèen R
	 else  pm.R_locked=false;

	 if(scGPButton_zamek_Rz->ImageIndex==37)  pm.Rz_locked=true;    //zamèen Rz
	 else  pm.Rz_locked=false;

	 if(scGPButton_zamek_Rz->ImageIndex==37)  pm.Rx_locked=true;    //zamèen Rx
	 else  pm.Rx_locked=false;


	 pm.TT=getTT();



 if(input_state==TT || input_state==jednotky_prevod)  //vìtev TT - aktuálnì není využívána, POUZE PRO PØEVOD
 {

 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	 {
		
		pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i])/60.0;
		if(Runit==MM)	pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i])/1000;
		else       	  pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i]);
		pm.Rz =  getRz(i);
		pm.Rx =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][i]);

		OUTPUT(i,0,0);  // po projití øádku, okamžité volání output - naplnìní obsahem konkrétního øádku
		//VALIDACE(0,0);
	 }

	} else    // aktuálnì využívána pouze tato konstukce níže

	{
		 // uložení do struktury konkrétního øádku
		pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][Radek])/60.0;
		pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][Radek]);
		pm.Rz =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][Radek]);
		pm.Rx =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][Radek]);

		Memo2->Lines->Add(pm.aRD);
		Memo2->Lines->Add(pm.R);
		Memo2->Lines->Add(pm.Rz);
		Memo2->Lines->Add(pm.Rx);

		Memo2->Lines->Add("radek"+AnsiString(Radek));
		Memo2->Lines->Add("sloupec"+AnsiString(Sloupec));

		}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TForm_parametry_linky::OUTPUT(double i, double Sloupec, double Radek)
{

		Memo3->Lines->Add("OUTPUT"+ AnsiString(input_state));

	 if(i>0) // plnìní celé tabulky, pouze v pøípadì zmìny TT nebo pøevodu jednotek

	 {
	   rStringGridEd_tab_dopravniky->Cells[4][i]=pm.aRD*60.0;
		 if(Runit==M)		rStringGridEd_tab_dopravniky->Cells[5][i]=pm.R*1000.0;
		 else       	  rStringGridEd_tab_dopravniky->Cells[5][i]=pm.R;

		rStringGridEd_tab_dopravniky->Cells[6][i]=pm.Rz;
		rStringGridEd_tab_dopravniky->Cells[7][i]=pm.Rx;
	 }

	 else
	 {
			// naplnìní konkrétního øádku na kterém došlo ke zmìnì

		if(input_state!=aRD) rStringGridEd_tab_dopravniky->Cells[4][Radek]=pm.aRD*60.0;
		if(input_state!=R)   rStringGridEd_tab_dopravniky->Cells[5][Radek]=pm.R;
		if(input_state!=RZ)  rStringGridEd_tab_dopravniky->Cells[6][Radek]=pm.Rz;
		if(input_state!=RX)   rStringGridEd_tab_dopravniky->Cells[7][Radek]=pm.Rx;



		VALIDACE(Sloupec,Radek);

//		Memo3->Lines->Add(pm.aRD);
//		Memo3->Lines->Add(pm.R);
//		Memo3->Lines->Add(pm.Rz);
//		Memo3->Lines->Add(pm.Rx);
//
//		Memo3->Lines->Add("radek"+AnsiString(Radek));
//		Memo3->Lines->Add("sloupec"+AnsiString(Sloupec));

	 }



}

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyCellClick(TObject *Sender,
					int ACol, int ARow)
{

if(ACol==1) {

 input_clicked_edit=nazev_klik;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_Rx->Visible=true;

}
if(ACol==2) {

 input_clicked_edit=od_klik;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_Rx->Visible=true;

}
if(ACol==3) {

 input_clicked_edit=do_klik;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_Rx->Visible=true;


}


if(ACol==4) {    //zmìna aRD

 input_clicked_edit=aRD_klik;
 Nastav_zamky(empty_klik_ico,aRD_klik);
// if(ReadOnly) scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=false; //tato promìnná je vždy ovlivnìna, schovám její zámek

 if(scGPButton_zamek_aRD->ImageIndex==38) scGPButton_zamek_aRD->Visible=false;
 else  scGPButton_zamek_aRD->Visible=true;

	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_roztec->Visible=true;

 rStringGridEd_tab_dopravniky->Invalidate();



}
if(ACol==5) {    //zmìna R

 input_clicked_edit=R_klik;
 Nastav_zamky(empty_klik_ico,R_klik);

 Roletka_roztec(ARow);
 if(scGPButton_zamek_roztec->ImageIndex==38)  scGPButton_zamek_roztec->Visible=false;
 else   scGPButton_zamek_roztec->Visible=true;
	scGPButton_zamek_Rz->Visible=true;
	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_aRD->Visible=true;

}
if(ACol==6) {    //zmìna Rz

  input_clicked_edit=Rz_klik;
	Nastav_zamky(empty_klik_ico,Rz_klik);
	scGPButton_zamek_aRD->Visible=false;   //tato promìnná je vždy ovlivnìna, schovám její zámek

	if(scGPButton_zamek_Rz->ImageIndex==38)  scGPButton_zamek_Rz->Visible=false;
 else scGPButton_zamek_Rz->Visible=true;

	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_roztec->Visible=true;

}
if(ACol==7) {    //zmìna Rx

 input_clicked_edit=Rx_klik;
 Nastav_zamky(empty_klik_ico,Rx_klik);
	if(scGPButton_zamek_Rx->ImageIndex==38)  scGPButton_zamek_Rx->Visible=false;
  else scGPButton_zamek_Rx->Visible=true;

	scGPButton_zamek_aRD->Visible=true;
	scGPButton_zamek_roztec->Visible=true;
	scGPButton_zamek_Rz->Visible=true;



}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPButton_zamek_RxClick(TObject *Sender)
{

	if(input_clicked_edit==aRD_klik) Nastav_zamky(Rx_klik_ico,aRD_klik);  //v pøídì, kdy došlo ke kliknutí do buòky aRD nastavím chování Rx
	if(input_clicked_edit==Rz_klik) Nastav_zamky(Rx_klik_ico,Rz_klik);

	else   Nastav_zamky(Rx_klik_ico,empty_klik);

		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;

}
//---------------------------------------------------------------------------






void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikySetEditText(TObject *Sender,
					int ACol, int ARow, const UnicodeString Value)
{

				 //tato událost je volána vždy, nikoliv pouze pøi zmìnì obsahu - stringgrid nemá nativnì onchange událost
				 // z tohoto dùvodu je pøi splnìní podmínky ihned uložen obsah buòky, a následnì porovnán.
				 // pokud je rozdílný, došlo ke zmìnì a volám výpoèetní model
		if(ACol==1)
		{
				if(input_state==NOTHING && input_clicked_edit==nazev_klik)
				{
					if(rStringGridEd_tab_dopravniky->Cells[1][ARow]!=NAZEV)
					{
			//	 ShowMessage("tady");
				 INPUT(ACol,ARow);
				 OUTPUT(0,ACol,ARow);
				 rStringGridEd_tab_dopravniky->Invalidate();
				 input_state=NOTHING;
					}
				}

		}
			if(ACol==2)
		{
				if(input_state==NOTHING && input_clicked_edit==od_klik)
				{
				if(rStringGridEd_tab_dopravniky->Cells[2][ARow]!=OD)
				{
				 INPUT(ACol,ARow);
				 OUTPUT(0,ACol,ARow);
				 rStringGridEd_tab_dopravniky->Invalidate();
				 input_state=NOTHING;
				 }
				}

		}
			if(ACol==3)
		{
				if(input_state==NOTHING && input_clicked_edit==do_klik)
				{
				if(rStringGridEd_tab_dopravniky->Cells[3][ARow]!=DO)
				{
				 INPUT(ACol,ARow);
				 OUTPUT(0,ACol,ARow);
				 rStringGridEd_tab_dopravniky->Invalidate();
				 input_state=NOTHING;
				 }
				}

		}

	if(ACol==4)
		{
				if(input_state==NOTHING && input_clicked_edit==aRD_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[4][ARow]!=pm.aRD*60.0)
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_aRD");
				input_state=aRD;
				INPUT(ACol,ARow);
				pm.input_aRD();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
				vypis("Došlo ke zmìnì obsahu roletky rozteèe, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypoèítání nových dat do roletky na základì zmìny Rz
				zobrazOramovani=true;

				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepoužíván")  F_gapoR->pohony_zmena[getPID(ARow)].X=true;

				input_state=NOTHING;
				}

				}


		}

	 if(ACol==5)
		{
				if(input_state==NOTHING && input_clicked_edit==R_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[5][ARow]!=pm.R)
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_R");
				input_state=R;
				INPUT(ACol,ARow);
				pm.input_R();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
			 if(scGPButton_zamek_aRD->ImageIndex==38)  //pokud je aRD odemèeno volám akt.data do roletky
			 {	Roletka_roztec(ARow);
					vypis("Došlo ke zmìnì obsahu roletky rozteèe, vyberte hodnotu.",false);
					Memo2->Lines->Add("volam aktualiz R");
					zobrazOramovani=true;
					if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepoužíván")  F_gapoR->pohony_zmena[getPID(ARow)].X=true;
			 } //vypoèítání nových dat do roletky na základì zmìny Rz
			 else zobrazOramovani=false;

				input_state=NOTHING;
				}

				}

		}

		 if(ACol==6)
		{
				if(input_state==NOTHING && input_clicked_edit==Rz_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[6][ARow]!=pm.Rz)
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_Rz");
				input_state=RZ;
				INPUT(ACol,ARow);
				pm.input_Rz();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
				vypis("Došlo ke zmìnì obsahu roletky rozteèe, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypoèítání nových dat do roletky na základì zmìny Rz
				zobrazOramovani=true;
				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepoužíván")  F_gapoR->pohony_zmena[getPID(ARow)].X=true;

				input_state=NOTHING;
				}

				}



		}

	if(ACol==7)
		{
		if(input_state==NOTHING && input_clicked_edit==Rx_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[7][ARow]!=pm.Rx)
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_Rx");
				input_state=RX;
				INPUT(ACol,ARow);
				pm.input_Rx();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
				if(scGPButton_zamek_roztec->ImageIndex==37)
				{
				zobrazOramovani=true;
				vypis("Došlo ke zmìnì obsahu roletky rozteèe, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypoèítání nových dat do roletky na základì zmìny Rz
				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepoužíván")  F_gapoR->pohony_zmena[getPID(ARow)].X=true;
				}    else zobrazOramovani=false;
				rStringGridEd_tab_dopravniky->Invalidate();
				input_state=NOTHING;
				}

				}

		}



//		if (ACol==4)  //aRD  // tohle je dobrý, že v ARow mám pøedanej øádek zrovna, který edituji, èili pak velmi jednoduše to pøedám tam kam potøebuji
//		{
//
//				//  pokud má rozteè a je nepoužívaný tak dovolím dopoèítávat resp. navrhovat hodnoty k uložení
//		 if(!rStringGridEd_tab_dopravniky->Cells[5][ARow].IsEmpty() /*&& rStringGridEd_tab_dopravniky->Cells[8][ARow]=="nepoužíván"*/)
//			{
//				//Doporuèení Rz
//				rStringGridEd_tab_dopravniky->Cells[6][ARow]=F->m.Rz(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0);
//
//			//Doporuèení rozteèí
//				double Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]); //pøedám Rz
//					AnsiString data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";");
//
//			 //	EditStyle=sgbDropDown;   //vyber typu  - nyní je nastaven globálnì v dfm
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
//				TStringList *S=new TStringList;
//				S->Add(data);
//				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
//				S->Delimiter=';';     //nutno v jednoduchých uvozovkách, dvojí hodí chybu pøi pøekladu
//				S->DelimitedText=data;
//
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Assign(S); //Standartnì se používá Add(), ale v tomto pøípadì Assign()
//			////////////////////////////////////////////////////////////////////////////////////////////////////////
//				Memo2->Lines->Clear();
//				Memo2->Lines->Add("vypis - menim obsah roletky R"+AnsiString (ARow));
//			//rStringGridEd_tab_dopravniky->Columns->Items[5]->Font->Color=(TColor)RGB(211,211,211);   //=Background=(TColor)RGB(211,211,211);
//
//				//Doporuèení Rx
//				rStringGridEd_tab_dopravniky->Cells[7][ARow]=F->m.Rx(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0,F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]));
//
//				roletka_data=ARow;
//			 }
//		}
//
//		if (ACol==5)  // rozteè R
//
//		{      //v pøípadì, že v roletce vyberu rozteè, dojde k pøepoètu Rx
//				if(!rStringGridEd_tab_dopravniky->Cells[5][ARow].IsEmpty() /*&& rStringGridEd_tab_dopravniky->Cells[8][ARow]=="nepoužíván"*/)
//			{
//
//				//Doporuèení Rx
//				rStringGridEd_tab_dopravniky->Cells[7][ARow]=F->m.Rx(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0,F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]));
//			 }
//
//		}
//
//			if (ACol==6)  // Rz  - zmìna Rz dopoèítá nové RD a obsah roletky Rozteèe
//
//		{
//
//		 rStringGridEd_tab_dopravniky->Cells[4][ARow]=F->m.RD(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]))*60.0;
//
//		 // + roletka rozteèe
//
//		 	//Doporuèení rozteèí
//				double Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]); //pøedám Rz
//					AnsiString data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";");
//
//			 //	EditStyle=sgbDropDown;   //vyber typu  - nyní je nastaven globálnì v dfm
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
//				TStringList *S=new TStringList;
//				S->Add(data);
//				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
//				S->Delimiter=';';     //nutno v jednoduchých uvozovkách, dvojí hodí chybu pøi pøekladu
//				S->DelimitedText=data;
//					Memo3->Lines->Clear();
//        	Memo3->Lines->Add("vypis - menim obsah roletky R"+AnsiString(ARow));
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Assign(S); //Standartnì se používá Add(), ale v tomto pøípadì Assign()
//      /////////////////////////////////////////////////////////////
//			 //  nastavi pro celý sloupec :-/
//			 // rStringGridEd_tab_dopravniky->Columns->Items[5]->Font->Size=5;
//			 roletka_data=ARow;
//		}
//
//				if (ACol==7)  // Rx  - zmìna Rx vypoèítá novou rozteè - pøesnou hodnotu
//
//		{
//		 rStringGridEd_tab_dopravniky->Cells[5][ARow] = (F->d.v.PP.TT * F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0) / F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]);
//		}
}
//---------------------------------------------------------------------------




void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyGetEditText(TObject *Sender,
					int ACol, int ARow, UnicodeString &Value)
{

		//tato událost je volána okamžitì po vstupu do buòky a uloží do struktury akt.zobrazená data
		 if(ACol==1)   NAZEV=rStringGridEd_tab_dopravniky->Cells[1][ARow];

		 if(ACol==2)   OD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][ARow]);

		 if(ACol==3)   DO = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][ARow]);

		 if(ACol==4)	pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0;

		 if(ACol==5)  pm.R = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]);

		 if(ACol==6)  pm.Rz = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]);

		 if(ACol==7)  pm.Rx = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPButton_zamek_roztecClick(TObject *Sender)

{
		if(input_clicked_edit==aRD_klik) Nastav_zamky(R_klik_ico,aRD_klik);  //v pøídì, kdy došlo ke kliknutí do buòky aRD nastavím chování Rx
	//	if(input_clicked_edit==R_klik) Nastav_zamky(R_klik_ico,R_klik);  - není potøeba, je schován zámek
		if(input_clicked_edit==Rz_klik) Nastav_zamky(R_klik_ico,Rz_klik);
		if(input_clicked_edit==Rx_klik) Nastav_zamky(R_klik_ico,Rx_klik);

		if(input_clicked_edit==empty_klik) Nastav_zamky(R_klik_ico,empty_klik);   // std.chování


		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPButton_zamek_RzClick(TObject *Sender)
{

	 input_clicked_icon=Rz_klik_ico;
	// if(input_clicked_edit==Rz_klik) Nastav_zamky(Rz_klik_ico,Rz_klik);   - není potøeba, je schován zámek
	 if(input_clicked_edit==R_klik)	 Nastav_zamky(Rz_klik_ico,R_klik);
	 if(input_clicked_edit==Rx_klik)	 Nastav_zamky(Rz_klik_ico,Rx_klik);

	 if(input_clicked_edit==empty_klik) Nastav_zamky(Rz_klik_ico,empty_klik);  //samostatný klik do zámku, bez pøedchozího vstupu do bunky

		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
}

//---------------------------------------------------------------------------

void TForm_parametry_linky::Roletka_roztec(double Row)

{
					double Rz;
											 //pokud Rz není prázdné, uložím si jeho hodnotu a použiju k pøedání.
											 //jinak nastavím Rz na -1 tzn., že do roletky zobrazím všechny rozteèe z katalogu
					if(!rStringGridEd_tab_dopravniky->Cells[6][Row].IsEmpty())
					{
					Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][Row]);
					}  else Rz=-1;

					AnsiString	data;
				 if(Runit==MM) data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",true);
				 else          data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",false);

				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
				TStringList *S=new TStringList;
				S->Add(data);
				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
				S->Delimiter=';';     //nutno v jednoduchých uvozovkách, dvojí hodí chybu pøi pøekladu
				S->DelimitedText=data;
				S->DelimitedText=data;

				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Assign(S); //Standartnì se používá Add(), ale v tomto pøípadì Assign()
				//zobrazOramovani=true;
				//F->m.frameRect(Rect(200,100,60,80),clBlue,2);

		}






void __fastcall TForm_parametry_linky::FormClick(TObject *Sender)
{
//zobrazení všech zámkù pøi kliku do formu
 scGPButton_zamek_roztec->Visible=true;
 scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=true;
 scGPButton_zamek_Rx->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyDrawCell(TObject *Sender,
					int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
if (ACol==5) {

	//F->m.frameRect(Rect,clBlue,2);
 //   Rect=
}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyPicklistDropdown(TObject *Sender,
          int Col, int Row, TStringList *&PickList)
{
if(Col==5)  vypis("",false);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rMemoEx1_roztecClick(TObject *Sender)
{

		input_state = jednotky_prevod; // zámìr, aby se nepøepoèítavaly hodnoty
		if (Runit == MM) // pokud je v milimetrech, tak pøepne na metry
		{

			  rMemoEx1_roztec->Text="    Rozteè [m]";
				INPUT(0,0);   //tento input volá zároveò i output
				Runit = M;
		}
		else // pokud je metrech, tak pøepne na milimetry
		{
			rMemoEx1_roztec->Text="    Rozteè [mm]";
			INPUT(0,0);    //tento input volá zároveò i output
			Runit = MM;
		}
	
		input_state = NOTHING; // už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------

double  TForm_parametry_linky::getRz(double i)
{
	double Rz=0;
	if(Rzunit==M) Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i]);
	else          Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i])*1000.0;
	return Rz;
}

double  TForm_parametry_linky::getTT()
{
	double TT=0;
	if(Taktunit==S) TT=rEditNum_takt->Value;
	else            TT=rEditNum_takt->Value*60.0;
	return TT;
}

////////////////////////////////////////////////////////////////////////////////

void TForm_parametry_linky::VALIDACE(int ACol,int ARow)
{

vypis("");VID=-1;
Row_validace=0;

				 if(ACol==4 || ACol==5 || ACol==6 || ACol==7)
				 {
						 if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepoužíván")
						 {
							VID=ACol;
							VID_value           =	F->ms.MyToDouble(F->d.v.validaceR(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),0));
							AnsiString  retezec =	F->d.v.validaceR(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),1);
							Row_validace=ARow;
							if(VID_value!="") 	vypis(retezec);
						 }
						 else
						 {

									double value=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]);
									if(value!=floor(value))
									{
										 double dop_Rx=	Form1->m.round(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]));
										 vypis("Hodnota musí být celoèíselná, doporuèená hodnota Rx: "+ AnsiString(dop_Rx));
										 Row_validace=ARow;
										 VID=7;
										 VID_value=dop_Rx;
								}

						}
				 }

 switch(ACol)
	 {
		///////////////////////////////////////////////////////////////////////
			case 1:     //NAZEV
				{
				if(rStringGridEd_tab_dopravniky->Cells[1][ARow]=="")
						{
						vypis("Název pohonu nesmí být prázdný!");
						VID=1;
						Row_validace=ARow;
						}

				}
				break;
				case 2:     //RD OD
				{
						if(rStringGridEd_tab_dopravniky->Cells[2][ARow]<=0)
						{
						vypis("Neplatná hodnota rychlosti pohonu od!");
						VID=2;
						Row_validace=ARow;
						}

					//od je vìtší než do
						if(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][ARow]) > F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][ARow]))
							{
							vypis("Neplatný rozsah rychlosti pohonu od-do!");
							VID=23;
							Row_validace=ARow;
							}

				}
				break;
				case 3:     //RD OD
				{
				if(rStringGridEd_tab_dopravniky->Cells[3][ARow]<=0)
						{
						vypis("Neplatná hodnota rychlosti pohonu do!");
						VID=3;
						Row_validace=ARow;
						}

				}
				break;
	 }

}


void __fastcall TForm_parametry_linky::scGPGlyphButton_TTClick(TObject *Sender)
{

	bool Changes_TT=false;
	Form_TT_kalkulator->Left=Form1->ClientWidth/2-Form_TT_kalkulator->Width/2;
	Form_TT_kalkulator->Top=Form1->ClientHeight/2-Form_TT_kalkulator->Height/2;

	 if(Form1->d.v.OBJEKTY->dalsi!=NULL || Form1->d.v.POHONY->dalsi!=NULL)
	 {

				if(mrOk==Form_TT_kalkulator->ShowModal())
				{
				 //
					 if(Form_TT_kalkulator->rEditNum_takt->Value!=F->d.v.PP.TT)
					 {

					// ShowMessage("Došlo ke zmìne TT - volání GAPO");
					 Changes_TT=true;
					 rEditNum_takt->Value=Form_TT_kalkulator->rEditNum_takt->Value;
					 }
				}
		}

		if(Changes_TT)//pri zmene TT + jiz existuje nejaky objekt nebo pohon
		{
			F_gapoTT->ShowModal();
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPGlyphButton_vozik_editClick(TObject *Sender)

{
		bool Changes_vozik=false;
		Form_parametry_vozik->Left=Form1->ClientWidth/2-Form_parametry_vozik->Width/2;
		Form_parametry_vozik->Top=Form1->ClientHeight/2-Form_parametry_vozik->Height/2;
	if(mrOk==Form_parametry_vozik->ShowModal())
	{
	// naplnìní nových dat do struktury

	if(F->d.v.PP.delka_jig!=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value) Changes_vozik=true;
	if(F->d.v.PP.sirka_jig!=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value) Changes_vozik=true;
	if(F->d.v.PP.vyska_jig!=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value) Changes_vozik=true;
	if(F->d.v.PP.delka_podvozek!=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value)  Changes_vozik=true;

	scGPNumericEdit_delka_jig->Value=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value;
	scGPNumericEdit_sirka_jig->Value=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value;
	scGPNumericEdit_vyska_jig->Value=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value;
	scGPNumericEdit_delka_podvozek->Value=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value;
	}
		if(Changes_vozik)//pri zmene voziku
		{
			F_gapoV->ShowModal();
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button2Click(TObject *Sender)
{
rStringGridEd_tab_dopravniky->Visible=true;
}
//---------------------------------------------------------------------------






void __fastcall TForm_parametry_linky::rHTMLLabel_InfoTextClick(TObject *Sender)
{
		if(VID==4 || VID==5 || VID==6 || VID==7)
		{
		 rStringGridEd_tab_dopravniky->Cells[VID][Row_validace]=VID_value;
		 INPUT(VID,Row_validace);
		 OUTPUT(0,VID,Row_validace);
		 rStringGridEd_tab_dopravniky->Invalidate();
		}

		switch(VID)
		{

    	case -1:break;
			case 0:break;
			case 7:
			{
//			 rStringGridEd_tab_dopravniky->Cells[7][Row_validace]=VID_value;
//			 INPUT(7,Row_validace);
//			 OUTPUT(0,7,Row_validace);
//			 rStringGridEd_tab_dopravniky->Invalidate();
			}
			break;



		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button3Click(TObject *Sender)
{
Memo4->Lines->Clear();
Memo4->Lines->Add("input_state:"+AnsiString(input_state));
Memo4->Lines->Add("input_clicked_edit:"+AnsiString(input_clicked_edit));
Memo4->Lines->Add("input_clicked_icon:"+AnsiString(input_clicked_icon));
}
//---------------------------------------------------------------------------

