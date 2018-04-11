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
__fastcall TForm_parametry::TForm_parametry(TComponent* Owner) : TForm(Owner) {
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
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender) {
		// formul�� bude p�i prvn�m zobrazen� v sekund�ch a metrech nebo dle INI v odvozen�ch jednotk�ch, jinak dle SI
		minsec = S;
		CTunit = S;
		RDunitT = S;
		m_mm = M;
		DDunit = M;
		DMunit = M;
		RDunitD = M;

		// GLOBAL
		if (Form1->readINI("nastaveni_form_parametry", "cas") == "1") {
				minsec = MIN;
				scGPButton_min_sec->Caption = "v�e na s";
		} // tedy MIN
		if (Form1->readINI("nastaveni_form_parametry", "vzdalenost") == "1") {
				m_mm = MM;
				scGPButton_metry_milimetry->Caption = "v�e na m";
		} // tedy MM
		// CT
		if (Form1->readINI("nastaveni_form_parametry", "CT") == "1")
				rHTMLLabel_CTClick(this); // p�evede na min tzn. CTunit=MIN;
		// RD
		if (Form1->readINI("nastaveni_form_parametry", "RDt") == "1")
				rHTMLLabel_RDClick(this); // p�evede na min tzn. RDunitT=MIN;
		if (Form1->readINI("nastaveni_form_parametry", "RDd") == "1")
				rHTMLLabel_RDClick(this); // p�evede na mm tzn. RDunitD=MM;
		// DD
		if (Form1->readINI("nastaveni_form_parametry", "DD") == "1")
				rHTMLLabel_delka_dopravnikuClick(this); // p�evede na mm tzn. DDunit=MM;
		// DM
		if (Form1->readINI("nastaveni_form_parametry", "DM") == "1")
				rHTMLLabel_mezeraClick(this); // p�evede na mm tzn. DMunit=MM;

		// nastaven� defaultn�ch hodnot
		if (scGPNumericEdit_CT->Value == 0)
				// if(d.v.ZAKAZKY->dalsi!=NULL)//pokud existuje prvn� zak�zka
		{
				scGPNumericEdit_CT->Value = Form1->d.v.PP.TT / (1 + 59 * CTunit);
				// ji� nepou��v�me, proto�e se u prvn� zak�zky uva�uje glob�ln� TT,d.v.ZAKAZKY->dalsi->TT;
				scGPNumericEdit_kapacita->Value = 1;
		}

		input_state = NOTHING; // nutnost!!!
	 //	input_clicked_edit=empty_klik;
		kapacitaSG = 1; // nen� podn�t k rozkl�d�n� na v�ce objekt�
		scGPEdit_name->SetFocus();
		// nastav� v�choz� focus, kde se p�edpokl�d� v�choz� nastavov�n�
		scGPEdit_name->SelectAll(); // ozna�� cele pro editace
		scGPButton_OK->Enabled = true;
		scGPButton_OK->Caption = "Ulo�it";
		form_zobrazen = true;
		// detekuje zda je form aktu�ln� zobrazen, slou�� proto aby p�i zm�n� combo re�im pokud si nastavil u�ivatel formula� jinam, aby z�stal nastaven dle u�ivatele

		// pohon_je_pouzivan  - nastaven� z�mk� a editbox� dle nastaven�ho pohonu.
		Pohon_pouzivan();
		Nacti_rx(); // vypo��t�n� Rx a zobrazeni
		INPUT();   // p�i prvn�m zobrazen� formu "otisknu" data z formu do math struktury, bez ��dn�ch v�po�t�, prim�rn� pou�ito pro nastaven� decimal checkboxu, kdy pot�ebuje m�t data v output ji� p�i formshow
		OUTPUT();  // na�ten� dat ze struktury

}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TForm_parametry::vypis(UnicodeString text, bool RED)
{
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption = "Ulo�it";
		if (text != "") // zobraz� a vyp�e
		{
			 //	rHTMLLabel_InfoText->Hint=text;//zaji��uje zobrazen� cel�ho textu
				rHTMLHint1->ToString()=text;
				// if(!rHTMLLabel_InfoText->Visible)Form_parametry->Height+=(40+19);//pouze pokud byl popisek skryt�
				if (RED)
				{
						scGPButton_OK->Enabled=false;
						scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor = clRed;
						rHTMLLabel_InfoText->Font->Color = clRed;
						// zv�razn�n� polo�ky technologick� �as
						if (text.Pos("technologick� �as") && scGPNumericEdit_CT->Enabled)
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
				if (text.Length() <= 35)// v p��pad�, �e je text del�� ne� 35 znak� skryje ikonu u zvol� nov� lev� odsazen� textu
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
				// zv�razn�n� polo�ky technologick� �as
				if (scGPNumericEdit_CT->Enabled)
				{ /* ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1; */
				}
				// Form_parametry->Height-=(40+19);
		}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender) {
		if (input_state != NO) // pokud to nen� p�i startu (formshow)
		{

				// v�choz� nastaven� z�mk� p�i p�ekliku re�imu na KK
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
				// v�choz� nastaven� z�mk� p�i p�ekliku re�imu na PP
				if (scComboBox_rezim->ItemIndex == 2) {
						K_zamek = LOCKED;
						DD_zamek = UNLOCKED;
						scButton_K_zamek->Visible = false;
						scButton_zamek_DD->ImageIndex = 38;
						scButton_K_zamek->ImageIndex = 37;

				}


				// nadesignov�n� a napozicov�n� komponent dle zvolen�ho re�imu
				setForm4Rezim(scComboBox_rezim->ItemIndex);
				// resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy

				//nastav� edity, podle toho, zdali je pohon pou��v�n �i nikoliv - volat a� po setForm4Re�im
				Pohon_pouzivan();

				if(scComboBox_rezim->ItemIndex == 0) scGPNumericEdit_pozice->Value=1;

				INPUT();  // ulo�en� dat z edit� do struktury p�i zm�n� re�imu
				OUTPUT(); // na�ten� ze struktury - aby prob�hla validace dat p�i zm�n� re�imu

				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
				if (scComboBox_rezim->ItemIndex == 1 && obj!=NULL ){  // u KK re�imu pokud je pohon pou��v�n - nat�hnut� spr�vn� mezery z dat

					 scGPNumericEdit_mezera->Value=obj->mezera;
					}

				// napozicov�n� cel�ho formul��e resp. o�et�en� aby z�stal dialog na monitoru, pouze pro prvotn� zobrazen� dle sou�adnic kurzoru my�i, jinak dle u�ivatele
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
	 /*			// v�pis doporu�en� mezery
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
						if (P->roztec > 0) // pokud existuje rozte�
						{
								double mezera=0;
								if(scGPNumericEdit_mezera->Value==0) mezera=0;
								else mezera=scGPNumericEdit_mezera->Value;

								double doporuc_mezera = Form1->m.mezera_mezi_voziky(dV, sV, rotace, P->roztec, mezera);
								if(Form1->ms.MyToDouble(doporuc_mezera)!=Form1->ms.MyToDouble(mezera))
								{
								vypis("Doporu�en� mezera: " + AnsiString(doporuc_mezera) + " m");
								VID=28;
								} else vypis("",false);
						}  else    vypis("",false);
				}  */

		Kontrola_mezery();

		}
}

// ---------------------------------------------------------------------------
// resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
		// v�choz� zmen�en� formul��e
		offset = 0;
		if (Form1->STATUS == Form1->NAVRH)rezim += 10; // posunut� o 10 vytv��� re�im+navrh��
		//pozice hlavi�kov�ch komponent (+pou�ito n�sledn� v kodu)
		scGPGlyphButton_info->Visible=false;
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
						set(ROZESTUP, HIDE);
						scGPNumericEdit_kapacita->Value = 1;
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
						set(ROZESTUP, READONLY);
						//hlavi�kov� komponenty
						scGPGlyphButton_info->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_info->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_info->Width;
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
						set(ROZESTUP, READONLY);
						//hlavi�kov� komponenty
						scGPGlyphButton_info->Visible=true;
						scGPButton_min_sec->Left=Konec->Left-scGPButton_min_sec->Width-scGPGlyphButton_info->Width;
						scGPButton_metry_milimetry->Left=Konec->Left-scGPButton_metry_milimetry->Width-scGPGlyphButton_info->Width;
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
						set(ROZESTUP, HIDE);
				} break;
		case 12: // POSTPROCESN� - N�VRH��
				{
						set(POHON, ENABLED);
						set(TIME, ENABLED);
						set(RYCHLOST, HIDE);
						set(DELKA, ENABLED);
						set(KAPACITA, ENABLED);
						set(POZICE, ENABLED);
						set(ODCHYLKA, HIDE);
						set(CEKANI, HIDE);
						set(STOPKA, HIDE);
						set(ROTACE, ENABLED);
						set(MEZERA, ENABLED);
						set(ROZESTUP, HIDE);
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

		// VELIKOST FORMUL��E
		Form_parametry->Height = defaultForm_parametryHeight + offset;
		// vertik�ln� (pop�. horizont�ln�) POZICE TLA��TEK OK,STORNO,COPY,PASTE
		scGPButton_OK->Top = Form_parametry->Height - scGPButton_OK->Height - 10;
		scGPButton_storno->Top = Form_parametry->Height -	scGPButton_storno->Height - 10;
		scGPGlyphButton_copy->Top = Form_parametry->Height  - scGPGlyphButton_copy->Height;
		scGPGlyphButton_copy->Left=0;
		scGPGlyphButton_paste->Top = Form_parametry->Height -	scGPGlyphButton_paste->Height;
		scGPGlyphButton_paste->Left=scGPGlyphButton_copy->Width+scGPGlyphButton_copy->Left;
		scGPCheckBox_zaokrouhlit->Top = Form_parametry->Height -scGPCheckBox_zaokrouhlit->Height;
}
// ---------------------------------------------------------------------------
// zaji��uje zobrazen� a napozicov�n� pat�i�n� konkr�tn� komponenty a z�rove� udr�ov�n� hodnoty offsetu - to pokud je move==true, jinak jen nastav� komponenty
void TForm_parametry::set(Tcomponents C, Tcomponents_state S, bool move) {
		// defaultn� hodnoty
		short O = 40; // vertik�ln� velikost odsazen� komponent
		int L = rHTMLLabel_rezim->Top; // v�choz� komponenta
		int P = scComboBox_rezim->Top; // v�choz� komponenta

		switch (C) {
		case POHON: {
						////o�et�en� pokud nen� nadefinov�n ��dn� pohon
						if (!existuje_pohon)
								S = READONLY;
						////pozice
						if (move) {
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
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case TIME: // technologick� �as v re�imu n�vrh��
				{
						////pozice
						if (move) {
								rHTMLLabel_CT->Top = L + 2 * O + offset;
								scGPNumericEdit_CT->Top = P + 2 * O + offset;
								scButton_zamek_CT->Top = scGPNumericEdit_CT->Top;
								scGPGlyphButton_PO_text_memo->Top = scGPNumericEdit_CT->Top + 1;
								// glyph pro memo - ziskany ct
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scGPNumericEdit_CT->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_CT->Visible = true;
						scGPNumericEdit_CT->Enabled = true;
						scGPNumericEdit_CT->Visible = true;
						scGPNumericEdit_CT->Options->FrameNormalColor = clGray;
						scGPNumericEdit_CT->Options->FrameWidth = 1;
						if (scComboBox_rezim->ItemIndex == 1)
								scButton_zamek_CT->Visible = true;
						else
								scButton_zamek_CT->Visible = false;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case RYCHLOST: // RD v re�imu n�vrh��
				{
						////pozice
						if (move) {
								rHTMLLabel_RD->Top = L + 3 * O + offset;
								scGPNumericEdit_RD->Top = P + 3 * O + offset;
								scButton_zamek_RD->Top = scGPNumericEdit_RD->Top;
						}
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
						switch (S) {
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
								scButton_zamek_RD->Visible = false;
								break;
						case HIDE:
								rHTMLLabel_RD->Visible = false;
								scGPNumericEdit_RD->Visible = false;
								scButton_zamek_RD->Visible = false;
								if (move)
										offset -= O;
								break;
						}
				} break;
		case DELKA: // d�lka dopravn�ku
				{
						////pozice
						if (move) {
								rHTMLLabel_delka_dopravniku->Top = L + 4 * O + offset;
								scGPNumericEdit_delka_dopravniku->Top = P + 4 * O + offset;
								scButton_zamek_DD->Top = scGPNumericEdit_delka_dopravniku->Top;
						}
						// ty co jsou stejn�
						scGPNumericEdit_delka_dopravniku->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_delka_dopravniku->Visible = true;
						scGPNumericEdit_delka_dopravniku->Visible = true;
						scGPNumericEdit_delka_dopravniku->Enabled = true;
						scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor =
								clGray;
						scGPNumericEdit_delka_dopravniku->Options->FrameWidth = 1;
						if (scComboBox_rezim->ItemIndex == 0 ||
								scComboBox_rezim->ItemIndex == 2)
								scButton_zamek_DD->Visible = false;
						else
								scButton_zamek_DD->Visible = true;
						// ty co jsou rozd�ln�
						////funk�n� vlastnosti
						switch (S) {
						case HIGHLIGHT:
								scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor =
										hl_color;
								scGPNumericEdit_delka_dopravniku->Options->FrameWidth =
										hlFrameWidth;
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case KAPACITA: // po�adov�na kapacita objektu
				{
						////pozice
						rHTMLLabel_kapacita->Top = L + 5 * O + offset;
						scGPNumericEdit_kapacita->Top = P + 5 * O + offset;
						scButton_K_zamek->Top = scGPNumericEdit_kapacita->Top;
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
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case POZICE: // zji�t�n� kapacita objektu
				{
						////pozice
						rHTMLLabel_pozice->Top = L + 6 * O + offset;
						scGPNumericEdit_pozice->Top = P + 6 * O + offset;
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scGPNumericEdit_pozice->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_pozice->Visible = true;
						scGPNumericEdit_pozice->Visible = true;
						scGPNumericEdit_pozice->Enabled = true;
						scGPNumericEdit_pozice->Options->FrameNormalColor = clGray;
						scGPNumericEdit_pozice->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case ODCHYLKA: // povolen� odchylka z CT
				{
						////pozice
						rHTMLLabel_odchylka->Top = L + 7 * O + offset;
						scGPNumericEdit_odchylka->Top = P + 7 * O + offset;
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scGPNumericEdit_odchylka->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_odchylka->Visible = true;
						scGPNumericEdit_odchylka->Visible = true;
						scGPNumericEdit_odchylka->Enabled = true;
						scGPNumericEdit_odchylka->Options->FrameNormalColor = clGray;
						scGPNumericEdit_odchylka->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case CEKANI: // �ek�n�
				{
						////pozice
						if (move) {
								rHTMLLabel_cekani->Top = L + 8 * O + offset;
								scComboBox_cekani_palec->Top = P + 8 * O + offset;
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scComboBox_cekani_palec->Options->FrameNormalColor = clGray;
						scComboBox_cekani_palec->Options->FrameWidth = 1;
						rHTMLLabel_cekani->Visible = true;
						scComboBox_cekani_palec->Visible = true;
						scComboBox_cekani_palec->Enabled = true;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case STOPKA: // stop stanice na konci objektu
				{
						////pozice
						if (move) {
								rHTMLLabel_stopka->Top = L + 9 * O + offset;
								scComboBox_stopka->Top = P + 9 * O + offset;
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						rHTMLLabel_stopka->Visible = true;
						scComboBox_stopka->Visible = true;
						scComboBox_stopka->Enabled = true;
						scComboBox_stopka->Options->FrameNormalColor = clGray;
						scComboBox_stopka->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case ROTACE: // rotace jigu v objektu, zat�m jen p�ep�n�tko 0-90
				{
						////pozice
						if (move) {
								rHTMLLabel_rotace->Top = L + 10 * O + offset;
								scComboBox_rotace->Top = P + 10 * O + offset;
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						rHTMLLabel_rotace->Visible = true;
						scComboBox_rotace->Visible = true;
						scComboBox_rotace->Enabled = true;
						scComboBox_rotace->Options->FrameNormalColor = clGray;
						scComboBox_rotace->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case MEZERA: // po�adov�na vs. zji�t�n� kapacita objektu
				{
						////pozice
						if (move) {
								rHTMLLabel_mezera->Top = L + 11 * O + offset;
								scGPNumericEdit_mezera->Top = P + 11 * O + offset;
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scGPNumericEdit_mezera->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_mezera->Visible = true;
						scGPNumericEdit_mezera->Visible = true;
						scGPNumericEdit_mezera->Enabled = true;
						scGPNumericEdit_mezera->Options->FrameNormalColor = clGray;
						scGPNumericEdit_mezera->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		case ROZESTUP: // po�adov�na vs. zji�t�n� kapacita objektu
				{
						////pozice
						if (move) {
								rHTMLLabel_rozestup->Top = L + 12 * O + offset;
								scGPNumericEdit_rozestup->Top = P + 12 * O + offset;
								scGPNumericEdit1_rx->Top=scGPNumericEdit_rozestup->Top;
								rHTMLLabel_palec_vzd->Top=rHTMLLabel_rozestup->Top;
								rHTMLLabel_jednotky_vzdalenostpalcu->Top=rHTMLLabel_palec_vzd->Top;
						}
						////funk�n� vlastnosti
						// ty co jsou stejn�
						scGPNumericEdit_rozestup->Options->ShapeStyle = scgpessRect;
						rHTMLLabel_rozestup->Visible = true;
						scGPNumericEdit1_rx->Visible=true;
						scGPNumericEdit_rozestup->Visible = true;
						rHTMLLabel_palec_vzd->Visible=true;
						rHTMLLabel_jednotky_vzdalenostpalcu->Visible=true;
						scGPNumericEdit_rozestup->Enabled = true;
						scGPNumericEdit_rozestup->Options->FrameNormalColor = clGray;
						scGPNumericEdit_rozestup->Options->FrameWidth = 1;
						// ty co jsou rozd�ln�
						switch (S) {
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
								if (move)
										offset -= O;
								break;
						}
				} break;
		}
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// p�i psan� n�zvu objektu se m�n� titulek a generuje zkratka
void __fastcall TForm_parametry::scGPEdit_nameChange(TObject *Sender) {
		Form_parametry->scLabel_titulek->Caption = scGPEdit_name->Text + " - parametry";
		scGPEdit_shortname->Text = scGPEdit_name->Text.SubString(1, 4).UpperCase();
}
// ---------------------------------------------------------------------------
// p�i zm�n�ch EDIT�

void __fastcall TForm_parametry::scGPNumericEdit_poziceChange(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == P_klik)
		{
					input_P();
					if(scComboBox_rezim->ItemIndex==2) //PP - pokud je CT vy��� ne� TT nezobraz�m z�mek u CT
				{
						if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
				}
		}
}

// ----------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_CTChange(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == CT_klik) {

				input_CT(); // p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
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
		if (input_state == NOTHING && input_clicked_edit == mezera_klik && scComboBox_rezim->ItemIndex == 2)
		{ // pokud nen� zad�v�no z jin�ho vstupu
				input_M(); // lok�ln� p�i PP re�imu
		}

		if (input_state == NOTHING)
		{    	vypis("");
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&	input_clicked_edit == mezera_klik)
				{
					if (!Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom)) 	vypis("Povolte zm�nu rychlosti pohonu.");  // pokud nen� pohon pou��v�n, d�m v�pis, jinak v�pis nem� smysl - ji� nelze m�nit rychlost


				} // nen� zam�eno - doporu��m mezeru, �ek�m zdali zad� spr�vnou mezeru
				else
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex>0)
				{
							vypis("");
					if (DMunit == MM) // pokud je v milimetrech, tak p�epne na metry
					{
						mezera=scGPNumericEdit_mezera->Value/1000.0;
					}  else  mezera=scGPNumericEdit_mezera->Value;

						if (Kontrola_mezery() == Form1->ms.MyToDouble(mezera))
						{

						   input_M(); // lok�ln�
							 Nacti_rx();
						}
				}  if(scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED && input_clicked_edit == mezera_klik && scComboBox_pohon->ItemIndex==0) input_M();  // p��pad kdy m�n�m mezeru ale jsem bez pohonu
		}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RD_Change(TObject *Sender) {
		if (input_state == NOTHING && input_clicked_edit == RD_klik)
		{

		if (scComboBox_rezim->ItemIndex==1) // controller pro KK re�im
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
				input_K(); // p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
				if(scComboBox_rezim->ItemIndex==2)
				{
				//PP - pokud je CT vy��� ne� TT nezobraz�m z�mek u CT
						if(scGPNumericEdit_CT->Value>Form1->d.v.PP.TT)	scButton_zamek_CT->Visible=false;
				}
			}
}

// ---------------------------------------------------------------------------
// p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
void TForm_parametry::input_K() {
		input_state = K;
		INPUT();
		double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
		if (CTunit == MIN)CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
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
			OUTPUT();
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
				CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy

				if (CT / Form1->d.v.PP.TT <= K && scButton_zamek_CT->Visible==true  && CT_zamek == LOCKED  && Form1->pom->n>1 && CT<Form1->d.v.PP.TT)
				{
					 Memo1->Lines->Add("input_P(false) ");
					pm.input_P(false);
				}
				else
				{
					if(scButton_zamek_CT->Visible==true && CT_zamek == LOCKED && CT / Form1->d.v.PP.TT > K)
					{
						pm.input_P(false);
						Memo1->Lines->Add("input_P(false) ");
					}// Memo1->Lines->Add("volam KK  ");//	vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
					else//za ka�d� situace standardn� vol�n�
					pm.input_P();
					Memo1->Lines->Add("input_P() ");
				}
		}
		// na�ten� dat zp�t do formul��e po v�po�tu
		OUTPUT();
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
void TForm_parametry::input_CT() {
		input_state = CT;
		INPUT();
		if (scComboBox_rezim->ItemIndex != 0)
		{ // pokud je v re�imu SG nevol� se v�po�etn� model
			 //	if (scGPNumericEdit_CT->Value > 0)
						// nutn� o�et�en� pro obdob� zad�v�n�/psan�
			 //	{
						pm.input_CT(); // zavol�n� v�po�etn�ho modelu
						////////na�ten� dat zp�t do formul��e po v�po�tu////////////
						OUTPUT(); // u SG toto v�bec nevol�m
			 //	}

		}

		if (scComboBox_rezim->ItemIndex == 0)
		{ // pokud je v re�imu SG nevol� se v�po�etn� model
//				if (scGPNumericEdit_CT->Value > 0)
//						// nutn� o�et�en� pro obdob� zad�v�n�/psan�
//				{
//						//pm.input_CT(); // zavol�n� v�po�etn�ho modelu
						////////na�ten� dat zp�t do formul��e po v�po�tu////////////
					 	OUTPUT(); // u SG toto v�bec nevol�m
		 //		}

		}
					//OUTPUT(); //

//		///////////////////// re�im SG - nevstupuje do v�po�tu math //////////////////////////
//		if (scComboBox_rezim->ItemIndex == 0)
//		{ // pokud je v re�imu SG nevol� se v�po�etn� model
//				double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
//				if (CTunit == MIN)
//						CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
//				int pocet_obj_vSG = Form1->d.v.pocet_objektu(0);
//
//				/////////////////pokud je CT == TT////////////////////////////////////
//				if (CT == Form1->ms.MyToDouble(Form1->d.v.PP.TT)
//						&& scComboBox_rezim->ItemIndex == 0) {
//						scGPButton_OK->Enabled = true;
//						scGPButton_OK->Caption = "Ulo�it";
//
//				}
//
//				/////////////////pokud je CT > ne�li TT////////////////////////////////////
//				if (CT > Form1->ms.MyToDouble(Form1->d.v.PP.TT)
//						&& scComboBox_rezim->ItemIndex == 0) {
//						if (fmod(CT, Form1->d.v.PP.TT) == 0) {
//								kapacitaSG = CT / Form1->d.v.PP.TT; // pro dal�� pou�it�
//								vypis(" Rozlo�it na " + AnsiString(kapacitaSG) + "x " +
//										scGPEdit_name->Text.UpperCase() + "?");
//								scGPButton_OK->Enabled = true;
//								scGPButton_OK->Caption = "Ano a ulo�it";
//						}
//						else {
//								scGPButton_OK->Enabled = false;
//								scGPButton_OK->Caption = "Ulo�it";
//								vypis("Zm�nte re�im nebo rozlo�te do v�ce objekt�!");
//						}
//				}
//				else
//						kapacitaSG = 1;
//
//				/////////////////pokud je CT < ne�li TT////////////////////////////////////
//				if (CT < Form1->ms.MyToDouble(Form1->d.v.PP.TT)
//						&& scComboBox_rezim->ItemIndex == 0)
//						// podm�nky pouze pro re�im SG!!
//				{
//						if (Form1->pom->n == 1) { // prvn� objekt na lince
//
//								if (pocet_obj_vSG == 1) {
//										scGPButton_OK->Enabled = false;
//										scGPButton_OK->Caption = "Ulo�it";
//										vypis("M��ete vstoupit na PL a sn�it TT linky!");
//								}
//
//								if (pocet_obj_vSG > 1) {
//										scGPButton_OK->Enabled = false;
//										scGPButton_OK->Caption = "Ulo�it";
//										vypis("Nastavte technologick� �as shodn� s TT!");
//								}
//						}
//						else // na lince je v�ce objekt�, pokud maj� ni��� CT dovol�m je ulo�it
//						{
//								scGPButton_OK->Caption = "Ulo�it";
//								scGPButton_OK->Enabled = true;
//								// ostatn� objekty v po�ad� na lince mohu ulo�it s ni���m CT ne� je TT linky
//
//						}
//				}
//		}

		input_state = NOTHING;

}

// ---------------------------------------------------------------------------
// p�epo�et hodnot vypl�vaj�c�ch ze zm�ny DD
void TForm_parametry::input_DD() {
		input_state = DD;
		INPUT();
		if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex>0) // KK a PP re�im
				// nutn� o�et�en� pro obdob� zad�v�n�/psan�
		{
				pm.input_DD(); // zavol�n� v�po�etn�ho modelu
				OUTPUT();
		}

			if (/*scGPNumericEdit_delka_dopravniku->Value > 0 && */scComboBox_rezim->ItemIndex==0)
		{
				OUTPUT(); // u SG re�imu nevol�m v�po�etn� model, output je vol�n pouze pro validaci dat
		}

		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////

		input_state = NOTHING;
}
// -----------------------------------------------------------------------------

void TForm_parametry::input_M() {
		input_state = mezera;
		INPUT();

		pm.input_M();
		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////
		OUTPUT();
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD, pouze pro kontinual
void TForm_parametry::input_RD() {
			input_state = RD;
			INPUT();
				pm.input_RD();
		///////////na�ten� dat zp�t do formul��e po v�po�tu/////////////////////////////////
		OUTPUT();


		input_state = NOTHING;
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// vynuluje vstupn� hodnoty
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
		 */
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender) {
		// vykresli_vozik(true);//na d�lku
		// ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender) {
		// vykresli_vozik(false);//na ���ku
		// ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}

// ---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender) {
		// if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
		// else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}

// ---------------------------------------------------------------------------
// p�ep�n�n� zobrazen� min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double RD = 0.0;
		double CT = 0.0;
		if (minsec == MIN) // pokud je v minut�ch, tak p�epne na sekundy
		{
				minsec = S;
				scGPButton_min_sec->Caption = "v�e na min";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// CT - p�epo��t�n�
				CTunit = S;
				CT = scGPNumericEdit_CT->Value * 60.0;
				rHTMLLabel_CT->Caption =
						"Technologick� �as <font color=#2b579a>[s]</font>";
				// RD - p�epo��t�n�
				RDunitT = S;
				if (m_mm == MM)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/s]</font>";
				// pokud je v milimetrech
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/s]</font>";
				// pokud je v metrech
				RD = scGPNumericEdit_RD->Value / 60.0;

		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				minsec = MIN;
				scGPButton_min_sec->Caption = "v�e na s";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// CT - p�epo��t�n�
				CTunit = MIN;
				CT = scGPNumericEdit_CT->Value / 60.0;
				rHTMLLabel_CT->Caption =
						"Technologick� �as <font color=#2b579a>[min]</font>";
				// RD - p�epo��t�n�
				RDunitT = MIN;
				if (m_mm == MM)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v milimetrech
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/min]</font>";
				// pokud je v metrech
				RD = scGPNumericEdit_RD->Value * 60.0;
		}
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_CT->Value = CT;
		// ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_RD->Value = RD;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}

// ---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek CT
void __fastcall TForm_parametry::rHTMLLabel_CTClick(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double CT = 0.0;
		if (CTunit == MIN) // pokud je v minut�ch, tak p�epne na sekundy
		{
				CTunit = S;
				// CT - p�epo��t�n�
				CT = scGPNumericEdit_CT->Value * 60.0;
				rHTMLLabel_CT->Caption =
						"Technologick� �as <font color=#2b579a>[s]</font>";
		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				CTunit = MIN;
				// CT - p�epo��t�n�
				CT = scGPNumericEdit_CT->Value / 60.0;
				rHTMLLabel_CT->Caption =
						"Technologick� �as <font color=#2b579a>[min]</font>";
		}
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_CT->Value = CT;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}

// ---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek RD
void __fastcall TForm_parametry::rHTMLLabel_RDClick(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double RD = 0.0;

		if (RDunitT == MIN) // pokud je v minut�ch, tak p�epne na sekundy
		{
				RDunitT = S;
				if (m_mm == MM)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/s]</font>";
				// pokud je v milimetrech
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/s]</font>";
				// pokud je v metrech
				RD = scGPNumericEdit_RD->Value / 60.0;
				// RD=RDunitD_funkce(RD);
		}
		else // pokud je v sekund�ch p�epne na minuty
		{
				RDunitT = MIN;
				if (m_mm == MM)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v milimetrech
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/min]</font>";
				// pokud je v metrech
				RD = scGPNumericEdit_RD->Value * 60.0;
				RD = RDunitD_funkce(RD);
		}
		RD = RDunitD_funkce(RD);
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_RD->Value = RD;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}

// ---------------------------------------------------------------------------
double TForm_parametry::RDunitD_funkce(double RD)
		// podp�rn� metoda v��e uveden�
{
		if (RDunitD == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				RDunitD = M;
				if (RDunitT == MIN)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/min]</font>";
				// pokud je v minut�ch
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/s]</font>";
				// pokud je v sekund�ch
				return RD / 1000.0;
		}
		else {
				RDunitD = MM;
				if (RDunitT == MIN)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v minut�ch
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/s]</font>";
				// pokud je v sekund�ch
				return RD * 1000.0;
		}
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// p�ep�n�n� zobrazen� m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double DD = 0.0;
		double DM = 0.0;
		double RD = 0.0;
		if (m_mm == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				m_mm = M;
				scGPButton_metry_milimetry->Caption = "v�e na mm";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// DD
				DDunit = M;
				rHTMLLabel_delka_dopravniku->Caption =
						"D�lka kabiny <font color=#2b579a>[m]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value / 1000.0;
				// DM
				DMunit = M;
				rHTMLLabel_mezera->Caption =
						"Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
				DM = scGPNumericEdit_mezera->Value / 1000.0;
				// RD
				RDunitD = M;
				if (minsec == MIN)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/min]</font>";
				// pokud je v minut�ch
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[m/s]</font>";
				// pokud je v sekund�ch
				RD = scGPNumericEdit_RD->Value / 1000.0;
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
				m_mm = MM;
				scGPButton_metry_milimetry->Caption = "v�e na m";
				// samotn� tla��tko,ukazuje n�zev opa�n�
				// DD
				DDunit = MM;
				rHTMLLabel_delka_dopravniku->Caption =
						"D�lka kabiny <font color=#2b579a>[mm]</font>";
				DD = scGPNumericEdit_delka_dopravniku->Value * 1000.0;
				// DM
				DMunit = MM;
				rHTMLLabel_mezera->Caption =
						"Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
				DM = scGPNumericEdit_mezera->Value * 1000.0;
				// RD
				RDunitD = MM;
				if (minsec == MIN)
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/min]</font>";
				// pokud je v minut�ch
				else
						rHTMLLabel_RD->Caption =
								"Rychlost pohonu <font color=#2b579a>[mm/s]</font>";
				// pokud je v sekund�ch
				RD = scGPNumericEdit_RD->Value * 1000.0;
		}
		// pln�n� + po�et desetinn�ch m�st
		// ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_delka_dopravniku->Value = DD;
		// ROSTA//scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_mezera->Value = DM;
		// ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_RD->Value = RD;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}

// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// po�adavek na zm�nu jednotek DM
void __fastcall TForm_parametry::rHTMLLabel_mezeraClick(TObject *Sender) {
		input_state = NO; // z�m�r, aby se nep�epo��tavaly hodnoty
		double DM = 0.0;
		if (DMunit == MM) // pokud je v milimetrech, tak p�epne na metry
		{
				// DM
				DMunit = M;
				rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
				DM = scGPNumericEdit_mezera->Value / 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption=" m";
		}
		else // pokud je metrech, tak p�epne na milimetry
		{
				// DM
				DMunit = MM;
				rHTMLLabel_mezera->Caption = "Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
				DM = scGPNumericEdit_mezera->Value * 1000.0;
				rHTMLLabel_jednotky_vzdalenostpalcu->Caption=" mm";
		}
		// pln�n� + po�et desetinn�ch m�st
		scGPNumericEdit_mezera->Value = DM;
		input_state = NOTHING; // u� se mohou p�epo��t�vat
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// p�i stisku kl�vesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift) {
		if (Key == 13) // ENTER
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
		}
		if (Key == 27) // ESC
		{
				Form_parametry->ModalResult = mrCancel;
				// vr�t� stejnou hodnotu jako tla��tko
				Form_parametry->VisibleChanging();
				// skryje form, stejn� jako visible=false
		}
		if (Key == 123) // F12
		{
				Memo1->Visible = true;
				Memo1->Lines->Clear();
				Memo1->Lines->Add(pm.T);
				Memo1->Top = 0;
				Memo1->Left = 0;
		}

}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// p�i stisku storna
void __fastcall TForm_parametry::scGPButton_stornoClick(TObject *Sender) {
		kapacitaSG = 1; // nastav� na default
}

// ---------------------------------------------------------------------------
// p�i stisku storna
void __fastcall TForm_parametry::KonecClick(TObject *Sender) {
		kapacitaSG = 1; // nastav� na default
}

// ---------------------------------------------------------------------------
// ulo�� do INI nastaven� formul��e
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
// zkop�ruje parametry objektu pro dal�� pou�it�
void __fastcall TForm_parametry::scGPGlyphButton_copyClick(TObject *Sender)
{
		// povol� tla��tko vkl�d�n�
		scGPGlyphButton_paste->Enabled = true;

		// p�evod jednotek
		double jednotky_cas = 1.0;if (minsec == MIN)jednotky_cas = 60.0;
		double jednotky_vzdalenost = 1.0;if (m_mm == MM)jednotky_vzdalenost = 1000.0;

		// text do schr�nky
		AnsiString T = "";

		// atributy
		Form1->copyObjekt->name=scGPEdit_name->Text;T=scGPEdit_name->Text;T+=";";
		Form1->copyObjekt->short_name=scGPEdit_shortname->Text;T+=scGPEdit_shortname->Text;T+=";";
		Form1->copyObjekt->rezim=scComboBox_rezim->ItemIndex;T+=scComboBox_rezim->ItemIndex;T+=";";
		Form1->copyObjekt->pohon=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex+1);/* indexuje se od nuly */ T+=scComboBox_pohon->ItemIndex+1;T+=";";
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
		//je�t� zv�it kop�rov�n� nastaven� z�mk�


		// zkop�rov�n� textu do schr�nky
		TClipboard *c = new TClipboard;
		c->AsText = T;
		c = NULL;
		delete c;
}
// ---------------------------------------------------------------------------
// zaji��uje vlo�en� objektu
void __fastcall TForm_parametry::scGPGlyphButton_pasteClick(TObject *Sender)
{
		// aby se nevolalo p�epo��t�v�n� p�i editu polo�ek
		input_state = NO;

		// p�evod jednotek
		double jednotky_cas = 1.0;if (minsec == MIN)jednotky_cas = 60.0;
		double jednotky_vzdalenost = 1.0;if (m_mm == MM)jednotky_vzdalenost = 1000.0;

		scGPEdit_name->Text = Form1->copyObjekt->name;
		scGPEdit_shortname->Text = Form1->copyObjekt->short_name;
		scComboBox_rezim->ItemIndex = Form1->copyObjekt->rezim;
		if (Form1->copyObjekt->pohon == NULL)scComboBox_pohon->ItemIndex=0;
		else scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n-1;
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
		//je�t� zv�it p�eb�r�n� stavu z�mk�

		// nastaven� aktu�ln�ho re�imu objektu
		setForm4Rezim(scComboBox_rezim->ItemIndex);

		// povolen� p�epo��t�v�n�
		input_state = NOTHING;
}

// ---------------------------------------------------------------------------
// z�mek procesn�ho �asu
void __fastcall TForm_parametry::scButton_zamek_CTClick(TObject *Sender)
{
	 	Nastav_zamky(scComboBox_rezim->ItemIndex, CT_klik_ico, empty_klik, true);

		INPUT();
		double	K=scGPNumericEdit_kapacita->Value;
		double  CT=scGPNumericEdit_CT->Value; // CT od u�ivatele
		if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy

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
}

// ---------------------------------------------------------------------------
// z�mek d�lky dopravn�ku
void __fastcall TForm_parametry::scButton_zamek_DDClick(TObject *Sender) {
		Nastav_zamky(scComboBox_rezim->ItemIndex, DD_klik_ico, empty_klik, true);

}

void __fastcall TForm_parametry::scButton_K_zamekClick(TObject *Sender) {
		Nastav_zamky(scComboBox_rezim->ItemIndex, C_klik_ico, empty_klik, true);
}

// ---------------------------------------------------------------------------
// p�i kliknut� na doporu�en� nastane aplikace doporu�en� do dan�ho editboxu
void __fastcall TForm_parametry::rHTMLLabel_InfoTextClick(TObject *Sender)
{
//		if (rHTMLLabel_InfoText->Caption.Pos("Doporu�en� technologick� �as je")) {
//				if (CTunit == MIN)
//						scGPNumericEdit_CT->Value =
//								Form1->m.round(scGPNumericEdit_kapacita->Value)
//								* Form1->d.v.PP.TT / 60.0;
//				else
//						scGPNumericEdit_CT->Value =
//								Form1->m.round(scGPNumericEdit_kapacita->Value)
//								* Form1->d.v.PP.TT;
//		}
//		if (rHTMLLabel_InfoText->Caption.Pos("Doporu�en� kapacita je 1!")) {
//				scGPNumericEdit_kapacita->Value = 1;
//		}
//		if (rHTMLLabel_InfoText->Caption.Pos
//				("Zm�nte re�im nebo rozlo�te do v�ce objekt�!")) {
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
// kontrola vybran�ho pohonu v��i zadan� rychlosti dopravn�ku
void __fastcall TForm_parametry::scComboBox_pohonChange(TObject *Sender) {

	INPUT();
	OUTPUT();
		Pohon_pouzivan();
		Nacti_rx();
		if (scComboBox_pohon->ItemIndex != 0)
		{   // POKUD je pohon ji� pou��v�n, nat�hnu si jeho data
		Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);

				if (obj!=NULL)
				{
					scGPNumericEdit_RD->Value=obj->RD*60;
					scGPNumericEdit_mezera->Value=obj->mezera;
					if(obj->rotace==0) scComboBox_rotace->ItemIndex=0;
					else scComboBox_rotace->ItemIndex=1;

						if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // p�i p�echodu mezi pohony, zkontroluje zdali je mezera v po��dku, pouze u KK re�imu

				}

				else {
				if(scComboBox_rezim->ItemIndex==1) Kontrola_mezery(); // p�i p�echodu mezi pohony, zkontroluje zdali je mezera v po��dku, pouze u KK re�imu

						}
		}


}

// ---------------------------------------------------------------------------
// doplnit koment��
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
// doplnit koment��
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
// doplnit koment��
void __fastcall TForm_parametry::scGPNumericEdit_RD_Click(TObject *Sender) {
		input_clicked_edit = RD_klik;
		Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, RD_klik, false);

}

// ---------------------------------------------------------------------------
// doplnit koment��
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
		//Form_parametry->Close();
		Form1->m.frameForm(Form_parametry_linky,clWebOrange,1);
		Form1->Button_dopravnik_parametryClick(Sender);
}
// ---------------------------------------------------------------------------

void __fastcall TForm_parametry::scButton_zamek_RDClick(TObject *Sender) {

		Nastav_zamky(scComboBox_rezim->ItemIndex, RD_klik_ico, empty_klik, true);
}
// ---------------------------------------------------------------------------

void TForm_parametry::INPUT()
{

		/////////////////// nacteni vsech hodnot z formulare do lokalnich promennych////////////////////////////////////////////////////
		short rezim;
		if (scComboBox_rezim->ItemIndex == 0)
				rezim = 0; // S&G
		if (scComboBox_rezim->ItemIndex == 1)
				rezim = 1; // Kontinual
		if (scComboBox_rezim->ItemIndex == 2)
				rezim = 2; // Postprocesni

		double TT = Form1->d.v.PP.TT;

		double CT = scGPNumericEdit_CT->Value; // CT - nov� zad�no u�ivatelem
		double RD = scGPNumericEdit_RD->Value; // RD	od u�ivatele
		double DD = scGPNumericEdit_delka_dopravniku->Value; // DD od u�ivatele
		double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele
		double Odchylka = scGPNumericEdit_odchylka->Value; // odchylka od u�ivatele
		double Nasleduje_cekani = scComboBox_cekani_palec->ItemIndex;
		// 0 - ne, 1 -ano, 2 - automaticky
		double Stop_stanice = scComboBox_stopka->ItemIndex;
		// 0 - ne, 1 -ano, 2 - automaticky
		double dV = Form1->d.v.PP.delka_voziku; // d�lka jigu
		double sV = Form1->d.v.PP.sirka_voziku; // ���ka jigu
		double m = scGPNumericEdit_mezera->Value; // mezera mezi voziky
		double P = scGPNumericEdit_pozice->Value; // po�et pozic

		bool CT_locked;
		bool RD_locked;
		bool DD_locked;
		bool K_locked;

		Cvektory::TPohon *Pohon =
				Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);

		short rotace;
		if (scComboBox_rotace->ItemIndex == 0)
				rotace = 0; // na d�lku
		if (scComboBox_rotace->ItemIndex == 1)
				rotace = 90; // na ���ku

		//////////////////////// stavy zamku/////////////////////////////////////
		if (CT_zamek == LOCKED)
				CT_locked = true;
		else
				CT_locked = false;
		if (RD_zamek == LOCKED)
				RD_locked = true;
		else
				RD_locked = false;
		if (DD_zamek == LOCKED)
				DD_locked = true;
		else
				DD_locked = false;
		if (K_zamek == LOCKED)
				K_locked = true;
		else
				K_locked = false;

		//////////////////////// prevody jednotek///////////////////////////////

		if (CTunit == MIN)
				CT = CT * 60.0;
		// pokud bylo zad�n� v minut�ch p�evede na sekundy - jinak je CT v Si a mohu ho hned ulo�it k v�po�tu
		if (RDunitT == MIN)
				RD /= 60.0;
		if (RDunitD == MM)
				RD *= 1000.0;
		if (DDunit == MM)
				DD /= 1000.0; // v�dy ukl�d�m do metr�
		if (DMunit == MM)
				m /= 1000.0;
		// Memo1->Lines->Add(RD);

		///////////////ulo�en� do v�po�etn�ho modulu PO/////////////////////////
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
		if (Pohon != NULL)
				pm.R = Pohon->roztec;
		else
				pm.R = 0;
		pm.CT_locked = CT_locked;
		pm.RD_locked = RD_locked;
		pm.DD_locked = DD_locked;
		pm.K_locked = K_locked;

}

