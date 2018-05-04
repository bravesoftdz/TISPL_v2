// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "parametry.h"
#include "unit1.h"
#include "parametry_linky.h"
#include "poznamky.h"
#include "kabina_schema.h"
#include "MyMessageBox.h"
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
__fastcall TForm_parametry::TForm_parametry(TComponent* Owner) : TForm(Owner)
{
		// nové nastavení barvy formuláøe
		Form_parametry->Color = (TColor)RGB(240, 240, 240);
		scGPButton_header_projekt->Options->NormalColor = (TColor)RGB(240, 240, 240);

		// nastavení zvýraznìní
		hl_color = clRed;// (TColor)RGB(255,141,28);//barva zvýraznìní rámeèku komponenty napø.pro povinné položky
		hlFrameWidth = 2;// šíøka zvýraznìní rámeèku komponenty napø.pro povinné položky

		// pøevzetí defaultní velikosti formuláøe z designu pro další užití (slouží spíše pro usnadnìní vývoje, než funkènì)
		defaultForm_parametryHeight = Form_parametry->Height;

		// matamaticky exaktní napozicování tlaèítek OK a storno
		Form1->m.designButton(scGPButton_OK, Form_parametry, 1, 2);
		Form1->m.designButton(scGPButton_storno, Form_parametry, 2, 2);

		// výchozí nastavení zámkù
		CT_zamek = UNLOCKED;
		DD_zamek = UNLOCKED;
		RD_zamek = LOCKED;
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	input_state = NOTHING; // nutnost!!! - test pøesunuto nahoru
	// formuláø bude pøi prvním zobrazení v sekundách a metrech nebo dle INI v odvozených jednotkách, jinak dle SI
	if(form_zobrazen==false)
	{
		minsec = S;
		m_mm = M;
		CTunit = S;
		RDunitT = S;
		RDunitD = M;
		DDunit = M;
		DMunit = M;

		// GLOBAL
		if (Form1->readINI("nastaveni_form_parametry", "cas") == "1")
		{
				minsec = MIN;
				scGPButton_min_sec->Caption = "vše na s";
		} // tedy MIN
		if (Form1->readINI("nastaveni_form_parametry", "vzdalenost") == "1")
		{
				m_mm = MM;
				scGPButton_metry_milimetry->Caption = "vše na m";
		} // tedy MM
		// CT
		if (Form1->readINI("nastaveni_form_parametry", "CT") == "1")rHTMLLabel_CTClick(this); // pøevede na min tzn. CTunit=MIN;
		// RD
		if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")rHTMLLabel_RDClick(this); // pøevede na min tzn. RDunitT=MIN;
		//if (Form1->readINI("nastaveni_form_parametry", "RDd") == "1")rHTMLLabel_RDClick(this); // pøevede na mm tzn. RDunitD=MM;
		// DD
		if (Form1->readINI("nastaveni_form_parametry", "DD") == "1")rHTMLLabel_delka_dopravnikuClick(this); // pøevede na mm tzn. DDunit=MM;
		// DM
		if (Form1->readINI("nastaveni_form_parametry", "DM") == "1")rHTMLLabel_mezeraClick(this); // pøevede na mm tzn. DMunit=MM;
		// nastavení defaultních hodnot
		if (scGPNumericEdit_CT->Value == 0)
				// if(d.v.ZAKAZKY->dalsi!=NULL)//pokud existuje první zakázka
		{
				scGPNumericEdit_CT->Value = Form1->d.v.PP.TT / (1 + 59 * CTunit);
				// již nepoužíváme, protože se u první zakázky uvažuje globální TT,d.v.ZAKAZKY->dalsi->TT;
				scGPNumericEdit_kapacita->Value = 1;
		}
	}
		//M: byl zde než jsem ho pøesunul zcela nahoru: input_state = NOTHING; // nutnost!!!
		//	input_clicked_edit=empty_klik; M: toto bylo i pøedtím zakomentované, prosím, pokud není zcela tøeba mazat, a není zavadìjící
		kapacitaSG = 1; // není podnìt k rozkládání na více objektù
		scGPEdit_name->SetFocus();
		// nastaví výchozí focus, kde se pøedpokládá výchozí nastavování
		scGPEdit_name->SelectAll(); // oznaèí cele pro editace
		scGPButton_OK->Enabled = true;
		scGPButton_OK->Caption = "Uložit";
		form_zobrazen = true;
		// detekuje zda je form aktuálnì zobrazen, slouží proto aby pøi zmìnì combo režim pokud si nastavil uživatel formulaø jinam, aby zùstal nastaven dle uživatele

		// pohon_je_pouzivan  - nastavení zámkù a editboxù dle nastaveného pohonu.
		Pohon_pouzivan();
		Nacti_rx(); // vypoèítání Rx a zobrazeni
		INPUT();   // pøi prvním zobrazení formu "otisknu" data z formu do math struktury, bez žádných výpoètù, primárnì použito pro nastavení decimal checkboxu, kdy potøebuje mít data v output již pøi formshow
		OUTPUT();  // naètení dat ze struktury
		if(scComboBox_rezim->ItemIndex==1) 	Check_rozmezi_RD();
		if(scComboBox_rezim->ItemIndex!=1)  // pro jiné režimy vždy povolím zobrazení zámkù
		{
			scButton_zamek_CT->Enabled=true;
			scButton_zamek_DD->Enabled=true;
		}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TForm_parametry::vypis(UnicodeString text,bool red,bool link)
{
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption = "Uložit";
		if (text != "") // zobrazí a vypíše
		{
				//rHTMLLabel_InfoText->Hint=text;//zajišuje zobrazení celého textu
				rHTMLHint1->ToString()=text;//natežení do hintu
				// if(!rHTMLLabel_InfoText->Visible)Form_parametry->Height+=(40+19);//pouze pokud byl popisek skrytý

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnutí podtrženého písma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
						scGPButton_OK->Enabled=false;
						scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor = clRed;
						rHTMLLabel_InfoText->Font->Color = clRed;
						// zvýraznìní položky technologický èas
						if (text.Pos("technologický èas") && scGPNumericEdit_CT->Enabled)
						{ /* ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth; */
						}
						else { /* ROSTA-stylscGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1; */
						}
				}
				else
				{
						scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor =(TColor)RGB(0, 128, 255);
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0, 128, 255);
				}
				scGPGlyphButton_InfoIcon->Top = Form_parametry->Height - 81;
				if (text.Length() <= 35)// v pøípadì, že je text delší než 35 znakù skryje ikonu u zvolí nové levé odsazení textu
				{
						scGPGlyphButton_InfoIcon->Visible = true;
						rHTMLLabel_InfoText->Left = 34;
				}
				else
				{
						scGPGlyphButton_InfoIcon->Visible = false;
						rHTMLLabel_InfoText->Left = 8;
				}
				rHTMLLabel_InfoText->Top = Form_parametry->Height - 74;
				rHTMLLabel_InfoText->Visible = true;
				rHTMLLabel_InfoText->Caption = text;
		}
		else // skryje
		{
				scGPGlyphButton_InfoIcon->Visible = false;
				rHTMLLabel_InfoText->Visible = false;
				// zvýraznìní položky technologický èas
				if (scGPNumericEdit_CT->Enabled)
				{ /* ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1; */
				}
				// Form_parametry->Height-=(40+19);
		}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
		if (input_state != NO) // pokud to není pøi startu (formshow)
		{

				// výchozí nastavení zámkù pøi pøekliku režimu na KK
				if (scComboBox_rezim->ItemIndex == 1) {
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
				// výchozí nastavení zámkù pøi pøekliku režimu na PP
				if (scComboBox_rezim->ItemIndex == 2) {
						K_zamek = LOCKED;
						DD_zamek = UNLOCKED;
						scButton_K_zamek->Visible = false;
						scButton_zamek_DD->ImageIndex = 38;
						scButton_K_zamek->ImageIndex = 37;

				}


				// nadesignování a napozicování komponent dle zvoleného režimu
				setForm4Rezim(scComboBox_rezim->ItemIndex);
				// resize a napozicování formuláøe+povoleni a zakazani komponent pro jednotlivé režimy

				//nastaví edity, podle toho, zdali je pohon používán èi nikoliv - volat až po setForm4Režim
				Pohon_pouzivan();

				if(scComboBox_rezim->ItemIndex == 0) scGPNumericEdit_pozice->Value=1;

				INPUT();  // uložení dat z editù do struktury pøi zmìnì režimu
				OUTPUT(); // naètení ze struktury - aby probìhla validace dat pøi zmìnì režimu

				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
				if (scComboBox_rezim->ItemIndex == 1 && obj!=NULL ){  // u KK režimu pokud je pohon používán - natáhnutí správné mezery z dat

					 scGPNumericEdit_mezera->Value=obj->mezera;
					}

				// napozicování celého formuláøe resp. ošetøení aby zùstal dialog na monitoru, pouze pro prvotní zobrazení dle souøadnic kurzoru myši, jinak dle uživatele
				long X = Form1->akt_souradnice_kurzoru_PX.x + 10;
				long Y = Form1->akt_souradnice_kurzoru_PX.y + 10;
				if (form_zobrazen) {
						X = Form_parametry->Left;
						Y = Form_parametry->Top;
				}

				if (X + Form_parametry->ClientWidth < Form1->ClientWidth) Form_parametry->Left = X;
				else
						Form_parametry->Left = Form1->ClientWidth - Form_parametry->ClientWidth - 10;
				if (Y + Form_parametry->ClientHeight < Form1->ClientHeight) Form_parametry->Top = Y;
				else
						Form_parametry->Top = Form1->ClientHeight - Form_parametry->ClientHeight - Form1->scGPPanel_statusbar->Height - 10;

				// aktualizace hodnot
				// if(scGPNumericEdit_CT->Value>0)/*input_CT();*/ ShowMessage(input_state);
				// else
				// {
				// if(scGPNumericEdit_delka_dopravniku->Value>0)input_DD();
				// else
				// {
				// if(scGPNumericEdit_kapacita->Value>0)input_K();
				// else input_RD();
				// }
				// }
			 //	vypis("",false);
		}
		if (scComboBox_rezim->ItemIndex == 1) // mezera se doporucuje pouze u KK rezimu
		{
	 /*			// výpis doporuèené mezery
				Cvektory::TPohon *P =	Form1->d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex);
				// mezera_mezi_voziky
				double dV = Form1->d.v.PP.delka_voziku; // delka voziku
				double sV = Form1->d.v.PP.sirka_voziku; // delka voziku
				double rotace;
				if (scComboBox_rotace->ItemIndex == 0)
						rotace = 0;
				else
						rotace = 90;

				if (P != NULL)
				{ //pokud existuje pohon
						if (P->roztec > 0) // pokud existuje rozteè
						{
								double mezera=0;
								if(scGPNumericEdit_mezera->Value==0) mezera=0;
								else mezera=scGPNumericEdit_mezera->Value;

								double doporuc_mezera = Form1->m.mezera_mezi_voziky(dV, sV, rotace, P->roztec, mezera);
								if(Form1->ms.MyToDouble(doporuc_mezera)!=Form1->ms.MyToDouble(mezera))
								{
								vypis("Doporuèená mezera: " + AnsiString(doporuc_mezera) + " m");
								VID=28;
								} else vypis("",false);
						}  else    vypis("",false);
				}  */

		Kontrola_mezery();

		}
}

