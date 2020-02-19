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
  def_gray = (TColor)RGB(200,200,200);

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
  volno=true;
  ////////definice tabulky////////
  mGrid = new TmGrid(this); // vždy nutno jako první
  mGrid->Create(4, 6);
  // vypis(""); // prozmanuti vypisu - pro sicher
  mGrid->Tag = 9;
  // ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
  mGrid->ID = 0;
  mGrid->AntiAliasing_text = true;
  mGrid->Border.Width = 1;
  mGrid->DefaultRowHeight = 28;
  // vìtší výška øádku, kvùli velikosti comba - aby se vešlo celé

  ////nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
  nacti_PP();

  ////naètení zakázek a cest
  if (Form1->d.v.ZAKAZKY->dalsi == NULL) // kdyz je spojak prazdny
  {
    // vytvoøí defaultní øadek se zakázkou a její cestou
    predvypln_default_zakazku();
    // predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
    // a hned se uloží do temp spojáku
    uloz_Defaulttemp_zakazku();
    uloz_Default_cestu();
    Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
    // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
    // po uložení do hl.spojaku zakázek potøebuji stejnì znovu uložit øádek do temp_zakazek
    uloz_Defaulttemp_zakazku();
    uloz_Default_cestu();
  }
  else // pokud je uloženo nìco v zakázkách tak je naètu
  {
    nacti_zakazky();
  }
}

// ---------------------------------------------------------------------------
// nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
void TForm_definice_zakazek::nacti_PP()
{
  F->log(__func__); // logování
}

// ---------------------------------------------------------------------------
// vytvoøí defaultní øadek se zakázkou
void TForm_definice_zakazek::predvypln_default_zakazku()
{
  F->log(__func__); // logování
  loadHeader(0, true);
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
void TForm_definice_zakazek::uloz_Defaulttemp_zakazku()
{
  F->log(__func__); // logování
  Cvektory::TJig j;
  // defaultní hodnoty jigu
  j.sirka = Form1->d.v.PP.sirka_jig;
  j.delka = Form1->d.v.PP.delka_jig;
  j.vyska = Form1->d.v.PP.vyska_jig;
  j.ks = 1;

 // mGrid->Refresh();
  // ROSTA TODO PØEINDEXOVAT
//  Form1->d.v.vloz_temp_zakazku(UnicodeString(mGrid->Cells[1][0].Text), 0,
//      UnicodeString(0), clRed, Form1->ms.MyToDouble(100),
//      Form1->d.v.PP.TT, j, 0,
//      0, 0);
}

// ----------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Default_cestu() {
  F->log(__func__); // logování
//  Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
//  Cvektory::TZakazka *default_zakazka = Form1->d.v.vrat_temp_zakazku(1);
//  Form1->d.v.inicializace_cesty(default_zakazka);
//  while (objekt != NULL)
//  { // vložení defaulní cesty                        Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]                                        //v tomto pøípadì zámìrnì takto protože se pøebírá vypoètená hodnota
//    Form1->d.v.vloz_segment_cesty(default_zakazka,
//        /* sloupec poøadí se neukládá */ objekt->n, objekt->CT, 0, 0,
//        objekt->RD, 0);
//    objekt = objekt->dalsi;
//  }
}

// ---------------------------------------------------------------------------
// naètení hotových zakázek, tato metoda je volána z FormShow(), pokud existují zakázky
void TForm_definice_zakazek::nacti_zakazky()
{
  ShowMessage("nacti_zakazky");
  F->log(__func__); // logování
  ////vyplnìní stringgridu
  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
  // ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
  Cvektory::TZakazka *ukaz2 = Form1->d.v.ZAKAZKY->predchozi;
  mGrid->RowCount = ukaz2->n * 2;
  // pøipravím si poèet øádkù tabulky podle poètu zakázek *2 (slouèené 2 øádky)
  int i = 0;
  // Memo4->Lines->Add(AnsiString(ukaz->id));

  while (ukaz != NULL) {
    nacitam_zakazky = true;
    barva = (TColor)ukaz->barva;
    loadHeader(i, false);
    i++; // zvýšení indexu øádku

    mGrid->Refresh(); // musí být
    TscGPComboBox *C = mGrid->getCombo(1, i);
    TscGPListBoxItem *I;
    mGrid->Cells[1][i - 1].Text = ukaz->name;
    I = C->Items->Add();
    I->Caption = "Servisní";
    I = C->Items->Add();
    I->Caption = "Bìžná";
    if (ukaz->typ == 0)
      C->ItemIndex = 0;
    else
      C->ItemIndex = 1;

    mGrid->getButton(2, i)->Options->NormalColor = (TColor)ukaz->barva;
    mGrid->Cells[4][i].Text = ukaz->pocet_voziku;
    mGrid->Cells[6][i].Text = ukaz->serv_vozik_pocet;
    mGrid->Cells[9][i].Text = ukaz->opakov_servis;
    mGrid->Cells[10][i].Text = ukaz->TT;
    mGrid->Cells[11][i].Text = ukaz->id;

    // vložím další øádek (resp. navýším jejich poèítadlo, poèítá se od 1)
    // rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky

    i++; // další zvýšení indexu øádku
    // posun na další prvek v seznamu
    ukaz = ukaz->dalsi;
  }
  nacitam_zakazky = false;
  // uložení ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejná data jako jsou zobrazena
  Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp(); // pouze ZAKAZKY_temp=ZAKAZKY
}

