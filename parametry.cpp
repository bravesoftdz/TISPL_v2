//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "parametry.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
__fastcall TForm_parametry::TForm_parametry(TComponent* Owner)
	: TForm(Owner)
{
  //nové nastavení barvy formuláøe
	Form_parametry->Color=(TColor)RGB(240,240,240);

	//nastavení zvýraznìní
	hl_color=clRed;//(TColor)RGB(255,141,28);//barva zvýraznìní rámeèku komponenty napø.pro povinné položky
	hlFrameWidth=2;//šíøka zvýraznìní rámeèku komponenty napø.pro povinné položky

	//pøevzetí defaultní velikosti formuláøe z designu pro další užití (slouží spíše pro usnadnìní vývoje, než funkènì)
	defaultForm_parametryHeight=Form_parametry->Height;

	//matamaticky exaktní napozicování tlaèítek OK a storno
	Form1->m.designButton(scGPButton_OK,Form_parametry,1,2);
	Form1->m.designButton(scGPButton_storno,Form_parametry,2,2);

	//výchozí nastavení zámkù
	CT_zamek=UNLOCKED;
	RD_zamek=LOCKED;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	//formuláø bude pøi prvním zobrazení v sekundách a metrech nebo dle INI v odvozených jednotkách, jinak dle SI
	minsec=S;CTunit=S;RDunitT=S;
	m_mm=M;DDunit=M;DMunit=M;RDunitD=M;
	//GLOBAL
	if(Form1->readINI("nastaveni_form_parametry","cas")=="1"){minsec=MIN;scGPButton_min_sec->Caption="vše na s";}//tedy MIN
	if(Form1->readINI("nastaveni_form_parametry","vzdalenost")=="1"){m_mm=MM;scGPButton_metry_milimetry->Caption="vše na m";}//tedy MM
	//CT
	if(Form1->readINI("nastaveni_form_parametry","CT")=="1") rHTMLLabel_CTClick(this);//pøevede na min tzn. CTunit=MIN;
	//RD
	if(Form1->readINI("nastaveni_form_parametry","RDt")=="1")	rHTMLLabel_RDClick(this);//pøevede na min tzn. RDunitT=MIN;
	if(Form1->readINI("nastaveni_form_parametry","RDd")=="1")	rHTMLLabel_RDClick(this);//pøevede na mm tzn. RDunitD=MM;
	//DD
	if(Form1->readINI("nastaveni_form_parametry","DD")=="1")	rHTMLLabel_delka_dopravnikuClick(this);//pøevede na mm tzn. DDunit=MM;
	//DM
	if(Form1->readINI("nastaveni_form_parametry","DM")=="1")	rHTMLLabel_mezeraClick(this);//pøevede na mm tzn. DMunit=MM;

	//nastavení defaultních hodnot
	if(scGPNumericEdit_CT->Value==0)//if(d.v.ZAKAZKY->dalsi!=NULL)//pokud existuje první zakázka
	{
		scGPNumericEdit_CT->Value=Form1->d.v.PP.TT/(1+59*CTunit);//již nepoužíváme, protože se u první zakázky uvažuje globální TT,d.v.ZAKAZKY->dalsi->TT;
		scGPNumericEdit_kapacita->Value=1;
	}

	input_state=NOTHING;//nutnost!!!
	kapacitaSG=1;//není podnìt k rozkládání na více objektù
	scGPEdit_name->SetFocus();//nastaví výchozí focus, kde se pøedpokládá výchozí nastavování
	scGPEdit_name->SelectAll();//oznaèí cele pro editace
	form_zobrazen=true;//detekuje zda je form aktuálnì zobrazen, slouží proto aby pøi zmìnì combo režim pokud si nastavil uživatel formulaø jinam, aby zùstal nastaven dle uživatele
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry::vypis(UnicodeString text,bool RED)
{
	if(text!="")//zobrazí a vypíše
	{
		//if(!rHTMLLabel_InfoText->Visible)Form_parametry->Height+=(40+19);//pouze pokud byl popisek skrytý
		if(RED)
		{
			scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor=clRed;
			rHTMLLabel_InfoText->Font->Color=clRed;
      //zvýraznìní položky technologický èas
			if(text.Pos("technologický èas") && scGPNumericEdit_CT->Enabled)
			{/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;*/}
			else
			{/*ROSTA-stylscGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;*/}
		}
		else
		{
			scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor=(TColor)RGB(0,128,255);
			rHTMLLabel_InfoText->Font->Color=(TColor)RGB(0,128,255);
		}
		scGPGlyphButton_InfoIcon->Top=Form_parametry->Height-81;
		if(text.Length()<=35)//v pøípadì, že je text delší než 35 znakù skryje ikonu u zvolí nové levé odsazení textu
		{
			scGPGlyphButton_InfoIcon->Visible=true;
			rHTMLLabel_InfoText->Left=34;
		}
		else
		{
			scGPGlyphButton_InfoIcon->Visible=false;
			rHTMLLabel_InfoText->Left=8;
    }
		rHTMLLabel_InfoText->Top=Form_parametry->Height-74;
		rHTMLLabel_InfoText->Visible=true;
		rHTMLLabel_InfoText->Caption=text;
	}
	else//skryje
	{
		scGPGlyphButton_InfoIcon->Visible=false;
		rHTMLLabel_InfoText->Visible=false;
		//zvýraznìní položky technologický èas
		if(scGPNumericEdit_CT->Enabled){/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;*/}
		//Form_parametry->Height-=(40+19);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
	if(input_state!=NO)//pokud to není pøi startu (formshow)
	{
			//nadesignování a napozicování komponent dle zvoleného režimu
			setForm4Rezim(scComboBox_rezim->ItemIndex);//resize a napozicování formuláøe+povoleni a zakazani komponent pro jednotlivé režimy

			//napozicování celého formuláøe resp. ošetøení aby zùstal dialog na monitoru, pouze pro prvotní zobrazení dle souøadnic kurzoru myši, jinak dle uživatele
			long X=Form1->akt_souradnice_kurzoru_PX.x+10;long Y=Form1->akt_souradnice_kurzoru_PX.y+10;
			if(form_zobrazen){X=Form_parametry->Left;Y=Form_parametry->Top;}

			if(X+Form_parametry->ClientWidth<Form1->ClientWidth)
				Form_parametry->Left=X;
			else
				Form_parametry->Left=Form1->ClientWidth-Form_parametry->ClientWidth-10;
			if(Y+Form_parametry->ClientHeight<Form1->ClientHeight)
				Form_parametry->Top=Y;
			else
				Form_parametry->Top=Form1->ClientHeight-Form_parametry->ClientHeight-Form1->scGPPanel_statusbar->Height-10;

			//aktualizace hodnot
			if(scGPNumericEdit_CT->Value>0)input_CT();
			else
			{
				if(scGPNumericEdit_delka_dopravniku->Value>0)input_DD();
				else
				{
					 if(scGPNumericEdit_kapacita->Value>0)input_K();
					 else input_RD();
				}
			}
	}
}
//---------------------------------------------------------------------------
//resize a napozicování formuláøe+povoleni a zakazani komponent pro jednotlivé režimy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
	//výchozí zmenšení formuláøe
	offset=0;
	if(Form1->STATUS==Form1->NAVRH) rezim+=10;//posunutí o 10 vytváøí režim+navrháø
	switch(rezim)
	{
		 case 0://STOP & GO
		 {
			 set(POHON,READONLY);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,READONLY);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,READONLY);
			 set(CEKANI,READONLY);
			 set(STOPKA,READONLY);
			 set(ROTACE,HIDE);
			 set(MEZERA,HIDE);
			 set(POCET_MEZER,HIDE);
			 scGPNumericEdit_kapacita->Value=1;
		 }break;
		 case 10://STOP & GO - NÁVRHÁØ
		 {
			 //nastavení komponent
			 set(POHON,ENABLED);
			 set(TIME,ENABLED);
			 set(RYCHLOST,HIDE);
			 set(DELKA,ENABLED);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
			 set(ROTACE,HIDE);
			 set(MEZERA,HIDE);
			 set(POCET_MEZER,HIDE);
			 scGPNumericEdit_kapacita->Value=1;
		 }break;
		 case 1://KONTINUÁLNÍ
		 {
			 set(POHON,READONLY);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,READONLY);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,READONLY);
			 set(STOPKA,READONLY);
			 set(ROTACE,ENABLED);
			 set(MEZERA,ENABLED);
			 set(POCET_MEZER,ENABLED);
		 }break;
		 case 11://KONTINUÁLNÍ - NÁVRHÁØ
		 {
			 set(POHON,ENABLED);
			 set(TIME,ENABLED);
			 set(RYCHLOST,ENABLED);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
			 set(ROTACE,ENABLED);
			 set(MEZERA,ENABLED);
			 set(POCET_MEZER,ENABLED);
		 }break;
		 case 2://POSTPROCESNÍ
		 {
			 set(POHON,ENABLED);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,ENABLED);
			 set(CEKANI,ENABLED);
			 set(STOPKA,ENABLED);
			 set(ROTACE,ENABLED);
			 set(MEZERA,ENABLED);
			 set(POCET_MEZER,ENABLED);
		 }break;
		 case 12://POSTPROCESNÍ - NÁVRHÁØ
		 {
			 set(POHON,ENABLED);
			 set(TIME,ENABLED);
			 set(RYCHLOST,HIDE);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
			 set(ROTACE,ENABLED);
			 set(MEZERA,ENABLED);
			 set(POCET_MEZER,ENABLED);
		 }break;
			case 13://SG - NÁVRHÁØ  - ROZŠÍØENÉ
		 {
//		   set(POHON,READONLY);
//			 set(TIME,HIDE);
//			 set(RYCHLOST,HIDE);
//			 set(DELKA,READONLY);
//			 set(KAPACITA,READONLY);
//			 set(ODCHYLKA,READONLY);
//			 set(CEKANI,READONLY);
//			 set(STOPKA,READONLY);
//			 set(ROTACE,HIDE);
//			 set(MEZERA,HIDE);
//			 set(POCET_MEZER,HIDE);
//			 scGPNumericEdit_kapacita->Value=1;
		 }break;
	}

	//VELIKOST FORMULÁØE
	Form_parametry->Height=defaultForm_parametryHeight+offset;
	//vertikální POZICE TLAÈÍTEK OK,STORNO,COPY,PASTE
	scGPButton_OK->Top=Form_parametry->Height-scGPButton_OK->Height-10;
	scGPButton_storno->Top=Form_parametry->Height-scGPButton_storno->Height-10;
	scGPGlyphButton_paste->Top=Form_parametry->Height-scGPGlyphButton_paste->Height;
	scGPGlyphButton_copy->Top=scGPGlyphButton_paste->Top-scGPGlyphButton_copy->Height;
}
//---------------------------------------------------------------------------
//zajišuje zobrazení a napozicování patøièné konkrétní komponenty a zároveò udržování hodnoty offsetu - to pokud je move==true, jinak jen nastaví komponenty
void TForm_parametry::set(Tcomponents C,Tcomponents_state S,bool move)
{
	//defaultní hodnoty
	short O=40;//vertikální velikost odsazení komponent
	int L=rHTMLLabel_rezim->Top;//výchozí komponenta
	int P=scComboBox_rezim->Top;//výchozí komponenta

	switch (C)
	{
		case POHON:
		{
		 ////ošetøení pokud není nadefinován žádný pohon
			if(!existuje_pohon)S=READONLY;
		 ////pozice
			if(move)
			{
				rHTMLLabel_pohon->Top=L+O;
				scComboBox_pohon->Top=P+O;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
		 /*ROSTA-styl//	scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect; */
			rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=true;
			scComboBox_pohon->Options->FrameNormalColor=clGray;scComboBox_pohon->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scComboBox_pohon->Options->FrameNormalColor=hl_color;scComboBox_pohon->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_pohon->Enabled=false;break;
				case READONLY:scComboBox_pohon->Enabled=false;scComboBox_pohon->Options->ShapeStyle=scgpessNone;break;
				case HIDE:		rHTMLLabel_pohon->Visible=false;scComboBox_pohon->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case TIME://technologický èas v režimu návrháø
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_CT->Top=L+2*O+offset;
				scGPNumericEdit_CT->Top=P+2*O+offset;
				scButton_zamek_CT->Top=scGPNumericEdit_CT->Top;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
		 /*ROSTA-styl//	scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect;*/
			rHTMLLabel_CT->Visible=true;scGPNumericEdit_CT->Enabled=true;scGPNumericEdit_CT->Visible=true;
			/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;*/
			if(scComboBox_rezim->ItemIndex==1)scButton_zamek_CT->Visible=true;else scButton_zamek_CT->Visible=false;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;*/break;
				case ENABLED:	/*scButton_zamek_CT->ImageIndex=38;CT_zamek=UNLOCKED;*/break;
				case DISABLED:scGPNumericEdit_CT->Enabled=false;/*scButton_zamek_CT->ImageIndex=37;CT_zamek=LOCKED;*/break;
				case READONLY:/*ROSTA-styl//scGPNumericEdit_CT->Options->ShapeStyle=scgpessNone;*/scButton_zamek_CT->Visible=false;scGPNumericEdit_CT->Enabled=false;break;
				case HIDE:		rHTMLLabel_CT->Visible=false;scGPNumericEdit_CT->Visible=false;scButton_zamek_CT->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case RYCHLOST://RD v režimu návrháø
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_RD->Top=L+3*O+offset;
				scGPNumericEdit_RD->Top=P+3*O+offset;
				scButton_zamek_RD->Top=scGPNumericEdit_RD->Top;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
			/*ROSTA-styl//scGPNumericEdit_RD->Options->ShapeStyle=scgpessRect;*/
			rHTMLLabel_RD->Visible=true;scGPNumericEdit_RD->Visible=true;scGPNumericEdit_RD->Enabled=true;
			/*ROSTA-styl//scGPNumericEdit_RD->Options->FrameNormalColor=clGray;scGPNumericEdit_RD->Options->FrameWidth=1;*/
			if(scComboBox_rezim->ItemIndex==1)scButton_zamek_RD->Visible=true;else scButton_zamek_RD->Visible=false;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:/*ROSTA-styl//scGPNumericEdit_RD->Options->FrameNormalColor=hl_color;scGPNumericEdit_RD->Options->FrameWidth=hlFrameWidth;*/
				case ENABLED:	/*scButton_zamek_RD->ImageIndex=38;RD_zamek=UNLOCKED;*/break;
				case DISABLED:/*scGPNumericEdit_RD->Enabled=false;scButton_zamek_RD->ImageIndex=37;RD_zamek=UNLOCKED;*/break;
				case READONLY:/*ROSTA-styl//scGPNumericEdit_RD->Options->ShapeStyle=scgpessNone;*/scGPNumericEdit_RD->Enabled=false;scButton_zamek_RD->Visible=false;break;
				case HIDE:		rHTMLLabel_RD->Visible=false;scGPNumericEdit_RD->Visible=false;scButton_zamek_RD->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case DELKA://délka dopravníku
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_delka_dopravniku->Top=L+4*O+offset;
				scGPNumericEdit_delka_dopravniku->Top=P+4*O+offset;
				scButton_zamek_DD->Top=scGPNumericEdit_delka_dopravniku->Top;
			}
			//ty co jsou stejné
			scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Enabled=true;
			scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=clGray;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=1;
			//scButton_zamek_DD->Visible=true;
			//ty co jsou rozdílné
		 ////funkèní vlastnosti
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=hl_color;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	scButton_zamek_DD->ImageIndex=38;DD_zamek=UNLOCKED;break;
				case DISABLED:scGPNumericEdit_delka_dopravniku->Enabled=false;scButton_zamek_DD->ImageIndex=37;DD_zamek=LOCKED;break;
				case READONLY:scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessNone;scGPNumericEdit_delka_dopravniku->Enabled=false;scButton_zamek_DD->Visible=false;break;
				case HIDE:		rHTMLLabel_delka_dopravniku->Visible=false;scGPNumericEdit_delka_dopravniku->Visible=false;scButton_zamek_DD->Visible=false;if(move)offset-=O;break;
			}
		}break;
		case KAPACITA://požadována vs. zjištìná kapacita objektu
		{
		 ////pozice
			rHTMLLabel_kapacita->Top=L+5*O+offset;
			scGPNumericEdit_kapacita->Top=P+5*O+offset;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_kapacita->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_kapacita->Visible=true;scGPNumericEdit_kapacita->Visible=true;scGPNumericEdit_kapacita->Enabled=true;
			scGPNumericEdit_kapacita->Options->FrameNormalColor=clGray;scGPNumericEdit_kapacita->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_kapacita->Options->FrameNormalColor=hl_color;scGPNumericEdit_kapacita->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_kapacita->Enabled=false;break;
				case READONLY:scGPNumericEdit_kapacita->Options->ShapeStyle=scgpessNone;scGPNumericEdit_kapacita->Enabled=false;break;
				case HIDE:		rHTMLLabel_kapacita->Visible=false;scGPNumericEdit_kapacita->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case ODCHYLKA://povolená odchylka z CT
		{
		 ////pozice
			rHTMLLabel_odchylka->Top=L+6*O+offset;
			scGPNumericEdit_odchylka->Top=P+6*O+offset;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_odchylka->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_odchylka->Visible=true;scGPNumericEdit_odchylka->Visible=true;scGPNumericEdit_odchylka->Enabled=true;
			scGPNumericEdit_odchylka->Options->FrameNormalColor=clGray;scGPNumericEdit_odchylka->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_odchylka->Options->FrameNormalColor=hl_color;scGPNumericEdit_odchylka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_odchylka->Enabled=false;break;
				case READONLY:scGPNumericEdit_odchylka->Options->ShapeStyle=scgpessNone;scGPNumericEdit_odchylka->Visible=false;break;
				case HIDE:		rHTMLLabel_odchylka->Visible=false;scGPNumericEdit_odchylka->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case CEKANI://èekání
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_cekani->Top=L+7*O+offset;
				scComboBox_cekani_palec->Top=P+7*O+offset;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scComboBox_cekani_palec->Options->FrameNormalColor=clGray;scComboBox_cekani_palec->Options->FrameWidth=1;
			rHTMLLabel_cekani->Visible=true;scComboBox_cekani_palec->Visible=true;scComboBox_cekani_palec->Enabled=true;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scComboBox_cekani_palec->Options->FrameNormalColor=hl_color;scComboBox_cekani_palec->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_cekani_palec->Enabled=false;break;
				case READONLY:scComboBox_cekani_palec->Enabled=false;break;
				case HIDE:		rHTMLLabel_cekani->Visible=false;scComboBox_cekani_palec->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case STOPKA://stop stanice na konci objektu
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_stopka->Top=L+8*O+offset;
				scComboBox_stopka->Top=P+8*O+offset;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
			rHTMLLabel_stopka->Visible=true;scComboBox_stopka->Visible=true;scComboBox_stopka->Enabled=true;
			scComboBox_stopka->Options->FrameNormalColor=clGray;scComboBox_stopka->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scComboBox_stopka->Options->FrameNormalColor=hl_color;scComboBox_stopka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_stopka->Enabled=false;break;
				case READONLY:scComboBox_stopka->Enabled=false;break;
				case HIDE:		rHTMLLabel_stopka->Visible=false;scComboBox_stopka->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case ROTACE://rotace jigu v objektu, zatím jen pøepínátko 0-90
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_rotace->Top=L+9*O+offset;
				scComboBox_rotace->Top=P+9*O+offset;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
			rHTMLLabel_rotace->Visible=true;scComboBox_rotace->Visible=true;scComboBox_rotace->Enabled=true;
			scComboBox_rotace->Options->FrameNormalColor=clGray;scComboBox_rotace->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scComboBox_rotace->Options->FrameNormalColor=hl_color;scComboBox_rotace->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_rotace->Enabled=false;break;
				case READONLY:scComboBox_rotace->Enabled=false;break;
				case HIDE:		rHTMLLabel_rotace->Visible=false;scComboBox_rotace->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case MEZERA://požadována vs. zjištìná kapacita objektu
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_mezera->Top=L+10*O+offset;
				scGPNumericEdit_mezera->Top=P+10*O+offset;
			}
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_mezera->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_mezera->Visible=true;scGPNumericEdit_mezera->Visible=true;scGPNumericEdit_mezera->Enabled=true;
			scGPNumericEdit_mezera->Options->FrameNormalColor=clGray;scGPNumericEdit_mezera->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_mezera->Options->FrameNormalColor=hl_color;scGPNumericEdit_mezera->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_mezera->Enabled=false;break;
				case READONLY:scGPNumericEdit_mezera->Options->ShapeStyle=scgpessNone;scGPNumericEdit_mezera->Enabled=false;break;
				case HIDE:		rHTMLLabel_mezera->Visible=false;scGPNumericEdit_mezera->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case POCET_MEZER://požadována vs. zjištìná kapacita objektu
		{
		 ////pozice
			if(move)scGPCheckBox_pocet_mezer->Top=L+11*O-5+offset;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPCheckBox_pocet_mezer->Visible=true;scGPCheckBox_pocet_mezer->Enabled=true;
			//scGPCheckBox_pocet_mezer->Options->FrameNormalColor=clGray;scGPCheckBox_pocet_mezer->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				//case HIGHLIGHT:scGPCheckBox_pocet_mezer->Options->FrameNormalColor=hl_color;scGPCheckBox_pocet_mezer->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPCheckBox_pocet_mezer->Enabled=false;break;
				case READONLY:scGPCheckBox_pocet_mezer->Enabled=false;break;
				case HIDE:		scGPCheckBox_pocet_mezer->Visible=false;if(move)offset-=O;break;
			}
		}	break;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøi psaní názvu objektu se mìní titulek a generuje zkratka
void __fastcall TForm_parametry::scGPEdit_nameChange(TObject *Sender)
{
	 Form_parametry->scLabel_titulek->Caption=scGPEdit_name->Text+" - parametry";
	 scGPEdit_shortname->Text=scGPEdit_name->Text.SubString(1,4).UpperCase();
}
//---------------------------------------------------------------------------
//pøi zmìnách EDITù
void __fastcall TForm_parametry::scGPNumericEdit_CTChange(TObject *Sender)
{
	if(input_state==NOTHING && input_clicked_edit==CT_klik)//pokud není zadáváno z jiného vstupu
	input_CT();//pøepoèet hodnot vyplývajících ze zmìny CT
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuChange(TObject *Sender)
{
	if(input_state==NOTHING && input_clicked_edit==DD_klik)//pokud není zadáváno z jiného vstupu
	input_DD();//pøepoèet hodnot vyplývajících ze zmìny délky dopravníku
	//hlídání velikosti mezery dle rozteèe
	if(scComboBox_rezim->ItemIndex!=0 && scGPNumericEdit_mezera->Value>0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{                            //je "zbytek po dìlení"
			if(P->roztec>0 && !Form1->m.cele_cislo(scGPNumericEdit_mezera->Value/P->roztec))//nesplòuje rozmezí
			{
				vypis("Doporuèeno: "+AnsiString(Form1->m.round(scGPNumericEdit_mezera->Value/P->roztec)*P->roztec)+" m",true);
				//-scGPButton_OK->Enabled=false;
			}
			else
			{
				vypis("");
				//-scGPButton_OK->Enabled=true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RD_Change(TObject *Sender)
{
	if(input_state==NOTHING && input_clicked_edit==RD_klik)//pokud není zadáváno z jiného vstupu
	input_RD();//pøepoèet hodnot vyplývajících ze zmìny RD
	//kontrola rozsahu zadaného pohonu
	if(scComboBox_rezim->ItemIndex!=0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{
			if(scGPNumericEdit_RD->Value<P->rychlost_od || P->rychlost_do<scGPNumericEdit_RD->Value)//nesplòuje rozmezí
			{
				vypis("Rychlost neodpovídá rozmezí!",true);
				//-scGPButton_OK->Enabled=false;
			}
			else
			{
				vypis("");
				//-scGPButton_OK->Enabled=true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaChange(TObject *Sender)
{
//	ShowMessage("zadavano z jineho vstupu?");
	if(input_state==NOTHING && input_clicked_edit==C_klik)//pokud není zadáváno z jiného vstupu

	input_K();//pøepoèet hodnot vyplývajících ze zmìny K
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPCheckBox_pocet_mezerClick(TObject *Sender)
{
 if(input_state==NOTHING && input_clicked_edit==mezera_klik)
 input_DD();//pøepoèet hodnot vyplývajících ze zmìny délky dopravníku
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøepoèet hodnot vyplývajících ze zmìny CT
void TForm_parametry::input_CT()
{
	 input_state=CT;
	 double CT=scGPNumericEdit_CT->Value;//CT od uživatele
	 if(CTunit==MIN)CT=CT*60.0;//pokud bylo zadání v minutách pøevede na sekundy
	 if(CT>0)//nutné ošetøení pro období zadávání/psaní
	 {
			 //default nastavení komponent
			 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
			 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Uložit";

			 //volba šíøka èi délka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je požadován šíøka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//poèet mezer mezi vozíky

			 //dle øežimu objektu
			 if(scComboBox_rezim->ItemIndex==0)//S&G
			 {
				 //ošetøení a pøípadnì následné øešení situací, kdy není totožný procesní èas a TT
				 if(CT<Form1->d.v.PP.TT)
    		 {
    			scGPButton_OK->Enabled=false;
					vypis("Technologický èas je nižší než TT!");
				 }
				 if(CT>Form1->d.v.PP.TT)
				 {
						if(fmod(CT,Form1->d.v.PP.TT)==0)
    				{
							kapacitaSG=CT/Form1->d.v.PP.TT;//pro další použití
    					vypis("Rozložit na "+AnsiString(kapacitaSG)+"x "+scGPEdit_name->Text.UpperCase()+"?");
    		 			scGPButton_OK->Enabled=true;
    					scGPButton_OK->Caption="Ano a uložit";
    		 		}
    				else
    		 		{
							scGPButton_OK->Enabled=false;
							vypis("Zmìnte režim nebo rozložte do více objektù!");
						}
				 }
				 else kapacitaSG=1;
    	 }
    	 else//KONTINUAL+PP
			 {
				 //KAPACITA
				 double K=CT/Form1->d.v.PP.TT;//výpoèet
				 //ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
				 scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
				 scGPNumericEdit_kapacita->Font->Color=clBlack;
				 //pokud obsahuje kapacita reálnou èást, vypíše doporuèení
				 if(Form1->ms.get_count_decimal(K)>0)
				 {
					scGPNumericEdit_kapacita->Font->Color=clRed;//zvýraznìní neplatné vyplývající položky
					if(Form1->m.round(K)==0)K=1;//ošetøení pokud by vycházela kapacita 0,nìco bylo by zaokrouhleno na 0 a tudíž by se vypisoval doporuèený technologický èas 0
					if(CTunit==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
					else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
           scGPButton_OK->Enabled=true;
					//ROSTA-puvodni//scGPButton_OK->Enabled=false;//zakáže ukládací tlaèítko
				 }
				 //DÉLKA DOPRAVNÍKU
				 double DD=K*dV+(K-p)*m;//ošetøeno i pro stav kdy je stejný poèet mezer jako vozíku
				 if(DDunit==MM)DD*=1000.0;
				 //ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
				 scGPNumericEdit_delka_dopravniku->Value=DD;//plnìní patøièného políèka
				 if(DDunit==MM)DD/=1000.0;//vracení do pùvodního stavu k dalšímu použití
				 //RYCHLOST DOPRAVNÍKU (èistì jen pro KONTINUAL)
				 if(scComboBox_rezim->ItemIndex==1)
				 {
					double RD=DD/CT;
					if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
					//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_RD->Value=RD;//plnìní patøièného políèka
				 }
			 }
	 }
	 else
	 null_input_value();
	 input_state=NOTHING;
}
//---------------------------------------------------------------------------
//pøepoèet hodnot vyplývajících ze zmìny DD
void TForm_parametry::input_DD()
{
 	input_state=DD;//pozor myšleno DD - z ENUM
	//default hodnoty
	double DD=scGPNumericEdit_delka_dopravniku->Value;if(DDunit==MM)DD/=1000.0;//pøevede vždy do metrù

	if(DD>0)//nutné ošetøení pro období zadávání/psaní
	{
		//default nastavení komponent
		//ROSTA//scGPNumericEdit_kapacita->Decimal=0;
		vypis("");
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption="Uložit";

		//volba šíøka èi délka jigu
		double dV=Form1->d.v.PP.delka_voziku;//delka voziku
		if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je požadován šíøka jigu
		double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
		short p=!scGPCheckBox_pocet_mezer->Checked;//poèet mezer mezi vozíky

		//KAPACITA                 //pokud je stejný poèet mezer jako vozíkù
		double K=(DD+m)/(dV+m);if(p==1)K=DD/(dV+m);
		//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
		scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
		scGPNumericEdit_kapacita->Font->Color=clBlack;
		//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
		if(Form1->ms.get_count_decimal(K)>0)
		{
      scGPNumericEdit_kapacita->Font->Color=clRed;//zvýraznìní neplatné vyplývající položky
			if(Form1->m.round(K)==0)K=1;//ošetøení pokud by vycházela kapacita 0,nìco bylo by zaokrouhleno na 0 a tudíž by se vypisoval doporuèený technologický èas 0
			if(CTunit==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
			else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
      scGPButton_OK->Enabled=false;//zakáže ukládací tlaèítko
		}

		/////////CT,RD
		//výpoèet
		double CT=0;double RD=0;
		if(CT_zamek==UNLOCKED)//CT odemèený, RD zamèený
		{
				CT = Form1->d.v.PP.TT*K;
				RD = DD/CT;//poèítá z již pøepoèítané hodnoty, takže zùstane stejný
		}
		else//je zamèený CT, odemèený RD
		{
				if(scGPNumericEdit_CT->Value==0)CT=(Form1->d.v.PP.TT*K);//ošetøení pokud nebyl aktualizován
									 //ze zadaného CT
				if(CTunit==MIN)CT=scGPNumericEdit_CT->Value*60.0;else CT=scGPNumericEdit_CT->Value;
				RD = DD/CT;//ze zadaného CT
				CT = DD/RD;//poèítá z již pøepoèítané hodnoty, takže zùstane stejný
		}
		//plnìní a formátování editboxu CT
		if(CTunit==MIN)
		{
			//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastaví zobrazení poètu desetinných míst
			scGPNumericEdit_CT->Value=CT/60.0;//plnìní patøièného políèka
		}
		else
		{
			//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
			scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
		}
		//plnìní a formátování editboxu RD
		if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
		//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
		scGPNumericEdit_RD->Value=RD;//plnìní patøièného políèka
	}
	else
	null_input_value();
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//pøepoèet hodnot vyplývajících ze zmìny RD, pouze pro kontinual
void TForm_parametry::input_RD()
{
	 input_state=RD;
	 double RD=scGPNumericEdit_RD->Value;
	 //pøevede pøípadnì na metr a na jednu sekundu, není-li
	 if(RDunitD==MM)RD/=1000.0;
	 if(RDunitT==MIN)RD/=60.0;

	 if(RD>0)//nutné ošetøení pro období zadávání/psaní
	 {
			 //default nastavení komponent
			 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Uložit";

			 //volba šíøka èi délka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je požadován šíøka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//poèet mezer mezi vozíky

    	 double K=0;
			 if(m>0 && p==1)//pokud je rozdílný poèet mezer jako vozíkù a mezera je nenulova, lze pøímo z RD vypoèítat ostatní hodnoty
    	 {
    			//KAPACITA
					K=m/(dV+m-RD*Form1->d.v.PP.TT);
    			//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
					scGPNumericEdit_kapacita->Font->Color=clBlack;
					//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
					if(Form1->ms.get_count_decimal(K)>0)
					{
            scGPNumericEdit_kapacita->Font->Color=clRed;//zvýraznìní neplatné vyplývající položky
						if(Form1->m.round(K)==0)K=1;//ošetøení pokud by vycházela kapacita 0,nìco bylo by zaokrouhleno na 0 a tudíž by se vypisoval doporuèený technologický èas 0
						if(CTunit==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
						else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
            scGPButton_OK->Enabled=false;//zakáže ukládací tlaèítko
					}

					//DÉLKA DOPRAVNÍKU
					double DD = K*dV+(K-p)*m;
					if(DDunit==MM)DD*=1000.0;
					//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_delka_dopravniku->Value=DD;//plnìní patøièného políèka
					if(DDunit==MM)DD/=1000.0;//vrácení do pùvodního stavu kvùli dalšímu použit


    			//PROCESNÍ ÈAS resp. CT
					double CT = RD/DD;
					if(CTunit==MIN)CT/60.0;
					//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
			 }
    	 else //pokud ne, je nutné získat další nenulový vstup
			 {
					double DD=scGPNumericEdit_delka_dopravniku->Value;if(DDunit==MM)DD/=1000.0; //na metry
					double CT=scGPNumericEdit_CT->Value;if(CTunit==MIN)CT*60.0; //na sekunkdy
    			double K=scGPNumericEdit_kapacita->Value;

    			if(DD!=0)//pokud je známá délka dopravníku
					{
    				//KAPACITA
						K=DD/RD/Form1->d.v.PP.TT;
						//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
						scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
						scGPNumericEdit_kapacita->Font->Color=clBlack;

						Memo1->Lines->Add(AnsiString(RD)+" "+DD+" "+Form1->d.v.PP.TT);
						//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
						if(Form1->ms.get_count_decimal(K)>0)
						{
							scGPNumericEdit_kapacita->Font->Color=clRed;//zvýraznìní neplatné vyplývající položky
							if(Form1->m.round(K)==0)K=1;//ošetøení pokud by vycházela kapacita 0,nìco bylo by zaokrouhleno na 0 a tudíž by se vypisoval doporuèený technologický èas 0
							if(CTunit==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
							else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
              scGPButton_OK->Enabled=false;//zakáže ukládací tlaèítko
						}

    				//PROCESNÍ ÈAS resp. CT
						double CT = DD/RD;
						if(CTunit==MIN)CT/60.0;
						//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
						scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
							//Memo1->Lines->Add(K);
    			}
    			else
					{
						if(CT!=0)//pokud není známá délka ale je známe CT
    				{
        			//DÉLKA DOPRAVNÍKU
							DD=RD*CT;
							if(DDunit==MM)DD*=1000.0;
							//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
							scGPNumericEdit_delka_dopravniku->Value=DD;//plnìní patøièného políèka

        			//KAPACITA
							K=CT/Form1->d.v.PP.TT;
        			//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
        			scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
              scGPNumericEdit_kapacita->Font->Color=clBlack;
							//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
							if(Form1->ms.get_count_decimal(K)>0)
							{
                scGPNumericEdit_kapacita->Font->Color=clRed;//zvýraznìní neplatné vyplývající položky
								if(Form1->m.round(K)==0)K=1;//ošetøení pokud by vycházela kapacita 0,nìco bylo by zaokrouhleno na 0 a tudíž by se vypisoval doporuèený technologický èas 0
								if(CTunit==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
								else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
                scGPButton_OK->Enabled=false;//zakáže ukládací tlaèítko
							}
						}
    				else
    				{
    					if(K!=0)//pokud není známá délka ani CT a je známá K
    					{
    						//DÉLKA DOPRAVNÍKU
								DD=RD*Form1->d.v.PP.TT*K;
								if(DDunit==MM)DD*=1000.0;
								//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
								scGPNumericEdit_delka_dopravniku->Value=DD;//plnìní patøièného políèka
								if(DDunit==MM)DD/=1000.0;//navrácení do pùvodního stavu kvùli dalšímu použití

    						//PROCESNÍ ÈAS resp. CT
								double CT = RD/DD;
								if(CTunit==MIN)CT/60.0;
								//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
								scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
    					}
    					else//pokud kromì RD není nic zadané a je požadovaná další hodnota
							{
								vypis("Zadejte libovolnou další hodnotu!");
              }
    				}
    			}
			 }
	 }
	 else
	 null_input_value();
	 input_state=NOTHING;
}
//---------------------------------------------------------------------------
//pøepoèet hodnot vyplývajících ze zmìny K
void TForm_parametry::input_K()
{

   //ShowMessage("Ne - prepocitam");
	 input_state=C;
	 double K=scGPNumericEdit_kapacita->Value;//získání kapacity od uživatele
	 if(K>0)//nutné ošetøení pro období zadávání/psaní
	 {
	 //ShowMessage(K);
    	 //default nastavení komponent
    	 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
			 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Uložit";

			 //volba šíøka èi délka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je požadován šíøka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//poèet mezer mezi vozíky

			 //PROCESNÍ ÈAS resp. CT
			 double CT = 0;//Form1->d.v.PP.TT;
			 if(CT_zamek==UNLOCKED)
			 {
					 CT=Form1->d.v.PP.TT*K;
					 if(CTunit==MIN)
					 {
							//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastaví zobrazení poètu desetinných míst
							scGPNumericEdit_CT->Value=CT/60.0;//plnìní patøièného políèka
					 }
					 else
					 {
							//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
							scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
					 }
			 }
			 else
			 {
						CT=scGPNumericEdit_CT->Value; if(CTunit==MIN)CT*=60;
			 }

       //DÉLKA DOPRAVNÍKU
			 double DD = K*dV+(K-p)*m;//ošetøeno i pro stav stejného poètu vozíku a mezer;
			 if(DDunit==MM)DD*=1000.0;
			 //ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
			 scGPNumericEdit_delka_dopravniku->Value=DD;//plnìní patøièného políèka
			 if(DDunit==MM)DD/=1000.0;//navrácení do pùvodního stavu kvùli dalšímu použití

    	 //RYCHLOST DOPRAVNÍKU (èistì jen pro KONTINUAL)
    	 if(scComboBox_rezim->ItemIndex==1)
			 {
					double RD=DD/CT;
					if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
					//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_RD->Value=RD;//plnìní patøièného políèka
			 }
	 }
	 else
	 {
		null_input_value();
		vypis("Doporuèená kapacita je 1!");
		scGPButton_OK->Enabled=false;
	 }
	 input_state=NOTHING;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vynuluje vstupní hodnoty
void TForm_parametry::null_input_value()
{
	scGPNumericEdit_RD->Value=0;
	scGPNumericEdit_CT->Value=0;
	scGPNumericEdit_delka_dopravniku->Value=0;
	scGPNumericEdit_kapacita->Value=0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry::vykresli_vozik(bool na_delku)
{ /*ZDM
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
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender)
{
//	 vykresli_vozik(true);//na délku
//	 ValueListEditorStringsChange(Sender);//zajistí pøepoèítání hodnot
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender)
{
//	 vykresli_vozik(false);//na šíøku
//	 ValueListEditorStringsChange(Sender);//zajistí pøepoèítání hodnot
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender)
{
//	 if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
//	 else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}
//---------------------------------------------------------------------------
//pøepínání zobrazení min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double RD=0.0;double CT=0.0;
	if(minsec==MIN)//pokud je v minutách, tak pøepne na sekundy
	{
		minsec=S;scGPButton_min_sec->Caption="vše na min";//samotné tlaèítko,ukazuje název opaènì
		//CT - pøepoèítání
		CTunit=S;
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologický èas <font color=#2b579a>[s]</font>";
		//RD - pøepoèítání
		RDunitT=S;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value/60.0;

	}
	else//pokud je v sekundách pøepne na minuty
	{
		minsec=MIN;scGPButton_min_sec->Caption="vše na s";//samotné tlaèítko,ukazuje název opaènì
		//CT - pøepoèítání
		CTunit=MIN;
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologický èas <font color=#2b579a>[min]</font>";
		//RD - pøepoèítání
		RDunitT=MIN;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value*60.0;
	}
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_CT->Value=CT;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//požadavek na zmìnu jednotek CT
void __fastcall TForm_parametry::rHTMLLabel_CTClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double CT=0.0;
	if(CTunit==MIN)//pokud je v minutách, tak pøepne na sekundy
	{
		CTunit=S;
		//CT - pøepoèítání
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologický èas <font color=#2b579a>[s]</font>";
	}
	else//pokud je v sekundách pøepne na minuty
	{
		CTunit=MIN;
		//CT - pøepoèítání
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologický èas <font color=#2b579a>[min]</font>";
	}
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_CT->Value=CT;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//požadavek na zmìnu jednotek RD
void __fastcall TForm_parametry::rHTMLLabel_RDClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double RD=0.0;

	if(RDunitT==MIN)//pokud je v minutách, tak pøepne na sekundy
	{
		RDunitT=S;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value/60.0;
		//RD=RDunitD_funkce(RD);
	}
	else//pokud je v sekundách pøepne na minuty
	{
		RDunitT=MIN;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value*60.0;
		RD=RDunitD_funkce(RD);
	}
  RD=RDunitD_funkce(RD);
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
double TForm_parametry::RDunitD_funkce(double RD)//podpùrná metoda výše uvedené
{
		if(RDunitD==MM)//pokud je v milimetrech, tak pøepne na metry
		{
			RDunitD=M;
			if(RDunitT==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v minutách
			else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekundách
			return RD/1000.0;
		}
		else
		{
			RDunitD=MM;
			if(RDunitT==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v minutách
			else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekundách
			return RD*1000.0;
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøepínání zobrazení m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double DD=0.0;double DM=0.0;double RD=0.0;
	if(m_mm==MM)//pokud je v milimetrech, tak pøepne na metry
	{
		m_mm=M;scGPButton_metry_milimetry->Caption="vše na mm";//samotné tlaèítko,ukazuje název opaènì
		//DD
		DDunit=M;
		rHTMLLabel_delka_dopravniku->Caption="Délka kabiny <font color=#2b579a>[m]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value/1000.0;
		//DM
		DMunit=M;
		rHTMLLabel_mezera->Caption="Mezera mezi vozíky <font color=#2b579a>[m]</font>";
		DM=scGPNumericEdit_mezera->Value/1000.0;
		//RD
		RDunitD=M;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v minutách
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekundách
		RD=scGPNumericEdit_RD->Value/1000.0;
	}
	else//pokud je metrech, tak pøepne na milimetry
	{
		m_mm=MM;scGPButton_metry_milimetry->Caption="vše na m";//samotné tlaèítko,ukazuje název opaènì
		//DD
		DDunit=MM;
		rHTMLLabel_delka_dopravniku->Caption="Délka kabiny <font color=#2b579a>[mm]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value*1000.0;
		//DM
		DMunit=MM;
		rHTMLLabel_mezera->Caption="Mezera mezi vozíky <font color=#2b579a>[mm]</font>";
		DM=scGPNumericEdit_mezera->Value*1000.0;
		//RD
		RDunitD=MM;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v minutách
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekundách
		RD=scGPNumericEdit_RD->Value*1000.0;
	}
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_delka_dopravniku->Value=DD;
	//ROSTA//scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_mezera->Value=DM;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//požadavek na zmìnu jednotek DD
void __fastcall TForm_parametry::rHTMLLabel_delka_dopravnikuClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double DD=0.0;
	if(DDunit==MM)//pokud je v milimetrech, tak pøepne na metry
	{
		DDunit=M;
		rHTMLLabel_delka_dopravniku->Caption="Délka kabiny <font color=#2b579a>[m]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value/1000.0;
	}
	else//pokud je metrech, tak pøepne na milimetry
	{
		DDunit=MM;
		rHTMLLabel_delka_dopravniku->Caption="Délka kabiny <font color=#2b579a>[mm]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value*1000.0;
	}
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_delka_dopravniku->Value=DD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//požadavek na zmìnu jednotek DM
void __fastcall TForm_parametry::rHTMLLabel_mezeraClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double DM=0.0;
	if(DMunit==MM)//pokud je v milimetrech, tak pøepne na metry
	{
		//DM
		DMunit=M;
		rHTMLLabel_mezera->Caption="Mezera mezi vozíky <font color=#2b579a>[m]</font>";
		DM=scGPNumericEdit_mezera->Value/1000.0;
	}
	else//pokud je metrech, tak pøepne na milimetry
	{
		//DM
		DMunit=MM;
		rHTMLLabel_mezera->Caption="Mezera mezi vozíky <font color=#2b579a>[mm]</font>";
		DM=scGPNumericEdit_mezera->Value*1000.0;
	}
	//plnìní + poèet desetinných míst
	//ROSTA//scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_mezera->Value=DM;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøi stisku klávesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 if(Key==13)//ENTER
 {
		 if(scGPButton_OK->Enabled)//pokud jsou zároveò splnìny podmínky pro stisk OK
		 {
			Form_parametry->ModalResult=mrOk;//vrátí stejnou hodnotu jako tlaèítko
			Form_parametry->VisibleChanging();//skryje form, stejné jako visible=false
		 }
		 else MessageBeep(0);//pípnutím upozorní, že nelze
 }
 if(Key==27)//ESC
 {
		 Form_parametry->ModalResult=mrCancel;//vrátí stejnou hodnotu jako tlaèítko
		 Form_parametry->VisibleChanging();//skryje form, stejné jako visible=false
 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøi stisku storna
void __fastcall TForm_parametry::scGPButton_stornoClick(TObject *Sender)
{
	kapacitaSG=1;//nastaví na default
}
//---------------------------------------------------------------------------
//pøi stisku storna
void __fastcall TForm_parametry::KonecClick(TObject *Sender)
{
	kapacitaSG=1;//nastaví na default
}
//---------------------------------------------------------------------------
//uloží do INI nastavení formuláøe
void __fastcall TForm_parametry::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->writeINI("nastaveni_form_parametry","cas",minsec);
	Form1->writeINI("nastaveni_form_parametry","vzdalenost",m_mm);
	Form1->writeINI("nastaveni_form_parametry","CT",CTunit);
	Form1->writeINI("nastaveni_form_parametry","RDt",RDunitT);
	Form1->writeINI("nastaveni_form_parametry","RDd",RDunitD);
	Form1->writeINI("nastaveni_form_parametry","DD",DDunit);
	Form1->writeINI("nastaveni_form_parametry","DM",DMunit);
}
//---------------------------------------------------------------------------
//zkopíruje parametry objektu pro další použití
void __fastcall TForm_parametry::scGPGlyphButton_copyClick(TObject *Sender)
{
	//povolí tlaèítko vkládání
	scGPGlyphButton_paste->Enabled=true;

	//pøevod jednotek
	double jednotky_cas=1.0;if(minsec==MIN)jednotky_cas=60.0;
	double jednotky_vzdalenost=1.0;if(m_mm==MM)jednotky_vzdalenost=1000.0;

	//text do schránky
	AnsiString T="";

	//atributy
	Form1->copyObjekt->name=scGPEdit_name->Text;T=scGPEdit_name->Text;T+=";";
	Form1->copyObjekt->short_name=scGPEdit_shortname->Text;T+=scGPEdit_shortname->Text;T+=";";
	Form1->copyObjekt->delka_dopravniku=scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;T+=scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;T+=";";
	Form1->copyObjekt->pohon=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex+1);/*indexuje se od nuly*/T+=scComboBox_pohon->ItemIndex+1;T+=";";
	Form1->copyObjekt->rezim=scComboBox_rezim->ItemIndex;T+=scComboBox_rezim->ItemIndex;T+=";";
	Form1->copyObjekt->CT=scGPNumericEdit_CT->Value*jednotky_cas;T+=Form1->copyObjekt->CT;T+=";";
	Form1->copyObjekt->cekat_na_palce=scComboBox_cekani_palec->ItemIndex;T+=scComboBox_cekani_palec->ItemIndex;T+=";";
	Form1->copyObjekt->kapacita=scGPNumericEdit_kapacita->Value;T+=Form1->copyObjekt->kapacita;T+=";";
	Form1->copyObjekt->odchylka=scGPNumericEdit_odchylka->Value;T+=Form1->copyObjekt->odchylka;T+=";";
	Form1->copyObjekt->stopka=scComboBox_stopka->ItemIndex;T+=Form1->copyObjekt->stopka;T+=";";
	Form1->copyObjekt->RD=scGPNumericEdit_RD->Value/jednotky_cas/jednotky_vzdalenost;T+=Form1->copyObjekt->RD;T+=";";
	Form1->copyObjekt->rotace=scComboBox_rotace->ItemIndex;T+=Form1->copyObjekt->rotace;T+=";";
	Form1->copyObjekt->mezera=scGPNumericEdit_mezera->Value/jednotky_vzdalenost;T+=Form1->copyObjekt->mezera;T+=";";
	Form1->copyObjekt->mV=!scGPCheckBox_pocet_mezer->Checked;T+=Form1->copyObjekt->mV;T+=";";

	//zkopírování textu do schránky
	TClipboard *c=new TClipboard;
	c->AsText=T;
	c=NULL;delete c;
}
//---------------------------------------------------------------------------
//zajišuje vložení objektu
void __fastcall TForm_parametry::scGPGlyphButton_pasteClick(TObject *Sender)
{
	//aby se nevolalo pøepoèítávání pøi editu položek
	input_state=NO;

  //pøevod jednotek
	double jednotky_cas=1.0;if(minsec==MIN)jednotky_cas=60.0;
	double jednotky_vzdalenost=1.0;if(m_mm==MM)jednotky_vzdalenost=1000.0;

	scGPEdit_name->Text=Form1->copyObjekt->name;
	scGPEdit_shortname->Text=Form1->copyObjekt->short_name;
	scComboBox_rezim->ItemIndex=Form1->copyObjekt->rezim;
	if(Form1->copyObjekt->pohon==NULL)scComboBox_pohon->ItemIndex=0;
	else scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n-1;
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->CT/jednotky_cas);//nastaví zobrazení poètu desetinných míst;
	scGPNumericEdit_CT->Value=Form1->copyObjekt->CT/jednotky_cas;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst;
	scGPNumericEdit_RD->Value=Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost;
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst;
	scGPNumericEdit_delka_dopravniku->Value=Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost;
	scComboBox_cekani_palec->ItemIndex=Form1->copyObjekt->cekat_na_palce;
	scGPNumericEdit_kapacita->Value=Form1->copyObjekt->kapacita;
	scGPNumericEdit_odchylka->Value=Form1->copyObjekt->odchylka;
	scComboBox_stopka->ItemIndex=Form1->copyObjekt->stopka;
	scComboBox_rotace->ItemIndex=Form1->copyObjekt->rotace;
	scGPNumericEdit_mezera->Value=Form1->copyObjekt->mezera*jednotky_vzdalenost;
	scGPCheckBox_pocet_mezer->Checked=!Form1->copyObjekt->mV;

	//nastavení aktuálního režimu objektu
	setForm4Rezim(scComboBox_rezim->ItemIndex);

	//povolení pøepoèítávání
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//zámek procesního èasu
void __fastcall TForm_parametry::scButton_zamek_CTClick(TObject *Sender)
{
	if(scButton_zamek_CT->ImageIndex==37)//když je zamèeno
	{
		//CT
		scButton_zamek_CT->ImageIndex=38;
		CT_zamek=UNLOCKED;
		//RD
		scButton_zamek_RD->ImageIndex=37;
		RD_zamek=LOCKED;
	}
	else//odemèeno
	{
		//CT
		scButton_zamek_CT->ImageIndex=37;
		CT_zamek=LOCKED;
		//RD
		scButton_zamek_RD->ImageIndex=38;
		RD_zamek=UNLOCKED;
	}

}
//---------------------------------------------------------------------------
//zámek délky dopravníku
void __fastcall TForm_parametry::scButton_zamek_DDClick(TObject *Sender)
{
	if(scButton_zamek_DD->ImageIndex==37)//když je zamèeno
	{
		scButton_zamek_DD->ImageIndex=38;
		DD_zamek=UNLOCKED;
		set(DELKA,ENABLED,false);
	}
	else//odemèeno
	{
		scButton_zamek_DD->ImageIndex=37;
		DD_zamek=LOCKED;
		set(DELKA,DISABLED,false);
	}
}
//---------------------------------------------------------------------------
//pøi kliknutí na doporuèení nastane aplikace doporuèení do daného editboxu
void __fastcall TForm_parametry::rHTMLLabel_InfoTextClick(TObject *Sender)
{
	 if(rHTMLLabel_InfoText->Caption.Pos("Doporuèený technologický èas je"))
	 {
		if(CTunit==MIN)scGPNumericEdit_CT->Value=Form1->m.round(scGPNumericEdit_kapacita->Value)*Form1->d.v.PP.TT/60.0;
		else scGPNumericEdit_CT->Value=Form1->m.round(scGPNumericEdit_kapacita->Value)*Form1->d.v.PP.TT;
	 }
	 if(rHTMLLabel_InfoText->Caption.Pos("Doporuèená kapacita je 1!"))
	 {
		scGPNumericEdit_kapacita->Value=1;
	 }
	 if(rHTMLLabel_InfoText->Caption.Pos("Zmìnte režim nebo rozložte do více objektù!"))
	 {
		scComboBox_rezim->ItemIndex=1;
	 }
}
//---------------------------------------------------------------------------
//kontrola vybraného pohonu vùèi zadané rychlosti dopravníku
void __fastcall TForm_parametry::scComboBox_pohonChange(TObject *Sender)
{
	if(scComboBox_rezim->ItemIndex!=0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{
			if(scGPNumericEdit_RD->Value<P->rychlost_od || P->rychlost_do<scGPNumericEdit_RD->Value)//nesplòuje rozmezí
			{
				vypis("Pohon neodpovídá rychlosti!",true);
				//-scGPButton_OK->Enabled=false;
			}
			else
			{
				vypis("");
				//-scGPButton_OK->Enabled=true;
			}
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry::scGPNumericEdit_kapacitaClick(TObject *Sender)
{
input_clicked_edit=C_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit_CTClick(TObject *Sender)
{
input_clicked_edit=CT_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit_RD_Click(TObject *Sender)
{
input_clicked_edit=RD_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuClick(TObject *Sender)

{
input_clicked_edit=DD_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit_mezeraClick(TObject *Sender)
{
input_clicked_edit==mezera_klik;
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPNumericEdit_mezeraChange(TObject *Sender)
{
	if(input_state==NOTHING && input_clicked_edit==mezera_klik)//pokud není zadáváno z jiného vstupu
	input_DD();//pøepoèet hodnot vyplývajících ze zmìny délky dopravníku
	//hlídání velikosti mezery dle rozteèe
	if(scComboBox_rezim->ItemIndex!=0 && scGPNumericEdit_mezera->Value>0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{                            //je "zbytek po dìlení"
			if(P->roztec>0 && !Form1->m.cele_cislo(scGPNumericEdit_mezera->Value/P->roztec))//nesplòuje rozmezí
			{
				vypis("Doporuèeno: "+AnsiString(Form1->m.round(scGPNumericEdit_mezera->Value/P->roztec)*P->roztec)+" m",true);
				//-scGPButton_OK->Enabled=false;
			}
			else
			{
				vypis("");
				//-scGPButton_OK->Enabled=true;
			}
		}
	}
}
//---------------------------------------------------------------------------