// ---------------------------------------------------------------------------
// resize a napozicování formuláøe+povoleni a zakazani komponent pro jednotlivé režimy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
		// výchozí zmenšení formuláøe
		offset = 0;
		if (Form1->STATUS == Form1->NAVRH)rezim += 10; // posunutí o 10 vytváøí režim+navrháø
		//pozice hlavièkových komponent (+použito následnì v kodu)
		scGPGlyphButton_view->Visible=false;
		scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width;
		scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width;
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
						set(ROZESTUP, HIDE);
						scGPNumericEdit_kapacita->Value = 1;
				} break;
		case 10: // STOP & GO - NÁVRHÁØ
				{
						// nastavení komponent
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
						set(ROZESTUP, HIDE);
						scGPNumericEdit_kapacita->Value = 1;
				} break;
		case 1: // KONTINUÁLNÍ
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
						set(ROZESTUP, READONLY);
						//hlavièkové komponenty
						scGPGlyphButton_view->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_view->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_view->Width;
				} break;
		case 11: // KONTINUÁLNÍ - NÁVRHÁØ
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
						set(ROZESTUP, READONLY);
						//hlavièkové komponenty
						scGPGlyphButton_view->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_view->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_view->Width;
				} break;
		case 2: // POSTPROCESNÍ
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
						set(ROZESTUP, HIDE);
				} break;
		case 12: // POSTPROCESNÍ - NÁVRHÁØ
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
						set(ROZESTUP, HIDE);
				} break;
		case 13: // SG - NÁVRHÁØ  - ROZŠÍØENÉ
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

		// VELIKOST FORMULÁØE
		Form_parametry->Height = defaultForm_parametryHeight + offset;
		// vertikální (popø. horizontální) POZICE TLAÈÍTEK OK,STORNO,COPY,PASTE
		scGPButton_OK->Top = Form_parametry->Height - scGPButton_OK->Height - 10;
		scGPButton_storno->Top = Form_parametry->Height -	scGPButton_storno->Height - 10;
		scGPGlyphButton_copy->Top = Form_parametry->Height  - scGPGlyphButton_copy->Height;
		scGPGlyphButton_copy->Left=0;
		scGPGlyphButton_paste->Top = Form_parametry->Height -	scGPGlyphButton_paste->Height;
		scGPGlyphButton_paste->Left=scGPGlyphButton_copy->Width+scGPGlyphButton_copy->Left;
		scGPCheckBox_zaokrouhlit->Top = Form_parametry->Height -scGPCheckBox_zaokrouhlit->Height;
}
// ---------------------------------------------------------------------------
// zajišuje zobrazení a napozicování patøièné konkrétní komponenty a zároveò udržování hodnoty offsetu - to pokud je move==true, jinak jen nastaví komponenty
void TForm_parametry::set(Tcomponents C, Tcomponents_state S, bool move)
{
	// defaultní hodnoty
	short O = 40;//vertikální velikost odsazení komponent
	int L = rHTMLLabel_rezim->Top;//výchozí komponenta pro usazení horní pozice levé komponenty
	int P = scComboBox_rezim->Top;//výchozí komponenta pro usazení horní pozice prave komponenty
	int Le=234;//levé umístìní kratších editboxu a comboboxu (od tech.èasu níže)
	int W=156;//šíøka kratších editboxu a comboboxu (od tech.èasu níže)

	switch (C)
	{
		case POHON:
		{
			 ////ošetøení pokud není nadefinován žádný pohon
			 if (!existuje_pohon)S = READONLY;
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_pohon->Top = L + O;
					scComboBox_pohon->Top = P + O;
					scGPButton_header_projekt->Top=scComboBox_pohon->Top;
			 }
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_CT->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_pohon->Visible = true;
			 scComboBox_pohon->Visible = true;
			 scComboBox_pohon->Enabled = true;
			 rHTMLLabel_pohon->Left = 8;
			 scComboBox_pohon->Options->FrameNormalColor = clGray;
			 scComboBox_pohon->Options->FrameWidth = 1;
			 scComboBox_pohon->Left = 56;
			 // ty co jsou rozdílné
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
			 		scComboBox_pohon->Enabled = false;
					break;
			 		// case READONLY:scComboBox_pohon->Enabled=false;scComboBox_pohon->Options->ShapeStyle=scgpessNone;break;
				case HIDE:
			 		rHTMLLabel_pohon->Visible = false;
					scComboBox_pohon->Visible = false;
					if (move)offset -= O;
					break;
			 }
		} break;
		case TIME: // technologický èas v režimu návrháø
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
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_CT->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_CT->Visible = true;
			 scGPNumericEdit_CT->Enabled = true;
			 scGPNumericEdit_CT->Visible = true;
			 scGPNumericEdit_CT->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_CT->Options->FrameWidth = 1;
			 if (scComboBox_rezim->ItemIndex == 1)scButton_zamek_CT->Visible = true;
			 else	scButton_zamek_CT->Visible = false;
			 // ty co jsou rozdílné
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
		case RYCHLOST: // RD v režimu návrháø
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
			 ////funkèní vlastnosti
			 // ty co jsou stejné
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
			 // ty co jsou rozdílné
			 switch (S)
			 {
				case HIGHLIGHT:
					scGPNumericEdit_RD->Options->FrameNormalColor = hl_color;
					scGPNumericEdit_RD->Options->FrameWidth = hlFrameWidth;
				case ENABLED: // scButton_zamek_RD->ImageIndex=38;RD_zamek=UNLOCKED;
					break;
				case DISABLED
							: // scGPNumericEdit_RD->Enabled=false;scButton_zamek_RD->ImageIndex=37;RD_zamek=UNLOCKED;
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
		case DELKA: // délka dopravníku
		{
			 ////pozice
			 if (move)
			 {
			 		rHTMLLabel_delka_dopravniku->Top = L + 4 * O + offset;
					scGPNumericEdit_delka_dopravniku->Top = P + 4 * O + offset;
			 		scButton_zamek_DD->Top = scGPNumericEdit_delka_dopravniku->Top;
			 }
			 scGPNumericEdit_delka_dopravniku->Left=Le;
			 scGPNumericEdit_delka_dopravniku->Width=W;
			 // ty co jsou stejné
			 scGPNumericEdit_delka_dopravniku->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_delka_dopravniku->Visible = true;
			 scGPNumericEdit_delka_dopravniku->Visible = true;
			 scGPNumericEdit_delka_dopravniku->Enabled = true;
			 scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_delka_dopravniku->Options->FrameWidth = 1;
			 if (scComboBox_rezim->ItemIndex == 0 || scComboBox_rezim->ItemIndex == 2)scButton_zamek_DD->Visible = false;
			 else	scButton_zamek_DD->Visible = true;
			 // ty co jsou rozdílné
			 ////funkèní vlastnosti
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
					scGPNumericEdit_delka_dopravniku->Options->ShapeStyle =
			 				scgpessNone;
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
		case KAPACITA: // požadována kapacita objektu
		{
			 ////pozice
			 rHTMLLabel_kapacita->Top = L + 5 * O + offset;
			 scGPNumericEdit_kapacita->Top = P + 5 * O + offset;
			 scButton_K_zamek->Top = scGPNumericEdit_kapacita->Top;
			 scGPNumericEdit_kapacita->Left=Le;
			 scGPNumericEdit_kapacita->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_kapacita->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_kapacita->Visible = true;
			 scGPNumericEdit_kapacita->Visible = true;
			 scGPNumericEdit_kapacita->Enabled = true;
			 scButton_K_zamek->Visible = false;
			 scGPNumericEdit_kapacita->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_kapacita->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
		case POZICE: // zjištìná kapacita objektu
		{
				////pozice
			 rHTMLLabel_pozice->Top = L + 6 * O + offset;
			 scGPNumericEdit_pozice->Top = P + 6 * O + offset;
			 scGPNumericEdit_pozice->Left=Le;
			 scGPNumericEdit_pozice->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_pozice->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_pozice->Visible = true;
			 scGPNumericEdit_pozice->Visible = true;
			 scGPNumericEdit_pozice->Enabled = true;
			 scGPNumericEdit_pozice->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_pozice->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
		case ODCHYLKA: // povolená odchylka z CT
		{
			 ////pozice
			 rHTMLLabel_odchylka->Top = L + 7 * O + offset;
			 scGPNumericEdit_odchylka->Top = P + 7 * O + offset;
			 scGPNumericEdit_odchylka->Left=Le;
			 scGPNumericEdit_odchylka->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_odchylka->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_odchylka->Visible = true;
			 scGPNumericEdit_odchylka->Visible = true;
			 scGPNumericEdit_odchylka->Enabled = true;
			 scGPNumericEdit_odchylka->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_odchylka->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
		case CEKANI: // èekání
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_cekani->Top = L + 8 * O + offset;
			 		scComboBox_cekani_palec->Top = P + 8 * O + offset;
			 }
			 scComboBox_cekani_palec->Left=Le;
			 scComboBox_cekani_palec->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scComboBox_cekani_palec->Options->FrameNormalColor = clGray;
			 scComboBox_cekani_palec->Options->FrameWidth = 1;
			 rHTMLLabel_cekani->Visible = true;
			 scComboBox_cekani_palec->Visible = true;
			 scComboBox_cekani_palec->Enabled = true;
			 // ty co jsou rozdílné
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
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 rHTMLLabel_stopka->Visible = true;
			 scComboBox_stopka->Visible = true;
			 scComboBox_stopka->Enabled = true;
			 scComboBox_stopka->Options->FrameNormalColor = clGray;
			 scComboBox_stopka->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
		case ROTACE: // rotace jigu v objektu, zatím jen pøepínátko 0-90
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_rotace->Top = L + 10 * O + offset;
			 		scComboBox_rotace->Top = P + 10 * O + offset;
			 }
			 scComboBox_rotace->Left=Le;
			 scComboBox_rotace->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 rHTMLLabel_rotace->Visible = true;
			 scComboBox_rotace->Visible = true;
			 scComboBox_rotace->Enabled = true;
			 scComboBox_rotace->Options->FrameNormalColor = clGray;
			 scComboBox_rotace->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
		case MEZERA: // požadována vs. zjištìná kapacita objektu
		{
			 ////pozice
			 if (move)
			 {
					rHTMLLabel_mezera->Top = L + 11 * O + offset;
					scGPNumericEdit_mezera->Top = P + 11 * O + offset;
			 }
			 scGPNumericEdit_mezera->Left=Le;
			 scGPNumericEdit_mezera->Width=W;
			 ////funkèní vlastnosti
			 // ty co jsou stejné
			 scGPNumericEdit_mezera->Options->ShapeStyle = scgpessRect;
			 rHTMLLabel_mezera->Visible = true;
			 scGPNumericEdit_mezera->Visible = true;
			 scGPNumericEdit_mezera->Enabled = true;
			 scGPNumericEdit_mezera->Options->FrameNormalColor = clGray;
			 scGPNumericEdit_mezera->Options->FrameWidth = 1;
			 // ty co jsou rozdílné
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
					if (move)offset -= O;
					break;
			 }
			} break;
		case ROZESTUP: // požadována vs. zjištìná kapacita objektu
		{
			 	////pozice
			if (move)
			{
					rHTMLLabel_rozestup->Top = L + 12 * O + offset;
					scGPNumericEdit_rozestup->Top = P + 12 * O + offset;
					scGPNumericEdit1_rx->Top=scGPNumericEdit_rozestup->Top;
					rHTMLLabel_palec_vzd->Top=rHTMLLabel_rozestup->Top;
					rHTMLLabel_jednotky_vzdalenostpalcu->Top=rHTMLLabel_palec_vzd->Top;
			}
			////funkèní vlastnosti
			// ty co jsou stejné
			scGPNumericEdit_rozestup->Options->ShapeStyle = scgpessRect;
			rHTMLLabel_rozestup->Visible = true;
			scGPNumericEdit1_rx->Visible=true;
			scGPNumericEdit_rozestup->Visible = true;
			rHTMLLabel_palec_vzd->Visible=true;
			rHTMLLabel_jednotky_vzdalenostpalcu->Visible=true;
			scGPNumericEdit_rozestup->Enabled = true;
			scGPNumericEdit_rozestup->Options->FrameNormalColor = clGray;
			scGPNumericEdit_rozestup->Options->FrameWidth = 1;
			// ty co jsou rozdílné
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
					scGPNumericEdit1_rx->Enabled=false;
					break;
				case READONLY:
					scGPNumericEdit_rozestup->Options->ShapeStyle = scgpessNone;
					scGPNumericEdit_rozestup->Enabled = false;
					scGPNumericEdit1_rx->Enabled=true; //zmena oproti rozestupu
					break;
				case HIDE:
					rHTMLLabel_rozestup->Visible = false;
					scGPNumericEdit_rozestup->Visible = false;
					scGPNumericEdit1_rx->Visible=false;
					rHTMLLabel_palec_vzd->Visible=false;
					rHTMLLabel_jednotky_vzdalenostpalcu->Visible=false;
					if (move)offset -= O;
					break;
			}
		} break;
	}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// pøi psaní názvu objektu se mìní titulek a generuje zkratka
void __fastcall TForm_parametry::scGPEdit_nameChange(TObject *Sender) {
		Form_parametry->scLabel_titulek->Caption = scGPEdit_name->Text + " - parametry";
		scGPEdit_shortname->Text = scGPEdit_name->Text.SubString(1, 4).UpperCase();
}
// ---------------------------------------------------------------------------
// pøi zmìnách EDITù

void __fastcall TForm_parametry::scGPNumericEdit_poziceChange(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == P_klik)
		{
					input_P();
					if(scComboBox_rezim->ItemIndex==2) //PP - pokud je CT vyšší než TT nezobrazím zámek u CT
				{
						if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
				}
		}
}

// ----------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_CTChange(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == CT_klik) {

				input_CT(); // pøepoèet hodnot vyplývajících ze zmìny CT
		}
}