////////////////////////////////////////////////////////////////////////////
void __fastcall TForm_parametry::scGPGlyphButton_PO_text_memoClick
		(TObject *Sender)

{
		// formul�� na st�ed
		Form_poznamky->Left = Form1->ClientWidth / 2 - Form_poznamky->Width / 2;
		Form_poznamky->Top = Form1->ClientHeight / 2 - Form_poznamky->Height / 2;
		// zobrazeni formul��e
		Form_poznamky->ShowModal();
}
// ---------------------------------------------------------------------------

void TForm_parametry::OUTPUT()
{
					// pokud bylo zad�v�no nap�. z input_CT nebudu z math modelu tento �daj znovu natahovat do formu,
					// zp�sobuje to toti� p�episov�n� hodnot p�i zad�v�n� hodnoty do editboxu
					// takto jsou o�et�eny v�echny vstupy
		if (input_state != CT)
		{
				scGPNumericEdit_CT->Value = pm.CT;
				if (CTunit == MIN)scGPNumericEdit_CT->Value /= 60.0;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_CT->Decimal=2;

		}
		if (input_state != RD)
		{
				scGPNumericEdit_RD->Value = pm.RD;
				if (RDunitT == MIN) scGPNumericEdit_RD->Value *= 60.0;
				else if (RDunitD == MM) scGPNumericEdit_RD->Value *= 1000.0;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_RD->Decimal=2;
		}
		if (input_state != DD)
		{
				scGPNumericEdit_delka_dopravniku->Value = pm.DD;
				if (DDunit == MM)scGPNumericEdit_delka_dopravniku->Value *= 1000.0;
				scGPNumericEdit_delka_dopravniku->Hint=scGPNumericEdit_delka_dopravniku->Value;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_delka_dopravniku->Decimal=2;
		}
		if (input_state != K) {
				scGPNumericEdit_kapacita->Value = pm.K;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_kapacita->Decimal=2;
		}
		if (input_state != mezera) {
				scGPNumericEdit_mezera->Value = pm.M;
			 //	Memo1->Lines->Add("OUTPUT:"+AnsiString(pm.M));
				if (DMunit == MM)
				scGPNumericEdit_mezera->Value *= 1000.0;
				if(scGPCheckBox_zaokrouhlit->Checked)	scGPNumericEdit_mezera->Decimal=2;
		}
		if (input_state != P)
		{
				scGPNumericEdit_pozice->Value = pm.P;
				if(scGPCheckBox_zaokrouhlit->Checked)scGPNumericEdit_pozice->Decimal=2;
		}

		//ZOBRAZEN� HINT�
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

void __fastcall TForm_parametry::scComboBox_rotaceChange(TObject *Sender) {


		if (scComboBox_rezim->ItemIndex == 2 && input_state == NOTHING && input_clicked_edit == Rotace_klik)
		{
				input_M(); // p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT  pro re�im PP
		}
		// KK re�im zavol�n� input_M
		if (input_state == NOTHING) {
				if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&
						input_clicked_edit == Rotace_klik) {
					if(scGPNumericEdit_RD->ReadOnly==false && scButton_zamek_RD->Visible==true){
            // ShowMessage(input_clicked_edit);
						Form1->MB("Pokud chcete zm�nit orientaci jigu, je nejprve nutn� odemknut�m z�mku rychlosti pohonu povolit zm�nu hodnoty.");
            }
						scComboBox_rotace->Items->Items[0]->Enabled = false;
						scComboBox_rotace->Items->Items[1]->Enabled = false;
						// scComboBox_rotace->ItemIndex=0;  // za�ne se cyklit - zde by to cht�lo close combobox
				} // nen� zam�eno - doporu��m mezeru, �ek�m zdali zad� spr�vnou mezeru
				else if (scComboBox_rezim->ItemIndex == 1 && RD_zamek == UNLOCKED &&
						input_clicked_edit == Rotace_klik) {


						//v�dy dovol�m vol�n� input_m bez ohledu, zda vyjde RD OK v��i rozte�i

						// pro p��pad kdy orotuji jig a vypln�n� mezera z p�edt�m bude OK, �ili pak hned vol�m input M
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
												// RD
												scButton_zamek_RD->ImageIndex = 37;
												RD_zamek = LOCKED;
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
										}
										else // odem�eno
											{
												// CT
												scButton_zamek_CT->ImageIndex = 37;
												CT_zamek = LOCKED;
												// RD
												scButton_zamek_RD->ImageIndex = 38;
												RD_zamek = UNLOCKED;
												// DD - d�lka kabiny
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

										scComboBox_rotace->Items->Items[0]->Enabled = true;
										scComboBox_rotace->Items->Items[1]->Enabled = true;
								}
								else // odem�eno
								{
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;

										scComboBox_rotace->Items->Items[0]->Enabled = false;
										scComboBox_rotace->Items->Items[1]->Enabled = false;
								}
						}

						if (I == DD_klik_ico)
						{    scButton_K_zamek->Visible = false;
								if (DD_zamek == LOCKED) // kdy� je zam�eno
								{
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;

										scButton_zamek_CT->ImageIndex = 37;
										CT_zamek = LOCKED;

										scButton_zamek_RD->ImageIndex = 38;
										RD_zamek = UNLOCKED;
										// set(DELKA,ENABLED,false);
								}
								else // odem�eno
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
											if (CT_zamek == LOCKED) // kdy� je zam�eno
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
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 37;
												DD_zamek = LOCKED;
										}
										else // odem�eno
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
												// DD - d�lka kabiny
												scButton_zamek_DD->ImageIndex = 38;
												DD_zamek = UNLOCKED;
										}

								}

				}
				if (!ikonka) {
						///////KLIKNUTO DO EDITU///////////// KK re�im

						if (E == CT_klik) {
								 scButton_K_zamek->Visible = false;
								// Pohon_pouzivan();
								if (CT_zamek == LOCKED) // kdy� je zam�eno
								{
										// CT
										scButton_zamek_CT->ImageIndex = 38;
										CT_zamek = UNLOCKED;
										// RD
										scButton_zamek_RD->ImageIndex = 37;
										RD_zamek = LOCKED;
										// DD - d�lka kabiny
										scButton_zamek_DD->ImageIndex = 38;
										DD_zamek = UNLOCKED;
								}
						}
						if (E == RD_klik) {
								scButton_K_zamek->Visible = false;
								// Pohon_pouzivan();
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
								// Pohon_pouzivan();
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
							scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;
								if(RD_zamek==LOCKED)
								{

								 RD_zamek=UNLOCKED;
								 scButton_zamek_RD->ImageIndex = 38;

								 scButton_zamek_DD->ImageIndex = 37;
								 DD_zamek = LOCKED;

								 CT_zamek=UNLOCKED;
								 scButton_zamek_CT->ImageIndex = 38;

								 vypis("Byl odem�en z�mek rychlosti pohonu",false);
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

								 vypis("Byl odem�en z�mek rychlosti pohonu",false);
								}

									if(!scButton_K_zamek->Visible)
								{
								 scButton_K_zamek->Visible=true;
								 scButton_K_zamek->ImageIndex = scButton_zamek_CT->ImageIndex;

								}

						}

						if (E == C_klik || E == P_klik) {
								scButton_K_zamek->Visible = false;
								// Pohon_pouzivan();
								if(scGPNumericEdit_CT->Value>=Form1->d.v.PP.TT) {  //CT je vetsi nez TT

								if(CT_zamek==LOCKED){

								 CT_zamek=UNLOCKED;
								 scButton_zamek_CT->ImageIndex = 38;

								 scButton_zamek_RD->ImageIndex = 37;
								 RD_zamek = LOCKED;

									scButton_zamek_DD->ImageIndex = 38;
									DD_zamek = UNLOCKED;

									 }


								} else //CT je men�� ne� TT
								{



								}
						}
						if (E == Rotace_klik) {
						scButton_K_zamek->Visible = false;
								// Pohon_pouzivan();
								// RD_zamek = LOCKED;   //rotace v�dy zamkne RD
								// scButton_zamek_RD->ImageIndex=37;
						}

				}
		}
		//////KONEC KK re�imu//////////////////////////////////////////////////

		if (rezim == 2) { // PP re�im

				if (E == mezera_klik && scGPNumericEdit_mezera->Value >= 0) {

						scButton_zamek_DD->Visible = true;
						scButton_K_zamek->Visible = true;

				}

				if (I == C_klik_ico) {
						if (K_zamek == LOCKED) // kdy� je zam�eno
						{
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;

								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
						}
						else // odem�eno
						{
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;

								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
						}
				}
				if (I == DD_klik_ico) {
						if (DD_zamek == LOCKED) // kdy� je zam�eno
						{
								scButton_zamek_DD->ImageIndex = 38;
								DD_zamek = UNLOCKED;
								scButton_K_zamek->ImageIndex = 37;
								K_zamek = LOCKED;
						}
						else // odem�eno
						{
								scButton_zamek_DD->ImageIndex = 37;
								DD_zamek = LOCKED;
								scButton_K_zamek->ImageIndex = 38;
								K_zamek = UNLOCKED;
						}
				}
				if (I == CT_klik_ico) {
						if (CT_zamek == LOCKED) // kdy� je zam�eno
						{
								scButton_zamek_CT->ImageIndex = 38;
								CT_zamek = UNLOCKED;

						}
						else // odem�eno
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

						if(scGPNumericEdit_CT->Value< Form1->d.v.PP.TT && Form1->pom->n > 1){
						 scButton_zamek_CT->Visible = true;  }
						else
						{ scButton_zamek_CT->Visible = false; }

				}
				if (E == mezera_klik) {
						scButton_zamek_CT->Visible = false;
				}

		}
}

