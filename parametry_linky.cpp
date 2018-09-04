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
	////designov� z�le�itosti
	Form_parametry_linky->Color=(TColor)RGB(240,240,240); //nastaven� barvy formul��e
  rMemoEx1_rychlost->Color=(TColor)RGB(128,128,128);
	Form1->m.designButton(Button_save,Form_parametry_linky,1,2);
	Form1->m.designButton(Button_storno,Form_parametry_linky,2,2);
	zobrazitFrameForm=false;

	 rStringGridEd_tab_dopravniky->SetColumnAutoFit(0);
	 rStringGridEd_hlavicka_tabulky->SetColumnAutoFit(0);

	//pop-up menu
	//pozad�
	clBg=(TColor)RGB(240,240,240); //pasive
	clAcBg=(TColor)RGB(212,208,200);//active
	clAcBg2=(TColor)RGB(195,192,184);//active k��ku
	//v�pl� glyphu
	clGlyph=(TColor)RGB(43,87,154);//pasive
	clAcGlyph=(TColor)RGB(0,128,255);//active
	PopUPmenu->Color=clBg;//nastaven� pozad� barvy formul��e
	pasiveColor();//nastav� v�echny polo�ky na pasivn� resp. default barvu
	//------

	////jednotky
	//Delkaunit=MM;
	//Sirkaunit=Delkaunit;

	//roletka_data=0;


}
//---------------------------------------------------------------------------
void TForm_parametry_linky::pasiveColor()//nastav� v�echny polo�ky pop-up na pasivn� resp. default barvu
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
   Taktunit=S;
	 Runit=MM;
	 Rzunit=MM;
   aRDunit=S;
   Dmunit=M;

   scHTMLLabel_rozestup->Height=27;

  	if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")
    {  //budu p�ev�d�t na m/min
    aRDunit=MIN;
    }
    if(Form1->readINI("nastaveni_form_parametry", "DM") == "1")
    {  //budu p�ev�d�t na metry - rozestup, dle nastaven�ch jednotek mezery na PO zobraz�m rozestup na PL
    Dmunit=MM;
    }


   rHTMLLabel_info_zmenaR->Caption="";
	 input_state = NOTHING;   //v�choz� nastaven� stavu
	 Nastav_zamky(empty_klik_ico,empty_klik);
	 vypis(""); VID=-1;

	 //povolen� vstup� do zm�ny TT a voz�ku + Ulo�it + Storno button
		scGPGlyphButton_vozik_edit->Enabled=true;
		scGPGlyphButton_TT->Enabled=true;
		Button_storno->Enabled=true;
		Button_save->Enabled=true;

		scExPanel_doporuc_pohony->Visible=false;
		PopUPmenu->Visible=false;
		rMemoEx_Nazev->SetFocus();
		zobrazOramovani=false;

		if(scGPSwitch->State==0) {rHTMLLabel_podvozek_zaves->Caption="Podvozek";   rHTMLLabel_podvozek_zaves->Left=34;  }
		else  { rHTMLLabel_podvozek_zaves->Caption="Z�v�s";  rHTMLLabel_podvozek_zaves->Left=56; }


		if(Form1->d.v.navrhni_POHONY()=="")
		{
			scGPButton_doporucene->Visible=false;
			rHTMLLabel_doporuc_pohony->Caption=""; // neexistuj� ��dn� objekty -> neum�m spo��tat doporu�. rychlosti
		}
		else
		{
			scGPButton_doporucene->Visible=true;
			rHTMLLabel_doporuc_pohony->Caption="Navr�en� pohony pro objekty bez p�i�azen�ch pohon�:";
		}

		if(Form1->STATUS==Form1->NAVRH)    //Architekt
		{
			scGPButton_vozik->Caption="   Voz�k";
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
			rStringGridEd_tab_dopravniky->RowCount=1;    //defaultn� po�et ��dk� - hlavi�ka
		}


    //nastaveni rozmeru formu - dle poctu pohonu

      rStringGridEd_tab_dopravniky->Height=rStringGridEd_tab_dopravniky->RowCount*30 + 48;
      Form_parametry_linky->Height= rStringGridEd_tab_dopravniky->Height +428;
      scGPGlyphButton_ADD->Top=Form_parametry_linky->Height - 65 ;
      Button_save->Top=Form_parametry_linky->Height - 40;
      Button_storno->Top=Form_parametry_linky->Height - 40;
      scGPGlyphButton_DEL_nepouzite->Top=Form_parametry_linky->Height-30;

		//pro vytvo�en� z�lohy zru�en�ch p���azen� - vyfikundace z d�vodu mo�n�ho storna
		//mus� b�t um�st�no a� za nacti_pohony
		zrusena_prirazeni_PID_size=rStringGridEd_tab_dopravniky->RowCount-1;//velikost sta�� jako po�et ��dk�/pohonu po na�ten�, v�ce jich b�t p�i�azeno do nov�ho na�ten� formu b�t nem��e
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

	 //nahr�n� hodnot

			scGPNumericEdit_delka_jig->Value=Form1->d.v.PP.delka_jig*(1+999*Delkaunit);
			scGPNumericEdit_sirka_jig->Value=Form1->d.v.PP.sirka_jig*(1+999*Delkaunit);
			scGPNumericEdit_delka_podvozek->Value=Form1->d.v.PP.delka_podvozek*(1+999*Delkaunit);


	 if(Form1->d.v.PP.typ_voziku==0) scGPSwitch->State=scswOff;
	 else  { scGPSwitch->State=scswOn; }
	 //scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;


	 if(Taktunit==MIN)
	 {
	 rEditNum_takt->Value=Form1->d.v.PP.TT/60.0;
	 } else rEditNum_takt->Value=Form1->d.v.PP.TT;

	 //rStringGridEd_tab_dopravniky->Cells[0][0]="ID";
	 //rStringGridEd_tab_dopravniky->Cells[1][0]="N�zev";
	 rStringGridEd_tab_dopravniky->Cells[2][0]="od";
	 rStringGridEd_tab_dopravniky->Cells[3][0]="do";
	 rStringGridEd_tab_dopravniky->Cells[4][0]="aktu�ln�";
	 rStringGridEd_tab_dopravniky->Cells[5][0]="";
	 rStringGridEd_tab_dopravniky->Cells[6][0]="";
	 rStringGridEd_tab_dopravniky->Cells[7][0]="ka�d� n-t� palec";
	 //rStringGridEd_tab_dopravniky->Cells[8][0]="p�i�azen";

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

	 if(rStringGridEd_tab_dopravniky->RowCount>1 && rStringGridEd_tab_dopravniky->ColCount>=5){rStringGridEd_tab_dopravniky->Row=1;rStringGridEd_tab_dopravniky->Col=5;}//rStringGridEd_tab_dopravniky->FinishEditing();//ukon�� editaci a t�m odstran� nep��jemn� vizu�ln� efekt

	 //pozice info tla��tka - asi je tla��tko stejn� provizorn�
	 pozice_scGPGlyphButton_hint();

	 //testuje zda existuj� nepou��t� pohony a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
	 if(existuji_nepouzivane_pohony())scGPGlyphButton_DEL_nepouzite->Visible=true;
	 else scGPGlyphButton_DEL_nepouzite->Visible=false;

	 rMemoEx_ID->Text="";
   rMemoEx_Nazev->Text="";
     // rMemoEx1_rychlost->Caption="";
	 rMemoEx2_prirazen->Text="";




	 rMemoEx_ID->Lines->Add("    ID");
	 rMemoEx_Nazev->Lines->Add("    N�zev");
   if(aRDunit==S)   rMemoEx1_rychlost->Caption="Rychlost <u>[m/s]</u></br>";//("   Rychlost [m/min]");
   else    rMemoEx1_rychlost->Caption="Rychlost <u>[m/min]</u></br>";
   rMemoEx1_rychlost->Height=28;


	 if(Runit==M)  scHTMLLabel_roztec->Caption="rozte� <u>[m]</u>";
	 else scHTMLLabel_roztec->Caption="rozte� <u>[mm]</u></br>";

 	 if(Dmunit==M)  scHTMLLabel_rozestup->Caption="rozestup <u>[m]</u>";
	 else scHTMLLabel_rozestup->Caption="rozestup <u>[mm]</u></br>";

	 rMemoEx2_prirazen->Lines->Add("   P�i�azen");

	 Storno=false;
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
						//p�vodn� if(Form1->d.v.pohon_je_pouzivan(ukaz->n))rStringGridEd_tab_dopravniky->Cells[5][i]="ano";else 	rStringGridEd_tab_dopravniky->Cells[5][i]="ne";
						AnsiString OBJEKTY_POUZIVAJICI_POHON=Form1->d.v.vypis_objekty_vyuzivajici_pohon(ukaz->n);
						if(OBJEKTY_POUZIVAJICI_POHON!="")rStringGridEd_tab_dopravniky->Cells[8][i]=OBJEKTY_POUZIVAJICI_POHON;
						else rStringGridEd_tab_dopravniky->Cells[8][i]="nepou��v�n";

					 //	ShowMessage(ukaz->roztec*1000.0);

						//	 ShowMessage(ukaz->rychlost_do*60.0);
						rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
						rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;

					 //	if(ukaz->rychlost_od==0)  	rStringGridEd_tab_dopravniky->Cells[2][i] = "";
					//	else
						 rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od*(1+59.0*aRDunit);

						if(ukaz->rychlost_do==0)   rStringGridEd_tab_dopravniky->Cells[3][i] ="";
						else rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do*(1+59.0*aRDunit);

						if(ukaz->aRD==0)  	rStringGridEd_tab_dopravniky->Cells[4][i] = "";
						else rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->aRD*(1+59.0*aRDunit);
            //ShowMessage(ukaz->aRD*60.0);

						if(ukaz->roztec==0) rStringGridEd_tab_dopravniky->Cells[5][i]="";
						if(Runit==MM) rStringGridEd_tab_dopravniky->Cells[5][i] = ukaz->roztec*(1+999*Runit);
						else    rStringGridEd_tab_dopravniky->Cells[5][i] = ukaz->roztec;

						if(ukaz->Rz==0) rStringGridEd_tab_dopravniky->Cells[6][i]="";
						else rStringGridEd_tab_dopravniky->Cells[6][i] = ukaz->Rz*(1+999*Dmunit);

						if(ukaz->Rx==0) rStringGridEd_tab_dopravniky->Cells[7][i]="";
						else rStringGridEd_tab_dopravniky->Cells[7][i] = ukaz->Rx;

						ukaz = ukaz->dalsi;
				 }
				 F_gapoR->pohony_zmena=new TPoint[F->d.v.POHONY->predchozi->n+1]; //alokace o jedni�ku vy���, nult� index nen� toti� vyu��v�n
				 for(int i=0; i<=F->d.v.POHONY->predchozi->n;i++){F_gapoR->pohony_zmena[i].X=false;F_gapoR->pohony_zmena[i].Y=false;}
	}
	else {  //pokud je spoj�k pr�zdn�, zobraz�m tyto p�ednastaven� hodnoty

	data_nalezena=false; // default se nepou��v� resp. po soubor nov� se okam�it� vlo�� do spoj�ku def.pohon
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
	//M toto tu nesm� b�t:Form_parametry_linky->Close();
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
	Storno=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::KonecClick(TObject *Sender)
{
	Button_stornoClick(Sender);//stejn� funkcionalita jako u storna
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_saveClick(TObject *Sender)
{
		Changes=false;  //obecna zmena = zmena PP ci TT
		Changes_TT=false;    // konkretni zmena TT
		Changes_PP=false;   // konkretni zmena PP
		Changes_roztec=false;
		Changes_prirazen=false; //pokud odp�i�ad�m pou��van� pohon
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

		//zobrazen� formu gapoR - jedin� GAPO, kter� je vol�no zde, proto�e se vol� a� p�i stisku OK PL formu
		bool zobrazGAPO_R=false;
		for(int i=1; i<=F->d.v.POHONY->predchozi->n;i++)
		{
			if(F_gapoR->pohony_zmena[i].X) zobrazGAPO_R=true;
			//	ShowMessage(AnsiString(i)+" "+AnsiString((short)(F_gapoR->pohony_zmena[i])));
		}
		if(zobrazGAPO_R)//pokud byl n�jak� pou��van� pohon zm�n�n
		{
			//scGPPanel2->FillColor = (TColor)RGB(200,200,200);//F->m.clIntensive((TColor)RGB(43,87,154),40);
			Button_save->Enabled=false;Button_storno->Enabled=false;
			if(F_gapoR->ShowModal()!=mrOk)
			{
				Ulozit=false;//pokud bude stisknuto �i k��ek na gapo_R storno, ukl�d�n� PL formu se p�eru��, resp. neprovede
				//scGPPanel2->FillColor = (TColor)RGB(43,87,154);
				Button_save->Enabled=true;Button_storno->Enabled=true;
				Storno=false; //nejedna se o storno PL formu, ale gapoR formu, proto storno false, jinak by doslo k ukon�en� PL formu
			}else
			{
				Storno=false;
				Ulozit=true;
			}
			mGrid->Delete();
		}

		//NEW
		//kontrola rozmez� jednotliv�ch pohon�   - je to spravne, cekovat vzdy vuci RD?
		AnsiString T="";
		for(short i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)

		{
			//pr�chod jednotliv�mi objekty, zda je dan� pohon objektu p�i�azen a pokud ano, tak zda je mimo rozsah
			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			while(O!=NULL)
			{
      double grid_od = 0;
      double grid_do = 0;   //zde nen� nutn� nic p�ev�d�t, p�ev�d� se lev� ��st v podm�nce IF dle akt. jednotek
       grid_od =  Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]);
       grid_do =  Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]);

				if(
					O->pohon!=NULL && //kdy� m� objekt p�i�azen pohon a z�rove�
					O->rezim==1 && //je v re�imu kontinu�l a z�rove�
					O->pohon->n==Form1->ms.a2i(rStringGridEd_tab_dopravniky->Cells[0][i]) &&//pokud objekt m� pohon p�i�azen a z�rove�
					(Form1->ms.MyToDouble(O->RD)*(1+59.0*aRDunit) < grid_od ||//je mimo rozsah (pod) nebo
					 Form1->ms.MyToDouble(O->RD)*(1+59.0*aRDunit) > grid_do)//je mimo rozsah (nad)
				)
				{
          Memo3->Lines->Add(grid_od);
          Memo3->Lines->Add(grid_do);
          Memo3->Lines->Add(aRDunit);
					T+="Objekt: "+O->name+" Rychlost:"+O->RD*(1+59.0*aRDunit)+" vs. Pohon: "+rStringGridEd_tab_dopravniky->Cells[1][i];
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


		////////////////////////////////////////////////////////////////////////
		//Pri zmene roztece  - volani zm�ny rozte�e - pokud dojde ke zm�n� rozte�e u pou��van�ho pohonu - p�ed�m status pro zobrazen� PL_priority
			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
			while(P!=NULL)
			{
			     double roztec;
					 if(Runit==M) roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n]);
					 else 				roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n])/1000.0;
					if(rStringGridEd_tab_dopravniky->Cells[8][P->n]!="nepou��v�n"  && roztec!=P->roztec)
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

			// Hl�dan� n�sleduj�c�ch zm�n, pokud je pohon pou��van�

				Cvektory::TPohon *p_prirazen=Form1->d.v.POHONY->dalsi;
			while(p_prirazen!=NULL)
			{
						bool pohon_pouzivan=false;
						AnsiString pohon_prirazen=Form1->d.v.vypis_objekty_vyuzivajici_pohon(p_prirazen->n);
						if(pohon_prirazen!="")pohon_pouzivan=true;
						else   pohon_pouzivan=false;

				 //zm�na odp�i�azen�
					if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]=="nepou��v�n"  && pohon_pouzivan)
					{
							Changes_prirazen=true;
							zrusena_prirazeni_PID[getPID(p_prirazen->n)-1]=true;

					}
				 //zm�na aRD
						if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepou��v�n"  && rStringGridEd_tab_dopravniky->Cells[4][p_prirazen->n]!=p_prirazen->aRD*60.0)
					{
							Changes_aRD=true;

					}

					//zm�na Rz
							if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepou��v�n"  && rStringGridEd_tab_dopravniky->Cells[6][p_prirazen->n]!=p_prirazen->Rz)
					{
							Changes_Rz=true;

					}
					 //zm�na Rx
								if(rStringGridEd_tab_dopravniky->Cells[8][p_prirazen->n]!="nepou��v�n"  && rStringGridEd_tab_dopravniky->Cells[7][p_prirazen->n]!=p_prirazen->Rx)
					{
							Changes_Rx=true;

					}

					p_prirazen=p_prirazen->dalsi;
				 }


			p_prirazen=NULL;delete p_prirazen;


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

				if (rStringGridEd_tab_dopravniky->Cells[1][i].IsEmpty()) nazev="nov� pohon";
				else  nazev=rStringGridEd_tab_dopravniky->Cells[1][i];

				if (rStringGridEd_tab_dopravniky->Cells[2][i].IsEmpty()) rychlost_od=0;
				else  rychlost_od=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i])/(1+59.0*aRDunit);

				if(rStringGridEd_tab_dopravniky->Cells[3][i].IsEmpty()) rychlost_do=0;
				else 	rychlost_do=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i])/(1+59.0*aRDunit);

				if(rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty()) aRD=0;
				else aRD=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i])/(1+59.0*aRDunit);

				if(rStringGridEd_tab_dopravniky->Cells[5][i].IsEmpty()) roztec=0;
				if(Runit==MM) roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i])/1000.0;
				else roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i]);


				if(rStringGridEd_tab_dopravniky->Cells[6][i].IsEmpty()) Rz=0;
				else Rz=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i])/(1+999.0*Dmunit);

				if(rStringGridEd_tab_dopravniky->Cells[7][i].IsEmpty()) Rx=0;
				else Rx=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][i]);

				//ulo�en� pohonu do spoj�ku
				Form1->d.v.vloz_pohon (nazev,rychlost_od,rychlost_do,aRD,roztec,Rz,Rx);

				//v�em objekt�m, kter� m�ly p�i�azen pohon s oldN(oldID), p�i�ad� pohon s newN(newID), podle toho, jak jsou ukl�d�ny nov� do spoj�ku, d�le�it�, pokud dojde k naru�en� po�ad� ID resp n pohon� a po�ad� jednotliv�ch ��dk� ve stringridu, nap�. kopirov�n�m, smaz�n�m, zm�nou po�ad� ��dk� atp., �e�� i pro p��pad nap�. 2->3,3->4 pomoc� atributu objektu probehla_aktualizace_prirazeni_pohonu (aby prvn� nebyl p�i�ezn pohon s id 2 na 3 a potom v�echny pohony s id 3 na pohon 4, proto�e m�ly b�t p�i�azen� jen n�kter�...)
				Form1->d.v.aktualizace_prirazeni_pohonu_k_objektum(getPID(i),i);
			}
			//po dokon�en� aktualizace p�i�azen� pohonu (p�i ukl�d�n� pohonu na PL) vr�t� atribut probehla_aktualizace_prirazeni_pohonu v�ech objekt� na false, aby bylo p�ipraveno k dal��mu op�tovn�mu u��t�, nep��mo spolupracuje s metodou v��e uvedenou aktualizace_prirazeni_pohonu_k_objektum
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
				//doplnil 1.2.2018 M, aktualizace i �asov�ch os a pop�. ROMA, nutn� z d�vodu zm�ny parametr� objektu
				//zv�it zda neimplementovat rovnou do aktualizace objektu, ale zat�m nejde z d�vodu, �e se vol� ve Form1 z d�vodu "fale�n�ho" p�ekresledn� �asov�ch os p�ed ROMA
				Form1->aktualizace_maro_a_roma();//aktualizace a p�epo�et volan� kv�li �asov�m os�m (maro) a techn.proces�m(roma)
			}

			Form1->DuvodUlozit(true);
			//M toto tu nesm� b�t:Form_parametry_linky->Close();
		}
		Close();//v testu, m��e padat
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_ADD_Click(TObject *Sender)
{
	//nav��� po�et ��dk�
	rStringGridEd_tab_dopravniky->RowCount++;
  if(rStringGridEd_tab_dopravniky->RowCount>4)
  {
  rStringGridEd_tab_dopravniky->Height=rStringGridEd_tab_dopravniky->Height+30;
  Form_parametry_linky->Height= Form_parametry_linky->Height+30;
  scGPGlyphButton_ADD->Top=scGPGlyphButton_ADD->Top + 30;
  Button_save->Top=Button_save->Top + 30;
  Button_storno->Top=Button_storno->Top + 30;
  scGPGlyphButton_DEL_nepouzite->Top=scGPGlyphButton_DEL_nepouzite->Top+30;
  }
	//zobrazOramovani=false;

	//za�ne plnit jednotliv� ��dky
	int i = rStringGridEd_tab_dopravniky->RowCount - 1;//po�ad� ��dku o jedni�ku ni��� ne� po�et ��dk�
	rStringGridEd_tab_dopravniky->Cells[0][i] = getMaxPID()+1;
	rStringGridEd_tab_dopravniky->Cells[1][i] = "nov� pohon "+AnsiString(getMaxPID());//rStringGridEd_tab_dopravniky->Cells[1][i - 1];
	rStringGridEd_tab_dopravniky->Cells[2][i] = "";//rStringGridEd_tab_dopravniky->Cells[2][i - 1];
	rStringGridEd_tab_dopravniky->Cells[3][i] = "";//rStringGridEd_tab_dopravniky->Cells[3][i - 1];
	rStringGridEd_tab_dopravniky->Cells[4][i] = "";//rStringGridEd_tab_dopravniky->Cells[4][i - 1];
	rStringGridEd_tab_dopravniky->Cells[5][i] = "";
	rStringGridEd_tab_dopravniky->Cells[6][i] = "";
	rStringGridEd_tab_dopravniky->Cells[7][i] = "";
	rStringGridEd_tab_dopravniky->Cells[8][i] = "nepou��v�n";

	//dopln� min Rz
	show_min_Rz();

	//p�esune focus na posledn� ��dek
	rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;

	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
	pozice_scGPGlyphButton_hint();

	//existuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
	scGPGlyphButton_DEL_nepouzite->Visible=true;
 // input_clicked_edit=od_klik;
	INPUT(2,i);
	OUTPUT(0,2,i);
	rStringGridEd_tab_dopravniky->Invalidate();
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//sma�e posledn� ��dek - ji� se nepou��v�, ale nechv�m
void __fastcall TForm_parametry_linky::Button_DEL_Click(TObject *Sender)
{
	if(Form1->d.v.pohon_je_pouzivan(rStringGridEd_tab_dopravniky->RowCount-1))
	{
		AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(rStringGridEd_tab_dopravniky->RowCount-1),true);
				if(mrOk==Form1->MB("Pohon je pou��v�n pro objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_OKCANCEL)){

				//p�vodn� zakomentovan� konstrukcenefunguje spr�vn� pro p��pad storna, proto �e�� n�sleduj�c� ��dek, Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(rStringGridEd_tab_dopravniky->RowCount-1));
				zrusena_prirazeni_PID[getPID(rStringGridEd_tab_dopravniky->RowCount-1)-1]=true;
				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

					if(rStringGridEd_tab_dopravniky->RowCount>1)
					{
					rStringGridEd_tab_dopravniky->RowCount--;
					}
				 //	Form1->MB("Smazano");
				}
				else
				{
				 //storno   - nic se ned�je
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

	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
	pozice_scGPGlyphButton_hint();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Vypis_pohonyClick(TObject *Sender)
{
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

	 while (ukaz!=NULL)
	 {
	 		ShowMessage(ukaz->n);
			ShowMessage(ukaz->name);
			ShowMessage(ukaz->roztec);

			ukaz = ukaz->dalsi;
	 }
}
//---------------------------------------------------------------------------
//zobraz� panel se navr�en�mi pohony
void __fastcall TForm_parametry_linky::scGPButton_doporuceneClick(TObject *Sender)
{
		scExPanel_doporuc_pohony->Visible=true;
		scGPButton_doporucene->Visible=false;
		if(F->pom==NULL)//pokud je vol�no PL p��mo                        //zajist� zobrazen� ve stejn�ch jednotk�ch jako na PO
		scHTMLLabel_doporuc_pohony->Caption=F->d.v.navrhni_POHONY("</br>",F->ms.a2i(F->readINI("nastaveni_form_parametry", "RDt")));
		else// pokud je PL vol�no z PO                                    //zajist� zobrazen� ve stejn�ch jednotk�ch jako na PO
		scHTMLLabel_doporuc_pohony->Caption=F->d.v.navrhni_POHONY("</br>",Form_parametry->RDunitT);
		if(scHTMLLabel_doporuc_pohony->Caption=="")
		{
			scHTMLLabel_doporuc_pohony->Caption="Nejsou k dispozici ��dn� navr�en� pohony";
			scGPGlyphButton_add_mezi_pohony;
			scGPGlyphButton_add_mezi_pohony->Visible=false;
		}
		else
		{
			scGPGlyphButton_add_mezi_pohony->Visible=true;
		}
		//���ka komponenty dle aktu�ln� zobrazen�ho textu
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

	if(Taktunit==MIN)//pokud je v MM, tak p�epne na metry
	{
		Taktunit=S;
		//delka - p�epo��t�n�
		rEditNum_takt->Value=rEditNum_takt->Value*60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[s]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Taktunit=MIN;
		//delka - p�epo��t�n�
		rEditNum_takt->Value=rEditNum_takt->Value/60.0;
		rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[m]</font>";
	}

}
//---------------------------------------------------------------------------
//tla��tko na kop�rov�n� doporu�en�ch pohon� do striggridu, nep�id�v� ale do pohon�
void __fastcall TForm_parametry_linky::scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender)
{
	 //najde max pou�it� ID pohonu (proto�e ID nejsou se�azena, nutno hledat, nikoliv vz�t id z posledn�ho ��dku)
	 unsigned int ID=getMaxPID();

	 //nov� konstrukce zaji��uj�c� pouze vlo�en� do stringgridu, o samotn� ulo�en� pohon� se star� a� tla��tko ulo�it
	 AnsiString T=scHTMLLabel_doporuc_pohony->Caption;

	 while(T.Pos("</br>"))//bude parsovat dokud bude </br>
	 {
	 	//zv��en� po�tu ��dk�
   	rStringGridEd_tab_dopravniky->RowCount++;
   	unsigned int i=rStringGridEd_tab_dopravniky->RowCount-1;//pouze zkr�cen� z�pisu
	 	//pln�n� ��dku a parsov�n� daty
		rStringGridEd_tab_dopravniky->Cells[0][i]=++ID;//ID
		rStringGridEd_tab_dopravniky->Cells[1][i]="Navr�en� pohon pro"+Form1->ms.EP(T,":",".");//n�zev pohonu a vy�e�t�n� �et�zce pro dal�� u�it�
		rStringGridEd_tab_dopravniky->Cells[2][i]=Form1->ms.EP(T,"Navr�en� pohon s rychlost�"," [");//T=Form1->ms.TrimLeftFrom_UTF(T," </br>");
		rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
		//pro aRD a� bude sloupec: rStringGridEd_tab_dopravniky->Cells[?][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
		rStringGridEd_tab_dopravniky->Cells[8][i]="nepou��v�n";
		//smaz�n� jednoho ji� nepot�ebn�ho z�znamu
		T=Form1->ms.TrimLeftFromText(T,"</br>");
	 }
	 //smaz�n� ji� nahran�ch a skryt� panelu
	 scHTMLLabel_doporuc_pohony->Caption="Nejsou k dispozici ��dn� navr�en� pohony";//nepou��v� se tento v�pis viditeln�, situace nenastane
	 scExPanel_doporuc_pohony->Visible=false;
	 scGPButton_doporucene->Visible=true;
	 //p�esune focus na posledn� ��dek
	 rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;
	 //pozice info tla��tka - asi je tla��tko stejn� provizorn�
	 pozice_scGPGlyphButton_hint();
	 //existuj� ur�it� nepou��t� pohony a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
	 scGPGlyphButton_DEL_nepouzite->Visible=true;
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
		 if(PopUPmenu->Visible)//pokud je zobrazeno pop-up, tak skryje to
		 PopUPmenu->Visible=false;
		 else //jinak ukon�en� formul��e
		 {
			Button_stornoClick(Sender);
			Close();//mus� tu b�t, proto�e Button_storno ukon�uje form pouze p�es modalresults a bez tohoto by se neukon�ilo
		 }
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
  	//if(Col==5)  // Roletka_roztec(Row);
	 // pokud je pohon ve stavu pou��v�n, vygeneruji roletku kter� umo�n� prov�st zm�nu na nepou��v�n

		if(Col==8 && rStringGridEd_tab_dopravniky->Cells[8][Row]!="nepou��v�n")
    {
			 EditStyle=sgbDropDown;
			 rStringGridEd_tab_dopravniky->Columns->Items[8]->PickList->Clear();
			 rStringGridEd_tab_dopravniky->Columns->Items[8]->PickList->Add("nepou��v�n");
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

		//Pokud je pohon pou��v�n, nastav�m polo�ky rychlost, rozte� atd jako readonly - nelze je m�nit.
//	 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
//	 {
//		 if(rStringGridEd_tab_dopravniky->Cells[5][i]=="" && rStringGridEd_tab_dopravniky->Cells[8][i]=="nepou��v�n" )
//		 {
//			// if(Row==i && Col==6) CanEdit=false;      //nakonec je v�dy povoleno editovat
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

			 //pokud se nejedn� o ��dek, kde pr�v� doch�z� k validaci a z�rove� vynech�m nult� ��dek (hlavi�ka)
			 //tak do v�ech sloupc� d�m �edou barvu

			 if(Row!=Row_validace && Row!=0 && Col==1)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==2)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==3)  	  CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==4)  	  CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==5)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==6)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==7)  		CanEdit=false;
			 if(Row!=Row_validace && Row!=0 && Col==8)  		CanEdit=false;
       if(Row==Row_validace && Row!=0 && Col!=Col_validace) 	CanEdit=false;


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
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�

		rHTMLLabel_delka_jig->Caption="d�lka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value/1000.0;


		rHTMLLabel_sirka_jig->Caption="���ka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value/1000.0;

		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_podvozek->Value=scGPNumericEdit_delka_podvozek->Value/1000.0;
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;

		rHTMLLabel_delka_jig->Caption="d�lka <font color=#2b579a>[mm]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value*1000.0;


		rHTMLLabel_sirka_jig->Caption="���ka <font color=#2b579a>[mm]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value*1000.0;

		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[mm]</font>";
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
 F->d.vykresli_packy_PL(Canvas,scGPButton_zamek_aRD,scGPButton_zamek_roztec,scGPButton_zamek_Rz,scGPButton_zamek_Rx);

	if(zobrazitFrameForm)Form1->m.frameForm(Form_parametry_linky,clWebOrange,1);

	if(VID==-1) scGPGlyphButton_ADD->Enabled=true;
	else        scGPGlyphButton_ADD->Enabled=false;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry_linky::show_min_Rz()
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zavol� n�hled kabiny, p��padn� v budoucnu info a o po�adovan�ch parametrech
void __fastcall TForm_parametry_linky::scGPGlyphButton_infoClick(TObject *Sender)
{
		bool zFFtemp=false;if(zobrazitFrameForm){zFFtemp=true;zobrazitFrameForm=false;Invalidate();}//pokud je or�mov�n�, tak zru��, aby mohlo m�t or�mov�n� jen na formu kabina_schema, ale zapamatuje si stav pro n�sledn� navr�cen�
		// formul�� na st�ed
		if(!Form_objekt_nahled->Visible)
		{
			Form_objekt_nahled->Left = Form1->ClientWidth / 2 - Form_objekt_nahled->Width / 2;
			Form_objekt_nahled->Top = Form1->ClientHeight / 2 - Form_objekt_nahled->Height / 2;
			// zobrazeni formul��e
			Form_objekt_nahled->scLabel_titulek->Visible=false;
			Form_objekt_nahled->zobrazitFrameForm=true;
			Form_objekt_nahled->ShowModal();
			if(zFFtemp)zobrazitFrameForm=true;//pokud bylo or�mov�n�, tak vr�t�
		}
		else  Form_objekt_nahled->Show();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vr�t� ID pohonu na dan�m ��dku
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
//najde max pou�it� ID pohonu (proto�e ID nejsou se�azena,nelze vz�t index posledn�ho ��dku)
unsigned int TForm_parametry_linky::getMaxPID()
{
	unsigned int ID=0;//id
	for(unsigned i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	if(ID<getPID(i))ID=getPID(i);
	return ID;
}
//---------------------------------------------------------------------------
//vr�t� ��slo ��dku dle pohon ID, pokud nenajde vr�t� -1
int TForm_parametry_linky::getROW(int PID)
{
	int RET=-1;
	for(unsigned i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	{
		if(PID==rStringGridEd_tab_dopravniky->Cells[0][i].ToInt())
		{
			RET=i;
			break;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
void TForm_parametry_linky::zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum()
{   //p�eindexovat nesmazan� nebo jim dat nov� odkaz, nebo mazat jen konkr�tn�
	for(unsigned PID=0;PID<zrusena_prirazeni_PID_size;PID++)
	{
		if(zrusena_prirazeni_PID[PID])
		Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(PID+1);
	}
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
}
//---------------------------------------------------------------------------
//pozice info tla��tka - asi je tla��tko stejn� provizorn�
void TForm_parametry_linky::pozice_scGPGlyphButton_hint()
{

}
//---------------------------------------------------------------------------
//testuje zda existuj� nepou��t� pohony, pokud ano,vrac� true jinak false
//mus� b�t zde, nikoliv ve vektorech, proto�e zde mohou vznikat nov�h n�vrhy na pohony, kter� nejsou je�t� ve spoj�ku POHONY
bool TForm_parametry_linky::existuji_nepouzivane_pohony()
{
	 bool RET=false;
	 for(unsigned int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�eho
	 {
		if(!Form1->d.v.pohon_je_pouzivan(getPID(i)))//pohon nen� pou��van�
		{
			RET=true;
			break;//sta�� naj�t jeden
		}
	 }
	 return RET;
}
//---------------------------------------------------------------------------
//-------------------------POP-UP MENU---------------------------------------
//---------------------------------------------------------------------------
//vol�n� pop-up menu
void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyMouseDown(TObject *Sender,
					TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 if(Button==mbRight && rStringGridEd_tab_dopravniky->Row>0)//je stisknuto prav� tla��tko my�i ve stringridu, tzn. volat popupmenu
	 {
			PopUPmenu->Height=34*2;//zobrazen� dv� polo�ky
			PopUPmenu->Left=X;PopUPmenu->Top=Y+rStringGridEd_tab_dopravniky->Top;//pozice
			//nast�ven� textu polo�e
			scLabel_kopirovat->Caption="  Kop�rovat "+rStringGridEd_tab_dopravniky->Cells[1][rStringGridEd_tab_dopravniky->Row];
			scLabel_smazat->Caption="  Smazat "+rStringGridEd_tab_dopravniky->Cells[1][rStringGridEd_tab_dopravniky->Row];
			//testuje zda existuj� nepou��t� pohony a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
			if(existuji_nepouzivane_pohony())
			{
				PopUPmenu->Height=34*3;//zobrazen� ji� t�i polo�ky
				Item_smazat_nepouzite->Visible=true;
				scGPGlyphButton_DEL_nepouzite->Visible=true;
			}
			else
			{
				Item_smazat_nepouzite->Visible=false;
				scGPGlyphButton_DEL_nepouzite->Visible=false;
			}
			//o�et�en�, pokud je mimo obrazovku + 5 px okraj
			if(PopUPmenu->Left>=ClientWidth-PopUPmenu->Width)//nastala situace �e je mimo obraz (nebo ��ste�n�)
			PopUPmenu->Left=ClientWidth-PopUPmenu->Width-5;
			if(PopUPmenu->Top>=rStringGridEd_tab_dopravniky->Top+rStringGridEd_tab_dopravniky->Height-PopUPmenu->Height)
			PopUPmenu->Top=rStringGridEd_tab_dopravniky->Top+rStringGridEd_tab_dopravniky->Height-PopUPmenu->Height-5;
			PopUPmenu->Visible=true;
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//k��ek, kter� skryje pop-up menu
void __fastcall TForm_parametry_linky::GlyphButton_closeClick(TObject *Sender)
{
	PopUPmenu->Visible=false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
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
//zaji��uje zkop�rov�n� pohonu, vlo�� na konec, nevkl�d�me za kop�rovan� objekt, proto�e podle po�ad� z�st�v� p�i�a�en� k jednotliv�m objekt�m
void __fastcall TForm_parametry_linky::scLabel_kopirovatClick(TObject *Sender)
{
	int ROW=rStringGridEd_tab_dopravniky->Row;
	rStringGridEd_tab_dopravniky->InsertRowEx(ROW+1,false);
	//zkop�rov�n� ��dku za kop�rovan� pohon
	rStringGridEd_tab_dopravniky->Rows[ROW+1]=rStringGridEd_tab_dopravniky->Rows[ROW];
	//n�zev p�ejmenov�n�
	rStringGridEd_tab_dopravniky->Cells[1][ROW+1]=rStringGridEd_tab_dopravniky->Cells[1][ROW]+" - kopie";
	//nepou��v�n
	rStringGridEd_tab_dopravniky->Cells[8][ROW+1]="nepou��v�n";
	//indexace
	rStringGridEd_tab_dopravniky->Cells[0][ROW+1]=getMaxPID()+1;
	//p�esune focus na posledn� ��dek
	rStringGridEd_tab_dopravniky->Row=ROW+1;
	//skryt� pop-up menu
	PopUPmenu->Visible=false;
	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
	pozice_scGPGlyphButton_hint();
	//existuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
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
	top_positon(Item_kopirovat->Top);//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
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
//zaji��uje sm�z�n� pohonu na vybran�m ��dku
void __fastcall TForm_parametry_linky::scLabel_smazatClick(TObject *Sender)
{
	int ROW=rStringGridEd_tab_dopravniky->Row;
	bool smazat=false;
	if(Form1->d.v.pohon_je_pouzivan(getPID(ROW)))//pohon je pou��van�
	{
			AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(ROW),true);
			myMessageBox->zobrazitFrameForm=true;//zajist� or�mov�n� MB
			if(mrYes==Form1->MB("Pohon je pou��v�n objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_YESNO))
			{
				//Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(ROW)); p�vodn� p��m� smaz�n�, ale nereflektovalo by p��padn� storno
				//pozor nen� p�ipraveno na situaci, pokud by bylo mo�n� p��mo v PL p�i�azovan pohony a potom zase odp�i�azovat (muselo by se nav��it pole zrusena_prirazeni_PID)
				zrusena_prirazeni_PID[getPID(ROW)-1]=true;//nahrazeno novou filozofii, z d�vodu mo�n�ho storna formu
				smazat=true;
			}
			myMessageBox->zobrazitFrameForm=false;//zajist� odor�mov�n� MB - kv�li dal��mu pou�it�
	}
	else//pohon nen� pou��van� a m��eme tedy smazat rovnou
	{
		smazat=true;
	}
	//samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
	if(smazat)rStringGridEd_tab_dopravniky->DeleteRowEx(ROW);
	//skryt� pop-up menu
	PopUPmenu->Visible=false;
	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
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
	top_positon(Item_smazat->Top);//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
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
//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�e ho
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteClick(TObject *Sender)
{
	for(int j=1;j<rStringGridEd_tab_dopravniky->RowCount;j++)//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�eho
	{
		if(Form1->d.v.pohon_je_pouzivan(getPID(j))==false)//pohon nen� pou��van�
		{
			//samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
			rStringGridEd_tab_dopravniky->DeleteRowEx(j);
			j--;//mus� po smaz�n� nutn� sn�it index

      rStringGridEd_tab_dopravniky->Height=rStringGridEd_tab_dopravniky->RowCount*30 + 48;
      Form_parametry_linky->Height= rStringGridEd_tab_dopravniky->Height +428;
      scGPGlyphButton_ADD->Top=Form_parametry_linky->Height - 65 ;
      Button_save->Top=Form_parametry_linky->Height - 40;
      Button_storno->Top=Form_parametry_linky->Height - 40;
      scGPGlyphButton_DEL_nepouzite->Top=Form_parametry_linky->Height-30;
      rHTMLLabel_InfoText->Top= scGPGlyphButton_ADD->Top + 8;
      vypis("",false);
		}
	}
	//skryt� pop-up menu
	PopUPmenu->Visible=false;
 	rStringGridEd_tab_dopravniky->FinishEditing();//ukon�� editaci a t�m odstran� nep��jemn� vizu�ln� efekt
	pozice_scGPGlyphButton_hint();//pozice info tla��tka - asi je tla��tko stejn� provizorn�
	//neexistuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch nezobrazovat
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
	top_positon(Item_smazat_nepouzite->Top);//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
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
 // vynech� prvn� ��dek a obarv� sloupec tak jak m� - pokud je zam�en z�mek, je �ediv� sloupec a nastav� se,
 // �e nen� mo�n� sloupec editovat - to se nastavuje v metod� rStringGridEd_tab_dopravnikyCanEdit

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

			 //pokud se nejedn� o ��dek, kde pr�v� doch�z� k validaci a z�rove� vynech�m nult� ��dek (hlavi�ka)
			 //tak do v�ech sloupc� d�m �edou barvu

			 if(Row!=Row_validace && Row!=0 && Col==1)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==2)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==3)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==4)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==5)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==6)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==7)  			 Background=(TColor)RGB(212,212,212);
			 if(Row!=Row_validace && Row!=0 && Col==8)  			 Background=(TColor)RGB(212,212,212);

       if(Row==Row_validace && Row!=0 && Col!=Col_validace)  Background=(TColor)RGB(212,212,212);


       }

}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scHTMLLabel_doporuc_pohonyClick(TObject *Sender)
{
		// zavol� funkcionalitu tla��tka na kop�rov�n� navr�en�ch pohon� do striggridu, nep�id�v� ale do pohon�
	scGPGlyphButton_add_mezi_pohonyClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPSwitchChangeState(TObject *Sender)
{
if(scGPSwitch->State==0) {rHTMLLabel_podvozek_zaves->Caption="Podvozek";rHTMLLabel_podvozek_zaves->Left=34;}
else  {rHTMLLabel_podvozek_zaves->Caption="Z�v�s"; rHTMLLabel_podvozek_zaves->Left=56; }
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::FormClose(TObject *Sender, TCloseAction &Action)

{
	//ShowMessage(Delkaunit);
	Form1->writeINI("nastaveni_form_parametry_linky", "rozmery", Delkaunit);
	Form1->writeINI("nastaveni_form_parametry_linky", "TT", Taktunit);
  Form1->writeINI("nastaveni_form_parametry_linky", "R", Runit);
  //zapis do PO ini
 	Form1->writeINI("nastaveni_form_parametry", "RDt", aRDunit);
  Form1->writeINI("nastaveni_form_parametry", "DM", Dmunit);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry_linky::vypis(UnicodeString text,bool red,bool link)
{
		Button_save->Enabled=true;
		Button_save->Caption = "Ulo�it";
		if (text != "") // zobraz� a vyp�e
		{
				rHTMLHint1->ToString()=text;//nate�en� do hintu zaji��uje zobrazen� cel�ho textu, nepou��v� se klasick� hint
				//prodllu�en� formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl p�edt�m popisek skryt� + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnut� podtr�en�ho p�sma
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
				//zkr�cen� formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
				rHTMLLabel_InfoText->Visible = false;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rEditNum_taktClick(TObject *Sender)
{
//
		 input_clicked_edit=TT_klik;
		 input_clicked_icon=empty_klik_ico;
		 Nastav_zamky(empty_klik_ico,TT_klik);

		 Form_TT_kalkulator->Left=Form1->ClientWidth/2-Form_TT_kalkulator->Width/2;
		 Form_TT_kalkulator->Top=Form1->ClientHeight/2-Form_TT_kalkulator->Height/2;

		 //p�ekreslen� podbarven� sloupc�
		 rStringGridEd_tab_dopravniky->Visible=false;
		 rStringGridEd_tab_dopravniky->Visible=true;

		 vypis("Pozor, p�i zm�n� taktu dojde p�i ulo�en� ke zm�n� hodnot aktu�ln� rychlosti pohonu nebo rozte�ov� vzd�lenosti a dal��ch parametr� dle nastaven�ch z�mk� v tabulce pohon�. ",false);
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::Nastav_zamky(Tinput_clicked_icon I,Tinput_clicked_edit E)
{

 if(I==empty_klik_ico && E==empty_klik) //v�choz� nastaven� z�mk� - p�i formshowformu
 {

		scGPButton_zamek_Rz->Visible=true;

		scGPButton_zamek_aRD->ImageIndex=37; // zamknu aRD
		scGPButton_zamek_Rz->ImageIndex=37; // zamknu Rz - jsou v korelaci

		//odem�eno  Rx a Rz
		scGPButton_zamek_Rx->ImageIndex=38;
		scGPButton_zamek_roztec->ImageIndex=38;
 }

 ///////////////////////////////////////////////

	if(I==empty_klik_ico && E==TT_klik) // nastaven� z�mk� p�i kliku do TT
 {
		//	 ShowMessage("nastav");
		scGPButton_zamek_aRD->ImageIndex=37; // zamknu aRD
		scGPButton_zamek_Rz->ImageIndex=38; // odemknu Rz

		//odem�eno  Rx a Rz
		scGPButton_zamek_Rx->ImageIndex=38;
		scGPButton_zamek_roztec->ImageIndex=38;
 }

	if(I==aRD_klik_ico && E==aRD_klik) // nastaven� z�mk� p�i kliku do aRD s p�edchoz�m vstupem do aRD - souvis� s formshow
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

 if(I==aRD_klik_ico && E==TT_klik) {  //nastaven� z�mk� p�i kliku do TT  a za�nu m�nit z�mek aRD


 	if(scGPButton_zamek_aRD->ImageIndex==37) //odem�eno
			{
				scGPButton_zamek_aRD->ImageIndex=38; // odemknu aRD
				scGPButton_zamek_Rz->ImageIndex=37; // zamknu Rz

				 //odem�eno roztec
				scGPButton_zamek_roztec->ImageIndex=38; //odem�eno
				scGPButton_zamek_Rx->ImageIndex=38;
			}
			else
			{
			//odem�eno
			 scGPButton_zamek_aRD->ImageIndex=37;  //zamknu aRD
			 scGPButton_zamek_Rz->ImageIndex=38;   //odemknu Rz

				 //odemceno  roztec   a Rx
				scGPButton_zamek_roztec->ImageIndex=38;
				scGPButton_zamek_Rx->ImageIndex=38;


			}
 }

 ///////////////////////////////////////////////////////////////////////


	if(I==aRD_klik_ico && E==empty_klik)   //obecn� nastaven� z�mku aRD bez ovlivn�n� vstupu TT
	{


			if(scGPButton_zamek_aRD->ImageIndex==37) //zam�eno
			{
				scGPButton_zamek_aRD->ImageIndex=38; // odemknu aRD
				scGPButton_zamek_Rz->ImageIndex=38; // odemknu Rz - jsou v korelaci

				scGPButton_zamek_roztec->ImageIndex=37; //zam�eno
				scGPButton_zamek_Rx->ImageIndex=38;      //odem�eno
			}
			else
			{
			//zamknu
			 scGPButton_zamek_aRD->ImageIndex=37;
			 scGPButton_zamek_Rz->ImageIndex=37;

				 //odemknu  roztec   a zamknu Rx
				scGPButton_zamek_roztec->ImageIndex=38;     //odem�eno
				scGPButton_zamek_Rx->ImageIndex=37;       //zam�eno


			}

	}
		 // nastaven� chov�n� z�mku Rx, v p��pad�, �e p�edt�m do�lo ke vstupu do bu�ky aRD

	//Nastav_zamky(Rx_klik_ico,aRD_klik)

	if(E==aRD_klik) {

					 //Rz nen� vid�t a lze jej m�nit
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

				 //Rz nen� vid�t a lze jej m�nit
				 scGPButton_zamek_Rz->ImageIndex=38;
				// scGPButton_zamek_Rz->Visible=false;

			 } else

			 {

				 scGPButton_zamek_Rx->ImageIndex=37;
				 scGPButton_zamek_roztec->ImageIndex=38;  //odemknu R

				 //Rz nen� vid�t a lze jej m�nit
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

				 //Rz nen� vid�t a lze jej m�nit
				 scGPButton_zamek_Rz->ImageIndex=38;
				// scGPButton_zamek_Rz->Visible=false;

			 } else

			 {

				 scGPButton_zamek_Rx->ImageIndex=38;
				 scGPButton_zamek_roztec->ImageIndex=37;  //odemknu R

				 //Rz nen� vid�t a lze jej m�nit
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

				if(scGPButton_zamek_aRD->ImageIndex==37)
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

      if(I==Rx_klik_ico && E==R_klik)
      {

      	if(scGPButton_zamek_Rx->ImageIndex==37)
				{
						scGPButton_zamek_Rx->ImageIndex=38;

            scGPButton_zamek_Rz->ImageIndex=37;
						scGPButton_zamek_aRD->ImageIndex=37;




				}   else

				{
						scGPButton_zamek_Rx->ImageIndex=37;

            scGPButton_zamek_Rz->ImageIndex=38;
						scGPButton_zamek_aRD->ImageIndex=38;

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


			 //Rz nastaven�
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
       ////////////////////////////////////

       		 if(I==Rz_klik_ico && E==empty_klik)    //vstup z Rz zamku bez predchoziho kliku do editu
			 {
				if(scGPButton_zamek_Rz->ImageIndex==38)  //odemceno
				{
					scGPButton_zamek_Rz->ImageIndex=37;



				}   else   //Rz zam�eno

				{

						scGPButton_zamek_Rz->ImageIndex=38;

				}

			 }
			 Invalidate();
			 FormPaint(this);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPButton_zamek_aRDClick(TObject *Sender)

{
		 //pokud p�ed klikem do aRD bylo kliknuto do TT, vol� se jin� chov�n� z�mk� - aRD a Rz nemus� b�t prov�z�ny
	 input_clicked_icon=aRD_klik_ico;

	 if(input_clicked_edit==TT_klik) Nastav_zamky(aRD_klik_ico,TT_klik);
	 if(input_clicked_edit==R_klik)  Nastav_zamky(aRD_klik_ico, R_klik);
	 if(input_clicked_edit==Rx_klik)  Nastav_zamky(aRD_klik_ico, Rx_klik);
	 if(input_clicked_edit==aRD_klik) Nastav_zamky(aRD_klik_ico,aRD_klik);

	 if(input_clicked_edit==empty_klik) Nastav_zamky(aRD_klik_ico, empty_klik);


		//	ShowMessage(input_clicked_edit);  //6
	 //	ShowMessage(input_clicked_icon);   //0


		//workaround
		//tato konstukce za��d�, �e se okam�it� nastav� �ed� pozad� pro zam�en� z�mek
		// pokud by to zde nebylo, tak se �ed� pozad� vykresl� a� p�i p�ejet� my�� p�es zam�en� sloupec, co� je pozd�
		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
    rStringGridEd_tab_dopravniky->SetFocus();

}
//---------------------------------------------------------------------------

void TForm_parametry_linky::input_TT()

{
	input_state=TT;
	INPUT(0,0);   // p�i vol�n� INPUT z TT je sou��st� rovnou i vol�n� OUTPUT + vol�n� v�po�etn�ho modelu
	input_state=NOTHING;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TForm_parametry_linky::INPUT(double Sloupec, double Radek)
{
		 Memo2->Lines->Add("INPUT"+ AnsiString(input_state));
	 //Nastaven� z�mk�
	 if(scGPButton_zamek_aRD->ImageIndex==37)  pm.aRD_locked=true;    //zam�en aRD
	 else  pm.aRD_locked=false;

	 if(scGPButton_zamek_roztec->ImageIndex==37)  pm.R_locked=true;    //zam�en R
	 else  pm.R_locked=false;

	 if(scGPButton_zamek_Rz->ImageIndex==37)  pm.Rz_locked=true;    //zam�en Rz
	 else  pm.Rz_locked=false;

	 if(scGPButton_zamek_Rz->ImageIndex==37)  pm.Rx_locked=true;    //zam�en Rx
	 else  pm.Rx_locked=false;


	 pm.TT=getTT();



 if(input_state==TT || input_state==aRD_prevod || input_state==R_prevod || input_state==Rz_prevod)  //v�tev TT - aktu�ln� nen� vyu��v�na, POUZE PRO P�EVOD
 {

 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	 {

  if(aRDunit==S)  pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]); else pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]) /60.0;
  if(Runit==M)		pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i]); else pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][i])/1000.0;
  if(Dmunit==M) 	pm.Rz =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i]); else pm.Rz =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i])/1000.0;
		              pm.Rx =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][i]);

		OUTPUT(i,0,0);  // po projit� ��dku, okam�it� vol�n� output - napln�n� obsahem konkr�tn�ho ��dku

    }

	} else    // aktu�ln� vyu��v�na pouze tato konstukce n�e

	{
		 // ulo�en� do struktury konkr�tn�ho ��dku
		pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][Radek])/(1+60*aRDunit);
		pm.R  =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][Radek])/(1+999*Runit);
		pm.Rz =  F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][Radek])/(1+999*Dmunit);
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

		//Memo3->Lines->Add("OUTPUT"+ AnsiString(input_state));
    rHTMLLabel_info_zmenaR->Caption="";

	 if(i>0) // pln�n� cel� tabulky, pouze v p��pad� zm�ny TT nebo p�evodu jednotek
	 { //obraceny "logicky" postup - pokud m�m nastaven� s, chci zobrazit v m/min, �ili dle toho p�evedu

   if(input_state==aRD_prevod)
   {
    if(aRDunit==S)
    {
     rStringGridEd_tab_dopravniky->Cells[4][i]=pm.aRD*60.0;
     rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[3][i]*60.0;
     rStringGridEd_tab_dopravniky->Cells[2][i]=rStringGridEd_tab_dopravniky->Cells[2][i]*60.0;

    } else
    {
      rStringGridEd_tab_dopravniky->Cells[4][i]=pm.aRD;
      rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[3][i]/60.0;
      rStringGridEd_tab_dopravniky->Cells[2][i]=rStringGridEd_tab_dopravniky->Cells[2][i]/60.0;

    }
   }

   if(input_state==R_prevod)
   {
    if(Runit==M)
    {
    rStringGridEd_tab_dopravniky->Cells[5][i]=pm.R*1000.0;
    } else   rStringGridEd_tab_dopravniky->Cells[5][i]=pm.R;
   }

   if(input_state==Rz_prevod)
   {
     if(Dmunit==M)
    {
    rStringGridEd_tab_dopravniky->Cells[6][i]=pm.Rz*1000.0;
    } else  rStringGridEd_tab_dopravniky->Cells[6][i]=pm.Rz;
   }

		rStringGridEd_tab_dopravniky->Cells[7][i]=pm.Rx;
	 }

	 else
	 {
			// napln�n� konkr�tn�ho ��dku na kter�m do�lo ke zm�n�

		if(input_state!=aRD)  rStringGridEd_tab_dopravniky->Cells[4][Radek]=pm.aRD*(1+60*aRDunit);
		if(input_state!=R)    rStringGridEd_tab_dopravniky->Cells[5][Radek]=pm.R*(1+999*Runit);
		if(input_state!=RZ)   rStringGridEd_tab_dopravniky->Cells[6][Radek]=pm.Rz*(1+999*Dmunit);
		if(input_state!=RX)   rStringGridEd_tab_dopravniky->Cells[7][Radek]=pm.Rx;

		VALIDACE(Sloupec,Radek);

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


if(ACol==4) {    //zm�na aRD

 input_clicked_edit=aRD_klik;
 Nastav_zamky(empty_klik_ico,aRD_klik);
// if(ReadOnly) scGPButton_zamek_aRD->Visible=true;
 scGPButton_zamek_Rz->Visible=false; //tato prom�nn� je v�dy ovlivn�na, schov�m jej� z�mek

 if(scGPButton_zamek_aRD->ImageIndex==38) scGPButton_zamek_aRD->Visible=false;
 else  scGPButton_zamek_aRD->Visible=true;

	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_roztec->Visible=true;

 rStringGridEd_tab_dopravniky->Invalidate();

}
if(ACol==5) {    //zm�na R

 input_clicked_edit=R_klik;
 Nastav_zamky(empty_klik_ico,R_klik);

 Roletka_roztec(ARow);
 if(scGPButton_zamek_roztec->ImageIndex==38)  scGPButton_zamek_roztec->Visible=false;
 else   scGPButton_zamek_roztec->Visible=true;
	scGPButton_zamek_Rz->Visible=true;
	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_aRD->Visible=true;

}
if(ACol==6) {    //zm�na Rz

  input_clicked_edit=Rz_klik;
	Nastav_zamky(empty_klik_ico,Rz_klik);
	scGPButton_zamek_aRD->Visible=false;   //tato prom�nn� je v�dy ovlivn�na, schov�m jej� z�mek

	if(scGPButton_zamek_Rz->ImageIndex==38)  scGPButton_zamek_Rz->Visible=false;
 else scGPButton_zamek_Rz->Visible=true;

	scGPButton_zamek_Rx->Visible=true;
	scGPButton_zamek_roztec->Visible=true;

}
if(ACol==7) {    //zm�na Rx

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

	if(input_clicked_edit==aRD_klik) Nastav_zamky(Rx_klik_ico,aRD_klik);  //v p��d�, kdy do�lo ke kliknut� do bu�ky aRD nastav�m chov�n� Rx
	if(input_clicked_edit==Rz_klik) Nastav_zamky(Rx_klik_ico,Rz_klik);
  if(input_clicked_edit==R_klik)  Nastav_zamky(Rx_klik_ico,R_klik);

	else   Nastav_zamky(Rx_klik_ico,empty_klik);

		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
    rStringGridEd_tab_dopravniky->SetFocus();

}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikySetEditText(TObject *Sender,
					int ACol, int ARow, const UnicodeString Value)
{

				 //tato ud�lost je vol�na v�dy, nikoliv pouze p�i zm�n� obsahu - stringgrid nem� nativn� onchange ud�lost
				 // z tohoto d�vodu je p�i spln�n� podm�nky ihned ulo�en obsah bu�ky, a n�sledn� porovn�n.
				 // pokud je rozd�ln�, do�lo ke zm�n� a vol�m v�po�etn� model
    if(ACol==1 || ACol==2 || ACol==3 || ACol==4 || ACol==5 || ACol==6 || ACol==7)
    {
    rHTMLLabel_info_zmenaR->Caption="";


    }
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

				if(rStringGridEd_tab_dopravniky->Cells[4][ARow]!=pm.aRD*(1+59.0*aRDunit))
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_aRD");
				input_state=aRD;
				INPUT(ACol,ARow);
				pm.input_aRD();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
			 if(rHTMLLabel_InfoText->Caption=="")	vypis("Do�lo ke zm�n� obsahu roletky rozte�e, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypo��t�n� nov�ch dat do roletky na z�klad� zm�ny Rz
				zobrazOramovani=true;

				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepou��v�n")
					{
					F_gapoR->pohony_zmena[getPID(ARow)].X=true;
          rHTMLLabel_info_zmenaR->Caption="P�i ukl�d�n� bude nutn� prov�st �pravu parametr� objekt� (GAPO R).";
					}
				input_state=NOTHING;
				}

				}


		}

	 if(ACol==5)
		{
				if(input_state==NOTHING && input_clicked_edit==R_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[5][ARow]!=pm.R*(1+999.0*Runit))
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_R");
				input_state=R;
				INPUT(ACol,ARow);
				pm.input_R();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
			 if(scGPButton_zamek_aRD->ImageIndex==38 && rHTMLLabel_InfoText->Caption=="" )  //pokud je aRD odem�eno vol�m akt.data do roletky
			 {	Roletka_roztec(ARow);
					vypis("Do�lo ke zm�n� obsahu roletky rozte�e, vyberte hodnotu.",false);
					Memo2->Lines->Add("volam aktualiz R");
					zobrazOramovani=true;
					if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepou��v�n")
					{
					F_gapoR->pohony_zmena[getPID(ARow)].X=true;
         rHTMLLabel_info_zmenaR->Caption="P�i ukl�d�n� bude nutn� prov�st �pravu parametr� objekt� (GAPO R).";
					}
			 } //vypo��t�n� nov�ch dat do roletky na z�klad� zm�ny Rz
			 else zobrazOramovani=false;

				input_state=NOTHING;
				}

				}

		}

		 if(ACol==6)
		{
				if(input_state==NOTHING && input_clicked_edit==Rz_klik)
				{

				if(rStringGridEd_tab_dopravniky->Cells[6][ARow]!=pm.Rz*(1+999.0*Dmunit))
				{
				Memo2->Lines->Clear();
				Memo2->Lines->Add("volam model pm.input_Rz");
				input_state=RZ;
				INPUT(ACol,ARow);
				pm.input_Rz();
				OUTPUT(0,ACol,ARow);
				rStringGridEd_tab_dopravniky->Invalidate();
			  if(rHTMLLabel_InfoText->Caption=="")		vypis("Do�lo ke zm�n� obsahu roletky rozte�e, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypo��t�n� nov�ch dat do roletky na z�klad� zm�ny Rz
				zobrazOramovani=true;
				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepou��v�n")
				{
					F_gapoR->pohony_zmena[getPID(ARow)].X=true;
          rHTMLLabel_info_zmenaR->Caption="P�i ukl�d�n� bude nutn� prov�st �pravu parametr� objekt� (GAPO R).";
				}

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
				if(scGPButton_zamek_roztec->ImageIndex==37 && rHTMLLabel_InfoText->Caption=="")
				{
				zobrazOramovani=true;
				vypis("Do�lo ke zm�n� obsahu roletky rozte�e, vyberte hodnotu.",false);
				Roletka_roztec(ARow); //vypo��t�n� nov�ch dat do roletky na z�klad� zm�ny Rz
				if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepou��v�n")
				{
				F_gapoR->pohony_zmena[getPID(ARow)].X=true;
        rHTMLLabel_info_zmenaR->Caption="P�i ukl�d�n� bude nutn� prov�st �pravu parametr� objekt� (GAPO R).";
				}
				}    else zobrazOramovani=false;
				rStringGridEd_tab_dopravniky->Invalidate();
				input_state=NOTHING;
				}

				}

		}



//		if (ACol==4)  //aRD  // tohle je dobr�, �e v ARow m�m p�edanej ��dek zrovna, kter� edituji, �ili pak velmi jednodu�e to p�ed�m tam kam pot�ebuji
//		{
//
//				//  pokud m� rozte� a je nepou��van� tak dovol�m dopo��t�vat resp. navrhovat hodnoty k ulo�en�
//		 if(!rStringGridEd_tab_dopravniky->Cells[5][ARow].IsEmpty() /*&& rStringGridEd_tab_dopravniky->Cells[8][ARow]=="nepou��v�n"*/)
//			{
//				//Doporu�en� Rz
//				rStringGridEd_tab_dopravniky->Cells[6][ARow]=F->m.Rz(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/60.0);
//
//			//Doporu�en� rozte��
//				double Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]); //p�ed�m Rz
//					AnsiString data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";");
//
//			 //	EditStyle=sgbDropDown;   //vyber typu  - nyn� je nastaven glob�ln� v dfm
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
//				TStringList *S=new TStringList;
//				S->Add(data);
//				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
//				S->Delimiter=';';     //nutno v jednoduch�ch uvozovk�ch, dvoj� hod� chybu p�i p�ekladu
//				S->DelimitedText=data;

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyGetEditText(TObject *Sender,
					int ACol, int ARow, UnicodeString &Value)
{

		//tato ud�lost je vol�na okam�it� po vstupu do bu�ky a ulo�� do struktury akt.zobrazen� data
		 if(ACol==1)   NAZEV=rStringGridEd_tab_dopravniky->Cells[1][ARow];

		 if(ACol==2)   OD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][ARow]);

		 if(ACol==3)   DO = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][ARow]);

		 if(ACol==4)	pm.aRD = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]);

		 if(ACol==5)  pm.R = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]);

		 if(ACol==6)  pm.Rz = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]);

		 if(ACol==7)  pm.Rx = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPButton_zamek_roztecClick(TObject *Sender)