// ---------------------------------------------------------------------------
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

	 Nacti_rx();
	 double mezera=0.0;
	 Check_rozmezi_RD();
		if (input_state == NOTHING && input_clicked_edit == mezera_klik && scComboBox_rezim->ItemIndex == 2)
		{ // pokud není zadáváno z jiného vstupu
				input_M(); // lokální pøi PP režimu
		}

		if (input_state == NOTHING)
		{    	//vypis("");

				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&	input_clicked_edit == mezera_klik)
				{
					if (!Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom)) 	vypis("Povolte zmìnu rychlosti pohonu.");  // pokud není pohon používán, dám výpis, jinak výpis nemá smysl - již nelze mìnit rychlost


				} // není zamèeno - doporuèím mezeru, èekám zdali zadá správnou mezeru
				else
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex>0)
				{
							vypis("");
					if (DMunit == MM) // pokud je v milimetrech, tak pøepne na metry
					{
						mezera=scGPNumericEdit_mezera->Value/1000.0;
					}  else  mezera=scGPNumericEdit_mezera->Value;

						if (Kontrola_mezery() == Form1->ms.MyToDouble(mezera))
						{

							 input_M(); // lokální
							 Nacti_rx();
						}
				}  if(scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex==0) input_M();  // pøípad kdy mìním mezeru ale jsem bez pohonu
		}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RD_Change(TObject *Sender) {

		if (input_state == NOTHING && input_clicked_edit == RD_klik)
		{

		if (scComboBox_rezim->ItemIndex==1) // controller pro KK režim
		{

		 input_RD();
		 Check_rozmezi_RD();

		}

	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaChange(TObject *Sender)
{
			if (input_state == NOTHING && input_clicked_edit == C_klik)
			{
				input_K(); // pøepoèet hodnot vyplývajících ze zmìny K
				if(scComboBox_rezim->ItemIndex==2)
				{
				//PP - pokud je CT vyšší než TT nezobrazím zámek u CT
						if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
				}
			}
}

// ---------------------------------------------------------------------------
// pøepoèet hodnot vyplývajících ze zmìny K
void TForm_parametry::input_K() {
		input_state = K;
		INPUT();
		double CT = scGPNumericEdit_CT->Value; // CT od uživatele
		if (CTunit == MIN)CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy
		double K = scGPNumericEdit_kapacita->Value; // K od uživatele

		//if (K > 0)   // shodne volání pro KK i PP režimy
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
						pm.input_K(false); // Memo1->Lines->Add("volam KK  ");//	vypis("Byla zadána neplatná kapacita! Zvolte kapacitu vyšší nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemknìte technologický èas a zaktulizujte hodnoty!",true);
						else//za každé situace standardní volání
						pm.input_K();
					}
		}

		///////////naètení dat zpìt do formuláøe po výpoètu/////////////////////////////////
		 OUTPUT();

			if(scComboBox_rezim->ItemIndex==1 && scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
		{  // u KK probíhá kontrola vypoèítaných dat a doporuèí K pokud RD!=dopRD

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =scGPNumericEdit_CT->Value;
				double K = scGPNumericEdit_kapacita->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }
				if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
				if (DDunit == MM)  DD = DD / 1000.0;
				if (CTunit == M)  CT = CT / 60.0;


			if(dopRD!=RD)
			{
			//Memo1->Lines->Add(DD);
		 //	Memo1->Lines->Add(dopRD);
			//Memo1->Lines->Add(Form1->d.v.PP.TT);
				K =  (DD/dopRD) / (Form1->d.v.PP.TT/60.0);
				vypis("Doporuèená kapacita : " +AnsiString(K) +" ");
				VID=32;
			}

		}
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TForm_parametry::input_P() {

		input_state = P;
		INPUT();

		//if (scGPNumericEdit_pozice->Value > 0)
		{
				double K=pm.P2K(scGPNumericEdit_pozice->Value);
				double CT=scGPNumericEdit_CT->Value;
				if (CTunit == MIN)
				CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy

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
					}// Memo1->Lines->Add("volam KK  ");//	vypis("Byla zadána neplatná kapacita! Zvolte kapacitu vyšší nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemknìte technologický èas a zaktulizujte hodnoty!",true);
					else//za každé situace standardní volání
					pm.input_P();
				}
		}
		// naètení dat zpìt do formuláøe po výpoètu
		OUTPUT();


				if(scComboBox_rezim->ItemIndex==1 && scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
		{  // u KK probíhá kontrola vypoèítaných dat a doporuèí P pokud RD!=dopRD   ImageIndex==38 = odemèen zámek RD

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =scGPNumericEdit_CT->Value;
				double K = scGPNumericEdit_kapacita->Value;
				double Pozice = scGPNumericEdit_pozice->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }
				if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
				if (DDunit == MM)  DD = DD / 1000.0;
				if (CTunit == M)  CT = CT / 60.0;


			if(dopRD!=RD)
			{
				K =  (DD/dopRD) / (Form1->d.v.PP.TT/60.0);
				//Memo1->Lines->Add(K);
				double dop_K =  pm.K2P(K);
				if(dop_K != Pozice )  {
			 // Memo1->Lines->Add(dop_K);
				vypis("Doporuèený poèet pozic : " +AnsiString(dop_K) +" ");
				VID=33; }
			} else vypis("",false);

	 }

		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// pøepoèet hodnot vyplývajících ze zmìny CT
void TForm_parametry::input_CT() {
		input_state = CT;
		INPUT();

		if (scComboBox_rezim->ItemIndex == 1)
	{ //KK režim
						pm.input_CT();
						OUTPUT();
			 if(scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
		 {
				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =0;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }
				if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
				if (DDunit == MM)  DD = DD / 1000.0;


					if(dopRD!=RD)

					{
						CT = DD/dopRD*60.0;
						if(Form1->ms.MyToDouble(CT)!=Form1->ms.MyToDouble(scGPNumericEdit_CT->Value))
						{
						vypis("Doporuèený techn.èas : " +AnsiString(CT) +" s ");
						VID=30;
						}    else vypis("",false);
					}


		 }
	}

		if (scComboBox_rezim->ItemIndex == 0)
		{ // pokud je v režimu SG nevolá se výpoèetní model
						OUTPUT();
		}

			if (scComboBox_rezim->ItemIndex == 2)
				{
								pm.input_CT();
								OUTPUT();
				}

		input_state = NOTHING;

}

// ---------------------------------------------------------------------------
// pøepoèet hodnot vyplývajících ze zmìny DD
void TForm_parametry::input_DD() {
		input_state = DD;
		INPUT();
		if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==1) // KK
		{
				pm.input_DD(); // zavolání výpoèetního modelu
				OUTPUT();

			if(scButton_zamek_RD->ImageIndex==38 && scComboBox_pohon->ItemIndex>0)
			{

				double roztec=0;
				double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN) RD=scGPNumericEdit_RD->Value/60;
				double DD=scGPNumericEdit_delka_dopravniku->Value;
				double CT =scGPNumericEdit_CT->Value;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0; }
				if (RDunitD == MM) {dopRD /= 1000.0;  RD /= 1000.0;}
				if (DDunit == MM) { DD = DD / 1000.0; }
				if (CTunit == M)  CT = CT / 60.0;

					if(dopRD!=RD)

					{

					double	 DD_dop = CT*dopRD;

				 //	Memo1->Lines->Add(DD_dop);
				 //	Memo1->Lines->Add(DD);

						if(Form1->ms.MyToDouble(DD_dop)!= Form1->ms.MyToDouble(DD))
						{
						vypis("Doporuèená délka kabiny : " +AnsiString(DD_dop) +" [m] ");
						VID=31;
						} else vypis("",false);
					}

			}


		}


			if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==2) // KK a PP režim
				// nutné ošetøení pro období zadávání/psaní
		{
				pm.input_DD(); // zavolání výpoèetního modelu
				OUTPUT();
		}

			if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==0)
		{
				OUTPUT(); // u SG režimu nevolám výpoèetní model, output je volán pouze pro validaci dat
		}

		///////////naètení dat zpìt do formuláøe po výpoètu/////////////////////////////////

		input_state = NOTHING;
}
// -----------------------------------------------------------------------------

