//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "parametry.h"
#include "unit1.h"
#include "parametry_linky.h"
#include "poznamky.h"
#include "kabina_schema.h"
#include "MyMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scHint"
#pragma link "rHintWindow"
#pragma resource "*.dfm"
TForm_parametry *Form_parametry;

//---------------------------------------------------------------------------
__fastcall TForm_parametry::TForm_parametry(TComponent* Owner) : TForm(Owner)
{
		// nov� nastaven� barvy formul��e
		Form_parametry->Color = (TColor)RGB(240, 240, 240);
		scGPButton_header_projekt->Options->NormalColor = (TColor)RGB(240, 240, 240);

		// nastaven� zv�razn�n�
		hl_color = clRed;// (TColor)RGB(255,141,28);//barva zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky
		hlFrameWidth = 2;// ���ka zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky

		// p�evzet� defaultn� velikosti formul��e z designu pro dal�� u�it� (slou�� sp�e pro usnadn�n� v�voje, ne� funk�n�)
		defaultForm_parametryHeight = Form_parametry->Height;

		// matamaticky exaktn� napozicov�n� tla��tek OK a storno
		Form1->m.designButton(scGPButton_OK, Form_parametry, 1, 2);
		Form1->m.designButton(scGPButton_storno, Form_parametry, 2, 2);

		// v�choz� nastaven� z�mk�
		CT_zamek = UNLOCKED;
		DD_zamek = UNLOCKED;
		RD_zamek = LOCKED;
		// povolen�/ zak�z�n� nastaven� combo rotace, podle nastaven� z�mku RD a pou�it� pohonu
	 //	Povol_comboRotace();
	 input_state_Rz=false; // separ�tn� nastaven� pro Rz
}

//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
		input_state = NOTHING; // nutnost!!! - test p�esunuto nahoru
		// formul�� bude p�i prvn�m zobrazen� v sekund�ch a metrech nebo dle INI v odvozen�ch jednotk�ch, jinak dle SI
		if(form_zobrazen==false)
		{
			minsec = S;
			m_mm = M;
			CTunit = S;
			RDunitT = S;
			//RDunitD = M;
			DDunit = M;
			DMunit = M;

			// GLOBAL
			if (Form1->readINI("nastaveni_form_parametry", "cas") == "1")
			{
					minsec = MIN;
					scGPButton_min_sec->Caption = "v�e na s";
			} // tedy MIN
			if (Form1->readINI("nastaveni_form_parametry", "vzdalenost") == "1")
			{
					m_mm = MM;
					scGPButton_metry_milimetry->Caption = "v�e na m";
			} // tedy MM
			// CT
			if (Form1->readINI("nastaveni_form_parametry", "CT") == "1")rHTMLLabel_CTClick(this); // p�evede na min tzn. CTunit=MIN;
			// RD
			if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")rHTMLLabel_RDClick(this); // p�evede na min tzn. RDunitT=MIN;
			//if (Form1->readINI("nastaveni_form_parametry", "RDd") == "1")rHTMLLabel_RDClick(this); // p�evede na mm tzn. RDunitD=MM;
			// DD
			if (Form1->readINI("nastaveni_form_parametry", "DD") == "1")rHTMLLabel_delka_dopravnikuClick(this); // p�evede na mm tzn. DDunit=MM;
			// DM
			if (Form1->readINI("nastaveni_form_parametry", "DM") == "1")rHTMLLabel_mezera_jednotkyClick(this); // p�evede na mm tzn. DMunit=MM;
			// nastaven� defaultn�ch hodnot
			if (scGPNumericEdit_CT->Value == 0)
					// if(d.v.ZAKAZKY->dalsi!=NULL)//pokud existuje prvn� zak�zka
			{
					scGPNumericEdit_CT->Value = Form1->d.v.PP.TT / (1 + 59 * CTunit);
					// ji� nepou��v�me, proto�e se u prvn� zak�zky uva�uje glob�ln� TT,d.v.ZAKAZKY->dalsi->TT;
					scGPNumericEdit_kapacita->Value = 1;
			}
		}
		//M: byl zde ne� jsem ho p�esunul zcela nahoru: input_state = NOTHING; // nutnost!!!

		//	input_clicked_edit=empty_klik; M: toto bylo i p�edt�m zakomentovan�, pros�m, pokud nen� zcela t�eba mazat, a� nen� zavad�j�c�
		kapacitaSG = 1; // nen� podn�t k rozkl�d�n� na v�ce objekt�
		scGPEdit_name->SetFocus();
		// nastav� v�choz� focus, kde se p�edpokl�d� v�choz� nastavov�n�
		scGPEdit_name->SelectAll(); // ozna�� cele pro editace
		scGPButton_OK->Enabled = true;
		scGPButton_OK->Caption = "Ulo�it";
		//form_zobrazen = true;
		pohon_pouzivan = false;

		if(Form_parametry_linky->scGPSwitch->State==0)rHTMLLabel_jig_podvozek->Caption="jig a podvozek";
		else rHTMLLabel_jig_podvozek->Caption="jig a z�v�s";
		// detekuje zda je form aktu�ln� zobrazen, slou�� proto aby p�i zm�n� combo re�im pokud si nastavil u�ivatel formula� jinam, aby z�stal nastaven dle u�ivatele
		// pohon_je_pouzivan  - nastaven� z�mk� a editbox� dle nastaven�ho pohonu.
		Pohon_pouzivan();
		Nacti_rx(); // vypo��t�n� Rx a zobrazeni
		INPUT();   // p�i prvn�m zobrazen� formu "otisknu" data z formu do math struktury, bez ��dn�ch v�po�t�, prim�rn� pou�ito pro nastaven� decimal checkboxu, kdy pot�ebuje m�t data v output ji� p�i formshow
		OUTPUT();  // na�ten� dat ze struktury
		form_zobrazen = true;//pozor byl v��e, ale asi by nem�lo d�lat probl�my, vyu��v� se tak� mimojin� k tomu, aby se nevolalo opakovan� output (n�kde patrn� nen� o�et�en input_statem a m�lo za n�sledeko opakovan� p�evody jednotek)
		if(scComboBox_rezim->ItemIndex==1) 	Check_rozmezi_RD();
		if(scComboBox_rezim->ItemIndex!=1)  // pro jin� re�imy v�dy povol�m zobrazen� z�mk�
		{
			scButton_zamek_CT->Enabled=true;
			scButton_zamek_DD->Enabled=true;
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry::vypis(UnicodeString text,bool red,bool link)
{
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption = "Ulo�it";
		if (text != "") // zobraz� a vyp�e
		{
				rHTMLHint1->ToString()=text;//nate�en� do hintu zaji��uje zobrazen� cel�ho textu, nepou��v� se klasick� hint
        rHTMLLabel_InfoText->Hint=text;//doplnil M, v��e uveden� mi nefungovalo
				//prodllu�en� formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl p�edt�m popisek skryt� + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnut� podtr�en�ho p�sma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
					 	scGPButton_OK->Enabled=false;
						rHTMLLabel_InfoText->Font->Color = clRed;
				}
				else
				{
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
				rHTMLLabel_InfoText->Left = 8;
				rHTMLLabel_InfoText->Top = Form_parametry->Height - 74;
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
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
		if (input_state != NO) // pokud to nen� p�i startu (formshow)
		{
				// v�choz� nastaven� z�mk� p�i p�ekliku re�imu na KK
				if (scComboBox_rezim->ItemIndex == 1)
				{
						CT_zamek = UNLOCKED;
						scButton_zamek_CT->ImageIndex = 38;
						RD_zamek = LOCKED;
						scButton_zamek_RD->ImageIndex = 37;
						DD_zamek = UNLOCKED;
						scButton_zamek_DD->ImageIndex = 38;

						K_zamek=CT_zamek;
						scButton_K_zamek->Visible = false;
						scButton_K_zamek->ImageIndex = 	scButton_zamek_CT->ImageIndex;
				}
				// v�choz� nastaven� z�mk� p�i p�ekliku re�imu na PP
				if (scComboBox_rezim->ItemIndex == 2)
				{
						K_zamek = LOCKED;
						DD_zamek = UNLOCKED;
						scButton_K_zamek->Visible = false;
						scButton_zamek_DD->ImageIndex = 38;
						scButton_K_zamek->ImageIndex = 37;

				}
				// nadesignov�n� a napozicov�n� komponent dle zvolen�ho re�imu
				setForm4Rezim(scComboBox_rezim->ItemIndex);// resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy

				//nastav� edity, podle toho, zdali je pohon pou��v�n �i nikoliv - volat a� po setForm4Re�im
				Pohon_pouzivan();

				// povolen�/ zak�z�n� nastaven� combo rotace, podle nastaven� z�mku RD a pou�it� pohonu
				// Povol_comboRotace();


				if(scComboBox_rezim->ItemIndex == 0) scGPNumericEdit_pozice->Value=1;

				INPUT();  // ulo�en� dat z edit� do struktury p�i zm�n� re�imu
				OUTPUT(); // na�ten� ze struktury - aby prob�hla validace dat p�i zm�n� re�imu

				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
				if (scComboBox_rezim->ItemIndex == 1 && obj!=NULL )  // u KK re�imu pokud je pohon pou��v�n - nat�hnut� spr�vn� mezery z dat
				{
					if(DMunit==M)
					{
						scGPNumericEdit_mezera->Value=obj->mezera;
						scGPNumericEdit_mezera_JIG->Value=obj->mezera_jig;
						scGPNumericEdit_mezera_PODVOZEK->Value=obj->mezera_podvozek;
						//scGPNumericEdit1_rx->Value=obj->pohon->Rx; patrn� se dopo��t�v� formou nacti_rx
					}
					else
					{
						scGPNumericEdit_mezera->Value=obj->mezera*1000;
						scGPNumericEdit_mezera_JIG->Value=obj->mezera_jig*1000;
						scGPNumericEdit_mezera_PODVOZEK->Value=obj->mezera_podvozek*1000;
						//scGPNumericEdit1_rx->Value=obj->pohon->Rx*1000; patrn� se dopo��t�v� formou nacti_rx
					}
				}

				position();// napozicov�n� cel�ho formul��e resp. o�et�en� aby z�stal dialog na monitoru, pouze pro prvotn� zobrazen� dle sou�adnic kurzoru my�i, jinak dle u�ivatele
		}
		if (scComboBox_rezim->ItemIndex == 1) // mezera se doporucuje pouze u KK rezimu
		{
			Kontrola_mezery();
		}
		Invalidate();//kv�li v�em pack�m + zaji�tuje z�rove� vol�n� frameCorrelation()//stejnou barvou or�muje hodnoty v korelaci + vykresl� korela�n� packy
		FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
//hl�d� a �e�� pozici formul��e, napozicov�n� cel�ho formul��e resp. o�et�en� aby z�stal dialog na monitoru, pouze pro prvotn� zobrazen� dle sou�adnic kurzoru my�i, jinak dle u�ivatele
void TForm_parametry::position()
{
		long X = Form1->akt_souradnice_kurzoru_PX.x + 10;
		long Y = Form1->akt_souradnice_kurzoru_PX.y + 10;
		if (form_zobrazen)
		{
				X = Form_parametry->Left;
				Y = Form_parametry->Top;
		}

		if(X + Form_parametry->ClientWidth < Form1->ClientWidth) Form_parametry->Left = X;
		else Form_parametry->Left = Form1->ClientWidth - Form_parametry->ClientWidth - 10;
		if(Y + Form_parametry->ClientHeight < Form1->ClientHeight) Form_parametry->Top = Y;
		else Form_parametry->Top = Form1->ClientHeight - Form_parametry->ClientHeight - Form1->scGPPanel_statusbar->Height - 10;
}
//---------------------------------------------------------------------------
//resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
		// v�choz� zmen�en� formul��e
		offset = 0;
		if (Form1->STATUS == Form1->NAVRH)rezim += 10; // posunut� o 10 vytv��� re�im+navrh��
		//pozice hlavi�kov�ch komponent (+pou�ito n�sledn� v kodu)
		scGPGlyphButton_view->Visible=false;
    scGPGlyphButton_odchylka->Visible=false;
	 	scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width- scGPGlyphButton_view->Width;
	 	scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width - scGPGlyphButton_view->Width;
		switch (rezim)
		{
				case 0: // STOP & GO
				{
						set(POHON, READONLY);
						set(TIME, HIDE);
						set(RYCHLOST, HIDE);
						set(DELKA, READONLY);
						set(KAPACITA, READONLY);
						set(POZICE, READONLY);
						set(ODCHYLKA, READONLY);
						set(CEKANI, READONLY);
						set(STOPKA, READONLY);
						set(ROTACE, HIDE);
						set(MEZERA, HIDE);
						set(MEZERA_JIG, HIDE);
						set(MEZERA_PODVOZEK, HIDE);
						set(ROZESTUP, HIDE);
						scGPNumericEdit_kapacita->Value = 1;
				} break;
				case 10: // STOP & GO - N�VRH��
				{
						// nastaven� komponent
						set(POHON, ENABLED);
						set(TIME, ENABLED);
						set(RYCHLOST, HIDE);
						set(DELKA, ENABLED);
						set(KAPACITA, READONLY);
						set(POZICE, READONLY);
						set(ODCHYLKA, HIDE);
						set(CEKANI, HIDE);
						set(STOPKA, HIDE);
						set(ROTACE, HIDE);
						set(MEZERA, HIDE);
						set(MEZERA_JIG, HIDE);
						set(MEZERA_PODVOZEK, HIDE);
						set(ROZESTUP, HIDE);
						scGPNumericEdit_kapacita->Value = 1;
					 	offset+=40;//workaround kv�li skryt� meze�e
				} break;
				case 1: // KONTINU�LN�
				{
						set(POHON, READONLY);
						set(TIME, HIDE);
						set(RYCHLOST, HIDE);
						set(DELKA, READONLY);
						set(KAPACITA, READONLY);
						set(POZICE, READONLY);
						set(ODCHYLKA, HIDE);
						set(CEKANI, READONLY);
						set(STOPKA, READONLY);
						set(ROTACE, ENABLED);
						set(MEZERA, ENABLED);
						set(MEZERA_JIG, ENABLED);
						set(MEZERA_PODVOZEK, ENABLED);
						set(ROZESTUP, READONLY);
						//hlavi�kov� komponenty
						scGPGlyphButton_view->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_view->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_view->Width;
				} break;
				case 11: // KONTINU�LN� - N�VRH��
				{
						set(POHON, ENABLED);
						set(TIME, ENABLED);
						set(RYCHLOST, ENABLED);
						set(DELKA, ENABLED);
						set(KAPACITA, ENABLED);
						set(POZICE, ENABLED);
						set(ODCHYLKA, HIDE);
						set(CEKANI, HIDE);
						set(STOPKA, HIDE);
						set(ROTACE, ENABLED);
						set(MEZERA, ENABLED);
						set(MEZERA_JIG, ENABLED);
						set(MEZERA_PODVOZEK, ENABLED);
						set(ROZESTUP, READONLY);
						//hlavi�kov� komponenty
						scGPGlyphButton_view->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_refresh->Width - scGPGlyphButton_view->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_refresh->Width - scGPGlyphButton_view->Width;
				} break;
				case 2: // POSTPROCESN�
				{
						set(POHON, ENABLED);
						set(TIME, HIDE);
						set(RYCHLOST, HIDE);
						set(DELKA, ENABLED);
						set(KAPACITA, ENABLED);
						set(POZICE, ENABLED);
						set(ODCHYLKA, ENABLED);
						set(CEKANI, ENABLED);
						set(STOPKA, ENABLED);
						set(ROTACE, ENABLED);
						set(MEZERA, ENABLED);
						set(MEZERA_JIG, ENABLED);
						set(MEZERA_PODVOZEK, ENABLED);
						set(ROZESTUP, HIDE);
						//hlavi�kov� komponenty
						scGPGlyphButton_view->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_view->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_view->Width;
				} break;
				case 12: // POSTPROCESN� - N�VRH��
				{
						set(POHON, ENABLED);
						set(TIME, ENABLED);
						set(RYCHLOST, HIDE);
						set(DELKA, ENABLED);
						set(KAPACITA, ENABLED);
						set(POZICE, ENABLED);
						set(ODCHYLKA, ENABLED);
						set(CEKANI, HIDE);
						set(STOPKA, HIDE);
						set(ROTACE, ENABLED);
						set(MEZERA, ENABLED);
						set(MEZERA_JIG, ENABLED);
						set(MEZERA_PODVOZEK, ENABLED);
						set(ROZESTUP, HIDE);
						//hlavi�kov� komponenty
						scGPGlyphButton_view->Visible=true;
            scGPGlyphButton_odchylka->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_refresh->Width - scGPGlyphButton_view->Width;;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_refresh->Width - scGPGlyphButton_view->Width;;
				} break;
				case 13: // SG - N�VRH��  - ROZ���EN�
				{
						// set(POHON,READONLY);
						// set(TIME,HIDE);
						// set(RYCHLOST,HIDE);
						// set(DELKA,READONLY);
						// set(KAPACITA,READONLY);
						// set(ODCHYLKA,READONLY);
						// set(CEKANI,READONLY);
						// set(STOPKA,READONLY);
						// set(ROTACE,HIDE);
						// set(MEZERA,HIDE);
						// set(ROZESTUP,HIDE);
						// scGPNumericEdit_kapacita->Value=1;
				} break;
		}

		// VELIKOST FORMUL��E                                  //offset m�m k dispozici dle z p�edchoz�ch vol�n� set
		Form_parametry->Height = defaultForm_parametryHeight + offset;
		// vertik�ln� (pop�. horizont�ln�) POZICE TLA��TEK OK,STORNO,COPY,PASTE
		scGPButton_OK->Top = Form_parametry->Height - scGPButton_OK->Height - 10;
		scGPButton_storno->Top = Form_parametry->Height -	scGPButton_storno->Height - 10;
		scGPGlyphButton_copy->Top = Form_parametry->Height  - scGPGlyphButton_copy->Height;
		scGPGlyphButton_copy->Left=0;
		scGPGlyphButton_paste->Top = Form_parametry->Height -	scGPGlyphButton_paste->Height;
		scGPGlyphButton_paste->Left=scGPGlyphButton_copy->Width+scGPGlyphButton_copy->Left;
		scGPCheckBox_zaokrouhlit->Top = Form_parametry->Height -scGPCheckBox_zaokrouhlit->Height;

		//grafick� upozorn�n� (upozorn�n�) na nep�i�azen� pohon
		if(scComboBox_pohon->ItemIndex==0)
		{scComboBox_pohon->Options->FrameWidth=2;scComboBox_pohon->Options->FrameFocusedColor=clRed;scComboBox_pohon->Options->FrameNormalColor=clRed;scComboBox_pohon->Options->FrameDisabledColor=clRed;scComboBox_pohon->Options->FrameDisabledColorAlpha=128;}
		else {scComboBox_pohon->Options->FrameNormalColor=clGray;scComboBox_pohon->Options->FrameFocusedColor=clHighlight;scComboBox_pohon->Options->FrameWidth=1;scComboBox_pohon->Options->FrameDisabledColor=clBtnShadow;scComboBox_pohon->Options->FrameDisabledColorAlpha=255;}
    }
//---------------------------------------------------------------------------
//zaji��uje zobrazen� a napozicov�n� pat�i�n� konkr�tn� komponenty a z�rove� udr�ov�n� hodnoty offsetu - to pokud je move==true, jinak jen nastav� komponenty
void TForm_parametry::set(Tcomponents C, Tcomponents_state S, bool move)
{
	// defaultn� hodnoty
	short O = 40;//vertik�ln� velikost odsazen� komponent
	int L = rHTMLLabel_rezim->Top;//v�choz� komponenta pro usazen� horn� pozice lev� komponenty
	int P = scComboBox_rezim->Top;//v�choz� komponenta pro usazen� horn� pozice prave komponenty
	int Le=234;//lev� um�st�n� krat��ch editboxu a comboboxu (od tech.�asu n�e)
	int W=156;//���ka krat��ch editboxu a comboboxu (od tech.�asu n�e)

	switch (C)
	{
		case POHON:
		{
			 ////o�et�en� pokud nen� nadefinov�n ��dn� pohon
			 if (!existuje_pohon)S = DISABLED;
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_pohon->Top = L + O;
					scComboBox_pohon->Top = P + O;
					scGPButton_header_projekt->Top=scComboBox_pohon->Top;
			 }
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_CT->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_pohon->Visible = true;
			 scComboBox_pohon->Visible = true;
			 scComboBox_pohon->Enabled = true;
			 rHTMLLabel_pohon->Left = 8;
			 scComboBox_pohon->Options->FrameNormalColor = clGray;
			 scComboBox_pohon->Options->FrameWidth = 1;
			 scComboBox_pohon->Left = 56;
       scGPGlyphButton_odchylka->Top=331;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scComboBox_pohon->Options->FrameNormalColor = hl_color;
					scComboBox_pohon->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scComboBox_pohon->Enabled = false;
			 		break;
				case READONLY:
					scComboBox_pohon->Enabled = false;scComboBox_pohon->Options->ShapeStyle=scgpessNone;break;
				case HIDE:
			 		rHTMLLabel_pohon->Visible = false;
					scComboBox_pohon->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case TIME: // technologick� �as v re�imu n�vrh��
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_CT->Top = L + 2 * O + offset;
			 		scGPNumericEdit_CT->Top = P + 2 * O + offset;
					scButton_zamek_CT->Top = scGPNumericEdit_CT->Top;
					scGPGlyphButton_PO_text_memo->Top = scGPNumericEdit_CT->Top + 1;
					// glyph pro memo - ziskany ct
			 }
			 scGPNumericEdit_CT->Left=Le;
			 scGPNumericEdit_CT->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_CT->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_CT->Visible = true;
			 scGPNumericEdit_CT->Enabled = true;
			 scGPNumericEdit_CT->Visible = true;
			 scGPNumericEdit_CT->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_CT->Options->FrameWidth = 1;
			 if (scComboBox_rezim->ItemIndex == 1)scButton_zamek_CT->Visible = true;
			 else	scButton_zamek_CT->Visible = false;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_CT->Options->FrameNormalColor = hl_color;
			 		scGPNumericEdit_CT->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED: // scButton_zamek_CT->ImageIndex=38;CT_zamek=UNLOCKED;
					break;
				case DISABLED:
					scGPNumericEdit_CT->Enabled = false;
			 		/* scButton_zamek_CT->ImageIndex=37;CT_zamek=LOCKED; */ break;
				case READONLY:
					scGPNumericEdit_CT->Options->ShapeStyle = scgpessNone;
					scButton_zamek_CT->Visible = false;
			 		scGPNumericEdit_CT->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_CT->Visible = false;
			 		scGPNumericEdit_CT->Visible = false;
					scButton_zamek_CT->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case RYCHLOST: // RD v re�imu n�vrh��
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_RD->Top = L + 3 * O + offset;
					scGPNumericEdit_RD->Top = P + 3 * O + offset;
					scButton_zamek_RD->Top = scGPNumericEdit_RD->Top;
			 }
			 scGPNumericEdit_RD->Left=Le;
			 scGPNumericEdit_RD->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_RD->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_RD->Visible = true;
			 scGPNumericEdit_RD->Visible = true;
			 scGPNumericEdit_RD->Enabled = true;
			 scGPNumericEdit_RD->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_RD->Options->FrameWidth = 1;
			 if (scComboBox_rezim->ItemIndex == 1)
					scButton_zamek_RD->Visible = true;
			 else
			 		scButton_zamek_RD->Visible = false;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_RD->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_RD->Options->FrameWidth = hlFrameWidth;
				case ENABLED: // scButton_zamek_RD->ImageIndex=38;RD_zamek=UNLOCKED;
					break;
				case DISABLED:
					scGPNumericEdit_RD->Enabled=false;//scButton_zamek_RD->ImageIndex=37;RD_zamek=UNLOCKED;
					break;
				case READONLY:
					scGPNumericEdit_RD->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_RD->Enabled = false;
					scButton_zamek_RD->Visible = true;
			 		break;
				case HIDE:
			 		rHTMLLabel_RD->Visible = false;
					scGPNumericEdit_RD->Visible = false;
			 		scButton_zamek_RD->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case DELKA: // d�lka dopravn�ku
		{
			 ////pozice
			 if (move)
			 {
			 		rHTMLLabel_delka_dopravniku->Top = L + 4 * O + offset;
					scGPNumericEdit_delka_dopravniku->Top = P + 4 * O + offset;
					scButton_zamek_DD->Top = scGPNumericEdit_delka_dopravniku->Top;
					if (scComboBox_rezim->ItemIndex == 0)
					{
						scGPGlyphButton_PO_text_memo2->Visible=true;
						scGPGlyphButton_PO_text_memo2->Top = scGPNumericEdit_delka_dopravniku->Top + 1;//pouze u S&G
					}
					else scGPGlyphButton_PO_text_memo2->Visible=false;
			 }
			 scGPNumericEdit_delka_dopravniku->Left=Le;
			 scGPNumericEdit_delka_dopravniku->Width=W;
			 // ty co jsou stejn�
			 scGPNumericEdit_delka_dopravniku->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_delka_dopravniku->Visible = true;
			 scGPNumericEdit_delka_dopravniku->Visible = true;
			 scGPNumericEdit_delka_dopravniku->Enabled = true;
			 scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_delka_dopravniku->Options->FrameWidth = 1;
			 if (scComboBox_rezim->ItemIndex == 0 || scComboBox_rezim->ItemIndex == 2)scButton_zamek_DD->Visible = false;
			 else	scButton_zamek_DD->Visible = true;
			 // ty co jsou rozd�ln�
			 ////funk�n� vlastnosti
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor =	hl_color;
					scGPNumericEdit_delka_dopravniku->Options->FrameWidth = hlFrameWidth;
			 		break;
				case ENABLED:
			 		// scButton_zamek_DD->ImageIndex = 38;
					// DD_zamek = UNLOCKED;
			 		break;
			 case DISABLED:
					scGPNumericEdit_delka_dopravniku->Enabled = false;
					// scButton_zamek_DD->ImageIndex = 37;
			 		// DD_zamek = LOCKED;
					break;
			 case READONLY:
					scGPNumericEdit_delka_dopravniku->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_delka_dopravniku->Enabled = false;
			 		scButton_zamek_DD->Visible = false;
					break;
			 case HIDE:
					rHTMLLabel_delka_dopravniku->Visible = false;
					scGPNumericEdit_delka_dopravniku->Visible = false;
					scButton_zamek_DD->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case KAPACITA: // po�adov�na kapacita objektu
		{
			 ////pozice
			 rHTMLLabel_kapacita->Top = L + 5 * O + offset;
			 scGPNumericEdit_kapacita->Top = P + 5 * O + offset;
			 scButton_K_zamek->Top = scGPNumericEdit_kapacita->Top;
			 scGPNumericEdit_kapacita->Left=Le;
			 scGPNumericEdit_kapacita->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_kapacita->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_kapacita->Visible = true;
			 scGPNumericEdit_kapacita->Visible = true;
			 scGPNumericEdit_kapacita->Enabled = true;
			 scButton_K_zamek->Visible = false;
			 scGPNumericEdit_kapacita->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_kapacita->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_kapacita->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_kapacita->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_kapacita->Enabled = false;
					break;
				case READONLY:
					scGPNumericEdit_kapacita->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_kapacita->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_kapacita->Visible = false;
					scGPNumericEdit_kapacita->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case POZICE: // zji�t�n� kapacita objektu
		{
				////pozice
			 rHTMLLabel_pozice->Top = L + 6 * O + offset;
			 scGPNumericEdit_pozice->Top = P + 6 * O + offset;
			 scGPNumericEdit_pozice->Left=Le;
			 scGPNumericEdit_pozice->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_pozice->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_pozice->Visible = true;
			 scGPNumericEdit_pozice->Visible = true;
			 scGPNumericEdit_pozice->Enabled = true;
			 scGPNumericEdit_pozice->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_pozice->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_pozice->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_pozice->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_pozice->Enabled = false;
					break;
				case READONLY:
					scGPNumericEdit_pozice->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_pozice->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_pozice->Visible = false;
					scGPNumericEdit_pozice->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case ODCHYLKA: // povolen� odchylka z CT
		{
			 ////pozice
			 rHTMLLabel_odchylka->Top = L + 7 * O + offset;
			 scGPNumericEdit_odchylka->Top = P + 7 * O + offset;
			 scGPNumericEdit_odchylka->Left=Le;
			 scGPNumericEdit_odchylka->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_odchylka->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_odchylka->Visible = true;
			 scGPNumericEdit_odchylka->Visible = true;
			 scGPNumericEdit_odchylka->Enabled = true;
			 scGPNumericEdit_odchylka->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_odchylka->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_odchylka->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_odchylka->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_odchylka->Enabled = false;
					break;
				case READONLY:
					scGPNumericEdit_odchylka->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_odchylka->Visible = false;
			 		break;
				case HIDE:
			 		rHTMLLabel_odchylka->Visible = false;
					scGPNumericEdit_odchylka->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case CEKANI: // �ek�n�
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_cekani->Top = L + 8 * O + offset;
			 		scComboBox_cekani_palec->Top = P + 8 * O + offset;
			 }
			 scComboBox_cekani_palec->Left=Le;
			 scComboBox_cekani_palec->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scComboBox_cekani_palec->Options->FrameNormalColor = clGray;
			 scComboBox_cekani_palec->Options->FrameWidth = 1;
			 rHTMLLabel_cekani->Visible = true;
			 scComboBox_cekani_palec->Visible = true;
			 scComboBox_cekani_palec->Enabled = true;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scComboBox_cekani_palec->Options->FrameNormalColor = hl_color;
					scComboBox_cekani_palec->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scComboBox_cekani_palec->Enabled = false;
			 		break;
				case READONLY:
			 		scComboBox_cekani_palec->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_cekani->Visible = false;
					scComboBox_cekani_palec->Visible = false;
					if (move)offset -= O;
			 		break;
			 }
		} break;
		case STOPKA: // stop stanice na konci objektu
		{
			 ////pozice
			 if (move)
			 {
			 		rHTMLLabel_stopka->Top = L + 9 * O + offset;
					scComboBox_stopka->Top = P + 9 * O + offset;
			 }
			 scComboBox_stopka->Left=Le;
			 scComboBox_stopka->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 rHTMLLabel_stopka->Visible = true;
			 scComboBox_stopka->Visible = true;
			 scComboBox_stopka->Enabled = true;
			 scComboBox_stopka->Options->FrameNormalColor = clGray;
			 scComboBox_stopka->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scComboBox_stopka->Options->FrameNormalColor = hl_color;
					scComboBox_stopka->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scComboBox_stopka->Enabled = false;
					break;
				case READONLY:
					scComboBox_stopka->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_stopka->Visible = false;
					scComboBox_stopka->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case ROTACE: // rotace jigu v objektu, zat�m jen p�ep�n�tko 0-90
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_rotace->Top = L + 10 * O + offset;
			 		scComboBox_rotace->Top = P + 10 * O + offset;
			 }
			 scComboBox_rotace->Left=Le;
			 scComboBox_rotace->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 rHTMLLabel_rotace->Visible = true;
			 scComboBox_rotace->Visible = true;
			 scComboBox_rotace->Enabled = true;
			 scComboBox_rotace->Options->FrameNormalColor = clGray;
			 scComboBox_rotace->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scComboBox_rotace->Options->FrameNormalColor = hl_color;
			 		scComboBox_rotace->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scComboBox_rotace->Enabled = false;
					break;
				case READONLY:
			 		scComboBox_rotace->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_rotace->Visible = false;
			 		scComboBox_rotace->Visible = false;
					if (move)offset -= O;
			 		break;
			 }
		} break;
		case MEZERA: // po�adov�na vs. zji�t�n� kapacita objektu
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_mezera->Top = L + 11 * O + offset;
					rHTMLLabel_mezera_jednotky->Top=rHTMLLabel_mezera->Top;
					rHTMLLabel_kriticka->Top = L + 11 * O + offset;
					scGPNumericEdit_mezera->Top = P + 11 * O + offset;
			 }
			 scGPNumericEdit_mezera->Left=Le;
			 scGPNumericEdit_mezera->Width=W;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_mezera->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_mezera->Visible = true;
			 scGPNumericEdit_mezera->Visible = true;
			 scGPNumericEdit_mezera->Enabled = true;
			 rHTMLLabel_mezera_jednotky->Visible = true;
			 scGPNumericEdit_mezera->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_mezera->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_mezera->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_mezera->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_mezera->Enabled = false;
					break;
				case READONLY:
			 		scGPNumericEdit_mezera->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_mezera->Enabled = false;
			 		break;
				case HIDE:
					rHTMLLabel_mezera->Visible = false;
					scGPNumericEdit_mezera->Visible = false;
					rHTMLLabel_mezera_jednotky->Visible = false;
					if (move)offset -= O;
					break;
			 }
			 // zkop�rov�n� vlastnost� pro MJ a MP
			 //toto nefungovalo spr�vn� (a bez ukazatele h�zelo chybu): *scGPNumericEdit_mezera_JIG->Options=*scGPNumericEdit_mezera->Options;
			 scGPNumericEdit_mezera_JIG->Options->ShapeStyle 				= scGPNumericEdit_mezera->Options->ShapeStyle;
			 scGPNumericEdit_mezera_JIG->Options->FrameNormalColor 	= scGPNumericEdit_mezera->Options->FrameNormalColor;
			 scGPNumericEdit_mezera_JIG->Options->FrameWidth 				= scGPNumericEdit_mezera->Options->FrameWidth;
			 scGPNumericEdit_mezera_JIG->Visible=scGPNumericEdit_mezera->Visible;
			 scGPNumericEdit_mezera_JIG->Enabled=scGPNumericEdit_mezera->Enabled;
			 //toto nefungovalo spr�vn� (a bez ukazatele h�zelo chybu):*scGPNumericEdit_mezera_PODVOZEK->Options=*scGPNumericEdit_mezera->Options;
			 scGPNumericEdit_mezera_PODVOZEK->Options->ShapeStyle 			= scGPNumericEdit_mezera->Options->ShapeStyle;
			 scGPNumericEdit_mezera_PODVOZEK->Options->FrameNormalColor = scGPNumericEdit_mezera->Options->FrameNormalColor;
			 scGPNumericEdit_mezera_PODVOZEK->Options->FrameWidth 			= scGPNumericEdit_mezera->Options->FrameWidth;
			 scGPNumericEdit_mezera_PODVOZEK->Visible=scGPNumericEdit_mezera->Visible;
			 scGPNumericEdit_mezera_PODVOZEK->Enabled=scGPNumericEdit_mezera->Enabled;

			 //PROVIZORN� WORKAROUND - PROTO�E MEZERU NYN� NEZOBRAZUJI, TAK ABYCH NEMUSEL SLO�IT� M�NIT KOD
			 scGPNumericEdit_mezera->Visible = false;
		} break;
		case MEZERA_JIG:
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_jig_podvozek->Top = L + 11 * O + offset;
					scGPNumericEdit_mezera_JIG->Top = P + 11 * O + offset;
			 }
			 scGPNumericEdit_mezera_JIG->Left=Le;
			 scGPNumericEdit_mezera_JIG->Width=74;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_mezera_JIG->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_jig_podvozek->Visible = true;
			 scGPNumericEdit_mezera_JIG->Visible = true;
			 scGPNumericEdit_mezera_JIG->Enabled = true;
			 scGPNumericEdit_mezera_JIG->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_mezera_JIG->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_mezera_JIG->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_mezera_JIG->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_mezera_JIG->Enabled = false;
					break;
				case READONLY:
					scGPNumericEdit_mezera_JIG->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_mezera_JIG->Enabled = false;
					break;
				case HIDE:
					rHTMLLabel_jig_podvozek->Visible = false;
					scGPNumericEdit_mezera_JIG->Visible = false;
					if (move)offset -= O;
					break;
			 }

		} break;
		case MEZERA_PODVOZEK:
		{
			 ////pozice
			 if (move)
			 {
					scGPNumericEdit_mezera_PODVOZEK->Top = P + 11 * O + offset;
					scGPNumericEdit_mezera_PODVOZEK->Left = scGPNumericEdit_mezera_JIG->Left +  scGPNumericEdit_mezera_JIG->Width + 10;
			 }
			 scGPNumericEdit_mezera_PODVOZEK->Left=Le+scGPNumericEdit_mezera_JIG->Width+8;
			 scGPNumericEdit_mezera_PODVOZEK->Width=scGPNumericEdit_mezera_JIG->Width;
			 ////funk�n� vlastnosti
			 // ty co jsou stejn�
			 scGPNumericEdit_mezera_PODVOZEK->Options->ShapeStyle = scgpessRect;
			 //rHTMLLabel_mezera->Visible = true;
			 scGPNumericEdit_mezera_PODVOZEK->Visible = true;
			 scGPNumericEdit_mezera_PODVOZEK->Enabled = true;
			 scGPNumericEdit_mezera_PODVOZEK->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_mezera_PODVOZEK->Options->FrameWidth = 1;
			 // ty co jsou rozd�ln�
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_mezera_PODVOZEK->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_mezera_PODVOZEK->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_mezera_PODVOZEK->Enabled = false;
					break;
				case READONLY:
					scGPNumericEdit_mezera_PODVOZEK->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_mezera_PODVOZEK->Enabled = false;
					break;
				case HIDE:
					scGPNumericEdit_mezera_PODVOZEK->Visible = false;
					if (scGPNumericEdit_mezera_JIG->Enabled==false)rHTMLLabel_jig_podvozek->Visible = false;//proto�e jsou na stejn�m ��dku, tak jen pokud if(scGPNumericEdit_mezera_JIG->Enabled==false)
					if (move && scGPNumericEdit_mezera_JIG->Enabled==false)offset -= O;//proto�e jsou na stejn�m ��dku, tak jen pokud if(scGPNumericEdit_mezera_JIG->Enabled==false)
					break;
			 }

		} break;
		case ROZESTUP: // po�adov�na vs. zji�t�n� kapacita objektu
		{
			////pozice
			if (move)
			{
					rHTMLLabel_rozestup->Top = L + 12 * O + offset;
					scGPNumericEdit_rozestup->Top = P + 12 * O + offset;
					scGPNumericEdit_rx->Top=scGPNumericEdit_rozestup->Top;
					rHTMLLabel_palec_vzd->Top=rHTMLLabel_rozestup->Top;
					rHTMLLabel_jednotky_vzdalenostpalcu->Top=rHTMLLabel_palec_vzd->Top;
			}
			////funk�n� vlastnosti
			// ty co jsou stejn�
			scGPNumericEdit_rozestup->Options->ShapeStyle = scgpessRect;
			rHTMLLabel_rozestup->Visible = true;
			scGPNumericEdit_rx->Visible=true;
			scGPNumericEdit_rozestup->Visible = true;
			rHTMLLabel_palec_vzd->Visible=true;
			rHTMLLabel_jednotky_vzdalenostpalcu->Visible=true;
			scGPNumericEdit_rozestup->Enabled = true;
			scGPNumericEdit_rozestup->Options->FrameNormalColor = clGray;
			scGPNumericEdit_rozestup->Options->FrameWidth = 1;
			// ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:
					scGPNumericEdit_rozestup->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_rozestup->Options->FrameWidth = hlFrameWidth;
					break;
				case ENABLED:
					break;
				case DISABLED:
					scGPNumericEdit_rozestup->Enabled = false;
					scGPNumericEdit_rx->Enabled=false;
					break;
				case READONLY:
					scGPNumericEdit_rozestup->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_rozestup->Enabled = false;
					scGPNumericEdit_rx->Enabled=true; //zmena oproti rozestupu
					break;
				case HIDE:
					rHTMLLabel_rozestup->Visible = false;
					scGPNumericEdit_rozestup->Visible = false;
					scGPNumericEdit_rx->Visible=false;
					rHTMLLabel_palec_vzd->Visible=false;
					rHTMLLabel_jednotky_vzdalenostpalcu->Visible=false;
					if (move)offset -= O;
					break;
			}
		} break;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�i psan� n�zvu objektu se m�n� titulek a generuje zkratka
