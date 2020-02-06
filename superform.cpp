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

  rStringGridEd1->Color = clWhite; // nastaven� pozad� barvy formul��e
  Form_definice_zakazek->Color = light_gray; // RGB(43,87,154);
  rHTMLLabel_effektivita->Font->Color = (TColor)RGB(89, 89, 89);
  rHTMLLabel_pocet_dnu->Font->Color = rHTMLLabel_effektivita->Font->Color;
  rHTMLLabel_pocet_prac_hod->Font->Color = rHTMLLabel_effektivita->Font->Color;
  rHTMLLabel_pozad_mnozstvi->Font->Color = rHTMLLabel_effektivita->Font->Color;
  rHTMLLabel_zacatek->Font->Color = rHTMLLabel_effektivita->Font->Color;
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

  ////////definice tabulky////////
  mGrid = new TmGrid(this); // v�dy nutno jako prvn�
  mGrid->Create(9, 5);
  // vypis(""); // prozmanuti vypisu - pro sicher
  mGrid->Tag = 9;
  // ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
  mGrid->ID = 0;
  mGrid->AntiAliasing_text = true;
  mGrid->Border.Width = 1;
  mGrid->DefaultRowHeight = 28;
  // v�t�� v��ka ��dku, kv�li velikosti comba - aby se ve�lo cel�

  ////nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
  nacti_PP();

  ////na�ten� zak�zek a cest
  if (Form1->d.v.ZAKAZKY->dalsi == NULL) // kdyz je spojak prazdny
  {
    // vytvo�� defaultn� �adek se zak�zkou a jej� cestou
    predvypln_default_zakazku();
    // predvypln_cestu(); ji� se nepou��v� pln� se z default_cestu() dat tak jako u�ivatlsky definovan� zak�zky
    // a hned se ulo�� do temp spoj�ku
    uloz_Defaulttemp_zakazku();
    uloz_Default_cestu();
    Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
    // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
    // po ulo�en� do hl.spojaku zak�zek pot�ebuji stejn� znovu ulo�it ��dek do temp_zakazek
    uloz_Defaulttemp_zakazku();
    uloz_Default_cestu();
  }
  else // pokud je ulo�eno n�co v zak�zk�ch tak je na�tu
  {
    nacti_zakazky();
  }
}

// ---------------------------------------------------------------------------
// nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
void TForm_definice_zakazek::nacti_PP() {
  F->log(__func__); // logov�n�
  rEditNum_pozad_mnozstvi->Text = Form1->d.v.PP.mnozstvi;
  rEditNum_pocet_dnu->Text = Form1->d.v.PP.dni_rok;
  rEditNum_effektivita->Text = Form1->d.v.PP.efektivita;
  rEditNum_pocet_prac_hod->Text = Form1->d.v.PP.hod_den;
  scEdit_zacatek->Text = Form1->d.v.PP.cas_start;
}

// ---------------------------------------------------------------------------
// vytvo�� defaultn� �adek se zak�zkou
void TForm_definice_zakazek::predvypln_default_zakazku() {
  // rStringGridEd1->Cells[0][1]="1";
  // rStringGridEd1->Cells[1][1]="1";
  // rStringGridEd1->Cells[2][1]="Nov� zak�zka";
  // rStringGridEd1->Cells[3][1]="�erven�";
  // rStringGridEd1->Cells[4][1]="100";  //pomer
  // rStringGridEd1->Cells[5][1]="NASTAVIT"; //tla��tko pro nastaven� JIG
  // rStringGridEd1->Cells[6][1]="200";
  // rStringGridEd1->Cells[7][1]="0";
  // rStringGridEd1->Cells[8][1]="0";
  // rStringGridEd1->Cells[9][1]="NASTAVIT";//tla��tko pro nastaven� ceesty
  // rStringGridEd1->Cells[10][1]=Form1->d.v.PP.TT;//TT
  F->log(__func__); // logov�n�
  loadHeader(0, true);

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

  mGrid->Refresh();
  // ROSTA TODO P�EINDEXOVAT
  Form1->d.v.vloz_temp_zakazku(UnicodeString(mGrid->Cells[1][0].Text), 0,
      UnicodeString(0), clRed, Form1->ms.MyToDouble(100),
      Form1->d.v.PP.TT, j, 0,
      0, 0);

}

// ----------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Default_cestu() {
  F->log(__func__); // logov�n�
  Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
  Cvektory::TZakazka *default_zakazka = Form1->d.v.vrat_temp_zakazku(1);
  Form1->d.v.inicializace_cesty(default_zakazka);
  while (objekt != NULL)
  { // vlo�en� defauln� cesty                        Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]                                        //v tomto p��pad� z�m�rn� takto proto�e se p�eb�r� vypo�ten� hodnota
    Form1->d.v.vloz_segment_cesty(default_zakazka,
        /* sloupec po�ad� se neukl�d� */ objekt->n, objekt->CT, 0, 0,
        objekt->RD, 0);
    objekt = objekt->dalsi;
  }
}