void TForm_parametry::input_M() {
		input_state = mezera;
		INPUT();
		 //	ShowMessage("volano");
		pm.input_M();
		///////////naètení dat zpìt do formuláøe po výpoètu/////////////////////////////////
		OUTPUT();
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// pøepoèet hodnot vyplývajících ze zmìny RD, pouze pro kontinual
void TForm_parametry::input_RD() {
			input_state = RD;
			INPUT();
				pm.input_RD();
		///////////naètení dat zpìt do formuláøe po výpoètu/////////////////////////////////
		OUTPUT();


		input_state = NOTHING;
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// vynuluje vstupní hodnoty
void TForm_parametry::null_input_value() {
		// scGPNumericEdit_RD->Value=0;
		// scGPNumericEdit_CT->Value=0;
		// scGPNumericEdit_delka_dopravniku->Value=0;
		// scGPNumericEdit_kapacita->Value=0;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TForm_parametry::vykresli_vozik(bool na_delku) { /* ZDM
		 Image_vozik->Canvas->Pen->Color=clBtnFace;
		 Image_vozik->Canvas->Pen->Width=1;
		 Image_vozik->Canvas->Brush->Color=clBtnFace;
		 Image_vozik->Canvas->Rectangle(0,0,Image_vozik->Width,Image_vozik->Height);//vybarvení celého
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
		 //sipka -levá
		 Image_vozik->Canvas->Brush->Color=clGray;
		 Image_vozik->Canvas->Pen->Color=clBlack;
		 Image_vozik->Canvas->Pen->Width=1;
		 POINT bodyL[3]={{stred.x-D/2*meritko-13,stred.y-3},{stred.x-D/2*meritko-7,stred.y},{stred.x-D/2*meritko-13,stred.y+3}};
		 Image_vozik->Canvas->Polygon((TPoint*)bodyL,2);
		 //sipka - pravá
		 POINT bodyP[3]={{stred.x+D/2*meritko+7,stred.y-3},{stred.x+D/2*meritko+13,stred.y},{stred.x+D/2*meritko+7,stred.y+3}};
		 Image_vozik->Canvas->Polygon((TPoint*)bodyP,2);
		 //vozík
		 Image_vozik->Canvas->Pen->Width=2;
		 Image_vozik->Canvas->Brush->Color=clWhite;
		 Image_vozik->Canvas->Rectangle(stred.x-D/2*meritko,stred.y-S/2*meritko,stred.x+D/2*meritko,stred.y+S/2*meritko);
		 //popisek
		 Image_vozik->Canvas->Font->Size=7;
		 Image_vozik->Canvas->TextOutW(stred.x-Image_vozik->Canvas->TextWidth(D)/2,stred.y+S/2*meritko-Image_vozik->Canvas->TextHeight(D)-2,D);
		 Image_vozik->Canvas->TextOutW(stred.x-D/2*meritko+2,stred.y-Image_vozik->Canvas->TextHeight(S)/2,S);
		 */
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender) {
		// vykresli_vozik(true);//na délku
		// ValueListEditorStringsChange(Sender);//zajistí pøepoèítání hodnot
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender) {
		// vykresli_vozik(false);//na šíøku
		// ValueListEditorStringsChange(Sender);//zajistí pøepoèítání hodnot
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender) {
		// if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
		// else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}

// ---------------------------------------------------------------------------
// pøepínání zobrazení min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender)
{
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double RD = scGPNumericEdit_RD->Value;
		double CT = scGPNumericEdit_CT->Value;
		if (minsec == MIN) // pokud je v minutách, tak pøepne na sekundy
		{
				minsec = S;
				scGPButton_min_sec->Caption = "vše na min";
				// samotné tlaèítko,ukazuje název opaènì
				// CT - pøepoèítání
				if(CTunit != S)
				{
					CTunit = S;
					CT = scGPNumericEdit_CT->Value * 60.0;
					rHTMLLabel_CT->Caption = "Technologický èas <font color=#2b579a>[s]</font>";
				}
				// RD - pøepoèítání
				if(RDunitT != S)
				{
					RDunitT = S;
					if (RDunitD == MM) rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v milimetrech
					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v metrech
					RD = scGPNumericEdit_RD->Value / 60.0;
				}
		}
		else // pokud je v sekundách pøepne na minuty
		{
				minsec = MIN;
				scGPButton_min_sec->Caption = "vše na s";
				// samotné tlaèítko,ukazuje název opaènì
				// CT - pøepoèítání
				if(CTunit != MIN)
				{
					CTunit = MIN;
					CT = scGPNumericEdit_CT->Value / 60.0;
					rHTMLLabel_CT->Caption = "Technologický èas <font color=#2b579a>[min]</font>";
				}
				// RD - pøepoèítání
				if(RDunitT != MIN)
				{
					RDunitT = MIN;
					if (RDunitD == MM) rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v milimetrech
					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v metrech
					RD = scGPNumericEdit_RD->Value * 60.0;
				}
		}
		// plnìní + poèet desetinných míst
		scGPNumericEdit_CT->Value = CT;
		scGPNumericEdit_RD->Value = RD;
		Form1->aktualizace_combobox_pohony_v_PO(RDunitD,RDunitT);//zaktualizovat výpis + o jednotky
		input_state = NOTHING; // už se mohou pøepoèítávat
}
// ---------------------------------------------------------------------------
// požadavek na zmìnu jednotek CT
void __fastcall TForm_parametry::rHTMLLabel_CTClick(TObject *Sender)
{
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double CT = 0.0;
		if (CTunit == MIN) // pokud je v minutách, tak pøepne na sekundy
		{
				CTunit = S;
				// CT - pøepoèítání
				CT = scGPNumericEdit_CT->Value * 60.0;
				rHTMLLabel_CT->Caption =
						"Technologický èas <font color=#2b579a>[s]</font>";
		}
		else // pokud je v sekundách pøepne na minuty
		{
				CTunit = MIN;
				// CT - pøepoèítání
				CT = scGPNumericEdit_CT->Value / 60.0;
				rHTMLLabel_CT->Caption =
						"Technologický èas <font color=#2b579a>[min]</font>";
		}
		// plnìní + poèet desetinných míst
		// ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
		scGPNumericEdit_CT->Value = CT;
		input_state = NOTHING; // už se mohou pøepoèítávat
}

// ---------------------------------------------------------------------------
// požadavek na zmìnu jednotek RD
void __fastcall TForm_parametry::rHTMLLabel_RDClick(TObject *Sender)
{
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double RD = 0.0;

		if(RDunitT == MIN)// pokud je v minutách, tak pøepne na sekundy
		{
				RDunitT = S;
				if(RDunitD == MM)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v milimetrech
				else rHTMLLabel_RD->Caption =	"Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v metrech
				RD = scGPNumericEdit_RD->Value / 60.0;
				// RD=RDunitD_funkce(RD); tady opravdu nesmí být
		}
		else // pokud je v sekundách pøepne na minuty
		{
				RDunitT = MIN;
				if(RDunitD == MM)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v milimetrech
				else rHTMLLabel_RD->Caption ="Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v metrech
				RD = scGPNumericEdit_RD->Value * 60.0;
				//zrušeno užití RD = RDunitD_funkce(RD);//tady ano
		}
		//zrušeno užití RD = RDunitD_funkce(RD);//ano tady opravdu znovu

		// plnìní + poèet desetinných míst
		scGPNumericEdit_RD->Value = RD;
		F->aktualizace_combobox_pohony_v_PO(RDunitD,RDunitT);//zaktualizovat výpis + o jednotky
		input_state = NOTHING; // už se mohou pøepoèítávat
}
// ---------------------------------------------------------------------------
//zrušeno užití
double TForm_parametry::RDunitD_funkce(double RD)// podpùrná metoda výše uvedené
{
//		if (RDunitD == MM) // pokud je v milimetrech, tak pøepne na metry
//		{
//				RDunitD = M;
//				if (RDunitT == MIN)	rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v minutách
//				else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>"; // pokud je v sekundách
//				return RD / 1000.0;
//		}
//		else //pokud je v metrech, tak pøepne na milimetry
//		{
//				RDunitD = MM;
//				if (RDunitT == MIN)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v minutách
//				else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekundách
//				return RD * 1000.0;
//		}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// pøepínání zobrazení m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender)
{
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double DD = scGPNumericEdit_delka_dopravniku->Value;//naplnìní hodnot zde, pro pøípad, že by se pøepínalo z jednotek do stejných jednotek
		double DM = scGPNumericEdit_mezera->Value;//naplnìní hodnot zde, pro pøípad, že by se pøepínalo z jednotek do stejných jednotek
		double Rz = scGPNumericEdit_rozestup->Value;//naplnìní hodnot zde, pro pøípad, že by se pøepínalo z jednotek do stejných jednotek
		double RD = scGPNumericEdit_RD->Value;//naplnìní hodnot zde, pro pøípad, že by se pøepínalo z jednotek do stejných jednotek

		if (m_mm == MM) // pokud je v milimetrech, tak pøepne na metry
		{
				m_mm = M;scGPButton_metry_milimetry->Caption = "vše na mm";
				// samotné tlaèítko,ukazuje název opaènì
				// DD
				if(DDunit != M)
				{
					DDunit = M;
					rHTMLLabel_delka_dopravniku->Caption ="Délka kabiny <font color=#2b579a>[m]</font>";
					DD = scGPNumericEdit_delka_dopravniku->Value / 1000.0;
				}
				// DM + Rz
				if(DMunit != M)
				{
					DMunit = M;
					rHTMLLabel_mezera->Caption = "Mezera mezi vozíky <font color=#2b579a>[m]</font>";
					DM = scGPNumericEdit_mezera->Value / 1000.0;
					rHTMLLabel_jednotky_vzdalenostpalcu->Caption=" <font color=#2b579a>[m]";
					Rz = scGPNumericEdit_rozestup->Value / 1000.0;
				}
				// RD //zrušeno užití
//				if(RDunitD != M)
//				{
//					RDunitD = M;
//					if (RDunitT == MIN)rHTMLLabel_RD->Caption ="Rychlost pohonu <font color=#2b579a>[m/min]</font>";// pokud je v minutách
//					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekundách
//					RD = scGPNumericEdit_RD->Value / 1000.0;
//				}
		}
		else // pokud je metrech, tak pøepne na milimetry
		{
				m_mm = MM;scGPButton_metry_milimetry->Caption = "vše na m";
				// samotné tlaèítko,ukazuje název opaènì
				// DD
				if(DDunit != MM)
				{
					DDunit = MM;
					rHTMLLabel_delka_dopravniku->Caption = "Délka kabiny <font color=#2b579a>[mm]</font>";
					DD = scGPNumericEdit_delka_dopravniku->Value * 1000.0;
				}
				// DM + Rz
				if(DMunit != MM)
				{
					DMunit = MM;
					rHTMLLabel_mezera->Caption = "Mezera mezi vozíky <font color=#2b579a>[mm]</font>";
					DM = scGPNumericEdit_mezera->Value * 1000.0;
					rHTMLLabel_jednotky_vzdalenostpalcu->Caption=" <font color=#2b579a>[mm]";
					Rz = scGPNumericEdit_rozestup->Value * 1000.0;
				}
				// RD  //zrušeno užití
//				if(RDunitD != MM)
//				{
//					RDunitD = MM;
//					if (RDunitT == MIN)rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/min]</font>";// pokud je v minutách
//					else rHTMLLabel_RD->Caption = "Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekundách
//					RD = scGPNumericEdit_RD->Value * 1000.0;
//				}
		}
		// plnìní + poèet desetinných míst
		scGPNumericEdit_delka_dopravniku->Value = DD;
		scGPNumericEdit_mezera->Value = DM;
		scGPNumericEdit_rozestup->Value=Rz;
		scGPNumericEdit_RD->Value = RD;
		Form1->aktualizace_combobox_pohony_v_PO(RDunitD,RDunitT);//zaktualizovat výpis + o jednotky
		input_state = NOTHING; // už se mohou pøepoèítávat
}
// ---------------------------------------------------------------------------
// požadavek na zmìnu jednotek DD
void __fastcall TForm_parametry::rHTMLLabel_delka_dopravnikuClick
		(TObject *Sender) {
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double DD = 0.0;
		if (DDunit == MM) // pokud je v milimetrech, tak pøepne na metry
		{
				DDunit = M;
				rHTMLLabel_delka_dopravniku->Caption =
						"Délka kabiny <font color=#2b579a>[m]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value / 1000.0;
		}
		else // pokud je metrech, tak pøepne na milimetry
		{
				DDunit = MM;
				rHTMLLabel_delka_dopravniku->Caption =
						"Délka kabiny <font color=#2b579a>[mm]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value * 1000.0;
		}
		// plnìní + poèet desetinných míst
		// ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
		scGPNumericEdit_delka_dopravniku->Value = DD;
		input_state = NOTHING; // už se mohou pøepoèítávat
}

// ---------------------------------------------------------------------------
// požadavek na zmìnu jednotek DM
void __fastcall TForm_parametry::rHTMLLabel_mezeraClick(TObject *Sender)
{
		input_state = NO; // zámìr, aby se nepøepoèítavaly hodnoty
		double DM = 0.0;
		double Rz=0.0;
		if (DMunit == MM) // pokud je v milimetrech, tak pøepne na metry
		{
				// DM
				DMunit = M;
				rHTMLLabel_mezera->Caption = "Mezera mezi vozíky <font color=#2b579a>[m]</font>";
				DM = scGPNumericEdit_mezera->Value / 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption=" <font color=#2b579a>[m]";
				Rz = scGPNumericEdit_rozestup->Value / 1000.0;
		}
		else // pokud je metrech, tak pøepne na milimetry
		{
				// DM
				DMunit = MM;
				rHTMLLabel_mezera->Caption = "Mezera mezi vozíky <font color=#2b579a>[mm]</font>";
				DM = scGPNumericEdit_mezera->Value * 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption="<font color=#2b579a>[mm]";
				Rz = scGPNumericEdit_rozestup->Value * 1000.0;
		}
		// plnìní + poèet desetinných míst
		scGPNumericEdit_mezera->Value = DM;
		scGPNumericEdit_rozestup->Value=Rz;
		input_state = NOTHING; // už se mohou pøepoèítávat
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// pøi stisku klávesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
		if (Key == 13) // ENTER
		{
				if (scGPButton_OK->Enabled)
						// pokud jsou zároveò splnìny podmínky pro stisk OK
				{
						Form_parametry->ModalResult = mrOk;
						// vrátí stejnou hodnotu jako tlaèítko
						Form_parametry->VisibleChanging();
						// skryje form, stejné jako visible=false
				}
				else
						MessageBeep(0); // pípnutím upozorní, že nelze
		}
		if (Key == 27) // ESC
		{
				Form_parametry->ModalResult = mrCancel;
				// vrátí stejnou hodnotu jako tlaèítko
				Form_parametry->VisibleChanging();
				// skryje form, stejné jako visible=false
		}
		if (Key == 123 && DEBUG) // F12 + pouze v DEBUGU
		{
				Memo1->Visible = true;
				Memo1->Lines->Clear();
			 //	Memo1->Lines->Add(pm.T);
				Memo1->Top = 0;
				Memo1->Left = 0;
		}
		if(Key==116 && Shift.Contains(ssCtrl))//ctrl+F5
		{
				if(mrYes==F->MB("Chcete nastavit výchozí hodnoty parametrù? Nastavením výchozích hodnot boudou souèasné hodnoty parametrù ztraceny!",MB_YESNO))
				{
						//nastavení hodnot parametrù do default
        }
		}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// pøi stisku storna
void __fastcall TForm_parametry::scGPButton_stornoClick(TObject *Sender) {
		kapacitaSG = 1; // nastaví na default
}

// ---------------------------------------------------------------------------
// pøi stisku storna
void __fastcall TForm_parametry::KonecClick(TObject *Sender) {
		kapacitaSG = 1; // nastaví na default
}

// ---------------------------------------------------------------------------
// uloží do INI nastavení formuláøe
void __fastcall TForm_parametry::FormClose(TObject *Sender,
		TCloseAction &Action) {
		Form1->writeINI("nastaveni_form_parametry", "cas", minsec);
		Form1->writeINI("nastaveni_form_parametry", "vzdalenost", m_mm);
		Form1->writeINI("nastaveni_form_parametry", "CT", CTunit);
	 	Form1->writeINI("nastaveni_form_parametry", "RDt", RDunitT);
		Form1->writeINI("nastaveni_form_parametry", "RDd", RDunitD);
		Form1->writeINI("nastaveni_form_parametry", "DD", DDunit);
		Form1->writeINI("nastaveni_form_parametry", "DM", DMunit);
}

// ---------------------------------------------------------------------------
// zkopíruje parametry objektu pro další použití
void __fastcall TForm_parametry::scGPGlyphButton_copyClick(TObject *Sender)
{
		// povolí tlaèítko vkládání
		scGPGlyphButton_paste->Enabled = true;

		// pøevod jednotek
		double jednotky_cas = 1.0;if (minsec == MIN)jednotky_cas = 60.0;
		double jednotky_vzdalenost = 1.0;if (m_mm == MM)jednotky_vzdalenost = 1000.0;

		// text do schránky
		AnsiString T = "";

		// atributy
		Form1->copyObjekt->name=scGPEdit_name->Text;T=scGPEdit_name->Text;T+=";";
		Form1->copyObjekt->short_name=scGPEdit_shortname->Text;T+=scGPEdit_shortname->Text;T+=";";
		Form1->copyObjekt->rezim=scComboBox_rezim->ItemIndex;T+=scComboBox_rezim->ItemIndex;T+=";";
		Form1->copyObjekt->pohon=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);/* indexuje se od nuly */ T+=scComboBox_pohon->ItemIndex;T+=";";
		Form1->copyObjekt->CT=scGPNumericEdit_CT->Value*jednotky_cas;T+=Form1->copyObjekt->CT;T+=";";
		Form1->copyObjekt->RD=scGPNumericEdit_RD->Value/jednotky_cas/jednotky_vzdalenost;T+=Form1->copyObjekt->RD;T+=";";
		Form1->copyObjekt->delka_dopravniku=scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;T+= scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;T += ";";
		Form1->copyObjekt->kapacita=scGPNumericEdit_kapacita->Value;T+=Form1->copyObjekt->kapacita;T+=";";
		Form1->copyObjekt->pozice=scGPNumericEdit_pozice->Value;T+=Form1->copyObjekt->pozice;T+=";";
		Form1->copyObjekt->odchylka=scGPNumericEdit_odchylka->Value;T+=Form1->copyObjekt->odchylka;T+=";";
		Form1->copyObjekt->cekat_na_palce = scComboBox_cekani_palec->ItemIndex;T+=scComboBox_cekani_palec->ItemIndex;T+=";";
		Form1->copyObjekt->stopka=scComboBox_stopka->ItemIndex;T+=Form1->copyObjekt->stopka;T+=";";
		Form1->copyObjekt->rotace=scComboBox_rotace->ItemIndex;T+=Form1->copyObjekt->rotace;T+=";";
		Form1->copyObjekt->mezera=scGPNumericEdit_mezera->Value/jednotky_vzdalenost;T+=Form1->copyObjekt->mezera;T+=";";
		Form1->copyObjektRzRx.x=scGPNumericEdit1_rx->Value;T+=Form1->copyObjektRzRx.x;T+=";";
		Form1->copyObjektRzRx.y=scGPNumericEdit_rozestup->Value/jednotky_vzdalenost;T+=Form1->copyObjektRzRx.x;T+=";";
		//ještì zvážit kopírování nastavení zámkù


		// zkopírování textu do schránky
		TClipboard *c = new TClipboard;
		c->AsText = T;
		c = NULL;
		delete c;
}
// ---------------------------------------------------------------------------
// zajišuje vložení objektu
void __fastcall TForm_parametry::scGPGlyphButton_pasteClick(TObject *Sender)
{
		// aby se nevolalo pøepoèítávání pøi editu položek
		input_state = NO;

		// pøevod jednotek
		double jednotky_cas = 1.0;if (minsec == MIN)jednotky_cas = 60.0;
		double jednotky_vzdalenost = 1.0;if (m_mm == MM)jednotky_vzdalenost = 1000.0;

		scGPEdit_name->Text = Form1->copyObjekt->name;
		scGPEdit_shortname->Text = Form1->copyObjekt->short_name;
		scComboBox_rezim->ItemIndex = Form1->copyObjekt->rezim;
		if (Form1->copyObjekt->pohon == NULL)scComboBox_pohon->ItemIndex=0;
		else scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n;
		scGPNumericEdit_CT->Value=Form1->copyObjekt->CT/jednotky_cas;
		scGPNumericEdit_RD->Value=Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost;
		scGPNumericEdit_delka_dopravniku->Value=Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost;
		scComboBox_cekani_palec->ItemIndex=Form1->copyObjekt->cekat_na_palce;
		scGPNumericEdit_kapacita->Value=Form1->copyObjekt->kapacita;
		scGPNumericEdit_odchylka->Value=Form1->copyObjekt->odchylka;
		scComboBox_stopka->ItemIndex=Form1->copyObjekt->stopka;
		scComboBox_rotace->ItemIndex=Form1->copyObjekt->rotace;
		scGPNumericEdit_mezera->Value=Form1->copyObjekt->mezera*jednotky_vzdalenost;
		scGPNumericEdit1_rx->Value=Form1->copyObjektRzRx.x;
		scGPNumericEdit_rozestup->Value=Form1->copyObjektRzRx.y*jednotky_vzdalenost;
		//ještì zvážit pøebírání stavu zámkù

		// nastavení aktuálního režimu objektu
		setForm4Rezim(scComboBox_rezim->ItemIndex);

		// povolení pøepoèítávání
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// zámek procesního èasu
void __fastcall TForm_parametry::scButton_zamek_CTClick(TObject *Sender)
{
	 	Nastav_zamky(scComboBox_rezim->ItemIndex, CT_klik_ico, empty_klik, true);

		INPUT();
		double	K=scGPNumericEdit_kapacita->Value;
		double  CT=scGPNumericEdit_CT->Value; // CT od uživatele
		if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy

		if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT){
		// Memo1->Lines->Add("volam KK s false ");
		pm.input_K(false); }
		else
		{
			if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
			{;} // Memo1->Lines->Add("volam KK  ");//	vypis("Byla zadána neplatná kapacita! Zvolte kapacitu vyšší nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemknìte technologický èas a zaktulizujte hodnoty!",true);
			else//za každé situace standardní volání
			pm.input_K();
		}
	 	OUTPUT();
}

// ---------------------------------------------------------------------------
// zámek délky dopravníku
void __fastcall TForm_parametry::scButton_zamek_DDClick(TObject *Sender) {
		Nastav_zamky(scComboBox_rezim->ItemIndex, DD_klik_ico, empty_klik, true);

}

void __fastcall TForm_parametry::scButton_K_zamekClick(TObject *Sender) {
		Nastav_zamky(scComboBox_rezim->ItemIndex, C_klik_ico, empty_klik, true);
}

// ---------------------------------------------------------------------------
// pøi kliknutí na doporuèení nastane aplikace doporuèení do daného editboxu
void __fastcall TForm_parametry::rHTMLLabel_InfoTextClick(TObject *Sender)
{
//		if (rHTMLLabel_InfoText->Caption.Pos("Doporuèený technologický èas je")) {
//				if (CTunit == MIN)
//						scGPNumericEdit_CT->Value =
//								Form1->m.round(scGPNumericEdit_kapacita->Value)
//								* Form1->d.v.PP.TT / 60.0;
//				else
//						scGPNumericEdit_CT->Value =
//								Form1->m.round(scGPNumericEdit_kapacita->Value)
//								* Form1->d.v.PP.TT;
//		}
//		if (rHTMLLabel_InfoText->Caption.Pos("Doporuèená kapacita je 1!")) {
//				scGPNumericEdit_kapacita->Value = 1;
//		}
//		if (rHTMLLabel_InfoText->Caption.Pos
//				("Zmìnte režim nebo rozložte do více objektù!")) {
//				scComboBox_rezim->ItemIndex = 1;
//		}
		switch(VID)
		{
			case -1:break;
			case 0:break;
			case 11:break;
			case 12:break;
			case 13:break;
			case 14:break;
			case 15:break;
			case 16:break;
			case 22:break;
			case 23:break;
			case 24:break;
			case 25:break;
			case 26:break;
			case 27:break;
			case 28:break;
			case 40:break;
			case 41:break;
			case 42:break;
			case 44:break;
		}
}
// ---------------------------------------------------------------------------
// kontrola vybraného pohonu vùèi zadané rychlosti dopravníku
void __fastcall TForm_parametry::scComboBox_pohonChange(TObject *Sender)
{
		INPUT();
	 	OUTPUT();
		Pohon_pouzivan();
		Nacti_rx();
		if(scComboBox_rezim->ItemIndex!=1)
		{
			scButton_zamek_CT->Enabled=true;
			scButton_zamek_DD->Enabled=true;
		}
		if (scComboBox_pohon->ItemIndex != 0 && scComboBox_rezim->ItemIndex==1 )
		{   // POKUD je pohon již používán, natáhnu si jeho data
				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
				if (obj!=NULL)
				{
				double RD=obj->RD;
				if (RDunitT == MIN)RD *= 60.0;
				if (RDunitD == MM) RD /= 1000.0;

						 scGPNumericEdit_RD->Value=RD;
						 scGPNumericEdit_mezera->Value=obj->mezera;

					if(obj->rotace==0) scComboBox_rotace->ItemIndex=0;
					else scComboBox_rotace->ItemIndex=1;

						if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // pøi pøechodu mezi pohony, zkontroluje zdali je mezera v poøádku, pouze u KK režimu
				}
				else
				{
					scButton_zamek_RD->Enabled=true;  // pokud pohon není používán povolím zobrazení zámku RD
					if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // pøi pøechodu mezi pohony, zkontroluje zdali je mezera v poøádku, pouze u KK režimu
				}
		}
}
// ---------------------------------------------------------------------------
// doplnit komentáø
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaClick(TObject *Sender)
{
		input_clicked_edit = C_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, C_klik, false);
}

// --------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_mezeraClick(TObject *Sender) {
		input_clicked_edit = mezera_klik;
		if(scComboBox_rezim->ItemIndex==1) scGPNumericEdit_mezera->Visible=true;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico,mezera_klik, false);
}

// ----------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_poziceClick(TObject *Sender) {
		input_clicked_edit = P_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, P_klik, false);
}

