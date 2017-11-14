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
  //nov� nastaven� barvy formul��e
	Form_parametry->Color=(TColor)RGB(240,240,240);

	//p�evzet� defaultn� velikosti formul��e z designu pro dal�� u�it� (slou�� sp�e pro usnadn�n� v�voje, ne� funk�n�)
	defaultForm_parametryHeight=Form_parametry->Height;

	//matamaticky exaktn� napozicov�n� tla��tek OK a storno
	Form1->m.designButton(scGPButton_OK,Form_parametry,1,2);
	Form1->m.designButton(scGPButton_storno,Form_parametry,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	minsec=MIN;scGPButton_min_sec->Caption="na sec";//formul�� bude po zobrazen� v minut�ch
	input_state=NOTHING;//nutnost
	kapacitaSG=1;//nen� podn�t k rozkl�d�n� na v�ce objekt�
	scGPEdit_name->SetFocus();//nastav� v�choz� focus, kde se p�edpokl�d� v�choz� nastavov�n�
	scGPEdit_name->SelectAll();//ozna�� cele pro editace
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm_parametry::vypis(UnicodeString text,bool RED)
{
	if(text!="")//zobraz� a vyp�e
	{
		//if(!rHTMLLabel_InfoText->Visible)Form_parametry->Height+=(40+19);//pouze pokud byl popisek skryt�
		if(RED)
		{
			scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor=clRed;
			rHTMLLabel_InfoText->Font->Color=clRed;
		}
		else
		{
			scGPGlyphButton_InfoIcon->GlyphOptions->NormalColor=(TColor)RGB(0,128,255);
			rHTMLLabel_InfoText->Font->Color=(TColor)RGB(0,128,255);
		}
		scGPGlyphButton_InfoIcon->Top=Form_parametry->Height-76;
		scGPGlyphButton_InfoIcon->Visible=true;
		rHTMLLabel_InfoText->Top=Form_parametry->Height-69;
		rHTMLLabel_InfoText->Visible=true;
		rHTMLLabel_InfoText->Caption=text;
	}
	else//skryje
	{
		scGPGlyphButton_InfoIcon->Visible=false;
		rHTMLLabel_InfoText->Visible=false;
		//Form_parametry->Height-=(40+19);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
	if(input_state!=NO)//p�i startu
	{
			//nadesignov�n� a napozicov�n� dle zvolen�ho re�imu
			setForm4Rezim(scComboBox_rezim->ItemIndex);//resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy
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
//resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
	//v�choz� zmen�en� formul��e
	offset=0;
	if (Form1->STATUS==Form1->NAVRH) rezim+=10;//posunut� o 10 vytv��� re�im+navrh��
	switch(rezim)
	{
		 case 0://STOP & GO
		 {
			 set(POHON,ENABLED);
			 set(TIME,HIDE);
			 set(RYCHLOST,HIDE);
			 set(DELKA,READONLY);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,ENABLED);
			 set(CEKANI,ENABLED);
			 set(STOPKA,ENABLED);
			 scGPNumericEdit_kapacita->Value=1;
		 }break;
		 case 10://STOP & GO - N�VRH��
		 {
			 //nastaven� komponent
			 set(POHON,HIDE);
			 set(TIME,ENABLED);
			 set(RYCHLOST,HIDE);
			 set(DELKA,HIDE);
			 set(KAPACITA,READONLY);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
			 scGPNumericEdit_kapacita->Value=1;
		 }break;
		 case 1://KONTINU�LN�
		 {

		 }break;
		 case 11://KONTINU�LN� - N�VRH��
		 {
			 set(POHON,HIDE);
			 set(TIME,ENABLED);
			 set(RYCHLOST,ENABLED);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
		 }break;
		 case 2://POSTPROCESN�
		 {

		 }break;
		 case 12://POSTPROCESN� - N�VRH��
		 {
			 set(POHON,HIDE);
			 set(TIME,ENABLED);
			 set(RYCHLOST,HIDE);
			 set(DELKA,ENABLED);
			 set(KAPACITA,ENABLED);
			 set(ODCHYLKA,HIDE);
			 set(CEKANI,HIDE);
			 set(STOPKA,HIDE);
		 }break;
	}

	//VELIKOST FORMUL��E
	Form_parametry->Height=defaultForm_parametryHeight+offset;
	//vertik�ln� POZICE TLA��TEK OK A STORNO
	scGPButton_OK->Top=Form_parametry->Height-scGPButton_OK->Height-10;
	scGPButton_storno->Top=Form_parametry->Height-scGPButton_storno->Height-10;
}
//---------------------------------------------------------------------------
void TForm_parametry::set(Tcomponents C,Tcomponents_state S)
{
	//defaultn� hodnoty
	short O=40;//vertik�ln� velikost odsazen� komponent
	int L=rHTMLLabel_rezim->Top;//v�choz� komponenta
	int P=scComboBox_rezim->Top;//v�choz� komponenta
	TColor hl_color=(TColor)RGB(255,141,28);//barva zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky
	short hlFrameWidth=2;//���ka zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky

	switch (C)
	{
		case POHON:
		{
			//pozice
			rHTMLLabel_pohon->Top=L+O;
			scComboBox_pohon->Top=P+O;
			//funk�n� vlastnosti
			switch (S)
			{
				case ENABLED:	rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=true;break;
				case DISABLED:rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=false;break;
				case READONLY:rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=false;break;
				case HIDE:		rHTMLLabel_pohon->Visible=false;scComboBox_pohon->Visible=false;offset-=O;break;
			}
		}	break;
		case TIME://technologick� �as v re�imu n�vrh��
		{
		 ////pozice
			rHTMLLabel_CT->Top=L+2*O+offset;
			scGPNumericEdit_CT->Top=P+2*O+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_CT->Visible=true;scGPNumericEdit_CT->Enabled=true;scGPNumericEdit_CT->Visible=true;
			//scGPNumericEdit_CT->Options->FrameColor=clGray;
			scGPNumericEdit_CT->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*scGPNumericEdit_CT->Options->FrameColor=hl_color;*/scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_CT->Enabled=false;break;
				case READONLY:scGPNumericEdit_CT->Enabled=false;break;
				case HIDE:		rHTMLLabel_CT->Visible=false;scGPNumericEdit_CT->Visible=false;offset-=O;break;
			}
		}	break;
		case RYCHLOST://RD v re�imu n�vrh��
		{
		 ////pozice
			rHTMLLabel_RD->Top=L+3*O+offset;
			scGPNumericEdit_RD->Top=P+3*O+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPNumericEdit_RD->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_RD->Visible=true;scGPNumericEdit_RD->Visible=true;scGPNumericEdit_RD->Enabled=true;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_RD->Enabled=false;break;
				case READONLY:scGPNumericEdit_CT->Options->ShapeStyle=scgpessNone;scGPNumericEdit_RD->Enabled=false;break;
				case HIDE:		rHTMLLabel_RD->Visible=false;scGPNumericEdit_RD->Visible=false;offset-=O;break;
			}
		}	break;
		case DELKA://d�lka dopravn�ku
		{
		 ////pozice
			rHTMLLabel_delka_dopravniku->Top=L+4*O+offset;
			scGPNumericEdit_delka_dopravniku->Top=P+4*O+offset;
			//ty co jsou stejn�
			scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Enabled=true;
			/*scGPNumericEdit_delka_dopravniku->Options->FrameColor=hl_color;*/scGPNumericEdit_delka_dopravniku->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
		 ////funk�n� vlastnosti
			switch (S)
			{
				case HIGHLIGHT:/*scGPNumericEdit_delka_dopravniku->Options->FrameColor=hl_color;*/scGPNumericEdit_delka_dopravniku->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_delka_dopravniku->Enabled=false;break;
				case READONLY:scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessNone;scGPNumericEdit_delka_dopravniku->Enabled=false;break;
				case HIDE:		rHTMLLabel_delka_dopravniku->Visible=false;scGPNumericEdit_delka_dopravniku->Visible=false;offset-=O;break;
			}
		}break;
		case KAPACITA://po�adov�na vs. zji�t�n� kapacita objektu
		{
		 ////pozice
			rHTMLLabel_kapacita->Top=L+5*O+offset;
			scGPNumericEdit_kapacita->Top=P+5*O+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPNumericEdit_kapacita->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_kapacita->Visible=true;scGPNumericEdit_kapacita->Visible=true;scGPNumericEdit_kapacita->Enabled=true;
			/*scGPNumericEdit_kapacita->Options->FrameColor=hl_color;*/scGPNumericEdit_kapacita->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*scGPNumericEdit_kapacita->Options->FrameColor=hl_color;*/scGPNumericEdit_kapacita->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_kapacita->Enabled=false;break;
				case READONLY:scGPNumericEdit_kapacita->Options->ShapeStyle=scgpessNone;scGPNumericEdit_kapacita->Enabled=false;break;
				case HIDE:		rHTMLLabel_kapacita->Visible=false;scGPNumericEdit_kapacita->Visible=false;offset-=O;break;
			}
		}	break;
		case ODCHYLKA://povolen� odchylka z CT
		{
		 ////pozice
			rHTMLLabel_odchylka->Top=L+6*O+offset;
			scGPNumericEdit_odchylka->Top=P+6*O+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPNumericEdit_odchylka->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_odchylka->Visible=true;scGPNumericEdit_odchylka->Visible=true;scGPNumericEdit_odchylka->Enabled=true;
			/*scGPNumericEdit_odchylka->Options->FrameColor=hl_color;*/scGPNumericEdit_odchylka->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*scGPNumericEdit_odchylka->Options->FrameColor=hl_color;*/scGPNumericEdit_odchylka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_odchylka->Enabled=false;break;
				case READONLY:scGPNumericEdit_odchylka->Options->ShapeStyle=scgpessNone;scGPNumericEdit_odchylka->Visible=false;break;
				case HIDE:		rHTMLLabel_odchylka->Visible=false;scGPNumericEdit_odchylka->Visible=false;offset-=O;break;
			}
		}	break;
		case CEKANI://�ek�n�
		{
		 ////pozice
			rHTMLLabel_cekani->Top=L+7*O+offset;
			scComboBox_cekani_palec->Top=P+7*O+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			rHTMLLabel_cekani->Visible=true;scComboBox_cekani_palec->Visible=true;scComboBox_cekani_palec->Enabled=true;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*scComboBox_cekani_palec->Options->FrameColor=hl_color;*/scComboBox_cekani_palec->Options->FrameWidth=hlFrameWidth;break;
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
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			rHTMLLabel_stopka->Visible=true;scComboBox_stopka->Visible=true;scComboBox_stopka->Enabled=true;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*scComboBox_stopka->Options->FrameColor=hl_color;*/scComboBox_stopka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_stopka->Enabled=false;break;
				case READONLY:scComboBox_stopka->Enabled=false;break;
				case HIDE:		rHTMLLabel_stopka->Visible=false;scComboBox_stopka->Visible=false;offset-=O;break;
			}
		}	break;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�i psan� n�zvu objektu se m�n� titulek a generuje zkratka
