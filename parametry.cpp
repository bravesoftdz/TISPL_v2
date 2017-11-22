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

	//formuláø bude pøi prvním zobrazení v sekundách a metrech nebo dle INI v odvozených jednotkách, jinak dle SI
	minsec=S;m_mm=M;
	if(Form1->readINI("nastaveni_form_parametry","cas")=="1")Button_min_secClick(this);//tedy MIN
	if(Form1->readINI("nastaveni_form_parametry","vzdalenost")=="1")Button_metry_milimetryClick(this);//tedy MM
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	input_state=NOTHING;//nutnost
	kapacitaSG=1;//není podnìt k rozkládání na více objektù
	scGPEdit_name->SetFocus();//nastaví výchozí focus, kde se pøedpokládá výchozí nastavování
	scGPEdit_name->SelectAll();//oznaèí cele pro editace
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
			{scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;}
			else
			{scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;}
		}
		else
		{
			scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor=(TColor)RGB(0,128,255);
			rHTMLLabel_InfoText->Font->Color=(TColor)RGB(0,128,255);
		}
		scGPGlyphButton_InfoIcon->Top=Form_parametry->Height-81;
		scGPGlyphButton_InfoIcon->Visible=true;
		rHTMLLabel_InfoText->Top=Form_parametry->Height-74;
		rHTMLLabel_InfoText->Visible=true;
		rHTMLLabel_InfoText->Caption=text;
	}
	else//skryje
	{
		scGPGlyphButton_InfoIcon->Visible=false;
		rHTMLLabel_InfoText->Visible=false;
		//zvýraznìní položky technologický èas
		if(scGPNumericEdit_CT->Enabled){scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;}
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

			//napozicování celého formuláøe resp. ošetøení aby zùstal dialog na monitoru
			if(Form1->akt_souradnice_kurzoru_PX.x+10+Form_parametry->ClientWidth<Form1->ClientWidth)
				Form_parametry->Left=Form1->akt_souradnice_kurzoru_PX.x+10;
			else
				Form_parametry->Left=Form1->ClientWidth-Form_parametry->ClientWidth-10;
			if(Form1->akt_souradnice_kurzoru_PX.y+10+Form_parametry->ClientHeight<Form1->ClientHeight)
				Form_parametry->Top=Form1->akt_souradnice_kurzoru_PX.y+10;
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
	if (Form1->STATUS==Form1->NAVRH) rezim+=10;//posunutí o 10 vytváøí režim+navrháø
	switch(rezim)
	{
		 case 0://STOP & GO
		 {
			 set(POHON,ENABLED);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,HIDE);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,ENABLED);
			 set(CEKANI,ENABLED);
			 set(STOPKA,ENABLED);
			 set(ROTACE,HIDE);
			 set(MEZERA,HIDE);
			 set(POCET_MEZER,HIDE);
			 scGPNumericEdit_kapacita->Value=1;
		 }break;
		 case 10://STOP & GO - NÁVRHÁØ
		 {
			 //nastavení komponent
			 set(POHON,HIDE);
			 set(TIME,ENABLED);
			 set(RYCHLOST,HIDE);
			 set(DELKA,HIDE);
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
			 set(POHON,ENABLED);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,ENABLED);
			 set(STOPKA,ENABLED);
			 set(ROTACE,ENABLED);
			 set(MEZERA,ENABLED);
			 set(POCET_MEZER,ENABLED);
		 }break;
		 case 11://KONTINUÁLNÍ - NÁVRHÁØ
		 {
			 set(POHON,HIDE);
			 set(TIME,ENABLED);
			 set(RYCHLOST,READONLY);
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
			 set(POHON,HIDE);
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
void TForm_parametry::set(Tcomponents C,Tcomponents_state S)
{
	//defaultní hodnoty
	short O=40;//vertikální velikost odsazení komponent
	int L=rHTMLLabel_rezim->Top;//výchozí komponenta
	int P=scComboBox_rezim->Top;//výchozí komponenta

	switch (C)
	{
		case POHON:
		{
		 ////pozice
			rHTMLLabel_pohon->Top=L+O;
			scComboBox_pohon->Top=P+O;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=true;
			scComboBox_pohon->Options->FrameNormalColor=clGray;scComboBox_pohon->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scComboBox_pohon->Options->FrameNormalColor=hl_color;scComboBox_pohon->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=false;break;
				case READONLY:rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=false;break;
				case HIDE:		rHTMLLabel_pohon->Visible=false;scComboBox_pohon->Visible=false;offset-=O;break;
			}
		}	break;
		case TIME://technologický èas v režimu návrháø
		{
		 ////pozice
			rHTMLLabel_CT->Top=L+2*O+offset;
			scGPNumericEdit_CT->Top=P+2*O+offset;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_CT->Visible=true;scGPNumericEdit_CT->Enabled=true;scGPNumericEdit_CT->Visible=true;
			scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_CT->Enabled=false;break;
				case READONLY:scGPNumericEdit_CT->Enabled=false;break;
				case HIDE:		rHTMLLabel_CT->Visible=false;scGPNumericEdit_CT->Visible=false;offset-=O;break;
			}
		}	break;
		case RYCHLOST://RD v režimu návrháø
		{
		 ////pozice
			rHTMLLabel_RD->Top=L+3*O+offset;
			scGPNumericEdit_RD->Top=P+3*O+offset;
		 ////funkèní vlastnosti
			//ty co jsou stejné
			scGPNumericEdit_RD->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_RD->Visible=true;scGPNumericEdit_RD->Visible=true;scGPNumericEdit_RD->Enabled=true;
			scGPNumericEdit_RD->Options->FrameNormalColor=clGray;scGPNumericEdit_RD->Options->FrameWidth=1;
			//ty co jsou rozdílné
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_RD->Options->FrameNormalColor=hl_color;scGPNumericEdit_RD->Options->FrameWidth=hlFrameWidth;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_RD->Enabled=false;break;
				case READONLY:scGPNumericEdit_RD->Options->ShapeStyle=scgpessNone;scGPNumericEdit_RD->Enabled=false;break;
				case HIDE:		rHTMLLabel_RD->Visible=false;scGPNumericEdit_RD->Visible=false;offset-=O;break;
			}
		}	break;
		case DELKA://délka dopravníku
		{
		 ////pozice
			rHTMLLabel_delka_dopravniku->Top=L+4*O+offset;
			scGPNumericEdit_delka_dopravniku->Top=P+4*O+offset;
			//ty co jsou stejné
			scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Enabled=true;
			scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=clGray;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=1;
			//ty co jsou rozdílné
		 ////funkèní vlastnosti
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=hl_color;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_delka_dopravniku->Enabled=false;break;
				case READONLY:scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessNone;scGPNumericEdit_delka_dopravniku->Enabled=false;break;
				case HIDE:		rHTMLLabel_delka_dopravniku->Visible=false;scGPNumericEdit_delka_dopravniku->Visible=false;offset-=O;break;
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
				case HIDE:		rHTMLLabel_kapacita->Visible=false;scGPNumericEdit_kapacita->Visible=false;offset-=O;break;
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
				case HIDE:		rHTMLLabel_odchylka->Visible=false;scGPNumericEdit_odchylka->Visible=false;offset-=O;break;
			}
		}	break;
		case CEKANI://èekání
		{
		 ////pozice
			rHTMLLabel_cekani->Top=L+7*O+offset;
			scComboBox_cekani_palec->Top=P+7*O+offset;
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
				case HIDE:		rHTMLLabel_cekani->Visible=false;scComboBox_cekani_palec->Visible=false;offset-=O;break;
			}
		}	break;
		case STOPKA://stop stanice na konci objektu
		{
		 ////pozice
			rHTMLLabel_stopka->Top=L+8*O+offset;
			scComboBox_stopka->Top=P+8*O+offset;
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
				case HIDE:		rHTMLLabel_stopka->Visible=false;scComboBox_stopka->Visible=false;offset-=O;break;
			}
		}	break;
		case ROTACE://rotace jigu v objektu, zatím jen pøepínátko 0-90
		{
		 ////pozice
			rHTMLLabel_rotace->Top=L+9*O+offset;
			scComboBox_rotace->Top=P+9*O+offset;
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
				case HIDE:		rHTMLLabel_rotace->Visible=false;scComboBox_rotace->Visible=false;offset-=O;break;
			}
		}	break;
		case MEZERA://požadována vs. zjištìná kapacita objektu
		{
		 ////pozice
			rHTMLLabel_mezera->Top=L+10*O+offset;
			scGPNumericEdit_mezera->Top=P+10*O+offset;
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
				case HIDE:		rHTMLLabel_mezera->Visible=false;scGPNumericEdit_mezera->Visible=false;offset-=O;break;
			}
		}	break;
		case POCET_MEZER://požadována vs. zjištìná kapacita objektu
		{
		 ////pozice
			scGPCheckBox_pocet_mezer->Top=L+11*O-5+offset;
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
				case HIDE:		scGPCheckBox_pocet_mezer->Visible=false;offset-=O;break;
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
	if(input_state==NOTHING)//pokud není zadáváno z jiného vstupu
	input_CT();//pøepoèet hodnot vyplývajících ze zmìny CT
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud není zadáváno z jiného vstupu
	input_DD();//pøepoèet hodnot vyplývajících ze zmìny délky dopravníku
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RDChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud není zadáváno z jiného vstupu
	input_RD();//pøepoèet hodnot vyplývajících ze zmìny RD
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud není zadáváno z jiného vstupu
	input_K();//pøepoèet hodnot vyplývajících ze zmìny K
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pøepoèet hodnot vyplývajících ze zmìny CT
void TForm_parametry::input_CT()
{
	 input_state=CT;
	 double CT=scGPNumericEdit_CT->Value;//CT od uživatele
	 if(minsec==MIN)CT=CT*60.0;//pokud bylo zadání v minutách pøevede na sekundy
	 if(CT>0)//nutné ošetøení pro období zadávání/psaní
	 {
			 //default nastavení komponent
			 scGPNumericEdit_kapacita->Decimal=0;
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
					vypis("Pozor, technologický èas je nižší než hodnota TT!");
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
    	 }
    	 else//KONTINUAL+PP
			 {
    		 //KAPACITA
				 double K=CT/Form1->d.v.PP.TT;//výpoèet
    		 scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
    		 scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
    		 //pokud obsahuje kapacita reálnou èást, vypíše doporuèení
				 if(Form1->ms.get_count_decimal(K)>0)
				 {
					if(minsec==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
					else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
				 }
				 //DÉLKA DOPRAVNÍKU
				 double jednotky_vzdalenost=1.0;if(m_mm=MM)jednotky_vzdalenost=1000.0;
				 double DD=K*dV+(K-p)*m;//ošetøeno i pro stav kdy je stejný poèet mezer jako vozíku
				 scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
				 scGPNumericEdit_delka_dopravniku->Value=DD*jednotky_vzdalenost;//plnìní patøièného políèka
				 //RYCHLOST DOPRAVNÍKU (èistì jen pro KONTINUAL)
				 if(scComboBox_rezim->ItemIndex==1)
				 {
					double RD=DD/CT;
					if(minsec==MIN)RD*=60.0;
					scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_RD->Value=RD*jednotky_vzdalenost;//plnìní patøièného políèka
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
	double jednotky_vzdalenost=1.0;if(m_mm=MM)jednotky_vzdalenost=1000.0;
	input_state=DD;//pozor myšleno DD - z ENUM
	//default hodnoty
	double DD=scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;//pøevede vždy do metrù
	if(DD>0)//nutné ošetøení pro období zadávání/psaní
	{
		//default nastavení komponent
		scGPNumericEdit_kapacita->Decimal=0;
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
		scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
		scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
		//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
		if(Form1->ms.get_count_decimal(K)>0)
		{
			if(minsec==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
			else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
		}
		//PROCESNÍ ÈAS resp. CT
		double CT = Form1->d.v.PP.TT*K;
		if(minsec==MIN)
		{
			scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastaví zobrazení poètu desetinných míst
			scGPNumericEdit_CT->Value=CT/60.0;//plnìní patøièného políèka
		}
		else
		{
			scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
			scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
		}

		//RYCHLOST DOPRAVNÍKU (èistì jen pro KONTINUAL)
		if(scComboBox_rezim->ItemIndex==1)
		{
			double RD = DD/CT;
			if(minsec==MIN)RD*=60.0;
			scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
			scGPNumericEdit_RD->Value=RD*jednotky_vzdalenost;//plnìní patøièného políèka
		}
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
	 double jednotky_vzdalenost=1.0;if(m_mm=MM)jednotky_vzdalenost=1000.0;
	 double jednotky_cas=1.0;if(m_mm=MM)jednotky_cas=60.0;
	 double RD=scGPNumericEdit_RD->Value/jednotky_vzdalenost/jednotky_cas;//pøevede na metr a na jednu sekundu

	 if(RD>0)//nutné ošetøení pro období zadávání/psaní
	 {
			 //default nastavení komponent
			 scGPNumericEdit_kapacita->Decimal=0;
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
    			scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
    			scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
    			//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
					if(Form1->ms.get_count_decimal(K)>0)
					{
						if(minsec==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
						else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
					}

					//DÉLKA DOPRAVNÍKU
					double DD = K*dV+(K-p)*m;
					scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_delka_dopravniku->Value=DD*jednotky_vzdalenost;//plnìní patøièného políèka

    			//PROCESNÍ ÈAS resp. CT
					double CT = RD/DD;
					scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/jednotky_cas);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_CT->Value=CT/jednotky_cas;//plnìní patøièného políèka
			 }
    	 else //pokud ne, je nutné získat další nenulový vstup
			 {
					double DD=scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;
					double CT=scGPNumericEdit_CT->Value*jednotky_cas;
    			double K=scGPNumericEdit_kapacita->Value;

    			if(DD!=0)//pokud je známá délka dopravníku
    			{
    				//KAPACITA
						K=RD/DD/Form1->d.v.PP.TT;
    				scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
    				scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
    				//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
						if(Form1->ms.get_count_decimal(K)>0)
						{
							if(minsec==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
							else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
						}

    				//PROCESNÍ ÈAS resp. CT
						double CT = RD/DD;
						scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/jednotky_cas);//nastaví zobrazení poètu desetinných míst
						scGPNumericEdit_CT->Value=CT/jednotky_cas;//plnìní patøièného políèka
    			}
    			else
    			{
    				if(CT!=0)//pokud není známá délka ale je známe CT
    				{
        			//DÉLKA DOPRAVNÍKU
        			DD=RD*CT;
							scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
							scGPNumericEdit_delka_dopravniku->Value=DD*jednotky_vzdalenost;//plnìní patøièného políèka

        			//KAPACITA
							K=CT/Form1->d.v.PP.TT;
        			scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastaví zobrazení poètu desetinných míst
        			scGPNumericEdit_kapacita->Value=K;//plnìní patøièného políèka
    					//pokud obsahuje kapacita reálnou èást, vypíše doporuèení
							if(Form1->ms.get_count_decimal(K)>0)
							{
								if(minsec==MIN)vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
								else vypis("Doporuèený technologický èas je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
							}
						}
    				else
    				{
    					if(K!=0)//pokud není známá délka ani CT a je známá K
    					{
    						//DÉLKA DOPRAVNÍKU
    						DD=RD*Form1->d.v.PP.TT*K;
								scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
								scGPNumericEdit_delka_dopravniku->Value=DD*jednotky_vzdalenost;//plnìní patøièného políèka

    						//PROCESNÍ ÈAS resp. CT
								double CT = RD/DD;
								scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/jednotky_cas);//nastaví zobrazení poètu desetinných míst
								scGPNumericEdit_CT->Value=CT/jednotky_cas;//plnìní patøièného políèka
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
	 input_state=C;
	 double K=scGPNumericEdit_kapacita->Value;//získání kapacity od uživatele
	 if(K>0)//nutné ošetøení pro období zadávání/psaní
	 {
    	 //default nastavení komponent
    	 scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Uložit";
			 double jednotky_vzdalenost=1.0;if(m_mm=MM)jednotky_vzdalenost=1000.0;

			 //volba šíøka èi délka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je požadován šíøka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//poèet mezer mezi vozíky

			 //PROCESNÍ ÈAS resp. CT
			 double CT = Form1->d.v.PP.TT*K;
			 if(minsec==MIN)
			 {
					scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_CT->Value=CT/60.0;//plnìní patøièného políèka
			 }
			 else
			 {
					scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_CT->Value=CT;//plnìní patøièného políèka
			 }

    	 //DÉLKA DOPRAVNÍKU
			 double DD = K*dV+(K-p)*m;//ošetøeno i pro stav stejného poètu vozíku a mezer;
			 scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
			 scGPNumericEdit_delka_dopravniku->Value=DD*jednotky_vzdalenost;//plnìní patøièného políèka

    	 //RYCHLOST DOPRAVNÍKU (èistì jen pro KONTINUAL)
    	 if(scComboBox_rezim->ItemIndex==1)
			 {
					double RD=DD/CT;
					if(minsec==MIN)RD*=60;
					scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst
					scGPNumericEdit_RD->Value=RD*jednotky_vzdalenost;//plnìní patøièného políèka
			 }
	 }
	 else
	 null_input_value();
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
		minsec=S;scGPButton_min_sec->Caption="na min";//samotné tlaèítko,ukazuje název opaènì
		//CT - pøepoèítání
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologický èas [s]";
		//RD - pøepoèítání
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost dopravníku [mm/s]";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost dopravníku [m/s]";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value/60.0;

	}
	else//pokud je v sekundách pøepne na minuty
	{
		minsec=MIN;scGPButton_min_sec->Caption="na s";//samotné tlaèítko,ukazuje název opaènì
		//CT - pøepoèítání
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologický èas [min]";
		//RD - pøepoèítání
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost dopravníku [mm/min]";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost dopravníku [m/min]";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value*60.0;
	}
	//plnìní + poèet desetinných míst
	scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_CT->Value=CT;
	scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//pøepínání zobrazení m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender)
{
	input_state=NO;//zámìr, aby se nepøepoèítavaly hodnoty
	double DD=0.0;double DM=0.0;double RD=0.0;
	if(m_mm==MM)//pokud je v milimetrech, tak pøepne na metry
	{
		m_mm=M;scGPButton_metry_milimetry->Caption="na mm";//samotné tlaèítko,ukazuje název opaènì
		rHTMLLabel_delka_dopravniku->Caption="Délka dopravníku [m]";
		DD=scGPNumericEdit_delka_dopravniku->Value/1000.0;
		rHTMLLabel_mezera->Caption="Délka mezery mezi vozíky [m]";
		DM=scGPNumericEdit_mezera->Value/1000.0;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost dopravníku [m/min]";//pokud je v minutách
		else rHTMLLabel_RD->Caption="Rychlost dopravníku [m/s]";// pokud je v sekundách
		RD=scGPNumericEdit_RD->Value/1000.0;
	}
	else//pokud je metrech, tak pøepne na milimetry
	{
		m_mm=MM;scGPButton_metry_milimetry->Caption="na m";//samotné tlaèítko,ukazuje název opaènì
		rHTMLLabel_delka_dopravniku->Caption="Délka dopravníku [mm]";
		DD=scGPNumericEdit_delka_dopravniku->Value*1000.0;
		rHTMLLabel_mezera->Caption="Délka mezery mezi vozíky [mm]";
		DM=scGPNumericEdit_mezera->Value*1000.0;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost dopravníku [mm/min]";//pokud je v minutách
		else rHTMLLabel_RD->Caption="Rychlost dopravníku [mm/s]";// pokud je v sekundách
		RD=scGPNumericEdit_RD->Value*1000.0;
	}
	//plnìní + poèet desetinných míst
	scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_delka_dopravniku->Value=DD;
	scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_mezera->Value=DM;
	scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastaví zobrazení poètu desetinných míst
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//už se mohou pøepoèítávat
}
//---------------------------------------------------------------------------
//pøi stisku klávesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 if(Key==13)//ENTER
 {
		 Form_parametry->ModalResult=mrOk;//vrátí stejnou hodnotu jako tlaèítko
		 Form_parametry->VisibleChanging();//skryje form, stejné jako visible=false
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

	//atributy
	Form1->copyObjekt->name=Form_parametry->scGPEdit_name->Text;
	Form1->copyObjekt->short_name=Form_parametry->scGPEdit_shortname->Text;
	Form1->copyObjekt->delka_dopravniku=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;
	Form1->copyObjekt->pohon=Form1->d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex+1);//indexuje se od nuly
	Form1->copyObjekt->rezim=Form_parametry->scComboBox_rezim->ItemIndex;
	Form1->copyObjekt->CT=scGPNumericEdit_CT->Value*jednotky_cas;
	Form1->copyObjekt->cekat_na_palce=Form_parametry->scComboBox_cekani_palec->ItemIndex;
	Form1->copyObjekt->kapacita=Form_parametry->scGPNumericEdit_kapacita->Value;
	Form1->copyObjekt->odchylka=Form_parametry->scGPNumericEdit_odchylka->Value;
	Form1->copyObjekt->stopka=Form_parametry->scComboBox_stopka->ItemIndex;
	Form1->copyObjekt->RD=Form_parametry->scGPNumericEdit_RD->Value/jednotky_cas/jednotky_vzdalenost;
	Form1->copyObjekt->rotace=Form_parametry->scComboBox_rotace->ItemIndex;
	Form1->copyObjekt->mezera=Form_parametry->scGPNumericEdit_mezera->Value/jednotky_vzdalenost;
	Form1->copyObjekt->mV=!Form_parametry->scGPCheckBox_pocet_mezer->Checked;

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
	scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n-1;
	scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->CT/jednotky_cas);//nastaví zobrazení poètu desetinných míst;
	scGPNumericEdit_CT->Value=Form1->copyObjekt->CT/jednotky_cas;
	scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst;
	scGPNumericEdit_RD->Value=Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost;
	scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost);//nastaví zobrazení poètu desetinných míst;
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


void __fastcall TForm_parametry::scGPCheckBox_pocet_mezerClick(TObject *Sender)
{
 if(input_state==NOTHING)
 ShowMessage("dodìlat aktualizaci po kliknutí");
}
//---------------------------------------------------------------------------





