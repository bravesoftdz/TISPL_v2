// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "superform.h"
#include "unit1.h"
#include "cesty.h"
#include "jig.h"
#include "minicolordialog.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzGrids"
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scGPImages"
#pragma link "scImageCollection"
#pragma link "scColorControls"
#pragma link "scExtControls"
#pragma resource "*.dfm"
TForm_definice_zakazek *Form_definice_zakazek;

// ---------------------------------------------------------------------------
__fastcall TForm_definice_zakazek::TForm_definice_zakazek(TComponent* Owner)
    : TForm(Owner) {

  nastav_form(); // nastaven� barev komponent
  zmena_TT = false;
  add_zakazka = false;
  nacitam_zakazky = false;
}

// ---------------------------------------------------------------------------
// nastaven� barev komponent
void TForm_definice_zakazek::nastav_form() {
  F->log(__func__); // logov�n�
  // nastaven� glob�ln�ch barev
	light_gray = (TColor)RGB(240, 240, 240);
  def_gray = (TColor)RGB(200, 200, 200);

  rStringGridEd1->Color = clWhite; // nastaven� pozad� barvy formul��e
  Form_definice_zakazek->Color = light_gray; // RGB(43,87,154);

  // rHTMLLabel3->Font->Color=(TColor)RGB(50,50,50);   //velky nadpis  1

  scGPButton2->Options->NormalColor = Form_definice_zakazek->Color;
  scGPButton2->Options->FocusedColor = Form_definice_zakazek->Color;
  scGPButton2->Options->HotColor = Form_definice_zakazek->Color;
  scGPButton2->Options->PressedColor = Form_definice_zakazek->Color;
  scGPButton2->Options->FramePressedColor = Form_definice_zakazek->Color;

  scGPButton_plan_vyroby->Options->NormalColor = Form_definice_zakazek->Color;
  scGPButton_plan_vyroby->Options->FocusedColor = Form_definice_zakazek->Color;
  scGPButton_plan_vyroby->Options->HotColor = Form_definice_zakazek->Color;
  scGPButton_plan_vyroby->Options->PressedColor = Form_definice_zakazek->Color;
  scGPButton_plan_vyroby->Options->FrameNormalColor =
      Form_definice_zakazek->Color;
  scGPButton_plan_vyroby->Options->FramePressedColor =
      Form_definice_zakazek->Color;

  Form1->m.designButton(scGPButton_Ulozit, Form_definice_zakazek, 1, 2);
  Form1->m.designButton(scGPButton_storno, Form_definice_zakazek, 2, 2);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// zobrazen� formul��e
void __fastcall TForm_definice_zakazek::FormShow(TObject *Sender) {
  F->log(__func__); // logov�n�
  Left = Form1->ClientWidth / 2 - Width / 2;
	Top = Form1->ClientHeight / 2 - Height / 2;
  volno = true;
  pocet_davek = 0;
  ////nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
	nacti_PP();
	nastav_jazyk();
	F->d.v.vytvor_default_zakazku();//vytvo�� nebo aktualizuje defaultn� zak�zku

  ////na�ten� zak�zek a cest
  if (Form1->d.v.ZAKAZKY->dalsi == NULL) // kdyz je spojak prazdny
  {
    // vytvo�� defaultn� �adek se zak�zkou a jej� cestou

    // predvypln_cestu(); ji� se nepou��v� pln� se z default_cestu() dat tak jako u�ivatlsky definovan� zak�zky
    // a hned se ulo�� do temp spoj�ku
//    uloz_Defaulttemp_zakazku();
//    uloz_Default_cestu();
//    Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
//    // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
//    // po ulo�en� do hl.spojaku zak�zek pot�ebuji stejn� znovu ulo�it ��dek do temp_zakazek
//    uloz_Defaulttemp_zakazku();
//    uloz_Default_cestu();
		loadHeader();
  }
  else // pokud je ulo�eno n�co v zak�zk�ch tak je na�tu
  {
		nacti_zakazky();
	}
}
// ---------------------------------------------------------------------------
//provede p�epnut� jazyka na komponent�ch, mGridy se zm�n� automaticky p�i vytv��en�
void TForm_definice_zakazek::nastav_jazyk()
{
	scLabel_header->Caption=F->ls->Strings[432];
	scGPGlyphButton_add_zakazka->Hint=F->ls->Strings[433];
	scGPButton_Ulozit->Caption=F->ls->Strings[24];
	scGPButton_storno->Caption=F->ls->Strings[71];
}
// ---------------------------------------------------------------------------
// nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
void TForm_definice_zakazek::nacti_PP() {
  F->log(__func__); // logov�n�
}

// ---------------------------------------------------------------------------
// vytvo�� defaultn� �adek se zak�zkou
void TForm_definice_zakazek::predvypln_default_zakazku() {
  F->log(__func__); // logov�n�

}

// ----------------------------------------------------------------------------
// p�edvypln�n� default cesta
// void TForm_definice_zakazek::predvypln_cestu()
// ji� se nepou��v� pln� se z default_cestu() dat tak jako u�ivatlsky definovan� zak�zky
// {
// Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
// int i=0;
// while(objekt!=NULL)
// {
// i++;
// Form_cesty->rStringGridEd_cesty->Cells[0][i]=i;//po�ad�
// Form_cesty->rStringGridEd_cesty->Cells[1][i]=objekt->name;
// if(objekt->rezim==1)//pokud je kontin�ln� tak CT spo��t� s�m
// Form_cesty->rStringGridEd_cesty->Cells[2][i]=60*objekt->delka_dopravniku/objekt->pohon->rychlost_od;//CT
// else
// Form_cesty->rStringGridEd_cesty->Cells[2][i]="0";//CT
// Form_cesty->rStringGridEd_cesty->Cells[3][i]=objekt->pohon->rychlost_od;//Rychlost dopravniku, minim�ln� ze zadan�ho rozmez�
//
// //pokud jde o lakovani, predvyplnim hodnoty jinak jsou 0 pro ostatni objekty
// //!!!!ned�lat pro LAK ale pro objekt typu resp ID odpov�daj�c� lak
// if(objekt->short_name=="LAK")
// {
// Form_cesty->rStringGridEd_cesty->Cells[4][i]="0";  //Cas vymeny
// Form_cesty->rStringGridEd_cesty->Cells[5][i]="0";  //Cas cisteni
// Form_cesty->rStringGridEd_cesty->Cells[6][i]="0";  //Opakovani
// Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ano"; //priznak pruchodu cesty
// }
// else
// {
// Form_cesty->rStringGridEd_cesty->Cells[4][i]="0";  //Cas vymeny
// Form_cesty->rStringGridEd_cesty->Cells[5][i]="0";  //Cas cisteni
// Form_cesty->rStringGridEd_cesty->Cells[6][i]="0";  //Opakovani
// Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ano";  //priznak pruchodu cesty
// }
//
// Form_cesty->rStringGridEd_cesty->RowCount=i+1;
// objekt=objekt->dalsi;
// }
// }
// ---------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Defaulttemp_zakazku() {
  F->log(__func__); // logov�n�
  Cvektory::TJig j;
  // defaultn� hodnoty jigu
  j.sirka = Form1->d.v.PP.sirka_jig;
  j.delka = Form1->d.v.PP.delka_jig;
  j.vyska = Form1->d.v.PP.vyska_jig;
  j.ks = 1;

  // mGrid->Refresh();
  // ROSTA TODO P�EINDEXOVAT
  // F->d.v.vloz_temp_zakazku("id",0,"Zakazka ",clBlack,0,0,j,0,0,0);
}

