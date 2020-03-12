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
void TForm_definice_zakazek::nastav_form()
{
  F->log(__func__); // logování
  // nastavení globálních barev
  light_gray = (TColor)RGB(240, 240, 240);
  def_gray = (TColor)RGB(200, 200, 200);

  Form_definice_zakazek->Color = light_gray; // RGB(43,87,154);

  scGPButton2->Options->NormalColor = Form_definice_zakazek->Color;
  scGPButton2->Options->FocusedColor = Form_definice_zakazek->Color;
  scGPButton2->Options->HotColor = Form_definice_zakazek->Color;
  scGPButton2->Options->PressedColor = Form_definice_zakazek->Color;
  scGPButton2->Options->FramePressedColor = Form_definice_zakazek->Color;

  Form1->m.designButton(scGPButton_Ulozit, Form_definice_zakazek, 1, 2);
  Form1->m.designButton(scGPButton_storno, Form_definice_zakazek, 2, 2);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// zobrazení formuláøe
void __fastcall TForm_definice_zakazek::FormShow(TObject *Sender)
{
  F->log(__func__); // logování
  volno = false;
	pocet_davek = 0;
	Akce=NIC;
	jedno_ze_tri_otoceni_koleckem_mysi=1;
	doba_neotaceni_mysi=0;
	//bitmapa pro uložení pøesovaného obrazu - PAN
	Pan_bmp=new Graphics::TBitmap();
  Top=F->scLabel_titulek->Height;
	Left=F->ClientWidth - Form_definice_zakazek->Width;
	pan_non_locked=false;
  ////nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
  nacti_PP();
  nastav_jazyk();
  F->d.v.vytvor_default_zakazku(); // vytvoøí nebo aktualizuje defaultní zakázku

  ////naètení zakázek a cest
  if (Form1->d.v.ZAKAZKY->dalsi == NULL) // kdyz je spojak prazdny
  {
    // vytvoøí defaultní øadek se zakázkou a její cestou

    // predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
    // a hned se uloží do temp spojáku
    // uloz_Defaulttemp_zakazku();
    // uloz_Default_cestu();
    // Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
    // // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
    // // po uložení do hl.spojaku zakázek potøebuji stejnì znovu uložit øádek do temp_zakazek
    // uloz_Defaulttemp_zakazku();
    // uloz_Default_cestu();
    loadHeader();
  }
  else // pokud je uloženo nìco v zakázkách tak je naètu
  {
    nacti_zakazky();
    set_formHW_button_positions();
  }
  volno = true;

}

// ---------------------------------------------------------------------------
// provede pøepnutí jazyka na komponentách, mGridy se zmìní automaticky pøi vytváøení
void TForm_definice_zakazek::nastav_jazyk() {
  F->log(__func__); // logování
	scLabel_header->Caption = F->ls->Strings[432];
  scGPGlyphButton_add_zakazka->Hint = F->ls->Strings[433];
  scGPButton_Ulozit->Caption = F->ls->Strings[24];
  scGPButton_storno->Caption = F->ls->Strings[71];
}

// ---------------------------------------------------------------------------
// nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
void TForm_definice_zakazek::nacti_PP() {
  F->log(__func__); // logování
}

// ---------------------------------------------------------------------------
// vytvoøí defaultní øadek se zakázkou
void TForm_definice_zakazek::predvypln_default_zakazku()
{
  F->log(__func__); // logování
}
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
void TForm_definice_zakazek::uloz_Default_cestu()
{
  F->log(__func__); // logování

}

// ---------------------------------------------------------------------------
// naètení hotových zakázek, tato metoda je volána z FormShow(), pokud existují zakázky
void TForm_definice_zakazek::nacti_zakazky()
{
  F->log(__func__); // logování
  F->d.v.kopirujZAKAZKY2ZAKAZKY_temp();
  // pøekopíruje zakázky do temp zakázek, pro jejich editaci
  // procházení vytvoøených kopií zakázek a vytváøení jejich mGridù
  Cvektory::TZakazka *Z = F->d.v.ZAKAZKY_temp->dalsi;
  Cvektory::TDavka *D = NULL;
  nacitam_zakazky = true; // nutné??
  pocet_zakazek = 0;
	while (Z != NULL)
	{
		loadHeader(Z->n,false);
    pocet_zakazek++;//zvýšení poètu zakázek, které následnì nastaví výšku formuláøe v metodì set_formHW_button_positions
		//vytvoøení tabulky zakázky, nikoliv zakázky a tabulky
		D=Z->davky->dalsi;
		while (D!=NULL)
		{
			vloz_davku(Z,D);
      D=D->dalsi;
		}
		delete D;D=NULL;
		if(Z->davky->dalsi!=NULL)//nastavení disabled ostatních editù u zakázky, nesmí se pøenést na dávky
		{
			Z->mGrid->getEdit(2,2)->Enabled=false;
			Z->mGrid->getEdit(2,3)->Enabled=false;
		}
		// naètení informací ze zakázky do mGridu øešeno tady nebo v metodì loadHeader()
    Z=Z->dalsi;
  }
  // vykreslení
  FormPaint(this);
  // ukazatelové záležitosti
	delete Z;Z=NULL;
  nacitam_zakazky=false; // nutné??
}
// TLAÈÍTKO ULOŽIT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
  F->log(__func__); // logování
  // naètení hodnot z tabulek mGridù do temp zakázek
  // mazání mgridù zakázek, dùležité, kopírování zakázek nesmaže mGridy ale odstraní zakázky temp ukazatel
	if (F->d.v.ZAKAZKY_temp != NULL)
	{
		//naèítání hodnot z tabulky do dat a mazání mGridu
		Cvektory::TZakazka *Z = F->d.v.ZAKAZKY_temp->dalsi;
		while (Z != NULL)
		{
			//naètení hodnot do dávek
			if(Z->davky->dalsi!=NULL)
			{
				Cvektory::TDavka *D=Z->davky->dalsi;
				unsigned int Col=0;
				while(D!=NULL)
				{
					Col=D->n+2;
					D->pocet_voziku=F->ms.MyToDouble(Z->mGrid->Cells[Col][2].Text);
					D->pocet_prazdnych=F->ms.MyToDouble(Z->mGrid->Cells[Col][3].Text);
					D->pocet_celkem=F->ms.MyToDouble(Z->mGrid->Cells[Col][4].Text);
					D=D->dalsi;
				}
				delete D;D=NULL;
			}
			//naètení hodnot do zakázky
			Z->name=Z->mGrid->Cells[1][0].Text;
			Z->pocet_voziku=F->ms.MyToDouble(Z->mGrid->Cells[2][2].Text);
			Z->serv_vozik_pocet=F->ms.MyToDouble(Z->mGrid->Cells[2][3].Text);
			Z->opakov_servis=F->ms.MyToDouble(Z->mGrid->Cells[2][4].Text);
			//smazání mGridu
			Z->mGrid->Delete();
      Z->mGrid = NULL;
      Z = Z->dalsi;
    }
    delete Z;Z=NULL;
  }
  // kopírování temp zakázek do ostrých zakázek
  F->d.v.kopirujZAKAZKY_temp2ZAKAZKY();
	F->d.v.vytvor_default_zakazku();//po kopírování zakázek dojkde ke smazání hlavièky
	F->DuvodUlozit(true);
  // ukonèení formu a smazání temp zakázek
  KonecClick(Sender);
}

