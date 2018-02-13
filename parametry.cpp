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

	//nastaven� zv�razn�n�
	hl_color=clRed;//(TColor)RGB(255,141,28);//barva zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky
	hlFrameWidth=2;//���ka zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky

	//p�evzet� defaultn� velikosti formul��e z designu pro dal�� u�it� (slou�� sp�e pro usnadn�n� v�voje, ne� funk�n�)
	defaultForm_parametryHeight=Form_parametry->Height;

	//matamaticky exaktn� napozicov�n� tla��tek OK a storno
	Form1->m.designButton(scGPButton_OK,Form_parametry,1,2);
	Form1->m.designButton(scGPButton_storno,Form_parametry,2,2);

	//v�choz� nastaven� z�mk�
	CT_zamek=UNLOCKED;
	RD_zamek=LOCKED;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::FormShow(TObject *Sender)
{
	//formul�� bude p�i prvn�m zobrazen� v sekund�ch a metrech nebo dle INI v odvozen�ch jednotk�ch, jinak dle SI
	minsec=S;CTunit=S;RDunitT=S;
	m_mm=M;DDunit=M;DMunit=M;RDunitD=M;
	//GLOBAL
	if(Form1->readINI("nastaveni_form_parametry","cas")=="1"){minsec=MIN;scGPButton_min_sec->Caption="v�e na s";}//tedy MIN
	if(Form1->readINI("nastaveni_form_parametry","vzdalenost")=="1"){m_mm=MM;scGPButton_metry_milimetry->Caption="v�e na m";}//tedy MM
	//CT
	if(Form1->readINI("nastaveni_form_parametry","CT")=="1") rHTMLLabel_CTClick(this);//p�evede na min tzn. CTunit=MIN;
	//RD
	if(Form1->readINI("nastaveni_form_parametry","RDt")=="1")	rHTMLLabel_RDClick(this);//p�evede na min tzn. RDunitT=MIN;
	if(Form1->readINI("nastaveni_form_parametry","RDd")=="1")	rHTMLLabel_RDClick(this);//p�evede na mm tzn. RDunitD=MM;
	//DD
	if(Form1->readINI("nastaveni_form_parametry","DD")=="1")	rHTMLLabel_delka_dopravnikuClick(this);//p�evede na mm tzn. DDunit=MM;
	//DM
	if(Form1->readINI("nastaveni_form_parametry","DM")=="1")	rHTMLLabel_mezeraClick(this);//p�evede na mm tzn. DMunit=MM;

	//nastaven� defaultn�ch hodnot
	if(scGPNumericEdit_CT->Value==0)//if(d.v.ZAKAZKY->dalsi!=NULL)//pokud existuje prvn� zak�zka
	{
		scGPNumericEdit_CT->Value=Form1->d.v.PP.TT/(1+59*CTunit);//ji� nepou��v�me, proto�e se u prvn� zak�zky uva�uje glob�ln� TT,d.v.ZAKAZKY->dalsi->TT;
		scGPNumericEdit_kapacita->Value=1;
	}

	input_state=NOTHING;//nutnost!!!
	kapacitaSG=1;//nen� podn�t k rozkl�d�n� na v�ce objekt�
	scGPEdit_name->SetFocus();//nastav� v�choz� focus, kde se p�edpokl�d� v�choz� nastavov�n�
	scGPEdit_name->SelectAll();//ozna�� cele pro editace
	form_zobrazen=true;//detekuje zda je form aktu�ln� zobrazen, slou�� proto aby p�i zm�n� combo re�im pokud si nastavil u�ivatel formula� jinam, aby z�stal nastaven dle u�ivatele
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
      //zv�razn�n� polo�ky technologick� �as
			if(text.Pos("technologick� �as") && scGPNumericEdit_CT->Enabled)
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
		if(text.Length()<=35)//v p��pad�, �e je text del�� ne� 35 znak� skryje ikonu u zvol� nov� lev� odsazen� textu
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
		//zv�razn�n� polo�ky technologick� �as
		if(scGPNumericEdit_CT->Enabled){/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;*/}
		//Form_parametry->Height-=(40+19);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scComboBox_rezimChange(TObject *Sender)
{
	if(input_state!=NO)//pokud to nen� p�i startu (formshow)
	{
			//nadesignov�n� a napozicov�n� komponent dle zvolen�ho re�imu
			setForm4Rezim(scComboBox_rezim->ItemIndex);//resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy

			//napozicov�n� cel�ho formul��e resp. o�et�en� aby z�stal dialog na monitoru, pouze pro prvotn� zobrazen� dle sou�adnic kurzoru my�i, jinak dle u�ivatele
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
//resize a napozicov�n� formul��e+povoleni a zakazani komponent pro jednotliv� re�imy
void TForm_parametry::setForm4Rezim(unsigned short rezim)
{
	//v�choz� zmen�en� formul��e
	offset=0;
	if(Form1->STATUS==Form1->NAVRH) rezim+=10;//posunut� o 10 vytv��� re�im+navrh��
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
		 case 10://STOP & GO - N�VRH��
		 {
			 //nastaven� komponent
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
		 case 1://KONTINU�LN�
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
		 case 11://KONTINU�LN� - N�VRH��
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
		 case 2://POSTPROCESN�
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
		 case 12://POSTPROCESN� - N�VRH��
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
			case 13://SG - N�VRH��  - ROZ���EN�
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

	//VELIKOST FORMUL��E
	Form_parametry->Height=defaultForm_parametryHeight+offset;
	//vertik�ln� POZICE TLA��TEK OK,STORNO,COPY,PASTE
	scGPButton_OK->Top=Form_parametry->Height-scGPButton_OK->Height-10;
	scGPButton_storno->Top=Form_parametry->Height-scGPButton_storno->Height-10;
	scGPGlyphButton_paste->Top=Form_parametry->Height-scGPGlyphButton_paste->Height;
	scGPGlyphButton_copy->Top=scGPGlyphButton_paste->Top-scGPGlyphButton_copy->Height;
}
//---------------------------------------------------------------------------
//zaji��uje zobrazen� a napozicov�n� pat�i�n� konkr�tn� komponenty a z�rove� udr�ov�n� hodnoty offsetu - to pokud je move==true, jinak jen nastav� komponenty
void TForm_parametry::set(Tcomponents C,Tcomponents_state S,bool move)
{
	//defaultn� hodnoty
	short O=40;//vertik�ln� velikost odsazen� komponent
	int L=rHTMLLabel_rezim->Top;//v�choz� komponenta
	int P=scComboBox_rezim->Top;//v�choz� komponenta

	switch (C)
	{
		case POHON:
		{
		 ////o�et�en� pokud nen� nadefinov�n ��dn� pohon
			if(!existuje_pohon)S=READONLY;
		 ////pozice
			if(move)
			{
				rHTMLLabel_pohon->Top=L+O;
				scComboBox_pohon->Top=P+O;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
		 /*ROSTA-styl//	scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect; */
			rHTMLLabel_pohon->Visible=true;scComboBox_pohon->Visible=true;scComboBox_pohon->Enabled=true;
			scComboBox_pohon->Options->FrameNormalColor=clGray;scComboBox_pohon->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scComboBox_pohon->Options->FrameNormalColor=hl_color;scComboBox_pohon->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_pohon->Enabled=false;break;
				case READONLY:scComboBox_pohon->Enabled=false;scComboBox_pohon->Options->ShapeStyle=scgpessNone;break;
				case HIDE:		rHTMLLabel_pohon->Visible=false;scComboBox_pohon->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case TIME://technologick� �as v re�imu n�vrh��
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_CT->Top=L+2*O+offset;
				scGPNumericEdit_CT->Top=P+2*O+offset;
				scButton_zamek_CT->Top=scGPNumericEdit_CT->Top;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
		 /*ROSTA-styl//	scGPNumericEdit_CT->Options->ShapeStyle=scgpessRect;*/
			rHTMLLabel_CT->Visible=true;scGPNumericEdit_CT->Enabled=true;scGPNumericEdit_CT->Visible=true;
			/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=clGray;scGPNumericEdit_CT->Options->FrameWidth=1;*/
			if(scComboBox_rezim->ItemIndex==1)scButton_zamek_CT->Visible=true;else scButton_zamek_CT->Visible=false;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*ROSTA-styl//scGPNumericEdit_CT->Options->FrameNormalColor=hl_color;scGPNumericEdit_CT->Options->FrameWidth=hlFrameWidth;*/break;
				case ENABLED:	/*scButton_zamek_CT->ImageIndex=38;CT_zamek=UNLOCKED;*/break;
				case DISABLED:scGPNumericEdit_CT->Enabled=false;/*scButton_zamek_CT->ImageIndex=37;CT_zamek=LOCKED;*/break;
				case READONLY:/*ROSTA-styl//scGPNumericEdit_CT->Options->ShapeStyle=scgpessNone;*/scButton_zamek_CT->Visible=false;scGPNumericEdit_CT->Enabled=false;break;
				case HIDE:		rHTMLLabel_CT->Visible=false;scGPNumericEdit_CT->Visible=false;scButton_zamek_CT->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case RYCHLOST://RD v re�imu n�vrh��
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_RD->Top=L+3*O+offset;
				scGPNumericEdit_RD->Top=P+3*O+offset;
				scButton_zamek_RD->Top=scGPNumericEdit_RD->Top;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			/*ROSTA-styl//scGPNumericEdit_RD->Options->ShapeStyle=scgpessRect;*/
			rHTMLLabel_RD->Visible=true;scGPNumericEdit_RD->Visible=true;scGPNumericEdit_RD->Enabled=true;
			/*ROSTA-styl//scGPNumericEdit_RD->Options->FrameNormalColor=clGray;scGPNumericEdit_RD->Options->FrameWidth=1;*/
			if(scComboBox_rezim->ItemIndex==1)scButton_zamek_RD->Visible=true;else scButton_zamek_RD->Visible=false;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:/*ROSTA-styl//scGPNumericEdit_RD->Options->FrameNormalColor=hl_color;scGPNumericEdit_RD->Options->FrameWidth=hlFrameWidth;*/
				case ENABLED:	/*scButton_zamek_RD->ImageIndex=38;RD_zamek=UNLOCKED;*/break;
				case DISABLED:/*scGPNumericEdit_RD->Enabled=false;scButton_zamek_RD->ImageIndex=37;RD_zamek=UNLOCKED;*/break;
				case READONLY:/*ROSTA-styl//scGPNumericEdit_RD->Options->ShapeStyle=scgpessNone;*/scGPNumericEdit_RD->Enabled=false;scButton_zamek_RD->Visible=false;break;
				case HIDE:		rHTMLLabel_RD->Visible=false;scGPNumericEdit_RD->Visible=false;scButton_zamek_RD->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case DELKA://d�lka dopravn�ku
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_delka_dopravniku->Top=L+4*O+offset;
				scGPNumericEdit_delka_dopravniku->Top=P+4*O+offset;
				scButton_zamek_DD->Top=scGPNumericEdit_delka_dopravniku->Top;
			}
			//ty co jsou stejn�
			scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Visible=true;scGPNumericEdit_delka_dopravniku->Enabled=true;
			scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=clGray;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=1;
			//scButton_zamek_DD->Visible=true;
			//ty co jsou rozd�ln�
		 ////funk�n� vlastnosti
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=hl_color;scGPNumericEdit_delka_dopravniku->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	scButton_zamek_DD->ImageIndex=38;DD_zamek=UNLOCKED;break;
				case DISABLED:scGPNumericEdit_delka_dopravniku->Enabled=false;scButton_zamek_DD->ImageIndex=37;DD_zamek=LOCKED;break;
				case READONLY:scGPNumericEdit_delka_dopravniku->Options->ShapeStyle=scgpessNone;scGPNumericEdit_delka_dopravniku->Enabled=false;scButton_zamek_DD->Visible=false;break;
				case HIDE:		rHTMLLabel_delka_dopravniku->Visible=false;scGPNumericEdit_delka_dopravniku->Visible=false;scButton_zamek_DD->Visible=false;if(move)offset-=O;break;
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
			scGPNumericEdit_kapacita->Options->FrameNormalColor=clGray;scGPNumericEdit_kapacita->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_kapacita->Options->FrameNormalColor=hl_color;scGPNumericEdit_kapacita->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_kapacita->Enabled=false;break;
				case READONLY:scGPNumericEdit_kapacita->Options->ShapeStyle=scgpessNone;scGPNumericEdit_kapacita->Enabled=false;break;
				case HIDE:		rHTMLLabel_kapacita->Visible=false;scGPNumericEdit_kapacita->Visible=false;if(move)offset-=O;break;
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
			scGPNumericEdit_odchylka->Options->FrameNormalColor=clGray;scGPNumericEdit_odchylka->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_odchylka->Options->FrameNormalColor=hl_color;scGPNumericEdit_odchylka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_odchylka->Enabled=false;break;
				case READONLY:scGPNumericEdit_odchylka->Options->ShapeStyle=scgpessNone;scGPNumericEdit_odchylka->Visible=false;break;
				case HIDE:		rHTMLLabel_odchylka->Visible=false;scGPNumericEdit_odchylka->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case CEKANI://�ek�n�
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_cekani->Top=L+7*O+offset;
				scComboBox_cekani_palec->Top=P+7*O+offset;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scComboBox_cekani_palec->Options->FrameNormalColor=clGray;scComboBox_cekani_palec->Options->FrameWidth=1;
			rHTMLLabel_cekani->Visible=true;scComboBox_cekani_palec->Visible=true;scComboBox_cekani_palec->Enabled=true;
			//ty co jsou rozd�ln�
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
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			rHTMLLabel_stopka->Visible=true;scComboBox_stopka->Visible=true;scComboBox_stopka->Enabled=true;
			scComboBox_stopka->Options->FrameNormalColor=clGray;scComboBox_stopka->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scComboBox_stopka->Options->FrameNormalColor=hl_color;scComboBox_stopka->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_stopka->Enabled=false;break;
				case READONLY:scComboBox_stopka->Enabled=false;break;
				case HIDE:		rHTMLLabel_stopka->Visible=false;scComboBox_stopka->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case ROTACE://rotace jigu v objektu, zat�m jen p�ep�n�tko 0-90
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_rotace->Top=L+9*O+offset;
				scComboBox_rotace->Top=P+9*O+offset;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			rHTMLLabel_rotace->Visible=true;scComboBox_rotace->Visible=true;scComboBox_rotace->Enabled=true;
			scComboBox_rotace->Options->FrameNormalColor=clGray;scComboBox_rotace->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scComboBox_rotace->Options->FrameNormalColor=hl_color;scComboBox_rotace->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scComboBox_rotace->Enabled=false;break;
				case READONLY:scComboBox_rotace->Enabled=false;break;
				case HIDE:		rHTMLLabel_rotace->Visible=false;scComboBox_rotace->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case MEZERA://po�adov�na vs. zji�t�n� kapacita objektu
		{
		 ////pozice
			if(move)
			{
				rHTMLLabel_mezera->Top=L+10*O+offset;
				scGPNumericEdit_mezera->Top=P+10*O+offset;
			}
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPNumericEdit_mezera->Options->ShapeStyle=scgpessRect;
			rHTMLLabel_mezera->Visible=true;scGPNumericEdit_mezera->Visible=true;scGPNumericEdit_mezera->Enabled=true;
			scGPNumericEdit_mezera->Options->FrameNormalColor=clGray;scGPNumericEdit_mezera->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
			switch (S)
			{
				case HIGHLIGHT:scGPNumericEdit_mezera->Options->FrameNormalColor=hl_color;scGPNumericEdit_mezera->Options->FrameWidth=hlFrameWidth;break;
				case ENABLED:	break;
				case DISABLED:scGPNumericEdit_mezera->Enabled=false;break;
				case READONLY:scGPNumericEdit_mezera->Options->ShapeStyle=scgpessNone;scGPNumericEdit_mezera->Enabled=false;break;
				case HIDE:		rHTMLLabel_mezera->Visible=false;scGPNumericEdit_mezera->Visible=false;if(move)offset-=O;break;
			}
		}	break;
		case POCET_MEZER://po�adov�na vs. zji�t�n� kapacita objektu
		{
		 ////pozice
			if(move)scGPCheckBox_pocet_mezer->Top=L+11*O-5+offset;
		 ////funk�n� vlastnosti
			//ty co jsou stejn�
			scGPCheckBox_pocet_mezer->Visible=true;scGPCheckBox_pocet_mezer->Enabled=true;
			//scGPCheckBox_pocet_mezer->Options->FrameNormalColor=clGray;scGPCheckBox_pocet_mezer->Options->FrameWidth=1;
			//ty co jsou rozd�ln�
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
	if(input_state==NOTHING && input_clicked_edit==CT_klik)//pokud nen� zad�v�no z jin�ho vstupu
	input_CT();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPNumericEdit_delka_dopravnikuChange(TObject *Sender)
{
	if(input_state==NOTHING && input_clicked_edit==DD_klik)//pokud nen� zad�v�no z jin�ho vstupu
	input_DD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny d�lky dopravn�ku
	//hl�d�n� velikosti mezery dle rozte�e
	if(scComboBox_rezim->ItemIndex!=0 && scGPNumericEdit_mezera->Value>0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{                            //je "zbytek po d�len�"
			if(P->roztec>0 && !Form1->m.cele_cislo(scGPNumericEdit_mezera->Value/P->roztec))//nespl�uje rozmez�
			{
				vypis("Doporu�eno: "+AnsiString(Form1->m.round(scGPNumericEdit_mezera->Value/P->roztec)*P->roztec)+" m",true);
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
	if(input_state==NOTHING && input_clicked_edit==RD_klik)//pokud nen� zad�v�no z jin�ho vstupu
	input_RD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD
	//kontrola rozsahu zadan�ho pohonu
	if(scComboBox_rezim->ItemIndex!=0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{
			if(scGPNumericEdit_RD->Value<P->rychlost_od || P->rychlost_do<scGPNumericEdit_RD->Value)//nespl�uje rozmez�
			{
				vypis("Rychlost neodpov�d� rozmez�!",true);
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
	if(input_state==NOTHING && input_clicked_edit==C_klik)//pokud nen� zad�v�no z jin�ho vstupu

	input_K();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny K
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry::scGPCheckBox_pocet_mezerClick(TObject *Sender)
{
 if(input_state==NOTHING && input_clicked_edit==mezera_klik)
 input_DD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny d�lky dopravn�ku
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
void TForm_parametry::input_CT()
{
	 input_state=CT;
	 double CT=scGPNumericEdit_CT->Value;//CT od u�ivatele
	 if(CTunit==MIN)CT=CT*60.0;//pokud bylo zad�n� v minut�ch p�evede na sekundy
	 if(CT>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
			 //default nastaven� komponent
			 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
			 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Ulo�it";

			 //volba ���ka �i d�lka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je po�adov�n ���ka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//po�et mezer mezi voz�ky

			 //dle �e�imu objektu
			 if(scComboBox_rezim->ItemIndex==0)//S&G
			 {
				 //o�et�en� a p��padn� n�sledn� �e�en� situac�, kdy nen� toto�n� procesn� �as a TT
				 if(CT<Form1->d.v.PP.TT)
    		 {
    			scGPButton_OK->Enabled=false;
					vypis("Technologick� �as je ni��� ne� TT!");
				 }
				 if(CT>Form1->d.v.PP.TT)
				 {
						if(fmod(CT,Form1->d.v.PP.TT)==0)
    				{
							kapacitaSG=CT/Form1->d.v.PP.TT;//pro dal�� pou�it�
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
				 else kapacitaSG=1;
    	 }
    	 else//KONTINUAL+PP
			 {
				 //KAPACITA
				 double K=CT/Form1->d.v.PP.TT;//v�po�et
				 //ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
				 scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
				 scGPNumericEdit_kapacita->Font->Color=clBlack;
				 //pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
				 if(Form1->ms.get_count_decimal(K)>0)
				 {
					scGPNumericEdit_kapacita->Font->Color=clRed;//zv�razn�n� neplatn� vypl�vaj�c� polo�ky
					if(Form1->m.round(K)==0)K=1;//o�et�en� pokud by vych�zela kapacita 0,n�co bylo by zaokrouhleno na 0 a tud� by se vypisoval doporu�en� technologick� �as 0
					if(CTunit==MIN)vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
					else vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
           scGPButton_OK->Enabled=true;
					//ROSTA-puvodni//scGPButton_OK->Enabled=false;//zak�e ukl�dac� tla��tko
				 }
				 //D�LKA DOPRAVN�KU
				 double DD=K*dV+(K-p)*m;//o�et�eno i pro stav kdy je stejn� po�et mezer jako voz�ku
				 if(DDunit==MM)DD*=1000.0;
				 //ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
				 scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka
				 if(DDunit==MM)DD/=1000.0;//vracen� do p�vodn�ho stavu k dal��mu pou�it�
				 //RYCHLOST DOPRAVN�KU (�ist� jen pro KONTINUAL)
				 if(scComboBox_rezim->ItemIndex==1)
				 {
					double RD=DD/CT;
					if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
					//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
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
 	input_state=DD;//pozor my�leno DD - z ENUM
	//default hodnoty
	double DD=scGPNumericEdit_delka_dopravniku->Value;if(DDunit==MM)DD/=1000.0;//p�evede v�dy do metr�

	if(DD>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	{
		//default nastaven� komponent
		//ROSTA//scGPNumericEdit_kapacita->Decimal=0;
		vypis("");
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption="Ulo�it";

		//volba ���ka �i d�lka jigu
		double dV=Form1->d.v.PP.delka_voziku;//delka voziku
		if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je po�adov�n ���ka jigu
		double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
		short p=!scGPCheckBox_pocet_mezer->Checked;//po�et mezer mezi voz�ky

		//KAPACITA                 //pokud je stejn� po�et mezer jako voz�k�
		double K=(DD+m)/(dV+m);if(p==1)K=DD/(dV+m);
		//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
		scGPNumericEdit_kapacita->Font->Color=clBlack;
		//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
		if(Form1->ms.get_count_decimal(K)>0)
		{
      scGPNumericEdit_kapacita->Font->Color=clRed;//zv�razn�n� neplatn� vypl�vaj�c� polo�ky
			if(Form1->m.round(K)==0)K=1;//o�et�en� pokud by vych�zela kapacita 0,n�co bylo by zaokrouhleno na 0 a tud� by se vypisoval doporu�en� technologick� �as 0
			if(CTunit==MIN)vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min.");
			else vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s.");
      scGPButton_OK->Enabled=false;//zak�e ukl�dac� tla��tko
		}

		/////////CT,RD
		//v�po�et
		double CT=0;double RD=0;
		if(CT_zamek==UNLOCKED)//CT odem�en�, RD zam�en�
		{
				CT = Form1->d.v.PP.TT*K;
				RD = DD/CT;//po��t� z ji� p�epo��tan� hodnoty, tak�e z�stane stejn�
		}
		else//je zam�en� CT, odem�en� RD
		{
				if(scGPNumericEdit_CT->Value==0)CT=(Form1->d.v.PP.TT*K);//o�et�en� pokud nebyl aktualizov�n
									 //ze zadan�ho CT
				if(CTunit==MIN)CT=scGPNumericEdit_CT->Value*60.0;else CT=scGPNumericEdit_CT->Value;
				RD = DD/CT;//ze zadan�ho CT
				CT = DD/RD;//po��t� z ji� p�epo��tan� hodnoty, tak�e z�stane stejn�
		}
		//pln�n� a form�tov�n� editboxu CT
		if(CTunit==MIN)
		{
			//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastav� zobrazen� po�tu desetinn�ch m�st
			scGPNumericEdit_CT->Value=CT/60.0;//pln�n� pat�i�n�ho pol��ka
		}
		else
		{
			//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
			scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
		}
		//pln�n� a form�tov�n� editboxu RD
		if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
		//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
		scGPNumericEdit_RD->Value=RD;//pln�n� pat�i�n�ho pol��ka
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
	 //p�evede p��padn� na metr a na jednu sekundu, nen�-li
	 if(RDunitD==MM)RD/=1000.0;
	 if(RDunitT==MIN)RD/=60.0;

	 if(RD>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
			 //default nastaven� komponent
			 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
    	 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Ulo�it";

			 //volba ���ka �i d�lka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je po�adov�n ���ka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//po�et mezer mezi voz�ky

    	 double K=0;
			 if(m>0 && p==1)//pokud je rozd�ln� po�et mezer jako voz�k� a mezera je nenulova, lze p��mo z RD vypo��tat ostatn� hodnoty
    	 {
    			//KAPACITA
					K=m/(dV+m-RD*Form1->d.v.PP.TT);
    			//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
					scGPNumericEdit_kapacita->Font->Color=clBlack;
					//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
					if(Form1->ms.get_count_decimal(K)>0)
					{
            scGPNumericEdit_kapacita->Font->Color=clRed;//zv�razn�n� neplatn� vypl�vaj�c� polo�ky
						if(Form1->m.round(K)==0)K=1;//o�et�en� pokud by vych�zela kapacita 0,n�co bylo by zaokrouhleno na 0 a tud� by se vypisoval doporu�en� technologick� �as 0
						if(CTunit==MIN)vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
						else vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
            scGPButton_OK->Enabled=false;//zak�e ukl�dac� tla��tko
					}

					//D�LKA DOPRAVN�KU
					double DD = K*dV+(K-p)*m;
					if(DDunit==MM)DD*=1000.0;
					//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka
					if(DDunit==MM)DD/=1000.0;//vr�cen� do p�vodn�ho stavu kv�li dal��mu pou�it


    			//PROCESN� �AS resp. CT
					double CT = RD/DD;
					if(CTunit==MIN)CT/60.0;
					//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
			 }
    	 else //pokud ne, je nutn� z�skat dal�� nenulov� vstup
			 {
					double DD=scGPNumericEdit_delka_dopravniku->Value;if(DDunit==MM)DD/=1000.0; //na metry
					double CT=scGPNumericEdit_CT->Value;if(CTunit==MIN)CT*60.0; //na sekunkdy
    			double K=scGPNumericEdit_kapacita->Value;

    			if(DD!=0)//pokud je zn�m� d�lka dopravn�ku
					{
    				//KAPACITA
						K=DD/RD/Form1->d.v.PP.TT;
						//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
						scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
						scGPNumericEdit_kapacita->Font->Color=clBlack;

						Memo1->Lines->Add(AnsiString(RD)+" "+DD+" "+Form1->d.v.PP.TT);
						//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
						if(Form1->ms.get_count_decimal(K)>0)
						{
							scGPNumericEdit_kapacita->Font->Color=clRed;//zv�razn�n� neplatn� vypl�vaj�c� polo�ky
							if(Form1->m.round(K)==0)K=1;//o�et�en� pokud by vych�zela kapacita 0,n�co bylo by zaokrouhleno na 0 a tud� by se vypisoval doporu�en� technologick� �as 0
							if(CTunit==MIN)vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
							else vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
              scGPButton_OK->Enabled=false;//zak�e ukl�dac� tla��tko
						}

    				//PROCESN� �AS resp. CT
						double CT = DD/RD;
						if(CTunit==MIN)CT/60.0;
						//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
						scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
							//Memo1->Lines->Add(K);
    			}
    			else
					{
						if(CT!=0)//pokud nen� zn�m� d�lka ale je zn�me CT
    				{
        			//D�LKA DOPRAVN�KU
							DD=RD*CT;
							if(DDunit==MM)DD*=1000.0;
							//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
							scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka

        			//KAPACITA
							K=CT/Form1->d.v.PP.TT;
        			//ROSTA//scGPNumericEdit_kapacita->Decimal=Form1->ms.get_count_decimal(K);//nastav� zobrazen� po�tu desetinn�ch m�st
        			scGPNumericEdit_kapacita->Value=K;//pln�n� pat�i�n�ho pol��ka
              scGPNumericEdit_kapacita->Font->Color=clBlack;
							//pokud obsahuje kapacita re�lnou ��st, vyp�e doporu�en�
							if(Form1->ms.get_count_decimal(K)>0)
							{
                scGPNumericEdit_kapacita->Font->Color=clRed;//zv�razn�n� neplatn� vypl�vaj�c� polo�ky
								if(Form1->m.round(K)==0)K=1;//o�et�en� pokud by vych�zela kapacita 0,n�co bylo by zaokrouhleno na 0 a tud� by se vypisoval doporu�en� technologick� �as 0
								if(CTunit==MIN)vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT/60.0)+" min!");
								else vypis("Doporu�en� technologick� �as je: "+AnsiString(Form1->m.round(K)*Form1->d.v.PP.TT)+" s!");
                scGPButton_OK->Enabled=false;//zak�e ukl�dac� tla��tko
							}
						}
    				else
    				{
    					if(K!=0)//pokud nen� zn�m� d�lka ani CT a je zn�m� K
    					{
    						//D�LKA DOPRAVN�KU
								DD=RD*Form1->d.v.PP.TT*K;
								if(DDunit==MM)DD*=1000.0;
								//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
								scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka
								if(DDunit==MM)DD/=1000.0;//navr�cen� do p�vodn�ho stavu kv�li dal��mu pou�it�

    						//PROCESN� �AS resp. CT
								double CT = RD/DD;
								if(CTunit==MIN)CT/60.0;
								//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
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

   //ShowMessage("Ne - prepocitam");
	 input_state=C;
	 double K=scGPNumericEdit_kapacita->Value;//z�sk�n� kapacity od u�ivatele
	 if(K>0)//nutn� o�et�en� pro obdob� zad�v�n�/psan�
	 {
	 //ShowMessage(K);
    	 //default nastaven� komponent
    	 //ROSTA//scGPNumericEdit_kapacita->Decimal=0;
			 vypis("");
			 scGPButton_OK->Enabled=true;
			 scGPButton_OK->Caption="Ulo�it";

			 //volba ���ka �i d�lka jigu
			 double dV=Form1->d.v.PP.delka_voziku;//delka voziku
			 if(scComboBox_rotace->ItemIndex==1)dV=Form1->d.v.PP.sirka_voziku;//pokud je po�adov�n ���ka jigu
			 double m=scGPNumericEdit_mezera->Value;//mezera mezi voziky
			 short p=!scGPCheckBox_pocet_mezer->Checked;//po�et mezer mezi voz�ky

			 //PROCESN� �AS resp. CT
			 double CT = 0;//Form1->d.v.PP.TT;
			 if(CT_zamek==UNLOCKED)
			 {
					 CT=Form1->d.v.PP.TT*K;
					 if(CTunit==MIN)
					 {
							//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT/60.0);//nastav� zobrazen� po�tu desetinn�ch m�st
							scGPNumericEdit_CT->Value=CT/60.0;//pln�n� pat�i�n�ho pol��ka
					 }
					 else
					 {
							//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
							scGPNumericEdit_CT->Value=CT;//pln�n� pat�i�n�ho pol��ka
					 }
			 }
			 else
			 {
						CT=scGPNumericEdit_CT->Value; if(CTunit==MIN)CT*=60;
			 }

       //D�LKA DOPRAVN�KU
			 double DD = K*dV+(K-p)*m;//o�et�eno i pro stav stejn�ho po�tu voz�ku a mezer;
			 if(DDunit==MM)DD*=1000.0;
			 //ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
			 scGPNumericEdit_delka_dopravniku->Value=DD;//pln�n� pat�i�n�ho pol��ka
			 if(DDunit==MM)DD/=1000.0;//navr�cen� do p�vodn�ho stavu kv�li dal��mu pou�it�

    	 //RYCHLOST DOPRAVN�KU (�ist� jen pro KONTINUAL)
    	 if(scComboBox_rezim->ItemIndex==1)
			 {
					double RD=DD/CT;
					if(RDunitT==MIN)RD*=60.0; if(RDunitD==MM)RD*=1000.0;
					//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
					scGPNumericEdit_RD->Value=RD;//pln�n� pat�i�n�ho pol��ka
			 }
	 }
	 else
	 {
		null_input_value();
		vypis("Doporu�en� kapacita je 1!");
		scGPButton_OK->Enabled=false;
	 }
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
	double RD=0.0;double CT=0.0;
	if(minsec==MIN)//pokud je v minut�ch, tak p�epne na sekundy
	{
		minsec=S;scGPButton_min_sec->Caption="v�e na min";//samotn� tla��tko,ukazuje n�zev opa�n�
		//CT - p�epo��t�n�
		CTunit=S;
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologick� �as <font color=#2b579a>[s]</font>";
		//RD - p�epo��t�n�
		RDunitT=S;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value/60.0;

	}
	else//pokud je v sekund�ch p�epne na minuty
	{
		minsec=MIN;scGPButton_min_sec->Caption="v�e na s";//samotn� tla��tko,ukazuje n�zev opa�n�
		//CT - p�epo��t�n�
		CTunit=MIN;
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologick� �as <font color=#2b579a>[min]</font>";
		//RD - p�epo��t�n�
		RDunitT=MIN;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value*60.0;
	}
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_CT->Value=CT;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//po�adavek na zm�nu jednotek CT
void __fastcall TForm_parametry::rHTMLLabel_CTClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double CT=0.0;
	if(CTunit==MIN)//pokud je v minut�ch, tak p�epne na sekundy
	{
		CTunit=S;
		//CT - p�epo��t�n�
		CT=scGPNumericEdit_CT->Value*60.0;
		rHTMLLabel_CT->Caption="Technologick� �as <font color=#2b579a>[s]</font>";
	}
	else//pokud je v sekund�ch p�epne na minuty
	{
		CTunit=MIN;
		//CT - p�epo��t�n�
		CT=scGPNumericEdit_CT->Value/60.0;
		rHTMLLabel_CT->Caption="Technologick� �as <font color=#2b579a>[min]</font>";
	}
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(CT);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_CT->Value=CT;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//po�adavek na zm�nu jednotek RD
void __fastcall TForm_parametry::rHTMLLabel_RDClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double RD=0.0;

	if(RDunitT==MIN)//pokud je v minut�ch, tak p�epne na sekundy
	{
		RDunitT=S;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value/60.0;
		//RD=RDunitD_funkce(RD);
	}
	else//pokud je v sekund�ch p�epne na minuty
	{
		RDunitT=MIN;
		if(m_mm==MM)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v milimetrech
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v metrech
		RD=scGPNumericEdit_RD->Value*60.0;
		RD=RDunitD_funkce(RD);
	}
  RD=RDunitD_funkce(RD);
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
double TForm_parametry::RDunitD_funkce(double RD)//podp�rn� metoda v��e uveden�
{
		if(RDunitD==MM)//pokud je v milimetrech, tak p�epne na metry
		{
			RDunitD=M;
			if(RDunitT==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v minut�ch
			else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekund�ch
			return RD/1000.0;
		}
		else
		{
			RDunitD=MM;
			if(RDunitT==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v minut�ch
			else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekund�ch
			return RD*1000.0;
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�ep�n�n� zobrazen� m vs. mm
void __fastcall TForm_parametry::Button_metry_milimetryClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double DD=0.0;double DM=0.0;double RD=0.0;
	if(m_mm==MM)//pokud je v milimetrech, tak p�epne na metry
	{
		m_mm=M;scGPButton_metry_milimetry->Caption="v�e na mm";//samotn� tla��tko,ukazuje n�zev opa�n�
		//DD
		DDunit=M;
		rHTMLLabel_delka_dopravniku->Caption="D�lka kabiny <font color=#2b579a>[m]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value/1000.0;
		//DM
		DMunit=M;
		rHTMLLabel_mezera->Caption="Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
		DM=scGPNumericEdit_mezera->Value/1000.0;
		//RD
		RDunitD=M;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/min]</font>";//pokud je v minut�ch
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[m/s]</font>";// pokud je v sekund�ch
		RD=scGPNumericEdit_RD->Value/1000.0;
	}
	else//pokud je metrech, tak p�epne na milimetry
	{
		m_mm=MM;scGPButton_metry_milimetry->Caption="v�e na m";//samotn� tla��tko,ukazuje n�zev opa�n�
		//DD
		DDunit=MM;
		rHTMLLabel_delka_dopravniku->Caption="D�lka kabiny <font color=#2b579a>[mm]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value*1000.0;
		//DM
		DMunit=MM;
		rHTMLLabel_mezera->Caption="Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
		DM=scGPNumericEdit_mezera->Value*1000.0;
		//RD
		RDunitD=MM;
		if(minsec==MIN)rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/min]</font>";//pokud je v minut�ch
		else rHTMLLabel_RD->Caption="Rychlost pohonu <font color=#2b579a>[mm/s]</font>";// pokud je v sekund�ch
		RD=scGPNumericEdit_RD->Value*1000.0;
	}
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_delka_dopravniku->Value=DD;
	//ROSTA//scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_mezera->Value=DM;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(RD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_RD->Value=RD;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//po�adavek na zm�nu jednotek DD
void __fastcall TForm_parametry::rHTMLLabel_delka_dopravnikuClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double DD=0.0;
	if(DDunit==MM)//pokud je v milimetrech, tak p�epne na metry
	{
		DDunit=M;
		rHTMLLabel_delka_dopravniku->Caption="D�lka kabiny <font color=#2b579a>[m]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value/1000.0;
	}
	else//pokud je metrech, tak p�epne na milimetry
	{
		DDunit=MM;
		rHTMLLabel_delka_dopravniku->Caption="D�lka kabiny <font color=#2b579a>[mm]</font>";
		DD=scGPNumericEdit_delka_dopravniku->Value*1000.0;
	}
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(DD);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_delka_dopravniku->Value=DD;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//po�adavek na zm�nu jednotek DM
void __fastcall TForm_parametry::rHTMLLabel_mezeraClick(TObject *Sender)
{
	input_state=NO;//z�m�r, aby se nep�epo��tavaly hodnoty
	double DM=0.0;
	if(DMunit==MM)//pokud je v milimetrech, tak p�epne na metry
	{
		//DM
		DMunit=M;
		rHTMLLabel_mezera->Caption="Mezera mezi voz�ky <font color=#2b579a>[m]</font>";
		DM=scGPNumericEdit_mezera->Value/1000.0;
	}
	else//pokud je metrech, tak p�epne na milimetry
	{
		//DM
		DMunit=MM;
		rHTMLLabel_mezera->Caption="Mezera mezi voz�ky <font color=#2b579a>[mm]</font>";
		DM=scGPNumericEdit_mezera->Value*1000.0;
	}
	//pln�n� + po�et desetinn�ch m�st
	//ROSTA//scGPNumericEdit_mezera->Decimal=Form1->ms.get_count_decimal(DM);//nastav� zobrazen� po�tu desetinn�ch m�st
	scGPNumericEdit_mezera->Value=DM;
	input_state=NOTHING;//u� se mohou p�epo��t�vat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//p�i stisku kl�vesy enter nebo esc
void __fastcall TForm_parametry::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 if(Key==13)//ENTER
 {
		 if(scGPButton_OK->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_parametry->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_parametry->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
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
//ulo�� do INI nastaven� formul��e
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
//zkop�ruje parametry objektu pro dal�� pou�it�
void __fastcall TForm_parametry::scGPGlyphButton_copyClick(TObject *Sender)
{
	//povol� tla��tko vkl�d�n�
	scGPGlyphButton_paste->Enabled=true;

	//p�evod jednotek
	double jednotky_cas=1.0;if(minsec==MIN)jednotky_cas=60.0;
	double jednotky_vzdalenost=1.0;if(m_mm==MM)jednotky_vzdalenost=1000.0;

	//text do schr�nky
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

	//zkop�rov�n� textu do schr�nky
	TClipboard *c=new TClipboard;
	c->AsText=T;
	c=NULL;delete c;
}
//---------------------------------------------------------------------------
//zaji��uje vlo�en� objektu
void __fastcall TForm_parametry::scGPGlyphButton_pasteClick(TObject *Sender)
{
	//aby se nevolalo p�epo��t�v�n� p�i editu polo�ek
	input_state=NO;

  //p�evod jednotek
	double jednotky_cas=1.0;if(minsec==MIN)jednotky_cas=60.0;
	double jednotky_vzdalenost=1.0;if(m_mm==MM)jednotky_vzdalenost=1000.0;

	scGPEdit_name->Text=Form1->copyObjekt->name;
	scGPEdit_shortname->Text=Form1->copyObjekt->short_name;
	scComboBox_rezim->ItemIndex=Form1->copyObjekt->rezim;
	if(Form1->copyObjekt->pohon==NULL)scComboBox_pohon->ItemIndex=0;
	else scComboBox_pohon->ItemIndex=Form1->copyObjekt->pohon->n-1;
	//ROSTA//scGPNumericEdit_CT->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->CT/jednotky_cas);//nastav� zobrazen� po�tu desetinn�ch m�st;
	scGPNumericEdit_CT->Value=Form1->copyObjekt->CT/jednotky_cas;
	//ROSTA//scGPNumericEdit_RD->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost);//nastav� zobrazen� po�tu desetinn�ch m�st;
	scGPNumericEdit_RD->Value=Form1->copyObjekt->RD*jednotky_cas*jednotky_vzdalenost;
	//ROSTA//scGPNumericEdit_delka_dopravniku->Decimal=Form1->ms.get_count_decimal(Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost);//nastav� zobrazen� po�tu desetinn�ch m�st;
	scGPNumericEdit_delka_dopravniku->Value=Form1->copyObjekt->delka_dopravniku*jednotky_vzdalenost;
	scComboBox_cekani_palec->ItemIndex=Form1->copyObjekt->cekat_na_palce;
	scGPNumericEdit_kapacita->Value=Form1->copyObjekt->kapacita;
	scGPNumericEdit_odchylka->Value=Form1->copyObjekt->odchylka;
	scComboBox_stopka->ItemIndex=Form1->copyObjekt->stopka;
	scComboBox_rotace->ItemIndex=Form1->copyObjekt->rotace;
	scGPNumericEdit_mezera->Value=Form1->copyObjekt->mezera*jednotky_vzdalenost;
	scGPCheckBox_pocet_mezer->Checked=!Form1->copyObjekt->mV;

	//nastaven� aktu�ln�ho re�imu objektu
	setForm4Rezim(scComboBox_rezim->ItemIndex);

	//povolen� p�epo��t�v�n�
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
//z�mek procesn�ho �asu
void __fastcall TForm_parametry::scButton_zamek_CTClick(TObject *Sender)
{
	if(scButton_zamek_CT->ImageIndex==37)//kdy� je zam�eno
	{
		//CT
		scButton_zamek_CT->ImageIndex=38;
		CT_zamek=UNLOCKED;
		//RD
		scButton_zamek_RD->ImageIndex=37;
		RD_zamek=LOCKED;
	}
	else//odem�eno
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
//z�mek d�lky dopravn�ku
void __fastcall TForm_parametry::scButton_zamek_DDClick(TObject *Sender)
{
	if(scButton_zamek_DD->ImageIndex==37)//kdy� je zam�eno
	{
		scButton_zamek_DD->ImageIndex=38;
		DD_zamek=UNLOCKED;
		set(DELKA,ENABLED,false);
	}
	else//odem�eno
	{
		scButton_zamek_DD->ImageIndex=37;
		DD_zamek=LOCKED;
		set(DELKA,DISABLED,false);
	}
}
//---------------------------------------------------------------------------
//p�i kliknut� na doporu�en� nastane aplikace doporu�en� do dan�ho editboxu
void __fastcall TForm_parametry::rHTMLLabel_InfoTextClick(TObject *Sender)
{
	 if(rHTMLLabel_InfoText->Caption.Pos("Doporu�en� technologick� �as je"))
	 {
		if(CTunit==MIN)scGPNumericEdit_CT->Value=Form1->m.round(scGPNumericEdit_kapacita->Value)*Form1->d.v.PP.TT/60.0;
		else scGPNumericEdit_CT->Value=Form1->m.round(scGPNumericEdit_kapacita->Value)*Form1->d.v.PP.TT;
	 }
	 if(rHTMLLabel_InfoText->Caption.Pos("Doporu�en� kapacita je 1!"))
	 {
		scGPNumericEdit_kapacita->Value=1;
	 }
	 if(rHTMLLabel_InfoText->Caption.Pos("Zm�nte re�im nebo rozlo�te do v�ce objekt�!"))
	 {
		scComboBox_rezim->ItemIndex=1;
	 }
}
//---------------------------------------------------------------------------
//kontrola vybran�ho pohonu v��i zadan� rychlosti dopravn�ku
void __fastcall TForm_parametry::scComboBox_pohonChange(TObject *Sender)
{
	if(scComboBox_rezim->ItemIndex!=0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{
			if(scGPNumericEdit_RD->Value<P->rychlost_od || P->rychlost_do<scGPNumericEdit_RD->Value)//nespl�uje rozmez�
			{
				vypis("Pohon neodpov�d� rychlosti!",true);
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
	if(input_state==NOTHING && input_clicked_edit==mezera_klik)//pokud nen� zad�v�no z jin�ho vstupu
	input_DD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny d�lky dopravn�ku
	//hl�d�n� velikosti mezery dle rozte�e
	if(scComboBox_rezim->ItemIndex!=0 && scGPNumericEdit_mezera->Value>0)//mimo S&G
	{
		Cvektory::TPohon *P=Form1->d.v.vrat_pohon(scComboBox_pohon->ItemIndex);
		if(P!=NULL)
		{                            //je "zbytek po d�len�"
			if(P->roztec>0 && !Form1->m.cele_cislo(scGPNumericEdit_mezera->Value/P->roztec))//nespl�uje rozmez�
			{
				vypis("Doporu�eno: "+AnsiString(Form1->m.round(scGPNumericEdit_mezera->Value/P->roztec)*P->roztec)+" m",true);
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

