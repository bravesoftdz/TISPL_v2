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

  nastav_form(); // nastavení barev komponent
  zmena_TT = false;
  add_zakazka = false;
  nacitam_zakazky = false;
}

// ---------------------------------------------------------------------------
// nastavení barev komponent
void TForm_definice_zakazek::nastav_form() {
  F->log(__func__); // logování
  // nastavení globálních barev
	light_gray = (TColor)RGB(240, 240, 240);
  def_gray = (TColor)RGB(200, 200, 200);

  rStringGridEd1->Color = clWhite; // nastavení pozadí barvy formuláøe
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
// zobrazení formuláøe
void __fastcall TForm_definice_zakazek::FormShow(TObject *Sender) {
  F->log(__func__); // logování
  Left = Form1->ClientWidth / 2 - Width / 2;
	Top = Form1->ClientHeight / 2 - Height / 2;
  volno = true;
  pocet_davek = 0;
  ////nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
	nacti_PP();
	nastav_jazyk();
	F->d.v.vytvor_default_zakazku();//vytvoøí nebo aktualizuje defaultní zakázku

  ////naètení zakázek a cest
  if (Form1->d.v.ZAKAZKY->dalsi == NULL) // kdyz je spojak prazdny
  {
    // vytvoøí defaultní øadek se zakázkou a její cestou

    // predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
    // a hned se uloží do temp spojáku
//    uloz_Defaulttemp_zakazku();
//    uloz_Default_cestu();
//    Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
//    // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
//    // po uložení do hl.spojaku zakázek potøebuji stejnì znovu uložit øádek do temp_zakazek
//    uloz_Defaulttemp_zakazku();
//    uloz_Default_cestu();
		loadHeader();
  }
  else // pokud je uloženo nìco v zakázkách tak je naètu
  {
		nacti_zakazky();
	}
}
// ---------------------------------------------------------------------------
//provede pøepnutí jazyka na komponentách, mGridy se zmìní automaticky pøi vytváøení
void TForm_definice_zakazek::nastav_jazyk()
{
	scLabel_header->Caption=F->ls->Strings[432];
	scGPGlyphButton_add_zakazka->Hint=F->ls->Strings[433];
	scGPButton_Ulozit->Caption=F->ls->Strings[24];
	scGPButton_storno->Caption=F->ls->Strings[71];
}
// ---------------------------------------------------------------------------
// nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
void TForm_definice_zakazek::nacti_PP() {
  F->log(__func__); // logování
}

// ---------------------------------------------------------------------------
// vytvoøí defaultní øadek se zakázkou
void TForm_definice_zakazek::predvypln_default_zakazku() {
  F->log(__func__); // logování

}

// ----------------------------------------------------------------------------
// pøedvyplnìní default cesta
// void TForm_definice_zakazek::predvypln_cestu()
// již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
// {
// Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
// int i=0;
// while(objekt!=NULL)
// {
// i++;
// Form_cesty->rStringGridEd_cesty->Cells[0][i]=i;//poøadí
// Form_cesty->rStringGridEd_cesty->Cells[1][i]=objekt->name;
// if(objekt->rezim==1)//pokud je kontinální tak CT spoèítá sám
// Form_cesty->rStringGridEd_cesty->Cells[2][i]=60*objekt->delka_dopravniku/objekt->pohon->rychlost_od;//CT
// else
// Form_cesty->rStringGridEd_cesty->Cells[2][i]="0";//CT
// Form_cesty->rStringGridEd_cesty->Cells[3][i]=objekt->pohon->rychlost_od;//Rychlost dopravniku, minimální ze zadaného rozmezí
//
// //pokud jde o lakovani, predvyplnim hodnoty jinak jsou 0 pro ostatni objekty
// //!!!!nedìlat pro LAK ale pro objekt typu resp ID odpovídající lak
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
  F->log(__func__); // logování
  Cvektory::TJig j;
  // defaultní hodnoty jigu
  j.sirka = Form1->d.v.PP.sirka_jig;
  j.delka = Form1->d.v.PP.delka_jig;
  j.vyska = Form1->d.v.PP.vyska_jig;
  j.ks = 1;

  // mGrid->Refresh();
  // ROSTA TODO PØEINDEXOVAT
  // F->d.v.vloz_temp_zakazku("id",0,"Zakazka ",clBlack,0,0,j,0,0,0);
}

