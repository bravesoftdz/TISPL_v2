//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "kabina_schema.h"
#include "unit1.h"
#include "antialiasing.h"
#include "parametry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_objekt_nahled *Form_objekt_nahled;
//---------------------------------------------------------------------------
__fastcall TForm_objekt_nahled::TForm_objekt_nahled(TComponent* Owner)
	: TForm(Owner)
{
	//nastaven� barvy formul��e
	Form_objekt_nahled->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,Form_objekt_nahled,1,1);

	//Okraj x vykreslovan�ho n�hledu v px
	Ox=5;

	//design pop-up menu
	PopUPmenu->Color=(TColor)RGB(240,240,240); //nastaven� barvy
	Item_posouvat->FillColor=PopUPmenu->Color;
	Item_posunout->FillColor=PopUPmenu->Color;
	Item_priblizit->FillColor=PopUPmenu->Color;
	Item_oddalit->FillColor=PopUPmenu->Color;
	Item_vybrat_oknem->FillColor=PopUPmenu->Color;
	Item_cely_pohled->FillColor=PopUPmenu->Color;

	pom=NULL;//pomocn� ukazatel na objekt s kter�m se bude pracovat
	MAX_pozic=50;//maxim�ln� hodnota zobrazen�ch pozic, jinak ilustrativn� n�hled

}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormShow(TObject *Sender)
{
	Zoom_predchozi=F->Zoom; //pro n�hled z gapo
	if(F->pom!=NULL || pom!=NULL)MODEL();//pokud je n�hled vol�n z PO a jedn� se o n�hled konkr�tn�ho objektu, zde je F->pom OK
	else PREVIEW();//pokud je n�hled vol�n z PL a jedn� se pouze o ilustrativn� n�hled a bude zobrazen pouze ilustrativn� obr�zek, mohlo by se zd�t, �e se jedn� o zbyte�nou v�tev, proto�e se to �e�� v OUTPUTu, ale toto m� speci�ln� v�znam pro situaci odchodu z PO a p��chodu na PL a vol�n� tohoto formu
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		// ENTER
		case 13:
		{
			Form_objekt_nahled->ModalResult = mrOk; // vr�t� stejnou hodnotu jako tla��tko
			KonecClick(Sender);
		}
		break;
		// ESC
		case 27:
		{
			if(PopUPmenu->Visible)//pokud je zobrazeno popupmenu je to ESC pro n�j
			PopUPmenu->Visible=false;
			else
			{
				Form_objekt_nahled->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
				KonecClick(Sender);
			}
		}
		break;
		//F7
		case 118:GlyphButton_priblizitClick(Sender);break;
		//F8
		case 119:GlyphButton_oddalitClick(Sender);break;
		//F12
		case 123:if(DEBUG && !Button1->Visible)Button1->Visible=true;else Button1->Visible=false;break;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	 if(Button==mbRight && pom!=NULL)//je stisknuto prav� tla��tko my�i a n�hled kabiny nen� jen v ilustrativn�m p��kladu, ale konkr�tn�m n�hledu objektu
	 {
			PopUPmenu->Left=X;PopUPmenu->Top=Y;
			//provizorn� zak�z�no PopUPmenu->Visible=true;
	 }
}
//---------------------------------------------------------------------------
//pokud je n�hled vol�n z PO a jedn� se o n�hled konkr�tn�ho objektu, zde je F->pom OK
void TForm_objekt_nahled::MODEL()
{
	OUTPUT();//na�ten� dat z PO
	if(pom->pozice<=MAX_pozic)
	{
		//sekce anima�n� nastaven�
//nen� ji� pot�eba �e�� START_POZICE		Poffset=F->m.UDV(pom->rotace)/2;//cel� voz�k na za��tku kabiny p�v. 0 - to by bylo z p�lky voz�ku
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
		ButtonPLAY->Hint="spustit animaci";
		Timer_animace->Enabled=false;
		timerTakt=0;
		if(Form_parametry->CTunit==0)ButtonPLAY->Caption="0 [s]";
		else ButtonPLAY->Caption="0 [min]";
		//--
		scGPButton_OK->Visible=false;
		Image_nahled->Visible=false;
		Form_objekt_nahled->Color=clWhite;
		F->nahled_objektu=true;//uchov�v� stav, zda se jedn� o n�hled objekt �i regulern� zobrazen� ve form1
		Max_Min_Button->Visible=true;
		reposition_windowButton->Visible=true;
		scGPGlyphButton_info->Visible=true;//pro zobrazen� info n�hledu
		nahledZmodelu=false;//eviduje zda byl zobrazen n�hled z modelu, slou�� k mo�nosti navr�cen� do modelu
		ENTIRE();//cel� pohled, max p�ibl�en� objektu
	}
}
//---------------------------------------------------------------------------
//pokud je n�hled vol�n z PL a jedn� se pouze o ilustrativn� n�hled a bude zobrazen pouze ilustrativn� obr�zek
void TForm_objekt_nahled::PREVIEW()
{
	scGPGlyphButton_info->Visible=false;//mus� b�t pravd�podobn� p�ed invalidate
	Invalidate();
	scGPButton_OK->Visible=true;
	Image_nahled->Visible=true;
	Form_objekt_nahled->Color=(TColor)RGB(240,240,240);
	F->nahled_objektu=false;//uchov�v� stav, zda se jedn� o n�hled objekt �i regulern� zobrazen� ve form1
	Max_Min_Button->Visible=false;
	ButtonPLAY->Visible=false;
	reposition_windowButton->Visible=false;
}
//---------------------------------------------------------------------------
//cely_nahled, zajist� v�po�et zoomu tak, aby se objekt zobrazil p�es cel� okno, ale v�etn� okraje Ox
void TForm_objekt_nahled::ENTIRE()
{
	if(pom!=NULL)
	{                 //z�m�rnou z�m�nou parametr� z�sk�m u�itou ���ku
		double S=F->m.UDJ(F->d.v.PP.sirka_jig,F->d.v.PP.delka_jig,pom->rotace);
		double D=pom->delka_dopravniku;
		if(D>=S)//dle d�lky objektu a ���ky formul��e
			F->Zoom=Width/(Ox+D/F->m2px+Ox);//stanoven� velikosti zoomu tak, aby se zobrazil cel� objekt, co nejv�ce zv�t�en�
		else //dle ���ky jigu a v��ky formul��e
			F->Zoom=Height/(Ox+S/F->m2px+Ox);//stanoven� velikosti zoomu tak, aby se zobrazil cel� objekt, co nejv�ce zv�t�en�
	}
}
//---------------------------------------------------------------------------
//vybere bu� Invalidate nebo FormPaint(this) dle if(!antialiasing)
void TForm_objekt_nahled::REFRESH(bool invalidate)
{
	if(!F->antialiasing && invalidate)Invalidate();
	else FormPaint(this);//pokude je zapntut� antialiasing neproblikne, ale jen se "p�epl�cne" bitmapou nedojde k probliknut�
}
//---------------------------------------------------------------------------
//obnov� dat ve formu Form_objekt_nahled v�etn� p�ekreslen�
void TForm_objekt_nahled::REFRESH_DATA()
{
	OUTPUT();
	ENTIRE();
	REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormPaint(TObject *Sender)
{
	//n�hled objektu
	if(pom!=NULL)
	{
		if(pom->pozice<=MAX_pozic)//aby se nevykreslovalo pod obr�zekem
		{
			scGPLabel_info->Visible=false;                                             //z�loha F->m.P2Lx(Ox/F->m2px)
			if(!F->antialiasing)F->d.vykresli_objekt(Canvas,pom,F->m.P2Lx(F->m.round((Width-pom->delka_dopravniku*F->Zoom/F->m2px)/2.0)),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)/2.0)),Poffset,Timer_animace->Enabled);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
				F->Zoom*=3;//*3 vypl�v� z logiky algoritmu antialiasingu           //pop�. ne�e�� "kr�tk�" objekty, z�loha F->m.P2Lx(Ox/F->m2px)
				F->d.vykresli_objekt(bmp_in->Canvas,pom,F->m.P2Lx(F->m.round((Width*3-pom->delka_dopravniku*F->Zoom/F->m2px)/2.0)),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)*3/2.0)),Poffset,Timer_animace->Enabled);
				F->Zoom/=3;//navr�cen� zoomu na p�vodn� hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutn�
				delete (bmp_in);//velice nutn�
			}
			//grafick� m���tko                        //t�m �e nen� v bmp-tak p�i animaci problik�v�, zkou�el jsem i zvolit i j�t p�es druhou bmp s transparentnost�, ale tak� doch�zelo k problik�v�n�, jedin� �e�en� je asi tak� aplikace do bmp_in v r�mci AA, ale m���tko by se muselo na v��ku (asi 3x) zv�t�it a to sam� font m���tka
			if(F->scGPSwitch_meritko->State==true && !Timer_animace->Enabled)F->d.meritko(Canvas,Ox/F->m2px,/*0+scGPPanel_hlavicka->Height+Ox/2*/Height-22);
		}
	}

	//or�mov�n� formul��e    //r�m ru�iv� p�i animaci blikal
	if(zobrazitFrameForm && !Timer_animace->Enabled)F->m.frameForm(Form_objekt_nahled,clWebOrange,1);
}
//---------------------------------------------------------------------------
//napln� data do lok�ln� (glob�ln� v tomto formul��i) prom�nn� pom,nejedn� se o F->pom, ta toto�i nen� v tomto momentu z d�vodu prob�haj�c�ch zm�na zcela aktu�ln�
void TForm_objekt_nahled::OUTPUT()
{
	 //p�evzet� hodnot z Form_Parametry
	 if(F->pom!=NULL && Form_parametry->Visible)//pokud je vol�no z PO
	 {
		 pom=new Cvektory::TObjekt;
		 double jednotky_cas=60.0;double jednotky_vzdalenost=1000.0;
		 pom->name=Form_parametry->scGPEdit_name->Text;scLabel_titulek->Caption=pom->name.UpperCase()+" - n�hled";
		 pom->short_name=Form_parametry->scGPEdit_shortname->Text;
		 pom->pohon=F->d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex);//indexuje se od nuly,ale pohony od 1 (nicm�n� nov� je na prvn�m m�st� polo�ka nep�i�azen), pokud pohon neexituje vr�ti null
		 pom->rezim=Form_parametry->scComboBox_rezim->ItemIndex;
		// pom->cekat_na_palce=Form_parametry->scComboBox_cekani_palec->ItemIndex;
		 pom->kapacita=Form_parametry->scGPNumericEdit_kapacita->Value;
		 pom->pozice=Form_parametry->scGPNumericEdit_pozice->Value;
		// pom->odchylka=Form_parametry->scGPNumericEdit_odchylka->Value;
		// pom->stopka=Form_parametry->scComboBox_stopka->ItemIndex;
		 //DD
		 if(Form_parametry->DDunit==Form_parametry->MM)jednotky_vzdalenost=1000.0;else jednotky_vzdalenost=1.0;
		 pom->delka_dopravniku=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;
		 //RD
		 if(Form_parametry->RDunitT==Form_parametry->MIN)jednotky_cas=60.0;else jednotky_cas=1.0;
		 //if(Form_parametry->RDunitD==Form_parametry->MM)jednotky_vzdalenost=1000.0;else
		 jednotky_vzdalenost=1.0;
		 pom->RD=Form_parametry->scGPNumericEdit_RD->Value/jednotky_cas/jednotky_vzdalenost;
		 //if(pom->pohon!=NULL && pom->rezim==1)pom->pohon->aRD=pom->RD;//ulo�� i aktul�n� rychlost pohonu
		 //DM
		 if(Form_parametry->DMunit==Form_parametry->MM)jednotky_vzdalenost=1000.0;else jednotky_vzdalenost=1.0;
		 pom->mezera=Form_parametry->scGPNumericEdit_mezera->Value/jednotky_vzdalenost;
		 //ostatni
		 pom->rotace=Form_parametry->scComboBox_rotace->ItemIndex;
		 //CT
		 if(Form_parametry->CTunit==Form_parametry->MIN)jednotky_cas=60.0;else jednotky_cas=1.0;
		 pom->CT=Form_parametry->scGPNumericEdit_CT->Value*jednotky_cas;
	 }
	 else//pro gapo form
	 {
			 //pom=F->pom; net�eba prob�h� p��mo v pln�n� gapa formu
	 }
	 //nov� v�choz� pozice po rotaci voz�ku
	 START_POZICE();

	 //povolen� zobrazen� animace
	 if(pom->rezim==1)ButtonPLAY->Visible=true;else ButtonPLAY->Visible=false;//pouze v kontinu�ln�m re�imu je mo�n� animace

	 //Nelze zobrazit n�hled objektu s v�ce jak s MAX_pozic pozicemi, bude zobrazen pouze ilustrativn� n�hled
	 if(pom->pozice>MAX_pozic)
	 {
		 scGPLabel_info->Caption="Nelze zobrazit n�hled objektu s v�ce jak s "+AnsiString(MAX_pozic)+" pozicemi.";
		 scGPLabel_info->Left=Width-scGPLabel_info->Width-10;
		 scGPLabel_info->Visible=true;
		 PREVIEW();
	 }
}
//---------------------------------------------------------------------------
//nov� v�choz� pozice po rotaci voz�ku
void TForm_objekt_nahled::START_POZICE()
{
	if(Timer_animace->Enabled==false)//zm�na pozice naz�klad� rotace je mo�n� jen a pouze pokud neb�� animace
	Poffset=F->m.UDV(pom->rotace)/2;//slou�� pro p��pad rotace voz�ku//cel� voz�k na za��tku kabiny p�v. 0 - to by bylo z p�lky voz�ku,tzn. v momentu aktivnih� palce
}
//---------------------------------------------------------------------------
//odchod z okna a to i p�i stisku tla��tka OK
void __fastcall TForm_objekt_nahled::KonecClick(TObject *Sender)
{
	if(!nahledZmodelu)
	{
		Timer_animace->Enabled=false;
		scGPLabel_info->Visible=false;
		F->Zoom=Zoom_predchozi;//n�vrat do p�vodn�ho stavu
		F->nahled_objektu=false;//uchov�v� stav, zda se jedn� o n�hled objekt �i regulern� zobrazen� ve form1
    pom->pohon=NULL; delete pom->pohon;//otestovat!!!
		pom=NULL; delete pom;
		Close();
	}
	else
	{
		MODEL();
		nahledZmodelu=false;
	}
}
//---------------------------------------------------------------------------
//maximalizuje nebo minalizuje pohled
void __fastcall TForm_objekt_nahled::Max_Min_ButtonClick(TObject *Sender)
{
	if(Max_Min_Button->GlyphOptions->Kind==scgpbgkMaximize)
	{
		Top=/*scGPPanel_hlavicka->Height*/0;Left=0;
		Height=F->Height-Top;Width=F->Width;
		Max_Min_Button->GlyphOptions->Kind=scgpbgkRestore;
		reposition_windowButton->Visible=false;
		scGPGlyphButton_info->Visible=false;//workaround kv�li po�ad�
		zobrazitFrameForm=false;
	}
	else
	{
		Konec->Visible=false;//workaround kv�li po�ad�
		Max_Min_Button->Visible=false;//workaround kv�li po�ad�
		scGPGlyphButton_info->Visible=false;//workaround kv�li po�ad�
		reposition_windowButtonClick(Sender);//vol� porovn�n� oken
		Max_Min_Button->GlyphOptions->Kind=scgpbgkMaximize;
		zobrazitFrameForm=true;
	}
	//nutn� p�edch�zej�c� refresh Form1
	F->nahled_objektu=false;
	F->Zoom=Zoom_predchozi;
	F->REFRESH();
	F->nahled_objektu=true;
	//refresh a p�epo��t�n� m���tka  Form_objekt_nahled
	ENTIRE();//cely_nahled, zajist� v�po�et zoomu tak, aby se objekt zobrazil p�es cel� okno, ale v�etn� okraje Ox
	REFRESH();
}
//---------------------------------------------------------------------------
//p�eorganizuje okna vedle sebe n�hled kabiny a form PO
void __fastcall TForm_objekt_nahled::reposition_windowButtonClick(TObject *Sender)
{
	Width=737;Height=423;Left=5;Top=Form_parametry->Top;
	Form_parametry->Left=Left+Form_objekt_nahled->Width+5;
	scGPGlyphButton_info->Visible=true;//workaround kv�li po�ad�
	reposition_windowButton->Visible=true;//workaround kv�li po�ad�
	Max_Min_Button->Visible=true;//workaround kv�li po�ad�
	Konec->Visible=true;//workaround kv�li po�ad�
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajist� p�ibl�en� obrazu
void __fastcall TForm_objekt_nahled::GlyphButton_priblizitClick(TObject *Sender)
{
	F->ZOOM_IN();PopUPmenu->Visible=false;REFRESH();
}
//---------------------------------------------------------------------------
//zajist� odd�len� obrazu
void __fastcall TForm_objekt_nahled::GlyphButton_oddalitClick(TObject *Sender)
{
	F->ZOOM_OUT();PopUPmenu->Visible=false;REFRESH();
}
//---------------------------------------------------------------------------
//zajist� cel� n�hled
void __fastcall TForm_objekt_nahled::GlyphButton_cely_pohledClick(TObject *Sender)
{
	ENTIRE();//cely_nahled
	PopUPmenu->Visible=false;
	REFRESH();
}
//---------------------------------------------------------------------------
//ukon�en� pop-up menu
void __fastcall TForm_objekt_nahled::GlyphButton_closeClick(TObject *Sender)
{
	PopUPmenu->Visible=false;
}
//---------------------------------------------------------------------------
//p�epne do ilustrativn�ho n�hledu s popisky
void __fastcall TForm_objekt_nahled::scGPGlyphButton_infoClick(TObject *Sender)
{
	nahledZmodelu=true;
	PREVIEW();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//ANIMACE
void __fastcall TForm_objekt_nahled::ButtonPLAYClick(TObject *Sender)
{
	Timer_animace->Enabled=!Timer_animace->Enabled;
	if(Timer_animace->Enabled)//b�� animace
	{
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPause;
		ButtonPLAY->Hint="zastavit animaci";
		ButtonPLAY->ShowCaption=true;
		Timer_animace->Interval=F->m.round(F->m.get_timePERpx(pom->RD,0));//stejn� rychlost pro v�echny RD
		//Timer_animace->Interval=ceil(F->m.get_timePERpx(pom->RD,0,F->d.v.vrat_min_rychlost_prejezdu()));//r�zn� rychlost dle RD, s afps se po��t� dle min RD, ale n�jak �patn� vych�zela animace ke konci (nestihl voz�k vyjet)
	}
	else//animace zastavena
	{
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
		ButtonPLAY->Hint="spustit animaci";
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::Timer_animaceTimer(TObject *Sender)
{
	Poffset+=1*F->m2px/F->Zoom;//zajist� posun animace o 1px (tedy nejmen�� mo�nou grafickou jednotku), ale posouv�n� prob�h� v metrech

	double Z=1;if(F->antialiasing)Z=3.0;

	//v�po�et aktu�ln�ho �asu
	double Time=F->m.round(++timerTakt*F->afps/Z*Timer_animace->Interval/1000.0);//chybn� �vaha, koncipov�no pouze pro rychlosti 1m/s a vy���//vr�t� �as, tak aby se jednalo o kontin�ln� animaci

	//vypisuje aktu�ln� CT
	if(Form_parametry->CTunit==0)ButtonPLAY->Caption=AnsiString(F->m.round(Time))+" [s]";
	else ButtonPLAY->Caption=AnsiString(F->m.round2double(Time/60.0,2))+" [min]";

	//zastav� animaci po dovr�en� CT
	if(Time>=pom->CT)//zastav� animaci, jak vypr�el CT
	{
		ButtonPLAYClick(Sender);
		START_POZICE();//v�choz� pozice voz�k�
		if(Form_parametry->CTunit==0)ButtonPLAY->Caption="0 [s]";
		else ButtonPLAY->Caption="0 [min]";
		timerTakt=0;//vynuluje �as do v�choz� hodnoty
		zobrazitFrameForm=true;
	}
	REFRESH_DATA();//na�te aktu�ln� data (umo��uje tedy i b�hem animace m�nit za b�hu parametry - mo�n� blbost),zjist� vhodn� m���tka a na z�v�r REFRESHNE obraz
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::Button1Click(TObject *Sender)
{
		if(1/F->fps*1000<Timer_animace->Interval)ShowMessage("Animace nebude vykreslena kontinu�ln� �i zcela kontinu�ln�!");
		else ShowMessage("Animace by m�la b�t vykreslena kontinu�ln�");

//ShowMessage(F->m2px/F->Zoom);
//ShowMessage(F->Zoom);
ShowMessage(Timer_animace->Interval);
//Timer_animace->Interval=3.838541666666875;
}
//---------------------------------------------------------------------------



