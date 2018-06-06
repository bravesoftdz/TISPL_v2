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
#pragma resource "*.dfm"
TmyMessageBox *myMessageBox;
//---------------------------------------------------------------------------
__fastcall TmyMessageBox::TmyMessageBox(TComponent* Owner)
	: TForm(Owner)
{
	myMessageBox->Color=(TColor)RGB(240,240,240);//nastaven� pozad� barvy formul��e
	zobrazitFrameForm=false;
}
//---------------------------------------------------------------------------
void __fastcall TmyMessageBox::FormShow(TObject *Sender)
{
	//o�et�en�, pokud je mimo obrazovku + 5 px okraj
	if(myMessageBox->Left>=Form1->ClientWidth-myMessageBox->Width)//nastala situace �e je mimo obraz (nebo ��ste�n�)
	myMessageBox->Left=Form1->ClientWidth-myMessageBox->Width-5;
	if(myMessageBox->Top>=Form1->ClientHeight-myMessageBox->Height)
	myMessageBox->Top=Form1->ClientHeight-myMessageBox->Height-5;
}
//---------------------------------------------------------------------------
//p�et�en� metoda
int TmyMessageBox::Show(UnicodeString text,int mbTYPE,bool centrovat_text,int width,bool default_button_caption)
{
	 return Show(-1,-1,text,"",mbTYPE,centrovat_text,false,width,default_button_caption);
}
int TmyMessageBox::Show(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit,int width,bool default_button_caption)
{
	short O=26;//v�choz� offset pro zohledn�n� checkboxu
	myMessageBox->Width=width;//v�choz� ���ka MB z implicitn�ho parametru 366px, pokud nen� u�ivatelsky zadan�

	////napln�n� daty + naform�tov�n� Label_text
	Label_text->Width=myMessageBox->Width-8-8;//p�evzet� ���ky labelu dle ���ky formu - oba okraje
	if(centrovat_text)Label_text->Alignment=taCenter;else Label_text->Alignment=taLeftJustify;
	Label_text->Caption=text;
	if(text.Length()>=46)Label_text->AutoSize=true;

	//v��ka myMessageBoxu dle zadan�ho textu
	myMessageBox->ClientHeight=139-O+Label_text->Height-19;//po��t�no oproti v�choz� pozici

	//Caption napln�n� daty
	if(caption_text!="")scLabel_caption->Caption=caption_text;else scLabel_caption->Caption=Application->Title; //hl�d�n� zda nep�ijde pr�zdn� �etezec

	//checkbox
	CheckBox_pamatovat->Visible=checkbox_zobrazit;
	if(checkbox_zobrazit)myMessageBox->Height+=O;//pokud je checkbox zobrazen, je form�l�� o 10px v�t�� ne� je konec tla��tek
	CheckBox_pamatovat->Top=myMessageBox->Height-O;

	////tla��tka
	//caption tla��tka - pro p��pad, �e by v p�edchoz�m vol�n� MB bylo tla��tko p�ejmenovan�
	if(default_button_caption)
	{
		Button_OK->Caption="OK";
		Button_Cancel->Caption="Storno";
		Button_Yes->Caption="Ano";
		Button_No->Caption="Ne";
  }

	//horizont�ln� um�st�n�
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
				Button_Cancel->Left=myMessageBox->Width*2/3+myMessageBox->Width/3/2-Button_Cancel->Width/2;
			}break;
	}
	//vertik�ln� um�st�n�
	if(!checkbox_zobrazit)O=11;//11px je default odsazen� v�ech buttonu v na�em designu
	Button_OK->Top=myMessageBox->Height-Button_OK->Height-O;//um�st� vertik�ln�
	Button_Yes->Top=Button_OK->Top;
	Button_No->Top=Button_OK->Top;
	Button_Cancel->Top=Button_OK->Top;

	////pozice formul��e
	if(left<0 && top<0)//na st�ed, pokud se zad� libovoln� z�porn� ��slo
	{
		myMessageBox->Left=Form1->Left+Form1->Width/2-myMessageBox->Width/2;
		myMessageBox->Top=Form1->Top+Form1->Height/2-myMessageBox->Height/2;
	}
	else//dle zadan�ch sou�adnic
	{
		myMessageBox->Left=left;
		myMessageBox->Top=top;
		if(left<0)myMessageBox->Left=5;//je�t� o�et�en� samostatn�ch z�porn�ch sou�adnic
		if(top<0)myMessageBox->Top=5;//je�t� o�et�en� samostatn�ch z�porn�ch sou�adnic
	}

	////vol� samotn� zobrazen�
	return ShowModal();
}
//---------------------------------------------------------------------------
//K��ek
void __fastcall TmyMessageBox::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
//p�i stisku kl�vesy v MessageBoxu
void __fastcall TmyMessageBox::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch(Key)
	{
		//BACKSPACE
		case 8: break;
		//ENTER
		case 13:break;
		//ESC
		case 27:Close();break;
		//MEZERN�K
		case 32:break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TmyMessageBox::FormPaint(TObject *Sender)
{
	//zaji��uje p�ekreslov�n� or�mov�n� okolo
	if(zobrazitFrameForm)Form1->m.frameForm(myMessageBox,clWebOrange,1);
}
//---------------------------------------------------------------------------