// ----------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Default_cestu() {
  F->log(__func__); // logování
  // Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
  // Cvektory::TZakazka *default_zakazka = Form1->d.v.vrat_temp_zakazku(1);
  // Form1->d.v.inicializace_cesty(default_zakazka);
  // while (objekt != NULL)
  // { // vložení defaulní cesty                        Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]                                        //v tomto pøípadì zámìrnì takto protože se pøebírá vypoètená hodnota
  // Form1->d.v.vloz_segment_cesty(default_zakazka,
  // /* sloupec poøadí se neukládá */ objekt->n, objekt->CT, 0, 0,
  // objekt->RD, 0);
  // objekt = objekt->dalsi;
  // }
}

// ---------------------------------------------------------------------------
// naètení hotových zakázek, tato metoda je volána z FormShow(), pokud existují zakázky
void TForm_definice_zakazek::nacti_zakazky() {
	ShowMessage("nacti_zakazky");
	F->log(__func__); // logování
//  ////vyplnìní stringgridu
//  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
//  // ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
//  Cvektory::TZakazka *ukaz2 = Form1->d.v.ZAKAZKY->predchozi;
//  mGrid->RowCount = ukaz2->n * 2;
//  // pøipravím si poèet øádkù tabulky podle poètu zakázek *2 (slouèené 2 øádky)
//  int i = 0;
//  // Memo4->Lines->Add(AnsiString(ukaz->id));
//
//  while (ukaz != NULL) {
//		nacitam_zakazky = true;
//		barva = (TColor)ukaz->barva;
//    loadHeader(i, false);
//    i++; // zvýšení indexu øádku
//
//    mGrid->Refresh(); // musí být
//    TscGPComboBox *C = mGrid->getCombo(1, i);
//    TscGPListBoxItem *I;
//    mGrid->Cells[1][i - 1].Text = ukaz->name;
//    I = C->Items->Add();
//    I->Caption = "Servisní";
//    I = C->Items->Add();
//    I->Caption = "Bìžná";
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
//    // vložím další øádek (resp. navýším jejich poèítadlo, poèítá se od 1)
//		// rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky
//
//		i++; // další zvýšení indexu øádku
//		// posun na další prvek v seznamu
//		ukaz = ukaz->dalsi;
//	}
//	nacitam_zakazky = false;
//	// uložení ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejná data jako jsou zobrazena
//	Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp(); // pouze ZAKAZKY_temp=ZAKAZKY

	F->d.v.kopirujZAKAZKY2ZAKAZKY_temp();//pøekopíruje zakázky do temp zakázek, pro jejich editaci
	//procházení vytvoøených kopií zakázek a vytváøení jejich mGridù
	Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
	Cvektory::TDavka *D=NULL;
	nacitam_zakazky=true;//nutné??
	while(Z!=NULL)
	{
		loadHeader(Z->n,false);//vytvoøení tabulky zakázky, nikoliv zakázky a tabulky
		D=Z->davky->dalsi;
		unsigned int Col=3;
		while(D!=NULL)
		{
			Z->mGrid->InsertColumn(Col,true,true);
//			if(Z->mGrid->ColCount==4+1)Z->mGrid->Cells[Col][1].Type =Z->mGrid->glyphBUTTON; // smazani davky
//			setGlyphButtonDavka_Add(Z->mGrid->ID,Col+1);   //vytvoøí glyph na pøidání dávky
//			setGlyphButtonDavka_Remove(Z->mGrid->ID,Col);   //vytvoøí glyph na smazání dávky
			Col+=1;
			D=D->dalsi;
		}
		delete D;D=NULL;
		//naètení informací ze zakázky do mGridu øešeno tady nebo v metodì loadHeader()
		Z=Z->dalsi;
	}
	//vykreslení
	FormPaint(this);
	//ukazatelové záležitosti
	delete Z;Z=NULL;
	nacitam_zakazky=false;//nutné??
}

