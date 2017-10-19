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
	myMessageBox->Color=(TColor)RGB(240,240,240);//nastavení pozadí barvy formuláøe
}
//---------------------------------------------------------------------------
void __fastcall TmyMessageBox::FormShow(TObject *Sender)
{
	//ošetøení, pokud je mimo obrazovku + 5 px okraj
	if(myMessageBox->Left>=Form1->ClientWidth-myMessageBox->Width)//nastala situace že je mimo obraz (nebo èásteènì)
	myMessageBox->Left=Form1->ClientWidth-myMessageBox->Width-5;
	if(myMessageBox->Top>=Form1->ClientHeight-myMessageBox->Height)
	myMessageBox->Top=Form1->ClientHeight-myMessageBox->Height-5;
}
//---------------------------------------------------------------------------
//pøetížená metoda
int TmyMessageBox::Show(UnicodeString text,int mbTYPE,bool centrovat_text)
{
	 return Show(-1,-1,text,"",mbTYPE,centrovat_text);
}
int TmyMessageBox::Show(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit)
{
	////naplnìní daty
	if(caption_text!="")scLabel_caption->Caption=caption_text;else scLabel_caption->Caption="TISPL"; //hlídání zda nepøijde prázdný øetezec
	//pokud text obsahuje enter, tak odøákuje
	if(text.Pos("\n"))
	{
		Label1->Caption=text.SubString(1,text.Pos("\n")-1);
		Label2->Caption=text.SubString(text.Pos("\n")+1,text.Length());
	}
	else//snaha nacpat vše do jednoho øádku pøípadnì automaticky odøákuje
	{
		Label1->Caption=text;
		Label2->Caption="";//nutné vymazání
		////pokud je text delší odøádkuje    //pokud druhý øádek je volný //pokud vùbec má smysl øetìzez zalamovat
		if(Label1->Width>=myMessageBox->Width && Label1->Caption.Length()>44)
		{
	 		 int Pos=0;int Pos_min=Pos;
	 		 AnsiString t=Label1->Caption;
	 		 while(t.Pos(" ") && Pos_min+t.Pos(" ")<44)//dìlá dokud øetìze obsahuje mezeru nebo dokud je mezera na nižším poøadí v øetìzci než 44
	 		 {
	 				Pos=t.Pos(" ");
	 				Pos_min+=Pos;
	 				t=t.SubString(Pos+1,t.Length());
	 		 }

	 		 if(Pos==0)Pos_min=44;//pokud text neobsahuje mezeru, rozdìlí umìle

	 		 //samotné rozdìlení
	 		 Label2->Caption=Label1->Caption.SubString(Pos_min+1,Label1->Caption.Length());
	 		 Label1->Caption=Label1->Caption.SubString(1,Pos_min-1);
		}
	}

	////zobrazení komponent + pøípadnì jejich pozice
	if(centrovat_text)//pokud je požadováno centrování
	{
			if(Label1->Width>Label2->Width)//podle toho, který label je delší, tak se oba vycentrují
			{
				Label1->Left=myMessageBox->Width/2-Label1->Width/2;
				Label2->Left=Label1->Left;
			}
			else
			{
				Label2->Left=myMessageBox->Width/2-Label2->Width/2;
				Label1->Left=Label2->Left;
			}
	}
	//checkbox
	CheckBox_pamatovat->Visible=checkbox_zobrazit;
	//tlaèítka
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

	////pozice formuláøe
	if(left<0 && top<0)//na støed, pokud se zadá libovolné záporné èíslo
	{
		myMessageBox->Left=Form1->ClientWidth/2-myMessageBox->Width/2;
		myMessageBox->Top=Form1->ClientHeight/2-myMessageBox->Height/2;
	}
	else//dle zadaných souøadnic
	{ //doøešit hlídání mimo monitor
		myMessageBox->Left=left;
		myMessageBox->Top=top;
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
		case 13:break;
		//ESC
		case 27:Close();break;
		//MEZERNÍK
		case 32:break;
	}
}
//---------------------------------------------------------------------------