// ---------------------------------------------------------------------------
// na�ten� hotov�ch zak�zek, tato metoda je vol�na z FormShow(), pokud existuj� zak�zky
void TForm_definice_zakazek::nacti_zakazky() {
  F->log(__func__); // logov�n�
  ////vypln�n� stringgridu
  Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
  // ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
  Cvektory::TZakazka *ukaz2 = Form1->d.v.ZAKAZKY->predchozi;
  mGrid->RowCount = ukaz2->n * 2;
  // p�iprav�m si po�et ��dk� tabulky podle po�tu zak�zek *2 (slou�en� 2 ��dky)
  int i = 0;
  // Memo4->Lines->Add(AnsiString(ukaz->id));

  while (ukaz != NULL) {
    nacitam_zakazky = true;
    barva = (TColor)ukaz->barva;
    loadHeader(i, false);
    i++; // zv��en� indexu ��dku

    mGrid->Refresh(); // mus� b�t
    TscGPComboBox *C = mGrid->getCombo(1, i);
    TscGPListBoxItem *I;
    mGrid->Cells[1][i - 1].Text = ukaz->name;
    I = C->Items->Add();
    I->Caption = "Servisn�";
    I = C->Items->Add();
    I->Caption = "B�n�";
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

    // vlo��m dal�� ��dek (resp. nav���m jejich po��tadlo, po��t� se od 1)
    // rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky

    i++; // dal�� zv��en� indexu ��dku
    // posun na dal�� prvek v seznamu
    ukaz = ukaz->dalsi;
  }
  nacitam_zakazky = false;
  // ulo�en� ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejn� data jako jsou zobrazena
  Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp(); // pouze ZAKAZKY_temp=ZAKAZKY
}

// ----------------------------------------------------------------------------
// vol�n� sub formul��� JIG a CESTA a p��padn� ukl�d�n� do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender) {
  // definice ukazatele na aktu�ln� editovanou zak�zku     //��slo ��dku kde kliku do�lo
  Cvektory::TZakazka *zakazka =
      Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
  AnsiString prochazet;

  ////////////jig form- byl klik na bu�ku v 5. sloupci
  if (rStringGridEd1->Col == 5) {

    Form1->MB
        ("V t�to verzi programu nen� mo�n� nastavovat parametry jigu. Jig je mo�n� nastavit pouze v parametrech linky."
        );

    // DOCASNE ODSTAVENI NASTAVENI JIGU PRO KONKRETNI ZAKAZKU
    // napln�n� dat
    // Form_jig->Edit_jig_pocet_ks->Text=zakazka->jig.ks;
    // Form_jig->Edit_jig_delka->Text=zakazka->jig.delka;
    // Form_jig->Edit_jig_sirka->Text=zakazka->jig.sirka;
    // Form_jig->Edit_jig_vyska->Text=zakazka->jig.vyska;
    // //zobrazen� formu + ulo�en� dat pokud je zvoleno OK
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
    // if(mrOk==Form1->MB("Nastala zm�na parametr� d�lka nebo ���ka jigu, budou p�epo��t�ny parametry linky pro tuto zak�zku.",MB_OKCANCEL)) {
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

  ////////////cesty form - byl klik na bu�ku 9. sloupci
  if (rStringGridEd1->Col == 9) {
    // Form_cesty->rStringGridEd_cesty->Columns->Clear();
    ////napln�n� picklistu
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
    Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");

    // AKTUALIZACE SEGMENT� DLE PARAMETR� OBJEKTU U PRVN� ZAK�ZKY,v p��pad� prvn� zak�zky se berou hodnoty z parametr� objektu nikoliv zak�zky, co� zajist� pat�i�nou aktuliazaci
    // to sam� je vol�no v unit1.cpp v Nastavitparametry1Click z ur�it�ho pohledu se jedn� o duplicitn� algoritmus, ale v p��pad� aktualizac� jinak ne� p�es parametry objektu lze pova�ovat za nutnost
    if (zakazka->n == 1) // pouze pokud se jedn� o prvn� zak�zku
    {
      Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
      while (O != NULL)
          // proch�z� v�echnyobjekty a bu� je ("Ano") objekt i na cest� nebo nen� ("Ne")
      {
        Cvektory::TCesta *C = Form1->d.v.obsahuje_segment_cesty_objekt(O,
            zakazka);
        if (C != NULL) // objekt je segmentem cesty
        {
          C->CT = O->CT;
          C->RD = O->RD;
          // v p��pad� prvn� zak�zky se berou hodnoty z parametr� objektu nikoliv zak�zky, co� zajist� pat�i�nou aktuliazaci
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

    ////na��t�n� dat - nen� t�eba o�et�ovat, proto�e existuje default cesta v seznamu
    // vyp�e d��ve nadefinovanou cestu v�etn� segment� z nejdel�� mo�n� cesty, kter� nejsou sou��st� aktu�ln� cesty
    Cvektory::TObjekt *O = Form1->d.v.OBJEKTY->dalsi;
    // p��d�n� po�tu ��dk�
    Form_cesty->rStringGridEd_cesty->RowCount =
        Form1->d.v.OBJEKTY->predchozi->n + 1;
    // p�id�n� dal��ho ��dku     //pridani k zobrazeni if ukaz param
    int i = 0;
    while (O != NULL)
        // proch�z� v�echnyobjekty a bu� je ("Ano") objekt i na cest� nebo nen� ("Ne")
    {
      i++;
      Cvektory::TCesta *C = Form1->d.v.obsahuje_segment_cesty_objekt(O,
          zakazka);
      if (C != NULL) // zahrnut� segmenty cesty
      {
        Form_cesty->rStringGridEd_cesty->Cells[0][i] = C->objekt->n;
        Form_cesty->rStringGridEd_cesty->Cells[1][i] = C->objekt->name;
        Form_cesty->rStringGridEd_cesty->Cells[2][i] = C->CT;
        Form_cesty->rStringGridEd_cesty->Cells[3][i] = C->RD * 60;
        // zobraz�m v�dy v [m/min]
        Form_cesty->rStringGridEd_cesty->Cells[4][i] = C->Tv;
        Form_cesty->rStringGridEd_cesty->Cells[5][i] = C->Tc;
        Form_cesty->rStringGridEd_cesty->Cells[6][i] = C->Opak;
        Form_cesty->rStringGridEd_cesty->Cells[7][i] = "Ano";
      }
      else // nezahrnut� segment nejdel�� cesty v aktu�ln� cest�
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

    ////ukl�d�n� dat - jednotliv�ho segmentu cesty pokud je pova�ov�n k zahrnutn� ze strany u�ivatele
    // formul�� na st�ed
    Form_cesty->Left = Form1->ClientWidth / 2 - Form_cesty->Width / 2;
    Form_cesty->Top = Form1->ClientHeight / 2 - Form_cesty->Height / 2;

    double CT;
    double Tv;
    double Tc;

    if (mrOk == Form_cesty->ShowModal()) {
      Form1->d.v.inicializace_cesty(zakazka);
      for (int i = 1; i < Form_cesty->rStringGridEd_cesty->RowCount; i++) {
        if (Form_cesty->rStringGridEd_cesty->Cells[7][i] == "Ano")
            // pokud je za�krnuto neproch�zek objekt se neulo�� do cesty
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
          else { // p�evedu minuty na sekundy
            CT = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[2][i]) * 60.0;
            // min na sekundy
            Tv = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[4][i]) * 60.0;
            Tc = Form1->ms.MyToDouble
                (Form_cesty->rStringGridEd_cesty->Cells[5][i]) * 60.0;
          }

          Form1->d.v.vloz_segment_cesty(zakazka,
              /* sloupec po�ad� se neukl�d� */ /* pozor na �azen� */ Form_cesty
              ->rStringGridEd_cesty->Cells[0][i].ToInt(), // ID-do�e�it
              Form1->ms.MyToDouble(CT), // CT
              Form1->ms.MyToDouble(Tv), // Tv
              Form1->ms.MyToDouble(Tc), // Tc
              Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i])
              / 60.0, // RD ulozim v m/sec
              Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])
              // Opak   //ulozeni stavu pro cestu - roletka
              );
        }
        // vymaz�n� textu z ji� nepot�ebn�ho ��dku
        Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();

      }
    }
  }

}