////metoda kter� kontroluje zdali je vybran� pohon pou��v�n �i nikoliv, dle toho nastavuje viditelnost editbox�

void TForm_parametry::Pohon_pouzivan() {

		if (scComboBox_rezim->ItemIndex == 1) { // pro KK re�im - nastaven�
				Cvektory::TPohon *pohon = Form1->d.v.POHONY->dalsi;
				// ShowMessage(scComboBox_pohon->ItemIndex);
				if (Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,
						Form1->pom)) {
						// ShowMessage(input_state);
						RD_zamek = LOCKED; // pohon je ji� pou�iv�n - nemohu h�bat RD
						scButton_zamek_RD->ImageIndex = 37;
						set(RYCHLOST, READONLY, false);
						set(MEZERA, READONLY, false);
						set(ROTACE, READONLY, false);
						set(ROZESTUP, READONLY, false);
						scGPNumericEdit1_rx->Enabled=false;
				}
				else {
						// ShowMessage(input_state);
						if (input_state == NOTHING)
								// pokud jde o prvn� zobrazen� formu - v�dy zobraz�m takto z�mky
						{
								RD_zamek = LOCKED;
								scButton_zamek_RD->ImageIndex = 37;
						}
						else {
								RD_zamek = UNLOCKED;
								scButton_zamek_RD->ImageIndex = 38;
						}
						set(RYCHLOST, ENABLED, false);
						// pohon nen� pou��v�n jin�m objektem, dovol�m zm�nu RD, M,R
						set(MEZERA, ENABLED, false);
						set(ROZESTUP, READONLY, false);
						set(ROTACE, ENABLED, false);
						scGPNumericEdit1_rx->Enabled=true;
				}
		}

}