// ---------------------------------------------------------------------------
// doplnit komentáø
void __fastcall TForm_parametry::scGPNumericEdit_CTClick(TObject *Sender) {
		input_clicked_edit = CT_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, CT_klik, false);
}

void __fastcall TForm_parametry::scComboBox_rotaceClick(TObject *Sender) {
		if(form_zobrazen){
		input_clicked_edit = Rotace_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico,
				Rotace_klik, false);   }
}

// ---------------------------------------------------------------------------
// doplnit komentáø
void __fastcall TForm_parametry::scGPNumericEdit_RD_Click(TObject *Sender) {
		input_clicked_edit = RD_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, RD_klik, false);

}

// ---------------------------------------------------------------------------
// doplnit komentáø
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuClick
		(TObject *Sender)

{
		input_clicked_edit = DD_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, DD_klik, false);

}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::Button_dopravnik_parametryClick(TObject *Sender)
{
		//!!!!!toto dostane možná trochu jinou podobu podle nové úvahy, pravdìpodobnì za situace, že bude námìt na pøídání nového pohonu (bude vysvíceno tlaèítko na pøidání pohonu)
		bool priradit_posledni=false; bool aktualizace_combo_pohony=false;
		unsigned int n_pohonu=F->d.v.POHONY->predchozi->n;
		if(scComboBox_pohon->ItemIndex==0)priradit_posledni=true;//když ještì nebyl pøiøazen, jinak mu nevnucovat jiný pohon, aby mohlo tlaèítko také dále fungovat ke správì pohonù

		//zobrazení formu PL
		Form_parametry_linky->zobrazitFrameForm=true;
		//nemùže tady být toto!!!:Form1->Button_dopravnik_parametryClick(Sender) ale musí být toto:
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Form_parametry->Visible=false;//nutnost
		if(mrOk!=Form_parametry_linky->ShowModal())//pøípadná návratová hodnota se øeši v knihovnì
			priradit_posledni=false;//pokud bude stisknuto storno tak se urèitì nepøiøazuje
		else
			aktualizace_combo_pohony=true;
		Form_parametry->Visible=true;//nutnost
		Form_parametry_linky->zobrazitFrameForm=false;

		if(F->d.v.POHONY->dalsi!=NULL)//pokud již existuje pohon
		{
			scComboBox_pohon->Enabled=true;//povolení vybírání pohonu - musí být samostatnì
			if(aktualizace_combo_pohony)//nebylo na PL stisknuto storno
			{
				Form1->aktualizace_combobox_pohony_v_PO(RDunitD,RDunitT);//zaktualizovat výpis + o jednotky
				//POKUD NEBYL PÙVODNÌ PØIØAZEN POHON, NEBYLO STISKNUTO STORNO, PØIBYL NOVÝ POHON A UŽIVATEL S TÍM SOUHLASÍ
				if(priradit_posledni && n_pohonu<F->d.v.POHONY->predchozi->n && mrYes==F->MB("Chcete pøiøadit poslední vytvoøený pohon?",MB_YESNO))
				scComboBox_pohon->ItemIndex=Form1->d.v.POHONY->predchozi->n;//pøíøadíme poslení vytvoøený pohon - to je trochu na zvážení, zda mu ho takto podsouvat ale s MB asi OK
			}
		}
		else scComboBox_pohon->Enabled=false;//i tato vìtev má význam, pokud by již novì neexistoval

		//pokud je náhled z PO zobrazen, zajišuje zároveò okamžitou aktualizaci hodnot v náhledu z PO
		if(Form_objekt_nahled->Visible)Form_objekt_nahled->REFRESH_DATA();//obnoví dat ve formu Form_objekt_nahled vèetnì pøekreslení
}
// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scButton_zamek_RDClick(TObject *Sender)
{
		Nastav_zamky(scComboBox_rezim->ItemIndex, RD_klik_ico, empty_klik, true);
}
// ---------------------------------------------------------------------------
void TForm_parametry::INPUT()
{
		/////////////////// nacteni vsech hodnot z formulare do lokalnich promennych////////////////////////////////////////////////////
		short rezim;
		if (scComboBox_rezim->ItemIndex == 0)	rezim = 0; // S&G
		if (scComboBox_rezim->ItemIndex == 1) rezim = 1; // Kontinual
		if (scComboBox_rezim->ItemIndex == 2) rezim = 2; // Postprocesni

		double TT = Form1->d.v.PP.TT;
		double CT = scGPNumericEdit_CT->Value; // CT - novì zadáno uživatelem
		double RD = scGPNumericEdit_RD->Value; // RD	od uživatele
		double DD = scGPNumericEdit_delka_dopravniku->Value; // DD od uživatele
		double K = scGPNumericEdit_kapacita->Value; // K od uživatele
		double Odchylka = scGPNumericEdit_odchylka->Value; // odchylka od uživatele
		//short Nasleduje_cekani = scComboBox_cekani_palec->ItemIndex;// 0 - ne, 1 -ano, 2 - automaticky
		//short Stop_stanice = scComboBox_stopka->ItemIndex;// 0 - ne, 1 -ano, 2 - automaticky
		double dV = Form1->d.v.PP.delka_voziku; // délka jigu
		double sV = Form1->d.v.PP.sirka_voziku; // šíøka jigu
		double m = scGPNumericEdit_mezera->Value; // mezera mezi voziky
		double P = scGPNumericEdit_pozice->Value; // poèet pozic

		bool CT_locked;
		bool RD_locked;
		bool DD_locked;
		bool K_locked;

		double rotace;
		if (scComboBox_rotace->ItemIndex == 0)rotace = 0; // na délku
		if (scComboBox_rotace->ItemIndex == 1)rotace = 90; // na šíøku

		//////////////////////// stavy zamku/////////////////////////////////////
		if (CT_zamek == LOCKED)CT_locked = true;  else CT_locked = false;
		if (RD_zamek == LOCKED) RD_locked = true; else RD_locked = false;
		if (DD_zamek == LOCKED) DD_locked = true;	else DD_locked = false;
		if (K_zamek == LOCKED) K_locked = true;   else K_locked = false;

		//////////////////////// prevody jednotek///////////////////////////////
		if (CTunit == MIN) CT *= 60.0;// pokud bylo zadání v minutách pøevede na sekundy - jinak je CT v Si a mohu ho hned uložit k výpoètu
		if (RDunitT == MIN)RD *= 60.0;
		if (RDunitD == MM) RD /= 1000.0;//již využítí zrušeno, nicménì nevadí zanechání
		if (DDunit == MM)	 DD /= 1000.0; // vždy ukládám do metrù
		if (DMunit == MM)		m /= 1000.0;

		///////////////uložení do výpoèetního modulu PO/////////////////////////
		pm.rezim = rezim;
		pm.TT = TT;
		pm.CT = CT;
		pm.RD = RD;
		pm.DD = DD;
		pm.K = K;
		pm.M = m;
		pm.P = P;
		pm.dV = dV;
		pm.sV = sV;
		pm.Rotace = rotace;
		pm.CT_locked = CT_locked;
		pm.RD_locked = RD_locked;
		pm.DD_locked = DD_locked;
		pm.K_locked = K_locked;
		Cvektory::TPohon *Pohon = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if (Pohon != NULL) pm.R = Pohon->roztec; else pm.R = 0;
		Pohon=NULL;delete Pohon;
}
// ---------------------------------------------------------------------------
void TForm_parametry::OUTPUT()
{
					// pokud bylo zadáváno napø. z input_CT nebudu z math modelu tento údaj znovu natahovat do formu,
					// zpùsobuje to totiž pøepisování hodnot pøi zadávání hodnoty do editboxu
					// takto jsou ošetøeny všechny vstupy
		if (input_state != CT)
		{
				scGPNumericEdit_CT->Value = pm.CT;
				if (CTunit == MIN)scGPNumericEdit_CT->Value /= 60.0;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_CT->Decimal=2;

		}
		if (input_state != RD)
		{
		 Memo1->Lines->Add("RD_UnitT OUTPUT: "+AnsiString(RDunitT));
		 Memo1->Lines->Add("RD OUTPUT: "+AnsiString(pm.RD));
			//	 RDunitT=S;
				scGPNumericEdit_RD->Value = pm.RD;
				if (RDunitT == MIN) scGPNumericEdit_RD->Value /= 60.0;
				if (RDunitD == MM) scGPNumericEdit_RD->Value *= 1000.0;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_RD->Decimal=2;

		}
		if (input_state != DD)
		{
				scGPNumericEdit_delka_dopravniku->Value = pm.DD;
				if (DDunit == MM)scGPNumericEdit_delka_dopravniku->Value *= 1000.0;
				scGPNumericEdit_delka_dopravniku->Hint=scGPNumericEdit_delka_dopravniku->Value;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_delka_dopravniku->Decimal=2;
		}
		if (input_state != K)
		{
				scGPNumericEdit_kapacita->Value = pm.K;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_kapacita->Decimal=2;
		}
		if (input_state != mezera)
		{
				scGPNumericEdit_mezera->Value = pm.M;
			 //	Memo1->Lines->Add("OUTPUT:"+AnsiString(pm.M));
				if (DMunit == MM)
				scGPNumericEdit_mezera->Value *= 1000.0;
				if(scGPCheckBox_zaokrouhlit->Checked)	scGPNumericEdit_mezera->Decimal=3;
		}
		if (input_state != P)
		{
				scGPNumericEdit_pozice->Value = pm.P;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_pozice->Decimal=2;
		}

		//ZOBRAZENÍ HINTÙ
			scGPNumericEdit_CT->Hint=scGPNumericEdit_CT->Value;
			scGPNumericEdit_RD->Hint=scGPNumericEdit_RD->Value;
			scGPNumericEdit_delka_dopravniku->Hint=scGPNumericEdit_delka_dopravniku->Value;
			scGPNumericEdit_kapacita->Hint=scGPNumericEdit_kapacita->Value;
		 //	scGPNumericEdit_odchylka->Hint=Odchylka;
			scGPNumericEdit_mezera->Hint=scGPNumericEdit_mezera->Value;
			scGPNumericEdit_pozice->Hint=scGPNumericEdit_pozice->Value;

		VALIDACE();

}
// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPGlyphButton_PO_text_memoClick
		(TObject *Sender)
{
		// formuláø na støed
		Form_poznamky->Left = Form1->ClientWidth / 2 - Form_poznamky->Width / 2;
		Form_poznamky->Top = Form1->ClientHeight / 2 - Form_poznamky->Height / 2;
		// zobrazeni formuláøe
		Form_poznamky->ShowModal();
}
// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rotaceChange(TObject *Sender)
{
		if (scComboBox_rezim->ItemIndex == 2 && input_state == NOTHING && input_clicked_edit == Rotace_klik)
		{
				input_M(); // pøepoèet hodnot vyplývajících ze zmìny CT  pro režim PP
		}
		// KK režim zavolání input_M
		if (input_state == NOTHING) {
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&
						input_clicked_edit == Rotace_klik && scButton_zamek_RD->Visible==true) {
					if(scGPNumericEdit_RD->ReadOnly==false && scButton_zamek_RD->Enabled==true){
						// ShowMessage(input_clicked_edit);
						Form1->MB("Pokud chcete zmìnit orientaci jigu, je nejprve nutné odemknutím zámku rychlosti pohonu povolit zmìnu hodnoty.");
						}
						scComboBox_rotace->Items->Items[0]->Enabled = false;
						scComboBox_rotace->Items->Items[1]->Enabled = false;
						// scComboBox_rotace->ItemIndex=0;  // zaène se cyklit - zde by to chtìlo close combobox
				}
				if(scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED && input_clicked_edit == Rotace_klik && scButton_zamek_RD->Enabled==false)
				{
						//if podminka splnena - povolim zmenu orientace
						scComboBox_rotace->Items->Items[0]->Enabled = true;
						scComboBox_rotace->Items->Items[1]->Enabled = true;
				}

				// není zamèeno - doporuèím mezeru
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == Rotace_klik)
				 {
					//vždy dovolím volání input_m bez ohledu, zda vyjde RD OK vùèi rozteèi
					// pro pøípad kdy orotuji jig a vyplnìná mezera z pøedtím bude OK, èili pak hned volám input M
					//	if (Kontrola_mezery() == scGPNumericEdit_mezera->Value) {
							 // Memo1->Lines->Add("volam input M z rotace");
								INPUT();
								pm.input_M();
								OUTPUT();
					//	}

				}

		}
}
// ---------------------------------------------------------------------------