// ----------------------------------------------------------------------------
// volání sub formuláøù JIG a CESTA a pøípadné ukládání do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender) {
  // definice ukazatele na aktuálnì editovanou zakázku     //èíslo øádku kde kliku došlo

 ShowMessage("staryStringGrid");
  Cvektory::TZakazka *zakazka =
      Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
  AnsiString prochazet;

  ////////////jig form- byl klik na buòku v 5. sloupci
  if (rStringGridEd1->Col == 5) {

    Form1->MB
        ("V této verzi programu není možné nastavovat parametry jigu. Jig je možné nastavit pouze v parametrech linky."
        );

    // DOCASNE ODSTAVENI NASTAVENI JIGU PRO KONKRETNI ZAKAZKU
    // naplnìní dat
    // Form_jig->Edit_jig_pocet_ks->Text=zakazka->jig.ks;
    // Form_jig->Edit_jig_delka->Text=zakazka->jig.delka;
    // Form_jig->Edit_jig_sirka->Text=zakazka->jig.sirka;
    // Form_jig->Edit_jig_vyska->Text=zakazka->jig.vyska;
    // //zobrazení formu + uložení dat pokud je zvoleno OK
    // if(mrOk==Form_jig->ShowModal())
    // {
    // //pri zmene delky
    // if(Form1->ms.MyToDouble(Form_jig->Edit_jig_delka->Text) != 	zakazka->jig.delka){
    // Changes=true;
    //
    // }
    // //pri zmene sirky
    // if(Form1->ms.MyToDouble(Form_jig->Edit_jig_sirka->Text) != zakazka->jig.sirka){
    // Changes=true;
    // }
    //
    // if(Changes){
    //
    // if(mrOk==Form1->MB("Nastala zmìna parametrù délka nebo šíøka jigu, budou pøepoèítány parametry linky pro tuto zakázku.",MB_OKCANCEL)) {
    // //aktualizacni fce a ulozeni do zakazky
    // zakazka->jig.ks=Form1->ms.MyToDouble(Form_jig->Edit_jig_pocet_ks->Text);
    // zakazka->jig.delka=Form1->ms.MyToDouble(Form_jig->Edit_jig_delka->Text);
    // zakazka->jig.sirka=Form1->ms.MyToDouble(Form_jig->Edit_jig_sirka->Text);
    // zakazka->jig.vyska=Form1->ms.MyToDouble(Form_jig->Edit_jig_vyska->Text);
    // }
    //
    // }
    // if(!Changes){   //pokud nejsou zadne zmeny na jigu  ulozim hodnoty z editboxu
    //
    // zakazka->jig.ks=Form1->ms.MyToDouble(Form_jig->Edit_jig_pocet_ks->Text);
    // zakazka->jig.delka=Form1->ms.MyToDouble(Form_jig->Edit_jig_delka->Text);
    // zakazka->jig.sirka=Form1->ms.MyToDouble(Form_jig->Edit_jig_sirka->Text);
    // zakazka->jig.vyska=Form1->ms.MyToDouble(Form_jig->Edit_jig_vyska->Text);
    // }

  }

  ////////////cesty form - byl klik na buòku 9. sloupci
  if (rStringGridEd1->Col == 9) {
    // Form_cesty->rStringGridEd_cesty->Columns->Clear();
    ////naplnìní picklistu
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");

    // AKTUALIZACE SEGMENTÙ DLE PARAMETRÙ OBJEKTU U PRVNÍ ZAKÁZKY,v pøípadì první zakázky se berou hodnoty z parametrù objektu nikoliv zakázky, což zajistí patøiènou aktuliazaci
    // to samé je voláno v unit1.cpp v Nastavitparametry1Click z urèitého pohledu se jedná o duplicitní algoritmus, ale v pøípadì aktualizací jinak než pøes parametry objektu lze považovat za nutnost
    if (zakazka->n == 1) // pouze pokud se jedná o první zakázku
    {
      Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
      while (O != NULL)
          // prochází všechnyobjekty a buï je ("Ano") objekt i na cestì nebo není ("Ne")
      {
        Cvektory::TCesta *C = Form1->d.v.obsahuje_segment_cesty_objekt(O,
            zakazka);
        if (C != NULL) // objekt je segmentem cesty
        {
          C->CT = O->CT;
          C->RD = O->RD;
          // v pøípadì první zakázky se berou hodnoty z parametrù objektu nikoliv zakázky, což zajistí patøiènou aktuliazaci
        }
        O = O->dalsi;
      }
    }

    // AKTAUALIZACE CT,RD DLE TT dle TT temp zakazky je rozdilny oproti TT ve stringgridu - budu prepocitavat hodnoty RD,CT na Ceste
    // if(Form1->ms.MyToDouble(zakazka->TT)!=Form1->ms.MyToDouble(rStringGridEd1->Cells[10][rStringGridEd1->Row]))  {
    // ShowMessage("prepocitej");
    Form1->d.v.aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(zakazka,
        Form1->ms.MyToDouble(rStringGridEd1->Cells[10][rStringGridEd1->Row]));
    // }

    ////naèítání dat - není tøeba ošetøovat, protože existuje default cesta v seznamu
    // vypíše døíve nadefinovanou cestu vèetnì segmentù z nejdelší možné cesty, které nejsou souèástí aktuální cesty
    Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
    // pøídání poètu øádkù
    Form_cesty->rStringGridEd_cesty->RowCount =
        Form1->d.v.OBJEKTY->predchozi->n + 1;
    // pøidání dalšího øádku     //pridani k zobrazeni if ukaz param
    int i = 0;
    while (O != NULL)
        // prochází všechnyobjekty a buï je ("Ano") objekt i na cestì nebo není ("Ne")
    {
      i++;
      Cvektory::TCesta *C = Form1->d.v.obsahuje_segment_cesty_objekt(O,
          zakazka);
      if (C != NULL) // zahrnuté segmenty cesty
      {
        Form_cesty->rStringGridEd_cesty->Cells[0][i] = C->objekt->n;
        Form_cesty->rStringGridEd_cesty->Cells[1][i] = C->objekt->name;
        Form_cesty->rStringGridEd_cesty->Cells[2][i] = C->CT;
        Form_cesty->rStringGridEd_cesty->Cells[3][i] = C->RD * 60;
        // zobrazím vždy v [m/min]
        Form_cesty->rStringGridEd_cesty->Cells[4][i] = C->Tv;
        Form_cesty->rStringGridEd_cesty->Cells[5][i] = C->Tc;
        Form_cesty->rStringGridEd_cesty->Cells[6][i] = C->Opak;
        Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ano";
      }
      else // nezahrnutý segment nejdelší cesty v aktuální cestì
      {
        Form_cesty->rStringGridEd_cesty->Cells[0][i] = O->n;
        Form_cesty->rStringGridEd_cesty->Cells[1][i] = O->name;
        Form_cesty->rStringGridEd_cesty->Cells[2][i] = "0";
        Form_cesty->rStringGridEd_cesty->Cells[3][i] = "0";
        Form_cesty->rStringGridEd_cesty->Cells[4][i] = "0";
        Form_cesty->rStringGridEd_cesty->Cells[5][i] = "0";
        Form_cesty->rStringGridEd_cesty->Cells[6][i] = "0";
        Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ne";
      }
      O = O->dalsi;
    }

    ////--------------

    ////ukládání dat - jednotlivého segmentu cesty pokud je považován k zahrnutní ze strany uživatele
    // formuláø na støed
    Form_cesty->Left = Form1->ClientWidth / 2 - Form_cesty->Width / 2;
    Form_cesty->Top = Form1->ClientHeight / 2 - Form_cesty->Height / 2;

    double CT;
    double Tv;
    double Tc;

    if (mrOk == Form_cesty->ShowModal()) {
      Form1->d.v.inicializace_cesty(zakazka);
      for (int i = 1; i < Form_cesty->rStringGridEd_cesty->RowCount; i++) {
        if (Form_cesty->rStringGridEd_cesty->Cells[7][i] == "Ano")
            // pokud je zaškrnuto neprocházek objekt se neuloží do cesty
        {
          if (Form_cesty->rStringGridEd_cesty->Columns->Items[2]
              ->TitleCaption == "CT [s]") { // pokud je v sekundach
            CT = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[2][i]);
            Tv = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[4][i]);
            Tc = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[5][i]);
          }
          else { // pøevedu minuty na sekundy
            CT = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[2][i]) * 60.0;
            // min na sekundy
            Tv = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[4][i]) * 60.0;
            Tc = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[5][i]) * 60.0;
          }

          Form1->d.v.vloz_segment_cesty(zakazka,
              /* sloupec poøadí se neukládá */ /* pozor na øazení */ Form_cesty
              ->rStringGridEd_cesty->Cells[0][i].ToInt(), // ID-doøešit
              Form1->ms.MyToDouble(CT), // CT
              Form1->ms.MyToDouble(Tv), // Tv
              Form1->ms.MyToDouble(Tc), // Tc
              Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i])
              / 60.0, // RD ulozim v m/sec
              Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])
              // Opak   //ulozeni stavu pro cestu - roletka
              );
        }
        // vymazání textu z již nepotøebného øádku
        Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();

      }
    }
  }

}