// ---------------------------------------------------------------------------
// TLA��TKO ULO�IT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
  F->log(__func__); // logov�n�
  zmena_TT = false;
  bool neukladat = false; // pokud nebudou spln�ny podm�nky, nelze form ulo�it
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
  // if(mrOk==Form1->MB("Nastala zm�na TakTime, kter� ovlivn� technologick� �as a rychlost pohonu upravovan� zak�zky.",MB_OKCANCEL)) {
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

  TColor barva; // mus� b�t v TColor kv�li ukl�d�n� do TColor objektu!!!

  // parametry v�roby
  Form1->d.v.PP.mnozstvi = Form1->ms.MyToDouble(rEditNum_pozad_mnozstvi->Text);
  Form1->d.v.PP.dni_rok = Form1->ms.MyToDouble(rEditNum_pocet_dnu->Text);
  Form1->d.v.PP.efektivita = Form1->ms.MyToDouble(rEditNum_effektivita->Text);
  Form1->d.v.PP.hod_den = Form1->ms.MyToDouble(rEditNum_pocet_prac_hod->Text);
  Form1->d.v.PP.cas_start = TDateTime(scEdit_zacatek->Text);

  // ulo�en� editovan�ch zak�zek
  for (unsigned int i = 1; i < mGrid->RowCount;
  i++) // dole je�t� zvy�uji po��tadlo!!!
  {

    double pomer;
    double TT;
    double nvoziku;
    double nservis_voziku;
    double nopak_voziku;
    UnicodeString nazev;

    // o�et�en� - pokud se p�i ukl�d�n� formu objev� pr�zdn� hodnoty, automaticky vypln�m "0" a p�id�m n�zev zak�zky

    // NEW
    if (mGrid->Cells[1][i - 1].Text.IsEmpty())
      nazev = "Nov� zak�zka";
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

    mGrid->Refresh(); // mus� b�t
    TscGPComboBox *C = mGrid->getCombo(1, i);
    if (usersColor)
      barva = (TColor)mGrid->getButton(2, i)->Options->NormalColor;
    // mus� b�t a� tady, jinak spadne
    // ShowMessage(C->ItemIndex);
    // ulo�en� aktu�ln�ch hodnot do dan� temp_zakazky

    if (i == 1) // ulo� TT do projektu pro status n�vrh��, pouze z prvn� zak�zky
          Form1->d.v.PP.TT = Form1->ms.MyToDouble(mGrid->Cells[10][i].Text);

    Form1->d.v.edituj_temp_zakazku(mGrid->Cells[11][i].Text.ToInt(), // n
        mGrid->Cells[11][i].Text.ToInt(), // ID
        C->ItemIndex, // TYP
        nazev, // NAME
        barva, // COLOR
        pomer, // POMER
        TT, // TT
        // JIG se u� jako parametr nepo�aduje, stejn� jako cesta, jedn� se o p�ed�v�n� ukazatelem p�i zav�r�n� pat�i�n�ho formul��e
        nvoziku, // n-voz�ku
        nservis_voziku, // n-servis voz�k�
        nopak_voziku // n-opak voz�k�
        );
    i++; // na��t�m ob jeden ��dek!!!
  }
  // samotn� ulo�en�
  Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
  // ulo�en� do ostr�ho spoj�ku ZAKAZKY+sm�znut� ZAKAZKY_temp
  Form1->d.v.generuj_VOZIKY(); // vygenerov�n� voz�k� dle zadan�ch zak�zek
  // konstrukce kter� m� vz�t CT a RD objektu z prvn� zak�zky a na�it ho do CT a RD objekt�
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

  // zav�en� formul��e s n�sledn�m DuvodUlozit(true) po modalshow v unit1
  if (neukladat == false)
    Form_definice_zakazek->Close(); // kdyby n�co tak nic
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
  // ukaz = ukaz->dalsi; // posun na dal�� prvek v seznamu
  // }
  //
  // TBarva *ukaz1 = BARVY->dalsi;
  // while (ukaz1 != NULL) {
  // if (ukaz1->id_radek == FromIndex)
  // ukaz1->barva_voziku = ColorToIndex;
  // if (ukaz1->id_radek == ToIndex)
  // ukaz1->barva_voziku = ColorFromIndex;
  // ukaz1 = ukaz1->dalsi; // posun na dal�� prvek v seznamu
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
void __fastcall TForm_definice_zakazek::RzStringGrid1GetEditMask
    (TObject *Sender, int ACol, int ARow, UnicodeString &Value)