// ----------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Default_cestu() {
  F->log(__func__); // logov�n�
  // Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
  // Cvektory::TZakazka *default_zakazka = Form1->d.v.vrat_temp_zakazku(1);
  // Form1->d.v.inicializace_cesty(default_zakazka);
  // while (objekt != NULL)
  // { // vlo�en� defauln� cesty                        Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]                                        //v tomto p��pad� z�m�rn� takto proto�e se p�eb�r� vypo�ten� hodnota
  // Form1->d.v.vloz_segment_cesty(default_zakazka,
  // /* sloupec po�ad� se neukl�d� */ objekt->n, objekt->CT, 0, 0,
  // objekt->RD, 0);
  // objekt = objekt->dalsi;
  // }
}

// ---------------------------------------------------------------------------
// na�ten� hotov�ch zak�zek, tato metoda je vol�na z FormShow(), pokud existuj� zak�zky
void TForm_definice_zakazek::nacti_zakazky() {
	ShowMessage("nacti_zakazky");
	F->log(__func__); // logov�n�
//  ////vypln�n� stringgridu
//  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
//  // ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
//  Cvektory::TZakazka *ukaz2 = Form1->d.v.ZAKAZKY->predchozi;
//  mGrid->RowCount = ukaz2->n * 2;
//  // p�iprav�m si po�et ��dk� tabulky podle po�tu zak�zek *2 (slou�en� 2 ��dky)
//  int i = 0;
//  // Memo4->Lines->Add(AnsiString(ukaz->id));
//
//  while (ukaz != NULL) {
//		nacitam_zakazky = true;
//		barva = (TColor)ukaz->barva;
//    loadHeader(i, false);
//    i++; // zv��en� indexu ��dku
//
//    mGrid->Refresh(); // mus� b�t
//    TscGPComboBox *C = mGrid->getCombo(1, i);
//    TscGPListBoxItem *I;
//    mGrid->Cells[1][i - 1].Text = ukaz->name;
//    I = C->Items->Add();
//    I->Caption = "Servisn�";
//    I = C->Items->Add();
//    I->Caption = "B�n�";
//    if (ukaz->typ == 0)
//      C->ItemIndex = 0;
//    else
//      C->ItemIndex = 1;
//
//    mGrid->getButton(2, i)->Options->NormalColor = (TColor)ukaz->barva;
//    mGrid->Cells[4][i].Text = ukaz->pocet_voziku;
//    mGrid->Cells[6][i].Text = ukaz->serv_vozik_pocet;
//    mGrid->Cells[9][i].Text = ukaz->opakov_servis;
//    mGrid->Cells[10][i].Text = ukaz->TT;
//    mGrid->Cells[11][i].Text = ukaz->id;
//
//    // vlo��m dal�� ��dek (resp. nav���m jejich po��tadlo, po��t� se od 1)
//		// rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky
//
//		i++; // dal�� zv��en� indexu ��dku
//		// posun na dal�� prvek v seznamu
//		ukaz = ukaz->dalsi;
//	}
//	nacitam_zakazky = false;
//	// ulo�en� ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejn� data jako jsou zobrazena
//	Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp(); // pouze ZAKAZKY_temp=ZAKAZKY

	F->d.v.kopirujZAKAZKY2ZAKAZKY_temp();//p�ekop�ruje zak�zky do temp zak�zek, pro jejich editaci
	//proch�zen� vytvo�en�ch kopi� zak�zek a vytv��en� jejich mGrid�
	Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
	Cvektory::TDavka *D=NULL;
	nacitam_zakazky=true;//nutn�??
	while(Z!=NULL)
	{
		loadHeader(Z->n,false);//vytvo�en� tabulky zak�zky, nikoliv zak�zky a tabulky
		D=Z->davky->dalsi;
		unsigned int Col=3;
		while(D!=NULL)
		{
			Z->mGrid->InsertColumn(Col,true,true);
//			if(Z->mGrid->ColCount==4+1)Z->mGrid->Cells[Col][1].Type =Z->mGrid->glyphBUTTON; // smazani davky
//			setGlyphButtonDavka_Add(Z->mGrid->ID,Col+1);   //vytvo�� glyph na p�id�n� d�vky
//			setGlyphButtonDavka_Remove(Z->mGrid->ID,Col);   //vytvo�� glyph na smaz�n� d�vky
			Col+=1;
			D=D->dalsi;
		}
		delete D;D=NULL;
		//na�ten� informac� ze zak�zky do mGridu �e�eno tady nebo v metod� loadHeader()
		Z=Z->dalsi;
	}
	//vykreslen�
	FormPaint(this);
	//ukazatelov� z�le�itosti
	delete Z;Z=NULL;
	nacitam_zakazky=false;//nutn�??
}