void TForm_parametry::Nastav_zamky(double rezim, Tinput_clicked_icon I,Tinput_clicked_edit E, bool ikonka)
{

		if (rezim == 1)
		{ // KK režim
			 //	scButton_K_zamek->Visible = false;
				if (ikonka)
				{ // pokud jde o klik na ikonu
						if (I == CT_klik_ico)
					{ // pokud je kliknuto na ikonu CT
										if (CT_zamek == LOCKED) // když je zamèeno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 38;
												CT_zamek = UNLOCKED;
												// RD
												scButton_zamek_RD->ImageIndex = 37;
												RD_zamek = LOCKED;
												// DD - délka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
										}
										else // odemèeno
											{
												// CT
												scButton_zamek_CT->ImageIndex = 37;
												CT_zamek = LOCKED;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												// DD - délka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
											}

											if (scButton_K_zamek->Visible==true) {
													//scButton_K_zamek->Visible = true;
													scButton_K_zamek->ImageIndex=scButton_zamek_CT->ImageIndex;
											}

					}
						if (I == RD_klik_ico)
						{
							scButton_K_zamek->Visible = false;
								if (RD_zamek == LOCKED) // když je zamèeno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// DD - délka kabiny
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;

										scComboBox_rotace->Items->Items[0]->Enabled = true;
										scComboBox_rotace->Items->Items[1]->Enabled = true;
								}
								else // odemèeno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// DD - délka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;

										scComboBox_rotace->Items->Items[0]->Enabled = false;
										scComboBox_rotace->Items->Items[1]->Enabled = false;
								}
						}

						if (I == DD_klik_ico)
						{    scButton_K_zamek->Visible = false;
								if (DD_zamek == LOCKED) // když je zamèeno
								{
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;

										scButton_zamek_CT->ImageIndex = 37;
										CT_zamek = LOCKED;

										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// set(DELKA,ENABLED,false);
								}
								else // odemèeno
								{
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;

										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;

										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// set(DELKA,DISABLED,false);
								}
						}

								if (I ==	C_klik_ico)
								{     scButton_K_zamek->Visible = true;
											if (CT_zamek == LOCKED) // když je zamèeno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 38;
												CT_zamek = UNLOCKED;
													// KAPACITA
												scButton_K_zamek->ImageIndex = 38;
												K_zamek = UNLOCKED;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												// DD - délka kabiny
												scButton_zamek_DD->ImageIndex = 37;
												DD_zamek = LOCKED;
										}
										else // odemèeno
										{
												// CT
												scButton_zamek_CT->ImageIndex = 37;
												CT_zamek = LOCKED;
													// KAPACITA
												scButton_K_zamek->ImageIndex = 37;
												K_zamek =  LOCKED;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												// DD - délka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
										}

								}

				}
				if (!ikonka) {
						///////KLIKNUTO DO EDITU///////////// KK režim

						if (E == CT_klik) {
								 scButton_K_zamek->Visible = false;

								if (CT_zamek == LOCKED) // když je zamèeno
								{
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
										// DD - délka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;
								}
						}
						if (E == RD_klik) {
								scButton_K_zamek->Visible = false;

								if (RD_zamek == LOCKED) // když je zamèeno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// DD - délka kabiny
										scButton_zamek_DD->ImageIndex = 37;
										DD_zamek = LOCKED;
								}
						}

						if (E == DD_klik) {
							scButton_K_zamek->Visible = false;

								if (DD_zamek == LOCKED) // když je zamèeno
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
							scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
								if(RD_zamek==LOCKED)
								{

								 RD_zamek=UNLOCKED;
								 scButton_zamek_RD->ImageIndex = 38;

								 scButton_zamek_DD->ImageIndex = 37;
								 DD_zamek = LOCKED;

								 CT_zamek=UNLOCKED;
								 scButton_zamek_CT->ImageIndex = 38;

								 vypis("Byl odemèen zámek rychlosti pohonu",false);
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
						 scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
							if(RD_zamek==LOCKED)
								{

								 RD_zamek=UNLOCKED;
								 scButton_zamek_RD->ImageIndex = 38;

								 scButton_zamek_DD->ImageIndex = 37;
								 DD_zamek = LOCKED;

								 CT_zamek=UNLOCKED;
								 scButton_zamek_CT->ImageIndex = 38;

								 vypis("Byl odemèen zámek rychlosti pohonu",false);
								}

									if(!scButton_K_zamek->Visible)
								{
								 scButton_K_zamek->Visible=true;
								 scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;

								}

						}

						if (E == C_klik || E == P_klik) {
								scButton_K_zamek->Visible = false;

								if(scGPNumericEdit_CT->Value>=Form1->d.v.PP.TT) {  //CT je vetsi nez TT

								if(CT_zamek==LOCKED){

								 CT_zamek=UNLOCKED;
								 scButton_zamek_CT->ImageIndex = 38;

								 scButton_zamek_RD->ImageIndex = 37;
								 RD_zamek = LOCKED;

									scButton_zamek_DD->ImageIndex = 38;
									DD_zamek = UNLOCKED;

									 }


								} else //CT je menší než TT
								{



								}
						}
						if (E == Rotace_klik) {
						scButton_K_zamek->Visible = false;

								// RD_zamek = LOCKED;   //rotace vždy zamkne RD
								// scButton_zamek_RD->ImageIndex=37;
						}

				}
		}
		//////KONEC KK režimu//////////////////////////////////////////////////

		if (rezim == 2) { // PP režim

				if (E == mezera_klik && scGPNumericEdit_mezera->Value >= 0) {

						scButton_zamek_DD->Visible = true;
						scButton_zamek_DD->Enabled = true;
						scButton_K_zamek->Visible = true;

				}

				if (I == C_klik_ico) {
						if (K_zamek == LOCKED) // když je zamèeno
						{
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;

								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
						}
						else // odemèeno
						{
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;

								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
						}
				}
				if (I == DD_klik_ico) {
						if (DD_zamek == LOCKED) // když je zamèeno
						{
								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;
						}
						else // odemèeno
						{
								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;
						}
				}
				if (I == CT_klik_ico) {
						if (CT_zamek == LOCKED) // když je zamèeno
						{
								scButton_zamek_CT->ImageIndex = 38;
								CT_zamek = UNLOCKED;

						}
						else // odemèeno
						{
								scButton_zamek_CT->ImageIndex = 37;
								CT_zamek = LOCKED;
						}
				}

				if (E == CT_klik) {
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;
						scButton_zamek_CT->Visible = false;
				}
				if (E == RD_klik) {
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;
						scButton_zamek_CT->Visible = false;
				}
				if (E == DD_klik) {
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;
						scButton_zamek_CT->Visible = false;
					//	ShowMessage("DD klik");
				}
				if (E == P_klik) {
					 //	scButton_zamek_DD->Visible = false;
						//scButton_K_zamek->Visible = false;
						//scButton_zamek_CT->Visible = false;
				}
				if (E == C_klik ||E == P_klik ) {
				 //	ShowMessage("kapacita a P klik");
						scButton_zamek_DD->Visible = false;
						scButton_K_zamek->Visible = false;

						if(scGPNumericEdit_CT->Value< Form1->d.v.PP.TT && Form1->pom->n > 1)
						{
						 scButton_zamek_CT->Visible = true;
             scButton_zamek_CT->Enabled = true;
						 }
						else
						{
						scButton_zamek_CT->Visible = false;
						}

				}
				if (E == mezera_klik) {
						scButton_zamek_CT->Visible = false;
				}

		}
}