{ // maska pro validaci zaznamu

}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button_DELClick(TObject *Sender) {

  //
  // // Tla��tko na smaz�n� ��dku
  // RzStringGrid1->Rows[RzStringGrid1->Row]->Clear();
  // rStringGridEd1->Rows[rStringGridEd1->Row]->Clear();
  //
  // TForm_definice_zakazek::TBarva *ukaz = BARVY->dalsi;
  // // ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
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
  // // posun na dal�� prvek v seznamu
  // ukaz = ukaz->dalsi;
  // }
  //
  // // posun ��dk�
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

void __fastcall TForm_definice_zakazek::RzStringGrid1Click(TObject *Sender) {

  // Button_DEL->Visible = true;
  // Button_DEL->Top = RzStringGrid1->Top + RzStringGrid1->Row * 19 + 1;

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
  // //MEZERN�K
  // case 32:break;
  // //PAGE UP
  // case 33:;break;
  // //PAGE DOWN
  // case 34:break;
  // //END
  // case 35:break;
  // //HOME
  // case 36:break;
  // //�IPKA DOL�
  // case 40:break;
  // //�IPKA LEV�
  // case 37:break;
  // //�IPKA PRAV�
  // case 39:break;
  // //�IPKA NAHORU
  // case 38:break;
  // //�IPKA DOL�
  // case 98:break;
  // //�IPKA LEV�
  // case 100:break;
  // //�IPKA PRAV�
  // case 102:break;
  // //�IPKA NAHORU
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
// zkop�ruje p�edchoz�
void __fastcall TForm_definice_zakazek::rButton1Click(TObject *Sender) {
  rStringGridEd1->RowCount++;
  rStringGridEd1->Cols[0]->Add(rStringGridEd1->RowCount - 1);

  if (rStringGridEd1->RowCount > 2) {
    int i = rStringGridEd1->RowCount - 1;

    rStringGridEd1->Cells[1][i] = i;
    rStringGridEd1->Cells[2][i] = rStringGridEd1->Cells[2][i - 1];
    rStringGridEd1->Cells[3][i] = rStringGridEd1->Cells[3][i - 1];
    rStringGridEd1->Cells[4][i] = rStringGridEd1->Cells[4][i - 1];
    rStringGridEd1->Cells[5][i] = rStringGridEd1->Cells[5][i - 1];
    rStringGridEd1->Cells[6][i] = rStringGridEd1->Cells[6][i - 1];
    rStringGridEd1->Cells[7][i] = rStringGridEd1->Cells[7][i - 1];
    rStringGridEd1->Cells[8][i] = rStringGridEd1->Cells[8][i - 1];
    rStringGridEd1->Cells[9][i] = rStringGridEd1->Cells[9][i - 1];
    rStringGridEd1->Cells[10][i] = rStringGridEd1->Cells[10][i - 1];
    rStringGridEd1->Cells[11][i] = rStringGridEd1->Cells[11][i - 1];
    rStringGridEd1->Cells[12][i] = rStringGridEd1->Cells[12][i - 1];
  }
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::scGPGlyphButton_add_zakazkaClick
    (TObject *Sender) {

  mGrid->RowCount = mGrid->RowCount + 2;

  int i = mGrid->RowCount - 1;
  Cvektory::TJig j;
  j.sirka = 1;
  j.delka = 1;
  j.vyska = 1;
  j.ks = 1;

  if (i > 1)
  { // defaultn� zak�zka je ulo�ena hned v temp_spojaku + hlavnim pri form_show, �ili tady ukl�d�m a� dal�� ��dky

    Cvektory::TZakazka *ukaz = F->d.v.ZAKAZKY_temp->predchozi;

    add_zakazka = true;
    loadHeader(i, false);
    mGrid->Cells[1][i - 1].Text = "Nov� zak�zka";
    mGrid->Cells[4][i].Text = "100";
    mGrid->Cells[6][i].Text = "0";
    mGrid->Cells[9][i].Text = "0";
    mGrid->Cells[10][i].Text = "120";
    mGrid->Cells[11][i].Text = ukaz->n + 1;

    mGrid->Refresh();
    // kv�li pr�ci s combem je nutn� refresh po nastaven� na typ COMBOEDIT
    TscGPComboBox *C = mGrid->getCombo(1, i);
    TscGPListBoxItem *I;
    // na�ten� hodnoty rozte�e do roletky + nastaven� jako ItemIndex=0
    I = C->Items->Add();
    I->Caption = "Servisn�";
    C->ItemIndex = 0;
    I = C->Items->Add();
    I->Caption = "B�n�";
    C->ItemIndex = 1;

    Form1->d.v.vloz_temp_zakazku(UnicodeString(mGrid->Cells[11][i].Text),
        C->ItemIndex, UnicodeString(mGrid->Cells[1][i - 1].Text), clRed,
        // Form_color_dialog->scColorGrid1->ColorValue,
        Form1->ms.MyToDouble(100),
        Form1->ms.MyToDouble(mGrid->Cells[10][i].Text), j,
        mGrid->Cells[4][i].Text.ToInt(), mGrid->Cells[6][i].Text.ToInt(),
        mGrid->Cells[9][i].Text.ToInt());

    // int j=rStringGridEd1->RowCount-1;
    int j = mGrid->Cells[11][i].Text.ToInt();

    // p�i p�id�n� dal�� zak�zky ulo��m do cesty defaultn� hodnoty
    Cvektory::TObjekt *objekt = Form1->d.v.OBJEKTY->dalsi; // inicializace
    Cvektory::TZakazka *nova_zakazka = Form1->d.v.vrat_temp_zakazku(j);
    Form1->d.v.inicializace_cesty(nova_zakazka);
    while (objekt != NULL) { // vlo�en� defauln� cesty
      Form1->d.v.vloz_segment_cesty(nova_zakazka,
          /* sloupec po�ad� se neukl�d� */ objekt->n, 0, 0, 0, 0, 0);
      objekt = objekt->dalsi;

    }

  }
  add_zakazka = false;

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
void __fastcall TForm_definice_zakazek::Button5Click(TObject *Sender) {
  // Cvektory::TZakazka *zakazka_temp=Form1->d.v.vrat_temp_zakazku(Edit_n_cesty->Text.ToInt());//inicializace
  // na��t�n� dat
  // AnsiString prochazet;
  // if(zakazka_temp!=NULL)
  // if(zakazka_temp->cesta!=NULL)//pokud ji� byla cesta definovan�
  // {
  // Memo4->Lines->Add("zak�zka_temp n�zev:\n"+zakazka_temp->name);
  // Cvektory::TCesta *ukaz=zakazka_temp->cesta->dalsi;//p�esko�� hlavi�ku, jde rovnou na prvn� segment cesty
  // while(ukaz!=NULL)
  // {
  // Memo4->Lines->Add
  // (
  // AnsiString(ukaz->n)+","+
  // AnsiString(ukaz->objekt->short_name)+","+
  // AnsiString(ukaz->CT)+","+
  // AnsiString(ukaz->RD)+","+
  // AnsiString(ukaz->Tc)+","+
  // AnsiString(prochazet)
  // );
  // ukaz=ukaz->dalsi;
  // }
  // }
  //
  // Cvektory::TZakazka *zakazka=Form1->d.v.ZAKAZKY->dalsi;//inicializace na prvn� zakazku
  // //na��t�n� dat
  //
  // if(zakazka!=NULL)
  // if(zakazka->cesta!=NULL)//pokud ji� byla cesta definovan�
  // {
  // Memo4->Lines->Add("zak�zka n�zev:\n"+zakazka->name);
  // Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//p�esko�� hlavi�ku, jde rovnou na prvn� segment cesty
  // while(ukaz!=NULL)
  // {
  // Memo4->Lines->Add
  // (
  // AnsiString(ukaz->n)+","+
  // AnsiString(ukaz->objekt->short_name)+","+
  // AnsiString(ukaz->CT)+","+
  // AnsiString(ukaz->RD)+","+
  // AnsiString(ukaz->Tc)+","+
  // AnsiString(ukaz->Tv)
  // );
  // ukaz=ukaz->dalsi;
  // }
  // }
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
  mGrid->Show(); // vykresl� tabulku

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

void __fastcall TForm_definice_zakazek::rEditNum_pocet_prac_hodKeyDown
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

void __fastcall TForm_definice_zakazek::rEditNum_pozad_mnozstviKeyDown
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

void __fastcall TForm_definice_zakazek::rEditNum_effektivitaKeyDown
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

void __fastcall TForm_definice_zakazek::scEdit_zacatekKeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift) {
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

void __fastcall TForm_definice_zakazek::rStringGridEd1KeyDown(TObject *Sender,
    WORD &Key, TShiftState Shift) {
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

void __fastcall TForm_definice_zakazek::FormKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift)

{
  F->log(__func__); // logov�n�
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

void __fastcall TForm_definice_zakazek::FormMouseMove(TObject *Sender,
    TShiftState Shift, int X, int Y) {
  F->log(__func__); // logov�n�
  if (mGrid->RowCount <= 2) {
    scGPGlyphButton_remove->Enabled = false;
  }
  else
    scGPGlyphButton_remove->Enabled = true;
}

// ---------------------------------------------------------------------------
void TForm_definice_zakazek::getmGridWidth() {
  F->log(__func__); // logov�n�
  mGrid->Columns[0].Width = 30;
  mGrid->Columns[1].Width = 80;
  mGrid->Columns[2].Width = 55;
  mGrid->Columns[3].Width = 80;
  mGrid->Columns[4].Width = 55;
  mGrid->Columns[5].Width = 80;
  mGrid->Columns[6].Width = 50;
  mGrid->Columns[7].Width = 80;
  mGrid->Columns[8].Width = 50;
  mGrid->Columns[9].Width = 30;
}

void TForm_definice_zakazek::setGlyphButton(double Row, Typ_buttonu typ) {
  if (typ == krizek_davky) {
    F->log(__func__); // logov�n�
    mGrid->Refresh();
    TscGPGlyphButton *H = mGrid->getGlyphButton(3, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->ShowCaption = true;
    H->Caption = "D�vka 1";
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
    H->Hint = "Smazat d�vku";
    H->ShowHint = true;

    H = NULL;
    delete H;

  }

  if (typ == krizek) {

    F->log(__func__); // logov�n�
    mGrid->Refresh();
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
    H->Hint = "Smazat zak�zku";
    H->ShowHint = true;

    H = NULL;
    delete H;

  }
}

void TForm_definice_zakazek::OnClick(long Tag, long ID, unsigned long Col,
    unsigned long Row) {
  F->log(__func__); // logov�n�
  if (Col == 2) {
    Form_color_dialog->SetDialogPosition(Row);
    Form_color_dialog->ShowModal();
    scGPButton_Ulozit->SetFocus();

  }

}

void TForm_definice_zakazek::setButtonColor(int Row) {
  F->log(__func__); // logov�n�
  mGrid->getButton(2, Row)->Options->HotColor =
      Form_color_dialog->scColorGrid1->ColorValue;
  mGrid->getButton(2, Row)->Options->HotColorAlpha = 255;
  mGrid->getButton(2, Row)->Options->FocusedColor =
      Form_color_dialog->scColorGrid1->ColorValue;
  mGrid->getButton(2, Row)->Options->FocusedColorAlpha = 225;
  mGrid->getButton(2, Row)->Options->NormalColor =
      Form_color_dialog->scColorGrid1->ColorValue;
}

void TForm_definice_zakazek::loadHeader(int Row, bool novy) {
  // F->log(__func__);//logov�n�
  // mGrid->Top=scLabel_header->Height + scGPButton_plan_vyroby->Height +  10;
  // mGrid->Left=5;
  // if(add_zakazka) Row=Row-1;
  // else  Row=Row;
  // mGrid->scGPImageCollection=scGPImageCollection_layout;
  // mGrid->SetColumnAutoFit(-4);
  // getmGridWidth();
  // mGrid->Cells[0][Row].Type=mGrid->IMAGE;
  // mGrid->Cells[0][Row].ImageIndex=0;   //dynamicky plnit
  // F->Memo("R3");
  // mGrid->Refresh();
  // // if(!add_zakazka) // mGrid->getImage(0,Row)->AutoSize=true;    //opodminkovan� prozat�m - pam�t chyba
  // mGrid->Cells[0][Row].Align=mGrid->LEFT;
  // F->Memo("R4");
  // mGrid->Cells[0][Row].Valign=mGrid->TOP;
  // // mGrid->getImage(0,Row)->Width=mGrid->Columns[0].Width;
  // // mGrid->getImage(0,Row)->Height=mGrid->DefaultRowHeight *2 ;
  // mGrid->Cells[1][Row].Type=mGrid->EDIT;
  // mGrid->Cells[1][Row].Align=mGrid->LEFT;
  // F->Memo("R5");
  // mGrid->Cells[2][Row].Text="";
  // mGrid->Cells[2][Row+1].Type=mGrid->BUTTON;
  //
  // mGrid->Refresh();
  // F->Memo(Row);
  // if(novy || add_zakazka)
  // {
  //
  // mGrid->getButton(2,Row+1)->Options->NormalColor=clRed;//Form_color_dialog->scColorGrid1->ColorValue;
  //
  // } else  mGrid->getButton(2,Row+1)->Options->NormalColor=barva; //p�i�azen� barvy ze spoj�ku zak�zek
  // F->Memo("R6");
  // mGrid->getButton(2,Row+1)->Options->NormalColorAlpha=250;
  // mGrid->getButton(2,Row+1)->Options->FrameWidth=1;
  // mGrid->getButton(2,Row+1)->Options->FrameNormalColor=mGrid->getButton(2,Row+1)->Options->NormalColor;
  // mGrid->getButton(2,Row+1)->Options->FrameNormalColorAlpha=250;
  // mGrid->getButton(2,Row+1)->Options->HotColor=mGrid->getButton(2,Row+1)->Options->NormalColor;
  // mGrid->getButton(2,Row+1)->Options->HotColorAlpha=255;
  // mGrid->getButton(2,Row+1)->Options->FocusedColor=mGrid->getButton(2,Row+1)->Options->NormalColor;
  // mGrid->getButton(2,Row+1)->Options->FocusedColorAlpha=225;
  // mGrid->Cells[2][Row+1].Background->Color=mGrid->getButton(2,Row+1)->Options->NormalColor;
  //
  // F->Memo("R7");
  // mGrid->Cells[3][Row].Text="";
  // mGrid->Cells[4][Row].Type=mGrid->glyphBUTTON;
  // setGlyphButton(Row);
  // mGrid->Cells[5][Row].Text="";
  // mGrid->Cells[6][Row].Text="";
  // mGrid->Cells[7][Row].Text="";
  // mGrid->Cells[7][Row].Align=mGrid->LEFT;
  // mGrid->Cells[8][Row].Text="";
  // mGrid->Cells[9][Row].Text="";
  // mGrid->Cells[10][Row].Text="";
  // mGrid->Cells[11][Row].Text="Id";
  // mGrid->Cells[1][Row+1].Type=mGrid->COMBO;
  // mGrid->Cells[4][Row].Align=mGrid->LEFT;
  // mGrid->Cells[3][Row+1].Text="";
  // mGrid->Cells[4][Row+1].Type=mGrid->EDIT;
  // mGrid->Cells[4][Row+1].Align=mGrid->LEFT;
  // mGrid->Cells[5][Row+1].Text="";
  // mGrid->Cells[6][Row+1].Type=mGrid->EDIT;
  // mGrid->Cells[6][Row+1].Align=mGrid->LEFT;
  // mGrid->Cells[7][Row+1].Text="";
  // mGrid->Cells[8][Row+1].Text="";
  // mGrid->Cells[9][Row+1].Type=mGrid->EDIT;
  // mGrid->Cells[10][Row+1].Type=mGrid->EDIT;
  // F->Memo("R8");
  //
  // mGrid->Cells[0][Row].RightBorder->Color=clWhite;
  // mGrid->Cells[1][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[3][Row].RightBorder->Color=clWhite;
  // mGrid->Cells[3][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[5][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[6][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[7][Row].RightBorder->Color=clWhite;
  // mGrid->Cells[7][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[8][Row+1].RightBorder->Color=clWhite;
  // mGrid->Cells[1][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[1][Row+1].BottomBorder->Color=clWhite;
  // mGrid->Cells[2][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[3][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[4][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[5][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[6][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[7][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[8][Row].BottomBorder->Color=clWhite;
  // mGrid->Cells[9][Row].BottomBorder->Color=clWhite;
  //
  // mGrid->MergeCells(0,Row,0,Row+1);
  // mGrid->MergeCells(1,Row,2,Row);
  // mGrid->MergeCells(5,Row,9,Row);
  //
  // ////default hodnoty pokud nejsou ��dn� zak�zky
  // if(Form1->d.v.ZAKAZKY->dalsi==NULL)//kdyz je spojak prazdny
  // {
  // mGrid->Cells[1][Row].Text="N�zev zak�zky";
  // mGrid->Cells[4][Row+1].Text="100";
  // mGrid->Cells[6][Row+1].Text="0";
  // mGrid->Cells[9][Row+1].Text="0";
  // mGrid->Cells[10][Row+1].Text="";
  // mGrid->Cells[11][Row+1].Text="1";
  //
  // //kv�li pr�ci s combem je nutn� refresh po nastaven� na typ COMBOEDIT
  // mGrid->Refresh();
  // TscGPComboBox *C=mGrid->getCombo(1,Row+1);
  // TscGPListBoxItem *I;
  // //na�ten� hodnoty rozte�e do roletky + nastaven� jako ItemIndex=0
  // I=C->Items->Add();
  // I->Caption = "Servisn�"; C->ItemIndex=0;
  // I=C->Items->Add();
  // I->Caption = "B�n�"; C->ItemIndex=1;
  // I=NULL;delete I;
  // }

  mGrid->SetColumnAutoFit(-4);
  getmGridWidth();

  if (novy || add_zakazka) {

    F->log(__func__); // logov�n�
    mGrid->Top = scLabel_header->Height + scGPButton_plan_vyroby->Height + 10;
    mGrid->Left = 5;

    // mGrid->Cells[0][0].Type=mGrid->DRAW;   //pic
   // mGrid->Cells[0][1].Type = mGrid->DRAW; // pic slou�it
  //  mGrid->Cells[0][2].Type = mGrid->DRAW; // pic slou�it svisle

 //   mGrid->Cells[1][0].Type = mGrid->EDIT; // nazev  text  - slou�it pod�ln�
    // mGrid->Cells[1][1].Type=mGrid->EDIT; //jig text
    // mGrid->Cells[1][2].Type=mGrid->EDIT; //jig text  slou�it   svisle

   // mGrid->Cells[3][1].Type = mGrid->EDIT; // jig celkem
 //   mGrid->Cells[3][2].Type = mGrid->EDIT; // jig servis
   // mGrid->Cells[5][3].Type = mGrid->EDIT; // d�vka po�et
   // mGrid->Cells[5][4].Type = mGrid->EDIT; // d�vka pr�zdn�ch
   // mGrid->Cells[4][1].Type = mGrid->glyphBUTTON; // roletka
    // mGrid->Cells[5][1].Type=mGrid->EDIT; //text d�vky (asi i count)
    // mGrid->Cells[6][1].Type=mGrid->EDIT; //text d�vky (asi i count)

    mGrid->Cells[0][0].Type = mGrid->glyphBUTTON; // X  zakazka
    mGrid->Cells[3][1].Type = mGrid->glyphBUTTON; // X  davka
    // mGrid->Cells[7][1].Type=mGrid->EDIT; //color
    mGrid->Cells[2][0].Font->Size=15;

    mGrid->Cells[0][0].Text = "1"; // id zak�zky
    mGrid->Cells[1][0].Text = "N�zev zak�zky";
    //mGrid->Cells[1][1].Text = "Jig";
   // mGrid->Cells[1][1].Font->Orientation=900;
   //	mGrid->Cells[1][1].Valign=mGrid->TOP;
    mGrid->Cells[1][1].Font->Size=14;
    mGrid->Cells[1][2].Text = "po�et";   mGrid->Cells[1][2].RightBorder->Color = clWhite;
    mGrid->Cells[1][3].Text = "pr�zdn�ch";   mGrid->Cells[1][3].RightBorder->Color = clWhite;
    mGrid->Cells[1][4].Text = "celkem";       mGrid->Cells[1][4].RightBorder->Color = clWhite;
    mGrid->Cells[2][1].Align = mGrid->LEFT;
    mGrid->Cells[2][2].Align = mGrid->LEFT;
    mGrid->Cells[2][3].Align = mGrid->LEFT;
    mGrid->Cells[4][3].Align = mGrid->LEFT;
//    mGrid->Cells[2][1].RightBorder->Color = clWhite;
//    mGrid->Cells[2][2].RightBorder->Color = clWhite;
//    mGrid->Cells[2][3].RightBorder->Color = clWhite;

   //mGrid->Col

    mGrid->Cells[2][2].Text = "100"; // value
    mGrid->Cells[2][3].Text = "3"; // value
    mGrid->Cells[2][4].Text = "103"; // value
    mGrid->Cells[4][1].Font->Size=14;
  	mGrid->Cells[4][1].Align=mGrid->CENTER;
   //	mGrid->Cells[4][1].Valign=mGrid->TOP;
    //mGrid->Cells[4][1].Font->Orientation=900;
  //  mGrid->Cells[3][1].Text = "D�vka 1"; // value
    mGrid->Cells[3][2].Text = "60"; // value
    mGrid->Cells[3][3].Text = "3"; // value
    mGrid->Cells[3][4].Text = "63"; // value


     //   mGrid->Cells[4][4].RightBorder->Color = clWhite;
     //  mGrid->Cells[4][3].RightBorder->Color = clWhite;
//    mGrid->Cells[5][2].RightBorder->Color = clWhite;
//    mGrid->Cells[5][3].RightBorder->Color = clWhite;
//    mGrid->Cells[5][4].RightBorder->Color = clWhite;
//    mGrid->Cells[6][3].RightBorder->Color = clWhite;
//    mGrid->Cells[6][4].RightBorder->Color = clWhite;
  //  mGrid->Cells[1][1].RightBorder->Color = clWhite;
 //  mGrid->Cells[4][1].RightBorder->Color = clWhite;

    mGrid->Cells[0][3].Background->Color = light_gray;
    mGrid->Cells[0][2].LeftBorder->Color = light_gray;
    mGrid->MergeCells(1, 0, 3, 0); // n�zev     - vodorobne
    mGrid->MergeCells(1, 1, 2, 1); // n�zev     - vodorobne
   // mGrid->MergeCells(1, 1, 1, 3); // jig text   - svisle
//
//    mGrid->MergeCells(4, 1, 4, 3); // davky text  - svisle
//
    mGrid->MergeCells(0, 1, 0, 4); // merge ID �i obr�zek
     mGrid->MergeCells(1, 0, 1, 1); // merge nazev svisle
     mGrid->MergeCells(2, 0, 2, 1); // merge nazev svisle
   // mGrid->MergeCells(4, 2, 7, 2); // hlavi�ka d�vka
   // mGrid->MergeCells(0, 4, 3, 4); // doln� ��dek zleva  IF - d�vky show
//    mGrid->MergeCells(5, 1, 6, 1); // D�vky
//    mGrid->MergeCells(7, 1, 8, 1); // D�vky
    // mGrid->MergeCells(0,3,3,3); //merge bunek p�ed roletkou (+1 ��dek)
    setGlyphButton(1, krizek_davky); // p�ed�vat Row
    setGlyphButton(0, krizek); // p�ed�vat Row
   // mGrid->MergeCells(4, 1, 7, 1); // merge roletka d�vky

   // mGrid->Cells[4][1].RightBorder->Color = clWhite;

  }

}

void __fastcall TForm_definice_zakazek::FormClose(TObject *Sender,
    TCloseAction &Action)

{
  F->log(__func__); // logov�n�
  mGrid->Delete();
}
// ---------------------------------------------------------------------------