// ----------------------------------------------------------------------------
// vol�n� sub formul��� JIG a CESTA a p��padn� ukl�d�n� do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender) {
	// definice ukazatele na aktu�ln� editovanou zak�zku     //��slo ��dku kde kliku do�lo

  ShowMessage("staryStringGrid");
//	Cvektory::TZakazka *zakazka =
//			Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
//	AnsiString prochazet;
//
//  ////////////cesty form - byl klik na bu�ku 9. sloupci
//	if (rStringGridEd1->Col == 9) {
//    // Form_cesty->rStringGridEd_cesty->Columns->Clear();
//    ////napln�n� picklistu
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");
//
//		// AKTUALIZACE SEGMENT� DLE PARAMETR� OBJEKTU U PRVN� ZAK�ZKY,v p��pad� prvn� zak�zky se berou hodnoty z parametr� objektu nikoliv zak�zky, co� zajist� pat�i�nou aktuliazaci
//		// to sam� je vol�no v unit1.cpp v Nastavitparametry1Click z ur�it�ho pohledu se jedn� o duplicitn� algoritmus, ale v p��pad� aktualizac� jinak ne� p�es parametry objektu lze pova�ovat za nutnost
//		if (zakazka->n == 1) // pouze pokud se jedn� o prvn� zak�zku
//		{
//			Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
//			while (O != NULL)
//          // proch�z� v�echnyobjekty a bu� je ("Ano") objekt i na cest� nebo nen� ("Ne")
//      {
//				Cvektory::TCesta *C = NULL;//Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
//        if (C != NULL) // objekt je segmentem cesty
//        {
//				 // C->CT = O->CT;
//					//C->RD = O->RD;
//          // v p��pad� prvn� zak�zky se berou hodnoty z parametr� objektu nikoliv zak�zky, co� zajist� pat�i�nou aktuliazaci
//        }
//        O = O->dalsi;
//      }
//    }
//
//    // AKTAUALIZACE CT,RD DLE TT dle TT temp zakazky je rozdilny oproti TT ve stringgridu - budu prepocitavat hodnoty RD,CT na Ceste
//    // if(Form1->ms.MyToDouble(zakazka->TT)!=Form1->ms.MyToDouble(rStringGridEd1->Cells[10][rStringGridEd1->Row]))  {
//    // ShowMessage("prepocitej");
//		//Form1->d.v.aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(zakazka,
//				//Form1->ms.MyToDouble(rStringGridEd1->Cells[10][rStringGridEd1->Row]));
//		// }
//
//    ////na��t�n� dat - nen� t�eba o�et�ovat, proto�e existuje default cesta v seznamu
//    // vyp�e d��ve nadefinovanou cestu v�etn� segment� z nejdel�� mo�n� cesty, kter� nejsou sou��st� aktu�ln� cesty
//    Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
//    // p��d�n� po�tu ��dk�
//    Form_cesty->rStringGridEd_cesty->RowCount =
//        Form1->d.v.OBJEKTY->predchozi->n + 1;
//		// p�id�n� dal��ho ��dku     //pridani k zobrazeni if ukaz param
//    int i = 0;
//    while (O != NULL)
//        // proch�z� v�echnyobjekty a bu� je ("Ano") objekt i na cest� nebo nen� ("Ne")
//    {
//      i++;
//			Cvektory::TCesta *C = NULL;//Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
//			if (C != NULL) // zahrnut� segmenty cesty
//      {
//				Form_cesty->rStringGridEd_cesty->Cells[0][i] = C->objekt->n;
//        Form_cesty->rStringGridEd_cesty->Cells[1][i] = C->objekt->name;
//        Form_cesty->rStringGridEd_cesty->Cells[2][i] = C->CT;
//        Form_cesty->rStringGridEd_cesty->Cells[3][i] = C->RD * 60;
//        // zobraz�m v�dy v [m/min]
//				Form_cesty->rStringGridEd_cesty->Cells[4][i] = C->Tv;
//        Form_cesty->rStringGridEd_cesty->Cells[5][i] = C->Tc;
//        Form_cesty->rStringGridEd_cesty->Cells[6][i] = C->Opak;
//        Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ano";
//      }
//      else // nezahrnut� segment nejdel�� cesty v aktu�ln� cest�
//      {
//				Form_cesty->rStringGridEd_cesty->Cells[0][i] = O->n;
//        Form_cesty->rStringGridEd_cesty->Cells[1][i] = O->name;
//        Form_cesty->rStringGridEd_cesty->Cells[2][i] = "0";
//        Form_cesty->rStringGridEd_cesty->Cells[3][i] = "0";
//        Form_cesty->rStringGridEd_cesty->Cells[4][i] = "0";
//        Form_cesty->rStringGridEd_cesty->Cells[5][i] = "0";
//        Form_cesty->rStringGridEd_cesty->Cells[6][i] = "0";
//				Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ne";
//      }
//      O = O->dalsi;
//    }
//
//    ////--------------
//
//		////ukl�d�n� dat - jednotliv�ho segmentu cesty pokud je pova�ov�n k zahrnutn� ze strany u�ivatele
//    // formul�� na st�ed
//    Form_cesty->Left = Form1->ClientWidth / 2 - Form_cesty->Width / 2;
//    Form_cesty->Top = Form1->ClientHeight / 2 - Form_cesty->Height / 2;
//
//    double CT;
//    double Tv;
//		double Tc;
//
//    if (mrOk == Form_cesty->ShowModal()) {
//      Form1->d.v.inicializace_cesty(zakazka);
//      for (int i = 1; i < Form_cesty->rStringGridEd_cesty->RowCount; i++) {
//        if (Form_cesty->rStringGridEd_cesty->Cells[7][i] == "Ano")
//            // pokud je za�krnuto neproch�zek objekt se neulo�� do cesty
//				{
//          if (Form_cesty->rStringGridEd_cesty->Columns->Items[2]
//              ->TitleCaption == "CT [s]") { // pokud je v sekundach
//            CT = Form1->ms.MyToDouble
//                (Form_cesty->rStringGridEd_cesty->Cells[2][i]);
//						Tv = Form1->ms.MyToDouble
//								(Form_cesty->rStringGridEd_cesty->Cells[4][i]);
//						Tc = Form1->ms.MyToDouble
//								(Form_cesty->rStringGridEd_cesty->Cells[5][i]);
//					}
//					else { // p�evedu minuty na sekundy
//            CT = Form1->ms.MyToDouble
//								(Form_cesty->rStringGridEd_cesty->Cells[2][i]) * 60.0;
//            // min na sekundy
//						Tv = Form1->ms.MyToDouble
//								(Form_cesty->rStringGridEd_cesty->Cells[4][i]) * 60.0;
//						Tc = Form1->ms.MyToDouble
//								(Form_cesty->rStringGridEd_cesty->Cells[5][i]) * 60.0;
//					}
//
////          Form1->d.v.vloz_segment_cesty(zakazka,
////							/* sloupec po�ad� se neukl�d� */ /* pozor na �azen� */ Form_cesty
////							->rStringGridEd_cesty->Cells[0][i].ToInt(), // ID-do�e�it
////							Form1->ms.MyToDouble(CT), // CT
////							Form1->ms.MyToDouble(Tv), // Tv
////							Form1->ms.MyToDouble(Tc), // Tc
////							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i])
////							/ 60.0, // RD ulozim v m/sec
////							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])
////							// Opak   //ulozeni stavu pro cestu - roletka
////							);
//				}
//				// vymaz�n� textu z ji� nepot�ebn�ho ��dku
//				Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();
//
//			}
//		}
//	}

}