////metoda která kontroluje zdali je vybraný pohon používán èi nikoliv, dle toho nastavuje viditelnost editboxù

void TForm_parametry::Pohon_pouzivan() {

		if (scComboBox_rezim->ItemIndex == 1) { // pro KK režim - nastavení
				Cvektory::TPohon *pohon = Form1->d.v.POHONY->dalsi;
				// ShowMessage(scComboBox_pohon->ItemIndex);
				if (Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1))
				{
						// ShowMessage(input_state);
						RD_zamek = LOCKED; // pohon je již použiván - nemohu hýbat RD
						CT_zamek = UNLOCKED;
						DD_zamek = UNLOCKED;

						scButton_zamek_CT->Enabled=false;
						scButton_zamek_DD->Enabled=false;
            scButton_zamek_RD->Enabled=false;

						scButton_zamek_RD->ImageIndex = 37;
						scButton_zamek_CT->ImageIndex = 38;
						scButton_zamek_DD->ImageIndex = 38;

						set(RYCHLOST, READONLY, false);
						set(MEZERA, READONLY, false);

					 //	rz ze vzoru
					 if(!scButton_zamek_RD->Visible)
					 {
							double roztec=0.0;
							Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
							if (P != NULL) roztec=P->roztec;  else  roztec=0.0;

						 double rotace=0.0;
						 if (scComboBox_rotace->ItemIndex==0) rotace=10; else rotace=0.0;  //potenciální hodnota pro výpoèet Rz

						 double mezera=0.0;
						 Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom,1);
						 if (obj!=NULL)   mezera=obj->mezera; else mezera = 0.0;


						double M = Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,roztec,mezera);
						double Rz_potencial =	Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,M);  //rotace je zde obracena nezli je nastaveno v editu
						double Rz_akt =  Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex,obj->mezera);

					 //	Memo1->Lines->Add(Rz_potencial);
           // Memo1->Lines->Add(Rz_akt);


						if(Rz_potencial == Rz_akt){

						//if podminka splnena - povolim zmenu orientace
						scComboBox_rotace->Items->Items[0]->Enabled = true;
						scComboBox_rotace->Items->Items[1]->Enabled = true;
						set(ROTACE,ENABLED, false);
						}  else

						{
						scComboBox_rotace->Items->Items[0]->Enabled = false;
						scComboBox_rotace->Items->Items[1]->Enabled = false;
						set(ROTACE,READONLY, false);


						}

				}

						set(ROTACE,READONLY, false);
						set(ROZESTUP, READONLY, false);
						scGPNumericEdit1_rx->Enabled=false;
				}
				else {
						// ShowMessage(input_state);
						if (input_state == NOTHING)
								// pokud jde o první zobrazení formu - vždy zobrazím takto zámky
						{
								RD_zamek = LOCKED;
								scButton_zamek_RD->ImageIndex = 37;
						}
						else {
								RD_zamek = UNLOCKED;
								scButton_zamek_RD->ImageIndex = 38;
						}
						set(RYCHLOST, ENABLED, false);
						// pohon není používán jiným objektem, dovolím zmìnu RD, M,R
						set(MEZERA, ENABLED, false);
						set(ROZESTUP, READONLY, false);
						set(ROTACE, ENABLED, false);
						scGPNumericEdit1_rx->Enabled=true;
						scButton_zamek_CT->Enabled=true;
						scButton_zamek_DD->Enabled=true;
						scButton_zamek_RD->Enabled=true;
				}
		}

}

// ---------------------------------------------------------------------------

void __fastcall TForm_parametry::scComboBox_rotaceEnter(TObject *Sender) {
		if (scComboBox_rezim->ItemIndex == 2) { // PP režim schování zámkù
				scButton_zamek_DD->Visible = false;
				scButton_K_zamek->Visible = false;
		}
		 //	ShowMessage(input_clicked_edit);
			if (input_state==NOTHING && scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&
						input_clicked_edit == Rotace_klik) {
					 if(scButton_zamek_RD->Visible==true){
						Form1->MB
								("Pokud chcete zmìnit orientaci jigu, je nejprve nutné odemknutím zámku rychlosti pohonu povolit zmìnu hodnoty.",
								MB_OK);
						scComboBox_rotace->Items->Items[0]->Enabled = false;
						scComboBox_rotace->Items->Items[1]->Enabled = false;
							}
								}
}
// ---------------------------------------------------------------------------
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
				if (P->roztec > 0) // pokud existuje rozteè
				{
						//vždy vrátím v metrech  a taky mezeru vždy pøedávám v metrech
						doporuc_mezera = Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_voziku, Form1->d.v.PP.sirka_voziku, scComboBox_rotace->ItemIndex,P->roztec,mezera);

						if(DMunit == MM)
						 {
							 doporuc_mezera=doporuc_mezera*1000.0;
							 jednotky=" mm";
							}
						else
						{
						jednotky=" m";
						}
						// Memo1->Lines->Add(doporuc_mezera);
						// Memo1->Lines->Add(mezera);

						vypis("Doporuèená mezera: " + AnsiString(doporuc_mezera) + jednotky +"");
						VID=28;

						if (Form1->ms.MyToDouble(Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_voziku, Form1->d.v.PP.sirka_voziku, scComboBox_rotace->ItemIndex,P->roztec,mezera)) == Form1->ms.MyToDouble(mezera))
						{
							vypis("",false);
							scGPButton_OK->Enabled = true;
							VID=-1;
						}
						else
						{
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
// ---------------------------------------------------------------------------

void TForm_parametry::Check_rozmezi_RD() {
			vypis("",false);
			bool  mimo_rozmezi = false;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) {
						double jednotky_cas_pohon = 60.0;
						if (Form_parametry->RDunitT == Form_parametry->MIN) jednotky_cas_pohon = 60.0;
						else jednotky_cas_pohon = 1.0;
		 //				if (scGPNumericEdit_RD->Value < Form1->ms.MyToDouble(P->rychlost_od * jednotky_cas_pohon) || scGPNumericEdit_RD->Value > Form1->ms.MyToDouble(P->rychlost_do * jednotky_cas_pohon)) // nesplòuje rozmezí
		 // náhrada pùvodního IF novým - chová se jako klasické between porovnání
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
				if (RDunitD == MM) RD *= 1000.0;

		if (scGPNumericEdit_RD->Value > 0)
				// nutné ošetøení pro období zadávání/psaní
		{
				double roztec=0;
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double rotace=scComboBox_rotace->ItemIndex;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;

				double dopRD=Form1->m.dopRD(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,roztec,Form1->d.v.PP.TT,RD);

				if (RDunitT == MIN){dopRD *= 60.0; RD *= 60.0;}
				if (RDunitD == MM) dopRD /= 1000.0;

				if(mimo_rozmezi==true)
				{
					vypis("Rychlost neodpovídá rozmezí!");
					VID=25;
				}
				if(Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
					vypis("Rychlost neodpovídá rozmezí!");
					VID=25;
				}
				if(scComboBox_pohon->ItemIndex!=0 && roztec>0 && Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi==false)
				{
					double M=(Form1->d.v.PP.TT*RD/60-DV);
					vypis("Zadejte doporuèenou rychlost pohonu: " +AnsiString(dopRD)/*+ ", nebo doporuèenou velikost mezery:" +AnsiString(M)+"m"*zakomentováno, protože nebyl smysl vypisovat M - bylo matoucí*/);
					VID=27;
				}
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
						vypis("Rychlost neodpovídá rozmezí!");
						VID=25;
					}
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi==false) {
						vypis("");
					}
		}
		else vypis("Neplatná hodnota rychlosti pohonu!");
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
				 //vždy pøedám do metody v metrech

		Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if (P != NULL) roztec=P->roztec;  else  roztec=0;

		if (P != NULL)
		{
			scGPNumericEdit1_rx->ReadOnly=false;
			rx=Form1->m.Rx(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,mezera,roztec);
			}	else
			{
					scGPNumericEdit1_rx->ReadOnly=true;
					scGPNumericEdit1_rx->Enabled=false;
			}

		rz = Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,mezera);

		if(DMunit == MM) scGPNumericEdit_rozestup->Value=rz*1000;
		else        		 scGPNumericEdit_rozestup->Value=rz;

		scGPNumericEdit1_rx->Value =rx;
		scGPNumericEdit1_rx->Hint="tj. každý " +AnsiString(rx)+ " palec zachytává.";
 }