void __fastcall TForm_parametry::scGPEdit_nameChange(TObject *Sender)
{
		Form_parametry->scLabel_titulek->Caption = scGPEdit_name->Text + " - parametry";
		scGPEdit_shortname->Text = scGPEdit_name->Text.SubString(1, 4).UpperCase();
}
//---------------------------------------------------------------------------
//p�i zm�n�ch EDIT�
void __fastcall TForm_parametry::scGPNumericEdit_poziceChange(TObject *Sender)
{
		if (input_state == NOTHING && input_clicked_edit == P_klik)
		{
				input_P();
				if(scComboBox_rezim->ItemIndex==2) //PP - pokud je CT vy��� ne� TT nezobraz�m z�mek u CT
				{
						if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
				}
		}
}
//----------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_CTChange(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == CT_klik) {

				input_CT(); // p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT

		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuChange(TObject *Sender)
{
		if (input_state == NOTHING && input_clicked_edit == DD_klik)
		{
				input_DD();
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_mezeraChange(TObject *Sender)
{
	 Nacti_rx();//m� vliv i na zm�nu Rz, Rx p�i zm�n� RD, pop�. dal��ch parametr�
	 Check_rozmezi_RD();

	 if (input_state == NOTHING && input_clicked_edit == mezera_klik && scComboBox_rezim->ItemIndex == 2)
	 { // pokud nen� zad�v�no z jin�ho vstupu
			input_M(); // lok�ln� p�i PP re�imu
	 }

	 if (input_state == NOTHING)
	 {    	//vypis("");
			if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&	input_clicked_edit == mezera_klik)
			{
				if (!Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom)) 	vypis("Povolte zm�nu rychlosti pohonu.");  // pokud nen� pohon pou��v�n, d�m v�pis, jinak v�pis nem� smysl - ji� nelze m�nit rychlost


			} // nen� zam�eno - doporu��m mezeru, �ek�m zdali zad� spr�vnou mezeru
			else
			if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex>0)
			{
					vypis("");
					if (Kontrola_mezery() == Form1->ms.MyToDouble(getM()))
					{
						 input_M(); // lok�ln�
						 Nacti_rx();
					}
			}
			if(scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex==0) input_M();  // p��pad kdy m�n�m mezeru ale jsem bez pohonu
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RD_Change(TObject *Sender)
{
		if (input_state == NOTHING && scButton_zamek_RD->Enabled)
		{
				if(input_clicked_edit == RD_klik || input_state_Rz)
				{
					if (scComboBox_rezim->ItemIndex==1) // controller pro KK re�im
					{
						input_RD();
						Check_rozmezi_RD();
					}
				}
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaChange(TObject *Sender)
{
		if (input_state == NOTHING && input_clicked_edit == C_klik)
		{
			input_K(); // p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
			if(scComboBox_rezim->ItemIndex==2)
			{
			//PP - pokud je CT vy��� ne� TT nezobraz�m z�mek u CT
					if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
			}
		}
}
//---------------------------------------------------------------------------
// p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
void TForm_parametry::input_K()
{
		input_state = K;
		INPUT();
		double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
		if (CTunit == MIN)CT = CT * 60.0; //pokud bylo zad�n� v minut�ch p�evede na sekundy
		double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele

		//if (K > 0)   // shodne vol�n� pro KK i PP re�imy
		{
				 //	vypis("");
					if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
					{
						// Memo1->Lines->Add("volam KK s false ");
						pm.input_K(false);
					}
					else
					{
						if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
						pm.input_K(false); // Memo1->Lines->Add("volam KK  ");//	vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
						else//za ka�d� situace standardn� vol�n�
						pm.input_K();
					}
		}

		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////
		 OUTPUT(); //REVIZE  -    je takto vyseparovano output spr�vn�? Ano v�po�etn� model vr�ti data a d�le pokud je spln�n IF tak se za�nou data porovn�vat

			if(scComboBox_rezim->ItemIndex==1 && scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
		{  // u KK prob�h� kontrola vypo��tan�ch dat a doporu�� K pokud RD!=dopRD

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60.0;//REVIZE spr�vn�
				double DD=scGPNumericEdit_delka_dopravniku->Value;
			//	double CT =scGPNumericEdit_CT->Value;
				double K = scGPNumericEdit_kapacita->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,roztec,Form1->d.v.PP.TT,RD);


		 //		if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; } //REVIZE- p�evod je za ��elem n�sledn�ho v�po�tu pro K, �patn� pro� znovu RD*=60
			 //	if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;} //REVIZE -  tento p�evod ji� nen� pou��v�n
				if (DDunit == MM)  DD = DD / 1000.0;
			//	if (CTunit == M)  CT = CT / 60.0; //REVIZE - v t�to metod� se CT nepou��v�,  je ok?


			if(dopRD!=RD)
			{
			//Memo1->Lines->Add(DD);
		 //	Memo1->Lines->Add(dopRD);
			//Memo1->Lines->Add(Form1->d.v.PP.TT);
				K =  (DD/dopRD) / (Form1->d.v.PP.TT);
				vypis("Doporu�en� kapacita : <u>" +AnsiString(K) +"</u> ");
				VID=32;VID_value=K;
					//AnsiString relation_id=GetCurrentProcessId();
					//AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(K)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
					//Form1->FDConnection1->ExecSQL(strSQL);
			}

		}
		input_state = NOTHING;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry::input_P()
{
		input_state = P;
		INPUT();

		//if (scGPNumericEdit_pozice->Value > 0)
		{
				double K=pm.P2K(scGPNumericEdit_pozice->Value);
				double CT=scGPNumericEdit_CT->Value;
				if (CTunit == MIN)
				CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy

				if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
				{
					pm.input_P(false);
				}
				else
				{
					if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
					{
						pm.input_P(false);
						//Memo1->Lines->Add("input_P(false) ");
					}// Memo1->Lines->Add("volam KK  ");//	vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
					else//za ka�d� situace standardn� vol�n�
					pm.input_P();
				}
		}
		// na�ten� dat zp�t do formul��e po v�po�tu
		OUTPUT();


		if(scComboBox_rezim->ItemIndex==1 && scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
		{  // u KK prob�h� kontrola vypo��tan�ch dat a doporu�� P pokud RD!=dopRD   ImageIndex==38 = odem�en z�mek RD

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60.0;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
//				double CT =scGPNumericEdit_CT->Value;
				double K = scGPNumericEdit_kapacita->Value;
				double Pozice = scGPNumericEdit_pozice->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,roztec,Form1->d.v.PP.TT,RD);

//				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }   //REVIZE - K po��tan bez p�evodu, nen� nutn� zde p�ev�d�t
		 //		if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
				if (DDunit == MM)  DD = DD / 1000.0;
//				if (CTunit == M)  CT = CT / 60.0;


			if(dopRD!=RD)
			{
				K =  (DD/dopRD) / (Form1->d.v.PP.TT);
				//Memo1->Lines->Add(K);
				double dop_K =  pm.K2P(K);
				if(dop_K != Pozice )
				{
			 // Memo1->Lines->Add(dop_K);
				vypis("Doporu�en� po�et pozic : <u>" +AnsiString(dop_K) +"</u> ");
				VID=33;
				VID_value=dop_K;
					//AnsiString relation_id=GetCurrentProcessId();
					//AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(dop_K)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
					//Form1->FDConnection1->ExecSQL(strSQL);

				}
			} else vypis("",false);

	 }
		input_state = NOTHING;
}
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
void TForm_parametry::input_CT()
{
		input_state = CT;
		INPUT();

		if (scComboBox_rezim->ItemIndex == 1)
		{ //KK re�im
			pm.input_CT();
			OUTPUT();
			if(scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
			{
				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60.0;//spr�vn�  - p�evedu do m/s kv�li pou�it� v metod�
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =0;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,roztec,Form1->d.v.PP.TT,RD);

			 //	if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; } //REVIZE - p�evod k ni�emu
				//if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;} //REVIZE - ji� se nepou��v�, to sam�
				if (DDunit == MM)  DD = DD / 1000.0;

					if(dopRD!=RD)

					{
						CT = DD/dopRD; //REVIZE - upraveno bez p�evodu,  pro� kdy� u� je p�evedeno dopRD na sekundy v��e p��padn� bacha na z�vorky
						if(Form1->ms.MyToDouble(CT)!=Form1->ms.MyToDouble(scGPNumericEdit_CT->Value))
						{
						vypis("Doporu�en� techn.�as : <u>" +AnsiString(CT) +"</u> s ");
						VID=30;
						VID_value=CT;
						//AnsiString relation_id=GetCurrentProcessId();
						//		AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(CT)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
						//		Form1->FDConnection1->ExecSQL(strSQL);
						}
								else vypis("",false);
					}
			}
		}

		if (scComboBox_rezim->ItemIndex == 0)
		{ // pokud je v re�imu SG nevol� se v�po�etn� model
						OUTPUT();
		}

		if (scComboBox_rezim->ItemIndex == 2)
		{

           	Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, CT_klik, false); // p�i ka�d� zm�n� CT vol�m nastaven� z�mk�, kv�li tomu aby kdy� je zad�van� CT ni��� ne� TT aby do�lo ihned k zobrazen� z�mku
						pm.input_CT();
						OUTPUT();
		}

		input_state = NOTHING;
}
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny DD
void TForm_parametry::input_DD()
{
		input_state = DD;
		INPUT();
		if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==1) // KK
		{
				pm.input_DD(); // zavol�n� v�po�etn�ho modelu
				OUTPUT();

			if(scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
			{

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60.0;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =scGPNumericEdit_CT->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,roztec,Form1->d.v.PP.TT,RD);

			//	if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }  //REVIZE - p�evody nejsou nutn�
			//	if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
			//	if (DDunit == MM) { DD = DD / 1000.0; }
			//	if (CTunit == M)  CT = CT / 60.0;

					if(dopRD!=RD)

					{

					double	 DD_dop = CT*dopRD;

						if(Form1->ms.MyToDouble(DD_dop)!= Form1->ms.MyToDouble(DD))
						{
						vypis("Doporu�en� d�lka kabiny : <u>" +AnsiString(DD_dop) +"</u> [m] ");
						VID=31;
						VID_value=DD_dop;
				//		AnsiString relation_id=GetCurrentProcessId();
				//		AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(DD_dop)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
				//		Form1->FDConnection1->ExecSQL(strSQL);

						} else vypis("",false);
					}
			}
		}

		if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==2) // KK a PP re�im
				// nutn� o�et�en� pro obdob� zad�v�n�/psan�
		{
				pm.input_DD(); // zavol�n� v�po�etn�ho modelu
				OUTPUT();
		}

		if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==0)
		{
				OUTPUT(); // u SG re�imu nevol�m v�po�etn� model, output je vol�n pouze pro validaci dat
		}
		input_state = NOTHING;
}
//-----------------------------------------------------------------------------
void TForm_parametry::input_M()
{
		input_state = mezera;
		INPUT();
		pm.input_M();
		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////
		OUTPUT();
		input_state = NOTHING;
}
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD, pouze pro kontinual
void TForm_parametry::input_RD()
{
		input_state = RD;
		INPUT();
		pm.input_RD();
		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////
		OUTPUT();
		input_state = NOTHING;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vynuluje vstupn� hodnoty
void TForm_parametry::null_input_value() {
		// scGPNumericEdit_RD->Value=0;
		// scGPNumericEdit_CT->Value=0;
		// scGPNumericEdit_delka_dopravniku->Value=0;
		// scGPNumericEdit_kapacita->Value=0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//void TForm_parametry::vykresli_vozik(bool na_delku)
/*{  ZDM
		 Image_vozik->Canvas->Pen->Color=clBtnFace;
		 Image_vozik->Canvas->Pen->Width=1;
		 Image_vozik->Canvas->Brush->Color=clBtnFace;
		 Image_vozik->Canvas->Rectangle(0,0,Image_vozik->Width,Image_vozik->Height);//vybarven� cel�ho
		 TPoint stred; stred.x=Image_vozik->Width/2;stred.y=Image_vozik->Height/2;
		 double D,S;
		 unsigned short meritko=(Image_vozik->Width-6)/Form1->d.v.PP.delka_voziku;
		 if(meritko>(Image_vozik->Width-6)/Form1->d.v.PP.sirka_voziku)meritko=(Image_vozik->Width-6)/Form1->PP.sirka_voziku;
		 if(meritko>(Image_vozik->Height-6)/Form1->d.v.PP.delka_voziku)meritko=(Image_vozik->Height-6)/Form1->PP.delka_voziku;
		 if(meritko>(Image_vozik->Height-6)/Form1->d.v.PP.sirka_voziku)meritko=(Image_vozik->Height-6)/Form1->PP.sirka_voziku;

		 if(na_delku){D=Form1->PP.delka_voziku;S=Form1->d.v.PP.sirka_voziku;}
		 else{S=Form1->PP.delka_voziku;D=Form1->d.v.PP.sirka_voziku;}

		 //packy
		 Image_vozik->Canvas->Pen->Color=clBlack;
		 Image_vozik->Canvas->Pen->Width=1;
		 Image_vozik->Canvas->MoveTo(stred.x-D/2*meritko,stred.y);
		 Image_vozik->Canvas->LineTo(stred.x-D/2*meritko-20,stred.y);
		 Image_vozik->Canvas->MoveTo(stred.x+D/2*meritko,stred.y);
		 Image_vozik->Canvas->LineTo(stred.x+D/2*meritko+20,stred.y);
		 //sipka -lev�
		 Image_vozik->Canvas->Brush->Color=clGray;
		 Image_vozik->Canvas->Pen->Color=clBlack;
		 Image_vozik->Canvas->Pen->Width=1;
		 POINT bodyL[3]={{stred.x-D/2*meritko-13,stred.y-3},{stred.x-D/2*meritko-7,stred.y},{stred.x-D/2*meritko-13,stred.y+3}};
		 Image_vozik->Canvas->Polygon((TPoint*)bodyL,2);
		 //sipka - prav�
		 POINT bodyP[3]={{stred.x+D/2*meritko+7,stred.y-3},{stred.x+D/2*meritko+13,stred.y},{stred.x+D/2*meritko+7,stred.y+3}};
		 Image_vozik->Canvas->Polygon((TPoint*)bodyP,2);
		 //voz�k
		 Image_vozik->Canvas->Pen->Width=2;
		 Image_vozik->Canvas->Brush->Color=clWhite;
		 Image_vozik->Canvas->Rectangle(stred.x-D/2*meritko,stred.y-S/2*meritko,stred.x+D/2*meritko,stred.y+S/2*meritko);
		 //popisek
		 Image_vozik->Canvas->Font->Size=7;
		 Image_vozik->Canvas->TextOutW(stred.x-Image_vozik->Canvas->TextWidth(D)/2,stred.y+S/2*meritko-Image_vozik->Canvas->TextHeight(D)-2,D);
		 Image_vozik->Canvas->TextOutW(stred.x-D/2*meritko+2,stred.y-Image_vozik->Canvas->TextHeight(S)/2,S);

}*/
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender)
{
		// vykresli_vozik(true);//na d�lku
		// ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender)
{
		// vykresli_vozik(false);//na ���ku
		// ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender)
{
		// if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
		// else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}
//---------------------------------------------------------------------------
// p�ep�n�n� zobrazen� min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender)
{
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double RD = scGPNumericEdit_RD->Value;
		double CT = scGPNumericEdit_CT->Value;
		if (minsec == MIN) // pokud je v minut�ch, tak p�epne na sekundy
		{
				minsec = S;
				scGPButton_min_sec->Caption = "v�e na min";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// CT - p�epo��t�n�
				if(CTunit != S)
				{
					CTunit = S;
					CT = scGPNumericEdit_CT->Value * 60.0;
					rHTMLLabel_CT->Caption = "Technologick� �as <font color=#2b579a>[s]</font>";
				}
				// RD - p�epo��t�n�
				if(RDunitT != S)
				{
					RDunitT = S;
					//if (RDunitD == MM) rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v milimetrech
					//else
					rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v metrech
					RD = scGPNumericEdit_RD->Value / 60.0;
				}
		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				minsec = MIN;
				scGPButton_min_sec->Caption = "v�e na s";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// CT - p�epo��t�n�
				if(CTunit != MIN)
				{
					CTunit = MIN;
					CT = scGPNumericEdit_CT->Value / 60.0;
					rHTMLLabel_CT->Caption = "Technologick� �as <font color=#2b579a>[min]</font>";
				}
				// RD - p�epo��t�n�
				if(RDunitT != MIN)
				{
					RDunitT = MIN;
					//if (RDunitD == MM) rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v milimetrech
					//else
					rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v metrech
					RD = scGPNumericEdit_RD->Value * 60.0;
				}
		}
		// pln�n� + po�et desetinn�ch m�st
		scGPNumericEdit_CT->Value = CT;
		scGPNumericEdit_RD->Value = RD;
		Form1->aktualizace_combobox_pohony_v_PO(0,RDunitT);//zaktualizovat v�pis + o jednotky
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek CT
void __fastcall TForm_parametry::rHTMLLabel_CTClick(TObject *Sender)
{
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double CT = 0.0;
    double odchylka=0.0;
		if (CTunit == MIN) // pokud je v minut�ch, tak p�epne na sekundy
		{
				CTunit = S;
				// CT - p�epo��t�n�
				CT = scGPNumericEdit_CT->Value * 60.0;
        odchylka=scGPNumericEdit_odchylka->Value * 60.0;
				rHTMLLabel_CT->Caption = "Technologick� �as <font color=#2b579a>[s]</font>";
        rHTMLLabel_odchylka->Caption="Odchylka z techn. �asu [s]";
				Form_objekt_nahled->ButtonPLAY->Caption = "0 [s]";
		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				CTunit = MIN;
				// CT - p�epo��t�n�
				CT = scGPNumericEdit_CT->Value / 60.0;
        odchylka = scGPNumericEdit_odchylka->Value / 60.0;
				rHTMLLabel_CT->Caption = "Technologick� �as <font color=#2b579a>[min]</font>";
        rHTMLLabel_odchylka->Caption="Odchylka z techn. �asu [m]";
				Form_objekt_nahled->ButtonPLAY->Caption = "0 [min]";
		}
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
    scGPNumericEdit_odchylka->Value=odchylka;
		scGPNumericEdit_CT->Value = CT;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek RD
void __fastcall TForm_parametry::rHTMLLabel_RDClick(TObject *Sender)
{
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double RD = 0.0;

		if(RDunitT == MIN)// pokud je v minut�ch, tak p�epne na sekundy
		{
				RDunitT = S;
				//if(RDunitD == MM)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v milimetrech
				//else
				rHTMLLabel_RD->Caption =	"Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v metrech
				RD = scGPNumericEdit_RD->Value / 60.0;
				// RD=RDunitD_funkce(RD); tady opravdu nesm� b�t
		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				RDunitT = MIN;
				//if(RDunitD == MM)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v milimetrech
				//else
				rHTMLLabel_RD->Caption ="Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v metrech
				RD = scGPNumericEdit_RD->Value * 60.0;
				//zru�eno u�it� RD = RDunitD_funkce(RD);//tady ano
		}
		//zru�eno u�it� RD = RDunitD_funkce(RD);//ano tady opravdu znovu

		// pln�n� + po�et desetinn�ch m�st
		scGPNumericEdit_RD->Value = RD;
		F->aktualizace_combobox_pohony_v_PO(0,RDunitT);//zaktualizovat v�pis + o jednotky
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//zru�eno u�it�
double TForm_parametry::RDunitD_funkce(double RD)// podp�rn� metoda v��e uveden�
{
//		if (RDunitD == MM) // pokud je v milimetrech, tak p�epne na metry
//		{
//				RDunitD = M;
//				if (RDunitT == MIN)	rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v minut�ch
//				else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>"; // pokud je v sekund�ch
//				return RD / 1000.0;
//		}
//		else //pokud je v metrech, tak p�epne na milimetry
//		{
//				RDunitD = MM;
//				if (RDunitT == MIN)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v minut�ch
//				else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekund�ch
//				return RD * 1000.0;
//		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// p�ep�n�n� zobrazen� m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender)
{
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double DD = scGPNumericEdit_delka_dopravniku->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek
		double DM = scGPNumericEdit_mezera->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek
		double MJ = scGPNumericEdit_mezera_JIG->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek
		double MP = scGPNumericEdit_mezera_PODVOZEK->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek
		double Rz = scGPNumericEdit_rozestup->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek
		double RD = scGPNumericEdit_RD->Value;//napln�n� hodnot zde, pro p��pad, �e by se p�ep�nalo z jednotek do stejn�ch jednotek

		if (m_mm == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				m_mm = M;scGPButton_metry_milimetry->Caption = "v�e na mm";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// DD
				if(DDunit != M)
				{
					DDunit = M;
					rHTMLLabel_delka_dopravniku->Caption ="D�lka kabiny <font color=#2b579a>[m]</font>";
					DD = scGPNumericEdit_delka_dopravniku->Value / 1000.0;
				}
				// M + MJ + MP + Rz
				if(DMunit != M)
				{
					DMunit = M;
					//rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
					rHTMLLabel_mezera_jednotky->Caption = "<font color=#2b579a>[m]</font>";
					DM = scGPNumericEdit_mezera->Value / 1000.0;
					MJ = scGPNumericEdit_mezera_JIG->Value / 1000.0;
					MP = scGPNumericEdit_mezera_PODVOZEK->Value / 1000.0;
					rHTMLLabel_jednotky_vzdalenostpalcu->Caption="<font color=#2b579a>[m]</font>";
					Rz = scGPNumericEdit_rozestup->Value / 1000.0;
				}
				// RD //zru�eno u�it�
//				if(RDunitD != M)
//				{
//					RDunitD = M;
//					if (RDunitT == MIN)rHTMLLabel_RD->Caption ="Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v minut�ch
//					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekund�ch
//					RD = scGPNumericEdit_RD->Value / 1000.0;
//				}
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
				m_mm = MM;scGPButton_metry_milimetry->Caption = "v�e na m";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// DD
				if(DDunit != MM)
				{
					DDunit = MM;
					rHTMLLabel_delka_dopravniku->Caption = "D�lka kabiny <font color=#2b579a>[mm]</font>";
					DD = scGPNumericEdit_delka_dopravniku->Value * 1000.0;
				}
				// DM + Rz
				if(DMunit != MM)
				{
					DMunit = MM;
					//rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
					rHTMLLabel_mezera_jednotky->Caption = "<font color=#2b579a>[mm]</font>";
					DM = scGPNumericEdit_mezera->Value * 1000.0;
					MJ = scGPNumericEdit_mezera_JIG->Value * 1000.0;
					MP = scGPNumericEdit_mezera_PODVOZEK->Value * 1000.0;
					rHTMLLabel_jednotky_vzdalenostpalcu->Caption="<font color=#2b579a>[mm]</font>";
					Rz = scGPNumericEdit_rozestup->Value * 1000.0;
				}
				// RD  //zru�eno u�it�
//				if(RDunitD != MM)
//				{
//					RDunitD = MM;
//					if (RDunitT == MIN)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v minut�ch
//					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekund�ch
//					RD = scGPNumericEdit_RD->Value * 1000.0;
//				}
		}
		// pln�n� + po�et desetinn�ch m�st
		scGPNumericEdit_delka_dopravniku->Value = DD;
		scGPNumericEdit_mezera->Value = DM;
		scGPNumericEdit_mezera_JIG->Value =  MJ;
		scGPNumericEdit_mezera_PODVOZEK->Value = MP;
		scGPNumericEdit_rozestup->Value=Rz;
		scGPNumericEdit_RD->Value = RD;
		Form1->aktualizace_combobox_pohony_v_PO(0,RDunitT);//zaktualizovat v�pis + o jednotky
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek DD
void __fastcall TForm_parametry::rHTMLLabel_delka_dopravnikuClick
		(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double DD = 0.0;
		if (DDunit == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				DDunit = M;
				rHTMLLabel_delka_dopravniku->Caption =
						"D�lka kabiny <font color=#2b579a>[m]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value / 1000.0;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
				DDunit = MM;
				rHTMLLabel_delka_dopravniku->Caption =
						"D�lka kabiny <font color=#2b579a>[mm]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value * 1000.0;
		}
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_delka_dopravniku->Value = DD;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}

//---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek DM
void __fastcall TForm_parametry::rHTMLLabel_mezera_jednotkyClick(TObject *Sender)
{
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double DM = 0.0;
		double MJ = 0.0;
		double MP = 0.0;
		double Rz=0.0;
		if (DMunit == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				// DM
				DMunit = M;
				//rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
				rHTMLLabel_mezera_jednotky->Caption = "<font color=#2b579a>[m]</font>";
				DM = scGPNumericEdit_mezera->Value / 1000.0;
				MJ = scGPNumericEdit_mezera_JIG->Value / 1000.0;
				MP = scGPNumericEdit_mezera_PODVOZEK->Value / 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption="<font color=#2b579a>[m]</font>";
				Rz = scGPNumericEdit_rozestup->Value / 1000.0;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
				// DM
				DMunit = MM;
				//rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
				rHTMLLabel_mezera_jednotky->Caption = "<font color=#2b579a>[mm]</font>";
				DM = scGPNumericEdit_mezera->Value * 1000.0;
				MJ = scGPNumericEdit_mezera_JIG->Value * 1000.0;
				MP = scGPNumericEdit_mezera_PODVOZEK->Value * 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption="<font color=#2b579a>[mm]</font>";
				Rz = scGPNumericEdit_rozestup->Value * 1000.0;
		}
		// pln�n� + po�et desetinn�ch m�st
		scGPNumericEdit_mezera->Value = DM;
		scGPNumericEdit_mezera_JIG->Value =  MJ;
		scGPNumericEdit_mezera_PODVOZEK->Value = MP;
		scGPNumericEdit_rozestup->Value=Rz;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// p�i stisku kl�vesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	 switch(Key)
	 {
		 case 13: // ENTER
		 {
		 	if (scGPButton_OK->Enabled)
		 			// pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 	{
		 			Form_parametry->ModalResult = mrOk;
		 			// vr�t� stejnou hodnotu jako tla��tko
		 			Form_parametry->VisibleChanging();
		 			// skryje form, stejn� jako visible=false
		 	}
		 	else
		 			MessageBeep(0); // p�pnut�m upozorn�, �e nelze
		 }break;
		 case 27:// ESC
		 {
		 	Form_parametry->ModalResult = mrCancel;
		 	// vr�t� stejnou hodnotu jako tla��tko
		 	Form_parametry->VisibleChanging();
		 	// skryje form, stejn� jako visible=false
		 }break;
		 case 67://c nebo C
		 {
		 	if(Shift.Contains(ssCtrl))//CTRL+C
			{
				scGPGlyphButton_copyClick(Sender);//to sam� co ikona kop�rovat
			}
		 }break;
		 case 86://v nebo V
		 {
		 	if(Shift.Contains(ssCtrl))//CTRL+V
		 	{
		 		if(mrYes==F->MB("Chcete vlo�it hodnoty parametr� ze schr�nky? Sou�asn� hodnoty budou p�eps�ny.",MB_YESNO))
		 		{
					scGPGlyphButton_pasteClick(Sender);//to sam� co ikona vlo�it
				}
		 	}
		 }break;
		 case 90://z nebo Z
		 {
		 	if(Shift.Contains(ssCtrl))//CTRL+Z
		 	{
		 		//if(mrYes==F->MB("Chcete nastavit p�edchoz� hodnoty parametr�?",MB_YESNO))
		 		{
		 			//
		 		}
		 	}
		 }break;
		 case 116://F5
		 {
	   refresh_data();
		 }break;
		 case 123:// F12
		 {
     scGPButton_OK->Enabled=true;
		 	if (DEBUG) // pouze v DEBUGU
		 	{
//		 		Memo1->Visible = true;
//		 		Memo1->Lines->Clear();
//		 		//	Memo1->Lines->Add(pm.T);
//		 		Memo1->Top = 0;
//				Memo1->Left = 0;
				//test
				/*
				frameCorrelation();
        TCanvas *C=new(TCanvas);
				C->Handle=GetWindowDC(HWND_DESKTOP);
				C->Pen->Color=clWebOrange;
				C->Pen->Mode=pmCopy;//pmNotXor;
				C->Pen->Width=2;
				C->Brush->Style=bsClear;
				C->MoveTo(Form_objekt_nahled->Left+50,Form_objekt_nahled->Top+Form_objekt_nahled->Height/2+Form_objekt_nahled->scLabel_titulek->Height);
				C->LineTo(Form_objekt_nahled->Left+50,Top+rHTMLLabel_rozestup->Top+rHTMLLabel_rozestup->Height/2);
				C->LineTo(Left+rHTMLLabel_rozestup->Left,Top+rHTMLLabel_rozestup->Top+rHTMLLabel_rozestup->Height/2);
				*/
//				TPoint S=getRMComponent(MEZERA);TPoint E=getRMComponent(ROZESTUP);
//				ShowMessage(AnsiString(S.x)+";"+AnsiString(E.x));
//				ShowMessage(rHTMLLabel_mezera_jednotky->Width);
//				ShowMessage(rHTMLLabel_jednotky_vzdalenostpalcu->Width);
			}
		 }break;
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// p�i stisku storna
void __fastcall TForm_parametry::scGPButton_stornoClick(TObject *Sender)
{
		kapacitaSG = 1; // nastav� na default
}
//---------------------------------------------------------------------------
// p�i stisku storna
void __fastcall TForm_parametry::KonecClick(TObject *Sender)
{
		kapacitaSG = 1; // nastav� na default
}
//---------------------------------------------------------------------------
// ulo�� do INI nastaven� formul��e
void __fastcall TForm_parametry::FormClose(TObject *Sender,TCloseAction &Action)
{
		Form1->writeINI("nastaveni_form_parametry", "cas", minsec);
		Form1->writeINI("nastaveni_form_parametry", "vzdalenost", m_mm);
		Form1->writeINI("nastaveni_form_parametry", "CT", CTunit);
	 	Form1->writeINI("nastaveni_form_parametry", "RDt", RDunitT);
		Form1->writeINI("nastaveni_form_parametry", "RDd", 0);
		Form1->writeINI("nastaveni_form_parametry", "DD", DDunit);
		Form1->writeINI("nastaveni_form_parametry", "DM", DMunit);
}
//---------------------------------------------------------------------------
// zkop�ruje parametry objektu pro dal�� pou�it�
void __fastcall TForm_parametry::scGPGlyphButton_copyClick(TObject *Sender)
{
		// povol� tla��tko vkl�d�n�
		scGPGlyphButton_paste->Enabled = true;

		// p�evod jednotek
		double CT = scGPNumericEdit_CT->Value; 							 if(CTunit == MIN) CT = CT * 60.0;
		double RD = scGPNumericEdit_RD->Value; 							 if(RDunitT == MIN)RD = RD / 60.0;
		double DD = scGPNumericEdit_delka_dopravniku->Value; if(DDunit == MM)  DD = DD / 1000.0;
		double M	= scGPNumericEdit_mezera->Value;           if(DMunit == MM)  M  = M  / 1000.0;
		double MJ = scGPNumericEdit_mezera_JIG->Value;       if(DMunit == MM)  MJ = MJ / 1000.0;
		double MP = scGPNumericEdit_mezera_PODVOZEK->Value;  if(DMunit == MM)  MP = MP / 1000.0;
		double Rz =	scGPNumericEdit_rozestup->Value;         if(DMunit == MM)  Rz = Rz / 1000.0;
		// text do schr�nky
		AnsiString T = "";

		// atributy
		F->copyObjekt->name=scGPEdit_name->Text;T=scGPEdit_name->Text;T+=";";
		F->copyObjekt->short_name=scGPEdit_shortname->Text;T+=scGPEdit_shortname->Text;T+=";";
		F->copyObjekt->rezim=scComboBox_rezim->ItemIndex;T+=scComboBox_rezim->ItemIndex;T+=";";
		F->copyObjekt->pohon=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);/* indexuje se od nuly */ T+=scComboBox_pohon->ItemIndex;T+=";";
		F->copyObjekt->CT=CT;T+=CT;T+=";";
		F->copyObjekt->RD=RD;T+=RD;T+=";";
		F->copyObjekt->delka_dopravniku=DD;T+=DD;T+= ";";
		F->copyObjekt->kapacita=scGPNumericEdit_kapacita->Value;T+=Form1->copyObjekt->kapacita;T+=";";
		F->copyObjekt->pozice=scGPNumericEdit_pozice->Value;T+=Form1->copyObjekt->pozice;T+=";";
		F->copyObjekt->odchylka=scGPNumericEdit_odchylka->Value;T+=Form1->copyObjekt->odchylka;T+=";";
		F->copyObjekt->cekat_na_palce = scComboBox_cekani_palec->ItemIndex;T+=scComboBox_cekani_palec->ItemIndex;T+=";";
		F->copyObjekt->stopka=scComboBox_stopka->ItemIndex;T+=Form1->copyObjekt->stopka;T+=";";
		F->copyObjekt->rotace=scComboBox_rotace->ItemIndex;T+=Form1->copyObjekt->rotace;T+=";";
		F->copyObjekt->mezera=M;T+=M;T+=";";
		F->copyObjekt->mezera_jig=MJ;T+=MJ;T+=";";
		F->copyObjekt->mezera_podvozek=MP;T+=MP;T+=";";
		F->copyObjektRzRx.x=scGPNumericEdit_rx->Value;T+=Form1->copyObjektRzRx.x;T+=";";
		F->copyObjektRzRx.y=Rz;T+=Rz;T+=";";
		//je�t� zv�it kop�rov�n� nastaven� z�mk�

		// zkop�rov�n� textu do schr�nky
		TClipboard *c = new TClipboard;
		c->AsText = T;
		c = NULL;
		delete c;
}
//---------------------------------------------------------------------------
// zaji��uje vlo�en� objektu
void __fastcall TForm_parametry::scGPGlyphButton_pasteClick(TObject *Sender)
{
		// aby se nevolalo p�epo��t�v�n� p�i editu polo�ek
		input_state = NO;

		// p�evod jednotek
		double CT = Form1->copyObjekt->CT; 							 if(CTunit == MIN) CT = CT / 60.0;
		double RD = Form1->copyObjekt->RD; 							 if(RDunitT == MIN)RD = RD * 60.0;
		double DD = Form1->copyObjekt->delka_dopravniku; if(DDunit == MM)  DD = DD * 1000.0;
		double M	= Form1->copyObjekt->mezera;           if(DMunit == MM)  M  = M  * 1000.0;
		double MJ = Form1->copyObjekt->mezera_jig;       if(DMunit == MM)  MJ = MJ * 1000.0;
		double MP = Form1->copyObjekt->mezera_podvozek;  if(DMunit == MM)  MP = MP * 1000.0;
		double Rz = Form1->copyObjektRzRx.y;  					 if(DMunit == MM)  MP = MP * 1000.0;

		scGPEdit_name->Text = Form1->copyObjekt->name;
		scGPEdit_shortname->Text = Form1->copyObjekt->short_name;
		scComboBox_rezim->ItemIndex = Form1->copyObjekt->rezim;
		if (Form1->copyObjekt->pohon == NULL)scComboBox_pohon->ItemIndex=0;
		else scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n;
		scGPNumericEdit_CT->Value=CT;
		scGPNumericEdit_RD->Value=RD;
		scGPNumericEdit_delka_dopravniku->Value=DD;
		scComboBox_cekani_palec->ItemIndex=Form1->copyObjekt->cekat_na_palce;
		scGPNumericEdit_kapacita->Value=Form1->copyObjekt->kapacita;
		scGPNumericEdit_odchylka->Value=Form1->copyObjekt->odchylka;
		scComboBox_stopka->ItemIndex=Form1->copyObjekt->stopka;
		scComboBox_rotace->ItemIndex=Form1->copyObjekt->rotace;
		scGPNumericEdit_mezera->Value=M;
		scGPNumericEdit_mezera_JIG->Value=MJ;
		scGPNumericEdit_mezera_PODVOZEK->Value=MP;

		scGPNumericEdit_rx->Value=Form1->copyObjektRzRx.x;
		scGPNumericEdit_rozestup->Value=Rz;
		//je�t� zv�it p�eb�r�n� stavu z�mk�

		//nastaven� aktu�ln�ho re�imu objektu
		setForm4Rezim(scComboBox_rezim->ItemIndex);

		//napozicov�n� cel�ho formul��e resp. o�et�en� aby z�stal dialog na monitoru, pouze pro prvotn� zobrazen� dle sou�adnic kurzoru my�i, jinak dle u�ivatele
		position();

		// povolen� p�epo��t�v�n�
		input_state = NOTHING;

		VALIDACE();//nutnost po vlo�en� zkontrolovat platnost hodnot a mo�nost ulo�en�
}
//---------------------------------------------------------------------------
// z�mek procesn�ho �asu
void __fastcall TForm_parametry::scButton_zamek_CTClick(TObject *Sender)
{
		input_clicked_edit=empty_klik;
		input_clicked_icon=CT_klik_ico;
		Nastav_zamky(scComboBox_rezim->ItemIndex, CT_klik_ico, empty_klik, true);
		scButton_zamek_CT->SetFocus(); // o�et�en� proti zm�n� dat p�i zam�en�m z�mku

		INPUT();
		double	K=scGPNumericEdit_kapacita->Value;
		double  CT=scGPNumericEdit_CT->Value; // CT od u�ivatele
		if (CTunit == MIN) CT = CT * 60.0; //REVIZE pokud bylo zad�n� v minut�ch p�evede na sekundy

		if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT){
		// Memo1->Lines->Add("volam KK s false ");
		pm.input_K(false); }
		else
		{
			if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
			{;} // Memo1->Lines->Add("volam KK  ");//	vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
			else//za ka�d� situace standardn� vol�n�
			pm.input_K();
		}
		OUTPUT();
		input_clicked_icon=empty_klik_ico;
}
//---------------------------------------------------------------------------
// z�mek d�lky dopravn�ku
void __fastcall TForm_parametry::scButton_zamek_DDClick(TObject *Sender)
{
		input_clicked_edit=empty_klik;
		input_clicked_icon=DD_klik_ico;
		Nastav_zamky(scComboBox_rezim->ItemIndex, DD_klik_ico, empty_klik, true);
		scButton_zamek_DD->SetFocus(); // o�et�en� proti zm�n� dat p�i zam�en�m z�mku
		input_clicked_icon=empty_klik_ico;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scButton_zamek_RDClick(TObject *Sender)
{
		input_clicked_edit=empty_klik;
		input_clicked_icon=RD_klik_ico;
		Nastav_zamky(scComboBox_rezim->ItemIndex, RD_klik_ico, empty_klik, true);
		scButton_zamek_RD->SetFocus(); // o�et�en� proti zm�n� dat p�i zam�en�m z�mku
		Nastav_M_R_Rx();
		input_clicked_icon=empty_klik_ico;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scButton_K_zamekClick(TObject *Sender)
{
		input_clicked_edit=empty_klik;
		input_clicked_icon=C_klik_ico;
		Nastav_zamky(scComboBox_rezim->ItemIndex, C_klik_ico, empty_klik, true);
		scButton_K_zamek->SetFocus(); // o�et�en� proti zm�n� dat p�i zam�en�m z�mku
		input_clicked_icon=empty_klik_ico;
}
//---------------------------------------------------------------------------
// p�i kliknut� na doporu�en� nastane aplikace doporu�en� do dan�ho editboxu
void __fastcall TForm_parametry::rHTMLLabel_InfoTextClick(TObject *Sender)
{
		//AnsiString relation_id=GetCurrentProcessId();

		switch(VID)
		{
			case -1:break;
			case 0:break;
			case 11:break;   //��dn� hodnota k vypln�n�
			case 12:break;   //��dn� hodnota k vypln�n�
			case 13:break;   //��dn� hodnota k vypln�n�
			case 14:break;   //��dn� hodnota k vypln�n�
			case 15:scGPNumericEdit_CT->Value=VID_value; break;//doporu�ene CT
			case 16:break;   //��dn� hodnota k vypln�n�
			case 21:    //doporuc kapacitu
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"21\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_kapacita->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_kapacita->Value=VID_value;
			}
			break;
			case 22:    //doporuc pozice
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"22\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_pozice->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_pozice->Value=VID_value;
			}
			break;
			case 23:break;   //��dn� hodnota k vypln�n�
			case 24:break;   //��dn� hodnota k vypln�n�
			case 25:break;   //��dn� hodnota k vypln�n�
			case 26:break;   //��dn� hodnota k vypln�n�
			case 27:    // doporu� rychlost pohonu
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"27\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_RD->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_RD->Value=VID_value;
			}
			break;
			case 28:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"28\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_mezera->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_mezera->Value=VID_value;
			} break;
			case 30:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"30\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_CT->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_CT->Value=VID_value;
			} break;
			case 31:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"31\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_delka_dopravniku->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_delka_dopravniku->Value=VID_value;
			} break;
			case 32:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"32\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_kapacita->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					 scGPNumericEdit_kapacita->Value=VID_value;
			}
			case 33:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"33\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_pozice->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_pozice->Value=VID_value;
			}
			break;
			case 40:break;   //��dn� hodnota k vypln�n�
			case 41:break;   //��dn� hodnota k vypln�n�
			case 42:break;   //��dn� hodnota k vypln�n�
			case 44:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"44\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_kapacita->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_kapacita->Value=VID_value;
			}
			break;
			case 45:
			{
//				Form1->FDQuery1->Active = False;
//				Form1->FDQuery1->Open("SELECT doporuc_hodnota FROM vid_validace WHERE VID=\"45\" AND username= \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\" ORDER BY id DESC LIMIT 1 ");
//				Form1->FDQuery1->Active = True;
//				scGPNumericEdit_pozice->Value=Form1->FDQuery1->Fields->Fields[0]->AsFloat;
					scGPNumericEdit_pozice->Value=VID_value;
			} break;

					case 281:
			{
					scGPNumericEdit_mezera_JIG->Value=VID_value;
			} break;

					case 282:
			{
					scGPNumericEdit_mezera_PODVOZEK->Value=VID_value;
			} break;
		}
}
//---------------------------------------------------------------------------
// kontrola vybran�ho pohonu v��i zadan� rychlosti dopravn�ku
void __fastcall TForm_parametry::scComboBox_pohonChange(TObject *Sender)
{
		ComboBoxPohonyProbihaZmena=true;//pou��v� se kv�li aktualizaci n�pln� comba
		INPUT();
		OUTPUT();
 		Pohon_pouzivan();
 		Nacti_rx(); //bu� vypo��t� rx nebo v p��pad� pou��van�ho pohonu rx a rz na�te z dat
 	 	Nastav_M_R_Rx();
		if(scComboBox_rezim->ItemIndex!=1)
		{
			scButton_zamek_CT->Enabled=true;
			scButton_zamek_DD->Enabled=true;
		}
		if(scComboBox_pohon->ItemIndex != 0 && scComboBox_rezim->ItemIndex==1)
		{   // POKUD je pohon ji� pou��v�n, nat�hnu si jeho data
				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
				if (obj!=NULL)
				{
				pohon_pouzivan=true;
					//RD
					double RD=obj->RD;
					if (RDunitT == MIN)RD *= 60.0;
					//if (RDunitD == MM) RD /= 1000.0;ji� se nepou��v�
					scGPNumericEdit_RD->Value=RD;

					//MEZERY
					if(DMunit==M)
					{
						scGPNumericEdit_mezera->Value=obj->mezera;
						scGPNumericEdit_mezera_JIG->Value=obj->mezera_jig;
						scGPNumericEdit_mezera_PODVOZEK->Value=obj->mezera_podvozek;
						//scGPNumericEdit1_rx->Value=obj->pohon->Rx; patrn� se dopo��t�v� formou nacti_rx
					}
					else
					{
						scGPNumericEdit_mezera->Value=obj->mezera*1000;
						scGPNumericEdit_mezera_JIG->Value=obj->mezera_jig*1000;
						scGPNumericEdit_mezera_PODVOZEK->Value=obj->mezera_podvozek*1000;
						//scGPNumericEdit1_rx->Value=obj->pohon->Rx*1000; patrn� se dopo��t�v� formou nacti_rx
					}

					if(obj->rotace==0) scComboBox_rotace->ItemIndex=0;
					else scComboBox_rotace->ItemIndex=1;

					if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // p�i p�echodu mezi pohony, zkontroluje zdali je mezera v po��dku, pouze u KK re�imu
				}
				else
				{
				  pohon_pouzivan=false;
					scButton_zamek_RD->Enabled=true;  // pokud pohon nen� pou��v�n povol�m zobrazen� z�mku RD
			 		if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // p�i p�echodu mezi pohony, zkontroluje zdali je mezera v po��dku, pouze u KK re�imu
				}
		}
		input_clicked_edit=empty_klik; // v�b�rem pohonu doch�z� k vol�n� onchange rotace a to zp�sob� zobrazen� pacek, co� je ne��douc�, proto p�i zm�n� pohonu v�dy nastav�m, �e jde o pr�zdn� klik
		//upozorn�n� na nep�i�azen� pohon
		if(scComboBox_pohon->ItemIndex==0){scComboBox_pohon->Options->FrameWidth=2;scComboBox_pohon->Options->FrameFocusedColor=clRed;scComboBox_pohon->Options->FrameNormalColor=clRed;}else {scComboBox_pohon->Options->FrameNormalColor=clGray;scComboBox_pohon->Options->FrameFocusedColor=clHighlight;scComboBox_pohon->Options->FrameWidth=1;}
		ComboBoxPohonyProbihaZmena=false;//pou��v� se kv�li aktualizaci n�pln� comba
}
//---------------------------------------------------------------------------
// doplnit koment��
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaClick(TObject *Sender)
{
		input_clicked_edit = C_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, C_klik, false);
}
// --------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_mezeraClick(TObject *Sender)
{
		input_clicked_edit = mezera_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico,mezera_klik, false);
}
//----------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_poziceClick(TObject *Sender)
{
		input_clicked_edit = P_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, P_klik, false);
}
//---------------------------------------------------------------------------
// doplnit koment��
void __fastcall TForm_parametry::scGPNumericEdit_CTClick(TObject *Sender)
{
		input_clicked_edit = CT_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, CT_klik, false);
}
//---------------------------------------------------------------------------
// doplnit koment��
void __fastcall TForm_parametry::scGPNumericEdit_RD_Click(TObject *Sender)
{
		input_clicked_edit = RD_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, RD_klik, false);
		Nastav_M_R_Rx();
}
//---------------------------------------------------------------------------
// doplnit koment��
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuClick
		(TObject *Sender)
{
		input_clicked_edit = DD_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, DD_klik, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_rxClick(TObject *Sender)
{
input_clicked_edit=Rx_klik;
Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, Rx_klik, false);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rotaceClick(TObject *Sender)
{
		if(form_zobrazen)
		{
			input_clicked_edit = Rotace_klik;
			//	if(RD_zamek==LOCKED && scButton_zamek_RD->Enabled) vypis("Byl odem�en z�mek rychlosti pohonu",false);
			Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico,Rotace_klik, false);
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Button_dopravnik_parametryClick(TObject *Sender)
{
		//!!!!!toto dostane mo�n� trochu jinou podobu podle nov� �vahy, pravd�podobn� za situace, �e bude n�m�t na p��d�n� nov�ho pohonu (bude vysv�ceno tla��tko na p�id�n� pohonu)
		bool priradit_posledni=false; bool aktualizace_combo_pohony=false;
		unsigned int n_pohonu=F->d.v.POHONY->predchozi->n;
		if(scComboBox_pohon->ItemIndex==0)priradit_posledni=true;//kdy� je�t� nebyl p�i�azen, jinak mu nevnucovat jin� pohon, aby mohlo tla��tko tak� d�le fungovat ke spr�v� pohon�

		//zobrazen� formu PL
		Form_parametry_linky->zobrazitFrameForm=true;
		//nem��e tady b�t toto!!!:Form1->Button_dopravnik_parametryClick(Sender) ale mus� b�t toto:
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Form_parametry->Visible=false;//nutnost

		Form_parametry_linky->ShowModal();//nelze pou��t if(mrOk==Form_parametry_linky->ShowModal())proto�e se pou��v� v PL formclosequery (a ta p�eb�j� mrOK) kv�li GAPO stornu, mus� se tedy pou��vat Form_parametry_linky->Ulozit
		if(Form_parametry_linky->Ulozit)aktualizace_combo_pohony=true;
		else priradit_posledni=false;//pokud bude stisknuto storno tak se ur�it� nep�i�azuje
		Form_parametry->Visible=true;//nutnost
		Form_parametry_linky->zobrazitFrameForm=false;

		if(F->d.v.POHONY->dalsi!=NULL)//pokud ji� existuje pohon
		{
			scComboBox_pohon->Enabled=true;//povolen� vyb�r�n� pohonu - mus� b�t samostatn�
			if(aktualizace_combo_pohony)//nebylo na PL stisknuto storno
			{
				Form1->aktualizace_combobox_pohony_v_PO(0,RDunitT);//zaktualizovat v�pis + o jednotky
				//POKUD NEBYL P�VODN� P�I�AZEN POHON, NEBYLO STISKNUTO STORNO, P�IBYL NOV� POHON A U�IVATEL S T�M SOUHLAS�
				if(priradit_posledni && n_pohonu<F->d.v.POHONY->predchozi->n && mrYes==F->MB("Chcete p�i�adit posledn� vytvo�en� pohon?",MB_YESNO))
				scComboBox_pohon->ItemIndex=Form1->d.v.POHONY->predchozi->n;//p���ad�me poslen� vytvo�en� pohon - to je trochu na zv�en�, zda mu ho takto podsouvat ale s MB asi OK
			}
		}
		else scComboBox_pohon->Enabled=false;//i tato v�tev m� v�znam, pokud by ji� nov� neexistoval

		//pokud je n�hled z PO zobrazen, zaji��uje z�rove� okam�itou aktualizaci hodnot v n�hledu z PO
		if(Form_objekt_nahled->Visible)Form_objekt_nahled->REFRESH_DATA();//obnov� dat ve formu Form_objekt_nahled v�etn� p�ekreslen�
}
//---------------------------------------------------------------------------
void TForm_parametry::INPUT()
{
		/////////////////// nacteni vsech hodnot z formulare do lokalnich promennych////////////////////////////////////////////////////
		short rezim;
		if (scComboBox_rezim->ItemIndex == 0)	rezim = 0; // S&G
		if (scComboBox_rezim->ItemIndex == 1) rezim = 1; // Kontinual
		if (scComboBox_rezim->ItemIndex == 2) rezim = 2; // Postprocesni

		double TT = Form1->d.v.PP.TT;
		double CT = scGPNumericEdit_CT->Value; // CT - nov� zad�no u�ivatelem
		double RD = scGPNumericEdit_RD->Value; // RD	od u�ivatele
		double DD = scGPNumericEdit_delka_dopravniku->Value; // DD od u�ivatele
		double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele
		double Odchylka = scGPNumericEdit_odchylka->Value; // odchylka od u�ivatele
		//short Nasleduje_cekani = scComboBox_cekani_palec->ItemIndex;// 0 - ne, 1 -ano, 2 - automaticky
		//short Stop_stanice = scComboBox_stopka->ItemIndex;// 0 - ne, 1 -ano, 2 - automaticky
		double P = scGPNumericEdit_pozice->Value; // po�et pozic
		double MJ = scGPNumericEdit_mezera_JIG->Value;
		double MP = scGPNumericEdit_mezera_PODVOZEK->Value;
		double Rz = scGPNumericEdit_rozestup->Value;
		double Rx = scGPNumericEdit_rx->Value;

		bool CT_locked;
		bool RD_locked;
		bool DD_locked;
		bool K_locked;

		double rotace;
		if (scComboBox_rotace->ItemIndex == 0)rotace = 0; // na d�lku
		if (scComboBox_rotace->ItemIndex == 1)rotace = 90; // na ���ku

		//////////////////////// stavy zamku/////////////////////////////////////
		if (CT_zamek == LOCKED)	CT_locked = true; else CT_locked = false;
		if (RD_zamek == LOCKED) RD_locked = true; else RD_locked = false;
		if (DD_zamek == LOCKED) DD_locked = true;	else DD_locked = false;
		if (K_zamek == LOCKED) 	K_locked = 	true; else K_locked =  false;

		//povol� nebo zak�e vstup do zm�ny rotace, mezery a Rx
		Nastav_M_R_Rx();

		//////////////////////// prevody jednotek///////////////////////////////
		if (CTunit == MIN) CT *= 60.0;// pokud bylo zad�n� v minut�ch p�evede na sekundy - jinak je CT v Si a mohu ho hned ulo�it k v�po�tu
		if (RDunitT == MIN)RD /= 60.0;//opravdu d�leno
		//if (RDunitD == MM) RD /= 1000.0;//ji� vyu��t� zru�eno, nicm�n� nevad� zanech�n�
		if (DDunit == MM)	 DD /= 1000.0; // v�dy ukl�d�m do metr�
		if (DMunit)
		{
				MJ = scGPNumericEdit_mezera_JIG->Value * 1000.0;
				MP = scGPNumericEdit_mezera_PODVOZEK->Value * 1000.0;
		}

		///////////////ulo�en� do v�po�etn�ho modulu PO/////////////////////////
		pm.rezim = rezim;
		pm.TT = TT;
		pm.CT = CT;
		pm.RD = RD;
		pm.DD = DD;
		pm.K = K;
		pm.M = getM();//p�vodn� bylo m
		pm.MJ	= MJ;
		pm.MP = MP;
		pm.P = P;
		pm.dJ = Form1->d.v.PP.delka_jig; // d�lka jigu
		pm.sJ = Form1->d.v.PP.sirka_jig; // ���ka jigu
		pm.dP = Form1->d.v.PP.delka_podvozek; // d�lka podvozku/zavesu
		pm.Rotace = rotace;
		pm.Rz = Rz;
		pm.Rx = Rx;
		pm.CT_locked = CT_locked;
		pm.RD_locked = RD_locked;
		pm.DD_locked = DD_locked;
		pm.K_locked = K_locked;
		//pohon
		Cvektory::TPohon *Pohon = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if (Pohon != NULL) pm.R = Pohon->roztec; else pm.R = 0;
		Pohon=NULL;delete Pohon;
}
//---------------------------------------------------------------------------
void TForm_parametry::OUTPUT()
{
		// pokud bylo zad�v�no nap�. z input_CT nebudu z math modelu tento �daj znovu natahovat do formu,
		// zp�sobuje to toti� p�episov�n� hodnot p�i zad�v�n� hodnoty do editboxu
		// takto jsou o�et�eny v�echny vstupy
		//!!!!Rz a Rx jsou nacitany pri volani metody Nacti_rx a nejsou soucasti OUTPUT resp. v PO_math jsou p�ipraveny, ale nevyu�ity a neotestov�ny!!!

		//TECHNOLOGICK� �AS
		if (input_state != CT)
		{
				scGPNumericEdit_CT->Value = pm.CT;
				if (CTunit == MIN)scGPNumericEdit_CT->Value /= 60.0;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_CT->Decimal=3;

		}
		/////RYCHLOST DOPRAVN�KU
		if(!pohon_pouzivan) // v p��pad�, �e v roletce pohonu vyberu pohon, kter� je ji� pou��v�n  nat�hnu data p�i onchange nikoliv tady
		{
				if (input_state != RD)
				{
					//	 Memo1->Lines->Add("RD_UnitT OUTPUT: "+AnsiString(RDunitT));
					//	 Memo1->Lines->Add("RD OUTPUT: "+AnsiString(pm.RD));
					//		 RDunitT=S;
						scGPNumericEdit_RD->Value = pm.RD;
						if (RDunitT == MIN) scGPNumericEdit_RD->Value *= 60.0;
						//if (RDunitD == MM) scGPNumericEdit_RD->Value *= 1000.0;
						if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_RD->Decimal=3;
				}
		}
		/////D�LKA OBJEKTU
		if (input_state != DD)
		{
				scGPNumericEdit_delka_dopravniku->Value = pm.DD;
				if (DDunit == MM)scGPNumericEdit_delka_dopravniku->Value *= 1000.0;
				scGPNumericEdit_delka_dopravniku->Hint=scGPNumericEdit_delka_dopravniku->Value;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_delka_dopravniku->Decimal=3;
		}
		/////KAPACITA
		if (input_state != K)
		{
				scGPNumericEdit_kapacita->Value = pm.K;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_kapacita->Decimal=3;
		}
		/////POZICE
		if (input_state != P)
		{
				scGPNumericEdit_pozice->Value = pm.P;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_pozice->Decimal=3;
		}
		/////MEZERY
		//mezera mezi voz�ky, kritick� mezera
		if(!pohon_pouzivan && form_zobrazen) // v p��pad�, �e v roletce pohonu vyberu pohon, kter� je ji� pou��v�n  nat�hnu data p�i onchange nikoliv tady
		{
				if(input_state != mezera)
				{
					scGPNumericEdit_mezera->Value = pm.M;
					if (DMunit == MM)scGPNumericEdit_mezera->Value *= 1000.0;
					if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_mezera->Decimal=3;
				}
				//mezera mezi jigy
				if(input_state != mezera_jig)
				{
					scGPNumericEdit_mezera_JIG->Value = pm.MJ;
					if (DMunit == MM)scGPNumericEdit_mezera_JIG->Value *= 1000.0;
					if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_mezera_JIG->Decimal=3;
				}
				//mezera mezi podvozky
				if(input_state != mezera_podvozek)
				{
					scGPNumericEdit_mezera_PODVOZEK->Value = pm.MP;
					if (DMunit == MM)scGPNumericEdit_mezera_PODVOZEK->Value *= 1000.0;
					if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_mezera_PODVOZEK->Decimal=3;
				}
	 }


		////ZOBRAZEN� HINT� + odfiltrov�n� velmi mal�ho ��sla, kter� n�kdy naplnilo hint
		// pokud je ��slo velmi mal� do hintu se zobraz� 0

		if(scGPNumericEdit_CT->Value < 0.0000000000000004)	scGPNumericEdit_CT->Hint=0;
		else 		scGPNumericEdit_CT->Hint=	scGPNumericEdit_CT->Value;

		if(scGPNumericEdit_RD->Value < 0.0000000000000004)	scGPNumericEdit_RD->Hint=0;
		else 	scGPNumericEdit_RD->Hint=scGPNumericEdit_RD->Value;

		if(scGPNumericEdit_delka_dopravniku->Value < 0.0000000000000004)	scGPNumericEdit_delka_dopravniku->Hint=0;
		else 	scGPNumericEdit_delka_dopravniku->Hint=scGPNumericEdit_delka_dopravniku->Value;

		if(scGPNumericEdit_kapacita->Value < 0.0000000000000004)	scGPNumericEdit_kapacita->Hint=0;
		else 	scGPNumericEdit_kapacita->Hint=scGPNumericEdit_kapacita->Value;

		if(scGPNumericEdit_odchylka->Value < 0.0000000000000004)	scGPNumericEdit_odchylka->Hint=0;
		else 	scGPNumericEdit_odchylka->Hint=scGPNumericEdit_odchylka->Value;

		if(scGPNumericEdit_mezera->Value < 0.0000000000000004)	scGPNumericEdit_mezera->Hint=0;
		else 	scGPNumericEdit_mezera->Hint=scGPNumericEdit_mezera->Value;

		if(scGPNumericEdit_mezera_JIG->Value < 0.0000000000000004)	scGPNumericEdit_mezera_JIG->Hint=0;
		else 	scGPNumericEdit_mezera_JIG->Hint=scGPNumericEdit_mezera_JIG->Value;

		if(scGPNumericEdit_mezera_PODVOZEK->Value < 0.0000000000000004)	scGPNumericEdit_mezera_PODVOZEK->Hint=0;
		else 	scGPNumericEdit_mezera_PODVOZEK->Hint=scGPNumericEdit_mezera_PODVOZEK->Value;

		if(scGPNumericEdit_pozice->Value < 0.0000000000000004)	scGPNumericEdit_pozice->Hint=0;
		else 	scGPNumericEdit_pozice->Hint=scGPNumericEdit_pozice->Value;

		VALIDACE();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPGlyphButton_PO_text_memoClick
		(TObject *Sender)
{
		//formul�� na st�ed
		Form_poznamky->Left = F->ClientWidth / 2 - Form_poznamky->Width / 2;
		Form_poznamky->Top = F->ClientHeight / 2 - Form_poznamky->Height / 2;
		//zobrazeni formul��e
		Form_poznamky->scGPMemo->Text=poznamka;
		MT=0;//prozatim
		scGPButton_OK->Enabled=false;scGPButton_storno->Enabled=false;Konec->Enabled=false;//zdisablovn� spodn�ch buttonu
		this->Color=F->m.clIntensive(this->Color,8);//zesv�tlen� spodn�ho formu
		//Visible=false;
		if(Form_poznamky->ShowModal()==mrOk)
		{
			poznamka=Form_poznamky->scGPMemo->Text;
			MT;
		}
		scGPButton_storno->Enabled=true;Konec->Enabled=true;//povolen� storna a k��ku
		this->Color=F->m.clIntensive(this->Color,-8);//navr�cen� do p�vodn� hodnoty
		//Visible=true;
		VALIDACE();//kontrola, �e je v�e OK + navr�cen� tla��tka OK do p�edchoz�ho stavu
}
//---------------------------------------------------------------------------
//ud�lost zaji��uj�c� rotaci jigu, p�i kliknut� do roletky rotace
//pozn. o�et�en� zda mo�no rotovat �i nikoliv �e�t� Ros�a extra metoudou na �rovni hl�d�n� zam�en�ho �i odem�en�ho z�mku RD resp. roletky rotace (ta je mo�n� pokud se voz�ky vejdou i p�i zam�en�m RD, tzn. beze zm�ny RD, Rz a Rx, pouze zm�nou M)
//tzn. nesleduj�c� metoda u� �e�� jen rotaci rotovateln�ho (tzn. pokud je zam�en RD, tak zda nem� rotace vliv na RD, �i RD odem�en),  a v�po�et souvesej�ch paramater�
void __fastcall TForm_parametry::scComboBox_rotaceChange(TObject *Sender)
{
		if (input_state == NOTHING && form_zobrazen)//o�et�en� pouze kv�li formshow
		{
			INPUT();//nahr�n� hodnot z PO pro v�po�etn� model, v�etn� stavu ROTACE do pm.Rotace
			//dodate�n� dosazen� (suplov�n� INPUT() pomoc� pm.M) aktu�ln� nov� �i pop�. staronov� mezery, mus� b�t a� za samotn�m INPUT
			//37 pouze pojistka podminky, kdyby nekde nesedel korektne stav zamku tak imageindex je vzdy OK
			if(RD_zamek == LOCKED || scButton_zamek_RD->ImageIndex == 37)//pokud je z�mek RD zam�en� a lze m�nit rotaci jedn� se o situaci, kdy se m�n� M a nem�n� se RD, Rz a Rx
			{
				pm.M=F->m.mezera(pm.Rotace,pm.Rz);//jedn� se o situaci, kdy se m�n� M a nem�n� se RD, Rz a Rx
			}
			else//odem�en� z�mek, tud� m��u zm�nit RD a t�m p�dem M, Rz,Rx. M tedy hled�m doporu�enou nejmen�� (posledn� parametr=0 nebo� nebli���) dle rozhodnut� u�ivatele pomoc� MB, MB pouze nen� zobrazen, pokud nem� smysl zobrazovat
			{
				if(F->m.mezera_mezi_voziky(pm.dJ,pm.sJ,pm.Rotace,pm.R,F->m.mezera(pm.Rotace,pm.Rz))==F->m.mezera_mezi_voziky(pm.dJ,pm.sJ,pm.Rotace,pm.R,0))//pokud nem� cenu MB zobrazovat, jedn� se stejn� o nejmen�� mo�nou mezeru, tak ji pou�ije rovnou bez zobrazen� MB
					pm.M=F->m.mezera_mezi_voziky(pm.dJ,pm.sJ,pm.Rotace,pm.R,0);//po rotaci je nov� nejmen�� mo�n� mezera ta se mus� aplikovat do nov�ho v�po�tu ostatn�ch parametr�
				else
				{
					if(mrYes==F->MB("Chcete zachovat co nejbli��� mo�n� rozestup v��i aktu�ln�mu nastaven�? Pokud ne, bude nalezen nejmen�� mo�n� rozestup.",MB_YESNO))
						pm.M=F->m.mezera_mezi_voziky(pm.dJ,pm.sJ,pm.Rotace,pm.R,F->m.mezera(pm.Rotace,pm.Rz));//bude nalezena nejbli��� mo�n� dle p�vodn�ho Rz
					else
						pm.M=F->m.mezera_mezi_voziky(pm.dJ,pm.sJ,pm.Rotace,pm.R,0);//po rotaci je nov� nejmen�� mo�n� mezera ta se mus� aplikovat do nov�ho v�po�tu ostatn�ch parametr�
				}
			}
			pm.input_M();//v�po�et ostatn�ch parametr� z nov� �i p��padn� staronov� mezery
			OUTPUT();//n�vrat do PO po v�po�tu
			Nacti_rx();//aktualizace hodnot Rz, Rx
		}
		ComboRotace_predchozi_stav=scComboBox_rotace->ItemIndex;//ulo�� p�edchoz� stav pro dal�� pou�it�
}
//---------------------------------------------------------------------------
void TForm_parametry::Nastav_zamky(double rezim, Tinput_clicked_icon I,Tinput_clicked_edit E, bool ikonka)
{
		if (rezim == 1)
		{ // KK re�im
			 //	scButton_K_zamek->Visible = false;
				if (ikonka)
				{ // pokud jde o klik na ikonu
						if (I == CT_klik_ico)
					{ // pokud je kliknuto na ikonu CT
										if (CT_zamek == LOCKED) // kdy� je zam�eno
										{

												// CT
												scButton_zamek_CT->ImageIndex = 38;
												CT_zamek = UNLOCKED;
												set (TIME,ENABLED,false);
												// RD
												scButton_zamek_RD->ImageIndex = 37;
												RD_zamek = LOCKED;
												set (RYCHLOST,DISABLED,false);
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
												set (DELKA,ENABLED,false);



										}
										else // odem�eno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 37;
												CT_zamek = LOCKED;
												set (TIME,DISABLED,false);
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												set (RYCHLOST,ENABLED,false);
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
												set (DELKA,ENABLED,false);
										}

										if (scButton_K_zamek->Visible==true)
										{
													//scButton_K_zamek->Visible = true;
													scButton_K_zamek->ImageIndex=scButton_zamek_CT->ImageIndex;
													scGPNumericEdit_kapacita->Enabled=scGPNumericEdit_CT->Enabled;
										}

					}
						if (I == RD_klik_ico)
						{
							scButton_K_zamek->Visible = false;
								if (RD_zamek == LOCKED) // kdy� je zam�eno   - SET RD m� svoj� metodu
								{
										// RD
										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										set (RYCHLOST,ENABLED,false);
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										set (TIME,ENABLED,false);
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;
										set (DELKA,DISABLED,false);

										scComboBox_rotace->Items->Items[0]->Enabled = true;
										scComboBox_rotace->Items->Items[1]->Enabled = true;

											if(scComboBox_pohon->ItemIndex>=0 && 	RD_zamek == UNLOCKED)
											{
											set(ROZESTUP, ENABLED, false);
											} else  set(ROZESTUP, READONLY, false);
								}
								else // odem�eno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
										set (RYCHLOST,DISABLED,false);
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										set (TIME,ENABLED,false);
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;
										set (DELKA,ENABLED,false);

										if(scComboBox_pohon->ItemIndex==0 && 	RD_zamek == UNLOCKED)
											{
											set(ROZESTUP, ENABLED, false);
											} else  set(ROZESTUP, READONLY, false);

									 //	scComboBox_rotace->Items->Items[0]->Enabled = false;
									//	scComboBox_rotace->Items->Items[1]->Enabled = false;
								}
						}

						if (I == DD_klik_ico)
						{    scButton_K_zamek->Visible = false;
								if (DD_zamek == LOCKED) // kdy� je zam�eno
								{
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;
                    set (DELKA,ENABLED,false);

										scButton_zamek_CT->ImageIndex = 37;
										CT_zamek = LOCKED;
										set (TIME,DISABLED,false);

										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										set (RYCHLOST,ENABLED,false);
										// set(DELKA,ENABLED,false);
								}
								else // odem�eno
								{
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;
                    set (DELKA,DISABLED,false);
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										set (TIME,ENABLED,false);
										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										set (RYCHLOST,ENABLED,false);
										// set(DELKA,DISABLED,false);
								}
								 if(RD_zamek==UNLOCKED) { 	set(MEZERA,ENABLED,false); scGPNumericEdit_rx->Enabled=true;}
								 else { set(MEZERA,DISABLED,false); scGPNumericEdit_rx->Enabled=false;}
						}

								if (I ==	C_klik_ico)
								{     scButton_K_zamek->Visible = true;
											if (CT_zamek == LOCKED) // kdy� je zam�eno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 38;
												CT_zamek = UNLOCKED;
												set(TIME,ENABLED,false);
													// KAPACITA
												scButton_K_zamek->ImageIndex = 38;
												K_zamek = UNLOCKED;
										 //		set(KAPACITA,ENABLED,false);  ne�lo mi nastavit p�es SET
										 scGPNumericEdit_kapacita->Enabled=true;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												set(RYCHLOST,ENABLED,false);
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 37;
												DD_zamek = LOCKED;
												set(DELKA,DISABLED,false);
										}
										else // odem�eno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 37;
												CT_zamek = LOCKED;
												set(TIME,DISABLED,false);
													// KAPACITA
												scButton_K_zamek->ImageIndex = 37;
												K_zamek =  LOCKED;
										 //		set(KAPACITA,DISABLED,false); ne�lo mi nastavit p�es SET
										 scGPNumericEdit_kapacita->Enabled=false;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												set(RYCHLOST,ENABLED,false);
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
												set(DELKA,ENABLED,false);
										}

								}

				}
				if (!ikonka) {
						///////KLIKNUTO DO EDITU///////////// KK re�im

						if (E == CT_klik) {
								 scButton_K_zamek->Visible = false;
								 scGPNumericEdit_kapacita->Enabled=true;
								if (CT_zamek == LOCKED) // kdy� je zam�eno
								{
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
											set(TIME,ENABLED,false);
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
											set(RYCHLOST,DISABLED,false);
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;
											set(DELKA,ENABLED,false);
								}
						}
						if (E == RD_klik) {
								scButton_K_zamek->Visible = false;
								scGPNumericEdit_kapacita->Enabled=true;
								if (RD_zamek == LOCKED) // kdy� je zam�eno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;
								}
						}

						if (E == DD_klik) {
							scButton_K_zamek->Visible = false;
							scGPNumericEdit_kapacita->Enabled=true;
								if (DD_zamek == LOCKED) // kdy� je zam�eno
								{
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;

										scButton_zamek_CT->ImageIndex = 37;
										CT_zamek = LOCKED;

										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
								}
						}
						if (E == mezera_klik)
						{
							scButton_K_zamek->Visible = true;
						 if(CT_zamek==UNLOCKED) scGPNumericEdit_kapacita->Enabled=true;
						 else  scGPNumericEdit_kapacita->Enabled=false;
							scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
								if(RD_zamek==LOCKED && scButton_zamek_RD->Enabled)  //tohle mysl�m u� nen� pot�eba
								{
//
//								 RD_zamek=UNLOCKED;
//								 scButton_zamek_RD->ImageIndex = 38;
//								 set(RYCHLOST,ENABLED,false);
//
//								 scButton_zamek_DD->ImageIndex = 37;
//								 DD_zamek = LOCKED;
//								 set(DELKA,DISABLED,false);
//
//								 CT_zamek=UNLOCKED;
//								 scButton_zamek_CT->ImageIndex = 38;
//								 set(TIME,ENABLED,false);
//
//								 vypis("Byl odem�en z�mek rychlosti pohonu",false);
								}

								if(scButton_K_zamek->Visible==false)
								{
								 scButton_K_zamek->Visible=true;
								 scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;

								}

						}

						if(E == Rx_klik)
						{
							scButton_K_zamek->Visible = true;
							if(CT_zamek==UNLOCKED) scGPNumericEdit_kapacita->Enabled=true;
							else  scGPNumericEdit_kapacita->Enabled=false;
							scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
							if(RD_zamek==LOCKED && scButton_zamek_RD->Enabled) // pokud je RD zamcen a zaroven povolen k editace
							{
//                 //tohle se asi u� ned�je - editace je povolena pouze kdy� je RD odem�en
//								 RD_zamek=UNLOCKED;
//								 scButton_zamek_RD->ImageIndex = 38;
//								 set(RYCHLOST,ENABLED,false);
//
//								 scButton_zamek_DD->ImageIndex = 37;
//								 DD_zamek = LOCKED;
//								 set(DELKA,DISABLED,false);
//
//								 CT_zamek=UNLOCKED;
//								 scButton_zamek_CT->ImageIndex = 38;
//								 set(TIME,ENABLED,false);
//
//								 vypis("Byl odem�en z�mek rychlosti pohonu",false);
							}

							if(!scButton_K_zamek->Visible)
							{
								 scButton_K_zamek->Visible=true;
								 scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
							}
						}

						if (E == C_klik || E == P_klik)
						{
								scButton_K_zamek->Visible = false;
								scGPNumericEdit_kapacita->Enabled=true;

								if(scGPNumericEdit_CT->Value>=Form1->d.v.PP.TT)
								{  //CT je vetsi nez TT

										if(CT_zamek==LOCKED)
										{
										 CT_zamek=UNLOCKED;
										 scButton_zamek_CT->ImageIndex = 38;
										 set(TIME,ENABLED,false);

										 scButton_zamek_RD->ImageIndex = 37;
										 RD_zamek = LOCKED;
										 set(RYCHLOST,DISABLED,false);

											scButton_zamek_DD->ImageIndex = 38;
											DD_zamek = UNLOCKED;
											set(DELKA,ENABLED,false);
										}
								}
						}
						if (E == Rotace_klik)
						{
						scButton_K_zamek->Visible = false;
						scGPNumericEdit_kapacita->Enabled=true;
						}
				}
		}
		//////KONEC KK re�imu//////////////////////////////////////////////////
		if (rezim == 2)// PP re�im
		{
				if (E == mezera_klik && scGPNumericEdit_mezera->Value >= 0)
				{


						scButton_zamek_DD->Visible = true;
						scButton_zamek_DD->Enabled = true;
					 //	scButton_zamek_DD->ImageIndex = 38;
					//	DD_zamek=UNLOCKED;
				 //		scButton_K_zamek->ImageIndex = 37;
				 //   K_zamek=LOCKED;
						scButton_K_zamek->Visible = true;
						if(DD_zamek==LOCKED)
						{
					 //	 set(DELKA,DISABLED,false); //ne�lo p�es set nastavit
						 scGPNumericEdit_kapacita->Enabled=true;
						 scGPNumericEdit_delka_dopravniku->Enabled=false;

						 }
						else if(K_zamek == LOCKED)
						{
					 //	 set(DELKA,ENABLED,false);
						 scGPNumericEdit_kapacita->Enabled=false;
						 scButton_K_zamek->ImageIndex = 37;
						 scGPNumericEdit_delka_dopravniku->Enabled=true;
						}
				}

				if (I == C_klik_ico)
				{
						if (scButton_K_zamek->ImageIndex==37  /*K_zamek == LOCKED*/) // kdy� je zam�eno - pojistka
						{
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;
							 //	set(KAPACITA,ENABLED,false);
							 scGPNumericEdit_kapacita->Enabled=true;

								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
							//	set(DELKA,DISABLED,false);
								scGPNumericEdit_delka_dopravniku->Enabled=false;

						}
						else // odem�eno
						{
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;
								//	set(KAPACITA,DISABLED,false);
								 scGPNumericEdit_kapacita->Enabled=false;

								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
							//	set(DELKA,ENABLED,false);
							scGPNumericEdit_delka_dopravniku->Enabled=true;
						}
				}
				if (I == DD_klik_ico)
				{
						if (DD_zamek == LOCKED) // kdy� je zam�eno
						{
								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
								//set(DELKA,ENABLED,false);
								scGPNumericEdit_delka_dopravniku->Enabled=true;
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;
							 //	set(KAPACITA,DISABLED,false);
								scGPNumericEdit_kapacita->Enabled=false;
						}
						else // odem�eno
						{
								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
							//	set(DELKA,DISABLED,false);
								scGPNumericEdit_delka_dopravniku->Enabled=false;
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;
								scGPNumericEdit_kapacita->Enabled=true;
						}
				}
				if (I == CT_klik_ico)
				{
						if (CT_zamek == LOCKED) // kdy� je zam�eno
						{
								scButton_zamek_CT->ImageIndex = 38;
								CT_zamek = UNLOCKED;
								//set(TIME,ENABLED,false);
								scGPNumericEdit_CT->Enabled=true;


						}
						else // odem�eno
						{
								scButton_zamek_CT->ImageIndex = 37;
								CT_zamek = LOCKED;
							 //	set(TIME,DISABLED,false);
								scGPNumericEdit_CT->Enabled=false;
						}
				}
							//PP re�im
				if (E == CT_klik)
				{
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;

						 double CT=0.0;
						if (CTunit==MIN) {
								CT=scGPNumericEdit_CT->Value*60.0;
						} else CT=scGPNumericEdit_CT->Value;

						if(CT < F->d.v.PP.TT) {
						scButton_zamek_CT->Visible = true;
						scGPNumericEdit_CT->Enabled=true;
            scButton_zamek_CT->ImageIndex = 38;
						}  else

						{

             	scButton_zamek_CT->Visible = false;
						}

						scGPNumericEdit_delka_dopravniku->Enabled=true;
						scGPNumericEdit_kapacita->Enabled=true;
						scGPNumericEdit_CT->Enabled=true;

				}
				if (E == RD_klik)
				{
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;
						scButton_zamek_CT->Visible = false;

						scGPNumericEdit_delka_dopravniku->Enabled=true;
						scGPNumericEdit_kapacita->Enabled=true;
						scGPNumericEdit_CT->Enabled=true;
				}
				if (E == DD_klik)
				{
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;
						scButton_zamek_CT->Visible = false;

						scGPNumericEdit_delka_dopravniku->Enabled=true;
						scGPNumericEdit_kapacita->Enabled=true;
						scGPNumericEdit_CT->Enabled=true;
				}
				if (E == P_klik)
				{
					 //	scButton_zamek_DD->Visible = false;
						//scButton_K_zamek->Visible = false;
						//scButton_zamek_CT->Visible = false;
				}
				if (E == C_klik ||E == P_klik)
				{
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;

						scGPNumericEdit_delka_dopravniku->Enabled=true;
						scGPNumericEdit_kapacita->Enabled=true;

						if(scGPNumericEdit_CT->Value< Form1->d.v.PP.TT && Form1->pom->n > 1)
						{
						 scButton_zamek_CT->Visible = true;
						 scButton_zamek_CT->Enabled = true;
						 if(CT_zamek == LOCKED) 	scGPNumericEdit_CT->Enabled=false;
						 else  	scGPNumericEdit_CT->Enabled=true;

						}
						else
						{
							scButton_zamek_CT->Visible = false;
							scGPNumericEdit_CT->Enabled=true;
						}
				}
				if (E == mezera_klik)
				{
						scButton_zamek_CT->Visible = false;
						scGPNumericEdit_CT->Enabled=true;
				}
		}
		Invalidate();//kv�li v�em pack�m + zaji�tuje z�rove� vol�n� frameCorrelation()//stejnou barvou or�muje hodnoty v korelaci + vykresl� korela�n� packy
		FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