// ---------------------------------------------------------------------------

void __fastcall TForm_parametry::scComboBox_rotaceEnter(TObject *Sender) {
		if (scComboBox_rezim->ItemIndex == 2) { // PP re�im schov�n� z�mk�
				scButton_zamek_DD->Visible = false;
				scButton_K_zamek->Visible = false;
		}
		 //	ShowMessage(input_clicked_edit);
			if (input_state==NOTHING && scComboBox_rezim->ItemIndex == 1 && RD_zamek == LOCKED &&
						input_clicked_edit == Rotace_klik) {
					 if(scButton_zamek_RD->Visible==true){
						Form1->MB
								("Pokud chcete zm�nit orientaci jigu, je nejprve nutn� odemknut�m z�mku rychlosti pohonu povolit zm�nu hodnoty.",
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
				if (P->roztec > 0) // pokud existuje rozte�
				{
						//v�dy vr�t�m v metrech  a taky mezeru v�dy p�ed�v�m v metrech
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
						 Memo1->Lines->Add(doporuc_mezera);
						 Memo1->Lines->Add(mezera);

						vypis("Doporu�en� mezera: " + AnsiString(doporuc_mezera) + jednotky +"");

						if (Form1->ms.MyToDouble(Form1->m.mezera_mezi_voziky(Form1->d.v.PP.delka_voziku, Form1->d.v.PP.sirka_voziku, scComboBox_rotace->ItemIndex,P->roztec,mezera)) == Form1->ms.MyToDouble(mezera))
						{vypis("",false); scGPButton_OK->Enabled = true; }
						else
						{
						 scGPButton_OK->Enabled = false;
						 }
				}
				else
				{
						scGPButton_OK->Enabled = true;
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
						if (scGPNumericEdit_RD->Value <P->rychlost_od * jednotky_cas_pohon || P->rychlost_do * jednotky_cas_pohon <scGPNumericEdit_RD->Value) // nespl�uje rozmez�
						{
							//	vypis("Rychlost neodpov�d� rozmez�!");
							if(scGPNumericEdit_RD->Value>P->rychlost_do)  VID=25;
							if(scGPNumericEdit_RD->Value<P->rychlost_od)  VID=26;
								 mimo_rozmezi=true;
						}
				}
			double RD=scGPNumericEdit_RD->Value;
				if (RDunitT == MIN)RD /= 60.0;
				if (RDunitD == MM) RD *= 1000.0;

		if (scGPNumericEdit_RD->Value > 0)
				// nutn� o�et�en� pro obdob� zad�v�n�/psan�
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
					vypis("Rychlost neodpov�d� rozmez�!");
				}
				if(Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
					vypis("Rychlost neodpov�d� rozmez�!");
				}
				if(scComboBox_pohon->ItemIndex!=0 && roztec>0 && Form1->ms.MyToDouble(dopRD)!= Form1->ms.MyToDouble(RD) && mimo_rozmezi==false)
				{
					double M=(Form1->d.v.PP.TT*RD/60-DV);
					vypis("Zadejte doporu�enou rychlost pohonu: " +AnsiString(dopRD)/*+ ", nebo doporu�enou velikost mezery:" +AnsiString(M)+"m"*zakomentov�no, proto�e nebyl smysl vypisovat M - bylo matouc�*/);
					VID=27;
				}
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi)
				{
						vypis("Rychlost neodpov�d� rozmez�!");
					}
				if (Form1->ms.MyToDouble(dopRD)== Form1->ms.MyToDouble(RD) && mimo_rozmezi==false) {
						vypis("");
					}
		}
		else vypis("Neplatn� hodnota rychlosti pohonu!");
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
		scGPNumericEdit1_rx->Hint="tj. ka�d� " +AnsiString(rx)+ " palec zachyt�v�.";
 }
