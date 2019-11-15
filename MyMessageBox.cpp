//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MyMessageBox.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TmyMessageBox *myMessageBox;
//---------------------------------------------------------------------------
__fastcall TmyMessageBox::TmyMessageBox(TComponent* Owner)
	: TForm(Owner)
{
	myMessageBox->Color=(TColor)RGB(240,240,240);//nastavení pozadí barvy formuláøe
	zobrazitFrameForm=false;
}
//---------------------------------------------------------------------------
void __fastcall TmyMessageBox::FormShow(TObject *Sender)
{
	//ošetøení, pokud je mimo obrazovku + 5 px okraj
	if(myMessageBox->Left>=Form1->Left+Form1->ClientWidth-myMessageBox->Width)//nastala situace že je mimo obraz (nebo èásteènì)
	myMessageBox->Left=Form1->Left+Form1->ClientWidth-myMessageBox->Width-5;
	if(myMessageBox->Top>=Form1->Top+Form1->ClientHeight-myMessageBox->Height)
	myMessageBox->Top=Form1->Top+Form1->ClientHeight-myMessageBox->Height-5;
	scGPEdit1->SetFocus();//použito kvùli odchytávání stisku kláves
}
//---------------------------------------------------------------------------
//pøetížená metoda
int TmyMessageBox::Show(UnicodeString text,int mbTYPE,bool centrovat_text,int width,bool default_button_caption,bool copy_zobrazit)
{
	 return Show(-1,-1,text,"",mbTYPE,centrovat_text,false,width,default_button_caption,copy_zobrazit);
}
int TmyMessageBox::Show(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit,int width,bool default_button_caption,bool copy_zobrazit)
{
	short O=26;//výchozí offset pro zohlednìní checkboxu
	myMessageBox->Width=width;//výchozí šíøka MB z implicitního parametru 366px, pokud není uživatelsky zadaná

	////naplnìní daty + naformátování Label_text
	Label_text->Width=myMessageBox->Width-8-8;//pøevzetí šíøky labelu dle šíøky formu - oba okraje
	Label_text->Caption=text;
	if(centrovat_text)Label_text->Alignment=taCenter;else Label_text->Alignment=taLeftJustify;
	if(text.Length()>=46)Label_text->AutoSize=true;else Label_text->AutoSize=false;
	//výška myMessageBoxu dle zadaného textu
	myMessageBox->ClientHeight=139-O+Label_text->Height-19;//poèítáno oproti výchozí pozici

	//Caption naplnìní daty
	if(caption_text!="")scLabel_caption->Caption=caption_text;else scLabel_caption->Caption=Application->Title; //hlídání zda nepøijde prázdný øetezec

	//checkbox
	CheckBox_pamatovat->Visible=checkbox_zobrazit;
	if(checkbox_zobrazit)myMessageBox->Height+=O;//pokud je checkbox zobrazen, je formáláø o 10px vìtší než je konec tlaèítek
	CheckBox_pamatovat->Top=myMessageBox->Height-O;

	////tlaèítka
	//caption tlaèítka - pro pøípad, že by v pøedchozím volání MB bylo tlaèítko pøejmenované
	if(default_button_caption)
	{
		if(F->ls->Strings[186]!="")Button_OK->Caption=F->ls->Strings[186];else Button_OK->Caption="OK";
		if(F->ls->Strings[188]!="")Button_Cancel->Caption=F->ls->Strings[188];else Button_Cancel->Caption="Storno";
		if(F->ls->Strings[185]!="")Button_Yes->Caption=F->ls->Strings[185];else Button_Yes->Caption="Ano";
		if(F->ls->Strings[187]!="")Button_No->Caption=F->ls->Strings[187];else Button_No->Caption="Ne";
	}
  //zobrazení èi skrytí tlaèítka kopíruj do schránky
	if(copy_zobrazit)scGPGlyphButton_copy->Visible=true;
	else scGPGlyphButton_copy->Visible=false;

	//horizontální umístìní
	switch(mbTYPE) //OK=0,OKCANCEL,YESNO,YESNOCANCEL
	{
			case OK:
			{
				Button_OK->Visible=true;
				Button_Cancel->Visible=false;
				Button_Yes->Visible=false;
				Button_No->Visible=false;
				Button_OK->Left=myMessageBox->Width/2-Button_OK->Width/2;//vycentruje
			}break;
			case OKCANCEL:
			{
				Button_OK->Visible=true;
				Button_Cancel->Visible=true;
				Button_Yes->Visible=false;
				Button_No->Visible=false;
				Button_OK->Left=myMessageBox->Width/2-Button_OK->Width-Button_OK->Width/4;
				Button_Cancel->Left=myMessageBox->Width/2+Button_Cancel->Width/4;
			}break;
			case YESNO:
			{
				Button_OK->Visible=false;
				Button_Cancel->Visible=false;
				Button_Yes->Visible=true;
				Button_No->Visible=true;
				Button_Yes->Left=myMessageBox->Width/2-Button_Yes->Width-Button_Yes->Width/4;
				Button_No->Left=myMessageBox->Width/2+Button_No->Width/4;
			}break;
			case YESNOCANCEL:
			{
				Button_OK->Visible=false;
				Button_Cancel->Visible=true;
				Button_Yes->Visible=true;
				Button_No->Visible=true;
				Button_Yes->Left=myMessageBox->Width/3-myMessageBox->Width/3/2-Button_Yes->Width/2;
				Button_No->Left=myMessageBox->Width/3+myMessageBox->Width/3/2-Button_No->Width/2;
				Button_Cancel->Left=myMessageBox->Width*2/3+myMessageBox->Width/3/2-Button_Cancel->Width/2;if(copy_zobrazit)myMessageBox->Width+=scGPGlyphButton_copy->Width+11+10;
			}break;
	}
	//vertikální umístìní
	if(!checkbox_zobrazit)O=11;//11px je default odsazení všech buttonu v našem designu
	Button_OK->Top=myMessageBox->Height-Button_OK->Height-O;//umístí vertikálnì
	Button_Yes->Top=Button_OK->Top;
	Button_No->Top=Button_OK->Top;
	Button_Cancel->Top=Button_OK->Top;
	scGPGlyphButton_copy->Top=Button_OK->Top;//Button_OK->Top;
	scGPGlyphButton_copy->Left=myMessageBox->Width-12-scGPGlyphButton_copy->Width;
	////pozice formuláøe
	if(left<0 && top<0)//na støed, pokud se zadá libovolné záporné èíslo
	{
		myMessageBox->Left=Form1->Left+Form1->Width/2-myMessageBox->Width/2;
		myMessageBox->Top=Form1->Top+Form1->Height/2-myMessageBox->Height/2;
	}
	else//dle zadaných souøadnic
	{
		myMessageBox->Left=left;
		myMessageBox->Top=top;
		if(left<0)myMessageBox->Left=5;//ještì ošetøení samostatných záporných souøadnic
		if(top<0)myMessageBox->Top=5;//ještì ošetøení samostatných záporných souøadnic
	}

	////volá samotné zobrazení
	return ShowModal();
}
//---------------------------------------------------------------------------
//Køížek
void __fastcall TmyMessageBox::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
//pøi stisku klávesy v MessageBoxu
void __fastcall TmyMessageBox::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch(Key)
	{
		//BACKSPACE
		case 8: break;
		//ENTER
		case 13:if(Button_OK->Visible)Button_OK->Down=true;else Button_Yes->Down=true;break;
		//ESC
		case 27:Close();break;
		//MEZERNÍK
		case 32:break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TmyMessageBox::FormPaint(TObject *Sender)
{
	//zajišuje pøekreslování orámování okolo
	if(zobrazitFrameForm)Form1->m.frameForm(myMessageBox,clWebOrange,1);
}
//---------------------------------------------------------------------------
//kopírování obsahu label_text do schránky
void __fastcall TmyMessageBox::scGPGlyphButton_copyClick(TObject *Sender)
{
	Clipboard()->AsText=Label_text->Caption;
}
//---------------------------------------------------------------------------
//funguje pouze jako pøesmìrování této události na form, edit má na sobì neustále focus
void __fastcall TmyMessageBox::scGPEdit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	FormKeyDown(this,Key,Shift);
}
//---------------------------------------------------------------------------