//---------------------------------------------------------------------------


 void __fastcall TForm_parametry::scGPNumericEdit1_rxChange(TObject *Sender)
{
						double  mezer=0;
  	if (DMunit == MM) // pokud je v milimetrech, tak pøepne na metry
		{
							mezer=scGPNumericEdit_mezera->Value/1000;
		} else	  mezer=scGPNumericEdit_mezera->Value;


				double roztec=0;
				double delka=0;
				double rotace=0;
				double rx=0;
				Cvektory::TPohon *P = Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
				if (P != NULL) roztec=P->roztec;  else  roztec=0;


				if(scComboBox_rotace->ItemIndex==0)
				{
					delka =Form1->d.v.PP.delka_voziku; rotace=0;
				} else
					{
					delka = Form1->d.v.PP.sirka_voziku; rotace=90;
					}


		if (P != NULL)rx=Form1->m.Rx(pm.dV,pm.sV,rotace,mezer,roztec);
		else rx=0;
				//Memo1->Lines->Add(rx);
 double mezera=Form1->m.mezera(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,rotace,scGPNumericEdit1_rx->Value,roztec);
 double rz= Form1->m.Rz(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.delka_voziku,rotace,mezera);

	 if(input_state == NOTHING || scButton_zamek_RD->Enabled==false)
	 {
			//spocitani mezery pri prvnim zobrazeni formu
			if(scButton_zamek_RD->Enabled==true
			&& RD_zamek==UNLOCKED
			&& input_state==NOTHING
			&& scComboBox_rezim->ItemIndex==1)

			if(input_clicked_edit==Rx)scGPNumericEdit_mezera->Value=mezera; // pøi zmìnì Rx vrátím dopoèítanou mezeru
			scGPNumericEdit_rozestup->Value=rz;
				 //	ShowMessage("z rx");
			input_M();
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit1_rxClick(TObject *Sender)
{
input_clicked_edit=Rx_klik;
Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, Rx_klik, false);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//validace všech hodnot po pøepoètu z PO_math
void TForm_parametry::VALIDACE(Tinput_state input_state)
{
	 vypis("");VID=-1;
	 //////////////////////////////Neplatná hodnota////////////////////////
	 if(pm.RD<=0 || pm.CT<=0 || pm.DD<=0 || pm.K<=0 || pm.P<=0 || pm.M<-0.0000000000000004){VID=0;vypis("Neplatná hodnota!");}
	 if(pm.RD<=0)scGPNumericEdit_RD->Font->Color=clRed;else scGPNumericEdit_RD->Font->Color=clBlack;
	 if(pm.CT<=0)scGPNumericEdit_CT->Font->Color=clRed;else scGPNumericEdit_CT->Font->Color=clBlack;
	 if(pm.DD<=0)scGPNumericEdit_delka_dopravniku->Font->Color=clRed;else scGPNumericEdit_delka_dopravniku->Font->Color=clBlack;
	 if(pm.K<=0)scGPNumericEdit_kapacita->Font->Color=clRed;else scGPNumericEdit_kapacita->Font->Color=clBlack;
	 if(pm.P<=0)scGPNumericEdit_pozice->Font->Color=clRed;else scGPNumericEdit_pozice->Font->Color=clBlack;
	 if(pm.M<-0.0000000000000004)scGPNumericEdit_mezera->Font->Color=clRed;else scGPNumericEdit_mezera->Font->Color=clBlack;

	 switch(scComboBox_rezim->ItemIndex)
	 {
		///////////////////////////////////S&G////////////////////////////////////
		case 0:
		{


				double CT = scGPNumericEdit_CT->Value; // CT od uživatele
				if (CTunit == MIN)CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy
				int pocet_obj_vSG = Form1->d.v.pocet_objektu(0);
				/////////////////pokud je CT == TT////////////////////////////////////
//				if (CT == Form1->ms.MyToDouble(Form1->d.v.PP.TT) && scComboBox_rezim->ItemIndex == 0)
//				{
//						vypis("");
//						VID=-1;
//				}
				/////////////////pokud je pohon pøiøazen a RD<=(DD)/CT////////////////////////////////////
				Cvektory::TObjekt *O=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
				if(O!=NULL)
				{
					if(pm.DD/pm.CT>O->RD && CT<Form1->d.v.PP.TT && Form1->pom->n>1)
					{
						vypis("Vozík nestíhá pøejezd! Zvolte jiný pohon, nebo upravte délku kabiny èi technolog.èas.");
						VID=11;
					}
					else if (pm.DD/pm.CT>O->RD && CT>=Form1->d.v.PP.TT && Form1->pom->n==1)
					{
						 vypis("Vozík nestíhá pøejezd! Zvolte jiný pohon, nebo upravte délku kabiny.");
						 VID=11;
					}
				}
				O=NULL;delete O;
				/////////////////pokud je CT > nežli TT////////////////////////////////////
				if (CT > Form1->ms.MyToDouble(Form1->d.v.PP.TT)
						&& scComboBox_rezim->ItemIndex == 0) {
						if (fmod(CT, Form1->d.v.PP.TT) == 0) {
								kapacitaSG = CT / Form1->d.v.PP.TT; // pro další použití
								vypis(" Rozložit na " + AnsiString(kapacitaSG) + "x " + scGPEdit_name->Text.UpperCase() + "?",true);
								scGPButton_OK->Caption = "Ano a uložit";
								VID=12;
						}
						else
						{
								vypis("Zmìnte režim nebo rozložte do více objektù!");
								VID=13;
						}
				}
				else
						kapacitaSG = 1;

				/////////////////pokud je CT < nežli TT////////////////////////////////////
				if (CT < Form1->ms.MyToDouble(Form1->d.v.PP.TT)	&& scComboBox_rezim->ItemIndex == 0)
				{
						if (Form1->pom->n == 1) { // první objekt na lince

								if (pocet_obj_vSG == 1)
								{
										vypis("Techn. èas je nižší než TT. Ve formuláøi parametrù linky mùžete snížit TT linky!");
										VID=14;
								}

								if (pocet_obj_vSG > 1)
								{
										vypis("Nastavte technologický èas shodný s TT!");
										VID=15;
								}
						}
						else // na lince je více objektù, pokud mají nižší CT dovolím je uložit
						{
								VID=16;// ostatní objekty v poøadí na lince mohu uložit s nižším CT než je TT linky
						}
				}
		}
		break;
		///////////////////////////////////KK////////////////////////////////////
		case 1:
		{
				if(input_clicked_edit==C_klik)
				{ //  pokud je nastaveno, že bylo kliknuto do K
					double CT = scGPNumericEdit_CT->Value; // CT od uživatele
					if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy
					double K = scGPNumericEdit_kapacita->Value; // K od uživatele
					if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
					{;}
					else
					{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							{
								vypis("Byla zadána neplatná kapacita! Zvolte kapacitu vyšší nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemknìte technologický èas a zaktulizujte hodnoty!",true);
								VID=22;
							}
					}
				}
				//-------------------------------------------------------------------------------------------------//
		    // deklarace promìnných je výše = je shodná, jen je potøeba odlišit výpis, zdali se vztahuje k K nebo P
				if(input_clicked_edit==P_klik)
				{ //  pokud je nastaveno, že bylo kliknuto do P
					//Memo1->Lines->Add("klik do P");
					if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
					{;}
					else
					{
						if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
						vypis("Byl zadán neplatný poèet pozic! Zvolte poèet pozic vyšší nebo rovno "+AnsiString(pm.K2P(CT/Form1->d.v.PP.TT))+" , nebo odemknìte technologický èas a zaktulizujte hodnoty!",true);
						VID=22;
					}
				}
				//----------------------------------------------------------------------------------------------------------------------//
				//---------------------------------------------------------------------------------------------------------------------------------//
				if(scGPNumericEdit_mezera->Value<-0.0000000000000004)   //ROSTA mezera ošetøení viz výše
				{
					double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
					double RDV=Form1->m.UDV(Form1->d.v.PP.sirka_voziku,Form1->d.v.PP.delka_voziku,scComboBox_rotace->ItemIndex);
					if(scGPNumericEdit_mezera->Value+DV-RDV>=0)
					{
						vypis("Mezera je záporná, zkuste rotaci jigu"); VID=23;
					}


					else
					{
						vypis("Mezera je záporná, zkuste následující palec, nebo zvažte zmìnu rozmìrù jigu"); VID=24;
					}
				}
                                                                                                    // pokud je pohon nepøiøazen nekontroluji rozestup
				if (scButton_zamek_RD->Visible==true && RD_zamek==LOCKED && input_clicked_edit==Rx_klik && scComboBox_pohon->ItemIndex>0)
				{
					vypis("Pro zmìnu rozestupu nejdøíve povolte zmìnu rychlosti pohonu.");
				}
		///---------------------------------------------------------------
		//Pøiøazen používaný pohon, RD<minimální možné RD objektù (S&G èi PP) na pøiøazeném pohonu (fce minRD)

//				if (scComboBox_pohon->ItemIndex != 0)
//				{
//					Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
//					double	minRD=Form1->d.v.minRD(Form1->pom->pohon);
//					if (obj!=NULL && obj->RD<minRD)
//					{
//							AnsiString vypis_nestihaji=Form1->d.v.vypis_objekty_nestihajici_prejezd(Form1->pom->pohon,obj->RD);
//					// TODO - KONTROLA 	//vypis("Pøi zvolené rychlosti pohonu, by nebylo možné stíhat pøejezd v tìchto objektech "+vypis_nestihaji+", navyšte hodnotu RD minimálnì na "+AnsiString(minRD)+" [m/s].");
//							VID=29;
//					}
//				}
//OPRAVA MARTIN:
				if (scComboBox_pohon->ItemIndex != 0)
				{                                                       //z roletky uvažovaný pohon (ale pom->pohon ho ještì nemá!!!
					double	minRD=Form1->d.v.minRD(Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex));//vratí nejnižší možnou rychlost pro objekt, který používá tento pohon
					Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);

					if (obj!=NULL && pm.RD<minRD)//pokud by novým RD byl pod minRD (tj. pod RD nejrizikovìjšího objektu, tak je problém)
					{    // TODO PRAVDEPODOBNE BUDE ROZSIRENE
							AnsiString vypis_nestihaji=Form1->d.v.vypis_objekty_nestihajici_prejezd(Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex),pm.RD);
							vypis("Pøi zvolené rychlosti pohonu by nebylo možné stíhat pøejezd v tìchto objektech "+vypis_nestihaji+", navyšte hodnotu RD minimálnì na "+AnsiString(minRD*60.0)+" [m/min].");
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
								scGPNumericEdit_RD->Hint="rychlost je mimo nastavený rozsah pohonu";
								vypis("Rychlost pohonu je mimo nastavený rozsah");
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
				double CT = scGPNumericEdit_CT->Value; // CT od uživatele
				double DD = scGPNumericEdit_delka_dopravniku->Value; // CT od uživatele
				if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zadání v minutách pøevede na sekundy
				if (DDunit == MM)  DD = DD / 1000.0;

				double K = scGPNumericEdit_kapacita->Value; // K od uživatele
				/////////////////pokud je pohon pøiøazen a RD<(DV+M)/TT////////////////////////////////////
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

						//akt RD je vyšší než RD uloženého pohonu -> tzn s uloženou RD pohonu bych nestihl pøejet DD
					if(Form1->ms.MyToDouble(DD_CT) > Form1->ms.MyToDouble(O->RD))
					{
					 //	Memo1->Lines->Add("DD_CT > RD");
						vypis("Vozík nestíhá pøejezd! Zvolte jinou mezeru, nebo vyberte jiný pohon.",true);
						VID=40;
					}
				}
			 	O=NULL;delete O;
			 //------------------------------------------------------------------------------------------------//
				if(input_clicked_edit==C_klik)
				{ //  pokud je nastaveno, že bylo kliknuto do P

						if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
						{;}
						else
						{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							{
								VID=44;
								vypis("Byla zadána neplatná kapacita! Zvolte kapacitu vyšší nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+", nebo odemknìte technologický èas a dojde k jeho pøepoètu ve vztahu k zadané kapacitì! kód chyby: "+AnsiString(VID)+"",true);

							}
						}
				}
				//-------------------------------------------------------------------------------------------------//
				// deklarace promìnných je výše = je shodná, jen je potøeba odlišit výpis, zdali se vztahuje k K nebo P
				if(input_clicked_edit==P_klik)
				{ //  pokud je nastaveno, že bylo kliknuto do P
						if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
						{;}
						else
						{
							if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
							vypis("Byl zadán neplatný poèet pozic! Zvolte poèet pozic vyšší nebo rovno "+AnsiString(pm.K2P(CT/Form1->d.v.PP.TT))+" , nebo odemknìte technologický èas a dojde k jeho pøepoètu ve vztahu k poètu pozic!",true);
							VID=44;
						}
				}
			 //---------------------------------------------------------------------------------------------------//
					if(scGPNumericEdit_mezera->Value<0)
				{
				double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
				double RDV=Form1->m.UDV(Form1->d.v.PP.sirka_voziku,Form1->d.v.PP.delka_voziku,scComboBox_rotace->ItemIndex);
				if(scGPNumericEdit_mezera->Value+DV-RDV>=0)
				{
				vypis("Mezera je záporná, zkuste rotaci jigu"); VID=41;
				}
						else
						{
						vypis("Mezera je záporná, zvažte zmìnu rozmìrù jigu"); VID=42;
						}
				}
				//------------------------------------------------------------------------------------------------------//
		}
		break;
	}

	//pokud je náhled z PO zobrazen, zajišuje zároveò okamžitou aktualizaci hodnot v náhledu z PO
	if(Form_objekt_nahled->Visible)Form_objekt_nahled->REFRESH_DATA();//obnoví dat ve formu Form_objekt_nahled vèetnì pøekreslení
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPCheckBox_zaokrouhlitClick(TObject *Sender)
{
//pøepínání zobrazení desetinného èísla na dvì nebo neomezený poèet desetinných míst
	int N_mist=1000;if(scGPCheckBox_zaokrouhlit->Checked)N_mist=2;
	scGPNumericEdit_CT->Decimal=N_mist;
	scGPNumericEdit_RD->Decimal=N_mist;
	scGPNumericEdit_delka_dopravniku->Decimal=N_mist;
	scGPNumericEdit_kapacita->Decimal=N_mist;
	scGPNumericEdit_mezera->Decimal=N_mist;
	scGPNumericEdit_pozice->Decimal=N_mist;
	OUTPUT();
}
//---------------------------------------------------------------------------
//zavolá náhled kabiny, pøípadnì v budoucnu info a o požadovaných parametrech
void __fastcall TForm_parametry::scGPGlyphButton_viewClick(TObject *Sender)
{
		// formuláø na støed
		Form_objekt_nahled->Left = Form1->ClientWidth / 2 - Form_objekt_nahled->Width / 2;
		Form_objekt_nahled->Top = Form1->ClientHeight / 2 - Form_objekt_nahled->Height / 2;
		// zobrazeni formuláøe
		Form_objekt_nahled->zobrazitFrameForm=true;
		Form_objekt_nahled->Show();//nemodální zobrazení
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
// //Form1->MB("Údaje nejsou zadány korektnì a není možné je uložit");
// VID=0;
// }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