{
		if(input_clicked_edit==aRD_klik) Nastav_zamky(R_klik_ico,aRD_klik);  //v p��d�, kdy do�lo ke kliknut� do bu�ky aRD nastav�m chov�n� Rx
	//	if(input_clicked_edit==R_klik) Nastav_zamky(R_klik_ico,R_klik);  - nen� pot�eba, je schov�n z�mek
		if(input_clicked_edit==Rz_klik) Nastav_zamky(R_klik_ico,Rz_klik);
		if(input_clicked_edit==Rx_klik) Nastav_zamky(R_klik_ico,Rx_klik);

		if(input_clicked_edit==empty_klik) Nastav_zamky(R_klik_ico,empty_klik);   // std.chov�n�


		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
    rStringGridEd_tab_dopravniky->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scGPButton_zamek_RzClick(TObject *Sender)
{

	 input_clicked_icon=Rz_klik_ico;
	// if(input_clicked_edit==Rz_klik) Nastav_zamky(Rz_klik_ico,Rz_klik);   - nen� pot�eba, je schov�n z�mek
	 if(input_clicked_edit==R_klik)	 Nastav_zamky(Rz_klik_ico,R_klik);
	 if(input_clicked_edit==Rx_klik)	 Nastav_zamky(Rz_klik_ico,Rx_klik);

	 if(input_clicked_edit==empty_klik){ Nastav_zamky(aRD_klik_ico,empty_klik);} //shodne chov�n� jako klik do aRD

		rStringGridEd_tab_dopravniky->Visible=false;
		rStringGridEd_tab_dopravniky->Visible=true;
    rStringGridEd_tab_dopravniky->SetFocus();
}

//---------------------------------------------------------------------------

void TForm_parametry_linky::Roletka_roztec(int Row)

{
					double Rz=0.0;
											 //pokud Rz nen� pr�zdn�, ulo��m si jeho hodnotu a pou�iju k p�ed�n�.
											 //jinak nastav�m Rz na -1 tzn., �e do roletky zobraz�m v�echny rozte�e z katalogu
					if(!rStringGridEd_tab_dopravniky->Cells[6][Row].IsEmpty())
					{
					Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][Row]);
					}  else Rz=-1;

					AnsiString	data;
				 if(Runit==MM) data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",true);
				 else          data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",false);
         // Memo3->Lines->Clear();
         // Memo4->Lines->Clear();
          Memo3->Lines->Add(data);
          Memo4->Lines->Add(Rz);
				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
				TStringList *S=new TStringList;
				S->Add(data);
				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
				S->Delimiter=';';     //nutno v jednoduch�ch uvozovk�ch, dvoj� hod� chybu p�i p�ekladu
				S->DelimitedText=data;
				S->DelimitedText=data;

				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Assign(S); //Standartn� se pou��v� Add(), ale v tomto p��pad� Assign()
				//zobrazOramovani=true;
				//F->m.frameRect(Rect(200,100,60,80),clBlue,2);

		}






