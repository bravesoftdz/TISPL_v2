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
#include "TmGrid.h"
#include "katalog_dopravniku.h"
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
#pragma link "scGPImages"
#pragma link "scStyledForm"
#pragma link "scImageCollection"
#pragma link "scGPMeters"
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
	input_state=NO;
	////designov� z�le�itosti
 //	Form_parametry_linky->Color=F->m.clIntensive((TColor)RGB(43,87,154),10);//RGB(240,240,240); //nastaven� barvy formul��e

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


}
//---------------------------------------------------------------------------
void TForm_parametry_linky::pasiveColor()//nastav� v�echny polo�ky pop-up na pasivn� resp. default barvu
{
  F->log(__func__); //logov�n�
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
 // scGPGlyphButton_ADD->Options->NormalColor=(TColor)RGB(11,221,35);
 // scGPGlyphButton_ADD->Options->NormalColorAlpha=200;
  //scGPSwitch->ThumbColor= scGPGlyphButton_ADD->Options->NormalColor;
//  scGPSwitch->ThumbOnColor=scGPSwitch->ThumbColor;
  scGPSwitch->ThumbColorAlpha= 200;//scGPGlyphButton_ADD->Options->NormalColorAlpha;
  scGPSwitch->ThumbOnColorAlpha=200;//scGPSwitch->ThumbColorAlpha;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{
    F->log(__func__); //logov�n�
    input_state=LOADING;
    COL=0; ROW=0;
		Form_parametry_linky->Color=F->m.clIntensive((TColor)RGB(43,87,154),10);
    //F->scStyledForm1->ShowClientInActiveEffect();
   // scHTMLLabel1->Caption="Ahojky - <bgcolor =clWhite>[mm]</bgcolor>";
     scGPTrackBar_uchyceni->Top=196;
     scGPTrackBar_uchyceni->Left=614;


    Cvektory::Ttyp_dopravniku *K=F->d.v.vrat_typ_dopravniku(F->d.v.PP.katalog);
    scGPGlyphButton_katalog->Caption=K->name+", r�dius "+F->d.v.PP.radius*1000.0 +" mm";

  	if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")
    {  //budu p�ev�d�t na m/min
    aRDunit=MIN;
    F->aRDunit=F->MIN;
    }
    else F->aRDunit=F->SEC;

    if(Form1->readINI("nastaveni_form_parametry", "DM") == "1")
    {  //budu p�ev�d�t na metry - rozestup, dle nastaven�ch jednotek mezery na PO zobraz�m rozestup na PL
    Dmunit=MM;
    }
    if(Form1->readINI("nastaveni_form_parametry_linky", "R") == "1")
    {  //budu p�ev�d�t na metry - roztec, dle nastaven�ch jednotek mezery na PO zobraz�m rozestup na PL
    Runit=MM;
    }
    if(Form1->readINI("nastaveni_form_parametry_linky", "TT") == "1")
    {  //budu p�ev�d�t na metry - rozestup, dle nastaven�ch jednotek mezery na PO zobraz�m rozestup na PL
    Taktunit=MIN;
    rHTMLLabel_takt->Caption="TaktTime <font color=#2b579a>[m] </font>";
    }
    if(Form1->readINI("nastaveni_form_parametry_linky", "rozmery") == "1")
    {  //budu p�ev�d�t na metry - rozestup, dle nastaven�ch jednotek mezery na PO zobraz�m rozestup na PL
    Delkaunit=MM;
		rHTMLLabel_delka_jig->Caption="<font color=#2b579a>[mm]</font>";
		rHTMLLabel_sirka_jig->Caption="<font color=#2b579a>[mm]</font>";
    rHTMLLabel_vyska_jig->Caption="<font color=#2b579a>[mm]</font>";
    rHTMLLabel_delka_podvozek->Caption="<font color=#2b579a>[mm]</font>";

     //nahr�n� hodnot / bud v MM nebo M

    }
     //Runit=MM;

      scGPNumericEdit_delka_jig->Value=Form1->d.v.PP.delka_jig*(1+999*Delkaunit);
			scGPNumericEdit_sirka_jig->Value=Form1->d.v.PP.sirka_jig*(1+999*Delkaunit);
			scGPNumericEdit_delka_podvozek->Value=Form1->d.v.PP.delka_podvozek*(1+999*Delkaunit);
      scGPNumericEdit_vyska_jig->Value=Form1->d.v.PP.vyska_jig*(1+999*Delkaunit);

      scGPTrackBar_uchyceni->MaxValue=Form1->d.v.PP.delka_podvozek*(1+999*Delkaunit);
      scGPTrackBar_uchyceni->Value=Form1->d.v.PP.uchyt_pozice*(1+999*Delkaunit);
      scGPTrackBar_uchyceni->Hint=scGPTrackBar_uchyceni->Value;

     // ShowMessage(Form1->d.v.PP.uchyt_pozice);


		clBACKGROUND=(TColor)RGB(240,240,240);//F->m.clIntensive((TColor)RGB(128,128,128),115);//(250,250,250);

   	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
  vypis(""); // prozmanuti vypisu - pro sicher
	mGrid->Tag=7;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
  mGrid->ID=0;
	mGrid->Left=scGPButton_pohon->Left;
  mGrid->Top=scGPButton_pohon->Top+scGPButton_pohon->Height ;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->Border.Width=1;
  mGrid->DefaultRowHeight=28; //v�t�� v��ka ��dku, kv�li velikosti comba - aby se ve�lo cel�
	//mGrid->DefaultCell.Font->Size=14;
	//mGrid->DefaultCell.Font->Name="Roboto";
  mGrid->Create(9,2);//samotn� vytvo�en� matice-tabulky
 // mGrid->Border.Color=(TColor)RGB(240,240,240);

  getmGridWidth();

	mGrid->SetColumnAutoFit(-4);
  scHTMLLabel_jig_info->Font->Color=F->m.clIntensive(clBlack,50);

//	Form_parametry_linky->Width=scPanel_takt->Width + scPanel_vozik->Width + scPanel_takt->Left + 20;//mGrid->Width + scGPButton_pohon->Left + 30;

	////////pln�n� daty - hlavi�ka////////
	mGrid->Cells[0][0].Text="ID";
 	mGrid->Cells[1][0].Text="N�zev";
 if(aRDunit==MIN)	mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/min]</a>";
 else             mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/s]</a>";
  mGrid->Cells[3][0].Text="";
  mGrid->Cells[4][0].Text="";
 mGrid->Cells[5][0].Text="Rozte� palce";
 if(Runit==MM) mGrid->Cells[5][1].Text="<a>[mm]</a>";
 else          mGrid->Cells[5][1].Text="<a>[m]</a>";
  mGrid->Cells[6][0].Text="Pou��v�n - na objektech";
  mGrid->Cells[7][0].Text="";
  mGrid->Cells[8][0].Text="";

  mGrid->Cells[7][0].Align=mGrid->LEFT;

  mGrid->Cells[0][1].Text="";
	mGrid->Cells[1][1].Text="";
	mGrid->Cells[2][1].Text="od";
  mGrid->Cells[3][1].Text="do";
  mGrid->Cells[4][1].Text="aktu�ln�";
//  mGrid->Cells[5][1].Text="";
  mGrid->Cells[6][1].Text="";


	getmGridColors();

 	mGrid->MergeCells(2,0,4,0);
	mGrid->MergeCells(0,0,0,1);
  mGrid->MergeCells(1,0,1,1);
