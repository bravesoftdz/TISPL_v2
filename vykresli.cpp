//---------------------------------------------------------------------------
#pragma hdrstop
#include "vykresli.h"
#include "Unit1.h"
#include "kabina_schema.h"
#include "stdlib.h"
//--------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//konstruktor
Cvykresli::Cvykresli()
{
	//parametry vykreslovaného obdelníku technologického objektu
	O_width=50;
	O_height=40;
	V_width=10;//parametry vykreslované spojky či vyhýbky
	//měřítko PX na MIN, globální proměná i pro využítí výpisu ve SB v Unit1
	PX2MIN=30.0;
	KrokY=30;//vizuální rozteč na ose Y mezi jednotlivými vozíky
	mod_vytizenost_objektu=false;
	NOLIEX=2;
	oY=5;//ofset na ose Y, 5 pouze grafická korekce
	sizeP=3.3;//velikost textů popisků elementů v knihovně
	grafickeDilema=true;
	Pom_proces=new Cvektory::TProces;
	precision=2;//počet desetinných míst čísel na časové ose
	Robot_sirka_zakladny=1.0;
	Robot_delka_zakladny=1.2;
	DoSkRB=(1.2+Robot_sirka_zakladny/2.0);//delka od středu (X,Y bodu) robota k referenčnímu bodu robota (tj. k trysce) v metrech
	DkRB=0.8;//délka k referenčnímu bodu od uchopovacího bodu, respektive odsazení člověka od linky
	clStenaHaly=m.clIntensive(clBlue,100);
	clStenaKabiny=m.clIntensive(clRed,180);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//stav: -3 kurzor, -2 normal (implicitně), -1-highlight bez editace, 0-editace zvýrazní všechny body, 1-až počet bodů zvýraznění daného bodu,počet bodů+1 zvýraznění dané hrany včetně sousedícícíh úchopů (např. pro polygono o 6 bodech) bude hodnota stavu 7 zvýraznění první hrany (od bodu 1 do bodu 2)
void Cvykresli::vykresli_halu(TCanvas *canv,int stav)
{
	//změny stavů
	stav=-2;//defaultní stav
	if(F->MOD==F->SCHEMA)//ošetření, tato metoda se spouští i při náhledu !!!!
	{
    if(F->Akce==F->MOVE_HALA)stav=0;
		if(F->JID==0)stav=F->pom_bod->n;//body
		if(F->JID==1&&F->pom_bod->n==1)stav=2*v.HALA.body->predchozi->n;//poslední úsečka
		if(F->JID==1&&F->pom_bod->n!=1)stav=v.HALA.body->predchozi->n+F->pom_bod->n-1;//ostatní úsečky
	}
	//nastavení kót
	bool zobrazit_koty=true;
	if(F->MOD==F->NAHLED)zobrazit_koty=false;
	//vykreslení
	short sirka_steny_px=m.m2px(0.4);//m->px
	polygon(canv,v.HALA.body,clStenaHaly,sirka_steny_px,stav,zobrazit_koty);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vrátí souřadnice dle typu buď středové nebo excentrické v podobě levého horního rohu objektu
int Cvykresli::CorEx(Cvektory::TObjekt *O)
{
	if(O->id!=(unsigned)F->VyID&&O->id!=pocet_objektu_knihovny+1)//nejde o výhybku a spojku
	return m.L2Px(O->X)+O_width*Form1->Zoom/2;
	else//jde o výhybku
	return m.L2Px(O->X);
}
//---------------------------------------------------------------------------
//vrátí souřadnice dle typu buď středové nebo excentrické v podobě levého horního rohu objektu
int Cvykresli::CorEy(Cvektory::TObjekt *O)
{
	if(O->id!=(unsigned)F->VyID&&O->id!=pocet_objektu_knihovny+1)//nejde o výhybku a spojku
	return m.L2Py(O->Y)+O_height*Form1->Zoom/2;
	else//jde o výhybku
	return m.L2Py(O->Y);
}
//---------------------------------------------------------------------------
//vrátí referenční logické (v metrech) souřadnice  robota (tzn. bod v místě trysky), převede dle aktuální rotace symbolu a uchopovacích (skutečných) souřadnic robota, kontroluje, zda se jedná skutečně o Robota
TPointD Cvykresli::Rxy(Cvektory::TElement *Element)
{
	TPointD RET; RET.x=Element->X; RET.y=Element->Y;
	if(1<=Element->eID && Element->eID<=4 || 7<=Element->eID && Element->eID<=18 || 101<=Element->eID && Element->eID<=108)//ovlivní patřičně pouze roboty
	{
		switch(Element->orientace)
		{
			case 0:		RET.y=Element->Y+DoSkRB;break;
			case 90:	RET.x=Element->X+DoSkRB;break;
			case 180:	RET.y=Element->Y-DoSkRB;break;
			case 270:	RET.x=Element->X-DoSkRB;break;
		}
	}
	if(101<=Element->eID && Element->eID<=108)//ovlivní patřičně pouze lidské roboty
	{
		switch(Element->orientace)
		{
			case 0:		RET.y=Element->Y+DkRB;break;
			case 90:	RET.x=Element->X+DkRB;break;
			case 180:	RET.y=Element->Y-DkRB;break;
			case 270:	RET.x=Element->X-DkRB;break;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
//vykreslí zakázky, cesty, spojnice, kabiny, pohony, elementy
void Cvykresli::vykresli_vektory(TCanvas *canv)
{
	F->Z("",false);//smazání přechozích zpráv
	//cesty ZAKAZeK - jsou li k dispozici
	if(v.ZAKAZKY!=NULL && v.ZAKAZKY->predchozi->n>0)
	{
		 Cvektory::TZakazka *Z=v.ZAKAZKY->dalsi;
		 while(Z!=NULL)//prochází seznam ZAKÁZEK, který obsahuje jednotlivé cesty
		 {
				Cvektory::TCesta *C=Z->cesta->dalsi;
				while(C!=NULL)//prochází jednotlivé prvky cesty
				{
					canv->Pen->Color=Z->barva;
					canv->Pen->Style=psSolid;
					canv->Pen->Width=m.round(2*Form1->Zoom);
					if(Z->barva==clGray)canv->Pen->Mode=pmCopy;else canv->Pen->Mode=pmNotXor;pmMask; //pokud se jedná o defaultní barvu vykopíruje se jina se vytvoří kombinace
					if(C->objekt->n==1)//pro situaci: z posledního prvku na první
					{
						//if(ukaz->n!=ukaz->predchozi->predchozi->n)//pokud jsou minimálně dva prky, ale šipka bude obousměrnná - možná žádoucí
						if(v.OBJEKTY->predchozi->n>=3)//až budou alespoň tři prvky,tj. poslední prvek bude mít index n větší než 3
						{
							canv->MoveTo(m.L2Px(C->predchozi->predchozi->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->predchozi->predchozi->objekt->Y)+O_height*Form1->Zoom/2);
							canv->LineTo(m.L2Px(C->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->objekt->Y)+O_height*Form1->Zoom/2);
							sipka(canv,m.L2Px((C->predchozi->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->predchozi->objekt->X,C->predchozi->predchozi->objekt->Y,C->objekt->X,C->objekt->Y));//zajistí vykreslení šipky - orientace spojovací linie
							if(Z->barva!=clGray)prislusnost_cesty(canv,Z->barva,m.L2Px((C->predchozi->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->predchozi->objekt->X,C->predchozi->predchozi->objekt->Y,C->objekt->X,C->objekt->Y),Z->n);
						}
					}
					else//pro běžné situace
					{
						canv->MoveTo(m.L2Px(C->predchozi->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->predchozi->objekt->Y)+O_height*Form1->Zoom/2);
						canv->LineTo(m.L2Px(C->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->objekt->Y)+O_height*Form1->Zoom/2);
						sipka(canv,m.L2Px((C->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->objekt->X,C->predchozi->objekt->Y,C->objekt->X,C->objekt->Y));//zajistí vykreslení šipky - orientace spojovací linie
						if(Z->barva!=clGray)prislusnost_cesty(canv,Z->barva,m.L2Px((C->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->objekt->X,C->predchozi->objekt->Y,C->objekt->X,C->objekt->Y),Z->n);
					}
					C=C->dalsi;
				}
				Z=Z->dalsi;
		 }
	}
	else //pokud nejsou k dispozici nadefinované cesty vykreslí se přímo jen spojovací linie mezi objekty (tj. defaultní cesta)
	{
		int n;//číslo vyhybky nebo spojky
		TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
		Cvektory::TObjekt* ukaz=v.OBJEKTY->dalsi;//přeskočí hlavičku
		while (ukaz!=NULL)
		{
			Cvektory::TObjekt *O=F->d.v.dalsi_krok(ukaz,tab_pruchodu);//přepínání kroků v cyklu (dalsi/dalsi2),zde na začátku z důvodu potřeby tab_průchodů při vykreslění
			canv->Pen->Style=psSolid;
			canv->Pen->Mode=pmCopy;  //bude tady 2-3
			canv->Pen->Width=m.round(1*F->Zoom);//musí být tady, jina to přebije nastavení metody sipka
			canv->Pen->Color=clGray;//bude tady clBlack
			if(ukaz->n==1)//pro situaci z posledního prvku na první
			{
				//if(ukaz->n!=ukaz->predchozi->predchozi->n)//pokud jsou minimálně dva prky, ale šipka bude obousměrnná - možná žádoucí
				if(v.OBJEKTY->predchozi->n>=3)//až budou alespoň tři prvky,tj. poslední prvek bude mít index n větší než 3
				{
					Cvektory::TObjekt *pom=ukaz->predchozi->predchozi;//pomocný ukazatel, který uchovává předchozí objekt
					if(ukaz->id==pocet_objektu_knihovny+1)//pokud jsem na spojce poprvé musí dojít k vykrelení spojnice k predchozi2 objektu, pokud podruhé dojde k vykreslení spojnice k predchozi objektu
					{
						n=F->ms.MyToDouble(ukaz->short_name.SubString(2,1));//extrakce pořadového čísla výhybky;
						if(tab_pruchodu[n].y==1)pom=ukaz->predchozi2->predchozi;
						else pom=ukaz->predchozi->predchozi;
					}
					canv->MoveTo(m.L2Px(pom->elementy->predchozi->geo.X4),m.L2Py(pom->elementy->predchozi->geo.Y4));
					canv->LineTo(m.L2Px(ukaz->elementy->dalsi->geo.X1),m.L2Py(ukaz->elementy->dalsi->geo.Y1));
					sipka(canv,(m.L2Px(pom->elementy->predchozi->geo.X4)+m.L2Px(ukaz->elementy->dalsi->geo.X1))/2.0,(m.L2Py(pom->elementy->predchozi->geo.Y4)+m.L2Py(ukaz->elementy->dalsi->geo.Y1))/2.0,m.azimut(m.L2Px(ukaz->elementy->dalsi->geo.X1),m.L2Py(ukaz->elementy->dalsi->geo.Y1),m.L2Px(pom->elementy->predchozi->geo.X4),m.L2Py(pom->elementy->predchozi->geo.Y4))*(-1));//zajistí vykreslení šipky - orientace spojovací linie
					pom=NULL;delete pom;
				}
			}
			else
			{
				Cvektory::TObjekt *pom=ukaz->predchozi;//pomocný ukazatel, který uchovává předchozí objekt
				if(ukaz->id==pocet_objektu_knihovny+1)//pokud jsem na spojce poprvé musí dojít k vykrelení spojnice k predchozi2 objektu, pokud podruhé dojde k vykreslení spojnice k predchozi objektu
				{
					n=F->ms.MyToDouble(ukaz->short_name.SubString(2,1));//extrakce pořadového čísla spojky
					if(tab_pruchodu[n].y==1)pom=ukaz->predchozi2;
					else pom=ukaz->predchozi;
				}
				canv->MoveTo(m.L2Px(pom->elementy->predchozi->geo.X4),m.L2Py(pom->elementy->predchozi->geo.Y4));
				canv->LineTo(m.L2Px(ukaz->elementy->dalsi->geo.X1),m.L2Py(ukaz->elementy->dalsi->geo.Y1));
				sipka(canv,(m.L2Px(pom->elementy->predchozi->geo.X4)+m.L2Px(ukaz->elementy->dalsi->geo.X1))/2.0,(m.L2Py(pom->elementy->predchozi->geo.Y4)+m.L2Py(ukaz->elementy->dalsi->geo.Y1))/2.0,m.azimut(m.L2Px(ukaz->elementy->dalsi->geo.X1),m.L2Py(ukaz->elementy->dalsi->geo.Y1),m.L2Px(pom->elementy->predchozi->geo.X4),m.L2Py(pom->elementy->predchozi->geo.Y4))*(-1));//zajistí vykreslení šipky - orientace spojovací linie
				pom=NULL;delete pom;
			}
			ukaz=O;
			O=NULL;delete O;
		}
		tab_pruchodu=NULL;delete tab_pruchodu;
	}
	////OBJEKTY
	//samotné objekty, kreslím až v samostatném následujícím cyklu, aby se nakreslilo do horní vrstvy
	TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if(F->pom_temp!=NULL && F->pom_temp->n==O->n)O=F->pom_temp;//slouží pro vykreslení aktuálně editované kabiny
		vykresli_objekt(canv,O);
		////vykreslení elementů
		short stav=1;
		Cvektory::TElement *E=O->elementy;
		if(F->pom_temp!=NULL&&F->pom_temp->n==O->n){stav=1;E=F->pom_temp->elementy;}//elementy v aktivním objektu
		else stav=-1;//disabled elementy ostatních objektů
		while(E!=NULL)//pokud elementy existují
		{
			if(E->n>0)
			{
				vykresli_element(canv,m.L2Px(E->X),m.L2Py(E->Y),E->name,E->short_name,E->eID,1,E->orientace,stav,E->LO1,E->OTOC_delka,E->LO2,E->LO_pozice);
				E->citelna_oblast.rect3=aktOblast;//uložení citelné oblasti pro další použití
				//vykreslení kót
				if(F->pom_temp!=NULL && F->pom_temp->n==O->n && F->pom_temp->zobrazit_koty && E->eID!=MaxInt){vykresli_kotu(canv,E->predchozi,E);}//mezi elementy
			}
			//zde bude ještě vykreslení g_elementu
			E=E->dalsi;//posun na další element
		}
		E=NULL;delete E;
		if(F->pom_temp!=NULL && F->pom_temp->n==O->n)O=v.vrat_objekt(O->n);//pokud byl objekt nahrazen pom_temp, musí dojít k jeho vrácení, pom_temp->dalsi != Objekty->dalsi
		O=F->d.v.dalsi_krok(O,tab_pruchodu);//přepínání kroků v cyklu (dalsi/dalsi2)
	}
	//povolení zobrazování LAYOUTU a ČASOVÝCH OS, pokud existují objekty, jinak ne
//	if(v.OBJEKTY->dalsi!=NULL && !Form1->TZF)
//	{
//		if(v.OBJEKTY->predchozi->n>3)Form1->Layout->Enabled=true;else Form1->Layout->Enabled=false;///pokud je více jak 3 objekty
//		if(DEBUG)Form1->Layout->Enabled=true;
//		Form1->Analyza->Enabled=true;
//	}
//	else
//	{
//		Form1->Layout->Enabled=false;
//		Form1->Analyza->Enabled=false;
//	}
	if(F->scHTMLLabel_log_vypis->Caption=="")       //toto budeme rušit
	F->Z("<b>Linka v pořádku.</b>",false);
	O=NULL;delete O;
	tab_pruchodu=NULL;delete tab_pruchodu;
}
//---------------------------------------------------------------------------
//zajišťuje vykreslení pouze obrysu dle typu objektu
void Cvykresli::vykresli_kabinu(TCanvas *canv,Cvektory::TObjekt *O,int stav,bool zobrazit_koty)
{
	////vstupní proměnné
	TColor clAkt=clStenaKabiny;
	double orientace=O->orientace; //něco s tím udělat!!!! short->double
	long X1=m.L2Px(O->body->dalsi->X);
	long Y1=m.L2Py(O->body->dalsi->Y);
	long X2=m.L2Px(O->body->dalsi->dalsi->X);
	long Y2=m.L2Py(O->body->predchozi->Y);
	if(orientace==0 || orientace==180)
	{
		X2=m.L2Px(O->body->predchozi->X);
		Y2=m.L2Py(O->body->predchozi->predchozi->Y);
	}
	short sirka_steny_px=m.m2px(O->sirka_steny);//m->px
	short W=0;//smazat m.round(sirka_steny_px/2.0);//posunutí vykreslení orámování nad vnější rozměry kabiny
	short pmpp=m.m2px(v.PP.delka_jig); if(v.PP.delka_jig<v.PP.sirka_jig)pmpp=m.m2px(v.PP.sirka_jig);pmpp=m.round(pmpp/2.0);if(pmpp>m.m2px(1))pmpp=m.m2px(1);/*ošetření*///polovina max. průjezdního profilu
	//nastavení zobrazení, rozdíl mezi Layoutem a editaci, editovaným objektem a ostatnímy
	if(F->pom_temp!=NULL){if(F->pom_temp->n==O->n){stav=-2;if(F->pom_temp->zobrazit_koty)zobrazit_koty=true;else zobrazit_koty=false;}else {stav=-1;zobrazit_koty=false;}}
	else {stav=-2;zobrazit_koty=false;}
  //highlight polygonu (editace rozmerů, bodů)
	if(F->pom_temp!=NULL && F->pom_temp->n==O->n && F->pom_bod!=NULL)
	switch(F->JID)
	{
		//highlight úsečky
		case -2:if(F->pom_bod->n==1)stav=O->body->predchozi->n*2;else stav=O->body->predchozi->n+F->pom_bod->n-1;break;
    //highlight bodu
		case -3:stav=F->pom_bod->n;break;
	}

  ////vykreslení prozatimní osy POHONU
	vykresli_retez(canv,O);

	////vnější obrys kabiny
	polygon(canv,O->body,clAkt,sirka_steny_px,stav,zobrazit_koty);//nové vykreslování příprava

	///název
	//název objektu - nastavení                 //záměrně nuly, aby se ztučněním nepřepozivávalo - působilo to moc dynamacky
	nastavit_text_popisu_objektu_v_nahledu(canv,0);AnsiString Tn=O->name.UpperCase();AnsiString Tl=+" / ";short Wn=canv->TextWidth(Tn);
  ////poloha nadpisu
	double X=O->Xt;
	double Y=O->Yt;
	switch((int)orientace)
	{
		case 0:X=m.L2Px(X)-canv->TextHeight(Tl);Y=m.L2Py(Y)+m.round((Wn)/2.0);canv->Font->Orientation=(orientace+90)*10;break;//nastavení rotace canvasu
		case 90:X=m.L2Px(X)-m.round((Wn)/2.0);Y=m.L2Py(Y)-canv->TextHeight(Tl);break;
		case 180:X=m.L2Px(X)+canv->TextHeight(Tl);Y=m.L2Py(Y)-m.round((Wn)/2.0);canv->Font->Orientation=(orientace+90)*10;break;//nastavení rotace canvasu
		case 270:X=m.L2Px(X)-m.round((Wn)/2.0);Y=m.L2Py(Y)-canv->TextHeight(Tl);break;
	}
	//nastavení highlight textu
	if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID==-6 || F->JID==-7))canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
	else canv->Font->Style = TFontStyles();//vypnutí
	//samotné vypsání názvu
	nastavit_text_popisu_objektu_v_nahledu(canv,1);
	TextFraming(canv,X,Y,Tn);//záměrně Tl,aby se ztučněním nepřepozivávalo - působilo to moc dynamacky
	canv->Font->Orientation=0;//vrácení původní hodnoty rotace canvasu
	//vykreslení uchopovacího kříže u textu
	if(F->pom_temp!=NULL &&(F->JID==-6 || F->JID==-7))
	{
		line(canv,m.L2Px(F->pom_temp->Xt)-m.round(Wn/2.0),m.L2Py(F->pom_temp->Yt)-canv->TextHeight(Tn)+20,m.L2Px(F->pom_temp->Xt)-m.round(Wn/2.0)-40,m.L2Py(F->pom_temp->Yt)-canv->TextHeight(Tn)+20);
		line(canv,m.L2Px(F->pom_temp->Xt)-m.round(Wn/2.0)-20,m.L2Py(F->pom_temp->Yt)-canv->TextHeight(Tn),m.L2Px(F->pom_temp->Xt)-m.round(Wn/2.0)-20,m.L2Py(F->pom_temp->Yt)-canv->TextHeight(Tn)+40);
	}
	//vykreslení kříže posuvu u tabulky pohonu, natrvalo
	if(F->pom_temp!=NULL && F->PmG->Highlight)
	{
		line(canv,m.L2Px(F->pom_temp->Xp)-20,m.L2Py(F->pom_temp->Yp)-20,m.L2Px(F->pom_temp->Xp)-60,m.L2Py(F->pom_temp->Yp)-20);
		line(canv,m.L2Px(F->pom_temp->Xp)-40,m.L2Py(F->pom_temp->Yp),m.L2Px(F->pom_temp->Xp)-40,m.L2Py(F->pom_temp->Yp)-40);
	}

	short highlight=0;//nastavování zda mají být koty highlightovány

	////vykreslení komor u POW - pokud je objekt obsahuje, poslední komora má vždy velikost do konce objektu (nehledě na její skutečné délku), stav, kdy začíná komora za objektem, je nutné ošetřit separátně
	if(O->komora!=NULL && O->komora->predchozi->n>0)
	{
		Cvektory::TKomora *K=O->komora->dalsi;//přeskočí hlavičku
		double vzdalenost=0;
		while(K->dalsi!=NULL)
		{
			if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0 && F->pom_komora->n==K->n))clAkt=m.clIntensive(clStenaKabiny,-50);//highlight
			else clAkt=clStenaKabiny;
			set_pen(canv,clAkt,sirka_steny_px,PS_ENDCAP_SQUARE);
			vzdalenost+=K->velikost;//dle velikosti předchozích komor uchovává hodnotu součtu/pozice aktuálně vykreslované komory
			short W1=0;if(K->n==1)W1=W;//pro první komoru odsazeni
			if(orientace==90 || orientace==270)
			{
      	//vykreslení přepážek
				long X=X1+m.m2px(vzdalenost);
				if(orientace==270)X=X1-m.m2px(vzdalenost);
				if(orientace==270 && pmpp>0)pmpp*=-1;
				long Y=(Y1+Y2)/2.0;
				line(canv,X,Y1,X,Y-pmpp);
				line(canv,X,Y2,X,Y+pmpp);
				//highlight komory
				if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0 && F->pom_komora->n==K->n))
				{
					double hl_X=0;
					if(orientace==90)hl_X=X-m.m2px(K->velikost)-W1;else hl_X=X+m.m2px(K->velikost)-W1;
					canv->MoveTo(X,Y1-W);
					canv->LineTo(hl_X,Y1-W);
					if(K->n!=1)//mimo první komory
					{
						canv->LineTo(hl_X,Y-pmpp);
						canv->MoveTo(hl_X,Y+pmpp);
					}
					canv->LineTo(hl_X,Y2+W);
					canv->LineTo(X,Y2+W);
				}
				//symbolika "sprchy"
				if(K->typ==1)vykresli_pow_sprchu(canv,X,X,Y1,Y2,m.m2px(K->velikost),clAkt,sirka_steny_px/4,pmpp,0,orientace);
			}
			else
			{
				//vykreslení přepážek
				long X=(X1+X2)/2.0;
				long Y=Y1-m.m2px(vzdalenost);
				if(orientace==180)Y=Y1+m.m2px(vzdalenost);
				if(orientace==180 && pmpp>0)pmpp*=-1;
				line(canv,X1,Y,X-pmpp,Y);
				line(canv,X2,Y,X+pmpp,Y);
				//highlight komory
				if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0 && F->pom_komora->n==K->n))//highlight komory
				{
					double hl_Y=0;
					if(orientace==180)hl_Y=Y-m.m2px(K->velikost)-W1;else hl_Y=Y+m.m2px(K->velikost)-W1;
					canv->MoveTo(X1-W,Y);
					canv->LineTo(X1-W,hl_Y);
					if(K->n!=1)
					{
						canv->LineTo(X-pmpp,hl_Y);
						canv->MoveTo(X+pmpp,hl_Y);
					}
					canv->LineTo(X2+W,hl_Y);
					canv->LineTo(X2+W,Y);
				}
				//symbolika "sprchy"
				if(K->typ==1)vykresli_pow_sprchu(canv,X1,X2,Y,Y,m.m2px(K->velikost),clAkt,sirka_steny_px/4,pmpp,0,orientace);
			}
			//KÓTY
			if(zobrazit_koty && F->pom_temp->n==O->n)
			{
				//nastavení highlight
				if((F->JID==0&&F->pom_komora->n==K->n) || (F->JID*(-1)-10==(signed)K->n || F->JID*(-1)-10==(signed)K->predchozi->n)&&F->d.v.PtInKota_komory(O,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)==-1)highlight=2;
				else if(F->JID*(-1)-10==(signed)K->n || F->JID>=11 && F->JID<=99)highlight=1;else highlight=0;
				//vykreslení kót komor
				switch((int)orientace)
				{
					case 0:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost-K->velikost,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost,NULL,O->koty_elementu_offset,highlight,0.2,clGray,K);break;
					case 90:vykresli_kotu(canv,m.P2Lx(X1)+vzdalenost-K->velikost,m.P2Ly(Y2),m.P2Lx(X1)+vzdalenost,m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,K);break;
					case 180:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost+K->velikost,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost,NULL,O->koty_elementu_offset,highlight,0.2,clGray,K);break;
					case 270:vykresli_kotu(canv,m.P2Lx(X1)-vzdalenost+K->velikost,m.P2Ly(Y2),m.P2Lx(X1)-vzdalenost,m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,K);break;
				}
			}
			K=K->dalsi;//posun ve spojáku na další prvek
		}
		K=NULL;delete K;
		////poslední komora
		if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)F->pom_temp->komora->predchozi->n || (F->JID==0 && F->pom_komora->n==F->pom_temp->komora->predchozi->n)))clAkt=m.clIntensive(clStenaKabiny,-50);//highlight
		else clAkt=clStenaKabiny;
		set_pen(canv,clAkt,sirka_steny_px,PS_ENDCAP_SQUARE);
		if(orientace==90 || orientace==270)
		{
			if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)F->pom_temp->komora->predchozi->n || (F->JID==0 && F->pom_komora->n==F->pom_temp->komora->predchozi->n)))//highlight komory
			{
				//nastavení proměnných podle orientace
				double hl_X=0;
				if(orientace==90)hl_X=X2-m.m2px(F->pom_temp->komora->predchozi->velikost);else hl_X=X2+m.m2px(F->pom_temp->komora->predchozi->velikost);
				//vykreslení
				canv->MoveTo(X2,Y1-W);
				canv->LineTo(hl_X,Y1-W);
				canv->LineTo(hl_X,(Y1+Y2)/2.0-pmpp);
				canv->MoveTo(hl_X,(Y1+Y2)/2.0+pmpp);
				canv->LineTo(hl_X,Y2+W);
				canv->LineTo(X2,Y2+W);
				canv->LineTo(X2,Y1-W);
			}
			//symbolika tekoucí kapaliny u POW //dodělat po změně souřadnicového modelu
			if(O->komora->predchozi->typ==1)vykresli_pow_sprchu(canv,m.L2Px(O->elementy->dalsi->geo.X4),m.L2Px(O->elementy->predchozi->geo.X4),m.L2Py(O->body->dalsi->Y),m.L2Py(O->body->predchozi->Y),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4,pmpp,0,orientace);
		}
		else
		{
			bool highlight=false;
			if(F->pom_temp!=NULL && F->pom_temp->n==O->n && (F->JID*(-1)-10==(signed)F->pom_temp->komora->predchozi->n || (F->JID==0 && F->pom_komora->n==F->pom_temp->komora->predchozi->n)))//highlight komory
			{
				//nastavení proměnných podle orientace
				highlight=true;
				double hl_Y=0;
				if(orientace==180){hl_Y=Y2-m.m2px(F->pom_temp->komora->predchozi->velikost);pmpp*=-1;}else hl_Y=Y2+m.m2px(F->pom_temp->komora->predchozi->velikost);
				//vykreslení
				canv->MoveTo(X1-W,Y2);
				canv->LineTo(X1-W,hl_Y);
				canv->LineTo((X1+X2)/2.0-pmpp,hl_Y);
				canv->MoveTo((X1+X2)/2.0+pmpp,hl_Y);
				canv->LineTo(X2+W,hl_Y);
				canv->LineTo(X2+W,Y2);
				canv->LineTo(X1-W,Y2);
			}
			//symbolika tekoucí kapaliny u POW //dodělat po změně souřadnicového modelu
			if(O->komora->predchozi->typ==1 && (O->orientace==0 || O->orientace==180 && !highlight))vykresli_pow_sprchu(canv,m.L2Px(O->body->dalsi->X),m.L2Px(O->body->predchozi->X),m.L2Py(O->elementy->predchozi->geo.Y4),m.L2Py(O->elementy->predchozi->geo.Y4),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4,pmpp,0,orientace);
			if(O->komora->predchozi->typ==1 && O->orientace==180 && highlight)vykresli_pow_sprchu(canv,m.L2Px(O->body->predchozi->X),m.L2Px(O->body->dalsi->X),m.L2Py(O->elementy->predchozi->geo.Y4),m.L2Py(O->elementy->predchozi->geo.Y4),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4,pmpp,0,orientace);//z důvodu špatného vykreslení při highlightu
		}
		//vykreslení KÓTY od poslení komory k okraji kabiny
		if(zobrazit_koty && F->pom_temp->n==O->n)
		{
			if((F->JID==0 && F->pom_komora->n==O->komora->predchozi->n) || (F->JID*(-1)-10==(signed)O->komora->predchozi->n || F->JID*(-1)-10==(signed)O->komora->predchozi->predchozi->n)&&F->d.v.PtInKota_komory(O,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)==-1)highlight=2;
			else if(F->JID*(-1)-10==(signed)O->komora->predchozi->n || F->JID>=11&&F->JID<=99)highlight=1;
			else highlight=0;
			//vykreslení kót komor
			switch((int)orientace)
			{
				case 0:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost,m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,O->komora->predchozi);break;
				case 90:vykresli_kotu(canv,m.P2Lx(X1)+vzdalenost,m.P2Ly(Y2),m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,O->komora->predchozi);break;
				case 180:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost,m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,O->komora->predchozi);break;
				case 270:vykresli_kotu(canv,m.P2Lx(X1)-vzdalenost,m.P2Ly(Y2),m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset,highlight,0.2,clGray,O->komora->predchozi);break;
			}
			canv->Brush->Style=bsClear;//navrácení na průhledné pero, kvůli následujícím popiskům objektu, kóty jej totiž změnily
		}
	}
}
//---------------------------------------------------------------------------
//bool name, zda se jedná o zadavání name nebo short_name
void Cvykresli::nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv,unsigned short typ)
{
	canv->Font->Name=F->aFont->Name;
	canv->Font->Color=clRed;
	canv->Font->Size=2*3*F->Zoom;
	canv->Font->Style = TFontStyles();
//	if((F->JID==-6 && typ==1) || (F->JID==-7 && typ==2))canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
//	else canv->Font->Style = TFontStyles();//vypnutí
}
//---------------------------------------------------------------------------
//symbolika tekoucí kapaliny u POW
void Cvykresli::vykresli_pow_sprchu(TCanvas *canv,long X1,long X2,long Y1,long Y2,unsigned int velikost_komory_px,TColor color,short sirka,short pmpp,short typ,double orientace)
{
	if(typ!=-2)set_pen(canv,color,sirka,PS_ENDCAP_SQUARE);//pokud se jedná o typ kurzor, tak se nezobrazuje
	short krok=sirka*8;//pouze zneužití sirka
	if(X1==X2)//pro vodorovnou situaci
	{
		long Xp=X1-velikost_komory_px;//Xp - X předchozí
		if(orientace==270)Xp=X1+velikost_komory_px;
		for(unsigned int i=krok;i<velikost_komory_px;i+=krok)
		{				     //pouze zneužití krok                     //pouze zneužití krok a pmpp
			if(orientace==90 || orientace==-90)//ošetření při nezadání parametru orientace
			{
				line(canv,Xp+krok,Y1,Xp+i,m.round((Y1+Y2)/2.0-pmpp));
				line(canv,Xp+krok,Y2,Xp+i,m.round((Y1+Y2)/2.0+pmpp));
			}
			else
			{
				line(canv,Xp-krok,Y1,Xp-i,m.round((Y1+Y2)/2.0-pmpp));
				line(canv,Xp-krok,Y2,Xp-i,m.round((Y1+Y2)/2.0+pmpp));
      }
		}
	}
	else//pro svislou
	{
		long Yp=Y1-velikost_komory_px;//Xp - X předchozí
		if(orientace==0)Yp=Y1+velikost_komory_px;
		for(unsigned int i=krok;i<velikost_komory_px;i+=krok)
		{				       //pouze zneužití krok                //pouze zneužití krok a pmpp
			if(orientace==0 || orientace==-90)//ošetření při nezadání parametru orientace
			{
				line(canv,X1,Yp-krok,m.round((X1+X2)/2.0-pmpp),Yp-i);
				line(canv,X2,Yp-krok,m.round((X1+X2)/2.0+pmpp),Yp-i);
			}
			else
			{
				line(canv,X1,Yp+krok,m.round((X1+X2)/2.0-pmpp),Yp+i);
				line(canv,X2,Yp+krok,m.round((X1+X2)/2.0+pmpp),Yp+i);
      }
		}
	}
}
//---------------------------------------------------------------------------
//vykreslí barevný čtvereček jako příslušnost k dané cestě
void Cvykresli::prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N)
{
	short S=m.round(4*Form1->Zoom);//šířka čtverečku od středu
	short O=m.round(10*Form1->Zoom);//odsazení od šipky
	canv->Pen->Width=1;
	canv->Pen->Color=Color;
	canv->Pen->Style=psSolid;
	canv->Pen->Mode=pmCopy;//pmNotXor;//pmMask;
	canv->Brush->Color=Color;
	if(0<=A && A<15 || 135<=A && A<250 || 315<A && A<=360)canv->Rectangle(O+X-S+S*2*(N-1),Y-S,O+X+S+S*2*(N-1),Y+S);//vodorovně za
	if(15<=A && A<135 || 250<=A && A<315 )canv->Rectangle(X-S,O+Y-S+S*2*(N-1),X+S,O+Y+S+S*2*(N-1));//svisle pod
}
//---------------------------------------------------------------------------
//zajistí vykreslení šipky - orientace spojovací linie
void Cvykresli::sipka(TCanvas *canv, int X, int Y, float azimut, bool bez_vyplne, float size,COLORREF color,COLORREF color_brush,TPenMode PenMode,TPenStyle PenStyle,bool teziste_stred)
{
	azimut=fmod(azimut,360);//ošetření proti přetýkání azimutu
	canv->Pen->Mode=PenMode;
	canv->Pen->Width=1;
	canv->Pen->Style=PenStyle;
	canv->Pen->Color=(TColor)color;
	canv->Brush->Color=(TColor)color_brush;
	size=m.round(size*2*Form1->Zoom);//bacha může způsobovat problémy
	short sklon=230;

	if(!teziste_stred)//referenční bod není v těžišti,tak posun šipky tak, aby špička byla referenčním bodem
	{
		TPointD R=m.rotace(m.delka(X,Y,m.round(X+m.rotace(1,0,azimut).x*size),m.round(Y+m.rotace(1,0,azimut).y*size)),0,azimut);
		X-=m.round(R.x);
		Y-=m.round(R.y);
	}

	if(!bez_vyplne)//barevná výplň trojúhelníku
	{
		POINT body[3]={{m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size)},{m.round(X+m.rotace(1,0,azimut).x*size),m.round(Y+m.rotace(1,0,azimut).y*size)},{m.round(X+m.rotace(1,360-sklon,azimut).x*size),m.round(Y+m.rotace(1,360-sklon,azimut).y*size)}};
		canv->Polygon((TPoint*)body,2);
	}
	else//transparentní střed
	{
		canv->MoveTo(m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size));
		canv->LineTo(m.round(X+m.rotace(1,0,azimut).x*size),m.round(Y+m.rotace(1,0,azimut).y*size));
		canv->LineTo(m.round(X+m.rotace(1,360-sklon,azimut).x*size),m.round(Y+m.rotace(1,360-sklon,azimut).y*size));
		canv->LineTo(m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size));
	}
}
//---------------------------------------------------------------------------
void Cvykresli::vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *ukaz)
{
	//nová koncepce - metodu vykresli_rectangle - patřičně přejmenovat
	if((long)ukaz->id!=F->VyID&&(long)ukaz->id!=pocet_objektu_knihovny+1)//vykreslování objektů, pro výhybky a spojky zvlášť vykreslení
	{
		vykresli_kabinu(canv,ukaz,-2,false);
		//+doplnit volání vykreslení elementární osy (je na to metoda), metodu volat buď přímo zde nebo jako součásts vykresli_kabinu = porada
	}
	else vykresli_kruh(canv,ukaz);//vykreslování výhybky a spojky zvlášť

//old verze - brzy bude nahrazeno
//	if((long)ukaz->id!=F->VyID&&(long)ukaz->id!=pocet_objektu_knihovny+1)//vykreslování výhybky a spojky zvlášť
//	{
//		//INFO: Zoom_predchozi_AA je v případě nepoužítí AA totožný jako ZOOM
//
//		////referenčni bod jsem nakonce stanovil pravý konec levé packy
//		TPoint S=m.L2P(ukaz->X,ukaz->Y);//Převede logické souřadnice na fyzické (displej zařízení), vrací fyzické souřadnice
//
//		unsigned short W=O_width*Form1->Zoom;
//		unsigned short H=O_height*Form1->Zoom;
//
////		if(Form1->zobrazit_barvy_casovych_rezerv)
////		{
//			TColor errorColor=set_color(canv,ukaz);
//			if(errorColor!=0)
//			{
//			unsigned short O=m.round(6*Form1->Zoom);//Okraj nutno zaokrouhlit tady
//			canv->Rectangle(S.x-O,S.y-O,S.x+W+O,S.y+H+O);
//      }
////		}
//
//		////obdelník objektu
//		canv->Pen->Style=psSolid;
//		canv->Brush->Style=bsSolid;
//		canv->Brush->Color=(TColor)RGB(19,115,169);//(TColor)RGB(254,254,254);//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid
//		canv->Pen->Color=(TColor)RGB(19,115,169);//clBlack;
//		canv->Pen->Mode=pmCopy;
//		canv->Pen->Width=m.round(2*Form1->Zoom);
//		canv->Rectangle(S.x,S.y,S.x+W,S.y+H);
//
//		////packy
//		/*unsigned short packy_W=5*Form1->Zoom;
//		canv->Pen->Width=1*Form1->Zoom;
//		canv->MoveTo(S.x-packy_W,S.y+H/2);canv->LineTo(S.x,S.y+H/2);
//		canv->MoveTo(S.x+W,S.y+H/2);canv->LineTo(S.x+W+packy_W,S.y+H/2);*/
//
//		////nastavení písma
//		canv->Font->Name="Arial";//canv->Font->Name="MS Sans Serif";
//		if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)
//		{
//			canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
//			//asi nepřináší zcela přínos
//			canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
//			canv->Font->Size=11*3+3;//+3 grafická korekce protože při AA dochází ke zmenšení písma
//		}
//		else
//		{
//			//asi nepřináší zcela přínos
//			canv->Font->Pitch = TFontPitch::fpVariable;//každé písmeno fontu stejně široké
//			canv->Font->Pitch = System::Uitypes::TFontPitch::fpVariable;
//			canv->Font->Size=12;//tady zajist rozšíření písma
//		}
//		rotace_textu(canv,0);
//
//		//barva
//		canv->Font->Color=(TColor)RGB(254,254,254);//clBlack;//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid
//
//		short zAA=1;//zvětšení pro antialising, jinak 1
//		if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)zAA=3;
//
//		////samotný text - pro jednotlivé zoomu různé podoby výpisu
//		//název objektu
//		if(Form1->Zoom_predchozi_AA>1)
//		{
//		 canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
//		 if(Form1->Zoom_predchozi_AA==1.5)	drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
//		 else
//		 {
//			canv->Font->Size=canv->Font->Size; //*(Form1->Zoom_predchozi_AA-1) - zatím nepoužito
//			canv->TextOutW(S.x+4*zAA,S.y+2*zAA,ukaz->name.UpperCase());
//		 }
//
//		 canv->Font->Style = TFontStyles();//vypnutí tučného písma
//		 if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)canv->Font->Size=8*3+2;//+3 grafická korekce protože při AA dochází ke zmenšení písma
//		}
//
//		//vypis další datových položek
//		if(Form1->Zoom_predchozi_AA>1.5)
//		{
//		 UnicodeString T=""; unsigned short R=20;//řádkování
//
//		 bool CTunit=0;AnsiString CTunitT="[s]";
//		 bool aRDunit=0;AnsiString aRDunitT="[m/s]";
//		 bool DDunit=0;AnsiString DDunitT="[m]";
//		 if(Form1->readINI("nastaveni_form_parametry", "CT") == "1"){CTunit=1;CTunitT="[min]";}
//		 if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1"){aRDunit=1;aRDunitT="[m/min]";}
//		 if(Form1->readINI("nastaveni_form_parametry", "DD") == "1") {DDunit=1;DDunitT="[mm]";}
//
//		 switch(ukaz->rezim)
//		 {
//			case 0:T="STOP & GO";break;
//			case 1:T="KONTINUÁLNÍ";break;
//			case 2:T="POSTPROCESNÍ";break;
//		 }
//		 canv->TextOutW(S.x+4*zAA,S.y+R*zAA,T);//výpis režimu
//		 if(ukaz->pohon==NULL)
//		 {
//			 canv->Font->Color=clRed;
//			 canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
//			 canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"POHON NEPŘIŘAZEN");//pohon name
//			 canv->Font->Style = TFontStyles();//vypnutí tučného písma
//			 canv->Font->Color=(TColor)RGB(254,254,254);
//		 }
//		 else canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,ukaz->pohon->name);//pohon name
//		 if(F->STATUS!=Form1->OVEROVANI && Form1->Zoom_predchozi_AA>2 && ukaz->pohon!=NULL)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"RP: "+F->m.round2double(ukaz->pohon->aRD*(1+aRDunit*59.0),3,"..")+" "+aRDunitT);
//		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"DD: "+F->m.round2double(ukaz->delka_dopravniku*(1+DDunit*999.0),3,"..")+" "+DDunitT);
//		 if(F->STATUS!=Form1->OVEROVANI && Form1->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"CT: "+F->m.round2double(ukaz->CT/(1+CTunit*59.0),3,"..")+" "+CTunitT);
//		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"Kap.: "+F->m.round2double(ukaz->kapacita,3,"..")+" [v+mz]");
//		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"Poz.: "+F->m.round2double(ukaz->pozice,3,"..")+" [v]");
//		}
//
//		//pro největší oddálení zobrazí jenom zkratku objektu
//		if(Form1->Zoom_predchozi_AA<=1)
//		{
//			if(Form1->Zoom_predchozi_AA==1)canv->Font->Style = TFontStyles()<< fsBold;else canv->Font->Style = TFontStyles();
//			if(Form1->Zoom_predchozi_AA==0.25)
//			drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->short_name.SubString(1,1));
//			else
//			{
//				if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)//při antialiasingu se zobrazuje popisek trochu jinak
//				drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->short_name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
//				else
//				drawRectText(canv,TRect(S.x-1,S.y,S.x+W,S.y+H),ukaz->short_name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
//			}
//		}
//	}
//	else
//	vykresli_kruh(canv,ukaz);
}
////---------------------------------------------------------------------------
void Cvykresli::vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O)
{
		//INFO: Zoom_predchozi_AA je v případě nepoužítí AA totožný jako ZOOM

		////referenčni bod jsem nakonce stanovil pravý konec levé packy
		TPoint S=m.L2P(O->X,O->Y);//Převede logické souřadnice na fyzické (displej zařízení), vrací fyzické souřadnice

		unsigned short W=V_width*Form1->Zoom;

		////obdelník objektu
		canv->Pen->Style=psSolid;
		canv->Brush->Style=bsSolid;
		if(O->id==(unsigned)F->VyID)//dočasné rozlišení výhybek a spojek
		{
		canv->Brush->Color=(TColor)RGB(19,115,169);//(TColor)RGB(254,254,254);//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid
		canv->Pen->Color=(TColor)RGB(19,115,169);}//clBlack;
		if (O->id==pocet_objektu_knihovny+1){
		canv->Brush->Color=clRed;
		canv->Pen->Color=(TColor)RGB(19,115,169);
    }
		canv->Pen->Mode=pmCopy;
		canv->Pen->Width=m.round(2*Form1->Zoom);
		canv->Ellipse(S.x-W,S.y-W,S.x+W,S.y+W);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::drawRectText(TCanvas *canv,TRect Rect,UnicodeString Text)
{
		canv->TextOutW(Rect.Left+Rect.Width()/2-canv->TextWidth(Text)/2,Rect.Top+Rect.Height()/2-canv->TextHeight(Text)/2,Text);
}
////---------------------------------------------------------------------------
//zajistí vykreslení daného textu dle nastaveného Fontu (pokud je NULL, převezme se akutální font canvasu) včetně framingu, který je baravně a velikostně nastavitelný
void Cvykresli::TextFraming(TCanvas *canv,int X,int Y,UnicodeString Text,TFont *Font,TColor clFraming,unsigned short FramingSize)
{
	//pokud není font nastaven, převezme se akutální font, převezme se akutální font canvasu
	if(Font==NULL)Font=canv->Font;else canv->Font=Font;

	//záloha
	TColor clText=Font->Color;

	//nastavení průhledného pozadí písma - důležitée
	canv->Brush->Color=clWhite;
	canv->Brush->Style=bsClear;//nastavení výplně

	//framing
	canv->Font->Color=clFraming;
	for(short i=-1*FramingSize;i<=FramingSize;i++)
	{
		if(i==0)continue;
		canv->TextOutW(X+i,Y+i,Text);
		canv->TextOutW(X+i,Y+i*(-1),Text);
	}

	//samotný text
	canv->Font->Color=clText;
	canv->TextOutW(X,Y,Text);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::vykresli_grid(TCanvas *canv, int size_grid)
{
    if(F->pom_temp!=NULL)size_grid/=10;//zjemnění gridu v případě náhledu, grid po 0,1 m místo po 1m
		//if(Form1->Zoom==1.75 || Form1->Zoom==0.75)bacha u těchto hodnot dochází ke špatnému vykreslování asi zaokrouhlouvaním
		for(int x=m.round((m.round(-1*Form1->Posun.x)%size_grid)*Form1->Zoom);x<=Form1->ClientWidth;x+=m.round(size_grid*Form1->Zoom))
		for(int y=m.round((m.round(-1*Form1->Posun.y)%size_grid)*Form1->Zoom);y<=Form1->ClientHeight;y+=m.round(size_grid*Form1->Zoom))
		{
			canv->Pixels[x][y]=clGray; //při změně barvy nutno provést změnu barevného filtru v podmínce v antialiasingu
		}
}
////---------------------------------------------------------------------------
//void Cvykresli::vykresli_graf_rezervy(TCanvas *canv)
//{
//	 SetBkMode(canv->Handle,OPAQUE);
//	 canv->Font->Size=12;
//	 canv->Font->Name="Arial";
//	 UnicodeString text_horizontal="časová osa procesů";
//	 UnicodeString text_vertical="TAKT TIME [čas na vozík]";
//	 unsigned int L=/*Form1->RzSizePanel_knihovna_objektu->Width+*/canv->TextHeight(text_vertical)+3;
//	 unsigned int P=Form1->ClientWidth-1;
//	 unsigned int D=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-canv->TextHeight(text_horizontal)-1;
//	 unsigned int H=Form1->RzToolbar1->Height;
//	 unsigned int offset_horizont=0;//bylo 20 - nyní nepoužívám, pouze vizuální záležitost na ose X, pro graf, aby nebyl až ke kraji
//	 unsigned int offset_vertical=100;
//	 double akt_max_TTo=0;
//	 bool graficka_legenda=false;
//	 v.get_LT_a_max_min_TT();
//
//	 ////objekty
//		canv->Brush->Style=bsSolid;
//		canv->Font->Color=clWhite;
//		canv->Font->Style = TFontStyles() /*<< fsItalic << fsBold*/;
//		canv->Font->Size=14;
//		rotace_textu(canv,900);
//
//		Cvektory::TObjekt *ukaz;
//		unsigned int sirka_uzita=L;
//		ukaz=v.OBJEKTY->dalsi;//přeskočí hlavičku
//		if(ukaz==NULL){v.MAX_TT=0;v.MIN_TT=0;}//pokud neexistuje žádný objekt, ošetření, aby se nic nezobrazovalo v grafu
//		while (ukaz!=NULL)//pokud existuje nějaký prvek
//		{
//			if(ukaz->TTo>0)//vykreslí, jenom pokud má objekt TTo>0
//			{
//				//sloupce
//				set_color(canv,ukaz->TTo);
//				canv->Pen->Style=psSolid;
//				canv->Brush->Style=bsSolid;
//				int sirka_sloupce=m.round((P-offset_horizont-L)*ukaz->CT/v.LT);
//				int vyska_sloupce=m.round((D-H-offset_vertical)*ukaz->TTo/v.MAX_TT);//pokud bych chtěl rovnoměrně rozdělovat tak jako u sirka_sloupce:unsigned int vyska_sloupce=m.round((D-H-offset_vertical)*ukaz->time/LT);
//				canv->Rectangle(sirka_uzita,D-vyska_sloupce,sirka_uzita+sirka_sloupce,D);
//				//zajistí grafické oddělení sloupců bílá svislá čára
//				canv->Pen->Color=clWhite;
//				canv->Pen->Width=1;
//				canv->Pen->Style=psSolid;
//				canv->MoveTo(sirka_uzita,D-vyska_sloupce);
//				canv->LineTo(sirka_uzita,D);
//
//				////vykreslení doporučeného TTo či buffrování,je-li důvod,tzn. je-li výška slopuce pod TT
//				akt_max_TTo=(ukaz->TTo > akt_max_TTo) ? ukaz->TTo : akt_max_TTo;//přiřadí aktuálně nejvyšší TT
//				if(akt_max_TTo!=ukaz->TTo)graficka_legenda=true;
//				//doporučení zvýšení TTo
//				canv->Brush->Style=bsDiagCross;
//				canv->Rectangle(sirka_uzita,D-vyska_sloupce,sirka_uzita+sirka_sloupce,D-m.round((D-H-offset_vertical)*akt_max_TTo/v.MAX_TT));
//				//doporučení BUFFROVANÍ či STOP STANIC
//				if(ukaz->dalsi!=NULL && akt_max_TTo<ukaz->dalsi->TTo)
//				{
//					canv->Brush->Style=bsCross;
//					canv->Rectangle(sirka_uzita,D-m.round((D-H-offset_vertical)*akt_max_TTo/v.MAX_TT),sirka_uzita+sirka_sloupce,D-m.round((D-H-offset_vertical)*ukaz->dalsi->TTo/v.MAX_TT));
//				}
//
//				/*//popisek
//				if(ukaz->rezim==0)//S&G
//				{
//						//
//				}
//				else//kontinual, dále postprocesní nemusím z principu řešit
//				{
//					 //
//				}
//				////----
//				*/
//				UnicodeString Popisek=ukaz->name.UpperCase();//standardně celý název jinak zkratka, pokud se nevejde do sloupce
//				if(vyska_sloupce<canv->TextWidth(Popisek))Popisek=ukaz->short_name;
//				if(D-vyska_sloupce/2+canv->TextWidth(Popisek)/2<D)//zajišťuje, aby byl popisek vždy nad osou
//					canv->TextOutW((sirka_uzita+sirka_uzita+sirka_sloupce)/2-canv->TextHeight(Popisek)/2,D-vyska_sloupce/2+canv->TextWidth(Popisek)/2,Popisek);//popisek
//				else
//				canv->TextOutW((sirka_uzita+sirka_uzita+sirka_sloupce)/2-canv->TextHeight(Popisek)/2,D-1,Popisek);//popisek
//
//				sirka_uzita+=sirka_sloupce;
//
//			}
//			ukaz=ukaz->dalsi;//posun na další prvek
//		}
//		//vypnutí tučného písma
//		canv->Font->Style = TFontStyles();
//		canv->Font->Size=12;
//	 ////----------
//
//	 ////legenda a osy
//		canv->Pen->Style=psClear;
//		canv->Font->Color=clGray;
//		canv->Font->Size=9;
//		canv->Pen->Color=clGray;
//		canv->Brush->Color=clGray;
//		rotace_textu(canv,0);
//		//grafická legenda
//		if(graficka_legenda)
//		{
//				canv->Brush->Style=bsDiagCross;
//				UnicodeString P1="zvýšit TAKT TIME objektu";
//				UnicodeString P2="zařadit BUFFER/STOP STANICI";
//				unsigned short T_W=(canv->TextWidth(P1)>canv->TextWidth(P2)) ? canv->TextWidth(P1):canv->TextWidth(P2);//přiřadí aktuálně nejvyšší TT
//				unsigned short T_H=canv->TextHeight(P1);
//				canv->TextOutW(P-T_W-2,(H+10+H+20+10)/2-T_H/2,P1);
//				canv->Rectangle(P-50-T_W-4,H+10,P-T_W-4,H+20+10);
//				canv->Brush->Style=bsCross;
//				canv->TextOutW(P-T_W-2,(H+20+20+H+20+20+20)/2-T_H/2,P2);
//				canv->Rectangle(P-50-T_W-4,H+20+20,P-T_W-4,H+20+20+20);
//		}
//
//		//osy a nastavení
//		canv->Pen->Width=1;
//		canv->Pen->Style=psSolid;
//		canv->Brush->Color=clWhite;
//		//vodorovná osa
//		canv->MoveTo(L,D);canv->LineTo(P,D);//linie
//		canv->LineTo(P-10,D-10);canv->MoveTo(P,D);canv->LineTo(P-10,D+10);//šipka
//		canv->TextOutW((L+P)/2-canv->TextWidth(text_horizontal)/2,D+1,text_horizontal);//popisek
//		//svislá osa
//		canv->MoveTo(L,D);canv->LineTo(L,H);//linie
//		canv->LineTo(L-10,H+10);canv->MoveTo(L,H);canv->LineTo(L+10,H+10);//šipka
//		rotace_textu(canv,900);canv->TextOutW(L-1-canv->TextHeight(text_vertical),(D+H)/2+canv->TextWidth(text_vertical)/2,text_vertical);//popisek
//		//další popisky a osy
//		canv->Pen->Style=psDot;
//		rotace_textu(canv,0);
//		UnicodeString T="";
//		//min TTo
//		if(v.MIN_TT!=Form1->PP.TT && v.MAX_TT!=v.MIN_TT )//zobrazí osu s popiskem pouze pokud nebude stejná jako TT a zároveň MAX_TT
//		{
//			canv->MoveTo(L,D-m.round((D-H-offset_vertical)*v.MIN_TT/v.MAX_TT));canv->LineTo(P,D-m.round((D-H-offset_vertical)*v.MIN_TT/v.MAX_TT));
//			T="min. TAKT TIME objektů";
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)*v.MIN_TT/v.MAX_TT-canv->TextHeight(T),T);
//		}
//		//max TTo
//		if(v.MAX_TT!=Form1->PP.TT)//zobrazí osu s popiskem pouze pokud nebude stejná jako TT
//		{
//			canv->MoveTo(L,D-m.round(D-H-offset_vertical));canv->LineTo(P,D-m.round(D-H-offset_vertical));
//			if(v.MAX_TT!=0)T="uskutečnitelný (max.) TAKT TIME objektů";else T="požadovaný TAKT TIME";
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)-canv->TextHeight(T),T);
//			T="požadovaný TAKT TIME";
//		}
//		else T="požadovaný (uskutečnitelný) TAKT TIME";
//		//TT osa
//		canv->Pen->Style=psDash;
//		if(v.MAX_TT>0)
//		{
//			canv->MoveTo(L,D-m.round((D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT));canv->LineTo(P,D-m.round((D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT));
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT-canv->TextHeight(T),T);
//    }
//		//vypis hodnoty LT
//		T="LEAD TIME: "+UnicodeString(v.LT)+" [min]";
//		canv->TextOutW(P-canv->TextWidth(T)-11,D+1,T);
//		//Legenda pravý horní roh
//		unsigned short n=0;
//		canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"požadovaný TAKT TIME: "+UnicodeString(Form1->PP.TT)+" [min/vozík]");
//		canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"uskutečnitelný (max.) TAKT TIME: "+UnicodeString(v.MAX_TT)+" [min/vozík]");
//		if(Form1->PP.TT>0)
//		{
//			//canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"Počet realizovaných produktů v čase:...[ks]");
//			canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"WIP: "+UnicodeString((1/Form1->PP.TT)*v.LT)+" [vozíků]");
//			if(v.LT>0)canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"PCE: "+UnicodeString(m.round2double(v.sum_WT()/v.LT*100,2))+" [%]");//zokrouhleno na dvě desetinná místa
//		}
//		canv->Brush->Style=bsSolid;
//	 ///------
//}
////---------------------------------------------------------------------------
//v případě měření vzdálenosti vykreslí spojnici a popř. vypisuje hodnotu vzdálenosti
void Cvykresli::vykresli_meridlo(TCanvas *canv,int X,int Y,bool kalibracni_sipka)
{
//		bool popisek_napravo=true;
//		if(X<=F->vychozi_souradnice_kurzoru.x)popisek_napravo=false;
//
//		canv->Font->Size=12;
//		canv->Font->Name="Arial";
//		canv->Font->Color=clWhite;
//
//		canv->Brush->Color=clWhite;
//		//canv->Brush->Style=bsClear;
//
//		//délka text předchozí smazání
//		double delka=m.round2double(m.delka(m.P2Lx(F->vychozi_souradnice_kurzoru.X),m.P2Ly(F->vychozi_souradnice_kurzoru.Y),m.P2Lx(F->minule_souradnice_kurzoru.x),m.P2Ly(F->minule_souradnice_kurzoru.y)),2);
//		long Xt=F->minule_souradnice_kurzoru.x;
//		short Z=1;//znamenko
//		if(!popisek_napravo)
//		{
//			Xt=Xt-canv->TextWidth(AnsiString(delka)+" [m]");
//			Z=-1;
//		}
//
//		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");
//
//		canv->TextOutW(Xt+9*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");
//		canv->TextOutW(Xt+11*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");
//		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-9,AnsiString(delka)+" [m]");
//		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-11,AnsiString(delka)+" [m]");
//		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-8,AnsiString(delka)+" [m]");
//
////	 nedoděláno	HRGN hreg_old=CreateRectRgn(Xt+10*Z,F->minule_souradnice_kurzoru.y-10,F->minule_souradnice_kurzoru.x+10+canv->TextWidth(AnsiString(delka)+" [m]  "),F->minule_souradnice_kurzoru.y-10+canv->TextHeight(AnsiString(delka)+" [m]"));
//		delka=m.round2double(m.delka(m.P2Lx(F->vychozi_souradnice_kurzoru.X),m.P2Ly(F->vychozi_souradnice_kurzoru.Y),m.P2Lx(X),m.P2Ly(Y)),2);
//		if(popisek_napravo)Xt=X+10;else Xt=X-10-canv->TextWidth(AnsiString(delka)+" [m]");
////	 nedoděláno	HRGN hreg_new=CreateRectRgn(Xt,Y-10,X+10+canv->TextWidth(AnsiString(delka)+" [m]"),Y-10+canv->TextHeight(AnsiString(delka)+" [m]"));
////		HRGN hreg_diff=CreateRectRgn(0,0,0,0);
////		CombineRgn(hreg_diff,hreg_old,hreg_new,RGN_DIFF);
////		InvalidateRgn(F->Handle,hreg_diff,TRUE);
//		//InvertRgn(canv->Handle,hreg_diff);//označí region)
//
//		//délka text aktuální
//		//canv->Brush->Color=clWhite;
//		canv->Brush->Style=bsClear;
//		canv->Font->Color=m.clIntensive(clRed,100);
//		canv->TextOutW(Xt,Y-10,AnsiString(delka)+" [m]");
													 //zabraňuje nechtěnému úvodnímu efektu
		if(kalibracni_sipka && F->minule_souradnice_kurzoru!=F->vychozi_souradnice_kurzoru)
		{
			double A=m.azimut(F->vychozi_souradnice_kurzoru.x,-F->vychozi_souradnice_kurzoru.y,F->minule_souradnice_kurzoru.x,-F->minule_souradnice_kurzoru.y);
			sipka(canv,(F->vychozi_souradnice_kurzoru.x+F->minule_souradnice_kurzoru.x)/2,(F->vychozi_souradnice_kurzoru.y+F->minule_souradnice_kurzoru.y)/2,A,true,10,m.clIntensive(clRed,100),m.clIntensive(clRed,100),pmNotXor,psSolid);
		}

		//linie
		short W=10;
		if(kalibracni_sipka)W=5;
		set_pen(canv, m.clIntensive(clRed,100),W,PS_ENDCAP_FLAT);
		canv->Pen->Mode=pmNotXor;
		canv->MoveTo(F->vychozi_souradnice_kurzoru.x,F->vychozi_souradnice_kurzoru.y);canv->LineTo(F->minule_souradnice_kurzoru.x,F->minule_souradnice_kurzoru.y);
		F->minule_souradnice_kurzoru=TPoint(X,Y);
		canv->MoveTo(F->vychozi_souradnice_kurzoru.x,F->vychozi_souradnice_kurzoru.y);canv->LineTo(X,Y);
																				//zabraňuje nechtěnému úvodnímu efektu
		if(kalibracni_sipka  && F->minule_souradnice_kurzoru!=F->vychozi_souradnice_kurzoru)
		{
			double A=m.azimut(F->vychozi_souradnice_kurzoru.x,-F->vychozi_souradnice_kurzoru.y,X,-Y);
			sipka(canv,(F->vychozi_souradnice_kurzoru.x+X)/2,(F->vychozi_souradnice_kurzoru.y+Y)/2,A,true,10,m.clIntensive(clRed,100),m.clIntensive(clRed,100),pmNotXor,psSolid);
		}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////celkové vykreslení módu časové osy - MaRO algoritmus
void Cvykresli::vykresli_casove_osy(TCanvas *canv)
{
	if(!mod_vytizenost_objektu)//pokud se bude jednat o zobrazení formou časových os, nikoliv formou vytížení objektů
	{
		//nastavení do výchozí stavu, zajištěno pro nový výpočet
		v.vymazat_casovou_obsazenost_objektu_a_pozice_voziku(v.OBJEKTY,v.VOZIKY);//vymaže předchozí časovou obsazenost objektů, jinak by se při každém dalším překreslení objekty posovali o obsazenost z předchozího vykreslení
		if(!JIZPOCITANO)//aby se znovu nevypočítávalo, pokud je již spočítáno a není třeba přepočítávat (např. z důvodu nového volání časových os či změny parametrů)
		{
			v.vymaz_seznam_PROCESY();
			v.hlavicka_PROCESY();//vymaže uložené procesy //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
			for(short i=0;i<8;i++)legenda_polozky[i]=false;//nastaví všechny položky na false
		}
		Pom_proces=v.PROCESY->dalsi;//pomocný ukazatel na (v tomto případě na první)proces, využívá se v při načítání původně vytvořených náhodných hodnot čekání na palec
		double X=0;//výchozí odsazení na ose X
		//KrokY je vizuální rozteč na ose Y mezi jednotlivými vozíky zadáno globálně, kvůli jednotlivým krokům + používáno v Unit1 např. na posun obrazu po vozících
		long Y=Form1->scGPPanel_mainmenu->Height+oY;//+5=oY pouze grafická korekce
		Cvektory::TZakazka *Z=v.ZAKAZKY->dalsi;
		while(Z!=NULL)//jde po zakázkách
		{
			long Yloc;
			Cvektory::TCesta *C=Z->cesta->dalsi;
			while(C!=NULL)//jde po konkrétní cestě
			{
				Yloc=Y;
				unsigned int n=0;//pořádí v rámci zakázky
				Cvektory::TVozik *vozik=v.VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
				while (vozik!=NULL)//jde po pořadí vozíků, které řeší jenom konkrétní cestu dané zakázky
				{
					if(vozik->zakazka->n==Z->n)//řeší jenom pro konrétní cestu, pokud tedy vozík odpovídá aktuální zakázce
					{
						////nastvení počáteční ale i následující pozice objektu na časové ose daného vozíku
						if(vozik->pozice==-1)vozik->pozice=0;//protože implicitní hodnota pozice vozíku==-1 z důvodu mimo linku, 0 na začátku ve výchozí pozici
						if(C->objekt->rezim==0 && vozik->pozice<=C->objekt->obsazenost)//zohlednění obsazenost objektu v režimu S&G
							X=C->objekt->obsazenost;//převezme se koncová pozice v objektu z předchozího vozíku
						else//pro ostatní režimy
						{
							if(vozik->pozice==0 && vozik->n!=1)//nejednáli se o první vozík a zároveň se jedná o výchozí pozici v režimu kontinuál či pp (sice trochu nelogické, ale může se uvažovat že navěšování bude kontinuál)
							X=vozik->predchozi->start+C->objekt->CT/C->objekt->kapacita/60.0*PX2MIN;//tak stanový výchozí pozici dle CT a kapacity (např. při 3min CT a kapacitě 2 je časový offset mezi vozíky 1,5 min)
							else//ostatní pozice
							X=vozik->pozice;
						}
//						unsigned int WIP=v.WIP(1);
//						if(vozik->pozice==0 && vozik->n>=WIP+1)
//						X=v.vrat_start_a_pozici_vozikuPX(vozik->n-WIP).x;

						////uložení hodnot pro další využítí
						if(C->n==1)vozik->start=X;//uloží výchozí X hodnotu, prvního objektu pro daný vozík
						double X_predchozi=X;//uloží povodní X hodnotu

						////BUFFER vykreslení a uložení buffer pokud předchází
						if(vozik->pozice<X && vozik->pozice>0)
						vykresli_proces(canv,"BUF - "+C->predchozi->objekt->short_name,m.clIntensive(vozik->zakazka->barva,80),4,vozik->pozice-PosunT.x,X-PosunT.x,Yloc-PosunT.y);
						//Nefunguje zatím správněCvektory::TProces *P=new Cvektory::TProces;
						//P->n_v_zakazce=n+1;P->Tpoc=vozik->pozice-PosunT.x/PX2MIN;P->Tkon=X-PosunT.x/PX2MIN;P->Tdor=P->Tkon;P->Tpre=P->Tkon;P->Tcek=P->Tkon;P->cesta=C;P->vozik=vozik;
						//v.vloz_proces(P);

						////čekání na čištění pistole a výměnu barev včetně čekání v rámci OBJEKTU
						if(Form1->CheckBoxVymena_barev->Checked)
						{
								if(C->Opak!=0)
								if(n%C->Opak==0 && n!=0)//čištění, mimo první vozík protože buď je připravená linka (v případě první zakázky nebo je čištění součástí mezizakázkové výměny barev)
								{
									vykresli_proces(canv,"Č",m.clIntensive(vozik->zakazka->barva,-20),5,X-PosunT.x,X+C->Tc/60.0*PX2MIN-PosunT.x,Yloc-PosunT.y);
									X+=C->Tc/60.0*PX2MIN;
									if(!legenda_polozky[1])legenda_polozky[0]++;legenda_polozky[1]=true;
								}
								if(n==0 && Z->n>1)//výměna barev + čistění, mimo první zakázku, u té předpokládáme připravenost linky
								{
									vykresli_proces(canv,"V+Č",m.clIntensive(vozik->zakazka->barva,-40),4,X-PosunT.x,X+C->Tv/60.0*PX2MIN-PosunT.x,Yloc-PosunT.y);
									X+=C->Tv/60.0*PX2MIN;
									if(!legenda_polozky[2])legenda_polozky[0]++;legenda_polozky[2]=true;
								}
								X_predchozi=X;//pokud toto zakomentuji prodlouží se CT resp. vykreslí se např. LAK o ten kus delší
						}

						////vykreslení procesu (jednoho obdelníčku "v plavecké dráze") včetně výpočtu koncové pozice a uložení dílčích hodnot
						X=proces(canv,++n,X_predchozi,X,Yloc,C,vozik);

						//posunutí na ose Y na další vozík
						Yloc+=KrokY;
					}

					///vypis mezivozíkového TAKTIMU (pokud se jedná vozíky od dané cesty (bacha, prochází se všechny) a zároveň pokud se jedná o poslední proces vozíku (např. svě) a nejedná o zcela první vozík
					if(vozik->zakazka->n==Z->n && C->dalsi==NULL && vozik->n!=1)
					{
						vypis_mezivozikovy_takt(canv,vozik,X,Yloc);//ten přímo za vozíky
						vypis_mezivozikovy_takt(canv,vozik,X,KrokY*2.5+oY+3,true);//ten na ose X nahoře +3 grafická korekce
					}
					///-

					vozik=vozik->dalsi;//posun na další prvek v seznamu vozíků
				}
				C=C->dalsi;//posun na další prvek v seznamu segmentů cesty, jde po cestě
			}
			if(Form1->STATUS==Form1->OVEROVANI)//(pokud je status nastaven na klienta, jinak jen první zakázka)
			Z=Z->dalsi;//posun na další prvek v seznamu ZAKÁZEK, jde po zakázakách
			else Z=NULL;

			Y=Yloc;
			delete C;
		}
		delete Z;

		////DALŠÍ VYPLÝVAJÍCÍ NASTAVENÍ
		//výpočet hodnot kapacit pro další využítí (pro grafy, ROMA atd.)
		if(!JIZPOCITANO)v.uloz_doporucene_kapacity_objetku();
		//hodnoty pro další grafické použití či nastavení
		WidthCanvasCasoveOsy=m.round(X);//uchová velikost nejdelší osy, pro použítí pro export canvasu do rastru
		HeightCanvasCasoveOsy=Y-KrokY/2;//uchová výšku grafu
		if(Form1->grid)vykresli_Xosy(canv);//vykreslí statické svislice na časové osy pokud je aktivovaná mřížka
		//povolení zobrazení grafu
		if(Form1->scGPGlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkDownArrow)
		{
			Form1->scGPGlyphButton_close_grafy->Left=Form1->ClientWidth-Form1->scGPGlyphButton_close_grafy->Width;
			Form1->scGPGlyphButton_close_grafy->Top=Form1->Chart2->Top/*-Form1->GlyphButton_close_grafy->Height*/;
			Form1->g.ShowGrafy(true);
		}
		//nastavení a zobrazení zpráv
		if(JIZPOCITANO)Form1->g.zpravy();
		else Form1->g.nastav_zpravy();//nastavují jen v momentu nového výpočtu, tj. zadání nových parametrů nebo nového načtení
		//zobrazení legendy časových os
		vykresli_legendu_casovych_os(canv);
		//už se nebude ukladat proces znovu, protože byl vypočten a už není třeba zatěžovat znovu systémové prostředky (nehledě na to, že to bylo dost znát)
		JIZPOCITANO=true;
		Form1->Synteza->Enabled=true;//povolí mod technologické procesy
	}
	else
	{
		vykresli_vytizenost_objektu(canv);
		if(Form1->grid)vykresli_Xosy(canv);
	}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//vypočítá konec procesu (odbdelníčku)
double Cvykresli::proces(TCanvas *canv, unsigned int n, double X_predchozi, double X, int Y, Cvektory::TCesta *C/*segment cesty*/,Cvektory::TVozik *vozik)
{
	 TColor barva=vozik->zakazka->barva;
	 if(vozik->typ)barva=clSilver;//pokud se jedná o servisní vozík, tak bude šedivý

	 //uložení hodnot pro zcela další použítí
	 Cvektory::TProces *P=new Cvektory::TProces;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->n_v_zakazce=n;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->segment_cesty=C;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->vozik=vozik;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->Tpoc=X_predchozi*60.0/PX2MIN;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 //standardní situace
	 X+=C->CT/60.0*PX2MIN;
	 vykresli_proces(canv,C->objekt->short_name,barva,0,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
	 P->Tkon=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 // nestandardní - nelogická situace, pokud bude čas procesu včetně času přejezdu vozíku kratší než u totožného přechozího objektu (vozíky např. v rámci CO2 se nemohou předbíhat), přičte se i tato vzdálenost (vykresleno šrafovaně)
	 double DcS=vozik->zakazka->jig.delka;//old: v.PP.delka_voziku;
	 if(P->segment_cesty->Rotace)DcS=vozik->zakazka->jig.sirka;
	 double Xt=C->objekt->obsazenost;
	 //pokud se nejedná o S&G objekt je třeba ještě zohledňovat nutnou dobu přejezdu/zpoždění vozíku za objektem, naopak u S&G se předpokládá, že může být v kabině jenom jeden vozík, takže netřeba řešit
	 if(P->segment_cesty->objekt->rezim!=0)Xt+=m.prejezd_voziku(DcS,C->RD)/60.0*PX2MIN;//musí být takto vyseparované, protože jinak v podmínce dávalo chybu, že např. 60<60 je true
	 if(X < Xt)//komentář o řádek výše, pokud není u S&G délka může vracet přejezd/zpoždění vozíku
	 {
			//dorovnání na čas předchozího vozíku, je-li to nutné
			X_predchozi=X;//uloží povodní X hodnotu
			X=C->objekt->obsazenost;
			vykresli_proces(canv,C->objekt->short_name,barva,1,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
			P->Tdor=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
			//cas_prekonani_zpozdeni_o_min_delku_jednoho_voziku
			X_predchozi=X;//uloží povodní X hodnotu
			X=Xt;//to samé jako X+=m.prejezd_voziku(DcS,C->RD)/60.0*PX2MIN;
			vykresli_proces(canv,C->objekt->short_name,barva,2,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
			P->Tpre=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 }
	 else//pokud situace NEnastane, tak ošetření proti tomu, aby se neukládaly náhodného hodnoty,ale hodnoty standardního konce procesu tzn. srovnatelné s CT
	 {
		 P->Tdor=X*60.0/PX2MIN;
		 P->Tpre=X*60.0/PX2MIN;
	 }
	 X_predchozi=X;

	 ////PALCE - posun o čekání na palce
	 P->Trand=0;
	 Cvektory::TObjekt *Objekt_dalsi=vozik->zakazka->cesta->dalsi->objekt;if(C->dalsi!=NULL)Objekt_dalsi=C->dalsi->objekt;//pokud neexistuje následující objekt v cestě, uvažuje se o po přechodu poslední/první objekt (tedy typicky svěšování/navěšování) a vezme se pohon prvního objektu, jinak se bere pohon v cestě následující
	 if(Form1->ComboBoxCekani->ItemIndex>0 && //pokud je požadováno v menu
			C->objekt->cekat_na_palce!=0 && //a zároveň nění uživatelsky zakázáno
			 (// a zároveň je splňuje následují:
					 C->objekt->rezim==0 ||//čekání je to po objektu v režimu S&G nebo
					(C->objekt->rezim==1 && Objekt_dalsi->rezim==1 && C->objekt->pohon!=Objekt_dalsi->pohon)||//je to mezi K a K režimem s přechodem na jiný dopravník nebo
					(C->objekt->rezim==1 && Objekt_dalsi->rezim==2 && C->objekt->pohon!=Objekt_dalsi->pohon)||//K->PP a jiný dopravník nebo
					(C->objekt->rezim==2 && Objekt_dalsi->rezim==1)||//PP->K nebo
					(C->objekt->rezim==2 && Objekt_dalsi->rezim==2 && C->objekt->pohon!=Objekt_dalsi->pohon)||//PP->PP a jiný dopravník nebo
					 C->objekt->stopka==1 ||//když je za objektem stopka (//0-ne,1-ano,2-automaticky) nebo
					 C->objekt->cekat_na_palce==1//automaticky-uživaztelsky požadovano zohledňování čekání na palce//0-ne,1-ano,2-automaticky
			 )
	 )
	 {            //případ první/poslední                            //ostatní připady
			double RD=vozik->zakazka->cesta->dalsi->RD;if(C->dalsi!=NULL)RD=C->dalsi->RD;//pokud neexistuje následující objekt v cestě, uvažuje se o po přechodu poslední/první objekt (tedy typicky svěšování/navěšování) a vezme se rychlost pohonu prvního objektu, jinak se bere RD objektu v cestě následujícího
			double R=0;if(Objekt_dalsi->pohon!=NULL)R=Objekt_dalsi->pohon->roztec;//přiřazení rozteče pokud existuje
			double Cekani=m.cekani_na_palec(X*60.0/PX2MIN+C->CT,R,RD,Form1->ComboBoxCekani->ItemIndex)/60.0*PX2MIN;
			if(Form1->ComboBoxCekani->ItemIndex==2)//pokud je vybraná položka o náhodná hodnota
			{
				if(RANDOM){P->Trand=Cekani;}//a má se převzít nová, tak se uloží pro další použítí nově vygenerovanou hodnotu
				else
				{
					Cekani=Pom_proces->Trand;//načte původní vygenerovanou hodnotu
					Pom_proces=Pom_proces->dalsi;//pomocný ukazatel na proces, využívá se v při načítání původně vytvořených náhodných hodnot čekání na palec
				}
			}
			X+=Cekani;
	 }
	 if(X_predchozi!=X)vykresli_proces(canv,C->objekt->short_name,barva,3,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku čakání na palec na časové ose
	 P->Tcek=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 ////--

	 //uložení hodnot pro další použití (v dalších kolech)
	 C->objekt->obsazenost=X;//nahraje koncovou X hodnotu do obsaženosti objektu pro další využítí
	 vozik->pozice=X;//uložení pro další použítý vozík
	 if(!JIZPOCITANO)v.vloz_proces(P); //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 else {delete P; P=NULL;}

	 return X;
}
//---------------------------------------------------------------------------
//vykreslí jeden dílčí časový proces (obdelníček procesu objektu) pro jeden vozík, vytaženo pouze kvůli přehlednosti
void Cvykresli::vykresli_proces(TCanvas *canv, AnsiString shortname, TColor color, short typ, long X1, long X2, long Y,bool legenda)
{
	////osa
	//set_pen(canv,color,2);//nastavení pera barvy osy
	canv->Pen->Width=1;
	canv->Pen->Mode=pmCopy;
	canv->Pen->Style=psSolid;
	canv->Brush->Color=color;
	if(color==clSilver){if(!legenda_polozky[4])legenda_polozky[0]++;legenda_polozky[4]=true;}
	switch(typ)
	{
			case 0: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;break;//pro typ: normální proces
			case 1: canv->Brush->Style=bsDiagCross;canv->Pen->Color=color;if(!legenda_polozky[5])legenda_polozky[0]++;legenda_polozky[5]=true;break;//pro typ: doplněný o konec na čekání na proces totožný předchozí
			case 2: canv->Brush->Style=bsCross;canv->Pen->Color=color;if(!legenda_polozky[6])legenda_polozky[0]++;legenda_polozky[6]=true;break;//pro typ: nutná doba přejezdu vozíku
			case 3: canv->Brush->Style=bsVertical;canv->Pen->Color=color;if(!legenda_polozky[7])legenda_polozky[0]++;legenda_polozky[7]=true;break;//pro typ: doba čekání na palec
			case 4: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;canv->Pen->Mode=pmMask;if(!legenda_polozky[3])legenda_polozky[0]++;legenda_polozky[3]=true;//pmNotXor;/*zajistí vykreslení procesu transparentně*/break;//pro typ: obsazenost procesu či buffer
			case 5: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;canv->Pen->Mode=pmMask;//výměna barev či čištění pistole
	}
	//samotný obdelníček
	short o=0;if(legenda)o=1;//v případe zobrazeného orámování zmenší o jeden pixel u legendy
	short o2=0;if(typ==1 || typ==2 || typ==3)o2=1;//v případe daného typu zmenší o jeden pixel
	canv->Rectangle(X1+o,Y-KrokY/2+o2,X2+1-o,Y+KrokY/2-o2);//X2+1 pouze grafická záležitost - zmenšení mezery
	//následující musí být mimo switch kvůli pořadí vykreslování po rectanglu
	if(typ==4 && !legenda)//v případě bufferu vykreslení svislice přemaskující bílý spoj, tím se buffer napojí na předchozí objekt
	{
		canv->Pen->Color=color;
		canv->MoveTo(X1,Y-KrokY/2+1);canv->LineTo(X1,Y+KrokY/2-1);//+-1 grafická vyfikundace
	}

	////popisek
	//normal 0    buffer který má popisek menší než délku obdelničku
	if(typ==0 /*|| (typ==4 && canv->TextWidth(shortname)<X2-X1)*/)
	{
			SetBkMode(canv->Handle,OPAQUE);//nastvení netransparentního pozadí
			//if(color!=clBlack)canv->Font->Color=clBlack;else canv->Font->Color=clWhite;//pokud je výplň obdelníčku černě, tak popisek bude bíle
			canv->Font->Color=clWhite;
			canv->Font->Size=9;
			canv->Font->Name="Arial";
			canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
			canv->TextOutW(((X2+X1)/2)-canv->TextWidth(shortname)/2,Y-canv->TextHeight(shortname)/2,shortname);//vypíše vycentrovaný (polovina nových a starých souřadnic a posun referenčního písma o horizontálně=TextWidth/2 a verticálně=TextHeight/2) popisek shorname t-objektu
	}
}
//---------------------------------------------------------------------------
//vykreslí legendu pro jednotlivé procesy na časových osách
void Cvykresli::vykresli_legendu_casovych_os(TCanvas *canv)
{
	if(Form1->scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind==scgpbgkDownArrow)//pokud je legenda zobrazena + napozicování dle toho, zda jsou zobrazeny či skryty grafy
	{
		//pozice
		short P=legenda_polozky[0]+2;//počet obdelníků,kterých se bude vykreslovat
		short L=20;
		int T=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-P*KrokY-KrokY/2-9;//levé a top usazení legendy
		if(Form1->Chart2->Visible)T=Form1->Chart2->Top-P*KrokY-KrokY/2-9;//pokud jsou zobrazeny grafy, bude TOP pozice dle grafů
		int R=L+KrokY+canv->TextWidth("čištění pistole a výměna barev")+KrokY/2;//pravý okraj
		AnsiString V="";//výpis

		//top pozice
		Form1->scGPGlyphButton_close_legenda_casove_osy->Top=T+KrokY+KrokY/2+Form1->scGPGlyphButton_close_legenda_casove_osy->Options->FrameWidth;//top je nehledě na to, zda je či není zobrazeno
		//levá pozice tlačítka, pokud je legenda zobrazena
		Form1->scGPGlyphButton_close_legenda_casove_osy->Left=R-Form1->scGPGlyphButton_close_legenda_casove_osy->Width-1;

		//vykreslení obdelníku bíléhopozadí
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clWhite;
		canv->Pen->Style=psSolid;
		canv->Pen->Color=clMedGray;
		canv->Pen->Width=1;
		canv->Pen->Mode=pmCopy;
		canv->Rectangle(L-2,T+KrokY/2-2,R,T+P*KrokY-KrokY/2+2);

		//zjistí barvu (raději) druhého zobrazeného voziků, podle této barvy se vykreslí legenda
		unsigned int Voz=ceil((Form1->scLabel_titulek->Height+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
		TColor C=v.vrat_vozik(Voz+1)->zakazka->barva;

		//vykreslování samotné legendy, podle položek, které byly použity
		vykresli_proces(canv, "",C,0,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;V="technologický proces";canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,V);
		if(legenda_polozky[1]){vykresli_proces(canv, "",m.clIntensive(C,-20),5,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"čištění pistole");}
		if(legenda_polozky[2]){vykresli_proces(canv, "",m.clIntensive(C,-40),5,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"čištění pistole a výměna barev");}
		if(legenda_polozky[3]){vykresli_proces(canv, "",m.clIntensive(C,80),4,L-1,L+KrokY+1,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"buffer či čekání");}
		if(legenda_polozky[4]){vykresli_proces(canv, "",clSilver,0,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"vozíky čištění a výměny");}
		if(legenda_polozky[5]){vykresli_proces(canv, "",C,1,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"čekání na předchozí proces");}
		if(legenda_polozky[6]){vykresli_proces(canv, "",C,2,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"nutná doba přejezdu vozíku");}
		if(legenda_polozky[7]){vykresli_proces(canv, "",C,3,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"doba čekání na palec");}
	}
	else//pokud je legenda skryta, tak pouze napozicování dle toho, zda jsou zobrazeny či skryty grafy
	{
		Form1->scGPGlyphButton_close_legenda_casove_osy->Left=0;
		if(Form1->Chart2->Visible)Form1->scGPGlyphButton_close_legenda_casove_osy->Top=Form1->Chart2->Top;
		else Form1->scGPGlyphButton_close_legenda_casove_osy->Top=Form1->scGPPanel_statusbar->Top-Form1->scGPGlyphButton_close_grafy->Height;
	}
}
//---------------------------------------------------------------------------
//textový výpis a kóta mezivozíkového taktu, pouze pro zpřehlednění zapisu samostatně
void Cvykresli::vypis_mezivozikovy_takt(TCanvas *canv,Cvektory::TVozik *vozik,double X,long Y,bool index)
{
		long X1=Form1->m.round(vozik->predchozi->pozice-PosunT.x);
		long X2=Form1->m.round(X-PosunT.x);
		short S=2;//size
		//short O=0;//offset
		double Y0=Y-KrokY*2-PosunT.y; //-3 pouze grafické korekce

		////vykreslení kóty
		//linie
		canv->Pen->Width=1;
		canv->Pen->Color=vozik->zakazka->barva;
		canv->MoveTo(X1,Y0);
		canv->LineTo(X2,Y0);
		//šipky
		POINT bodyL[3]={{X1+S,Y0-S},{X1,Y0},{X1+S,Y0+S}};
		POINT bodyP[3]={{X2-S,Y0-S},{X2,Y0},{X2-S,Y0+S}};
		canv->Polygon((TPoint*)bodyL,2);
		canv->Polygon((TPoint*)bodyP,2);
		//spojovací linie k následujícímu vozíku
		if(!index)
		{
			canv->MoveTo(X2,Y0);
			canv->LineTo(X2,Y0+KrokY/2);
		}
		////text
		SetBkMode(canv->Handle,TRANSPARENT);
		canv->Font->Color=vozik->zakazka->barva;
		canv->Font->Size=9;
		canv->Font->Name="Arial";
		canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		AnsiString T="";
		//pro nezokrouhlený výpis čísla: if(!index)T="TT: "+AnsiString(floor(v.vrat_TT_voziku(vozik)*1000000.0)/1000000.0)+" s";
		//pro nezokrouhlený výpis čísla: else T=AnsiString(floor(v.vrat_TT_voziku(vozik)/60.0*1000000.0)/1000000.0);
		if(!index)T="TT: "+AnsiString(m.round2double(v.vrat_TT_voziku(vozik),precision))+" s";
		else T=AnsiString(m.round2double(v.vrat_TT_voziku(vozik)/60.0,precision));
		canv->TextOut((X1+X2)/2-canv->TextWidth(T)/2,Y0-canv->TextHeight(T),T);
}
//---------------------------------------------------------------------------
//vykreslí pohyblivou svislici na časové osy dle umístění kurzoru myši
void Cvykresli::vykresli_svislici_na_casove_osy(TCanvas *canv,int X,int Y)
{
	if(X!=-200)//pokud je mimo obraz -200 jen nahodilá hodnota pro zneplatenění čí výchozí obraz
	{
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Width=1;
		canv->Pen->Style=psDashDot;//nastevení čarkované čáry
		canv->Pen->Color=clGray;
		canv->Brush->Style=bsClear;
		//svislice
		canv->MoveTo(X,Form1->scGPPanel_mainmenu->Height);
		canv->LineTo(X,Form1->ClientHeight);
		//vodorovna
		if(!mod_vytizenost_objektu)//při modu vytížení objektů se nezobrazí
		{
			canv->MoveTo(0,Y);
			canv->LineTo(Form1->ClientWidth,Y);
			canv->Brush->Style=bsSolid;//vracím raději do původního stavu
			unsigned int V=ceil((Y+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
			if(V<=v.VOZIKY->predchozi->n)Form1->SB("Vozík: "+AnsiString(V));
			else Form1->SB("");//pokud je už mimo oblast
		}
	}
}
//---------------------------------------------------------------------------
//vypíše labal zaměřovač na pozici kurzoru myši
void Cvykresli::zobrazit_label_zamerovac(int X,int Y)
{
	unsigned int V=ceil((Y+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
	if(!mod_vytizenost_objektu && 0<V && V<=v.VOZIKY->predchozi->n) //pokud se nejedná o řežim vytíženost objektu a zároveň se jedná o číslo vozík od min do max vozíků
	{
				vykresli_svislici_na_casove_osy(Form1->Canvas,X,Y);//nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac
				Form1->Label_zamerovac->Transparent=false;
				Form1->Label_zamerovac->Color=clWhite;
				Form1->Label_zamerovac->Font->Color=(TColor) RGB(100,100,100);
				Form1->Label_zamerovac->Left=X+5; Form1->Label_zamerovac->Top=Y+20; //+ odsazení
				Form1->Label_zamerovac->Caption=" vozík: "+AnsiString(V)+" \n min: "+AnsiString((X+PosunT.x)/PX2MIN)+" ";
				Form1->Label_zamerovac->Visible=true;
				Form1->RM();//korekce chyby oskakování pravého menu
	}
	else Form1->Label_zamerovac->Visible=false;
}
//---------------------------------------------------------------------------
//vykreslí statické svislice na časové osy
void Cvykresli::vykresli_Xosy(TCanvas *canv)
{
	canv->Pen->Mode=pmNotXor;
	canv->Pen->Width=1;    //nastavení šířky pera
	canv->Pen->Style=psDot;
	canv->Pen->Color=TColor RGB(200,200,200);   //míchání světlě šedé
	canv->Brush->Style=bsClear;
	canv->Font->Color=clGray;
	canv->Font->Size=9;
	canv->Font->Name="Arial";
	canv->Font->Style = TFontStyles();
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
	short o=1;
	//už používám globálně short oY=5;//ofset na ose Y, 5 pouze grafická korekce
	if(PosunT.x>10)o=-30;
	if(!mod_vytizenost_objektu)canv->TextOutW(o-PosunT.x,0,"voz|min"); //popisek osy x
	else canv->TextOutW(o-PosunT.x,0,"obj|min"); //popisek osy x

	//svislice po dvou minutách
	int start=PX2MIN*2;if(PosunT.x>0)start=0;
	for(int i=start;i<=WidthCanvasCasoveOsy;i+=PX2MIN*2)//po dvou minutách
	{
		canv->MoveTo(i-PosunT.x,oY);
		if(mod_vytizenost_objektu && v.OBJEKTY->predchozi!=NULL)canv->LineTo(i-PosunT.x,v.OBJEKTY->predchozi->n*KrokY+KrokY+1);//+1 pouze optická korekce
		else canv->LineTo(i-PosunT.x,HeightCanvasCasoveOsy+oY-1);//-1 pouze optická korekce
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clWhite;
		canv->TextOutW(i-canv->TextWidth(i/PX2MIN)/2-PosunT.x,0,i/PX2MIN);
	}

	if(!mod_vytizenost_objektu)
	{
			//svislé číslování vozíků (to barevné úplně nalevo)
			canv->Brush->Style=bsSolid;
			canv->Font->Style=TFontStyles()<< fsBold;
			canv->Font->Color=clWhite;
			Cvektory::TVozik *voz=v.VOZIKY->dalsi;
			while(voz!=NULL)
			{
				canv->Brush->Color=voz->zakazka->barva;
				canv->TextOutW(0,voz->n*KrokY+1-PosunT.y,voz->n); //+1 pouze grafická korekce
				voz=voz->dalsi;
			}

			//začátky a konce zakázek
			Cvektory::TZakazka *ukaz=v.ZAKAZKY->dalsi;
			int konec=KrokY;
			while (ukaz!=NULL)//projede všechny zakázky, cesty
			{                                                                  //ze sekund na min
				TPointD RET=v.vrat_zacatek_a_konec_zakazky(ukaz);RET.x=RET.x/60.0;RET.y=RET.y/60.0;
				konec+=v.vrat_pocet_voziku_zakazky(ukaz)*KrokY;
				canv->Pen->Color=ukaz->barva;
				canv->Pen->Style=psSolid;
				canv->Pen->Width=2;
				canv->Brush->Style=bsSolid;
				canv->Brush->Color=clWhite;
				//x - plete, jedná se jen o začátek zakázky
				if(RET.x>0)
				{
					canv->MoveTo(RET.x*PX2MIN-PosunT.x,0);
					canv->LineTo(RET.x*PX2MIN-PosunT.x,konec-Form1->scGPPanel_mainmenu->Height+KrokY/2-3-PosunT.y/*HeightCanvasCasoveOsy*/);
				}
				//náběh linky
				unsigned int WIP=v.WIP(1);canv->Pen->Style=psDash;canv->Pen->Width=1;
				if(ukaz->n==1)//pro první zakázku               //y -jen konec
				{
					canv->MoveTo(v.VOZIKY->dalsi->pozice-PosunT.x,0);
					canv->LineTo(v.VOZIKY->dalsi->pozice-PosunT.x,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
					canv->LineTo(0,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
				}
				//doběh linky
				if(ukaz->predchozi==v.ZAKAZKY->predchozi && v.VOZIKY->predchozi->n-WIP>WIP)//pokud se jedná o poslední zakázku a doběh nastane později než náběh
				{
					canv->MoveTo(v.vrat_start_a_pozici_vozikuPX(v.VOZIKY->predchozi->n-WIP).y-PosunT.x,0);
					canv->LineTo(v.vrat_start_a_pozici_vozikuPX(v.VOZIKY->predchozi->n-WIP).y-PosunT.x,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
					canv->LineTo(0,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
				}
				//y - plete, jedná se jen o konec zakázky
				canv->Pen->Width=2;	canv->Pen->Style=psSolid;
				if(RET.y>0)
				{
					canv->MoveTo(RET.y*PX2MIN-PosunT.x,0);
					canv->LineTo(RET.y*PX2MIN-PosunT.x,konec-Form1->scGPPanel_mainmenu->Height+KrokY-3-PosunT.y/*HeightCanvasCasoveOsy*/);
				}
				canv->Brush->Style=bsSolid;
				canv->Brush->Color=ukaz->barva;
				canv->Font->Style=TFontStyles()<< fsBold;
				canv->Font->Color=clWhite;
				//výpis začátku zakázky
				if(RET.x>0)canv->TextOutW(RET.x*PX2MIN-canv->TextWidth(RET.x)/2-PosunT.x,0,AnsiString(m.round2double(RET.x,precision))+"<");//zobrazuje pouze větší než začátek obrazovky
				//výpis náběhu linky, pro první zakázku
				if(ukaz->n==1)canv->TextOutW(0,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-canv->TextHeight("N")-PosunT.y,"NÁBĚH LINKY");
				//výpis doběh linky,pokud se jedná o poslední zakázku a doběh nastane později než náběh
				if(ukaz->predchozi==v.ZAKAZKY->predchozi && v.VOZIKY->predchozi->n-WIP>WIP)canv->TextOutW(0,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y,"DOBĚH LINKY");
				//výpis konce zakázky + LT
				AnsiString LT=AnsiString(m.round2double(RET.y,precision));
				if(RET.y>0)canv->TextOutW(RET.y*PX2MIN-canv->TextWidth(LT)/2-PosunT.x,0,"<"+AnsiString(LT));//zobrazuje pouze větší než začátek obrazovky
				ukaz=ukaz->dalsi;
			}
	}
}
//---------------------------------------------------------------------------
//vykreslí vytíženost od daného objektu
void Cvykresli::vykresli_vytizenost_objektu(TCanvas *canv)
{
	int Y=KrokY+oY;
	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{                      //záměrné nadhodnocení kvůli hledání minima
		int Pocatek=v.PROCESY->predchozi->Tcek/60.0*PX2MIN-PosunT.x;int Konec=0-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
		Cvektory::TProces *P=v.PROCESY->dalsi;
		double X=0;
		while (P!=NULL)
		{
			if(ukaz==P->segment_cesty->objekt)
			{
				if(P->segment_cesty->objekt->kapacita_dop==1 || NOLIEX==0)vykresli_proces(canv,"",P->vozik->zakazka->barva,4,P->Tpoc/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y+oY-PosunT.y);//pro jednokapacitní vytíženost vykreslení přímo maximální, popř. pokud není požadováno vykreslení intenzity
				else vytizenost_procesu(canv,P,X,Y+oY); //pro vícekapacitně včetně škály vytíženosti, pokud je požadována
				if(Pocatek>P->Tpoc/60.0*PX2MIN-PosunT.x)Pocatek=P->Tpoc/60.0*PX2MIN-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
				if(Konec<P->Tcek/60.0*PX2MIN-PosunT.x)Konec=P->Tcek/60.0*PX2MIN-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
			}
			P=P->dalsi;
		};

		//vycentrovaný popisek v rámci objektu
		SetBkMode(canv->Handle,TRANSPARENT/*OPAQUE*/);//nastvení transparentního pozadí
		canv->Font->Color=clWhite;
		canv->Font->Size=10;
		canv->Font->Name="Arial";
		canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		canv->TextOutW(((Konec+Pocatek)/2)-canv->TextWidth(ukaz->name.UpperCase())/2,Y+oY-PosunT.y-canv->TextHeight(ukaz->name.UpperCase())/2,ukaz->name.UpperCase());//vypíše vycentrovaný (polovina nových a starých souřadnic a posun referenčního písma o horizontálně=TextWidth/2 a verticálně=TextHeight/2) popisek shorname t-objektu

		//svislé popisování objektů
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clGray;
		canv->TextOutW(0,Y+oY-canv->TextHeight(ukaz->short_name)/2/*-Form1->scGPPanel_mainmenu->Height+oY*/-PosunT.y,ukaz->short_name);

		ukaz=ukaz->dalsi;
		Y+=KrokY+2;//+2 pouze grafické odsazení pro všechny objekty mimo prvního
	}
}
////---------------------------------------------------------------------------
void Cvykresli::vytizenost_procesu(TCanvas *canv, Cvektory::TProces *P,double X,int Y)
{
	 if(X==0)X=P->Tpoc;
	 Cvektory::TProces *P_dal=P;
	 //exponenciální
	 short I=170;//aktuální intenzita světlá nejvíc , 120 pro linearní 170 pro epxponenicální
	 double IK=sqrt(I*1.0)/(P->segment_cesty->objekt->kapacita/*kapacita_dop*/);//intenzita krok  pro exponencialni
	 double A=0;short B=I;//pomocné pro exponeciální snižování intenzity
	 //linearní
	 if(NOLIEX==1)  //u lineárního je chyba u nejvyšší intenzity
	 {
			I=120;
			IK=I/(P->segment_cesty->objekt->kapacita_dop-1);//intenzita krok pro linearni
	 }

	 do
	 {
		 if(NOLIEX==2)//exponenciální
		 {
				vykresli_proces(canv,"",m.clIntensive(P->vozik->zakazka->barva,B),4,X/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y-PosunT.y);
				B=m.round(I-pow(A+=IK,2));//exponenciální snižování světlosti
		 }
		 if(NOLIEX==1)//lineární
		 {
				vykresli_proces(canv,"",m.clIntensive(P->vozik->zakazka->barva,I),4,X/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y-PosunT.y);
				I-=IK;//lineární snižování světlosti
		 }
		 P_dal=v.vrat_nasledujici_proces_objektu(P_dal);
		 if(P_dal!=NULL && X<P_dal->Tpoc)
		 X=P_dal->Tpoc;
		 else break;
	 }
	 while(X<P->Tcek);
	 X=P->Tcek;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//////ROMA metoda, vykreslí graf technologických procesů vůči jednotlivým t-objektům v čase
void Cvykresli::vykresli_technologicke_procesy(TCanvas *canv)
{
	////////VÝCHOZÍ NASTAVENÍ
	//--nastavení proměnných k účelu filtrace, nastavováno z unit1 či volání animace, pro přehlednost a lenost necháno zde předáním na lokální proměnné
	double K=TP.K;//Krok po kolika minutach se bude zobrazovat
	double OD=TP.OD;//od které min se proces začne vypisovat
	double DO=TP.DO;//konec zakazky v min
	unsigned int Nod=TP.Nod;//rozmezí Jaký se vypíše vozik,
	unsigned int Ndo=TP.Ndo;//rozmezí Jaký se vypíše vozik, pokud bude 0 vypisují se všechny
	bool A=TP.A;//jednořádková animace
	//--
	//výchozí proměnné
	int PXM=50;//měřítko pixelů na metr v tomto modu, zároveň však používám jako krok posunu na ose Y (přetížení proměnné)
	int D=Form1->m.round(v.PP.delka_jig*PXM);//vozik délka
	int S=Form1->m.round(v.PP.sirka_jig*PXM);//vozik šířka
	unsigned int X=0;//posun po X-ové ose
	short Yofset=D;if(S>D)Yofset=S;//výška řádku - daného časového úseku, podle šířky vozíku či největší hodnoty šířka/délka
	unsigned int Y=Yofset;//Posun po Y-oso včetně výchozí pozice
	canv->Font->Size=10;//nutno tady kvůli správné velikosti, pokud dojde ke změně je nutné párově změnit
	Xofset=4+canv->TextWidth(TP.KZ+K)+4;//zajistí správný počátek prvního objektu dle šířky nejdelší vypisované minuty

	////////VÝPOČET A ULOŽENÍ POZICE OBJEKTŮ NA X OSE
	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		if(ukaz->delka_dopravniku<=0 /*doplnit ještě pokud je požadováno uživatelsky dle kapacity*/)//pokud by byla zadaná neplatná hodnota, tak se vezme dle kapacity zadané
		{
			if(Form1->CheckBox_pouzit_zadane_kapacity->Checked)
			{
				if(ukaz->orientace)X+=ukaz->kapacita*S;//pokud se mají použít zadané kapacity
				else X+=ukaz->kapacita*D;//pokud se mají použít zadané kapacity
			}
			else
			{
				if(ukaz->orientace)X+=ukaz->kapacita_dop*S;//pokud se mají použít zadané kapacity
				else X+=ukaz->kapacita_dop*D;//pokud se mají použít zadané kapacity
			}
		}
		else
		{
			X+=ukaz->delka_dopravniku*PXM;
		}
		ukaz->obsazenost=X;//zneužití proměné obsazenost,//přes ->předchozí asi by nešlo načítat, protože pořadí objektu na cestě a ve spojaku OBJEKTY nemusí být totožné
		ukaz=ukaz->dalsi;
	}

	////////VOZÍKY
	//nastavení popisku
	SetBkMode(canv->Handle,/*TRANSPARENT*/OPAQUE);//nastvení netransparentního pozadí
	canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	canv->Font->Size=12;
	canv->Font->Name="Arial";
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
	//nastavení pera pro vykreslení vozíčku
	canv->Pen->Mode=pmCopy;
	canv->Pen->Width=1;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=clWhite;
	canv->Brush->Style=bsSolid;
	canv->Font->Color=clWhite;
	//samotné vykreslení, vypíše vždy všechny procesy v dané minutě
	Cvektory::TProces *P=v.PROCESY->dalsi;
	while (P!=NULL)
	{
		for(double MIN=OD;MIN<=DO;MIN+=K)
		{   //filtr na rozsah vozíků, nebo pokud Ndo==0, tak se vypíší všechny             //nemůže být "="    //pro poslední vozík
				if(((Nod<=P->vozik->n && P->vozik->n<=Ndo) || Ndo==0) && P->Tpoc/60.0<=MIN && (MIN<P->Tcek/60.0 || DO==P->Tcek/60.0))//filtr
				{
					int Rx=D;int Ry=S;//rozměr
					if(P->segment_cesty->objekt->orientace)//pokud je požadovaná rotace jigu v objektu
					{
						Rx=S;Ry=D;
						//v případě, že se vozík blíží ke konci objektu orotuje jig zase zpět, Xp - X predikce následného výpočtu
						int Xp=P->segment_cesty->objekt->predchozi->obsazenost+((P->segment_cesty->objekt->obsazenost-P->segment_cesty->objekt->predchozi->obsazenost)*(MIN-P->Tpoc/60.0)/(P->Tcek/60.0-P->Tpoc/60.0))+Xofset+Rx/2;
						//lze odbrat /2 za Xofset či >= předělat na =, ale stále se nejedná o opticky dokonalý jev, rotace totiž "probíhá" v začátku vozíku nikoliv jeho středu, je dobré, si pro otestování vypsat ve filtru krok po 0,1
						if(Xp-Rx/2+Xofset/2>=P->segment_cesty->objekt->obsazenost){Rx=D;Ry=S;}
					}
					//výpočet umístění na ose X už jen v rámci objektu, tzn. aby byl znatelný posun (po částech i v rámci objektu)
					//pro jednokapacitní resp. S&G neanimuje, pokud není nastaveno Checkboxem jina
					if(P->segment_cesty->objekt->kapacita==1 && !Form1->CheckBoxAnimovatSG->Checked)
					{
						X=P->segment_cesty->objekt->obsazenost;//pokud se do objektu vejde pouze jenom jeden objekt
						X+=Xofset-Rx/2;//ještě grafické odsazení o odsazení výchozí osy a o šířku jednoho vozíku
					}
					else//animace i v rámci objektu
					{
						X=P->segment_cesty->objekt->predchozi->obsazenost+
						(
							(P->segment_cesty->objekt->obsazenost-P->segment_cesty->objekt->predchozi->obsazenost)*
							(MIN-P->Tpoc/60.0)/(P->Tcek/60.0-P->Tpoc/60.0)
						);
						X+=Xofset+Rx/2;//ještě grafické odsazení o odsazení výchozí osy a o šířku jednoho vozíku
					}
					//vykreslení samotného vozíku (obdélníčku)
					canv->Brush->Color=P->vozik->zakazka->barva;
					AnsiString T=P->vozik->n;
					//if(P->segment_cesty->objekt->rotace)if(P->segment_cesty->objekt->obsazenost+Xofset<X){Rx=D;Ry=S;}
					if(!A)//pokud se nejedná o animaci, aby bylo možné posouvat obraz na ose Y a při animaci naopak nebylo možné
					{
						Y=Yofset*(MIN-OD)/K+Yofset;//výpočet umístění na ose Y (jedná se pouze o umístění na řádku správné minuty)
						canv->Rectangle(X-Rx/2-PosunT.x,Y-Ry/2-PosunT.y,X+Rx/2+1-PosunT.x,Y+Ry/2-PosunT.y);  //+1 pouze grafická vyfikundace
						canv->TextOutW(X-canv->TextWidth(T)/2-PosunT.x,Y-canv->TextHeight(T)/2-PosunT.y,T);
					}
					else//jedná se o animaci
					{
						canv->Rectangle(X-Rx/2-PosunT.x,Y-Ry/2,X+Rx/2+1-PosunT.x,Y+Ry/2);  //+1 pouze grafická vyfikundace
						canv->TextOutW(X-canv->TextWidth(T)/2-PosunT.x,Y-canv->TextHeight(T)/2,T);
					}
				}
		}
		P=P->dalsi;
	}

	////////POPISKY a svislice
	//nastavení
	canv->Pen->Width=2;
	canv->Pen->Mode=pmCopy;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=clGray;
	canv->Brush->Color=clWhite;
	canv->Font->Color=clGray;
	canv->Font->Size=10;
	Y=4;
	X=Xofset;unsigned int Xpuv=X;
	////vodorovný popisek ale svislé čáry - OBJEKTY
	canv->MoveTo(X-PosunT.x,Y);if(!A)canv->LineTo(X-PosunT.x,Yofset+PXM*DO/K-PosunT.y);else canv->LineTo(X-PosunT.x,Yofset+PXM);//nakreslení první svislice (začátek pravděpodobně navěšování)
	ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		//ověřit proč není použito int Rx=D;
		//int Ry=S;if(ukaz->rotace==90){Rx=S;Ry=D;}//rozměr
		//prověřit§§§§
		int Ry=m.round(m.UDV(S,D,ukaz->orientace));//hodnoty záměrně obráceně
		X=ukaz->obsazenost+Xofset;
		canv->MoveTo(X-PosunT.x,Y);
		if(!A)canv->LineTo(X-PosunT.x,Yofset+Ry*DO/K+Yofset/2-PosunT.y);//pokud se nejedná o animaci, pozn. osa Y si stejně vypisuje nějak divně
		else canv->LineTo(X-PosunT.x,Yofset+Ry);//pokud se jedná o animaci
		AnsiString T=ukaz->short_name;//výpis popisku objektu
		canv->TextOutW((Xpuv+X)/2-canv->TextWidth(T)/2-PosunT.x,Y+1,T);//+1 pouze grafická korekce
		if(ukaz->delka_dopravniku>0 && ukaz->kapacita>1)//v případě vícekapacitního objektu s uvedeného délkou, výpiše i jeho délku a případně rychlost dopravníku
		{
			AnsiString T1="";//další řádek
			T1+=AnsiString(ukaz->delka_dopravniku)+"[m]";
			if(ukaz->RD!=0)T1+=", "+AnsiString(Form1->m.round2double(ukaz->RD,4))+"[m/s]";//pozor zaokrouhleno na 4desetinná
			canv->TextOutW((Xpuv+X)/2-canv->TextWidth(T1)/2-PosunT.x,Y+canv->TextHeight(T1)+2,T1);//+2 pouze grafická korekce
		}
		//posun na další
		Xpuv=X;
		ukaz=ukaz->dalsi;
	}
	////svislý popisek - MINUTY  (ale vodorovné čáry)
	Y=Yofset;
	X=4;//2 - grafické odsazení
	canv->Pen->Width=1;
	canv->Pen->Mode=pmNotXor;
	canv->Pen->Style=psDot;
	canv->Pen->Color=TColor RGB(200,200,200);   //míchání světlě šedé
	canv->Brush->Color=clWhite;
	canv->Font->Color=clGray;
	canv->Font->Size=10;//musí být stejný jako u objektů
	for(double MIN=OD;MIN<=DO;MIN+=K)//po půlminutách či nastaveném kroku
	{
		if(!A)//pokud se nejedná o animaci
		{
			if(Form1->grid)//pokud je požadován grid
			{
				canv->MoveTo(X,Y-PosunT.y-Yofset/2);//-PXM/2 aby linie byly nád a pod vozíkem
				canv->LineTo(Form1->ClientWidth,Y-PosunT.y-Yofset/2);
			}
			canv->TextOutW(X,Y-canv->TextHeight(MIN)/2-PosunT.y,MIN);
			Y+=Yofset;
		}
		else//pokud se jedná o animaci
		{
			canv->TextOutW(X,Y-canv->TextHeight(MIN)/2,MIN);
		}
	}
	//popisek pouze [min]
	canv->TextOutW(1,oY-1,"[min]");//-1 pouze grafická korekce
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::rotace_textu(TCanvas *canv, long rotace)//úhel rotace je desetinách stupně
{
	LOGFONT LogRec;
	GetObject(canv->Font->Handle,sizeof(LogRec),&LogRec);
	LogRec.lfEscapement=rotace;
	canv->Font->Handle=CreateFontIndirect(&LogRec);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////nastaví pero                                                             //http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
void Cvykresli::set_pen(TCanvas *canv, TColor color, int width, int style)//PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matoušek III str. 179
{
	DeleteObject(canv->Pen->Handle);//zruší původní pero
	DWORD pStyle = PS_GEOMETRIC | PS_SOLID | style /*| PS_JOIN_BEVEL*/ | PS_INSIDEFRAME; //|PS_JOIN_MITER
	DWORD pWidth = width;

	LOGBRUSH lBrush;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbColor = color;
	lBrush.lbHatch = 0;
	//LOGBRUSH lBrush={BS_PATTERN,color,(unsigned)srafura()->Handle};

	canv->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, 0, NULL);
}
////---------------------------------------------------------------------------
Graphics::TBitmap *Cvykresli::srafura()
{
	Graphics::TBitmap * Vzor=new Graphics::TBitmap;
	Vzor->Height=50*F->Zoom;Vzor->Width=50*F->Zoom;
	Vzor->Canvas->Pen->Color=clStenaHaly;
	for(int i=0;i<=Vzor->Width;i+=5*F->Zoom)
	{
		Vzor->Canvas->MoveTo(i,0);
		Vzor->Canvas->LineTo(m.round(i-2*F->Zoom)/*m.rotace(i,180,sklon).x*/,Vzor->Height);
	}
//		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
//		Vzor->LoadFromFile("Vzor.bmp");

	return Vzor;
}
////---------------------------------------------------------------------------
TColor Cvykresli::set_color(TCanvas *canv, Cvektory::TObjekt *O)
{
		unsigned short i=0;
		Cvektory::TObjekt *D=O->dalsi;
		if(O!=NULL && D==NULL)D=v.OBJEKTY->dalsi;//jedná se o poslední objekt a další bude první
		if(O->pohon!=NULL && D->pohon!=NULL)//pro situaci kdy je pohon přiřazen
		{
			if(D->pohon->roztec==0)
			{
				i=10;//pokud je u dalšího pohon přiřazen, ale není zadaná rozteč, není kompletní a kompetentní info
				F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - pohon <b>"+D->pohon->name+"</b> přiřazen objektu <b>"+D->name+"</b> nemá přiřazenou rozteč!<br><b>řešení</b> - nastavte pohonu patřičnou rozteč.<br><br>",true);
			}
			else//jsou k dispozici data
			{                         //zvažit zda RD či aRD (kvůli PP)
				if(O->mezera>=m.minM(O->pohon->aRD,D->pohon->aRD,D->pohon->roztec))
				{
					i=0;//je v pořádku
					//nelze přepíše F->Z("<b>Linka v pořádku.</b>",false);
				}
				else
				{
					i=1;//není v pořádku nestíhá se čekání
					F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - nedostatečná doba čekání na palec, resp. rozestup!<br><b>řešení</b> - upravte rychlost pohonu nebo pohonu objektu následujícího či nastavte větší mezeru resp. rozestup mezi vozíky!<br><br>",true);
				}
			}
		}
		else//situace kdy nejsou pohony přiřazeny
		{
			if(O->pohon==NULL && D->pohon==NULL)//ani jeden z nich
			{
				F->Z("<b>"+O->name.UpperCase()+" a "+D->name.UpperCase()+"<br>problém</b> - objekty nemají přiřazeny pohony!<br><b>řešení</b> - přiřaďte patřičné pohony.<br><br>",true);
			}
			else//jenom jeden není přiřazen
			{
				if(O->pohon==NULL)F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - není přiřazen pohon!<br><b>řešení</b> - přiřaďte patřičný pohon.<br><br>",true);
				if(D->pohon==NULL && D!=v.OBJEKTY->dalsi)F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - následující objekt <b>"+D->name.UpperCase()+"</b> nemá přiřazen pohon!<br><b>řešení</b> - přiřaďte patřičný pohon objektu "+D->name.UpperCase()+".<br><br>",true);
			}
			i=10;//červeně šrafování, není kompletní a kompetentní info
		}

	 /*	rgb(255,140,0) - DarkOrange
		rgb(255,215,0) - Gold (žlutá)
		rgb(218,112,214) - Orchid (fialová)
		LightSkyBlue	rgb(135,206,250)
		//rgb(135,206,235) - SkyBlue
		rgb(152,251,152) -PaleGreen

		*/
		canv->Pen->Style=psClear;
		canv->Pen->Width=1;
		canv->Brush->Style=bsSolid;
		if(m.cele_cislo(Form1->Zoom)==false && i>0)canv->Pen->Style=psSolid;
		switch(i)
		{
			case 0:/*canv->Pen->Color=clWhite;canv->Brush->Color=clWhite*/;break;
			case 1:canv->Pen->Color=clRed;canv->Brush->Color=clRed;break;
			case 2:canv->Pen->Color=(TColor)RGB(255,140,0);canv->Brush->Color=(TColor)RGB(255,140,0);break;
			case 3:canv->Pen->Color=(TColor)RGB(255,215,0);canv->Brush->Color=(TColor)RGB(255,215,0);break;
			case 4:canv->Pen->Color=(TColor)RGB(218,112,214);canv->Brush->Color=(TColor)RGB(218,112,214);break;
			case 5:canv->Pen->Color=(TColor)RGB(135,206,250);canv->Brush->Color=(TColor)RGB(135,206,250);break;
			case 6:canv->Pen->Color=(TColor)RGB(152,251,152);canv->Brush->Color=(TColor)RGB(152,251,152);break;
			//default:
			default:canv->Brush->Style=bsDiagCross;canv->Pen->Color=clRed;canv->Brush->Color=clRed;break;
		}
		if(i==0) return (TColor)0;
		else return canv->Pen->Color;
}
//---------------------------------------------------------------------------
//nakreslí editační okno
void Cvykresli::editacni_okno(TCanvas *canv,unsigned int  X1, unsigned int  Y1, unsigned int  X2, unsigned int  Y2, unsigned short int size, COLORREF color)//implicitně černá a velikost 1
{
	canv->Pen->Width=size;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=(TColor)color;
	canv->Pen->Mode=pmNotXor;
	canv->Brush->Style=bsClear;
	canv->Rectangle(X1,Y1,X2,Y2);//obdelník editačního okna
	canv->Pen->Mode=pmCopy;
	canv->Brush->Style=bsSolid;
}
//nakreslí editační okno, přetížená fce
void Cvykresli::editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size, COLORREF color)
{
	editacni_okno(canv, LH.x, LH.y, PD.x, PD.y, size, color);
}
//---------------------------------------------------------------------------
//vykresluje kurzor kabiny + spojnice
void Cvykresli::vykresli_kurzor_kabiny (TCanvas *canv, int id, int X, int Y, Cvektory::TObjekt *p,int push_rotace)
{
	if((long)id!=F->VyID && (long)id!=pocet_objektu_knihovny+1 && X!=-200)
	{
		int rotace=270;//defaultní hodnota,dáno azimutem přímky
		double azimut,X1,Y1,X2,Y2,Xp,Yp;
		//nastavení rozměrů objektu
		TPoint rozmery_kabiny;
  	switch(id)
  	{
  		case 0:case 9://navěšování + svěšování
			rozmery_kabiny.x=5;rozmery_kabiny.y=3;break;//navěšování
  		case 5:rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//lakovna
  		case 6:rozmery_kabiny.x=10;rozmery_kabiny.y=3;break;//vytěkání
  		case 7:rozmery_kabiny.x=20;rozmery_kabiny.y=3;break;//sušárna
  		case 8:rozmery_kabiny.x=20;rozmery_kabiny.y=6;break;//chlazení
			default: rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//ostatní
		}
		//nastavení pera
		canv->Pen->Color=clBlack;
		canv->Pen->Width=1;
		canv->Pen->Style=psDot;//nastevení čarkované čáry
		canv->Pen->Mode=pmNotXor;
		canv->Brush->Style=bsClear;
		//vykreslení
		if(v.OBJEKTY->dalsi==NULL)//pokud se vkládá první objekt
		{
			canv->Rectangle(X,Y-m.m2px(rozmery_kabiny.y)/2,X+m.m2px(rozmery_kabiny.x),Y+m.m2px(rozmery_kabiny.y)/2);//vykreslení objektu
		}
		else//1 a více objektů
		{
			if(v.OBJEKTY->predchozi->n<2)//pouze jedna spojnice s předchozím
			{
				//ukazatel na předchozí
				if(p==NULL)p=v.OBJEKTY->predchozi;
				//zjištění rotace
				azimut=m.azimut(X,Y,m.L2Px(p->elementy->predchozi->geo.X4),m.L2Py(p->elementy->predchozi->geo.Y4));
				rotace=m.Rt90(azimut);
				//nastavení bodů objektu
				switch(rotace)
				{
					case 0:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y-m.m2px(rozmery_kabiny.x);Xp=X;Yp=Y2;break;
					case 90:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X-m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xp=X2;Yp=Y;break;
					case 180:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y+m.m2px(rozmery_kabiny.x);Xp=X;Yp=Y2;break;
					case 270:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X+m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xp=X2;Yp=Y;break;
				}
				//vykreslení kurzoru
				canv->Rectangle(X1,Y1,X2,Y2);
				//vykreslení spojnice k předchozímu
				canv->MoveTo(m.L2Px(p->elementy->predchozi->geo.X4),m.L2Py(p->elementy->predchozi->geo.Y4));
				canv->LineTo(X,Y);                                                                                                                                                                                                                                                                                                                                   //azimut musí být násoben -1
				sipka(canv,(X+m.L2Px(p->elementy->predchozi->geo.X4))/2.0,(Y+m.L2Py(p->elementy->predchozi->geo.Y4))/2.0,azimut*(-1),true,3,clBlack,clWhite,pmNotXor);
			}
			else//spojnice s předchozím + spojnice s posledním/dalším
			{
				//ukazatel na předchozí a poslední/další
				Cvektory::TObjekt *dalsi;
				if(p==NULL){p=v.OBJEKTY->predchozi;dalsi=v.OBJEKTY->dalsi;}
				else dalsi=p->dalsi;
				//zjištění rotace
				azimut=m.azimut(X,Y,m.L2Px(p->elementy->predchozi->geo.X4),m.L2Py(p->elementy->predchozi->geo.Y4));
				rotace=m.Rt90(azimut);
				//nastavení bodů objektu
				switch(rotace)
				{
					case 0:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y-m.m2px(rozmery_kabiny.x);Xp=X;Yp=Y2;break;
					case 90:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X-m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xp=X2;Yp=Y;break;
					case 180:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y+m.m2px(rozmery_kabiny.x);Xp=X;Yp=Y2;break;
					case 270:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X+m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xp=X2;Yp=Y;break;
				}
				//vykreslení kurzoru
				canv->Rectangle(X1,Y1,X2,Y2);
				//vykreslení spojnice k předchozímu
				canv->MoveTo(m.L2Px(p->elementy->predchozi->geo.X4),m.L2Py(p->elementy->predchozi->geo.Y4));
				canv->LineTo(X,Y);
				//vykreslení spojnice k poslední/další
				canv->MoveTo(Xp,Yp);
				canv->LineTo(m.L2Px(dalsi->elementy->dalsi->geo.X1),m.L2Py(dalsi->elementy->dalsi->geo.Y1));
				//vykreslení šipek                                                                                                                                                                     //azimut musí být násoben -1
				sipka(canv,(X+m.L2Px(p->elementy->predchozi->geo.X4))/2.0,(Y+m.L2Py(p->elementy->predchozi->geo.Y4))/2.0,azimut*(-1),true,3,clBlack,clWhite,pmNotXor);
				sipka(canv,(Xp+m.L2Px(dalsi->elementy->dalsi->geo.X1))/2.0,(Yp+m.L2Py(dalsi->elementy->dalsi->geo.Y1))/2.0,m.azimut(m.L2Px(dalsi->body->dalsi->X),m.L2Py(dalsi->body->dalsi->Y)+m.m2px(dalsi->body->dalsi->Y-dalsi->body->predchozi->Y)/2,Xp,Yp)*(-1),true,3,clBlack,clWhite,pmNotXor);
			}
		}
		//uložení rotace objektu, využítí při vkládání objektu, přepočet z azimutu přímky na orientaci objektu
		switch(rotace)
		{
			case 0:orientace_objektu=0;break;
			case 90:orientace_objektu=270;break;
			case 180:orientace_objektu=180;break;
			case 270:orientace_objektu=90;break;
		}
	}
	else if(X!=-200)odznac_oznac_vyhybku(canv,X,Y,p);
}
//---------------------------------------------------------------------------
//označí nebo odznačí objekt používá se při posouvání objektů
void Cvykresli::odznac_oznac_objekt(TCanvas *canv, Cvektory::TObjekt *p, int posunX, int posunY,COLORREF color)
{
		if((long)p->id!=F->VyID&&(long)p->id!=pocet_objektu_knihovny+1)
		{
			//ShowMessage(UnicodeString(p->X)+" "+UnicodeString(p->Y));
			//nastavení pera
			canv->Pen->Color=(TColor)color;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;

			//provizorní spojovací linie + znovupřekreslení zůčastněných objektů pro lepší vzhled
			if(v.OBJEKTY->predchozi->n>=3)//pokud budou alespoň 3 prky
			{
				if(p->n==1)//pokud se jedná o první prvek
				{
					canv->MoveTo(CorEx(p->predchozi->predchozi),CorEy(p->predchozi->predchozi));
					canv->LineTo(CorEx(p)+posunX,CorEy(p)+posunY);
					canv->LineTo(CorEx(p->dalsi),CorEy(p->dalsi));
					sipka(canv,(CorEx(p->predchozi->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->predchozi->X,p->predchozi->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->predchozi->X,p->predchozi->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(p->dalsi)+CorEx(p)+posunX)/2,(CorEy(p->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_objekt(canv,v.OBJEKTY->predchozi);
						vykresli_objekt(canv,p);
						vykresli_objekt(canv,p->dalsi);
					}
				}
				if(p->n==v.OBJEKTY->predchozi->n)//pokud se jedná o poslední prvek
				{
					canv->MoveTo(CorEx(p->predchozi),CorEy(p->predchozi));
					canv->LineTo(CorEx(p)+posunX,CorEy(p)+posunY);
					canv->LineTo(CorEx(v.OBJEKTY->dalsi),CorEy(v.OBJEKTY->dalsi));
					sipka(canv,(CorEx(p->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(v.OBJEKTY->dalsi)+CorEx(p)+posunX)/2,(CorEy(v.OBJEKTY->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((v.OBJEKTY->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((v.OBJEKTY->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_objekt(canv,p->predchozi);
						vykresli_objekt(canv,p);
						vykresli_objekt(canv,v.OBJEKTY->dalsi);
					}
				}
				if(p->n!=1 && p->n!=v.OBJEKTY->predchozi->n)//pokud se nejedná o první ani poslední prvek
				{
					canv->MoveTo(CorEx(p->predchozi),CorEy(p->predchozi));
					canv->LineTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					canv->LineTo(CorEx(p->dalsi),CorEy(p->dalsi));
					sipka(canv,(CorEx(p->dalsi)+CorEx(p)+posunX)/2,(CorEy(p->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie následující za prvkem
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(p->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_objekt(canv,p->predchozi);
						vykresli_objekt(canv,p);
						vykresli_objekt(canv,p->dalsi);
					}
				}
			}

			if(v.OBJEKTY->predchozi->n==2)//pokud budou pouze 2 prky - zde netřeba řešit výhybku, posun, neměla by být
			{
				if(p->n==1)//pokud se jedná o první prvek
				{
					canv->MoveTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					canv->LineTo(m.L2Px(p->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(p->dalsi->Y)+O_height*Form1->Zoom/2);
					sipka(canv,(m.L2Px(p->dalsi->X)+m.L2Px(p->X)+posunX+O_width*Form1->Zoom)/2,(m.L2Py(p->dalsi->Y)+m.L2Py(p->Y)+posunY+O_height*Form1->Zoom)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_objekt(canv,p);
						vykresli_objekt(canv,p->dalsi);
					}
				}
				else//pokud se jedná o druhý prvek
				{
					canv->MoveTo(m.L2Px(p->predchozi->X)+O_width*Form1->Zoom/2,m.L2Py(p->predchozi->Y)+O_height*Form1->Zoom/2);
					canv->LineTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					sipka(canv,(m.L2Px(p->predchozi->X)+m.L2Px(p->X)+O_width*Form1->Zoom+posunX)/2,(m.L2Py(p->predchozi->Y)+m.L2Py(p->Y)+O_height*Form1->Zoom+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_objekt(canv,p->predchozi);
						vykresli_objekt(canv,p);
					}
				}
			}
			//samotná imitace posovaného objektu
			editacni_okno(canv, m.L2Px(p->X)+posunX,m.L2Py(p->Y)+posunY,m.L2Px(p->X)+O_width*Form1->Zoom+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom+posunY,1);
		}
		else
		odznac_oznac_vyhybku(canv,m.L2Px(p->X)+posunX,m.L2Py(p->Y)+posunY,p,true);//mohlo by být zakomponováno výše
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//používá se pro přidávání objektu mezi poslední a první prvek
void Cvykresli::odznac_oznac_objekt_novy_posledni(TCanvas *canv,int X, int Y)
{
		if(F->vybrany_objekt!=F->VyID&&F->Akce!=F->VYH)
		{
			if(v.OBJEKTY->predchozi->n>0)//pokud už existuje nějaký prvek
			{
				if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
				{
					canv->Pen->Color=clBlack;
					canv->Pen->Width=1;
					canv->Pen->Style=psDot;//nastevení čarkované čáry
					canv->Pen->Mode=pmNotXor;

					canv->MoveTo(CorEx(v.OBJEKTY->predchozi),CorEy(v.OBJEKTY->predchozi));
					canv->LineTo(X+O_width*Form1->Zoom/2,Y+O_height*Form1->Zoom/2);
					if(v.OBJEKTY->predchozi->n>1)//pokud je více než jenom jeden prvek
					{
						canv->LineTo(CorEx(v.OBJEKTY->dalsi),CorEy(v.OBJEKTY->dalsi));
						sipka(canv,(CorEx(v.OBJEKTY->dalsi)+X)/2+O_width*Form1->Zoom/4,(CorEy(v.OBJEKTY->dalsi)+Y)/2+O_height*Form1->Zoom/4,m.azimut(m.P2Lx(X),m.P2Ly(Y),v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
						if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
						vykresli_objekt(canv,v.OBJEKTY->dalsi); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
					}
					sipka(canv,(CorEx(v.OBJEKTY->predchozi)+X)/2+O_width*Form1->Zoom/4,(CorEy(v.OBJEKTY->predchozi)+Y)/2+O_height*Form1->Zoom/4,m.azimut(v.OBJEKTY->predchozi->X,v.OBJEKTY->predchozi->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_objekt(canv,v.OBJEKTY->predchozi); //znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				}
			}
			editacni_okno(canv,X,Y,X+O_width*Form1->Zoom,Y+O_height*Form1->Zoom,1);
		}
		else
		odznac_oznac_vyhybku(canv,X,Y,NULL);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//používá se pro přidvání objektu mimo poslední a první prvek
void Cvykresli::odznac_oznac_objekt_novy(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p)
{
		if(F->vybrany_objekt!=F->VyID&&F->Akce!=F->VYH)
		{
			if(p->n>0)//pokud už existuje nějaký prvek
			{
				if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
				{
					canv->Pen->Color=clBlack;
					canv->Pen->Width=1;
					canv->Pen->Style=psDot;//nastevení čarkované čáry
					canv->Pen->Mode=pmNotXor;
					//definování dalšího prvnku, ke kterému bude vykreslena linie (hlavní / sekundární větev)
					Cvektory::TObjekt *dalsi=NULL;
					if(v.akt_vetev)dalsi=p->dalsi;
					else dalsi=p->dalsi2;
					//spojovací linie
					canv->MoveTo(CorEx(p),CorEy(p));
					canv->LineTo(X+O_width*Form1->Zoom/2,Y+O_height*Form1->Zoom/2);
					canv->LineTo(CorEx(dalsi),CorEy(dalsi));
					sipka(canv,(CorEx(dalsi)+X)/2+O_width*Form1->Zoom/4,(CorEy(dalsi)+Y)/2+O_height*Form1->Zoom/4,m.azimut(m.P2Lx(X),m.P2Ly(Y),dalsi->X,dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_objekt(canv,dalsi); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
					sipka(canv,(CorEx(p)+X)/2+O_width*Form1->Zoom/4,(CorEy(p)+Y)/2+O_height*Form1->Zoom/4,m.azimut(p->X,p->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_objekt(canv,p);//znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
					dalsi=NULL;delete dalsi;
				}
			}
			editacni_okno(canv,X,Y,X+O_width*Form1->Zoom,Y+O_height*Form1->Zoom,1);
		}
		else
		odznac_oznac_vyhybku(canv,X,Y,p);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//pří umistivání či posouvání vyhýbky
void Cvykresli::odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p,bool posun)
{
		//definice souřadnic
		Cvektory::TObjekt *p1=NULL; if(p==NULL)p1=v.OBJEKTY->predchozi;else p1=p;
		Cvektory::TObjekt *p2=NULL;	if(p==NULL)p2=v.OBJEKTY->dalsi;		 else {if(v.akt_vetev)p2=p->dalsi;else p2=p->dalsi2;}
		if(posun)p1=p->predchozi;//pokud se jedná o posun objektu
		if(posun && p->dalsi==NULL)p2=v.OBJEKTY->dalsi;//pokud se jedná o posun objektu a jedná se o poslední prvek

    //samotný algoritmus
		if(p1->n>0&&!posun)//pokud už existuje nějaký prvek
		{
			if(X!=-200 && Y!=-200 && p2!=NULL)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
			{
				//nastavení pera
				canv->Pen->Color=clBlack;
				canv->Pen->Width=1;
				canv->Pen->Style=psDot;//nastevení čarkované čáry
				canv->Pen->Mode=pmNotXor;
				canv->Brush->Style=bsClear;
				//spojovací linie
				canv->MoveTo(CorEx(p1),CorEy(p1));
				canv->LineTo(X,Y);
				canv->LineTo(CorEx(p2),CorEy(p2));
				if(F->Akce==F->VYH)
				{
					canv->MoveTo(X,Y);
					canv->LineTo(CorEx(F->pom_vyhybka),CorEy(F->pom_vyhybka));
					sipka(canv,(CorEx(F->pom_vyhybka)+X)/2,(CorEy(F->pom_vyhybka)+Y)/2,m.azimut(F->pom_vyhybka->X,F->pom_vyhybka->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);
				}
				sipka(canv,(CorEx(p2)+X)/2,(CorEy(p2)+Y)/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),p2->X,p2->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
				if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				//problém ve vykresli_rectangle
				vykresli_objekt(canv,p2); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				sipka(canv,(CorEx(p1)+X)/2,(CorEy(p1)+Y)/2,m.azimut(p1->X,p1->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
				if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				vykresli_objekt(canv,p1);//znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				//nutné znovu nastavení pera
				canv->Pen->Color=clBlack;
				canv->Pen->Width=1;
				canv->Pen->Style=psDot;//nastevení čarkované čáry
				canv->Pen->Mode=pmNotXor;
				canv->Brush->Style=bsClear;
				//kruh
				canv->Ellipse(X-V_width*F->Zoom,Y-V_width*F->Zoom,X+V_width*F->Zoom,Y+V_width*F->Zoom);
			}
		}
		if(posun)
		{
      //nastavení pera
			canv->Pen->Color=clBlack;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;
			//spojovací linie
			canv->MoveTo(CorEx(p1),CorEy(p1));
			canv->LineTo(X,Y);
			canv->LineTo(CorEx(p2),CorEy(p2));
			if(p->dalsi2!=NULL&&p->id==(unsigned)F->VyID)
	  	{
	  		canv->MoveTo(X,Y);
	  		canv->LineTo(CorEx(p->dalsi2),CorEy(p->dalsi2));
	  		sipka(canv,(CorEx(p->dalsi2)+X)/2,(CorEy(p->dalsi2)+Y)/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),p->dalsi2->X,p->dalsi2->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
	  		if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				vykresli_objekt(canv,p->dalsi2); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
	  	}
			if(p->predchozi2!=NULL&&p->id==pocet_objektu_knihovny+1)
			{
	  		canv->MoveTo(X,Y);
	  		canv->LineTo(CorEx(p->predchozi2),CorEy(p->predchozi2));
	  		sipka(canv,(CorEx(p->predchozi2)+X)/2,(CorEy(p->predchozi2)+Y)/2,m.azimut(p->predchozi2->X,p->predchozi2->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
	  		if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				vykresli_objekt(canv,p->dalsi2); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
			}
			sipka(canv,(CorEx(p2)+X)/2,(CorEy(p2)+Y)/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),p2->X,p2->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
			if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
			vykresli_objekt(canv,p2); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
			sipka(canv,(CorEx(p1)+X)/2,(CorEy(p1)+Y)/2,m.azimut(p1->X,p1->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
			if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
			vykresli_objekt(canv,p1);//znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
			//nutné znovu nastavení pera
			canv->Pen->Color=clBlack;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;
			//kruh
			canv->Ellipse(X-V_width*F->Zoom,Y-V_width*F->Zoom,X+V_width*F->Zoom,Y+V_width*F->Zoom);
		}
		p1=NULL;delete p1;
		p2=NULL;delete p2;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí či odznačí potenciální umístění větve, další fáze po umístění výhybky
void Cvykresli::odznac_oznac_vetev(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p)
{
		if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
		{
			//nastavení pera
			canv->Pen->Color=clBlack;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;

			//vykreslení spojovací linie
			canv->MoveTo(m.L2Px(p->X),m.L2Py(p->Y));
			canv->LineTo(X,Y);
			sipka(canv,(m.L2Px(p->X)+X)/2,(m.L2Py(p->Y)+Y)/2,m.azimut(p->X,p->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
			canv->Ellipse(X-V_width*F->Zoom,Y-V_width*F->Zoom,X+V_width*F->Zoom,Y+V_width*F->Zoom);
		}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//slouží k určení zda se kurzor vyskytuje v pásmu mezi dvěma objekty,0= nic nenalezeno, 1= nalezen dalsi objekt, 2= nalezen dalsi2 objekt
short Cvykresli::lezi_v_pasmu(TCanvas *c,long X,long Y,Cvektory::TObjekt *p,bool odecti_region)
{
		short ret=0;//návratová proměnná, 0= nic nenalezeno, 1= nalezen dalsi objekt, 2= nalezen dalsi2 objekt
		bool pom;//pomocná proměnná, která uchovává zda je kurzor v pásmu
		int W=m.round(O_width*Form1->Zoom);
		int H=m.round(O_height*Form1->Zoom);
		if(p->dalsi!=NULL)//pouze v případě, že se nejedná o poslední objekt, prohledá oblast mezi p a p->dalsím
		{
	  	if((p->X<=p->dalsi->X && p->Y<p->dalsi->Y)||(p->X>p->dalsi->X && p->Y>=p->dalsi->Y))
			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y),m.L2Px(p->X)+W,m.L2Py(p->Y)+H,m.L2Px(p->dalsi->X)+W,m.L2Py(p->dalsi->Y)+H,m.L2Px(p->dalsi->X),m.L2Py(p->dalsi->Y),odecti_region);

	  	if((p->X>p->dalsi->X && p->Y<p->dalsi->Y)||(p->X<=p->dalsi->X && p->Y>=p->dalsi->Y))
			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y)+H,m.L2Px(p->X)+W,m.L2Py(p->Y),m.L2Px(p->dalsi->X)+W,m.L2Py(p->dalsi->Y),m.L2Px(p->dalsi->X),m.L2Py(p->dalsi->Y)+H,odecti_region);

			if(pom)ret=1;//pokud bylo něco nalezeno jedná se o nález další, ret = 1
		}
		if(!ret&&p->dalsi2!=NULL&&p->id==(unsigned)F->VyID)//pokud nebyl nalezen další objekt a zároveň se jedná o výhybku, která ma nadefinovanou sekundární větev, prohledá oblast mezi p a p->další2
		{
			if((p->X<=p->dalsi2->X && p->Y<p->dalsi2->Y)||(p->X>p->dalsi2->X && p->Y>=p->dalsi2->Y))
			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y),m.L2Px(p->X)+W,m.L2Py(p->Y)+H,m.L2Px(p->dalsi2->X)+W,m.L2Py(p->dalsi2->Y)+H,m.L2Px(p->dalsi2->X),m.L2Py(p->dalsi2->Y),odecti_region);

			if((p->X>p->dalsi2->X && p->Y<p->dalsi2->Y)||(p->X<=p->dalsi2->X && p->Y>=p->dalsi2->Y))
			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y)+H,m.L2Px(p->X)+W,m.L2Py(p->Y),m.L2Px(p->dalsi2->X)+W,m.L2Py(p->dalsi2->Y),m.L2Px(p->dalsi2->X),m.L2Py(p->dalsi2->Y)+H,odecti_region);

			if(pom)ret=2;//pokud bylo něco nalezeno jedná se o nález další2, ret = 2
		}
		return ret;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cvykresli::lezi_v_pasmu_poslednim(TCanvas *c,long X,long Y,bool odecti_region)
{
		bool ret=false;
		if(v.OBJEKTY->predchozi!=NULL)
		{
			int W=m.round(O_width*Form1->Zoom);
			int H=m.round(O_height*Form1->Zoom);

			if((v.OBJEKTY->predchozi->X<=v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y<v.OBJEKTY->dalsi->Y)||(v.OBJEKTY->predchozi->X>v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y>=v.OBJEKTY->dalsi->Y))
			ret=lezi_v_pasmu(c,X,Y,m.L2Px(v.OBJEKTY->predchozi->X),m.L2Py(v.OBJEKTY->predchozi->Y),m.L2Px(v.OBJEKTY->predchozi->X)+W,m.L2Py(v.OBJEKTY->predchozi->Y)+H,m.L2Px(v.OBJEKTY->dalsi->X)+W,m.L2Py(v.OBJEKTY->dalsi->Y)+H,m.L2Px(v.OBJEKTY->dalsi->X),m.L2Py(v.OBJEKTY->dalsi->Y),odecti_region);

			if((v.OBJEKTY->predchozi->X>v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y<v.OBJEKTY->dalsi->Y)||(v.OBJEKTY->predchozi->X<=v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y>=v.OBJEKTY->dalsi->Y))
			ret=lezi_v_pasmu(c,X,Y,m.L2Px(v.OBJEKTY->predchozi->X),m.L2Py(v.OBJEKTY->predchozi->Y)+H,m.L2Px(v.OBJEKTY->predchozi->X)+W,m.L2Py(v.OBJEKTY->predchozi->Y),m.L2Px(v.OBJEKTY->dalsi->X)+W,m.L2Py(v.OBJEKTY->dalsi->Y),m.L2Px(v.OBJEKTY->dalsi->X),m.L2Py(v.OBJEKTY->dalsi->Y)+H,odecti_region);
		}
		return ret;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cvykresli::lezi_v_pasmu(TCanvas *c,long X,long Y,long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,bool odecti_region)
{
		POINT *body=new POINT[4];
		body[0].x=x1;body[0].y=y1;
		body[1].x=x2;body[1].y=y2;
		body[2].x=x3;body[2].y=y3;
		body[3].x=x4;body[3].y=y4;

		HRGN hreg=CreatePolygonRgn(body,4,WINDING);//vytvoření regionu
		if(odecti_region)//aby nevybíral oblast i samotných objektů, tento region odečtu
		{
				HRGN hregO=CreateRectRgn(x1,y1,x2,y2);
				HRGN hregO1=CreateRectRgn(x3,y3,x4,y4);
				CombineRgn(hreg,hreg,hregO,RGN_DIFF);
				DeleteObject(hregO);
				CombineRgn(hreg,hreg,hregO1,RGN_DIFF);
				DeleteObject(hregO1);
		}
		//c->Brush->Color = RGB(96, 96, 96);  //testovací
		//FrameRgn(c->Handle,hreg,c->Brush->Handle,5,5);//testovací
		bool ret=PtInRegion(hreg,X,Y);
		delete body;body=NULL;
		DeleteObject(hreg);
		return ret;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vratí trend schématu pro objekt z parametru,pro jeden prvek automaticky bude trend schématu 0°, pokud se jedná o první prvek, tak výjimka, řeší trend podle prvku následujícího, pro další se hledí na trend podle azimutu k předchozímu prvku
double Cvykresli::trend(Cvektory::TObjekt *Objekt)
{
	double trend=0;
	if(v.OBJEKTY->predchozi->n==1)trend=90;//pro jeden prvek automaticky bude trend schématu 90°
	else//pro více objektů
	{
		if(Objekt->n==1)trend=m.azimut(Objekt->X,Objekt->Y,Objekt->dalsi->X,Objekt->dalsi->Y);//pokud se jedná o první prvek, tak výjimka, řeší trend podle prvku následujícího
		else trend=m.azimut(Objekt->predchozi->X,Objekt->predchozi->Y,Objekt->X,Objekt->Y);//pro další se hledí na trend podle azimutu k předchozímu prvku
	}
	return trend;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslení layoutu
//void Cvykresli::vykresli_layout(TCanvas *canv)
//{
//	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
//	if(O!=NULL)//pokud existuje nějaký objekt
//	{
//		 ////stanovení rozeměrů a orientace nosného obdelníku layoutu
//		 //délka stran nosného obdelníka
//		 double obvod=v.vrat_soucet_delek_vsech_objektu();
//		 int W=Form1->ClientWidth;
//		 int H=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-Form1->scGPPanel_mainmenu->Height;
//		 double A=m.round(obvod/2*W/((W+H)/1.0));//rozměry poměrově k monitoru, vůči monitoru
//		 double B=obvod/2-A;//B se už jenom dopočítá jako rozdíl
//		 //orietnace nosného obdelníka dle zakresleného schématu
//		 short smerZ=0;if(O->dalsi!=NULL)smerZ=m.round(m.azimut(O->X,O->Y,O->dalsi->X,O->dalsi->Y)/90.0)*90;//udá uvodní směr obdelníku
//		 short smerDo=0;if(O->predchozi!=NULL)smerDo=m.round(m.azimut(O->predchozi->predchozi->X,O->predchozi->predchozi->Y,O->X,O->Y)/90.0)*90;//udá uvodní směr obdelníku
//		 double X1=O->X,Y1=O->Y,X2=O->X,Y2=O->Y,X3=O->X,Y3=O->Y,X4=O->X,Y4=O->Y;
//		 if(smerZ==90&&(smerDo==0||smerDo==90)){X2+=A;X3+=A;Y3-=B;Y4-=B;}
//		 if(smerZ==90&&smerDo==180){X2+=A;X3+=A;Y3+=B;Y4+=B;}
//		 if(smerZ==270&&(smerDo==0||smerDo==270)){X2-=A;X3-=A;Y3-=B;Y4-=B;}
//		 if(smerZ==270&&smerDo==180){X2-=A;X3-=A;Y3+=B;Y4+=B;}
//		 if(smerZ==0&&(smerDo==270||smerDo==0)){Y2+=B;X3+=A;Y3+=B;X4+=A;}
//		 if(smerZ==0&&smerDo==90){Y2+=B;X3+=A;Y3+=B;X4-=A;Y4+=B;}
//		 if(smerZ==180&&(smerDo==270||smerDo==180)){Y2-=B;X3+=A;Y3-=B;X4+=A;}
//		 if(smerZ==180&&smerDo==90){Y2-=B;X3-=A;Y3-=B;X4-=A;}
//		 //uložení bodů nosného obdelníka pro další použití
//		 short velikost_pole=4;
//		 TPointD *P=new TPointD[velikost_pole];//vytovoří pole pro polyline
//		 P[0].x=X1;P[0].y=Y1;P[1].x=X2;P[1].y=Y2;P[2].x=X3;P[2].y=Y3;P[3].x=X4;P[3].y=Y4;
//		 ////----
//
//		 ////grafické nastavení
//		 //pero+výplň
//		 canv->Brush->Color=clWhite;
//		 canv->Brush->Style=bsSolid;
//		 canv->Pen->Color=clRed;
//		 canv->Pen->Width=Form1->Zoom*1;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*1;
//		 //font
//		 canv->Font->Color=clRed;
//		 SetBkMode(canv->Handle,TRANSPARENT);
//		 canv->Font->Size=Form1->Zoom*9; if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*9;
//		 canv->Font->Name="Arial";
//
//		 ////pomocné promněnné
//		 double akt_pozice=0;//v segmentu
//		 double zbytek=0;//z předchozí segmentu
//		 unsigned int i=0;//číslo prvního bodu segmentu
//		 TPointD S;
//		 S.x=P[i].x;S.y=P[i].y;//na uchování výchozí pozice
//
//		 ////procházení jednotlivých objektů
//		 while (O!=NULL)
//		 {
//			////stanovení resp. načtení délky a šířky objektu
//		 	double delka=0;
//		 	double sirkaV=0;
//		 	double delkaV=0;
//		 	if(O->rezim==0)//S&G, u tohoto režimu se bere délka nebo šířka vozíku, dle nastavení, zda je vozík v objektu na šířku či na délku
//			{
//		 		if(O->rotace==0)
//		 		{
//		 			delka=v.PP.delka_voziku;delkaV=delka;
//		 			sirkaV=v.PP.sirka_voziku;
//				}
//		 		else
//		 		{
//		 			delka=v.PP.sirka_voziku;delkaV=delka;
//		 			sirkaV=v.PP.delka_voziku;
//				}
//		 	}
//		 	else //u kontinuálního a pp se uvažuje jako délka přímo jen délka dopravníku
//		 	{
//		 		delka=O->delka_dopravniku;
//				if(O->rotace==0)
//		 		{
//		 			sirkaV=v.PP.sirka_voziku;//sířka dle rotace (pokud je na délku či šířku)
//		 			delkaV=v.PP.delka_voziku;
//		 		}
//				else
//		 		{
//		 			sirkaV=v.PP.delka_voziku;
//		 			delkaV=v.PP.sirka_voziku;
//		 		}
//			}
//		  ////----
//
//		 	////vykreslení jednoho objektu
//			bool popisek_se_jiz_vypisoval=false;//detekce, zda se má vypsat popisek za zlomem či ne
//			unsigned int Poz_i=0;//pozice se indexují od začátku objektu
//			do
//			{
//					bool posunuti_segmentu=false;//dekece zda se má zvýšit počítadlo pozice segmentu - i
//					unsigned int n=i;if(i<3)n=i+1;else n=0;//"přeindexování" pokude se bude jednat o poslední spojnici (tj. poslední-první prvek)
//					double DS=m.delka(P[i].x,P[i].y,P[n].x,P[n].y);//delka_segmentu obrazce
//		 			if(zbytek>0)delka=zbytek;//zbytek objektu z minulého segmentu
//		 			if(delka+akt_pozice>=DS)//přetekl do dalšího segmentu
//		 			{
//		 				zbytek=delka+akt_pozice-DS;
//						delka-=zbytek;
//						akt_pozice=0;
//						posunuti_segmentu=true;//posunutí segmentu
//		 				//ShowMessage("přetekl "+AnsiString(i));
//		 			}
//					else//nepřetekl
//		 			{
//		 				akt_pozice+=delka;
//		 				zbytek=0;
//						posunuti_segmentu=false;
//						Poz_i=0;
//		 				//ShowMessage("nepřetekl "+AnsiString(i));
//		 			}
//
//					double PO=delka/DS;//pomer delky objektu a segmentu obrazce
//					//ShowMessage(AnsiString(delka)+"/"+AnsiString(DS));
//
//		 			////vykreslení
//					//objekt
//					TPointD S_puv=S;
//					canv->MoveTo(m.L2Px(S.x+sirkaV/2),m.L2Py(S.y));//pero na výchozí (minulou pozici)
//					S.x+=(P[n].x-P[i].x)*PO;//posun ze začátku objektu nakonec
//					S.y+=(P[n].y-P[i].y)*PO;//posun ze začátku objektu nakonec
//					canv->Pen->Width=Form1->Zoom*1;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*1;canv->Pen->Color=clRed;
//					canv->LineTo(m.L2Px(S.x+sirkaV/2),m.L2Py(S.y));//nakreslení linie
//
//					//pozice - vykreslí pozice v daném objektu
//					Poz_i=vykresli_pozice(canv,Poz_i,S_puv,S,delka,delkaV,sirkaV,O->mezera);
//
////					canv->Pen->Width=Form1->Zoom*3;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*3;
////					canv->Brush->Color=clRed;
////					canv->FrameRect(TRect(m.L2Px(S_puv.x)-1,m.L2Py(S_puv.y+sirkaV/2)+1,m.L2Px(S.x)+1,m.L2Py(S.y-sirkaV/2)-1));//provizorní řešení potom polyline
//
//
//					//zarazka
//					if(zbytek==0)//zarážka se zobrazí pouze pokud se nepokračuje ve vykreslování objektu v dalším segmentu
//		 			{
//						 double Alfa=m.azimut(S_puv.x,S_puv.y,S.x,S.y)+90;if(Alfa>=360)Alfa-=360;
//						 //ShowMessage(Alfa);
//						 if(posunuti_segmentu)Alfa=135;//v případě rohu je to 45°
//						 Alfa*=(M_PI/180);
//						 set_pen(canv,clRed,Form1->Zoom*2,PS_ENDCAP_SQUARE);//nastavení pera barvy osy
//						 //canv->Pen->Width=Form1->Zoom*2;canv->Pen->Color=clRed;
//
//						 canv->MoveTo(m.L2Px(S.x-sin(Alfa)*sirkaV/2),m.L2Py(S.y-cos(Alfa)*sirkaV/2));
//						 canv->LineTo(m.L2Px(S.x+sin(Alfa)*sirkaV/2),m.L2Py(S.y+cos(Alfa)*sirkaV/2));
//					}
//
////					//popisek objektu
////					canv->Brush->Color=clWhite;
////					canv->Font->Color=clRed;
////					if(zbytek<delka && popisek_se_jiz_vypisoval==false)//zajistí, že se vypisuje pouze jednou a navíc v tom z delších segmentů
////					{
////
////						AnsiString T=O->name.UpperCase();
////						AnsiString T1="K: "+AnsiString(O->kapacita)+"[v] DD: "+AnsiString(O->delka_dopravniku)+" [m]";
////						if(O->rezim==2)T1+=" RD: "+AnsiString(O->RD).SubString(1,5)+" [m/s]";//pokud se jedná o kontinual, tak ještě RD
////						double A=m.azimut(S_puv.x,S_puv.y,S.x,S.y);
////						bool RT=false;//rotace textu ano ne
////						if(A==270 ||  A==90)rotace_textu(canv,0);else {RT=true;rotace_textu(canv,A*10-900);}//rotace textu,ošetření, aby se zprava doleva nevypisovalo obráceně
////						if(!RT)
////						{
////							canv->TextOutW(m.L2Px((S.x+S_puv.x)/2.0)-canv->TextWidth(T)/2,m.L2Py((S.y+S_puv.y+sirkaV)/2.0)-canv->TextHeight(T),T);//vypíše název objektu uprostřed nad
////							canv->TextOutW(m.L2Px((S.x+S_puv.x)/2.0)-canv->TextWidth(T1)/2,m.L2Py((S.y+S_puv.y-sirkaV)/2.0),T1);//vypíše parametry objektu uprostřed pod
////						}
////						else
////						{  //ShowMessage(T+AnsiString(A));
////							if(A==180)
////							{
////								canv->TextOutW(m.L2Px((S.x+S_puv.x-sirkaV)/2.0+sirkaV/2)-canv->TextHeight(T)-1,m.L2Py((S.y+S_puv.y)/2.0)+canv->TextWidth(T)/2,T);//vypíše parametry objektu uprostřed pod
////								canv->TextOutW(m.L2Px((S.x+S_puv.x+sirkaV)/2.0+sirkaV/2)+1,m.L2Py((S.y+S_puv.y)/2.0)+canv->TextWidth(T1)/2,T1);//vypíše název objektu uprostřed nad
////							}
////							else
////							{
////								canv->TextOutW(m.L2Px((S.x+S_puv.x+sirkaV)/2.0+sirkaV/2)+canv->TextHeight(T)+1,m.L2Py((S.y+S_puv.y)/2.0)-canv->TextWidth(T)/2,T);//vypíše název objektu uprostřed nad
////								canv->TextOutW(m.L2Px((S.x+S_puv.x-sirkaV)/2.0+sirkaV/2)-1,m.L2Py((S.y+S_puv.y)/2.0)-canv->TextWidth(T1)/2,T1);//vypíše parametry objektu uprostřed pod
////							}
////						}
////						popisek_se_jiz_vypisoval=true;
////					}
//
//					//posunutí na další segment vykreslovaného obrazce layoutu
//					if(posunuti_segmentu)i++;
//			}
//		 	while(zbytek>0);
//
//			//posun na další prvek
//			O=O->dalsi;
//		 }
//
////		 //prozatim testovac9 //vykreslení oramování obdelníku
////		 canv->Pen->Color=clGray;
////		 canv->Brush->Color=clWhite;
////		 POINT O1[4]={{m.L2Px(X1),m.L2Py(Y1)},{m.L2Px(X2),m.L2Py(Y2)},{m.L2Px(X3),m.L2Py(Y3)},{m.L2Px(X4),m.L2Py(Y4)}};
////		 canv->Polygon((TPoint*)O1,3);
//
//		 ////celkový středový výpis
//		 rotace_textu(canv,0);
//		 canv->Font->Color=clGray;
//		 canv->Brush->Color=clWhite;
//		 W=0;//nejširší text
//
//		 AnsiString T1="obvod linky: "+AnsiString(obvod)+" [m]";if(canv->TextWidth(T1)>W)W=canv->TextWidth(T1);
//		 AnsiString T2="kapacita linky: "+AnsiString(v.WIP(1))+" [voz.]";if(canv->TextWidth(T2)>W)W=canv->TextWidth(T2);
//     //T3-T5 bylo zakázáno používat
//		 /*AnsiString T3="akt. plocha linky: "+AnsiString(A*B)+" [m2]";W=canv->TextWidth(T3);
//		 AnsiString T4="akt. X-rozměr linky: "+AnsiString(A)+" [m]";if(canv->TextWidth(T4)>W)W=canv->TextWidth(T4);
//		 AnsiString T5="akt. Y-rozměr linky: "+AnsiString(B)+" [m]";if(canv->TextWidth(T5)>W)W=canv->TextWidth(T5);*/
//		 AnsiString T6="lead time: "+AnsiString(v.vrat_LT())+" [s]";if(canv->TextWidth(T6)>W)W=canv->TextWidth(T6);
//		 /*if(P[0].x!=P[3].x)
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T1)*3,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T2)*2,T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T2)*1,T3);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2),T4);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T5)*1,T5);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T6)*2,T6);
//		 }
//		 else
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T1)*3,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T2)*2,T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T3)*1,T3);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2),T4);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T5)*1,T5);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T6)*2,T6);
//		 }*/
//		 if(P[0].x!=P[3].x)
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T1)*1,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2),T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T6)*1,T6);
//		 }
//		 else
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T1)*1,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2),T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T6)*1,T6);
//		 }
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//duplicita - pouze pro testovací účely, hlavní odstavena
void Cvykresli::vykresli_layout(TCanvas *canv)
{
	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	while(O!=NULL)//pokud existuje nějaký objekt
	{
		//vykresli_objekt(canv,O,0,50);
		//vykresli_objekt(bmp_in->Canvas,pom,F->m.P2Lx(Ox/F->m2px),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)*3/2.0)),Poffset,Timer_animace->Enabled);
		O=O->dalsi;
	}
	O=NULL;delete O;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//////------------------------------------------------------------------------------------------------------------------------------------------------------
//metoda používaná ve starém náhledu objektu - možno odstranit
//zajistí vykreslení náhledu objektu, XY -umístění L začátek (střed dopravníku) objektu v m, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
//za zmínění stojí lokální proměnná KR, což je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, DV - konec,
unsigned int Cvykresli::vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset,bool animace)
{
	////vychozí geometrické proměnné
	double DD=O->delka_dopravniku;//délka objektu v metrech
	double dJ=m.UDJ(v.PP.delka_jig,v.PP.sirka_jig,O->orientace);//délka jigu
	double sJ=m.UDJ(v.PP.sirka_jig,v.PP.delka_jig,O->orientace);//šířka jigu a tím pádem i minimální kabiny
	double dP=v.PP.delka_podvozek;
	double DV=dJ;if(dP>dJ)DV=dP;
	double M=O->mezera;//mezera
	double R=0;if(O->pohon!=NULL)R=O->pohon->roztec;//rozteč palců řetězu
	double KR=0;//kalibrace řetězu vůči podvozku např. 0 - střed, -DP/2 - začátek, DP/2 - konec, či libovolný v m od začátku podvozku
	TPointD S;S.x=X;S.y=Y;//Start
	TPointD K;K.x=X+DD;K.y=Y;//Konec
																 //ShowMessage("R="+AnsiString(R)+"Rz="+AnsiString(M+DV)+"Rx="+AnsiString(F->m.round((M+DV)/R)));
	////obrys objektu
	//pero+výplň
	canv->Brush->Color=clWhite;
	canv->Brush->Style=bsSolid;
	canv->Pen->Color=clRed;        //pův. 0.5 bez duble linie
	canv->Pen->Width=Form1->Zoom*0.2;//if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*0.1;
	//samotné vykreslení obrysu kabiny, dvojitou linii, ale pozor může být nepříjemné ve vykreslování celkového layoutu!!!
	short Ov=Form1->Zoom*0.4;
	canv->Rectangle(m.L2Px(X)-Ov,m.L2Py(Y+sJ/2)-Ov,m.L2Px(K.x)+Ov,m.L2Py(K.y-sJ/2)+Ov);//dvojitý rám - vnější
	canv->Rectangle(m.L2Px(X)+Ov,m.L2Py(Y+sJ/2)+Ov,m.L2Px(K.x)-Ov,m.L2Py(K.y-sJ/2)-Ov);//dvojitý rám - vnitřní
	//canv->Rectangle(m.L2Px(X),m.L2Py(Y+SV/2),m.L2Px(K.x),m.L2Py(K.y-SV/2));//jenom jednoduché orámování

	////vykreslení řetězu a palců řetězu
	if(O->pohon!=NULL && O->rezim==1)//řetez - je-li přiřazen pohon a jedná se kontinuální režim
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Width=F->Zoom*0.5;
		canv->MoveTo(m.L2Px(X),m.L2Py(Y));
		canv->LineTo(m.L2Px(K.x),m.L2Py(K.y));
		//palce, pokud je zadaná rozteč tak se vykreslí
		if(R>0)
		{
			//palce řetězu   //pozn. M+DV je Rz
			int Rx=F->m.round((M+DV)/R);//může být zaokrouhleno, protože musí vycházet celé číslo
													 //*O->pozice - používát jen v animaci, kvůli tomu, aby byl řetěz dostatečně dlouhý
			double startR=-(M+DV)+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu, DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			if(animace)startR=-(M+DV)*ceil(O->pozice)+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			unsigned int j=0;      //+R pouze grafická záležitost, aby na výstupu neořezávalo palce
			for(double i=startR;i<=DD+R;i+=R)
			{     //již využívám přemaskování bílým obdélníkem, nakonci této metody, zajišťuje lepší grafický efekt
				if(/*i>=0 && */Rx>0)//zobrazí se pouze ty, které jsou v objektu (řeší pro začátek, konec řeší podmínka, která je součástí for cyklu), druhá část podmínky je pouze ošetření, což paralelně řeší i výjimka
				{
					try//ošetření situaci při real-time nastavování parametrů, tak v situacích, kdy nebyly, ještě hodnoty od uživatele dopsány a přepočítány, Rx bylo 0
					{
						if(j%Rx==0){vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,true);}//palec vyšel do rozestupu, jedná se o aktivní palec
						else vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,false);//jinak pasivní
					}
					catch(...){;}
				}
				j++;//musí být mimo
			}
		}
	}

	////jednotlivé pozice/vozíky
	unsigned int RET;
	if(!animace)RET=vykresli_pozice(canv,ceil(O->pozice)/*bylo pro číslování od jedné: 1*/,S,K,DD,dJ,sJ,dP,M,Poffset);
	else RET=vykresli_pozice(canv,ceil(O->pozice)*2/*bylo pro číslování od jedné: -O->pozice+1*/,S,K,DD,dJ,sJ,dP,M,-(M+DV)*ceil(O->pozice)+Poffset);

	////maskování vstupu a výstup, řetězu, pokud budu chtít,
	//aby byly vidět vstupující a vystupující vozíky musím přesunout před "////jednotlivé pozice/vozíky"  ,takto nyní nejsou vidět
//	canv->Brush->Color=clWhite;
//	canv->Brush->Style=bsSolid;      //+1 pouze rozšíření přes indexaci vozíků
//	canv->FillRect(TRect(0,m.L2Py(Y+sJ/2+1)-Ov,m.L2Px(X)-Ov-Ov/2,m.L2Py(K.y-sJ/2)+Ov));//nalevo
//	canv->FillRect(TRect(m.L2Px(K.x)+Ov+Ov/2,m.L2Py(Y+sJ/2+1)-Ov,Form_objekt_nahled->Width*3,m.L2Py(K.y-sJ/2)+Ov));//napravo

	return RET;//vrátí index
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslení pozic v layoutu + příprava konstrukce když nebudu chtít vykreslovat objekt vodorovně, pouze bude nutné zajistit ještě rotaci pozic a podvozků
unsigned int Cvykresli::vykresli_pozice(TCanvas *canv,int i,TPointD OD, TPointD DO,double delka,double delkaV,double sirkaV,double delkaP,double mezera,double akt_pozice)
{
	//grafické nastavení
	canv->Brush->Color=clWhite;//pozadí textu a podvozku a jigu
	canv->Brush->Style=bsClear;//transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Pen->Width=1/3.0*F->Zoom;

	//font
	rotace_textu(canv,0);
	short SF_puv=canv->Font->Size;//uchování původní velikosti textu
	canv->Font->Name="Arial";
	canv->Font->Style = TFontStyles();
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;

	//vykreslování jednotlivých obdelníčků - pozic
	TPointD S;S=OD;//konstrukce proto,když nebudu chtít vykreslovat objekt vodorovně, pouze bude nutné zajistit ještě rotaci pozic a podvozků
	S.x+=(DO.x-OD.x)*akt_pozice/delka;//sřed pozice
	S.y+=(DO.y-OD.y)*akt_pozice/delka;//střed pozice
	double PO=delkaV/delka;if(delkaV<delkaP)PO=delkaP/delka;//pomer užitné delky vozíku a delky objektu
	double POm=mezera/delka;//poměr délky mezery a délky objekty
	while(akt_pozice<=delka)
	{
		//volání komplexního vykreslení jednoho vozíku (podvozek včetně jigu)
		vykresli_vozik(canv,i,S.x,S.y,delkaP,delkaV,sirkaV);
		//posun o další vozík
		S.x+=(DO.x-OD.x)*PO;//posun ze začátku objektu nakonec
		S.y+=(DO.y-OD.y)*PO;//posun ze začátku objektu nakonec
		//posun o mezeru
		S.x+=(DO.x-OD.x)*POm;//posun ze začátku objektu nakonec
		S.y+=(DO.y-OD.y)*POm;//posun ze začátku objektu nakonec
		akt_pozice+=delkaV+mezera;//posun na další pozici
		//dekrementace
		i--;
	}
	canv->Font->Size=SF_puv;//vrátí původní velikost fontu
	return i;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslení jednoho komplexního vozíku (podvozek včetně jigu)
void Cvykresli::vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dP,double dJ,double sJ,double rotaceP,double rotaceJ,TColor clChassis, TColor clJig)
{
	float sP=0.12;//šířka podvozku, pouze stanovane

	//transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Brush->Style=bsClear;

//	////zvýraznění animovaného dle CT - zlobilo při provizorním zobrazování v náhledu, proto provizorně odstaveno, F->MOD!=F->NAHLED nepomohlo, divné
//	if(ID==ceil(Form_objekt_nahled->pom->pozice) && Form_objekt_nahled->Timer_animace->Enabled && F->MOD!=F->NAHLED)//v případě animace zvýrazní pozici, pro kterou se čítá technologický čas
//	{
//		canv->Pen->Width=3/3.0*F->Zoom;canv->Pen->Color=clWebOrange;//nebo lze použít: clYellow;
//		canv->Rectangle(m.L2Px(X-dP/2),m.L2Py(Y+sP),m.L2Px(X+dP/2),m.L2Py(Y-sP));//podvozek
//		canv->Rectangle(m.L2Px(X-dJ/2),m.L2Py(Y+sJ/2.0),m.L2Px(X+dJ/2),m.L2Py(Y-sJ/2.0));//jig
//		canv->Pen->Width=1/3.0*F->Zoom;
//	}

	////podvozek
	canv->Pen->Color=clChassis;
	canv->Pen->Width=m.round(1/3.0*F->Zoom);
	canv->Rectangle(m.L2Px(X-dP/2.0),m.L2Py(Y+sP),m.L2Px(X+dP/2.0),m.L2Py(Y-sP));//vykreslení pozice podvozku

	////jig
	canv->Pen->Color=clJig;
	canv->Pen->Width=m.round(2/3.0*F->Zoom);
	if(rotaceJ==90 || rotaceJ==270) canv->Rectangle(m.L2Px(X-sJ/2.0),m.L2Py(Y+dJ/2.0),m.L2Px(X+sJ/2.0),m.L2Py(Y-dJ/2.0));
	else canv->Rectangle(m.L2Px(X-dJ/2.0),m.L2Py(Y+sJ/2.0),m.L2Px(X+dJ/2.0),m.L2Py(Y-sJ/2.0));

//	////text - ID vozíku
//	//framing
//	if(Form1->Zoom>10)//pokud je více přiblížený objekt, tak se používá pouze framing, jinak bílé pozadí, pro lepší přehlednost
//	{
//		canv->Font->Color=clWhite;
//		canv->Font->Style = TFontStyles()<<fsBold;//vypnutí tučného písma
//		canv->Font->Size=Form1->Zoom*(4+1); if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*(5+1);
//		canv->TextOutW(m.L2Px(X)-canv->TextWidth(ID)/2.0,m.L2Py(Y+sJ/2.0)-canv->TextHeight(ID)/2.0,ID);//indexace pozice v rámci objektu
//	}
//	//samotný text
//	if(Form1->Zoom<=10)canv->Brush->Style=bsSolid;//pokud je více přiblížený objekt, tak se používá pouze framing, jinak bílé pozadí, pro lepší přehlednost//bez bílého pozadí toto zrušit/zakomentovat pokud bych chtěl bílý framing, ten jsem dělal pomocí tučného písma a fontu o 1pt větší
//	canv->Font->Color=clJig;
//	canv->Font->Style = TFontStyles();//vypnutí tučného písma
//	canv->Font->Size=Form1->Zoom*4; if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*5;
//	canv->TextOutW(m.L2Px(X)-canv->TextWidth(ID)/2.0,m.L2Py(Y+sJ/2.0)-canv->TextHeight(ID)/2.0,ID);//indexace pozice v rámci objektu
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajistí vykreslení řetězz, XY -umístění L začátek (střed dopravníku) objektu v metrech, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu, za zmínění stojí lokální proměnná této metody KR, což je kalibrace řetězu vůči podvozku např. 0 - střed, -DP/2 - začátek, DP/2 - konec, či libovolný v m od začátku podvozku
//za zmínění stojí lokální proměnná KR, což je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, DV - konec,
void Cvykresli::vykresli_retez(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset,bool animace)
{
	////vychozí geometrické proměnné
	//řetěz
	double DD=100;O->delka_dopravniku;//délka objektu v metrech
	//double M=O->mezera;//mezera
	double R=O->pohon->roztec;//rozteč palců řetězu
	double Rz=O->pohon->Rz;//rozestup
	int Rx=m.round(O->pohon->Rx);//může být zaokrouhleno, protože musí vycházet celé číslo
	short Rezim=O->rezim;//podle eID prvního použitého robota je nastaven ve stejnojmenné metodě režim objektu
	double KR=0;//kalibrace řetězu vůči podvozku např. 0 - střed, -DP/2 - začátek, DP/2 - konec, či libovolný v m od začátku podvozku
	TPointD S;S.x=X;S.y=Y;//Start
	TPointD K;K.x=X+DD;K.y=Y;//Konec
	//vozíková data - v případě nevykreslení vozíku zde monžno odstranit
	double dJ=m.UDJ(v.PP.delka_jig,v.PP.sirka_jig,O->orientace);//délka jigu
	double sJ=m.UDJ(v.PP.sirka_jig,v.PP.delka_jig,O->orientace);//šířka jigu a tím pádem i minimální kabiny
	double dP=v.PP.delka_podvozek;
	double DV=dJ;if(dP>dJ)DV=dP;
																	 //ShowMessage("R="+AnsiString(R)+"Rz="+AnsiString(Rz)+"Rx="+AnsiString(Rx));
	////obrys objektu
	//pero+výplň
	canv->Brush->Color=clWhite;
	canv->Brush->Style=bsSolid;

	////vykreslení řetězu a palců řetězu
//	if(O->pohon!=NULL)//řetez - je-li přiřazen pohon
//	{

		//vykreslí samotný pohon - spojnici
		linie(canv,m.L2Px(X),m.L2Py(Y),m.L2Px(K.x),m.L2Py(K.y),F->Zoom*0.5,clBlack);

		//palce, pokud je zadaná rozteč tak se vykreslí
		if(R>0)
		{                    // F->Memo3->Lines->Add(Poffset);
													 //*O->pozice - používát jen v animaci, kvůli tomu, aby byl řetěz dostatečně dlouhý
			double startR=-(Rz)+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu, DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			if(animace)startR=-(Rz)*20/**ceil(O->pozice)*/+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			unsigned int j=0;      //+R pouze grafická záležitost, aby na výstupu neořezávalo palce
			for(double i=startR;i<=DD+R;i+=R)
			{     //již využívám přemaskování bílým obdélníkem, nakonci této metody, zajišťuje lepší grafický efekt
				if(/*i>=0 && */Rx>0 || Rezim!=1)//zobrazí se pouze ty, které jsou v objektu (řeší pro začátek, konec řeší podmínka, která je součástí for cyklu), druhá část podmínky je pouze ošetření, což paralelně řeší i výjimka
				{
					try//ošetření situaci při real-time nastavování parametrů, tak v situacích, kdy nebyly, ještě hodnoty od uživatele dopsány a přepočítány, Rx bylo 0
					{
						if(Rezim==1)//pro kontinuální zobrazení
						{
							if(j%Rx==0)//palec vyšel do rozestupu, jedná se o aktivní palec unášející vozík
							{
								vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,true);
								/////////////////////provizorně zjištění aktuální rotace
								int rotace=0;
								Cvektory::TElement *E=O->elementy;//nastaveno na hlavičku, ošetřeno níže
								while(E!=NULL)
								{
									if((E->eID==3 || E->eID==4 || E->eID==5 || E->eID==6 || E->eID==9 || E->eID==10 || E->eID==13 || E->eID==14 || E->eID==17 || E->eID==18 || E->eID==103 || E->eID==104 || E->eID==107 || E->eID==108) && E->X<=X+i)//jedná se o roto element a je před aktuálně vykreslovaným vozíkem
									{
										 rotace+=E->rotace_jig;
									}
									E=E->dalsi;
								}
								E=NULL; delete E;
								/////////////////////
								vykresli_vozik(canv,j,X+i,Y,dP,dJ,sJ,0,m.Rt90(rotace));//provizorně
							}
							else vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,false);//jinak pasivní
						}
						else vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,false);//u S&G jakýkoliv
					}
					catch(...){;}
				}
				j++;//musí být mimo
			}
		}
//	}


	////jednotlivé pozice/vozíky
//	unsigned int RET;
//	if(!animace)RET=vykresli_pozice(canv,ceil(O->pozice)/*bylo pro číslování od jedné: 1*/,S,K,DD,dJ,sJ,dP,M,Poffset);
//	else RET=vykresli_pozice(canv,ceil(O->pozice)*2/*bylo pro číslování od jedné: -O->pozice+1*/,S,K,DD,dJ,sJ,dP,M,-(M+DV)*ceil(O->pozice)+Poffset);

	////maskování vstupu a výstup, řetězu, pokud budu chtít,
	//aby byly vidět vstupující a vystupující vozíky musím přesunout před "////jednotlivé pozice/vozíky"  ,takto nyní nejsou vidět
//	canv->Brush->Color=clWhite;
//	canv->Brush->Style=bsSolid;      //+1 pouze rozšíření přes indexaci vozíků
//	canv->FillRect(TRect(0,m.L2Py(Y+sJ/2+1)-Ov,m.L2Px(X)-Ov-Ov/2,m.L2Py(K.y-sJ/2)+Ov));//nalevo
//	canv->FillRect(TRect(m.L2Px(K.x)+Ov+Ov/2,m.L2Py(Y+sJ/2+1)-Ov,Form_objekt_nahled->Width*3,m.L2Py(K.y-sJ/2)+Ov));//napravo

//	return RET;//vrátí index
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_retez(TCanvas *canv,Cvektory::TObjekt *O)//sloučit s výše uvedenou metodou
{
	if(O!=NULL && O->elementy!=NULL)
	{
		TPoint *POLE=new TPoint[O->elementy->predchozi->n*3+1];
		Cvektory::TElement *E=O->elementy->dalsi;
		long i=-1;//počítadlo aktuálního vkládaného bodu
		while(E!=NULL)
		{
			//plnění do pole
			POLE[++i]=TPoint(m.L2Px(E->geo.X1),m.L2Py(E->geo.Y1));
			POLE[++i]=TPoint(m.L2Px(E->geo.X2),m.L2Py(E->geo.Y2));
			POLE[++i]=TPoint(m.L2Px(E->geo.X3),m.L2Py(E->geo.Y3));
			if(E->n==O->elementy->predchozi->n)//poslední bod se bere pouze v případě posledního segmentu, jinak je poslední bod totožný s prvním následujícího segmentu, takže se zbytečně nepoužívá, vyplývá z principu algoritmu bézierovy křivky
			POLE[++i]=TPoint(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
			//ukazatelové záležitosti
			E=E->dalsi;//posun na další element
		}
		delete E;E=NULL;//smazání již nepotřebného ukazatele
		if(O->pohon==NULL)canv->Pen->Width=1;//pokud není pohon přiřazen, tak jen elementární osa
		else canv->Pen->Width=m.round(F->Zoom*0.5);//pokud není přiřazen
		canv->Pen->Color=clBlack;
		canv->PolyBezier(POLE,O->elementy->predchozi->n*3);
		delete[]POLE;POLE=NULL;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////zajišťuje vykreslení simulace
//void Cvykresli::vykresli_simulaci(TCanvas *canv)
//{
//	//vykreslení linky
//	vykresli_linku(canv);
//
//	//vozíky
//	Cvektory::TVozik *ukaz1;
//	ukaz1=v.VOZIKY->dalsi;//přeskočí hlavičku
//	while (ukaz1!=NULL)
//	{
//			umisti_vozik(canv,ukaz1);
//			ukaz1=ukaz1->dalsi;//posun na další prvek
//	}
//
//	//vykreslení palce
//	Cvektory::TPalec *ukaz;
//	ukaz=v.PALCE->dalsi;//přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//			umisti_palec(canv,ukaz);
//			ukaz=ukaz->dalsi;//posun na další prvek
//	}
//
//	//vypis
//	canv->Brush->Color=clWhite;
//	double sec=cas*Form1->Timer_simulace->Interval/1000.0;
//	short h=floor(sec/3600);sec=sec-h*3600;
//	short m=floor(sec/60);sec=sec-m*60;
//	AnsiString H=h; if (h<10)H="0"+H;//pouze přidání 0 prefix
//	AnsiString M=m; if (m<10)M="0"+M;//pouze přidání 0 prefix
//	canv->Pen->Color=clGrayText;
//	canv->TextOutW(10,30,H+":"+M+":"+AnsiString(sec));
//
//	//ukončení simulace
//	//Form1->Timer_simulace->Enabled=false;
//	//sound();
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_linku(TCanvas *canv)//zajišťuje vykreslení osy linky
//{
//		//nastavení šířky a barvy linie
//		canv->Pen->Color=(TColor)RGB(255,0,0);
//		canv->Pen->Width=1*Form1->Zoom;
//		canv->Pen->Mode=pmCopy;
//
//		//samotné vykreslení
//		Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			//počátek úsečky
//			canv->MoveTo(m.L2Px(ukaz->X)+O_width*Form1->Zoom/2,m.L2Py(ukaz->Y)+O_height*Form1->Zoom/2);
//
//			//konec úsečky
//			if(ukaz->dalsi!=NULL)
//			canv->LineTo(m.L2Px(ukaz->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(ukaz->dalsi->Y)+O_height*Form1->Zoom/2);
//			else//pro spojnici z posledního bodu do prvního
//			canv->LineTo(m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2);
//
//			//posun na další prvek v seznamu
//			ukaz=ukaz->dalsi;
//		}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::umisti_vozik(TCanvas *canv,Cvektory::TVozik *ukaz)//zajišťuje umístění vozíku na lince
//{
//	STOPKA(ukaz);
//	SG(ukaz);
//	if(ukaz->predchozi!=NULL)
//	if(KOLIZE(ukaz,ukaz->predchozi))Form1->Memo1->Lines->Add("KOLIZE!");
//
//
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//
//	if(ukaz->stav==1 && cas%10==0)ukaz->stav=2;//pokud vozík čeká na palec a palec přijde nastaví stav na zelenou
//	if(ukaz->stav==2)ukaz->pozice+=1;//pokud je povolen pohyb daného vozíku
//	double rest=0;
//
//	if(u_seg!=NULL)
//	{
//		double X1=m.L2Px(u_seg->X)+O_width*Form1->Zoom/2;
//		double Y1=m.L2Py(u_seg->Y)+O_height*Form1->Zoom/2;
//		double X2=0;double Y2=0;
//
//		if(u_seg->dalsi!=NULL)
//		{
//			X2=m.L2Px(u_seg->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(u_seg->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//		else//pro poslední linii
//		{
//			X2=m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//
//		double delka=m.delka(X1,Y1,X2,Y2);
//		double krok=1.0;//metrů optimálně by měl odpovídat 1px, aby nebylo znatelné problikávání při animaci
//
//		//smazání starého čtverec-vozíku
//		if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round((ukaz->pozice-1)*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round((ukaz->pozice-1)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),false);
//
//		//zajistit překreslení umazávaného gridu, toto přimo ještě nefunguje //vykresli_grid(canv);
//
//		//nakreslí nový čtverec-vozík
//		if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round(ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round(ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),true);
//
//		//posun na další prvek v seznamu
//		if(krok*(ukaz->pozice+1)*Form1->Zoom>delka)
//		{
//			//výpis zbytkukrok*ukaz->pozice*Form1->Zoom
//			//canv->TextOutW(50,50,AnsiString(krok*ukaz->pozice*Form1->Zoom)+";"+AnsiString(delka));
//			if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round(ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round(ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),false);//v případě přechodu na další linii smazání starého
//			rest=delka-(krok*(ukaz->pozice+1)*Form1->Zoom);//rest z minulé úsečky
//			ukaz->pozice=rest;ukaz->segment=u_seg->dalsi;
//		}
//	}
//	else//pokračování do dalšího kola
//	{
//		//pokračování do dalšího kola
//		ukaz->pozice=rest;//rest z minulého kola
//		ukaz->segment=v.OBJEKTY->dalsi;
//		umisti_vozik(canv,ukaz);//nutná rekurze zajišťující, aby se nepřišlo o jeden krok simulace
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_vozik(TCanvas *canv,Cvektory::TVozik *ukaz,long X,long Y,bool NEW=true)//zajišťuje vykreslení vozíku při simulaci
//{
//	//záměrná záměna z důvodu špatně navrženého (o 90° orotovaného) výpočtu souřadnic vozíku
//	double delka=ukaz->sirka;
//	double sirka=ukaz->delka;
//
//	//nastavení stylu vykreslování
//	canv->Pen->Width=0;
//	canv->Pen->Mode=pmCopy;
//	if(NEW)
//	{
//		canv->Pen->Color=(TColor)RGB(0,0,0);
//		canv->Brush->Style=bsSolid;
//		canv->Brush->Color=ukaz->barva;
//	}
//	else
//	{
//		canv->Pen->Color=(TColor)RGB(255,255,255);
//		canv->Brush->Style=bsSolid;
//		canv->Brush->Color=clWhite;
//	}
//
//	//příprava souřadnic bodů polygonu dle rotace
//	double size=sqrt(pow(Form1->Zoom/Form1->m2px*delka/2,2)+pow(Form1->Zoom/Form1->m2px*sirka/2,2));
//	double A=0;
//	if(ukaz->segment->dalsi!=NULL)//mimo poslední spojnice
//		A=m.azimut(ukaz->segment->X,ukaz->segment->Y,ukaz->segment->dalsi->X,ukaz->segment->dalsi->Y);
//	else// pro poslední spojnici
//		A=m.azimut(ukaz->segment->X,ukaz->segment->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y);
//	A+=ukaz->rotace;//přídání rotace vozíku k azimut segmentu
//	double U=m.uhel(X,Y,X-delka/2,Y-sirka/2);
//	POINT body[4]=
//	{
//		{m.round(X+m.rotace(1,360-U,A).x*size),m.round(Y+m.rotace(1,360-U,A).y*size)},
//		{m.round(X+m.rotace(1,180-(360-U),A).x*size),m.round(Y+m.rotace(1,180-(360-U),A).y*size)},
//		{m.round(X+m.rotace(1,180-U,A).x*size),m.round(Y+m.rotace(1,180-U,A).y*size)},
//		{m.round(X+m.rotace(1,U,A).x*size),m.round(Y+m.rotace(1,U,A).y*size)}
//	};
//	canv->Polygon((TPoint*)body,3);
//
//	//prozatim, potom budu ukladat celé vnější souřadnice
//	ukaz->X=m.P2Lx(X);ukaz->X=m.P2Ly(Y);
//
//	//provizorní vykreslení středu (pomocí uhlopříček)
//	canv->MoveTo(body[0].x,body[0].y);canv->LineTo(body[2].x,body[2].y);
//	canv->MoveTo(body[1].x,body[1].y);canv->LineTo(body[3].x,body[3].y);
//	if(ukaz->n==1)
//	{
//	canv->TextOutW(50,110,AnsiString(ukaz->pozice));
//	canv->TextOutW(50,130,AnsiString(v.PALCE->dalsi->pozice));
//	}
//
//	//provizorní výpis čísel rohu polygonu
//	/*canv->TextOutW(body[0].x,body[0].y,"0");
//	canv->TextOutW(body[1].x,body[1].y,"1");
//	canv->TextOutW(body[2].x,body[2].y,"2");
//	canv->TextOutW(body[3].x,body[3].y,"3");*/
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////přidá nový palec do seznamu PALCE s umístěním přímo na linku dle stanovené rozteče
//void Cvykresli::priprav_palce()
//{
//	double roztec=10.0;//metr;
//	double i=0.0;//krok pozice palců;
//	double delka=0.0;
//	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//		if(ukaz->dalsi!=NULL)delka=m.delka(ukaz->X,ukaz->Y,ukaz->dalsi->X,ukaz->dalsi->Y);//mimo posledního segmentu
//		else delka=m.delka(ukaz->X,ukaz->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y);// pro poslední segment
//		while(i<=delka)
//		{
//			v.vloz_palec();
//			v.PALCE->predchozi->pozice=i/Form1->m2px;
//			v.PALCE->predchozi->segment=ukaz;
//			i+=roztec;
//		}
//		//ShowMessage(AnsiString(delka)+" | "+AnsiString(i));
//		i-=delka;//rest zbytek chybějící do celé délky řetězu se přenese/zohlední do dalšího segmentu
//		//ShowMessage(AnsiString(i));
//		ukaz=ukaz->dalsi;//posun na další prvek v seznamu
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////zajišťuje aktuální umístění vozíku na lince vůči animaci
//void Cvykresli::umisti_palec(TCanvas *canv,Cvektory::TPalec *ukaz)
//{
//	double rest=0.0;
//  ukaz->pozice+=1.0;
//	//přesunoto z konce:ukaz->pozice+=1; //pozice na dílčí úsečce
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//	if(u_seg!=NULL)
//	{
//		double X1=m.L2Px(u_seg->X)+O_width*Form1->Zoom/2;
//		double Y1=m.L2Py(u_seg->Y)+O_height*Form1->Zoom/2;
//		double X2=0;double Y2=0;
//
//		if(u_seg->dalsi!=NULL)
//		{
//			X2=m.L2Px(u_seg->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(u_seg->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//		else//pro poslední linii
//		{
//			X2=m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//
//		double delka=m.delka(X1,Y1,X2,Y2);
//		double krok=1.0;//metrů optimálně by měl odpovídat 1px, aby nebylo znatelné problikávání při animaci
//
//		//smazání starého palec
//		if(ukaz->pozice>-1)vykresli_palec(canv,(ukaz->pozice-1)*Form1->Zoom*(X2-X1)/(delka/krok)+X1,(ukaz->pozice-1)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,false);
//
//		//nakreslí nový palec
//		if(ukaz->pozice>-1)vykresli_palec(canv,ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1,ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,true);
//
//		//posun na další prvek v seznamu (na další zub
//		if(krok*(ukaz->pozice+1)*Form1->Zoom>delka)
//		{
//			if(ukaz->pozice>-1)
//			vykresli_palec(canv,(ukaz->pozice)*Form1->Zoom*(X2-X1)/(delka/krok)+X1,(ukaz->pozice)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,false);//v případě přechodu na další linii smazání starého
//			rest=delka-(krok*ukaz->pozice+1*Form1->Zoom);//rest z minulé úsečky
//			ukaz->pozice=rest;ukaz->segment=u_seg->dalsi;
//		}
//	}
//	else//pokračování do dalšího kola (na další segment
//	{
//		//pokračování do dalšího kola
//		ukaz->segment=v.OBJEKTY->dalsi;
//		ukaz->pozice=rest;//přiřadí restovou hodnotu z minulého kola
//		umisti_palec(canv,ukaz);//nutná rekurze zajišťující, aby se nepřišlo o jeden krok simulace
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje samotné vykreslení palce, parametr NEW rozlišuje nový palec a palace starý již ke smazání (to slouží pro simulaci), poslední parametr značí, zda palec označit jako aktivní
void Cvykresli::vykresli_palec(TCanvas *canv,double X,double Y,bool NEW,bool ACTIVE)
{                    //pokud se jedná o aktivní palec ještě přičte jedničku
	double size=0.7*Form1->Zoom;
	if(NEW)//nový palace
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		if(ACTIVE)canv->Brush->Color=clRed;
		else canv->Brush->Color=clBlack;
		canv->Pen->Width=1;
		canv->Ellipse(m.round(X-size),m.round(Y-size),m.round(X+size),m.round(Y+size));
	}
	else//tento smaže starý - slouží pro simulaci
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Pen->Color=clWhite;
		canv->Brush->Color=clWhite;
		canv->Pen->Width=1;
		canv->Ellipse(m.round(X-size),m.round(Y-size),m.round(X+size),m.round(Y+size));
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//celková vykreslovací metoda, vykreslí buď stopku, robota nebo otoč
void Cvykresli::vykresli_element(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2,double LO_pozice)
{
	rotace=m.Rt90(rotace);
	switch(eID)
	{
		case 0: vykresli_stopku(canv,X,Y,name,short_name,typ,rotace,stav);break;//stopka
		case 1: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 2: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 3: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 4: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 5: vykresli_otoc(canv,X,Y,name,short_name,eID,typ,rotace,stav);break;//pasivní otoč
		case 6: vykresli_otoc(canv,X,Y,name,short_name,eID,typ,rotace,stav);break;//aktivní otoč
		case 7: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 8: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 9: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 10: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 11: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 12: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 13: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 14: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 15: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 16: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 17: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 18: vykresli_robota(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 100:vykresli_ion(canv,X,Y,name,short_name,typ,rotace,stav,F->ROst);break;//ion tyč
		case 101:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot
		case 102:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot se stop stanicí
		case 103:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2);break;//lidský robot s pasivní otočí
		case 104:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot s aktivní otočí (resp. s otočí a stop stanicí)
		case 105:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot ionizace
		case 106:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot  ionizace se stop stanicí
		case 107:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2);break;//lidský robot  ionizace s pasivní otočí
		case 108:vykresli_cloveka(canv,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0);break;//lidský robot  ionizace s aktivní otočí (resp. s otočí a stop stanicí)
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,double rotace,short stav)
{
	double Z=Form1->Zoom;
	short size=m.round(8*F->Zoom); if(stav==2)size=m.round(9*F->Zoom);
	short sklon=50;
	rotace=m.Rt90(rotace+180);//kvůli převrácenému symbolu

	//barva výplně
	TColor barva=clRed;if(stav==0)barva=(TColor)RGB(60,179,113);//zelená světlejší(TColor)RGB(50,205,50);
	if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	//typ zobrazení
	if(typ==-1)//kurzor
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		size=6*F->Zoom;//kvůli bílému orámování stopky v normálním zobrazení musí být menší
	}//normální
	else
	{
		canv->Pen->Color=clWhite;//bílý framing okolo stopky
		canv->Pen->Width=m.round(0.6*Z);
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Brush->Color=barva;
		canv->Brush->Style=bsSolid;
	}
	//rotace
	switch((int)rotace)//tento switch pouze posun referenčního bodu kvůli bílému orámování
	{
		case 0: 	Y+=m.round(1*Z);break;
		case 90: 	X-=m.round(1*Z/2.0);break;
		case 180: Y-=m.round(1*Z);break;
		case 270: X+=m.round(1*Z/2.0);break;
	}
	//referenční bode ve špičce, špička je směrem dolu (při nulové rotaci)
	float sizeX=size;float sizeY=size;
	if(rotace==0 || rotace==180)sizeX/=2.0;
	if(rotace==90 || rotace==270)sizeY/=2.0;
	POINT body[3]={{F->m.round(X+m.rotace(1,sklon,rotace).x*sizeX),F->m.round(Y+m.rotace(1,sklon,rotace).y*sizeY)},{X,Y},{F->m.round(X+m.rotace(1,360-sklon,rotace).x*sizeX),F->m.round(Y+m.rotace(1,360-sklon,rotace).y*sizeY)}};
	canv->Polygon((TPoint*)body,2);

	//text
	if(typ!=-1 && name!="")//v módu kurzor nebo pokud je součástí nadřazeného elementu se název nezobrazuje
	{
		canv->Font->Color=barva;
		canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
		canv->Font->Name=F->aFont->Name;
		canv->Font->Style = TFontStyles();
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		AnsiString T1="STOP";//short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		AnsiString T2=name.SubString(T1.Length()+1,name.Length());//pouze index stopky, +1 je mezera
		if(typ==1)//normální zobrazení typ==1
		{
			if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;//zvýraznění
			short h=0,w=canv->TextWidth(T2);
			canv->Font->Style = TFontStyles();h=canv->TextHeight(T1);w+=canv->TextWidth(T1+" ");//pro náze normální písmo
			float zAA=1.0;if(F->antialiasing)zAA=3.0;
			long x,y;
			//rotace
			switch((int)rotace)//posun referenčního bodu kvůli bílému orámování
			{                                                                                                                                                                             //nechat duplicitně        //vypsání indexu stopky, v případě editace i tučně
				case 0: 	rotace_textu(canv,0+900); x=m.round(X-h/2.0);   		y=m.round(Y-size+2*Z);	aktOblast=TRect(m.round(x/zAA),m.round((y-w)/zAA),m.round((x+h)/zAA),m.round(y/zAA)); canv->TextOutW(x,y,T1+" ");if(stav==3){canv->Font->Style = TFontStyles()<< fsBold;rotace_textu(canv,0+900);}canv->TextOutW(x,y-canv->TextWidth(T1+" "),T2);break;
				case 90:	rotace_textu(canv,0);		  x=m.round(X+size-2*Z);		y=m.round(Y-h/2);     	aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA)); canv->TextOutW(x,y,T1+" ");if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;canv->TextOutW(x+canv->TextWidth(T1+" "),y,T2);break;
				case 180:	rotace_textu(canv,2700);  x=m.round(X+h/2.0);   		y=m.round(Y+size-2*Z);	aktOblast=TRect(m.round((x-h)/zAA),m.round(y/zAA),m.round(x/zAA),m.round((y+w)/zAA)); canv->TextOutW(x,y,T1+" ");if(stav==3){canv->Font->Style = TFontStyles()<< fsBold;rotace_textu(canv,2700);}canv->TextOutW(x,y+canv->TextWidth(T1+" "),T2);break;
				case 270:	rotace_textu(canv,0);	    x=m.round(X-w-size+2.0*Z);y=m.round(Y-h/2); 			aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA)); canv->TextOutW(x,y,T1+" ");if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;canv->TextOutW(x+canv->TextWidth(T1+" "),y,T2);break;
			}
			rotace_textu(canv,0);//vrací nastavení do původního stavu
		}
		else//ikona v knihovně elementů je text pod elementem
		{
			//canv->Font->Name="Arial";
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(F->m.round(X-canv->TextWidth(name)/2),Y,name);
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_robota(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2,double aP,float TS,double LO_pozice)
{
	try
	{
	double Z=F->Zoom;//zoom, pouze zkrácení zápisu

	//vstupní parametry - budou součástí parametrů metody  robotoa
	double LO=(LO1+OTOC_delka+LO2)*Z/F->m2px;//délka cekového lakovacího okna
	if(typ==0)LO=1.5*Z/F->m2px;//v knihovně se symbol robota zobrazuje s 1,5 m velkých lakovacím oknem vždy
	aP; if(aP>LO/2.0)aP=LO/2.0;if(aP<-LO/2.0)aP=-LO/2.0;//Aktuální Pozice odsazení tryskového kloubu i trysky (ve svislé poloze trysky) v lakovacím okně + ochrana proti přetečení
	TS;//Tryska Sklon stupně

	//konstanty
	double pLOvC=1/2.0;//část z LO v delší části (většinou v polovina),bude sloužit na vyosení LO mimo osu robota
	double DkRB=1.2*Z/F->m2px;//délka k referenčnímu bodu od středu prvního kloubu ramena, respektive odsazení robota od linky
	float tloustka_linie=1.05/3.0;if(stav==2)tloustka_linie*=1.3;//pokud má být zvýrazněn  //vykreslovací linie robota
	float zaobleni=4*Z;//míra zaoblení základny
	float sirka_zakladny=Robot_sirka_zakladny*Z/F->m2px;
	float delka_zakladny=Robot_delka_zakladny*Z/F->m2px;
	float sirka_ramena=0.2*Z/F->m2px;
	float DT=0.35*Z/F->m2px;//délka trysky
	float TW=sirka_ramena;//tryska šířka před zúžením, v místě z kloubu
	float TZ=TW/2.0;//tryska ve zúžení
	double DR=sqrt(pow(LO*pLOvC,2)+pow(DkRB-DT,2));//délka ruky/ramene obou částí (paže i předloktí) ///old model (tryska v extremní poloze stejný úhel jako rameno): double DR=(DkRB-DT)/cos(atan((LO*pLOvC)/DkRB));

	//nastavení barev
	TColor barva=clBlack; TColor clOzeh=(TColor)RGB(255,165,0);TColor clIon=(TColor)RGB(7,107,171);TColor clCO2=(TColor)RGB(135,206,250);
	if(stav==-1)//pokud je aktivní nebo neaktivní
	{
		barva=m.clIntensive(barva,180);
		clOzeh=m.clIntensive(clOzeh,180);
		clIon=m.clIntensive(clIon,180);
	}

  //rotace základny
	if(rotace==90 || rotace==270){float temp=delka_zakladny;delka_zakladny=sirka_zakladny;sirka_zakladny=temp;}

	//přidružené elementy
	long pX=X;long pY=m.round(Y-sirka_zakladny/2.0-DkRB);
	long lX=X;long lY=m.round(Y-sirka_zakladny/2.0);
	if(rotace==90){pX=m.round(X+delka_zakladny/2.0+DkRB);pY=Y;lX=m.round(X+delka_zakladny/2.0);lY=Y;}
	if(rotace==270){pX=m.round(X-delka_zakladny/2.0-DkRB);pY=Y;lX=m.round(X-delka_zakladny/2.0);lY=Y;}
	if(rotace==180){pX=X;pY=m.round(Y+sirka_zakladny/2.0+DkRB);lX=X;lY=m.round(Y+sirka_zakladny/2.0);}
	switch(eID)
	{
		case 1: case 7: case 11: case 15: if(typ==1)vykresli_lakovaci_okno(canv,lX,lY,LO1,0,0,DkRB,rotace,LO_pozice);break;//pokud se jedná o kontinuálního robota v normálním zobrazení, zobrazí se ještě lakovací okno
		case 2: case 8: case 12: case 16: vykresli_stopku(canv,pX,pY,"","",typ,m.Rt90(rotace+180),stav);break;//robot se stopkou
		case 3: case 9: case 13: case 17: if(typ==1)vykresli_lakovaci_okno(canv,lX,lY,LO1,OTOC_delka,LO2,DkRB,rotace,LO_pozice);vykresli_otoc(canv,pX,pY,"","",5,typ,rotace,stav);break;//s pasivní otočí
		case 4: case 10: case 14:case 18: vykresli_otoc(canv,pX,pY,"","",6,typ,m.Rt90(rotace+180),stav);break;//s aktivní otočí (tj. s otočí a se stopkou)
	}

	//nastavení pera
	TPenMode PenMode=pmCopy;
	if(typ==-1)//typ kurzor
	{
		PenMode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Color=barva;
		canv->Pen->Width=1;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Style=psSolid;
		canv->Pen->Width=F->m.round(tloustka_linie*Z);
		canv->Brush->Style=bsSolid;
	}
	canv->Pen->Mode=PenMode;
	canv->Pen->Color=barva;
	canv->Brush->Color=clWhite;

	////základna
	TRect zakladna=TRect(m.round(X-delka_zakladny/2.0),m.round(Y-sirka_zakladny/2.0),m.round(X+delka_zakladny/2.0),m.round(Y+sirka_zakladny/2.0));
	canv->RoundRect(zakladna,zaobleni,zaobleni);

	////tryska
	long cX=X-aP;long cY=m.round(Y-sirka_zakladny/2.0-DkRB+DT);//skutečný referenční bod (nikoliv uchopovací, ten je u robota odlišný) minus výška trysky
	if(rotace==90){cX=m.round(X+delka_zakladny/2.0+DkRB-DT);cY=Y-aP;}
	if(rotace==270){cX=m.round(X-delka_zakladny/2.0-DkRB+DT);cY=Y+aP;}
	if(rotace==180){cX=X+aP;cY=m.round(Y+sirka_zakladny/2.0+DkRB-DT);}
	//typ trysky dle typu robota 														//zde bude TS (tryska sklon) pro animaci či parametrizování
	if(1<=eID && eID<=4)polygonDleOsy(canv,cX,cY,DT,TW,TZ,270+TS,rotace,PenMode,barva,clWhite);//lakovací
	if(7<=eID && eID<=10){TPoint P=polygonDleOsy(canv,cX,cY,DT/1.5,TW,TZ,270+TS,rotace,PenMode,barva,clWhite);if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2.0,TW/2.0,TZ*4,270+TS,rotace,pmMask,clWhite,clIon);}//ion
	if(11<=eID && eID<=14){if(stav>0 && typ!=-1)polygonDleOsy(canv,cX,cY,DT,TW,TZ*6,270+TS,rotace,pmMask,clWhite,clOzeh);polygonDleOsy(canv,cX,cY,DT/2.0,TW,TZ*4,270+TS,rotace,PenMode,barva,clWhite);}//ožeh
	if(15<=eID && eID<=18){TPoint P=polygonDleOsy(canv,cX,cY,DT/1.5,TW,TZ,270+TS,rotace,PenMode,barva,clWhite);if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2.0,TW/2.0,TZ*4,270+TS,rotace,pmMask,clWhite,clCO2);}//CO2

	////ramena
	double Alfa1=atan((aP)/(DkRB-DT));
	double Prepona=sqrt(pow(aP,2)+pow(DkRB-DT,2));//v dokumentaci označeno jako X
	double Alfa2=acos(Prepona/DR);
	double Alfa=Alfa2-Alfa1;
	double Gama=180.0-(180.0-2*Alfa2)-Alfa;//dokumentace: Beta=180.0-2*Alfa2;Gama=180.0-Beta-Alfa;
	//rotace i kloubu
	pX=X;pY=Y-sirka_zakladny/2.0;
	if(rotace==90){pX=X+delka_zakladny/2.0;pY=Y;}
	if(rotace==270){pX=X-delka_zakladny/2.0;pY=Y;}
	if(rotace==180){pY=Y+sirka_zakladny/2.0;}
	//první část - od základny
	TPoint S=polygonDleOsy(canv,m.round(pX),m.round(pY),DR/2,sirka_ramena,sirka_ramena,270+m.ToDeg(Alfa),rotace,PenMode,barva,clWhite);//vykreslí polygon dle osy, umí i kónický tvar, vratí souřadnice konce osy polygonu
	//druhá část k trysce (kresleno směrem od trysky)
	polygonDleOsy(canv,cX,cY,DR/2,sirka_ramena,sirka_ramena,270+180-m.ToDeg(Gama),rotace,PenMode,barva,clWhite);

	////klouby
	//první kloub mezi prvním ramenem a základnou
	canv->Ellipse(m.round(pX-sirka_ramena/2.0),m.round(pY-sirka_ramena/2.0),m.round(pX+sirka_ramena/2.0),m.round(pY+sirka_ramena/2.0));
	//druhý kloub mezi rameny
	canv->Ellipse(m.round(S.x-sirka_ramena/2.0),m.round(S.y-sirka_ramena/2.0),m.round(S.x+sirka_ramena/2.0),m.round(S.y+sirka_ramena/2.0));
	//třetí kloub mezi ramenem a tryskou
	canv->Ellipse(m.round(cX-sirka_ramena/2.0),m.round(cY-sirka_ramena/2.0),m.round(cX+sirka_ramena/2.0),m.round(cY+sirka_ramena/2.0));

	//	//testovací osa - smazat
//	linie(canv,cX-100,cY,cX+100,cY,1,clRed);
//	linie(canv,X,Y,cX,cY,1,clRed);
	//name="Robot1";//prozatim
	//short_name="R1";//prozatim

	//text
	if(typ!=-1)//v módu kurzor se název nezobrazuje
	{              //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		//if(F->aFont->Size==12)canv->Font->Size=m.round(5.4*Z);else canv->Font->Size=m.round(5*Z);
		AnsiString T=short_name;
		//if(Z>4*3) //short_name odstaveno
		{T=name;if(F->aFont->Size==12)canv->Font->Size=m.round(2*Z); else canv->Font->Size=m.round(2.4*Z);}//od daného zoomu zobrazuje celý název
		if(typ==1)//pokud se jedná o standardní zobrazení
		{
			canv->Font->Name=F->aFont->Name;
			if(/*stav==2 || */stav==3)canv->Font->Style = TFontStyles()<< fsBold;//došlo k vybrání elementu-tato část odstavena nebo přímo jeho textu
			float zAA=1.0;if(F->antialiasing)zAA=3.0;
			long x,y;
			short h=canv->TextHeight(T);short w=canv->TextWidth(T);   //pozn. pro 180° neobracím text vzhůru nohama
			if(rotace==0 || rotace==180)//lze používat i drawRectText(canv,zakladna,T);//nefunguje správně při rotaci //pro po orototování o 180:canv->TextOutW(m.round(X+canv->TextWidth(T)/2.0),m.round(Y+canv->TextHeight(T)/2.0),name);
			{
				x=m.round(X-w/2.0);
				y=m.round(Y-h/2.0);
				aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));//souřadnice pro citelnou oblast
			}
			else
			{
				rotace_textu(canv,-rotace*10);
				if(rotace==90){x=m.round(X-h/2.0);y=m.round(Y-w/2.0);}
				if(rotace==270){x=m.round(X-h/2.0);y=m.round(Y-w/2.0);}
				aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+h)/zAA),m.round((y+w)/zAA));//souřadnice pro citelnou oblast
				if(rotace==90){x=m.round(X+h/2.0);y=m.round(Y-w/2.0);}//souřadnice vykreslení textu a souřadnice citelné oblasti nejsou totožné, nutné znova určit
				if(rotace==270){x=m.round(X-h/2.0);y=m.round(Y+w/2.0);}
			}
			canv->TextOutW(x,y,name);//samotný vypis
		}
		else//ikona
		{
			canv->Font->Name=F->aFont->Name;//musí tu být, jinak chyba popisku u prvního robota  //canv->Font->Name="Arial";
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(X-canv->TextWidth(name)/2,m.round(Y+sirka_zakladny/2.0+1*Z),name); //1 pouze korekce
			canv->TextOutW(X-canv->TextWidth(short_name)/2,m.round(Y+sirka_zakladny/2.0+1*Z+1*Z+canv->TextHeight(name)),short_name);
		}
	}
	}catch(...){if(DEBUG)MessageBeep(0);}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykresli siluetu člověk s případně přidruženým elememntem, rotuje po směru hodinových ručiček, pro animaci slouží okolo hraničních stupňu 0,90,180,270, vždy rozsah -45° až +44°, např. 45-134° je maximální pracovní rozsah pro člověka rotovaného na 90° atd.
void Cvykresli::vykresli_cloveka(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2)
{
	////změna orientace - pozor
	short rotace90=m.Rt90(rotace);
	if(rotace90==0 || rotace90==180)//kvůli převrácené kresbě ve výchozí pozici tj. 0°
	{
		rotace+=180;
		rotace=fmod(rotace,360.0);// včetně ošetření přetečení přes 360 stupňů
		if(rotace<0){rotace+=360;}//pro záporné hodnoty
		rotace90=m.Rt90(rotace);
	}
	rotace=rotace90-1*(rotace-rotace90);//kvůli převrácené orientaci rotace, kdy rotační metody rotují proti hodinovým ručičkám a naopak rotace elementu je po směru hodinových ručiček

	////proměnné mimo paramety metody
	double Z=F->Zoom;
	double oX=0.5665; double oY=-0.33425;//souřadnice těžiště hlavy - uchopovací bod, offset v souřadnicích, lze snadno změnit, pokud bych chtěl uchopovací bod jinde
	float  zAA=1.0;if(F->antialiasing)zAA=3.0;
	double DkRBpx=m.m2px(DkRB);//délka k referenčnímu bodu od uchopovacího bodu, respektive odsazení člověka od linky
	float  DT=m.m2px(0.35);//délka trysky
	float  TW=m.m2px(0.1);//tryska šířka před zúžením, v místě z kloubu
	float  TZ=TW/1.5;//tryska ve zúžení
	float  PS=25;//pistole sklon ve stupních, nechat na 25, jinak nutno změnit 41, ale dělalo problémy zajistit výpočet u plamenu

	////přidružené elementy
	long pX=X;long pY=m.round(Y-DkRBpx);
	short rotace2=0;
	if(rotace90==0)	 {pX=X;pY=m.round(Y+DkRBpx);}
	if(rotace90==90) {pX=m.round(X+DkRBpx);pY=Y;rotace2=180;}
	if(rotace90==270){pX=m.round(X-DkRBpx);pY=Y;rotace2=180;}
	if(typ==0)F->Zoom/=1.5;//pro případ ikony je třeba z důvodu zvětšení člověka zmenšit přidružený element
	switch(eID)
	{
		case 101: case 105: if(typ==1)vykresli_lakovaci_okno(canv,X,Y,LO1,0,0,DkRBpx,m.Rt90(rotace90+180+rotace2));break;//pokud se jedná o kontinuálního robota v normálním zobrazení, zobrazí se ještě lakovací okno
		case 102: case 106: vykresli_stopku(canv,pX,pY,"","",typ,m.Rt90(rotace90+rotace2),stav);break;//robot se stopkou
		case 103: case 107: if(typ==1)vykresli_lakovaci_okno(canv,X,Y,LO1,OTOC_delka,LO2,DkRBpx,m.Rt90(rotace90+180+rotace2));vykresli_otoc(canv,pX,pY,"","",5,typ,m.Rt90(rotace90+rotace2),stav);break;//s pasivní otočí
		case 104: case 108: vykresli_otoc(canv,pX,pY,"","",6,typ,m.Rt90(rotace90+rotace2),stav);break;//s aktivní otočí (tj. s otočí a se stopkou)
	}
	if(typ==0)F->Zoom*=1.5;//navrácení do původního stavu

	////nastavení barev
	TColor barva=clBlack;TColor clIon=(TColor)RGB(7,107,171);
	if(stav==-1){barva=m.clIntensive(barva,180);clIon=m.clIntensive(clIon,180);}//pokud je aktivní nebo neaktivní

	////nastavení pera
	float W=0.02;if(F->antialiasing)W*=3;
	TPenMode PenMode=pmCopy;
	if(typ==-1)//typ kurzor
	{
		PenMode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Style=psSolid;
		canv->Pen->Width=m.round(W*Z);if(stav==2)canv->Pen->Width*=2;//pokud má být zvýrazněn
		canv->Brush->Style=bsSolid;
	}
	canv->Pen->Mode=PenMode;
	canv->Pen->Color=barva;
	canv->Brush->Color=clWhite;

	////vytvoření dvou pracovních statických polí s maximálním rozsahem alokace, logických a fyzických souřadnic
	TPointD PL[49];TPoint PF[49];

	////souřadnice kresby a vykreslování jednotlivých částí siluety
	if(Z/zAA>=9)//boty se vykreslují až od daného zoomu
	{
		//pravá bota
		PL[0].x=0.375569775287343;PL[0].y=-0.3889123449191;PL[1].x=0.371860255677329;PL[1].y=-0.440639618371226;PL[2].x=0.373070181874266;PL[2].y=-0.463963074881838;PL[3].x=0.382399534818622;PL[3].y=-0.500749498347804;PL[4].x=0.388562240639585;PL[4].y=-0.525049565820684;PL[5].x=0.435996889747796;PL[5].y=-0.565413359326415;PL[6].x=0.449989963736543;PL[6].y=-0.542001450146329;PL[7].x=0.465669342636659;PL[7].y=-0.515768172518775;PL[8].x=0.470331724441489;PL[8].y=-0.490280224477424;PL[9].x=0.478;PL[9].y=-0.46;
		bezier(canv,PL,X,Y,oX,oY,rotace,9);
		//levá bota
		PL[0].x=0.760430224712657;PL[0].y=-0.3909123449191;PL[1].x=0.764139744322671;PL[1].y=-0.442639618371226;PL[2].x=0.762929818125734;PL[2].y=-0.465963074881838;PL[3].x=0.753600465181378;PL[3].y=-0.502749498347804;PL[4].x=0.747437759360415;PL[4].y=-0.527049565820684;PL[5].x=0.700003110252204;PL[5].y=-0.567413359326415;PL[6].x=0.686010036263457;PL[6].y=-0.544001450146329;PL[7].x=0.670330657363341;PL[7].y=-0.517768172518775;PL[8].x=0.665668275558511;PL[8].y=-0.492280224477424;PL[9].x=0.658;PL[9].y=-0.462;
		bezier(canv,PL,X,Y,oX,oY,rotace,9);
	}
	////ION pistole
	if(105<=eID && eID<=108)//ion pistole se zobrazuje pouze u ION robota
	{
		//pistole obrys
		PL[0].x=0.28875;PL[0].y=-0.68;PL[1].x=0.34;PL[1].y=-0.68;PL[2].x=0.34;PL[2].y=-0.9475;PL[3].x=0.32;PL[3].y=-0.975;PL[4].x=0.32;PL[4].y=-1.04375;PL[5].x=0.27;PL[5].y=-1.04375;PL[6].x=0.27;PL[6].y=-0.975;PL[7].x=0.25;PL[7].y=-0.9475;PL[8].x=0.25;PL[8].y=-0.7225;
		m.rotace_polygon(oX,oY,PL,8,rotace);
		if(PS!=0)m.rotace_polygon(PL[0].x,PL[0].y,PL,8,PS);//jen samotná pistole, pozn. optimalizaci takto jsem nerozchodil: if(PS!=0)m.rotace_polygon(oX,oY,PL[0].x,PL[0].y,PL,PF,8,PS);//jen samotná pistole
		for(int i=0;i<=8;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
		canv->Polyline(PF,8);
		//osa ion pistole
		if(Z/zAA>=5)
		{
			TPointD PL0=PL[0];
			PL[0].x=(0.32+0.27)/2.0;PL[0].y=-0.7225-0.02;PL[1].x=PL[0].x;PL[1].y=-1.04375-0.05;
			m.rotace_polygon(oX,oY,PL,1,rotace);
			if(PS!=0)m.rotace_polygon(PL0.x,PL0.y,PL,1,PS);//jen samotná pistole
			for(int i=0;i<=1;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
			canv->Polyline(PF,1);
		}
		//"plamen"                                                                                             //PS něměnit, jinak problémy s plamenem
		if(stav>0 && typ!=-1)polygonDleOsy(canv,(PF[4].x+PF[5].x)/2.0,(PF[4].y+PF[5].y)/2.0,DT/2.0,TW/2.0,TZ*4,PS+41+(rotace2-rotace)*2,rotace,pmMask,clWhite,clIon);//ion
	}
	//pravá ruka
	PL[0].x=0.145;PL[0].y=-0.605;PL[1].x=0.169761356440884;PL[1].y=-0.648;PL[2].x=0.17329076820559;PL[2].y=-0.653098039215686;PL[3].x=0.19485939565657;PL[3].y=-0.684470588235294;PL[4].x=0.200775079385735;PL[4].y=-0.693075219114079;PL[5].x=0.205084597834833;PL[5].y=-0.69613157037026;PL[6].x=0.216428023107551;PL[6].y=-0.699372549019608;PL[7].x=0.260349591735002;PL[7].y=-0.711921568627451;PL[8].x=0.259565278009511;PL[8].y=-0.712313725490196;PL[9].x=0.268408415264413;PL[9].y=-0.712666666666667;PL[10].x=0.277074590335663;PL[10].y=-0.713012545050885;PL[11].x=0.288192728989904;PL[11].y=-0.713882352941176;PL[12].x=0.303486846636962;PL[12].y=-0.713098039215686;PL[13].x=0.32703754096554;PL[13].y=-0.7118903113014;PL[14].x=0.316455232303218;PL[14].y=-0.69839669172869;PL[15].x=0.308192728989904;PL[15].y=-0.697019607843137;PL[16].x=0.284663317225198;PL[16].y=-0.693098039215686;PL[17].x=0.270545670166374;PL[17].y=-0.692705882352941;PL[18].x=0.259957434872256;PL[18].y=-0.684078431372549;PL[19].x=0.251347886559472;PL[19].y=-0.677063243858428;PL[20].x=0.227429956425431;PL[20].y=-0.657044033129204;PL[21].x=0.230545670166374;PL[21].y=-0.654274509803922;PL[22].x=0.23407508193108;PL[22].y=-0.651137254901961;PL[23].x=0.241526062323237;PL[23].y=-0.651529411764706;PL[24].x=0.256428023107551;PL[24].y=-0.652705882352941;PL[25].x=0.262695275296554;PL[25].y=-0.653200665420494;PL[26].x=0.269565278009511;PL[26].y=-0.659568627450981;PL[27].x=0.270937827029119;PL[27].y=-0.661725490196078;PL[28].x=0.27958973180111;PL[28].y=-0.671098387032402;PL[29].x=0.304663317225198;PL[29].y=-0.66878431372549;PL[30].x=0.292114297617354;PL[30].y=-0.656235294117647;PL[31].x=0.288209923548386;PL[31].y=-0.652330920048679;PL[32].x=0.284290739103446;PL[32].y=-0.647576439545949;PL[33].x=0.27329076820559;PL[33].y=-0.635843137254902;PL[34].x=0.267408415264413;PL[34].y=-0.62956862745098;PL[35].x=0.266891476519433;PL[35].y=-0.628397119265468;PL[36].x=0.231329983891864;PL[36].y=-0.615843137254902;PL[37].x=0.216666666666667;PL[37].y=-0.610666666666667;PL[38].x=0.216745098039216;PL[38].y=-0.606823529411764;PL[39].x=0.204;PL[39].y=-0.591333333333333;
	bezier(canv,PL,X,Y,oX,oY,rotace,39);
	//levá ruka
	PL[0].x=0.998333333333333;PL[0].y=-0.606666666666667;PL[1].x=0.974313725490196;PL[1].y=-0.648;PL[2].x=0.97078431372549;PL[2].y=-0.653098039215686;PL[3].x=0.94921568627451;PL[3].y=-0.684470588235294;PL[4].x=0.943300002545345;PL[4].y=-0.693075219114079;PL[5].x=0.938990484096247;PL[5].y=-0.69613157037026;PL[6].x=0.927647058823529;PL[6].y=-0.699372549019608;PL[7].x=0.883725490196078;PL[7].y=-0.711921568627451;PL[8].x=0.884509803921569;PL[8].y=-0.712313725490196;PL[9].x=0.875666666666667;PL[9].y=-0.712666666666667;PL[10].x=0.867000491595417;PL[10].y=-0.713012545050885;PL[11].x=0.855882352941176;PL[11].y=-0.713882352941176;PL[12].x=0.840588235294118;PL[12].y=-0.713098039215686;PL[13].x=0.81703754096554;PL[13].y=-0.7118903113014;PL[14].x=0.827619849627862;PL[14].y=-0.69839669172869;PL[15].x=0.835882352941176;PL[15].y=-0.697019607843137;PL[16].x=0.859411764705882;PL[16].y=-0.693098039215686;PL[17].x=0.873529411764706;PL[17].y=-0.692705882352941;PL[18].x=0.884117647058824;PL[18].y=-0.684078431372549;PL[19].x=0.892727195371608;PL[19].y=-0.677063243858428;PL[20].x=0.916645125505649;PL[20].y=-0.657044033129204;PL[21].x=0.913529411764706;PL[21].y=-0.654274509803922;PL[22].x=0.91;PL[22].y=-0.651137254901961;PL[23].x=0.902549019607843;PL[23].y=-0.651529411764706;PL[24].x=0.887647058823529;PL[24].y=-0.652705882352941;PL[25].x=0.881379806634526;PL[25].y=-0.653200665420494;PL[26].x=0.874509803921569;PL[26].y=-0.659568627450981;PL[27].x=0.873137254901961;PL[27].y=-0.661725490196078;PL[28].x=0.86448535012997;PL[28].y=-0.671098387032402;PL[29].x=0.839411764705882;PL[29].y=-0.66878431372549;PL[30].x=0.851960784313726;PL[30].y=-0.656235294117647;PL[31].x=0.855865158382694;PL[31].y=-0.652330920048679;PL[32].x=0.859784342827634;PL[32].y=-0.647576439545949;PL[33].x=0.87078431372549;PL[33].y=-0.635843137254902;PL[34].x=0.876666666666667;PL[34].y=-0.62956862745098;PL[35].x=0.877183605411647;PL[35].y=-0.628397119265468;PL[36].x=0.912745098039216;PL[36].y=-0.615843137254902;PL[37].x=0.927408415264413;PL[37].y=-0.610666666666667;PL[38].x=0.927329983891864;PL[38].y=-0.606823529411764;PL[39].x=0.94007508193108;PL[39].y=-0.591333333333333;
	bezier(canv,PL,X,Y,oX,oY,rotace,39);
	//tělo
	if(typ!=-1)canv->Pen->Width*=2;
	PL[0].x=0.438;PL[0].y=-0.405333333333333;PL[1].x=0.364;PL[1].y=-0.389333333333333;PL[2].x=0.352;PL[2].y=-0.382;PL[3].x=0.27558333333333;PL[3].y=-0.3335;PL[4].x=0.26933333333333;PL[4].y=-0.329333333333333;PL[5].x=0.17905052562645;PL[5].y=-0.39211010512529;PL[6].x=0.12558333333333;PL[6].y=-0.381416666666667;PL[7].x=0.11516666666667;PL[7].y=-0.379333333333333;PL[8].x=0.16377777777778;PL[8].y=-0.499333333333333;PL[9].x=0.21725;PL[9].y=-0.587666666666667;PL[10].x=0.21725;PL[10].y=-0.585583333333333;PL[11].x=0.14641666666667;PL[11].y=-0.606416666666667;PL[12].x=0.12975;PL[12].y=-0.6085;PL[13].x=0.076625;PL[13].y=-0.521;PL[14].x=0.03391666666667;PL[14].y=-0.51475;PL[15].x=0.0130833333333301;PL[15].y=-0.366833333333333;PL[16].x=0.0113159293774199;PL[16].y=-0.354284765246321;PL[17].x=-0.01115105718522;PL[17].y=-0.30999031404117;PL[18].x=0.0714166666666698;PL[18].y=-0.271;PL[19].x=0.180208333333335;PL[19].y=-0.2165;PL[20].x=0.181;PL[20].y=-0.232;PL[21].x=0.291;PL[21].y=-0.175;PL[22].x=0.407528527185643;PL[22].y=-0.114617035912894;PL[23].x=0.434999999999999;PL[23].y=-0.108;PL[24].x=0.571;PL[24].y=-0.108;
	PL[25].x=0.707000000000001;PL[25].y=-0.108;PL[26].x=0.734471472814357;PL[26].y=-0.114617035912894;PL[27].x=0.851;PL[27].y=-0.175;PL[28].x=0.961;PL[28].y=-0.232;PL[29].x=0.961791666666665;PL[29].y=-0.2165;PL[30].x=1.07058333333333;PL[30].y=-0.271;PL[31].x=1.15315105718522;PL[31].y=-0.30999031404117;PL[32].x=1.13068407062258;PL[32].y=-0.354284765246321;PL[33].x=1.12891666666667;PL[33].y=-0.366833333333333;PL[34].x=1.10808333333333;PL[34].y=-0.51475;PL[35].x=1.065375;PL[35].y=-0.521;PL[36].x=1.01225;PL[36].y=-0.6085;PL[37].x=0.99558333333333;PL[37].y=-0.606416666666667;PL[38].x=0.92475;PL[38].y=-0.585583333333333;PL[39].x=0.92475;PL[39].y=-0.587666666666667;PL[40].x=0.97822222222222;PL[40].y=-0.499333333333333;PL[41].x=1.02683333333333;PL[41].y=-0.379333333333333;PL[42].x=1.01641666666667;PL[42].y=-0.381416666666667;PL[43].x=0.96294947437355;PL[43].y=-0.39211010512529;PL[44].x=0.87266666666667;PL[44].y=-0.329333333333333;PL[45].x=0.86641666666667;PL[45].y=-0.3335;PL[46].x=0.78266666666667;PL[46].y=-0.386;PL[47].x=0.76977777777777;PL[47].y=-0.392444444444445;PL[48].x=0.692;PL[48].y=-0.408;
	bezier(canv,PL,X,Y,oX,oY,rotace,48);
	if(typ!=-1)canv->Pen->Width/=2;
	//lemy se vykreslují až od daného zoomu a pokud se nejedná o kurzor
	if(Z/zAA>=9 && typ!=-1)
	{
		//lem pravá ruka
		PL[0].x=0.123;PL[0].y=-0.381;PL[1].x=0.069;PL[1].y=-0.374;PL[2].x=0.052;PL[2].y=-0.342;PL[3].x=0.069;PL[3].y=-0.374;PL[4].x=0.051;PL[4].y=-0.391;
		if(rotace)m.rotace_polygon(oX,oY,PL,4,rotace);
		for(int i=0;i<=4;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
		canv->Polyline(PF,4);
		//sekundární lem pravá ruka
		PL[0].x=0.126;PL[0].y=-0.245;PL[1].x=0.169;PL[1].y=-0.32;PL[2].x=0.152;PL[2].y=-0.291;PL[3].x=0.138;PL[3].y=-0.32;
		if(rotace)m.rotace_polygon(oX,oY,PL,3,rotace);
		for(int i=0;i<=3;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
		canv->Polyline(PF,3);
		//lem levá ruka
		PL[0].x=1.018;PL[0].y=-0.381;PL[1].x=1.063;PL[1].y=-0.354;PL[2].x=1.101;PL[2].y=-0.353;PL[3].x=1.063;PL[3].y=-0.354;PL[4].x=1.088;PL[4].y=-0.319;
		if(rotace)m.rotace_polygon(oX,oY,PL,4,rotace);
		for(int i=0;i<=4;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
		canv->Polyline(PF,4);
		//sekundární lem levá ruka
		PL[0].x=1.022;PL[0].y=-0.249;PL[1].x=0.995;PL[1].y=-0.296;PL[2].x=0.963;PL[2].y=-0.307;PL[3].x=0.995;PL[3].y=-0.296;PL[4].x=1.003;PL[4].y=-0.323;
		if(rotace)m.rotace_polygon(oX,oY,PL,4,rotace);
		for(int i=0;i<=4;i++){PF[i].x=X+m.m2px(PL[i].x-oX);PF[i].y=Y+m.m2px(oY-PL[i].y);}
		canv->Polyline(PF,4);
	}
	//nos
	PL[0].x=0.539;PL[0].y=-0.48;PL[1].x=0.553684210526316;PL[1].y=-0.483684210526316;PL[2].x=0.548954612960862;PL[2].y=-0.506794038841696;PL[3].x=0.564736842105263;PL[3].y=-0.506315789473684;PL[4].x=0.582105263157895;PL[4].y=-0.505789473684211;PL[5].x=0.572631578947368;PL[5].y=-0.484736842105263;PL[6].x=0.593;PL[6].y=-0.481;
	bezier(canv,PL,X,Y,oX,oY,rotace,6);
	//čelo
	PL[0].x=0.441;PL[0].y=-0.421;PL[1].x=0.46;PL[1].y=-0.491;PL[2].x=0.668;PL[2].y=-0.514;PL[3].x=0.691;PL[3].y=-0.4165;
	bezier(canv,PL,X,Y,oX,oY,rotace,3);
	//zadní část hlavy - včetně vykreslení textury vlasů
	PL[0].x=0.691;PL[0].y=-0.417;PL[1].x=0.6415;PL[1].y=-0.3485;PL[2].x=0.634;PL[2].y=-0.425;PL[3].x=0.568;PL[3].y=-0.425;PL[4].x=0.4975;PL[4].y=-0.425;PL[5].x=0.499;PL[5].y=-0.348;PL[6].x=0.441;PL[6].y=-0.421;PL[7].x=0.4105;PL[7].y=-0.243;PL[8].x=0.4665;PL[8].y=-0.1545;PL[9].x=0.571;PL[9].y=-0.155;PL[10].x=0.6795;PL[10].y=-0.1545;PL[11].x=0.7225;PL[11].y=-0.2425;PL[12].x=0.691;PL[12].y=-0.417;
	canv->Brush->Style=bsDiagCross;//šrafy mód
	canv->Brush->Color=clBlack;
	if(typ!=-1)BeginPath(canv->Handle);
	bezier(canv,PL,X,Y,oX,oY,rotace,12);
	if(typ!=-1)EndPath(canv->Handle);
	if(typ!=-1)FillPath(canv->Handle);

	////text
	if(typ!=-1)//v módu kurzor se název nezobrazuje
	{              //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		//if(F->aFont->Size==12)canv->Font->Size=m.round(5.4*Z);else canv->Font->Size=m.round(5*Z);
		AnsiString T=short_name;
		//if(Z>4*3) //short_name odstaveno
		{T=name;if(F->aFont->Size==12)canv->Font->Size=m.round(2*Z); else canv->Font->Size=m.round(2.8*Z);}//od daného zoomu zobrazuje celý název
		float Odsazeni=3.5*Z+41;//+41 z důvodu vycentrování ikony člověka v knihovně elementů
		if(typ==1)//pokud se jedná o standardní zobrazení
		{
			canv->Font->Name=F->aFont->Name;
			if(/*stav==2 || */stav==3)canv->Font->Style = TFontStyles()<< fsBold;//došlo k vybrání elementu-tato část odstavena nebo přímo jeho textu
			long x,y;
			short h=canv->TextHeight(T);short w=canv->TextWidth(T);   //pozn. pro 180° neobracím text vzhůru nohama
			if(rotace90==0 || rotace90==180)
			{
				x=m.round(X-w/2.0);
				if(rotace90==0)y=m.round(Y-Odsazeni-h);
				if(rotace90==180)y=m.round(Y+Odsazeni);
				aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));//souřadnice pro citelnou oblast
			}
			else
			{
				rotace_textu(canv,-rotace90*10);
				if(rotace90==90){x=m.round(X-Odsazeni);y=m.round(Y-w/2.0);}
				if(rotace90==270){x=m.round(X+Odsazeni);y=m.round(Y+w/2.0);}
				aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+h)/zAA),m.round((y+w)/zAA));//souřadnice pro citelnou oblast
			}
			canv->TextOutW(x,y,name);//samotný vypis
		}
		else//ikona
		{
			canv->Font->Name=F->aFont->Name;//musí tu být, jinak chyba popisku u prvního robota  //canv->Font->Name="Arial";
			canv->Font->Size=F->m.round(sizeP*Z/1.5);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z/1.5);//1.5 kvůli dodatečnému zvětšování 1.5x člověka v knihovně elemementů
			canv->TextOutW(X-canv->TextWidth(name)/2,m.round(Y+Odsazeni),name); //1 pouze korekce
			canv->TextOutW(X-canv->TextWidth(short_name)/2,m.round(Y+Odsazeni+1*Z/1.5+canv->TextHeight(name)),short_name);
		}                                                                       //1.5 kvůli dodatečnému zvětšování 1.5x člověka v knihovně elemementů
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_otoc(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav)
{
	double Z=F->Zoom;
	short size=m.round(3.5*Z);if(stav==2)size=m.round(3.7*Z);//ve skutečnosti poloměr
	float width=0.8*Z;if(stav==2)width=1*Z;

	TColor barva=clBlack; if(eID==6)barva=clRed;if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	if(typ==-1)//v módu kurzor
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Pen->Width=width;
		canv->Pen->Color=barva;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
	}

	//vykreslení elementu
	canv->Ellipse(X-size,Y-size,X+size,Y+size);

	double R=rotace;
	if(R==0 || R==180)
	{
		R=0;//nerozlišuje se zda 0 nebo 180, v těchto případech je symbol stejně orotován
		if(typ==-1)//mód kurzor
		{
			//šipka                                     //pokud bych chtěl rotovat
			sipka(canv,m.round(X-size),m.round(Y+width),/*rotace*/R-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+size),m.round(Y-width),/*rotace*/R-180-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
		}
		else
		{
			canv->Pen->Color=clWhite;
			canv->Brush->Style=bsSolid;

			//přeříznutí pod šipkou
			canv->Rectangle(X-size,Y-width,m.round(X-size+1*Z),Y);//přežíznutí, možná lépe řešit obloukem
			canv->Rectangle(m.round(X+size-1*Z),Y,X+size,Y+width);//přežíznutí, možná lépe řešit obloukem
			//šipka
			sipka(canv,m.round(X-size),m.round(Y+width),R-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+size),m.round(Y-width),R-180-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
		}
	}
	else
	{
		R=90;//nerozlišuje se zda 90 nebo 270, v těchto případech je symbol stejně orotován
		if(typ==-1)//mód kurzor
		{
			//šipka
			sipka(canv,m.round(X-width),m.round(Y-size),R-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+width),m.round(Y+size),R-180-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
		}
		else
		{
			canv->Pen->Color=clWhite;
			canv->Brush->Style=bsSolid;

			//přeříznutí pod šipkou
			canv->Rectangle(X+width,Y-size,m.round(X+width-1*Z),m.round(Y-size+width+1*Z));//přežíznutí, možná lépe řešit obloukem
			canv->Rectangle(X+width,Y+size,m.round(X+width-1.5*Z),m.round(Y+size+width-1*Z));//přežíznutí, možná lépe řešit obloukem
			//šipka
			sipka(canv,m.round(X-width),m.round(Y-size),R-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+width),m.round(Y+size),R-180-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
		}
	}

	//pokud je otoč aktivní tj. se stopkou
	if(eID==6)vykresli_stopku(canv,X,Y,"","",typ,rotace,stav);

	//text
	if(typ!=-1)//v módu kurzor se název nezobrazuje
	{
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		canv->Font->Color=barva;if(eID==5 && stav!=-1 && typ==0)canv->Font->Color=m.clIntensive(barva,100);
		canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
		canv->Font->Name=F->aFont->Name;
		canv->Font->Style = TFontStyles();
		AnsiString T=name;//short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		if(typ==1)//normální zobrazení
		{
			if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;//zvýraznění
			short h=canv->TextHeight(T);if(T=="")h=canv->TextHeight("NIC"); short w=canv->TextWidth(T);
			float zAA=1.0;if(F->antialiasing)zAA=3.0;
			long x,y;
			//rotace
			switch((int)rotace)//posun referenčního bodu kvůli bílému orámování
			{
				case 0: 	rotace_textu(canv,2700);	x=m.round(X+h/2.0);   	 	 y=m.round(Y+size+1.4*Z); aktOblast=TRect(m.round((x-h)/zAA),m.round(y/zAA),m.round(x/zAA),m.round((y+w)/zAA));break;
				case 90:	rotace_textu(canv,0);		 	x=m.round(X-w-size-1.4*Z); y=m.round(Y-h/2.0); 			aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));break;
				case 180:	rotace_textu(canv,0+900); x=m.round(X-h/2.0);   	 	 y=m.round(Y-size-1.4*Z);	aktOblast=TRect(m.round(x/zAA),m.round((y-w)/zAA),m.round((x+h)/zAA),m.round(y/zAA)); break;
				case 270:	rotace_textu(canv,0);	   	x=m.round(X+size+1.4*Z);	 y=m.round(Y-h/2.0); 			aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));break;
			}
			canv->TextOutW(x,y,T);
			rotace_textu(canv,0);//navrácení do původního stavu
		}
		else//ikona
		{
			//canv->Font->Name="Arial";
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(X-canv->TextWidth(name)/2,Y+size+1*Z,name); //1 pouze korekce
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_ion(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,double rotace,short stav,float TS)
{
	double Z=Form1->Zoom;//zoom, pouze zkrácení zápisu

	////konstanty
	int polomer=m.m2px(0.2);if(stav==2)polomer=m.m2px(0.22);//poloměr kružnic zadaná v metrech
	int vzdalenost=m.m2px(1.3/2.0);//vzdálenost kružnice od středu v metrech (vzádlenost kružnic podělená dvěmi)
	float tloustka_linie=1.05/3.0;if(stav==2)tloustka_linie*=1.3;//pokud má být zvýrazněn  //vykreslovací linie
	float DT=m.m2px(0.35);//delka trysky v metrech
	float TW=m.m2px(0.2);//tryska šířka před zúžením, nastavení dle fobota sirka_ramena v metrech
	float TZ=TW/2.0;//tryska ve zúžení //sirka2
	//TS;//sklon trysky, slouží pro animaci simulace (pohyb trysky), přebíráno již z parametru metody
	float TO=polomer/2;//odsazeni trysky od okraje tyče
	short o=1*Z;//odsazení textu

	////nastavení barev
	TColor barva=clBlack;
	canv->Brush->Color=clWhite;//výplň kružnic
	TColor clIon=(TColor)RGB(7,107,171);
	if(stav==-1)//pokud je aktivní nebo neaktivní
	{
		barva=m.clIntensive(barva,180);
		clIon=m.clIntensive(clIon,180);
	}

	////nastavení pera - dle typu zobrazení
	TPenMode PenMode=pmCopy;
	if(typ==-1)//typ kurzor
	{
		PenMode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Color=barva;
		canv->Pen->Width=1;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Style=psSolid;
		canv->Pen->Width=F->m.round(tloustka_linie*Z);
		canv->Brush->Style=bsSolid;
	}

	////vykreslení elementu
	if (rotace==0 || rotace==180)//svislé vykreslení
	{
		//horní tryska
		TPoint P=polygonDleOsy(canv,X,Y-vzdalenost+polomer-TO,DT/1.5,TW,TZ,90+TS,0,PenMode,barva,clWhite);
		if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2.0,TW/2,TZ*4,90+TS,0,pmMask,clWhite,clIon);

		//dolní tryska
		P=polygonDleOsy(canv,X,Y+vzdalenost-polomer+TO,DT/1.5,TW,TZ,270+TS,0,PenMode,barva,clWhite);
		if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2,TW/2,TZ*4,270+TS,0,pmMask,clWhite,clIon);

		//vykreslení kružnic
		canv->Pen->Mode=PenMode;
		canv->Pen->Color=barva;
		canv->Ellipse(X-polomer,Y-vzdalenost-polomer,X+polomer,Y-vzdalenost+polomer);
		canv->Ellipse(X-polomer,Y+vzdalenost+polomer,X+polomer,Y+vzdalenost-polomer);
	}
	else//vodorovne vykreslení
	{
		//levá tryska
		TPoint P=polygonDleOsy(canv,X-vzdalenost+polomer-TO,Y,DT/1.5,TW,TZ,0+TS,0,PenMode,barva,clWhite);
		if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2.0,TW/2.0,TZ*4,0+TS,0,pmMask,clWhite,clIon);

		P=polygonDleOsy(canv,X+vzdalenost-polomer+TO,Y,DT/1.5,TW,TZ,180+TS,0,PenMode,barva,clWhite);
		if(stav>0 && typ!=-1)polygonDleOsy(canv,P.x,P.y,DT/2.0,TW/2.0,TZ*4,180+TS,0,pmMask,clWhite,clIon);

		//vykreslení kružnic
		canv->Pen->Mode=PenMode;
		canv->Pen->Color=barva;
		canv->Ellipse(X-vzdalenost-polomer,Y-polomer,X-vzdalenost+polomer,Y+polomer);
		canv->Ellipse(X+vzdalenost-polomer,Y-polomer,X+vzdalenost+polomer,Y+polomer);
	}

	////text
	if(typ!=-1 && name!="")//v módu kurzor nebo pokud je součástí nadřazeného elementu se název nezobrazuje
	{
		//nastavení písma  //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
		canv->Font->Name=F->aFont->Name;
		canv->Font->Style = TFontStyles();
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;

		if(typ==1)//normální zobrazení typ==1
		{
			if(/*stav==2 || */stav==3)canv->Font->Style = TFontStyles()<< fsBold;//došlo k vybrání elementu-tato část odstavena nebo přímo jeho textu
			float zAA=1.0;if(F->antialiasing)zAA=3.0;

			AnsiString Text=short_name;/*if(Z>4*3) */Text=name;//odstaveno
			int w=canv->TextWidth(Text);
			int h=canv->TextHeight(Text);

			//rotace textu
			long x,y;short K=0.25*Z;//pouze grafická korekce, text aby se nezohledňovalo zarovnání na diakritiku, vypadá to dinvě
			switch((int)rotace)//posun referenčního bodu kvůli bílému orámování
			{
				case 0:   canv->Font->Orientation=2700;  x=m.round(X+h/2.0+K); 	   						y=m.round(Y+vzdalenost+polomer+o);	aktOblast=TRect(m.round((x-h)/zAA),m.round(y/zAA),m.round(x/zAA),m.round((y+w)/zAA));break;
				case 90:  canv->Font->Orientation=0;     x=m.round(X-vzdalenost-polomer-o-w); y=m.round(Y-h/2.0-K);        				aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));break;
				case 180: canv->Font->Orientation=0+900; x=m.round(X-h/2.0-K);        				y=m.round(Y-vzdalenost-polomer-o); 	aktOblast=TRect(m.round(x/zAA),m.round((y-w)/zAA),m.round((x+h)/zAA),m.round(y/zAA));break;
				case 270: canv->Font->Orientation=0;     x=m.round(X+vzdalenost+polomer+o);   y=m.round(Y-h/2.0-K);          			aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA));break;
			}
			canv->TextOutW(x,y,Text);
			canv->Font->Orientation=0;//vrací nastavení do původního stavu
		}
		else//ikona v knihovně elementů je text pod elementem
		{
			int odsazeni=55;//odsazení z důvodu správného zobrazení v knihovně
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(X-canv->TextWidth(name)/2,m.round(Y+vzdalenost+polomer-odsazeni),name); //1 pouze korekce
			canv->TextOutW(X-canv->TextWidth(short_name)/2,m.round(Y+vzdalenost+polomer+1*Z+canv->TextHeight(name)-odsazeni),short_name);
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_lakovaci_okno(TCanvas *canv,long X,long Y,double LO1,double OTOC_delka,double LO2,double delka_ramena,double rotace,double LO_pozice)
{
	LO1=m.m2px(LO1);
	OTOC_delka=m.m2px(OTOC_delka);
	LO2=m.m2px(LO2);
	LO_pozice=m.m2px(LO_pozice);//zajišťuje vyosení lakovacího okna

	canv->Pen->Width=1;
	canv->Pen->Color=clBlack;
	canv->Pen->Mode=pmCopy;
	canv->Pen->Style=psDash;
	//rotace
	switch((int)rotace)
	{
		case 0:
		{
			if(LO2==0)//pouze kk
			{
				line(canv,X,Y,m.round(X-LO1/2.0+LO_pozice),m.round(Y-delka_ramena));
				line(canv,X,Y,m.round(X+LO1/2.0+LO_pozice),m.round(Y-delka_ramena));
			}
			else//kk s otočí či kk s vyoseným LO
			{
				line(canv,X,Y,m.round(X-LO1-OTOC_delka/2.0),m.round(Y-delka_ramena));
				if(OTOC_delka)line(canv,X,Y,m.round(X-OTOC_delka/2.0),m.round(Y-delka_ramena));
				if(OTOC_delka)line(canv,X,Y,m.round(X+OTOC_delka/2.0),m.round(Y-delka_ramena));
				line(canv,X,Y,m.round(X+OTOC_delka/2.0+LO2),m.round(Y-delka_ramena));
			}
		}break;
		case 90:
		{
			if(LO2==0)//pouze kk
			{
				line(canv,X,Y,m.round(X+delka_ramena),m.round(Y-LO1/2.0+LO_pozice));
				line(canv,X,Y,m.round(X+delka_ramena),m.round(Y+LO1/2.0+LO_pozice));
			}
			else//kk s otočí či kk s vyoseným LO
			{
				line(canv,X,Y,m.round(X+delka_ramena),m.round(Y-LO1-OTOC_delka/2.0));
				if(OTOC_delka)line(canv,X,Y,m.round(X+delka_ramena),m.round(Y-OTOC_delka/2.0));
				if(OTOC_delka)line(canv,X,Y,m.round(X+delka_ramena),m.round(Y+OTOC_delka/2.0));
				line(canv,X,Y,m.round(X+delka_ramena),m.round(Y+OTOC_delka/2.0+LO2));
			}
		}break;
		case 180:
		{
			if(LO2==0)//pouze kk
			{
				line(canv,X,Y,m.round(X-LO1/2.0+LO_pozice),m.round(Y+delka_ramena));
				line(canv,X,Y,m.round(X+LO1/2.0+LO_pozice),m.round(Y+delka_ramena));
			}
			else//kk s otočí či kk s vyoseným LO
			{
				line(canv,X,Y,m.round(X-LO1-OTOC_delka/2.0),m.round(Y+delka_ramena));
				if(OTOC_delka)line(canv,X,Y,m.round(X-OTOC_delka/2.0),m.round(Y+delka_ramena));
				if(OTOC_delka)line(canv,X,Y,m.round(X+OTOC_delka/2.0),m.round(Y+delka_ramena));
				line(canv,X,Y,m.round(X+OTOC_delka/2.0+LO2),m.round(Y+delka_ramena));
			}
		}break;
		case 270:
		{
			if(LO2==0)//pouze kk
			{
				line(canv,X,Y,m.round(X-delka_ramena),m.round(Y-LO1/2.0+LO_pozice));
				line(canv,X,Y,m.round(X-delka_ramena),m.round(Y+LO1/2.0+LO_pozice));
			}
			else//kk s otočí či kk s vyoseným LO
			{
				line(canv,X,Y,m.round(X-delka_ramena),m.round(Y-LO1-OTOC_delka/2.0));
				if(OTOC_delka)line(canv,X,Y,m.round(X-delka_ramena),m.round(Y-OTOC_delka/2.0));
				if(OTOC_delka)line(canv,X,Y,m.round(X-delka_ramena),m.round(Y+OTOC_delka/2.0));
				line(canv,X,Y,m.round(X-delka_ramena),m.round(Y+OTOC_delka/2.0+LO2));
			}
		}break;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_ikonu_linie(TCanvas *canv,int X,int Y,AnsiString Popisek,short stav)
{
	short o=10*3;
	int W=F->DrawGrid_geometrie->DefaultColWidth*3/2-o;
	int odsazeni=6;//vycentrování linie mezi hlavičkou a textem
	TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	//vykreslení linie
	set_pen(canv,barva,1*10,PS_ENDCAP_FLAT);
	line(canv,X-W+8,Y-W/2+odsazeni,X+W-8,Y-W/2+odsazeni);

	//popisek
	canv->Brush->Style=bsClear;
	if(stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
	canv->Font->Name=F->aFont->Name;
	canv->Font->Size=m.round(o*sizeP/3.0);if(F->aFont->Size==12)canv->Font->Size=o;
	canv->TextOutW(X-canv->TextWidth(Popisek)/2,Y+o/2,Popisek);

//	set_pen(canv,clRed,1,PS_ENDCAP_FLAT);
//	line(canv,X-5,Y-5,X+5,Y+5);
//	line(canv,X+5,Y+5,X-5,Y-5);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_ikonu_oblouku(TCanvas *canv,int X,int Y,AnsiString Popisek,short stav)
{
	short o=10*3;
	int W=F->DrawGrid_geometrie->DefaultColWidth*3/2-o;
	int odsazeni=3;//vycentrování linie mezi hlavičkou a textem
	short C=W/2;//zajištění vycentrování
	TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	//vykreslení oblouku
	set_pen(canv,barva,1*10,PS_ENDCAP_FLAT);
	canv->Arc(X-W-C,Y-W+odsazeni,X+W-C,Y+W+odsazeni,X+W-C,Y+odsazeni,X-C,Y-W+odsazeni);//směr proti hodinovým ručičkám

	//popisek
	canv->Brush->Style=bsClear;
	if(stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
	canv->Font->Name=F->aFont->Name;
	canv->Font->Size=m.round(o*sizeP/3.0);if(F->aFont->Size==12)canv->Font->Size=o;
	canv->TextOutW(X-canv->TextWidth(Popisek)/2,Y+o/2,Popisek);
}

//		int X=300;int Y=600;//střed čtverce obsaného
//		int R=500;//Radius
//		int SA=90;//výchozí úhel, pod kterým oblouk začíná, musí být kladný
//		int RA=-90;//rotační úhel, pod kterým je oblouk rotován - směřován, může být záporný
//		Canvas->MoveTo(X, Y-R);//musí se přesunout pero na začátek, v případě kontinuálního kreslení netřeba
//		Canvas->AngleArc(X, Y, R,SA,RA);
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_ikonu_textu(TCanvas *canv,int X,int Y,AnsiString Popisek,short stav)
{
	short o=10*3;
	//int W=F->DrawGrid_knihovna->DefaultColWidth*3/2-o;
	TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní
	canv->Brush->Style=bsClear;
	canv->Font->Color=barva;

	//ikona - ABC
	canv->Font->Name="Arial";
	canv->Font->Size=o*1.5;
	canv->TextOutW(X-canv->TextWidth("ABC")/2,Y-o*2,"ABC");

	//popisek
	canv->Brush->Style=bsClear;
	canv->Font->Name=F->aFont->Name;
	canv->Font->Size=m.round(o*sizeP/3.0);if(F->aFont->Size==12)canv->Font->Size=o;
	if(stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
	canv->TextOutW(X-canv->TextWidth(Popisek)/2,Y+o/2,Popisek);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_ikonu_sipky(TCanvas *canv,int X,int Y,AnsiString Popisek,short stav)
{
	short o=10*3;
	int W=F->DrawGrid_geometrie->DefaultColWidth*3/2-o;
	//short C=W/2;//zajištění vycentrování
	TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní
	canv->Brush->Style=bsClear;

	//vykreslení linie
	set_pen(canv,barva,1*3,PS_ENDCAP_SQUARE);
	line(canv,X-W+8,Y,X+W-8,Y-W+8);

	//vykreslení šipky                      //pozor musí být invertované souřadnice Y (log. vs. fyz. souřadnice), metoda je stavěna na kartéské (logické souřadnice), nikoliv soužadnice monitoru (fyzické)
	sipka(canv,X+W-8,Y-W+8,m.azimut(X-W+8,Y,X+W-8,Y+W-8),false,1,clBlack,clBlack);//děleno Z na negaci *Zoom v metodě šipka

	//popisek
	canv->Font->Size=m.round(o*sizeP/3.0);if(F->aFont->Size==12)canv->Font->Size=o;
	canv->Brush->Style=bsClear;
	if(stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
	canv->TextOutW(X-canv->TextWidth(Popisek)/2,Y+o/2,Popisek);
}
//---------------------------------------------------------------------------
//typ: -2 kurzor se sprchy, -1 kurzor bez sprchy, 0 ikona bez sprchy, 1 ikona se sprchou, stav: -1 disabled
void Cvykresli::vykresli_ikonu_komory(TCanvas *canv,int X,int Y,AnsiString Popisek,short typ,short stav,double orientace)
{
	////výchozí nastavení
	short W=10;//šířka torza objektu
	short H=15;//výška torza objektu
	short o=3;//komora odsazení

	////nastavení pera
	if(typ==-1 || typ==-2)//kurzor
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		W*=10;//šířka torza objektu
		H*=10;//výška torza objektu
		o*=10;//komora odsazení
	}
	else//ikona
	{
		canv->Brush->Color=clWhite;canv->Brush->Style=bsClear;//nastavení výplně
		canv->Pen->Mode=pmCopy;//pro transparentní zákres
		canv->Pen->Color=clStenaKabiny;if(stav==-1)canv->Pen->Color=m.clIntensive(clStenaKabiny,50);//barva
		canv->Pen->Width=m.m2px(0.12);//šířka v pixelech
		set_pen(canv,canv->Pen->Color,canv->Pen->Width,PS_ENDCAP_SQUARE);
		W*=30/1.5;//šířka torza objektu
		H*=30/1.5;//výška torza objektu
		o*=30/1.5;//komora odsazení
	}

	////vykreslení obrysu
	if(F->pom_temp!=NULL && (F->pom_temp->orientace==90 || F->pom_temp->orientace==270) || typ>=0)
	{
  	line(canv,X,Y,X+W,Y);//horní vodorovná
  	line(canv,X+W,Y,X+W,Y+H);//pravá svislá
  	line(canv,X+W,Y+H,X,Y+H);//dolní vodorovná
  	line(canv,X+o,Y+H,X+o,Y+H/2+o/2);//dolní kus komory
		line(canv,X+o,Y,X+o,Y+H/2-o/2);//horní kus komory
		////symbolika tekoucí kapaliny
		if(typ==1 || typ==-2)vykresli_pow_sprchu(canv,X+W,X+W,Y,Y+H,W-o,canv->Pen->Color,ceil(canv->Pen->Width/4.0),m.round(o/2.0),typ);
	}
	if(F->pom_temp!=NULL && (F->pom_temp->orientace==0 || F->pom_temp->orientace==180) && typ<0)
	{
		line(canv,X,Y,X,Y-W);//horní vodorovná
		line(canv,X,Y-W,X+H,Y-W);//pravá svislá
		line(canv,X+H,Y-W,X+H,Y);//dolní vodorovná
		line(canv,X,Y-o,X+H/2-o/2,Y-o);//dolní kus komory,
		line(canv,X+H/2+o/2,Y-o,X+H,Y-o);//horní kus komory
		////symbolika tekoucí kapaliny
		if(typ==1 || typ==-2)vykresli_pow_sprchu(canv,X,X+H,Y+o+10,Y+o+10,W-o,canv->Pen->Color,ceil(canv->Pen->Width/4.0),m.round(o/2.0),typ);//+10 korekce vykreslení
	}

	////popisek
	if(typ==0 || typ==1)//pouze pro typ ikona se sprchou nebo bez
	{
		short oT=20/2;//odsazení textu od ikony
		TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní
		canv->Font->Color=barva;
		canv->Font->Size=F->m.round(2.8*12);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*12);
		canv->Font->Name=F->aFont->Name;
		canv->Font->Style = TFontStyles();
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		canv->TextOutW(X+W/2-canv->TextWidth(Popisek)/2,Y+H+oT,Popisek);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::linie(TCanvas *canv,long X1,long Y1,long X2,long Y2,int Width,TColor Color,TPenStyle PenStyle,TPenMode PenMode)
{
	canv->Pen->Width=Width;
	canv->Pen->Color=Color;
	canv->Pen->Mode=PenMode;
	canv->Pen->Style=PenStyle;
	//set_pen(canv,clBlack,1*10,PS_ENDCAP_FLAT);
	line(canv,X1,Y1,X2,Y2);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::line(TCanvas *canv,long X1,long Y1,long X2,long Y2)
{
	canv->MoveTo(X1,Y1);
	canv->LineTo(X2,Y2);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c nebo téměř totožné wiki   //http://www.yevol.com/bcb/Lesson12.htm
void Cvykresli::bezier(TCanvas *canv,TPointD *POLE,long posledni_prvek)
{
	bezier(canv,m.L2P(POLE,posledni_prvek),posledni_prvek);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::bezier(TCanvas *canv,TPointD *POLE,long X,long Y,double oX,double oY,double rotace,long posledni_prvek)
{
	TPoint *PF=new TPoint[posledni_prvek+1];
	if(rotace)m.rotace_polygon(oX,oY,POLE,posledni_prvek,rotace);
	for(int i=0;i<=posledni_prvek;i++){PF[i].x=X+m.m2px(POLE[i].x-oX);PF[i].y=Y+m.m2px(oY-POLE[i].y);}
	canv->PolyBezier(PF,posledni_prvek);
	PF=NULL;delete[] PF;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::bezier(TCanvas *canv,TPoint *POLE_px,long posledni_prvek)
{
	float O=0.02;if(F->antialiasing)O*=3;
	canv->Pen->Style=psSolid;canv->Pen->Color=clBlack;
	canv->Pen->Width=m.round(O*F->Zoom);
	canv->PolyBezier(POLE_px,posledni_prvek);//výsledné vykreslení Bézierovy křivky
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí polygon dle osy, umí i kónický tvar, vratí souřadnice konce osy polygonu
TPoint Cvykresli::polygonDleOsy(TCanvas *canv,long X,long Y,float delka, float sirka1, float sirka2, double sklon, double rotace,TPenMode pmMode,TColor clFillOut,TColor clFillIn)
{
	//polovina od osy
	sirka1/=2.0;
	sirka2/=2.0;

	//vytvoření pole pro vykreslení celého polygonu
	TPoint *P=new TPoint[5];//vytvoří pole pro polyline
	//nahoru ze skutečného referenčního bodu
	TPointD K=m.rotace(sirka1,0+sklon,rotace);
	P[0].x=m.round(X+K.x);P[0].y=m.round(Y+K.y);
	///K=m.rotace(sirka1,0+sklon,rotace);
	//nahoře doprava ze skutečného referenčního bodu
	K=m.rotace(delka,90+sklon,rotace);
	TPointD K2=m.rotace(sirka2,0+sklon,rotace);
	P[1].x=m.round(X+K.x+K2.x);P[1].y=m.round(Y+K.y+K2.y);
	//dolu ze z předchozího bodu
	K=m.rotace(sirka2*2,180+sklon,rotace);
	P[2].x=m.round(P[1].x+K.x);P[2].y=m.round(P[1].y+K.y);
	//dole ze skutečného referenčního bodu
	K=m.rotace(sirka1,0+sklon+180,rotace);
	P[3].x=m.round(X+K.x);P[3].y=m.round(Y+K.y);
	//zpět nahoru do P[0]
	P[4]=P[0];

	//záloha nastavení pera
	TColor clBuffPen=canv->Pen->Color;
	TColor clBuffBrush=canv->Brush->Color;
	TPenStyle psBuffPen=canv->Pen->Style;
	TBrushStyle bsBuffBrush=canv->Brush->Style;

	//nastavení pera
	canv->Pen->Color=clFillOut;
	canv->Brush->Color=clFillIn;
	canv->Pen->Mode=pmMode;
	//if(transparent)canv->Pen->Mode=pmMask;else canv->Pen->Mode=pmCopy;
	if(clFillOut==clWhite)canv->Pen->Style=psClear;//pokud je nastaveno bílé orámování je bráno, že se nemá orámování vykreslovat

	//samotné vykreslení
	canv->Polygon(P,4);//bylo tady polyline pro průhlednou variantu

	//navrácení původních hodnot pera
	canv->Pen->Color=clBuffPen;
	canv->Brush->Color=clBuffBrush;
	canv->Pen->Style=psBuffPen;
	canv->Brush->Style=bsBuffBrush;

	//vratí souřadnice konce osy polygonu
	TPoint RET;
	RET.x=m.round((P[1].x+P[2].x)/2.0);RET.y=m.round((P[1].y+P[2].y)/2.0);

	//zrušení a uvolnění ukazatele
	P=NULL;delete P;

	return RET;//vratí souřadnice konce osy polygonu
}
//---------------------------------------------------------------------------
//stav: -3 kurzor, -2 normal (implicitně), -1-disabled, 0-editace zvýrazní všechny body, 1-až počet bodů zvýraznění daného bodu,počet bodů+1 zvýraznění dané hrany včetně sousedícícíh úchopů (např. pro polygono o 6 bodech) bude hodnota stavu 7 zvýraznění první hrany (od bodu 1 do bodu 2)
void Cvykresli::polygon(TCanvas *canv,Cvektory::TBod *body,TColor barva, short sirka,int stav,bool zobrazit_koty,bool automaticky_spojovat)
{
	if(body!=NULL && body->predchozi->n>1)
	{
		////výchozí parametry
		//short W=m.round(sirka/2.0);//posunutí vykreslení orámování nad vnější rozměry kabiny
		TColor clHighlight=m.clIntensive(barva,-50);
		TColor clDisabled=m.clIntensive(barva,50);

		////nastavení pera spojnic
		canv->Brush->Color=clWhite;canv->Brush->Style=bsClear;//nastavení výplně
		if(stav==-3)//typ kurzor
		{
			canv->Pen->Mode=pmNotXor;
			canv->Pen->Style=psDot;
			canv->Pen->Color=clBlack;
			canv->Pen->Width=1;
			canv->Brush->Style=bsClear;
		}
		else//normální
		{
			//canv->Pen->Mode=pmNotXor;//pro transparentní zákres
			canv->Pen->Mode=pmCopy;
			canv->Pen->Color=barva;if(stav==-1)canv->Pen->Color=clDisabled;//barva
			canv->Pen->Width=sirka;//šířka v pixelech
			set_pen(canv,canv->Pen->Color,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);
		}

		////vykreslení spojnic/hrany
		Cvektory::TBod *B=body->dalsi->dalsi;//přeskočí hlavičku i první prvek
		canv->MoveTo(m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y));//nastavení pera na výchozí pozici
		while(B!=NULL)
		{
			//normální spojnice/hrana
			if((signed)(body->predchozi->n+B->n-1)==stav)set_pen(canv,clHighlight,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);//zvýraznění konkrétní spojnice/hrany
			canv->LineTo(m.L2Px(B->X),m.L2Py(B->Y));
			if((signed)(body->predchozi->n+B->n-1)==stav)set_pen(canv,barva,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);//navrácení do výchozí barvy
			//virtuální spojnice, pokud se jedná o poslendí prvek spojí ještě s prvním, aby byl obrazec uzavřený, pokud je požadováno
			if(B==body->predchozi && automaticky_spojovat)
			{
				if((signed)(body->predchozi->n+B->n)==stav)set_pen(canv,clHighlight,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);//zvýraznění konkrétní spojnice/hrany
				canv->LineTo(m.L2Px(body->dalsi->X),m.L2Py(body->dalsi->Y));
				if((signed)(body->predchozi->n+B->n)==stav)set_pen(canv,barva,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);//navrácení do výchozí barvy
			}
			B=B->dalsi;//posun na další
		}

		////uchopy - pokud je považována editace, nutno vykreslit v samostatném cyklu až nad spojnice
		if(stav>=0)
		{
			B=body->dalsi;//přeskakuje hlavičku
			while(B!=NULL)
			{
				if((signed)B->n==stav){uchop(canv,B,clHighlight);break;}//pouze jeden konkrétní bod
				if((signed)body->predchozi->n<stav && ((signed)B->n==stav-(signed)body->predchozi->n || (signed)B->n==stav-(signed)body->predchozi->n+1))uchop(canv,B,clHighlight);//editace hrany (dva body)
				if(B==body->predchozi && automaticky_spojovat && (signed)(body->predchozi->n+B->n)==stav)uchop(canv,body->dalsi,clHighlight);//editace hrany druhý bod - ten virtuální resp. první
				if(stav==0)uchop(canv,B,clHighlight);//všechny body
				B=B->dalsi;//posun na další bod
			}
		}

		////vykreslení kót
		if(zobrazit_koty)//pokud je požádováno
		{
			//nastavení hodnot pro highlight, jiné hodnoty pro highlighty na hale a kabině
      int hodnota_koty=-2,oblast_koty=2;//defaultní hodnoty pro halu
			if(F->pom_temp!=NULL)//highlight pro objekt
			{hodnota_koty=-5,oblast_koty=-4;}
			//kontrola zda se jedná o čtverec či odelník (vykreslení pouze dvou kót)
			Cvektory::TBod *kota_od=NULL;
			double delka_koty=0;//zavedeno pro přepočet mezi časovou a délkovou kótou
			if(F->pom_temp!=NULL && body->predchozi->n==4)//pokud má objekt jen 4 body je otestován zda se jedná o obdelník nebo čtverec
			{
				double a=m.delka(body->dalsi->X,body->dalsi->Y,body->dalsi->dalsi->X,body->dalsi->dalsi->Y),b=m.delka(body->dalsi->dalsi->X,body->dalsi->dalsi->Y,body->predchozi->predchozi->X,body->predchozi->predchozi->Y),c=m.delka(body->predchozi->predchozi->X,body->predchozi->predchozi->Y,body->predchozi->X,body->predchozi->Y),d=m.delka(body->dalsi->X,body->dalsi->Y,body->predchozi->X,body->predchozi->Y);
				if(a==c && b==d)kota_od=body->dalsi->dalsi->dalsi;//jedná se o obdelník nebo čtverec
				if(F->scGPComboBox_prepinacKot->ItemIndex==1)kota_od=body->predchozi;//vykreslení pouze poslední kóty v časovém režimu
			}
			//příprava pro vykreslení kót
			short AA=1;if(F->antialiasing)AA=3;
			//akcelerátor
			if(kota_od!=NULL)B=kota_od;
			else B=body->dalsi->dalsi;
			short highlight=0;
			while(B!=NULL)//vykreslení kót musí být v samostatém cyklu!!!!!(jinak ovlivňuje vykreslení spojnic bodů)
			{
				//nastavení highlightu
				if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==B->n)highlight=2;
				else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==B->n)highlight=1;else highlight=0;
				//výpočet délky kóty
				delka_koty=m.round2double(m.delka(B->predchozi->X,B->predchozi->Y,B->X,B->Y),3);
				if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->pom_temp->pohon->aRD;
				//vykreslení kóty
				if(kota_od==NULL)                                                                                                                                              //převedení na mm
				vykresli_kotu(canv,m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y),m.L2Px(B->X),m.L2Py(B->Y),F->outDK(delka_koty),NULL,B->kota_offset*F->Zoom/AA,highlight,0.2,clGray,false,NULL,B);
				else
				{
					//určení nové vzdálenosti
					delka_koty=m.round2double(m.delka(B->predchozi->X,B->predchozi->Y,B->X,B->Y),3);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->pom_temp->pohon->aRD;
					//vykreslení jedné kóty pro obdelník/čtverec
					vykresli_kotu(canv,m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y),m.L2Px(B->X),m.L2Py(B->Y),F->outDK(delka_koty),NULL,B->kota_offset*F->Zoom/AA,highlight,0.2,clGray,false,NULL,B);
					if(kota_od->dalsi!=NULL)//ošetření
					{
				  	//zjištění highlightu pro druhou kótu obdelníku/čtverce
				  	if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==B->dalsi->n)highlight=2;
				  	else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==B->dalsi->n)highlight=1;else highlight=0;
				  	//délka
				  	delka_koty=m.round2double(m.delka(B->X,B->Y,B->dalsi->X,B->dalsi->Y),3);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->pom_temp->pohon->aRD;
				  	//vykreslení
						vykresli_kotu(canv,m.L2Px(B->X),m.L2Py(B->Y),m.L2Px(B->dalsi->X),m.L2Py(B->dalsi->Y),F->outDK(delka_koty),NULL,B->dalsi->kota_offset*F->Zoom/AA,highlight,0.2,clGray,false,NULL,B->dalsi);
					}
					break;
				}
				B=B->dalsi;
			}
			//vykreslení poslední kóty
			delka_koty=m.round2double(m.delka(body->predchozi->X,body->predchozi->Y,body->dalsi->X,body->dalsi->Y),0);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->pom_temp->pohon->aRD;
			if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==body->dalsi->n)highlight=2;
			else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==body->dalsi->n)highlight=1;else highlight=0;
			if(kota_od==NULL && body->predchozi->n>2)vykresli_kotu(canv,m.L2Px(body->predchozi->X),m.L2Py(body->predchozi->Y),m.L2Px(body->dalsi->X),m.L2Py(body->dalsi->Y),F->outDK(delka_koty),NULL,body->dalsi->kota_offset*F->Zoom/AA,highlight,0.2,clGray,false,NULL,body->dalsi);

			////odstranění pomocného ukazatele
			B=NULL; delete B;
			kota_od=NULL; delete kota_od;
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí jeden uchop/kolečko znázorňující bod na polygonu
void Cvykresli::uchop(TCanvas *canv,Cvektory::TBod *B,TColor barva)
{
	//nastavení pera a velikosti
	float z=1;if(F->Zoom<=1+2*(short)F->antialiasing)z=1.5/F->Zoom*(1+2*(short)F->antialiasing);//pokud bude hodnota zoomu menší nebo rovno 1, bude uchop stejně velký jako při zoomu 1,5x
  if(F->pom_temp!=NULL)z=z/2.5;//pokud je úchop vykreslován pro obrys kabiny = náhled, musí být zmenšen
	short o=m.m2px(0.4*z);//citelná oblast uchopovací kružnice, pokud by se zde hodnota měnila, nutno změnit i v v.najdi_bod!!!
	canv->Pen->Color=clWhite;//orámování uchopu
	canv->Pen->Width=m.round(0.5*F->Zoom);
	canv->Brush->Style=bsSolid;//nastavení výplně
	canv->Brush->Color=barva;

	//samotné vykreslení
	canv->Ellipse(m.L2Px(B->X)-o,m.L2Py(B->Y)-o,m.L2Px(B->X)+o,m.L2Py(B->Y)+o);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_mGridy(TCanvas *canv)
{
	if(F->pom_temp!=NULL && F->Timer_animace->Enabled==false)//pokud není editovaný nějaký objekt nebo při timeru aplikace se tabulky nezobrazí
	{
		////tabulky elementů
		if(F->pom_temp->elementy!=NULL)
		{
			Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//přeskočí rovnou hlavičku
			while(E!=NULL)
			{
				if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
				{
					E->mGrid->Redraw=false;
					E->mGrid->SetVisibleComponents(false);
					E->mGrid->Left=m.L2Px(E->Xt);//kvůli případnému přesouvání tabulky
					E->mGrid->Top=m.L2Py(E->Yt);//kvůli případnému přesouvání tabulky
					E->mGrid->Show(canv);
				}
				else
				{
					if(F->pom_temp->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE)//pokud je mGrid zobrazen a nejedná se o posun obrazu
					{
						E->mGrid->Redraw=true;
						E->mGrid->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak E->mGrid->Buffer(false);
						if(E->mGrid->VisibleComponents>-1)E->mGrid->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty na základě tohoto zobrazí pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
						E->mGrid->Left=m.L2Px(E->Xt);
						E->mGrid->Top=m.L2Py(E->Yt);
						E->mGrid->Show(canv);
					}
					else//pokud ne, je třeba skrýt všechny komponenty (posun obrazu PAN MOVE či skryté mGridy)
					{
						E->mGrid->SetVisibleComponents(false);
					}
				}
				E=E->dalsi;
			}
			E=NULL;delete E;
		}
		////tabulka pohonu
		if(F->PmG!=NULL)
		{
			TRect oblast_kabiny=F->vrat_max_oblast(F->pom_temp);
			if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
			{
				F->PmG->Redraw=false;
				F->PmG->Left=m.L2Px(F->pom_temp->Xp);
				F->PmG->Top=m.L2Py(F->pom_temp->Yp);
				F->PmG->SetVisibleComponents(false);
				F->PmG->Show(canv);
			}
			else
			{
				if(F->pom_temp->zobrazit_mGrid &&  F->Akce!=F->Takce::PAN_MOVE)//pokud je mGrid zobrazen a nejedná se o posun obrazu
				{
					F->PmG->Redraw=true;
					F->PmG->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak F->PmG->Buffer(false);
					if(F->PmG->VisibleComponents>-1)F->PmG->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty, pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
					F->PmG->Left=m.L2Px(F->pom_temp->Xp);
					F->PmG->Top=m.L2Py(F->pom_temp->Yp);
					F->PmG->Show(canv);
				}
				else//pokud ne, je třeba skrýt komponenty
				{
					F->PmG->SetVisibleComponents(false);
				}
			}
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//pomocná metoda vytvářející rastrové obrazy mGridů, pokud je buffer na NULL, buffrují se všechny tabulky, pokud ne, tak pouze ta z parametru
void Cvykresli::nabuffrovat_mGridy(TmGrid *mGrid)
{
	if(mGrid==NULL)
	{
		if(F->pom_temp->elementy!=NULL)
		{
			Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//přeskočí rovnou hlavičku
			while(E!=NULL)
			{
				E->mGrid->Buffer(true);
				E=E->dalsi;
			}
			E=NULL;delete E;
		}
		if(F->PmG!=NULL)
		{
			F->PmG->Buffer(true);
		}
	}
	else mGrid->Buffer(true);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_kotu(TCanvas *canv,Cvektory::TElement *Element_od,Cvektory::TElement *Element_do)
{
	double O=F->pom_temp->koty_elementu_offset;

	//highlight
	short highlight=0;
	if(Element_do->stav==2)highlight=2;//pokud bude jeden ze zúčastněných elementů vybrán, zvýrazní se a vystoupí daná kóta
	if(Element_do!=NULL && F->MOD==F->NAHLED)
	{
		if(!F->posun_dalsich_elementu && ((F->JID+10)*(-1)==(long)Element_do->n || (F->JID==-101 && F->pom_element->n==Element_do->n)))highlight=1;//v případě, že není požadován posun dalších elementů, zvýrazní i kótu následujícího elementu, že se bude také měnit
		if((F->JID+10)*(-1)==(long)Element_do->n ||  (10<F->JID && F->JID<100))highlight=1;//když se bude editovat hodnota kóty, nebo se bude kóta posouvat, kvůli následnému zaokrouhlování musí bohužel zůstat tady
	}

	//samotné vykreslení kóty -nehotové   využít makro d.Rxy
	//if(Element_od->n==0) vykresli_kotu(canv,F->pom_temp->Xk,F->pom_temp->Yk-F->pom_temp->rozmer_kabiny.y/2.0,Element_do->X,Element_do->Y,Element_do,O,highlight);//od kabiny k prvnímu elementu + dodělat
	//else vykresli_kotu(canv,Element_od->X,Element_od->Y,Element_do->X,Element_do->Y,Element_do,O,highlight);//mezi elementy

	//nový fix    využít makro d.Rxy
//	double C1=0;if(Element_od!=NULL)if(1<=Element_od->eID && Element_od->eID<=4)C1=DoSkRB;bude to chtít pořešit rotaci a dva offsety od a do
//	double C2=0;if(Element_do!=NULL)if(1<=Element_do->eID && Element_od->eID<=4)C2=DoSkRB;
//	if(Element_od->n==0)
//	vykresli_kotu(canv,F->pom_temp->Xk,F->pom_temp->Yk-F->pom_temp->rozmer_kabiny.y/2.0,Element_do->X,Element_do->Y,Element_do,O,highlight);//od kabiny k prvnímu elementu + dodělat
//	else
//	vykresli_kotu(canv,Element_od->X,Element_od->Y-C1,Element_do->X,Element_do->Y-C2,Element_do,O-C2,highlight);//mezi elementy

	//pouze pro rychlé zobrazení - provizorní řešení pro levopravou vodorovnou kabinu
	double x1,y1,x2,y2;
	if(Element_do->orientace==0||Element_do->orientace==180)//vodorovná kabina
		{if(Element_od->n==0)x1=F->pom_temp->elementy->dalsi->geo.X1;else x1=Element_od->X;y1=F->pom_temp->elementy->dalsi->geo.Y1;x2=Element_do->X;y2=y1;}
	else
		{if(Element_od->n==0)y1=F->pom_temp->elementy->dalsi->geo.Y1;else y1=Element_od->Y;x1=F->pom_temp->elementy->dalsi->geo.X1;y2=Element_do->Y;x2=x1;}
	if(x2<F->pom_temp->elementy->dalsi->geo.X1)O=(O-0.66)*(-1);//ošetření chybného zobrazení kóty elementu, který je před kabinou
	vykresli_kotu(canv,x1,y1,x2,y2,Element_do,O,highlight);
	if(Element_od->n!=0&&Element_do->n>1)//pokud jsou minimálně 2 elementy vložené
	{
		//dojde k otestování zda mají tyto 2 elementy nebo alespoň jeden lakovací okna
		bool test1=false,test2=false;
		double x1,x2,y1,y2;
		switch(Element_od->eID)
		{case 1:case 7:case 11:case 15:case 101:case 105:case 3:case 9:case 13:case 17:case 103:case 107:test1=true;break;}
		switch(Element_do->eID)
		{case 1:case 7:case 11:case 15:case 101:case 105:case 3:case 9:case 13:case 17:case 103:case 107:test2=true;break;}
		//pokud první element nemá lakovací okno projde se objekt a prohlédne všechny elementy před posledním zda nějaky nemá LO
		if(!test1)
		{
			Cvektory::TElement *E=F->pom_temp->elementy->dalsi;
			while(E!=NULL)
			{
				if(E->n==Element_do->n)break;
				switch(E->eID)//pokud nějaký má dojde k uložení jeho ukazatele do prvního elementu
				{case 1:case 7:case 11:case 15:case 101:case 105:case 3:case 9:case 13:case 17:case 103:case 107:Element_od=E;test1=true;break;}
				E=E->dalsi;
			}E=NULL;delete E;
		}
		//nastavení bodů vykreslení pro jednotlivé rotace
		TRect E_od=F->souradnice_LO(Element_od),E_do=F->souradnice_LO(Element_do);
		double offset=1;
		switch((int)F->pom_temp->orientace)
		{
			case 0:y1=m.P2Ly(E_od.top);y2=m.P2Ly(E_do.bottom);x1=x2=F->pom_temp->elementy->dalsi->geo.X1;break;
			case 90:x1=m.P2Lx(E_od.right);x2=m.P2Lx(E_do.left);y1=y2=F->pom_temp->elementy->dalsi->geo.Y1;break;
			case 180:y1=m.P2Ly(E_od.bottom);y2=m.P2Ly(E_do.top);x1=x2=F->pom_temp->elementy->dalsi->geo.X1;break;
			case 270:x1=m.P2Lx(E_od.left);x2=m.P2Lx(E_do.right);y1=y2=F->pom_temp->elementy->dalsi->geo.Y1;offset*=-1;break;
		}
		if(10<F->JID && F->JID<100)highlight=0;//vypnutí highlightu při naznačení změny offsetu
		//pokud mají oba elementy lakovací okna je mezi nimi vykreslena needitavatelná kóta
		if(test1&&test2)vykresli_kotu(canv,x1,y1,x2,y2,Element_do,1,highlight,0.2,clGray,true);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//v metrických jednotkách kromě width, zde v px + automaticky dopočítává délku a dosazuje aktuálně nastavené jednotky,highlight: 0-ne,1-ano,2-ano+vystoupení kóty i pozičně, aktElement pokud bude NULL, předpokládá se, že je to kóta kabiny
void Cvykresli::vykresli_kotu(TCanvas *canv,double X1,double Y1,double X2,double Y2,Cvektory::TElement *aktElement,double Offset,short highlight,float width,TColor color,bool LO_kota,Cvektory::TKomora *komora)
{    //Jednotky=" [s]";if(F->DKunit==3)Jednotky=" [min]";
	double delka=0;
	if(F->pom_temp->pohon==NULL && F->DKunit>1)F->DKunit=F->DKunit-2;//ošetření pro případ není pohon a jsou špatně nastaveny jednotky
	if(F->DKunit>1)//zobrazení kót v čase
	{
		if(F->pom_temp->pohon!=NULL && F->pom_temp->pohon->aRD>0)
		delka=m.delka(X1,Y1,X2,Y2)/F->pom_temp->pohon->aRD/(1+59.0*(F->DKunit-2));//výpočet délky a šířky kabiny + případný převod m->mm
		if(aktElement!=NULL) delka=v.vzdalenost_od_predchoziho_elementu(aktElement)/F->pom_temp->pohon->aRD/(1+59.0*(F->DKunit-2));//výpočet vzdálenosti mezi elementy
		if(LO_kota)delka=m.round2double(F->vzdalenost_meziLO(aktElement,F->pom_temp->orientace),2)/F->pom_temp->pohon->aRD/(1+59.0*(F->DKunit-2));
	}
	else//standardní zobrazení ve vzdálenost
	{
		delka=m.delka(X1,Y1,X2,Y2)*(1+999*F->DKunit);//výpočet délky a šířky kabiny + případný převod m->mm
		if(aktElement!=NULL) delka=v.vzdalenost_od_predchoziho_elementu(aktElement)*(1+999*F->DKunit);//výpočet vzdálenosti mezi elementy
		if(LO_kota)delka=F->outDK(m.round2double(F->vzdalenost_meziLO(aktElement,F->pom_temp->orientace),2));
	}
	//odstaveno zobrazujeme na 3 realná delka=m.round2double(delka,8);//výpočet délky s max zobrazením na 8 míst (z důvodu případů 0.000000001 atp.) pouze v případě metrů, v mm by přetékalo při výpočtu, bylo by třeba long double
	//if(!F->DKunit)delka=m.round2double(delka,5);//výpočet délky s max zobrazením na 8 míst (z důvodu případů 0.000000001 atp.) pouze v případě metrů, v mm by přetékalo při výpočtu, bylo by třeba long double
	//else delka=m.round2double(delka,3);//if(AnsiString(delka).Pos("00000000001"))F->ms.MyToDouble(AnsiString(delka).SubString(1,AnsiString(delka).Pos("00000000001")-1));//pro mm ošetření proti 00000000001, protože nelze použít zaokrouhlení na větší počet desitnných míst
	AnsiString T=m.round2double(delka,3/*nefuguje zde správně,".."*/);//standardní zobrazení na 3 reálná místa
	//odstaveno zobrazujeme na 3 realná if(highlight==1 || F->editace_textu)T=delka;//pokud se na kótu najede a předpokládá se editace tak se číslo rozbalí - nezaokrouhluje se, editace textu je možná navíc
	vykresli_kotu(canv,m.L2Px(X1),m.L2Py(Y1),m.L2Px(X2),m.L2Py(Y2),T,aktElement,m.m2px(Offset),highlight,width,color,LO_kota,komora);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//v px + dosazuje aktuálně nastavené jednotky,highlight: 0-ne,1-ano,2-ano+vystoupení kóty i pozičně, aktElement pokud bude NULL, předpokládá se, že je to kóta kabiny
void Cvykresli::vykresli_kotu(TCanvas *canv,long X1,long Y1,long X2,long Y2,AnsiString Text,Cvektory::TElement *aktElement,int Offset,short highlight,float width, TColor color,bool LO_kota,Cvektory::TKomora *komora,Cvektory::TBod *bod)
{
	////vstupní proměnné
	if(F->JID==-10 && F->MOD==F->NAHLED)highlight=0;//highlight - pokud se mění pouze jednotky, tak se kóta nehiglightuje
	if(aktElement==NULL&&komora==NULL&&bod==NULL)highlight=0;//highlight -odstranění highlightu na kótách mezi lak. okny
	short meritko=1;if(F->MOD==F->SCHEMA){width*=5;meritko=5;}//měřítko (náhled vs. schéma)
	width=m.round(width*F->Zoom);if(highlight)width*=2;//šířka linie
	short Presah=m.round(1.3*F->Zoom);if(Offset<0)Presah*=-1;//přesah packy u kóty,v případě záporného offsetu je vystoupení kóty nazákladě tohot záporné
  if(F->pom_temp!=NULL)Presah/=2.0;//zmenšení odsazení kót při highlightu v náhledu
	short V=0;if(highlight==2)V=1;//vystoupení kóty
	short H=0;if(highlight)H=1;
	short M=0;if(10<F->JID && F->JID<100 && F->MOD==F->NAHLED)M=1;//při celkovém posunu kót se postranní spojnice nově nezvýrazňují

	////samotné vykreslení kót
	//nastaveni pera
	canv->Brush->Style=bsSolid;//asi nemá význam, možná kvůli písmu níže
	set_pen(canv,color,width/(1+M*1.0),PS_ENDCAP_SQUARE);
	//vykreslení postraních spojnic
	float azimut=fmod(m.azimut(X1,Y1,X2,Y2)+90,360);//určení azimutu kótované přímky
	double x1=X1,y1=Y1,x2=X2,y2=Y2;//body sloužící k přepočtu souřadnic + hodnota celkového odsazení
	x1-=sin(DegToRad(azimut))*(Offset+Presah+Presah*V);y1-=cos(DegToRad(azimut))*(Offset+Presah+Presah*V);
	x2-=sin(DegToRad(azimut))*(Offset+Presah+Presah*V);y2-=cos(DegToRad(azimut))*(Offset+Presah+Presah*V);
	line(canv,X1,Y1,x1,y1);
	line(canv,X2,Y2,x2,y2);
	//vykreslení hlavní linie
	x1+=sin(DegToRad(azimut))*(Presah);y1+=cos(DegToRad(azimut))*(Presah);
	x2+=sin(DegToRad(azimut))*(Presah);y2+=cos(DegToRad(azimut))*(Presah);
	line(canv,x1,y1,x2,y2);
	//vykreslení šipek
	sipka(canv,x1,y1,m.azimut(X1,Y1,X2,Y2)*(-1),false,0.5*(1+0.3*H)*meritko,color,color,pmCopy,psSolid,false);
	sipka(canv,x2,y2,m.azimut(X1,Y1,X2,Y2)*(-1)-180,false,0.5*(1+0.3*H)*meritko,color,color,pmCopy,psSolid,false);

	////záměna (podsunutí editovaného) textu v případě EDITACE právě touto metodou vykreslované kóty - editovaného textu (abychom mohli text koty refreshovat, ale aby ještě nebylo nutné měnit rozměry) (protože se cyklem vykreslují všechny kóty i při platném JID)
	if(F->editace_textu)//ošetření proti vykreslování editovaného textu na kótě mezi lak. okny
	{
		if(aktElement==NULL)//předpokládá se, že je to kóta kabiny
		{
			if(F->index_kurzoru==-8 && Y1==Y2)if(F->editovany_text=="")Text="";else Text=F->editovany_text;//pro vodorovnou kótu
			if(F->index_kurzoru==-9 && X1==X2)if(F->editovany_text=="")Text="";else Text=F->editovany_text;//pro svislou kótu
		}
		else if(F->index_kurzoru<=-11)//ostatní kóty
		{
			if(F->pom_element_temp!=NULL)//nutné ošetření z pohledu paměťové chyby, toto nemůže být znegované výše, protože by při přepisování kóty kabiny se přepisovaly i kóty elementů
			if((aktElement->n==F->pom_element_temp->n && F->index_kurzoru==-101 && LO_kota) || (aktElement->n==F->pom_element_temp->n && F->index_kurzoru!=-101 && !LO_kota))//aktuální vykreslováná kota
			{
				if(F->editovany_text=="")Text="";//musí být v každé zvlášť pro řešení konkrétní editované kóty
				else Text=F->editovany_text;
			}
		}
	}
	if(F->editace_textu&&komora!=NULL)//zobrazování editovaného textu v kótách komor
	{
		if(F->pom_komora_temp!=NULL)if(komora->n==F->pom_komora_temp->n)//ošetření + aktuální vykreslovaná kóta
		{if(F->editovany_text=="")Text="";else Text=F->editovany_text;}
	}
	if(F->editace_textu&&bod!=NULL)//zobrazování editovaného textu v kótách komor
	{
		if(F->pom_bod_temp!=NULL)if(bod->n==F->pom_bod_temp->n)//ošetření + aktuální vykreslovaná kóta
		{if(F->editovany_text=="")Text="";else Text=F->editovany_text;}
	}

	////popisek
	canv->Font->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	canv->Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	canv->Font->Name=F->aFont->Name;
	canv->Font->Color=color;
	canv->Font->Size=m.round(width*F->aFont->Size);//už se nenásobí *Zoom, protože width se již násobí v úvodu metody
	if(highlight)
	{                                                                 //v případě, že není požadován posun dalších elementů, zvýrazní i kótu následujícího elementu, že se bude také měnit
		if(aktElement!=NULL && (F->JID+10)*(-1)==(long)aktElement->n || !F->posun_dalsich_elementu && aktElement!=NULL && (F->JID+10)*(-1)==(long)aktElement->predchozi->n || ((F->JID==-8 || F->JID==-9) && F->MOD==F->NAHLED))canv->Font->Style = TFontStyles()<< fsBold;//pouze když se mění hodnota kóty
		canv->Font->Size=m.round(canv->Font->Size/2.0);//při highlighnutí se text se šířkou nezvětštuje (proto /2 návrat na původní hodnotu, pouze ztučňuje a to jen za předpokladu, změny hodnot kót nikoliv linie kóty (její pozice/offsetu)
	}
	else canv->Font->Style = TFontStyles();//vypnutí tučného písma
	SetBkMode(canv->Handle,OPAQUE);//nastvení netransparentního pozadí kóty
	canv->Brush->Color=clWhite;//nastvení netransparentního pozadí popisku kóty
//	AnsiString Jednotky=" [m]";if(F->DKunit==1)Jednotky=" [mm]";if(F->DKunit==2)Jednotky=" [s]";if(F->DKunit==3)Jednotky=" [min]";
	long X=(x1+x2)/2-canv->TextWidth(Text)/2;if(y1==y2)X=(x1+x2)/2-canv->TextWidth(Text/*+Jednotky*/)/2;//pro vodorovnou kótu zarovnání jinak
	long Y=(y1+y2)/2-canv->TextHeight(/*Jednotky*/Text/*nahrazeno*/)/2; //pozn. záměrně je zde TextHeight(Jednotky) z důvodu, že při smazání hodnoty by byl text prázdný a následně by to špatně pozicovalo jednotky
	canv->TextOutW(X,Y,Text);//číselná hodnota kóty
	canv->Font->Color=(TColor)RGB(43,87,154);
	if(F->JID==-10 && F->MOD==F->NAHLED)canv->Font->Style = TFontStyles()<< fsBold;else canv->Font->Style = TFontStyles();//pokud se editují jednotky, jinak (ani při highlightu se neztučňují)
//	canv->TextOutW(X+canv->TextWidth(Text),Y,Jednotky);//jednotky

	////navrácení citelné oblasti popisku a jednotek kóty pro další použití a šetření strojového času
//	if(F->MOD==F->NAHLED && F->pom_temp!=NULL)//pouze pokud se jedná o náhled a existuje ukazatel na pom_temp (což by mělo být při náhledu sice vždy...)
//	{
		T3Rect R;float AA=3.0;if(!F->antialiasing)AA=1;
		//oblast kóty (pro kótu kabiny se zatím nevyužívá, protože kóta kabiny nelze odsadit)
		TRect R0;   R0=TRect(m.round(x1/AA),m.round((y1)/AA),m.round(x2/AA),m.round((y2)/AA));
//		if(y1==y2)R0=TRect(m.round(x1/AA),m.round((y1-Presah)/AA),m.round(x2/AA),m.round((y2+Presah)/AA));//pro vodorovnou kótu
//		else R0=TRect(m.round((x1-Presah)/AA),m.round(y1/AA),m.round((y2+Presah)/AA),m.round(y2/AA));//pro svislou kótu
		//oblast hodnota
		R.rect1=TRect(m.round(X/AA),m.round(Y/AA),m.round((X+canv->TextWidth(Text))/AA),m.round((Y+canv->TextHeight(/*Jednotky*/Text/*nahrazeno*/))/AA));//pozn. záměrně je zde TextHeight(Jednotky) z důvodu, že při smazání hodnoty by byl text prázdný a následně by to špatně pozicovalo jednotky
		//oblast jednotky
//		R.rect2=TRect(m.round((X+canv->TextWidth(Text)+canv->TextWidth(" "))/AA),m.round(Y/AA),m.round((X+canv->TextWidth(Text)+canv->TextWidth(Jednotky))/AA),m.round((Y+canv->TextHeight(Jednotky))/AA));
																															 //odebrání mezery
//		if(aktElement==NULL&&ukladat_do_elementu)//předpokládá se, že je to kóta kabiny
//		{
//			if(Y1==Y2)//pro vodorovnou kótu
//			{
//				F->pom_temp->kabinaKotaX_oblastHodnotaAJednotky.rect1=R.rect1;//hodnoty
//				F->pom_temp->kabinaKotaX_oblastHodnotaAJednotky.rect2=R.rect2;//jednotky
//			}
//			else//pro svislou kótu
//			{
//				F->pom_temp->kabinaKotaY_oblastHodnotaAJednotky.rect1=R.rect1;//hodnoty
//				F->pom_temp->kabinaKotaY_oblastHodnotaAJednotky.rect2=R.rect2;//jednotky
//			}
//		}
		if(aktElement!=NULL && !LO_kota)//kóty mezi elementy
		{
			aktElement->citelna_oblast.rect0=R0;//oblast kóty
			aktElement->citelna_oblast.rect1=R.rect1;//hodnoty
			aktElement->citelna_oblast.rect2=R.rect2;//jednotky
		}
		if(aktElement!=NULL && LO_kota)aktElement->citelna_oblast.rect4=R.rect1;//LO kóta
		if(komora!=NULL)//ukládání citelných oblastí do komory
		{
			komora->kota.rect0=R0;
			komora->kota.rect1=R.rect1;
			komora->kota.rect2=R.rect2;
		}
		if(bod!=NULL)//ukládání citelných oblastí do druhého bodu úsečky
		{
			bod->kota.rect0=R0;
			bod->kota.rect1=R.rect1;
			bod->kota.rect2=R.rect2;
		}
//	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::STOPKA(Cvektory::TVozik *ukaz)//zajištuje základní funkcionalitu technologického objektu v režimu S&G při vykreslování simulaci
//{
//	if(ukaz->segment!=NULL)
//	{
//		if(ukaz->segment->id==11)//stopka -- zatimí provizorně na objekt s ID 11
//		{
//		 if(ukaz->pozice<=0)ukaz->stav=ukaz->segment->stav;
//		}
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::SG(Cvektory::TVozik *ukaz)//zajištuje základní funkcionalitu technologického objektu STOPKA při vykreslování simulaci
//{
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//	if(u_seg!=NULL)
//	{
//		if(u_seg->rezim==0)//S&G
//		{
//			switch (u_seg->stav)
//			{
//				case 0:
//				{
//					if(ukaz->pozice<=0)//pouze pro nový příchozí prvek
//					{
//						ukaz->stav=0;
//						ukaz->timer=u_seg->CT*60;
//						u_seg->stav=1;//čekací mod
//					}
//					//Form1->Memo1->Lines->Add(ukaz->pozice);
//					break;
//				}
//				case 1:
//				{
//					if(--ukaz->timer==0)
//					{
//						u_seg->stav=2;//vozík může opustit objekt
//						ukaz->stav=1;//vozík bude čekat na palec
//					}
//				}
//				break;
//				case 2:
//				u_seg->stav=0;//objekt pozdrží další vozík
//				break;
//			}
//		}
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//bool Cvykresli::KOLIZE(Cvektory::TVozik *V1,Cvektory::TVozik *V2)//vrací logickou hodnotu zda došlo či nedošlo ke kolizi s jiným vozíkem
//{  //provizorní algoritmus není dotažený
//	 if(m.delka(V1->X,V1->Y,V2->X,V2->Y) <= V1->delka+V2->delka)//zatím nedokonalá detekce konfliktu
//		return true;
//	 else
//		return false;
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykresluje měřítko
void Cvykresli::meritko(TCanvas *canv,long X,long Y)
{
	if(F->MOD!=F->TECHNOPROCESY)
	{
			//proměnné nastavení měřítka
			int L=Form1->scSplitView_LEFTTOOLBAR->Width+5;//umístění na X - levého výchozího kraje měřítka
			if(Form1->scSplitView_LEFTTOOLBAR->Visible==false)L=5+168;//pokud je levé menu skryto
			if(X>-1 && Y>-1)L=X;
			int T=Form1->scGPPanel_statusbar->Top-20;//umistění na Y - horního výchozího kraje měřítka
			if(Y>-1 && Y>-1)T=Y;
			if (F->scGPPanel_bottomtoolbar->Visible) T=T-F->scGPPanel_bottomtoolbar->Height;//posun při zobrazení toolbaru
			int H=5;//výška měřítka
			int K=1;//krok v metrech
			if(Form1->Zoom==0.5)K=2;
			if(Form1->Zoom==0.25)K=5;
			int M=10;//MAX políček
			TColor barva_meritko=(TColor)RGB(127,127,127);//barva měřítka - nesmí být 128,128,128, resp. clGray
			if(F->MOD==F->NAHLED)barva_meritko=(TColor)RGB(150,150,150);

			//nastavení pera a fontu canvasu
			canv->Pen->Color=barva_meritko;
			canv->Pen->Width=1;
			canv->Pen->Style=psSolid;
			canv->Brush->Style=bsSolid;
			canv->Pen->Mode=pmCopy;
			canv->Font->Size=8;
			canv->Font->Name="Arial";
			canv->Font->Color=barva_meritko;

			//popisek 0
			canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
			SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
			canv->TextOutW(L-canv->TextWidth("0")/2+1,T+5,"0");

			//vykreslení políček měřítka
			int i=0;
			for(;i<M;i+=K)
			{
				if(i%(2*K))canv->Brush->Color=barva_meritko;//výplň barevna
				else canv->Brush->Color=clWhite;//výplň bílá                 //+1 pouze grafická korekce
				canv->Rectangle(m.L2Px(m.P2Lx(L)+i),T,m.L2Px(m.P2Lx(L)+(i+K))+1,T+H);
			}

			//musí být zde znovu, nastavení transparentního pozadí
			SetBkMode(canv->Handle,TRANSPARENT);
			//popisek polovina
			if(Form1->Zoom>=1)
			{
				canv->MoveTo(m.L2Px(m.P2Lx(L)+i/2),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i/2),T+7);
				canv->TextOutW(m.L2Px(m.P2Lx(L)+i/2)-canv->TextWidth(M/2)/2,T+5,AnsiString(M/2));
			}
			//popisek MAX
			canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);
			canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+5,AnsiString(M)+" m");
	}
	else //pro mod technologické procesy
	{
			//proměnné nastavení měřítka
			int L=Form1->scSplitView_OPTIONS->Width-50*2-15;//umístění na X - levého výchozího kraje měřítka
			if(Form1->scSplitView_LEFTTOOLBAR->Visible==false)L=Form1->ClientWidth-50*2-15;//pokud je levé menu skryto
			int T=Form1->scGPPanel_mainmenu->Height+5;//umistění na Y - horního výchozího kraje měřítka
			int H=5;//výška měřítka
			int K=1*5;//krok v metrech
			int M=2*5;//MAX políček
			TColor barva_meritko=(TColor)RGB(128,128,128);//barva měřítka
			//TColor barva_meritko=(TColor)RGB(43,87,154);//(0,120,215);barva měřítka

			//nastavení pera a fontu canvasu
			canv->Pen->Color=barva_meritko;
			canv->Pen->Width=1;
			canv->Pen->Style=psSolid;
			canv->Brush->Style=bsSolid;
			canv->Pen->Mode=pmCopy;
			canv->Font->Size=8;
			canv->Font->Name="Arial";
			canv->Font->Color=barva_meritko;

			//popisek 0
			canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
			SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
			canv->TextOutW(L-canv->TextWidth("0")/2+1,T+5,"0");

			//vykreslení políček měřítka
			int i=0;
			for(;i<M;i+=K)
			{
				if(i%(2*K))canv->Brush->Color=barva_meritko;//výplň barevna
				else canv->Brush->Color=clWhite;//výplň bílá                 //+1 pouze grafická korekce
				canv->Rectangle(m.L2Px(m.P2Lx(L)+i),T,m.L2Px(m.P2Lx(L)+(i+K))+1,T+H);
			}

			//musí být zde znovu, nastavení transparentního pozadí
			SetBkMode(canv->Handle,TRANSPARENT);

			//popisek MAX
			canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);
			canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+5,AnsiString(2)+" m");
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí packu jako spojnici mezi komponentami
void Cvykresli::vykresli_packu(TCanvas *canv, int X1,int Y1,int X2,int Y2,TColor color,short Width,short OffsetX,short OffsetY)
{
	//nastavení pera
	canv->Pen->Width=Width;
	canv->Pen->Color=color;
	//vykreslení spojnice
	canv->MoveTo(X1,Y1);
	canv->LineTo(X1+OffsetX,Y1+OffsetY);
	if(OffsetX==0 && OffsetY!=0)canv->LineTo(X2,Y2+OffsetY);//horizontální packy
	else canv->LineTo(X1+OffsetX,Y2+OffsetY);//vertikální packy
	canv->LineTo(X2,Y2);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_packy_PL(TCanvas *canv,short typ,short zamek_aRD,short zamek_R,short zamek_Rz,short zamek_Rx)
void Cvykresli::vykresli_packy_PL(TCanvas *canv,TscGPButton *zamek_aRD,TscGPButton *zamek_R,TscGPButton *zamek_Rz,TscGPButton *zamek_Rx)
{
	TColor Color=(TColor)10114859;

	short O=-8;
	short Top=326;
	short aRD=636;
	short R	 =776;
	short Rz =917;
	short Rx =1059;

	//korelace aRD,Rz,Rx
	if(zamek_R->ImageIndex==37){vykresli_packu(canv,aRD,Top,Rz,Top,Color,1,0,O);vykresli_packu(canv,Rz,Top,Rx,Top,Color,1,0,O);}
	//korelace aRD,Rz,R
	if(zamek_Rx->ImageIndex==37){vykresli_packu(canv,aRD,Top,R,Top,Color,1,0,O);vykresli_packu(canv,R,Top,Rz,Top,Color,1,0,O);}
	//korelace R,Rx
	if(zamek_aRD->ImageIndex==37 || zamek_Rz->ImageIndex==37){vykresli_packu(canv,R,Top,Rx,Top,Color,1,0,O);}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslování-vypisování tool tipu
void Cvykresli::vykresli_tip(TCanvas *canv)
{
	canv->Font->Color=m.clIntensive(clRed,100);
	//SetBkMode(canv->Handle,TRANSPARENT);//nastvení netransparentního pozadí
	canv->Font->Size=14;
	canv->Font->Name="Arial";
	canv->Brush->Color=clWhite;
	canv->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	if(F->scGPPanel_bottomtoolbar->Visible)
		canv->TextOutW(F->ClientWidth-canv->TextWidth(F->TIP)-10,F->scGPPanel_bottomtoolbar->Top-25,F->TIP);
	else
		canv->TextOutW(F->ClientWidth-canv->TextWidth(F->TIP)-10,F->scGPPanel_statusbar->Top-25,F->TIP);
	canv->Font->Color=clBlack;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//vytvoří zvuk
void Cvykresli::sound()
{
	Beep(400,250);	// 440 hertz (A4) for half a second
	Beep(500,250);	// 494 hertz for half a second
	Beep(600,250);	// 523 hertz for half a second
	Beep(700,250);	// 587 hertz for half a second
	Beep(800,500);	// 659 hertz for half a second
}
//------------------------------------------------------------------------------------------------------------------------------------------------------