// ---------------------------------------------------------------------------
// Zavøení formuláøe (storno a køížek je to samé)
void __fastcall TForm_definice_zakazek::scGPGlyphButton4Click(TObject *Sender)
{
  KonecClick(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::KonecClick(TObject *Sender)
{
	// mazání mgridù zakázek
  if (F->d.v.ZAKAZKY_temp != NULL) {
    Edit_for_Focus->SetFocus();
    Cvektory::TZakazka *Z = F->d.v.ZAKAZKY_temp->dalsi;
    while (Z != NULL) {
      Z->mGrid->Delete();
      Z->mGrid = NULL;
      Z = Z->dalsi;
    }
    delete Z;
    Z = NULL;
	}
	//mazání bmp
	for(int i=scGPImageCollection_layout->Count-1;i>=0;i--)
	{
    scGPImageCollection_layout->Images->Delete(i);
  }

	//ostatní
	Form1->d.v.vymaz_seznam_ZAKAZKY_temp();
	delete(Pan_bmp);//mazání bmp pro posun
	//zmìna záložek
	F->Analyza->Options->NormalColor=F->scGPPanel_mainmenu->FillColor;
	F->Analyza->Options->FrameNormalColor=F->scGPPanel_mainmenu->FillColor;
	F->Schema->Options->NormalColor=F->DetailsButton->Options->NormalColor;
	F->Schema->Options->FrameNormalColor=F->DetailsButton->Options->NormalColor;
  Close();
}

// ---------------------------------------------------------------------------

// zkopíruje pøedchozí
void __fastcall TForm_definice_zakazek::scGPGlyphButton_add_zakazkaClick(TObject *Sender)
{
	loadHeader();
}

// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::FormPaint(TObject *Sender)
{
	////deklarace
	F->log(__func__); //logování
	if(Akce!=BLOK)//nepøekreslovat v nežádoucích pøípadech (napø. posun fomru)
	{
		int width=682;//defaul ClientWidth formu
		int height=283;//defaul ClientHeight formu (pro jednu zakázku), mezera mezi koncem formu a posledním mGridem = 64px
  	max_oblast_mGridu.Left=0;max_oblast_mGridu.Right=0;max_oblast_mGridu.Top=0;max_oblast_mGridu.Bottom=0;
		//vytvoøení bmp
  	Graphics::TBitmap *bmp_total=new Graphics::TBitmap;
  	bmp_total->Width=Form_definice_zakazek->Width;
  	bmp_total->Height=Form_definice_zakazek->Height;

  	//vykreslení podkladní barvy
  	bmp_total->Canvas->Brush->Color = light_gray;
  	bmp_total->Canvas->FillRect(TRect(0,0,bmp_total->Width,bmp_total->Height));
  	////vykreslìní mGridù
  	Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
  	while (Z!=NULL)
  	{
			//dynamické pozicování tabulek, pouze jednou
			if(Z->n==1 && Z->mGrid->Top==-500)Z->mGrid->Top=scGPButton_plan_vyroby->Height+scLabel_header->Height+Z->mGrid->Rows[0].Height;
			if(Z->n!=1)Z->mGrid->Top=Z->predchozi->mGrid->Top+Z->predchozi->mGrid->Height+Z->mGrid->Rows[0].Height;
			//ukládání max oblasti, možná nebude potøeba
			if(Z->n==1){max_oblast_mGridu.Left=Z->mGrid->Left;max_oblast_mGridu.Top=Z->mGrid->Top;}
  		if(Z->dalsi==NULL)max_oblast_mGridu.Bottom=Z->mGrid->Top+Z->mGrid->Height;
  		if(max_oblast_mGridu.Right<Z->mGrid->Left+Z->mGrid->Width)max_oblast_mGridu.Right=Z->mGrid->Left+Z->mGrid->Width;
  		//vykreslení tabulky
  		if(Akce!=PAN_MOVE && Akce!=PAN)Z->mGrid->Show(bmp_total->Canvas);
  		else Z->mGrid->SetVisibleComponents(false);
  		//kontrola zda nenní tabulka mimo obraz, musí být za vykreslením/skrytím tabulek !!!!
  		mGrid_mimo_obraz(Z);
  		//kontrola zda je form dostateènì široký
  		if(5+Z->mGrid->Width+5>width)width=5+Z->mGrid->Width+5;    // 5 náhrada za Z->mGrid->Left
			if(Z->dalsi==NULL && 94+Z->n*Z->mGrid->Height+25*(Z->n-1)+64>(unsigned)height)height=94+Z->n*Z->mGrid->Height+25*(Z->n-1)+64;   //94 = Z->mGrid->Top, 25 = mezera mezi mGridy64 pdsazení od spodní hrany formu
  		Z=Z->dalsi;
  	}
  	delete Z;Z=NULL;

  	////kontrola zda není nutné zvìtšit form
  	if(width>1135)width=1135;
		if(height>883)height=883;
		if(width!=Form_definice_zakazek->ClientWidth || height!=Form_definice_zakazek->ClientHeight)
  	{
  		//upravení rozmìrù formu
  		Form_definice_zakazek->Width=width+2;//+2 rozdíl mezi clientwidth a width
  		Form_definice_zakazek->Height=height+2;//+2 rozdíl mezi clientwidth a height
  		//pozicování buttonu
  		scGPButton_Ulozit->Top=Form_definice_zakazek->Height-scGPButton_Ulozit->Height - 10;
  		scGPButton_storno->Top=scGPButton_Ulozit->Top;
  		scGPButton_Ulozit->Left=Form_definice_zakazek->Width/2-scGPButton_Ulozit->Width-22/2;
  		scGPButton_storno->Left=Form_definice_zakazek->Width/2+22/2;
  		//znovuvykreslìní
			FormPaint(this);//pokud je nutné zvìtšít form, musí se znova spustit formpaint (bmp ma nastavené rozmìry pøed zmìnou)
		}
		else
		{
			if(Akce!=PAN_MOVE && Akce!=PAN)Canvas->Draw(0,0,bmp_total);//finální pøedání bmp_out do Canvasu
			else Canvas->Draw(0,0,Pan_bmp);//zabránìní probliku pøi spouštìní posunu
		}

  	////smazání bmp
  	delete(bmp_total);//velice nutné
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

void TForm_definice_zakazek::getmGridWidth()
{
  F->log(__func__); // logování
}

void TForm_definice_zakazek::setGlyphButtonDefault(unsigned long Row, unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z)
{
  if (typ == krizek_davky)  //pøidat dávku
  {
    F->log(__func__); // logování
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
    J->Options->ShapeStyle = scgpRect;
    // H->Width=30;
    J->Hint = F->ls->Strings[438]; // "Vytvoøit novou dávku"
    J->ShowHint = true;

    J = NULL;
    delete J;
  }
  if (typ == krizek)      //smazat zakázku
  {
    F->log(__func__); // logování
    TscGPGlyphButton *H = Z->mGrid->getGlyphButton(0, Row);
    H->GlyphOptions->Kind = scgpbgkClose;
    H->GlyphOptions->Thickness = 1;
    H->Options->NormalColor = clWhite;
    H->Options->NormalColorAlpha = 255;
    H->Options->FrameWidth = 1;
    H->Options->FrameNormalColor = clWhite;
    H->Options->ShapeStyle = scgpRect;
    H->Hint = F->ls->Strings[439]; // "Smazat zakázku"
    H->ShowHint = true;
    H = NULL;
    delete H;
  }
}
//////////////////////////////////////////////////////////////
void TForm_definice_zakazek::vloz_davku(Cvektory::TZakazka *Z,Cvektory::TDavka *davka)
{
	volno = false;
  // backup pùvodního názvu zakázky pro následný merge bunek a naplnìní
 //	AnsiString nazev = Z->mGrid->Cells[1][0].Text;
	unsigned long Col=Z->mGrid->ColCount-1;
  Edit_for_Focus->SetFocus();
  Z->mGrid->InsertColumn(Col,true, true);
  Z->mGrid->MergeCells(1, 0, Z->mGrid->ColCount-1, 0); // název     - vodorovne
 // Z->mGrid->Cells[1][0].Align=Z->mGrid->LEFT;
  // naplnìní pùvodním textem
//  Z->mGrid->Cells[1][0].Text = nazev;
  // pokud neexistují dávky tak pøi prvním pøidání dávky nastavím typ na glyphbutton
	if (Z->mGrid->ColCount == 4 + 1) Z->mGrid->Cells[Col][1].Type = Z->mGrid->glyphBUTTON;
	if(davka==NULL)
	{
		F->d.v.vloz_davku(Z,0,0,0);//vkládat do dat pouze v pøípadì, že tvoøím novou dávku
		davka=Z->davky->predchozi;
	}
	else//naèítání dat do mGridu
	{
		Z->mGrid->Cells[Col][2].Text = davka->pocet_voziku;
		Z->mGrid->Cells[Col][3].Text = davka->pocet_prazdnych;
		Z->mGrid->Cells[Col][4].Text = davka->pocet_celkem;
	}
	Z->mGrid->Update();
	setGlyphButtonDavka_Add(Z->n, Col + 1); // vytvoøí glyph na pøidání dávky
 	setGlyphButtonDavka_Remove(Col,Z,davka); // nastaví glyph na smazání dávky
	volno = true;
}
//////////////////////////////////////////////////////////
void TForm_definice_zakazek::OnClick(long Tag, long ID, unsigned long Col, unsigned long Row)
{
	F->log(__func__); // logování
  // naètení aktuálnì editované zakázky
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku_z_mGridu(ID);


  if (Col >= 3 && Row == 1 && volno == true)
  {
    TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, Row);
    // VÌTEV NA PØIDÁVÁNÍ DÁVEK DO ZAKÁZKY
    if (J->GlyphOptions->Kind == scgpbgkPlus)
    {
      vloz_davku(Z);
      Z->mGrid->Cells[1][0].Background->Color =Z->mGrid->getGlyphButton(1,1)->Options->NormalColor;
      Z->mGrid->getEdit(1,0)->Options->FocusedColorAlpha=255;
      Z->mGrid->getEdit(1,0)->Options->FocusedColor=Z->mGrid->getGlyphButton(1,1)->Options->NormalColor;
      Z->mGrid->getEdit(1,0)->Options->HotColorAlpha = 255;
      Z->mGrid->getEdit(1,0)->Options->HotColor=Z->mGrid->getGlyphButton(1,1)->Options->NormalColor;
      Z->mGrid->getEdit(1,0)->Options->NormalColor=Z->mGrid->getGlyphButton(1,1)->Options->NormalColor;
     //Z->mGrid->Cells[1][1].TopBorder->Color = Z->mGrid->getGlyphButton(1,1)->Options->NormalColor;
			Z->mGrid->getEdit(2,2)->Enabled=false;
			Z->mGrid->getEdit(2,3)->Enabled=false;
      Z->mGrid->Update();

      //v pøípadì, že pøidám dávku, nastavím na 0 pùvodní hodnoty (situace kdy jsem jel bez dávek)
			if(Z->mGrid->ColCount==5) // pouze ale pøi prvním pøidání dávky, abych vždy nenuloval hodnoty pøi dalším pøidávání dávek
      {
      Z->mGrid->Cells[2][2].Text = "0";
      Z->mGrid->Cells[2][3].Text = "0";
      Z->mGrid->Cells[2][4].Text = "0";
      }

    }
    // VÌTEV NA MAZÁNÍ DÁVEK ZE ZAKÁZKY
    if (J->GlyphOptions->Kind == scgpbgkClose && volno == true)
    {
      volno = false;
      Edit_for_Focus->SetFocus();
      Z->mGrid->DeleteColumn(Col, true);
      Z->mGrid->MergeCells(1, 0, Z->mGrid->ColCount-1, 0); //novy merge názvu
      Z->mGrid->Update();
      volno = true;
			F->d.v.smaz_davku(Z,Col-2);
			//aktualizace èíslování dávek
      OnChange(Tag, ID, Col, Row);
			if(Z->mGrid->ColCount>=4 && Z->davky->predchozi->n>0)
			{
				Cvektory::TDavka *D=Z->davky->dalsi;
				for(unsigned int i=3;i<=Z->mGrid->ColCount-2;i++)
				{
					setGlyphButtonDavka_Remove(i,Z,D);
					D=D->dalsi;
        }
			}
    }
    J = NULL;
    delete J;
  }

  if (Col == 1 && Row == 1)
  {
    TscGPGlyphButton *J = Z->mGrid->getGlyphButton(Col, Row);
    if (J->GlyphOptions->Kind == scgpbgkMore && volno == true)
    {
      Form_color_dialog->SetDialogPosition(1, ID);
      Form_color_dialog->ShowModal();
    }
    J = NULL;
    delete J;
  }
  // odstranìnní tabulky a zakázky
  if (Col == 0 && Row == 0)
  {
    volno = false;
		Edit_for_Focus->SetFocus();
	// ShowMessage(Z->mGrid->scGPImageCollection->Images->Count);
		F->d.v.smaz_temp_zakazku(Z->n);
    pocet_zakazek=F->d.v.ZAKAZKY_temp->predchozi->n; //zjisteni akt.poctu zakázek
    set_formHW_button_positions(); //nové rozmìry formuláøe + pozice buttonu
		FormPaint(this);
    volno = true;
  }
  // odmazání pomocného ukazatele
  //ShowMessage(Z->mGrid->scGPImageCollection->Images->Count);
  Z = NULL;
	delete Z;
}
////////////////////////////////////////////////////////////
void TForm_definice_zakazek::OnChange(long Tag, long ID, unsigned long Col,unsigned long Row)
{

  if (Col >= 2 && Row >= 2 && volno == true)
  {
    Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku_z_mGridu(ID);
    volno = false;
    // update CELKEM v miste zmeny
    Z->mGrid->Cells[Col][4].Text = F->ms.MyToDouble(Z->mGrid->Cells[Col][2].Text) + F->ms.MyToDouble (Z->mGrid->Cells[Col][3].Text);

    // pri pouziti dávek projiti vsech sloupcu a update vsech SUM
    if (Z->mGrid->ColCount >= 3 && Col > 2)
    {
      long pocet = 0;
      long pocet_prazdnych = 0;
      long celkem = 0;
			for (unsigned int i = 3; i < Z->mGrid->ColCount - 1;i++) // prochazeni hodnot po sloupcich
      {
        pocet += F->ms.MyToDouble(Z->mGrid->Cells[i][2].Text);
        pocet_prazdnych += F->ms.MyToDouble(Z->mGrid->Cells[i][3].Text);
        celkem += F->ms.MyToDouble(Z->mGrid->Cells[i][4].Text);
      }
      Z->mGrid->Cells[2][2].Text = pocet; // naplneni CELKEM
      Z->mGrid->Cells[2][3].Text = pocet_prazdnych;
      Z->mGrid->Cells[2][4].Text = celkem;
    }
    Z->mGrid->Update();
    volno = true;
  }

  // pøípad odmazávání dávek
  if(Col>=2 && Row == 1 && volno == true)
  {
      volno = false;
      Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku_z_mGridu(ID);
      long pocet = 0;
      long pocet_prazdnych = 0;
      long celkem = 0;
      for (unsigned int i = 3; i < Z->mGrid->ColCount - 1;i++) // prochazeni hodnot po sloupcich
      {
        pocet += F->ms.MyToDouble(Z->mGrid->Cells[i][2].Text);
        pocet_prazdnych += F->ms.MyToDouble(Z->mGrid->Cells[i][3].Text);
        celkem += F->ms.MyToDouble(Z->mGrid->Cells[i][4].Text);
      }
      Z->mGrid->Cells[2][2].Text = pocet; // naplneni CELKEM
      Z->mGrid->Cells[2][3].Text = pocet_prazdnych;
      Z->mGrid->Cells[2][4].Text = celkem;

    if(Z->mGrid->ColCount == 4) //v pøípadì kdy není už žádná dávka, povolím editaci výchozích sloupcù, která byla pøi využití dávek zakázána
    {
     Z->mGrid->getEdit(2,2)->Enabled=true;
     Z->mGrid->getEdit(2,3)->Enabled=true;
    }
    Z->mGrid->Update();
    volno = true;
  }
}
//////////////////////////////////////////////////////
void TForm_definice_zakazek::setButtonColor(long ID)
{
  F->log(__func__); // logování
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku_z_mGridu(ID);
  Z->mGrid->getGlyphButton(1, 1)->Options->HotColor = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->HotColorAlpha = 255;
  Z->mGrid->getGlyphButton(1, 1)->Options->FocusedColor = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FocusedColorAlpha = 225;
  Z->mGrid->getGlyphButton(1, 1)->Options->NormalColor = Form_color_dialog->scColorGrid1->ColorValue;
  //Z->mGrid->getGlyphButton(1, 1)->Options->FrameColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FrameNormalColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FramePressedColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FrameHotColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getGlyphButton(1, 1)->Options->FrameFocusedColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->Cells[1][1].BottomBorder->Color =Form_color_dialog->scColorGrid1->ColorValue ;
  Z->mGrid->Cells[1][0].Background->Color = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getEdit(1,0)->Options->FocusedColorAlpha=255;
  Z->mGrid->getEdit(1,0)->Options->FocusedColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getEdit(1,0)->Options->HotColorAlpha = 255;
  Z->mGrid->getEdit(1,0)->Options->HotColor= Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->getEdit(1,0)->Options->NormalColor= Form_color_dialog->scColorGrid1->ColorValue;

  Z->mGrid->Cells[1][1].TopBorder->Color = Form_color_dialog->scColorGrid1->ColorValue;
  Z->mGrid->Update();
}

void TForm_definice_zakazek::loadHeader(unsigned long zakazka_n, bool novy)
{
  // if (novy || add_zakazka)
  {
    F->log(__func__); // logování
    Cvektory::TZakazka *Z = NULL;
    if (novy) // vytváøení nové zákázky a jejícho mGridu
    {
      Cvektory::TJig J;
      unsigned int n = 1;
      if (F->d.v.ZAKAZKY_temp != NULL && F->d.v.ZAKAZKY_temp->predchozi->n > 0)
      n = F->d.v.ZAKAZKY_temp->predchozi->n + 1;
      F->d.v.vloz_temp_zakazku("id", 0, "Zakazka " + AnsiString(n), clBlack, 0, 0, J, 0, 0, 0);

      Z = F->d.v.ZAKAZKY_temp->predchozi;
      F->d.v.inicializace_cesty(Z); // vytvoøí hlavièku cesty
      pocet_zakazek=n;
      set_formHW_button_positions();
    }
    else
			Z = F->d.v.vrat_temp_zakazku(zakazka_n);
    // vytváøení mGridu stavající zakázce
    ////vytvoøení mgridu nové zakázce
    Z->mGrid = new TmGrid(this);
    Z->mGrid->Tag = 9;
		Z->mGrid->ID = Z->n;
    Z->mGrid->Create(4, 5);
    Z->mGrid->SetColumnAutoFit(-4);
    // if(novy)
    {
      Z->mGrid->Columns[0].Width = 110;
      Z->mGrid->Columns[1].Width = 80;
      Z->mGrid->Columns[2].Width = 90;
      Z->mGrid->Columns[3].Width = 25;
    }
    ////parametry mgridu
		Z->mGrid->Left = 5;
		if(Z->predchozi->n>0)Z->mGrid->Left = Z->predchozi->mGrid->Left;//pokud došlo k posunu, dojde k zarovnání k ostatním tabulkám

    Z->mGrid->Top = -500;
    // if (Z->predchozi->n > 0) Z->mGrid->Top = Z->predchozi->mGrid->Top + Z->predchozi->mGrid->Height + Z->mGrid->Rows[0].Height;
    // else Z->mGrid->Top = Z->mGrid->Rows[0].Height + scGPButton_plan_vyroby->Height + scLabel_header->Height;



    Z->mGrid->Cells[1][0].Type = Z->mGrid->EDIT;
    Z->mGrid->Cells[1][0].Font->Color=clBlack;
    // nazev  text  - slouèit podélnì
    Z->mGrid->Cells[0][0].Type = Z->mGrid->glyphBUTTON; // X  zakazka
    Z->mGrid->Cells[3][1].Type = Z->mGrid->glyphBUTTON; // X dávka

    Z->mGrid->Cells[0][1].Text = Z->n; // id zakázky
    if (novy)
      Z->mGrid->Cells[1][0].Text = F->ls->Strings[434]; // "Název zakázky"
    else
    Z->mGrid->Cells[1][0].Text = Z->name;
		Z->mGrid->Cells[1][1].Type = Z->mGrid->glyphBUTTON; // color zakazka
    Z->mGrid->Cells[1][0].Font->Size = 15;

    Z->mGrid->Cells[2][1].Font->Size = 14;
    Z->mGrid->Cells[2][1].Text = "Jig";
    Z->mGrid->Cells[1][2].Text = " " + F->ls->Strings[435]; // " poèet"
    Z->mGrid->Cells[1][3].Text = " " + F->ls->Strings[436]; // " prázdných"
    Z->mGrid->Cells[1][4].Text = " " + F->ls->Strings[437]; // " celkem"

    Z->mGrid->Cells[1][2].Align = Z->mGrid->LEFT;
    Z->mGrid->Cells[1][3].Align = Z->mGrid->LEFT;
    Z->mGrid->Cells[1][4].Align = Z->mGrid->LEFT;

    Z->mGrid->Cells[2][2].Align = Z->mGrid->CENTER;
    Z->mGrid->Cells[2][3].Align = Z->mGrid->CENTER;
    Z->mGrid->Cells[2][4].Align = Z->mGrid->CENTER;

    Z->mGrid->Cells[2][2].Type = Z->mGrid->EDIT; //
    Z->mGrid->Cells[2][3].Type = Z->mGrid->EDIT;
		Z->mGrid->Cells[2][4].Type = Z->mGrid->readEDIT; // celkem - suma
		if(novy)
		{
      Z->mGrid->Cells[2][2].Text = "0"; // value
	  	Z->mGrid->Cells[2][3].Text = "0"; // value
			Z->mGrid->Cells[2][4].Text = "0"; // value
		}
		else
		{
			Z->mGrid->Cells[2][2].Text=Z->pocet_voziku;
			Z->mGrid->Cells[2][3].Text=Z->serv_vozik_pocet;
			Z->mGrid->Cells[2][4].Text=Z->opakov_servis;
    }


  // if(Z->mGrid->Cells[1][1].Background->Color!=clWhite) Z->mGrid->Cells[1][1].TopBorder->Color = clWhite;
    // podbarvení sloupcù s "+"
    Z->mGrid->Cells[3][4].Background->Color = light_gray;
    Z->mGrid->Cells[3][4].Text = " ";
    Z->mGrid->Cells[3][3].Background->Color = light_gray;
    Z->mGrid->Cells[3][3].Text = " ";
    Z->mGrid->Cells[3][3].BottomBorder->Color = light_gray;
    Z->mGrid->Cells[3][2].Background->Color = light_gray;
    Z->mGrid->Cells[3][2].Text = " ";
    Z->mGrid->Cells[3][2].BottomBorder->Color = light_gray;

    Z->mGrid->MergeCells(1, 0, 3, 0); // název     - vodorovne
    //Z->mGrid->MergeCells(1, 1, 2, 1); // merge color glyph
    Z->mGrid->MergeCells(0, 2, 0, 4); // merge ID èi obrázek
		//Z->mGrid->Update();
		GetImages(Z);//slouží jak pro naèítání tak pro vkládání nové zakázky, provede Update()
    // default rozmístìní glyphbuttonù
    setGlyphButtonColor(1, 1, color, Z);
    setGlyphButtonDefault(0, 0, krizek, Z);
    setGlyphButtonDefault(1, 2, krizek_davky, Z);
		//Z->mGrid->Cells[1][0].Align=Z->mGrid->LEFT;  // po slouèení pøijdu (add dávka) o text - proto zakomentováno

    ////ukazatelové záležitosti
    Z->mGrid->Refresh();

    Z = NULL;
    delete Z;
    ////vykreslení mGridù
    if (novy)
    FormPaint(this);
    // pouze v pøípadì, že vytváøím novou zakázku s mGridem, pøi naèítání již vytvoøených zakázek musé dojít k vykreslìní až budou všehny naèteny, volaní vykreslení v metodì nacti_zakazky()
  }

}

void __fastcall TForm_definice_zakazek::FormClose(TObject *Sender,TCloseAction &Action)
{
  F->log(__func__); // logování
  // Z->mGrid->Delete();
}
// ---------------------------------------------------------------------------

void TForm_definice_zakazek::setGlyphButtonDavka_Add(unsigned long ID,unsigned long Col)
{
	Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku_z_mGridu(ID);
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
  J->Options->ShapeStyle = scgpRect;
  J->Hint = F->ls->Strings[438]; // "Vytvoøit novou dávku"
  J->ShowHint = true;
  J = NULL;
  delete J;
}

void TForm_definice_zakazek::setGlyphButtonDavka_Remove(unsigned long Col, Cvektory::TZakazka *Z, Cvektory::TDavka *davka)
{
	F->log(__func__); // logování
	TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, 1);
	H->GlyphOptions->Kind = scgpbgkClose;
	H->GlyphOptions->Thickness = 1;
	H->ShowCaption = true;
	H->Caption = " "+F->ls->Strings[440]+" "+AnsiString(davka->n);// " Dávka "
  // " Dávka "
  // H->Font->Color=(TColor)RGB(43,87,154);
	H->Options->NormalColor = clWhite;
	H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
	H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
	H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
	H->Options->NormalColorAlpha = 255;
	H->Options->FrameWidth = 1;
// H->Width=  Z->mGrid->Columns[2].Width;
//  H->Height=Z->mGrid->Rows[1].Height;
	H->Options->FrameNormalColor = clWhite;
  H->Options->ShapeStyle = scgpRect;
  H->Hint = F->ls->Strings[441]; // "Smazat dávku";
	H->ShowHint = true;
	H = NULL;
	delete H;
}

void TForm_definice_zakazek::setGlyphButtonColor(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z)
{
  F->log(__func__); // logování
  TscGPGlyphButton *H = Z->mGrid->getGlyphButton(Col, Row);
  H->GlyphOptions->Kind = scgpbgkMore;
  H->GlyphOptions->Thickness = 1;
  H->ShowCaption = false;
 // H->Caption = "  ";
  H->Options->NormalColor = clWhite;
  H->Options->FontNormalColor = (TColor)RGB(43, 87, 154);
  H->Options->FontFocusedColor = (TColor)RGB(43, 87, 154);
  H->Options->FontHotColor = (TColor)RGB(43, 87, 154);
  H->Options->NormalColorAlpha = 255;
  H->Options->FrameWidth = 1;
  H->Options->FrameNormalColor = clWhite;
  H->Options->ShapeStyle = scgpRect;
  H->Hint = F->ls->Strings[442]; //"Barva zakázky"
  H->ShowHint = true;
  H = NULL;
  delete H;
}

void TForm_definice_zakazek::set_formHW_button_positions()
{
//  Form_definice_zakazek->Height = pocet_zakazek *150  + 100 + scGPButton_Ulozit->Height; //150px vyska mgridu 100 - hlavicky + buttony kolem
//  scGPButton_Ulozit->Top= Form_definice_zakazek->Height - scGPButton_Ulozit->Height - 10;
//  scGPButton_storno->Top=scGPButton_Ulozit->Top;
//  if(pocet_zakazek>=3) Form_definice_zakazek->Top=50;
}

void TForm_definice_zakazek::GetImages(Cvektory::TZakazka *Z)
{
	Z->mGrid->scGPImageCollection=scGPImageCollection_layout;
	Z->mGrid->scGPImageCollection->Images->Add()->Bitmap=F->d.nacti_nahled(Z->n);
  Z->mGrid->Cells[0][2].Type=Z->mGrid->IMAGE;
	Z->mGrid->Cells[0][2].ImageIndex=Z->n-1;   //dynamicky plnit
	Z->mGrid->Update();
	Z->mGrid->getImage(0,2)->Stretch=true;
	Z->mGrid->Cells[0][2].Align=Z->mGrid->LEFT;
	Z->mGrid->Cells[0][2].Valign=Z->mGrid->TOP;
}
void __fastcall TForm_definice_zakazek::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	vychozi_souradnice_kurzoru=TPoint(X,Y);//výchozí souøadnice
	Posun.x=0;Posun.y=0;
	//zapnutí posunu
	if(Akce==NIC)
	{
		Akce=PAN;
		pan_non_locked=true;
		F->kurzor(F->pan_move);
		Akce=PAN_MOVE;//pøepne z PAN na PAN_MOVE
		//FormPaint(this);
		pan_create();//vytvoøí výøez pro pan_move
		Posun_predchozi=TPoint(0,0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
  akt_souradnice_kurzoru_PX=TPoint(X,Y);
	switch(Akce)
	{
		case PAN_MOVE:
		{
			//výpoèet nového posunu
			Posun.x=(akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x);
			Posun.y=(akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y);
			//zajišuje vázání posunu na hrany formu, upravý Posun
			vazat_posun();
			//samotný posun bmp
			pan_map(Canvas,vychozi_souradnice_kurzoru.x+Posun.x,vychozi_souradnice_kurzoru.y+Posun.y);
			break;
		}
		default:break;
	}
}
//---------------------------------------------------------------------------
//upravý globální promìnnou posun, tak aby byl posun vázaný, tz. nelze posunout pravý konec tabulky dál než 5px od okraje formu
void TForm_definice_zakazek::vazat_posun()
{
	//vázání na Top
	if(max_oblast_mGridu.Top+Posun.y>=94)//kontrola, zda se snazím jít pøes hranici
	{
		if(Posun_predchozi.x==0 && Posun_predchozi.y==0)//kontrola, zda se to stalo poprvé
		{
			Posun.y-=max_oblast_mGridu.Top+Posun.y-94;//upravení posunu, rychlím pohybem kurzoru se lze dostat za hranici, nutnost pøizpùsobit Posun pøesnì na hranici
			Posun_predchozi.y=Posun.y;//uložení Posunu pro další použití
		}
		else Posun.y=Posun_predchozi.y;//poøád se snažím pøekonat stejnou hranici, nastavování posunu na pøedchozí
	}
	else Posun_predchozi=TPoint(0,0);//nepøekraèuji hranici, vynulování pøedchozího popsunu

	//vázání na Bottom
	if(max_oblast_mGridu.Bottom+Posun.y<=scGPButton_Ulozit->Top-20)
	{
		if(Posun_predchozi.x==0 && Posun_predchozi.y==0)
		{
			Posun.y-=max_oblast_mGridu.Bottom+Posun.y-(scGPButton_Ulozit->Top-20);
			Posun_predchozi.y=Posun.y;
		}
		else Posun.y=Posun_predchozi.y;
	}
	else Posun_predchozi=TPoint(0,0);

	//vázání na Left
	if(max_oblast_mGridu.Left+Posun.x>=5)
	{
		if(Posun_predchozi.x==0 && Posun_predchozi.y==0)
		{
			Posun.x-=max_oblast_mGridu.Left+Posun.x-5;
			Posun_predchozi.x=Posun.x;
		}
		else Posun.x=Posun_predchozi.x;
	}
	else Posun_predchozi=TPoint(0,0);

	//vázání Right
	if(max_oblast_mGridu.Right+Posun.x<=Form_definice_zakazek->ClientWidth-5)
	{
		if(Posun_predchozi.x==0 && Posun_predchozi.y==0)
		{
			Posun.x-=max_oblast_mGridu.Right+Posun.x-(Form_definice_zakazek->ClientWidth-5);
			Posun_predchozi.x=Posun.x;
		}
		else Posun.x=Posun_predchozi.x;
	}
	else Posun_predchozi=TPoint(0,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if(Button==mbLeft)//zohlední jenom stisk levého tlaèítka myši
	{
		if(pan_non_locked){pan_non_locked=false;Akce=NIC;F->kurzor(F->standard);pan_move_map();}
		switch(Akce)
		{
			case PAN_MOVE://vratí z PAN_MOVE na PAN, funkcionalita mezerník
			{
				F->kurzor(F->pan);
				Akce=PAN;
				pan_move_map();
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//vytvoøí výøez pro pan_move
void TForm_definice_zakazek::pan_create()
{
	F->log(__func__);//logování
	//vypnutí všech prvkù, které by se jinak promítly do bmp
	scGPButton_Ulozit->Visible=false;
	scGPButton_storno->Visible=false;
	scGPGlyphButton_add_zakazka->Visible=false;
	//vytvoøení bmp
	Pan_bmp->Width=ClientWidth;Pan_bmp->Height=ClientHeight;//velikost pan plochy
	Pan_bmp->Canvas->CopyRect(Rect(0,0,ClientWidth,ClientHeight),Canvas,Rect(0,0,ClientWidth,ClientHeight));//uloží pan výøez
	FormPaint(this);
	//zakrytí panelu v bmp, pokud by byly bmp menší, pøi posunu bílí pruh, musí mít stejnou velikost jako form
	Pan_bmp->Canvas->Brush->Color=light_gray;
	Pan_bmp->Canvas->FillRect(TRect(0,0,Form_definice_zakazek->Width,scGPPanel2->Height));
	//zobrazení skrytých
	scGPButton_Ulozit->Visible=true;
	scGPButton_storno->Visible=true;
	scGPGlyphButton_add_zakazka->Visible=true;
}
//---------------------------------------------------------------------------
//Posouvá výøez mapy pøi stisknutém mezerníku a L-myši
void TForm_definice_zakazek::pan_map(TCanvas * canv, int X, int Y)
{
	F->log(__func__);//logování
	////zajištìní skrytí komponent, vedlejší produkt metody d.vykresli_mGridy();, protože má v sobì podmínky pøi pan_move
	//if(akt_Objekt!=NULL)if(akt_Objekt->zobrazit_mGrid)d.vykresli_mGridy();

	////vykreslení aktuální pan_bmp
	canv->Brush->Color=light_gray;canv->Brush->Style=bsSolid;
	//maže pøi posouvání obrazu starý obraz
	canv->FillRect(TRect(0,0,ClientWidth,Y-vychozi_souradnice_kurzoru.y));//horní okraj
	canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height));//levy okraj
	canv->FillRect(TRect(X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,Y-vychozi_souradnice_kurzoru.y,ClientWidth,ClientHeight));//pravy okraj
	canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height,X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,ClientHeight));//dolní okraj
	//samotné posouvání Pan_bmp
	canv->Draw(X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y,Pan_bmp);
}
//---------------------------------------------------------------------------
//realizuje posunutí obrazu
void TForm_definice_zakazek::pan_move_map()
{
	F->log(__func__);//logování
	Cvektory::TZakazka *Z=F->d.v.ZAKAZKY_temp->dalsi;
	while(Z!=NULL)
	{
		Z->mGrid->Left+=Posun.x;
		Z->mGrid->Top+=Posun.y;
		Z=Z->dalsi;
	}
	delete Z;Z=NULL;
	FormPaint(this);
}
//---------------------------------------------------------------------------
//koleèko dolù
void __fastcall TForm_definice_zakazek::FormMouseWheelDown(TObject *Sender, TShiftState Shift,TPoint &MousePos, bool &Handled)
{
	F->log(__func__);//logování
	if(jedno_ze_tri_otoceni_koleckem_mysi%2!=0)//Velice nutná konstrukce kvùli špatnì fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
		if(Akce==NIC)
		{
			Akce=PAN;
			pan_create();
			FormPaint(this);//nutné
			Posun.x=0;Posun.y=0;
			vychozi_souradnice_kurzoru=TPoint(0,0);
		}
		if(funkcni_klavesa==0)Posun.y+=20;
		else Posun.x-=20;
		vazat_posun();
		pan_map(Canvas,Posun.x,Posun.y);
	}
	jedno_ze_tri_otoceni_koleckem_mysi++;
	if(jedno_ze_tri_otoceni_koleckem_mysi>4)jedno_ze_tri_otoceni_koleckem_mysi=1;
	doba_neotaceni_mysi=0;TimerMouseWheel->Enabled=true;
}
//---------------------------------------------------------------------------
//koleèko nahoru
void __fastcall TForm_definice_zakazek::FormMouseWheelUp(TObject *Sender, TShiftState Shift,TPoint &MousePos, bool &Handled)
{
	F->log(__func__);//logování
	if(jedno_ze_tri_otoceni_koleckem_mysi%2!=0)//Velice nutná konstrukce kvùli špatnì fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
		if(Akce==NIC)
		{
			Akce=PAN;
			pan_create();
			FormPaint(this);//nutné
			Posun.x=0;Posun.y=0;
			vychozi_souradnice_kurzoru=TPoint(0,0);
		}
		if(funkcni_klavesa==0)Posun.y-=20;
		else Posun.x+=20;
		vazat_posun();
		pan_map(Canvas,Posun.x,Posun.y);
	}
	jedno_ze_tri_otoceni_koleckem_mysi++;
	if(jedno_ze_tri_otoceni_koleckem_mysi>4)jedno_ze_tri_otoceni_koleckem_mysi=1;
	doba_neotaceni_mysi=0;TimerMouseWheel->Enabled=true;
}
//---------------------------------------------------------------------------
//slouží k ukonèení posuvu bmp a posunu mGridù
void __fastcall TForm_definice_zakazek::TimerMouseWheelTimer(TObject *Sender)
{
	F->log(__func__);//logování
	if(++doba_neotaceni_mysi==3)
	{
		if(Akce==PAN)
		{
			Akce=NIC;
			pan_move_map();
		}
		TimerMouseWheel->Enabled=false;
		jedno_ze_tri_otoceni_koleckem_mysi=1;
	}
}
//---------------------------------------------------------------------------
//odchytává stisknuté klávesy
void __fastcall TForm_definice_zakazek::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	//detekce stisknutí funkèní klávesy
	funkcni_klavesa=0;
	if(Shift.Contains(ssShift) && Shift.Contains(ssCtrl)){funkcni_klavesa=3;}//SHIFT + CTRL
	else
	{
		if(Shift.Contains(ssShift)){funkcni_klavesa=2;}//SHIFT
		if(Shift.Contains(ssCtrl)){funkcni_klavesa=1;}//CTRL
	}
}
//---------------------------------------------------------------------------
//slouží pro odmazávání funkèní klávesy po jejím puštìní
void __fastcall TForm_definice_zakazek::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	funkcni_klavesa=0;
}
//---------------------------------------------------------------------------
//testování skrývání komponent
void TForm_definice_zakazek::mGrid_mimo_obraz(Cvektory::TZakazka *Z)
{
	//uvedení do pùvodního stavu
	//Z->mGrid->VisibleComponents=1;
	//kontrola + skrytí
	Z->mGrid->VisibleComponents=1;
	if(Akce==NIC && Z->mGrid->Top<scGPPanel2->Height)//pokud je mgrid v pøekryvu, nejdøíve obecná podmínka
	{
		unsigned int pocet_radku=Ceil((double)(scGPPanel2->Height-Z->mGrid->Top)/(double)Z->mGrid->DefaultRowHeight);
		Z->mGrid->VisibleComponents=-1;
		for(unsigned int i=0;i<=pocet_radku-1;i++)
		{
			for(unsigned int j=0;j<=Z->mGrid->ColCount-1;j++)
			{
				if(Z->mGrid->Cells[j][i].Type!=Z->mGrid->DRAW)Z->mGrid->SetVisibleComponent(j,i,false);
			}
		}
	}
}
//---------------------------------------------------------------------------
/////zabránìní spouštìní FormPaint pøi posunu formu
void __fastcall TForm_definice_zakazek::scLabel_headerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	Akce=BLOK;//blok pøi zahájení posnunu formu
}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::scLabel_headerMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	Akce=NIC;//uvolnìní akce po posunu
	FormPaint(this);//pøekreslení, nìkdy pøi velkém pøesunu mùže dojít k deformaci vykreslení tabulky, proto pøekreslení po ukonèení pøesunu
}
//---------------------------------------------------------------------------