//  mGrid->MergeCells(5,0,5,1);
  mGrid->MergeCells(6,0,7,0);
  mGrid->MergeCells(6,1,7,1);
  mGrid->MergeCells(6,0,6,1);
  mGrid->MergeCells(7,0,7,1);
  mGrid->MergeCells(8,0,8,1);


   rHTMLLabel_InfoText->Caption="";
   rHTMLLabel_InfoText->Top = Button_storno->Top - Button_storno->Height + 5;
   rHTMLLabel_info_zmenaR->Top= Button_storno->Top - Button_storno->Height + 5;
   rHTMLLabel_info_zmenaR->Left=Button_storno->Left;
   rHTMLLabel_info_zmenaR->Caption="";
	 //Nastav_zamky(empty_klik_ico,empty_klik);
	 vypis(""); VID=-1;

	 //povolen� vstup� do zm�ny TT a voz�ku + Ulo�it + Storno button
		scGPGlyphButton_vozik_edit->Enabled=true;
		scGPGlyphButton_TT->Enabled=true;
		Button_storno->Enabled=true;
		Button_save->Enabled=true;

		scExPanel_doporuc_pohony->Visible=false;
		PopUPmenu->Visible=false;


    if(rEditNum_takt->Enabled) rEditNum_takt->SetFocus();
    else Button_save->SetFocus();
		zobrazOramovani=false;

		if(scGPSwitch->State==0)
    {
		rImageEx_jig_podlahovy->Visible=true;
    rImageEx_jig_podvesny->Visible=false;
    }
		else
    {
     rImageEx_jig_podvesny->Visible=true;
     rImageEx_jig_podlahovy->Visible=false;
     }
    nastav_edity();//nastav� edity

		if(Form1->d.v.navrhni_POHONY()=="")
		{
			scGPButton_doporucene->Visible=false;
			rHTMLLabel_doporuc_pohony->Caption=""; // neexistuj� ��dn� objekty -> neum�m spo��tat doporu�. rychlosti
		}
		else
		{
			scGPButton_doporucene->Visible=false;
			rHTMLLabel_doporuc_pohony->Caption="Navr�en� pohony pro objekty bez p�i�azen�ch pohon�:";
		}

		if(Form1->STATUS==Form1->NAVRH)    //Architekt
		{
			//scGPButton_vozik->Caption="   Voz�k";
			//scGPButton_vozik->ImageIndex=21;
			//scGPButton_obecne->Caption="   Takt time";
			rHTMLLabel_takt->Visible=true;
			rEditNum_takt->Visible=true;
			rHTMLLabel_sirka_jig->Visible=true;
		}
		else          //Klient
		{
			//scGPButton_vozik->Caption="   Jig";
			//scGPButton_vozik->ImageIndex=19;
			//scGPButton_pohon->Caption="   Pohon";
		 //	rHTMLLabel_sirka_jig->Visible=false;

			scGPButton_obecne->Visible=false;
			rHTMLLabel_takt->Visible=false;
			rEditNum_takt->Visible=false;
		}

       //defaultn� povolen� vstup� do edit� TT a rozm�r� voz�ku, pokud m� b�t vstup do edit� zak�z�n - je to o�et�eno
       //v na�ti pohony
       rEditNum_takt->Enabled=true;
       scGPNumericEdit_delka_jig->Enabled=true;
       scGPNumericEdit_sirka_jig->Enabled=true;
       scGPNumericEdit_vyska_jig->Enabled=true;
       scGPNumericEdit_delka_podvozek->Enabled=true;

		nacti_pohony();

		if(!data_nalezena)
		{
      mGrid->RowCount=2; //slou�en� hlavi�ka jsou 2 ��dky
      scGPGlyphButton_DEL_nepouzite->Visible=false;

      mGrid->Columns[7].Width=190+30;
      mGrid->Columns[8].Width=1;

		}


    //nastaveni rozmeru formu - dle poctu pohonu
     setFormHeight();

		//pro vytvo�en� z�lohy zru�en�ch p���azen� - vyfikundace z d�vodu mo�n�ho storna
		//mus� b�t um�st�no a� za nacti_pohony
		zrusena_prirazeni_PID_size=mGrid->RowCount-1;//velikost sta�� jako po�et ��dk�/pohonu po na�ten�, v�ce jich b�t p�i�azeno do nov�ho na�ten� formu b�t nem��e
		zrusena_prirazeni_PID=new bool[zrusena_prirazeni_PID_size];
		for(unsigned int PID=0;PID<zrusena_prirazeni_PID_size;PID++)zrusena_prirazeni_PID[PID]=false;


	// rStringGridEd_tab_dopravniky->Columns->Items[0]->Visible=false;

	 scHTMLLabel_doporuc_pohony->Color=(TColor)RGB(230,230,230);

	 scGPButton_vozik->Options->NormalColor=Button_save->Options->NormalColor;
	 scGPButton_vozik->Options->FocusedColor=Button_save->Options->NormalColor;
	 scGPButton_vozik->Options->HotColor=Button_save->Options->NormalColor;
	 scGPButton_vozik->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_vozik->Options->FrameNormalColor=Button_save->Options->NormalColor;
	 scGPButton_vozik->Options->FramePressedColor=Button_save->Options->NormalColor;

	 scGPButton_pohon->Options->NormalColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->FocusedColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->HotColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->FrameNormalColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_pohon->Options->FramePressedColor=Button_save->Options->NormalColor;


	 scGPButton_obecne->Options->NormalColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->FocusedColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->HotColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->FrameNormalColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_obecne->Options->FramePressedColor=Button_save->Options->NormalColor;

	 scGPButton_jig->Options->NormalColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->FocusedColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->HotColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->FrameNormalColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->PressedColor=Button_save->Options->NormalColor;
	 scGPButton_jig->Options->FramePressedColor=Button_save->Options->NormalColor;

	// rStringGridEd_tab_dopravniky->Left=1;
 //	 rStringGridEd_tab_dopravniky->Width=Form_parametry_linky->Width-2;

    rHTMLLabel_podlahovy->Color=Form_parametry_linky->Color;
    rHTMLLabel_podvesny->Color=rHTMLLabel_podlahovy->Color;

	 scGPButton_doporucene->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton_doporucene->Options->FrameNormalColor=Form_parametry_linky->Color;


	 if(Form1->d.v.PP.typ_linky==0) scGPSwitch->State=scswOff;
	 else  { scGPSwitch->State=scswOn; }
	 //scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;

  // rEditNum_takt->

	 if(Taktunit==MIN)
	 {
	 rEditNum_takt->Value=Form1->d.v.PP.TT/60.0;
	 } else {rEditNum_takt->Value=Form1->d.v.PP.TT;}

    rEditNum_takt->SelStart=6;//F->ms.MyToDouble(rEditNum_takt->Value.Length());//6; //pozice kurzoru

	 //pozice info tla��tka - asi je tla��tko stejn� provizorn�
	 pozice_scGPGlyphButton_hint();

   setADD_ButtonPosition();

  mGrid->Note.margin_left=5;
  mGrid->Note.margin_right=0;
  mGrid->Note.margin_bootom=4;
  mGrid->Note.margin_top=1;

	 Storno=false;

	 //nastaven� rozlo�en� komponent
	 int odsazeni=Form_parametry_linky->Height-Button_save->Top-Button_save->Height;
	 //LEFT
	 scGPButton_obecne->Left=odsazeni;
	 scPanel_takt->Left=odsazeni;
	 scGPButton_pohon->Left=odsazeni;
	 mGrid->Left=odsazeni;
	 scGPGlyphButton_ADD->Left=odsazeni;
	 scGPButton_vozik->Left=scPanel_takt->Left+scPanel_takt->Width+odsazeni;
	 scPanel_vozik->Left=scGPButton_vozik->Left;
	 Form_parametry_linky->Width=odsazeni+mGrid->Width+odsazeni+1;//mus� se ru�n� p�idat jeden pixel!
	 //TOP
	 scGPButton_obecne->Top=scGPPanel2->Height+odsazeni;
	 scPanel_takt->Top=scGPButton_obecne->Top+scGPButton_obecne->Height;
	 scGPButton_pohon->Top=scPanel_takt->Top+scPanel_takt->Height+odsazeni;
	 mGrid->Top=scGPButton_pohon->Top+scGPButton_pohon->Height;
	 scGPGlyphButton_ADD->Top=mGrid->Top+mGrid->Height + 1;
	 scGPButton_vozik->Top=scGPButton_obecne->Top;
	 scPanel_vozik->Top=scPanel_takt->Top;
	 rHTMLLabel_podlahovy->Top=scGPButton_vozik->Top+scGPButton_vozik->Height/2-rHTMLLabel_podlahovy->Height/2;
	 scGPSwitch->Top=rHTMLLabel_podlahovy->Top;
	 rHTMLLabel_podvesny->Top=rHTMLLabel_podlahovy->Top;
	 //TOP uvnit�
	 scImage1->Top=scPanel_takt->Height/2-scImage1->Height/2;
	 rEditNum_takt->Top=scPanel_takt->Height/2-rEditNum_takt->Height/2+1;
	 rHTMLLabel_takt->Top=scPanel_takt->Height/2-rHTMLLabel_takt->Height/2+1;
	 //Left uvnit�
	 scImage1->Left=40+6;//scImage1->Left=40 nelze pou��t scImage1->Left proto zm��eno a p�id�no ru�n� 40
	 rEditNum_takt->Left=169;
	 rHTMLLabel_takt->Left=rHTMLLabel_takt->Left+6;
	 //obr�zky
	 rImageEx_jig_podlahovy->Left=scPanel_vozik->Width/2-rImageEx_jig_podlahovy->Width/2;
	 rImageEx_jig_podvesny->Left=scPanel_vozik->Width/2-rImageEx_jig_podvesny->Width/2;
	 //design tla��tek
	 Form1->m.designButton(Button_save,Form_parametry_linky,1,2);
	 Form1->m.designButton(Button_storno,Form_parametry_linky,2,2);
	 //vycentrov�n� formul��e, mus� b� na konci!
	 Form_parametry_linky->Left=Form1->Left+Form1->Width/2-Form_parametry_linky->Width/2;
	 Form_parametry_linky->Top=Form1->Top+Form1->Height/2-Form_parametry_linky->Height/2;
	 //nastaven� jednotek v panelu voz�k
	 vozik_zmena_jednotek();
	 //zapnut� tla��tka add, v p��pad� chybn� validace (tla��tko skryto) a n�sledn�mu stisku storno a znovu otev�en� PL, z�stal button skryt
	 scGPGlyphButton_ADD->Visible=true;
   scGPGlyphButton_katalog->Top=scGPButton_pohon->Top;
   scGPGlyphButton_katalog->Left=scGPButton_pohon->Left + scGPButton_pohon->Width;
}
//---------------------------------------------------------------------------
//
void TForm_parametry_linky::nacti_pohony ()
{
   F->log(__func__); //logov�n�
	 data_nalezena=false;
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;
	 if (ukaz!=NULL)
	 {
				mGrid->RowCount = Form1->d.v.POHONY->predchozi->n + 2;
        mGrid->Refresh();  //nutn�
				data_nalezena=true; //pokud jsou ve spojaku nejaka data, nastavit na true
				 for (unsigned int i=2;i<mGrid->RowCount;i++)
				 {
						 OBJEKTY_POUZIVAJICI_POHON=Form1->d.v.vypis_objekty_vyuzivajici_pohon(ukaz->n);
 						if(OBJEKTY_POUZIVAJICI_POHON!="")
            {
            if(OBJEKTY_POUZIVAJICI_POHON.Length()>15) mGrid->Cells[7][i].Text=OBJEKTY_POUZIVAJICI_POHON.SubString(1,20)+"...";
            else mGrid->Cells[7][i].Text=OBJEKTY_POUZIVAJICI_POHON;

            }
						else mGrid->Cells[7][i].Text="";

    				mGrid->Cells[0][i].Text = ukaz->n;
						mGrid->Cells[1][i].Text = ukaz->name;

						 mGrid->Cells[2][i].Text = ukaz->rychlost_od*(1+59.0*aRDunit);

						if(ukaz->rychlost_do==0)   mGrid->Cells[3][i].Text ="";
						else mGrid->Cells[3][i].Text = ukaz->rychlost_do*(1+59.0*aRDunit);

						if(ukaz->aRD==0)  	mGrid->Cells[4][i].Text = "";
						else mGrid->Cells[4][i].Text = ukaz->aRD*(1+59.0*aRDunit);

            mGrid->Cells[5][i].Type=mGrid->COMBOEDIT;
            mGrid->Refresh();   //kv�li pr�ci s combem je nutn� refresh po nastaven� na typ COMBOEDIT
            TscGPComboEdit *C=mGrid->getComboEdit(5,i);
            TscGPListBoxItem *I;

           //na�ten� hodnoty rozte�e do roletky + nastaven� jako ItemIndex=0
          // ShowMessage(ukaz->roztec);
            I=C->Items->Add();
						if(ukaz->roztec==0) C->ItemIndex=-1;
						if(Runit==MM) { I->Caption = ukaz->roztec*(1+999.0*Runit); C->ItemIndex=0; }
						else  { I->Caption = ukaz->roztec; C->ItemIndex=0; }

           //  ShowMessage(Runit);
          // na�ten� ostatn�ch hodnot z katalogu do roletky
            if(!Form1->d.v.pohon_je_pouzivan(ukaz->n))
          {
           Cvektory::Ttyp_dopravniku *K=F->d.v.vrat_typ_dopravniku(F->d.v.PP.katalog);
           Cvektory::TDoubleHodnota *H=K->roztec->dalsi;
           C->Items->Clear();
            while(H!=NULL)
           {
            I=C->Items->Add();
            if(Runit==M) I->Caption=H->hodnota/(1+999.0*Runit); //obr�cen�, jeliko� v katalogu jsou v�dy mm
            else I->Caption=H->hodnota;
            H=H->dalsi;
           }
            I=NULL;delete I;
           }

         getmGridWidth();

         if(Form1->d.v.pohon_je_pouzivan(ukaz->n))
          {
          //pokud je pohon pou��v�n, z�m�rn� nenastav�m o jak� typ bun�k se jedn�, aby do nich ne�lo vstupovat a editovat
          // pouze povol�m zm�nu p�i�azen� a smaz�n� pohonu
					mGrid->Cells[1][i].Type=mGrid->EDIT;

          mGrid->Cells[2][i].Type=mGrid->readEDIT;  //TEST
          mGrid->Cells[3][i].Type=mGrid->readEDIT;
          mGrid->Cells[4][i].Type=mGrid->readEDIT;
          mGrid->Cells[5][i].Type=mGrid->readEDIT;
          mGrid->Cells[5][i].Type=mGrid->COMBOEDIT;
          mGrid->getComboEdit(5,i)->Enabled=false; //nech�m typ combo, pouze ho zak�u
          mGrid->getComboEdit(5,i)->Options->DisabledColorAlpha=250;
          mGrid->getComboEdit(5,i)->Options->FrameWidth=1;
          mGrid->getComboEdit(5,i)->Options->FrameDisabledColor=(TColor)RGB(200,200,200);
          mGrid->getComboEdit(5,i)->Options->FrameDisabledColorAlpha=250;
					mGrid->Cells[6][i].Type=mGrid->CHECK;
          mGrid->Cells[7][i].Type=mGrid->BUTTON;
          mGrid->Refresh();
          mGrid->getButton(7,i)->Options->NormalColor=(TColor)RGB(250,250,250);
          mGrid->getButton(7,i)->Options->NormalColorAlpha=250;
          mGrid->getButton(7,i)->Options->FrameWidth=1;
          mGrid->getButton(7,i)->Options->FrameNormalColor=(TColor)RGB(200,200,200);
          mGrid->getButton(7,i)->Options->FrameNormalColorAlpha=250;
          mGrid->Cells[8][i].Type=mGrid->glyphBUTTON;
          }
          else
          {
          mGrid->Cells[1][i].Type=mGrid->EDIT;
          mGrid->Cells[2][i].Type=mGrid->EDIT;
          mGrid->Cells[3][i].Type=mGrid->EDIT;
          mGrid->Cells[4][i].Type=mGrid->EDIT;
          mGrid->Cells[5][i].Type=mGrid->COMBOEDIT;
					/*mGrid->Cells[6][i].Type=mGrid->CHECK;*/mGrid->Cells[6][i].RightBorder->Color=clWhite;
					mGrid->Cells[7][i].Type=mGrid->BUTTON;
					mGrid->Cells[8][i].Type=mGrid->glyphBUTTON;

          mGrid->Refresh();
          mGrid->getButton(7,i)->Options->NormalColor=(TColor)RGB(250,250,250);
          mGrid->getButton(7,i)->Options->NormalColorAlpha=250;
          mGrid->getButton(7,i)->Options->FrameWidth=1;
          mGrid->getButton(7,i)->Options->FrameNormalColor=(TColor)RGB(200,200,200);
          mGrid->getButton(7,i)->Options->FrameNormalColorAlpha=250;

					mGrid->Cells[2][i].InputNumbersOnly=2;
					mGrid->Cells[3][i].InputNumbersOnly=2;
					mGrid->Cells[4][i].InputNumbersOnly=2;
          mGrid->Cells[5][i].InputNumbersOnly=2;
					}

           mGrid->Refresh(); // kv�li prac� s n�sledn�ch Checkboxem je nutn� refresh


          if(Form1->d.v.pohon_je_pouzivan(ukaz->n))
          {
          input_state=R;
          mGrid->getCheck(6,i)->ShowHint=true;
          mGrid->getCheck(6,i)->Hint="Zru�it p�i�azen� k objekt�m";
          mGrid->getCheck(6,i)->Checked=true;mGrid->getCheck(6,i)->Enabled=true;

          rEditNum_takt->Enabled=false;
          scGPNumericEdit_delka_jig->Enabled=false;
          scGPNumericEdit_sirka_jig->Enabled=false;
          scGPNumericEdit_vyska_jig->Enabled=false;
          scGPNumericEdit_delka_podvozek->Enabled=false;

         //pokud je pohon pou��v�n, nastav�m mu podbarven� bun�k, krome nazvu - ten je mo�ne v�dy m�nit
          mGrid->Cells[2][i].Background->Color= Form_parametry_linky->Color;
          mGrid->Cells[3][i].Background->Color=  mGrid->Cells[2][i].Background->Color;
          mGrid->Cells[4][i].Background->Color=  mGrid->Cells[2][i].Background->Color;
          mGrid->Cells[5][i].Background->Color=  mGrid->Cells[2][i].Background->Color;
          mGrid->Cells[7][i].Background->Color=  mGrid->Cells[2][i].Background->Color;
//          mGrid->getButton(7,i)->Options->FramePressedColor=clWhite;
//          mGrid->getButton(7,i)->Options->FrameNormalColor=clWhite;
//          mGrid->getButton(7,i)->Options->FrameFocusedColor=clWhite;
         // mGrid->getButton(7,i)->Enabled=false;

          }
          else
          {
          input_state=R;
//          mGrid->getCheck(6,i)->Checked=false;
//					mGrid->getCheck(6,i)->Enabled=false;
//					mGrid->getCheck(6,i)->ShowHint=true;
//					mGrid->getCheck(6,i)->Hint="Zru�it p�i�azen� k objekt�m";
          mGrid->Cells[7][i].Text="";
          }

						ukaz = ukaz->dalsi;

             getDeleteButtonSettings(i);
             getPrirazeneObjDesign(i);
				 }
				 F_gapoR->pohony_zmena=new TPoint[F->d.v.POHONY->predchozi->n+1]; //alokace o jedni�ku vy���, nult� index nen� toti� vyu��v�n
				 for(unsigned int i=0; i<=F->d.v.POHONY->predchozi->n;i++){F_gapoR->pohony_zmena[i].X=false;F_gapoR->pohony_zmena[i].Y=false;}


         mGrid->Refresh();
        //R- zakoment scGPGlyphButton_DEL_nepouzite->Visible=true;
         input_state=NOTHING;

	}
	else {  //pokud je spoj�k pr�zdn�, zobraz�m tyto p�ednastaven� hodnoty

	data_nalezena=false; // default se nepou��v� resp. po soubor nov� se okam�it� vlo�� do spoj�ku def.pohon
//nevytvari se zadny default pohon nikde
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{
	F->log(__func__); //logov�n�
  mGrid->Delete();
	//M toto tu nesm� b�t:Form_parametry_linky->Close();
	zrusena_prirazeni_PID=NULL;delete zrusena_prirazeni_PID;
	 F->scStyledForm1->HideClientInActiveEffect();
	Storno=true;
	//zm�na z�lo�ek
	F->Layout->Down=false;
	F->Schema->Down=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::KonecClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	Button_stornoClick(Sender);//stejn� funkcionalita jako u storna
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_saveClick(TObject *Sender)
{
    F->log(__func__); //logov�n�
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


     //p�ed samotn�m ulo�en�m, kontrola zdali jsou podstatn� �daje u pohonu nastaveny
     // pokud n�co chyb�, zak�u ulo�en�
     int count=0;

			for (unsigned int i = 2; i < mGrid->RowCount; i++)
			{
				if(mGrid->Cells[2][i].Text=="") count++;
				if(mGrid->Cells[3][i].Text=="") count++;
				if(mGrid->Cells[4][i].Text=="") count++;
				//if(mGrid->Cells[5][i].Text=="") count++;
        if(mGrid->getComboEdit(5,i)->Text=="") count++;
      }

        if(count>0)
        {

             scStyledForm2->ShowClientInActiveEffect();

            if(mrOk==Form1->MB("Nelze ulo�it, vypl�te v�echny �daje o pohonu",MB_OK))
            {
             Ulozit=false;
             scStyledForm2->HideClientInActiveEffect();
             }
        }

		//zobrazen� formu gapoR - jedin� GAPO, kter� je vol�no zde, proto�e se vol� a� p�i stisku OK PL formu
		bool zobrazGAPO_R=false;
		for(unsigned int i=1; i<=F->d.v.POHONY->predchozi->n;i++)
		{
			if(F_gapoR->pohony_zmena[i].X) zobrazGAPO_R=true;
			//	ShowMessage(AnsiString(i)+" "+AnsiString((short)(F_gapoR->pohony_zmena[i])));
		}

    //osetreni natvrdo aby se nezobrazovalo GAPO R
     zobrazGAPO_R=false;

                      // pokud chci budu zobrazovat gapo R, je�t� si zkontroluji cel� obsah tabulky, zda nejsou n�hodou vr�ceny v�ude v�choz� hodnoty
//      if(zobrazGAPO_R)//pokud se hodnoty shoduj�, ru��m zobrazen� gapoR
//     {
//     for(int j=1;j<=rStringGridEd_tab_dopravniky->RowCount-1;j++)
//      {
//      Memo3->Lines->Add(F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][j]));
//      Memo3->Lines->Add(F->d.v.vrat_pohon(j)->aRD*60.0);
//      if((rStringGridEd_tab_dopravniky->Cells[4][j]*(1+59.0*aRDunit))==F->d.v.vrat_pohon(j)->aRD &&
//      F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][j])*(1+999.0*Runit)==F->ms.MyToDouble(F->d.v.vrat_pohon(j)->roztec) /*&&
//      F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][j])*(1+999.0*Rzunit)==F->ms.MyToDouble(F->d.v.vrat_pohon(j)->Rz) &&
//     /* F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[7][j])==F->d.v.vrat_pohon(j)->Rx*/)
//      {
//       nezobrazuj++;
//       }
//       else nezobrazuj=0;
//      } //pokud po�et ��dk� beze zm�ny sed� s celkov�m po�tem ��dk� = ��dn� zm�na, nebudu nakonec gapo R zobrazovat, jinak zobraz�m gapoR
//      if(nezobrazuj==rStringGridEd_tab_dopravniky->RowCount-1)    zobrazGAPO_R=false;  else zobrazGAPO_R=true;
//      }

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
//		for(short i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)
//
//		{
//			//pr�chod jednotliv�mi objekty, zda je dan� pohon objektu p�i�azen a pokud ano, tak zda je mimo rozsah
//			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
//			while(O!=NULL)
//			{
//      double grid_od = 0;
//      double grid_do = 0;   //zde nen� nutn� nic p�ev�d�t, p�ev�d� se lev� ��st v podm�nce IF dle akt. jednotek
//       grid_od =  Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]);
//       grid_do =  Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]);
//
//				if(
//					O->pohon!=NULL && //kdy� m� objekt p�i�azen pohon a z�rove�
//					O->rezim==1 && //je v re�imu kontinu�l a z�rove�
//					O->pohon->n==Form1->ms.a2i(rStringGridEd_tab_dopravniky->Cells[0][i]) &&//pokud objekt m� pohon p�i�azen a z�rove�
//					(Form1->ms.MyToDouble(O->RD)*(1+59.0*aRDunit) < grid_od ||//je mimo rozsah (pod) nebo
//					 Form1->ms.MyToDouble(O->RD)*(1+59.0*aRDunit) > grid_do)//je mimo rozsah (nad)
//				)
//				{
//          Memo3->Lines->Add(grid_od);
//          Memo3->Lines->Add(grid_do);
//          Memo3->Lines->Add(aRDunit);
//					T+="Objekt: "+O->name+" Rychlost:"+O->RD*(1+59.0*aRDunit)+" vs. Pohon: "+rStringGridEd_tab_dopravniky->Cells[1][i];
//					if(O->predchozi!=O)T+=",";//u posledn�ho prvku nep�id� ��rku
//				}
//				O=O->dalsi;
//			}
//			O=NULL;delete O;
//		}

		if(T!="")//byly nalezeny objekty mimo rozmez� + v�pis
		{
			Changes=false;Ulozit=false;//zak�e ulo�en�
			Form1->MB("Pozor, nelze ulo�it hodnoty rozmez� pohon�, proto�e n�sleduj�c� objekty maj� rychlost mimo nov� nastaven� rozsah: "+T);
		}


     if(F_gapoV->UlozitGAPOV)
     {
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
    }

      if(F_gapoTT->UlozitGAPOTT)
      {
			double Takt=0;
			if(Taktunit==MIN)  Takt=rEditNum_takt->Value*60.0; else Takt=rEditNum_takt->Value;
			Form1->d.v.PP.TT=Takt;
      }






   if(F_gapoTT->UlozitGAPOTT==false && F_gapoV->UlozitGAPOV==false)  //pokud nedojde k vol�n� GAPOTT nebo GAPOV, ulo��m data na PL norm�ln�m pr�chodem
  {
		//////////////////////////////////////////////////////////////////////
	//	Pri zmene roztece  - volani zm�ny rozte�e - pokud dojde ke zm�n� rozte�e u pou��van�ho pohonu - p�ed�m status pro zobrazen� PL_priority
//			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
//			while(P!=NULL)
//			{
//			     double roztec;
//					// if(Runit==M) roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n]);
//				 //	 else 				roztec = F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[5][P->n])/1000.0;
//         ShowMessage(P->n+1);
//					if(mGrid->getCheck(6,P->n + 1)->Checked==true && roztec!=P->roztec)
//					{
//					if(mGrid->Cells[5][P->n + 1].Text!="")  //osetreni situace kdyz odmazu pohon a N je prazdne
//					{
//							Changes_roztec=true;
//						}
//					}
//					P=P->dalsi;
//				 }
//			P=NULL;delete P;


			//////////////////////////////////////////////////

			// Hl�dan� n�sleduj�c�ch zm�n, pokud je pohon pou��van�

//				Cvektory::TPohon *p_prirazen=Form1->d.v.POHONY->dalsi;
//			while(p_prirazen!=NULL)
//			{
//						bool pohon_pouzivan=false;
//						AnsiString pohon_prirazen=Form1->d.v.vypis_objekty_vyuzivajici_pohon(p_prirazen->n);
//						if(pohon_prirazen!="")pohon_pouzivan=true;
//						else   pohon_pouzivan=false;
//
//				 //zm�na odp�i�azen�
//
//					if(mGrid->Cells[5][p_prirazen->n].Text==""  && pohon_pouzivan)
//					{
//           ShowMessage(pohon_prirazen);
//							Changes_prirazen=true;
//							zrusena_prirazeni_PID[getPID(p_prirazen->n)-1]=true;
//
//					}
//				 //zm�na aRD
////						if(mGrid->getCheck(6,p_prirazen->n + 1)->Checked==false  && F->ms.MyToDouble(mGrid->Cells[4][p_prirazen->n + 1].Text)/(1+59.0*aRDunit)!=p_prirazen->aRD)
////					{
////							Changes_aRD=true;
////
////					}
//
//					p_prirazen=p_prirazen->dalsi;
//				 }
//
//			p_prirazen=NULL;delete p_prirazen;


	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 //	if(Form1->d.v.OBJEKTY->dalsi==NULL)Ulozit=true;   // pokud neexistuje zadny objekt, vzdy dovolim delat zmeny a moznost ulozit

		// ukladej
		if (Ulozit)
		{
			zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum();
			Form1->d.v.vymaz_seznam_POHONY();
			Form1->d.v.hlavicka_POHONY();

      bool nelze_ulozit=false;

			for (unsigned int i = 2; i < mGrid->RowCount; i++)
			{
				double rychlost_od;
				double rychlost_do;
				double roztec;
				double aRD;
			 	double Rz;  //nepouzivane
			 	double Rx;  //nepouzivane
				UnicodeString nazev;

				if (mGrid->Cells[1][i].Text=="") nazev="nov� pohon";
				else  nazev=mGrid->Cells[1][i].Text;

				if (mGrid->Cells[2][i].Text=="") {rychlost_od=0; nelze_ulozit=true; }
				else  rychlost_od=Form1->ms.MyToDouble(mGrid->Cells[2][i].Text)/(1+59.0*aRDunit);

				if(mGrid->Cells[3][i].Text=="") {rychlost_do=0; nelze_ulozit=true;  }
				else 	rychlost_do=Form1->ms.MyToDouble(mGrid->Cells[3][i].Text)/(1+59.0*aRDunit);

				if(mGrid->Cells[4][i].Text=="") {aRD=0; nelze_ulozit=true; }
				else aRD=Form1->ms.MyToDouble(mGrid->Cells[4][i].Text)/(1+59.0*aRDunit);

      //  ShowMessage(mGrid->getComboEdit(5,i)->Text);
				if(mGrid->getComboEdit(5,i)->Text=="") {roztec=0; nelze_ulozit=true; }
				if(Runit==MM) roztec=Form1->ms.MyToDouble(mGrid->getComboEdit(5,i)->Text)/1000.0;
				else roztec=Form1->ms.MyToDouble(mGrid->getComboEdit(5,i)->Text);

				if(mGrid->getButton(7,i)->Caption==""){Rz=0.0;Rx=0.0;}//pokud nen� pohon p�i�azen nuluj

				//ulo�en� pohonu do spoj�ku
				Form1->d.v.vloz_pohon (nazev,rychlost_od,rychlost_do,aRD,roztec,Rz,Rx);

				//v�em objekt�m, kter� m�ly p�i�azen pohon s oldN(oldID), p�i�ad� pohon s newN(newID), podle toho, jak jsou ukl�d�ny nov� do spoj�ku, d�le�it�, pokud dojde k naru�en� po�ad� ID resp n pohon� a po�ad� jednotliv�ch ��dk� ve stringridu, nap�. kopirov�n�m, smaz�n�m, zm�nou po�ad� ��dk� atp., �e�� i pro p��pad nap�. 2->3,3->4 pomoc� atributu objektu probehla_aktualizace_prirazeni_pohonu (aby prvn� nebyl p�i�ezn pohon s id 2 na 3 a potom v�echny pohony s id 3 na pohon 4, proto�e m�ly b�t p�i�azen� jen n�kter�...)
			 Form1->d.v.aktualizace_prirazeni_pohonu_k_objektum(getPID(i),i-1);
			}
			//po dokon�en� aktualizace p�i�azen� pohonu (p�i ukl�d�n� pohonu na PL) vr�t� atribut probehla_aktualizace_prirazeni_pohonu v�ech objekt� na false, aby bylo p�ipraveno k dal��mu op�tovn�mu u��t�, nep��mo spolupracuje s metodou v��e uvedenou aktualizace_prirazeni_pohonu_k_objektum
 	   Form1->d.v.aktualizace_prirazeni_pohonu_dokoncena();
//

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
				F->d.v.PP.delka_podvozek=F->ms.MyToDouble(scGPNumericEdit_delka_podvozek->Value)/1000.0;
        F->d.v.PP.uchyt_pozice=F->ms.MyToDouble(scGPTrackBar_uchyceni->Value)/1000.0;
			}
			else  //Metry
			{
				F->d.v.PP.delka_jig=F->ms.MyToDouble(scGPNumericEdit_delka_jig->Value);
				F->d.v.PP.sirka_jig=F->ms.MyToDouble(scGPNumericEdit_sirka_jig->Value);
				F->d.v.PP.delka_podvozek=F->ms.MyToDouble(scGPNumericEdit_delka_podvozek->Value);
        F->d.v.PP.uchyt_pozice=F->ms.MyToDouble(scGPTrackBar_uchyceni->Value);
			}
			int typ;
			if(scGPSwitch->State==scswOff){typ=0;}
			else {typ=1;}
			Form1->d.v.PP.typ_linky=Form1->ms.MyToDouble(typ);

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
		}

   } 	Form1->DuvodUlozit(true);

   if(Ulozit)
   {
    mGrid->Delete();
    F->scStyledForm1->HideClientInActiveEffect();
		Close();//v testu, m��e padat
	 }
	 //zm�na z�lo�ek
	 F->Layout->Down=false;
	 F->Schema->Down=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_ADD_Click(TObject *Sender)
{
  F->log(__func__); //logov�n�
	//nav��� po�et ��dk�
  input_state=JOB;
  mGrid->AddRow(true,false);

  int i = mGrid->RowCount -1 ;//po�ad� ��dku o jedni�ku ni��� ne� po�et ��dk�

	mGrid->Cells[0][i].Text = getMaxPID()+1;//mGrid->RowCount - 2;
	mGrid->Cells[1][i].Text = "nov� pohon ";//rStringGridEd_tab_dopravniky->Cells[1][i - 1];

   getmGridWidth();

   mGrid->Cells[1][i].Type=mGrid->EDIT;
   mGrid->Cells[2][i].Type=mGrid->EDIT;
   mGrid->Cells[3][i].Type=mGrid->EDIT;
   mGrid->Cells[4][i].Type=mGrid->EDIT;
   mGrid->Cells[5][i].Type=mGrid->COMBOEDIT;

	 /*mGrid->Cells[6][i].Type=mGrid->CHECK;*/mGrid->Cells[6][i].RightBorder->Color=clWhite;//Check zobrazen pouze v p��pad�, �e je pohon p�i�azen
   mGrid->Cells[7][i].Type=mGrid->BUTTON;

	 mGrid->Cells[2][i].InputNumbersOnly=2;
	 mGrid->Cells[3][i].InputNumbersOnly=2;
	 mGrid->Cells[4][i].InputNumbersOnly=2;
   mGrid->Cells[5][i].InputNumbersOnly=2;

  mGrid->Cells[8][i].Type=mGrid->glyphBUTTON;

   mGrid->Refresh();

   TscGPComboEdit *C=mGrid->getComboEdit(5,i);
   TscGPListBoxItem *I;

   Cvektory::Ttyp_dopravniku *K=F->d.v.vrat_typ_dopravniku(F->d.v.PP.katalog);
   Cvektory::TDoubleHodnota *H=K->roztec->dalsi;
    while(H!=NULL)
   {
    I=C->Items->Add();
    if(Runit==MM) I->Caption=H->hodnota;
    else I->Caption=H->hodnota/1000.0;
    H=H->dalsi;
   }

  	I=NULL;delete I;
   //defaultn� item index p�i klik na + pohonu
   mGrid->getComboEdit(5,i)->ItemIndex=0;

//  mGrid->getCheck(6,i)->Enabled=false;
//	mGrid->getCheck(6,i)->ShowHint=true; mGrid->getCheck(6,i)->Hint="Zru�it p�i�azen� k objekt�m";
//mGrid->getComboEdit(5,i)->Height=mGrid->getEdit(4,i)->Height;
	getDeleteButtonSettings(i);
  getPrirazeneObjDesign(i);
  setADD_ButtonPosition();
  setFormHeight();
  //R - zakoment scGPGlyphButton_DEL_nepouzite->Visible=true;
	vykresli_obdelnik_vpravo();
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//sma�e posledn� ��dek - ji� se nepou��v�, ale nechv�m
void __fastcall TForm_parametry_linky::Button_DEL_Click(TObject *Sender)
{
//	if(Form1->d.v.pohon_je_pouzivan(rStringGridEd_tab_dopravniky->RowCount-1))
//	{
//		AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(rStringGridEd_tab_dopravniky->RowCount-1),true);
//				if(mrOk==Form1->MB("Pohon je pou��v�n pro objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_OKCANCEL)){
//
//				//p�vodn� zakomentovan� konstrukcenefunguje spr�vn� pro p��pad storna, proto �e�� n�sleduj�c� ��dek, Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(rStringGridEd_tab_dopravniky->RowCount-1));
//				zrusena_prirazeni_PID[getPID(rStringGridEd_tab_dopravniky->RowCount-1)-1]=true;
//				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();
//
//					if(rStringGridEd_tab_dopravniky->RowCount>1)
//					{
//					rStringGridEd_tab_dopravniky->RowCount--;
//					}
//				 //	Form1->MB("Smazano");
//				}
//				else
//				{
//				 //storno   - nic se ned�je
//				}
//	}
//	else// pohon neni pouzivany, mohu ho smazat cokoliv ze stringgridu
//	{
//		rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();
//		if(rStringGridEd_tab_dopravniky->RowCount>1)
//		{
//			rStringGridEd_tab_dopravniky->RowCount--;
//		}
//	}
//
//	//	for (long i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
//	//	rStringGridEd_tab_dopravniky->Cells[0][i] = i;
//
//	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
//	pozice_scGPGlyphButton_hint();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Vypis_pohonyClick(TObject *Sender)
{
   F->log(__func__); //logov�n�
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
    F->log(__func__); //logov�n�
		scExPanel_doporuc_pohony->Visible=false;
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
    F->log(__func__); //logov�n�
    scExPanel_doporuc_pohony->Visible=false;
    scGPButton_doporucene->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::rEditNum_takt_Change(TObject *Sender)
{
   F->log(__func__); //logov�n�
	if(input_state==NOTHING && input_clicked_edit==TT_klik)
	{

	input_TT();

	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_delkavozikuClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_sirka_jigClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_taktClick(TObject *Sender)
{
//
   F->log(__func__); //logov�n�
	if(Taktunit==MIN)//pokud je v MM, tak p�epne na metry
	{
		Taktunit=S;
		//delka - p�epo��t�n�
		rEditNum_takt->Value=rEditNum_takt->Value*60.0;
		rHTMLLabel_takt->Caption="<font color=#2b579a>[s]</font>";
	}
	else//metrech tak se p�epne na MM
	{
		Taktunit=MIN;
		//delka - p�epo��t�n�
		rEditNum_takt->Value=rEditNum_takt->Value/60.0;
		rHTMLLabel_takt->Caption="<font color=#2b579a>[m]</font>";
	}

}
//---------------------------------------------------------------------------
//tla��tko na kop�rov�n� doporu�en�ch pohon� do striggridu, nep�id�v� ale do pohon�
void __fastcall TForm_parametry_linky::scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender)
{
//	 //najde max pou�it� ID pohonu (proto�e ID nejsou se�azena, nutno hledat, nikoliv vz�t id z posledn�ho ��dku)
//	 unsigned int ID=getMaxPID();
//
//	 //nov� konstrukce zaji��uj�c� pouze vlo�en� do stringgridu, o samotn� ulo�en� pohon� se star� a� tla��tko ulo�it
//	 AnsiString T=scHTMLLabel_doporuc_pohony->Caption;
//
//	 while(T.Pos("</br>"))//bude parsovat dokud bude </br>
//	 {
//	 	//zv��en� po�tu ��dk�
//   	rStringGridEd_tab_dopravniky->RowCount++;
//    //dynamick� zv�t�en� tabulky - aby se nemusel zobrazovat postrann� scrollbar
//       if(rStringGridEd_tab_dopravniky->RowCount>=1)
//      {
//      rStringGridEd_tab_dopravniky->Height=rStringGridEd_tab_dopravniky->Height+30;
//      Form_parametry_linky->Height= Form_parametry_linky->Height+30;
//      //scGPGlyphButton_ADD->Top=scGPGlyphButton_ADD->Top + 30;
//      Button_save->Top=Button_save->Top + 30;
//      Button_storno->Top=Button_storno->Top + 30;
//      scGPGlyphButton_DEL_nepouzite->Top=scGPGlyphButton_DEL_nepouzite->Top+30;
//      }
//
//   	unsigned int i=rStringGridEd_tab_dopravniky->RowCount-1;//pouze zkr�cen� z�pisu
//	 	//pln�n� ��dku a parsov�n� daty
//		rStringGridEd_tab_dopravniky->Cells[0][i]=++ID;//ID
//		rStringGridEd_tab_dopravniky->Cells[1][i]="Navr�en� pohon pro"+Form1->ms.EP(T,":",".");//n�zev pohonu a vy�e�t�n� �et�zce pro dal�� u�it�
//		rStringGridEd_tab_dopravniky->Cells[2][i]=Form1->ms.EP(T,"Navr�en� pohon s rychlost�"," [");//T=Form1->ms.TrimLeftFrom_UTF(T," </br>");
//		rStringGridEd_tab_dopravniky->Cells[3][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
//		//pro aRD a� bude sloupec: rStringGridEd_tab_dopravniky->Cells[?][i]=rStringGridEd_tab_dopravniky->Cells[2][i];
//		rStringGridEd_tab_dopravniky->Cells[6][i]="nepou��v�n";
//		//smaz�n� jednoho ji� nepot�ebn�ho z�znamu
//		T=Form1->ms.TrimLeftFromText(T,"</br>");
//	 }
//	 //smaz�n� ji� nahran�ch a skryt� panelu
//	 scHTMLLabel_doporuc_pohony->Caption="Nejsou k dispozici ��dn� navr�en� pohony";//nepou��v� se tento v�pis viditeln�, situace nenastane
//	 scExPanel_doporuc_pohony->Visible=false;
//	 scGPButton_doporucene->Visible=true;
//	 //p�esune focus na posledn� ��dek
//	 rStringGridEd_tab_dopravniky->Row=rStringGridEd_tab_dopravniky->RowCount-1;
//	 //pozice info tla��tka - asi je tla��tko stejn� provizorn�
//	 pozice_scGPGlyphButton_hint();
//	 //existuj� ur�it� nepou��t� pohony a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
//	 scGPGlyphButton_DEL_nepouzite->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  F->log(__func__); //logov�n�
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
  // Button_save->Enabled=true;
   //automatick� vygenerov�n� pohon�
   if(DEBUG)
	 {

	 scGPNumericEdit_delka_podvozek->Value=300;
	 scGPNumericEdit_sirka_jig->Value=400;
   scGPNumericEdit_delka_jig->Value=1200;
   scGPNumericEdit_vyska_jig->Value=800;

  for (int i = mGrid->RowCount ; i <= 4; i++) {

  input_state=JOB;
	mGrid->AddRow(true,false);

	mGrid->Cells[0][i].Text = getMaxPID()+1;//mGrid->RowCount - 2;
	mGrid->Cells[1][i].Text = "nov� pohon " + AnsiString(i-1);

   getmGridWidth();
   mGrid->Cells[1][i].Type=mGrid->EDIT;
   mGrid->Cells[2][i].Type=mGrid->EDIT;
   mGrid->Cells[3][i].Type=mGrid->EDIT;
   mGrid->Cells[4][i].Type=mGrid->EDIT;
   mGrid->Cells[5][i].Type=mGrid->COMBOEDIT;
	 /*mGrid->Cells[6][i].Type=mGrid->CHECK;*/ mGrid->Cells[6][i].RightBorder->Color=clWhite;
	 mGrid->Cells[7][i].Type=mGrid->BUTTON;

	 mGrid->Cells[2][i].InputNumbersOnly=2;
	 mGrid->Cells[3][i].InputNumbersOnly=2;
	 mGrid->Cells[4][i].InputNumbersOnly=2;
   mGrid->Cells[5][i].InputNumbersOnly=2;

   mGrid->Refresh();   //kv�li pr�ci s combem je nutn� refresh po nastaven� na typ COMBOEDIT
   TscGPComboEdit *C=mGrid->getComboEdit(5,i);
   TscGPListBoxItem *I;

   //na�ten� hodnoty rozte�e do roletky + nastaven� jako ItemIndex=0

   if(i==2)
   {
    mGrid->Cells[2][i].Text="0,2";
    mGrid->Cells[3][i].Text="5";
    mGrid->Cells[4][i].Text="0,5";
    I=C->Items->Add();
    I->Caption = "250";
    C->ItemIndex=0;
    }

    if(i==3)
    {
    mGrid->Cells[2][i].Text="1";
    mGrid->Cells[3][i].Text="3";
    mGrid->Cells[4][i].Text="2,2";
    I=C->Items->Add();
    I->Caption = "450";
    C->ItemIndex=0;
    }

    if(i==4)
    {
    mGrid->Cells[2][i].Text="1,5";
    mGrid->Cells[3][i].Text="5";
    mGrid->Cells[4][i].Text="4";
    I=C->Items->Add();
    I->Caption = "1230";
    C->ItemIndex=0;
    }

   mGrid->Cells[8][i].Type=mGrid->glyphBUTTON;
   mGrid->Update();

//  mGrid->getCheck(6,i)->Enabled=false;
//	mGrid->getCheck(6,i)->ShowHint=true; mGrid->getCheck(6,i)->Hint="Zru�it p�i�azen� k objekt�m";
  getDeleteButtonSettings(i);
	getPrirazeneObjDesign(i);
  setADD_ButtonPosition();
  setFormHeight();
	//R - zakoment scGPGlyphButton_DEL_nepouzite->Visible=true;
  //Form_parametry_linky->Color=F->m.clIntensive(clGray,10);
  }

  mGrid->Refresh();
	input_state=NOTHING;
	vykresli_obdelnik_vpravo();
	}
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm_parametry_linky::rEditNum_delkavozikuClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	rHTMLLabel_delka_jigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_delka_jigClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	if(Delkaunit==MM)//pokud je v MM, tak p�epne na metry
	{
		Delkaunit=M;
		//delka - p�epo��t�n�

		rHTMLLabel_delka_jig->Caption="<font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value/1000.0;

		rHTMLLabel_sirka_jig->Caption="<font color=#2b579a>[m]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value/1000.0;

		rHTMLLabel_delka_podvozek->Caption="<font color=#2b579a>[m]</font>";
		scGPNumericEdit_delka_podvozek->Value=scGPNumericEdit_delka_podvozek->Value/1000.0;

		rHTMLLabel_vyska_jig->Caption="<font color=#2b579a>[m]</font>";
		scGPNumericEdit_vyska_jig->Value=scGPNumericEdit_vyska_jig->Value/1000.0;
	}
	else//metrech tak se p�epne na MM
	{
		Delkaunit=MM;

		rHTMLLabel_delka_jig->Caption="<font color=#2b579a>[mm]</font>";
		scGPNumericEdit_delka_jig->Value=scGPNumericEdit_delka_jig->Value*1000.0;

		rHTMLLabel_sirka_jig->Caption="<font color=#2b579a>[mm]</font>";
		scGPNumericEdit_sirka_jig->Value=scGPNumericEdit_sirka_jig->Value*1000.0;

		rHTMLLabel_delka_podvozek->Caption="<font color=#2b579a>[mm]</font>";
		scGPNumericEdit_delka_podvozek->Value=scGPNumericEdit_delka_podvozek->Value*1000.0;

		rHTMLLabel_vyska_jig->Caption="<font color=#2b579a>[mm]</font>";
		scGPNumericEdit_vyska_jig->Value=scGPNumericEdit_vyska_jig->Value*1000.0;
	}
	vozik_zmena_jednotek();
}
//---------------------------------------------------------------------------
void TForm_parametry_linky::vozik_zmena_jednotek()
{
  F->log(__func__); //logov�n�
	if(Delkaunit==M)
	{
		scGPNumericEdit_sirka_jig->Width=34;//scGPNumericEdit_sirka_jig->Width-27;//61 default
		scGPNumericEdit_delka_jig->Width=34;//scGPNumericEdit_delka_jig->Width-27;
		scGPNumericEdit_delka_podvozek->Width=34;//scGPNumericEdit_delka_podvozek->Width-27;
		scGPNumericEdit_vyska_jig->Width=34;//scGPNumericEdit_vyska_jig->Width-27;
		scGPNumericEdit_vyska_jig->Left=78;//default 45, posun 41

	}
	else
	{
		scGPNumericEdit_sirka_jig->Width=61;//scGPNumericEdit_sirka_jig->Width+27;
		scGPNumericEdit_delka_jig->Width=61;//scGPNumericEdit_delka_jig->Width+27;
		scGPNumericEdit_delka_podvozek->Width=61;//scGPNumericEdit_delka_podvozek->Width+27;
		scGPNumericEdit_vyska_jig->Width=61;//scGPNumericEdit_vyska_jig->Width+27;
		scGPNumericEdit_vyska_jig->Left=37;
	}
	//nastavov�n� odkaz�
	if(rImageEx_jig_podlahovy->Visible)
	{
		rHTMLLabel_delka_jig->Left=scGPNumericEdit_delka_jig->Left+scGPNumericEdit_delka_jig->Width+2;
		rHTMLLabel_delka_podvozek->Left=scGPNumericEdit_delka_podvozek->Left+scGPNumericEdit_delka_podvozek->Width+2;
	}
	else
	{
		rHTMLLabel_delka_podvozek->Left=scGPNumericEdit_delka_jig->Left+scGPNumericEdit_delka_jig->Width+2;
		rHTMLLabel_delka_jig->Left=scGPNumericEdit_delka_podvozek->Left+scGPNumericEdit_delka_podvozek->Width+2;
	}
	rHTMLLabel_sirka_jig->Left=scGPNumericEdit_sirka_jig->Left+scGPNumericEdit_sirka_jig->Width+2;
	rHTMLLabel_vyska_jig->Left=scGPNumericEdit_vyska_jig->Left+scGPNumericEdit_vyska_jig->Width+2;
}
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
 //rStringGridEd_tab_dopravniky->Width=Form_parametry_linky->Width;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
//  TPoint RET=mGrid->CheckLink(X,Y);
//  Memo2->Lines->Add(RET.x);

}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormPaint(TObject *Sender)
{
 	mGrid->Show();//vykresl� tabulku

	if(zobrazitFrameForm)Form1->m.frameForm(Form_parametry_linky,clWebOrange,1);

	if(VID==-1) { /*scGPGlyphButton_ADD->Enabled=true;*/ /*scGPGlyphButton_ADD->Visible=true;*/ scGPGlyphButton_vozik_edit->Enabled=true; scGPGlyphButton_TT->Enabled=true;  }
	else {       /*scGPGlyphButton_ADD->Enabled=false;*/ /*scGPGlyphButton_ADD->Visible=false;*/  scGPGlyphButton_vozik_edit->Enabled=false; scGPGlyphButton_TT->Enabled=false; }

  //	workaround - zru�en� or�mov�n� okolo nepou�it�ch vn�j��ch bun�k
  vykresli_obdelnik_vpravo();
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
    F->log(__func__); //logov�n�
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
  F->log(__func__); //logov�n�
	try
	{
		return mGrid->Cells[0][ROW].Text.ToInt();
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
  F->log(__func__); //logov�n�
	unsigned int ID=0;//id
	for(unsigned i=2;i<mGrid->RowCount;i++)
	if(ID<getPID(i))ID=getPID(i);
	return ID;
}
//---------------------------------------------------------------------------
//vr�t� ��slo ��dku dle pohon ID, pokud nenajde vr�t� -1
int TForm_parametry_linky::getROW(int PID)
{
  F->log(__func__); //logov�n�
	int RET=-1;
	for(unsigned i=2;i<mGrid->RowCount;i++)
	{
		if(PID==mGrid->Cells[0][i].Text.ToInt())
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
  F->log(__func__); //logov�n�
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
   F->log(__func__); //logov�n�
	 bool RET=false;
	 for(unsigned int i=2;i<mGrid->RowCount;i++)//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�eho
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
//---------------------------------------------------------------------------
//k��ek, kter� skryje pop-up menu
void __fastcall TForm_parametry_linky::GlyphButton_closeClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	PopUPmenu->Visible=false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
void TForm_parametry_linky::top_positon(int top)
{
  F->log(__func__); //logov�n�
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
//	int ROW=rStringGridEd_tab_dopravniky->Row;
//	rStringGridEd_tab_dopravniky->InsertRowEx(ROW+1,false);
//	//zkop�rov�n� ��dku za kop�rovan� pohon
//	rStringGridEd_tab_dopravniky->Rows[ROW+1]=rStringGridEd_tab_dopravniky->Rows[ROW];
//	//n�zev p�ejmenov�n�
//	rStringGridEd_tab_dopravniky->Cells[1][ROW+1]=rStringGridEd_tab_dopravniky->Cells[1][ROW]+" - kopie";
//	//nepou��v�n
//	rStringGridEd_tab_dopravniky->Cells[8][ROW+1]="nepou��v�n";
//	//indexace
//	rStringGridEd_tab_dopravniky->Cells[0][ROW+1]=getMaxPID()+1;
//	//p�esune focus na posledn� ��dek
//	rStringGridEd_tab_dopravniky->Row=ROW+1;
//	//skryt� pop-up menu
//	PopUPmenu->Visible=false;
//	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
//	pozice_scGPGlyphButton_hint();
//	//existuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch zobrazovat
//	scGPGlyphButton_DEL_nepouzite->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_kopirovatMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
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
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_kopirovatMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
	scLabel_kopirovatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_kopirovatMouseLeave(TObject *Sender)
{
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zaji��uje sm�z�n� pohonu na vybran�m ��dku
void __fastcall TForm_parametry_linky::scLabel_smazatClick(TObject *Sender)
{
//	int ROW=rStringGridEd_tab_dopravniky->Row;
//	bool smazat=false;
//	if(Form1->d.v.pohon_je_pouzivan(getPID(ROW)))//pohon je pou��van�
//	{
//			AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(ROW),true);
//			myMessageBox->zobrazitFrameForm=true;//zajist� or�mov�n� MB
//			if(mrYes==Form1->MB("Pohon je pou��v�n objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_YESNO))
//			{
//				//Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(ROW)); p�vodn� p��m� smaz�n�, ale nereflektovalo by p��padn� storno
//				//pozor nen� p�ipraveno na situaci, pokud by bylo mo�n� p��mo v PL p�i�azovan pohony a potom zase odp�i�azovat (muselo by se nav��it pole zrusena_prirazeni_PID)
//				zrusena_prirazeni_PID[getPID(ROW)-1]=true;//nahrazeno novou filozofii, z d�vodu mo�n�ho storna formu
//				smazat=true;
//			}
//			myMessageBox->zobrazitFrameForm=false;//zajist� odor�mov�n� MB - kv�li dal��mu pou�it�
//	}
//	else//pohon nen� pou��van� a m��eme tedy smazat rovnou
//	{
//		smazat=true;
//	}
//	//samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
//	if(smazat)rStringGridEd_tab_dopravniky->DeleteRowEx(ROW);
//	//skryt� pop-up menu
//	PopUPmenu->Visible=false;
//	//pozice info tla��tka - asi je tla��tko stejn� provizorn�
//	pozice_scGPGlyphButton_hint();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazatMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
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
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazatMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
	scLabel_smazatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazatMouseLeave(TObject *Sender)
{
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//proch�z� v�echny pohany a pokud je pohon nepou��v�n, sma�e ho
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
	if(mrYes==F->MB("Opravdu chcete smazat nepou��van� pohony?",MB_YESNO))
	{
		for(unsigned int j=2;j<mGrid->RowCount;j++)//proch�z� v�echny pohony a pokud je pohon nepou��v�n, sma�eho
		{
			if(Form1->d.v.pohon_je_pouzivan(getPID(j))==false)//pohon nen� pou��van�
			{
				//samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
				mGrid->DeleteRow(j,false);
				j--;//mus� po smaz�n� nutn� sn�it index

				mGrid->Height=mGrid->RowCount*30 + 48;
				Form_parametry_linky->Height= mGrid->Height +428;
				//scGPGlyphButton_ADD->Top=Form_parametry_linky->Height - 65 ;
				Button_save->Top=Form_parametry_linky->Height - 40;
				Button_storno->Top=Form_parametry_linky->Height - 40;
				scGPGlyphButton_DEL_nepouzite->Top=Form_parametry_linky->Height-30;
				rHTMLLabel_InfoText->Top=  mGrid->Height + 8;
				vypis("",false);
			}
      else {mGrid->getCheck(6,j)->Checked=true;  mGrid->getCheck(6,j)->Enabled=true;   }
		}
     mGrid->Refresh();
     setADD_ButtonPosition();
		//neexistuje nepou��van� pohon a je tedy vhodn� nab�dku na smaz�n� nepou�it�ch nezobrazovat
		scGPGlyphButton_DEL_nepouzite->Visible=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scLabel_smazat_nepouziteMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
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
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazat_nepouziteMouseEnter(TObject *Sender)
{
  F->log(__func__); //logov�n�
	scLabel_smazat_nepouziteMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::GlyphButton_smazat_nepouziteMouseLeave(TObject *Sender)
{
  F->log(__func__); //logov�n�
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::scHTMLLabel_doporuc_pohonyClick(TObject *Sender)
{
  F->log(__func__); //logov�n�
  // zavol� funkcionalitu tla��tka na kop�rov�n� navr�en�ch pohon� do striggridu, nep�id�v� ale do pohon�
	scGPGlyphButton_add_mezi_pohonyClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPSwitchChangeState(TObject *Sender)
{
    F->log(__func__); //logov�n�
		if(scGPSwitch->State==0)
    {
		 rImageEx_jig_podlahovy->Visible=true;//bude podlahov�
		 rImageEx_jig_podvesny->Visible=false;
    }
		else
    {
		 rImageEx_jig_podvesny->Visible=true;//bude podv�sn�
     rImageEx_jig_podlahovy->Visible=false;
		}
		nastav_edity();
}
//---------------------------------------------------------------------------
//metoda vym�n� edit pro d�lku podvozku a d�lku jigu podle typu voz�ku podlahov�/podv�sn�
void TForm_parametry_linky::nastav_edity ()
{
  F->log(__func__); //logov�n�
	int y=0,x=0;

	if(rImageEx_jig_podlahovy->Visible)
	{
		scGPNumericEdit_sirka_jig->Top=100;
		rHTMLLabel_sirka_jig->Top=100;
		scGPNumericEdit_vyska_jig->Top=82;
		rHTMLLabel_vyska_jig->Top=82;

		scGPNumericEdit_delka_jig->Left=194; //194
		scGPNumericEdit_delka_podvozek->Left=184;
	}
	if(rImageEx_jig_podvesny->Visible)
	{
		scGPNumericEdit_sirka_jig->Top=148;
		rHTMLLabel_sirka_jig->Top=148;
		scGPNumericEdit_vyska_jig->Top=98;
		rHTMLLabel_vyska_jig->Top=98;

		scGPNumericEdit_delka_jig->Left=189;//189
		scGPNumericEdit_delka_podvozek->Left=181;
	}
	if(rImageEx_jig_podlahovy->Visible&&scGPNumericEdit_delka_jig->Top>scGPNumericEdit_delka_podvozek->Top||rImageEx_jig_podvesny->Visible&&scGPNumericEdit_delka_jig->Top<scGPNumericEdit_delka_podvozek->Top)//podlahov� voz�k a z�rove� �patn� edity
	{
		y=scGPNumericEdit_delka_jig->Top;
		x=scGPNumericEdit_delka_jig->Left;
		scGPNumericEdit_delka_jig->Top=scGPNumericEdit_delka_podvozek->Top;
		scGPNumericEdit_delka_podvozek->Top=y;
		scGPNumericEdit_delka_jig->Left=scGPNumericEdit_delka_podvozek->Left;
		scGPNumericEdit_delka_podvozek->Left=x;
	}
  if(rImageEx_jig_podlahovy->Visible)
	{
  	rHTMLLabel_delka_jig->Left=scGPNumericEdit_delka_jig->Left+scGPNumericEdit_delka_jig->Width+2;
		rHTMLLabel_delka_podvozek->Left=scGPNumericEdit_delka_podvozek->Left+scGPNumericEdit_delka_podvozek->Width+2;
	}
	else
	{
  	rHTMLLabel_delka_podvozek->Left=scGPNumericEdit_delka_jig->Left+scGPNumericEdit_delka_jig->Width+2;
		rHTMLLabel_delka_jig->Left=scGPNumericEdit_delka_podvozek->Left+scGPNumericEdit_delka_podvozek->Width+2;
  }
	rHTMLLabel_sirka_jig->Left=scGPNumericEdit_sirka_jig->Left+scGPNumericEdit_sirka_jig->Width+2;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormClose(TObject *Sender, TCloseAction &Action)
{
  F->log(__func__); //logov�n�
  //zapis do PL ini
  if(aRDunit==MIN)  F->aRDunit=F->MIN;
  else F->aRDunit=F->SEC;
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
    F->log(__func__); //logov�n�
		Button_save->Enabled=true;
		Button_save->Caption = "Ulo�it";
 //if(text=="m].</b>")text="";//provizorn� WA, p�i zm�n� Rz a byla-li v po��dku to vrac� toto  - ji� nen� t�eba, ale zat�m nech�v�m
		if (text != "") // zobraz� a vyp�e
		{
				rHTMLHint1->ToString()=text;//nate�en� do hintu zaji��uje zobrazen� cel�ho textu, nepou��v� se klasick� hint
				//prodllu�en� formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl p�edt�m popisek skryt� + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnut� podtr�en�ho p�sma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
						Button_save->Enabled=false;  //R - do�asn� povolen� ukl�d�n� p�i validaci
						rHTMLLabel_InfoText->Font->Color = clRed;
            rHTMLLabel_InfoText->Color=clWhite;
				}
				else
				{
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
				rHTMLLabel_InfoText->Left = 10;
				rHTMLLabel_InfoText->Top = Button_storno->Top - Button_storno->Height  + 5;
				rHTMLLabel_InfoText->Caption = text;
				rHTMLLabel_InfoText->Visible = true;
        rHTMLLabel_InfoText->Color=clWhite;
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
     F->log(__func__); //logov�n�
		 input_clicked_edit=TT_klik;
		 input_clicked_icon=empty_klik_ico;
		// Nastav_zamky(empty_klik_ico,TT_klik);

		 Form_TT_kalkulator->Left=Form1->ClientWidth/2-Form_TT_kalkulator->Width/2;
		 Form_TT_kalkulator->Top=Form1->ClientHeight/2-Form_TT_kalkulator->Height/2;

		 //p�ekreslen� podbarven� sloupc�
//		 rStringGridEd_tab_dopravniky->Visible=false;
//		 rStringGridEd_tab_dopravniky->Visible=true;

	//	 vypis("Pozor, p�i zm�n� taktu dojde p�i ulo�en� ke zm�n� hodnot aktu�ln� rychlosti pohonu nebo rozte�ov� vzd�lenosti a dal��ch parametr� dle nastaven�ch z�mk� v tabulce pohon�. ",false);
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::Nastav_zamky(Tinput_clicked_icon I,Tinput_clicked_edit E)
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TForm_parametry_linky::input_TT()

{
  F->log(__func__); //logov�n�
	input_state=TT;
	INPUT(0,0);   // p�i vol�n� INPUT z TT je sou��st� rovnou i vol�n� OUTPUT + vol�n� v�po�etn�ho modelu
	input_state=NOTHING;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TForm_parametry_linky::INPUT(double Sloupec, double Radek)
{

}

void TForm_parametry_linky::OUTPUT(double i, double Sloupec, double Radek)
{

}


void TForm_parametry_linky::Roletka_roztec(int Row)

{
//					double Rz=0.0;
//											 //pokud Rz nen� pr�zdn�, ulo��m si jeho hodnotu a pou�iju k p�ed�n�.
//											 //jinak nastav�m Rz na -1 tzn., �e do roletky zobraz�m v�echny rozte�e z katalogu
//					if(!rStringGridEd_tab_dopravniky->Cells[6][Row].IsEmpty())
//					{
//					Rz=F->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[6][Row]);
//					}  else Rz=-1;
//
//					AnsiString	data;
//				 if(Runit==MM) data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",true);
//				 else          data=Form1->d.v.vypis_retezy_s_pouzitelnou_rozteci(Rz,"",";",false);
//         // Memo3->Lines->Clear();
//         // Memo4->Lines->Clear();
//          Memo3->Lines->Add(data);
//          Memo4->Lines->Add(Rz);
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Clear();
//				TStringList *S=new TStringList;
//				S->Add(data);
//				S->StrictDelimiter=true;  //https://stackoverflow.com/questions/1335027/delphi-stringlist-delimiter-is-always-a-space-character-even-if-delimiter-is-se
//				S->Delimiter=';';     //nutno v jednoduch�ch uvozovk�ch, dvoj� hod� chybu p�i p�ekladu
//				S->DelimitedText=data;
//				S->DelimitedText=data;
//
//				rStringGridEd_tab_dopravniky->Columns->Items[5]->PickList->Assign(S); //Standartn� se pou��v� Add(), ale v tomto p��pad� Assign()
//				//zobrazOramovani=true;
//				//F->m.frameRect(Rect(200,100,60,80),clBlue,2);

		}

double  TForm_parametry_linky::getTT()
{
  F->log(__func__); //logov�n�
	double TT=0;
	if(Taktunit==S) TT=rEditNum_takt->Value;
	else            TT=rEditNum_takt->Value*60.0;
	return TT;
}

////////////////////////////////////////////////////////////////////////////////

void TForm_parametry_linky::VALIDACE(int ACol,int ARow)
{
    F->log(__func__); //logov�n�
    //vypis("");
    VID=-1;
    Row_validace=0;
    Col_validace=0;


 switch(ACol)
	 {

				case 2:     //OD - RD
				{
				double RD   = F->ms.MyToDouble(mGrid->Cells[4][ARow].Text)/(1+59.0*aRDunit);
		    double P_od = F->ms.MyToDouble(mGrid->Cells[2][ARow].Text)/(1+59.0*aRDunit);
				double P_do = F->ms.MyToDouble(mGrid->Cells[3][ARow].Text)/(1+59.0*aRDunit);

					if(Form1->m.between(RD,P_od,P_do))
						{
						mGrid->ShowNote("");
						scGPGlyphButton_ADD->Visible=true;
						Button_save->Enabled=true;
						}
						else
						{
              //vypis("Nastavte spr�vn� rozsah a rychlost pohonu.");
						 mGrid->ShowNote("Nastavte spr�vn� rozsah a rychlost pohonu.",clRed,13);
							VID=23;
							Row_validace=ARow;
							Col_validace=ACol;
							scGPGlyphButton_ADD->Visible=false;
							Button_save->Enabled=false;
						}
					if(mGrid->Cells[3][ARow].Text!="")
					{                        //do = 4 spodn� nesm� b�t men�� ne� 1
						AnsiString jednotky;
						if(aRDunit==0)jednotky="m/s";else jednotky="m/min";
						if(P_od<P_do/4)//chybn� zad�n�
						{
							mGrid->ShowNote("Spodn� hranice rychlosti mus� b�t v�t�� nebo rovna "+AnsiString(P_do/4*(1+59.0*aRDunit))+" "+jednotky+".",clRed,13);
							VID=23;
							Row_validace=ARow;
							Col_validace=ACol;
							scGPGlyphButton_ADD->Visible=false;
							Button_save->Enabled=false;
						}
						else
						{
            	mGrid->ShowNote("");
							scGPGlyphButton_ADD->Visible=true;
							Button_save->Enabled=true;
            }
					}
				}
				break;

        case 3:     //DO - RD
				{
        double RD   = F->ms.MyToDouble(mGrid->Cells[4][ARow].Text)/(1+59.0*aRDunit);
		    double P_od = F->ms.MyToDouble(mGrid->Cells[2][ARow].Text)/(1+59.0*aRDunit);
				double P_do = F->ms.MyToDouble(mGrid->Cells[3][ARow].Text)/(1+59.0*aRDunit);

					if(Form1->m.between(RD,P_od,P_do))
						{
             mGrid->ShowNote("");
						 scGPGlyphButton_ADD->Visible=true;
						 Button_save->Enabled=true;
						}
						else
						{
             // vypis("Nastavte spr�vn� rozsah a rychlost pohonu.");
              mGrid->ShowNote("Nastavte spr�vn� rozsah a rychlost pohonu.",clRed,13);
              VID=23;
              Row_validace=ARow;
              Col_validace=ACol;
							scGPGlyphButton_ADD->Visible=false;
							Button_save->Enabled=false;
						}
					if(mGrid->Cells[2][ARow].Text!="")
					{                        //do = 4 spodn� nesm� b�t men�� ne� 1
						AnsiString jednotky;
						if(aRDunit==0)jednotky="m/s";else jednotky="m/min";
						if(P_do>P_od*4)//chybn� zad�n�
						{
							mGrid->ShowNote("Horn� hranice rychlosti mus� b�t men�� nebo rovna "+AnsiString(P_od*4*(1+59.0*aRDunit))+" "+jednotky+".",clRed,13);
							VID=23;
							Row_validace=ARow;
							Col_validace=ACol;
							scGPGlyphButton_ADD->Visible=false;
							Button_save->Enabled=false;
						}
						else
						{
            	mGrid->ShowNote("");
							scGPGlyphButton_ADD->Visible=true;
							Button_save->Enabled=true;
            }
					}
				}
				break;

        case 4:     //aRD - RD
				{
        double RD   = F->ms.MyToDouble(mGrid->Cells[4][ARow].Text)/(1+59.0*aRDunit);
		    double P_od = F->ms.MyToDouble(mGrid->Cells[2][ARow].Text)/(1+59.0*aRDunit);
				double P_do = F->ms.MyToDouble(mGrid->Cells[3][ARow].Text)/(1+59.0*aRDunit);

					if(Form1->m.between(RD,P_od,P_do))
						{
            mGrid->ShowNote("");
						scGPGlyphButton_ADD->Visible=true;
						Button_save->Enabled=true;
						}
						else
						{
              //vypis("Nastavte spr�vn� rozsah a rychlost pohonu.");
              mGrid->ShowNote("Nastavte spr�vn� rozsah a rychlost pohonu.",clRed,13);
              VID=23;
              Row_validace=ARow;
              Col_validace=ACol;
							scGPGlyphButton_ADD->Visible=false;
							Button_save->Enabled=false;
						}
				}
				break;
	 }
}


void __fastcall TForm_parametry_linky::scGPGlyphButton_TTClick(TObject *Sender)
{

  F->log(__func__); //logov�n�
	bool Changes_TT=false;
	Form_TT_kalkulator->Left=Form1->ClientWidth/2-Form_TT_kalkulator->Width/2;
	Form_TT_kalkulator->Top=Form1->ClientHeight/2-Form_TT_kalkulator->Height/2;
  scGPButton_vozik->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
  scGPButton_vozik->Options->PressedColor=F->m.clIntensive(this->Color,8);
  scGPButton_obecne->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
  scGPButton_obecne->Options->PressedColor=F->m.clIntensive(this->Color,8);
  scGPButton_pohon->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
  scGPButton_pohon->Options->PressedColor=F->m.clIntensive(this->Color,8);

  this->Color=F->m.clIntensive(this->Color,8);//zesv�tlen� spodn�ho formu


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
      scGPButton_vozik->Options->FramePressedColor=F->m.clIntensive(Form_parametry_linky->Color,-8);
      scGPButton_vozik->Options->PressedColor=F->m.clIntensive(Form_parametry_linky->Color,-8);
      scGPButton_obecne->Options->FramePressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_obecne->Options->PressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_pohon->Options->FramePressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_pohon->Options->PressedColor=F->m.clIntensive(this->Color,-8);
    	this->Color=F->m.clIntensive(this->Color,-8);//navr�cen� do p�vodn� hodnoty
     // scGPButton_vozik->Options->PressedColor=F->m.clIntensive(this->Color,-8);
      }
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::sc(TObject *Sender)

{
    F->log(__func__); //logov�n�
		bool Changes_vozik=false;
		Form_parametry_vozik->Left=Form1->ClientWidth/2-Form_parametry_vozik->Width/2;
		Form_parametry_vozik->Top=Form1->ClientHeight/2-Form_parametry_vozik->Height/2;

      scGPButton_vozik->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
      scGPButton_vozik->Options->PressedColor=F->m.clIntensive(this->Color,8);
      scGPButton_obecne->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
      scGPButton_obecne->Options->PressedColor=F->m.clIntensive(this->Color,8);
      scGPButton_pohon->Options->FramePressedColor=F->m.clIntensive(this->Color,8);
      scGPButton_pohon->Options->PressedColor=F->m.clIntensive(this->Color,8);

    this->Color=F->m.clIntensive(this->Color,8);//zesv�tlen� spodn�ho formu

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

		if(Changes_vozik && Form1->d.v.OBJEKTY->dalsi!=NULL /*&& Form1->d.v.POHONY->dalsi!=NULL*/)//pri zmene voziku
		{
			F_gapoV->ShowModal();
      if(F_gapoV->myModalResult==mrOk)
      {
       Ulozit=true;
       Close();
      }
		}

      scGPButton_vozik->Options->FramePressedColor=F->m.clIntensive(Form_parametry_linky->Color,-8);
      scGPButton_vozik->Options->PressedColor=F->m.clIntensive(Form_parametry_linky->Color,-8);
      scGPButton_obecne->Options->FramePressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_obecne->Options->PressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_pohon->Options->FramePressedColor=F->m.clIntensive(this->Color,-8);
      scGPButton_pohon->Options->PressedColor=F->m.clIntensive(this->Color,-8);

    this->Color=F->m.clIntensive(this->Color,-8);//zesv�tlen� spodn�ho formu
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button2Click(TObject *Sender)
{
//rStringGridEd_tab_dopravniky->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::rHTMLLabel_InfoTextClick(TObject *Sender)
{
//		if(VID==4 || VID==5 || VID==6 || VID==7)
//		{
//		 rStringGridEd_tab_dopravniky->Cells[VID][Row_validace]=VID_value;
//		 INPUT(VID,Row_validace);
//     if(VID==7) pm.input_Rx();
//     if(VID==6) pm.input_Rz();
//     if(VID==5) pm.input_R();
//     if(VID==4) pm.input_aRD();
//		 OUTPUT(0,VID,Row_validace);
//		 rStringGridEd_tab_dopravniky->Invalidate();
//		}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  F->log(__func__); //logov�n�
	if(Ulozit || Storno)CanClose=true;
	else CanClose=false;
}
//---------------------------------------------------------------------------

//metody volan� z Tmgrid
void TForm_parametry_linky::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
  F->log(__func__); //logov�n�
//
//	ShowMessage("PL\nDo�lo ke kliku v tabulce tag formu: "+AnsiString(Tag)+", ID tabulky: "+AnsiString(ID)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
//	mGrid->HighlightCell(Col,Row);
//	mGrid->Cells[0][0].Text="test";
//	FormPaint(this);//zajist� p�ekreslen� bez probliku
//	//toto problik�v� mGrid->Refresh();
COL=Col;
ROW=Row;
		 bool smazat=false;
		 if(input_state==NOTHING)
		 {
				if(Col==6 && Row>=2)
				{   input_state=JOB;
						if(mGrid->getCheck(6,Row)->Checked==false)
            {
								if(mrOk==Form1->MB("Pohon je pou��v�n�, opravdu m� b�t zru�eno p�i�azen�?",MB_OKCANCEL))
								{
								 Button_save->SetFocus();
								 TscGPCheckBox *CH=mGrid->getCheck(6,Row);CH->Free();CH=NULL;delete CH;
//								 mGrid->getCheck(6,Row)->Enabled=false;
								 mGrid->Cells[7][Row].Text="";
                 //nastav�m typy bun�k ��m� povol�m okam�it� editaci pohonu, proto�e u� nen� pou��v�n
                 mGrid->Cells[1][Row].Type=mGrid->EDIT;
                 mGrid->Cells[2][Row].Type=mGrid->EDIT;
                 mGrid->Cells[3][Row].Type=mGrid->EDIT;
                 mGrid->Cells[4][Row].Type=mGrid->EDIT;
								 mGrid->Cells[5][Row].Type=mGrid->COMBOEDIT;
								 mGrid->Cells[6][Row].Type=mGrid->DRAW;mGrid->Cells[6][Row].RightBorder->Color=clWhite;

                 mGrid->Cells[1][Row].Background->Color=clWhite;
                 mGrid->Cells[2][Row].Background->Color=  mGrid->Cells[1][Row].Background->Color;
                 mGrid->Cells[3][Row].Background->Color=  mGrid->Cells[1][Row].Background->Color;
                 mGrid->Cells[4][Row].Background->Color=  mGrid->Cells[1][Row].Background->Color;
								 mGrid->Cells[5][Row].Background->Color=  mGrid->Cells[1][Row].Background->Color;
								 mGrid->Cells[7][Row].Background->Color=  mGrid->Cells[1][Row].Background->Color;

								 mGrid->Refresh();
                 zrusena_prirazeni_PID[getPID(Row)-1]=true;//nahrazeno novou filozofii, z d�vodu mo�n�ho storna formu
				         smazat=true;
								}  else {mGrid->getCheck(6,Row)->Checked=true;Button_save->SetFocus();}
						}
        }

        if(Col==2 && Row==0)
        {
         if(aRDunit==MIN)	mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/min]</a>";
         else             mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/s]</a>";
        }

				if(Col==5 && Row==1)
        {
				 if(Runit==MM) mGrid->Cells[5][1].Text="<a>[mm]</a>";
				 else          mGrid->Cells[5][1].Text="<a>[m]</a>";
        }



        if(Col==8 && Row>=2)
				{
          input_state=JOB;
          int ROW=Row;
					smazat=false;
          if(Form1->d.v.pohon_je_pouzivan(getPID(ROW)))//pohon je pou��van�
          {
              AnsiString objekty=Form1->d.v.vypis_objekty_vyuzivajici_pohon(getPID(ROW),true);
              myMessageBox->zobrazitFrameForm=true;//zajist� or�mov�n� MB
              if(mrYes==Form1->MB("Pohon je pou��v�n objekty: <b>"+objekty+"</b>. Opravdu m� b�t pohon smaz�n?",MB_YESNO))
              {
                //Form1->d.v.zrusit_prirazeni_pohunu_k_objektum(getPID(ROW)); p�vodn� p��m� smaz�n�, ale nereflektovalo by p��padn� storno
                //pozor nen� p�ipraveno na situaci, pokud by bylo mo�n� p��mo v PL p�i�azovan pohony a potom zase odp�i�azovat (muselo by se nav��it pole zrusena_prirazeni_PID)
								zrusena_prirazeni_PID[getPID(ROW-1)]=true;//nahrazeno novou filozofii, z d�vodu mo�n�ho storna formu
								smazat=true;
              }
              myMessageBox->zobrazitFrameForm=false;//zajist� odor�mov�n� MB - kv�li dal��mu pou�it�
          }
          else//pohon nen� pou��van� a m��eme tedy smazat rovnou
          {
						smazat=true;
          }

      //samotn� smaz�n� ��dku + zajist� sn�en� po�tu ��dk� + nesm� se p�eindexov�vat!!! kv�li metod�m, kter� sahaj� do spoj�ku POHONY
         if(smazat)
          {
					Button_save->SetFocus(); //EXTREMNE DULEZITE, JINAK PAMETOVA CHYBA PRI ODSTRANOVANI ROW
					mGrid->DeleteRow(ROW);
          //nastaveni rozmeru formu - dle poctu pohonu a nove pozice Add buttonu
          setADD_ButtonPosition();
          setFormHeight();
          vykresli_obdelnik_vpravo(); //workaround
          if(mGrid->RowCount<3) {   mGrid->Columns[7].Width=190+30; mGrid->Columns[8].Width=1;mGrid->Refresh();}
          }
			 }
			 input_state=NOTHING;//uvol�ov�n� po v�ech ud�lostech, nefungovalo po kliku na CheckBox
    }

}

void TForm_parametry_linky::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
void TForm_parametry_linky::OnChange(long Tag,unsigned long Col,unsigned long Row)
{
 F->log(__func__); //logov�n�
 if(Col>1 && Col<=4 && input_state==NOTHING )
    {
       VALIDACE(Col,Row);
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
 F->log(__func__); //logov�n�

   if(Button==mbLeft && mGrid->CheckPTinTable(X,Y))      //p�epnut� jednotek
   {

      TPoint RET=mGrid->CheckLink(X,Y);
      if(RET.x==4)
      {
       if(aRDunit==MIN) {	mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/s]</a>";   aRDunit=S;      }
       else             { mGrid->Cells[2][0].Text="Rozmez� a rychlost pohonu <a>[m/min]</a>"; aRDunit=MIN;    }

           if(aRDunit==MIN)
           {
               for(unsigned int i=2;i<mGrid->RowCount;i++)
               {
               mGrid->Cells[2][i].Text=F->ms.MyToDouble(mGrid->Cells[2][i].Text)*60.0;
               mGrid->Cells[3][i].Text=F->ms.MyToDouble(mGrid->Cells[3][i].Text)*60.0;
               mGrid->Cells[4][i].Text=F->ms.MyToDouble(mGrid->Cells[4][i].Text)*60.0;
               }
           }
           else
           {
               for(unsigned int i=2;i<mGrid->RowCount;i++)
               {
               mGrid->Cells[2][i].Text=F->ms.MyToDouble(mGrid->Cells[2][i].Text)/60.0;
               mGrid->Cells[3][i].Text=F->ms.MyToDouble(mGrid->Cells[3][i].Text)/60.0;
               mGrid->Cells[4][i].Text=F->ms.MyToDouble(mGrid->Cells[4][i].Text)/60.0;
               }
           }
       mGrid->MergeCells(2,0,4,0);

      }
      if(RET.x==5)  //ZAT�M NEN� ZCELA F�N�, proto�e ukl�d�n� do katalogu nen� v z�kl. SI jednotk�ch
      {
					 if(Runit==MM) {mGrid->Cells[5][1].Text="<a>[m]</a>";   Runit=M;  }
					 else          {mGrid->Cells[5][1].Text="<a>[mm]</a>";  Runit=MM; }

           if(Runit==MM)
           {
               for(unsigned int i=2;i<mGrid->RowCount;i++)
               {
                TscGPComboEdit *C=mGrid->getComboEdit(5,i);
                TscGPListBoxItem *I;
                double value=F->ms.MyToDouble(mGrid->getComboEdit(5,i)->Text);
                C->Items->Clear();
                I=C->Items->Add();
                I->Caption=value*1000.0;
                C->ItemIndex=0;
                getROtherValues(Runit,i);
               }

           }
           else
           {
               for(unsigned int i=2;i<mGrid->RowCount;i++)
               {
                TscGPComboEdit *C=mGrid->getComboEdit(5,i);
                TscGPListBoxItem *I;
                double value=F->ms.MyToDouble(mGrid->getComboEdit(5,i)->Text);
                C->Items->Clear();
                I=C->Items->Add();
                I->Caption=value/1000.0;
                C->ItemIndex=0;
                getROtherValues(Runit,i);
               }

           }
//            mGrid->MergeCells(5,0,5,1);
              mGrid->Refresh();
      }
       Button_storno->SetFocus();
       mGrid->Refresh();
   }
}
//---------------------------------------------------------------------------
void TForm_parametry_linky::getmGridColors()
  {
  F->log(__func__); //logov�n�
  mGrid->Cells[0][0].Font->Color=clBlack;//F->m.clIntensive(clBlack,80);
  mGrid->Cells[1][0].Font->Color=clBlack;//F->m.clIntensive(clBlack,50);
  mGrid->Cells[2][0].Font->Color= mGrid->Cells[1][0].Font->Color;
  mGrid->Cells[3][0].Font->Color= mGrid->Cells[1][0].Font->Color;
  mGrid->Cells[4][0].Font->Color= mGrid->Cells[1][0].Font->Color;
	mGrid->Cells[5][0].Font->Color= mGrid->Cells[1][0].Font->Color;mGrid->Cells[5][1].Font->Color= mGrid->Cells[1][0].Font->Color;
	mGrid->Cells[6][0].Font->Color= mGrid->Cells[1][0].Font->Color;
  mGrid->Cells[7][0].Font->Color= mGrid->Cells[1][0].Font->Color;
	mGrid->Cells[8][0].Font->Color= mGrid->Cells[1][0].Font->Color;

  mGrid->Cells[2][1].Font->Color= mGrid->Cells[1][0].Font->Color;
  mGrid->Cells[3][1].Font->Color= mGrid->Cells[1][0].Font->Color;
  mGrid->Cells[4][1].Font->Color= mGrid->Cells[1][0].Font->Color;

  mGrid->Cells[8][0].RightBorder->Color=Form_parametry_linky->Color;
  mGrid->Cells[7][0].RightBorder->Width=1;
  mGrid->Cells[8][1].RightBorder->Color=mGrid->Cells[8][0].RightBorder->Color;
	mGrid->Cells[8][0].TopBorder->Color=mGrid->Cells[8][0].RightBorder->Color;

	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
  mGrid->Cells[1][0].Background->Color=clBACKGROUND;
  mGrid->Cells[2][0].Background->Color=clBACKGROUND;
  mGrid->Cells[3][0].Background->Color=clBACKGROUND;
  mGrid->Cells[4][0].Background->Color=clBACKGROUND;
	mGrid->Cells[5][0].Background->Color=clBACKGROUND;mGrid->Cells[5][1].Background->Color=clBACKGROUND;
  mGrid->Cells[6][0].Background->Color=clBACKGROUND;
  mGrid->Cells[7][0].Background->Color=clBACKGROUND;
  mGrid->Cells[8][0].Background->Color=Form_parametry_linky->Color;
 // mGrid->Cells[8][0].

  mGrid->Cells[6][0].TopMargin=-11;

  mGrid->Cells[2][1].Background->Color=clBACKGROUND;
  mGrid->Cells[3][1].Background->Color=clBACKGROUND;
  mGrid->Cells[4][1].Background->Color=clBACKGROUND;
  //fale�n� zalamov�n� textu na bu�k�ch 5,0 a 5,1
	mGrid->Cells[5][0].BottomBorder->Color=clBACKGROUND;
  }
//---------------------------------------------------------------------------
  void TForm_parametry_linky::getmGridWidth()
  {
  F->log(__func__); //logov�n�
  mGrid->Columns[0].Width=30;
  mGrid->Columns[0].Visible=false;
  mGrid->Columns[1].Width=220;
  mGrid->Columns[2].Width=100;
  mGrid->Columns[3].Width=100;
  mGrid->Columns[4].Width=100;
  mGrid->Columns[5].Width=100;
  mGrid->Columns[6].Width=30;
  mGrid->Columns[7].Width=190;
  mGrid->Columns[8].Width=30;
  }
//---------------------------------------------------------------------------
  void TForm_parametry_linky::getDeleteButtonSettings(int Row)
  {
  F->log(__func__); //logov�n�
  TscGPGlyphButton *H=mGrid->getGlyphButton(8,Row);

  H->GlyphOptions->Kind=scgpbgkCancel;
  H->GlyphOptions->Thickness=1;
  H->Options->NormalColor=clWhite;
  H->Options->NormalColorAlpha=255;
  H->Options->FrameWidth=1;
  H->Options->FrameNormalColor=clWhite;
  H->Width=scGPGlyphButton_smazat_pohon->Width;
  H->Height=scGPGlyphButton_smazat_pohon->Height;
  H->Options->ShapeStyle=scgpRect;
  H->Width=30;
  H->Hint="Smazat tento pohon";
  H->ShowHint=true;

  H=NULL;delete H;
  }

  void TForm_parametry_linky::getPrirazeneObjDesign(int Row)
  {

  TscGPButton *CH=mGrid->getButton(7,Row);

  CH->ImageIndex=-1;
  CH->ShowCaption=true;
  CH->Caption=mGrid->Cells[7][Row].Text;

  CH->ShowHint=true;
  CH->Options->NormalColor=clWhite;
  CH->Options->NormalColorAlpha=255;
  CH->Options->FrameWidth=1;
  CH->WordWrap=false;
  CH->Margin=0;
  CH->Hint=OBJEKTY_POUZIVAJICI_POHON;
  CH->Options->FontNormalColor= clGlyph;
  CH->Options->FontFocusedColor= clGlyph;
  CH->Options->FontHotColor= clGlyph;
  CH->Options->FontDisabledColor=clGlyph;
  CH->Options->NormalColor=clWhite;
  CH->Options->FocusedColor=CH->Options->NormalColor;
  CH->Options->HotColor=CH->Options->NormalColor;
  CH->Options->PressedColor=CH->Options->NormalColor;
  CH->Options->FrameNormalColor=CH->Options->NormalColor;
  CH->Options->PressedColor=CH->Options->NormalColor;
  CH->Options->FramePressedColor=CH->Options->NormalColor;
  CH->Options->FocusedColor=clWhite;
  CH->Options->FocusedColorAlpha=255;
  CH->Options->FrameFocusedColor=clWhite;
  CH->CanFocused=false;
 // CH->Down=true;
  CH=NULL;delete CH;
  }

  void TForm_parametry_linky::setADD_ButtonPosition ()
  {
  F->log(__func__); //logov�n�
  scGPGlyphButton_ADD->Top=mGrid->Top+mGrid->Height + 1;
  scGPGlyphButton_ADD->Left=mGrid->Left;
  scGPGlyphButton_ADD->Width=26;//mGrid->Columns[0].Width+1;
  scGPGlyphButton_ADD->Height=mGrid->DefaultRowHeight;
  }

	void TForm_parametry_linky::setFormHeight()
  {
   F->log(__func__); //logov�n�
   Form_parametry_linky->Height=mGrid->Top + mGrid->RowCount*mGrid->DefaultRowHeight + 80;
	 Button_save->Top=Form_parametry_linky->Height-11-Button_save->Height;//Form_parametry_linky->Height - 40;
	 Button_storno->Top=Button_save->Top;//Form_parametry_linky->Height - 40;
   scGPGlyphButton_DEL_nepouzite->Top=Button_save->Top;
   scGPGlyphButton_DEL_nepouzite->Left=mGrid->Columns[8].Left - 5; //minus kvuli oramovani buttonu, kter� se zobraz� p�i najet� my��

  scGPGlyphButton_OPTIONS->Top=mGrid->Top + mGrid->Height ;
  scGPGlyphButton_OPTIONS->Left=Button_save->Left + Button_save->Width + 350;

  }

  void __fastcall TForm_parametry_linky::Button1Click(TObject *Sender)
  {
  F->log(__func__); //logov�n�
  //automatick� vygenerov�n� pohon�
  for (int i = mGrid->RowCount ; i <= 4; i++) {

  input_state=JOB;
  mGrid->AddRow(true,false);

	mGrid->Cells[0][i].Text = getMaxPID()+1;//mGrid->RowCount - 2;
	mGrid->Cells[1][i].Text = "nov� pohon " + AnsiString(i-1);

   getmGridWidth();
   mGrid->Cells[1][i].Type=mGrid->EDIT;
   mGrid->Cells[2][i].Type=mGrid->EDIT;
   mGrid->Cells[3][i].Type=mGrid->EDIT;
   mGrid->Cells[4][i].Type=mGrid->EDIT;
   mGrid->Cells[5][i].Type=mGrid->EDIT;
   mGrid->Cells[6][i].Type=mGrid->CHECK;
   mGrid->Cells[7][i].Type=mGrid->BUTTON;

	 mGrid->Cells[2][i].InputNumbersOnly=2;
	 mGrid->Cells[3][i].InputNumbersOnly=2;
	 mGrid->Cells[4][i].InputNumbersOnly=2;
	 mGrid->Cells[5][i].InputNumbersOnly=2;

   if(i==2){
    mGrid->Cells[2][i].Text="0,2";
    mGrid->Cells[3][i].Text="5";
    mGrid->Cells[4][i].Text="0,5";
    mGrid->Cells[5][i].Text="250";
    }

    if(i>=3)
    {
    mGrid->Cells[2][i].Text=mGrid->Cells[2][i-1].Text * AnsiString(i-1);
    mGrid->Cells[3][i].Text=mGrid->Cells[3][i-1].Text * AnsiString(i-1);
    mGrid->Cells[4][i].Text=mGrid->Cells[4][i-1].Text * AnsiString(i-1);
    mGrid->Cells[5][i].Text=mGrid->Cells[5][i-1].Text * AnsiString(i-1);
    }

	 mGrid->Cells[8][i].Type=mGrid->glyphBUTTON;

  mGrid->Refresh();
  mGrid->getCheck(6,i)->Enabled=false;
  mGrid->getCheck(6,i)->ShowHint=true; mGrid->getCheck(6,i)->Hint="Zru�it p�i�azen� k objekt�m";
  getDeleteButtonSettings(i);
  getPrirazeneObjDesign(i);
  setADD_ButtonPosition();
  setFormHeight();
  //R - zakoment scGPGlyphButton_DEL_nepouzite->Visible=true;

  }
	input_state=NOTHING;
	vykresli_obdelnik_vpravo();

  }
//---------------------------------------------------------------------------


void TForm_parametry_linky::OnKeyPress(TObject *Sender, System::WideChar &Key)

{

}

void TForm_parametry_linky::vykresli_obdelnik_vpravo()
{
    F->log(__func__); //logov�n�
     	 //	workaround - zru�en� or�mov�n� okolo nepou�it�ch vn�j��ch bun�k
		Canvas->Pen->Width=2;
		Canvas->Pen->Color=Form_parametry_linky->Color;//(TColor)RGB(240,240,240);

		Canvas->MoveTo(mGrid->Left+mGrid->Columns[8].Left+1,mGrid->Top);
		Canvas->LineTo(mGrid->Left+mGrid->Columns[8].Left+mGrid->Left+mGrid->Columns[8].Width-10,mGrid->Top);


   	Canvas->MoveTo(mGrid->Left+mGrid->Columns[8].Left+mGrid->Left+mGrid->Columns[8].Width-10,mGrid->Top+2*mGrid->DefaultRowHeight);
    Canvas->LineTo(mGrid->Left+mGrid->Columns[8].Left+mGrid->Left+mGrid->Columns[8].Width-10,mGrid->Top-1);
}


void __fastcall TForm_parametry_linky::scGPGlyphButton_katalogClick(TObject *Sender)
{
    if(Form_katalog->ShowModal()==mrOk)
    {
     Form_parametry_linky->Visible=false;
     Form_parametry_linky->Visible=true;
      setADD_ButtonPosition();
      setFormHeight();
      vykresli_obdelnik_vpravo();
    }
    else {Form_parametry_linky->Visible=false;Form_parametry_linky->Visible=true;}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPComboEdit12Click(TObject *Sender)
{
ShowMessage("Click");
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPComboEdit12DropDown(TObject *Sender)
{
ShowMessage("DropDown");
}
//---------------------------------------------------------------------------
void TForm_parametry_linky::getROtherValues(Tm_mm Runit, int Row)
{
//ShowMessage(Runit);

            TscGPComboEdit *C=mGrid->getComboEdit(5,Row);
            TscGPListBoxItem *I;

           //na�ten� ostatn�ch hodnot z katalogu do roletky
           Cvektory::Ttyp_dopravniku *K=F->d.v.vrat_typ_dopravniku(F->d.v.PP.katalog);
           Cvektory::TDoubleHodnota *H=K->roztec->dalsi;
           C->Items->Clear();
            while(H!=NULL)
           {
            I=C->Items->Add();
             if(Runit==MM) I->Caption=H->hodnota;
             else I->Caption=H->hodnota/1000.0;

            H=H->dalsi;
           }
            I=NULL;delete I;
}
void __fastcall TForm_parametry_linky::scGPTrackBar_uchyceniMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y)
{
 scGPTrackBar_uchyceni->Hint=scGPTrackBar_uchyceni->Value;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::scGPNumericEdit_delka_podvozekChange(TObject *Sender)

{
  scGPTrackBar_uchyceni->MaxValue=scGPNumericEdit_delka_podvozek->Value;/*/(1+999*Delkaunit);  */
}
//---------------------------------------------------------------------------

