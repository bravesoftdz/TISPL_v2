//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "parametry_linky.h"
#include "Unit1.h"
#include "kabina_schema.h"
#include "MyMessageBox.h"
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
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
	////designov� z�le�itosti
	Form_parametry_linky->Color=(TColor)RGB(240,240,240); //nastaven� barvy formul��e
	Form1->m.designButton(Button_save,Form_parametry_linky,1,2);
	Form1->m.designButton(Button_storno,Form_parametry_linky,2,2);
	zobrazitFrameForm=false;

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
	Delkaunit=MM;
	Sirkaunit=Delkaunit;
	Taktunit=S;
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
		input_state=NOTHING;//nutnost
		scExPanel_doporuc_pohony->Visible=false;
		PopUPmenu->Visible=false;
		//pro vytvo�en� z�lohy zru�en�ch p���azen� - vyfikundace z d�vodu mo�n�ho storna
		zrusena_prirazeni_PID_size=rStringGridEd_tab_dopravniky->RowCount;//velikost sta�� jako po�et ��dk�/pohonu po na�ten�, v�ce jich b�t p�i�azeno do nov�ho na�ten� formu b�t nem��e
		zrusena_prirazeni_PID=new bool[zrusena_prirazeni_PID_size];
		for(unsigned int PID=0;PID<=zrusena_prirazeni_PID_size;PID++)zrusena_prirazeni_PID[PID]=false;

		//provizorn� o�et�en�, p�ijde cel� smazat, a� nahod�me aktualizaci
		if(Form1->d.v.OBJEKTY->dalsi!=NULL)
		{
			rEditNum_takt->Enabled=false;
			rEditNum_delka_jigu->Enabled=false;
			rEditNum_sirka_jigu->Enabled=false;
			scGPNumericEdit_delka_podvozku->Enabled=false;
		}
		else
		{
			rEditNum_takt->Enabled=true;
			rEditNum_delka_jigu->Enabled=true;
			rEditNum_sirka_jigu->Enabled=true;
			scGPNumericEdit_delka_podvozku->Enabled=true;
		}


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
			rHTMLLabel_sirka->Visible=true;
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

			rHTMLLabel_sirka->Visible=false;
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
	 rEditNum_delka_jigu->Value=Form1->d.v.PP.delka_voziku;
	 rEditNum_sirka_jigu->Value=Form1->d.v.PP.sirka_voziku;
	 if(Form1->d.v.PP.typ_voziku==0) scGPSwitch->State=scswOff;
	 else  { scGPSwitch->State=scswOn; }
	 //scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;
	 rEditNum_takt->Value=Form1->d.v.PP.TT;

	 rStringGridEd_tab_dopravniky->SetColumnAutoFit(0);
	 rStringGridEd_hlavicka_tabulky->SetColumnAutoFit(0);

	 rStringGridEd_tab_dopravniky->Cells[0][0]="ID";
	 rStringGridEd_tab_dopravniky->Cells[1][0]="N�zev";
	 rStringGridEd_tab_dopravniky->Cells[2][0]="Rychlost od [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[3][0]="Rychlost do [m/min]";
	 rStringGridEd_tab_dopravniky->Cells[4][0]="Rozte� [mm]";
	 rStringGridEd_tab_dopravniky->Cells[5][0]="P�i�azen";
	 rStringGridEd_tab_dopravniky->Cells[6][0]="min. Rz [m]*";

	 rStringGridEd_hlavicka_tabulky->Cells[0][0]=rStringGridEd_tab_dopravniky->Cells[0][0];
	 rStringGridEd_hlavicka_tabulky->Cells[1][0]=rStringGridEd_tab_dopravniky->Cells[1][0];
	 rStringGridEd_hlavicka_tabulky->Cells[2][0]=rStringGridEd_tab_dopravniky->Cells[2][0];
	 rStringGridEd_hlavicka_tabulky->Cells[3][0]=rStringGridEd_tab_dopravniky->Cells[3][0];
	 rStringGridEd_hlavicka_tabulky->Cells[4][0]=rStringGridEd_tab_dopravniky->Cells[4][0];
	 rStringGridEd_hlavicka_tabulky->Cells[5][0]=rStringGridEd_tab_dopravniky->Cells[5][0];
	 rStringGridEd_hlavicka_tabulky->Cells[6][0]=rStringGridEd_tab_dopravniky->Cells[6][0];

	//	rStringGridEd_tab_dopravniky->Columns->

	 if(rStringGridEd_tab_dopravniky->RowCount>1 && rStringGridEd_tab_dopravniky->ColCount>=5){rStringGridEd_tab_dopravniky->Row=1;rStringGridEd_tab_dopravniky->Col=5;}//rStringGridEd_tab_dopravniky->FinishEditing();//ukon�� editaci a t�m odstran� nep��jemn� vizu�ln� efekt

	 //pozice info tla��tka - asi je tla��tko stejn� provizorn�
	 pozice_scGPGlyphButton_hint();

	 //testuje zda existuj� nepou��t� pohony a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
	 if(existuji_nepouzivane_pohony())scGPGlyphButton_DEL_nepouzite->Visible=true;
	 else scGPGlyphButton_DEL_nepouzite->Visible=false;
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
						if(OBJEKTY_POUZIVAJICI_POHON!="")rStringGridEd_tab_dopravniky->Cells[5][i]=OBJEKTY_POUZIVAJICI_POHON;
						else rStringGridEd_tab_dopravniky->Cells[5][i]="ne";

						//	 ShowMessage(ukaz->rychlost_do*60.0);
						rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
						rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;
						rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od*60.0;
						rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do*60.0;
						rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->roztec*1000.0;

						ukaz = ukaz->dalsi;
				 }

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
	Form_parametry_linky->Close();
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
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
		Ulozit=true;

		//pri zmene TT
		if(Form1->ms.MyToDouble(rEditNum_takt->Value) != Form1->d.v.PP.TT && Form1->d.v.OBJEKTY->dalsi!=NULL)
		{
		 Changes=true;
		 Changes_TT=true;
		}
		//pri zmene delky voziku
		if(Form1->ms.MyToDouble(rEditNum_delka_jigu->Value) != Form1->d.v.PP.delka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL)
		{
		 Changes=true;
		 Changes_PP=true;
		}
		//pri zmene sirky voziku
		if(Form1->ms.MyToDouble(rEditNum_sirka_jigu->Value) != Form1->d.v.PP.sirka_voziku && Form1->d.v.OBJEKTY->dalsi!=NULL)
		{
		 Changes=true;
		 Changes_PP=true;
		}
		//pri zmene delky podvozku
		if(Form1->ms.MyToDouble(scGPNumericEdit_delka_podvozku->Value) != Form1->d.v.PP.delka_podvozku && Form1->d.v.OBJEKTY->dalsi!=NULL)
		{
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
					(Form1->ms.MyToDouble(O->RD)*60.0<Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]) ||//je mimo rozsah (pod) nebo
					 Form1->ms.MyToDouble(O->RD)*60.0>Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]))//je mimo rozsah (nad)
				)
				{

					T+="Objekt: "+O->name+" Rychlost:"+O->RD*60.0+" vs. Pohon: "+rStringGridEd_tab_dopravniky->Cells[1][i];
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
			Form_PL_priority->rStringGridEd_tab->Cells[1][2]="Kapacita, D�lka kabiny, Rychlost pohonu";
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
			Form_PL_priority->rStringGridEd_tab->Cells[0][3]="D�lka kabiny";
		//	Form_PL_priority->rStringGridEd_tab->Cells[0][4]="Individu�ln� nastaven�";
				 //meni se
			Form_PL_priority->rStringGridEd_tab->Cells[1][1]="D�lka kabiny, Rychlost pohonu";
			Form_PL_priority->rStringGridEd_tab->Cells[1][2]="D�lka kabiny, Technologick� �as";
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
			zrusit_prirazeni_smazanych_pohunu_k_objektum();
			Form1->d.v.vymaz_seznam_POHONY();
			Form1->d.v.hlavicka_POHONY();

			for (unsigned int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
			{
				double rychlost_od;
				double rychlost_do;
				double roztec;
				UnicodeString nazev;

				if (rStringGridEd_tab_dopravniky->Cells[1][i].IsEmpty()) nazev="nov� pohon";
				else  nazev=rStringGridEd_tab_dopravniky->Cells[1][i];

				if (rStringGridEd_tab_dopravniky->Cells[2][i].IsEmpty()) rychlost_od=0;
				else  rychlost_od=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i])/60.0;

				if(rStringGridEd_tab_dopravniky->Cells[3][i].IsEmpty()) rychlost_do=0;
				else 	rychlost_do=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i])/60.0;

				if(rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty()) roztec=0;
				else roztec=Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i])/1000.0;

				//ulo�en� pohonu do spoj�ku
				Form1->d.v.vloz_pohon (nazev,rychlost_od,rychlost_do,roztec);

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
			F->d.v.PP.delka_voziku=F->ms.MyToDouble(rEditNum_delka_jigu->Value);
			F->d.v.PP.sirka_voziku=F->ms.MyToDouble(rEditNum_sirka_jigu->Value);
			F->d.v.PP.delka_podvozku=scGPNumericEdit_delka_podvozku->Value;
			int typ;
			if(scGPSwitch->State==scswOff){typ=0;}
			else {typ=1;}
			Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(typ);
			Form1->d.v.PP.TT=Form1->ms.MyToDouble(rEditNum_takt->Value);

			if(volat_aktualizaci)
			{
				Form1->d.v.aktualizace_objektu(aktualizace_id);
				//doplnil 1.2.2018 M, aktualizace i �asov�ch os a pop�. ROMA, nutn� z d�vodu zm�ny parametr� objektu
				//zv�it zda neimplementovat rovnou do aktualizace objektu, ale zat�m nejde z d�vodu, �e se vol� ve Form1 z d�vodu "fale�n�ho" p�ekresledn� �asov�ch os p�ed ROMA
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

	if (rStringGridEd_tab_dopravniky->RowCount > 2)
	{
		int i = rStringGridEd_tab_dopravniky->RowCount - 1;

		rStringGridEd_tab_dopravniky->Cells[0][i] = getMaxPID()+1;
		rStringGridEd_tab_dopravniky->Cells[1][i] = "nov� pohon";//rStringGridEd_tab_dopravniky->Cells[1][i - 1];
		rStringGridEd_tab_dopravniky->Cells[2][i] = "0";//rStringGridEd_tab_dopravniky->Cells[2][i - 1];
		rStringGridEd_tab_dopravniky->Cells[3][i] = "0";//rStringGridEd_tab_dopravniky->Cells[3][i - 1];
		rStringGridEd_tab_dopravniky->Cells[4][i] = "0";//rStringGridEd_tab_dopravniky->Cells[4][i - 1];
		rStringGridEd_tab_dopravniky->Cells[5][i] = "ne";
		rStringGridEd_tab_dopravniky->Cells[6][i] = "0";
	}
	rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;//p�esune focus na posledn� ��dek

	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
	pozice_scGPGlyphButton_hint();

	//existuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
	scGPGlyphButton_DEL_nepouzite->Visible=true;
}
//---------------------------------------------------------------------------
//sma�e posledn� ��dek - ji� se nepou��v�, ale nechv�m
void __fastcall TForm_parametry_linky::Button_DEL_Click(TObject *Sender)
{
	if(Form1->d.v.pohon_je_pouzivan(rStringGridEd_tab_dopravniky->RowCount-1))
	{
		AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(rStringGridEd_tab_dopravniky->RowCount-1),true);
				if(mrOk==Form1->MB("Pohon je pou��v�n pro objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_OKCANCEL)){

				//nefunguje spr�vn� pro p��pad storna, �e�� proto n�sleduj�c� ��dek, Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(rStringGridEd_tab_dopravniky->RowCount-1));
				zrusena_prirazeni_PID[getPID(rStringGridEd_tab_dopravniky->RowCount-1)]=true;
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

				 while (ukaz!=NULL) {

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
		scHTMLLabel_doporuc_pohony->Caption=Form1->d.v.navrhni_POHONY();
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
		rStringGridEd_tab_dopravniky->Cells[5][i]="ne";
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

//if (Col==4 && Row==3) {
//
//		EditStyle=sgbDropDown;   //vyber typu
//		rStringGridEd_tab_dopravniky->Columns->Items[4]->PickList->Clear();
//		rStringGridEd_tab_dopravniky->Columns->Items[4]->PickList->Add("Ano");
//		rStringGridEd_tab_dopravniky->Columns->Items[4]->PickList->Add("Ne");
//
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
		if (Col==6 && Row==0) {
			CanEdit=false;
			 }

		//Pokud je pohon pou��v�n, nastav�m polo�ky rychlost, rozte� atd jako readonly - nelze je m�nit.
	 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
	 {
		 if(rStringGridEd_tab_dopravniky->Cells[5][i]=="ano")
		 {
			 if(Row==i && Col>1) CanEdit=false;
		 }

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
		delka=scGPNumericEdit_delka_podvozku->Value*1000.0;
		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
		delka=scGPNumericEdit_delka_podvozku->Value/1000.0;
		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	scGPNumericEdit_delka_podvozku->Value=delka;
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
		delka=scGPNumericEdit_delka_podvozku->Value*1000.0;
		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[mm]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;
		//delka - p�epo��t�n�
			delka=scGPNumericEdit_delka_podvozku->Value/1000.0;
		rHTMLLabel_delka_podvozek->Caption="d�lka <font color=#2b579a>[m]</font>";
	}
	//pln�n�
	scGPNumericEdit_delka_podvozku->Value=delka;
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
//ControlPaint.DrawBorder(e.Graphics, this.panel1.ClientRectangle, Color.DarkBlue, ButtonBorderStyle.Solid);

 	//DrawBorder(Sender,Form_parametry_linky->ClientRect,clBlue,bsNone);
 //DrawBorder
	show_min_Rz();
	if(zobrazitFrameForm)Form1->m.frameForm(Form_parametry_linky,clWebOrange,1);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rStringGridEd_tab_dopravnikyKeyUp(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
	show_min_Rz();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TForm_parametry_linky::show_min_Rz() {

 double min_mezera_voziky;
 double rotace;
 scGPGlyphButton_hint_Rz->Enabled=true;
// if(Delkaunit=M) jednotky*1000.0; else jednotky*1.0;

 for(int i=1;i<=rStringGridEd_tab_dopravniky->RowCount;i++)
 {
	 if(!rStringGridEd_tab_dopravniky->Cells[4][i].IsEmpty())
	 {
	 min_mezera_voziky= Form1->m.mezera_mezi_voziky(rEditNum_delka_jigu->Value*1000,rEditNum_sirka_jigu->Value*1000,0,Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]),0);

	// ShowMessage(min_mezera_voziky);

		if(rEditNum_delka_jigu->Value<rEditNum_sirka_jigu->Value)
		{
		rotace=0;
		scGPGlyphButton_hint_Rz->Hint="Min. Rz = minim�ln� rozte�ov� vzd�lenost,po��tan� na z�klad� zadan� rozte�e a ���ky voz�ku.";
		}
		else
		{
		rotace=90;
		scGPGlyphButton_hint_Rz->Hint="Min. Rz = minim�ln� rozte�ov� vzd�lenost,po��tan� na z�klad� zadan� rozte�e a d�lky voz�ku.";
		}

	 rStringGridEd_tab_dopravniky->Cells[6][i]= Form1->m.Rz(rEditNum_delka_jigu->Value,rEditNum_sirka_jigu->Value,rotace,min_mezera_voziky/1000);

	 }
 }


}

void __fastcall TForm_parametry_linky::rEditNum_sirka_jiguChange(TObject *Sender)

{
show_min_Rz();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_delka_jiguChange(TObject *Sender)

{
show_min_Rz();
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
void TForm_parametry_linky::zrusit_prirazeni_smazanych_pohunu_k_objektum()
{   //p�eindexovat nesmazan� nebo jim dat nov� odkaz, nebo mazat jen konkr�tn�
	for(unsigned PID=0;PID<=zrusena_prirazeni_PID_size;PID++)
	{
		if(zrusena_prirazeni_PID[PID])
		Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(PID);
	}
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
}
//---------------------------------------------------------------------------
//pozice info tla��tka - asi je tla��tko stejn� provizorn�
void TForm_parametry_linky::pozice_scGPGlyphButton_hint()
{
	 if(rStringGridEd_tab_dopravniky->RowCount<=5)scGPGlyphButton_hint_Rz->Left=Width-scGPGlyphButton_hint_Rz->Width;
	 else scGPGlyphButton_hint_Rz->Left=1079;
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
	rStringGridEd_tab_dopravniky->Cells[5][ROW+1]="ne";
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
			if(mrOk==Form1->MB("Pohon je pou��v�n pro objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_OKCANCEL))
			{
				//Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(ROW));
				zrusena_prirazeni_PID[getPID(ROW)];//nahrazeno novou filozofii, z d�vodu mo�n�ho storna formu
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
//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�eho
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteClick(TObject *Sender)
{
	for(unsigned int j=1;j<rStringGridEd_tab_dopravniky->RowCount;j++)//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�eho
	{
		if(Form1->d.v.pohon_je_pouzivan(getPID(j))==false)//pohon nen� pou��van�
		{
			//samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
			rStringGridEd_tab_dopravniky->DeleteRowEx(j);
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
//chov� se divn�, chce to doladit
//	Highlight=false;
// if (Row==rStringGridEd_tab_dopravniky->Row)
// {
//		Background=(TColor)RGB(202,217,240);
//	Highlight=true;
// }
}
//---------------------------------------------------------------------------