void __fastcall TForm_parametry::scGPEdit_nameChange(TObject *Sender)
{
	 Form_parametry->scLabel_titulek->Caption=scGPEdit_name->Text+" - parametry";
	 scGPEdit_shortname->Text=scGPEdit_name->Text.SubString(1,4).UpperCase();
}
//---------------------------------------------------------------------------
//p�i zm�n�ch EDIT�
void __fastcall TForm_parametry::scGPNumericEdit_CTChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud nen� zad�v�no z jin�ho vstupu
	input_CT();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud nen� zad�v�no z jin�ho vstupu
	input_DD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny d�lky dopravn�ku
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_RDChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud nen� zad�v�no z jin�ho vstupu
	input_RD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_kapacitaChange(TObject *Sender)
{
	if(input_state==NOTHING)//pokud nen� zad�v�no z jin�ho vstupu
	input_K();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
void TForm_parametry::input_CT()
{
	 input_state=CT;
	 double CT=scGPNumericEdit_CT->Value;//CT od u�ivatele
	 if(minsec==SEC)CT=CT/60.0;//pokud bylo zad�n� v sekund�ch p�evede na minuty
	 if(CT>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
    	 //default hodnoty
    	 double TT=Form1->d.v.ZAKAZKY->dalsi->TT;//TT defaultn� zak�zky
    	 double dV=Form1->d.v.PP.delka_voziku; //delka voziku

    	 double m=0;//mezera mezi voziky
    	 short p=1;//ode�ten� do spr�vn�ho po�tu mezer
			 //default nastaven� komponent
			 scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
    	 scGPButton_OK->Caption="Ulo�it";

    	 //dle �e�imu objektu
			 if(scComboBox_rezim->ItemIndex==0)//S&G
			 {
    		 //o�et�en� a p��padn� n�sledn� �e�en� situac�, kdy nen� toto�n� procesn� �as a TT
    		 if(CT<TT)
    		 {
    			scGPButton_OK->Enabled=false;
    		 	vypis("Pozor, procesn� �as je ni��� ne� hodnota TT!");
    		 }
    		 if(CT>TT)
				 {
    		 		if(fmod(CT,TT)==0)
    				{
    		 			kapacitaSG=CT/TT;//pro dal�� pou�it�
    					vypis("Rozlo�it na "+AnsiString(kapacitaSG)+"x "+scGPEdit_name->Text.UpperCase()+"?");
    		 			scGPButton_OK->Enabled=true;
    					scGPButton_OK->Caption="Ano a ulo�it";
    		 		}
    				else
    		 		{
    					scGPButton_OK->Enabled=false;
    		 			vypis("Zm�nte re�im nebo rozlo�te do v�ce objekt�!");
    		 		}
    		 }
    	 }
    	 else//KONTINUAL+PP
			 {
    		 //KAPACITA
    		 double K=CT/TT;//v�po�et
    		 scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
    		 scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
    		 //pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
				 if(Form1->ms.get_count_decimal(K)>0)
				 {
					if(minsec==MIN)vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT)+" min.");
					else vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT*60)+" s.");
				 }
				 //D�LKA DOPRAVN�KU
				 double DD=K*dV+(K-p)*m;//o�et�eno i pro stav kdy je stejn� po�et mezer jako voz�ku
				 scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
				 scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka
    		 //RYCHLOST DOPRAVN�KU (�ist� jen pro KONTINUAL)
				 if(scComboBox_rezim->ItemIndex==1)
				 {
					double RD=DD/CT;
					if(minsec==SEC)RD/=60.0;
					scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_RD->Value=RD;//pln�n� pat�i�n�ho pol��ka
    		 }
			 }
	 }
	 else
	 null_input_value();
	 input_state=NOTHING;
}
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny DD
void TForm_parametry::input_DD()
{
	input_state=DD;
	//default hodnoty
	double DD=scGPNumericEdit_delka_dopravniku->Value;
	if(DD>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	{
		double TT=Form1->d.v.ZAKAZKY->dalsi->TT;//TT defaultn� zak�zky
		double dV=Form1->d.v.PP.delka_voziku; //delka voziku
		double m=0;//mezera mezi voziky
		short p=1;//ode�ten� do spr�vn�ho po�tu mezer
		//default nastaven� komponent
		scGPNumericEdit_kapacita->Decimal=0;
		vypis("");
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption="Ulo�it";

		//KAPACITA                 //pokud je stejn� po�et mezer jako voz�k�
		double K=(DD+m)/(dV+m);if(p==1)K=DD/(dV+m);
		scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
		//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
		if(Form1->ms.get_count_decimal(K)>0)
		vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT)+" min.");

		//PROCESN� �AS resp. CT
		double CT = TT*K;
		if(minsec==SEC)CT*=60.0;
		scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka

		//RYCHLOST DOPRAVN�KU (�ist� jen pro KONTINUAL)
		if(scComboBox_rezim->ItemIndex==1)
		{
			double RD = DD/CT;
			if(minsec==SEC)RD/=60.0;
			scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
			scGPNumericEdit_RD->Value=RD;//pln�n� pat�i�n�ho pol��ka
		}
	}
	else
	null_input_value();
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD, pouze pro kontinual
void TForm_parametry::input_RD()
{
	 input_state=RD;
	 double RD=scGPNumericEdit_RD->Value;
	 if(RD>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
    	 //default hodnoty
    	 double TT=Form1->d.v.ZAKAZKY->dalsi->TT;//TT defaultn� zak�zky
    	 double dV=Form1->d.v.PP.delka_voziku; //delka voziku
    	 double m=0;//mezera mezi voziky
    	 short p=1;//ode�ten� do spr�vn�ho po�tu mezer
    	 //default nastaven� komponent
			 scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
    	 scGPButton_OK->Caption="Ulo�it";

    	 double K=0;
    	 if(m>0 && p==1)//pokud je rozd�ln� po�et mezer jako voz�k� a mezera je nenulova, lze p��mo z RD vypo��tat ostatn� hodnoty
    	 {
    			//KAPACITA
    			K=m/(dV+m-RD*TT);
    			scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
    			scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
    			//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
    			if(Form1->ms.get_count_decimal(K)>0)
    			vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT)+" min.");

    			//D�LKA DOPRAVN�KU
    			double DD = K*dV+(K-p)*m;
    			scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
    			scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka

    			//PROCESN� �AS resp. CT
					double CT = RD/DD;
					if(minsec==SEC)CT*=60.0;
    			scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
    			scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
    	 }
    	 else //pokud ne, je nutn� z�skat dal�� nenulov� vstup
			 {
    			double DD=scGPNumericEdit_delka_dopravniku->Value;
					double CT=scGPNumericEdit_CT->Value;
    			double K=scGPNumericEdit_kapacita->Value;

    			if(DD!=0)//pokud je zn�m� d�lka dopravn�ku
    			{
    				//KAPACITA
    				K=RD/DD/TT;
    				scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
    				scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
    				//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
    				if(Form1->ms.get_count_decimal(K)>0)
    				vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT)+" min.");

    				//PROCESN� �AS resp. CT
						double CT = RD/DD;
						if(minsec==SEC)CT*=60.0;
    				scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
    				scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
    			}
    			else
    			{
    				if(CT!=0)//pokud nen� zn�m� d�lka ale je zn�me CT
    				{
        			//D�LKA DOPRAVN�KU
        			DD=RD*CT;
    					scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
    					scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka

        			//KAPACITA
        			K=CT/TT;
        			scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
        			scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
    					//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
    					if(Form1->ms.get_count_decimal(K)>0)
    					vypis("Doporu�en� procesn� �as je: "+AnsiString(Form1->m.round(K)*TT)+" min.");
    				}
    				else
    				{
    					if(K!=0)//pokud nen� zn�m� d�lka ani CT a je zn�m� K
    					{
    						//D�LKA DOPRAVN�KU
    						DD=RD*TT*K;
    						scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
    						scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka

    						//PROCESN� �AS resp. CT
								double CT = RD/DD;
								if(minsec==SEC)CT*=60.0;
    						scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
    						scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
    					}
    					else//pokud krom� RD nen� nic zadan� a je po�adovan� dal�� hodnota
							{
								vypis("Zadejte libovolnou dal�� hodnotu!");
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
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
void TForm_parametry::input_K()
{
	 input_state=C;
	 double K=scGPNumericEdit_kapacita->Value;//z�sk�n� kapacity od u�ivatele
	 if(K>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
    	 //default hodnoty
    	 double TT=Form1->d.v.ZAKAZKY->dalsi->TT;//TT defaultn� zak�zky
    	 double dV=Form1->d.v.PP.delka_voziku; //delka voziku
    	 double m=0;//velikost mezera mezi voziky
    	 short p=1;//ode�ten� do spr�vn�ho po�tu mezer
    	 //default nastaven� komponent
    	 scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
    	 scGPButton_OK->Caption="Ulo�it";

    	 //PROCESN� �AS resp. CT
    	 double CT = TT*K;
    	 scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
    	 scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka

    	 //D�LKA DOPRAVN�KU
    	 double DD = K*dV+(K-p)*m;//o�et�eno i pro stav stejn�ho po�tu voz�ku a mezer;
    	 scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
    	 scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka

    	 //RYCHLOST DOPRAVN�KU (�ist� jen pro KONTINUAL)
    	 if(scComboBox_rezim->ItemIndex==1)
			 {
    			double RD=DD/CT;
					scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_RD->Value=RD;//pln�n� pat�i�n�ho pol��ka
			 }
	 }
	 else
	 null_input_value();
	 input_state=NOTHING;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vynuluje vstupn� hodnoty
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
void __fastcall TForm_parametry::RadioButton_na_delkuClick(TObject *Sender)
{
//	 vykresli_vozik(true);//na d�lku
//	 ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry::RadioButton_na_sirkuClick(TObject *Sender)
{
//	 vykresli_vozik(false);//na ���ku
//	 ValueListEditorStringsChange(Sender);//zajist� p�epo��t�n� hodnot
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::Image_vozikClick(TObject *Sender)
{
//	 if(RadioButton_na_delku->Checked){RadioButton_na_sirku->Checked=true;RadioButton_na_sirkuClick(Sender);}
//	 else {RadioButton_na_delkuClick(Sender);RadioButton_na_delku->Checked=true;}
}
//---------------------------------------------------------------------------
//p�ep�n�n� zobrazen� min vs. sec
void __fastcall TForm_parametry::Button_min_secClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double RD=0;double CT=0;
	if(scGPButton_min_sec->Caption=="na sec")//p�epne na sekundy
	{
		minsec=SEC;scGPButton_min_sec->Caption="na min";//samotn� tla��tko
		//RD
		RD=scGPNumericEdit_RD->Value/60.0;
		rHTMLLabel_RD->Caption="Rychlost dopravn�ku [m/s]";
		//CT
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologick� �as [s]";
	}
	else//p�epne na minuty
	{
		minsec=MIN;scGPButton_min_sec->Caption="na sec";//samotn� tla��tko
		//RD
		RD=scGPNumericEdit_RD->Value*60.0;
		rHTMLLabel_RD->Caption="Rychlost dopravn�ku [m/min]";
		//CT
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologick� �as [min]";
	}
	//pln�n� + po�et desetinn�ch m�st
	scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_CT->Value=CT;
	scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�i stisku kl�vesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 if(Key==13)//ENTER
 {
		 Form_parametry->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
		 Form_parametry->VisibleChanging();//skryje form, stejn� jako visible=false
 }
 if(Key==27)//ESC
 {
		 Form_parametry->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_parametry->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�i stisku storna
void __fastcall TForm_parametry::scGPButton_stornoClick(TObject *Sender)
{
	kapacitaSG=1;//nastav� na default
}
//---------------------------------------------------------------------------
//p�i stisku storna
void __fastcall TForm_parametry::KonecClick(TObject *Sender)
{
	kapacitaSG=1;//nastav� na default
}
//---------------------------------------------------------------------------







