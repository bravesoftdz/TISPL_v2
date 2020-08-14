//---------------------------------------------------------------------------
#pragma hdrstop
#include "vykresli.h"
#include "Unit1.h"
#include "kabina_schema.h"
#include "stdlib.h"
#include "antialiasing.h"
#include "superform.h"
#include "parametry_linky.h"
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
	zobrazit_celou_zpravu=0;//proměnná určující, která zpráva bude zobrazena
	zprava_highlight=0;//pomocná proměnná sloužící na ztučnění dané zpravy či na její zobrazení
	//default barvy
	clStenaHaly=TColor RGB(147,166,182);
	clStenaKabiny=m.clIntensive(clStenaHaly,40);
	clPasiv=m.clIntensive(clBlack,180);
	clError=clRed;
	clWarning=TColor RGB(255,165,0);
	clChassis=(TColor)RGB(50,50,50);
	clJig=clPurple;//pozn. defualt zakázka má definici barvy přimo ve v.vytvor_default_zakazku()
	clMeridlo=TColor RGB(255,31,255);
	 //pěkná modrá 79,122,186   další pěkná modrá světle (YR.NO): 0,185,241   ico tispl: 33,209,255
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//stav: -3 kurzor, -2 normal (implicitně), -1-highlight bez editace, 0-editace zvýrazní všechny body, 1-až počet bodů zvýraznění daného bodu,počet bodů+1 zvýraznění dané hrany včetně sousedícícíh úchopů (např. pro polygono o 6 bodech) bude hodnota stavu 7 zvýraznění první hrany (od bodu 1 do bodu 2)
void Cvykresli::vykresli_halu(TCanvas *canv,int stav)
{
	//změny stavů
	stav=-2;//defaultní stav
	if(F->MOD==F->LAYOUT)//ošetření, tato metoda se spouští i při náhledu !!!!
	{
    if(F->Akce==F->MOVE_HALA)stav=0;
		if(F->JID==0)stav=F->pom_bod->n;//body
		if(F->JID==1 && F->pom_bod->n==1)stav=2*v.HALA.body->predchozi->n;//poslední úsečka
		if(F->JID==1 && F->pom_bod->n!=1)stav=v.HALA.body->predchozi->n+F->pom_bod->n-1;//ostatní úsečky
	}
	if(F->OBJEKT_akt!=NULL)stav=-1;
	//nastavení kót
	bool zobrazit_koty=true;
	if(F->MOD==F->EDITACE || F->MOD==F->SIMULACE)zobrazit_koty=false;
	//vykreslení
	short sirka_steny_px=m.m2px(0.4);//m->px
	TColor clHala=clStenaHaly;//defaultní barva
	if(F->OBJEKT_akt!=NULL)clHala=m.clIntensive(m.clIntensive(clBlack,110)/*clHala*/,100-F->scGPTrackBar_intenzita->Value);//zesvětlení při spuštěné editaci, zesvětlení tady i v metodě polygon (musí být 2x)
	if(F->OBJEKT_akt!=NULL && F->scGPTrackBar_intenzita->Value>5 || F->OBJEKT_akt==NULL)polygon(canv,v.HALA.body,clHala,sirka_steny_px,stav,zobrazit_koty);
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
//vrátí referenční logické (v metrech) souřadnice robota (tzn. bod v místě trysky), převede dle aktuální rotace symbolu a uchopovacích (skutečných) souřadnic robota, kontroluje, zda se jedná skutečně o Robota
TPointD Cvykresli::Rxy(Cvektory::TElement *Element)
{
	TPointD RET; RET.x=Element->X; RET.y=Element->Y;
	if(1<=Element->eID && Element->eID<=4 || 7<=Element->eID && Element->eID<=18 /*|| 101<=Element->eID && Element->eID<=108*/)//ovlivní patřičně pouze roboty
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
void Cvykresli::vykresli_vektory(TCanvas *canv, short scena)//scena 0 - vše, scena 1 - statická scéna, scena 2 - dynamická scéna (odkud je voláno), následně se vyfiltruje podle toho, jak je nastaveno
{
	/////Vykreslení haly
	if(m.getValueFromPosition(SCENA,1)==scena)vykresli_halu(canv);

	/////Vykreslení objektů
	if(m.getValueFromPosition(SCENA,2)==scena)vykresli_objekty(canv);
	if(m.getValueFromPosition(SCENA,2)==1 && scena==2 && F->pom!=NULL)vykresli_objekt(canv,F->pom);//v případě v layoutu editovaného objektu, jsou-li všechny objekty nastaveny jako statické, vykreslí editovaný v rámci dynamické scény

	/////Vykreslení dopravníků
	if(m.getValueFromPosition(SCENA,3)==scena)vykresli_dopravnik(canv);

	/////Vykreslení elementů
	if(m.getValueFromPosition(SCENA,4)==scena)vykresli_elementy(canv,scena);							//implicitně statická sekce (může být ale i v dynamické scéně), popř. pokud se vykresluje vše do dynamické vykresluje jak statickou tak dynamickou sekci společně
	if(m.getValueFromPosition(SCENA,5)==scena && SCENA!=0)vykresli_elementy(canv,scena+2);//implicitně dynamická sekce (může být ale i ve statické scéně), pokud se vykresluje vše do dynamické, tato se již kvůli zbytečnému dalšímu průchodu cyklu již nevykresluje, protože výše vykresluje statickou tak dynamickou sekci společně

	/////VALIDACE její samotné provední, vnitřek metody se provede jen pokud duvod_validovat==2
	if(m.getValueFromPosition(SCENA,7)==scena)v.VALIDACE();

	/////vykreslení VOZÍKů, musí být až za VALIDACí kvůli generování vozíků, které je ve VALIDACi
	if(F->MOD!=F->TVORBA_CESTY && F->Akce!=F->GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT && m.getValueFromPosition(SCENA,6)==scena)vykresli_voziky(canv);

	/////VALIDACE výpis formou zpráv musí být za vozíky, aby byla zcela nahoře
	if(F->MOD!=F->TVORBA_CESTY && F->Akce!=F->GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT && m.getValueFromPosition(SCENA,7)==scena)vypis_zpravy(canv);
}
//---------------------------------------------------------------------------
//samotné objekty, kreslím až v samostatném cyklu, aby se nakreslilo do horní vrstvy
void Cvykresli::vykresli_objekty(TCanvas *canv)
{
	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	////prvně se kreslí objekty bez stěny
	while (O!=NULL)
	{
		//pokud je aktivní editace přeskočí vykreslení obrysu aktuálně editovaného objektu nebo pokud se jedná o Layout přeskakuje aktuální pom objekt, je-li statická scéna nastavena na 1, protože ten se nativně vykresluje samostatně do dynamické
		if(O->sirka_steny==0 && (F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n || F->OBJEKT_akt==NULL && !(m.getValueFromPosition(SCENA,2)==1 && O==F->pom)))vykresli_objekt(canv,O);
		O=O->dalsi;
	}
	////potom až objekty se stěnou
	O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		//pokud je aktivní editace přeskočí vykreslení obrysu aktuálně editovaného objektu nebo pokud se jedná o Layout přeskakuje aktuální pom objekt, je-li statická scéna nastavena na 1, protože ten se nativně vykresluje samostatně do dynamické
		if(O->sirka_steny>0 && (F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n || F->OBJEKT_akt==NULL && !(m.getValueFromPosition(SCENA,2)==1 && O==F->pom)))vykresli_objekt(canv,O);
		O=O->dalsi;
	}
	delete O;O=NULL;
	////vykreslení aktuálně editovaného objektu nad všechny ostatní objekty
	if(F->OBJEKT_akt!=NULL)vykresli_objekt(canv,F->OBJEKT_akt);
}
//---------------------------------------------------------------------------
void Cvykresli::vykresli_elementy(TCanvas *canv,short scena)//scena 0 - vše do dynamické, scena 1 - implicitně statické elementy do statické scény, scena 2 - implicitně statické elementy do dynamické scény, scena 3 - implicitně dynamické elementy do statické scény, scena 4 - implicitně dynamické elementy do dynamické scény
{
	//vykreslování z ELEMENTY
	float sipka_velikost=0.1*F->Zoom; if(sipka_velikost<1*3)sipka_velikost=1*3;
	TPointD bod;
	short stav;
	if(F->MOD!=F->TVORBA_CESTY)
	{
		Cvektory::TElement *E=v.ELEMENTY->dalsi,*pom=NULL;
		TPoint *tab_pruchodu=new TPoint[v.pocet_vyhybek+1];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
		while(E!=NULL)
		{
			//nastavování stavu
			stav=1;
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==E->objekt_n)stav=1;//elementy v aktivním objektu
			else stav=-1;//disabled elementy ostatních objektů
			if(stav!=-1 || F->OBJEKT_akt==NULL)stav=E->stav;//předávání stavu v aktivní kabině pro highlightování elementů// přídání OBJEKT_akt!=NULL MV 24.6.2020
			if(E->eID==0 && E->data.pocet_voziku==0)//průjezdní stopka
			{
				stav=0;
				if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n)stav=-2;//průjezdní stopka v needitovaném objektu
			}
			//vykreslení elementu a pozic
			if(F->MOD!=F->SIMULACE && scena<=2 && stav!=-2 && stav!=0)vykresli_pozice_a_zony(canv,E);
			if(!(F->OBJEKT_akt!=NULL && E->objekt_n!=F->OBJEKT_akt->n && F->scGPTrackBar_intenzita->Value<5))vykresli_element(canv,scena,m.L2Px(E->X),m.L2Py(E->Y),E->name,E->short_name,E->eID,1,E->orientace,stav,E->data.LO1,E->OTOC_delka,E->data.LO2,E->data.LO_pozice,E);
			//uložení citelné oblasti pro další použití
			E->citelna_oblast.rect3=aktOblast;
			//vykreslení kót
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==E->objekt_n && F->OBJEKT_akt->zobrazit_koty)vykresli_kotu(canv,E);//mezi elementy
			//E=E->dalsi;
			pom=E->dalsi;

			////vykreslení spojnic pokud geometrie nenavazuje
			if(scena<=2)
			{
				canv->Pen->Style=psDash;
				canv->Pen->Mode=pmCopy;
				canv->Pen->Width=1;
				canv->Pen->Color=clRed;
				canv->Brush->Style=bsClear;
				if(pom!=NULL && pom->geo.delka!=0 && E->geo.delka!=0)
				{
					if(pom->eID==301 && pom->predchozi2==E){bod.x=pom->X;bod.y=pom->Y;}
					else {bod.x=pom->geo.X1;bod.y=pom->geo.Y1;}
					if(m.round2double(E->geo.X4,2)!=m.round2double(bod.x,2) || m.round2double(E->geo.Y4,2)!=m.round2double(bod.y,2))
					{
						canv->MoveTo(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
						canv->LineTo(m.L2Px(bod.x),m.L2Py(bod.y));
						sipka(canv,(m.L2Px(E->geo.X4)+m.L2Px(bod.x))/2.0,(m.L2Py(E->geo.Y4)+m.L2Py(bod.y))/2.0,m.azimut(m.L2Px(bod.x),m.L2Py(bod.y),m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4))*(-1),true,sipka_velikost,clRed);//zajistí vykreslení šipky - orientace spojovací linie
					}
				}
				else if(pom==NULL && v.OBJEKTY->predchozi->n>2)//vykreslení pouze v případě pokdud existjují více jak 2
				{
					if(m.round2double(E->geo.X4,2)!=m.round2double(v.ELEMENTY->dalsi->geo.X1,2) || m.round2double(E->geo.Y4,2)!=m.round2double(v.ELEMENTY->dalsi->geo.Y1,2))
					{
						canv->MoveTo(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
						canv->LineTo(m.L2Px(v.ELEMENTY->dalsi->geo.X1),m.L2Py(v.ELEMENTY->dalsi->geo.Y1));
						sipka(canv,(m.L2Px(E->geo.X4)+m.L2Px(v.ELEMENTY->dalsi->geo.X1))/2.0,(m.L2Py(E->geo.Y4)+m.L2Py(v.ELEMENTY->dalsi->geo.Y1))/2.0,m.azimut(m.L2Px(v.ELEMENTY->dalsi->geo.X1),m.L2Py(v.ELEMENTY->dalsi->geo.Y1),m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4))*(-1),true,sipka_velikost,clRed);//zajistí vykreslení šipky - orientace spojovací linie
					}
				}
				pom=E->dalsi2;
				if(E->eID==301)pom=NULL;//spojka si v tomto ukazateli uchovává svoji párovou výhybku
				if(pom!=NULL)
				{
					if(pom->eID==301 && pom->predchozi2==E){bod.x=pom->X;bod.y=pom->Y;}
					else {bod.x=pom->geo.X1;bod.y=pom->geo.Y1;}
					if(m.round2double(E->geo.X4,2)!=m.round2double(bod.x,2) || m.round2double(E->geo.Y4,2)!=m.round2double(bod.y,2))
					{
						canv->MoveTo(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
						canv->LineTo(m.L2Px(bod.x),m.L2Py(bod.y));
						sipka(canv,(m.L2Px(E->geo.X4)+m.L2Px(bod.x))/2.0,(m.L2Py(E->geo.Y4)+m.L2Py(bod.y))/2.0,m.azimut(m.L2Px(bod.x),m.L2Py(bod.y),m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4))*(-1),true,sipka_velikost,clRed);//zajistí vykreslení šipky - orientace spojovací linie
					}
				}
			}

			////posun na další element
			E=v.sekvencni_zapis_cteni(E,tab_pruchodu,NULL);//nutné použít tento průchodový algoritmus, v tomto průchodu je použit algoritmus dalsi_krok, nelze užit dalsi_krok pro průchod ve kterém bude vnořený znova algoritmus dalsi_krok, tento alg. používá glob. seznam výhybek, přes které prošel, pokud by běžely dva současně ukládaly by do jednoho seznamu, to by vedlo k chybným výsledkům obou průchodu, viz. https://docs.google.com/document/d/1ApxDG9tpTS6qEpKk2COsvrLZrzDl1fynWlD7xmoE6qM/edit?ts=5e3d669e#heading=h.a4ve3tnox7u5
		}
		delete E;E=NULL;
		pom=NULL;delete pom;
		delete []tab_pruchodu;
	}
}
//---------------------------------------------------------------------------
//zajišťuje vykreslení pouze obrysu dle typu objektu
void Cvykresli::vykresli_kabinu(TCanvas *canv,Cvektory::TObjekt *O,int stav,bool zobrazit_koty)
{
	////vstupní proměnné
	bool highlight_pow=false;  //tato podmínka tu původně nebývala před změnou červena - šedomodrá
	TColor clAkt/*clAktulální*/=clStenaKabiny;if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=clPasiv;//v případě editace jsou pasivní objekty šedé
	short I=100-F->scGPTrackBar_intenzita->Value;
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
	if(F->OBJEKT_akt!=NULL){if(F->OBJEKT_akt->n==O->n){stav=-2;if(F->OBJEKT_akt->zobrazit_koty)zobrazit_koty=true;else zobrazit_koty=false;}else {stav=-1;zobrazit_koty=false;}}
	else {stav=-2;zobrazit_koty=false;}
	//highlight polygonu (editace rozmerů, bodů)
	if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && F->pom_bod!=NULL)
	switch(F->JID)
	{
		//highlight hrany
		case -2:if(F->pom_bod->n==1)stav=O->body->predchozi->n*2;else stav=O->body->predchozi->n+F->pom_bod->n-1;break;
		//highlight bodu
		case -3:stav=F->pom_bod->n;break;
	}
	//highlight hrany objektu mimo editaci
	if(F->OBJEKT_akt==NULL && F->pom!=NULL && F->pom->n==O->n && F->JID==4){if(F->pom_bod->n==1)stav=O->body->predchozi->n*2;else stav=O->body->predchozi->n+F->pom_bod->n-1;}
	//highilightování kabiny při přejetí kurzorem přes název objektu, řešeno zde metoda polygon() neumí zvýraznit všechny hrany
	if(F->OBJEKT_akt==NULL && F->pom!=NULL && F->pom->n==O->n && F->JID==-6)clAkt=m.clIntensive(clStenaKabiny,-50);
	////vnější obrys kabiny
	if(!(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n && F->scGPTrackBar_intenzita->Value<5 || (F->MOD==F->SIMULACE || F->scButton_zamek_layoutu->ImageIndex==67) && sirka_steny_px==0))polygon(canv,O->body,clAkt,sirka_steny_px,stav,zobrazit_koty);//zobrazí se pouze za daných podmínek

	short highlight=0;//nastavování zda mají být koty highlightovány

	////vykreslení komor u POW - pokud je objekt obsahuje, poslední komora má vždy velikost do konce objektu (nehledě na její skutečné délku), stav, kdy začíná komora za objektem, je nutné ošetřit separátně
	if(O->komora!=NULL && O->komora->predchozi->n>0)
	{
		Cvektory::TKomora *K=O->komora->dalsi;//přeskočí hlavičku
		double vzdalenost=0;
		while(K->dalsi!=NULL)
		{
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0 && F->pom_komora!=NULL && F->pom_komora->n==K->n)){clAkt=m.clIntensive(clStenaKabiny,-50);highlight_pow=true;}//highlight
			else clAkt/*clAktualni*/=clStenaKabiny;if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=clPasiv;//v případě editace jsou pasivní objekty šedé, tato podmínka nebyla před změnou červená -> šedomodrá
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=m.clIntensive(clAkt,I);//tady před změnou červená -> šedomodrá bývala clStenaKabiny
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
				if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==K->n))
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
				if(K->typ==1)vykresli_pow_sprchu(canv,X,X,Y1,Y2,m.m2px(K->velikost),clAkt,sirka_steny_px/4.0,pmpp,0,orientace);
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
				if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)K->n || F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==K->n))//highlight komory
				{
					highlight_pow=true;
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
				if(K->typ==1)vykresli_pow_sprchu(canv,X1,X2,Y,Y,m.m2px(K->velikost),clAkt,sirka_steny_px/4.0,pmpp,0,orientace);
			}
			////překreslení obrysu, nutné u POW, jinak by komory překryli highlight stěny obrysu
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->id==3 && !highlight_pow && !(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n && F->scGPTrackBar_intenzita->Value<5))polygon(canv,O->body,clAkt,sirka_steny_px,stav,zobrazit_koty);//nové vykreslování příprava
			//KÓTY
			if(zobrazit_koty && F->OBJEKT_akt->n==O->n)
			{
				//nastavení highlight
				if((F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==K->n) || (F->JID*(-1)-10==(signed)K->n || F->JID*(-1)-10==(signed)K->predchozi->n)&&F->d.v.PtInKota_komory(O,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)==-1)highlight=2;
				else if(F->JID*(-1)-10==(signed)K->n || F->JID==13)highlight=1;else highlight=0;
				//vykreslení kót komor
				switch((int)orientace)
				{
					case 0:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost-K->velikost,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost,NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,K);break;
					case 90:vykresli_kotu(canv,m.P2Lx(X1)+vzdalenost-K->velikost,m.P2Ly(Y2),m.P2Lx(X1)+vzdalenost,m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,K);break;
					case 180:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost+K->velikost,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost,NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,K);break;
					case 270:vykresli_kotu(canv,m.P2Lx(X1)-vzdalenost+K->velikost,m.P2Ly(Y2),m.P2Lx(X1)-vzdalenost,m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,K);break;
				}
			}
			K=K->dalsi;//posun ve spojáku na další prvek
		}
		K=NULL;delete K;
		////poslední komora
		if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)F->OBJEKT_akt->komora->predchozi->n || (F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==F->OBJEKT_akt->komora->predchozi->n)))clAkt=m.clIntensive(clStenaKabiny,-50);//highlight
		//else clAkt=clStenaKabiny;
		//if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=m.clIntensive(clStenaKabiny,50);
		else clAkt/*clAktualni*/=clStenaKabiny;if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=clPasiv;//v případě editace jsou pasivní objekty šedé, tato podmínka nebyla před změnou červená -> šedomodrá
		if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clAkt=m.clIntensive(clAkt,I);//tady před změnou červená -> šedomodrá bývala clStenaKabiny
		set_pen(canv,clAkt,sirka_steny_px,PS_ENDCAP_SQUARE);
		if(orientace==90 || orientace==270)
		{
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)F->OBJEKT_akt->komora->predchozi->n || (F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==F->OBJEKT_akt->komora->predchozi->n)))//highlight komory
			{
				//nastavení proměnných podle orientace
				double hl_X=0;
				if(orientace==90)hl_X=X2-m.m2px(F->OBJEKT_akt->komora->predchozi->velikost);else hl_X=X2+m.m2px(F->OBJEKT_akt->komora->predchozi->velikost);
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
			if(O->komora->predchozi->typ==1)vykresli_pow_sprchu(canv,m.L2Px(O->element->geo.X4),m.L2Px(F->d.v.vrat_posledni_element_objektu(O)->geo.X4),m.L2Py(O->body->dalsi->Y),m.L2Py(O->body->predchozi->Y),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4.0,pmpp,0,orientace);
		}
		else
		{
			bool highlight=false;
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n && (F->JID*(-1)-10==(signed)F->OBJEKT_akt->komora->predchozi->n || (F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==F->OBJEKT_akt->komora->predchozi->n)))//highlight komory
			{
				//nastavení proměnných podle orientace
				highlight=true;
				double hl_Y=0;
				if(orientace==180){hl_Y=Y2-m.m2px(F->OBJEKT_akt->komora->predchozi->velikost);pmpp*=-1;}else hl_Y=Y2+m.m2px(F->OBJEKT_akt->komora->predchozi->velikost);
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
			Cvektory::TElement *e_posledni=F->d.v.vrat_posledni_element_objektu(O);
			if(O->komora->predchozi->typ==1 && (O->orientace==0 || O->orientace==180 && !highlight))vykresli_pow_sprchu(canv,m.L2Px(O->body->dalsi->X),m.L2Px(O->body->predchozi->X),m.L2Py(e_posledni->geo.Y4),m.L2Py(e_posledni->geo.Y4),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4.0,pmpp,0,orientace);
			if(O->komora->predchozi->typ==1 && O->orientace==180 && highlight)vykresli_pow_sprchu(canv,m.L2Px(O->body->predchozi->X),m.L2Px(O->body->dalsi->X),m.L2Py(e_posledni->geo.Y4),m.L2Py(e_posledni->geo.Y4),m.m2px(O->komora->predchozi->velikost),clAkt,sirka_steny_px/4.0,pmpp,0,orientace);//z důvodu špatného vykreslení při highlightu
    	e_posledni=NULL;delete e_posledni;
		}
		//vykreslení KÓTY od poslení komory k okraji kabiny
		if(zobrazit_koty && F->OBJEKT_akt->n==O->n)
		{
			if((F->JID==0  && F->pom_komora!=NULL && F->pom_komora->n==O->komora->predchozi->n) || (F->JID*(-1)-10==(signed)O->komora->predchozi->n || F->JID*(-1)-10==(signed)O->komora->predchozi->predchozi->n)&&F->d.v.PtInKota_komory(O,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)==-1)highlight=2;
			else if(F->JID*(-1)-10==(signed)O->komora->predchozi->n || F->JID==13)highlight=1;
			else highlight=0;
			//vykreslení kót komor
			switch((int)orientace)
			{
				case 0:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)+vzdalenost,m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,O->komora->predchozi);break;
				case 90:vykresli_kotu(canv,m.P2Lx(X1)+vzdalenost,m.P2Ly(Y2),m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,O->komora->predchozi);break;
				case 180:vykresli_kotu(canv,m.P2Lx(X2),m.P2Ly(Y1)-vzdalenost,m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,O->komora->predchozi);break;
				case 270:vykresli_kotu(canv,m.P2Lx(X1)-vzdalenost,m.P2Ly(Y2),m.P2Lx(X2),m.P2Ly(Y2),NULL,O->koty_elementu_offset.x,highlight,0.2,clGray,false,O->komora->predchozi);break;
			}
			canv->Brush->Style=bsClear;//navrácení na průhledné pero, kvůli následujícím popiskům objektu, kóty jej totiž změnily
		}
	}

	////název - na konec, vykreslení jako poslední vrstva
	nastavit_text_popisu_objektu_v_nahledu(canv);AnsiString Tn=O->name.UpperCase();AnsiString Tl=+" / ";short Wn=canv->TextWidth(Tn);
	//poloha nadpisu
	double X=O->Xt;
	double Y=O->Yt;
	switch((int)O->orientace_text)
	{
		case 0:X=m.L2Px(X)-canv->TextHeight(Tl);Y=m.L2Py(Y)+m.round((Wn)/2.0);canv->Font->Orientation=(O->orientace_text+90)*10;break;//nastavení rotace canvasu
		case 90:X=m.L2Px(X)-m.round((Wn)/2.0);Y=m.L2Py(Y)-canv->TextHeight(Tl);break;
		case 180:X=m.L2Px(X)+canv->TextHeight(Tl);Y=m.L2Py(Y)-m.round((Wn)/2.0);canv->Font->Orientation=(O->orientace_text+90)*10;break;//nastavení rotace canvasu
		case 270:X=m.L2Px(X)-m.round((Wn)/2.0);Y=m.L2Py(Y)-canv->TextHeight(Tl);break;
	}
	//nastavení normálního, disabled nebo highlight textu
	nastavit_text_popisu_objektu_v_nahledu(canv);
  //highlight názvu
	if((F->JID==-6 || F->editace_textu && F->index_kurzoru==-6) && (F->pom!=NULL && F->pom->n==O->n || F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n))canv->Font->Color=clStenaHaly; else canv->Font->Color=clStenaKabiny;
  if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)canv->Font->Color=m.clIntensive(clAkt,I);//pro neaktivní objekty při editaci
	//samotné vypsání názvu
	if(!(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n && F->scGPTrackBar_intenzita->Value<5))TextFraming(canv,X,Y,Tn);//záměrně Tl,aby se ztučněním nepřepozivávalo - působilo to moc dynamacky
	//vrácení původní hodnoty rotace canvasu
	canv->Font->Orientation=0;
}
//---------------------------------------------------------------------------
//bool name, zda se jedná o zadavání name nebo short_name
void Cvykresli::nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv)
{
	canv->Font->Name=F->aFont->Name;
	//if(F->JID==-6 || F->editace_textu && F->index_kurzoru==-6)canv->Font->Color=clStenaHaly; else canv->Font->Color=clStenaKabiny;
	canv->Font->Size=2*3*F->Zoom;
	canv->Font->Style = TFontStyles()<< fsBold;
}
//---------------------------------------------------------------------------
//symbolika tekoucí kapaliny u POW
void Cvykresli::vykresli_pow_sprchu(TCanvas *canv,long X1,long X2,long Y1,long Y2,unsigned int velikost_komory_px,TColor color,double sirka,short pmpp,short typ,double orientace)
{
	if(typ!=-2)set_pen(canv,color,sirka,PS_ENDCAP_SQUARE);//pokud se jedná o typ kurzor, tak se nezobrazuje
	double krok=sirka*8;//pouze zneužití sirka
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
//zajistí vykreslení šipky //zajistí vykreslení šipky, typ 0 - barevná výplň, 1 - transparentní výplň (bez výplně), 2 - transparentní výplň (bez výplně) a bez znázornění hrany přepony trojúhleníku tvořící šipku, pozor velikost šipky již nenásobit zoomem
void Cvykresli::sipka(TCanvas *canv, int X, int Y, float azimut, short typ, float size,COLORREF color,COLORREF color_brush,TPenMode PenMode,TPenStyle PenStyle,bool teziste_stred)
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

	if(typ==0)//barevná výplň trojúhelníku
	{
		POINT body[3]={{m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size)},{m.round(X+m.rotace(1,0,azimut).x*size),m.round(Y+m.rotace(1,0,azimut).y*size)},{m.round(X+m.rotace(1,360-sklon,azimut).x*size),m.round(Y+m.rotace(1,360-sklon,azimut).y*size)}};
		canv->Polygon((TPoint*)body,2);
	}
	else//transparentní střed
	{
		canv->MoveTo(m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size));
		canv->LineTo(m.round(X+m.rotace(1,0,azimut).x*size),m.round(Y+m.rotace(1,0,azimut).y*size));
		canv->LineTo(m.round(X+m.rotace(1,360-sklon,azimut).x*size),m.round(Y+m.rotace(1,360-sklon,azimut).y*size));
		if(typ==1)canv->LineTo(m.round(X+m.rotace(1,sklon,azimut).x*size),m.round(Y+m.rotace(1,sklon,azimut).y*size));//pouze pokud je požadováno vykreslení bez hrany
	}
}
//---------------------------------------------------------------------------
void Cvykresli::vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *ukaz)
{
	//nová koncepce - metodu vykresli_kruh - patřičně přejmenovat
	if((long)ukaz->id!=F->VyID&&(long)ukaz->id!=pocet_objektu_knihovny+1)//vykreslování objektů, pro výhybky a spojky zvlášť vykreslení
	{
		vykresli_kabinu(canv,ukaz,-2,false);
		//+doplnit volání vykreslení elementární osy (je na to metoda), metodu volat buď přímo zde nebo jako součásts vykresli_kabinu = porada
	}
	else vykresli_kruh(canv,ukaz);//vykreslování výhybky a spojky zvlášť
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
//zajistí vykreslení daného textu dle nastaveného Fontu (pokud je NULL, převezme se akutální font canvasu), možno vertikálně i horizontálně pozicovat, umí zalamovat řádky v případě výskytu \n, velikost odřádkování dle velikosti písma 1=100%, v případě záporné hodnoty řazení textu od prvního řádku dalším směrem nahoru, umí také framing, který je baravně a velikostně nastavitelný, neumí zatím rotaci textu!!!
HRGN Cvykresli::TextOut(TCanvas *canv,int X,int Y,UnicodeString Text,Talign align,Tvalign valign,float FaktorLineSpacing,TColor clFraming,unsigned short FramingSize,TFont *Font)
{
	//pokud není font nastaven, převezme se akutální font, převezme se akutální font canvasu
	if(Font==NULL)Font=canv->Font;else canv->Font=Font;

	//výchozí proměnné
	unsigned int count=F->ms.count(Text,"\n")+1;//počet řádků
	TTextWNumber *Texty=new TTextWNumber[count];
	unsigned short TW=0; unsigned short TH=canv->TextHeight(Text);
	HRGN RET=NULL;

	//rozseparování textu + zjištění největší šířky a délky
	for(unsigned int i=0;i<count;i++)
	{
		//rozseparování textu
		int CR=Text.Pos("\n");//pozice odřádkování
		Texty[i].text=Text.SubString(1,CR-1);if(Texty[i].text=="")Texty[i].text=Text;//uloží část před odřádkováním nebo poslední část
		Text=Text.SubString(CR+1,Text.Length()-CR+2);//zbytek textu pro další dělení

		//zjištění největší délky + uložení délky jednotlivých řádků k dalšímu použití
		Texty[i].number1=canv->TextWidth(Texty[i].text);
		TW = (TW > Texty[i].number1) ? TW : Texty[i].number1;
	}

	//vypisování jednotlivých řádků
	for(unsigned int i=0;i<count;i++)
	{
		//zarovnání horizontální a veritální
		int oX=0;
		switch(align)
		{
			case LEFT:	 oX=0;break;
			case CENTER: oX=m.round(-Texty[i].number1/2.0);break;
			case RIGHT:  oX=-Texty[i].number1;break;
		}
		double oY=0;
		switch(valign)
		{
			case TOP:	   oY=0;if(FaktorLineSpacing<0)oY-=TH*(count-1)*FaktorLineSpacing;break;
			case MIDDLE: oY=(0-(TH*(count-1)*FaktorLineSpacing+TH))/2.0;break;
			case BOTTOM: oY-=TH*(count-1)*FaktorLineSpacing+TH;if(FaktorLineSpacing<0)oY=-TH;break;
		}
		//výsledné souřadnice
		int Xl=X+oX;
		int Yl=Y+m.round(TH*i*FaktorLineSpacing+oY);
		//samotný výpis
		TextFraming(canv,Xl,Yl,Texty[i].text,Font,clFraming,FramingSize);
		//uložení do celkového regionu
		if(i>0)CombineRgn(RET,RET,CreateRectRgn(Xl,Yl,Xl+Texty[i].number1,Yl+TH),RGN_OR);
		else RET=CreateRectRgn(Xl,Yl,Xl+Texty[i].number1,Yl+TH);//nefungovalo, pokud region byl NULL
	}

	//smazání již nepotřebného textu
	delete[]Texty;

	//vrácení oblasti textu pro případné další použití
	return RET;
}
////---------------------------------------------------------------------------
//zajistí vykreslení daného textu dle nastaveného Fontu (pokud je NULL, převezme se akutální font canvasu) včetně framingu, který je baravně a velikostně nastavitelný
void Cvykresli::TextFraming(TCanvas *canv,int X,int Y,UnicodeString Text,TFont *Font,TColor clFraming,unsigned short FramingSize)
{
	//pokud není font nastaven, převezme se akutální font, převezme se akutální font canvasu
	if(Font==NULL)Font=canv->Font;else canv->Font=Font;

	//záloha
	TColor clText=Font->Color;

	//nastavení průhledného pozadí písma - důležité
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
    if(F->OBJEKT_akt!=NULL)size_grid/=10;//zjemnění gridu v případě náhledu, grid po 0,1 m místo po 1m
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
//------------------------------------------------------------------------------------------------------------------------------------------------------
//metoda na vykreslení měřítka á la magnetické laso nebo vzádlenost teploměrů na pohonu
void Cvykresli::vykresli_meridlo(TCanvas *canv)
{
  ////vykreslní citelných oblastí
	bool prichyceno=vykresli_cit_oblasti_lasa(canv);

	////přesměrování podle typu vykreslení
	if(F->pom_element!=NULL && ((v.MAG_LASO->dalsi==NULL && m.azimut(v.MAG_LASO->Element->geo.X1,v.MAG_LASO->Element->geo.Y1,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y)!=F->pom_element->geo.orientace-F->pom_element->geo.rotacni_uhel) || (v.MAG_LASO->dalsi!=NULL && v.MAG_LASO->sparovany!=NULL && ((v.MAG_LASO->predchozi->n==1 && ((F->pom_element->dalsi==v.MAG_LASO->sparovany || (F->pom_element->dalsi!=NULL && F->pom_element->dalsi->dalsi==v.MAG_LASO->sparovany)) || F->pom_element->dalsi2==v.MAG_LASO->sparovany)) || (v.MAG_LASO->predchozi->n>1 && (v.MAG_LASO->dalsi->dalsi->Element->dalsi==v.MAG_LASO->sparovany || (v.MAG_LASO->dalsi->dalsi->Element->dalsi!=NULL && v.MAG_LASO->dalsi->dalsi->Element->dalsi->dalsi==v.MAG_LASO->sparovany) || v.MAG_LASO->dalsi->dalsi->Element->dalsi2==v.MAG_LASO->sparovany))))))
		vykresli_meridlo_proti_trendu(canv,prichyceno);
	else
		vykresli_meridlo_po_trendu(canv,prichyceno);
}
////---------------------------------------------------------------------------
//vykreslí citelné oblasti elementů na které je možné se přichytit
bool Cvykresli::vykresli_cit_oblasti_lasa(TCanvas *canv)
{
  //bod vykreslení
	TPointD bod,P;
	bod.x=MaxInt;bod.y=MaxInt;
	bool ret=false;

	//vykreslení před měřením
	if(F->prichytavat_k_mrizce==1 && v.MAG_LASO->Element==NULL && v.MAG_LASO->sparovany==NULL && F->pom_element==NULL)
	{
		//hledání v elementech pro přichycení
		Cvektory::TElement *E=v.ELEMENTY->dalsi;
		while(E!=NULL)
		{
      //nulování stavu
			E->stav=1;
			if(E->eID==0 && E->data.pocet_voziku==0)E->stav=0;
			//kontrola vrátek
			P=v.InVrata(E);
			if(P.x!=-1*MaxInt && P.y!=-1*MaxInt)
			{
				bod=P;
				break;
			}
      //kontrola fce. elementů
			if(E->eID!=MaxInt && m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,F->velikost_citelne_oblasti_elementu))
			{
				bod.x=E->geo.X4;
				bod.y=E->geo.Y4;
				E->stav=2;
        break;
			}
      //kontrola začátku a konce stoupání
			if(E->geo.HeightDepp!=0)
			{
				if(m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,E->geo.X1,E->geo.Y1,F->velikost_citelne_oblasti_elementu))
				{
					bod.x=E->geo.X1;
					bod.y=E->geo.Y1;
					break;
        }
				if(m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,F->velikost_citelne_oblasti_elementu))
				{
					bod.x=E->geo.X4;
					bod.y=E->geo.Y4;
					break;
				}
			}
			E=v.dalsi_krok(E);
		}
		v.vymaz_seznam_VYHYBKY();
		E=NULL;delete E;

		//kontrola bodů vozíků
		if(bod.x==MaxInt && bod.y==MaxInt)
		{
			Cvektory::TVozik *V=v.VOZIKY->dalsi;
			while(V!=NULL)
			{
				if(m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,V->X,V->Y,F->velikost_citelne_oblasti_elementu))
				{
					bod.x=V->X;
					bod.y=V->Y;
					break;
        }
				V=V->dalsi;
			}
      V=NULL;delete V;
		}

    //nastavení geometrického pera
		short width=m.round(m.m2px(F->velikost_citelne_oblasti_elementu));
		set_pen(canv,clMeridlo,width,PS_ENDCAP_FLAT);
		canv->Pen->Mode=pmNotXor;

		//vykreslení
		if(bod.x!=MaxInt && bod.y!=MaxInt)
		{
			canv->Ellipse(m.L2Px(bod.x)-width,m.L2Py(bod.y)-width,m.L2Px(bod.x)+width,m.L2Py(bod.y)+width);
			ret=true;
		}
	}

	//vykreslení během měření
	if(F->prichytavat_k_mrizce==1 && F->pom_element!=NULL)
	{
		//nastavení geometrického pera
		short width=m.round(m.m2px(F->velikost_citelne_oblasti_elementu));
		set_pen(canv,clMeridlo,width,PS_ENDCAP_FLAT);
		canv->Pen->Mode=pmNotXor;
		//vykreslení
		if(F->pom_element->stav==2 && (F->pom_element!=v.MAG_LASO->sparovany || (v.MAG_LASO->Element->geo.X4!=v.MAG_LASO->sparovany->geo.X4 || v.MAG_LASO->Element->geo.Y4!=v.MAG_LASO->sparovany->geo.Y4)))
		{
			canv->Ellipse(m.L2Px(F->pom_element->geo.X4)-width,m.L2Py(F->pom_element->geo.Y4)-width,m.L2Px(F->pom_element->geo.X4)+width,m.L2Py(F->pom_element->geo.Y4)+width);
			F->akt_souradnice_kurzoru.x=F->pom_element->geo.X4;F->akt_souradnice_kurzoru.y=F->pom_element->geo.Y4;
			ret=true;
		}
		else if(F->pom_element->predchozi->stav==2 && (F->pom_element->predchozi!=v.MAG_LASO->sparovany || (v.MAG_LASO->Element->geo.X4!=v.MAG_LASO->sparovany->geo.X4 || v.MAG_LASO->Element->geo.Y4!=v.MAG_LASO->sparovany->geo.Y4)))
		{
			canv->Ellipse(m.L2Px(F->pom_element->predchozi->geo.X4)-width,m.L2Py(F->pom_element->predchozi->geo.Y4)-width,m.L2Px(F->pom_element->predchozi->geo.X4)+width,m.L2Py(F->pom_element->predchozi->geo.Y4)+width);
			F->akt_souradnice_kurzoru.x=F->pom_element->predchozi->geo.X4;F->akt_souradnice_kurzoru.y=F->pom_element->predchozi->geo.Y4;
			ret=true;
		}
		//kontrola zda jsem na vrátkách objektu
		else
		{
			bool pokracovat=true;
			//kontrola bodů vozíků
			Cvektory::TVozik *V=v.VOZIKY->dalsi;
			while(V!=NULL)
			{
	  		if(m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,V->X,V->Y,F->velikost_citelne_oblasti_elementu))
				{
					canv->Ellipse(m.L2Px(V->X)-width,m.L2Py(V->Y)-width,m.L2Px(V->X)+width,m.L2Py(V->Y)+width);
					pokracovat=false;
					F->akt_souradnice_kurzoru.x=V->X;F->akt_souradnice_kurzoru.y=V->Y;
          ret=true;
					break;
				}
				V=V->dalsi;
			}
			V=NULL;delete V;

			//kontrola vrátek
			if(pokracovat)
			{
		  	P=v.InVrata(F->pom_element);
				if(P.x!=-1*MaxInt && P.y!=-1*MaxInt)
				{
		  		//vykreslení
					canv->Ellipse(m.L2Px(P.x)-width,m.L2Py(P.y)-width,m.L2Px(P.x)+width,m.L2Py(P.y)+width);
					F->akt_souradnice_kurzoru=P;
					ret=true;
		  	}
		  	//kontrola začátku a konce stoupání / klesání
		  	else if(F->pom_element->geo.HeightDepp!=0)
		  	{
		  		if(m.delka(F->pom_element->geo.X1,F->pom_element->geo.Y1,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y)<=F->velikost_citelne_oblasti_elementu)
					{
						canv->Ellipse(m.L2Px(F->pom_element->geo.X1)-width,m.L2Py(F->pom_element->geo.Y1)-width,m.L2Px(F->pom_element->geo.X1)+width,m.L2Py(F->pom_element->geo.Y1)+width);
						F->akt_souradnice_kurzoru.x=F->pom_element->geo.X1;F->akt_souradnice_kurzoru.y=F->pom_element->geo.Y1;
						ret=true;
					}
					if(m.delka(F->pom_element->geo.X4,F->pom_element->geo.Y4,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y)<=F->velikost_citelne_oblasti_elementu)
					{
						canv->Ellipse(m.L2Px(F->pom_element->geo.X4)-width,m.L2Py(F->pom_element->geo.Y4)-width,m.L2Px(F->pom_element->geo.X4)+width,m.L2Py(F->pom_element->geo.Y4)+width);
						F->akt_souradnice_kurzoru.x=F->pom_element->geo.X4;F->akt_souradnice_kurzoru.y=F->pom_element->geo.Y4;
						ret=true;
					}
				}
			}
		}
	}

  return ret;
}
////---------------------------------------------------------------------------
//vykreslí měření po trendu linky
void Cvykresli::vykresli_meridlo_po_trendu(TCanvas *canv,bool prichyceno)
{
	////deklarace
	double R,RA,OR,X,Y,uhel=0,delka=0,azimut,cas=0,d;

	///vykreslení uložených segmentů v mag lasu, pokud je třeba
	if(F->pom_element!=NULL && (F->pom_element->predchozi==v.MAG_LASO->predchozi->Element || (F->pom_element->predchozi->eID==301 && F->pom_element->predchozi->predchozi2==v.MAG_LASO->predchozi->Element) || F->pom_element==v.MAG_LASO->predchozi->Element || (F->pom_element->eID==301 && F->pom_element->predchozi2==v.MAG_LASO->predchozi->Element) || (F->pom_element->predchozi->eID==301 && F->pom_element->predchozi->predchozi==v.MAG_LASO->predchozi->Element)))
	{
		Cvektory::TCesta *C=v.MAG_LASO->dalsi;
  	while(C!=NULL)
		{
			//nastavení parametrů pro vykreslení + výpočet délky
			X=C->Element->geo.X1;Y=C->Element->geo.Y1;
			RA=C->Element->geo.rotacni_uhel;
			OR=C->Element->geo.orientace;
			uhel=0;
			if(C->n==1)
			{
				X=v.MAG_LASO->Element->geo.X1;Y=v.MAG_LASO->Element->geo.Y1;
				RA=v.MAG_LASO->sparovany->geo.rotacni_uhel;
				OR=v.MAG_LASO->sparovany->geo.orientace;
			}
			//ypočet delky
			if(C->Element->n!=MaxInt && C->n!=1)d=C->Element->geo.delka;
			else if(C->Element->geo.typ==0)
			{
				d=m.delka(X,Y,C->Element->geo.X4,C->Element->geo.Y4);
				d=m.castPrepony(d,C->Element->geo.delka,C->Element->geo.delkaPud,C->Element->geo.HeightDepp);
			}
			else
			{
				d=m.delka(X,Y,C->Element->geo.X4,C->Element->geo.Y4);
				uhel=m.T2Aarc(C->Element->geo.radius,d);
				d=m.R2Larc(C->Element->geo.radius,uhel);
			}
			delka+=d;

  		if(C->Element->geo.typ==0)
			{
  			R=d;
				uhel=0;
			}
  		else
  		{
				R=C->Element->geo.radius;
				if(C->n==1)R=v.MAG_LASO->sparovany->geo.radius;
				if(uhel==0)uhel=RA;//max z rotačního úhlu
			}
			//výpočet času
			if((C->Element->n!=MaxInt && C->Element->pohon==NULL) || (C->Element->n==MaxInt && C->sparovany!=NULL && C->sparovany->pohon==NULL));
			else
			{
				if(C->Element->n==MaxInt && C->sparovany!=NULL)
				{
					cas+=d/C->sparovany->pohon->aRD;
					if(!(C->predchozi->sparovany!=NULL && C->predchozi->sparovany==C->Element && C->predchozi->sparovany->geo.X4==C->predchozi->Element->geo.X2 && C->predchozi->Element->geo.X2==C->predchozi->Element->geo.X3))
					{
						if(v.vrat_druh_elementu(C->sparovany)==0)cas+=C->sparovany->data.PT1+C->sparovany->data.PT2+C->sparovany->WT+C->sparovany->PTotoc;
				  	if(C->sparovany->eID==0 && C->dalsi==NULL && C->Element->geo.X2==C->Element->geo.X3 && C->Element->geo.X3==C->Element->geo.X4)
				  	{
				  		cas+=C->sparovany->data.WTstop;
				  		cas-=(C->sparovany->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)/C->sparovany->pohon->aRD;
				  	}
						if(!prichyceno && C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany==NULL && C->dalsi->Element->pohon!=NULL && C->sparovany->pohon!=C->dalsi->Element->pohon)cas+=m.cekani_na_palec(0,C->dalsi->Element->pohon->roztec,C->dalsi->Element->pohon->aRD,3);
						if(!prichyceno && C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany!=NULL && C->dalsi->sparovany->pohon!=NULL && C->sparovany->pohon!=C->dalsi->sparovany->pohon)cas+=m.cekani_na_palec(0,C->dalsi->sparovany->pohon->roztec,C->dalsi->sparovany->pohon->aRD,3);
						if(!prichyceno && C->dalsi==NULL && C->sparovany->dalsi!=NULL && C->sparovany->pohon!=C->sparovany->dalsi->pohon)m.cekani_na_palec(0,C->sparovany->dalsi->pohon->roztec,C->sparovany->dalsi->pohon->aRD,3);
					}
				}
				else
				{
					cas+=d/C->Element->pohon->aRD;
					if(!(C->predchozi->sparovany!=NULL && C->predchozi->sparovany==C->Element && C->predchozi->sparovany->geo.X4==C->predchozi->Element->geo.X2 && C->predchozi->Element->geo.X2==C->predchozi->Element->geo.X3))
					{
						if(v.vrat_druh_elementu(C->Element)==0)cas+=C->Element->data.PT1+C->Element->data.PT2+C->Element->WT+C->Element->PTotoc+C->Element->data.WTstop;;
				  	if(C->Element->eID==0)
						{
				  		if(d>=C->Element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)
				  			cas-=(C->Element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)/C->Element->pohon->aRD;
						}
						if(!prichyceno && C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany==NULL && C->dalsi->Element->pohon!=NULL && C->Element->pohon!=C->dalsi->Element->pohon)cas+=m.cekani_na_palec(0,C->dalsi->Element->pohon->roztec,C->dalsi->Element->pohon->aRD,3);
						if(!prichyceno && C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany!=NULL && C->dalsi->sparovany->pohon!=NULL && C->Element->pohon!=C->dalsi->sparovany->pohon)cas+=m.cekani_na_palec(0,C->dalsi->sparovany->pohon->roztec,C->dalsi->sparovany->pohon->aRD,3);
						if(!prichyceno && C->dalsi==NULL && C->Element->dalsi!=NULL && C->Element->pohon!=C->Element->dalsi->pohon)cas+=m.cekani_na_palec(0,C->Element->dalsi->pohon->roztec,C->Element->dalsi->pohon->aRD,3);
					}
				}
			}
			//nastavení typu vykreslení, zarážka na konci || zarážka na začátku || bez zarážky
			short typ=0;
			if(C->n==1)typ=1;
      //pokud je první segment část oblouku, vykresluje se od konce, změna parametrů
			if(C->n==1 && C->Element->geo.typ!=0)
			{
				X=C->Element->geo.X4;Y=C->Element->geo.Y4;
				OR=m.Rt90(C->Element->geo.orientace-C->Element->geo.rotacni_uhel-180);//obrácení orientace
				if(C->Element->geo.rotacni_uhel>0)uhel=-uhel;//změna směrování RA/úhlu
				typ=2;
			}
			//vykreslení segmentu
			vykresli_Gelement(canv,X,Y,OR,uhel,R,clMeridlo,2,"","",typ);
			//posun na další segment
			C=C->dalsi;
		}
		delete C;C=NULL;
	}

	////neliniové měření, pouze v případě, že jsem za posledním segmentem cesty a na pohonu
	if(F->pom_element!=NULL && (F->pom_element->dalsi==v.MAG_LASO->predchozi->Element ||F->pom_element->predchozi==v.MAG_LASO->predchozi->Element || (F->pom_element->predchozi->eID==301 && F->pom_element->predchozi->predchozi2==v.MAG_LASO->predchozi->Element) || (F->pom_element->predchozi->eID==301 && F->pom_element->predchozi->predchozi==v.MAG_LASO->predchozi->Element)))
	{
		//vykreslení části oblouku
		if(F->pom_element!=NULL && F->pom_element->geo.typ!=0 && v.MAG_LASO->dalsi!=NULL)
		{
			//načítání parametrů
			R=F->pom_element->geo.radius;
			RA=F->pom_element->geo.rotacni_uhel;//rotační úhel, pod kterým je oblouk rotován - směřován (proti směru hodinových ručiček), může být záporný (po směru hodinových ručiček)
			OR=F->pom_element->geo.orientace;
   		X=F->pom_element->geo.X1,Y=F->pom_element->geo.Y1;

			//výpočetní část, mělo by být volané v případě úspěchu podmínky if(m.PtInSegment....
			uhel=m.uhelObloukuVsMys(X,Y,OR,RA,R,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
			d=m.R2Larc(R,uhel);//požadovaná délka na oblouku vybraná myší, vracení délky dané výseče, tj. k na(při)počítání měřené délky
			delka+=d;
      //vypočet času
			if(F->pom_element->pohon!=NULL)
			{
				if(v.MAG_LASO->predchozi->Element->n==MaxInt && v.MAG_LASO->predchozi->sparovany!=NULL)
		  	{
					cas+=d/F->pom_element->pohon->aRD;
					//if(v.MAG_LASO->predchozi->sparovany->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
		  	else
		  	{
					cas+=d/F->pom_element->pohon->aRD;
					//if(v.MAG_LASO->predchozi->Element->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
			}

			//vykreslovací část																																																																			 //zjištění kam jsem kliknul v oblouku, viz. vykresli_Gelement, uhel = rotacni uhel
			//TPointD *souradnice_k_dalsimu_pouziti=//poslední souřadnice vráceného pole lze použít např. na umístění teploměru, či pokud se nebude hodit přímo při vykreslení (ale jinak zbytečné), lze použít samostatnou matematickou metodu: //TPointD *Cmy::getArcLine(double X,double Y,double orientace,double rotacni_uhel,double radius)
			vykresli_Gelement(canv,X,Y,OR,uhel,R,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",2);//vykreslení měřícího kurzoru, popisek není nutné používat, metodu ještě vylepším
		}

		//vykreslení části přímky
		else if(F->pom_element!=NULL && v.MAG_LASO->Element!=NULL)
		{
			//načítání parametrů
			double delka_Pud;
			X=F->pom_element->geo.X1;Y=F->pom_element->geo.Y1;
			if(v.MAG_LASO->dalsi==NULL){X=v.MAG_LASO->predchozi->Element->geo.X1;Y=v.MAG_LASO->predchozi->Element->geo.Y1;}
			OR=F->pom_element->geo.orientace;
			//výpočetní část
			TPointD konec=v.bod_na_geometrii(F->pom_element);
			delka_Pud=d=m.delka(X,Y,konec.x,konec.y);
			d=m.castPrepony(d,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
			delka+=d;

			//vypočet času
			if(F->pom_element->pohon!=NULL)
			{
				double d_pom=d;
				//přičítání času čekání při přejíždění nad pozicemi bufferu
				if(F->pom_element->eID==0)
				{
					double rozdil=F->pom_element->geo.delka-(F->pom_element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice);
					if(d>rozdil)
					{
						cas+=m.V2WT(ceil((d-rozdil)/v.PP.delka_podvozek),v.PP.TT);//připočítání WT na aktuálním vozíku
						d_pom=rozdil;//zmenšení délky jen na délku pojezdu
					}
				}
				//vypočet času přejezdu a WTpalec
				if(v.MAG_LASO->predchozi->Element->n==MaxInt && v.MAG_LASO->predchozi->sparovany!=NULL)
				{
					cas+=d_pom/F->pom_element->pohon->aRD;
					//if(v.MAG_LASO->predchozi->sparovany->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
				else
				{
					cas+=d_pom/F->pom_element->pohon->aRD;
					//if(v.MAG_LASO->predchozi->Element->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
			}

			//vykreslovací část
			vykresli_Gelement(canv,X,Y,OR,0,delka_Pud,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",2);
		}
	}

	////liniové měřění
	else if(v.MAG_LASO->Element!=NULL && (F->pom_element==NULL || (F->pom_element!=NULL && F->pom_element!=v.MAG_LASO->predchozi->Element && F->pom_element->predchozi2!=v.MAG_LASO->predchozi->Element && F->pom_element->predchozi->predchozi2!=v.MAG_LASO->predchozi->Element && F->pom_element->predchozi->predchozi!=v.MAG_LASO->predchozi->Element)))
	{
		//načtení parametrů
		X=v.MAG_LASO->Element->geo.X1;Y=v.MAG_LASO->Element->geo.Y1;
		//výpočetní část
		delka=m.delka(X,Y,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
		if(F->pom_element!=NULL)delka=m.castPrepony(delka,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
		azimut=m.azimut(X,Y,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
		//výpočt času a vykreslení
		if(v.MAG_LASO->dalsi==NULL && v.MAG_LASO->sparovany!=NULL && F->pom_element!=NULL && F->pom_element->pohon!=NULL && v.MAG_LASO->sparovany==F->pom_element)
		{
			//kontrola + vypočet WT na vozík v bufferu
			d=delka;
      if(F->pom_element->eID==0)
			{
				//výpočet vzdálenosti od stopstanice
				double s=m.delka(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,F->pom_element->geo.X4,F->pom_element->geo.Y4);
				s=m.castPrepony(s,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
				double check=m.delka(v.MAG_LASO->Element->geo.X4,v.MAG_LASO->Element->geo.Y4,F->pom_element->geo.X4,F->pom_element->geo.Y4);
				check=m.castPrepony(check,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
				//výpočet velikosti bufferu stopstanice
				double buf=F->pom_element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice;
				if(s<buf)//pokud je vzdálenost od stopstanice menší nž buffer, tzn. jsem v bufferu
				{
					cas+=m.V2WT(ceil((buf-s)/v.PP.delka_podvozek),v.PP.TT);//připočítání WT na aktuálním vozíku
					delka-=buf-s;//zmenšení délky jen na délku pojezdu
					if(check<buf)delka=0;
				}
			}
			//vypočet času přejezdu
			cas+=delka/F->pom_element->pohon->aRD;
      //vykreslení
			vykresli_Gelement(canv,X,Y,azimut,0,d,clMeridlo,2,String(m.round2double(d*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",3);
		}
		else vykresli_Gelement(canv,X,Y,azimut,0,delka,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]");
	}
}
////---------------------------------------------------------------------------
//vykreslí měření proti trendu linky
void Cvykresli::vykresli_meridlo_proti_trendu(TCanvas *canv,bool prichyceno)
{
	////deklarace
	double R,RA,OR,X,Y,uhel=0,delka=0,azimut,cas=0,d;

	////test měření proti trendu
	if(F->pom_element!=NULL && (F->pom_element->dalsi==v.MAG_LASO->predchozi->Element || F->pom_element->dalsi2==v.MAG_LASO->predchozi->Element || (F->pom_element->dalsi!=NULL && F->pom_element->dalsi->dalsi==v.MAG_LASO->predchozi->Element) || F->pom_element->dalsi2==v.MAG_LASO->predchozi->Element))
	{
		Cvektory::TCesta *C=v.MAG_LASO->dalsi;
		while(C!=NULL)
		{
			//nastavení parametrů pro vykreslení + výpočet délky
			X=C->Element->geo.X1;Y=C->Element->geo.Y1;
			RA=C->Element->geo.rotacni_uhel;
			OR=C->Element->geo.orientace;
			uhel=0;
			if(C->n==1)
			{
				X=v.MAG_LASO->Element->geo.X1;Y=v.MAG_LASO->Element->geo.Y1;
				RA=v.MAG_LASO->sparovany->geo.rotacni_uhel;
				OR=m.Rt90(v.MAG_LASO->sparovany->geo.orientace-180);
			}
			//výpočet delky
			if(C->Element->n!=MaxInt && C->n!=1)d=C->Element->geo.delka;
			else if(C->Element->geo.typ==0)
			{
				d=m.delka(X,Y,C->Element->geo.X1,C->Element->geo.Y1);
				d=m.castPrepony(d,C->Element->geo.delka,C->Element->geo.delkaPud,C->Element->geo.HeightDepp);
			}
			else
			{
				d=m.delka(X,Y,C->Element->geo.X4,C->Element->geo.Y4);
				uhel=m.T2Aarc(C->Element->geo.radius,d);
				d=m.R2Larc(C->Element->geo.radius,uhel);
			}
			delka+=d;

			if(C->Element->geo.typ==0)
			{
  			R=d;
				uhel=0;
			}
  		else
			{
				R=C->Element->geo.radius;
				if(C->n==1)R=v.MAG_LASO->sparovany->geo.radius;
				if(uhel==0)uhel=RA;//max z rotačního úhlu
			}
			//výpočet času
			if((C->Element->n!=MaxInt && C->Element->pohon==NULL) || (C->Element->n==MaxInt && C->sparovany!=NULL && C->sparovany->pohon==NULL));
			else
			{
				if(C->Element->n==MaxInt && C->sparovany!=NULL)
				{
					cas+=d/C->sparovany->pohon->aRD;
					if(v.vrat_druh_elementu(C->sparovany)==0)cas+=C->sparovany->data.PT1+C->sparovany->data.PT2+C->sparovany->WT+C->sparovany->PTotoc+C->sparovany->data.WTstop;;
					if(C->sparovany->eID==0 && C->dalsi==NULL && C->Element->geo.X2==C->Element->geo.X3 && C->Element->geo.X3==C->Element->geo.X4)
					{
						cas-=(C->sparovany->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)/C->sparovany->pohon->aRD;
					}
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany==NULL && C->dalsi->Element->pohon!=NULL && C->sparovany->pohon!=C->dalsi->Element->pohon)cas+=m.cekani_na_palec(0,C->dalsi->Element->pohon->roztec,C->dalsi->Element->pohon->aRD,3);
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany!=NULL && C->dalsi->sparovany->pohon!=NULL && C->sparovany->pohon!=C->dalsi->sparovany->pohon)cas+=m.cekani_na_palec(0,C->dalsi->sparovany->pohon->roztec,C->dalsi->sparovany->pohon->aRD,3);
				}
				else if(C->Element->eID!=0 || (C->Element->eID==0 && (v.MAG_LASO->sparovany!=C->Element || (v.MAG_LASO->sparovany!=C->Element && d==C->Element->geo.delka))))
				{
					cas+=d/C->Element->pohon->aRD;
					if(v.vrat_druh_elementu(C->Element)==0)cas+=C->Element->data.PT1+C->Element->data.PT2+C->Element->WT+C->Element->PTotoc;
					if(C->Element->eID==0)
					{
						cas+=C->Element->data.WTstop;
						if(d>=C->Element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)
							cas-=(C->Element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice)/C->Element->pohon->aRD;
					}
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany==NULL && C->dalsi->Element->pohon!=NULL && C->Element->pohon!=C->dalsi->Element->pohon)cas+=m.cekani_na_palec(0,C->dalsi->Element->pohon->roztec,C->dalsi->Element->pohon->aRD,3);
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany!=NULL && C->dalsi->sparovany->pohon!=NULL && C->Element->pohon!=C->dalsi->sparovany->pohon)cas+=m.cekani_na_palec(0,C->dalsi->sparovany->pohon->roztec,C->dalsi->sparovany->pohon->aRD,3);
				}
				else
				{
					//řešit pouze část bufferu
					//kontrola + vypočet WT na vozík v bufferu
					double d_pom=d;
					if(C->Element->eID==0)
					{
						//výpočet vzdálenosti od stopstanice
						double check=m.delka(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,C->Element->geo.X4,C->Element->geo.Y4);
						check=m.castPrepony(check,C->Element->geo.delka,C->Element->geo.delkaPud,C->Element->geo.HeightDepp);
						double s=m.delka(v.MAG_LASO->Element->geo.X4,v.MAG_LASO->Element->geo.Y4,C->Element->geo.X4,C->Element->geo.Y4);
						s=m.castPrepony(s,C->Element->geo.delka,C->Element->geo.delkaPud,C->Element->geo.HeightDepp);
						//výpočet velikosti bufferu stopstanice
						double buf=C->Element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice;
						//pokud je vzdálenost od stopstanice menší nž buffer, tzn. jsem v bufferu
	    			if(s<=buf)
	    			{
							cas+=m.V2WT(ceil((buf-s)/v.PP.delka_podvozek),v.PP.TT);//připočítání WT na aktuálním vozíku
							if(check<buf)d_pom=0;//nepočítám žádný další čas
							else d_pom-=buf-s;//zmenšení délky jen na délku pojezdu
						}
					}
      		//vypočet času přejezdu
					cas+=d_pom/C->Element->pohon->aRD;

					//výpočet WT při přechodu na jiný pohon
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany==NULL && C->dalsi->Element->pohon!=NULL && C->Element->pohon!=C->dalsi->Element->pohon)cas+=m.cekani_na_palec(0,C->dalsi->Element->pohon->roztec,C->dalsi->Element->pohon->aRD,3);
					if(C->dalsi!=NULL && C->dalsi->Element!=NULL && C->dalsi->sparovany!=NULL && C->dalsi->sparovany->pohon!=NULL && C->Element->pohon!=C->dalsi->sparovany->pohon)cas+=m.cekani_na_palec(0,C->dalsi->sparovany->pohon->roztec,C->dalsi->sparovany->pohon->aRD,3);
				}
			}
			//vykreslení segmentu
			short typ=0;
			if(C->n==1)typ=1;
			if(C->n==1 && C->Element->geo.typ!=0)//pokud je první segment část oblouku nutné pozměnit vykreslení
			{
				X=C->Element->geo.X1;Y=C->Element->geo.Y1;
				OR=C->Element->geo.orientace;
				uhel=90-uhel;
				if(C->Element->geo.rotacni_uhel<0)uhel=-uhel;
				typ=2;
			}
			vykresli_Gelement(canv,X,Y,OR,uhel,R,clMeridlo,2,"","",typ);
			//posun na další segment
			C=C->dalsi;
		}
		delete C;C=NULL;
	}

	if(v.MAG_LASO->dalsi!=NULL && F->pom_element!=NULL && (F->pom_element->dalsi==v.MAG_LASO->predchozi->Element || F->pom_element->dalsi2==v.MAG_LASO->predchozi->Element || (F->pom_element->dalsi!=NULL && F->pom_element->dalsi->dalsi==v.MAG_LASO->predchozi->Element) || F->pom_element->dalsi2==v.MAG_LASO->predchozi->Element))
	{
		//vykreslení části oblouku
		if(F->pom_element->geo.typ!=0)
		{
			//načítání parametrů
			R=F->pom_element->geo.radius;
			RA=-F->pom_element->geo.rotacni_uhel;//rotační úhel, pod kterým je oblouk rotován - směřován (proti směru hodinových ručiček), může být záporný (po směru hodinových ručiček)
			OR=m.Rt90(F->pom_element->geo.orientace-F->pom_element->geo.rotacni_uhel-180);
			X=F->pom_element->geo.X4,Y=F->pom_element->geo.Y4;

			//výpočetní část, mělo by být volané v případě úspěchu podmínky if(m.PtInSegment....
			uhel=m.uhelObloukuVsMys(X,Y,OR,RA,R,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
			d=m.R2Larc(R,uhel);//požadovaná délka na oblouku vybraná myší, vracení délky dané výseče, tj. k na(při)počítání měřené délky
			delka+=d;
      //vypočet času
			if(F->pom_element->pohon!=NULL)
			{
				if(v.MAG_LASO->predchozi->Element->n==MaxInt && v.MAG_LASO->predchozi->sparovany!=NULL)
		  	{
					cas+=d/F->pom_element->pohon->aRD;
					if(!prichyceno && v.MAG_LASO->predchozi->sparovany->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
		  	else
		  	{
					cas+=d/F->pom_element->pohon->aRD;
					if(!prichyceno && v.MAG_LASO->predchozi->Element->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);
				}
			}

			//vykreslovací část																																																																			 //zjištění kam jsem kliknul v oblouku, viz. vykresli_Gelement, uhel = rotacni uhel
			//TPointD *souradnice_k_dalsimu_pouziti=//poslední souřadnice vráceného pole lze použít např. na umístění teploměru, či pokud se nebude hodit přímo při vykreslení (ale jinak zbytečné), lze použít samostatnou matematickou metodu: //TPointD *Cmy::getArcLine(double X,double Y,double orientace,double rotacni_uhel,double radius)
			vykresli_Gelement(canv,X,Y,OR,uhel,R,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",2);//vykreslení měřícího kurzoru, popisek není nutné používat, metodu ještě vylepším
		}
		//vykreslení části linie
		else
		{
			//načítání parametrů
			double delka_Pud;
			X=F->pom_element->geo.X4;Y=F->pom_element->geo.Y4;
			if(v.MAG_LASO->dalsi==NULL){X=v.MAG_LASO->predchozi->Element->geo.X1;Y=v.MAG_LASO->predchozi->Element->geo.Y1;}
			OR=m.Rt90(F->pom_element->geo.orientace-180);
			//výpočetní část
			TPointD konec=v.bod_na_geometrii(F->pom_element);
			d=m.delka(X,Y,konec.x,konec.y);
			delka_Pud=d;
			d=m.castPrepony(d,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
			delka+=d;

			//vypočet času
			if(F->pom_element->pohon!=NULL)
			{
				double d_pom=d;
				//přičítání času čekání při přejíždění nad pozicemi bufferu
				if(F->pom_element->eID==0)
				{
					double buf=F->pom_element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice;
					cas+=F->pom_element->data.WTstop+F->pom_element->WT;
					if(d<=buf)d_pom=0;
					else d_pom-=buf;//odstranění přejezdu přes buffer
				}
				//vypočet času přejezdu a WTpalec
				if(v.MAG_LASO->predchozi->Element->n==MaxInt && v.MAG_LASO->predchozi->sparovany!=NULL)
				{
					cas+=d_pom/F->pom_element->pohon->aRD;
					if(!prichyceno && v.MAG_LASO->predchozi->sparovany->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);//není třeba zde řešít, řeší se výše v průchodu uložených segmentů
				}
				else
				{
					cas+=d_pom/F->pom_element->pohon->aRD;
					if(!prichyceno && v.MAG_LASO->predchozi->Element->pohon!=F->pom_element->pohon)cas+=m.cekani_na_palec(0,F->pom_element->pohon->roztec,F->pom_element->pohon->aRD,3);//není třeba zde řešít, řeší se výše v průchodu uložených segmentů
				}
			}

			//vykreslovací část
			vykresli_Gelement(canv,X,Y,OR,0,delka_Pud,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",2);
		}
	}
	else if(v.MAG_LASO->Element!=NULL)
	{
		//načtení parametrů
		X=v.MAG_LASO->Element->geo.X4;Y=v.MAG_LASO->Element->geo.Y4;
		//výpočetní část
		delka=m.delka(X,Y,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
		if(F->pom_element!=NULL)delka=m.castPrepony(delka,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
		azimut=m.azimut(X,Y,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
		//výpočt času a vykreslení
		if(v.MAG_LASO->dalsi==NULL && v.MAG_LASO->sparovany!=NULL && F->pom_element!=NULL && F->pom_element->pohon!=NULL && v.MAG_LASO->sparovany==F->pom_element)
		{
      //vykreslení části linie + výpočet času v bufferu
			if(F->pom_element->geo.typ==0)
			{
		  	//kontrola + vypočet WT na vozík v bufferu
		  	d=delka;
		  	if(F->pom_element->eID==0)
		  	{
  	  		//výpočet vzdálenosti od stopstanice
					double check=m.delka(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,F->pom_element->geo.X4,F->pom_element->geo.Y4);
					check=m.castPrepony(check,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
					double s=m.delka(v.MAG_LASO->Element->geo.X4,v.MAG_LASO->Element->geo.Y4,F->pom_element->geo.X4,F->pom_element->geo.Y4);
          s=m.castPrepony(s,F->pom_element->geo.delka,F->pom_element->geo.delkaPud,F->pom_element->geo.HeightDepp);
		  		//výpočet velikosti bufferu stopstanice
		  		double buf=F->pom_element->data.pocet_voziku*v.PP.delka_podvozek-v.PP.uchyt_pozice;
		  		//pokud je vzdálenost od stopstanice menší nž buffer, tzn. jsem v bufferu
		  		if(s<=buf)
		  		{
		  			cas+=m.V2WT(ceil((buf-s)/v.PP.delka_podvozek),v.PP.TT);//připočítání WT na aktuálním vozíku
		  			if(check<buf)delka=0;//nepočítám žádný další čas
		  			else delka-=buf-s;//zmenšení délky jen na délku pojezdu
		  		}
		  	}
		  	//vypočet času přejezdu
		  	cas+=delka/F->pom_element->pohon->aRD;
		  	//vykreslení
				vykresli_Gelement(canv,X,Y,azimut,0,d,clMeridlo,2,String(m.round2double(d*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",3);
			}
			//vykreslení části oblouku, na pohonu
			else
			{
				X=v.MAG_LASO->sparovany->geo.X1;Y=v.MAG_LASO->sparovany->geo.Y1;
				double u1,u2;
				u1=m.uhelObloukuVsMys(X,Y,v.MAG_LASO->sparovany->geo.orientace,v.MAG_LASO->sparovany->geo.rotacni_uhel,v.MAG_LASO->sparovany->geo.radius,v.MAG_LASO->Element->geo.X1,v.MAG_LASO->Element->geo.Y1);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
				u2=m.uhelObloukuVsMys(X,Y,v.MAG_LASO->sparovany->geo.orientace,v.MAG_LASO->sparovany->geo.rotacni_uhel,v.MAG_LASO->sparovany->geo.radius,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
				//měření proti trendu, zaměnit u1 za u2
				if((u2-u1<0 && u1>0 && u2>0) || (u2-u1>0 && u1<0 && u2<0))
				{
					delka=u1;
					u1=u2;
					u2=delka;
        }
				delka=m.R2Larc(v.MAG_LASO->sparovany->geo.radius,u2)-m.R2Larc(v.MAG_LASO->sparovany->geo.radius,u1);
				cas=delka/F->pom_element->pohon->aRD;
				vykresli_Gelement(canv,X,Y,v.MAG_LASO->sparovany->geo.orientace,u2,v.MAG_LASO->sparovany->geo.radius,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]",String(m.round2double(cas,2))+" [s]",2);
				vykresli_Gelement(canv,X,Y,v.MAG_LASO->sparovany->geo.orientace,u1,v.MAG_LASO->sparovany->geo.radius,(TColor) RGB(255,69,0),2,"","",2,clMeridlo);
			}
		}
		//vykreslení části oblouky bez pohonu
		else if(F->pom_element->geo.typ!=0 && F->pom_element==v.MAG_LASO->sparovany)
		{
			X=v.MAG_LASO->sparovany->geo.X1;Y=v.MAG_LASO->sparovany->geo.Y1;
			double u1,u2;
			u1=m.uhelObloukuVsMys(X,Y,v.MAG_LASO->sparovany->geo.orientace,v.MAG_LASO->sparovany->geo.rotacni_uhel,v.MAG_LASO->sparovany->geo.radius,v.MAG_LASO->Element->geo.X1,v.MAG_LASO->Element->geo.Y1);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
			u2=m.uhelObloukuVsMys(X,Y,v.MAG_LASO->sparovany->geo.orientace,v.MAG_LASO->sparovany->geo.rotacni_uhel,v.MAG_LASO->sparovany->geo.radius,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);
			//měření proti trendu, zaměnit u1 za u2
			if((u2-u1<0 && u1>0 && u2>0) || (u2-u1>0 && u1<0 && u2<0))
			{
				delka=u1;
				u1=u2;
				u2=delka;
			}
			delka=m.R2Larc(v.MAG_LASO->sparovany->geo.radius,u2)-m.R2Larc(v.MAG_LASO->sparovany->geo.radius,u1);
			vykresli_Gelement(canv,X,Y,v.MAG_LASO->sparovany->geo.orientace,u2,v.MAG_LASO->sparovany->geo.radius,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]","",2);
			vykresli_Gelement(canv,X,Y,v.MAG_LASO->sparovany->geo.orientace,u1,v.MAG_LASO->sparovany->geo.radius,(TColor) RGB(255,69,0),2,"","",2,clMeridlo);
		}
    //vykreslení linie bez pohonu
		else vykresli_Gelement(canv,X,Y,azimut,0,delka,clMeridlo,2,String(m.round2double(delka*1000,2))+" [mm]");
	}
}
////---------------------------------------------------------------------------
//vykreslí teploměry a cestu mezi nimi
void Cvykresli::vykresli_oblast_teplomery(TCanvas *canv,short scena,Cvektory::TObjekt *Objekt)
{
  //kontrola zda existují teploměry
	if(Objekt!=NULL && Objekt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *teplomery=v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,v.ZAKAZKA_akt);
		if(teplomery!=NULL)
		{
      //nastavení barev
			TColor clTeplomery=clBlack;//default, vytěkání
			if(teplomery->prvni->eID==400)clTeplomery=clRed;//sušení
			if(teplomery->prvni->eID==401)clTeplomery=clBlue;//chlazení
			clTeplomery=m.clIntensive(clTeplomery,210);//zesvětlení barvy

			////vykreslení teploměrů
			vykresli_element(canv,scena,m.L2Px(teplomery->prvni->X),m.L2Py(teplomery->prvni->Y),teplomery->prvni->name,"",teplomery->prvni->eID,1,m.Rt90(teplomery->prvni->sparovany->geo.orientace-teplomery->prvni->sparovany->geo.rotacni_uhel-90),1,1.5,0,0,0,teplomery->prvni);
			vykresli_element(canv,scena,m.L2Px(teplomery->posledni->X),m.L2Py(teplomery->posledni->Y),teplomery->posledni->name,"",teplomery->posledni->eID,1,m.Rt90(teplomery->posledni->sparovany->geo.orientace-teplomery->posledni->sparovany->geo.rotacni_uhel-90),1,1.5,0,0,0,teplomery->posledni);

			/////vykresení cesty
			if(teplomery->prvni->sparovany!=teplomery->posledni->sparovany)
			{
				vykresli_segment_cesty_teplomeru(canv,teplomery->prvni,clTeplomery,1);//vykreslení cesty od prvního teploměru
				vykresli_segment_cesty_teplomeru(canv,teplomery->posledni,clTeplomery,2);//vykreslení cesty k poslednímu teploměru
			}
      //vykrelsení spojnice mezi teploměry na jednom segmentu pohonu
			else
			{
				double X1,Y1,OR,RA,R;
				//vykreslení linie mezi teploměry
				if(teplomery->prvni->sparovany->geo.typ==0)
				{
			  	OR=teplomery->prvni->geo.orientace;
			  	RA=teplomery->prvni->geo.rotacni_uhel;
			  	X1=teplomery->prvni->geo.X1;Y1=teplomery->prvni->geo.Y1;
			  	R=m.delka(X1,Y1,teplomery->posledni->geo.X4,teplomery->posledni->geo.Y4);
					vykresli_Gelement(canv,X1,Y1,OR,RA,R,clTeplomery,3);
				}
        //vykrelsení čísti úseku mezi teploměry
				else
				{
					;//nelze aplikovat stejnou metodu vykreslení jako u mag_lasa, kreslí se na prázdný prostor, zde nopomůže barva kolejí
        }
      }
	  	Cvektory::TCesta *ct=teplomery->cesta->dalsi;
	  	while(ct!=NULL)
			{
				vykresli_segment_cesty_teplomeru(canv,ct->Element,clTeplomery);
				//posun na další segment cesty
				ct=ct->dalsi;
			}
			delete ct;ct=NULL;
		}
    teplomery=NULL;delete teplomery;
	}
}
////---------------------------------------------------------------------------
//vykreslí segment cesty oblasti teploměrů, parametr teploměr udává zda se bude vykreslovat prvni nebo posledni teploměr, 1 ... prvni, 2 ... posledni
void Cvykresli::vykresli_segment_cesty_teplomeru(TCanvas *canv,Cvektory::TElement *Element,TColor barva,short teplomer)
{
  //deklarace
	double X1,Y1,X2,Y2,OR,RA,R;
	//načtení obecných parametrů
	R=Element->geo.radius;
	OR=Element->geo.orientace;
	RA=Element->geo.rotacni_uhel;
	X1=Element->geo.X1;Y1=Element->geo.Y1;
	X2=Element->geo.X4;Y2=Element->geo.Y4;
	//načtení souřadnic
	if(teplomer==1){X2=Element->sparovany->geo.X4;Y2=Element->sparovany->geo.Y4;}
	if(teplomer==2){X1=Element->sparovany->geo.X1;Y1=Element->sparovany->geo.Y1;}
	//výpočet délky a úhlu, prouze pro teploměry
	if(teplomer>0)
	{
		R=Element->sparovany->geo.radius;
		OR=Element->sparovany->geo.orientace;
		RA=Element->sparovany->geo.rotacni_uhel;
		if(Element->sparovany->geo.typ==0)R=m.delka(X1,Y1,X2,Y2);
		else
		{
      //výpočet RA
			OR=Element->sparovany->geo.orientace;
			R=Element->sparovany->geo.radius;
			double d=m.delka(X1,Y1,X2,Y2);
			RA=m.T2Aarc(R,d);
      //kontrola zda sem dostal validní výsledek, pokud ne obrácení RA
			if(teplomer==2 && ((Element->sparovany->geo.rotacni_uhel<0 && RA>0) || (Element->sparovany->geo.rotacni_uhel>0 && RA<0)))RA=-RA;
			//vykreslit opačně
			if(teplomer==1)
			{
				d=X1;X1=X2;X2;d;
				d=Y1;Y1=Y2;Y2;d;
				OR=m.Rt90(Element->sparovany->geo.orientace-Element->sparovany->geo.rotacni_uhel-180);//obrácení orientace
				if((Element->sparovany->geo.rotacni_uhel<0 && RA<0) || (Element->sparovany->geo.rotacni_uhel>0 && RA>0))RA=-RA;
			}
		}
	}
	else if(Element->geo.typ==0)R=Element->geo.delka;
  //nastavení šířky
	float width=m.m2px(v.PP.sirka_podvozek/F->Zoom)+2*(1/3.0*F->Zoom)/F->Zoom;//nastavení šířky
	//samotné vykreslení segmentu, kontrola, pokud existuje jen jden segment, vykreslí ho pouze jednou ne 2x
	if(X1!=X2 || Y1!=Y2)vykresli_Gelement(canv,X1,Y1,OR,RA,R,barva,width);
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
////nastaví pero                                                             //http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079,http://mrxray.on.coocan.jp/Delphi/plSamples/190_LineEdgeStyle.htm
void Cvykresli::set_pen2(TCanvas *canv, TColor color, int width, int ENDCAP,int JOIN,bool INSIDEFRAME,DWORD *Array,unsigned short LenghtArray,int TextureWidth)//vrátí HANDLE na nastavení pera,//popř.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE a PS_JOIN_ROUND, PS_JOIN_MITER (lze nastavit přes SetMiterlimit) PS_JOIN_BEVEL, viz Matoušek III str. 179 či http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079, dodáním předposledního parametru UserLineArray např. DWORD pole[]={m.round(3/3.0*F->Zoom),m.round(0.5/3.0*F->Zoom),m.round(0.2/3.0*F->Zoom),m.round(0.5/3.0*F->Zoom)} apod.  lze nadefinovat vlastní typ linie, poslední parametr je počet prvků onoho pole
{                                                                         //PS_JOIN_ROUND, PS_JOIN_MITER (lze nastavit přes SetMiterlimit) PS_JOIN_BEVEL,
	DeleteObject(canv->Pen->Handle);//zruší původní pero
	DWORD pStyle = 0;
	int STYLE=PS_SOLID;
	DWORD Lenght=0;
	if(Array!=NULL)
	{
		//Lenght=sizeof(Array)/sizeof(Array[0]);//toto nefunguje pokud je pole voláno přes parametr a ne alokováno v této metodě lokálně
		Lenght=LenghtArray;
		STYLE=PS_USERSTYLE;
	}

	if(INSIDEFRAME)pStyle = PS_GEOMETRIC | STYLE | ENDCAP | JOIN | PS_INSIDEFRAME;
	else pStyle = PS_GEOMETRIC | STYLE | ENDCAP | JOIN;
	DWORD pWidth = width;

	LOGBRUSH lBrush;
	lBrush.lbColor = color;
	if(TextureWidth==0)//bez textury
	{
		lBrush.lbStyle = BS_SOLID;
		lBrush.lbHatch = 0;
	}
	else//textura
	{
		lBrush.lbStyle = BS_PATTERN;
		lBrush.lbHatch = (unsigned)rastrovani(color,TextureWidth,5)->Handle;
	}

	canv->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, Lenght, Array);
}
////---------------------------------------------------------------------------
//vytvoří šrafovaný vzor
Graphics::TBitmap *Cvykresli::srafura(TColor color)
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
//vytvoří rastrovaný vzor
Graphics::TBitmap *Cvykresli::rastrovani(TColor color,short width, short distance)
{
	Graphics::TBitmap * Vzor=new Graphics::TBitmap;
	Vzor->Height=width;Vzor->Width=Vzor->Height;
	for(int x=0;x<Vzor->Width;x+=distance)
	{
		for(int y=0;y<Vzor->Height;y+=distance)
		{
			Vzor->Canvas->Pixels[x][y]=color;
//			Vzor->Canvas->Pixels[x+1][y]=color;
			Vzor->Canvas->Pixels[x+1][y+1]=color;
//			Vzor->Canvas->Pixels[x][y+1]=color;
		}
	}
	return Vzor;
}
////---------------------------------------------------------------------------
//asi možno smazat, už se asi nevyužívá
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
void Cvykresli::vykresli_kurzor_kabiny (TCanvas *canv, int id, int X, int Y, Cvektory::TObjekt *p)
{
	Cvektory::TElement *E=F->pom_element,*e_posledni=NULL;
	if(F->pom_element==NULL)E=v.ELEMENTY->predchozi;
	if(E->eID!=300 && X!=-200)//nejedná se o vkládání na vedlejší větev
	{
		bool spojnice1=true,spojnice2=true;
		int rotace=90;//defaultní hodnota,dáno azimutem přímky
		double X1=0,Y1=0,X2=0,Y2=0,Xd=0,Yd=0,Xp=0,Yp=0,azimut=0;
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
			short oblast=0;
			Cvektory::TObjekt *dalsi;
			p=v.vrat_objekt(E->objekt_n);
			if(F->prichytavat_k_mrizce==1)oblast=v.oblast_objektu(p,X,Y);
			if(F->prichytavat_k_mrizce==2 && (p->n!=1 || p->n==1 && oblast==1 || p->n==1 && oblast==2 && v.OBJEKTY->predchozi->n==1))oblast=0;
			if(oblast>1 && p->n>1)oblast=0;
			if(predchozi_oblast!=oblast && oblast!=0 && v.OBJEKTY->predchozi->n==1)predchozi_oblast=oblast;
			if(predchozi_oblast==2){Xp=m.L2Px(p->element->geo.X1),Yp=m.L2Py(p->element->geo.Y1);}

      //nastavení pro případy přiichytávání
			if(oblast!=0)
			{
		  	if(p!=NULL && v.OBJEKTY->predchozi->n!=p->n){spojnice1=false;spojnice2=false;}
		  	//ukazatel na předchozí a poslední/další
		  	if(p==NULL){p=v.vrat_objekt(v.ELEMENTY->predchozi->objekt_n);dalsi=v.vrat_objekt(v.ELEMENTY->dalsi->objekt_n);}
		  	else {if(E->dalsi!=NULL)dalsi=v.vrat_objekt(E->dalsi->objekt_n);else dalsi=dalsi=v.vrat_objekt(v.ELEMENTY->dalsi->objekt_n);}
		  	//souřadncie předchozího objektu
		  	e_posledni=E;//F->d.v.vrat_posledni_element_objektu(p);
				Xp=m.L2Px(e_posledni->geo.X4);Yp=m.L2Py(e_posledni->geo.Y4);
			}

			switch(oblast)
			{
				case 0://mimo oblast objektu + vykreslení nepřichytávání
				{
        	Xp=m.L2Px(E->geo.X4);Yp=m.L2Py(E->geo.Y4);
		    	//zjištění rotace
		    	azimut=m.azimut(m.P2Lx(Xp),m.P2Ly(Yp),m.P2Lx(X),m.P2Ly(Y));
		    	rotace=m.Rt90(azimut);
		    	switch(rotace)
		    	{
		    		case 0:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y-m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		    		case 270:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X-m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
		    		case 180:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y+m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		    		case 90:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X+m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
		    	}
		    	//vykreslení kurzoru
		    	canv->Rectangle(X1,Y1,X2,Y2);

		    	canv->MoveTo(Xp,Yp);
		    	canv->LineTo(X,Y);
		    	sipka(canv,(X+Xp)/2.0,(Y+Yp)/2.0,azimut,true,3,clBlack,clWhite,pmNotXor);

		    	//vykreslení spojnice k poslední/další je-li to nutné
		    	Cvektory::TElement *dalsi=E->dalsi;
		    	if(dalsi==NULL)dalsi=v.ELEMENTY->dalsi;
		    	if(dalsi->eID==301 && dalsi->predchozi2==E){X2=m.L2Px(dalsi->X);Y2=m.L2Py(dalsi->Y);}
		    	else {X2=m.L2Px(dalsi->geo.X1);Y2=m.L2Py(dalsi->geo.Y1);}
		    	canv->Pen->Style=psDot;//nastevení čarkované čáry
		    	canv->MoveTo(Xd,Yd);
		    	canv->LineTo(X2,Y2);
		    	//vykreslení šipek
		    	sipka(canv,(Xd+X2)/2.0,(Yd+Y2)/2.0,m.azimut(X2,Y2,Xd,Yd)*(-1),true,3,clBlack,clWhite,pmNotXor);
		    	//vykreslení indikace vkládání mezi objekty
		    	if(E!=v.ELEMENTY->predchozi)
		    	{
		    		sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,false);
		    		sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,true);
		    	}
				}break;
				case 1://za objektem
				{
        	//zjištění rotace
					azimut=m.azimut(m.P2Lx(Xp),m.P2Ly(Yp),m.P2Lx(X),m.P2Ly(Y));
					rotace=m.Rt90(azimut);
					double p_rotace=m.Rt90(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel);
					if(!spojnice1 && E!=v.ELEMENTY->predchozi && p->orientace!=dalsi->orientace && rotace!=p->orientace &&  rotace!=dalsi->orientace)rotace=p->orientace;//objekt vkládán mezi ostatní objekty
					switch(rotace)
			  	{
			  		case 0:if(p_rotace!=180){X1=Xp+m.m2px(rozmery_kabiny.y)/2.0;Y1=Yp;X2=Xp-m.m2px(rozmery_kabiny.y)/2.0;Y2=Yp-m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;}else{X1=Xp+m.m2px(rozmery_kabiny.y)/2.0;Y1=Yp;X2=Xp-m.m2px(rozmery_kabiny.y)/2.0;Y2=Yp+m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;}break;
			  		case 90:if(p_rotace==90 || p_rotace==0 || p_rotace==180){X1=Xp;Y1=Yp-m.m2px(rozmery_kabiny.y)/2.0;X2=Xp+m.m2px(rozmery_kabiny.x);Y2=Yp+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;}else{X1=Xp;Y1=Yp-m.m2px(rozmery_kabiny.y)/2.0;X2=Xp-m.m2px(rozmery_kabiny.x);Y2=Yp+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;}break;
			  		case 180:if(p_rotace!=0){X1=Xp+m.m2px(rozmery_kabiny.y)/2.0;Y1=Yp;X2=Xp-m.m2px(rozmery_kabiny.y)/2.0;Y2=Yp+m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;}else{X1=Xp+m.m2px(rozmery_kabiny.y)/2.0;Y1=Yp;X2=Xp-m.m2px(rozmery_kabiny.y)/2.0;Y2=Yp-m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;}break;
			  		case 270:if(p_rotace==270 || p_rotace==0 || p_rotace==180){X1=Xp;Y1=Yp-m.m2px(rozmery_kabiny.y)/2.0;X2=Xp-m.m2px(rozmery_kabiny.x);Y2=Yp+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;}else{X1=Xp;Y1=Yp-m.m2px(rozmery_kabiny.y)/2.0;X2=Xp+m.m2px(rozmery_kabiny.x);Y2=Yp+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;}break;
					}
			  	if(rotace==0 || rotace==180)Xd=Xp;else Yd=Yp;
					if(rotace==m.Rt90(p_rotace-180))rotace=p_rotace;
				}break;
				case 2://před prvním objektem
				{
					if(p->n==1 && v.OBJEKTY->predchozi->n!=1)//vkladání objektu před první, změna pořadí
		    	{
		    		//zjištění rotace
		    		azimut=m.azimut(p->element->geo.X1,p->element->geo.Y1,m.P2Lx(X),m.P2Ly(Y));
		    		rotace=m.Rt90(azimut);
		    		spojnice1=spojnice2=false;
		    		X=m.L2Px(p->element->geo.X1);Y=m.L2Py(p->element->geo.Y1);
		    		switch((int)p->orientace)
		    		{
		    			case 0:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y-m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		    			case 270:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X-m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
		    			case 180:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y+m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		    			case 90:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X+m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
		    		}
		    	}
		    	if(v.OBJEKTY->predchozi->n==1)//vkládání druhého objektu před první, změna trendu linky
					{
		    		//zjištění rotace
		    		azimut=m.azimut(p->element->geo.X1,p->element->geo.Y1,m.P2Lx(X),m.P2Ly(Y));
		    		rotace=m.Rt90(azimut);
		    		double p_rotace=m.Rt90(p->element->geo.orientace-p->element->geo.rotacni_uhel);
		    		switch(rotace)
		    		{
		    			case 0:if(p_rotace!=0){X1=m.L2Px(p->element->geo.X1)+m.m2px(rozmery_kabiny.y)/2.0;Y1=m.L2Py(p->element->geo.Y1);X2=m.L2Px(p->element->geo.X1)-m.m2px(rozmery_kabiny.y)/2.0;Y2=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.x);}else{X1=m.L2Px(p->element->geo.X1)+m.m2px(rozmery_kabiny.y)/2.0;Y1=m.L2Py(p->element->geo.Y1);X2=m.L2Px(p->element->geo.X1)-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y1+m.m2px(rozmery_kabiny.x);}break;
		    			case 90:if(p_rotace!=90){X1=m.L2Px(p->element->geo.X1);Y1=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.y)/2.0;X2=X1+m.m2px(rozmery_kabiny.x);Y2=m.L2Py(p->element->geo.Y1)+m.m2px(rozmery_kabiny.y)/2.0;}else{X1=m.L2Px(p->element->geo.X1);Y1=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.y)/2.0;X2=X1-m.m2px(rozmery_kabiny.x);Y2=m.L2Py(p->element->geo.Y1)+m.m2px(rozmery_kabiny.y)/2.0;}break;
		    			case 180:if(p_rotace!=180){X1=m.L2Px(p->element->geo.X1)+m.m2px(rozmery_kabiny.y)/2.0;Y1=m.L2Py(p->element->geo.Y1);X2=m.L2Px(p->element->geo.X1)-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y1+m.m2px(rozmery_kabiny.x);}else{X1=m.L2Px(p->element->geo.X1)+m.m2px(rozmery_kabiny.y)/2.0;Y1=m.L2Py(p->element->geo.Y1);X2=m.L2Px(p->element->geo.X1)-m.m2px(rozmery_kabiny.y)/2.0;Y2=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.x);}break;
		    			case 270:if(p_rotace==270){X1=m.L2Px(p->element->geo.X1);Y1=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.y)/2.0;X2=X1+m.m2px(rozmery_kabiny.x);Y2=m.L2Py(p->element->geo.Y1)+m.m2px(rozmery_kabiny.y)/2.0;}else{X1=m.L2Px(p->element->geo.X1);Y1=m.L2Py(p->element->geo.Y1)-m.m2px(rozmery_kabiny.y)/2.0;X2=X1-m.m2px(rozmery_kabiny.x);Y2=m.L2Py(p->element->geo.Y1)+m.m2px(rozmery_kabiny.y)/2.0;}break;
		    		}
		    		if(rotace==p_rotace)rotace=m.Rt90(p_rotace-180);
					}
				}break;
			}

			//vykreslení pro přichytávání
			if(oblast!=0)
			{
		  	canv->Rectangle(X1,Y1,X2,Y2);
		  	//vykreslení spojnice k předchozímu
		  	if((spojnice1 || F->prichytavat_k_mrizce==2) && oblast==0)
		  	{
		  		canv->MoveTo(Xp,Yp);
		  		canv->LineTo(X,Y);
		  		sipka(canv,(X+Xp)/2.0,(Y+Yp)/2.0,azimut,true,3,clBlack,clWhite,pmNotXor);
		  	}
		  	//vykreslení spojnice k poslední/další je-li to nutné
				if(!spojnice2 && E->dalsi!=NULL && (E->geo.X4!=E->dalsi->geo.X1 || E->geo.Y4!=E->dalsi->geo.Y1))spojnice2=true;//pokud nejsou objekty na sobě nalepené vykreslit spojnici k následujícímu
				if((spojnice2 || F->prichytavat_k_mrizce==2) && v.OBJEKTY->predchozi->n>=2 && oblast!=2)
		  	{
		  		canv->Pen->Style=psDot;//nastevení čarkované čáry
		  		canv->MoveTo(Xd,Yd);
					canv->LineTo(m.L2Px(dalsi->element->geo.X1),m.L2Py(dalsi->element->geo.Y1));
					//vykreslení šipek
					sipka(canv,(Xd+m.L2Px(dalsi->element->geo.X1))/2.0,(Yd+m.L2Py(dalsi->element->geo.Y1))/2.0,m.azimut(m.P2Lx(Xd),m.P2Ly(Yd),dalsi->element->geo.X1,dalsi->element->geo.Y1),true,3,clBlack,clWhite,pmNotXor);
				}
				if((!spojnice1 || !spojnice2) && E!=v.ELEMENTY->predchozi)//vykreslení šipek indikujících posun ostatních objektů
				{
					if(F->prichytavat_k_mrizce==1){X=Xp;Y=Yp;}
					if(F->prichytavat_k_mrizce==1 && oblast!=1 && p->n==1){rotace=p->orientace;X=m.L2Px(p->element->geo.X1);Y=m.L2Py(p->element->geo.Y1);}
					if(F->prichytavat_k_mrizce==2 && oblast==2 && p->n==1)rotace=p->orientace;
					sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,false);
					sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,true);
				}
			}
			//ukazatelové záležitosti
			e_posledni=NULL;delete e_posledni;
			dalsi=NULL;delete dalsi;
		}
		//uložení rotace objektu, využítí při vkládání objektu, přepočet z azimutu přímky na orientaci objektu
		orientace_objektu=rotace;
	}
	else if(X!=-200)odznac_oznac_vyhybku(canv,X,Y,p);
}
//---------------------------------------------------------------------------
//pří umistivání či posouvání vyhýbky
void Cvykresli::odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p,bool posun)
{
	//definice souřadnic
	Cvektory::TElement *V=F->pom_element,*E=F->pom_element->dalsi2;
	//nastavení rozměrů objektu
	TPoint rozmery_kabiny;
	switch(F->vybrany_objekt)
	{
		case 0:case 9://navěšování + svěšování
		rozmery_kabiny.x=5;rozmery_kabiny.y=3;break;//navěšování
		case 5:rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//lakovna
		case 6:rozmery_kabiny.x=10;rozmery_kabiny.y=3;break;//vytěkání
		case 7:rozmery_kabiny.x=20;rozmery_kabiny.y=3;break;//sušárna
		case 8:rozmery_kabiny.x=20;rozmery_kabiny.y=6;break;//chlazení
		default: rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//ostatní
	}
	//zjištění rotace
	double azimut=m.azimut(V->X,V->Y,m.P2Lx(X),m.P2Ly(Y));
	short rotace=m.Rt90(azimut);
	double Xd=0,Yd=0,X1=0,Y1=0,X2=0,Y2=0;
	switch(rotace)
	{
		case 0:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y-m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		case 270:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X-m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
		case 180:X1=X+m.m2px(rozmery_kabiny.y)/2.0;Y1=Y;X2=X-m.m2px(rozmery_kabiny.y)/2.0;Y2=Y+m.m2px(rozmery_kabiny.x);Xd=X;Yd=Y2;break;
		case 90:X1=X;Y1=Y-m.m2px(rozmery_kabiny.y)/2.0;X2=X+m.m2px(rozmery_kabiny.x);Y2=Y+m.m2px(rozmery_kabiny.y)/2.0;Xd=X2;Yd=Y;break;
	}

	//vykreslení
	canv->Pen->Color=clBlack;
	canv->Pen->Width=1;
	canv->Pen->Style=psDot;//nastevení čarkované čáry
	canv->Pen->Mode=pmNotXor;
	canv->Brush->Style=bsClear;
	//vykreslení kurzoru
	canv->Rectangle(X1,Y1,X2,Y2);
	//spojovací linie
	canv->MoveTo(m.L2Px(V->X),m.L2Py(V->Y));
	canv->LineTo(X,Y);
	if(E->eID==301){X2=m.L2Px(E->X);Y2=m.L2Py(E->Y);}
	else {X2=m.L2Px(E->geo.X1);Y2=m.L2Py(E->geo.Y1);}
	canv->MoveTo(Xd,Yd);
	canv->LineTo(X2,Y2);
	//vykreslení šipek
	sipka(canv,(m.L2Px(V->X)+X)/2,(m.L2Py(V->Y)+Y)/2,azimut,true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
	sipka(canv,(Xd+X2)/2,(Yd+Y2)/2,m.azimut(X2,Y2,Xd,Yd)*(-1),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
	//vykreslení ikony vkládání mezi
	sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,false);
	sipka(canv,(X+Xd)/2.0,(Y+Yd)/2.0,rotace,2,3,clBlack,clWhite,pmNotXor,psSolid,true);

  //uložení rotace pro vkládání objektu
	orientace_objektu=rotace;

	//ukazatelové záležitosti
	V=NULL;delete V;
	E=NULL;delete E;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
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
					dalsi=p->dalsi;
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
//		if(!ret&&p->dalsi2!=NULL&&p->id==(unsigned)F->VyID)//pokud nebyl nalezen další objekt a zároveň se jedná o výhybku, která ma nadefinovanou sekundární větev, prohledá oblast mezi p a p->další2
//		{
//			if((p->X<=p->dalsi2->X && p->Y<p->dalsi2->Y)||(p->X>p->dalsi2->X && p->Y>=p->dalsi2->Y))
//			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y),m.L2Px(p->X)+W,m.L2Py(p->Y)+H,m.L2Px(p->dalsi2->X)+W,m.L2Py(p->dalsi2->Y)+H,m.L2Px(p->dalsi2->X),m.L2Py(p->dalsi2->Y),odecti_region);
//
//			if((p->X>p->dalsi2->X && p->Y<p->dalsi2->Y)||(p->X<=p->dalsi2->X && p->Y>=p->dalsi2->Y))
//			pom=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y)+H,m.L2Px(p->X)+W,m.L2Py(p->Y),m.L2Px(p->dalsi2->X)+W,m.L2Py(p->dalsi2->Y),m.L2Px(p->dalsi2->X),m.L2Py(p->dalsi2->Y)+H,odecti_region);
//
//			if(pom)ret=2;//pokud bylo něco nalezeno jedná se o nález další2, ret = 2
//		}
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
//vykresli pozic a obalových zón
void Cvykresli::vykresli_pozice_a_zony(TCanvas *canv,Cvektory::TElement *E)
{                                                                                                                                                                                                                                                                                                     //oblouk
	if(F->scGPTrackBar_intenzita->Value>5 && F->scGPCheckBox_zobrazit_pozice->Checked && E->data.pocet_pozic>0 || (F->scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked && E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180) || F->scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked && E->geo.typ==1)//pokud se má smysl algoritmem zabývat, pouze optimalizační podmínky
	{
		////výchozí hodnoty
		double orientaceP=E->geo.orientace;//bylo tady Rt90, proč? bylo tady také orientaceP=(E->geo.orientace-180);
		unsigned int pocet_pozic=E->data.pocet_pozic;
		double X=E->geo.X4;
		double Y=E->geo.Y4;
		double dJ=v.PP.delka_jig;
		double sJ=v.PP.sirka_jig; //v případě že se budou zakázkám nastavovat individuální rozměry jiguif(v.ZAKAZKA_akt!=NULL){dJ=v.ZAKAZKA_akt->jig.delka;sJ=v.ZAKAZKA_akt->jig.sirka;}//případně vezme rozměr ze zakázky
		double rotaceJ=v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);//nepůjde rovnou načítat z Elementu??? a metoda by se nemusela užívat?
		short rozmezi=55;//pouze empiricky dodaná hodnota barevného rozpětí od první až po poslední pozici rotace, bylo 40
		unsigned short clPotRGB=180;//hotnota barevných složek dle RGB potenciálních pozic
		TColor clPotencial=(TColor) RGB(clPotRGB,clPotRGB,clPotRGB),clChassis=(TColor) RGB(50,50,50),clJig=clPurple;
		short I=100-F->scGPTrackBar_intenzita->Value;
		if(F->OBJEKT_akt!=NULL && E->objekt_n!=F->OBJEKT_akt->n)//v případě editace změna intezity barev právě needitovaných objektů
		{
			clPotencial=m.clIntensive(clPotencial,I);if(I>5){clPotRGB=255-m.round((100-I)/4);rozmezi=0;}//toto neovlivňovat, je vždy přece stejné clChassis=m.clIntensive(clChassis,I*2);clJig=m.clIntensive(clJig,I*4);//*2,*4 pouze empiricky dodáno
		}

		////určení směru vykreslování pozic
		short x=0,y=0;
		switch(m.Rt90(orientaceP))
		{
			case 0:   y=-1; x=0;  break;
			case 90:  y=0;  x=-1; break;
			case 180: y=1;  x=0;  break;
			case 270: y=0;  x=1;  break;
		}

		////vykreslení ROTACE pozic u otočí a elementů s otočemi
		if(F->scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked && E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180)
		{
			//nastavení parametrů vykreslení různých stupňů rotace
			short krok=30;/*if(fabs(E->rotace_jig)==180)krok=45;*///zobrazení rotace krokem po x stupních (vhodné v násobcích 15,30,45)
			double posun=fabs(E->OTOC_delka/(E->rotace_jig/krok));//krok posunu animace rotace dle délky otoče a proměnné krok
			short Z=1;if(E->rotace_jig<0)Z=-1;//pro záporné rotace jigu
			double aopo=0;if(E->eID==4 || E->eID==6 || E->eID==10 || E->eID==14 || E->eID==18 || E->eID==104 || E->eID==108)aopo=v.PP.uchyt_pozice-(v.PP.delka_podvozek/2.0);//funkční elementy obsahující aktivní otoč posunutí otáčení o uchyt voziku
			double Xr=X+E->OTOC_delka/2.0*x-aopo*x;double Yr=Y+E->OTOC_delka/2.0*y-aopo*y;//začátek vykreslování rotace o posun poloviny délky otoče, *-1 kvůli opačné orientaci
			short clUroven=m.round(rozmezi/(fabs(E->rotace_jig)/krok));//rozmezí odstínu v RGB resp. (clPotRGB+40-clPotRGB)
			DWORD pole[]={m.round(5/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom),m.round(1/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom)};//definice uživatelského pera s vlastní definovanou linii
			//samotné cyklické vykreslení
			for(short i=0;abs(i)<=fabs(E->rotace_jig);i+=Z*krok)
			{
				unsigned short clAkt=clPotRGB+rozmezi-abs(i/krok)*clUroven;
				set_pen2(canv,(TColor)RGB(clAkt,clAkt,clAkt),m.round(1.3/3.0*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
				vykresli_jig(canv,Xr-x*posun*abs(i/krok),Yr-y*posun*abs(i/krok),dJ,sJ,orientaceP,rotaceJ+i,(TColor)RGB(clAkt,clAkt,clAkt),0);//pozn. barvu nastavujeme výše
			}
		}

		////vykreslí OBALOVOU zónu oblouků
		if(F->scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked && E->geo.typ==1)
		{
			double fRA=fabs(E->geo.rotacni_uhel);
			short z=E->geo.rotacni_uhel/fRA;//záznam znaménka + či -
			short pocet=fRA/15.0-1;if(pocet<2)pocet=2;//počet vykreslených vozíků automaticky volen dle velikosti rotačního úhlu
			short clUroven=m.round(rozmezi*pocet+1);//rozmezí odstínu v RGB
			DWORD pole[]={m.round(5/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom),m.round(1/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom)};//definice uživatelského pera s vlastní definovanou linii
			unsigned short clAkt=clPotRGB+rozmezi;
			set_pen2(canv,(TColor)RGB(clAkt,clAkt,clAkt),m.round(1.3/3.0*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
			//vstupní jig - vykresluji separé, kvůli ušetření výpočtu
			vykresli_jig(canv,E->geo.X1,E->geo.Y1,dJ,sJ,orientaceP,rotaceJ,(TColor)RGB(clAkt,clAkt,clAkt),0);//pozn. barvu nastavujeme výše, připadně zde v momentu zobrazování včetně výrobků
			//následující jig(y)
			for(double i=fRA/pocet;i<fRA;i+=fRA/pocet)
			{
				clAkt=clPotRGB+rozmezi-abs(i/fRA/pocet)*clUroven;
				set_pen2(canv,(TColor)RGB(clAkt,clAkt,clAkt),m.round(1.3/3.0*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
				TPointD *geo=m.getArcLine(E->geo.X1,E->geo.Y1,E->geo.orientace,i*z,E->geo.radius);
				vykresli_jig(canv,geo[3].x,geo[3].y,dJ,sJ,orientaceP+i*-z,rotaceJ,(TColor)RGB(clAkt,clAkt,clAkt),0);//pozn. barvu nastavujeme výše, připadně zde v momentu zobrazování včetně výrobků
				delete geo;geo=NULL;
			}
			//poslední jig - vykresluji separé, kvůli ušetření výpočtu
			set_pen2(canv,(TColor)RGB(clPotRGB,clPotRGB,clPotRGB),m.round(1.3/3.0*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
			vykresli_jig(canv,E->geo.X4,E->geo.Y4,dJ,sJ,orientaceP-E->geo.rotacni_uhel,rotaceJ,(TColor)RGB(clAkt,clAkt,clAkt),0);//pozn. barvu nastavujeme výše, připadně zde v momentu zobrazování včetně výrobků
		}

		////vykreslení POZIC na elementu + vzniklém buffru
		if(F->scGPCheckBox_zobrazit_pozice->Checked && v.vrat_druh_elementu(E)==0 && pocet_pozic>0)
		{
			unsigned int pocet_voziku=E->data.pocet_voziku;
			TColor clChassisTemp=m.clIntensive(clPotencial,-30),clJigTemp=m.clIntensive(clPotencial,-70),clPotencialBuffer=m.clIntensive(clPotencial,40);
			//vykreslení jednoho vozíku či pozice, od zadu, aby byly vykresleny nejdříve pozice
			if(pocet_voziku==1 && (m.Rt90(rotaceJ)==0 || m.Rt90(rotaceJ)==180) && v.PP.delka_podvozek<m.UDJ(rotaceJ))vykresli_vozik(canv,0,X,Y,dJ,sJ,orientaceP,rotaceJ,clChassisTemp,clJigTemp);//využitá pozice - když je na stopce jenom jeden vozík a stejně se překrývají jigy nezobrazuje se buffer, jinýmy slovy při této situaci se nepředpokládá, že má smysl zobrazovat buffer (jsou to např. situace ve stopkách v lakování či přímo na robotech)
			else
			{
				for(unsigned int i=pocet_pozic-1;0<i+1;i--)//nutno zápis 0<i+1, jinak zamrzá!!!
				{
					if(i+1>pocet_voziku)vykresli_vozik(canv,/*i+1*/0,X+x*v.PP.delka_podvozek*i,Y+y*v.PP.delka_podvozek*i,dJ,sJ,orientaceP,rotaceJ,m.clIntensive(clPotencialBuffer,-50),clPotencialBuffer,0);//nevyužitá pozice - záměrně šedou jak podvozek tak JIG jako potenicální pozice
					else vykresli_vozik(canv,/*i+1*/0,X+x*v.PP.delka_podvozek*i,Y+y*v.PP.delka_podvozek*i,dJ,sJ,orientaceP,rotaceJ,clChassisTemp,clJigTemp);//využitá pozice - vykresluje se tato větev pouze pro případ skrytí zobrazení vozíků či posun vozíků při simulaci, jinak by neměla význam, protože by na těchto pozicích měly být vozíky
				}  					//tuto větev nutné také duplicitně použít do tvorby vozíků!!!!!!!!!!!!!
			}
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí všechny vozíky ze seznamu vozíků
void Cvykresli::vykresli_voziky(TCanvas *canv)
{
	////test simulace
//	Cvektory::TPohon *P=v.POHONY->dalsi;
//	while(P!=NULL)
//	{
//		//v.vytvor_retez(P);//a palce - aktuálně generuje na MaKr tlačítku
//		//vykresli_retez(canv,P->retez);//spíše pouzdrou řetězu
//		vykresli_palce(canv,P);
//		P=P->dalsi;
//	}
//	delete P;
	//test2 simulace
	//set_pen2(canv,clRed,m.round(1*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true);
	//line(canv,0,500,0+F->sTIME,500);
	////----
	TColor clChassisTemp=clChassis;//záloha
	if(v.VOZIKY!=NULL && F->scGPCheckBox_rozmisteni_voziku->Checked)
	{
		Cvektory::TVozik *V=v.VOZIKY->dalsi;
		while(V!=NULL)
		{
			TColor clJigTemp=clJig;if(V->zakazka->n!=0)clJigTemp=V->zakazka->barva;//mimo default zakázky JIGy nesou barvy dané zakázky
			//F->Memo(String(V->n)+" "+String(V->X)+" "+String(V->Y));//pro ladění
			//narazil do vozíku před ním vyhákává či již je po nárazu vyháknutý -2, vyháknutý -1, čeká na palec 0, jede 1
			switch(V->stav)
			{
				case -2: clChassis=clRed;break;
				case -1: clChassis=clPasiv;break;
				case 0: clChassis=clWebOrange;break;
				case 1: clChassis=clGreen;break;
			}

			if(F->MOD==F->EDITACE && F->OBJEKT_akt!=NULL && V->element!=NULL && V->element->objekt_n!=F->OBJEKT_akt->n)//při editaci vozíky v pasivních objektech
				vykresli_vozik(canv,V->n,V->X,V->Y,v.PP.delka_jig,v.PP.sirka_jig,V->orientace_podvozek,V->rotace_jig/*připadně později dle zakázky V->Zakazka->Jig...*/,m.clIntensive(clChassis,m.get_intensity()),m.clIntensive(clJigTemp,m.get_intensity()));
			else//všechny ostatní situace
				vykresli_vozik(canv,V->n,V->X,V->Y,v.PP.delka_jig,v.PP.sirka_jig,V->orientace_podvozek,V->rotace_jig/*připadně později dle zakázky V->Zakazka->Jig...*/,clChassis,clJigTemp);
			V=V->dalsi;
		}
		delete V;
	}
	clChassis=clChassisTemp;//navrácení zpět
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslení jednoho komplexního vozíku (podvozek včetně jigu), X,Y jsou souřadnice uchycení vozíku k palci, což nemusí být střed vozíku
void Cvykresli::vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dJ,double sJ,double orientaceP,double rotaceJ,TColor clChassis, TColor clJig,float Width)
{
	//výchozí parametry
	double Xp=X-v.PP.uchyt_pozice;//posunutí umístění vozíku o nastavení uchycení pozice
	orientaceP=m.a360(orientaceP-180);//nově přídáno, vozík je při konstukci totiž vykreslování vykresklován obráceně

	//transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Brush->Style=bsClear;

	////podvozek
	set_pen2(canv,clChassis,m.round(1/3.0*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true);
	TPointD C=obdelnik(canv,Xp,Y+v.PP.sirka_podvozek/2.0,Xp+v.PP.delka_podvozek,Y-v.PP.sirka_podvozek/2.0,m.o2r(orientaceP),X,Y);

	////jig
	vykresli_jig(canv,C.x,C.y,dJ,sJ,orientaceP,rotaceJ,clJig,Width);

	////text - ID vozíku není vypisováno, pokud by se začlo používat, tak pozor u vykreslení pozic by bylo potřeba nastavit separátně písmo u chybových výpisů
	if(ID>0){canv->Font->Size=m.round(2*F->Zoom);canv->Font->Color=clJig;SetBkMode(canv->Handle,TRANSPARENT/*OPAQUE*/);TextFraming(canv,m.L2Px(X),m.L2Py(Y),ID);}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_jig(TCanvas *canv,double X,double Y,double dJ,double sJ,double orientaceP,double rotaceJ,TColor clJig,float Width)
{
	////transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Brush->Style=bsClear;

	////jig
	if(dJ!=0 && sJ!=0)//vykreslí, pouze pokud jsou oba parametry nenulové
	{              //situace volám buď rovnou vykresleni jigu nebo přes vozík včetně jigu, včetně definice pera předem
		bool vyrobky=false; if(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex>0)vyrobky=true;

		//styl pero
		if(Width>0)set_pen2(canv,clJig,m.round(Width/3.0*F->Zoom),PS_ENDCAP_ROUND,PS_JOIN_ROUND,true);//normální jig
		if(Width==0 && vyrobky && F->MOD!=F->EDITACE && F->Zoom>4*3)//pozice, zony oblouky a otoče při zobrazení výrobků
		{
			//DWORD pole[]={m.round(5/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom),m.round(1/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom)};//definice uživatelského pera s vlastní definovanou linii
			//set_pen2(canv,clJig,2,PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
			set_pen2(canv,clJig,2*2,PS_ENDCAP_SQUARE,PS_JOIN_MITER,true);
    }

		//zobrazení
		if(!vyrobky || F->MOD==F->EDITACE || Width==0 || F->Zoom<=4*3)obdelnik(canv,X-dJ/2.0,Y+sJ/2.0,X+dJ/2.0,Y-sJ/2.0,m.o2r(orientaceP)+rotaceJ);//rám, pro pozice či zóny nebo když není detailní zoom, nebo je editace
		else vykresli_vyrobky(canv,X,Y,dJ,sJ,orientaceP,rotaceJ,clJig,Width);//při detailním zoomu vykreslí i výrobky
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_vyrobky(TCanvas *canv,double X,double Y,double dJ,double sJ,double orientaceP,double rotaceJ,TColor clJig,float Width)
{
	//vykreslení jigu - tyče, rámu, kde jsou navěšené výrobky
	set_pen2(canv,m.clIntensive(clChassis,30),m.round(Width/3.0*F->Zoom),PS_ENDCAP_FLAT,PS_JOIN_MITER,true);
	TPointD P0=m.rotace(v.PP.delka_jig/2.0,0,m.o2r(orientaceP-90)+rotaceJ);
	line(canv,m.L2Px(X+P0.x),m.L2Py(Y+P0.y),m.L2Px(X-P0.x),m.L2Py(Y-P0.y));//obdelnik(canv,X-dJ/2.0,Y,X+dJ/2.0,Y,m.o2r(orientaceP)+rotaceJ);

	//vykreslení ochranné zóny okolo jigu - nepoužito
	if(Form_parametry_linky->scCheckBox_vyber_produkt->Checked)
	{
		//DWORD pole[]={m.round(5/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom),m.round(1/3.0*F->Zoom),m.round(2.5/3.0*F->Zoom)};//definice uživatelského pera s vlastní definovanou linii
		//set_pen2(canv,clRed,2,PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,pole,sizeof(pole)/sizeof(pole[0]));
		set_pen2(canv,clJig,2,PS_ENDCAP_SQUARE,PS_JOIN_MITER,true);
		obdelnik(canv,X-dJ/2.0,Y+sJ/2.0,X+dJ/2.0,Y-sJ/2.0,m.o2r(orientaceP)+rotaceJ);
	}

	//vykreslení výrobků
	double Z=3.5;//faktor zvětšení
	double H=0.085*Z;//výška výrobku (přibližná-naměřená měřidlem 0.0834, ale raději včetně rezervy 0.085 kvůli krajům)
	double W=0.08*Z;//šířka výrobku (přibližná-naměřená měřidlem 0.07753, ale raději včetně rezervy 0.08 kvůli krajům)
	double M=-W/2/3;if(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex==2)M=-W/2/3*1.8;//mezera mezi výrobky,poměrově vůči výrobku
	double iKrok=(W+M);//výrobek + mezera
	double iMax=(v.PP.delka_jig-W)/iKrok;//počet úseků
	TPointD P1=m.rotace(v.PP.delka_jig/2.0+M,0,m.o2r(orientaceP-90)+rotaceJ);//posun od vertikálního středu nalevo - kvůli excentritě
	TPointD P2=m.rotace(v.PP.delka_jig/2.0-M,0,m.o2r(orientaceP-90)+rotaceJ);//posun od vertikálního středu napravo - kvůli excentritě
	TPointD O=m.rotace(-H/2.0,0,m.o2r(orientaceP)+rotaceJ);if(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex==2)O=m.rotace(0,0,m.o2r(orientaceP)+rotaceJ);//odsazení od horizontálního středu, polovina výšky výrobku, zajistí vycentrování
	double xKrok=(-P1.x-P2.x)/iMax;//krok rozvěšení výrobků na X ose
	double yKrok=(-P1.y-P2.y)/iMax;//krok rozvěšení výrobků na Y ose
	for(unsigned int i=0;i<=floor(iMax);i++)
	{
		vykresli_vyrobek(canv,X+P1.x+O.x+xKrok*i,Y+P1.y+O.y+yKrok*i,Z,m.o2r(orientaceP)+rotaceJ,clJig/*TColor RGB(3,192,250)*/,-0.02);//jedna řada
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_vyrobek(TCanvas *canv,double X,double Y,double Z,double rotace,TColor color,double prohloubeni)//-prohloubení,+vypouknutí vykreslovaného zrcátka
{
	////ZRCÁTKA
	if(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex==1)
	{
		//definice tvaru
		TPointD *PL=new TPointD[7];
		PL[0].x=0.0962*Z;PL[0].y=-0.0508*Z;PL[1].x=0.228*Z;PL[1].y=-0.0088*Z;PL[2].x=0.1864*Z;PL[2].y=-0.1026*Z;PL[3].x=0.0966*Z;PL[3].y=-0.1176*Z;
		PL[4].x=(PL[0].x+PL[3].x)/2.0-prohloubeni*Z;PL[4].y=(PL[0].y+PL[3].y)/2.0;
		PL[5].x=PL[4].x;PL[5].y=PL[4].y;PL[6].x=PL[0].x;PL[6].y=PL[0].y;

		//vykreslení
		set_pen(canv,color,2*Z,PS_ENDCAP_FLAT);
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=color;
		BeginPath(canv->Handle);
		bezier(canv,PL,m.L2Px(X),m.L2Py(Y),PL[0].x,PL[0].y,rotace-90,6);//pozor, použitý polygon přetočí, tzn. při dalším použití polygonu nutno docílit stejného +180°, např. při použití obrysu
		EndPath(canv->Handle);
		FillPath(canv->Handle);

		//smazání nepotřebného dynamického objeku
		delete[]PL;PL=NULL;
	}

	////ČTVERCE
	if(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex==2)
	{
		//vykreslení
		double W=0.1;
		set_pen(canv,color,2*Z,PS_ENDCAP_FLAT);
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=color;
		BeginPath(canv->Handle);
		obdelnik(canv,X-W,Y-W,X+W,Y+W,rotace-90,X,Y);
		EndPath(canv->Handle);
		FillPath(canv->Handle);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//kreslí koleje, pouzdro řetězu, řetěz samotný, předávací místo i popisek pohonu, slouží i zároveň na náhled cest zakázek
void Cvykresli::vykresli_dopravnik(TCanvas *canv, Cvektory::TZakazka *zakazka)
{
	TPoint *POLE=new TPoint[4];
	bool zmena=true;
	TPoint zacatek;zacatek=TPoint(0,0);//detekce začátečního bodu popisku pohonu
	unsigned int pocet_pruchodu=0;//predchozi_pocet_pruchodu=0;

	Cvektory::TElement *E=v.ELEMENTY->dalsi;
	while(E!=NULL)
	{
		////vstupní proměnné
		//musí být uvnitř cyklu pro nové nastavení
		TColor clKolej=(TColor) RGB(255,69,0); if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n && F->Akce!=F->Takce::POSUN_TEPLOMER)clKolej=m.clIntensive(clKolej,m.get_intensity()/1.8);//zesvětlování neaktivních pohonů
		TColor clRetez=clBlack; if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n && F->Akce!=F->Takce::POSUN_TEPLOMER)clRetez=m.clIntensive(clRetez,m.get_intensity());//zesvětlování neaktivních pohonů
		if(F->Akce==F->Takce::POSUN_TEPLOMER && !v.obsahuje_segment_cesty_element(E,v.ZAKAZKA_akt)){clRetez=m.clIntensive(clRetez,m.get_intensity());clKolej=m.clIntensive(clKolej,m.get_intensity()/1.8);}//zesvětlení neaktivních věcí
		float RetezWidth=1;if(E->pohon!=NULL)RetezWidth=F->Zoom*0.5;//pokud není pohon přiřazen, tak jen elementární osa, jinak skutečná tloušťka

		////vykreslení paralelních kolejí
		if(zakazka==NULL && F->MOD!=F->TVORBA_CESTY)
		{
			if(E->predchozi!=NULL && E->predchozi->pohon!=NULL && E->predchozi->eID==200)vykresli_koleje(canv,E->predchozi);//pouze grafická korekce předchozího segmentu, pokud byl překryt předávacím místem
			if(E->pohon!=NULL)vykresli_koleje(canv,E);//pouze pokud je přiřazen pohon
		}

		////vykreslení segementu pohonu
		//plnění geo souřadnic do pole
		POLE[0]=TPoint(m.L2Px(E->geo.X1),m.L2Py(E->geo.Y1));
		POLE[1]=TPoint(m.L2Px(E->geo.X2),m.L2Py(E->geo.Y2));
		POLE[2]=TPoint(m.L2Px(E->geo.X3),m.L2Py(E->geo.Y3));
		POLE[3]=TPoint(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
		//vykreslení pouzdra řetězu
		if(zakazka==NULL && F->MOD!=F->TVORBA_CESTY && E->pohon!=NULL && (F->OBJEKT_akt==NULL || F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==E->objekt_n || F->scGPTrackBar_intenzita->Value>25 && F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n))//při editaci zobrazí pasivní jen s intenzitou větší než 25
		{
			set_pen(canv,clKolej,m.round(RetezWidth*2),PS_ENDCAP_FLAT);
			canv->PolyBezier(POLE,3);
		}
		//nastavení pera
		if(F->MOD==F->TVORBA_CESTY && zakazka==NULL)
		{
			pocet_pruchodu=v.kolikrat_obsahuje_segment_cesty_element(E,Form_definice_zakazek->Z_cesta);
			if(pocet_pruchodu>0)
			{
				RetezWidth=v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom)+2*m.px2m(m.round(F->Zoom));
				RetezWidth=m.m2px(RetezWidth/2.0);
			}
			else clRetez=(TColor)RGB(200, 200, 200);
		}
		if(zakazka!=NULL)
		{
			pocet_pruchodu=v.kolikrat_obsahuje_segment_cesty_element(E,zakazka);
			if(pocet_pruchodu==0)clRetez=(TColor)RGB(200, 200, 200);
		}
		set_pen(canv,clRetez,m.round(RetezWidth),PS_ENDCAP_SQUARE);
		//vykreslení řetězu
		canv->PolyBezier(POLE,3);

		////vykreslení předávacího místa
		if(E->eID==200 && F->MOD!=F->TVORBA_CESTY && zakazka==NULL)
		{
      RetezWidth=1;if(E->pohon!=NULL)RetezWidth=F->Zoom*0.5;//nutné znovu počítat width, nelze použít upravený v případě zakázky
			//výpočty souřadnic
			TPointD V=m.rotace(m.px2m(RetezWidth*4),180-E->geo.orientace,0);//vyosení, mezera mezi pohony v předávacím místě

			//zapouzdření řetezu
			set_pen(canv,clKolej,m.round(RetezWidth*2),PS_ENDCAP_FLAT);
			bezier(canv,m.getArcLine(E->geo.X4+V.x,E->geo.Y4+V.y,E->geo.orientace+180,45,v.PP.radius/1.3),3);//framing resp. zapouzdření
			bezier(canv,m.getArcLine(E->geo.X4-V.x,E->geo.Y4-V.y,E->geo.orientace,90,v.PP.radius/3),3);//framing resp. zapouzdření

			//řetez
			set_pen(canv,clRetez,m.round(RetezWidth),PS_ENDCAP_SQUARE);
			bezier(canv,m.getArcLine(E->geo.X4+V.x,E->geo.Y4+V.y,E->geo.orientace+180,45,v.PP.radius/1.3),3);//nového pohonu
			bezier(canv,m.getArcLine(E->geo.X4-V.x,E->geo.Y4-V.y,E->geo.orientace,90,v.PP.radius/3),3);//starého pohonu
//			TPointD *PL=m.getArcLine(E->geo.X4-V.x,E->geo.Y4-V.y,E->geo.orientace,90,v.PP.radius/3);bezier(canv,PL,3);//starého pohonu
//			delete PL;PL=NULL;
		}

		/////vykreslování popisku pohonu
		if(F->scGPCheckBox_popisek_pohonu->Checked && F->MOD!=F->TVORBA_CESTY && zakazka==NULL && E->pohon!=NULL && E->geo.typ==0 && E->geo.orientace==m.Rt90(E->geo.orientace))//vykreslení vodoznaku pohonu
		{
			if(zacatek.x==0 && zacatek.y==0)zacatek=m.L2P(E->geo.X1,E->geo.Y1);
			if(E->dalsi==NULL || E->eID==300 || E->eID==301 || (E->dalsi!=NULL && (E->dalsi->pohon!=E->pohon || (E->dalsi->geo.X1!=E->geo.X4 || E->dalsi->geo.Y1!=E->geo.Y4) || E->dalsi->geo.typ!=0 || (m.delka(m.P2Lx(zacatek.x),m.P2Ly(zacatek.y),E->geo.X4,E->geo.Y4)>=9.9 && E->dalsi->objekt_n!=E->objekt_n) || (E->dalsi->geo.typ==0 && E->geo.orientace!=E->dalsi->geo.orientace))))
			{
				//nastavení fontu pro zjištění velikosti textu
				canv->Font->Size=m.round(2.8*F->Zoom);if(F->aFont->Size==12)canv->Font->Size=m.round(2*F->Zoom);//stejné nastavení jako při vykreslení PM
				canv->Font->Name="Roboto Lt";
				//kontrola zda se popisek nad oblast vleze
				if(m.delka(m.P2Lx(zacatek.x),m.P2Ly(zacatek.y),E->geo.X4,E->geo.Y4)>m.px2m(canv->TextWidth(E->pohon->name)))
				{vykresli_popisek_pohonu(canv,E->pohon->name,zacatek,m.L2P(E->geo.X4,E->geo.Y4),E->geo.orientace,zmena);zmena=!zmena;}
				zacatek=TPoint(0,0);
			}
		}
		else zacatek=TPoint(0,0);//nulování začátečního bodu pokud není podmínka splněná

		////vykreslování mnohočetného průchodu cesty v náhledu cest zakázek
		if(pocet_pruchodu>1)
		{
			TColor barva;
			double o=0,DR2=0;
			short z=1;
			TPointD S2,*PL2;
			for(unsigned int i=2;i<=pocet_pruchodu;i++)
			{
				o=(i-1)*m.px2m(m.round(RetezWidth/2.0));
				//nastavení pera
				barva=m.getColorOfPalette(i-1);
				set_pen(canv,barva,m.round(RetezWidth/2.0),PS_ENDCAP_SQUARE);
	  		//výpočet odsazení a souřadnic
//				TPointD S1=m.rotace(o,180-E->geo.orientace,90);
				z=1;if(E->geo.rotacni_uhel>0)z*=-1;
//				double DR1=E->geo.delka;if(E->geo.typ==1)DR1=E->geo.radius+o*z;//delka či radius
//				TPointD *PL1=m.getArcLine(E->geo.X1+S1.x,E->geo.Y1+S1.y,E->geo.orientace,E->geo.rotacni_uhel,DR1);
				S2=m.rotace(o,180-E->geo.orientace,-90);
		  	DR2=E->geo.delka;if(E->geo.typ==1)DR2=E->geo.radius+o*z*-1;//delka či radius
				PL2=m.getArcLine(E->geo.X1+S2.x,E->geo.Y1+S2.y,E->geo.orientace,E->geo.rotacni_uhel,DR2);
				//samotné výkreslení obou parelelních kolejí
//				bezier(canv,PL1,3);
				bezier(canv,PL2,3);
			}
		}
		//predchozi_pocet_pruchodu=pocet_pruchodu;
		/////konec testů

		////ukazatelové záležitosti
		E=v.dalsi_krok(E);//posun na další element nahrazuje při použití výhybek dřívější E=E->dalsi;
	}
	delete E;E=NULL;//smazání již nepotřebného ukazatele
	delete[]POLE;POLE=NULL;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//samotné vykreslení jednoho řetězu
void Cvykresli::vykresli_retez(TCanvas *canv,Cvektory::TRetez *Retez)
{
	if(Retez!=NULL && Retez->predchozi->n>0)
	{
		//TColor clRetez=clBlack; if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n)clRetez=m.clIntensive(clRetez,m.get_intensity());//zesvětlování neaktivních pohonů
		//float RetezWidth=1;if(E->pohon!=NULL)RetezWidth=F->Zoom*0.5;//pokud není pohon přiřazen, tak jen elementární osa, jinak skutečná tloušťka
//		//vykreslení pouzdra řetězu
//		if(E->pohon!=NULL && (F->OBJEKT_akt==NULL || F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==O->n || F->scGPTrackBar_intenzita->Value>25 && F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=O->n))//při editaci zobrazí pasivní jen s intenzitou větší než 25
//		{
//			set_pen(canv,clKolej,m.round(RetezWidth*2),PS_ENDCAP_FLAT);
//			canv->PolyBezier(POLE,3);
//		}
//bude to chtít ukládat stav řetezu dle OBJEKT_akt, a potom dořešit elementární osy s geometrii, typicky po zadané geometrii a odebraném pohonu

		TPoint POLE[4];
		TColor clRetez=clBlue;
		//if((int)m.RAND(1,2)%2)clRetez=clGreen;
		set_pen(canv,clRetez,F->Zoom*0.5*2);//nastavení pera
		Cvektory::TRetez *R=Retez->dalsi;
		while(R!=NULL)
		{
			//plnění jednoho segmentu do pole
			POLE[0]=TPoint(m.L2Px(R->geo.X1),m.L2Py(R->geo.Y1));
			POLE[1]=TPoint(m.L2Px(R->geo.X2),m.L2Py(R->geo.Y2));
			POLE[2]=TPoint(m.L2Px(R->geo.X3),m.L2Py(R->geo.Y3));
			POLE[3]=TPoint(m.L2Px(R->geo.X4),m.L2Py(R->geo.Y4));
			//vykreslení jednoho segmentu - musí se bohužel vykreslovat postupně kvůli výhybkám atd.
			canv->PolyBezier(POLE,3);
			//ukazatelové záležitosti
			R=R->dalsi;//posun na další element
		}
		//smazání již nepotřebného ukazatele
		delete R;R=NULL;

		//smazání již nepotřebného ukazatele
		delete R;R=NULL;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslení jednoho geometrického segmentu dvou párů kolejí
void Cvykresli::vykresli_koleje(TCanvas *canv,Cvektory::TElement *E)
{
	if(F->OBJEKT_akt==NULL || F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n==E->objekt_n || F->scGPTrackBar_intenzita->Value>25 && F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n)//při editaci zobrazí pasivní jen s intenzitou větší než 25
	{
		////nastavení barev
		TColor clKolej=(TColor) RGB(255,69,0);
		if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n && F->Akce!=F->Takce::POSUN_TEPLOMER)clKolej=m.clIntensive(clKolej,m.round(m.get_intensity()/1.8));//zesvětlování neaktivních pohonů
		if(F->Akce==F->Takce::POSUN_TEPLOMER && !v.obsahuje_segment_cesty_element(E,v.ZAKAZKA_akt))clKolej=m.clIntensive(clKolej,m.round(m.get_intensity()/1.8));//zesvětlení neaktivních větví
		////řešení pokud je stoupání či klesání
		double delka=E->geo.delka;
		if(E->geo.HeightDepp!=0)//výšku či hloubku
		{
			delka=E->geo.delkaPud;//délka půdorysní
			vykresli_stoupani_klesani(canv,E,0.3);
		}
		////samotné volání vykreslení kolejí
		vykresli_koleje(canv,E->geo.X1,E->geo.Y1,E->geo.typ,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,delka,clKolej);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslení jednoho geometrického segmentu dvou párů kolejí,TypZarazky=0 bez (implicitně-default),1=na začátku,2=na konci,3=na začátku i na konci, barva colorZarazka nastaví pro případně zobrazovanou zarážku speciální barvu, pokud není parametr použit (je NULL), tak dostane případná zarážka stejnou barvu jako barva parametru color
void Cvykresli::vykresli_koleje(TCanvas *canv,double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka,TColor clKolej,short TypZarazky,TColor colorZarazka)
{
	//offset o poloviny nastavené šířky podvozku + tloušťka linie zakresu podvozku
	double o=v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom);
	//nastavení pera
	float width=0.5; //if(clKolej==clMeridlo){width*=3;canv->Pen->Mode=pmNotXor;}
	set_pen(canv,clKolej,m.round(F->Zoom*width),PS_ENDCAP_SQUARE);
	//výpočet odsazení a souřadnic
	TPointD S1=m.rotace(o,180-orientace,90);
	TPointD S2=m.rotace(o,180-orientace,-90);
	short z=1;if(rotacni_uhel>0)z*=-1;
	double DR1=delka;if(typ==1)DR1=radius+o*z;//delka (linie) či radius (oblouk)
	double DR2=delka;if(typ==1)DR2=radius+o*z*-1;//delka (linie) či radius (oblouk)
	TPointD *PL1=m.getArcLine(X+S1.x,Y+S1.y,orientace,rotacni_uhel,DR1);
	TPointD *PL2=m.getArcLine(X+S2.x,Y+S2.y,orientace,rotacni_uhel,DR2);
	//samotné výkreslení obou parelelních kolejí
	if(F->scGPCheckBox_zobrazit_koleje->Checked)//pokud je v menu povoleno
	{
		bezier(canv,PL1,3);
		bezier(canv,PL2,3);
	}
	//zarážka se zobrazuje pouze při geometrii (pro znázornění jednotlivých gemoetrických elementů), jinak nemá význam
	if(colorZarazka!=NULL && 1<=TypZarazky && TypZarazky<=3)set_pen(canv,colorZarazka,m.round(F->Zoom*width),PS_ENDCAP_SQUARE);
	if(TypZarazky==1 || TypZarazky==3 || F->Akce==F->GEOMETRIE || F->Akce==F->GEOMETRIE_LIGHT)line(canv,m.L2Px(X+S1.x),m.L2Py(Y+S1.y),m.L2Px(X+S2.x),m.L2Py(Y+S2.y));//na začátku
	if(TypZarazky==2 || TypZarazky==3 || F->Akce==F->GEOMETRIE || F->Akce==F->GEOMETRIE_LIGHT)line(canv,m.L2Px(PL1[3].x),m.L2Py(PL1[3].y),m.L2Px(PL2[3].x),m.L2Py(PL2[3].y));//na konci

	delete PL1;PL1=NULL;delete PL2;PL2=NULL;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vytvoření jednoho geometrického segmentu z dvou párů kolejí určeného k testování, zda se nachazí v dané oblasti bod, podružná metoda, volaná z matematické knihovny
//nelze využívat výše uvedenou metodu, oblast musí být uzavřená
void Cvykresli::vytvor_oblast_koleje(TCanvas *canv,double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka)
{
	//offset o poloviny nastavené šířky podvozku + tloušťka linie zakresu podvozku
	double o=v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom);
	//nastavení pera
	//není třeba set_pen(canv,clKolej,m.round(F->Zoom*0.5),PS_ENDCAP_SQUARE);
	//výpočet odsazení a souřadnic
	TPointD S1=m.rotace(o,180-orientace,90);
	TPointD S2=m.rotace(o,180-orientace,-90);
	short z=1;if(rotacni_uhel>0)z*=-1;
	double DR1=delka;if(typ==1)DR1=radius+o*z;//delka či radius
	double DR2=delka;if(typ==1)DR2=radius+o*z*-1;//delka či radius
	TPointD *PL1=m.getArcLine(X+S1.x,Y+S1.y,orientace,rotacni_uhel,DR1);
	TPointD *PL2=m.getArcLine(X+S2.x,Y+S2.y,orientace,rotacni_uhel,DR2);
	TPointD *PL=new TPointD[13];
	//kolej tam
	PL[0]=PL1[0];
	PL[1]=PL1[1];
	PL[2]=PL1[2];
	PL[3]=PL1[3];
	//spojinice
	PL[4].x=PL1[3].x;PL[4].y=PL1[3].y;
	PL[5].x=PL2[3].x;PL[5].y=PL1[3].y;
	PL[6]=PL2[3];
	//kolej zpet
	PL[7]=PL2[2];
	PL[8]=PL2[1];
	PL[9]=PL2[0];
	//spojinice
	PL[10]=PL2[0];
	PL[11]=PL1[0];
	PL[12]=PL1[0];
	//vytvoření do canvasu
	bezier(canv,PL,13-1);
	//odstranění
	delete PL1;PL1=NULL;delete PL2;PL2=NULL;delete PL;PL=NULL;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslení palců
void Cvykresli::vykresli_palce(TCanvas *canv,Cvektory::TPohon *pohon)
{
	if(pohon!=NULL && pohon->palec!=NULL)
	{  //F->Memo(pohon->name);
		Cvektory::TPalec *P=pohon->palec->dalsi;
		while(P!=NULL)
		{
			//F->Memo(String(P->X)+" "+String(P->Y));
			if(P->X!=DOUBLE_MIN && P->Y!=DOUBLE_MIN && P->orientace!=DOUBLE_MIN)//pouze pokud se nejedná o nezneplatněný palec, který nemá být vidět (DOUBLE_MIN - jedná se o "poznávací znamení")
			vykresli_palec(canv,P);
			P=P->dalsi;
		}
		delete P;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje samotné vykreslení palce
void Cvykresli::vykresli_palec(TCanvas *canv,Cvektory::TPalec *P)
{
	double s=0.025;//0.05;//0.01;
	canv->Pen->Color=clRed;if(P->n%2)canv->Pen->Color=clBlue;
	canv->Pen->Width=m.round(0.1*F->Zoom);
	//canv->Ellipse(m.L2Px(P->X-s),m.L2Py(P->Y+s),m.L2Px(P->X+s),m.L2Py(P->Y-s));
	obdelnik(canv,P->X-s/1.5,P->Y+s,P->X+s/1.5,P->Y-s,m.o2r(P->orientace),P->X,P->Y);//provizorně kvůli testování
	canv->Font->Size=m.round(0.75*F->Zoom);
	canv->Font->Color=canv->Pen->Color;
	TextOut(canv,m.L2Px(P->X),m.L2Py(P->Y),P->n,CENTER,TOP);//pouze pro testy
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí popisek pohonu ve středu zadané úsečky, parametr pozice zajišťuje střídání pozice popisku
void Cvykresli::vykresli_popisek_pohonu(TCanvas *canv,AnsiString text,TPoint zacatek,TPoint konec,short trend,bool pozice)
{
	////obecné nastavení
	TColor barva=clPasiv;
	int delka_sipky;

	////nastavení fontu
	canv->Font->Size=m.round(2.8*F->Zoom);if(F->aFont->Size==12)canv->Font->Size=m.round(2*F->Zoom);//stejné nastavení jako při vykreslení PM
	canv->Font->Name="Roboto Lt";
	canv->Font->Color=clPasiv;
	canv->Font->Style=TFontStyles();

	////nastavení odsazení a rotace fontu
	short W=canv->TextWidth(text),H=canv->TextHeight(text);//odsazení textu od středu
	delka_sipky=W;//nastavení délky šipky na délku textu
	short odsazeniX=W,odsazeniY=H;//default hodnoty
	short poziceX=(zacatek.x+konec.x)/2.0,poziceY=(zacatek.y+konec.y)/2.0;//uchovává střed, kde se bude vykreslovat text
	int obratit=1;if(!pozice)obratit=-1;//proměná, která obrací odsazení
	double odsazeni=1.2;if(obratit>0)odsazeni=1.1;//pomocná proměnná pro posun šipky od pohonu
	switch(trend)
	{
		case 0:
		{
			//nastavení odsazení pro text
			poziceY+=m.round(0.5*1.1*delka_sipky/4.0);//centrování podlě šipky, ne podle textu
			odsazeniX=H;odsazeniY=W;
			canv->Font->Orientation=900;
			odsazeniY=-odsazeniY/2.0;if(obratit>0)odsazeniX*=2.14;//jednou je třeba připočítat výšku textu
			odsazeniX*=obratit;//převrácení vodoznaku
			//nastavení bodů pro šipku
			zacatek=TPoint(poziceX-odsazeniX+m.round(H*odsazeni)*pozice,poziceY-m.round(delka_sipky/2.0));//počáteční bod
			konec=TPoint(poziceX-odsazeniX+m.round(H*odsazeni)*pozice,poziceY+m.round(delka_sipky/2.0));//koncový bod
			break;
		}
		case 180:
		{
			//nastavení odsazení pro text
			poziceY-=m.round(0.5*1.1*delka_sipky/4.0);//centrování podlě šipky, ne podle textu
			obratit*=-1;//nutno otočit, orientace 180 má více odlišností
			odsazeniX=H;odsazeniY=W;
			canv->Font->Orientation=2700;
			odsazeniY=+odsazeniY/2.0;if(obratit<0)odsazeniX*=2.14;//jednou je třeba připočítat výšku textu
			odsazeniX*=obratit;//převrácení vodoznaku
			//nastavení bodů pro šipku
			zacatek=TPoint(poziceX-odsazeniX-m.round(H*odsazeni)*pozice,poziceY-m.round(delka_sipky/2.0));//počáteční bod
			konec=TPoint(poziceX-odsazeniX-m.round(H*odsazeni)*pozice,poziceY+m.round(delka_sipky/2.0));//koncový bod
			obratit*=-1;//navrácení do původního stavu
			break;
		}
		case 90:case 270:
		{
			//nastavení odsazení pro text
			poziceX-=m.round(0.5*1.1*delka_sipky/4.0);//centrování podlě šipky, ne podle textu
			odsazeniX/=2.0;if(obratit>0)odsazeniY*=2.14;//jednou je třeba připočítat výšku textu
			odsazeniY*=obratit;//převrácení vodoznaku
			//nastavení bodů pro šipku
			zacatek=TPoint(poziceX-m.round(delka_sipky/2.0),poziceY-odsazeniY+m.round(H*odsazeni)*pozice);//počáteční bod
			konec=TPoint(poziceX+m.round(delka_sipky/2.0),poziceY-odsazeniY+m.round(H*odsazeni)*pozice);//koncový bod
			break;
    }
	}

	////vykreslení názvu
	TextFraming(canv,poziceX-odsazeniX,poziceY-odsazeniY,text);

	////vykreslení šipky u pohonu
	//nastavení grafického pera
	canv->Pen->Style=bsSolid;
	canv->Pen->Width=m.round(F->Zoom/5.0);
	canv->Pen->Color=barva;
	obratit=1;

	//vykreslení čáry šipky
	if(trend==90 || trend==270)
	{
		if(trend==90)konec.x+=m.round(1.1*delka_sipky/4.0);
		else zacatek.x-=m.round(1.1*delka_sipky/4.0);
	}
	else
	{
		if(trend==180)konec.y+=m.round(1.1*delka_sipky/4.0);
		else zacatek.y-=m.round(1.1*delka_sipky/4.0);
  }
	line(canv,zacatek.x,zacatek.y,konec.x,konec.y);

	//zajištění prohození začátku a konce pro opačné trnedy
	if(trend==270 || trend==0){konec=zacatek;obratit=-1;}
	//vykreslení skosených čar pro šipku
	if(trend==90 || trend==270)
	{
		line(canv,konec.x,konec.y,konec.x-m.round(delka_sipky/4.0)*obratit,konec.y+m.round(delka_sipky/17.0));
		line(canv,konec.x,konec.y,konec.x-m.round(delka_sipky/4.0)*obratit,konec.y-m.round(delka_sipky/17.0));
	}
	else
	{
		line(canv,konec.x,konec.y,konec.x+m.round(delka_sipky/17.0),konec.y-m.round(delka_sipky/4.0)*obratit);
		line(canv,konec.x,konec.y,konec.x-m.round(delka_sipky/17.0),konec.y-m.round(delka_sipky/4.0)*obratit);
	}

	////navrácení rotace canv, důležité pro správnost ostatního vykreslení!!!!!
	canv->Font->Orientation=0;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//celková vykreslovací metoda, vykreslí buď stopku, robota nebo otoč
void Cvykresli::vykresli_element(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,unsigned int eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2,double LO_pozice,Cvektory::TElement *E)
{ //scena 0 - vše do dynamické, scena 1 - implicitně statické elementy do statické scény, scena 2 - implicitně statické elementy do dynamické scény, scena 3 - implicitně dynamické elementy do statické scény, scena 4 - implicitně dynamické elementy do dynamické scény
	rotace=m.Rt90(rotace);
	switch(eID)
	{
		case 0:  if(scena==0 || scena>2)vykresli_stopku(canv,X,Y,name,short_name,typ,rotace,stav);break;//stopka
		case 1:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 2:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 3:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 4:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 5:  if(scena<=2)vykresli_otoc(canv,X,Y,name,short_name,eID,typ,rotace,stav);break;//pasivní otoč
		case 6:  if(scena<=2)vykresli_otoc(canv,X,Y,name,short_name,eID,typ,rotace,stav);break;//aktivní otoč
		case 7:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 8:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 9:  vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 10: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 11: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 12: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 13: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 14: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 15: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst,LO_pozice);break;//kontinuální robota
		case 16: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot se stopkou
		case 17: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,OTOC_delka,LO2,F->RO,F->ROst);break;//robot s pasivní otočí
		case 18: vykresli_robota(canv,scena,X,Y,name,short_name,eID,typ,rotace,stav,LO1,0,0,F->RO,F->ROst);break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 100:if(scena<=2)vykresli_ion(canv,X,Y,name,short_name,typ,rotace,stav,F->ROst);break;//ion tyč
		case 101:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot
		case 102:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot se stop stanicí
		case 103:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,OTOC_delka,LO2);break;//lidský robot s pasivní otočí
		case 104:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot s aktivní otočí (resp. s otočí a stop stanicí)
		case 105:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot ionizace
		case 106:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot ionizace se stop stanicí
		case 107:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,OTOC_delka,LO2);break;//lidský robot  ionizace s pasivní otočí
		case 108:vykresli_cloveka(canv,scena,X,Y,name,short_name,eID,typ,rotace+F->RO,stav,LO1,0,0);break;//lidský robot ionizace s aktivní otočí (resp. s otočí a stop stanicí)
		case 200:if(scena<=2)vykresli_predavaci_misto(canv,E,X,Y,name,typ,rotace,stav);break;//vykreslení předávacího místa - pouze popisek
		//case MaxInt:if(scena==0 || scena==1)vykresli_zarazku(canv,X,Y);break;//vykreslení zarážky pro testovací účely
		case 300://výhybka
		case 301://spojka
		{
			if(scena==0 || scena==1)
			{
				/////////provizorní řešení !!!!!!!!!!!!
				unsigned int velikost=m.round(2*F->Zoom);
				TColor barva=clRed;if(eID==301)barva=clBlue;
				canv->Pen->Color=barva;
				canv->Pen->Width=m.round(1);
				canv->Pen->Mode=pmCopy;
				canv->Pen->Style=psSolid;
				canv->Brush->Color=barva;
				canv->Brush->Style=bsSolid;
				canv->Rectangle(X-velikost,Y-velikost,X+velikost,Y+velikost);
        ////vykreslení popisku
        canv->Font->Color=barva;
				canv->Font->Size=F->m.round(2.8*F->Zoom);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*F->Zoom);
				canv->Font->Name=F->aFont->Name;
	    	canv->Font->Style = TFontStyles();
	    	canv->Brush->Color=clWhite;
				canv->Brush->Style=bsClear;
				AnsiString t="V";
				if(E->eID==301)t="S";
				t+=String(E->idetifikator_vyhybka_spojka);
				switch((int)E->geo.orientace)
				{
					case 90:case 270:X=m.round(X-canv->TextWidth(t)/2.0);Y=m.round(Y-velikost*1.2-canv->TextHeight(t));break;
					default:X=m.round(X-velikost*1.2-canv->TextWidth(t));Y=m.round(Y-canv->TextHeight(t)/2.0);break;
				}
				TextFraming(canv,X,Y,t);
			}
		}
		break;
		case 400:
		case 401:
		case 402:
		{  //teploměry
			vykresli_teplomer(canv,X,Y,name,short_name,eID,typ,rotace,stav,E);
			//E->citelna_oblast.rect3=aktOblast;
		}
		break;
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
	TColor barva=TColor RGB(218,36,44);if(stav==0 || stav==-2)barva=(TColor)RGB(60,179,113);//zelená světlejší(TColor)RGB(50,205,50);
	if((stav==-1 || stav==-2) && F->OBJEKT_akt!=NULL)
	{
		short I=m.get_intensity();
		if(typ==0)I=180;//pro ikony v knihovně elementů
		barva=m.clIntensive(barva,I);//pokud je aktivní nebo neaktivní
	}

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
		canv->Pen->Width=m.round(0.2*Z);
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
			if(F->scGPCheckBox1_popisky->Checked)//pokud je povoleno zobrazení popisků elementů
			{
		  	if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;//zvýraznění
		  	short h=0,w=canv->TextWidth(T2);
		  	canv->Font->Style = TFontStyles();h=canv->TextHeight(T1);w+=canv->TextWidth(T1+" ");//pro náze normální písmo
				float zAA=1.0;if(F->antialiasing)zAA=3.0;
		  	long x,y;
		  	//rotace
				switch((int)rotace)//posun referenčního bodu kvůli bílému orámování
				{                                                                                                                                                                             //nechat duplicitně        //vypsání indexu stopky, v případě editace i tučně
					case 0: 	rotace_textu(canv,0+900); x=m.round(X-h/2.0);   		y=m.round(Y-size+2*Z);	aktOblast=TRect(m.round(x/zAA),m.round((y-w)/zAA),m.round((x+h)/zAA),m.round(y/zAA)); TextFraming(canv,x,y,T1+" ");if(stav==3){canv->Font->Style = TFontStyles()<< fsBold;rotace_textu(canv,0+900);}TextFraming(canv,x,y-canv->TextWidth(T1+" "),T2);break;
					case 90:	rotace_textu(canv,0);		  x=m.round(X+size-2*Z);		y=m.round(Y-h/2);     	aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA)); TextFraming(canv,x,y,T1+" ");if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;TextFraming(canv,x+canv->TextWidth(T1+" "),y,T2);break;
					case 180:	rotace_textu(canv,2700);  x=m.round(X+h/2.0);   		y=m.round(Y+size-2*Z);	aktOblast=TRect(m.round((x-h)/zAA),m.round(y/zAA),m.round(x/zAA),m.round((y+w)/zAA)); TextFraming(canv,x,y,T1+" ");if(stav==3){canv->Font->Style = TFontStyles()<< fsBold;rotace_textu(canv,2700);}TextFraming(canv,x,y+canv->TextWidth(T1+" "),T2);break;
					case 270:	rotace_textu(canv,0);	    x=m.round(X-w-size+2.0*Z);y=m.round(Y-h/2); 			aktOblast=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+w)/zAA),m.round((y+h)/zAA)); TextFraming(canv,x,y,T1+" ");if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;TextFraming(canv,x+canv->TextWidth(T1+" "),y,T2);break;
				}
				rotace_textu(canv,0);//vrací nastavení do původního stavu
			}
		}
		else//ikona v knihovně elementů je text pod elementem
		{
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(F->m.round(X-canv->TextWidth(name)/2),Y,name);
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_robota(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2,double aP,float TS,double LO_pozice)
{
	try
	{
	double Z=F->Zoom;//zoom, pouze zkrácení zápisu

	//vstupní parametry - budou součástí parametrů metody robota
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
	if(stav==-1 && F->OBJEKT_akt!=NULL)//pokud je aktivní nebo neaktivní
	{
		short I=m.get_intensity();
		if(typ==0)I=180;//pro ikony v knihovně elementů
		barva=m.clIntensive(barva,I); //180 původní hodnota
		clOzeh=m.clIntensive(clOzeh,I);
		clIon=m.clIntensive(clIon,I);
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
		case 1: case 7: case 11: case 15: if(typ==1 && scena<=1)	vykresli_lakovaci_okno(canv,lX,lY,LO1,0,0,DkRB,rotace,LO_pozice);break;//pokud se jedná o kontinuálního robota v normálním zobrazení, zobrazí se ještě lakovací okno
		case 2: case 8: case 12: case 16: if(scena==0 ||scena==2) vykresli_stopku(canv,pX,pY,"","",typ,m.Rt90(rotace+180),stav);break;//robot se stopkou
		case 3: case 9: case 13: case 17: if(typ==1 && scena<=1) 	vykresli_lakovaci_okno(canv,lX,lY,LO1,OTOC_delka,LO2,DkRB,rotace,LO_pozice); if(scena<=1)vykresli_otoc(canv,pX,pY,"","",5,typ,rotace,stav);break;//s pasivní otočí
		case 4: case 10: case 14:case 18: if(scena==0 ||scena==1)	vykresli_otoc(canv,pX,pY,"","",6,typ,m.Rt90(rotace+180),stav);break;//s aktivní otočí (tj. s otočí a se stopkou)
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

	////statická scéna
	if(scena<=2)
	{
		////základna
		TRect zakladna=TRect(m.round(X-delka_zakladny/2.0),m.round(Y-sirka_zakladny/2.0),m.round(X+delka_zakladny/2.0),m.round(Y+sirka_zakladny/2.0));
		canv->RoundRect(zakladna,zaobleni,zaobleni);
	}

	//dynamická scéna
	if(scena==0 || 3<=scena)
	{
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
	}

	//	//testovací osa - smazat
//	linie(canv,cX-100,cY,cX+100,cY,1,clRed);
//	linie(canv,X,Y,cX,cY,1,clRed);
	//name="Robot1";//prozatim
	//short_name="R1";//prozatim

	////text
	if(typ!=-1 && scena<=2)//v módu kurzor se název nezobrazuje
	{              //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		//if(F->aFont->Size==12)canv->Font->Size=m.round(5.4*Z);else canv->Font->Size=m.round(5*Z);
		AnsiString T=short_name;
		//if(Z>4*3) //short_name odstaveno
		{T=name;if(F->aFont->Size==12)canv->Font->Size=m.round(2*Z); else canv->Font->Size=m.round(2.4*Z);}//od daného zoomu zobrazuje celý název
		if(typ==1)//pokud se jedná o standardní zobrazení
		{
			if(F->scGPCheckBox1_popisky->Checked)//pokud je povoleno zobrazení popisků elementů
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
				TextFraming(canv,x,y,name);//samotný vypis
			}
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
void Cvykresli::vykresli_cloveka(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2)
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
		case 101: case 105: if(typ==1 && scena<=1)  vykresli_lakovaci_okno(canv,X,Y,LO1,0,0,DkRBpx,m.Rt90(rotace90+180+rotace2));break;//pokud se jedná o kontinuálního robota v normálním zobrazení, zobrazí se ještě lakovací okno
		case 102: case 106: if(scena==0 ||scena==2) vykresli_stopku(canv,pX,pY,"","",typ,m.Rt90(rotace90+rotace2),stav);break;//robot se stopkou
		case 103: case 107: if(typ==1 && scena<=2) 	vykresli_lakovaci_okno(canv,X,Y,LO1,OTOC_delka,LO2,DkRBpx,m.Rt90(rotace90+180+rotace2));if(scena<=2)vykresli_otoc(canv,pX,pY,"","",5,typ,m.Rt90(rotace90+rotace2),stav);break;//s pasivní otočí
		case 104: case 108: if(scena==0 ||scena==1) vykresli_otoc(canv,pX,pY,"","",6,typ,m.Rt90(rotace90+rotace2),stav);break;//s aktivní otočí (tj. s otočí a se stopkou)
	}
	if(typ==0)F->Zoom*=1.5;//navrácení do původního stavu

	////nastavení barev
	TColor barva=clBlack;TColor clIon=(TColor)RGB(7,107,171);
	if(stav==-1 && F->OBJEKT_akt!=NULL)
	{
		short I=m.get_intensity();
		if(typ==0)I=180;//pro ikony v knihovně elementů
		barva=m.clIntensive(barva,I);
		clIon=m.clIntensive(clIon,I);
	}//pokud je aktivní nebo neaktivní

	////dynamická scéna
	if(scena==0 || 3<=scena)
	{
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
	}

	////text
	if(typ!=-1 && scena<=2)//v módu kurzor se název nezobrazuje
	{              //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		//if(F->aFont->Size==12)canv->Font->Size=m.round(5.4*Z);else canv->Font->Size=m.round(5*Z);
		AnsiString T=short_name;
		//if(Z>4*3) //short_name odstaveno
		{T=name;if(F->aFont->Size==12)canv->Font->Size=m.round(2*Z); else canv->Font->Size=m.round(2.8*Z);}//od daného zoomu zobrazuje celý název
		float Odsazeni=3.5*Z/*+41*/;//+41 z důvodu vycentrování ikony člověka v knihovně elementů
		if(typ==1)//pokud se jedná o standardní zobrazení
		{
			if(F->scGPCheckBox1_popisky->Checked)//pokud je povoleno zobrazení popisků elementů
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
				TextFraming(canv,x,y,name);//samotný vypis
			}
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

	TColor barva=clBlack; //odstaveno, vše černě if(eID==6)barva=clRed;
	if(stav==-1 && F->OBJEKT_akt!=NULL)//pokud je aktivní nebo neaktivní
	{
		short I=m.get_intensity();
		if(typ==0)I=180;//pro ikony v knihovně elementů
		barva=m.clIntensive(barva,I);
	}

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
			sipka(canv,m.round(X-size),m.round(Y+width),/*rotace*/R-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno s Z slouží na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+size),m.round(Y-width),/*rotace*/R-180-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z slouží  na negaci *Zoom v metodě šipka
		}
		else
		{
			canv->Pen->Color=clWhite;
			canv->Brush->Style=bsSolid;

			//přeříznutí pod šipkou
			canv->Rectangle(X-size,Y-width,m.round(X-size+1*Z),Y);//přežíznutí, možná lépe řešit obloukem
			canv->Rectangle(m.round(X+size-1*Z),Y,X+size,Y+width);//přežíznutí, možná lépe řešit obloukem
			//šipka
			sipka(canv,m.round(X-size),m.round(Y+width),R-25,false,width/Z,barva,barva);//děleno Z slouží na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+size),m.round(Y-width),R-180-25,false,width/Z,barva,barva);//děleno Z slouží na negaci *Zoom v metodě šipka
		}
	}
	else
	{
		R=90;//nerozlišuje se zda 90 nebo 270, v těchto případech je symbol stejně orotován
		if(typ==-1)//mód kurzor
		{
			//šipka
			sipka(canv,m.round(X-width),m.round(Y-size),R-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z slouží  na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+width),m.round(Y+size),R-180-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z slouží  na negaci *Zoom v metodě šipka
		}
		else
		{
			canv->Pen->Color=clWhite;
			canv->Brush->Style=bsSolid;

			//přeříznutí pod šipkou
			canv->Rectangle(X+width,Y-size,m.round(X+width-1*Z),m.round(Y-size+width+1*Z));//přežíznutí, možná lépe řešit obloukem
			canv->Rectangle(X+width,Y+size,m.round(X+width-1.5*Z),m.round(Y+size+width-1*Z));//přežíznutí, možná lépe řešit obloukem
			//šipka
			sipka(canv,m.round(X-width),m.round(Y-size),R-25,false,width/Z,barva,barva);//děleno Z slouží na negaci *Zoom v metodě šipka
			sipka(canv,m.round(X+width),m.round(Y+size),R-180-25,false,width/Z,barva,barva);//děleno Z slouží na negaci *Zoom v metodě šipka
		}
	}

	//pokud je otoč aktivní tj. se stopkou
	if(eID==6)vykresli_stopku(canv,X,Y,"","",typ,rotace,stav);

	//text
	if(typ!=-1)//v módu kurzor se název nezobrazuje
	{
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		canv->Font->Color=barva;if(/*eID==5 && odstaveno obě otoče černé*/stav!=-1 && typ==0)canv->Font->Color=m.clIntensive(barva,100);
		canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
		canv->Font->Name=F->aFont->Name;
		canv->Font->Style = TFontStyles();
		AnsiString T=name;//short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		if(typ==1)//normální zobrazení
		{
			if(F->scGPCheckBox1_popisky->Checked)//pokud je povoleno zobrazení popisků elementů
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
				TextFraming(canv,x,y,T);
				rotace_textu(canv,0);//navrácení do původního stavu
			}
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
	float tloustka_linie=1.05/3.0;if(stav==2)tloustka_linie*=1.3;//pokud má být zvýrazněn  //vykreslovací linie
	float DT=m.m2px(0.35);//delka trysky v metrech
	float TW=m.m2px(0.2);//tryska šířka před zúžením, nastavení dle fobota sirka_ramena v metrech
	float TZ=TW/2.0;//tryska ve zúžení //sirka2
	//TS;//sklon trysky, slouží pro animaci simulace (pohyb trysky), přebíráno již z parametru metody
	float TO=polomer/2.0;//odsazeni trysky od okraje tyče
	short o=1*Z;//odsazení textu
	int vzdalenost=m.m2px(v.PP.sirka_podvozek/2.0)+polomer+DT;if(typ==0)vzdalenost=m.m2px(1.3/2.0);//vzdálenost kružnice od středu v metrech (vzádlenost kružnic podělená dvěmi), pro kurzor a normální zobrazení dle šířky kolejí linky (šířky vozíku), jinak fixní šířka pro ikonu v galerii

	////nastavení barev
	TColor barva=clBlack;
	canv->Brush->Color=clWhite;//výplň kružnic
	TColor clIon=(TColor)RGB(7,107,171);
	if(stav==-1 && F->OBJEKT_akt!=NULL)//pokud je aktivní nebo neaktivní
	{
		short I=m.get_intensity();
		if(typ==0)I=180;//pro ikony v knihovně elementů
		barva=m.clIntensive(barva,I);
		clIon=m.clIntensive(clIon,I);
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

		//pravá tryska
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
			if(F->scGPCheckBox1_popisky->Checked)//pokud je povoleno zobrazení popisků elementů
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
				TextFraming(canv,x,y,Text);
				canv->Font->Orientation=0;//vrací nastavení do původního stavu
			}
		}
		else//ikona v knihovně elementů je text pod elementem
		{
			int odsazeni=58;//odsazení z důvodu správného zobrazení v knihovně
			canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
			canv->TextOutW(X-canv->TextWidth(name)/2,m.round(Y+vzdalenost+polomer-odsazeni),name); //1 pouze korekce
			canv->TextOutW(X-canv->TextWidth(short_name)/2,m.round(Y+vzdalenost+polomer+1*Z+canv->TextHeight(name)-odsazeni),short_name);
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_teplomer(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,Cvektory::TElement *Element)
{
	////vstupní proměnné či konstanty
	double Z=Form1->Zoom;//zoom, pouze zkrácení zápisu
	float polomer=0.2;if(stav==2)polomer=0.22;//poloměr kružnic zadaná v metrech
	int polomer1=m.m2px(polomer);//poloměr kružnic
	int polomer2=m.m2px(polomer/2.0);//poloměr kružnic
	int polomer8=m.m2px(polomer/8.0);//poloměr kružnic
	short DT=4;//násobek poloměru z kterého vznikné délka teploměru
	float tloustka_linie=1.05/2.0;if(stav==2)tloustka_linie*=1.3;//pokud má být zvýrazněn  //vykreslovací linie
	int vzdalenostY=m.m2px(v.PP.sirka_podvozek/2.0+polomer*2);//vzdálenost kružnice od středu v metrech (vzádlenost kružnic podělená dvěmi), pro kurzor a normální zobrazení dle šířky kolejí linky (šířky vozíku), jinak fixní šířka pro ikonu v galerii

	////rotace
	if(rotace==90 || rotace==270)
	{
		X+=vzdalenostY;
		Y+=vzdalenostY;
	}

	////nastavení barev
	TColor barva=clBlack,barva_vypln=clRed;if(eID==401)barva_vypln=clBlue;if(eID==402)barva_vypln=clBlack;
	canv->Brush->Color=clWhite;//výplň kružnic

	////nastavení pera
	TPenMode PenMode=pmCopy;
	canv->Pen->Style=psSolid;
	canv->Pen->Width=F->m.round(tloustka_linie*Z);
	canv->Pen->Mode=PenMode;
	canv->Pen->Color=barva;
	canv->Brush->Style=bsSolid;

	////vykreslení elementu
	//obrys
	canv->Ellipse(X-polomer1,Y-polomer1-vzdalenostY,X+polomer1,Y+polomer1-vzdalenostY);//baňka
	canv->RoundRect(X-polomer2,Y-polomer1*DT-vzdalenostY,X+polomer2,Y-vzdalenostY,polomer1,polomer1);//tyčka
	//bílá výplň
	canv->Pen->Color=canv->Brush->Color;canv->Ellipse(X-polomer1+canv->Pen->Width,Y-polomer1+canv->Pen->Width-vzdalenostY,X+polomer1-canv->Pen->Width,Y+polomer1-canv->Pen->Width-vzdalenostY);//baňka bílá výplň pozadí
	//barevná výplň - rtuť
	canv->Brush->Color=barva_vypln;canv->Pen->Color=canv->Brush->Color;
	canv->Ellipse(X-polomer2,Y-polomer2-vzdalenostY,X+polomer2,Y+polomer2-vzdalenostY);//baňka
	canv->Rectangle(X-polomer8,Y-polomer2*DT-vzdalenostY,X+polomer8,Y-vzdalenostY);//rtuť ve stupnici
	//stupnice
	canv->Pen->Color=barva;
	for(short i=0;i<=4;i++)
	{
		long y=Y-polomer1-polomer8-polomer2*i-vzdalenostY;
		long x1=X+polomer1;
		long x2=x1+polomer2;if(i%2)x2=x1+polomer8;//stupnice na přeskáčku
		line(canv,x1,y,x2,y);
	}

	////popisek
	//name="185";//provizorně pro testy
	canv->Brush->Color=clWhite;
	canv->Brush->Style=bsClear;
	canv->Font->Color=clBlack;
	canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
	canv->Font->Name=F->aFont->Name;//je nutné nastavovat kvůli správnosti zobrazení framingu
	if(stav==3)canv->Font->Style = TFontStyles()<< fsBold;else canv->Font->Style = TFontStyles();
	short Tw=canv->TextWidth(name);short Th=canv->TextHeight(name);if(name=="")Th=canv->TextHeight("°C");
	long x=m.round(X-Tw/2.0-canv->TextWidth("°C")/2.0);long y=Y-polomer1*DT-vzdalenostY-Th;
	TextFraming(canv,x,y,name);
	//citelná oblast popisku
	float zAA=1.0;if(F->antialiasing)zAA=3.0;
	Element->citelna_oblast.rect3=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+canv->TextWidth(name))/zAA),m.round((y+Th)/zAA));//souřadnice pro citelnou oblast, pro vykreslení oblasti by muselo být použito bez /zAA
	//°C
	canv->Font->Style = TFontStyles();
	x=x+Tw;
	TextFraming(canv,x,y,"°C");
	Element->citelna_oblast.rect4=TRect(m.round(x/zAA),m.round(y/zAA),m.round((x+canv->TextWidth("°C"))/zAA),m.round((y+Th)/zAA));//citelná oblast popisku "°C"
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_zarazku(TCanvas *canv,long X,long Y)
{
	if(F->Akce==F->GEOMETRIE || F->Akce==F->GEOMETRIE_LIGHT)
	{
		unsigned short W=Form1->Zoom*0.5;//0.25;
		canv->Pen->Style=psSolid;
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=m.clIntensive(clBlack,180);
		canv->Pen->Color=m.clIntensive(clBlack,180);
		canv->Pen->Mode=pmCopy;
		canv->Pen->Width=m.round(Form1->Zoom);
		canv->Ellipse(X-W,Y-W,X+W,Y+W);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_predavaci_misto(TCanvas *canv,Cvektory::TElement *E,long X,long Y,AnsiString name,short typ,double rotace,short stav)
{
	//typ: -1 = kurzor, 0 = ikona, 1 = klasické
	//stav:
	////konstanty
	double Z=Form1->Zoom;//zoom, pouze zkrácení zápisu
	////nastavení barev
	TColor barva=clBlack;
	canv->Brush->Color=clWhite;//výplň kružnic
	if(stav==-1 && F->OBJEKT_akt!=NULL)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	////////////////----
	if(typ==0)//ikona v knihovně elementů  - PROVIZORNĚ
	{
	////nastavení pera
  	float tloustka_linie=1.05/3.0;if(stav==2)tloustka_linie*=1.3;//pokud má být zvýrazněn  //vykreslovací linie
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

  	////vykreslení elementu
  	canv->Pen->Color=clWhite;
  	if(rotace==0 || rotace==180)canv->Rectangle(X-m.m2px(0.1),Y-m.m2px(0.25),X+m.m2px(0.1),Y+m.m2px(0.25));
  	else canv->Rectangle(X-m.m2px(0.25),Y-m.m2px(0.1),X+m.m2px(0.25),Y+m.m2px(0.1));
  	canv->Pen->Mode=PenMode;
  	canv->Pen->Color=barva;
  	if(rotace==0 || rotace==180)
  	{
			line(canv,X-m.m2px(0.1),Y-m.m2px(0.25)-15,X-m.m2px(0.1),Y+m.m2px(0.25)-15);
			line(canv,X+m.m2px(0.1),Y-m.m2px(0.25)-15,X+m.m2px(0.1),Y+m.m2px(0.25)-15);
		}
  	else
  	{
  		line(canv,X-m.m2px(0.25),Y-m.m2px(0.1),X+m.m2px(0.25),Y-m.m2px(0.1));
  		line(canv,X-m.m2px(0.25),Y+m.m2px(0.1),X+m.m2px(0.25),Y+m.m2px(0.1));
		}
	}
	////////////////----

	////vykreslení textu
	if(typ!=-1 && name!="" && F->OBJEKT_akt!=NULL && F->scGPCheckBox1_popisky->Checked)//v módu kurzor nebo pokud je součástí nadřazeného elementu se název nezobrazuje, nezobrazují se v layoutu
	{
		//nastavení písma  //pokud by tu nebylo ošetření zdisablovaného stavu, tak by se font již vypisoval bílou barvou....
		if(typ==0 && stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
		canv->Font->Size=F->m.round(2.8*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(2*Z);
		canv->Font->Name="Roboto Lt";
		canv->Font->Color=clPasiv;
		canv->Font->Style = TFontStyles();
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;

		if(typ==1 && E!=NULL)//normální zobrazení typ==1
		{
			if(/*stav==2 || */stav==3)canv->Font->Style = TFontStyles()<< fsBold;//došlo k vybrání elementu-tato část odstavena nebo přímo jeho textu
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n)canv->Font->Color=m.clIntensive(clBlack,m.get_intensity());
			//nastavení názvů pohonů
			AnsiString Tpom="",T1=F->ls->Strings[274],T2=F->ls->Strings[274];//"pohon nevybrán"
			if(E->pohon!=NULL)T1=E->pohon->name;
			if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)T2=E->dalsi->pohon->name;
			if(E->dalsi==NULL && v.ELEMENTY->dalsi->pohon!=NULL)T2=v.ELEMENTY->dalsi->pohon->name;
			//v případě 270 musí být popisky prohozeny
			if(v.vrat_objekt(E->objekt_n)->orientace==270){Tpom=T1;T1=T2;T2=Tpom;}
			int w1=canv->TextWidth(T1)/*,w2=canv->TextWidth(T2)*/;
			int h=canv->TextHeight(T1);
			long x1=0,y1=0,x2=0,y2=0;short K=0.25*Z;//pouze grafická korekce, text aby se nezohledňovalo zarovnání na diakritiku, vypadá to dinvě
			switch((int)m.Rt90(E->geo.orientace-E->geo.rotacni_uhel))
			{
				case 0:
				{
					x2=X-K;y2=Y+w1+m.m2px(0.2);
					x1=X-h-K;y1=Y-m.m2px(0.2);
					canv->Font->Orientation=900;
				}break;
				case 90:
				{
					x1=X-w1-m.m2px(0.2);y1=Y+K;
					x2=X+m.m2px(0.2);y2=Y-K-h;
				}break;
				case 180:
				{
					x1=X-K;y1=Y-w1-m.m2px(0.2);
					x2=X+h+K;y2=Y+m.m2px(0.2);
					canv->Font->Orientation=-900;
				}break;
				case 270:
				{
					x2=X-w1-m.m2px(0.2);y1=Y+K;
					x1=X+m.m2px(0.2);y2=Y-K-h;
				}break;

			}
			TextFraming(canv,x1,y1,T1);
			TextFraming(canv,x2,y2,T2);
			canv->Font->Orientation=0;
		}
	}
	if(typ==0)//ikona v knihovně elementů je text pod elementem
	{
		int odsazeni=-30;//odsazení z důvodu správného zobrazení v knihovně
		canv->Font->Name=F->aFont->Name;
		canv->Font->Size=F->m.round(sizeP*Z);if(F->aFont->Size==12)canv->Font->Size=F->m.round(3*Z);
		canv->TextOutW(X-canv->TextWidth(name)/2,m.round(Y/*+vzdalenost+polomer*/-odsazeni),name); //1 pouze korekce
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
	int W=60;//F->DrawGrid_geometrie->DefaultColWidth*3/2-o;
	//int odsazeni=-35;//vycentrování linie mezi hlavičkou a textem 6px
	TColor barva=clBlack; if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	//centrování pro vykreslení
	Y+=4*3;X-=5*3;  //X-=11*3;

	//vykreslení linie
	set_pen(canv,barva,1*10,PS_ENDCAP_FLAT);
	line(canv,X-36,Y-W-15,X+W,Y-W-15);//line(canv,X-W+8,Y-W/2+odsazeni,X+W-8,Y-W/2+odsazeni);
	line(canv,X-36,Y-W+6,X,Y-W+6);//linie před obloukem

	//short C=W/2;//zajištění vycentrování
	//vykreslení oblouku
	set_pen(canv,barva,1*10,PS_ENDCAP_FLAT);   //double Xodsaz=35;odsazeni=30;//6;
	canv->Arc(X-W,Y-W,X+W,Y+W,X+W,Y,X,Y-W);//canv->Arc(X-W-C-Xodsaz,Y-W+odsazeni,X+W-C-Xodsaz,Y+W+odsazeni,X+W-C-Xodsaz,Y+odsazeni,X-C-Xodsaz,Y-W+odsazeni);//směr proti hodinovým ručičkám

	//seříznutí oblouku
	canv->Pen->Color=clWhite;
	canv->Rectangle(X-30,Y+12,X+30,Y+50);

	//popisek
	Y-=4*3;X+=11*3;//centrování pro popise
	canv->Brush->Style=bsClear;
	if(stav!=-1)canv->Font->Color=m.clIntensive(barva,100);else canv->Font->Color=barva;//ikona vs. normální zobrazení
	canv->Font->Name=F->aFont->Name;
	canv->Font->Size=m.round(o*sizeP/3.0);if(F->aFont->Size==12)canv->Font->Size=o;
	canv->TextOutW(X-canv->TextWidth(Popisek)/2,Y+o/2,Popisek);
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
	sipka(canv,X+W-8,Y-W+8,m.azimut(X-W+8,Y,X+W-8,Y+W-8),false,1,barva,barva);//děleno Z na negaci *Zoom v metodě šipka

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
		canv->Pen->Color=clStenaKabiny;if(stav==-1)canv->Pen->Color=clPasiv;//barva
		canv->Pen->Width=m.m2px(0.12);//šířka v pixelech
		set_pen(canv,canv->Pen->Color,canv->Pen->Width,PS_ENDCAP_SQUARE);
		W*=30/1.5;//šířka torza objektu
		H*=30/1.5;//výška torza objektu
		o*=30/1.5;//komora odsazení
	}

	////vykreslení obrysu
	if(F->OBJEKT_akt!=NULL && (F->OBJEKT_akt->orientace==90 || F->OBJEKT_akt->orientace==270) || typ>=0)
	{
  	line(canv,X,Y,X+W,Y);//horní vodorovná
  	line(canv,X+W,Y,X+W,Y+H);//pravá svislá
  	line(canv,X+W,Y+H,X,Y+H);//dolní vodorovná
  	line(canv,X+o,Y+H,X+o,Y+H/2+o/2);//dolní kus komory
		line(canv,X+o,Y,X+o,Y+H/2-o/2);//horní kus komory
		////symbolika tekoucí kapaliny
		if(typ==1 || typ==-2)vykresli_pow_sprchu(canv,X+W,X+W,Y,Y+H,W-o,canv->Pen->Color,ceil(canv->Pen->Width/4.0),m.round(o/2.0),typ);
	}
	if(F->OBJEKT_akt!=NULL && (F->OBJEKT_akt->orientace==0 || F->OBJEKT_akt->orientace==180) && typ<0)
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
		TColor barva=clBlack; if(stav==-1)barva=clPasiv;//pokud je aktivní nebo neaktivní
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
Graphics::TBitmap *Cvykresli::nacti_nahled_cesty(Cvektory::TZakazka *zakazka)
{
	//deklarace proměnných
	double z=F->Zoom;//uchovávání původního Zoom
	TPointD posun=F->Posun;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	//nastavení velikostí bmp
	bmp_in->Height=3*98;
	bmp_in->Width=m.round((F->ClientWidth-F->scSplitView_LEFTTOOLBAR->Width)/(double)(F->ClientHeight-F->scGPPanel_statusbar->Height-F->scGPPanel_mainmenu->Height)*bmp_in->Height);//nemusí být násobeno 3x, počítá s bmp->Height, ta již je vynýsobená
	int W=bmp_in->Width/3.0-10,H=bmp_in->Height/3.0-10;//-10 == 5px odsazení od každé hrany bmp
	//nastavení základníh hodnot sloužících pro vyhledávání
	double MaxX=MaxInt*(-1),MaxY=MaxInt,MinX=MaxInt,MinY=MaxInt*(-1);
	////zjištění max oblasti vykreslení pohonu
	Cvektory::TElement *E=v.ELEMENTY->dalsi;
	while(E!=NULL)
	{
		if(E->geo.X1<MinX)MinX=E->geo.X1;
		if(E->geo.X1>MaxX)MaxX=E->geo.X1;
		if(E->geo.Y1<MaxY)MaxY=E->geo.Y1;
		if(E->geo.Y1>MinY)MinY=E->geo.Y1;
		if(E->geo.X4<MinX)MinX=E->geo.X4;
		if(E->geo.X4>MaxX)MaxX=E->geo.X4;
		if(E->geo.Y4<MaxY)MaxY=E->geo.Y4;
		if(E->geo.Y4>MinY)MinY=E->geo.Y4;
		E=v.dalsi_krok(E);
	}
	delete E;E=NULL;

	////výpočet nového Zoom, podle maximální oblasti vykreslení a velikosti bmp
	double z1,z2,rozdilX=m.m2px(MaxX-MinX),rozdilY=m.abs_d(m.m2px(MaxY-MinY));
	z1=abs(F->Zoom*W/rozdilX);z2=abs(F->Zoom*H/rozdilY);
	z1-=fmod(z1,0.05);z2-=fmod(z2,0.05);
	if(z1>z2)F->Zoom=z2;else F->Zoom=z1;

	//posun obrazu je ve fyzyckých souřadnicích, musí být dělen Zoom, posun středu oblasti na střed bmp
	F->Posun.x+=(m.L2Px((MaxX+MinX)/2.0)-W/2.0-5)/F->Zoom;
	F->Posun.y+=(m.L2Py((MinY+MaxY)/2.0)-H/2.0-5)/F->Zoom;

	//vykreslení cesty do pomocné bmp
	F->Zoom*=3;//kvůli AA
	vykresli_dopravnik(bmp_in->Canvas,zakazka);

	//vykreslení ID na bmp
	bmp_in->Canvas->Font=zakazka->mGrid->DefaultCell.Font;
	bmp_in->Canvas->Font->Size=m.round(35);
	bmp_in->Canvas->TextOutW(3*2,3*2,zakazka->n);

	//AA
	Cantialising a;
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);
	delete(bmp_in);

	//navrácení původního Zoomu
	F->Zoom=z;
	F->Posun=posun;

	//vracení výsledné bmp
	return bmp_out;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//metoda vypíše zprávy ze seznamu zpráv a zároveň uloží jejich citelné oblasti
void Cvykresli::vypis_zpravy(TCanvas *canv)
{
	if(v.ZPRAVY!=NULL)
	{
		//vstupní proměnné
		UnicodeString Text="";//vypisovaný rozšířený popis zprávy
		double Xr=0,Yr=0;//referenční souřadnice zpráv, které jsou přes sebe (od daného elementu)
		double Xh=0,Yh=0;//referenční souřadnice zpráv, které jsou přes sebe (od daného elementu) a mají být highlightované
		long X,Y;//fyzické souřadnice zprávy
		Cvektory::TZprava *Zt=NULL;//Zpráva s textem
		Cvektory::TZprava *Zh=v.vrat_zpravu(zprava_highlight);//highligtovaná zpráva, pomocný ukazatel
		short size=m.round(3*F->Zoom);//POZOR, v případě změny nutno ještě změnit i v v.PtInZpravy()

		//cyklické vypsání všech zpráv ze spojáku ZPRAVY
		Cvektory::TZprava *Z=v.ZPRAVY->dalsi;
		while(Z!=NULL)
		{
			////fyzické souřadnice zprávy
			long X=m.L2Px(Z->X);
			long Y=m.L2Py(Z->Y);

			////sebrání souřadnicové reference pro highlight pro případ překrytých zpráv
			if(zprava_highlight && Zh!=NULL && Zh->Element->n==Z->Element->n && Zh->X==Z->X && Zh->Y==Z->Y){Xh=Z->X;Yh=Z->Y;}

			////IKONA
			//test: F->Memo(AnsiString(Z->n)+"|"+AnsiString(Z->Element->n)+"|"+v.getVID(Z->VID)+"|"+AnsiString(m.round(Z->X))+"|"+AnsiString(m.round(Z->Y))+"||"+AnsiString(m.round(Z->predchozi->X))+"|"+AnsiString(m.round(Z->predchozi->Y)));
			if(Z->n==1 || Z->X!=Z->predchozi->X || Z->Y!=Z->predchozi->Y)//pokud jsou ikony přes sebe tak nezobrazuje ty další, první propouští vždy
			{
				TColor clCircle=clRed;
				AnsiString Tico="";
				switch(Z->zID)
				{
					 case -1: Tico="E";clCircle=clError;break;//barva errory
					 case 1:  Tico="W";clCircle=clWarning;break;//barva warningy
				}
				//kruhový podklad ikony
				canv->Brush->Style=bsSolid;
				//SetBkMode(canv->Handle,TRANSPARENT/*OPAQUE*/);//nastvení netransparentního pozadí
				canv->Brush->Color=clCircle;
				canv->Pen->Style=psSolid;
				canv->Pen->Mode=pmMask;if(F->Zoom<5*3)canv->Pen->Mode=pmCopy;//transparentní ikona od určitého zoomu
				canv->Pen->Color=clWhite;
				canv->Pen->Width=m.round(0.1*F->Zoom);//framing ikony
				canv->Ellipse(X-size,Y-size,X+size,Y+size);
				canv->Pen->Mode=pmCopy;//vrácení do původního stavu
				//text ikony
				canv->Font->Name=="Arial";
				canv->Font->Color=clWhite;
				canv->Font->Size=m.round(3.8*F->Zoom);
				if(/*zprava_highlight==Z->n || */zprava_highlight && Z->X==Xh && Z->Y==Yh){canv->Font->Style = TFontStyles()<<fsBold;}else canv->Font->Style = TFontStyles();//highlight zprávy připadně pokud jsou přes sebe tak i dalších
				canv->Brush->Style=bsClear;
				canv->TextOutW(X-m.round(canv->TextWidth(Tico)/2.0),Y-m.round(canv->TextHeight(Tico)/2.0),Tico);
			}

			////tvorba popisného TEXTU
			if(zobrazit_celou_zpravu==Z->n){Xr=Z->X;Yr=Z->Y;Zt=Z;}//získání souřadnic referenční zprávy a ukazatele na ni
			if(zobrazit_celou_zpravu && Z->X==Xr && Z->Y==Yr)//zobrazí všechny textové zprávy, co jsou v daném místě (na daném elementu a byly by přes sebe)
			{
				AnsiString S="";if(Text!="")S="\n";//případný separátor
				Text+=S+v.getVID(Z->VID);
			}

			//pouze "VYNULOVÁNÍ" CITELNÉ OBLASTI textu, plnění níže
			Z->citelna_oblast=NULL;

			//posun na další zpravu
			Z=Z->dalsi;
		}
		delete Z;

		////zobrazování popisného TEXTU
		if(zobrazit_celou_zpravu && Zt!=NULL)
		{
			//nastavení
			X=m.L2Px(Zt->X);
			Y=m.L2Py(Zt->Y);
			canv->Font->Name=F->aFont->Name;
			canv->Font->Size=m.round(3.5*F->Zoom);
			if(/*zprava_highlight==Zt->n || */zprava_highlight && Zt->X==Xh && Zt->Y==Yh)canv->Font->Style = TFontStyles()<<fsBold;else canv->Font->Style = TFontStyles();
			switch(Zt->zID)
			{
				case -1: canv->Font->Color=clError;break;//barva errory
				case 1:  canv->Font->Color=clWarning;break;//barva warningy
			}
			//samotné vykreslení výpisu + uložení CITELNÉ OBLASTI zobrazeného textu (oblast ikony se počítá ve vektorové metodě PtInZpravy())
			Zt->citelna_oblast=TextOut(canv,X,Y-size,Text,CENTER,BOTTOM,-0.8,clWhite,3);
		}
		Zt=NULL;delete Zt;
		Zh=NULL;delete Zh;
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
//orototuje obdelník, podle posledních parametrů, pokud jsou tyto parametry neuvdené, rotuje okolo středu obrazce, pro případné dalsí potřeby vrátí souřadnice středu orotovaného obdélniku
TPointD Cvykresli::obdelnik(TCanvas *canv,double X1,double Y1,double X2,double Y2,double rotace,double Sx,double Sy)
{
	TPointD body[]={{X1,Y1},{X2,Y1},{X2,Y2},{X1,Y2}};
	if(Sx==DOUBLE_MIN && Sx==Sy){Sx=(X1+X2)/2.0;Sy=(Y1+Y2)/2.0;}//pokud je požadováno orotuje okolo středu
	m.rotace_polygon(Sx,Sy,body,3,rotace);
	POINT body_px[]={{m.L2Px(body[0].x),m.L2Py(body[0].y)},{m.L2Px(body[1].x),m.L2Py(body[1].y)},{m.L2Px(body[2].x),m.L2Py(body[2].y)},{m.L2Px(body[3].x),m.L2Py(body[3].y)}};
	canv->Polygon((TPoint*)body_px,3);
	TPointD RET; RET.x=(body[0].x+body[2].x)/2.0;RET.y=(body[0].y+body[2].y)/2.0;//návratová hodnota středu (body do uhlopříčky) určená pro další použití
	return RET;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c nebo téměř totožné wiki   //http://www.yevol.com/bcb/Lesson12.htm
void Cvykresli::bezier(TCanvas *canv,TPointD *POLE,long posledni_prvek)
{
	canv->PolyBezier(m.L2P(POLE,posledni_prvek),posledni_prvek);
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
void Cvykresli::polygon(TCanvas *canv,Cvektory::TBod *body,TColor barva,short sirka,int stav,bool zobrazit_koty,bool automaticky_spojovat)
{
	if(body!=NULL && body->predchozi->n>1)
	{
		////výchozí parametry
		short I=100-F->scGPTrackBar_intenzita->Value;
		TColor clHighlight=m.clIntensive(barva,-50);
		TColor clDisabled=m.clIntensive(barva,I);

		////nastavení pera spojnic
		canv->Brush->Color=clWhite;canv->Brush->Style=bsClear;//nastavení výplně
		if(sirka>0 || F->MOD==F->EDITACE || stav>=0)//pro objekty se stěnou
		{
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
				if(F->MOD==F->EDITACE || stav>=0)sirka=m.m2px(0.15);
				canv->Pen->Mode=pmCopy;
				canv->Pen->Color=barva;if(stav==-1)canv->Pen->Color=clDisabled;//barva
				canv->Pen->Width=sirka;//šířka v pixelech
				set_pen(canv,canv->Pen->Color,sirka,/*PS_ENDCAP_FLAT*/PS_ENDCAP_SQUARE);
			}
		}
		else//pro objekty bez stěny
		{
			set_pen2(canv,barva,m.m2px(0.15),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true,NULL,NULL,m.m2px(0.15));
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
		if(stav>=0 && (F->OBJEKT_akt!=NULL || F->OBJEKT_akt==NULL && F->pom==NULL))//ošetření podmínkou oproti vykreslování činek místo hran objektů
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
			if(F->OBJEKT_akt!=NULL)//highlight pro objekt
			{hodnota_koty=-5,oblast_koty=-4;}
			//kontrola zda se jedná o čtverec či odelník (vykreslení pouze dvou kót)
			Cvektory::TBod *kota_od=NULL;
			double delka_koty=0;//zavedeno pro přepočet mezi časovou a délkovou kótou
			if(F->OBJEKT_akt!=NULL && body->predchozi->n==4)//pokud má objekt jen 4 body je otestován zda se jedná o obdelník nebo čtverec
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
			float width=0.1;if(m.m2px(0.4)>sirka)width*=2;//u haly kresli menší kóty
			while(B!=NULL)//vykreslení kót musí být v samostatém cyklu!!!!!(jinak ovlivňuje vykreslení spojnic bodů)
			{
				//nastavení highlightu
				if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==B->n)highlight=2;
				else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==B->n)highlight=1;else highlight=0;
				//výpočet délky kóty
				delka_koty=m.delka(B->predchozi->X,B->predchozi->Y,B->X,B->Y);
				if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->OBJEKT_akt->pohon->aRD;
				//vykreslení kóty
				if(kota_od==NULL)                                                                                                                                              //převedení na mm
				vykresli_kotu(canv,m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y),m.L2Px(B->X),m.L2Py(B->Y),m.round2double(F->outDK(delka_koty),3),NULL,B->kota_offset*F->Zoom/AA,highlight,width,clGray,false,NULL,B);
				else
				{
					//určení nové vzdálenosti
					delka_koty=m.round2double(m.delka(B->predchozi->X,B->predchozi->Y,B->X,B->Y),3);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->OBJEKT_akt->pohon->aRD;
					//vykreslení jedné kóty pro obdelník/čtverec
					vykresli_kotu(canv,m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y),m.L2Px(B->X),m.L2Py(B->Y),m.round2double(F->outDK(delka_koty),3),NULL,B->kota_offset*F->Zoom/AA,highlight,width,clGray,false,NULL,B);
					if(kota_od->dalsi!=NULL)//ošetření
					{
				  	//zjištění highlightu pro druhou kótu obdelníku/čtverce
				  	if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==B->dalsi->n)highlight=2;
						else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==B->dalsi->n)highlight=1;else highlight=0;
				  	//délka
				  	delka_koty=m.round2double(m.delka(B->X,B->Y,B->dalsi->X,B->dalsi->Y),3);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->OBJEKT_akt->pohon->aRD;
				  	//vykreslení
						vykresli_kotu(canv,m.L2Px(B->X),m.L2Py(B->Y),m.L2Px(B->dalsi->X),m.L2Py(B->dalsi->Y),m.round2double(F->outDK(delka_koty),3),NULL,B->dalsi->kota_offset*F->Zoom/AA,highlight,width,clGray,false,NULL,B->dalsi);
					}
					break;
				}
				B=B->dalsi;
			}
			//vykreslení poslední kóty
			delka_koty=m.delka(body->predchozi->X,body->predchozi->Y,body->dalsi->X,body->dalsi->Y);if(F->DKunit==2 || F->DKunit==3)delka_koty=delka_koty/F->OBJEKT_akt->pohon->aRD;
			if(F->pom_bod!=NULL && F->JID==oblast_koty&&F->pom_bod->n==body->dalsi->n)highlight=2;
			else if(F->pom_bod!=NULL && F->JID==hodnota_koty&&F->pom_bod->n==body->dalsi->n)highlight=1;else highlight=0;
			if(kota_od==NULL && body->predchozi->n>2)vykresli_kotu(canv,m.L2Px(body->predchozi->X),m.L2Py(body->predchozi->Y),m.L2Px(body->dalsi->X),m.L2Py(body->dalsi->Y),m.round2double(F->outDK(delka_koty),3),NULL,body->dalsi->kota_offset*F->Zoom/AA,highlight,width,clGray,false,NULL,body->dalsi);

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
	if(F->OBJEKT_akt!=NULL)z=z/2.5;//pokud je úchop vykreslován pro obrys kabiny = náhled, musí být zmenšen
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
//volání smart kurzoru sloužícího pro výběr geometrického elementu, to dle předchozích parametrů resp. typů geometrických elementu a zadaných vstupního paramerů metody, tj. dle posledního existujícího elementu
void Cvykresli::smart_kurzor(TCanvas *canv,Cvektory::TElement *E,short typElementu)
{
	Cvektory::TElement *Ep=NULL;
	//příprava atributů
	double preXk=0;
	double preYk=0;
	double preOR=0;
	double preRA=0;
	double prepreRA=0;
	if(E!=NULL)
	{
		preXk=E->geo.X4;
		preYk=E->geo.Y4;
		preOR=E->geo.orientace;
		preRA=E->geo.rotacni_uhel;
		if(E->predchozi!=NULL && E->predchozi->n>=1)Ep=E->predchozi;//nastavení předchozího elementu pokud exituje
	}
	else//pokud neexistuje žádny předchozí element bude smart kurzor umístěn na začátek objektu
	{
		Cvektory::TElement *e_posledni=NULL;
		//defaultně od prvního bodu aktuální kabiny
		preXk=F->OBJEKT_akt->element->geo.X1;
		preYk=F->OBJEKT_akt->element->geo.Y1;
		//pokud existuje předchozí kabina, tak od jejího posledního bodu
		if(F->OBJEKT_akt->element->predchozi->n!=0)
		{
			e_posledni=F->OBJEKT_akt->element->predchozi;
			preXk=e_posledni->geo.X4;
			preYk=e_posledni->geo.Y4;
		}
		preOR=F->OBJEKT_akt->orientace;
		if(e_posledni!=NULL && F->pom->predchozi!=NULL && F->pom->predchozi->n>=1){preRA=e_posledni->geo.rotacni_uhel;preOR=e_posledni->geo.orientace;}//nabrání rotačního úhlu a orientace z předchozího objektu
		e_posledni=NULL;delete e_posledni;
		//preRA=F->OBJEKT_akt->element->geo.rotacni_uhel;
	}
	if(Ep!=NULL && E!=NULL)prepreRA=Ep->geo.rotacni_uhel;
	Ep=NULL;delete Ep;
	//samotné volání smart kurzoru
	smart_kurzor(canv,preXk,preYk,preOR,preRA,prepreRA,typElementu);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//volání smart kurzoru sloužícího pro výběr geometrického elementu, to dle předchozích parametrů resp. typů geometrických elementu a zadaných vstupních paramerů metody, viz:
//preXk,preYk - koncové souřadnice, preOR - orientace, preRA - rotační úhel předchozího tj. posledního již vytvořeného geometrického elementu, prepreRArotační úhel před-předchozího geometrického elementu - neexistují-li tyto pre resp. prepre element(y), zadat nulu
void Cvykresli::smart_kurzor(TCanvas *canv,double preXk,double preYk,double preOR,double preRA,double prepreRA,short typElementu)
{
	////vstupní parametry
	double R=v.PP.radius;//radius dle katalogu
	short POLE_RA[]={90,45,30,15};//případně nahradit načítáním ze spojáku vybraného katalogu (ale zatím postrádá význam, všude jsou stejné úhly), pokud bych chtěl násleně break, muselo by být řazeno od nejmenšího RA
	double RA=-1000;//rotační úhel, výchozí hodnota -1000=nenalezen

	////hledání mezi oblouky
	if(typElementu!=2)
	{
		for(unsigned short i=0;i<4;i++)
		{
			//testování kladných hodnot a ošetření proti stavu 3x 90° oblouky za sebou a pokud je zároveň na základě předchozího geometrického prvku povoleno
			if(preRA>=0 && !(POLE_RA[i]==90 && prepreRA==preRA && preRA==90) && (preRA==0 || POLE_RA[i]==preRA) && m.LeziVoblouku(preXk,preYk,preOR-preRA,POLE_RA[i],R,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))RA=POLE_RA[i];
			//testování záporných hodnot a ošetření proti stavu 3x 90° oblouky za sebou a pokud je zároveň na základě předchozího geometrického prvku povoleno
			if(preRA<=0 && !(-POLE_RA[i]==-90 && prepreRA==preRA && preRA==-90) && (preRA==0 || -POLE_RA[i]==preRA) && m.LeziVoblouku(preXk,preYk,preOR-preRA,-POLE_RA[i],R,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))RA=-POLE_RA[i];
		} //pokud bych chtěl break, musel bych řadit od nejmenšího POLE_RA
	}

	////hledání v citelné oblasti linie ve tvaru "V" (toto rozmezí, rozptyl je hodnota nejmenšího oblouku (částečně logicky by se nabízelo /2, ale vhodnější /3, +- rozptylu nahrazuje fabs), hledá i přes výše nalezené řešní, což je důležíté ("jinak by se v některých případech na lini vůbec nedostalo")
	double delka_linie=0;
	if(typElementu!=1)
	{
		double a=m.azimut(preXk,preYk,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);//azimut mezi výchozím bodem a akt. pozicí myši
		double o=m.a360(preOR-preRA); if(o==0 && a>180)o=360;//orientace, pokud je nula a myš je v levém kvadrantu, je nutné z 0° udělat 360°
		if(POLE_RA[3]/3>fabs(o-a))RA=0;
		//if(RA==0)delka_linie=ceil(m.delka(preXk,preYk,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y)/3.0)*3;//u linie nabízí delší kresbu, po násobcích 3 metrů
		if(RA==0){delka_linie=m.round(m.delka(preXk,preYk,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y));if(delka_linie<1)delka_linie=1;}//provizorně nastaveno na 1 metr
	}

	////samotné vykreslení kurzoru dle hodnoty RA z předchozího algoritmu (aktuální orientace je prozatím z d.Temp.z, kde vypočtena jako je orientace minus rotace předchozího gElementu)
	vykresli_Gelement_kurzor(canv,preXk,preYk,m.a360(preOR-preRA),RA,R,delka_linie,preRA,prepreRA,typElementu);

	////!!!důležitá poznámka: uchování v globální proměnné aktuálně vracených hodnot ze smart kurzoru pro možné uložení do geometrického elementu nastává níže
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//obloukový či liniový (dle situace) kurzor g-elementu, X,Y jsou logické souřadnice výchozího vykreslování, parametry: orientace oblouku - dle světových stran (umí i jiné než 90° násobky), rotační úhel, pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0° jedná se o linii), radius - je radius oblouku v metrech nebo pokud je rotační úhel nastaven na 0° tedy se jedná o linii, je radius délkou linie)
void Cvykresli::vykresli_Gelement_kurzor(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,double delka_linie,double predchozi_rotacni_uhel,double predpredchozi_rotacni_uhel,short typElementu)
{
	TColor clPotencial=m.clIntensive(clBlack,245);
	TColor clAktual=m.clIntensive(clBlack,120);

	//vykreslení potenciální linie         //minimum 1 metry, potenciál je délka aktuální linie + 1 m, aby bylo jasné, že je možné stále prodlužovat
	if(typElementu==0 || typElementu==2)vykresli_potencial_Gelement(canv,X,Y,orientace,0,delka_linie+1,clPotencial,false);//provizorně nastaveno na 1 metr

	//vykreslení potenciálních oblouků dle katalogu
	if(typElementu==0 || typElementu==1)
	{
		short RA[]={90,45,30,15};//nahradit načítáním ze spojáku vybraného katalogu (ale zatím postrádá význam, všude jsou stejné úhly), pokud bych chtěl násleně break, muselo by být řazeno od nejmenšího RA
		short intenzitaK=0,intenzitaZ=0;//intenzita barvy
		for(unsigned short i=0;i<4;i++)
		{    //povoluje libovolný uhel po linii, ale pouze stejný po předch. oblouku && zajišťuje selekci nesmyslné varianty á la tangens) && !nesmí být 3x90° za sebou
			if((predchozi_rotacni_uhel==0 || RA[i]==predchozi_rotacni_uhel) && predchozi_rotacni_uhel>=0 && !(RA[i]==90 && predpredchozi_rotacni_uhel==predchozi_rotacni_uhel && predchozi_rotacni_uhel==90))vykresli_potencial_Gelement(canv,X,Y,orientace,RA[i],radius,m.clIntensive(clPotencial,-20*intenzitaK++),false);
			if((predchozi_rotacni_uhel==0 || -RA[i]==predchozi_rotacni_uhel) && predchozi_rotacni_uhel<=0 && !(-RA[i]==-90 && predpredchozi_rotacni_uhel==predchozi_rotacni_uhel && predchozi_rotacni_uhel==-90))vykresli_potencial_Gelement(canv,X,Y,orientace,-RA[i],radius,m.clIntensive(clPotencial,-20*intenzitaZ++),false);
		}
	}

	//vykreslení aktuální vybrané volby
	if(rotacni_uhel!=-1000)//byl-li uskutečněn výběr, -1000 znamená nebyl výběr
	{
		//vykreslení
		if(rotacni_uhel==0)radius=delka_linie;//pokud se jedná o linii, používá jeden předávací sloučený parametr
		TPointD *PL=vykresli_potencial_Gelement(canv,X,Y,orientace,rotacni_uhel,radius,clAktual,true);
		//uchování v globální proměnné aktuálně vracených hodnot ze smart kurzoru pro možné uložení do geometrického elementu
		if(rotacni_uhel==0)//pokud se jedná o linii
		{
			geoTemp.typ=0;
			geoTemp.radius=0;
			geoTemp.delka=delka_linie;
		}
		else//oblouk
		{
			geoTemp.typ=1;
			geoTemp.radius=radius;
			geoTemp.delka=m.R2Larc(radius,rotacni_uhel);
		}
		geoTemp.orientace=orientace;
		geoTemp.rotacni_uhel=rotacni_uhel;
		geoTemp.X1=PL[0].x;geoTemp.Y1=PL[0].y;geoTemp.X2=PL[1].x;geoTemp.Y2=PL[1].y;geoTemp.X3=PL[2].x;geoTemp.Y3=PL[2].y;geoTemp.X4=PL[3].x;geoTemp.Y4=PL[3].y;
		delete[] PL;PL=NULL;
	}
	else//neby-li uskutečněn výběr
	{
		geoTemp.typ=-1000;
  }
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajistí jednorázové vykreslení potenciálního obloukového či liniového (dle situace) g-elementu, X,Y jsou logické souřadnice výchozího vykreslování, parametry: orientace oblouku - dle světových stran (umí i jiné než 90° násobky), rotační úhel - pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0° jedná se o linii), radius - je radius oblouku v metrech nebo pokud je rotační úhel nastaven na 0° tedy se jedná o linii, je radius délkou linie)
TPointD *Cvykresli::vykresli_potencial_Gelement(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,TColor color,bool popisek)
{
	////vykreslení kolejí ve vybraném gelemntu       //resp. v tomto případě vybraný gelement
	if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->pohon!=NULL && F->scGPCheckBox_zobrazit_koleje->Checked && popisek)
	{
		short typ=1;if(rotacni_uhel==0)typ=0;
		vykresli_koleje(canv,X,Y,typ,orientace,rotacni_uhel,radius,radius,color);
	}

	////potenciální Gelement
	TPointD *PL=m.getArcLine(X,Y,orientace,rotacni_uhel,radius);
	POINT POLE[]={{m.L2Px(PL[0].x),m.L2Py(PL[0].y)},m.L2Px(PL[1].x),m.L2Py(PL[1].y),m.L2Px(PL[2].x),m.L2Py(PL[2].y),m.L2Px(PL[3].x),m.L2Py(PL[3].y)};//převod do fyzických souřadnic
	//nastavení geometrického pera
	if(F->scGPCheckBox_zobrazit_koleje->Checked && popisek)	set_pen(canv,color,m.round(F->Zoom*0.5),PS_ENDCAP_FLAT);//popisek v tomto případě vybraný gelement
	else set_pen(canv,color,m.round(F->Zoom*1),PS_ENDCAP_FLAT);//nastavení geometrického pera
	canv->PolyBezier((TPoint*)POLE,3);//samotné vykreslení bézierovy křivky

	////popisek, je-li požadován
	if(popisek)
	{
		canv->Brush->Style=bsClear;
		canv->Font->Style = TFontStyles();
		canv->Font->Name=F->aFont->Name;
		canv->Font->Size=3*F->Zoom;
		canv->Font->Color=m.clIntensive(color,-10);
		AnsiString T="";//původní společný výpis:AnsiString(rotacni_uhel)+"°, "+AnsiString(radius)+" m";
		//souřadnice textu
		int Xt=0,Yt=0;
		if(rotacni_uhel==0)//jedná se o linii
		{
			T=AnsiString(radius*1000)/*+" mm"*/;//zde radius je společná proměnnná i pro délku
			Xt=m.round((POLE[0].x+POLE[3].x)/2.0-canv->TextWidth(T)/2.0);
			Yt=m.round((POLE[0].y+POLE[3].y)/2.0-canv->TextHeight(T)/2.0);
		}
		else//jedné se o oblouk, najde polovinu oblouku
		{
      T=AnsiString(fabs(rotacni_uhel))+"°";
			TPointD *PL1=m.getArcLine(X,Y,orientace,rotacni_uhel/2.0,radius);
			Xt=m.round(m.L2Px(PL[3].x)-canv->TextWidth(T)/2.0);
			Yt=m.round(m.L2Py(PL[3].y)-canv->TextHeight(T)/2.0);
			delete[] PL1;PL1=NULL;
		}
		//volání výpisu textu
		TextFraming(canv,Xt,Yt,T,canv->Font);
	}
	return PL;//návrátová hodnota souřadnic oblouku pro případné další použití
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajistí jednorázové vykreslení libovolného obloukového či liniového (dle situace) g-elementu, X,Y jsou logické souřadnice výchozího vykreslování, parametry: orientace oblouku - dle světových stran (umí i jiné než 90° násobky), rotační úhel - pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0° jedná se o linii), radius - je radius oblouku v metrech nebo pokud je rotační úhel nastaven na 0° tedy se jedná o linii, je radius délkou linie), typ=-1 jen středová čára, typ=0 jen koleje, typ 1=koleje+zarážka na začátku, typ 2=koleje+zarážka na konci, typ 3=na začátku i na konci,, barva colorZarazka nastaví pro případně zobrazovanou zarážku speciální barvu, pokud není parametr použit (je NULL), tak dostane případná zarážka stejnou barvu jako barva parametru color
TPointD *Cvykresli::vykresli_Gelement(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,TColor color,float width,String Text,String Text2,short typ,TColor colorZarazka)
{
	////vykreslení Gelementu
	TPointD *PL=m.getArcLine(X,Y,orientace,rotacni_uhel,radius);
	if(typ==-1)//vykreslit pouze pohon
	{
		POINT POLE[]={{m.L2Px(PL[0].x),m.L2Py(PL[0].y)},m.L2Px(PL[1].x),m.L2Py(PL[1].y),m.L2Px(PL[2].x),m.L2Py(PL[2].y),m.L2Px(PL[3].x),m.L2Py(PL[3].y)};//převod do fyzických souřadnic
		//nastavení geometrického pera
		set_pen(canv,color,m.round(F->Zoom*width),PS_ENDCAP_FLAT);//nastavení geometrického pera
		canv->Pen->Mode=pmNotXor;
		canv->PolyBezier((TPoint*)POLE,3);//samotné vykreslení bézierovy křivky
	}
	if(typ>=0)//vykreslit pouze koleje, případně včetně zarážek
	{
		bool druh=0;if(rotacni_uhel!=0)druh=1;
		vykresli_koleje(canv,X,Y,druh,orientace,rotacni_uhel,radius,radius,color,typ,colorZarazka);
	}

	////záloha - nemazat, pokud bycho potřeboval oblouk vykreslovat standardní metodou (např. kvůli přesnosni):
	//float SA=F->Edit1->Text.ToDouble();//výchozí úhel, pod kterým oblouk začíná, musí být kladný - 0° je na 3 hodinách
	//set_pen(canv,clGreen,1/**F->Zoom*/,PS_ENDCAP_FLAT);
	//canv->MoveTo(m.L2Px(Xoblouku),m.L2Py(Yoblouku));//musí se přesunout pero na začátek, oblouku, v případě kontinuálního kreslení netřeba
	//canv->AngleArc(m.L2Px(Xoblouku),m.L2Py(Yoblouku),m.m2px(R),SA,RA);

	////výpis textu,vykreslí na konci Gelementu
	//odsazení
	short oX=m.m2px(0.5);//svislé výchozí odsazení o kurzoru zadané v metrech, použité následně v px
	short oY=m.m2px(0.2);//svislé výchozí odsazení o kurzoru zadané v metrech, použité následně v px
	if(Text!="" || Text2!="")
	{
		//nastavení písma
		canv->Brush->Style=bsClear;
		canv->Font->Style = TFontStyles();
		canv->Font->Name=F->aFont->Name;
		canv->Font->Size=3*F->Zoom;
		canv->Font->Color=color;//m.clIntensive(color,-10);
	}
	if(Text!="")TextFraming(canv,m.L2Px(PL[3].x)+oX,m.L2Py(PL[3].y)+oY,Text,canv->Font);
	if(Text2!="")
	{
		short W=0;if(Text!="")W=canv->TextHeight(Text);//pokud existuje Text, odřádkuje Text2 na další řádek
		TextFraming(canv,m.L2Px(PL[3].x)+oX,m.L2Py(PL[3].y)+oY+W,Text2,canv->Font);
	}

	return PL;//návrátová hodnota souřadnic oblouku pro případné další použití
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_mGridy(TCanvas *canv)
{
	if(F->OBJEKT_akt!=NULL && F->Timer_simulace->Enabled==false)//pokud není editovaný nějaký objekt nebo při timeru aplikace se tabulky nezobrazí
	{
		////tabulky elementů
		if(F->OBJEKT_akt->element!=NULL)
		{
			Cvektory::TElement *E=F->OBJEKT_akt->element;//přeskočí rovnou hlavičku
			while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
			{
				if((E->pohon==NULL && F->OBJEKT_akt->pohon==NULL || E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && E->pohon->n==F->OBJEKT_akt->pohon->n || E->eID==200 || E->eID==300 || E->eID==301) && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//vykreslení tabulek elementů, kteří mají stejný pohon jako aktuálně editovaný pohon
				{
					if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
					{
						if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE)//pokud nemají být zobrazeny mgridy nemá být zobrazen ani rastr
						{
							E->mGrid->Redraw=false;
							E->mGrid->SetVisibleComponents(false);
							E->mGrid->Left=m.L2Px(E->Xt);//kvůli případnému přesouvání tabulky
							E->mGrid->Top=m.L2Py(E->Yt);//kvůli případnému přesouvání tabulky
							E->mGrid->Show(canv);
						}
					}
					else
					{
						if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE)//pokud je mGrid zobrazen a nejedná se o posun obrazu
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
				}
				else E->mGrid->SetVisibleComponents(false);//pokud pohon elementu se nerovná aktuálně editovanému pohonu, je třeba skrýt všechny komponenty (posun obrazu PAN MOVE či skryté mGridy)
				E=v.dalsi_krok(E,F->OBJEKT_akt);
			}

			//pokud existuje předchozí předávací místo bude vykresleno
			E=F->predchozi_PM;
			if(E!=NULL)
			{
        if((E->pohon==NULL && F->OBJEKT_akt->pohon==NULL || E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && E->pohon->n==F->OBJEKT_akt->pohon->n || E->eID==200 || E->eID==300 || E->eID==301) && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//vykreslení tabulek elementů, kteří mají stejný pohon jako aktuálně editovaný pohon
				{
					if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
					{
						if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE)//pokud nemají být zobrazeny mgridy nemá být zobrazen ani rastr
						{
							E->mGrid->Redraw=false;
							E->mGrid->SetVisibleComponents(false);
							E->mGrid->Left=m.L2Px(E->Xt);//kvůli případnému přesouvání tabulky
							E->mGrid->Top=m.L2Py(E->Yt);//kvůli případnému přesouvání tabulky
							E->mGrid->Show(canv);
						}
					}
					else
					{
						if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE)//pokud je mGrid zobrazen a nejedná se o posun obrazu
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
				}
				else E->mGrid->SetVisibleComponents(false);//pokud pohon elementu se nerovná aktuálně editovanému pohonu, je třeba skrýt všechny komponenty (posun obrazu PAN MOVE či skryté mGridy)
			}
			E=NULL;delete E;
		}
		////tabulka pohonu
		if(F->PmG!=NULL)
		{
			TRect oblast_kabiny=F->vrat_max_oblast(F->OBJEKT_akt);
			if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
			{
				if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//pokud nemají být zobrazeny mgridy nemá být zobrazen ani rastr
				{
					F->PmG->Redraw=false;
			  	F->PmG->Left=m.L2Px(F->OBJEKT_akt->Xp);
			  	F->PmG->Top=m.L2Py(F->OBJEKT_akt->Yp);
			  	F->PmG->SetVisibleComponents(false);
					F->PmG->Show(canv);
				}
				else//pokud ne, je třeba skrýt komponenty
				{
					F->PmG->SetVisibleComponents(false);
				}
			}
			else
			{
				if(F->OBJEKT_akt->zobrazit_mGrid &&  F->Akce!=F->Takce::PAN_MOVE && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//pokud je mGrid zobrazen a nejedná se o posun obrazu
				{
					F->PmG->Redraw=true;
					F->PmG->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak F->PmG->Buffer(false);
					if(F->PmG->VisibleComponents>-1)F->PmG->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty, pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
					F->PmG->Left=m.L2Px(F->OBJEKT_akt->Xp);
					F->PmG->Top=m.L2Py(F->OBJEKT_akt->Yp);
					F->PmG->Show(canv);
				}
				else//pokud ne, je třeba skrýt komponenty
				{
					F->PmG->SetVisibleComponents(false);
				}
			}
		}
		////tabulka teploměru
		if(F->OBJEKT_akt->teplomery!=NULL && F->OBJEKT_akt->zobrazit_mGrid)
		{
			Cvektory::TTeplomery *T=v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,v.ZAKAZKA_akt);
      //kontrola zda existuje cesta pro akt zakázku
			if(T!=NULL && T->posledni->mGrid!=NULL)
			{
		  	if(F->refresh_mGrid==false)//zajistí načtení mGridu pouze z bufferu
		  	{
					if(F->OBJEKT_akt->zobrazit_mGrid && F->Akce!=F->Takce::PAN_MOVE && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//pokud nemají být zobrazeny mgridy nemá být zobrazen ani rastr
					{
						T->posledni->mGrid->Redraw=false;
						T->posledni->mGrid->Left=m.L2Px(T->posledni->Xt);
						T->posledni->mGrid->Top=m.L2Py(T->posledni->Yt);
						T->posledni->mGrid->SetVisibleComponents(false);
						T->posledni->mGrid->Show(canv);
					}
					else//pokud ne, je třeba skrýt komponenty
					{
		  			T->posledni->mGrid->SetVisibleComponents(false);
					}
		  	}
		  	else
				{
		  		if(F->OBJEKT_akt->zobrazit_mGrid &&  F->Akce!=F->Takce::PAN_MOVE && F->Akce!=F->Takce::GEOMETRIE && F->Akce!=F->GEOMETRIE_LIGHT)//pokud je mGrid zobrazen a nejedná se o posun obrazu
		  		{
		  			T->posledni->mGrid->Redraw=true;
						T->posledni->mGrid->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak T->posledni->mGrid->Buffer(false);
						if(T->posledni->mGrid->VisibleComponents>-1)T->posledni->mGrid->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty, pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
						T->posledni->mGrid->Left=m.L2Px(T->posledni->Xt);
		  			T->posledni->mGrid->Top=m.L2Py(T->posledni->Yt);
						T->posledni->mGrid->Show(canv);
					}
					else//pokud ne, je třeba skrýt komponenty
					{
						T->posledni->mGrid->SetVisibleComponents(false);
					}
		  	}
			}
      T=NULL;delete T;
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//pomocná metoda vytvářející rastrové obrazy mGridů, pokud je buffer na NULL, buffrují se všechny tabulky, pokud ne, tak pouze ta z parametru
void Cvykresli::nabuffrovat_mGridy(TmGrid *mGrid)
{
	if(mGrid==NULL)
	{
		if(F->OBJEKT_akt->element!=NULL)
		{
			Cvektory::TElement *E=F->OBJEKT_akt->element;//přeskočí rovnou hlavičku
			while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
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
void Cvykresli::vykresli_kotu(TCanvas *canv,Cvektory::TElement *Element_do)
{
	double O=F->OBJEKT_akt->koty_elementu_offset.x;

	//highlight
	short highlight=0;
	if(Element_do->stav==2)highlight=2;//pokud bude jeden ze zúčastněných elementů vybrán, zvýrazní se a vystoupí daná kóta
	if(Element_do!=NULL && F->MOD==F->EDITACE)
	{
		if(!F->posun_dalsich_elementu && ((F->JID+10)*(-1)==(long)Element_do->n || (F->JID==-101 && F->pom_element->n==Element_do->n)))highlight=1;//v případě, že není požadován posun dalších elementů, zvýrazní i kótu následujícího elementu, že se bude také měnit
		if((F->JID+10)*(-1)==(long)Element_do->n ||  (F->JID==13))highlight=1;//když se bude editovat hodnota kóty, nebo se bude kóta posouvat, kvůli následnému zaokrouhlování musí bohužel zůstat tady
	}
	//////probíhá editace kót
	if(F->Akce==F->GEOMETRIE || F->Akce==F->GEOMETRIE_LIGHT)
	{
		if(Element_do->geo.typ==0)vykresli_kotu(canv,Element_do->geo.X1,Element_do->geo.Y1,Element_do->geo.X4,Element_do->geo.Y4,Element_do,F->OBJEKT_akt->koty_elementu_offset.y,highlight);
	}
	//////bežná funkcionalita
	else if(Element_do->eID!=MaxInt)
	{
		////kota mezi elementy
		double x1,y1,x2,y2;          //////////////////////////kota mezi kabinou a prvním elementem !!!!!
		Cvektory::TElement *Element_od=Element_do->predchozi;
		while(Element_od!=NULL && Element_od->n>0 && Element_od->objekt_n==Element_do->objekt_n)
		{
			if(Element_od->geo.typ!=0)break;
			if(Element_od->eID!=MaxInt)break;//kota element - element
			Element_od=Element_od->predchozi;
		}
		//ošetření proti tomu je-li hned první element mimo kabinu, nebo hlavička elementů
		//if(Element_od!=NULL && (Element_od->n==0 || Element_od->objekt_n!=Element_do->objekt_n))Element_od=NULL;
		//určení bodů kóty
		if(Element_do->geo.orientace==90||Element_do->geo.orientace==270)//vodorovná kabina
		{
			if(Element_od==NULL)
			{x1=Element_do->geo.X1;y1=Element_do->geo.Y1;}
			else if(Element_od->n!=0 && Element_od->objekt_n==Element_do->objekt_n){x1=Element_od->X;y1=Element_od->geo.Y4;}
			else {x1=F->OBJEKT_akt->element->geo.X1;y1=F->OBJEKT_akt->element->geo.Y1;}
			x2=Element_do->X;y2=y1;
		}
		else
		{
			if(Element_od==NULL){x1=Element_do->geo.X1;y1=Element_do->geo.Y1;}
			else if(Element_od->n!=0 && Element_od->objekt_n==Element_do->objekt_n){x1=Element_od->geo.X4;y1=Element_od->Y;}
			else {x1=F->OBJEKT_akt->element->geo.X1;y1=F->OBJEKT_akt->element->geo.Y1;}
			y2=Element_do->Y;x2=x1;
		}
		if(x2<F->OBJEKT_akt->element->geo.X1)O=(O-0.66)*(-1);//ošetření chybného zobrazení kóty elementu, který je před kabinou
		//vykreslení kóty
		vykresli_kotu(canv,x1,y1,x2,y2,Element_do,O,highlight);
		////kota mezi LO
		bool vykreslit=false;
		bool el_od=false,el_do=false;
		if(Element_od!=NULL && (Element_od->eID==1 || Element_od->eID==7 || Element_od->eID==11 || Element_od->eID==15 || Element_od->eID==101 || Element_od->eID==105 || Element_od->eID==3 || Element_od->eID==9 || Element_od->eID==13 || Element_od->eID==17 || Element_od->eID==103 || Element_od->eID==107))el_od=true;
		if(Element_do->eID==1 || Element_do->eID==7 || Element_do->eID==11 || Element_do->eID==15 || Element_do->eID==101 || Element_do->eID==105 || Element_do->eID==3 || Element_do->eID==9 || Element_do->eID==13 || Element_do->eID==17 || Element_do->eID==103 || Element_do->eID==107)el_do=true;
		if(el_do && el_od)vykreslit=true;
		if(el_do && !el_od)//dodatečné hledání předchozího elementu s LO
		{
			Element_od=Element_do->predchozi;
			while(Element_od!=NULL && Element_od->n>0 && Element_od->objekt_n==Element_do->objekt_n)
			{
				if(Element_od->dalsi->geo.typ!=0)break;
				if(Element_od->eID==1 || Element_od->eID==7 || Element_od->eID==11 || Element_od->eID==15 || Element_od->eID==101 || Element_od->eID==105 || Element_od->eID==3 || Element_od->eID==9 || Element_od->eID==13 || Element_od->eID==17 || Element_od->eID==103 || Element_od->eID==107)
					{vykreslit=true;break;}
				Element_od=Element_od->predchozi;
			}
		}
		if(vykreslit)
		{
			TRectD E_od=F->souradnice_LO(Element_od),E_do=F->souradnice_LO(Element_do);
			double offset=1;
			switch((int)Element_do->geo.orientace)
			{
				case 0:y1=E_od.top;y2=E_do.bottom;x1=x2=Element_do->geo.X1;break;
				case 90:x1=E_od.right;x2=E_do.left;y1=y2=Element_do->geo.Y1;break;
				case 180:y1=E_od.bottom;y2=E_do.top;x1=x2=Element_do->geo.X1;break;
				case 270:x1=E_od.left;x2=E_do.right;y1=y2=Element_do->geo.Y1;offset*=-1;break;
			}
			vykresli_kotu(canv,x1,y1,x2,y2,Element_do,1,highlight,0.2,clGray,true);
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//v metrických jednotkách kromě width, zde v px + automaticky dopočítává délku a dosazuje aktuálně nastavené jednotky,highlight: 0-ne,1-ano,2-ano+vystoupení kóty i pozičně, aktElement pokud bude NULL, předpokládá se, že je to kóta kabiny
void Cvykresli::vykresli_kotu(TCanvas *canv,double X1,double Y1,double X2,double Y2,Cvektory::TElement *aktElement,double Offset,short highlight,float width,TColor color,bool LO_kota,Cvektory::TKomora *komora)
{    //Jednotky=" [s]";if(F->DKunit==3)Jednotky=" [min]";
	double delka=0;
	AnsiString T="";
	if(F->OBJEKT_akt->pohon==NULL && F->DKunit>1)F->DKunit=(TForm1::Tm_mm)(F->DKunit-2);//ošetření pro případ není pohon a jsou špatně nastaveny jednotky
	if(F->DKunit>1)//zobrazení kót v čase
	{
		if(aktElement!=NULL && aktElement->pohon!=NULL && aktElement->pohon->aRD>0 || komora!=NULL && F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->pohon->aRD>0)
		{
			if(aktElement!=NULL)delka=m.delka(X1,Y1,X2,Y2)/aktElement->pohon->aRD/(1+59.0*(F->DKunit-2));//výpočet délky a šířky kabiny + případný převod m->mm
			else delka=m.delka(X1,Y1,X2,Y2)/F->OBJEKT_akt->pohon->aRD/(1+59.0*(F->DKunit-2));//pro komory v POW
		}
		else T=F->ls->Strings[274];//"pohon nevybrán"
		//if(aktElement!=NULL) delka=v.vzdalenost_od_predchoziho_elementu(aktElement)/F->OBJEKT_akt->pohon->aRD/(1+59.0*(F->DKunit-2));//výpočet vzdálenosti mezi elementy
		//if(LO_kota)delka=m.round2double(F->vzdalenost_meziLO(aktElement,F->OBJEKT_akt->orientace),2)/F->OBJEKT_akt->pohon->aRD/(1+59.0*(F->DKunit-2));
	}
	else//standardní zobrazení ve vzdálenost
	{
		delka=m.delka(X1,Y1,X2,Y2)*(1+999*F->DKunit);//výpočet délky a šířky kabiny + případný převod m->mm
		//if(LO_kota)delka=m.round2double(delka,0);//problém v zaokrouhlování u LO kót
		//if(aktElement!=NULL)delka=v.vzdalenost_od_predchoziho_elementu(aktElement)*(1+999*F->DKunit);//výpočet vzdálenosti mezi elementy
		//if(LO_kota)delka=F->outDK(m.round2double(F->vzdalenost_meziLO(aktElement,F->OBJEKT_akt->orientace),2));
	}
	//odstaveno zobrazujeme na 3 realná delka=m.round2double(delka,8);//výpočet délky s max zobrazením na 8 míst (z důvodu případů 0.000000001 atp.) pouze v případě metrů, v mm by přetékalo při výpočtu, bylo by třeba long double
	//if(!F->DKunit)delka=m.round2double(delka,5);//výpočet délky s max zobrazením na 8 míst (z důvodu případů 0.000000001 atp.) pouze v případě metrů, v mm by přetékalo při výpočtu, bylo by třeba long double
	//else delka=m.round2double(delka,3);//if(AnsiString(delka).Pos("00000000001"))F->ms.MyToDouble(AnsiString(delka).SubString(1,AnsiString(delka).Pos("00000000001")-1));//pro mm ošetření proti 00000000001, protože nelze použít zaokrouhlení na větší počet desitnných míst
	if(T=="")T=m.round2double(delka,0/*nefuguje zde správně,".."*/);//standardní zobrazení na 3 reálná místa
	//odstaveno zobrazujeme na 3 realná if(highlight==1 || F->editace_textu)T=delka;//pokud se na kótu najede a předpokládá se editace tak se číslo rozbalí - nezaokrouhluje se, editace textu je možná navíc
	if(T!="0")vykresli_kotu(canv,m.L2Px(X1),m.L2Py(Y1),m.L2Px(X2),m.L2Py(Y2),T,aktElement,m.m2px(Offset),highlight,width,color,LO_kota,komora);
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//v px + dosazuje aktuálně nastavené jednotky,highlight: 0-ne,1-ano,2-ano+vystoupení kóty i pozičně, aktElement pokud bude NULL, předpokládá se, že je to kóta kabiny
void Cvykresli::vykresli_kotu(TCanvas *canv,long X1,long Y1,long X2,long Y2,AnsiString Text,Cvektory::TElement *aktElement,int Offset,short highlight,float width, TColor color,bool LO_kota,Cvektory::TKomora *komora,Cvektory::TBod *bod)
{                 
	////vstupní proměnné
  highlight=0;//riční vypnutí highlightu kót
	if(F->JID==-10 && F->MOD==F->EDITACE)highlight=0;//highlight - pokud se mění pouze jednotky, tak se kóta nehiglightuje
	if(aktElement==NULL&&komora==NULL&&bod==NULL)highlight=0;//highlight -odstranění highlightu na kótách mezi lak. okny
	short meritko=1;if(F->MOD==F->LAYOUT){width*=5;meritko=5;}//měřítko (náhled vs. schéma)
	width=m.round(width*F->Zoom);if(highlight)width*=2;//šířka linie
	short Presah=m.round(1.3*F->Zoom);if(Offset<0)Presah*=-1;//přesah packy u kóty,v případě záporného offsetu je vystoupení kóty nazákladě tohot záporné
  if(F->OBJEKT_akt!=NULL)Presah/=2.0;//zmenšení odsazení kót při highlightu v náhledu
	short V=0;//if(highlight==2)V=1;//vystoupení kóty
	short H=0;if(highlight)H=1;
	short M=0;if(F->JID==13 && F->MOD==F->EDITACE)M=1;//při celkovém posunu kót se postranní spojnice nově nezvýrazňují

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
//			if(F->index_kurzoru==-8 && Y1==Y2)if(F->editovany_text=="")Text="";else Text=F->editovany_text;//pro vodorovnou kótu
//			if(F->index_kurzoru==-9 && X1==X2)if(F->editovany_text=="")Text="";else Text=F->editovany_text;//pro svislou kótu
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
	{                                                               //v případě, že není požadován posun dalších elementů, zvýrazní i kótu následujícího elementu, že se bude také měnit
		if(aktElement!=NULL && (F->JID+10)*(-1)==(long)aktElement->n || !F->posun_dalsich_elementu && aktElement!=NULL && (F->JID+10)*(-1)==(long)aktElement->predchozi->n || ((F->JID==-8 || F->JID==-9) && F->MOD==F->EDITACE) || F->JID==-5 && F->OBJEKT_akt!=NULL || F->JID==-2 && F->OBJEKT_akt==NULL)canv->Font->Style = TFontStyles()<< fsBold;//pouze když se mění hodnota kóty
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
	if(F->JID==-10 && F->MOD==F->EDITACE)canv->Font->Style = TFontStyles()<< fsBold;else canv->Font->Style = TFontStyles();//pokud se editují jednotky, jinak (ani při highlightu se neztučňují)
//	canv->TextOutW(X+canv->TextWidth(Text),Y,Jednotky);//jednotky

	////navrácení citelné oblasti popisku a jednotek kóty pro další použití a šetření strojového času
//	if(F->MOD==F->EDITACE && F->OBJEKT_akt!=NULL)//pouze pokud se jedná o náhled a existuje ukazatel na OBJEKT_akt (což by mělo být při náhledu sice vždy...)
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
//				F->OBJEKT_akt->kabinaKotaX_oblastHodnotaAJednotky.rect1=R.rect1;//hodnoty
//				F->OBJEKT_akt->kabinaKotaX_oblastHodnotaAJednotky.rect2=R.rect2;//jednotky
//			}
//			else//pro svislou kótu
//			{
//				F->OBJEKT_akt->kabinaKotaY_oblastHodnotaAJednotky.rect1=R.rect1;//hodnoty
//				F->OBJEKT_akt->kabinaKotaY_oblastHodnotaAJednotky.rect2=R.rect2;//jednotky
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
//vykreslí trojúhelník indikující stoupání či klesání
void Cvykresli::vykresli_stoupani_klesani(TCanvas *canv,Cvektory::TElement *Element,float offset)
{
	////vstupní konstanty a proměnné
	TColor color=clLtGray;
	float width=0.2;
	double X1=Element->geo.X1;
	double Y1=Element->geo.Y1;
	double X2=Element->geo.X4;
	double Y2=Element->geo.Y4;
	double HeightDeep=Element->geo.HeightDepp;
	double X3,Y3;
	short Z=m.round(HeightDeep/fabs(HeightDeep));//znaménko
	float V=0.5;//výšku vykreslujeme konstatně, v měřítku by se nevešla
	//pokud je linie vodorovná
	if(Y1==Y2)
	{
		Y1+=offset;//odsazení
		Y2=Y1;
		Y3=Y2+V;//směr dle kladné či záporné hodnoty heightORdeep
		if(Z>0)X3=X2;else X3=X1;//umístění dle klesání či stoupání
	}
	//pokud je svislá
	else
	{
		X1+=offset;
		X2=X1;
		X3=X2+V;//směr dle kladné či záporné hodnoty heightORdeep
		if(Z>0)Y3=Y2;else Y3=Y1;//umístění dle klesání či stoupání
	}

	////trojúhelník vykreslení
	//nastavení vykreslení
	set_pen(canv,color,m.round(width*F->Zoom),PS_ENDCAP_SQUARE);//pero
	//definice bodů
	TPoint points[4];
	points[0].x = m.L2Px(X1);
	points[0].y = m.L2Py(Y1);
	points[1].x = m.L2Px(X2);
	points[1].y = m.L2Py(Y2);
	points[2].x = m.L2Px(X3);
	points[2].y = m.L2Py(Y3);
	points[3].x = points[0].x;
	points[3].y = points[0].y;
	//samotné vykreslení
	canv->Polyline(points,3);

	////popisek
	//nastavení textu
	canv->Font->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	canv->Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	canv->Font->Name=F->aFont->Name;
	canv->Font->Style=TFontStyles();
	canv->Font->Color=color;
	canv->Font->Size=m.round(width*F->Zoom*F->aFont->Size);
	//výpis
	String Text=String(HeightDeep*1000)+"  ";
	if(Z>0)Text="+"+Text;//příprava pro další pokračování: ±
	//orientace a vycentrování
	short W,H;
	if(X1==X2){canv->Font->Orientation=2700;W=-canv->TextHeight(Text);H=canv->TextWidth(Text);}
	else{W=canv->TextWidth(Text);H=canv->TextHeight(Text);}
	//souřadnice
	long x=m.round((points[0].x+points[2].x-W)/2.0);
	long y=m.round((points[0].y+points[2].y-H)/2.0);
	if(Z>0)
	{
		x=m.round((points[1].x+points[2].x-W)/2.0);
		y=m.round((points[1].y+points[2].y-H)/2.0);
  }
	//samotný výpis
	TextFraming(canv,x,y,Text);
	//pro jistotu vrácení do původního stavu
	canv->Font->Orientation=0;
	//uložení citelných oblastí - Z - souřadnice aktualní element (počátek stoupání či klesání) rect6, Z - souřadnice další element (konec stoupání či klesání) - rect 7, HeightDeep hodnota - rect 8
	float zAA=1.0;if(F->antialiasing)zAA=3.0;
	//Element->citelna_oblast.rect6=//Z - souřadnice aktualní element (počátek stoupání či klesání)
	//Element->citelna_oblast.rect7=//Z - souřadnice další element (konec stoupání či klesání)
	Element->citelna_oblast.rect8=TRect(m.round(x/zAA),m.round(y/zAA),m.round(x+W/zAA),m.round(y+H/zAA));//HeightDeep hodnota
	//upozornění pro testování výpisu citelné oblasti zde je nutné aZZ nastavit na 1!!! při kreslení přimo do Canvasu netřeba... Canvas->Rectangle(E->citelna_oblast.rect8);
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
		int T=Form1->scGPPanel_statusbar->Top-22;//umistění na Y - horního výchozího kraje měřítka
		if(Y>-1 && Y>-1)T=Y;
		if (F->scGPPanel_bottomtoolbar->Visible) T=T-F->scGPPanel_bottomtoolbar->Height;//posun při zobrazení toolbaru
		short H=4;//výška měřítka
		int   K=1;//krok v metrech
		short To=4;//odsazení textu popisku od měřítka
		if(Form1->Zoom==0.5)K=2;
		if(Form1->Zoom==0.25)K=5;
		int M=10;//MAX políček
		TColor barva_meritko=m.clIntensive(clRed,130);

		//nastavení pera a fontu canvasu
		canv->Pen->Color=barva_meritko;
		canv->Pen->Width=1;
		canv->Pen->Style=psSolid;
		canv->Brush->Style=bsSolid;
		canv->Pen->Mode=pmCopy;
		canv->Font->Size=11;
		canv->Font->Name=F->aFont->Name;
		canv->Font->Color=barva_meritko;

		//popisek 0
		//canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
		SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
		canv->TextOutW(L-canv->TextWidth("0")/2+1,T+To,"0");

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
			//canv->MoveTo(m.L2Px(m.P2Lx(L)+i/2),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i/2),T+7);//spojnice
			canv->TextOutW(m.L2Px(m.P2Lx(L)+i/2)-canv->TextWidth(M/2)/2,T+To,AnsiString(M/2));
		}
		//popisek MAX
		//canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);//spojnice
		canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+To,AnsiString(M)+" m");
	}
//	else //pro mod technologické procesy
//	{
//			//proměnné nastavení měřítka
//			int L=Form1->scSplitView_OPTIONS->Width-50*2-15;//umístění na X - levého výchozího kraje měřítka
//			if(Form1->scSplitView_LEFTTOOLBAR->Visible==false)L=Form1->ClientWidth-50*2-15;//pokud je levé menu skryto
//			int T=Form1->scGPPanel_mainmenu->Height+5;//umistění na Y - horního výchozího kraje měřítka
//			int H=5;//výška měřítka
//			int K=1*5;//krok v metrech
//			int M=2*5;//MAX políček
//			TColor barva_meritko=(TColor)RGB(128,128,128);//barva měřítka
//			//TColor barva_meritko=(TColor)RGB(43,87,154);//(0,120,215);barva měřítka
//
//			//nastavení pera a fontu canvasu
//			canv->Pen->Color=barva_meritko;
//			canv->Pen->Width=1;
//			canv->Pen->Style=psSolid;
//			canv->Brush->Style=bsSolid;
//			canv->Pen->Mode=pmCopy;
//			canv->Font->Size=8;
//			canv->Font->Name="Arial";
//			canv->Font->Color=barva_meritko;
//
//			//popisek 0
//			canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
//			SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
//			canv->TextOutW(L-canv->TextWidth("0")/2+1,T+5,"0");
//
//			//vykreslení políček měřítka
//			int i=0;
//			for(;i<M;i+=K)
//			{
//				if(i%(2*K))canv->Brush->Color=barva_meritko;//výplň barevna
//				else canv->Brush->Color=clWhite;//výplň bílá                 //+1 pouze grafická korekce
//				canv->Rectangle(m.L2Px(m.P2Lx(L)+i),T,m.L2Px(m.P2Lx(L)+(i+K))+1,T+H);
//			}
//
//			//musí být zde znovu, nastavení transparentního pozadí
//			SetBkMode(canv->Handle,TRANSPARENT);
//
//			//popisek MAX
//			canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);
//			canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+5,AnsiString(2)+" m");
//	}
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
	canv->Font->Color=m.clIntensive(clRed,110);
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
//	Beep(400,250);	// 440 hertz (A4) for half a second
//	Beep(500,250);	// 494 hertz for half a second
//	Beep(600,250);	// 523 hertz for half a second
//	Beep(700,250);	// 587 hertz for half a second
//	Beep(800,500);	// 659 hertz for half a second

//note
int C = 523; // DO - dura 191ms - 382ms
int D = 587; // RE - dura 170ms - 340ms
int E = 659; // MI - dura 152ms - 304ms
int F = 698; // FA - dura 143ms - 286ms
int G = 740; // SOL - dura 135ms - 270ms
int A = 880; // LA - dura 114ms - 228ms
int B = 988; // SI - dura 110ms - 220ms
// CCDCFE
Beep(C, 382);
Beep(C, 382);
Beep(D, 340);
Beep(C, 382);
Beep(F, 286);
Beep(E, 304);
// CCDCGF
Beep(C, 382);
Beep(C, 382);
Beep(D, 340);
Beep(C, 382);
Beep(G, 270);
Beep(F, 286);
// CCCAFE
Beep(C, 382);
Beep(C, 382);
Beep(C, 382);
Beep(A, 228);
Beep(F, 286);
Beep(E, 304);
//DBBAFGF
Beep(D, 340);
Beep(B, 220);
Beep(B, 220);
Beep(A, 228);
Beep(F, 286);
Beep(G, 270);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::kurzor_cesta(TCanvas *canv)
{
	Cvektory::TElement *E=Form_definice_zakazek->Z_cesta->cesta->predchozi->Element;      if(E==v.ELEMENTY->predchozi)E=v.ELEMENTY->dalsi;
	if(E->eID==300 || E==v.ELEMENTY->dalsi)vykresli_kurzor_cesta(canv,E);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_kurzor_cesta(TCanvas *canv,Cvektory::TElement *E)
{
	vykresli_potencial_cesty(canv,E);
	F->pom_element_temp=NULL;
	double X=E->geo.X4,Y=E->geo.Y4;
	if(E->dalsi2==NULL){X=E->geo.X1;Y=E->geo.Y1;}//pokdu nejsem na výhybce, jsem na začátku linky, proto musím začínat z X1,Y1
	set_pen(canv,clBlack,m.m2px(v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom)+2*m.px2m(m.round(F->Zoom))),PS_ENDCAP_FLAT);//nastavení geometrického pera
	TPointD *PL;  bool pokracovat=true,vykreslit=false;
	if(E->dalsi->geo.typ!=0)PL=m.getArcLine(X,Y,E->dalsi->geo.orientace,E->dalsi->geo.rotacni_uhel,E->dalsi->geo.radius);
	else
	{
		PL=new TPointD[4];
		PL[0].x=X;
		PL[0].y=Y;
		if(E->dalsi->geo.orientace==90 || E->dalsi->geo.orientace==270)
		{
			double x=2;if(E->dalsi->geo.orientace==270)x=-2;
			PL[3].x=X+x;
			PL[3].y=Y;
		}
		else
		{
			double y=2;if(E->dalsi->geo.orientace==180)y=-2;
			PL[3].x=X;
			PL[3].y=Y+y;
		}
		PL[2].x=PL[1].x=(PL[0].x+PL[3].x)/2.0;
		PL[2].y=PL[1].y=(PL[0].y+PL[3].y)/2.0;
	}

	if(E->dalsi->geo.typ==0 && m.LeziVblizkostiUsecky(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,PL[0].x,PL[0].y,PL[3].x,PL[3].y)==0 || E->dalsi->geo.typ!=0 && m.LeziVoblouku(X,Y,E->dalsi->geo.orientace,E->dalsi->geo.rotacni_uhel,E->dalsi->geo.radius,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))
	{
		vykreslit=true;
		pokracovat=false;
		if(E!=v.ELEMENTY->dalsi)
		{
			F->pom_element_temp=E->dalsi;
			E=E->dalsi;
		}
		else F->pom_element_temp=E;
	}
	if(pokracovat && E->dalsi2!=NULL)
	{
		if(E->dalsi2->geo.typ!=0)PL=m.getArcLine(X,Y,E->dalsi2->geo.orientace,E->dalsi2->geo.rotacni_uhel,E->dalsi2->geo.radius);
  	else
  	{
  		PL=new TPointD[4];
  		PL[0].x=X;
  		PL[0].y=Y;
			if(E->dalsi2->geo.orientace==90 || E->dalsi2->geo.orientace==270)
  		{
				double x=2;if(E->dalsi2->geo.orientace==270)x=-2;
  			PL[3].x=X+x;
  			PL[3].y=Y;
  		}
  		else
  		{
				double y=2;if(E->dalsi2->geo.orientace==180)y=-2;
  			PL[3].x=X;
  			PL[3].y=Y+y;
  		}
  		PL[2].x=PL[1].x=(PL[0].x+PL[3].x)/2.0;
  		PL[2].y=PL[1].y=(PL[0].y+PL[3].y)/2.0;
  	}
		if(E->dalsi2->geo.typ==0 && m.LeziVblizkostiUsecky(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,PL[0].x,PL[0].y,PL[3].x,PL[3].y)==0 || E->dalsi2->geo.typ!=0 && m.LeziVoblouku(X,Y,E->dalsi2->geo.orientace,E->dalsi2->geo.rotacni_uhel,E->dalsi2->geo.radius,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))
		{
			vykreslit=true;
			F->pom_element_temp=E->dalsi2;
			E=E->dalsi2;
		}
	}

	if(vykreslit)
	{
		bool preskocit=false;
		TPoint *POLE=new TPoint[4];
		while(E!=NULL)
		{
	  	//plnění geo souřadnic do pole
	  	POLE[0]=TPoint(m.L2Px(E->geo.X1),m.L2Py(E->geo.Y1));
	  	POLE[1]=TPoint(m.L2Px(E->geo.X2),m.L2Py(E->geo.Y2));
	  	POLE[2]=TPoint(m.L2Px(E->geo.X3),m.L2Py(E->geo.Y3));
	  	POLE[3]=TPoint(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));

	  	float	RetezWidth=v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom)+2*m.px2m(m.round(F->Zoom));
	  	RetezWidth=m.m2px(RetezWidth/2.0);

	  	set_pen(canv,clBlack,m.round(RetezWidth),PS_ENDCAP_SQUARE);
			//vykreslení řetězu
			if(!preskocit)
			{
				canv->PolyBezier(POLE,3);
				unsigned int pocet_pruchodu=v.kolikrat_obsahuje_segment_cesty_element(E,Form_definice_zakazek->Z_cesta);
				if(pocet_pruchodu>0)
				{
					for(unsigned int i=1;i<=pocet_pruchodu;i++)
	    		{
						double o=i*m.px2m(m.round(RetezWidth/2.0));
						set_pen(canv,m.getColorOfPalette(i),m.round(RetezWidth/2.0),PS_ENDCAP_SQUARE);
	    			short z=1;if(E->geo.rotacni_uhel>0)z*=-1;
	    			TPointD S2=m.rotace(o,180-E->geo.orientace,-90);
	    			double DR2=E->geo.delka;if(E->geo.typ==1)DR2=E->geo.radius+o*z*-1;//delka či radius
	    			TPointD *PL2=m.getArcLine(E->geo.X1+S2.x,E->geo.Y1+S2.y,E->geo.orientace,E->geo.rotacni_uhel,DR2);
						bezier(canv,PL2,3);
	    		}
				}
			}
			if(E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->predchozi2==E)preskocit=true;
			else preskocit=false;
			if(E->eID==300)break;
			else E=E->dalsi;
		}
		E=NULL;delete E;
	}

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_potencial_cesty(TCanvas *canv,Cvektory::TElement *E)
{
	double X=E->geo.X4,Y=E->geo.Y4;
	if(E->dalsi2==NULL){X=E->geo.X1;Y=E->geo.Y1;}//pokdu nejsem na výhybce, jsem na začátku linky, proto musím začínat z X1,Y1
	TColor color=m.clIntensive(clBlack,245);
	double Width=v.PP.sirka_podvozek/2.0+m.px2m(1/3.0*F->Zoom)+2*m.px2m(m.round(F->Zoom));
	Width=m.m2px(Width/2.0);
	////potenciální Gelement
	TPointD *PL;//=m.getArcLine(X,Y,orientace,rotacni_uhel,radius);
	if(E->dalsi->geo.typ!=0)PL=m.getArcLine(X,Y,E->dalsi->geo.orientace,E->dalsi->geo.rotacni_uhel,E->dalsi->geo.radius);
	else
	{
		PL=new TPointD[4];
		PL[0].x=X;
		PL[0].y=Y;
		if(E->dalsi->geo.orientace==90 || E->dalsi->geo.orientace==270)
		{
			double x=2;if(E->dalsi->geo.orientace==270)x=-2;
			PL[3].x=X+x;
			PL[3].y=Y;
		}
		else
		{
			double y=2;if(E->dalsi->geo.orientace==180)y=-2;
			PL[3].x=X;
			PL[3].y=Y+y;
		}
		PL[2].x=PL[1].x=(PL[0].x+PL[3].x)/2.0;
		PL[2].y=PL[1].y=(PL[0].y+PL[3].y)/2.0;
	}

	POINT POLE[]={{m.L2Px(PL[0].x),m.L2Py(PL[0].y)},m.L2Px(PL[1].x),m.L2Py(PL[1].y),m.L2Px(PL[2].x),m.L2Py(PL[2].y),m.L2Px(PL[3].x),m.L2Py(PL[3].y)};//převod do fyzických souřadnic
	//nastavení geometrického pera

	if(F->scGPCheckBox_zobrazit_koleje->Checked)set_pen(canv,color,Width,PS_ENDCAP_FLAT);//popisek v tomto případě vybraný gelement
	else set_pen(canv,color,Width,PS_ENDCAP_FLAT);//nastavení geometrického pera
	canv->PolyBezier((TPoint*)POLE,3);//samotné vykreslení bézierovy křivky
					color=m.clIntensive(clBlack,245);
	if(E->dalsi2!=NULL)
	{
  	if(E->dalsi2->geo.typ!=0)PL=m.getArcLine(X,Y,E->dalsi2->geo.orientace,E->dalsi2->geo.rotacni_uhel,E->dalsi2->geo.radius);
  	else
  	{
  		PL[0].x=X;
  		PL[0].y=Y;
  		if(E->dalsi2->geo.orientace==90 || E->dalsi2->geo.orientace==270)
  		{
  			double x=2;if(E->dalsi2->geo.orientace==270)x=-2;
  			PL[3].x=X+x;
  			PL[3].y=Y;
  		}
  		else
  		{
  			double y=2;if(E->dalsi2->geo.orientace==180)y=-2;
  			PL[3].x=X;
  			PL[3].y=Y+y;
  		}
  		PL[2].x=PL[1].x=(PL[0].x+PL[3].x)/2.0;
  		PL[2].y=PL[1].y=(PL[0].y+PL[3].y)/2.0;
  	}
	}

	POINT POLEa[]={{m.L2Px(PL[0].x),m.L2Py(PL[0].y)},m.L2Px(PL[1].x),m.L2Py(PL[1].y),m.L2Px(PL[2].x),m.L2Py(PL[2].y),m.L2Px(PL[3].x),m.L2Py(PL[3].y)};//převod do fyzických souřadnic
			 set_pen(canv,color,Width,PS_ENDCAP_FLAT);
	canv->PolyBezier((TPoint*)POLEa,3);//samotné vykreslení bézierovy křivky
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
