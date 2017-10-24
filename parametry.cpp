//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "parametry.h"
#include "unit1.h"
//#include "dopravniky.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
__fastcall TForm_parametry::TForm_parametry(TComponent* Owner)
	: TForm(Owner)
{
  //nov� nastaven� barvy formul��e
	Form_parametry->Color=(TColor)RGB(240,240,240);

	SG="PT - �ist� technologick� v�robn� �as=0\nMT - �as p�esouv�n� produktu=0\nWT - doba �ek�n� voz�ku=0\nIT - doba kontroly voz�ku=0\nQT - doba �ek�n� ve front�=0\n...=""";
	K="d�lka dopravn�ku [m]=0";
	P="WT - doba �ek�n� voz�ku=0";

	novy_parametr_n=0;
	offset=0;


}

void TForm_parametry::nacist_data(){

// Cvektory::TObjekt *ukaz=Form1->d.v.OBJEKTY->dalsi;
//
//
//				scEdit_name->Text=Form_parametry->Edit_name->Text;
//				scEdit_shortname->Text=Form_parametry->Edit_shortname->Text;
//				rEditNum_delka_dopravniku->Text=ukaz->delka_dopravniku;
//				scComboBox_pohon->ItemIndex=ukaz->pohon->n;
//				scComboBox_rezim->ItemIndex=ukaz->rezim;
//				rEditNum_kapacita->Text=ukaz->kapacita;


}

//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	minsec=MIN;//formul�� bude po zobrazen� v minut�ch
	returnOk=false;
	scGPGlyphButton_InfoIcon->Visible=false;
	rHTMLLabel_doporuc_cekani_value->Visible=false;

	scGPButton1_OK->Enabled=true;

		if(Form1->d.v.POHONY->dalsi==NULL) {

		//ShowMessage("Nejsou nastaveny pohony v parametrech linky. N�sleduj�c� formul�� nebude mo�n� ulo�it.");  //prozatimni reseni
		scGPButton1_OK->Enabled=false;
		scGPGlyphButton_InfoIcon->Visible=true;
		rHTMLLabel_doporuc_cekani_value->Visible=true;
		rHTMLLabel_doporuc_cekani_value->Caption="Nastavte pohony v parametrech linky";
		}

		if((scComboBox_rezim->ImageIndex!=1) || (scComboBox_rezim->ImageIndex!=2)){

    	rEditNum_odchylka->Enabled=false;
			rEditNum_kapacita->Enabled=false;
			
		}
      rEditNum_kapacita->Text="1";




	//Form_paremetry->Edit1->SetFocus();
	//Form_paremetry->Edit1->SelectAll();

	/*
	p�ednastaven� na Postprocesn� re��m, ji� nastavuji p�i tvorb� nov�ho objektu viz vektory.cpp
	if(Form_paremetry->Caption=="vyt�k�n� - parametry" || Form_paremetry->Caption=="su�en� - parametry" || Form_paremetry->Caption=="chlazen� - parametry")
	{ComboBox_druh_objektu->ItemIndex=2;ComboBox_druh_objektuChange(Sender);}
	*/