// ----------------------------------------------------------------------------
// volání sub formuláøù JIG a CESTA a pøípadné ukládání do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender) {
	// definice ukazatele na aktuálnì editovanou zakázku     //èíslo øádku kde kliku došlo

  ShowMessage("staryStringGrid");
//	Cvektory::TZakazka *zakazka =
//			Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
//	AnsiString prochazet;
//
//  ////////////cesty form - byl klik na buòku 9. sloupci
//	if (rStringGridEd1->Col == 9) {
//    // Form_cesty->rStringGridEd_cesty->Columns->Clear();
//    ////naplnìní picklistu
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
//		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");
//
//		// AKTUALIZACE SEGMENTÙ DLE PARAMETRÙ OBJEKTU U PRVNÍ ZAKÁZKY,v pøípadì první zakázky se berou hodnoty z parametrù objektu nikoliv zakázky, což zajistí patøiènou aktuliazaci
//		// to samé je voláno v unit1.cpp v Nastavitparametry1Click z urèitého pohledu se jedná o duplicitní algoritmus, ale v pøípadì aktualizací jinak než pøes parametry objektu lze považovat za nutnost
//		if (zakazka->n == 1) // pouze pokud se jedná o první zakázku
//		{
//			Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
//			while (O != NULL)
//          // prochází všechnyobjekty a buï je ("Ano") objekt i na cestì nebo není ("Ne")
//      {
//				Cvektory::TCesta *C = NULL;//Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
//        if (C != NULL) // objekt je segmentem cesty
//        {
//				 // C->CT = O->CT;
//					//C->RD = O->RD;
//          // v pøípadì první zakázky se berou hodnoty z parametrù objektu nikoliv zakázky, což zajistí patøiènou aktuliazaci
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
//    ////naèítání dat - není tøeba ošetøovat, protože existuje default cesta v seznamu
//    // vypíše døíve nadefinovanou cestu vèetnì segmentù z nejdelší možné cesty, které nejsou souèástí aktuální cesty
//    Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
//    // pøídání poètu øádkù
//    Form_cesty->rStringGridEd_cesty->RowCount =
//        Form1->d.v.OBJEKTY->predchozi->n + 1;
//		// pøidání dalšího øádku     //pridani k zobrazeni if ukaz param
//    int i = 0;
//    while (O != NULL)
//        // prochází všechnyobjekty a buï je ("Ano") objekt i na cestì nebo není ("Ne")
//    {
//      i++;
//			Cvektory::TCesta *C = NULL;//Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
//			if (C != NULL) // zahrnuté segmenty cesty
//      {
//				Form_cesty->rStringGridEd_cesty->Cells[0][i] = C->objekt->n;
//        Form_cesty->rStringGridEd_cesty->Cells[1][i] = C->objekt->name;
//        Form_cesty->rStringGridEd_cesty->Cells[2][i] = C->CT;
//        Form_cesty->rStringGridEd_cesty->Cells[3][i] = C->RD * 60;
//        // zobrazím vždy v [m/min]
//				Form_cesty->rStringGridEd_cesty->Cells[4][i] = C->Tv;
//        Form_cesty->rStringGridEd_cesty->Cells[5][i] = C->Tc;
//        Form_cesty->rStringGridEd_cesty->Cells[6][i] = C->Opak;
//        Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ano";
//      }
//      else // nezahrnutý segment nejdelší cesty v aktuální cestì
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
//		////ukládání dat - jednotlivého segmentu cesty pokud je považován k zahrnutní ze strany uživatele
//    // formuláø na støed
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
//            // pokud je zaškrnuto neprocházek objekt se neuloží do cesty
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
//					else { // pøevedu minuty na sekundy
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
////							/* sloupec poøadí se neukládá */ /* pozor na øazení */ Form_cesty
////							->rStringGridEd_cesty->Cells[0][i].ToInt(), // ID-doøešit
////							Form1->ms.MyToDouble(CT), // CT
////							Form1->ms.MyToDouble(Tv), // Tv
////							Form1->ms.MyToDouble(Tc), // Tc
////							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i])
////							/ 60.0, // RD ulozim v m/sec
////							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])
////							// Opak   //ulozeni stavu pro cestu - roletka
////							);
//				}
//				// vymazání textu z již nepotøebného øádku
//				Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();
//
//			}
//		}
//	}

}

