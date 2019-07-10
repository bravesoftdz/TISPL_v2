//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "popUP_menu.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scModernControls"
#pragma resource "*.dfm"
TPopUPmenu *PopUPmenu;
//---------------------------------------------------------------------------
__fastcall TPopUPmenu::TPopUPmenu(TComponent* Owner)
	: TForm(Owner)
{
	//pozadí
	clBg=(TColor)RGB(240,240,240); //pasive
	clAcBg=(TColor)RGB(212,208,200);//active
	clAcBg2=(TColor)RGB(195,192,184);//active køížku
	//výplò glyphu
	clGlyph=(TColor)RGB(43,87,154);//pasive
	clAcGlyph=(TColor)RGB(0,128,255);//active

	PopUPmenu->Color=clBg;//nastavení pozadí barvy formuláøe
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::FormShow(TObject *Sender)
{
	//nutné znova nastavit, problém pøi více obrazovkách
	PopUPmenu->Left=Form1->Left+F->akt_souradnice_kurzoru_PX.x;
	PopUPmenu->Top=Form1->Top+F->akt_souradnice_kurzoru_PX.y;
	//ošetøení, pokud je mimo obrazovku + 5 px okraj
	if(PopUPmenu->Left>=Form1->Left+Form1->ClientWidth-PopUPmenu->Width)//nastala situace že je mimo obraz (nebo èásteènì)
	PopUPmenu->Left=Form1->Left+Form1->ClientWidth-PopUPmenu->Width-5;
	if(PopUPmenu->Top>=Form1->Top+Form1->ClientHeight-PopUPmenu->Height)
	PopUPmenu->Top=Form1->Top+Form1->ClientHeight-PopUPmenu->Height-5;
 	//ošetøení pokud je zobrazeno jenom dolní panel, tak se nezobrazí rozdìlovací linka - frame
	if(Panel_DOWN->Top==0)Panel_DOWN->FrameWidth=0;
	else Panel_DOWN->FrameWidth=1;

	pasiveColor();//nastavení pasivních barev, pøipravano na aktivaci
}
//---------------------------------------------------------------------------
void TPopUPmenu::pasiveColor()//nastaví všechny položky na pasivní resp. default barvu
{
	Item_zobrazit_parametry->FillColor=clBg;
	Item_nastavit_parametry->FillColor=clBg;
	Item_smazat->FillColor=clBg;
	Item_rychly_export->FillColor=clBg;
	Item_posouvat->FillColor=clBg;
	Item_posunout->FillColor=clBg;
	Item_priblizit->FillColor=clBg;
	Item_oddalit->FillColor=clBg;
	Item_vybrat_oknem->FillColor=clBg;
	Item_cely_pohled->FillColor=clBg;
	Item_kopirovat->FillColor=clBg;
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
	GlyphButton_smazat->Options->NormalColor=clGlyph;
	GlyphButton_smazat->GlyphOptions->NormalColor=clWhite;
	GlyphButton_smazat->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_rychly_export->Options->NormalColor=clGlyph;
	GlyphButton_rychly_export->GlyphOptions->NormalColor=clWhite;
	GlyphButton_rychly_export->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_posouvat->Options->NormalColor=clGlyph;
	GlyphButton_posouvat->GlyphOptions->NormalColor=clWhite;
	GlyphButton_posouvat->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_posunout->Options->NormalColor=clGlyph;
	GlyphButton_posunout-> GlyphOptions->NormalColor=clWhite;
	GlyphButton_posunout-> GlyphOptions->NormalColorAlpha=200;
	GlyphButton_priblizit->Options->NormalColor=clGlyph;
	GlyphButton_priblizit->GlyphOptions->NormalColor=clWhite;
	GlyphButton_priblizit->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_oddalit->Options->NormalColor=clGlyph;
	GlyphButton_oddalit->GlyphOptions->NormalColor=clWhite;
	GlyphButton_oddalit->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_vybrat_oknem->Options->NormalColor=clGlyph;
	GlyphButton_vybrat_oknem->GlyphOptions->NormalColor=clWhite;
	GlyphButton_vybrat_oknem->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_cely_pohled->Options->NormalColor=clGlyph;
	GlyphButton_cely_pohled->GlyphOptions->NormalColor=clWhite;
	GlyphButton_cely_pohled->GlyphOptions->NormalColorAlpha=200;
	GlyphButton_kopirovat->Options->NormalColor=clGlyph;
	GlyphButton_kopirovat->GlyphOptions->NormalColor=clWhite;
	GlyphButton_kopirovat->GlyphOptions->NormalColorAlpha=200;
	closing=false;
}
//---------------------------------------------------------------------------
//hlídání horní pozice, je-li daná komponenta horní
void TPopUPmenu::top_positon(int top)
{
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
void __fastcall TPopUPmenu::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch(Key)
	{
		//BACKSPACE
		case 8: break;
		//ENTER
		case 13:pasiveColor();MessageBeep(0);break;
		//ESC
		case 27:closing=true;Close();break;
		//MEZERNÍK
		case 32:break;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_closeClick(TObject *Sender)
{
  closing=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_closeMouseEnter(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_nastavit_parametryMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_nastavit_parametry->FillColor=clAcBg;
	GlyphButton_nastavit_parametry->Options->NormalColor=clAcBg;
	GlyphButton_nastavit_parametry->Options->HotColor=clAcBg;
	GlyphButton_nastavit_parametry->Options->FocusedColor=clAcBg;
	GlyphButton_nastavit_parametry->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_nastavit_parametry->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_nastavit_parametry->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_nastavit_parametryMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_nastavit_parametryMouseEnter(TObject *Sender)
{
	scLabel_nastavit_parametryMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_nastavit_parametryMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_nastavit_parametryClick(TObject *Sender)
{
	closing=true;
	Close();
 //	Form1->NP(); //staré volání PO
	if(Form1->pom_bod_temp==NULL)Form1->NP_input();
	else
	{
		if(Form1->pom_bod_temp->n!=1)Form1->d.v.vloz_bod(Form1->akt_souradnice_kurzoru.x,Form1->akt_souradnice_kurzoru.y,Form1->pom,Form1->pom_bod_temp->predchozi);
		else Form1->d.v.vloz_bod(Form1->akt_souradnice_kurzoru.x,Form1->akt_souradnice_kurzoru.y,Form1->pom);
  	Form1->REFRESH();
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_zobrazit_parametryMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_zobrazit_parametry->FillColor=clAcBg;
	GlyphButton_zobrazit_parametry->Options->NormalColor=clAcBg;
	GlyphButton_zobrazit_parametry->Options->HotColor=clAcBg;
	GlyphButton_zobrazit_parametry->Options->FocusedColor=clAcBg;
	GlyphButton_zobrazit_parametry->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_zobrazit_parametry->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_zobrazit_parametry->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_zobrazit_parametryMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_zobrazit_parametryMouseEnter(TObject *Sender)
{
	scLabel_zobrazit_parametryMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_zobrazit_parametryMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_zobrazit_parametryClick(TObject *Sender)
{
	closing=true;
	Item_zobrazit_parametry->FillColor=clBg;//workaround, nutnost takto vytáhnout, jinak se položka pozdìji zvýrazòuje, musí být tady
	Close();
	Form1->Zobrazitparametry1Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_kopirovatMouseEnter(TObject *Sender)
{
 	pasiveColor();
	Item_kopirovat->FillColor=clAcBg;
	GlyphButton_kopirovat->Options->NormalColor=clAcBg;
	GlyphButton_kopirovat->Options->HotColor=clAcBg;
	GlyphButton_kopirovat->Options->FocusedColor=clAcBg;
	GlyphButton_kopirovat->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_kopirovat->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_kopirovat->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_smazatLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_kopirovatMouseEnter(TObject *Sender)
{
	scLabel_kopirovatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_kopirovatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_kopirovatClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->kopirovat_objekt();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_smazatMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_smazat->FillColor=clAcBg;
	GlyphButton_smazat->Options->NormalColor=clAcBg;
	GlyphButton_smazat->Options->HotColor=clAcBg;
	GlyphButton_smazat->Options->FocusedColor=clAcBg;
	GlyphButton_smazat->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_smazat->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_smazat->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_smazatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_smazatMouseEnter(TObject *Sender)
{
 scLabel_smazatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_smazatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_smazatClick(TObject *Sender)
{
	closing=true;
	PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);//workaround
	Close();
	Form1->Smazat1Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_rychly_exportMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_rychly_export->FillColor=clAcBg;
	GlyphButton_rychly_export->Options->NormalColor=clAcBg;
	GlyphButton_rychly_export->Options->HotColor=clAcBg;
	GlyphButton_rychly_export->Options->FocusedColor=clAcBg;
	GlyphButton_rychly_export->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_rychly_export->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_rychly_export->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_rychly_exportMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//--------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_rychly_exportMouseEnter(TObject *Sender)
{
	scLabel_rychly_exportMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_rychly_exportMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_rychly_exportClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Rychlexport1Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_posouvatMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_posouvat->FillColor=clAcBg;
	GlyphButton_posouvat->Options->NormalColor=clAcBg;
	GlyphButton_posouvat->Options->HotColor=clAcBg;
	GlyphButton_posouvat->Options->FocusedColor=clAcBg;
	GlyphButton_posouvat->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_posouvat->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_posouvat->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_posouvatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_posouvatMouseEnter(TObject *Sender)
{
	scLabel_posouvatMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_posouvatMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_posouvatClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Posouvat1Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_posunoutMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_posunout->FillColor=clAcBg;
	GlyphButton_posunout->Options->NormalColor=clAcBg;
	GlyphButton_posunout->Options->HotColor=clAcBg;
	GlyphButton_posunout->Options->FocusedColor=clAcBg;
	GlyphButton_posunout->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_posunout->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_posunout->Top);//hlídání horní pozice, je-li daná komponenta horní
}
void __fastcall TPopUPmenu::scLabel_posunoutMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_posunoutMouseEnter(TObject *Sender)
{
  scLabel_posunoutMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_posunoutMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_posunoutClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Posunout2Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_priblizitMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_priblizit->FillColor=clAcBg;
	GlyphButton_priblizit->Options->NormalColor=clAcBg;
	GlyphButton_priblizit->Options->HotColor=clAcBg;
	GlyphButton_priblizit->Options->FocusedColor=clAcBg;
	GlyphButton_priblizit->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_priblizit->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_priblizit->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_priblizitMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_priblizitMouseEnter(TObject *Sender)
{
  scLabel_priblizitMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_priblizitMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_priblizitClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Priblizit2Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_oddálitMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_oddalit->FillColor=clAcBg;
	GlyphButton_oddalit->Options->NormalColor=clAcBg;
	GlyphButton_oddalit->Options->HotColor=clAcBg;
	GlyphButton_oddalit->Options->FocusedColor=clAcBg;
	GlyphButton_oddalit->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_oddalit->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_oddalit->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_oddálitMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_oddalitMouseEnter(TObject *Sender)
{
	scLabel_oddálitMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_oddalitMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_oddálitClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Oddalit2Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_vybrat_oknemMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_vybrat_oknem->FillColor=clAcBg;
	GlyphButton_vybrat_oknem->Options->NormalColor=clAcBg;
	GlyphButton_vybrat_oknem->Options->HotColor=clAcBg;
	GlyphButton_vybrat_oknem->Options->FocusedColor=clAcBg;
	GlyphButton_vybrat_oknem->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_vybrat_oknem->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_vybrat_oknem->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_vybrat_oknemMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_vybrat_oknemMouseEnter(TObject *Sender)
{
  scLabel_vybrat_oknemMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_vybrat_oknemMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_vybrat_oknemClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->Vybratoknem1Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_cely_pohledMouseEnter(TObject *Sender)
{
	pasiveColor();
	Item_cely_pohled->FillColor=clAcBg;
	GlyphButton_cely_pohled->Options->NormalColor=clAcBg;
	GlyphButton_cely_pohled->Options->HotColor=clAcBg;
	GlyphButton_cely_pohled->Options->FocusedColor=clAcBg;
	GlyphButton_cely_pohled->GlyphOptions->NormalColor=clAcGlyph;
	GlyphButton_cely_pohled->GlyphOptions->NormalColorAlpha=255;
	top_positon(Item_cely_pohled->Top);//hlídání horní pozice, je-li daná komponenta horní
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_cely_pohledMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_cely_pohledMouseEnter(TObject *Sender)
{
  scLabel_cely_pohledMouseEnter(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::GlyphButton_cely_pohledMouseLeave(TObject *Sender)
{
	pasiveColor();
}
//---------------------------------------------------------------------------
void __fastcall TPopUPmenu::scLabel_cely_pohledClick(TObject *Sender)
{
	closing=true;
	Close();
	Form1->RzToolButton11Click(Sender);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