//ZDM
//	ComboBox_dopravnik->Items->Clear();
//	Form_dopravnik->ValueListEditor->Strings->SetText(Form1->d.v.seznam_dopravniku.c_str());
//	unsigned int n=Form_dopravnik->ValueListEditor->Strings->Count;
//	UnicodeString S=Form1->d.v.seznam_dopravniku; //Form_dopravnik->ValueListEditor->Strings->Text;
//	for(unsigned int i=0;i<n;i++)
//	{
//		ComboBox_dopravnik->Items->Add(Form1->ms.TrimRightFrom(S,"=")+" - "+Form1->ms.EP(S,"=","\n").TrimRight()+" [metr�/min]");
//		S=Form1->ms.delete_repeat(S,"\n",1);
//	}
//	ComboBox_dopravnik->ItemIndex=dopravnik_typ;
//	if(ComboBox_dopravnik->ItemIndex==-1){ComboBox_dopravnik->ItemIndex=0;dopravnik_typ=0;}//v p��pad�, �e byl dopravn�k smaz�n odk�e se na hlavn� dopravn�k
//
//	ValueListEditorStringsChange(Sender);
}
//---------------------------------------------------------------------------
void TForm_parametry::vykresli_vozik(bool na_delku)
{ /*ZDM
	Image_vozik->Canvas->Pen->Color=clBtnFace;
	Image_vozik->Canvas->Pen->Width=1;
	Image_vozik->Canvas->Brush->Color=clBtnFace;
	Image_vozik->Canvas->Rectangle(0,0,Image_vozik->Width,Image_vozik->Height);//vybarven� cel�ho
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
	//sipka -lev�
	Image_vozik->Canvas->Brush->Color=clGray;
	Image_vozik->Canvas->Pen->Color=clBlack;
	Image_vozik->Canvas->Pen->Width=1;
	POINT bodyL[3]={{stred.x-D/2*meritko-13,stred.y-3},{stred.x-D/2*meritko-7,stred.y},{stred.x-D/2*meritko-13,stred.y+3}};
	Image_vozik->Canvas->Polygon((TPoint*)bodyL,2);
	//sipka - prav�
	POINT bodyP[3]={{stred.x+D/2*meritko+7,stred.y-3},{stred.x+D/2*meritko+13,stred.y},{stred.x+D/2*meritko+7,stred.y+3}};
	Image_vozik->Canvas->Polygon((TPoint*)bodyP,2);
	//voz�k
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
void __fastcall TForm_parametry::Edit_CTKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 Form_parametry->ModalResult=idOK;//vr�t� stejnou hodnotu jako tla��tko
		 Form_parametry->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------
double TForm_parametry::get_sum()
{
	 double sum=0;
	 for(int i=1;i<=ValueListEditor->RowCount;i++)
	 {
		 try
		 {
			 sum+=ValueListEditor->Cells[1][i].ToDouble();
		 }
		 catch(...){}
	 }
	 return sum;
}
//---------------------------------------------------------------------------
void TForm_parametry::get_capacity(unsigned int input)
{
		try
		{
			switch (input)
			{
				case 1://kontinual, line tracking
				{
					if(Form1->d.v.PP.delka_voziku!=0)
					{
						double sirka_delka=Form1->d.v.PP.delka_voziku;//ZDMForm1->PP.sirka_voziku;if(RadioButton_na_delku->Checked)sirka_delka=Form1->PP.delka_voziku;//podle nastavene orientace voz�ku
						if(ValueListEditor->Cells[1][1].ToDouble()>=sirka_delka)
						{
							Label_kapacita_hodnota->Font->Color=clBlack;
							try{Edit_vzdalenost_voziku->Text.ToDouble();}catch(...){Edit_vzdalenost_voziku->Text=0;}
							Label_kapacita_hodnota->Caption=UnicodeString((ValueListEditor->Cells[1][1].ToDouble()+Edit_vzdalenost_voziku->Text.ToDouble())/(sirka_delka+Edit_vzdalenost_voziku->Text.ToDouble()));
						}
						else
						{
							Label_kapacita_hodnota->Font->Color=clRed;
							Label_kapacita_hodnota->Caption="Nepl. d�lka!";
						}
					}
					else {Label_kapacita_hodnota->Font->Color=clBlack;Label_kapacita_hodnota->Caption="Parametry voz�k!";}
				}break;

				case 2://postprocesn�
				{
					//ZDM if(Form1->PP.TT!=0)
					{
						unsigned short sm=1;if(minsec==SEC)sm=60;//pouze pro zkr�cen� zapisu
						//ZDM if(ValueListEditor->Cells[1][1].ToDouble()>=Form1->PP.TT*sm){Label_kapacita_hodnota->Font->Color=clBlack;Label_kapacita_hodnota->Caption=UnicodeString(ValueListEditor->Cells[1][1].ToDouble()/(Form1->PP.TT*sm));}
						//ZDM else	{Label_kapacita_hodnota->Font->Color=clRed;/*Label_kapacita_hodnota->Caption="Nepl. doba!";*/Label_kapacita_hodnota->Caption=UnicodeString(ValueListEditor->Cells[1][1].ToDouble()/(Form1->PP.TT*sm));}

						if(Label_kapacita_hodnota->Caption=="0")Label_delka_prepravniku_hodnota->Caption="0";
						else
						{
							//ZDM if(RadioButton_na_delku->Checked)
							 //ZDM 	Label_delka_prepravniku_hodnota->Caption=(Label_kapacita_hodnota->Caption.ToDouble()*Form1->PP.delka_voziku)+(Label_kapacita_hodnota->Caption.ToDouble()-1)*Edit_vzdalenost_voziku->Text.ToDouble();
							//ZDM else
							//ZDM 	Label_delka_prepravniku_hodnota->Caption=(Label_kapacita_hodnota->Caption.ToDouble()*Form1->PP.sirka_voziku)+(Label_kapacita_hodnota->Caption.ToDouble()-1)*Edit_vzdalenost_voziku->Text.ToDouble();
            }
					}
					//ZDM else {Label_kapacita_hodnota->Font->Color=clRed;Label_kapacita_hodnota->Caption="Zadejte TT!";}
				}break;
			}


		}catch(...){}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::ValueListEditorStringsChange(TObject *Sender)
{
	Label_vypis->Visible=false;
	unsigned short sm=1;if(minsec==SEC)sm=60;//pouze pro zkr�cen� zapisu
	switch(ComboBox_druh_objektu->ItemIndex)
	{
		case 0://stop and go
		{
			Label_CT_hodnota->Caption=get_sum();Label_kapacita_hodnota->Font->Color=clBlack;
			Label_TT_hodnota->Caption=Label_CT_hodnota->Caption;
			//ZDM if(Label_TT_hodnota->Caption.ToDouble()!=Form1->PP.TT*sm)Label_vypis->Visible=true;
		}
		break;
		case 1://kontinual
		{
			try
			{                                                                                  //zv�it zda to nebrat p��mo z form
					Label_CT_hodnota->Caption=ValueListEditor->Cells[1][1].ToDouble()/Form1->ms.EP(ComboBox_dopravnik->Items->operator[](ComboBox_dopravnik->ItemIndex),"- "," [").ToDouble();
					get_capacity(1);
					Label_TT_hodnota->Caption=Label_CT_hodnota->Caption.ToDouble()/Label_kapacita_hodnota->Caption.ToDouble();
					//ZDM if(Label_TT_hodnota->Caption.ToDouble()!=Form1->PP.TT*sm)Label_vypis->Visible=true;
			}catch(...){;};
		}
		break;
		case 2://postprocesn�
		{
			get_capacity(2);
			Label_CT_hodnota->Caption=get_sum();
			//ZDM Label_TT_hodnota->Caption=Form1->PP.TT*sm;
		}
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::ComboBox_druh_objektuChange(TObject *Sender)
{
	 setForm4Rezim(ComboBox_druh_objektu->ItemIndex);
}
//---------------------------------------------------------------------------
void TForm_parametry::setForm4Rezim(unsigned short rezim,bool default_text)
{
	switch(rezim)
 {
		 case 0://STOP & GO
		 {
			ComboBox_dopravnik->Visible=false;
		//	GroupBox_vzdalenost->Left=0;
		//	GroupBox_vzdalenost->Visible=false;
		//	GroupBox_orientace_voziku->Width=262;
			Button_DEL->Visible=false;
			Image_vozik->Width=185;if(RadioButton_na_delku->Checked)vykresli_vozik();else vykresli_vozik(false);
			if(default_text)ValueListEditor->Strings->SetText(SG.c_str());
			Label_CT->Top=296+offset;Label_CT_hodnota->Top=296+offset;
			Label_TT->Top=315+offset;Label_TT_hodnota->Top=315+offset;
			Label_kapacita->Top=334+offset;Label_kapacita_hodnota->Top=334+offset;
			Label_kapacita_hodnota->Caption=1;
			Label_delka_prepravniku->Visible=false;Label_delka_prepravniku_hodnota->Visible=false;
			Button_min_sec->Top=388+offset-6;
			Form_parametry->Height=429+offset;Button_OK->Top=374+offset;Label_vypis->Top=355+offset;
		 }break;
		 case 1://KONTINU�LN�
		 {
			ComboBox_dopravnik->Visible=true;
		//	GroupBox_vzdalenost->Visible=true;
		//	GroupBox_vzdalenost->Left=195;
		//	GroupBox_orientace_voziku->Width=187;
			Button_DEL->Visible=false;
			Image_vozik->Width=107;if(RadioButton_na_delku->Checked)vykresli_vozik();else vykresli_vozik(false);
			if(default_text)ValueListEditor->Strings->SetText(K.c_str());
			Label_CT->Top=296-19*5;Label_CT_hodnota->Top=296-19*5;
			Label_TT->Top=315-19*5;Label_TT_hodnota->Top=315-19*5;Label_TT_hodnota->Caption=0;
			Label_kapacita->Top=334-19*5;Label_kapacita_hodnota->Top=334-19*5;
			Label_delka_prepravniku->Visible=false;Label_delka_prepravniku_hodnota->Visible=false;
			Button_min_sec->Top=388-19*5-6;
			Form_parametry->Height=429-19*5;Button_OK->Top=374-19*5;Label_vypis->Top=355-19*5;
		 }break;
		 case 2://POSTPROCESN�
		 {
		//	GroupBox_vzdalenost->Visible=true;
	 //		GroupBox_vzdalenost->Left=195;
			ComboBox_dopravnik->Visible=false;
		//	GroupBox_orientace_voziku->Width=187;
			Button_DEL->Visible=false;
			Image_vozik->Width=107;if(RadioButton_na_delku->Checked)vykresli_vozik();else vykresli_vozik(false);
			if(default_text)ValueListEditor->Strings->SetText(P.c_str());
			Label_delka_prepravniku->Visible=true;Label_delka_prepravniku_hodnota->Visible=true;
			Label_delka_prepravniku->Top=296-19*5;Label_delka_prepravniku_hodnota->Top=296-19*5;
			Label_CT->Top=296-19*4;Label_CT_hodnota->Top=296-19*4;
			Label_TT->Top=315-19*4;Label_TT_hodnota->Top=315-19*4;
			Label_kapacita->Top=334-19*4;Label_kapacita_hodnota->Top=334-19*4;
			Form_parametry->Height=429-19*4;Button_OK->Top=374-19*4;
			Label_vypis->Top=355-19*4;Button_min_sec->Top=388-19*4-6;
		 }break;
 }
 ValueListEditor->Height=19*ValueListEditor->RowCount;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::ComboBox_dopravnikChange(TObject *Sender)
{
	ValueListEditorStringsChange(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender)
{
	 vykresli_vozik(true);//na d�lku
	 ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender)
{
	 vykresli_vozik(false);//na ���ku
	 ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender)
{
	 if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
	 else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::ValueListEditorClick(TObject *Sender)
{
	//po�adavek na p�id�n� �et�zce
	if(ValueListEditor->Col==0 && ValueListEditor->Row==ValueListEditor->RowCount-1)
	{
		offset+=19;

		ValueListEditor->KeyOptions>>keyUnique;
		ValueListEditor->Strings->Add("nov� parametr "+ UnicodeString(++novy_parametr_n)+"=0");
		ValueListEditor->Strings->Add("...=""");
		ValueListEditor->Strings->Delete(ValueListEditor->RowCount-4);
		ValueListEditor->KeyOptions<<keyUnique;

		setForm4Rezim(0,false);
	}

	if(ValueListEditor->Row!=ValueListEditor->RowCount-1)//pouze pokud se net�k� posledn�ho ��dku
	{
		Button_DEL->Visible=true;
		Button_DEL->Top=ValueListEditor->Top+ValueListEditor->Row*19+1;
	}

}
//---------------------------------------------------------------------------
//sma�e dan� ��dek
void __fastcall TForm_parametry::Button_DELClick(TObject *Sender)
{
	ValueListEditor->DeleteRow(ValueListEditor->Row);
	offset-=19;
	setForm4Rezim(0,false);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Edit_vzdalenost_vozikuChange(TObject *Sender)
{
	 ValueListEditorStringsChange(Sender);
}
//---------------------------------------------------------------------------
//p�ep�n�n� zobrazen� min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender)
{
 if(minsec==MIN)
 {
		minsec=SEC;
		Label_CT->Caption="CYCLE TIME [sec/voz�k] :";
		Label_CT_hodnota->Caption=Label_CT_hodnota->Caption.ToDouble()*60;
		Label_TT->Caption="TAKT TIME objektu [sec/voz�k] :";
		Label_TT_hodnota->Caption=Label_TT_hodnota->Caption.ToDouble()*60;
		ValueListEditor->TitleCaptions->Insert(1,"�as [sec/voz�k]");
		if(ComboBox_druh_objektu->ItemIndex!=1)//pokud se nejedn� o kontinu�l
		{
			for(unsigned short i=1;i<=ValueListEditor->RowCount-1;i++)
			try{ValueListEditor->Cells[1][i]=ValueListEditor->Cells[1][i].ToDouble()*60;}catch(...){;}
		}
 }
 else
 {
		minsec=MIN;
		Label_CT->Caption="CYCLE TIME [min/voz�k] :";
		Label_CT_hodnota->Caption=Label_CT_hodnota->Caption.ToDouble()/60;
		Label_TT->Caption="TAKT TIME objektu [min/voz�k] :";
		Label_TT_hodnota->Caption=Label_TT_hodnota->Caption.ToDouble()/60;
		ValueListEditor->TitleCaptions->Insert(1,"�as [min/voz�k]");
		if(ComboBox_druh_objektu->ItemIndex!=1)//pokud se nejedn� kontinu�l
		{
			for(unsigned short i=1;i<=ValueListEditor->RowCount-1;i++)
			try{ValueListEditor->Cells[1][i]=ValueListEditor->Cells[1][i].ToDouble()/60.0;}catch(...){;}
		}
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormClose(TObject *Sender, TCloseAction &Action)
{
	//v p��pad� p�epnut� na SEC zajist� p�evod zp�t na MIN
	if(minsec==SEC)Button_min_secClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Edit_nameChange(TObject *Sender)
{
	 Form_parametry->Caption=Edit_name->Text+" - parametry";
	 Edit_shortname->Text=Edit_name->Text.SubString(1,3);
}
//---------------------------------------------------------------------------
//p�i stisku Enteru do editu vzd�lenosti
void __fastcall TForm_parametry::Edit_vzdalenost_vozikuKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	 if(Key==13)
	 ComboBox_dopravnik->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scGPButton3Click(TObject *Sender)
{
	Form_parametry->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::KonecClick(TObject *Sender)
{
	returnOk=false;
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry::scGPButton1_OKClick(TObject *Sender)
{
	returnOk=true;
	Close(); // ve�ker� ukl�d�n� je �e�eno v Unit1 metoda  Nastvitparametry1Click

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
				switch(scComboBox_rezim->ItemIndex)
	{

		case 1: //kontinual
		{
      rEditNum_odchylka->Enabled=true;
			rEditNum_kapacita->Enabled=true;
		}
		break;
		case 2://postprocesni - povoleni a zakazani editboxu pro rezimy
		{
			rEditNum_odchylka->Enabled=true;
			rEditNum_kapacita->Enabled=true;
		}
		break;
		default :
		{
			rEditNum_odchylka->Enabled=false;
			rEditNum_kapacita->Enabled=false;
			rEditNum_odchylka->Text="0";
			rEditNum_kapacita->Text="1";

		}
		break;

		}
}
//---------------------------------------------------------------------------