// ---------------------------------------------------------------------------
// TLA��TKO ULO�IT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
	ShowMessage("ulozit");
  F->log(__func__); // logov�n�
//  zmena_TT = false;
//  bool neukladat = false; // pokud nebudou spln�ny podm�nky, nelze form ulo�it
//  bool dal_ok = false;
//
//  TColor barva; // mus� b�t v TColor kv�li ukl�d�n� do TColor objektu!!!
//
//  // ulo�en� editovan�ch zak�zek
//  for (unsigned int i = 1; i < mGrid->RowCount;i++) // dole je�t� zvy�uji po��tadlo!!!
//  {
//
//    double pomer;
//    double TT;
//    double nvoziku;
//    double nservis_voziku;
//    double nopak_voziku;
//    UnicodeString nazev;
//
//    // o�et�en� - pokud se p�i ukl�d�n� formu objev� pr�zdn� hodnoty, automaticky vypln�m "0" a p�id�m n�zev zak�zky
//
//    // NEW
//    if (mGrid->Cells[1][i - 1].Text.IsEmpty())
//      nazev = "Nov� zak�zka";
//    else
//      nazev = mGrid->Cells[1][i - 1].Text;
//
//    pomer = 100;
//
//    if (mGrid->Cells[10][i].Text.IsEmpty())
//      TT = 0;
//    else
//      TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);
//
//    if (mGrid->Cells[4][i].Text.IsEmpty())
//      nvoziku = 0;
//    else
//      nvoziku = mGrid->Cells[4][i].Text.ToInt();
//
//    if (mGrid->Cells[6][i].Text.IsEmpty())
//      nservis_voziku = 0;
//    else
//      nservis_voziku = mGrid->Cells[6][i].Text.ToInt();
//
//    if (mGrid->Cells[9][i].Text.IsEmpty())
//      nopak_voziku = 0;
//    else
//      nopak_voziku = mGrid->Cells[9][i].Text.ToInt();
//
//    bool usersColor = true;
//
//    mGrid->Refresh(); // mus� b�t
//    TscGPComboBox *C = mGrid->getCombo(1, i);
//    if (usersColor)
//      barva = (TColor)mGrid->getButton(2, i)->Options->NormalColor;
//    // mus� b�t a� tady, jinak spadne
//    // ShowMessage(C->ItemIndex);
//    // ulo�en� aktu�ln�ch hodnot do dan� temp_zakazky
//
//    if (i == 1) // ulo� TT do projektu pro status n�vrh��, pouze z prvn� zak�zky
//          Form1->d.v.PP.TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);
//
//    Form1->d.v.edituj_temp_zakazku(mGrid->Cells[11][i].Text.ToInt(), // n
//        mGrid->Cells[11][i].Text.ToInt(), // ID
//        C->ItemIndex, // TYP
//        nazev, // NAME
//        barva, // COLOR
//        pomer, // POMER
//        TT, // TT
//        // JIG se u� jako parametr nepo�aduje, stejn� jako cesta, jedn� se o p�ed�v�n� ukazatelem p�i zav�r�n� pat�i�n�ho formul��e
//        nvoziku, // n-voz�ku
//        nservis_voziku, // n-servis voz�k�
//        nopak_voziku // n-opak voz�k�
//        );
//    i++; // na��t�m ob jeden ��dek!!!
//  }
//  // samotn� ulo�en�
//  Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
//  // ulo�en� do ostr�ho spoj�ku ZAKAZKY+sm�znut� ZAKAZKY_temp
//  Form1->d.v.generuj_VOZIKY(); // vygenerov�n� voz�k� dle zadan�ch zak�zek
//  // konstrukce kter� m� vz�t CT a RD objektu z prvn� zak�zky a na�it ho do CT a RD objekt�
//  Cvektory::TObjekt *Obj = Form1->d.v.OBJEKTY->dalsi;
//  while (Obj != NULL) {
//    Cvektory::TCesta *Segment_cesty = Form1->d.v.obsahuje_segment_cesty_objekt
//        (Obj, Form1->d.v.ZAKAZKY->dalsi);
//    if (Segment_cesty != NULL) {
//      Obj->CT = Segment_cesty->CT; // ShowMessage(Obj->CT);
//      Obj->RD = Segment_cesty->RD;
//    }
//    Obj = Obj->dalsi;
//  }
//
//  if (dal_ok) {
//    // aktualiz fce pro CT,RD zakazky
//    Form1->d.v.aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky();
//
//  }
//
//  // zav�en� formul��e s n�sledn�m DuvodUlozit(true) po modalshow v unit1
//  if (neukladat == false)
//    Form_definice_zakazek->Close(); // kdyby n�co tak nic

	//na�ten� hodnot z tabulek mGrid� do temp zak�zek
	//maz�n� mgrid� zak�zek, d�le�it�, kop�rov�n� zak�zek nesma�e mGridy ale odstran� zak�zky temp ukazatel
	if(F->d.v.ZAKAZKY_temp!=NULL)
	{
		Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
		while(Z!=NULL)
		{
			Z->mGrid->Delete();
			Z->mGrid=NULL;
			Z=Z->dalsi;
		}
		delete Z;Z=NULL;
	}
	//kop�rov�n� temp zak�zek do ostr�ch zak�zek
	F->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
	//ukon�en� formu a smaz�n� temp zak�zek
	KonecClick(Sender);
}