//---------------------------------------------------------------------------


 void __fastcall TForm_parametry::scGPNumericEdit1_rxChange(TObject *Sender)
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

	 if(input_state == NOTHING || scButton_zamek_RD->Visible==false) {
			//spocitani mezery pri prvnim zobrazeni formu
			if(scButton_zamek_RD->Visible==true
			&& RD_zamek==UNLOCKED
			&& input_state==NOTHING
			&& scComboBox_rezim->ItemIndex==1)

	if(input_clicked_edit==Rx)scGPNumericEdit_mezera->Value=mezera; // p�i zm�n� Rx vr�t�m dopo��tanou mezeru
	scGPNumericEdit_rozestup->Value=rz;
	input_M();   }

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit1_rxClick(TObject *Sender)
{
input_clicked_edit=Rx_klik;
Nastav_zamky(scComboBox_rezim->ItemIndex, empty_klik_ico, Rx_klik, false);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//validace v�ech hodnot po p�epo�tu z PO_math
void TForm_parametry::VALIDACE(Tinput_state input_state)
{
	 vypis("");VID=-1;
	 //////////////////////////////Neplatn� hodnota////////////////////////
	 if(pm.RD<=0 || pm.CT<=0 || pm.DD<=0 || pm.K<=0 || pm.P<=0 || pm.M<-0.0000000000000004){VID=0;vypis("Neplatn� hodnota!");}
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
					if(pm.DD/pm.CT>O->RD && CT<Form1->d.v.PP.TT && Form1->pom->n>1)
					{
						vypis("Voz�k nest�h� p�ejezd! Zvolte jin� pohon, nebo upravte d�lku kabiny �i technolog.�as.");
						VID=11;
					}
					else if (pm.DD/pm.CT>O->RD && CT>=Form1->d.v.PP.TT && Form1->pom->n==1)
					{
						 vypis("Voz�k nest�h� p�ejezd! Zvolte jin� pohon, nebo upravte d�lku kabiny.");
						 VID=11;
					}
				}
				O=NULL;delete O;
				/////////////////pokud je CT > ne�li TT////////////////////////////////////
				if (CT > Form1->ms.MyToDouble(Form1->d.v.PP.TT)
						&& scComboBox_rezim->ItemIndex == 0) {
						if (fmod(CT, Form1->d.v.PP.TT) == 0) {
								kapacitaSG = CT / Form1->d.v.PP.TT; // pro dal�� pou�it�
								vypis(" Rozlo�it na " + AnsiString(kapacitaSG) + "x " + scGPEdit_name->Text.UpperCase() + "?");
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
										vypis("M��ete vstoupit na PL a sn�it TT linky!");
										VID=14;
								}

								if (pocet_obj_vSG > 1)
								{
										vypis("Nastavte technologick� �as shodn� s TT!");
										VID=15;
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
								vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+" nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
								VID=22;
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
						vypis("Byl zad�n neplatn� po�et pozic! Zvolte po�et pozic vy��� nebo rovno "+AnsiString(pm.K2P(CT/Form1->d.v.PP.TT))+" , nebo odemkn�te technologick� �as a zaktulizujte hodnoty!",true);
						VID=22;
					}
				}
				//----------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------------------------------------------------------------------------//
				if(scGPNumericEdit_mezera->Value<-0.0000000000000004)   //ROSTA mezera o�et�en� viz v��e
				{
					double DV=Form1->m.UDV(Form1->d.v.PP.delka_voziku,Form1->d.v.PP.sirka_voziku,scComboBox_rotace->ItemIndex);
					double RDV=Form1->m.UDV(Form1->d.v.PP.sirka_voziku,Form1->d.v.PP.delka_voziku,scComboBox_rotace->ItemIndex);
					if(scGPNumericEdit_mezera->Value+DV-RDV>=0)
					{
						vypis("Mezera je z�porn�, zkuste rotaci jigu"); VID=23;
					}
					else
					{
						vypis("Mezera je z�porn�, zkuste n�sleduj�c� palec, nebo zva�te zm�nu rozm�r� jigu"); VID=24;
					}
				}
                                                                                                    // pokud je pohon nep�i�azen nekontroluji rozestup
				if (scButton_zamek_RD->Visible==true && RD_zamek==LOCKED && input_clicked_edit==Rx_klik && scComboBox_pohon->ItemIndex>0)
				{
					vypis("Pro zm�nu rozestupu nejd��ve povolte zm�nu rychlosti pohonu.");
				}
		///---------------------------------------------------------------
		//P�i�azen pou��van� pohon, RD<minim�ln� mo�n� RD objekt� (S&G �i PP) na p�i�azen�m pohonu (fce minRD)

				if (scComboBox_pohon->ItemIndex != 0)
				{
				Cvektory::TObjekt *obj=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
				double	minRD=Form1->d.v.minRD(Form1->pom->pohon);

						if (obj!=NULL && obj->RD<minRD)
						{
							AnsiString vypis_nestihaji=Form1->d.v.vypis_objekty_nestihajici_prejezd(Form1->pom->pohon,obj->RD);
						vypis("P�i zvolen� rychlosti pohonu, by nebylo mo�n� st�hat p�ejezd v t�chto objektech "+vypis_nestihaji+", navy�te hodnotu RD minim�ln� na "+AnsiString(minRD)+" [m/s].");
							VID=29;
						}
				}


		//-----------------------------------------------------------------------------------------------------------------------------//
		}
		break;

		///////////////////////////////////PP////////////////////////////////////
		case 2://PP
		{
				double CT = scGPNumericEdit_CT->Value; // CT od u�ivatele
				if (CTunit == MIN) CT = CT * 60.0; // pokud bylo zad�n� v minut�ch p�evede na sekundy
				double K = scGPNumericEdit_kapacita->Value; // K od u�ivatele
				/////////////////pokud je pohon p�i�azen a RD<(DV+M)/TT////////////////////////////////////
				Cvektory::TObjekt *O=Form1->d.v.pohon_je_pouzivan(scComboBox_pohon->ItemIndex,Form1->pom);
				if(O!=NULL)
				{
					if((Form1->m.UDV(pm.dV,pm.sV,pm.Rotace)+pm.M)/pm.TT>O->RD)
					{
						vypis("Voz�k nest�h� p�ejezd! Zvolte jin� pohon nebo, upravte d�lku kabiny �i technologick� �as.");
						VID=40;
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
								VID=44;
								vypis("Byla zad�na neplatn� kapacita! Zvolte kapacitu vy��� nebo rovno "+AnsiString(CT/Form1->d.v.PP.TT)+", nebo odemkn�te technologick� �as a dojde k jeho p�epo�tu ve vztahu k zadan� kapacit�! k�d chyby: "+AnsiString(VID)+"",true);

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
							vypis("Byl zad�n neplatn� po�et pozic! Zvolte po�et pozic vy��� nebo rovno "+AnsiString(pm.K2P(CT/Form1->d.v.PP.TT))+" , nebo odemkn�te technologick� �as a dojde k jeho p�epo�tu ve vztahu k po�tu pozic!",true);
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


}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPCheckBox_zaokrouhlitClick(TObject *Sender)
{
//p�ep�n�n� zobrazen� desetinn�ho ��sla na dv� nebo neomezen� po�et desetinn�ch m�st
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
//zavol� n�hled kabiny, p��padn� v budoucnu info a o po�adovan�ch parametrech
void __fastcall TForm_parametry::scGPGlyphButton_infoClick(TObject *Sender)
{
		// formul�� na st�ed
		Form_kabina_schema->Left = Form1->ClientWidth / 2 - Form_kabina_schema->Width / 2;
		Form_kabina_schema->Top = Form1->ClientHeight / 2 - Form_kabina_schema->Height / 2;
		// zobrazeni formul��e
		Form_kabina_schema->ShowModal();
}
//---------------------------------------------------------------------------