// ---------------------------------------------------------------------------
// TLAÈÍTKO ULOŽIT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
	ShowMessage("ulozit");
  F->log(__func__); // logování
//  zmena_TT = false;
//  bool neukladat = false; // pokud nebudou splnìny podmínky, nelze form uložit
//  bool dal_ok = false;
//
//  TColor barva; // musí být v TColor kvùli ukládání do TColor objektu!!!
//
//  // uložení editovaných zakázek
//  for (unsigned int i = 1; i < mGrid->RowCount;i++) // dole ještì zvyšuji poèítadlo!!!
//  {
//
//    double pomer;
//    double TT;
//    double nvoziku;
//    double nservis_voziku;
//    double nopak_voziku;
//    UnicodeString nazev;
//
//    // ošetøení - pokud se pøi ukládání formu objeví prázdné hodnoty, automaticky vyplním "0" a pøidám název zakázky
//
//    // NEW
//    if (mGrid->Cells[1][i - 1].Text.IsEmpty())
//      nazev = "Nová zakázka";
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
//    mGrid->Refresh(); // musí být
//    TscGPComboBox *C = mGrid->getCombo(1, i);
//    if (usersColor)
//      barva = (TColor)mGrid->getButton(2, i)->Options->NormalColor;
//    // musí být až tady, jinak spadne
//    // ShowMessage(C->ItemIndex);
//    // uložení aktuálních hodnot do dané temp_zakazky
//
//    if (i == 1) // ulož TT do projektu pro status návrháø, pouze z první zakázky
//          Form1->d.v.PP.TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);
//
//    Form1->d.v.edituj_temp_zakazku(mGrid->Cells[11][i].Text.ToInt(), // n
//        mGrid->Cells[11][i].Text.ToInt(), // ID
//        C->ItemIndex, // TYP
//        nazev, // NAME
//        barva, // COLOR
//        pomer, // POMER
//        TT, // TT
//        // JIG se už jako parametr nepožaduje, stejnì jako cesta, jedná se o pøedávání ukazatelem pøi zavírání patøièného formuláøe
//        nvoziku, // n-vozíku
//        nservis_voziku, // n-servis vozíkù
//        nopak_voziku // n-opak vozíkù
//        );
//    i++; // naèítám ob jeden øádek!!!
//  }
//  // samotné uložení
//  Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
//  // uložení do ostrého spojáku ZAKAZKY+smáznutí ZAKAZKY_temp
//  Form1->d.v.generuj_VOZIKY(); // vygenerování vozíkù dle zadaných zakázek
//  // konstrukce která má vzít CT a RD objektu z první zakázky a našit ho do CT a RD objektù
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
//  // zavøení formuláøe s následným DuvodUlozit(true) po modalshow v unit1
//  if (neukladat == false)
//    Form_definice_zakazek->Close(); // kdyby nìco tak nic

	//naètení hodnot z tabulek mGridù do temp zakázek
	//mazání mgridù zakázek, dùležité, kopírování zakázek nesmaže mGridy ale odstraní zakázky temp ukazatel
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
	//kopírování temp zakázek do ostrých zakázek
	F->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
	//ukonèení formu a smazání temp zakázek
	KonecClick(Sender);
}