// ---------------------------------------------------------------------------
// Zav�en� formul��e (storno a k��ek je to sam�)
void __fastcall TForm_definice_zakazek::scGPGlyphButton4Click(TObject *Sender) {
	KonecClick(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::KonecClick(TObject *Sender) {
	for (int i = 1; i <= rStringGridEd1->RowCount; i++) {
    rStringGridEd1->Rows[i]->Clear(); // promaznuti radku, ktere nebudou ulozeny
  }
  // maz�n� mgrid� zak�zek
  if (F->d.v.ZAKAZKY_temp != NULL) {
		Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
		while(Z!=NULL)
		{
			Z->mGrid->Delete();
			Z->mGrid=NULL;
			Z=Z->dalsi;
		}
		delete Z;Z=NULL;
    Z = NULL;
  }

  Form1->d.v.vymaz_seznam_ZAKAZKY_temp();
	Close();
}

// ---------------------------------------------------------------------------

// zkop�ruje p�edchoz�
void __fastcall TForm_definice_zakazek::scGPGlyphButton_add_zakazkaClick
		(TObject *Sender) {

  // mGrid->RowCount = mGrid->RowCount + 2;
  //
  // int i = mGrid->RowCount - 1;
  // Cvektory::TJig j;
  // j.sirka = 1;
  // j.delka = 1;
  // j.vyska = 1;
  // j.ks = 1;
  //
  // if (i > 1)
  // { // defaultn� zak�zka je ulo�ena hned v temp_spojaku + hlavnim pri form_show, �ili tady ukl�d�m a� dal�� ��dky
  //
  // Cvektory::TZakazka *ukaz = F->d.v.ZAKAZKY_temp->predchozi;
  //
  // add_zakazka = true;
  // loadHeader(i, false);
  // mGrid->Cells[1][i - 1].Text = "Nov� zak�zka";
  // mGrid->Cells[4][i].Text = "100";
  // mGrid->Cells[6][i].Text = "0";
  // mGrid->Cells[9][i].Text = "0";
  // mGrid->Cells[10][i].Text = "120";
  // mGrid->Cells[11][i].Text = ukaz->n + 1;
  //
  // mGrid->Refresh();
  // // kv�li pr�ci s combem je nutn� refresh po nastaven� na typ COMBOEDIT
  // TscGPComboBox *C = mGrid->getCombo(1, i);
  // TscGPListBoxItem *I;
  // // na�ten� hodnoty rozte�e do roletky + nastaven� jako ItemIndex=0
  // I = C->Items->Add();
  // I->Caption = "Servisn�";
  // C->ItemIndex = 0;
  // I = C->Items->Add();
  // I->Caption = "B�n�";
  // C->ItemIndex = 1;
  //
  // Form1->d.v.vloz_temp_zakazku(UnicodeString(mGrid->Cells[11][i].Text),
  // C->ItemIndex, UnicodeString(mGrid->Cells[1][i - 1].Text), clRed,
  // // Form_color_dialog->scColorGrid1->ColorValue,
  // Form1->ms.MyToDouble(100),
  // Form1->ms.MyToDouble(mGrid->Cells[10][i].Text), j,
  // mGrid->Cells[4][i].Text.ToInt(), mGrid->Cells[6][i].Text.ToInt(),
  // mGrid->Cells[9][i].Text.ToInt());
  //
  // // int j=rStringGridEd1->RowCount-1;
  // int j = mGrid->Cells[11][i].Text.ToInt();
  //
  // // p�i p�id�n� dal�� zak�zky ulo��m do cesty defaultn� hodnoty
  // Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
  // Cvektory::TZakazka *nova_zakazka = Form1->d.v.vrat_temp_zakazku(j);
  // Form1->d.v.inicializace_cesty(nova_zakazka);
  // while (objekt != NULL) { // vlo�en� defauln� cesty
  // Form1->d.v.vloz_segment_cesty(nova_zakazka,
  // /* sloupec po�ad� se neukl�d� */ objekt->n, 0, 0, 0, 0, 0);
  // objekt = objekt->dalsi;
  //
  // }
  //
  // }
  // add_zakazka = false;
  // ShowMessage("add_zakazka");
  // add_zakazka=true;
  // for(int i=0;i<=5;i++)
  // {
  // mGrid->AddRow(true,false); //vytvo��m si pr�zdn� ��dky
  // }
  // loadHeader(mGrid->RowCount-6,false);  //nadesignuji ��dky(-6) z�m�rn�, abych �el od prvn�ho nov�ho ��dku
  //
  // add_zakazka=false;
  // //mGrid->Refresh();
  // FormPaint(this);

loadHeader();
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::FormPaint(TObject *Sender) {
  // vykreslov�n� mGrid� zak�zek
	F->log(__func__);
	//vytvo�en� bmp
	Graphics::TBitmap *bmp_total=new Graphics::TBitmap;bmp_total->Width=Form_definice_zakazek->Width;bmp_total->Height=Form_definice_zakazek->Height;
	//vykreslen� podkladn� barvy
	bmp_total->Canvas->Brush->Color=light_gray;
	bmp_total->Canvas->FillRect(TRect(0,0,bmp_total->Width,bmp_total->Height));
  //vykresl�n� mGrid�
    Cvektory::TZakazka *Z = F->d.v.ZAKAZKY_temp->dalsi;
    while (Z != NULL) {
		{
      // dynamick� pozicov�n� tabulek
      if (Z->predchozi->n == 0)
        Z->mGrid->Top = scGPButton_plan_vyroby->Height +
            scLabel_header->Height + Z->mGrid->Rows[0].Height;
      else
        Z->mGrid->Top = Z->predchozi->mGrid->Top + Z->predchozi->mGrid->Height +
            Z->mGrid->Rows[0].Height;
			//vykreslen� tabulky
			Z->mGrid->Show(bmp_total->Canvas);
      Z = Z->dalsi;
    }
		delete Z;Z=NULL;
    Z = NULL;
  }
	//fin�ln� p�ed�n� bmp_out do Canvasu
	Canvas->Draw(0,0,bmp_total);
	delete (bmp_total);//velice nutn�
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::rEditNum_pocet_dnuKeyDown
    (TObject *Sender, WORD &Key, TShiftState Shift) {
  if (Key == 13) // ENTER
  {
    if (scGPButton_Ulozit->Enabled)
        // pokud jsou z�rove� spln�ny podm�nky pro stisk OK
    {
      Form_definice_zakazek->ModalResult = mrOk;
      // vr�t� stejnou hodnotu jako tla��tko
      Form_definice_zakazek->VisibleChanging();
      // skryje form, stejn� jako visible=false
    }
    else
      MessageBeep(0); // p�pnut�m upozorn�, �e nelze
  }
  if (Key == 27) // ESC
  {
    Form_definice_zakazek->ModalResult = mrCancel;
    // vr�t� stejnou hodnotu jako tla��tko
    Form_definice_zakazek->VisibleChanging();
    // skryje form, stejn� jako visible=false
  }
}
// ---------------------------------------------------------------------------

void TForm_definice_zakazek::getmGridWidth() {
  F->log(__func__); // logov�n�

  // mGrid->Columns[4].Width = 25;

}

void TForm_definice_zakazek::setGlyphButtonDefault(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z) {

  // ShowMessage("default glyphy");
  if (typ == krizek_davky) {
    // F->log(__func__); // logov�n�
    // TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, Row);
    // H->GlyphOptions->Kind = scgpbgkClose;
    // H->GlyphOptions->Thickness = 1;
    // H->ShowCaption = true;
    // H->Caption = " D�vka";
    // // H->Font->Color=(TColor)RGB(43,87,154);
    // H->Options->NormalColor = clWhite;
    // H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
    // H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
    // H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
    // H->Options->NormalColorAlpha = 255;
    // H->Options->FrameWidth = 1;
    // H->Options->FrameNormalColor = clWhite;
    // H->Width = 80;
    // H->Height = Z->mGrid->DefaultRowHeight;
    // H->Options->ShapeStyle = scgpRect;
    // // H->Width=30;
    // H->Hint = "Smazat d�vku";
    // H->ShowHint = true;
    //
    // H = NULL;
    // delete H;
    //
    TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col + 1, Row);
    J->GlyphOptions->Kind = scgpbgkPlus;
    J->GlyphOptions->Thickness = 1;
    J->ShowCaption = true;
    J->Caption = "";
    // H->Font->Color=(TColor)RGB(43,87,154);
    J->Options->NormalColor = clWhite;
    J->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
    J->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
    J->Options->FontHotColor = (TColor)RGB(43, 87, 154);
    J->Options->NormalColorAlpha = 255;
    J->Options->FrameWidth = 1;
    J->Options->FrameNormalColor = clWhite;
    J->Width = 23;
    J->Height = Z->mGrid->DefaultRowHeight;
    J->Options->ShapeStyle = scgpRect;
    // H->Width=30;
		J->Hint = F->ls->Strings[438];//"Vytvo�it novou d�vku"
    J->ShowHint = true;

    J = NULL;
    delete J;
  }

  if (typ == krizek) {
    F->log(__func__); // logov�n�
    TscGPGlyphButton *H = Z->mGrid->getGlyphButton(0, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->Options->NormalColor = clWhite;
    H->Options->NormalColorAlpha = 255;
    H->Options->FrameWidth = 1;
    H->Options->FrameNormalColor = clWhite;
    H->Width = 30;
    H->Height = Z->mGrid->DefaultRowHeight;
    H->Options->ShapeStyle = scgpRect;
		// H->Width=30;
		H->Hint = F->ls->Strings[439];//"Smazat zak�zku"
		H->ShowHint = true;

    H = NULL;
    delete H;
  }
}

void TForm_definice_zakazek::OnClick(long Tag, long ID, unsigned long Col, unsigned long Row) {
  F->log(__func__); // logov�n�
  // na�ten� aktu�ln� editovan� zak�zky
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku(ID);

  if (Col >= 3 && Row == 1 && volno == true)
  {
   TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, Row);
    // V�TEV NA P�ID�V�N� D�VEK DO ZAK�ZKY
    if (J->GlyphOptions->Kind == scgpbgkPlus) {
      volno = false;
      // backup p�vodn�ho n�zvu zak�zky pro n�sledn� merge bunek a napln�n�
      AnsiString nazev = Z->mGrid->Cells[1][0].Text;
      scGPButton_Ulozit->SetFocus();
      Z->mGrid->InsertColumn(Col, true, true);
      Z->mGrid->MergeCells(1, 0, 2, 0); // n�zev     - vodorovne
      // napln�n� p�vodn�m textem
      Z->mGrid->Cells[1][0].Text = nazev;
      // pokud neexistuj� d�vky tak p�i prvn�m p�id�n� d�vky nastav�m typ na glyphbutton
      if (Z->mGrid->ColCount == 4 + 1) Z->mGrid->Cells[Col][1].Type = Z->mGrid->glyphBUTTON;
      Z->mGrid->Update();
      setGlyphButtonDavka_Add(ID, Col + 1); // vytvo�� glyph na p�id�n� d�vky
      setGlyphButtonDavka_Remove(ID, Col,Z); // vytvo�� glyph na smaz�n� d�vky
      volno = true;
					F->d.v.vloz_davku(Z,0,0,0);
    }
    // V�TEV NA MAZ�N� D�VEK ZE ZAK�ZKY
    if (J->GlyphOptions->Kind == scgpbgkClose && volno == true)
    {
      volno = false;
      scGPButton_Ulozit->SetFocus();
      Z->mGrid->DeleteColumn(Col,true);
      Z->mGrid->Update();
     // setGlyphButtonDavka_Add(Row, Z->mGrid->ColCount - 1);
      // vytvo�� glyph na p�id�n� d�vky
      volno = true;
					F->d.v.smaz_davku(Z,Col-2);
    }

    J = NULL;
    delete J;
  }

  if(Col==1 && Row == 1)
  {
    TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, Row);
    if (J->GlyphOptions->Kind == scgpbgkMore && volno == true)
    {
     Form_color_dialog->SetDialogPosition(1,ID);
     Form_color_dialog->ShowModal();
    }
   J = NULL;
   delete J;
  }
  // odstran�nn� tabulky a zak�zky
  if (Col == 0 && Row == 0) {
    Konec->SetFocus();
    Z->mGrid->Delete();
    Z->mGrid = NULL;
    F->d.v.smaz_temp_zakazku(Z->n);
    Invalidate(); // formpaint nesta�� nep�ekresl� poz�statky tabulky
    // FormPaint(this);
  }
  // odmaz�n� pomocn�ho ukazatele
  Z = NULL;
  delete Z;
}

void TForm_definice_zakazek::setButtonColor(long ID)
{
  F->log(__func__); // logov�n�
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku(ID);
  Z->mGrid->getGlyphButton(1, 1)->Options->HotColor = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->HotColorAlpha = 255;
  Z->mGrid->getGlyphButton(1, 1)->Options->FocusedColor = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FocusedColorAlpha = 225;
  Z->mGrid->getGlyphButton(1, 1)->Options->NormalColor = Form_color_dialog->scColorGrid1->ColorValue;
}

void TForm_definice_zakazek::loadHeader(unsigned long zakazka_n, bool novy)
{
	//if (novy || add_zakazka)
	{
		F->log(__func__); // logov�n�
    // ShowMessage("loadHeader");
		Cvektory::TZakazka *Z=NULL;
		if(novy)//vytv��en� nov� z�k�zky a jej�cho mGridu
		{
	  	Cvektory::TJig J;
	  	unsigned int n=1;
	  	if(F->d.v.ZAKAZKY_temp!=NULL && F->d.v.ZAKAZKY_temp->predchozi->n>0)n=F->d.v.ZAKAZKY_temp->predchozi->n+1;
			F->d.v.vloz_temp_zakazku("id",0,"Zakazka "+AnsiString(n),clBlack,0,0,J,0,0,0);
			Z=F->d.v.ZAKAZKY_temp->predchozi;
			F->d.v.inicializace_cesty(Z);//vytvo�� hlavi�ku cesty
		}
		else Z=F->d.v.vrat_temp_zakazku(zakazka_n);//vytv��en� mGridu stavaj�c� zak�zce
		////vytvo�en� mgridu nov� zak�zce
    Z->mGrid = new TmGrid(this);
    Z->mGrid->Tag = 9;
    Z->mGrid->ID = Z->n;
    Z->mGrid->Create(4, 5);
    Z->mGrid->SetColumnAutoFit(-4);

		//if(novy)
    {
      Z->mGrid->Columns[0].Width = 30;
      Z->mGrid->Columns[1].Width = 80;
      Z->mGrid->Columns[2].Width = 90;
      Z->mGrid->Columns[3].Width = 25;
    }
    ////parametry mgridu
    Z->mGrid->Left = 5;

    if (Z->predchozi->n > 0) Z->mGrid->Top = Z->predchozi->mGrid->Top + Z->predchozi->mGrid->Height + Z->mGrid->Rows[0].Height;
    else Z->mGrid->Top = Z->mGrid->Rows[0].Height + scGPButton_plan_vyroby->Height + scLabel_header->Height;

    Z->mGrid->Cells[1][0].Type = Z->mGrid->EDIT;
    // nazev  text  - slou�it pod�ln�
    Z->mGrid->Cells[0][0].Type = Z->mGrid->glyphBUTTON; // X  zakazka
    // Z->mGrid->Cells[2][i+1].Type = 	Z->mGrid->glyphBUTTON; // add  davka

    Z->mGrid->Cells[3][1].Type = Z->mGrid->glyphBUTTON; //

    Z->mGrid->Cells[0][1].Text = Z->n; // id zak�zky
			if(novy)Z->mGrid->Cells[1][i+0].Text = F->ls->Strings[434];//"N�zev zak�zky"
			else Z->mGrid->Cells[1][i+0].Text = Z->name;
    Z->mGrid->Cells[1][0].Text = "N�zev zak�zky";
    Z->mGrid->Cells[1][1].Type = Z->mGrid->glyphBUTTON; // color zakazka
    //Z->mGrid->Cells[2][1].Type = Z->mGrid->glyphBUTTON;
    Z->mGrid->Cells[1][0].Font->Size = 15;

    Z->mGrid->Cells[1][1].Font->Size = 14;
			Z->mGrid->Cells[1][i+2].Text = " "+F->ls->Strings[435];//" po�et"   	//Z->mGrid->Cells[1][2].RightBorder->Color = clWhite;
			Z->mGrid->Cells[1][i+3].Text = " "+F->ls->Strings[436];//" pr�zdn�ch"   //	Z->mGrid->Cells[1][3].RightBorder->Color = clWhite;
			Z->mGrid->Cells[1][i+4].Text = " "+F->ls->Strings[437];//" celkem"       //	Z->mGrid->Cells[1][4].RightBorder->Color = clWhite;
    // Z->mGrid->Cells[1][3].RightBorder->Color = clWhite;
    Z->mGrid->Cells[1][4].Text = " celkem";
    // Z->mGrid->Cells[1][4].RightBorder->Color = clWhite;

    Z->mGrid->Cells[1][2].Align = Z->mGrid->LEFT;
    Z->mGrid->Cells[1][3].Align = Z->mGrid->LEFT;
    Z->mGrid->Cells[1][4].Align = Z->mGrid->LEFT;

    Z->mGrid->Cells[2][2].Align = Z->mGrid->CENTER;
    Z->mGrid->Cells[2][3].Align = Z->mGrid->CENTER;
    Z->mGrid->Cells[2][4].Align = Z->mGrid->CENTER;

    Z->mGrid->Cells[2][2].Type = Z->mGrid->EDIT; //
    Z->mGrid->Cells[2][3].Type = Z->mGrid->EDIT;
    Z->mGrid->Cells[2][4].Type = Z->mGrid->EDIT;
    Z->mGrid->Cells[2][2].Text = "100"; // value
    Z->mGrid->Cells[2][3].Text = "3"; // value
    Z->mGrid->Cells[2][4].Text = "103"; // value

  //  Z->mGrid->Cells[1][i + 1].RightBorder->Color = clWhite;
    Z->mGrid->Cells[0][2].LeftBorder->Color = light_gray;
    // podbarven� sloupc� s "+"
    Z->mGrid->Cells[3][4].Background->Color = light_gray;
    Z->mGrid->Cells[3][4].Text = " ";
    Z->mGrid->Cells[3][4].RightBorder->Color = light_gray;
    Z->mGrid->Cells[3][4].BottomBorder->Color = light_gray;
    Z->mGrid->Cells[3][3].Background->Color = light_gray;
    Z->mGrid->Cells[3][3].Text = " ";
    Z->mGrid->Cells[3][3].RightBorder->Color = light_gray;
    Z->mGrid->Cells[3][3].BottomBorder->Color = light_gray;
    Z->mGrid->Cells[3][2].Background->Color = light_gray;
    Z->mGrid->Cells[3][2].Text = " ";
    Z->mGrid->Cells[3][2].RightBorder->Color = light_gray;
    Z->mGrid->Cells[3][2].BottomBorder->Color = light_gray;

    Z->mGrid->MergeCells(1, 0, 2, 0); // n�zev     - vodorovne
  //  Z->mGrid->MergeCells(1, i+1, 2, i+1); // merge color glyph
    Z->mGrid->MergeCells(0, 1, 0, 4); // merge ID �i obr�zek
    Z->mGrid->Update();
    // default rozm�st�n� glyphbutton�
    setGlyphButtonDefault(0, 0, krizek, Z);
    setGlyphButtonDefault(1, 2, krizek_davky, Z);
    setGlyphButtonColor(1, 1, color, Z);

    ////ukazatelov� z�le�itosti
    Z->mGrid->Refresh();

    Z = NULL;
    delete Z;
    ////vykreslen� mGrid�
	if(novy)FormPaint(this);//pouze v p��pad�, �e vytv���m novou zak�zku s mGridem, p�i na��t�n� ji� vytvo�en�ch zak�zek mus� doj�t k vykresl�n� a� budou v�ehny na�teny, volan� vykreslen� v metod� nacti_zakazky()
  }

}

void __fastcall TForm_definice_zakazek::FormClose(TObject *Sender,
    TCloseAction &Action)

{
  F->log(__func__); // logov�n�
  // Z->mGrid->Delete();
}
// ---------------------------------------------------------------------------

void TForm_definice_zakazek::setGlyphButtonDavka_Add(unsigned long ID,unsigned long Col)
{
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku(ID);
  TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, 1);
  J->GlyphOptions->Kind = scgpbgkPlus;
  J->GlyphOptions->Thickness = 1;
  J->ShowCaption = true;
  J->Caption = "";
  J->Options->NormalColor = clWhite;
  J->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
  J->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
  J->Options->FontHotColor = (TColor)RGB(43, 87, 154);
  J->Options->NormalColorAlpha = 255;
  J->Options->FrameWidth = 1;
  J->Options->FrameNormalColor = clWhite;
  J->Width = 25;
  J->Height = Z->mGrid->DefaultRowHeight;
  J->Options->ShapeStyle = scgpRect;
		J->Hint = F->ls->Strings[438];//"Vytvo�it novou d�vku"
  J->ShowHint = true;
  J = NULL;
  delete J;
}

void TForm_definice_zakazek::setGlyphButtonDavka_Remove(unsigned long ID, unsigned long Col, Cvektory::TZakazka *Z)
{
  F->log(__func__); // logov�n�
  TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, 1);
  H->GlyphOptions->Kind = scgpbgkClose;
  H->GlyphOptions->Thickness = 1;
  H->ShowCaption = true;
  if (Z->mGrid->ColCount == 5) pocet_davek = 1;
		H->Caption = " "+F->ls->Strings[440]+" "+AnsiString(pocet_davek);// " D�vka "
		// H->Font->Color=(TColor)RGB(43,87,154);
  H->Options->NormalColor = clWhite;
  H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
  H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
  H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
  H->Options->NormalColorAlpha = 255;
  H->Options->FrameWidth = 1;
  H->Options->FrameNormalColor = clWhite;
  H->Width = 80;
  H->Height = Z->mGrid->DefaultRowHeight;
  H->Options->ShapeStyle = scgpRect;
		H->Hint = F->ls->Strings[441];//"Smazat d�vku";
  H->ShowHint = true;
  H = NULL;
  delete H;
}

void TForm_definice_zakazek::setGlyphButtonColor(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z)
{
  F->log(__func__); // logov�n�
  TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, 1);
  H->GlyphOptions->Kind = scgpbgkMore;
  H->GlyphOptions->Thickness = 1;
  H->ShowCaption = true;
  H->Caption = "  ";
  H->Options->NormalColor = clWhite;
  H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
  H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
  H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
  H->Options->NormalColorAlpha = 255;
  H->Options->FrameWidth = 1;
  H->Options->FrameNormalColor = clWhite;
  H->Width = Z->mGrid->Columns[1].Width + Z->mGrid->Columns[2].Width - 3;
  H->Height = Z->mGrid->DefaultRowHeight;
  H->Options->ShapeStyle = scgpRect;
  H->Hint = "Barva zak�zky";
  H->ShowHint = true;
  H = NULL;
  delete H;
}