// ---------------------------------------------------------------------------
// TLAÈÍTKO ULOŽIT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
  ShowMessage("ulozit");
  F->log(__func__); // logování
  zmena_TT = false;
  bool neukladat = false; // pokud nebudou splnìny podmínky, nelze form uložit
  bool dal_ok = false;

  // for (int i=1; i < rStringGridEd1->RowCount; i++) {   //prochazim vsechny radky a hledam, kde je zmena TT
  //
  // Cvektory::TZakazka *zakazka=Form1->d.v.vrat_temp_zakazku(i);
  //
  // if(Form1->ms.MyToDouble(zakazka->TT)!=Form1->ms.MyToDouble(rStringGridEd1->Cells[10][i]))  {
  //
  // zmena_TT=true;
  // }
  // }

  //
  // if(zmena_TT) {
  //
  // if(mrOk==Form1->MB("Nastala zmìna TakTime, která ovlivní technologický èas a rychlost pohonu upravované zakázky.",MB_OKCANCEL)) {
  // dal_ok=true;
  // //aktualiz fce pro CT,RD zakazky
  // //Form1->d.v.aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky();
  // neukladat=false;
  // }
  // else {
  //
  // neukladat=true;    //pokud neulozim data, ulozim do dat puvodni hodnoty TT z temp_zakazek
  // for (int i=1; i < rStringGridEd1->RowCount; i++) {
  // rStringGridEd1->Cells[10][i]=Form1->d.v.vrat_temp_zakazku(i)->TT;
  // }
  // }
  // }

  TColor barva; // musí být v TColor kvùli ukládání do TColor objektu!!!

  // uložení editovaných zakázek
  for (unsigned int i = 1; i < mGrid->RowCount;
  i++) // dole ještì zvyšuji poèítadlo!!!
  {

    double pomer;
    double TT;
    double nvoziku;
    double nservis_voziku;
    double nopak_voziku;
    UnicodeString nazev;

    // ošetøení - pokud se pøi ukládání formu objeví prázdné hodnoty, automaticky vyplním "0" a pøidám název zakázky

    // NEW
    if (mGrid->Cells[1][i - 1].Text.IsEmpty())
      nazev = "Nová zakázka";
    else
      nazev = mGrid->Cells[1][i - 1].Text;

    pomer = 100;

    if (mGrid->Cells[10][i].Text.IsEmpty())
      TT = 0;
    else
      TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);

    if (mGrid->Cells[4][i].Text.IsEmpty())
      nvoziku = 0;
    else
      nvoziku = mGrid->Cells[4][i].Text.ToInt();

    if (mGrid->Cells[6][i].Text.IsEmpty())
      nservis_voziku = 0;
    else
      nservis_voziku = mGrid->Cells[6][i].Text.ToInt();

    if (mGrid->Cells[9][i].Text.IsEmpty())
      nopak_voziku = 0;
    else
      nopak_voziku = mGrid->Cells[9][i].Text.ToInt();

    // ShowMessage(nazev);
    // ShowMessage(mGrid->Cells[11][i].Text.ToInt());

    bool usersColor = true;

    mGrid->Refresh(); // musí být
    TscGPComboBox *C = mGrid->getCombo(1, i);
    if (usersColor)
      barva = (TColor)mGrid->getButton(2, i)->Options->NormalColor;
    // musí být až tady, jinak spadne
    // ShowMessage(C->ItemIndex);
    // uložení aktuálních hodnot do dané temp_zakazky

    if (i == 1) // ulož TT do projektu pro status návrháø, pouze z první zakázky
          Form1->d.v.PP.TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);

    Form1->d.v.edituj_temp_zakazku(mGrid->Cells[11][i].Text.ToInt(), // n
        mGrid->Cells[11][i].Text.ToInt(), // ID
        C->ItemIndex, // TYP
        nazev, // NAME
        barva, // COLOR
        pomer, // POMER
        TT, // TT
        // JIG se už jako parametr nepožaduje, stejnì jako cesta, jedná se o pøedávání ukazatelem pøi zavírání patøièného formuláøe
        nvoziku, // n-vozíku
        nservis_voziku, // n-servis vozíkù
        nopak_voziku // n-opak vozíkù
        );
    i++; // naèítám ob jeden øádek!!!
  }
  // samotné uložení
  Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
  // uložení do ostrého spojáku ZAKAZKY+smáznutí ZAKAZKY_temp
  Form1->d.v.generuj_VOZIKY(); // vygenerování vozíkù dle zadaných zakázek
  // konstrukce která má vzít CT a RD objektu z první zakázky a našit ho do CT a RD objektù
  Cvektory::TObjekt *Obj = Form1->d.v.OBJEKTY->dalsi;
  while (Obj != NULL) {
    Cvektory::TCesta *Segment_cesty = Form1->d.v.obsahuje_segment_cesty_objekt
        (Obj, Form1->d.v.ZAKAZKY->dalsi);
    if (Segment_cesty != NULL) {
      Obj->CT = Segment_cesty->CT; // ShowMessage(Obj->CT);
      Obj->RD = Segment_cesty->RD;
    }
    Obj = Obj->dalsi;
  }

  if (dal_ok) {
    // aktualiz fce pro CT,RD zakazky
    Form1->d.v.aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky();

  }

  // zavøení formuláøe s následným DuvodUlozit(true) po modalshow v unit1
  if (neukladat == false)
    Form_definice_zakazek->Close(); // kdyby nìco tak nic
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
	//mazání mgridù zakázek
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

	Form1->d.v.vymaz_seznam_ZAKAZKY_temp();
  Close();
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button1Click(TObject *Sender) {

  ShowMessage(mGrid->Cells[4][0].Type);
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button2Click(TObject *Sender) {
  // RzStringGrid1->ColCount--;
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::RzStringGrid1RowMoved(TObject *Sender,
    int FromIndex, int ToIndex) {
  // TColor ColorFromIndex, ColorToIndex;
  // TBarva *ukaz = BARVY->dalsi;
  //
  // while (ukaz != NULL) {
  // if (ukaz->id_radek == FromIndex)
  // ColorFromIndex = ukaz->barva_voziku;
  // if (ukaz->id_radek == ToIndex)
  // ColorToIndex = ukaz->barva_voziku;
  // ukaz = ukaz->dalsi; // posun na další prvek v seznamu
  // }
  //
  // TBarva *ukaz1 = BARVY->dalsi;
  // while (ukaz1 != NULL) {
  // if (ukaz1->id_radek == FromIndex)
  // ukaz1->barva_voziku = ColorToIndex;
  // if (ukaz1->id_radek == ToIndex)
  // ukaz1->barva_voziku = ColorFromIndex;
  // ukaz1 = ukaz1->dalsi; // posun na další prvek v seznamu
  // }
  //
  // RowMoved = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Memo_spojakClick(TObject *Sender) {
  //
  // TBarva *ukaz = BARVY->dalsi;
  // while (ukaz != NULL) {
  // Memo1->Lines->Add(AnsiString("id_radek: ") + ukaz->id_radek +
  // AnsiString(" barva: ") + ukaz->barva_voziku);
  //
  // ukaz = ukaz->dalsi;
  // }
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button_DELClick(TObject *Sender) {

  //
  // // Tlaèítko na smazání øádku
  // RzStringGrid1->Rows[RzStringGrid1->Row]->Clear();
  // rStringGridEd1->Rows[rStringGridEd1->Row]->Clear();
  //
  // TForm_definice_zakazek::TBarva *ukaz = BARVY->dalsi;
  // // ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
  // while (ukaz != NULL) {
  //
  // if (ukaz->id_radek == RzStringGrid1->Row) {
  // smaz_barvu_radku(ukaz);
  // sniz_indexy(ukaz);
  //
  // // ukaz=NULL;
  // // delete ukaz;  //smazani ukazatele nejak nefungovalo
  // break;
  //
  // }
  // // posun na další prvek v seznamu
  // ukaz = ukaz->dalsi;
  // }
  //
  // // posun øádkù
  //
  // for (int row = RzStringGrid1->Row; row < RzStringGrid1->RowCount - 1; ++row)
  //
  // {
  // RzStringGrid1->Rows[row] = RzStringGrid1->Rows[row + 1];
  // //  Memo1->Lines->Add(row);
  // //  Memo1->Lines->Add(RzStringGrid1->Row+1);
  // // aktualizuj_barvu(ukaz->barva_voziku,RzStringGrid1->Row+1);
  //
  // }
  //
  // RzStringGrid1->RowCount--;
  // // vykrleseni noveho poradi (n)
  // for (long i = 1; i < RzStringGrid1->RowCount; i++)
  // RzStringGrid1->Cells[0][i] = i;
  //
}
// ---------------------------------------------------------------------------


void __fastcall TForm_definice_zakazek::Button_OKKeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift) {
  // UINT funkcni_klavesa=0;
  // switch(Key)
  // {
  // //BACKSPACE
  // case 8: break;
  // //ENTER
  // case 13:break;
  // //ESC
  // case 27:Close();break;
  // //MEZERNÍK
  // case 32:break;
  // //PAGE UP
  // case 33:;break;
  // //PAGE DOWN
  // case 34:break;
  // //END
  // case 35:break;
  // //HOME
  // case 36:break;
  // //ŠIPKA DOLÙ
  // case 40:break;
  // //ŠIPKA LEVÁ
  // case 37:break;
  // //ŠIPKA PRAVÁ
  // case 39:break;
  // //ŠIPKA NAHORU
  // case 38:break;
  // //ŠIPKA DOLÙ
  // case 98:break;
  // //ŠIPKA LEVÁ
  // case 100:break;
  // //ŠIPKA PRAVÁ
  // case 102:break;
  // //ŠIPKA NAHORU
  // case 104:break;
  // //CTRL, SHIFT
  // default:
  // {
  // if(Shift.Contains(ssShift) && Shift.Contains(ssCtrl)){funkcni_klavesa=3;}//SHIFT + CTRL
  // else
  // {
  // if(Shift.Contains(ssShift)){funkcni_klavesa=2;}//SHIFT
  // if(Shift.Contains(ssCtrl)){funkcni_klavesa=1;}//CTRL
  // }
  // break;
  // }

  // ShowMessage("uloz");
  // }
}

// ---------------------------------------------------------------------------
// zkopíruje pøedchozí
void __fastcall TForm_definice_zakazek::scGPGlyphButton_add_zakazkaClick
    (TObject *Sender) {

//  mGrid->RowCount = mGrid->RowCount + 2;
//
//  int i = mGrid->RowCount - 1;
//  Cvektory::TJig j;
//  j.sirka = 1;
//  j.delka = 1;
//  j.vyska = 1;
//  j.ks = 1;
//
//  if (i > 1)
//  { // defaultní zakázka je uložena hned v temp_spojaku + hlavnim pri form_show, èili tady ukládám až další øádky
//
//    Cvektory::TZakazka *ukaz = F->d.v.ZAKAZKY_temp->predchozi;
//
//    add_zakazka = true;
//    loadHeader(i, false);
//    mGrid->Cells[1][i - 1].Text = "Nová zakázka";
//    mGrid->Cells[4][i].Text = "100";
//    mGrid->Cells[6][i].Text = "0";
//    mGrid->Cells[9][i].Text = "0";
//    mGrid->Cells[10][i].Text = "120";
//    mGrid->Cells[11][i].Text = ukaz->n + 1;
//
//    mGrid->Refresh();
//    // kvùli práci s combem je nutný refresh po nastavení na typ COMBOEDIT
//    TscGPComboBox *C = mGrid->getCombo(1, i);
//    TscGPListBoxItem *I;
//    // naètení hodnoty rozteèe do roletky + nastavení jako ItemIndex=0
//    I = C->Items->Add();
//    I->Caption = "Servisní";
//    C->ItemIndex = 0;
//    I = C->Items->Add();
//    I->Caption = "Bìžná";
//    C->ItemIndex = 1;
//
//    Form1->d.v.vloz_temp_zakazku(UnicodeString(mGrid->Cells[11][i].Text),
//        C->ItemIndex, UnicodeString(mGrid->Cells[1][i - 1].Text), clRed,
//        // Form_color_dialog->scColorGrid1->ColorValue,
//        Form1->ms.MyToDouble(100),
//        Form1->ms.MyToDouble(mGrid->Cells[10][i].Text), j,
//        mGrid->Cells[4][i].Text.ToInt(), mGrid->Cells[6][i].Text.ToInt(),
//        mGrid->Cells[9][i].Text.ToInt());
//
//    // int j=rStringGridEd1->RowCount-1;
//    int j = mGrid->Cells[11][i].Text.ToInt();
//
//    // pøi pøidání další zakázky uložím do cesty defaultní hodnoty
//    Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
//    Cvektory::TZakazka *nova_zakazka = Form1->d.v.vrat_temp_zakazku(j);
//    Form1->d.v.inicializace_cesty(nova_zakazka);
//    while (objekt != NULL) { // vložení defaulní cesty
//      Form1->d.v.vloz_segment_cesty(nova_zakazka,
//          /* sloupec poøadí se neukládá */ objekt->n, 0, 0, 0, 0, 0);
//      objekt = objekt->dalsi;
//
//    }
//
//  }
//  add_zakazka = false;
//ShowMessage("add_zakazka");
//	add_zakazka=true;
//	for(int i=0;i<=5;i++)
//	{
//	mGrid->AddRow(true,false); //vytvoøím si prázdné øádky
//	}
//	loadHeader(mGrid->RowCount-6,false);  //nadesignuji øádky(-6) zámìrnì, abych šel od prvního nového øádku
//
//	add_zakazka=false;
//	//mGrid->Refresh();
//	FormPaint(this);

	////vytvoøení nové temp zakázky, nutné nese si ukazatel na svùj mGrid
	Cvektory::TZakazka *Z=NULL; Cvektory::TJig J;
	unsigned int n=1;
	if(F->d.v.ZAKAZKY_temp!=NULL && F->d.v.ZAKAZKY_temp->predchozi->n>0)n=F->d.v.ZAKAZKY_temp->predchozi->n+1;
	F->d.v.vloz_temp_zakazku("id",0,"Zakazka "+AnsiString(n),clBlack,0,0,J,0,0,0);
	Z=F->d.v.ZAKAZKY_temp->predchozi;
	////vytvoøení mgridu nové zakázce
	Z->mGrid=new TmGrid(this);
	Z->mGrid->Tag=9;
	Z->mGrid->ID=Z->n;
	Z->mGrid->Create(3,3);
	////parametry mgridu
	Z->mGrid->Left=5;
	if(Z->predchozi->n>0)Z->mGrid->Top=Z->predchozi->mGrid->Top+Z->predchozi->mGrid->Height+Z->mGrid->Rows[0].Height;
	else Z->mGrid->Top=mGrid->Top+mGrid->Height+Z->mGrid->Rows[0].Height;

	////ukazatelové záležitosti
	Z=NULL;delete Z;
	////vykreslení mGridù
	FormPaint(this);
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::button_zakazky_tempClick
    (TObject *Sender) {
  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY_temp->dalsi;
  while (ukaz != NULL) {
    // Memo4->Lines->Add(AnsiString(ukaz->name)+";"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->opakov_servis)+";"+AnsiString(ukaz->TT));
    ukaz = ukaz->dalsi;
  }
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::zakazky_hlavni_spojakClick
    (TObject *Sender) {

  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
  while (ukaz != NULL) {

    // Memo4->Lines->Add(AnsiString(ukaz->name)+";"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->opakov_servis)+";"+AnsiString(ukaz->TT));

    ukaz = ukaz->dalsi;

  }
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::smaz_tempClick(TObject *Sender) {
  // Form1->d.v.smaz_temp_zakazku(Edit_smaz_temp->Text.ToInt());
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::scGPGlyphButton_removeClick
    (TObject *Sender)

{
  // mazani zakazek
  Cvektory::TZakazka *ukaz = F->d.v.ZAKAZKY_temp->predchozi;
  // mGrid->Rows[mGrid->RowCount]->Clear();
  mGrid->ClearRow(mGrid->RowCount);
  ShowMessage("1");

  if (mGrid->RowCount > 2) {
    Form1->d.v.smaz_temp_zakazku(ukaz->predchozi->n);
    mGrid->RowCount--;
    mGrid->RowCount--;
  }

}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::FormPaint(TObject *Sender) {
	mGrid->Show(); // vykreslí tabulku
	//vykreslování mGridù zakázek
	if(F->d.v.ZAKAZKY_temp!=NULL)
	{
  	Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
  	while(Z!=NULL)
  	{
  		Z->mGrid->Show();
  		Z=Z->dalsi;
  	}
  	delete Z;Z=NULL;
	}
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

void __fastcall TForm_definice_zakazek::rEditNum_pocet_prac_hodKeyDown
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

void __fastcall TForm_definice_zakazek::rEditNum_pozad_mnozstviKeyDown
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

void __fastcall TForm_definice_zakazek::rEditNum_effektivitaKeyDown
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

void __fastcall TForm_definice_zakazek::scEdit_zacatekKeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift) {
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

void __fastcall TForm_definice_zakazek::rStringGridEd1KeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift) {
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

void __fastcall TForm_definice_zakazek::FormKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift)

{
  F->log(__func__); // logování
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

void __fastcall TForm_definice_zakazek::FormMouseMove(TObject *Sender,
    TShiftState Shift, int X, int Y) {
  F->log(__func__); // logování
  if (mGrid->RowCount <= 2) {
    scGPGlyphButton_remove->Enabled = false;
  }
  else
    scGPGlyphButton_remove->Enabled = true;
}

// ---------------------------------------------------------------------------
void TForm_definice_zakazek::getmGridWidth() {
  F->log(__func__); // logování
  mGrid->Columns[0].Width = 30;
  mGrid->Columns[1].Width = 80;
  mGrid->Columns[2].Width = 55;
  mGrid->Columns[3].Width = 25;
 // mGrid->Columns[4].Width = 25;

}

void TForm_definice_zakazek::setGlyphButtonDefault (unsigned long Row,unsigned long Col, Typ_buttonu typ)
{
 ShowMessage("default glyphy");
  if (typ == krizek_davky)
  {
    F->log(__func__); // logování
    TscGPGlyphButton *H = mGrid->getGlyphButton(Col, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->ShowCaption = true;
    H->Caption = " Dávka";
    // H->Font->Color=(TColor)RGB(43,87,154);
    H->Options->NormalColor = clWhite;
    H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
    H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
    H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
    H->Options->NormalColorAlpha = 255;
    H->Options->FrameWidth = 1;
    H->Options->FrameNormalColor = clWhite;
    H->Width = 80;
    H->Height = mGrid->DefaultRowHeight;
    H->Options->ShapeStyle = scgpRect;
    // H->Width=30;
    H->Hint = "Smazat dávku";
    H->ShowHint = true;

    H = NULL;
    delete H;
//
    TscGPGlyphButton *J = mGrid->getGlyphButton(Col+1, Row);
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
    J->Width = 80;
    J->Height = mGrid->DefaultRowHeight;
    J->Options->ShapeStyle = scgpRect;
    // H->Width=30;
    J->Hint = "Vytvoøit novou dávku";
    J->ShowHint = true;

    J = NULL;
    delete J;
  }

  if (typ == krizek)
  {
    F->log(__func__); // logování
    TscGPGlyphButton *H = mGrid->getGlyphButton(0, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->Options->NormalColor = clWhite;
    H->Options->NormalColorAlpha = 255;
    H->Options->FrameWidth = 1;
    H->Options->FrameNormalColor = clWhite;
    H->Width = 30;
    H->Height = mGrid->DefaultRowHeight;
    H->Options->ShapeStyle = scgpRect;
    // H->Width=30;
    H->Hint = "Smazat zakázku";
    H->ShowHint = true;

    H = NULL;
    delete H;
  }
}

void TForm_definice_zakazek::OnClick(long Tag, long ID, unsigned long Col,unsigned long Row)
{
  F->log(__func__); // logování
    if (Col >= 3 && Row == 1 && volno == true)   //Row == 1 dynamicky
    {

       // mGrid->Update();
      // TscGPGlyphButton *J = mGrid->getGlyphButton(Col, Row);
        // VÌTEV NA PØIDÁVÁNÍ DÁVEK DO ZAKÁZKY
      // if(J->GlyphOptions->Kind==scgpbgkPlus)
      // {
          //Form_color_dialog->SetDialogPosition(Row);
          //Form_color_dialog->ShowModal();
          volno=false;
           //ShowMessage("ted");
          scGPButton_Ulozit->SetFocus();

           mGrid->Cells[Col][1].Text="X Dávka";


          //sloupec kde došlo ke kliku naplním daty + nastavim šíøku
          mGrid->Columns[Col].Width = 90;
          mGrid->Cells[Col][2].Type = mGrid->EDIT; //
          mGrid->Cells[Col][3].Type = mGrid->EDIT; //
          mGrid->Cells[Col][4].Type = mGrid->EDIT; //
          //pùvodní buòky, které sloužili pro ADD button nastavím zpìt na bílé + nastavim výchozí hodnoty
          mGrid->Cells[Col][4].Background->Color = clWhite;  mGrid->Cells[Col][4].Text="0";  mGrid->Cells[Col][4].RightBorder->Color = def_gray;   mGrid->Cells[Col][4].BottomBorder->Color = def_gray;
          mGrid->Cells[Col][3].Background->Color = clWhite;  mGrid->Cells[Col][3].Text="0";  mGrid->Cells[Col][3].RightBorder->Color = def_gray;   mGrid->Cells[Col][3].BottomBorder->Color = def_gray;
          mGrid->Cells[Col][2].Background->Color = clWhite;  mGrid->Cells[Col][2].Text="0";  mGrid->Cells[Col][2].RightBorder->Color = def_gray;   mGrid->Cells[Col][2].BottomBorder->Color = def_gray;
          mGrid->Cells[Col][0].Background->Color = clWhite;  mGrid->Cells[Col][0].Text=" ";  mGrid->Cells[Col][0].RightBorder->Color = def_gray;
        //  ShowMessage("pred");
         // mGrid->ColCount++;
        //  ShowMessage("po");
          //pøi volání colcount++ se rozhodí pozice glyph buttony dávek, musí se znovu projít a novì vytvoøit
//          for(int i=3;i<=mGrid->ColCount-1;i++) //od column 3 - ty jsou fixní
//          {  //podle poètu dávek for cyklus
//
//           setGlyphButtonDavka_Remove(Row,i);   //vytvoøí glyph na smazání dávky
//          }
           mGrid->AddColumn();
         // mGrid->Refresh();
           //setGlyphButtonDavka_Remove(Row,Col);   //vytvoøí glyph na smazání dávky
           //setGlyphButtonDavka_Add(Row,mGrid->ColCount);   //vytvoøí glyph na pøidání dávky
           mGrid->Update();
           mGrid->Cells[Col+1][1].Type =mGrid->EDIT;// mGrid->glyphBUTTON; // add  davka
           mGrid->Cells[Col+1][1].Text="+";
           mGrid->Columns[Col].Width = 90;
           mGrid->Columns[Col+1].Width = 25; //šíøka musí být nastavena až tady

           //novì vytvoøené buòky pro ADD button nastavím na šedou barvu
           mGrid->Cells[mGrid->ColCount-1][4].Background->Color = light_gray;  mGrid->Cells[mGrid->ColCount-1][4].Text=" ";  mGrid->Cells[mGrid->ColCount-1][4].RightBorder->Color = light_gray;   mGrid->Cells[mGrid->ColCount-1][4].BottomBorder->Color = light_gray;
           mGrid->Cells[mGrid->ColCount-1][3].Background->Color = light_gray;  mGrid->Cells[mGrid->ColCount-1][3].Text=" ";  mGrid->Cells[mGrid->ColCount-1][3].RightBorder->Color = light_gray;   mGrid->Cells[mGrid->ColCount-1][3].BottomBorder->Color = light_gray;
           mGrid->Cells[mGrid->ColCount-1][2].Background->Color = light_gray;  mGrid->Cells[mGrid->ColCount-1][2].Text=" ";  mGrid->Cells[mGrid->ColCount-1][2].RightBorder->Color = light_gray;   mGrid->Cells[mGrid->ColCount-1][2].BottomBorder->Color = light_gray;
           mGrid->Cells[mGrid->ColCount-1][0].Background->Color = light_gray;  mGrid->Cells[mGrid->ColCount-1][0].Text=" ";  mGrid->Cells[mGrid->ColCount-1][0].RightBorder->Color = light_gray;

         //  J = NULL;
         //  delete J;

         //mGrid->Refresh(); // refresh nestaci?
           Invalidate();
           volno=true;
    //  }
//      else if(J->GlyphOptions->Kind==scgpbgkClose && volno == true)  //VÌTEV NA MAZÁNÍ DÁVEK ZE ZAKÁZKY
//      {
//         ShowMessage("Kliknuto col:"+AnsiString(Col)); ShowMessage("ColCount"+AnsiString(mGrid->ColCount-1));
//        volno=false;
//        for (int i=Col; i < mGrid->ColCount-2; i++) // ColCount-2 protože mažu celý sloupec + o jednièku snižuji
//        {
////        ShowMessage("do Col"+AnsiString(i)+"radek 2 kopiruj"+AnsiString(mGrid->Cells[i+1][2].Text));
////        ShowMessage("do Col"+AnsiString(i)+"radek 3 kopiruj"+AnsiString(mGrid->Cells[i+1][3].Text));
////        ShowMessage("do Col"+AnsiString(i)+"radek 4 kopiruj"+AnsiString(mGrid->Cells[i+1][4].Text));
//         mGrid->Cells[i][2].Text=mGrid->Cells[i+1][2].Text;
//         mGrid->Cells[i][3].Text=mGrid->Cells[i+1][3].Text;
//         mGrid->Cells[i][4].Text=mGrid->Cells[i+1][4].Text;
//         mGrid->Refresh();
//        }
//         setGlyphButtonDavka_Add(Row,mGrid->ColCount-2);   //vytvoøí glyph na pøidání dávky
//         mGrid->Cells[mGrid->ColCount-2][2].Text=" ";
//         mGrid->Cells[mGrid->ColCount-2][3].Text=" ";
//         mGrid->Cells[mGrid->ColCount-2][4].Text=" ";
//         mGrid->ColCount--;
//
//        J = NULL;
//        delete J;
//        Invalidate();
//        volno=true;
//      }
    }
}

void TForm_definice_zakazek::setButtonColor(int Row) {
  F->log(__func__); // logování
  mGrid->getButton(2, Row)->Options->HotColor =
      Form_color_dialog->scColorGrid1->ColorValue;
  mGrid->getButton(2, Row)->Options->HotColorAlpha = 255;
  mGrid->getButton(2, Row)->Options->FocusedColor =
      Form_color_dialog->scColorGrid1->ColorValue;
  mGrid->getButton(2, Row)->Options->FocusedColorAlpha = 225;
  mGrid->getButton(2, Row)->Options->NormalColor =
      Form_color_dialog->scColorGrid1->ColorValue;
}

void TForm_definice_zakazek::loadHeader(unsigned long Row, bool novy) {

   ShowMessage("loadHeader");

  mGrid->SetColumnAutoFit(-4);
  if(novy)getmGridWidth();

  if (novy || add_zakazka)
  {
    F->log(__func__); // logování
    mGrid->Top = scLabel_header->Height + scGPButton_plan_vyroby->Height + 10;
		mGrid->Left = 5;
    int i=0;
    if(novy) i=0;
    if(add_zakazka) i=Row;

    mGrid->Cells[1][i+0].Type = mGrid->EDIT; // nazev  text  - slouèit podélnì
    mGrid->Cells[0][i+0].Type = mGrid->glyphBUTTON; // X  zakazka
    mGrid->Cells[3][i+1].Type = mGrid->EDIT;//mGrid->glyphBUTTON; // X  davka
 //   mGrid->Cells[4][i+1].Type = mGrid->EDIT;//mGrid->glyphBUTTON; // add  davka
    mGrid->Cells[3][i+1].Text="+";


    mGrid->Cells[0][i+1].Text = "1"; // id zakázky
    mGrid->Cells[1][i+0].Text = "Název zakázky";
    mGrid->Cells[1][i+0].Font->Size=15;

    mGrid->Cells[1][i+1].Font->Size=14;
    mGrid->Cells[1][i+2].Text = " poèet";   mGrid->Cells[1][2].RightBorder->Color = clWhite;
    mGrid->Cells[1][i+3].Text = " prázdných";   mGrid->Cells[1][3].RightBorder->Color = clWhite;
    mGrid->Cells[1][i+4].Text = " celkem";       mGrid->Cells[1][4].RightBorder->Color = clWhite;

    mGrid->Cells[1][i+2].Align = mGrid->LEFT;
    mGrid->Cells[1][i+3].Align = mGrid->LEFT;
		mGrid->Cells[1][i+4].Align = mGrid->LEFT;

    mGrid->Cells[2][i+2].Align = mGrid->CENTER;
    mGrid->Cells[2][i+3].Align = mGrid->CENTER;
    mGrid->Cells[2][i+4].Align = mGrid->CENTER;

    mGrid->Cells[2][i+2].Type = mGrid->EDIT; //
    mGrid->Cells[2][i+3].Type = mGrid->EDIT;
    mGrid->Cells[2][i+4].Type = mGrid->EDIT;
    mGrid->Cells[2][i+2].Text = "100"; // value
    mGrid->Cells[2][i+3].Text = "3"; // value
    mGrid->Cells[2][i+4].Text = "103"; // value


    mGrid->Cells[1][i+1].RightBorder->Color = clWhite;
    //mGrid->Cells[4][4].Background->Color = light_gray;
    mGrid->Cells[0][i+2].LeftBorder->Color = light_gray;

   //ØEŠIT DYNAMICKY
   mGrid->Cells[3][i+4].Background->Color = light_gray;  mGrid->Cells[3][i+4].Text=" ";  mGrid->Cells[3][i+4].RightBorder->Color = light_gray;   mGrid->Cells[3][i+4].BottomBorder->Color = light_gray;
   mGrid->Cells[3][i+3].Background->Color = light_gray;  mGrid->Cells[3][i+3].Text=" ";  mGrid->Cells[3][i+3].RightBorder->Color = light_gray;   mGrid->Cells[3][i+3].BottomBorder->Color = light_gray;
   mGrid->Cells[3][i+2].Background->Color = light_gray;  mGrid->Cells[3][i+2].Text=" ";  mGrid->Cells[3][i+2].RightBorder->Color = light_gray;   mGrid->Cells[3][i+2].BottomBorder->Color = light_gray;
 //  mGrid->Cells[3][i+0].Background->Color = light_gray;  mGrid->Cells[3][i+0].Text=" ";  mGrid->Cells[3][i+0].RightBorder->Color = light_gray;

    mGrid->MergeCells(1, i+0, 3, i+0); // název     - vodorovne
    mGrid->MergeCells(0, i+1, 0, i+4); // merge ID èi obrázek
   // if(add_zakazka)
    // mGrid->Refresh();
    //default rozmístìní glyphbuttonù
  //setGlyphButtonDefault(i+0,0, krizek); // pøedávat Row
  //  setGlyphButtonDefault(i+1,3, krizek_davky); // pøedávat Row
  }

}

void __fastcall TForm_definice_zakazek::FormClose(TObject *Sender,
    TCloseAction &Action)

{
  F->log(__func__); // logování
  mGrid->Delete();
}
// ---------------------------------------------------------------------------

void TForm_definice_zakazek::setGlyphButtonDavka_Add(unsigned long Row,unsigned long Col)    //pøedávám Col+1!!
{
    mGrid->Columns[Col].Width = 25; //nastavení šíøky pro ADD dávka button
    TscGPGlyphButton *J = mGrid->getGlyphButton(Col, Row);
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
    J->Width = 25;
    J->Height = mGrid->DefaultRowHeight;
    J->Options->ShapeStyle = scgpRect;
    // H->Width=30;
    J->Hint = "Vytvoøit novou dávku";
    J->ShowHint = true;

    J = NULL;
    delete J;
}
void TForm_definice_zakazek::setGlyphButtonDavka_Remove(unsigned long Row,unsigned long Col)
{
    F->log(__func__); // logování
    TscGPGlyphButton *H = mGrid->getGlyphButton(Col, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->ShowCaption = true;
    H->Caption = " Dávka";
    // H->Font->Color=(TColor)RGB(43,87,154);
    H->Options->NormalColor = clWhite;
    H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
    H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
    H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
    H->Options->NormalColorAlpha = 255;
    H->Options->FrameWidth = 1;
    H->Options->FrameNormalColor = clWhite;
    H->Width = 80;
    H->Height = mGrid->DefaultRowHeight;
    H->Options->ShapeStyle = scgpRect;
    H->Hint = "Smazat dávku";
    H->ShowHint = true;

    H = NULL;
    delete H;
}