// ---------------------------------------------------------------------------
// Zavøení formuláøe (storno a køížek je to samé)
void __fastcall TForm_definice_zakazek::scGPGlyphButton4Click(TObject *Sender) {
	KonecClick(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::KonecClick(TObject *Sender) {
	for (int i = 1; i <= rStringGridEd1->RowCount; i++) {
    rStringGridEd1->Rows[i]->Clear(); // promaznuti radku, ktere nebudou ulozeny
  }
  // mazání mgridù zakázek
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

// zkopíruje pøedchozí
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
  // { // defaultní zakázka je uložena hned v temp_spojaku + hlavnim pri form_show, èili tady ukládám až další øádky
  //
  // Cvektory::TZakazka *ukaz = F->d.v.ZAKAZKY_temp->predchozi;
  //
  // add_zakazka = true;
  // loadHeader(i, false);
  // mGrid->Cells[1][i - 1].Text = "Nová zakázka";
  // mGrid->Cells[4][i].Text = "100";
  // mGrid->Cells[6][i].Text = "0";
  // mGrid->Cells[9][i].Text = "0";
  // mGrid->Cells[10][i].Text = "120";
  // mGrid->Cells[11][i].Text = ukaz->n + 1;
  //
  // mGrid->Refresh();
  // // kvùli práci s combem je nutný refresh po nastavení na typ COMBOEDIT
  // TscGPComboBox *C = mGrid->getCombo(1, i);
  // TscGPListBoxItem *I;
  // // naètení hodnoty rozteèe do roletky + nastavení jako ItemIndex=0
  // I = C->Items->Add();
  // I->Caption = "Servisní";
  // C->ItemIndex = 0;
  // I = C->Items->Add();
  // I->Caption = "Bìžná";
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
  // // pøi pøidání další zakázky uložím do cesty defaultní hodnoty
  // Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
  // Cvektory::TZakazka *nova_zakazka = Form1->d.v.vrat_temp_zakazku(j);
  // Form1->d.v.inicializace_cesty(nova_zakazka);
  // while (objekt != NULL) { // vložení defaulní cesty
  // Form1->d.v.vloz_segment_cesty(nova_zakazka,
  // /* sloupec poøadí se neukládá */ objekt->n, 0, 0, 0, 0, 0);
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
  // mGrid->AddRow(true,false); //vytvoøím si prázdné øádky
  // }
  // loadHeader(mGrid->RowCount-6,false);  //nadesignuji øádky(-6) zámìrnì, abych šel od prvního nového øádku
  //
  // add_zakazka=false;
  // //mGrid->Refresh();
  // FormPaint(this);

loadHeader();
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::FormPaint(TObject *Sender) {
  // vykreslování mGridù zakázek
	F->log(__func__);
	//vytvoøení bmp
	Graphics::TBitmap *bmp_total=new Graphics::TBitmap;bmp_total->Width=Form_definice_zakazek->Width;bmp_total->Height=Form_definice_zakazek->Height;
	//vykreslení podkladní barvy
	bmp_total->Canvas->Brush->Color=light_gray;
	bmp_total->Canvas->FillRect(TRect(0,0,bmp_total->Width,bmp_total->Height));
  //vykreslìní mGridù
    Cvektory::TZakazka *Z = F->d.v.ZAKAZKY_temp->dalsi;
    while (Z != NULL) {
		{
      // dynamické pozicování tabulek
      if (Z->predchozi->n == 0)
        Z->mGrid->Top = scGPButton_plan_vyroby->Height +
            scLabel_header->Height + Z->mGrid->Rows[0].Height;
      else
        Z->mGrid->Top = Z->predchozi->mGrid->Top + Z->predchozi->mGrid->Height +
            Z->mGrid->Rows[0].Height;
			//vykreslení tabulky
			Z->mGrid->Show(bmp_total->Canvas);
      Z = Z->dalsi;
    }
		delete Z;Z=NULL;
    Z = NULL;
  }
	//finální pøedání bmp_out do Canvasu
	Canvas->Draw(0,0,bmp_total);
	delete (bmp_total);//velice nutné
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::rEditNum_pocet_dnuKeyDown
    (TObject *Sender, WORD &Key, TShiftState Shift) {
  if (Key == 13) // ENTER
  {
    if (scGPButton_Ulozit->Enabled)
        // pokud jsou zároveò splnìny podmínky pro stisk OK
    {
      Form_definice_zakazek->ModalResult = mrOk;
      // vrátí stejnou hodnotu jako tlaèítko
      Form_definice_zakazek->VisibleChanging();
      // skryje form, stejné jako visible=false
    }
    else
      MessageBeep(0); // pípnutím upozorní, že nelze
  }
  if (Key == 27) // ESC
  {
    Form_definice_zakazek->ModalResult = mrCancel;
    // vrátí stejnou hodnotu jako tlaèítko
    Form_definice_zakazek->VisibleChanging();
    // skryje form, stejné jako visible=false
  }
}
// ---------------------------------------------------------------------------

void TForm_definice_zakazek::getmGridWidth() {
  F->log(__func__); // logování

  // mGrid->Columns[4].Width = 25;

}

void TForm_definice_zakazek::setGlyphButtonDefault(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z) {

  // ShowMessage("default glyphy");
  if (typ == krizek_davky) {
    // F->log(__func__); // logování
    // TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, Row);
    // H->GlyphOptions->Kind = scgpbgkClose;
    // H->GlyphOptions->Thickness = 1;
    // H->ShowCaption = true;
    // H->Caption = " Dávka";
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
    // H->Hint = "Smazat dávku";
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
		J->Hint = F->ls->Strings[438];//"Vytvoøit novou dávku"
    J->ShowHint = true;

    J = NULL;
    delete J;
  }

  if (typ == krizek) {
    F->log(__func__); // logování
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
		H->Hint = F->ls->Strings[439];//"Smazat zakázku"
		H->ShowHint = true;

    H = NULL;
    delete H;
  }
}

void TForm_definice_zakazek::OnClick(long Tag, long ID, unsigned long Col, unsigned long Row) {
  F->log(__func__); // logování
  // naètení aktuálnì editované zakázky
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku(ID);

  if (Col >= 3 && Row == 1 && volno == true)
  {
   TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, Row);
    // VÌTEV NA PØIDÁVÁNÍ DÁVEK DO ZAKÁZKY
    if (J->GlyphOptions->Kind == scgpbgkPlus) {
      volno = false;
      // backup pùvodního názvu zakázky pro následný merge bunek a naplnìní
      AnsiString nazev = Z->mGrid->Cells[1][0].Text;
      scGPButton_Ulozit->SetFocus();
      Z->mGrid->InsertColumn(Col, true, true);
      Z->mGrid->MergeCells(1, 0, 2, 0); // název     - vodorovne
      // naplnìní pùvodním textem
      Z->mGrid->Cells[1][0].Text = nazev;
      // pokud neexistují dávky tak pøi prvním pøidání dávky nastavím typ na glyphbutton
      if (Z->mGrid->ColCount == 4 + 1) Z->mGrid->Cells[Col][1].Type = Z->mGrid->glyphBUTTON;
      Z->mGrid->Update();
      setGlyphButtonDavka_Add(ID, Col + 1); // vytvoøí glyph na pøidání dávky
      setGlyphButtonDavka_Remove(ID, Col,Z); // vytvoøí glyph na smazání dávky
      volno = true;
					F->d.v.vloz_davku(Z,0,0,0);
    }
    // VÌTEV NA MAZÁNÍ DÁVEK ZE ZAKÁZKY
    if (J->GlyphOptions->Kind == scgpbgkClose && volno == true)
    {
      volno = false;
      scGPButton_Ulozit->SetFocus();
      Z->mGrid->DeleteColumn(Col,true);
      Z->mGrid->Update();
     // setGlyphButtonDavka_Add(Row, Z->mGrid->ColCount - 1);
      // vytvoøí glyph na pøidání dávky
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
  // odstranìnní tabulky a zakázky
  if (Col == 0 && Row == 0) {
    Konec->SetFocus();
    Z->mGrid->Delete();
    Z->mGrid = NULL;
    F->d.v.smaz_temp_zakazku(Z->n);
    Invalidate(); // formpaint nestaèí nepøekreslí pozùstatky tabulky
    // FormPaint(this);
  }
  // odmazání pomocného ukazatele
  Z = NULL;
  delete Z;
}

void TForm_definice_zakazek::setButtonColor(long ID)
{
  F->log(__func__); // logování
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
		F->log(__func__); // logování
    // ShowMessage("loadHeader");
		Cvektory::TZakazka *Z=NULL;
		if(novy)//vytváøení nové zákázky a jejícho mGridu
		{
	  	Cvektory::TJig J;
	  	unsigned int n=1;
	  	if(F->d.v.ZAKAZKY_temp!=NULL && F->d.v.ZAKAZKY_temp->predchozi->n>0)n=F->d.v.ZAKAZKY_temp->predchozi->n+1;
			F->d.v.vloz_temp_zakazku("id",0,"Zakazka "+AnsiString(n),clBlack,0,0,J,0,0,0);
			Z=F->d.v.ZAKAZKY_temp->predchozi;
			F->d.v.inicializace_cesty(Z);//vytvoøí hlavièku cesty
		}
		else Z=F->d.v.vrat_temp_zakazku(zakazka_n);//vytváøení mGridu stavající zakázce
		////vytvoøení mgridu nové zakázce
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
    // nazev  text  - slouèit podélnì
    Z->mGrid->Cells[0][0].Type = Z->mGrid->glyphBUTTON; // X  zakazka
    // Z->mGrid->Cells[2][i+1].Type = 	Z->mGrid->glyphBUTTON; // add  davka

    Z->mGrid->Cells[3][1].Type = Z->mGrid->glyphBUTTON; //

    Z->mGrid->Cells[0][1].Text = Z->n; // id zakázky
			if(novy)Z->mGrid->Cells[1][i+0].Text = F->ls->Strings[434];//"Název zakázky"
			else Z->mGrid->Cells[1][i+0].Text = Z->name;
    Z->mGrid->Cells[1][0].Text = "Název zakázky";
    Z->mGrid->Cells[1][1].Type = Z->mGrid->glyphBUTTON; // color zakazka
    //Z->mGrid->Cells[2][1].Type = Z->mGrid->glyphBUTTON;
    Z->mGrid->Cells[1][0].Font->Size = 15;

    Z->mGrid->Cells[1][1].Font->Size = 14;
			Z->mGrid->Cells[1][i+2].Text = " "+F->ls->Strings[435];//" poèet"   	//Z->mGrid->Cells[1][2].RightBorder->Color = clWhite;
			Z->mGrid->Cells[1][i+3].Text = " "+F->ls->Strings[436];//" prázdných"   //	Z->mGrid->Cells[1][3].RightBorder->Color = clWhite;
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
    // podbarvení sloupcù s "+"
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

    Z->mGrid->MergeCells(1, 0, 2, 0); // název     - vodorovne
  //  Z->mGrid->MergeCells(1, i+1, 2, i+1); // merge color glyph
    Z->mGrid->MergeCells(0, 1, 0, 4); // merge ID èi obrázek
    Z->mGrid->Update();
    // default rozmístìní glyphbuttonù
    setGlyphButtonDefault(0, 0, krizek, Z);
    setGlyphButtonDefault(1, 2, krizek_davky, Z);
    setGlyphButtonColor(1, 1, color, Z);

    ////ukazatelové záležitosti
    Z->mGrid->Refresh();

    Z = NULL;
    delete Z;
    ////vykreslení mGridù
	if(novy)FormPaint(this);//pouze v pøípadì, že vytváøím novou zakázku s mGridem, pøi naèítání již vytvoøených zakázek musé dojít k vykreslìní až budou všehny naèteny, volaní vykreslení v metodì nacti_zakazky()
  }

}

void __fastcall TForm_definice_zakazek::FormClose(TObject *Sender,
    TCloseAction &Action)

{
  F->log(__func__); // logování
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
		J->Hint = F->ls->Strings[438];//"Vytvoøit novou dávku"
  J->ShowHint = true;
  J = NULL;
  delete J;
}

void TForm_definice_zakazek::setGlyphButtonDavka_Remove(unsigned long ID, unsigned long Col, Cvektory::TZakazka *Z)
{
  F->log(__func__); // logování
  TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, 1);
  H->GlyphOptions->Kind = scgpbgkClose;
  H->GlyphOptions->Thickness = 1;
  H->ShowCaption = true;
  if (Z->mGrid->ColCount == 5) pocet_davek = 1;
		H->Caption = " "+F->ls->Strings[440]+" "+AnsiString(pocet_davek);// " Dávka "
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
		H->Hint = F->ls->Strings[441];//"Smazat dávku";
  H->ShowHint = true;
  H = NULL;
  delete H;
}

void TForm_definice_zakazek::setGlyphButtonColor(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z)
{
  F->log(__func__); // logování
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
  H->Hint = "Barva zakázky";
  H->ShowHint = true;
  H = NULL;
  delete H;
}