void __fastcall TForm_parametry_linky::FormClick(TObject *Sender)
{
//zobrazen� v�ech z�mk� p�i kliku do formu
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
if(Col==5) { vypis("",false);  }
}
//---------------------------------------------------------------------------


double  TForm_parametry_linky::getRz(double i)
{
	double Rz=0;
	if(Dmunit==M) Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][i]);
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
    Col_validace=0;

				 if(ACol==4 || ACol==5 || ACol==6 || ACol==7)
				 {
         double R=0;
         if(Runit==M) R=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]); else R=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow])/(1+999.0*Runit);

            TTextNumber TNValue=F->d.v.rVALIDACE(ACol,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow])/(1+59.0*aRDunit),R,F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow])/(1+999.0*Runit),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),aRDunit,Runit,Dmunit);
            if(TNValue.text!="")
            {
               VID=ACol;
               VID_value=TNValue.number1;
               Row_validace=ARow;
               Col_validace=ACol;
               vypis(TNValue.text);
            }
//	 if(rStringGridEd_tab_dopravniky->Cells[8][ARow]!="nepou��v�n")
//						 {
//
//							VID=ACol;
//              TTextNumber TNValue=F->d.v.rVALIDACE(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]));
//            // double VID_value_M           =	F->ms.MyToDouble(F->d.v.validaceR(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),0));
//						  VID_value           =	Form1->m.round(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]));//F->ms.MyToDouble(F->d.v.validaceR(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),0));
//							AnsiString  retezec =	F->d.v.validaceR(VID,getPID(ARow),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][ARow]),F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]),1);
//							Row_validace=ARow;
//						// 	if(VID_value=!"")
//            vypis(retezec);
//						 }
//						 else
//						 {
//									double value=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]);
//									if(value!=floor(value))
//									{
//										 double dop_Rx=	Form1->m.round(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][ARow]));
//										 vypis("Hodnota mus� b�t celo��seln�, doporu�en� hodnota Rx: "+ AnsiString(dop_Rx));
//										 Row_validace=ARow;
//										 VID=7;
//										 VID_value=dop_Rx;
//								}
//
//						}
				 }

 switch(ACol)
	 {
		///////////////////////////////////////////////////////////////////////
			case 1:     //NAZEV
				{
				if(rStringGridEd_tab_dopravniky->Cells[1][ARow]=="")
						{
						vypis("N�zev pohonu nesm� b�t pr�zdn�!");
						VID=1;
						Row_validace=ARow;
            Col_validace=ACol;
						}

				}
				break;
				case 2:     //RD OD
				{

						if(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][ARow])<=0)
						{
						//vypis("Neplatn� hodnota rychlosti pohonu od!");
					 //	VID=2;
						Row_validace=ARow;
            Col_validace=ACol;
						}

					//od je v�t�� ne� do
						if(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][ARow]) > F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][ARow]))
							{
              Memo3->Lines->Add(rStringGridEd_tab_dopravniky->Cells[2][ARow]);
              Memo4->Lines->Add(rStringGridEd_tab_dopravniky->Cells[3][ARow]);
						 //	vypis("Neplatn� rozsah rychlosti pohonu od-do!");
						 //	VID=23;
							Row_validace=ARow;
              Col_validace=ACol;
							}

				}
				break;
				case 3:     //RD OD
				{
				if(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][ARow])<=0)
						{
					 //	vypis("Neplatn� hodnota rychlosti pohonu do!");
					 //	VID=3;
						Row_validace=ARow;
            Col_validace=ACol;
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

					// ShowMessage("Do�lo ke zm�ne TT - vol�n� GAPO");
					 Changes_TT=true;
					 rEditNum_takt->Value=Form_TT_kalkulator->rEditNum_takt->Value;
					 }
				}

		}

       // stav p�i nastaven� TT a nem�m p�itom pohon ani objekty
    	 if(Form1->d.v.OBJEKTY->dalsi==NULL && Form1->d.v.POHONY->dalsi==NULL)
	 {

				if(mrOk==Form_TT_kalkulator->ShowModal())
				{
				 //
					 if(Form_TT_kalkulator->rEditNum_takt->Value!=F->d.v.PP.TT)
					 {

					// ShowMessage("Do�lo ke zm�ne TT - vol�n� GAPO");
					 Changes_TT=false;
					 rEditNum_takt->Value=Form_TT_kalkulator->rEditNum_takt->Value;
           Form_parametry_linky->Button_save->Enabled=true;
           Form_parametry_linky->Button_storno->Enabled=true;
					 }
				}
		}

		if(Changes_TT)//pri zmene TT + jiz existuje nejaky objekt nebo pohon
		{
	 		F_gapoTT->ShowModal();
      if(F_gapoTT->myModalResult==mrOk)
      {
       Ulozit=true;
       Close();
      }
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
      if(F->d.v.PP.delka_jig!=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value) Changes_vozik=true;
	    if(F->d.v.PP.sirka_jig!=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value) Changes_vozik=true;
	    if(F->d.v.PP.vyska_jig!=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value) Changes_vozik=true;
	    if(F->d.v.PP.delka_podvozek!=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value)  Changes_vozik=true;

	    scGPNumericEdit_delka_jig->Value=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value;
	    scGPNumericEdit_sirka_jig->Value=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value;
	    scGPNumericEdit_vyska_jig->Value=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value;
	    scGPNumericEdit_delka_podvozek->Value=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value;

      Form_parametry_linky->Button_save->Enabled=true;
      Form_parametry_linky->Button_storno->Enabled=true;
	  }

		if(Changes_vozik && Form1->d.v.OBJEKTY->dalsi!=NULL && Form1->d.v.POHONY->dalsi!=NULL)//pri zmene voziku
		{
			F_gapoV->ShowModal();
      if(F_gapoV->myModalResult==mrOk)
      {
       Ulozit=true;
       Close();
      }
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
     if(VID==7) pm.input_Rx();
     if(VID==6) pm.input_Rz();
     if(VID==5) pm.input_R();
     if(VID==4) pm.input_aRD();
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
void __fastcall TForm_parametry_linky::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(Ulozit || Storno)CanClose=true;
	else CanClose=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::GlyphButton_refreshClick(TObject *Sender)

{
    nacti_pohony();
		if(!data_nalezena)
		{
			rStringGridEd_tab_dopravniky->RowCount=1;    //defaultn� po�et ��dk� - hlavi�ka
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scHTMLLabel_roztecClick(TObject *Sender)
{
		input_state = R_prevod; // z�m�r, aby se nep�epo��tavaly hodnoty
		if (Runit == MM) // pokud je v milimetrech, tak p�epne na metry
		{

			  scHTMLLabel_roztec->Caption="rozte� <u>[m]</u>";
				INPUT(0,0);   //tento input vol� z�rove� i output
				Runit = M;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
			scHTMLLabel_roztec->Caption="rozte� <u>[mm]</u>";
			INPUT(0,0);    //tento input vol� z�rove� i output
			Runit = MM;
		}

		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scHTMLLabel_rozestupClick(TObject *Sender)

{
	input_state = Rz_prevod; // z�m�r, aby se nep�epo��tavaly hodnoty
		if (Dmunit == MM) // pokud je v milimetrech, tak p�epne na metry
		{

			  scHTMLLabel_rozestup->Caption="rozestup <u>[m]</u>";
				INPUT(0,0);   //tento input vol� z�rove� i output
				Dmunit = M;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
			scHTMLLabel_rozestup->Caption="rozestup <u>[mm]</u>";
		 	INPUT(0,0);    //tento input vol� z�rove� i output
			Dmunit = MM;
		}

		input_state = NOTHING; // u� se mohou p�epo��t�vat

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rMemoEx1_rychlostClick(TObject *Sender)
{
		input_state = aRD_prevod; // z�m�r, aby se nep�epo��tavaly hodnoty
		if (aRDunit == S)
		{

			  rMemoEx1_rychlost->Caption="Rychlost <u>[m/min]</u></br> ";
				INPUT(0,0);   //tento input vol� z�rove� i output
				aRDunit = MIN;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
			rMemoEx1_rychlost->Caption="Rychlost <u>[m/s]</u></br> ";
			INPUT(0,0);    //tento input vol� z�rove� i output
			aRDunit = S;
		}

		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------


