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
	//nastavení barvy formuláøe
	Form_objekt_nahled->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK
	F->m.designButton(scGPButton_OK,Form_objekt_nahled,1,1);

	//Okraj x vykreslovaného náhledu v px
	Ox=5;

	//design pop-up menu
	PopUPmenu->Color=(TColor)RGB(240,240,240); //nastavení barvy
	Item_posouvat->FillColor=PopUPmenu->Color;
	Item_posunout->FillColor=PopUPmenu->Color;
	Item_priblizit->FillColor=PopUPmenu->Color;
	Item_oddalit->FillColor=PopUPmenu->Color;
	Item_vybrat_oknem->FillColor=PopUPmenu->Color;
	Item_cely_pohled->FillColor=PopUPmenu->Color;

	pom=NULL;//pomocný ukazatel na objekt s kterým se bude pracovat
	MAX_pozic=50;//maximální hodnota zobrazených pozic, jinak ilustrativní náhled

}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormShow(TObject *Sender)
{
	Zoom_predchozi=F->Zoom; //pro náhled z gapo
	if(F->pom!=NULL || pom!=NULL)MODEL();//pokud je náhled volán z PO a jedná se o náhled konkrétního objektu, zde je F->pom OK
	else PREVIEW();//pokud je náhled volán z PL a jedná se pouze o ilustrativní náhled a bude zobrazen pouze ilustrativní obrázek, mohlo by se zdát, že se jedná o zbyteènou vìtev, protože se to øeší v OUTPUTu, ale toto má speciální význam pro situaci odchodu z PO a pøíchodu na PL a volání tohoto formu
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		// ENTER
		case 13:
		{
			Form_objekt_nahled->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlaèítko
			KonecClick(Sender);
		}
		break;
		// ESC
		case 27:
		{
			if(PopUPmenu->Visible)//pokud je zobrazeno popupmenu je to ESC pro nìj
			PopUPmenu->Visible=false;
			else
			{
				Form_objekt_nahled->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
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
	 if(Button==mbRight && pom!=NULL)//je stisknuto pravé tlaèítko myši a náhled kabiny není jen v ilustrativním pøíkladu, ale konkrétním náhledu objektu
	 {
			PopUPmenu->Left=X;PopUPmenu->Top=Y;
			//provizornì zakázáno PopUPmenu->Visible=true;
	 }
}
//---------------------------------------------------------------------------
//pokud je náhled volán z PO a jedná se o náhled konkrétního objektu, zde je F->pom OK
void TForm_objekt_nahled::MODEL()
{
	OUTPUT();//naètení dat z PO
	if(pom->pozice<=MAX_pozic)
	{
		//sekce animaèní nastavení
//není již potøeba øeší START_POZICE		Poffset=F->m.UDV(pom->rotace)/2;//celý vozík na zaèátku kabiny pùv. 0 - to by bylo z pùlky vozíku
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
		F->nahled_objektu=true;//uchovává stav, zda se jedná o náhled objekt èi regulerní zobrazení ve form1
		Max_Min_Button->Visible=true;
		reposition_windowButton->Visible=true;
		scGPGlyphButton_info->Visible=true;//pro zobrazení info náhledu
		nahledZmodelu=false;//eviduje zda byl zobrazen náhled z modelu, slouží k možnosti navrácení do modelu
		ENTIRE();//celý pohled, max pøiblížení objektu
	}
}
//---------------------------------------------------------------------------
//pokud je náhled volán z PL a jedná se pouze o ilustrativní náhled a bude zobrazen pouze ilustrativní obrázek
void TForm_objekt_nahled::PREVIEW()
{
	scGPGlyphButton_info->Visible=false;//musí být pravdìpodobnì pøed invalidate
	Invalidate();
	scGPButton_OK->Visible=true;
	Image_nahled->Visible=true;
	Form_objekt_nahled->Color=(TColor)RGB(240,240,240);
	F->nahled_objektu=false;//uchovává stav, zda se jedná o náhled objekt èi regulerní zobrazení ve form1
	Max_Min_Button->Visible=false;
	ButtonPLAY->Visible=false;
	reposition_windowButton->Visible=false;
}
//---------------------------------------------------------------------------
//cely_nahled, zajistí výpoèet zoomu tak, aby se objekt zobrazil pøes celé okno, ale vèetnì okraje Ox
void TForm_objekt_nahled::ENTIRE()
{
	if(pom!=NULL)
	{                 //zámìrnou zámìnou parametrù získám užitou šíøku
		double S=F->m.UDJ(F->d.v.PP.sirka_jig,F->d.v.PP.delka_jig,pom->rotace);
		double D=pom->delka_dopravniku;
		if(D>=S)//dle délky objektu a šíøky formuláøe
			F->Zoom=Width/(Ox+D/F->m2px+Ox);//stanovení velikosti zoomu tak, aby se zobrazil celý objekt, co nejvíce zvìtšený
		else //dle šíøky jigu a výšky formuláøe
			F->Zoom=Height/(Ox+S/F->m2px+Ox);//stanovení velikosti zoomu tak, aby se zobrazil celý objekt, co nejvíce zvìtšený
	}
}
//---------------------------------------------------------------------------
//vybere buï Invalidate nebo FormPaint(this) dle if(!antialiasing)
void TForm_objekt_nahled::REFRESH(bool invalidate)
{
	if(!F->antialiasing && invalidate)Invalidate();
	else FormPaint(this);//pokude je zapntutý antialiasing neproblikne, ale jen se "pøeplácne" bitmapou nedojde k probliknutí
}
//---------------------------------------------------------------------------
//obnoví dat ve formu Form_objekt_nahled vèetnì pøekreslení
void TForm_objekt_nahled::REFRESH_DATA()
{
	OUTPUT();
	ENTIRE();
	REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::FormPaint(TObject *Sender)
{
	//náhled objektu
	if(pom!=NULL)
	{
		if(pom->pozice<=MAX_pozic)//aby se nevykreslovalo pod obrázekem
		{
			scGPLabel_info->Visible=false;                                             //záloha F->m.P2Lx(Ox/F->m2px)
			if(!F->antialiasing)F->d.vykresli_objekt(Canvas,pom,F->m.P2Lx(F->m.round((Width-pom->delka_dopravniku*F->Zoom/F->m2px)/2.0)),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)/2.0)),Poffset,Timer_animace->Enabled);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				F->Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu           //popø. neøeší "krátké" objekty, záloha F->m.P2Lx(Ox/F->m2px)
				F->d.vykresli_objekt(bmp_in->Canvas,pom,F->m.P2Lx(F->m.round((Width*3-pom->delka_dopravniku*F->Zoom/F->m2px)/2.0)),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)*3/2.0)),Poffset,Timer_animace->Enabled);
				F->Zoom/=3;//navrácení zoomu na pùvodní hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvùli smazání bitmapy vracené AA
				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutné
				delete (bmp_in);//velice nutné
			}
			//grafické mìøítko                        //tím že není v bmp-tak pøi animaci problikává, zkoušel jsem i zvolit i jít pøes druhou bmp s transparentností, ale také docházelo k problikávání, jediné øešení je asi také aplikace do bmp_in v rámci AA, ale mìøítko by se muselo na výšku (asi 3x) zvìtšit a to samé font mìøítka
			if(F->scGPSwitch_meritko->State==true && !Timer_animace->Enabled)F->d.meritko(Canvas,Ox/F->m2px,/*0+scGPPanel_hlavicka->Height+Ox/2*/Height-22);
		}
	}

	//orámování formuláøe    //rám rušivì pøi animaci blikal
	if(zobrazitFrameForm && !Timer_animace->Enabled)F->m.frameForm(Form_objekt_nahled,clWebOrange,1);
}
//---------------------------------------------------------------------------
//naplní data do lokální (globální v tomto formuláøi) promìnné pom,nejedná se o F->pom, ta totoži není v tomto momentu z dùvodu probíhajících zmìna zcela aktuální
void TForm_objekt_nahled::OUTPUT()
{
	 //pøevzetí hodnot z Form_Parametry
	 if(F->pom!=NULL && Form_parametry->Visible)//pokud je voláno z PO
	 {
		 pom=new Cvektory::TObjekt;
		 double jednotky_cas=60.0;double jednotky_vzdalenost=1000.0;
		 pom->name=Form_parametry->scGPEdit_name->Text;scLabel_titulek->Caption=pom->name.UpperCase()+" - náhled";
		 pom->short_name=Form_parametry->scGPEdit_shortname->Text;
		 pom->pohon=F->d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex);//indexuje se od nuly,ale pohony od 1 (nicménì novì je na prvním místì položka nepøiøazen), pokud pohon neexituje vráti null
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
		 //if(pom->pohon!=NULL && pom->rezim==1)pom->pohon->aRD=pom->RD;//uloží i aktulání rychlost pohonu
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
			 //pom=F->pom; netøeba probíhá pøímo v plnìní gapa formu
	 }
	 //nová výchozí pozice po rotaci vozíku
	 START_POZICE();

	 //povolení zobrazení animace
	 if(pom->rezim==1)ButtonPLAY->Visible=true;else ButtonPLAY->Visible=false;//pouze v kontinuálním režimu je možná animace

	 //Nelze zobrazit náhled objektu s více jak s MAX_pozic pozicemi, bude zobrazen pouze ilustrativní náhled
	 if(pom->pozice>MAX_pozic)
	 {
		 scGPLabel_info->Caption="Nelze zobrazit náhled objektu s více jak s "+AnsiString(MAX_pozic)+" pozicemi.";
		 scGPLabel_info->Left=Width-scGPLabel_info->Width-10;
		 scGPLabel_info->Visible=true;
		 PREVIEW();
	 }
}
//---------------------------------------------------------------------------
//nová výchozí pozice po rotaci vozíku
void TForm_objekt_nahled::START_POZICE()
{
	if(Timer_animace->Enabled==false)//zmìna pozice nazákladì rotace je možná jen a pouze pokud nebìží animace
	Poffset=F->m.UDV(pom->rotace)/2;//slouží pro pøípad rotace vozíku//celý vozík na zaèátku kabiny pùv. 0 - to by bylo z pùlky vozíku,tzn. v momentu aktivnihé palce
}
//---------------------------------------------------------------------------
//odchod z okna a to i pøi stisku tlaèítka OK
void __fastcall TForm_objekt_nahled::KonecClick(TObject *Sender)
{
	if(!nahledZmodelu)
	{
		Timer_animace->Enabled=false;
		scGPLabel_info->Visible=false;
		F->Zoom=Zoom_predchozi;//návrat do pùvodního stavu
		F->nahled_objektu=false;//uchovává stav, zda se jedná o náhled objekt èi regulerní zobrazení ve form1
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
		scGPGlyphButton_info->Visible=false;//workaround kvùli poøadí
		zobrazitFrameForm=false;
	}
	else
	{
		Konec->Visible=false;//workaround kvùli poøadí
		Max_Min_Button->Visible=false;//workaround kvùli poøadí
		scGPGlyphButton_info->Visible=false;//workaround kvùli poøadí
		reposition_windowButtonClick(Sender);//volá porovnání oken
		Max_Min_Button->GlyphOptions->Kind=scgpbgkMaximize;
		zobrazitFrameForm=true;
	}
	//nutný pøedcházející refresh Form1
	F->nahled_objektu=false;
	F->Zoom=Zoom_predchozi;
	F->REFRESH();
	F->nahled_objektu=true;
	//refresh a pøepoèítání mìøítka  Form_objekt_nahled
	ENTIRE();//cely_nahled, zajistí výpoèet zoomu tak, aby se objekt zobrazil pøes celé okno, ale vèetnì okraje Ox
	REFRESH();
}
//---------------------------------------------------------------------------
//pøeorganizuje okna vedle sebe náhled kabiny a form PO
void __fastcall TForm_objekt_nahled::reposition_windowButtonClick(TObject *Sender)
{
	Width=737;Height=423;Left=5;Top=Form_parametry->Top;
	Form_parametry->Left=Left+Form_objekt_nahled->Width+5;
	scGPGlyphButton_info->Visible=true;//workaround kvùli poøadí
	reposition_windowButton->Visible=true;//workaround kvùli poøadí
	Max_Min_Button->Visible=true;//workaround kvùli poøadí
	Konec->Visible=true;//workaround kvùli poøadí
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajistí pøiblížení obrazu
void __fastcall TForm_objekt_nahled::GlyphButton_priblizitClick(TObject *Sender)
{
	F->ZOOM_IN();PopUPmenu->Visible=false;REFRESH();
}
//---------------------------------------------------------------------------
//zajistí oddálení obrazu
void __fastcall TForm_objekt_nahled::GlyphButton_oddalitClick(TObject *Sender)
{
	F->ZOOM_OUT();PopUPmenu->Visible=false;REFRESH();
}
//---------------------------------------------------------------------------
//zajistí celý náhled
void __fastcall TForm_objekt_nahled::GlyphButton_cely_pohledClick(TObject *Sender)
{
	ENTIRE();//cely_nahled
	PopUPmenu->Visible=false;
	REFRESH();
}
//---------------------------------------------------------------------------
//ukonèení pop-up menu
void __fastcall TForm_objekt_nahled::GlyphButton_closeClick(TObject *Sender)
{
	PopUPmenu->Visible=false;
}
//---------------------------------------------------------------------------
//pøepne do ilustrativního náhledu s popisky
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
	if(Timer_animace->Enabled)//bìží animace
	{
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPause;
		ButtonPLAY->Hint="zastavit animaci";
		ButtonPLAY->ShowCaption=true;
		Timer_animace->Interval=F->m.round(F->m.get_timePERpx(pom->RD,0));//stejná rychlost pro všechny RD
		//Timer_animace->Interval=ceil(F->m.get_timePERpx(pom->RD,0,F->d.v.vrat_min_rychlost_prejezdu()));//rùzná rychlost dle RD, s afps se poèítá dle min RD, ale nìjak špatnì vycházela animace ke konci (nestihl vozík vyjet)
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
	Poffset+=1*F->m2px/F->Zoom;//zajistí posun animace o 1px (tedy nejmenší možnou grafickou jednotku), ale posouvání probíhá v metrech

	double Z=1;if(F->antialiasing)Z=3.0;

	//výpoèet aktuálního èasu
	double Time=F->m.round(++timerTakt*F->afps/Z*Timer_animace->Interval/1000.0);//chybná úvaha, koncipováno pouze pro rychlosti 1m/s a vyšší//vrátí èas, tak aby se jednalo o kontinální animaci

	//vypisuje aktuální CT
	if(Form_parametry->CTunit==0)ButtonPLAY->Caption=AnsiString(F->m.round(Time))+" [s]";
	else ButtonPLAY->Caption=AnsiString(F->m.round2double(Time/60.0,2))+" [min]";

	//zastaví animaci po dovršení CT
	if(Time>=pom->CT)//zastaví animaci, jak vypršel CT
	{
		ButtonPLAYClick(Sender);
		START_POZICE();//výchozí pozice vozíkù
		if(Form_parametry->CTunit==0)ButtonPLAY->Caption="0 [s]";
		else ButtonPLAY->Caption="0 [min]";
		timerTakt=0;//vynuluje èas do výchozí hodnoty
		zobrazitFrameForm=true;
	}
	REFRESH_DATA();//naète aktuální data (umožòuje tedy i bìhem animace mìnit za bìhu parametry - možná blbost),zjistí vhodné mìøítka a na závìr REFRESHNE obraz
}
//---------------------------------------------------------------------------
void __fastcall TForm_objekt_nahled::Button1Click(TObject *Sender)
{
		if(1/F->fps*1000<Timer_animace->Interval)ShowMessage("Animace nebude vykreslena kontinuálnì èi zcela kontinuálnì!");
		else ShowMessage("Animace by mìla být vykreslena kontinuálnì");

//ShowMessage(F->m2px/F->Zoom);
//ShowMessage(F->Zoom);
ShowMessage(Timer_animace->Interval);
//Timer_animace->Interval=3.838541666666875;
}
//---------------------------------------------------------------------------