////metoda kter� kontroluje zdali je vybran� pohon pou��v�n �i nikoliv, dle toho nastavuje viditelnost editbox�
void TForm_parametry::Pohon_pouzivan()
{
		if (scComboBox_rezim->ItemIndex == 1)
		{ // pro KK re�im - nastaven�
				Cvektory::TPohon *pohon = Form1->d.v.POHONY->dalsi;
				if(scComboBox_pohon->ItemIndex==0)
				{
					 // pokud je pohon nedefinov�n nebo nep�i�azen, v�dy povol�m rotaci a nastavit d�lku nebo ���ku
					 scComboBox_rotace->Items->Items[0]->Enabled = true;
					 scComboBox_rotace->Items->Items[1]->Enabled = true;
					 set(ROTACE,ENABLED, false);
				}
				if (Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1))
				{
					 RD_zamek = LOCKED; // pohon je ji� pou�iv�n - nemohu h�bat RD
					 CT_zamek = UNLOCKED;
					 DD_zamek = UNLOCKED;

					 scButton_zamek_CT->Enabled=false;
					 scButton_zamek_DD->Enabled=false;
					 scButton_zamek_RD->Enabled=false;

					 scButton_zamek_RD->ImageIndex = 37;
					 scButton_zamek_CT->ImageIndex = 38;
					 scButton_zamek_DD->ImageIndex = 38;

					 set(RYCHLOST, DISABLED, false);
					 set(MEZERA, DISABLED, false);
					 set(TIME,ENABLED,false);
					 set(DELKA,ENABLED,false);
					 //	rz ze vzoru
					if(scButton_zamek_RD->Enabled==false)
					{
						double roztec=0.0;
						Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
						if (P != NULL) roztec=P->roztec;  else  roztec=0.0;

						double rotace=0.0;
						if (scComboBox_rotace->ItemIndex==0) rotace=10; else rotace=0.0;  //potenci�ln� hodnota pro v�po�et Rz

						double mezera=0.0;
						Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
						if (obj!=NULL)   mezera=obj->mezera; else mezera = 0.0;

//n�sleduj�c� 4 ��dky zakomentoval 15.5. M
//						double M = Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,roztec,mezera);
//						double Rz_potencial =	Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,M);  //rotace je zde obracena nezli je nastaveno v editu
//						double Rz_akt =  Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex,obj->mezera);
//						if(Rz_potencial == Rz_akt)
						if(F->m.lze_rotovat_jig_bez_zmeny_RzRxRD(mezera,obj->rotace))//p�idal M
						{
							//if podminka splnena - povolim zmenu orientace
							scComboBox_rotace->Items->Items[0]->Enabled = true;
							scComboBox_rotace->Items->Items[1]->Enabled = true;
							set(ROTACE,ENABLED, false);
						}
						else
						{
								if(scComboBox_pohon->ItemIndex!=0)//pohon p�i�a�en
								{
							 //	scComboBox_rotace->Items->Items[0]->Enabled = false;
							 //	scComboBox_rotace->Items->Items[1]->Enabled = false;

									set(ROTACE,DISABLED, false);
								}
						}

					}
					//set(ROTACE, DISABLED, false);//zakomentoval 15.5. M toto tu podle m� nem� b�t
					set(ROZESTUP, READONLY, false);
					scGPNumericEdit_rx->Enabled=false;
				}
				else
				{
					if (input_state == NOTHING || input_state==NO)
							// pokud jde o prvn� zobrazen� formu - v�dy zobraz�m takto z�mky
					{
							RD_zamek = LOCKED;
							scButton_zamek_RD->ImageIndex = 37;
						//	Memo1->Lines->Add("zamkni");
					}
					else // mus� zde b�t i else v�tev - jinak p�i odp�i�azen� pohonu nastane situace, �e z�stanou 2 z�mky uzam�eny, co� je �patn�
					{
							RD_zamek = UNLOCKED;
							scButton_zamek_RD->ImageIndex = 38;
						//	Memo1->Lines->Add("odemkni");
					}
					set(RYCHLOST, ENABLED, false);
					// pohon nen� pou��v�n jin�m objektem, dovol�m zm�nu RD, M,R
					set(TIME,ENABLED,false);
					set(DELKA,ENABLED,false);
					set(MEZERA, ENABLED, false);
					set(ROZESTUP, READONLY, false);
					if(scComboBox_pohon->ItemIndex==0 && 	RD_zamek == UNLOCKED)
					{
					set(ROZESTUP, ENABLED, false);
					} else  set(ROZESTUP, READONLY, false);
					set(ROTACE, ENABLED, false);
					scGPNumericEdit_rx->Enabled=true;
					scButton_zamek_CT->Enabled=true;
					scButton_zamek_DD->Enabled=true;
					scButton_zamek_RD->Enabled=true;

					CT_zamek = UNLOCKED;
					DD_zamek = UNLOCKED;
					scButton_zamek_DD->ImageIndex = 38;
					scButton_zamek_CT->ImageIndex = 38;


				}
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rotaceEnter(TObject *Sender)
{
		input_clicked_edit = Rotace_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, Rotace_klik, false);
		if (scComboBox_rezim->ItemIndex == 2)
		{ // PP re�im schov�n� z�mk�
				scButton_zamek_DD->Visible = false;
				scButton_K_zamek->Visible = false;
		}

}
//---------------------------------------------------------------------------
//
double TForm_parametry::Kontrola_mezery()
{
		double doporuc_mezera=0.0;
		AnsiString jednotky=0.0;
		double mezera=0.0;
		if (DMunit == MM) mezera=scGPNumericEdit_mezera->Value/1000.0;
		else  						mezera=scGPNumericEdit_mezera->Value;
		Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if (P != NULL)
		{
				if (P->roztec > 0) // pokud existuje rozte�
				{
						//v�dy vr�t�m v metrech  a taky mezeru v�dy p�ed�v�m v metrech
						double typ=0;
						if(input_state == mezera_jig     && 	input_clicked_edit == mezera_klik) typ=1;
						if(input_state == mezera_podvozek && 	input_clicked_edit == mezera_klik) typ=2;

						doporuc_mezera = Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_jig, Form1->d.v.PP.sirka_jig, scComboBox_rotace->ItemIndex,P->roztec,mezera,typ);

						if(DMunit == MM)
						{
							 doporuc_mezera=doporuc_mezera*1000.0;
							 jednotky=" mm";
						}
						else
						{
							 jednotky=" m";
						}

						if(input_state == mezera_jig)
						{
							vypis("Doporu�en� mezera mezi jigy: <u>"+AnsiString(doporuc_mezera) + jednotky +"</u>");
							VID=281;
							VID_value=doporuc_mezera;
						}

						if(input_state == mezera_podvozek)
						{
							vypis("Doporu�en� mezera mezi podvozky: <u>"+AnsiString(doporuc_mezera) + jednotky +"</u>");
							VID=282;
							VID_value=doporuc_mezera;
						}

					//	vypis("Doporu�en� mezera: <u>"+AnsiString(doporuc_mezera) + jednotky +"</u>");
					//	VID=28;
				 //		VID_value=doporuc_mezera;
//						AnsiString relation_id=GetCurrentProcessId();
//						AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(doporuc_hodnota)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
//					  AnsiString strSQL = "UPDATE vid_validace set doporuc_hodnota = \""+AnsiString(doporuc_mezera)+"\",username = \""+F->get_user_name()+"\" ,relation_id= \""+relation_id+"\"    WHERE VID = \""+VID+"\"";
					 //	Form1->FDConnection1->ExecSQL(strSQL);

						if (Form1->ms.MyToDouble(Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_jig, Form1->d.v.PP.sirka_jig, scComboBox_rotace->ItemIndex,P->roztec,mezera)) == Form1->ms.MyToDouble(mezera))
						{
							vypis("",false);
							scGPButton_OK->Enabled = true;
							VID=-1;
						}
						else if (doporuc_mezera>0)

						{
             // mezera mezi jigy je mensi nebo rovno
              if(scGPNumericEdit_mezera_JIG<=scGPNumericEdit_mezera_PODVOZEK)
              {
              vypis("Doporu�en� mezera mezi jigy: <u>"+AnsiString(doporuc_mezera) + jednotky +"</u>");
							VID=281;
							VID_value=doporuc_mezera;

              }else //mezera mezi podvozky je mensi
              {
              vypis("Doporu�en� mezera mezi podvozky: <u>"+AnsiString(doporuc_mezera) + jednotky +"</u>");
							VID=282;
							VID_value=doporuc_mezera;
              }
              	scGPButton_OK->Enabled = false;

						}
				}
				else
				{
				  scGPButton_OK->Enabled = true; VID=-1;
				}
		}
		return doporuc_mezera;
}
//---------------------------------------------------------------------------
void TForm_parametry::Check_rozmezi_RD()
{
			vypis("",false);
			bool  mimo_rozmezi = false;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) {
						double jednotky_cas_pohon = 60.0;
						if (Form_parametry->RDunitT == Form_parametry->MIN) jednotky_cas_pohon = 60.0;
						else jednotky_cas_pohon = 1.0;
		 //				if (scGPNumericEdit_RD->Value < Form1->ms.MyToDouble(P->rychlost_od * jednotky_cas_pohon) || scGPNumericEdit_RD->Value > Form1->ms.MyToDouble(P->rychlost_do * jednotky_cas_pohon)) // nespl�uje rozmez�
		 // n�hrada p�vodn�ho IF nov�m - chov� se jako klasick� between porovn�n�
		 double RD   = scGPNumericEdit_RD->Value;
		 double P_od = P->rychlost_od*jednotky_cas_pohon;
		 double P_do = P->rychlost_do*jednotky_cas_pohon;


					//if ((unsigned)(RD - Form1->ms.MyToDouble(P_od)) <= (Form1->ms.MyToDouble(P_do) - Form1->ms.MyToDouble(P_od)))
					//if(RD >= P_od && RD <= P_do)
					if(Form1->m.between(RD,P_od,P_do))
						{
						 //	Memo1->Lines->Add("OK rozmezi");
						;
						}
						else
						{

							if(scGPNumericEdit_RD->Value>P->rychlost_do)  VID=25;
							if(scGPNumericEdit_RD->Value<P->rychlost_od)  VID=26;
								 mimo_rozmezi=true;
									//Memo1->Lines->Add("mimo rozmezi");
						}
				}
			double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN)RD /= 60.0;
				//if (RDunitD == MM) RD *= 1000.0;

	 if(scComboBox_rezim->ItemIndex==1) //hl�d�n� pouze u KK re�imu
	 {
		if (scGPNumericEdit_RD->Value > 0)
				// nutn� o�et�en� pro obdob� zad�v�n�/psan�
		{
				double roztec=0;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0;}
				//if (RDunitD == MM) dopRD /= 1000.0;

				if(mimo_rozmezi==true)
				{
					vypis("Rychlost neodpov�d� rozmez�!");
					VID=25;
				}
				if(Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
					vypis("Rychlost neodpov�d� rozmez�!");
					VID=25;
				}
				if(scComboBox_pohon->ItemIndex!=0 && roztec>0 && Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi==false)
				{
         if(scButton_zamek_RD->Enabled) //doporu�uji rychlost, pouze tehdy pokud lze RD m�nit (pohon nen� jinde pou��v�n)
            {
							double doporuc_hodnota = dopRD;
							vypis("Zadejte doporu�enou rychlost pohonu: <u>"+AnsiString(doporuc_hodnota)+"</u>");
              VID=27;
              VID_value=doporuc_hodnota;
             //	AnsiString relation_id=GetCurrentProcessId();
             //	AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(doporuc_hodnota)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
            //	Form1->FDConnection1->ExecSQL(strSQL);
            //	Form1->IBQuery1->SQL->Add(strSQL);
            //	Form1->IBQuery1->Open();
            }
        }
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
						vypis("Rychlost neodpov�d� rozmez�!");
						VID=25;
				}
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi==false)
				{
						vypis("");
				}
		}
		else vypis("Neplatn� hodnota rychlosti pohonu!");

	 }
}
//-------------------------------------------------------------------------------------------------------------------------------
 void TForm_parametry::Nacti_rx()
 {
		double roztec=0;
		double rx=0;
		double mezera=0;
		double rz=0;
		double rotace=scComboBox_rotace->ItemIndex;

		if (DMunit == MM) mezera=scGPNumericEdit_mezera->Value/1000.0;
		else  mezera=scGPNumericEdit_mezera->Value;
				 //v�dy p�ed�m do metody v metrech

		Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if (P != NULL) roztec=P->roztec;  else  roztec=0;

		if (P != NULL)
		{
			scGPNumericEdit_rx->ReadOnly=false;
			rx=Form1->m.Rx(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,mezera,roztec);
		}
		else
		{
			scGPNumericEdit_rx->ReadOnly=true;
			scGPNumericEdit_rx->Enabled=false;
		}

  	Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
		if (obj!=NULL)
		{
			scGPNumericEdit_rx->Value=obj->pohon->Rx;
			scGPNumericEdit_rozestup->Value=obj->pohon->Rz;
			Memo1->Lines->Add(scGPNumericEdit_rx->Value);
			Memo1->Lines->Add(scGPNumericEdit_rozestup->Value);
		}    //pohon nen� pou��v�n
		else
		{
			scGPNumericEdit_rx->Value =rx;//M 5. kv�tna 2018 p�esunuto sem - d�lalo n�e probl�my pokud bylo za Rz, proto�e se vol� je�t� duplicitn� v�po�et Rz p�i onclick do Rx
			rz = Form1->m.Rz(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,mezera);
			if(!input_state_Rz)   {  // Rz v tomto p��pad� nebudu plnit daty
			if(DMunit == MM) scGPNumericEdit_rozestup->Value=rz*1000;
			else        		 scGPNumericEdit_rozestup->Value=rz;
			}
			//scGPNumericEdit1_rx->Value =rx; M 5. kv�tna 2018 p�esunuto v��e - d�lalo probl�my za Rz, proto�e se vol� je�t� duplicitn� v�po�et Rz p�i onclick do Rx
		}
		scGPNumericEdit_rx->Hint="tj. ka�d� " +AnsiString(rx)+ " palec zachyt�v�.";
 }
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_rxChange(TObject *Sender)
{
	double  mezer=0;
	if (DMunit == MM) // pokud je v milimetrech, tak p�epne na metry
	{
						mezer=scGPNumericEdit_mezera->Value/1000;
	} else	  mezer=scGPNumericEdit_mezera->Value;

	double roztec=0;
	double delka=0;
	double rotace=0;
	double rx=0;
	Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
	if (P != NULL) roztec=P->roztec;  else  roztec=0;    // pokud existuje pohon, vr�t�m si jeho rozte�

	if(scComboBox_rotace->ItemIndex==0)rotace=0;
	else rotace=1;

	if (P != NULL)rx=Form1->m.Rx(pm.dJ,pm.sJ,rotace,mezer,roztec);  // vypo��t�m si Rx
	else rx=0;
			//Memo1->Lines->Add(rx);
	double mezera=Form1->m.mezera(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,rotace,scGPNumericEdit_rx->Value,roztec);
	double rz= Form1->m.Rz(Form1->d.v.PP.delka_jig,Form1->d.v.PP.delka_jig,rotace,mezera);

	if(input_state == NOTHING || scButton_zamek_RD->Enabled==false)
	{
	 	//spocitani mezery pri prvnim zobrazeni formu
	 	if(scButton_zamek_RD->Enabled==true
	 	&& RD_zamek==UNLOCKED
	 	&& input_state==NOTHING
	 	&& scComboBox_rezim->ItemIndex==1)

		if(input_clicked_edit==Rx_klik) scGPNumericEdit_mezera->Value=mezera; // p�i zm�n� Rx vr�t�m dopo��tanou mezeru

		scGPNumericEdit_rozestup->Value=rz;
		if(scButton_zamek_RD->Enabled)	input_M();
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//validace v�ech hodnot po p�epo�tu z PO_math
void TForm_parametry::VALIDACE(Tinput_state input_state)
{
	 vypis("");VID=-1;
	 //AnsiString relation_id=GetCurrentProcessId();
	 //////////////////////////////Neplatn� hodnota////////////////////////
	 if(pm.RD<=0 || pm.CT<=0 || pm.DD<=0 || pm.K<=0 || pm.P<=0 || pm.M<-0.0000000000000004){VID=0;vypis("Neplatn� hodnota!");}
	 if(pm.RD<=0)scGPNumericEdit_RD->Font->Color=clRed;else scGPNumericEdit_RD->Font->Color=clBlack;
	 if(pm.CT<=0)scGPNumericEdit_CT->Font->Color=clRed;else scGPNumericEdit_CT->Font->Color=clBlack;
	 if(pm.DD<=0)scGPNumericEdit_delka_dopravniku->Font->Color=clRed;else scGPNumericEdit_delka_dopravniku->Font->Color=clBlack;
	 if(pm.K<=0)scGPNumericEdit_kapacita->Font->Color=clRed;else scGPNumericEdit_kapacita->Font->Color=clBlack;
	 if(pm.P<=0)scGPNumericEdit_pozice->Font->Color=clRed;else scGPNumericEdit_pozice->Font->Color=clBlack;
	 if(pm.M<-0.0000000000000004)scGPNumericEdit_mezera->Font->Color=clRed;else scGPNumericEdit_mezera->Font->Color=clBlack;

	 if(VID==-1)
	 {
//					AnsiString dop_hodnota=0;
//				//	AnsiString strSQL = "UPDATE vid_validace set doporuc_hodnota = \""+AnsiString(dop_hodnota)+"\",username = \""+F->get_user_name()+"\" ,relation_id= \""+relation_id+"\"    WHERE VID > \""+VID+"\"";
//						AnsiString strSQL = "DELETE FROM vid_validace  WHERE username = \""+F->get_user_name()+"\" AND relation_id= \""+relation_id+"\"";
//
//					Form1->FDConnection1->ExecSQL(strSQL);
	 }

	 switch(scComboBox_rezim->ItemIndex)
	 {
		///////////////////////////////////S&G////////////////////////////////////
		case 0:
		{
				double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
				if (CTunit == MIN)CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
				int pocet_obj_vSG = Form1->d.v.pocet_objektu(0);
				/////////////////pokud je CT == TT////////////////////////////////////
//				if (CT == Form1->ms.MyToDouble(Form1->d.v.PP.TT) && scComboBox_rezim->ItemIndex == 0)
//				{
//						vypis("");
//						VID=-1;
//				}
				/////////////////pokud je pohon p�i�azen a RD<=(DD)/CT////////////////////////////////////
				Cvektory::TObjekt *O=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
				if(O!=NULL)
				{
					if(pm.DD/pm.CT>O->RD*60.0 && CT<Form1->d.v.PP.TT && Form1->pom->n>1)
					{
//						Memo1->Lines->Add(pm.DD);
//						Memo1->Lines->Add(pm.CT);
//						Memo1->Lines->Add(O->RD);
						vypis("Voz�k nest�h� p�ejezd! Zvolte jin� pohon, nebo upravte d�lku kabiny �i technolog.�as.");
						VID=11;
					}
					else if (pm.DD/pm.CT>O->RD*60.0 && CT>=Form1->d.v.PP.TT && Form1->pom->n==1)
					{
						 vypis("Voz�k nest�h� p�ejezd! Zvolte jin� pohon, nebo upravte d�lku kabiny.");
						 VID=11;
					}
				}
				O=NULL;delete O;
				/////////////////pokud je CT > ne�li TT////////////////////////////////////
				if (CT > Form1->ms.MyToDouble(Form1->d.v.PP.TT)	&& scComboBox_rezim->ItemIndex == 0)
				{
						if (fmod(CT, Form1->d.v.PP.TT) == 0)
						{
								kapacitaSG = CT / Form1->d.v.PP.TT; // pro dal�� pou�it�
								vypis(" Rozlo�it na " + AnsiString(kapacitaSG) + "x " + scGPEdit_name->Text.UpperCase() + "?",false);
								scGPButton_OK->Caption = "Ano a ulo�it";
								VID=12;
						}
						else
						{
								vypis("Zm�nte re�im nebo rozlo�te do v�ce objekt�!");
								VID=13;
						}
				}
				else
				kapacitaSG = 1;

				/////////////////pokud je CT < ne�li TT////////////////////////////////////
				if (CT < Form1->ms.MyToDouble(Form1->d.v.PP.TT)	&& scComboBox_rezim->ItemIndex == 0)
				{
						if (Form1->pom->n == 1) { // prvn� objekt na lince

								if (pocet_obj_vSG == 1)
								{
										vypis("Techn. �as je ni��� ne� TT. Ve formul��i parametr� linky m��ete sn�it TT linky!");
										VID=14;
								}

								if (pocet_obj_vSG > 1)
								{
										double doporuc_hodnota=F->d.v.PP.TT/(1+CTunit*59);
										vypis("Nastavte technologick� �as shodn� s TT! Tj. <u>"+AnsiString(doporuc_hodnota)+"</u>.");
										scGPNumericEdit_CT->Font->Color=clRed;
										VID=15;
										VID_value=doporuc_hodnota;
								}
						}
						else // na lince je v�ce objekt�, pokud maj� ni��� CT dovol�m je ulo�it
						{
								VID=16;// ostatn� objekty v po�ad� na lince mohu ulo�it s ni���m CT ne� je TT linky
						}
				}
		}
		break;
		///////////////////////////////////KK////////////////////////////////////
		case 1:
		{
				if(input_clicked_edit==C_klik)
				{ //  pokud je nastaveno, �e bylo kliknuto do K
					double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
					if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
					double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele
					if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
					{;}
					else
					{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							{
								double doporuc_hodnota = CT/Form1->d.v.PP.TT;
								vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno <u>"+AnsiString(doporuc_hodnota)+"</u> nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
								scGPNumericEdit_kapacita->Font->Color=clRed;
								VID=21;
								VID_value=doporuc_hodnota;
//								 AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(doporuc_hodnota)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
//								Form1->FDConnection1->ExecSQL(strSQL);
							}
					}
				}
				//-------------------------------------------------------------------------------------------------//
		    // deklarace prom�nn�ch je v��e = je shodn�, jen je pot�eba odli�it v�pis, zdali se vztahuje k K nebo P
				if(input_clicked_edit==P_klik)
				{ //  pokud je nastaveno, �e bylo kliknuto do P
					//Memo1->Lines->Add("klik do P");
					if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
					{;}
					else
					{
						if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
						vypis("Byl zad�n neplatn� po�et pozic! Zvolte po�et pozic vy��� nebo rovno <u>"+AnsiString(pm.K2P(CT/Form1->d.v.PP.TT))+"</u> , nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
						VID=22;
					}
				}
				//----------------------------------------------------------------------------------------------------------------------//
				//---------------------------------------------------------------------------------------------------------------------------------//
				if(scGPNumericEdit_mezera->Value<-0.0000000000000004)   //ROSTA mezera o�et�en� viz v��e
				{
					double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
					double RDV=Form1->m.UDV(Form1->d.v.PP.sirka_jig,Form1->d.v.PP.delka_jig,scComboBox_rotace->ItemIndex);
					if(scGPNumericEdit_mezera->Value+DV-RDV>=0)
					{
						vypis("Mezera je z�porn�, zkuste rotaci jigu");
						VID=23;
					}
					else
					{
						vypis("Mezera je z�porn�, zkuste n�sleduj�c� palec, nebo zva�te zm�nu rozm�r� jigu");
						VID=24;
					}
				}
                                                                                                    // pokud je pohon nep�i�azen nekontroluji rozestup
				if (scButton_zamek_RD->Visible==true && RD_zamek==LOCKED && input_clicked_edit==Rx_klik && scComboBox_pohon->ItemIndex>0)
				{
					vypis("Pro zm�nu rozestupu nejd��ve povolte zm�nu rychlosti pohonu.");
				}
		///---------------------------------------------------------------
		//P�i�azen pou��van� pohon, RD<minim�ln� mo�n� RD objekt� (S&G �i PP) na p�i�azen�m pohonu (fce minRD)

//				if (scComboBox_pohon->ItemIndex != 0)
//				{
//					Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
//					double	minRD=Form1->d.v.minRD(Form1->pom->pohon);
//					if (obj!=NULL && obj->RD<minRD)
//					{
//							AnsiString vypis_nestihaji=Form1->d.v.vypis_objekty_nestihajici_prejezd(Form1->pom->pohon,obj->RD);
//					// TODO - KONTROLA 	//vypis("P�i zvolen� rychlosti pohonu, by nebylo mo�n� st�hat p�ejezd v t�chto objektech "+vypis_nestihaji+", navy�te hodnotu RD minim�ln� na "+AnsiString(minRD)+" [m/s].");
//							VID=29;
//					}
//				}
//OPRAVA MARTIN:
				if (scComboBox_pohon->ItemIndex != 0)
				{                                                       //z roletky uva�ovan� pohon (ale pom->pohon ho je�t� nem�!!!
					double	minRD=Form1->d.v.minRD(Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex));//vrat� nejni��� mo�nou rychlost pro objekt, kter� pou��v� tento pohon
					Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);

					if (obj!=NULL && pm.RD<minRD)//pokud by nov�m RD byl pod minRD (tj. pod RD nejrizikov�j��ho objektu, tak je probl�m)
					{    // TODO PRAVDEPODOBNE BUDE ROZSIRENE
							AnsiString vypis_nestihaji=Form1->d.v.vypis_objekty_nestihajici_prejezd(Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex),pm.RD);
							vypis("P�i zvolen� rychlosti pohonu by nebylo mo�n� st�hat p�ejezd v t�chto objektech "+vypis_nestihaji+", navy�te hodnotu RD minim�ln� na "+AnsiString(minRD*60.0)+" [m/min].");
							VID=29;
					}
				}
//----------------------------------------------------------------------------------------------------------------
				if(scComboBox_rezim->ItemIndex==1)
				{
					MIMO_ROZMEZI = false;
					Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				 if (P != NULL)
				 {
						double jednotky_cas_pohon = 60.0;
						if (Form_parametry->RDunitT == Form_parametry->MIN) jednotky_cas_pohon = 60.0;
						else jednotky_cas_pohon = 1.0;
						double RD   = scGPNumericEdit_RD->Value;
						double P_od = P->rychlost_od*jednotky_cas_pohon;
						double P_do = P->rychlost_do*jednotky_cas_pohon;
					//if ((unsigned)(RD - Form1->ms.MyToDouble(P_od)) <= (Form1->ms.MyToDouble(P_do) - Form1->ms.MyToDouble(P_od)))
						if(RD >= P_od && RD <= P_do)
						{
							scGPNumericEdit_RD->Options->FrameNormalColor = clGray;
							scGPNumericEdit_RD->Options->FrameWidth = 1;
						}
						else
						{
							MIMO_ROZMEZI=true;
							//scGPNumericEdit_RD->Font->Color=clRed;
							scGPNumericEdit_RD->Options->FrameNormalColor = hl_color;
							scGPNumericEdit_RD->Options->FrameWidth = hlFrameWidth;
							scGPButton_OK->Enabled=false;
							scGPNumericEdit_RD->Hint="Rychlost je mimo nastaven� rozsah pohonu";
							vypis("Rychlost pohonu je mimo nastaven� rozsah");
							VID=25;
						}
				 }
		}
		//-----------------------------------------------------------------------------------------------------------------------------//
		}
		break;

		///////////////////////////////////PP////////////////////////////////////
		case 2://PP
		{
				double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
				double DD = scGPNumericEdit_delka_dopravniku->Value; // CT od u�ivatele
				if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
				if (DDunit == MM)  DD = DD / 1000.0;

				double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele
				/////////////////pokud je pohon p�i�azen a RD<(DV+M)/TT////////////////////////////////////
				Cvektory::TObjekt *O=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);

				if(O!=NULL)
				{
			 //	Memo1->Lines->Add(Form1->m.UDV(pm.dV,pm.sV,pm.Rotace));
				//Memo1->Lines->Add(pm.M);
			 //	Memo1->Lines->Add(pm.TT);
				double DD_CT = DD/CT;
				//double UDVM_TT = Form1->m.UDV(pm.dV,pm.sV,pm.Rotace)+pm.M/pm.TT;
			 //	Memo1->Lines->Add(DD_CT);
			 //	Memo1->Lines->Add(O->RD);

						//akt RD je vy��� ne� RD ulo�en�ho pohonu -> tzn s ulo�enou RD pohonu bych nestihl p�ejet DD
					if(Form1->ms.MyToDouble(DD_CT) > Form1->ms.MyToDouble(O->RD))
					{
					 //	Memo1->Lines->Add("DD_CT > RD");
						VID=40;
						vypis("Voz�k nest�h� p�ejezd! Zvolte jinou mezeru, nebo vyberte jin� pohon.");
					}
				}
			 	O=NULL;delete O;
			 //------------------------------------------------------------------------------------------------//
				if(input_clicked_edit==C_klik)
				{ //  pokud je nastaveno, �e bylo kliknuto do P
						if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
						{;}
						else
						{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							{
								double doporuc_hodnota =	(CT/Form1->d.v.PP.TT);
								vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno <u>"+AnsiString(doporuc_hodnota)+"</u>, nebo odemkn�te technologick� �as a dojde k jeho p�epo�tu ve vztahu k zadan� kapacit�! k�d chyby: "+AnsiString(VID)+"");
								scGPNumericEdit_kapacita->Font->Color=clRed;
								VID=44;
								VID_value=doporuc_hodnota;
								//									AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(doporuc_hodnota)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
//							//	AnsiString strSQL = "UPDATE vid_validace set doporuc_hodnota = \""+AnsiString(CT/Form1->d.v.PP.TT)+"\",username = \""+F->get_user_name()+"\" ,relation_id= \""+relation_id+"\"    WHERE VID = \""+VID+"\"";
//								Form1->FDConnection1->ExecSQL(strSQL);
							}
						}
				}
				//-------------------------------------------------------------------------------------------------//
				// deklarace prom�nn�ch je v��e = je shodn�, jen je pot�eba odli�it v�pis, zdali se vztahuje k K nebo P
				if(input_clicked_edit==P_klik)
				{ //  pokud je nastaveno, �e bylo kliknuto do P
						if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
						{;}
						else
						{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							{
								double doporuc_hodnota = pm.K2P(CT/Form1->d.v.PP.TT);
								vypis("Byl zad�n neplatn� po�et pozic! Zvolte po�et pozic vy��� nebo rovno <u>"+AnsiString(doporuc_hodnota)+"</u> , nebo odemkn�te technologick� �as a dojde k jeho p�epo�tu ve vztahu k po�tu pozic!");
								VID=45;
								VID_value=doporuc_hodnota;
//							AnsiString strSQL = "INSERT INTO vid_validace (VID,doporuc_hodnota,username,relation_id) VALUES (\""+AnsiString(VID)+"\",\""+AnsiString(doporuc_hodnota)+"\",\""+AnsiString(F->get_user_name())+"\",\""+relation_id+"\")";
//							Form1->FDConnection1->ExecSQL(strSQL);
							}
						}
				}
			 //---------------------------------------------------------------------------------------------------//
			 if(scGPNumericEdit_mezera->Value<-0.0000000000000004)
				{
					double DV=Form1->m.UDV(Form1->d.v.PP.delka_jig,Form1->d.v.PP.sirka_jig,scComboBox_rotace->ItemIndex);
					double RDV=Form1->m.UDV(Form1->d.v.PP.sirka_jig,Form1->d.v.PP.delka_jig,scComboBox_rotace->ItemIndex);
					if(scGPNumericEdit_mezera->Value+DV-RDV>=0)
					{
						vypis("Mezera je z�porn�, zkuste rotaci jigu"); VID=41;
					}
					else
					{
						vypis("Mezera je z�porn�, zva�te zm�nu rozm�r� jigu"); VID=42;
					}
				}
				//------------------------------------------------------------------------------------------------------//
		}
		break;
	}

	//pokud je n�hled z PO zobrazen, zaji��uje z�rove� okam�itou aktualizaci hodnot v n�hledu z PO
	if(Form_objekt_nahled->Visible)Form_objekt_nahled->REFRESH_DATA();//obnov� dat ve formu Form_objekt_nahled v�etn� p�ekreslen�

	Form1->aktualizace_combobox_pohony_v_PO(0,RDunitT);//zaktualizovat v�pis o doporu�en� pohon -- TEST 11.10.2018!!!
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPCheckBox_zaokrouhlitClick(TObject *Sender)
{
//p�ep�n�n� zobrazen� desetinn�ho ��sla na dv� nebo neomezen� po�et desetinn�ch m�st
	int N_mist=1000;if(scGPCheckBox_zaokrouhlit->Checked)N_mist=3;
	scGPNumericEdit_CT->Decimal=N_mist;
	scGPNumericEdit_RD->Decimal=N_mist;
	scGPNumericEdit_delka_dopravniku->Decimal=N_mist;
	scGPNumericEdit_kapacita->Decimal=N_mist;
	scGPNumericEdit_mezera->Decimal=N_mist;
	scGPNumericEdit_pozice->Decimal=N_mist;
	OUTPUT();
}
//---------------------------------------------------------------------------
//zavol� n�hled kabiny, p��padn� v budoucnu info a o po�adovan�ch parametrech
void __fastcall TForm_parametry::scGPGlyphButton_viewClick(TObject *Sender)
{
	 // formul�� na st�ed
	 Form_objekt_nahled->Left = Form1->ClientWidth / 2 - Form_objekt_nahled->Width / 2;
	 Form_objekt_nahled->Top = Form1->ClientHeight / 2 - Form_objekt_nahled->Height / 2;
	 // zobrazeni formul��e
	 Form_objekt_nahled->zobrazitFrameForm=true;
	 Form_objekt_nahled->Show();//nemod�ln� zobrazen�
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::Button1Click(TObject *Sender)
{
Memo1->Lines->Add(VID);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPButton_OKClick(TObject *Sender)
{
//INPUT();
//Check_rozmezi_RD();
//if(hlFrameWidth==2){
// //Form1->MB("�daje nejsou zad�ny korektn� a nen� mo�n� je ulo�it");
// VID=0;
// }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void	TForm_parametry::Nastav_M_R_Rx()
{
	if(scComboBox_rezim->ItemIndex==1)
	{
		if(RD_zamek==LOCKED || scButton_zamek_RD->ImageIndex==37)  //37 pouze pojistka podminky, kdyby nekde nesedel korektne stav zamku tak imageindex je vzdy OK
		{
			 double mezera=getM();//p�evod jednotek
																																									//mus� se pou��t stav p�edchoz�
			 if(F->m.lze_rotovat_jig_bez_zmeny_RzRxRD(mezera,ComboRotace_predchozi_stav/*scComboBox_rotace->ItemIndex*/))
			 {
			   set(ROTACE,ENABLED,false);
				 set(RYCHLOST,DISABLED,false);
			 }
			 else
			 {
				// set(ROTACE,DISABLED,false);  15.8 ROSTA - v n�kter�ch p��padech toto nedovol� ud�lat zp�tn� rotaci v roletce
				 set(RYCHLOST, DISABLED,false);
			 }
		}
		else
		{
			set(ROTACE,ENABLED,false);
			set(RYCHLOST, ENABLED,false);
		}

		if(RD_zamek==LOCKED)
		{
			set(MEZERA,DISABLED,false);
			set(RYCHLOST, DISABLED, false);
			//set(ROZESTUP,DISABLED,false);
			scGPNumericEdit_rx->Enabled=false;
		}
		else
		{
			set(MEZERA,ENABLED,false);
			//set(ROZESTUP,ENABLED,false);
			scGPNumericEdit_rx->Enabled=true;
			set(RYCHLOST, ENABLED, false);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormPaint(TObject *Sender)
{
	packa_RDzamek(Canvas);
	frameCorrelation();//zaji��uje stejnou barvou or�muje hodnoty v korelaci, pokud je default_value na true, nastav� v�echny komponenty do v�choz�ho stavu, z�rove� kresl� prav� packy
  if(scComboBox_rezim->ItemIndex!=0)	frameKritickaMezer(Canvas);//obkresl� obrysem kritickou mezeru
}
//---------------------------------------------------------------------------
//vykresl� packu od zam�en�ho z�mku RD k souvisej�c�m hodnot�m
void TForm_parametry::packa_RDzamek(TCanvas *canv)
{
	//vykresl� packu/spojnici k hodnot�m souvisej�c�m se z�mkem RD, pouze v kontinu�ln�m re�imu
 if(RD_zamek == LOCKED && scComboBox_rezim->ItemIndex==1)
 {
	 //v�choz� hodnoty - z�mek RD lev� okraj, na v��ku polovina ikony
	 int X1=scButton_zamek_RD->Left;
	 int Y1=scButton_zamek_RD->Top+scButton_zamek_RD->Height/2;
	 int X2=scComboBox_rotace->Left;
	 int Y2=scComboBox_rotace->Top+scComboBox_rotace->Height/2;

	 //p�eta�en� vodorovn� ��sti packy RD
	 canv->Pen->Color=(TColor)RGB(170,170,170);canv->Pen->Width=2;
	 canv->MoveTo(X1,Y1);
	 canv->LineTo(scGPNumericEdit_RD->Left,Y1);

																														 //zde m��e b�t aktu�ln� stav rotace
	 if(!F->m.lze_rotovat_jig_bez_zmeny_RzRxRD(getM(),scComboBox_rotace->ItemIndex))//tuto packu �e�it pouze pokud nem� rotace v�znam
	 {
			F->d.vykresli_packu(canv,X1,Y1,X2,Y2); //packa z�mek RD - rotace
			X2=X1;
			X1=scGPNumericEdit_mezera->Left;
			Y1=scGPNumericEdit_mezera->Top+scGPNumericEdit_mezera->Height/2;
			F->d.vykresli_packu(canv,X2,Y2,X1,Y1); //packa rotace - mezera
	 }
	 else
	 {
			X2=scGPNumericEdit_mezera->Left;
			Y2=scGPNumericEdit_mezera->Top+scGPNumericEdit_mezera->Height/2;
			F->d.vykresli_packu(canv,X1,Y1,X2,Y2);//packa z�mek RD - mezera
	 }

	 //packa mezera - Rx,Rz
	 X1=scGPNumericEdit_mezera_JIG->Left;
	 Y1=scGPNumericEdit_mezera_JIG->Top+scGPNumericEdit_mezera_JIG->Height/2;
	 Y2=scGPNumericEdit_rx->Top+scGPNumericEdit_rx->Height/2;
	 F->d.vykresli_packu(canv,X1-5,Y1,X1,Y2);

	 //packa mezery
	 X1=scGPNumericEdit_mezera_JIG->Left+scGPNumericEdit_rx->Width;
	 X2=scGPNumericEdit_mezera_PODVOZEK->Left;
	 Y1=scGPNumericEdit_mezera_JIG->Top+scGPNumericEdit_mezera_JIG->Height/2;
	 F->d.vykresli_packu(canv,X1,Y1,X2,Y1);

	 //packa Rx a Rz
	 X1=scGPNumericEdit_rx->Left+scGPNumericEdit_rx->Width;
	 X2=scGPNumericEdit_rozestup->Left;
	 Y1=scGPNumericEdit_rx->Top+scGPNumericEdit_rx->Height/2;
	 F->d.vykresli_packu(canv,X1,Y1,X2,Y1);//packa mezera - Rx,Rz
 }
}
//---------------------------------------------------------------------------
//vykresl� packu mezi edity a comby od start do end
void TForm_parametry::packa(Tcomponents start,Tcomponents end)
{
	TPoint S=getRMComponent(start);TPoint E=getRMComponent(end);
	short o=4;
	//workaround(y)
	if(scGPNumericEdit_pozice->Left+scGPNumericEdit_pozice->Width==E.x)E.x-=1;//workaround divn� 1px mezery u P
	if(end==MEZERA_JIG || end==ROZESTUP){E.x-=1;/*o=6;*/}//workaround divn� 1px mezery u M a Rx,Ry
	//--
	F->d.vykresli_packu(Canvas,S.x,S.y,E.x,E.y,scGPNumericEdit_CT->Options->FrameFocusedColor,1,o);
}
//---------------------------------------------------------------------------
//stejnou barvou or�muje hodnoty v korelaci, pokud je default_value na true, nastav� v�echny komponenty do v�choz�ho stavu, z�rove� kresl� prav� packy
void TForm_parametry::frameCorrelation(bool default_value)
{
	//definice barev
	TColor nColor=clGray;//norm�ln� barva
	TColor cColor=10114859;//focus barva (modr�), ozna�uj�c� korelaci //1871359;// - oran�ov�

	//default stav
	cCT(nColor);
	cRD(nColor);
	cDD(nColor);
	cK (nColor);
	cP (nColor);
	cRo(nColor);
	cM (nColor);
	cMJ(nColor);
	cMP(nColor);
	cRx(nColor);

	//vol�n� jednotliv�ch situac� na z�klad� input_clicked_edit stavu, kter� vznik� p�i kliku do dan�ho konkr�tn�ho editu
	if(!default_value)
	{
		switch(input_clicked_edit)
		{
			////////CT////////
			case CT_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(RD_zamek == LOCKED)
							{
								cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
								packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
							}
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
						}
						break;
						case 2://PP
						{
							cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
							packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
						}
						break;
				 }
			}break;
			////////RD////////
			case RD_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1:
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cK(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(CT_zamek == LOCKED)
							{
								cDD(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(RYCHLOST,DELKA);packa(DELKA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
						}
						break;//KK
						case 2:break;//PP
				 }
			}break;
			////////DD////////
			case DD_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(RD_zamek == LOCKED)
							{
								cCT(cColor);cK(cColor);cP(cColor);
								packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
							}
							if(CT_zamek == LOCKED)
							{
								cRD(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(RYCHLOST,DELKA);packa(DELKA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
						}
						break;
						case 2://PP
						{
							cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
							packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
						}
						break;
				 }
			}break;
			////////K////////
			case C_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);
							}
							if(RD_zamek == LOCKED)
							{
								cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
								packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
							}
						}
						break;
						case 2://PP
						{
							cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
							packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
						}
						break;
				 }
			}break;
			case P_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(RD_zamek == LOCKED)
							{
								cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
								packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
							}
						}
						break;
						case 2://PP
						{
							cCT(cColor);cDD(cColor);cK(cColor);cP(cColor);
							packa(TIME,DELKA);packa(DELKA,KAPACITA);packa(KAPACITA,POZICE);
						}
						break;
				 }
			}break;
			case Rotace_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);cRo(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,ROTACE);packa(ROTACE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(CT_zamek == LOCKED)
							{
								cRD(cColor);cDD(cColor);cRo(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(RYCHLOST,DELKA);packa(DELKA,ROTACE);packa(ROTACE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(RD_zamek == LOCKED)
							{
								if(F->m.lze_rotovat_jig_bez_zmeny_RzRxRD(getM(),ComboRotace_predchozi_stav))//pouze pokud nem� rotace vliv na RD
								{
									cP(cColor);cRo(cColor);cM(cColor);cMJ(cColor);cMP(cColor);
									packa(POZICE,ROTACE);packa(ROTACE,MEZERA_JIG);
                }
							}
						}
						break;
						case 2://PP
						{
							cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);
							packa(POZICE,MEZERA_JIG);
						}
						break;
				 }
			}break;
			case mezera_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(CT_zamek == LOCKED || K_zamek == LOCKED)
							{
								cRD(cColor);cDD(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(RYCHLOST,DELKA);packa(DELKA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
						}
						break;
						case 2://PP
						{
							cDD(cColor);cP(cColor);
							packa(DELKA,POZICE);
						}
						break;
				 }
			}break;
			case Rx_klik:
			{
				 switch(scComboBox_rezim->ItemIndex)
				 {
						case 1://KK
						{
							if(DD_zamek == LOCKED)
							{
								cCT(cColor);cRD(cColor);cK(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(TIME,RYCHLOST);packa(RYCHLOST,KAPACITA);packa(KAPACITA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
							if(CT_zamek == LOCKED || K_zamek == LOCKED)
							{
								cRD(cColor);cDD(cColor);cP(cColor);cM(cColor);cMJ(cColor);cMP(cColor);cRx(cColor);
								packa(RYCHLOST,DELKA);packa(DELKA,POZICE);packa(POZICE,MEZERA_JIG);packa(MEZERA_PODVOZEK,ROZESTUP);
							}
						}
						break;
						case 2:break;//PP
				 }
			}break;
		}
	}
}
//---------------------------------------------------------------------------
//vr�t� sou�adnice prav�ho okraje a horn�ho okraje+poloviny v��ky komponenty
TPoint TForm_parametry::getRMComponent(Tcomponents C)
{
	TPoint R;
	switch(C)
	{
		//case POHON: break;
		case TIME:		R.x=scGPNumericEdit_CT->Left+scGPNumericEdit_CT->Width; 							R.y=scGPNumericEdit_CT->Top+scGPNumericEdit_CT->Height/2;break;
		case RYCHLOST:R.x=scGPNumericEdit_RD->Left+scGPNumericEdit_RD->Width;								R.y=scGPNumericEdit_RD->Top+scGPNumericEdit_RD->Height/2;break;
		case DELKA:		R.x=scGPNumericEdit_delka_dopravniku->Left+scGPNumericEdit_delka_dopravniku->Width;		R.y=scGPNumericEdit_delka_dopravniku->Top+scGPNumericEdit_delka_dopravniku->Height/2;break;
		case KAPACITA:R.x=scGPNumericEdit_kapacita->Left+scGPNumericEdit_kapacita->Width;		R.y=scGPNumericEdit_kapacita->Top+scGPNumericEdit_kapacita->Height/2;break;
		case POZICE:	R.x=scGPNumericEdit_pozice->Left+scGPNumericEdit_pozice->Width;				R.y=scGPNumericEdit_pozice->Top+scGPNumericEdit_pozice->Height/2;break;
		case ROTACE:	R.x=scComboBox_rotace->Left+scComboBox_rotace->Width;									R.y=scComboBox_rotace->Top+scComboBox_rotace->Height/2;break;
		case MEZERA_JIG:	R.x=scGPNumericEdit_mezera_JIG->Left+scGPNumericEdit_mezera_JIG->Width;R.y=scGPNumericEdit_mezera_JIG->Top+scGPNumericEdit_mezera_JIG->Height/2;break;
		case MEZERA_PODVOZEK:	R.x=scGPNumericEdit_mezera_PODVOZEK->Left+scGPNumericEdit_mezera_PODVOZEK->Width;R.y=scGPNumericEdit_mezera_PODVOZEK->Top+scGPNumericEdit_mezera_PODVOZEK->Height/2;break;
		case ROZESTUP:R.x=scGPNumericEdit_rx->Left+scGPNumericEdit_rx->Width;							R.y=scGPNumericEdit_rx->Top+scGPNumericEdit_rx->Height/2;break;
		//case STOPKA:break;
		//case CEKANI:break;
		//case ODCHYLKA:break;
	}
	return R;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vr�t� hodnotu mezery z editboxu dle nastaven�ch jednotek mezery mezera p�evedenou do SI jednotek + o�et�uje divn� chov�n� okolo nuly
double TForm_parametry::getM()
{
	return getM(scGPNumericEdit_mezera->Value);
}
//---------------------------------------------------------------------------
//vr�t� hodnotu parametru dle nastaven�ch jednotek mezery p�evedenou do SI jednotek + o�et�uje divn� chov�n� okolo nuly
double TForm_parametry::getM(double M)
{
	//p�evod jednotek
	double mezera=M;
	if (DMunit == MM) mezera=M/1000.0;
	if(F->m.between(mezera,-0.0000000001,+0.0000000001))mezera=F->m.round(mezera);//o�et�en� proti divn�mu chov�n� ��seln� soustavy okolo +-0
	return mezera;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_mezera_JIGChange(TObject *Sender)
{
	if (input_state == NOTHING/* && input_clicked_edit == mezera_klik*/)
	{
		input_state = mezera_jig;
		INPUT();
		pm.input_MJ();//zavol�n� v�po�etn�ho modelu
		OUTPUT();
		if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery();
		input_state = NOTHING;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_mezera_PODVOZEKChange(TObject *Sender)
{
	if (input_state == NOTHING/* && input_clicked_edit == mezera_klik*/)
	{
		input_state = mezera_podvozek;
		INPUT();
		pm.input_MP();//zavol�n� v�po�etn�ho modelu
		OUTPUT();
		if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery();
		input_state = NOTHING;
	}
}
//---------------------------------------------------------------------------
//obkresl� obrysem kritickou mezeru
void TForm_parametry::frameKritickaMezer(TCanvas *C)
{
	if(scGPNumericEdit_mezera_JIG->Value!=scGPNumericEdit_mezera_PODVOZEK->Value)//pokud budou stejn� nebude se nic d�t
	{
		if(scGPNumericEdit_mezera_JIG->Enabled)Canvas->Pen->Color=F->m.clIntensive(clRed,70);
		else Canvas->Pen->Color=F->m.clIntensive(clRed,150);//pokud je zak�zano je sv�tlej�� barvou or�mov�n�
		Canvas->Pen->Mode=pmCopy;
		Canvas->Pen->Width=2;//mus� b�t d�liteln� dv�mi
		Canvas->Brush->Style=bsClear;
		TscGPNumericEdit *K;
		if(JKM())//rozhodne, co je kritick� mezera
		{
			K=scGPNumericEdit_mezera_JIG;
			//podbarven� editu
			//scGPNumericEdit_mezera_JIG->Options->NormalColor=clRed;
			//scGPNumericEdit_mezera_JIG->Options->NormalColorAlpha=1;
		}
		else K=scGPNumericEdit_mezera_PODVOZEK;
		Canvas->Rectangle(TRect(K->Left,K->Top,K->Left+K->Width+1,K->Top+K->Height+1));
		K=NULL;delete K;
	}
}
//---------------------------------------------------------------------------
//vrat� true, pokude je mezera_JIG kritick� mezera, false pokud je mezera_PODVOZEK kritick� mezera
bool TForm_parametry::JKM()
{
	 if(scGPNumericEdit_mezera_JIG->Value<scGPNumericEdit_mezera_PODVOZEK->Value)return true;//rozhodne, co je kritick� mezera
	 else return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_rozestupChange(TObject *Sender)
{
	 input_state_Rz=true;

	 if(input_clicked_edit==Rz_klik)
	 {
			double Rz=0;
			if (DMunit == M) Rz=scGPNumericEdit_rozestup->Value;
			else Rz=scGPNumericEdit_rozestup->Value/1000.0;

			if(RDunitT == MIN) scGPNumericEdit_RD->Value = Form1->m.RD(Rz)*60;
			else 	scGPNumericEdit_RD->Value = Form1->m.RD(Rz);
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_rozestupClick(TObject *Sender)
{
	 input_clicked_edit=Rz_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPGlyphButton_refreshClick(TObject *Sender)
{
  refresh_data();
}
//---------------------------------------------------------------------------

void TForm_parametry::refresh_data()
{

     if(mrYes==F->MB("Chcete nastavit v�choz� hodnoty parametr�? Nastaven�m v�choz�ch hodnot boudou sou�asn� hodnoty parametr� ztraceny!",MB_YESNO))
		 	{
		 			//nastaven� hodnot parametr� do default
		 		 if (CTunit==MIN) scGPNumericEdit_CT->Value= Form1->pom->CT/60.0;
				 else  scGPNumericEdit_CT->Value= Form1->pom->CT;
		 		 if(RDunitT==MIN)    scGPNumericEdit_RD->Value=  Form1->pom->RD*60.0;
		 		 else scGPNumericEdit_RD->Value =  Form1->pom->RD;
		 		 if (DDunit == MM)scGPNumericEdit_delka_dopravniku->Value = Form1->pom->delka_dopravniku* 1000.0;
		 		 else scGPNumericEdit_delka_dopravniku->Value = Form1->pom->delka_dopravniku;

				 scGPNumericEdit_kapacita->Value = Form1->pom->kapacita;
				 scGPNumericEdit_pozice->Value   = Form1->pom->pozice;
				 scGPNumericEdit_mezera->Value   = Form1->pom->mezera;
				 scGPNumericEdit_mezera_JIG->Value   = Form1->pom->mezera_jig;
				 scGPNumericEdit_mezera_PODVOZEK->Value   = Form1->pom->mezera_podvozek;
				 if(scComboBox_pohon->ItemIndex!=0)//data z pohonu na��t�m pouze kdy� je n�jak� pohon vybr�n
				 {
					 scGPNumericEdit_rx->Value      = Form1->pom->pohon->Rx;
					 scGPNumericEdit_rozestup->Value = Form1->pom->pohon->Rz;
				 }
			}


}





void __fastcall TForm_parametry::scGPGlyphButton_odchylkaClick(TObject *Sender)
{

    if(F->pom->dalsi!=NULL)
    {
    if(F->pom->dalsi->pohon==NULL) F->MB("N�sleduj�c� objekt, nem� p�i�azen� ��dn� pohon, proto nen� navrhov�na ��dn� hodnota odchylky.");
    else { scGPNumericEdit_odchylka->Value=F->m.cekani_na_palec(0,F->pom->dalsi->pohon->roztec,F->pom->dalsi->pohon->aRD,3)/(1+59.0*CTunit); /* scGPMemo1->Lines->Clear(); scGPMemo1->Lines->Add(F->pom->dalsi->pohon->roztec); scGPMemo1->Lines->Add(F->pom->dalsi->pohon->aRD); scGPMemo1->Lines->Add(F->m.cekani_na_palec(0,F->pom->dalsi->pohon->roztec,F->pom->dalsi->pohon->aRD,3)); */}
    }
    else
    {
    if(F->d.v.OBJEKTY->dalsi->pohon==NULL) F->MB("N�sleduj�c� objekt, nem� p�i�azen� ��dn� pohon, proto nen� navrhov�na ��dn� hodnota odchylky.");
    else scGPNumericEdit_odchylka->Value=F->m.cekani_na_palec(0,F->d.v.OBJEKTY->dalsi->pohon->roztec,F->d.v.OBJEKTY->dalsi->pohon->aRD,3)/(1+59.0*CTunit);
    }
    //MB - text, �ipka download, napozicov�n� skr�v�n�, p�evody jednotek/ odchylka p�ep�natelnost jednotek dle CT
    //kdy� pohon NULL - MB s textem, �e nen� co doporu�ovat
    //tato podm�nka bude prvn�
    //if F->pom->dalsi == NULL - jedn� se kdy� jde o posledn� objekt tak potom pohon z�skat F->d.v.OBJEKTY->dalsi->pohon
}
//---------------------------------------------------------------------------

