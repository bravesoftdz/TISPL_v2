////---------------------------------------------------------------------------
#pragma hdrstop
#include "vektory.h"
#include "unit1.h"
#include "miniform_zpravy.h"
////---------------------------------------------------------------------------
#pragma package(smart_init)
////---------------------------------------------------------------------------
////konstruktor
Cvektory::Cvektory()
{
	hlavicka_OBJEKTY();//vytvoří novou hlavičku pro objekty
	hlavicka_ELEMENTY();//vytvoří novou hlavičku pro elementy
	hlavicka_POHONY();//vytvoří novou hlavičku pro pohony
	hlavicka_ZAKAZKY();//vytvoří novou hlavičku pro zakazky
	hlavicka_VOZIKY();//vytvoří novou hlavičku pro vozíky
	hlavicka_DATA();//vytvoří novou hlavičku pro DATA
  hlavicka_MAG_LASO();//vytvoří novou hlavičku pro magnetické laso
	//hlavicka_RETEZY();//vytvoří novou hlavičku pro řetězy - nepoužíváno
	//hlavicka_palce();//vytvoří novou hlavičku pro palce - zatím nepoužíváno
	HALA.body=NULL;

	pozice_data=0;//nastavení na defaultní hodnotu
	pocet_kroku=5;//nastavení pevného počtu obrazů
	vyhybka_pom=NULL;//pomocná proměnná pro průchody ve spojáku elementy
	pocet_vyhybek=0;
	hlavicka_seznam_VYHYBKY();
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//pomocná metoda, není třeba volat, volá se automaticky při vkládání prvního bodu při vloz_bod, pokud je ukazatel na Objekt NULL, jedná se o metody pro HALU
void Cvektory::hlavicka_body(TObjekt *Objekt)
{
	if(Objekt!=NULL)//pro objekt
	{
		Objekt->body=new TBod;
		Objekt->body->n=0;
		Objekt->body->predchozi=Objekt->body;//hlavička ukazuje sama na sebe
		Objekt->body->dalsi=NULL;
	}
	else
	{
		HALA.body=new TBod;
		HALA.body->n=0;
		HALA.body->predchozi=HALA.body;//hlavička ukazuje sama na sebe
		HALA.body->dalsi=NULL;
	}
}
////---------------------------------------------------------------------------
//vloží nový bod na konec seznamu bodů, pokud je ukazatel na Objekt NULL, jedná se o metody pro HALU
void Cvektory::vloz_bod(TBod *Bod,TObjekt *Objekt)
{
	//OBJEKTY
	if(Objekt!=NULL)
	{
		//pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(Objekt->body==NULL)hlavicka_body(Objekt);

		//vložení nového bodu na konec seznamu bodů
		Bod->n=Objekt->body->predchozi->n+1;//navýšení počítadla
		Bod->predchozi=Objekt->body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
		Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
		Objekt->body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
		Objekt->body->predchozi=Bod;//hlavička ukazuje již na nový bod jako poslední prvek
	}
	else//HALA
	{
		//pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(HALA.body==NULL)hlavicka_body();

		Bod->n=HALA.body->predchozi->n+1;//navýšení počítadla
		Bod->predchozi=HALA.body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
		Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
		HALA.body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
		HALA.body->predchozi=Bod;//hlavička ukazuje již na nový bod jako poslední prvek
	}
}
////---------------------------------------------------------------------------
////společné metody pro HALU a objekty
//vloží nový bod na konec seznamu bodů pokud je Za=NULL, jinak vloží za tento bod, ošetřuje bod vložený na stejný místo jako předchozí, či jako první, pokud se jedná o poslední vložení při uzavírání polygonu a je zapnuta ortogonalizace, je zajištěno, aby byl první poslední a předposlední bod v ortogonalizovaném vztahu, zajištění poslední spojnice zajištuje vykreslovací metoda, pokud jsou vloženy pouze 3 body a ukončeno vkládání je dopočítán 4 bod do rozměrů obdélníku
void Cvektory::vloz_bod(double X, double Y,TObjekt *Objekt,TBod *ZaBod,bool ortogonalizovat,bool konec)
{
	////alokace paměti
	TBod *Bod=new TBod;

	////data
	//nastavení defaultních hodnot
	Bod->kota_offset=-55;if(Objekt!=NULL)Bod->kota_offset/=5.0*5.5/7.0;
	//souřadnice připadně ortogonalizované
	if(ortogonalizovat)//pokud je požadavek na ortogonalizaci, tak ověření zda je možný
	{
		ortogonalizovat=false;
		if(Objekt!=NULL && Objekt->body!=NULL)//pro objekt
		{
			if(Objekt->body->predchozi->n>0)ortogonalizovat=true;//nejedná se o první prvek, na ten se orotogonalizace nevztahuje
		}
		if(Objekt==NULL && HALA.body!=NULL)//pro halu
		{
			if(HALA.body->predchozi->n>0)ortogonalizovat=true;//nejedná se o první prvek, na ten se orotogonalizace nevztahuje
		}
	}
	if(ortogonalizovat)//pokud je požadavek na ortogonalizaci
	{
		TBod *B=ZaBod;if(B==NULL){if(Objekt!=NULL)B=Objekt->body->predchozi;else B=HALA.body->predchozi;}
		TBod *P=NULL;if(Objekt!=NULL)P=Objekt->body->dalsi;else P=HALA.body->dalsi;

		if(m.abs_d(B->X-X)<m.abs_d(B->Y-Y)){Bod->X=B->X;Bod->Y=Y;}//zarovnat dle X
		else{Bod->X=X;Bod->Y=B->Y;}//zarovnat dle Y

		if(konec==true && ZaBod==NULL && B->n>2)//poslední bod
		{
			if(Bod->X==B->X){Bod->Y=P->Y;}
			else{Bod->X=P->X;}
		}

		B=NULL;delete B;P=NULL;delete P;
	}
	else//vložení bez ortogonalizace
	{
		Bod->X=X;
		Bod->Y=Y;
	}

	////vkládání do bodů OBJEKTU
	if(Objekt!=NULL)
	{
		//pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(Objekt->body==NULL)
		{
			Objekt->body=new TBod;
			Objekt->body->n=0;
			Objekt->body->predchozi=Objekt->body;//hlavička ukazuje sama na sebe
			Objekt->body->dalsi=NULL;
		}
		//vložení nového bodu na konec seznamu bodů
		if(ZaBod==NULL || ZaBod!=NULL && ZaBod==Objekt->body->predchozi)//pokud se má vkládat nakonec
		{																	//situace aktuální (budoucí poslední) první         											//situace  aktuální (budoucí poslední) a poslední aktuální (budoucí předposlední)
			if(Objekt->body->dalsi==NULL || (Bod->X!=Objekt->body->dalsi->X || Bod->Y!=Objekt->body->dalsi->Y) && (Bod->X!=Objekt->body->predchozi->X || Bod->Y!=Objekt->body->predchozi->Y))//pokud se vkládá první prvek, ale pokud je poslední vkládaný totožný jako první nebo totožný jako předchozí (např. u ukočování kresby, nebo u chybného kliku), tak ho ignoruje a neuložího do spojáku)
			{
				Bod->n=Objekt->body->predchozi->n+1;//navýšení počítadla
				Bod->predchozi=Objekt->body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
				Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
				Objekt->body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
				Objekt->body->predchozi=Bod;//hlavička ukazuje již na nový bod jako poslední prvek
			}
		}
		else//vložení mezi body
		{
			//nastavení počítadla u vkládaného bodu
			Bod->n=ZaBod->n+1;
			//navýšení počítadla u následujícího bodu
			TBod *B=ZaBod->dalsi;
			while(B!=NULL)
			{
				B->n++;
				B=B->dalsi;
			}
			B=NULL;delete B;
			//nové ukazatelové propojení
			ZaBod->dalsi->predchozi=Bod;//následujícímu bodu přídá ukazatel na předchozí na vkladaný
			Bod->dalsi=ZaBod->dalsi;//vkládaná ukazuje na původní následují
			ZaBod->dalsi=Bod;//za požadovaný bod se vloží vkládaný bod
			Bod->predchozi=ZaBod;//vkládaná ukazuje na predchozí (ZaBod)
		}
	}
	else////pro HALU
	{
		//pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(HALA.body==NULL)
		{
			HALA.body=new TBod;
			HALA.body->n=0;
			HALA.body->predchozi=HALA.body;//hlavička ukazuje sama na sebe
			HALA.body->dalsi=NULL;
		}
		//vložení nového bodu na konec seznamu bodů
		if(ZaBod==NULL || ZaBod!=NULL && ZaBod==HALA.body->predchozi)//pokud se má vkládat nakonec
		{                               //situace aktuální (budoucí poslední) první         						//situace  aktuální (budoucí poslední) a poslední aktuální (budoucí předposlední)
			if(HALA.body->dalsi==NULL || (Bod->X!=HALA.body->dalsi->X || Bod->Y!=HALA.body->dalsi->Y) && (Bod->X!=HALA.body->predchozi->X || Bod->Y!=HALA.body->predchozi->Y))//pokud se vkládá první prvek, ale pokud je poslední vkládaný totožný jako první nebo totožný jako předchozí (např. u ukočování kresby, nebo u chybného kliku), tak ho ignoruje a neuložího do spojáku)
			{                             //zkontrolovat či doplnit duplcitu vůči předchozímu
				Bod->n=HALA.body->predchozi->n+1;//navýšení počítadla
				Bod->predchozi=HALA.body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
				Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
				HALA.body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
				HALA.body->predchozi=Bod;//hlavička ukazuje již na novbod jako poslední prvek
			}
		}
		else//vložení mezi body
		{
			//nastavení počítadla u vkládaného bodu
			Bod->n=ZaBod->n+1;
			//navýšení počítadla u následujícíh bodů
			TBod *B=ZaBod->dalsi;
			while(B!=NULL)
			{
				B->n++;
				B=B->dalsi;
			}
			B=NULL;delete B;
			//nové ukazatelové propojení
			ZaBod->dalsi->predchozi=Bod;//následující komoře přídá ukaztel na předchozí na vkladanou
			Bod->dalsi=ZaBod->dalsi;//vkládaná ukazuje na původní následují
			ZaBod->dalsi=Bod;//za požadovaný bod se vloží vkládaný bod
			Bod->predchozi=ZaBod;//vkládaná ukazuje na predchozí (ZaBod)
		}
	}

	////pokud jsou jenom 3 body a je konec, tak se 4 bod automaticky dopočítá
	if(ortogonalizovat && konec==true && ZaBod==NULL)
	{
		TBod *F=NULL;if(Objekt!=NULL)F=Objekt->body->dalsi; F=HALA.body->dalsi;//první
		TBod *L=NULL;if(L==NULL){if(Objekt!=NULL)L=Objekt->body->predchozi;else L=HALA.body->predchozi;}//poslední
		if(L->n==3)
		{
			if(L->Y>F->Y)vloz_bod(L->X,F->Y,Objekt,NULL,true,true);
			else vloz_bod(F->X,L->Y,Objekt,NULL,true,true);
		}
		F=NULL;delete F;L=NULL;delete L;
	}
}
////---------------------------------------------------------------------------
//posune bod HALy nebo Objektu, záleží, kam bod náleží
void Cvektory::posun_bod(double X, double Y,TBod* Bod)
{
	Bod->X=X;Bod->Y=Y;
}
////---------------------------------------------------------------------------
//posune hranu tvořenou danými body o zadaný offset
void Cvektory::posun_hranu(double OffsetX,double OffsetY,TBod* Bod1,TBod* Bod2)
{
	Bod1->X+=OffsetX;Bod1->Y+=OffsetY;Bod2->X+=OffsetX;Bod2->Y+=OffsetY;
}
////---------------------------------------------------------------------------
//posune všechny body polygonu objektu či haly o daný offset
void Cvektory::posun_body(double OffsetX,double OffsetY,TObjekt* Objekt)
{
	TBod *B=NULL;
	if(Objekt!=NULL && Objekt->body!=NULL)B=Objekt->body->dalsi;//posun bodů OBJEKTU
	else if(HALA.body!=NULL)B=HALA.body->dalsi;//posun bodů HALY

	while(B!=NULL)
	{
		B->X+=OffsetX;
		B->Y+=OffsetY;
		B=B->dalsi;
	}
	B=NULL;delete B;
}
////---------------------------------------------------------------------------
//orotuje celý polygon zadaného objektu či haly proti směru hodinových ručiček okolo osy dle bodu o souřadnicích X,Y, dle hodnoty rotace uhel
void Cvektory::rotuj_body(double X, double Y,double uhel,TObjekt* Objekt)
{
	if(fmod(uhel,360)!=0)//pouze ošetření proti zbytečné rotaci
	{
		TBod *B=NULL;
		if(Objekt!=NULL && Objekt->body!=NULL)B=Objekt->body->dalsi;//posun bodů OBJEKTU
		else if(HALA.body!=NULL)B=HALA.body->dalsi;//posun bodů HALY

		while(B!=NULL)
		{
			TPointD BOD=m.rotace(X,Y,B->X,B->Y,uhel);
			B->X=BOD.x;B->Y=BOD.y;
			B=B->dalsi;
		}
		B=NULL;delete B;
	}
}
////---------------------------------------------------------------------------
//na aktuálních souřadnicích myši hledá bod, pokud je nalezen vrátí na něj ukazatel, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
Cvektory::TBod *Cvektory::najdi_bod(TObjekt* Objekt)
{
	float o=0.4;if(F->Zoom<=1)o=m.round(o/F->Zoom*5.5);//citelná oblast v metrech, pokud nebudu chtít zvětšovat se zoomem, tak zde podělit zoomem, pokud by se hodnota měnila, tak změnit i v vykresli v uchop metodě!!!
	TBod *B=NULL;
	if(Objekt!=NULL&&Objekt->body!=NULL)B=Objekt->body->dalsi;//jedná se o body objektu + přeskočí hlavičku
	else if(HALA.body!=NULL)B=HALA.body->dalsi;//jedná se bod haly + přeskočí hlavičku
	while(B!=NULL)
	{
		if(m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,B->X,B->Y,o))break;
		B=B->dalsi;
	}
	return B;
}
////---------------------------------------------------------------------------
//na aktuálních souřadnicích myši hledá úsečku, pokud je nalezena je vracen ukazatel na druhý bod, pokd nebylo nalezeno nic vrátí NULL, parametr Objekt implicitně NULLL, rozlišuje hledání úsečky v HALE nebo v Objektu, parametr přesnost určuje vzdálenost od přímky, která je nutná k její detekci
Cvektory::TBod *Cvektory::najdi_usecku(TObjekt* Objekt,long presnost)
{
	double x=F->akt_souradnice_kurzoru_PX.x,y=F->akt_souradnice_kurzoru_PX.y;//souřadnice kurzoru jsou neměnné po celou dobu metody
	TBod *A=NULL,*B=NULL;//return proměnná + krokování cyklu
	if(Objekt!=NULL&&Objekt->body!=NULL)B=Objekt->body->dalsi->dalsi;//jedná se o body objektu + přeskočí hlavičku + začátek na druhém bodu
	else if(HALA.body!=NULL&&HALA.body->predchozi->n>1)B=HALA.body->dalsi->dalsi;//jedná se bod haly + přeskočí hlavičku + začně na druhém bodu (pokud existuje, jestli ne return NULL)
	while(B!=NULL)
	{
		//prohledávání úseček mezi akt. bodem a předchozím bodem
		if(m.LeziVblizkostiUsecky(x,y,m.L2Px(B->predchozi->X),m.L2Py(B->predchozi->Y),m.L2Px(B->X),m.L2Py(B->Y))<=presnost)break;
		//pokud jsem na posledním bodu musím zkontrolovat úsečku mezi posledním a prvním
		if(B->dalsi==NULL)
		{
			//předání ukazatele na první bod podle režimu (Objekt, HALA)
			if(Objekt!=NULL&&Objekt->body!=NULL)A=Objekt->body->dalsi;
			else if(HALA.body!=NULL&&HALA.body->predchozi->n>1)A=HALA.body->dalsi;
			//prohledávání první, poslední, pokud nalezeno uloží do ret. proměnné B ukazatel na první bod
			if(m.LeziVblizkostiUsecky(x,y,m.L2Px(A->X),m.L2Py(A->Y),m.L2Px(B->X),m.L2Py(B->Y))<=presnost){B=A;break;}
		}
		B=B->dalsi;
	}
	//nulování + mazání pomovného ukazatele
	A=NULL;delete A;
	return B;
}
////---------------------------------------------------------------------------
//ověří zda se na daných fyzických souřadnicích myši nachází kóta hrany/stěny HALy či Objektu (Objektu pokud Objekt!=NULL) , pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty , pozn. oblast kóty se testuje až jako poslední
short Cvektory::PtInKota_bod(TObjekt *Objekt)
{
	short RET=-1;//nic nenalezeno
	double x=F->akt_souradnice_kurzoru_PX.x,y=F->akt_souradnice_kurzoru_PX.y;//souřadnice kurzoru jsou neměnné po celou dobu metody, důležité použít fyzické souřadnice !!!
	TBod *B=NULL;//return proměnná + krokování cyklu
	if(Objekt!=NULL&&Objekt->body!=NULL)B=Objekt->body->dalsi;//jedná se o body objektu + přeskočí hlavičku + začátek na druhém bodu
	else if(HALA.body!=NULL&&HALA.body->predchozi->n>1)B=HALA.body->dalsi;//jedná se bod haly + přeskočí hlavičku + začně na druhém bodu (pokud existuje, jestli ne return NULL)
	while(B!=NULL)
	{
		if(B->kota.rect1.PtInRect(TPoint(x,y))){RET=1;F->pom_bod=B;break;}//hodnoty kóty
		else
		{
			if(B->kota.rect2.PtInRect(TPoint(x,y))){RET=2;F->pom_bod=B;break;}//jednotky kóty
			else if(m.LeziVblizkostiUsecky(x,y,B->kota.rect0.left,B->kota.rect0.top,B->kota.rect0.right,B->kota.rect0.bottom)<=F->Zoom){RET=0;F->pom_bod=B;break;}//oblast kóty
		}
		B=B->dalsi;
	}
	B=NULL;delete B;
	return RET;
}
////---------------------------------------------------------------------------
//ověří zda se souřadnicích myši nachází ve vnitř polygonu, pokud je Objekt==NULL, hledá se v polygonu HALy
bool Cvektory::PtInBody(TObjekt *Objekt,bool vcetne_poloviny_sirky_steny)
{                                     vcetne_poloviny_sirky_steny=true;
	////vstupní proměnné
	bool RET=false;
	TBod *B=NULL;
	if(Objekt!=NULL && Objekt->body!=NULL)B=Objekt->body;//jedná se o body objektu
	else if(HALA.body!=NULL && HALA.body->predchozi->n>1)B=HALA.body;//jedná se bod haly

	////vytvoření regionu
	if(B!=NULL)
	{
		unsigned long pocet=B->predchozi->n;//počet prvků pole
		POINT *body=new POINT[pocet];//dynamické pole bodů již ve fyzických souřadnicích, zde jen alokace
		B=B->dalsi;//přeskočí hlavičku
		//plnění do pole
		double maxX=DOUBLE_MIN; double minX=DOUBLE_MAX; double maxY=maxX; double minY=minX;
		while(B!=NULL)
		{
			if(!vcetne_poloviny_sirky_steny){body[B->n-1].x=m.L2Px(B->X);body[B->n-1].y=m.L2Py(B->Y);}
			else {maxX=maxX>B->X?maxX:B->X;maxY=maxY>B->Y?maxY:B->Y;minX=minX<B->X?minX:B->X;minY=minY<B->Y?minY:B->Y;}
			B=B->dalsi;
		}
		//v případě pokud má být započtena polovina stěny, je zajištěn výpočet koeficient zvětšení objektu resp. testované oblasti
		if(vcetne_poloviny_sirky_steny)
		{
			double faktorX=(maxX-minX+Objekt->sirka_steny)/(maxX-minX); double ofsetX=minX*faktorX-minX;
			double faktorY=(maxY-minY+Objekt->sirka_steny)/(maxY-minY); double ofsetY=minY*faktorY-minY;
			if(Objekt!=NULL && Objekt->body!=NULL)B=Objekt->body;//jedná se o body objektu
			else if(HALA.body!=NULL && HALA.body->predchozi->n>1)B=HALA.body;//jedná se bod haly
			B=B->dalsi;//přeskočí hlavičku
			//plnění do pole
			while(B!=NULL)
			{
				body[B->n-1].x=m.L2Px(B->X*faktorX-ofsetX);body[B->n-1].y=m.L2Py(B->Y*faktorY-ofsetY);
				B=B->dalsi;
			}
		}

		//testování oblasti
		HRGN hreg=CreatePolygonRgn(body,pocet,WINDING);//vytvoření regionu
		delete[] body;body=NULL;
		RET=PtInRegion(hreg,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y);
		DeleteObject(hreg);
	}

	////nulování + mazání pomovného ukazatele
	B=NULL;delete B;

	////návratová hodnota
	return RET;
}
////---------------------------------------------------------------------------
//zkopíruje body včetně z originálu na kopii bez ukazatelového propojení, funguje jenom pro body objektů nikoliv HALY!!!
void Cvektory::kopiruj_body(TObjekt *Original,TObjekt *Kopie)
{
	//pokud kopie obsahuje původní body, tak je smaže
	//vymaz_body(Kopie);
	//samotné nakopírování
	if(Original->body!=NULL)
	{
		TBod *B=Original->body->dalsi;//přeskočí hlavičku
		while(B!=NULL)
		{
			vloz_bod(B->X,B->Y,Kopie,NULL,false,false);//nutné vyplnit i nepovinné parametry
			B=B->dalsi;
		}
		B=NULL;delete B;
	}
}
////---------------------------------------------------------------------------
//smaže konkrétní bod, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
void Cvektory::smaz_bod(TBod* Bod,TObjekt* Objekt)
{
	if(Bod!=NULL && Bod->n!=0)//pokud existuje a zároveň mimo hlavičky, ta nejde smazat
	{
		if(Bod->dalsi==NULL)//jedná se o poslední prvek (múže být i za hlavičkou)
		{
			if(Objekt!=NULL)Objekt->body->predchozi=Bod->predchozi;//pro body
			else HALA.body->predchozi=Bod->predchozi;//pro HALU
			Bod->predchozi->dalsi=NULL;
		}
		else//nejedná se o poslední prvek
		{
			Bod->predchozi->dalsi=Bod->dalsi;
			Bod->dalsi->predchozi=Bod->predchozi;
		}

		//přeindexování následujících bodů
		Bod=Bod->dalsi;
		while(Bod!=NULL)
		{
			Bod->n--;
			Bod=Bod->dalsi;
		}
		Bod=NULL;delete Bod;
	}
}
////---------------------------------------------------------------------------
//vymaže všechny body včetně hlavičky, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
void Cvektory::vymaz_body(TObjekt* Objekt)
{
	if(Objekt!=NULL)//pro body
	{
		if(Objekt->body!=NULL)
		{
			//maže odzadu dokud nezbyde pouze hlavička
			while(Objekt->body->dalsi==NULL)smaz_bod(Objekt->body->predchozi,Objekt);
			//na závěr ještě smaže hlavičku
			Objekt->body=NULL;delete Objekt->body;
		}
	}
	else//pro HALU
	{
		if(HALA.body!=NULL)
		{
			//maže od zadu dokud nezbyde pouze hlavička
			while(HALA.body->dalsi==NULL)smaz_bod(HALA.body->predchozi);
			//na závěr ještě smaže hlavičku
			HALA.body=NULL;delete HALA.body;
		}
	}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////vytvoří novou hlavičku pro objekty
void Cvektory::hlavicka_OBJEKTY()
{
	TObjekt *novy=new TObjekt;
	novy->n=0;
	novy->id=0;
	novy->X=0;
	novy->Y=0;
	novy->body=NULL;
	novy->sirka_steny=0;
	novy->short_name="";//krátký název
	novy->name="";//celý název objektu
	novy->rezim=-1;
	novy->pohon=NULL;//ukazatel na použitý pohon
	novy->element=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->koty_elementu_offset.x=0;//odsazení kót elementů v metrech
	novy->koty_elementu_offset.y=0;//odsazení kót elementů v metrech
	novy->komora=NULL;//ukazatel na komory
  novy->teplomery=NULL;
	novy->probehla_aktualizace_prirazeni_pohonu=false;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
	novy->zobrazit_koty=true;//proměnná určující, zda se budou zobrzovat kóty
	novy->zobrazit_mGrid=true;//proměnná určující, zda budou zobrazeny mGridy
	novy->uzamknout_nahled=false;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu
	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	OBJEKTY=novy;//OBJEKTY
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////uloží objekt a jeho parametry do seznamu
Cvektory::TObjekt *Cvektory::vloz_objekt(unsigned int id, double X, double Y)
{
	TObjekt *novy=new TObjekt;
	//ukazatele
	novy->n=OBJEKTY->predchozi->n+1;
	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
	//atributy
	nastav_atributy_objektu(novy,id,X,Y);
	return novy;
}
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu za objekt p        //p předchozí
Cvektory::TObjekt *Cvektory::vloz_objekt(unsigned int id, double X, double Y,TObjekt *pred,TObjekt *po)
{
	TObjekt *novy=new TObjekt;
	//ukazatele
	novy->predchozi=pred;
	pred->dalsi=novy;
	novy->n=OBJEKTY->predchozi->n+1;
	//novy->predchozi=po->predchozi;
//	po->predchozi->dalsi=novy;
//	po->predchozi=novy;
	novy->dalsi=po;
	if(po!=NULL)po->predchozi=novy;
	else OBJEKTY->predchozi=novy;
	//pred->dalsi=novy;
	//novy->n=pred->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
	//indexy zvýšit separátně
	//atributy
	nastav_atributy_objektu(novy,id,X,Y);
	return novy;
}
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu - přetížená fce
Cvektory::TObjekt *Cvektory::vloz_objekt(TObjekt *Objekt)
{
	TObjekt *novy=new TObjekt;

	*novy=*Objekt;//novy bude ukazovat tam kam prvek Objekt
	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí

	return novy;
}
//---------------------------------------------------------------------------
//alokuje paměť pro objekt, nastavý atriuty objektu, vrátí ukazatel na nově vytvořený prvek
void Cvektory::nastav_atributy_objektu(TObjekt *novy,unsigned int id, double X, double Y)
{
	AnsiString name,short_name;//dočasná konstrukce pro přiřazování spráných názvů objektům
//	if(id==(unsigned)F->VyID){name=knihovna_objektu[id].name+" "+AnsiString(pocet_vyhybek);short_name=knihovna_objektu[id].short_name+AnsiString(pocet_vyhybek);}
//	else if(id<=pocet_objektu_knihovny) {name=knihovna_objektu[id].name;short_name=knihovna_objektu[id].short_name;}else {name="Spojka "+AnsiString(pocet_vyhybek);short_name="S"+AnsiString(pocet_vyhybek);}
	name=knihovna_objektu[id].name;
	switch(id)
	{
		case 0:name=F->ls->Strings[254];break;//"navěšování"
		case 2:name=F->ls->Strings[260];break;//"ožeh"
		case 4:name=F->ls->Strings[261];break;//"ionizace"
		case 5:name=F->ls->Strings[262];break;//"lakování"
		case 6:name=F->ls->Strings[279];break;//"vytěkání"
		case 7:name=F->ls->Strings[280];break;//"sušení"
		case 8:name=F->ls->Strings[281];break;//"chlazení"
		case 9:name=F->ls->Strings[255];break;//"svěšování"
		case 11:name=F->ls->Strings[282];break;//"výtah"
		case 12:name=F->ls->Strings[283];break;//"přejezd"
		case 13:name=F->ls->Strings[284];break;//"výhybka"
		case 14:name=F->ls->Strings[285];break;//"nedefinovaný"
	}
	if(id!=13 && id!=14){short_name=name.UpperCase();short_name=short_name.SubString(1,3);}else {short_name=name.UpperCase();short_name=short_name.SubString(1,1);}
	if(id==14)short_name=knihovna_objektu[id].short_name;

	novy->id=id;
	novy->short_name=short_name;
	novy->name=name;
	novy->rezim=-1;if(id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->stavPM=0;//0-normal (zobrazeny rychlosti a comba, dafaultní stav), 1-minimized (pouze hlavička tabulek a comba), 2-maximized (vše zobrazeno)
	novy->X=X;//přiřadím X osu,pozice objektu
	novy->Y=Y;//přiřadím Y osu,pozice objektu
	novy->Xp=-500;
	novy->Yp=-500;
	novy->body=NULL;//spojový seznam definičních bodů obrysu objektu
	novy->sirka_steny=0.15;//šířka stěny kabiny objektu v metrech
	novy->pohon=NULL;//při vložení nemá vložen žádný pohon
	novy->element=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	////
	if(id==3)novy->koty_elementu_offset.x=1;else novy->koty_elementu_offset.x=4*0.6;//odsazení kót elementů v metrech,v kabině POW se kóty vykroslují od hrany kabiny, ne od pohonu
	novy->koty_elementu_offset.y=0.4;
	novy->komora=NULL;//ukazatel na komory
	novy->teplomery=NULL;
	if(id==3)for(short i=1;i<=4;i++)vloz_komoru(novy,2.5,NULL,i%2);//pokud se jedná o POWash,nastaví defaultně 4 stejné komory
	novy->probehla_aktualizace_prirazeni_pohonu=false;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
	novy->zobrazit_koty=true;//proměnná určující, zda se budou zobrzovat kóty
	novy->zobrazit_mGrid=true;//proměnná určující, zda budou zobrazeny mGridy
	novy->uzamknout_nahled=false;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu

	//nově, vkládání bodů + defaultní rozměry různých objektů
	TPointD rozmery_kabiny,konec;
	switch(id)
	{
		case 0:case 9:novy->sirka_steny=0;//navěšování + svěšování
		rozmery_kabiny.x=5;rozmery_kabiny.y=3;break;//navěšování
		case 5:rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//lakovna
		case 6:rozmery_kabiny.x=10;rozmery_kabiny.y=3;break;//vytěkání
		case 7:rozmery_kabiny.x=20;rozmery_kabiny.y=3;break;//sušárna
		case 8:rozmery_kabiny.x=20;rozmery_kabiny.y=6;break;//chlazení
		case 12:novy->sirka_steny=0;
		default: rozmery_kabiny.x=10;rozmery_kabiny.y=6;break;//ostatní
	}
	vloz_bod(X,Y+rozmery_kabiny.y/2.0,novy);vloz_bod(X+rozmery_kabiny.x,Y+rozmery_kabiny.y/2.0,novy);
	vloz_bod(X+rozmery_kabiny.x,Y-rozmery_kabiny.y/2.0,novy);vloz_bod(X,Y-rozmery_kabiny.y/2.0,novy);
	//rotace na požadovanou orientaci
	novy->orientace=novy->orientace_text=F->d.orientace_objektu;
	rotuj_body(X,Y,90-novy->orientace,novy);
	//vložení zarážky na konec;
	if(novy->orientace==90||novy->orientace==270)//vodorovný objekt
		{konec.x=novy->body->dalsi->dalsi->X;konec.y=Y;}
	else
		{konec.x=X;konec.y=novy->body->dalsi->dalsi->Y;}
	TElement *zarazka=vloz_element(novy,MaxInt,konec.x,konec.y,0,F->pom_element);
	zarazka->objekt_n=novy->n;
	zarazka->pohon=NULL;//při vkládání objektu nemůže být přiřazen pohon
	zarazka->orientace=m.Rt90(novy->orientace-90);
	//definice bodů geometrie
	vloz_G_element(zarazka,0,X,Y,0,0,0,0,konec.x,konec.y,novy->orientace);
	zarazka=NULL;delete zarazka;
	//definice pozice názvu kabiny
	switch((int)novy->orientace)
	{
		case 0:novy->Xt=X-rozmery_kabiny.y/2.0;novy->Yt=Y+rozmery_kabiny.x/2.0;break;
		case 90:novy->Xt=X+rozmery_kabiny.x/2.0;novy->Yt=Y+rozmery_kabiny.y/2.0;break;
		case 180:novy->Xt=X+rozmery_kabiny.y/2.0;novy->Yt=Y-rozmery_kabiny.x/2.0;break;
		case 270:novy->Xt=X-rozmery_kabiny.x/2.0;novy->Yt=Y+rozmery_kabiny.y/2.0;break;
	}
	F->ortogonalizace();//nutné před kontrolou změny trendu
	//////nově je vše seřazeno ještě před touto metodou
	if(OBJEKTY->predchozi->n==2 && novy->element->geo.X1==OBJEKTY->dalsi->element->geo.X1 && novy->element->geo.Y1==OBJEKTY->dalsi->element->geo.Y1)// && OBJEKTY->dalsi->orientace==90 || F->d.predchozi_oblast==2 && OBJEKTY->predchozi->n==2)//změna trendu linky, pokud nebylo s prvním objektem rotováno
	{
		double rotace=-180;
		if(OBJEKTY->dalsi->orientace==270)rotace=180;
		rotuj_objekt(OBJEKTY->dalsi,rotace);
		posun_objekt(OBJEKTY->dalsi->element->geo.X1-OBJEKTY->dalsi->element->geo.X4,OBJEKTY->dalsi->element->geo.Y1-OBJEKTY->dalsi->element->geo.Y4,OBJEKTY->dalsi,false,false);
	}
}
//---------------------------------------------------------------------------
//zkopíruje objekt Objekt na konec spojového seznamu Objektů, za předpokladu že p==NULL,
//pokud p není NULL je objekt za tento objekt p ve spojovém seznamů objektů zařazen
//hodnota offsetu je hodnota odsazení zkopírovoaného objektu od objektu vzorového
//index_name slouží pro rozlišení např. LAK, LAK1, LAK2...
//zároveň vrací ukazatel na právě zkopírovaný objekt např. pro další použití
Cvektory::TObjekt *Cvektory::kopiruj_objekt(TObjekt *Objekt,short offsetX,short offsetY,AnsiString index_name,bool remove_pre_index,TObjekt *p)
{
	TObjekt *novy=new TObjekt;
	novy->id=Objekt->id;

	//název
	if(!remove_pre_index || index_name=="1")//pokud se předchozí index nemaže či nezohledňuje, vhodné např. pro kopírování z originálního objektu bez indexu
	{
		novy->short_name=Objekt->short_name+index_name;
		novy->name=Objekt->name+index_name;
	}
	else
	{
		novy->short_name=Objekt->short_name.SubString(1,Objekt->short_name.Length()-1)+index_name;
		novy->name=Objekt->name.SubString(1,Objekt->name.Length()-1)+index_name;
	}

	//souřadnice nového
	novy->X=Objekt->X+offsetX;//přiřadím X osu včetně požadovaného offsetu
	novy->Y=Objekt->Y+offsetY;//přiřadím Y osu  včetně požadovaného offsetu

	//ošetření pokud se jedná o poslední prvek - pro upomínku rozepsáno
	if(Objekt->dalsi==NULL)p=NULL;//tak se vkládá vždy nakonec

	if(p==NULL)//vkládání nakonec
	{                                   //dodělat!!!
		vloz_objekt(Objekt);
		OBJEKTY->predchozi->short_name=novy->short_name;
		OBJEKTY->predchozi->name=novy->name;
		OBJEKTY->predchozi->X=novy->X;
		OBJEKTY->predchozi->Y=novy->Y;
		OBJEKTY->predchozi->Xt=novy->Xt;
		OBJEKTY->predchozi->Yt=novy->Yt;
		kopiruj_body(OBJEKTY->predchozi,novy);
		return OBJEKTY->predchozi;//vrátí poslední ukazatel na prvek seznamu
	}
	else//vkládání mezi objekty
	{                                //dodělat!!!
		novy->rezim=Objekt->rezim;
		novy->sirka_steny=Objekt->sirka_steny;//šířka stěny kabiny objektu v metrech
		novy->pohon=Objekt->pohon;
		novy->element=Objekt->element;
		novy->min_prujezdni_profil=Objekt->min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
		//novy->rozmer_kabiny=Objekt->rozmer_kabiny;//výchozí rozměr kabiny
		novy->koty_elementu_offset=Objekt->koty_elementu_offset;//odsazení kót elementů v metrech
		novy->komora=NULL;
		if(novy->id==3)kopiruj_komory(Objekt,novy);//pokud se jedná o POWash
		novy->probehla_aktualizace_prirazeni_pohonu=Objekt->probehla_aktualizace_prirazeni_pohonu;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
		novy->zobrazit_koty=Objekt->zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty
		novy->zobrazit_mGrid=Objekt->zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy
		novy->uzamknout_nahled=Objekt->uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu

		novy->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		novy->dalsi=p->dalsi;
		p->dalsi->predchozi=novy;
		p->dalsi=novy;
		novy->n=p->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
		nove_indexy();//indexy zvýšit separátně se tady psalo
		return novy;//vrátí ukazatel na posledně kopírovaný objekt
	}
}
//---------------------------------------------------------------------------
//zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze ukazatel na mGrid originálu zůstané propojený
void Cvektory::kopiruj_objekt(TObjekt *Original,TObjekt *Kopie)
{
  F->log(__func__);//logování
	Kopie->n=Original->n;
	Kopie->id=Original->id;
	Kopie->short_name=Original->short_name;
	Kopie->name=Original->name;
	Kopie->X=Original->X;
	Kopie->Y=Original->Y;
	Kopie->Xt=Original->Xt;
	Kopie->Yt=Original->Yt;
	Kopie->Xp=Original->Xp;
	Kopie->Yp=Original->Yp;
	Kopie->orientace=Original->orientace;
	Kopie->orientace_text=Original->orientace_text;
	Kopie->sirka_steny=Original->sirka_steny;
	Kopie->rezim=Original->rezim;
	Kopie->stavPM=Original->stavPM;
	Kopie->pohon=Original->pohon;
	//kopiruj_pohon(Original->pohon,Kopie);//POHON
	Kopie->min_prujezdni_profil=Original->min_prujezdni_profil;
	//Kopie->rozmer_kabiny=Original->rozmer_kabiny;
	Kopie->koty_elementu_offset=Original->koty_elementu_offset;
	Kopie->komora=NULL;//POZOR TOTO NENÍ ZCELA SPRÁVNĚ, MĚLO BY SE NEJDŘÍVE SMAZAT PŘIDRUŽENÝ SPOJÁK, ABY NEZŮSTAL V PAMĚTI
	Kopie->body=NULL;//POZOR TOTO NENÍ ZCELA SPRÁVNĚ, MĚLO BY SE NEJDŘÍVE SMAZAT PŘIDRUŽENÝ SPOJÁK, ABY NEZŮSTAL V PAMĚTI
	Kopie->teplomery=NULL;//musí být před kopírováním
	kopiruj_seznam_teplomery(Original,Kopie);//kopírování záznamů pro teploměry
	if(Kopie->id==3)kopiruj_komory(Original,Kopie);//pokud se jedná o POWash
	Kopie->probehla_aktualizace_prirazeni_pohonu=Original->probehla_aktualizace_prirazeni_pohonu;
	Kopie->zobrazit_koty=Original->zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty
	Kopie->zobrazit_mGrid=Original->zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy
	Kopie->uzamknout_nahled=Original->uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu
	//obě kopírovací metody musí být ke konci
	Kopie->element=Original->element;
  Kopie->element_n=Original->element_n;
	kopiruj_body(Original,Kopie);
}
//---------------------------------------------------------------------------
//ověří, zda se na souřadnicích myši nachází nějaký objekt, pokud ano, vrátí na něj ukazatel, jinak vrátí NULL
Cvektory::TObjekt *Cvektory::PtInObjekt()
{
	//dodělat dle potřeby a vyhýbek
	Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if(O->id!=(unsigned)F->VyID && O->id!=pocet_objektu_knihovny+1 && PtInBody(O))break;
		O=O->dalsi;
	}
	return O;
}
//---------------------------------------------------------------------------
//hledá objekt v dané oblasti                                       //pracuje v logic souradnicich tzn. již nepouživat *Zoom  použít pouze m2px
Cvektory::TObjekt *Cvektory::najdi_objekt(double X, double Y,double offsetX, double offsetY,short typ)//hledá bod v dané oblasti
{
	Cvektory::TObjekt *O=OBJEKTY;//->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if((typ==-1 || typ==(long)O->id) && (long)O->id!=F->VyID && (long)O->id!=pocet_objektu_knihovny+1)
		{
			if(O->X<=X && X<=O->X+offsetX && O->Y>=Y && Y>=O->Y-offsetY){/*F->Memo3->Lines->Add(O->pohon->name);*/return O;}//nalezeno!
		}
		if((typ==-1 || typ==(long)O->id) && ((long)O->id==F->VyID || (long)O->id==pocet_objektu_knihovny+1))//výhybka + spojka
		{
			if(m.PtInCircle(X,Y,O->X,O->Y,offsetX)){return O;}//nalezeno !
		}
		O=O->dalsi;
	}
	return O;
}
//---------------------------------------------------------------------------
//vrátí ret podle toho v jaké jsem oblasti objektu, slouží k přilepování objektu při vkládání, ret = 0 - mimo oblasti, 1 - oblast za objektem, 2 - oblast před objektem
short Cvektory::oblast_objektu(TObjekt *O,double X, double Y)
{
	short ret=0;
	X=m.P2Lx(X);Y=m.P2Ly(Y);
	if(O!=NULL)
	{
		double delka_x=5,delka_y=delka_x;
		TElement *E_pom=vrat_posledni_element_objektu(O);
		if(O->id==0 || O->id==9 || O->body->predchozi->n>4)delka_x=delka_y=2.5;
		if(E_pom->geo.X4-delka_x<X && X<E_pom->geo.X4+delka_x && E_pom->geo.Y4-delka_y<Y && Y<E_pom->geo.Y4+delka_y)ret=1;//oblast před objektem
		if(O->element->geo.X1-delka_x<X && X<O->element->geo.X1+delka_x && O->element->geo.Y1-delka_y<Y && Y<O->element->geo.Y1+delka_y)ret=2;//oblast za objektem
		E_pom=NULL;delete E_pom;
	}
	return ret;
}
//---------------------------------------------------------------------------
//vrátí ukazatel na objekt dle n objektu
Cvektory::TObjekt *Cvektory::vrat_objekt(unsigned int n)
{
	TObjekt *p=OBJEKTY->dalsi;//přeskočí hlavičku
	while (p!=NULL)
	{
		if(p->n==n)break;//akce s ukazatelem
		else p=p->dalsi;//posun na další prvek v seznamu
	}
	return p;
}
//---------------------------------------------------------------------------
//dle X kurzoru myši vrátí z modu procesy (ROMA) ukazatel na aktuální objekt
Cvektory::TObjekt *Cvektory::vrat_objekt_z_roma(int X)
{
//	TObjekt *O=OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//	while (O!=NULL)
//	{
//		if(O->predchozi->obsazenost<X && X<=O->obsazenost)//akce s ukazatelem
//		break;//zastaví další hledání výsledek je O
//		O=O->dalsi;//posun na další prvek v seznamu
//	}
//	return O;
	return NULL;
}

//---------------------------------------------------------------------------
//vrátí rodičovský Objekt daného elementu, In_OBJEKT_akt - zda bude hledat místo daného ostrého objektu v něm
Cvektory::TObjekt *Cvektory::vrat_objekt(TElement *Element,bool In_OBJEKT_akt)
{
	TObjekt *O=vrat_objekt(Element->n),*ret=NULL;
	if(F->OBJEKT_akt!=NULL && In_OBJEKT_akt && O!=NULL && O->n==F->OBJEKT_akt->n)ret=O;
	return ret;
}
//---------------------------------------------------------------------------
//smaze objekt ze seznamu
short int Cvektory::smaz_objekt(TObjekt *Objekt)
{
  unsigned long mazany=Objekt->n;
	//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
	if(Objekt->dalsi!=NULL)//ošetření proti poslednímu prvku
	{
		Objekt->predchozi->dalsi=Objekt->dalsi;
		Objekt->dalsi->predchozi=Objekt->predchozi;
	}
	else//poslední prvek
	{
		if(Objekt->n==1)//pokud je mazaný prvek hned za hlavičkou, v takovémto případě nemůže existovat výhybka ani spojka
		{
			OBJEKTY->predchozi=Objekt->predchozi; //popř hlavička bude ukazovat sama na sebe
			OBJEKTY->dalsi=NULL;
		}
		else
		{
			OBJEKTY->predchozi=Objekt->predchozi;//zapis do hlavičky poslední prvek seznamu
			Objekt->predchozi->dalsi=NULL;
		}
	}
	vymaz_elementy(Objekt);
	vymaz_body(Objekt);
	vymaz_komory(Objekt);
  vymaz_seznam_teplomery(Objekt);
	delete Objekt;Objekt=NULL;//smaže mazaný prvek

	//aktualizace číslování objektů, řešeno zde, metoda nove_indexy slouží při vkládání objektů, není zcela vhodná pro mazání objektu
	TObjekt *O=OBJEKTY->dalsi;
	unsigned long n=1;
	while(O!=NULL)
	{
		O->n=n;
		n++;
    O=O->dalsi;
	}
	delete O;O=NULL;

	//aktualizace číslování elementů->objekt_n
	TElement *E=ELEMENTY->dalsi;
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		if(E->objekt_n>mazany)E->objekt_n--;
		E=dalsi_krok(VYHYBKY,E);//procházení skrze všechny elementy, nejjistější metoda
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);
	delete E;E=NULL;

	return 0;
}
//---------------------------------------------------------------------------
//dle zadaného TT  či případně dalších hodnot zaktualizuje paramametry všech objektů
//typ -2://zaktualizuje přiřazení pohonu k objektu, nutné pokud proběhla změna v pohonech, protože původní jsou smazané
//typ -1://dle zamčených a odemčených hodnot při změně TT
//typ 0://dle zamčených a odemčených hodnot při změně parametrů vozíku
//typ 1://při změně TT změna CT a RD, K a DD zůstává
//typ 2://při změně TT změna K,DD,RD zůstává CT
//typ 3://při změně parametrů vozíku změna DD, RD zůstává K, CT
//typ 4://při změně parametrů vozíku změna u DD, CT zůstává K,RD
//typ 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
void Cvektory::aktualizace_objektu(short typ)
{
//	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
//	while (O!=NULL)
//	{
//		//testovací MB: ShowMessage("Vstup CT:"+AnsiString(O->CT)+" RD:"+AnsiString(O->RD)+" DD:"+AnsiString(O->delka_dopravniku)+" K:"+AnsiString(O->kapacita)+" dV:"+AnsiString(dV)+" mV:"+AnsiString(O->mV));
//    //HELP: DLE ZAMČENÝCH ODEMČENÝCH HODNOT (INDIVIDUÁLNÍ NASTAVENÍ) NUTNÁ ZMĚNA DATOVÉHO MODELU - NUTNO ULOŽIT Z FORMULÁŘE PO, KTERÁ POLOŽKA JE ZAMČENÁ, KTERÁ ODEMČENÁ
//		if(typ==-1)//dle zamčených a odemčených hodnot při změně TT
//		{
//			////při zamčeném CT
//			//doplnit podmínku
//			typ=2;//přesměrování na daný typ
//			////při zamčené K a DD
//			//doplnit podmínku
//			typ=1;//přesměrování na daný typ
//			////při zamčeném RD
//			//doplnit podmínku
//			//doplnit počítání mezer
//		}
//		if(typ==0)//dle zamčených a odemčených hodnot při změně parametrů vozíku
//		{
//			//při zamčeném K či CT
//			typ=3;
//			//při zamčeném K,RD
//			typ=4;
//			//při zamčeném DD
//			typ=5;
//		}
//
//		switch(typ)
//		{
////asi se již nepoužívá
////			case -2://zaktualizuje přiřazení pohonu k objektu, nutné pokud proběhla změna v pohonech, protože původní jsou smazané
////			{
////				if(O->pohon!=NULL)//přiřazuje pouze pokud byl pohon již přiřazen
////				O->pohon=vrat_pohon(O->pohon->n);
////			}
////      break;
//			case 1://při změně TT změna CT a RD, K a DD zůstává
//			{
//				O->CT=PP.TT*O->kapacita;
//				if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
//			}
//			break;
//			case 2://při změně TT změna K,DD,RD zůstává CT mimo S&G
//			{
//				if(O->rezim==0)O->CT=PP.TT;//pro S&G
//				else //pro kontinuál a PP
//				{
//					//K
//					O->kapacita=O->CT/PP.TT;
//					//DD
//					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
//					//RD
//					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
//				}
//			}
//			break;
//			case 3://při změně parametrů vozíku změna DD, RD zůstává K, CT
//			{
//				if(O->rezim!=0)//pro kontinuál a PP
//				{
//					//DD
//					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
//					//RD
//					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
//				}
//			}
//			break;
//			case 4://při změně parametrů vozíku změna u DD, CT zůstává K,RD
//			{
//				if(O->rezim!=0)//pro kontinuál a PP
//				{
//					//DD
//					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//DD
//					//CT
//					if(O->rezim==1)O->CT=O->delka_dopravniku/O->RD;//pro kontinual
//					else//pro PP
//					{
//						O->kapacita=O->delka_dopravniku/(m.UDV(O->orientace)+O->mezera);
//					}
//				}
//			}
//			break;
//			case 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
//			{
//				if(O->rezim!=0)//pro kontinuál a PP
//				{
//					//K
//					O->kapacita=O->delka_dopravniku/(m.UDV(O->orientace)+O->mezera);//K
//					//CT
//					O->CT=PP.TT*O->kapacita;
//					//RD
//					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
//				}
//			}
//			break;
//		}
//		//prozatím zde a takto výpočte počet pozic
//		double P=floor(O->kapacita);//celočíselná kapacita
//		double DV=m.UDV(O->orientace);
//		double DVM=(DV+O->mezera)*(O->kapacita-P);//délka části poslední vozíko-mezery v kabině
//		if(DVM>=DV)P++;//navýší o celý vozík, protože je minimálně celý vozík v kabině
//		else P+=DVM/DV;//navýší o část vozíku, protože je jenom část vozíku v kabině
//		O->pozice=P;
//		//testovací MB:ShowMessage("Výstup CT:"+AnsiString(O->CT)+" RD:"+AnsiString(O->RD)+" DD:"+AnsiString(O->delka_dopravniku)+" K:"+AnsiString(O->kapacita)+" dV:"+AnsiString(dV));
//		O=O->dalsi;//posun na další prvek
//	}
//	O=NULL;delete O;
}
//---------------------------------------------------------------------------
//aktualizuje stav objektu, například po odmazání posledního KK robota objekt nezůstane v KK režimu
void Cvektory::aktualizace_rezimu_objektu(TObjekt *Objekt,bool aktualizovat_sta_mGridu)
{
	//základní nastavění podle typu objektu, 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	Objekt->rezim=-1;
	if(Objekt->id==5 || Objekt->id==6)Objekt->rezim=2;

	//kontrola podle elementů v objektu
	TElement *E=Objekt->element;
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		short druh=vrat_druh_elementu(E);//kontrola druhu elementu
		if(E->eID==0 || E->eID==5 || E->eID==6)druh=-1;
		//pokud je druh elementu KK nebo S&G, zapiš změnu a ukonči
		if(druh>=0)
		{
			Objekt->rezim=druh;//zapsání změny
			break;//ukončení průchodu
		}
		E=dalsi_krok(VYHYBKY,E,Objekt);
	}
	//odstranění průchodového spojáku
	vymaz_seznam_VYHYBKY(VYHYBKY);

	//zkontrolovat zda nejsou v objektu PM nebo výhybky, pokud ano aktualizovat jim editované položky
  if(aktualizovat_sta_mGridu && F->OBJEKT_akt!=NULL)FormX->zmena_rezimu_pohonu(NULL);

	//ukazatelové záležitosti
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//všem objektům, které měly přiřazen pohon s oldN(oldID), přiřadí pohon s newN(newID), podle toho, jak jsou ukládány nově do spojáku, důležité, pokud dojde k narušení pořadí ID resp n pohonů a pořadí jednotlivých řádků ve stringridu, např. kopirováním, smazáním, změnou pořadí řádků atp.
void Cvektory::aktualizace_prirazeni_pohonu_k_objektum(unsigned int oldN,unsigned int newN)
{
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if(O->pohon!=NULL && oldN==O->pohon->n && O->probehla_aktualizace_prirazeni_pohonu==false)//objekt měl přiřazen pohon a ještě nebyl nově přepřiřazen
		{
			O->pohon=vrat_pohon(newN);//danému objektu přiřadíme nové ID resp. n původního pohonu
			O->probehla_aktualizace_prirazeni_pohonu=true;//již se s tímto objektem nebude pracovat v dalších přiřazování dané aktualizace, důležitné např. pro situaci 2->3,3->4 (aby prvně nebyl přiřezn pohon s id 2 na 3 a potom všechny pohony s id 3 na pohon 4, protože měly být přiřazený jen některé...)
		}
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;//odstranění již nepotřebného ukazatele
}
//---------------------------------------------------------------------------
//po dokončení aktualizace přiřazení pohonu (při ukládání pohonu na PL) vrátí atribut probehla_aktualizace_prirazeni_pohonu všech objektů na false, aby bylo připraveno k dalšímu opětovnému užítí, nepřímo spolupracuje s metodou výše uvedenou aktualizace_prirazeni_pohonu_k_objektum
void Cvektory::aktualizace_prirazeni_pohonu_dokoncena()
{
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		O->probehla_aktualizace_prirazeni_pohonu=false;
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;//odstranění již nepotřebného ukazatele
}
//---------------------------------------------------------------------------
//sečte délky jednotlivých objektů
double Cvektory::vrat_soucet_delek_vsech_objektu()
{
//	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	double SUM=0.0;
//	while (O!=NULL)
//	{
//		SUM+=O->delka_dopravniku;
//		O=O->dalsi;//posun na další prvek
//	}
	return SUM;
}
//---------------------------------------------------------------------------
//vrátí počet objektů v požadovaném režimu, nebo v případě implicitního parametru -1 objekty ve všech režimech
unsigned int Cvektory::pocet_objektu(short typ)
{
	unsigned int pocet=0;
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{                     //celkový počet
		if(O->rezim==typ || typ==-1)pocet++;
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
	return pocet;
}
//---------------------------------------------------------------------------
//vrátí počet objektů v požadovaném režimu pro danou zakázku, nebo v případě implicitního parametru -1 objekty ve všech režimech pro danou zakázku
unsigned int Cvektory::pocet_objektu_zakazky(TZakazka *Zakazka,short typ)
{                    /////////////SMAZAT
//	unsigned int pocet=0;
//	TCesta *C=Zakazka->cesta->dalsi;//přeskočí hlavičku
//	while (C!=NULL)
//	{                     //celkový počet
//		if(C->objekt->rezim==typ || typ==-1)pocet++;
//		C=C->dalsi;//posun na další prvek
//	}
//	C=NULL;delete C;
//	return pocet;
	return 0;
}
//---------------------------------------------------------------------------
//vrátí AnsiString řetezec shortname či name (dle prvního parametru, který je implicitně na shortname=true) seznam objektů, které nemají přiřazený pohon, jednotlivé názvy objektů oddělí dle paramaterů seperátor, implicitně ", " tj. čárka a mezera, v případě že žádný objekt nenajde, vrátí prázdný řetězec
AnsiString Cvektory::vypsat_objekty_bez_prirazenych_pohonu(bool shortname,AnsiString separator)
{
	AnsiString T="";
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if(O->pohon==NULL)//pohon nepřiřazen
		{
			if(shortname)T+="<b>"+O->short_name+"</b>";//vypsat krátký název
			else T+="<b>"+O->name+"</b>";//vypsat celý název
			T+=separator;
		}
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
	if(T!="")T=T.SubString(1,T.Length()-separator.Length());//ještě odebere poslední seperator
	return T;
}
//---------------------------------------------------------------------------
//vrátí počet objektů bez či s přiřazenými pohony (dle vstupního parametru), parametr režim ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
unsigned long Cvektory::vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(bool s,short rezim)
{
	unsigned long RET=0;
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		if(O->rezim==rezim || rezim==-1)
		{
			if(s==true && O->pohon!=NULL)RET++;//pohon přiřazen
			if(s==false && O->pohon==NULL)RET++;//pohon nepřiřazen
		}
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
	return RET;
}
//---------------------------------------------------------------------------
//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů, které mají přiřazený pohon bez uvedené rozteče jednotlivé názvy objektů oddělí  ", " tj. čárkou a mezerou, v případě že žádný objekt nenajde, vrátí prázdný řetězec, pozor pohony bez přiřazení k objektům nevypisuje
AnsiString Cvektory::vypis_objekty_s_pohony_bez_roztece(bool shortname)
{
	AnsiString T="";
	TPohon *P=POHONY->dalsi;//přeskočí hlavičku
	while (P!=NULL)
	{
		if(P->roztec==0)//pohon neobsahuje uvedenou rozteč
		{
			AnsiString To=vypis_objekty_vyuzivajici_pohon(P->n,shortname);
			if(To!="")T+="<b>"+P->name+"</b> (přiřazen objektům: "+To+"),";//pouze pokud je pohon přiřazen
		}
		P=P->dalsi;//posun na další prvek
	}
	P=NULL;delete P;
	T=T.SubString(1,T.Length()-1);//odstraní poslední mezeru
	return T;
}
//---------------------------------------------------------------------------
//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů podle zakázek, které nemají 100% vytížení
AnsiString Cvektory::vypis_objekty_mimo_100vytizeni(bool shortname, bool vypsat_procetna, AnsiString separator)
{                    //////////////SMAZAT?
//	 TZakazka *Z=ZAKAZKY->dalsi;
	 AnsiString T="";
//	 while (Z!=NULL)
//	 {
//		TCesta *C=ZAKAZKY->cesta->dalsi;
//		while (C!=NULL)
//		{
//			double vytizeni=C->CT/C->objekt->kapacita/Z->TT*100.0;
//			if(vytizeni!=100)
//			{
//				AnsiString N=C->objekt->name;if(shortname)C->objekt->short_name;
//				AnsiString V=""; if(vypsat_procetna)"- "+AnsiString(vytizeni)+" %";
//				T=Z->name+"/"+N+V+separator;
//			}
//			C=C->dalsi;
//		}
//		delete C;
//		Z=Z->dalsi;
//	 }
//	 delete Z;
//	 T=T.SubString(1,T.Length()-separator.Length());//odebere ještě poslední separátor
	 return T;
}
//---------------------------------------------------------------------------
//najde minimální rychlost pohonu (tj. totál min RD) ze všech použitých pohonů, pomůcka pro stanovení referenční rychlosti animace
double Cvektory::vrat_min_rychlost_prejezdu()
{
	double minRD=0.0;
	TPohon *P=POHONY->dalsi;//přeskočí hlavičku a výše uvedený
	if(P!=NULL)minRD=P->aRD;
	while (P!=NULL)
	{
		if(P->aRD<minRD)minRD=P->aRD;
		P=P->dalsi;//posun na další prvek
	}
	P=NULL;delete P;
	return minRD;
}
//---------------------------------------------------------------------------
//změní zařazení objektů ve spojovém seznamu
//přetížená funkce
void Cvektory::zmen_poradi_objektu(unsigned long aktualni_poradi,unsigned long nove_poradi)
{
	 zmen_poradi_objektu(vrat_objekt(aktualni_poradi),vrat_objekt(nove_poradi));
}
//změní zařazení objektů ve spojovém seznamu
void Cvektory::zmen_poradi_objektu(TObjekt *aktualni_poradi,TObjekt *nove_poradi)
{
	if(aktualni_poradi->n!=nove_poradi->n && nove_poradi->n<=OBJEKTY->predchozi->n)//pokud jsou zadaná pořadí rozdílná a zároveň není zadáváno větší číslo než n posledního prvku
	{
		TObjekt *ukaz_ap=aktualni_poradi;
		TObjekt *ukaz_np=nove_poradi;//musí být tu, aby byl uložen aktuální seznam, ne po vyjmutí
		if(ukaz_ap!=NULL && ukaz_np!=NULL)
		{
			//vyjmutí ze spojáku
			if(ukaz_ap->dalsi!=NULL)//pokud se nejedná o poslední prvek
			{
				ukaz_ap->predchozi->dalsi=ukaz_ap->dalsi;//předchozí bude ukazovat na následující
				ukaz_ap->dalsi->predchozi=ukaz_ap->predchozi; //následující bude ukazovat na další
			}
			else//pokud se jedná o poslední prvek
			{
				OBJEKTY->predchozi=ukaz_ap->predchozi;
				ukaz_ap->predchozi->dalsi=NULL;
			}
			//vložení na novou pozici
			if(aktualni_poradi->n<nove_poradi->n)//vložení za ukaz_np
			{
				ukaz_ap->predchozi=ukaz_np;
				if(ukaz_np->dalsi!=NULL)//pokud se nejedná o poslední prvek
				{
					ukaz_ap->dalsi=ukaz_np->dalsi;
					ukaz_np->dalsi->predchozi=ukaz_ap;
				}
				else//pokud se jedná o poslední prvek
				{
					ukaz_ap->dalsi=NULL;
					OBJEKTY->predchozi=ukaz_ap;
				}
				ukaz_np->dalsi=ukaz_ap;
			}
			else//vložení před ukaz_np
			{
				ukaz_np->predchozi->dalsi=ukaz_ap;//přechozí budu ukazovat na vkládaný
				ukaz_ap->predchozi=ukaz_np->predchozi; //vkládaný bude mít jako předchozí původní předchozí
				ukaz_ap->dalsi=ukaz_np;//vkládaný bude ukazovat na následující na původní
				ukaz_np->predchozi=ukaz_ap;//původní bude mít před sebou vkládaný
			}
		}
		//přeindexování (N-hodnoty) v celém seznamu, možno řešit sepáratáně, ale takto to bylo rychleji napsané
		TObjekt *ukaz=OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
		unsigned long i=1;
		while (ukaz!=NULL)
		{
			ukaz->n=i++;
			ukaz=ukaz->dalsi;
		}
	}
}
//---------------------------------------------------------------------------
//projde všechny objekty a nastavý nové indexy podle aktuálního pořadí objektů, použití např. po kopírování objektu
void Cvektory::nove_indexy(bool nasledne_zmena_nazvu)
{
	TObjekt *O=OBJEKTY->dalsi;
	unsigned long n=1;;
	while(O!=NULL)
	{
		//načtení všech elementů objektu do pomocného spojáku, pro účely změny objekt_n
		TElement *E=O->element;
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		T2Element *zmena=new T2Element,*novy=NULL;;zmena->n=0;zmena->vyhybka=NULL;zmena->dalsi=NULL;zmena->predchozi=zmena;
		while(E!=NULL)
		{
			novy=new T2Element;
			novy->n=zmena->predchozi->n+1;
			novy->vyhybka=E;
			novy->predchozi=zmena->predchozi;
			zmena->predchozi->dalsi=novy;
			zmena->predchozi=novy;
			novy->dalsi=NULL;
			E=dalsi_krok(VYHYBKY,E,O);//E->dalsi;
		}
		//změna n objektu
		O->n=n;
		n++;
		//změna objekt_n u elementů objektu, musí být řešeno na 2x, pri realizaci první změna n objektu a pak průchod skrze jeho elementy (metoda průchodu nefunguje správně), realizace změna objekt_n u elementů a následná změna n objektu ... pam. chyba
		do
		{
	  	novy=zmena->predchozi;
			novy->vyhybka->objekt_n=O->n;//přepsání objekt_n
	  	//vyřazení záznamu se seznamu smazat
	  	zmena->predchozi=novy->predchozi;
	  	zmena->predchozi->dalsi=NULL;
	  	//smazání záznamu
	  	delete novy;novy=NULL;
	  	//pokud jsem na hlavičce seznamu smazat, smažu hlavičku a cyklus se ukončí
	  	if(zmena->predchozi->n==0)
			{
	  		delete zmena;
	  		zmena=NULL;
			}
		}while(zmena!=NULL);
		//ukazatelové záležitosti
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E;
		O=O->dalsi;
	}
	delete O;O=NULL;
}
//---------------------------------------------------------------------------
//ortogonalizuje schéma
void Cvektory::ortogonalizovat()
{
	if(OBJEKTY->predchozi->n>1)//ortogonalizace až od dvou prvků
	{
			//mimo posledního prvku
			TObjekt *O=OBJEKTY->dalsi;
			while(O->dalsi!=NULL)
			{
				 double A=m.azimut(O->X,O->Y,O->dalsi->X,O->dalsi->Y);
				 double D=m.delka(O->X,O->Y,O->dalsi->X,O->dalsi->Y);
				 if(315<A || A<=45)//A==0 - nahoru
				 {
					O->dalsi->X=O->X;
					if(Form1->prichytavat_k_mrizce==1)O->dalsi->Y=m.round((O->Y+D)/(Form1->size_grid*1.0*Form1->m2px))*Form1->size_grid*Form1->m2px;
					else O->dalsi->Y=O->Y+D;
				 }
				 if(45<A && A<=135)//A==90 - doprava
				 {
					if(Form1->prichytavat_k_mrizce==1)O->dalsi->X=m.round((O->X+D)/(Form1->size_grid*1.0*Form1->m2px))*Form1->size_grid*Form1->m2px;
					else O->dalsi->X=O->X+D;
					O->dalsi->Y=O->Y;
				 }
				 if(135<A && A<=255)//A==180 - dolu
				 {
					O->dalsi->X=O->X;
					if(Form1->prichytavat_k_mrizce==1)O->dalsi->Y=m.round((O->Y-D)/(Form1->size_grid*1.0*Form1->m2px))*Form1->size_grid*Form1->m2px;
					else O->dalsi->Y=O->Y-D;
				 }
				 if(225<A && A<=315)//A==270 - doleva
				 {
					if(Form1->prichytavat_k_mrizce==1)O->dalsi->X=m.round((O->X-D)/(Form1->size_grid*1.0*Form1->m2px))*Form1->size_grid*Form1->m2px;
					else O->dalsi->X=O->X-D;
					O->dalsi->Y=O->Y;
				 }
				 O=O->dalsi;
			}
			O=NULL;delete O;

			//ošetření a dorovnání posledního prvku pokud jsou minimálně čtyři prkvy
			if(OBJEKTY->predchozi->n>=4)
			{
				TObjekt *O1=OBJEKTY->dalsi;//první
				TObjekt *Op=OBJEKTY->predchozi;//poslední, tj. akutáulní
				TObjekt *Opp=OBJEKTY->predchozi->predchozi;//předposlední
				double A=m.azimut(O1->X,O1->Y,Opp->X,Opp->Y);//první vs. předposlední
				if(m.round(A)%90)//pokud se NEjedná o náseobek 90° je nutné řešit touto větví, zajistí, že budou do 90°
				{
					if(Op->X==Opp->X)Op->Y=O1->Y;
					if(Op->Y==Opp->Y)Op->X=O1->X;
				}
				else//pokud je první a předchozí v jedné linii, umístí dopřestřed mezi prvky
				{
					double B=m.azimut(Opp->X,Opp->Y,Opp->predchozi->X,Opp->predchozi->Y);
					double C=m.azimut(Op->X,Op->Y,Opp->X,Opp->Y);
					//ale za předpokladu, že je je první, předposlední a předpředposlední v pravém úhlu
					if(fabs(A-B)==90.0)//vloží mezi
					{
					 Op->X=(O1->X+Opp->X)/2;
					 Op->Y=(O1->Y+Opp->Y)/2;
					}
					else
					{
						if(A==C)//pokud by byly přes sebe tam a zpět
						{
							if(A==90 || A==270)//přes sebe horizontálně tam a zpět
							{
								short smer=-1;//pod
								if(m.P2Ly(Form1->akt_souradnice_kurzoru_PX.y)>O1->Y)smer=1;//nad //pokud je náznak vkládání nad
								Op->Y+=fabs(Op->X-Opp->X)*smer;
								Op->X=Opp->X;
							}
							if(A==0 || A==180)//přes sebe vertikalně tam a zpět
							{
								short smer=+1;//v pravo
								if(m.P2Lx(Form1->akt_souradnice_kurzoru_PX.x)<O1->X)smer=-1;//vlevo //pokud je náznak vkládání doleva
								Op->X+=fabs(Op->Y-Opp->Y)*smer;
								Op->Y=Opp->Y;
							}
						}
					}
				}
				O1=NULL;delete O1;
				Op=NULL;delete Op;
				Opp=NULL;delete Opp;
			}
			//ošetření pokud se jedná o tři prvky a jsou všechny v jedné linii a zároveň poslední prvek by byl mezi
			if(OBJEKTY->predchozi->n==3)
			{                                                                                                  //je mezi
				if(OBJEKTY->dalsi->X==OBJEKTY->dalsi->dalsi->X && OBJEKTY->predchozi->X==OBJEKTY->dalsi->X && (OBJEKTY->dalsi->Y<=OBJEKTY->predchozi->Y && OBJEKTY->predchozi->Y<=OBJEKTY->dalsi->dalsi->Y || OBJEKTY->dalsi->Y>=OBJEKTY->predchozi->Y && OBJEKTY->predchozi->Y>=OBJEKTY->dalsi->dalsi->Y))
				{
					short smer=+1;//v pravo
					if(m.P2Lx(Form1->akt_souradnice_kurzoru_PX.x)<OBJEKTY->dalsi->X)//pokud je náznak vkládání doleva
					smer=1;//v levo
					OBJEKTY->predchozi->X+=fabs(OBJEKTY->dalsi->dalsi->Y-OBJEKTY->predchozi->Y)*smer;
					OBJEKTY->predchozi->Y=OBJEKTY->dalsi->dalsi->Y;
				}                                                                                              //je mezi
				if(OBJEKTY->dalsi->Y==OBJEKTY->dalsi->dalsi->Y && OBJEKTY->predchozi->Y==OBJEKTY->dalsi->Y && (OBJEKTY->dalsi->X<=OBJEKTY->predchozi->X && OBJEKTY->predchozi->X<=OBJEKTY->dalsi->dalsi->X || OBJEKTY->dalsi->X>=OBJEKTY->predchozi->X && OBJEKTY->predchozi->X>=OBJEKTY->dalsi->dalsi->X))
				{
					short smer=-1;//pod
					if(m.P2Ly(Form1->akt_souradnice_kurzoru_PX.y)>OBJEKTY->dalsi->Y)//pokud je náznak vkládání nad
					smer=1;//nad
					OBJEKTY->predchozi->Y+=fabs(OBJEKTY->dalsi->dalsi->X-OBJEKTY->predchozi->X)*smer;
					OBJEKTY->predchozi->X=OBJEKTY->dalsi->dalsi->X;
				}
			}
	}
}
//---------------------------------------------------------------------------
//vymaže spojový seznam technologických objektů včetně bodů, přidružených elementů a případných komor z paměti
long Cvektory::vymaz_seznam_OBJEKTY()
{
	long pocet_smazanych_objektu=0;
	while (OBJEKTY!=NULL)
	{
		vymaz_body(OBJEKTY->predchozi);
		vymaz_komory(OBJEKTY->predchozi);
		vymaz_seznam_teplomery(OBJEKTY->predchozi);
		//vymaz_elementy(OBJEKTY->predchozi);
		pocet_smazanych_objektu++;
		OBJEKTY->predchozi=NULL;
		delete OBJEKTY->predchozi;
		OBJEKTY=OBJEKTY->dalsi;
	};

	return pocet_smazanych_objektu;
};
////---------------------------------------------------------------------------
//slouží k posunu objektu jako celku o X a Y, posun kabiny, pohonu, elementů, tabulek, nadpisu, kontrolovat_oblast slouží k nucenému přesunutí
void Cvektory::posun_objekt(double X,double Y,TObjekt *Objekt,bool kontrolovat_oblast,bool povolit_rotaci)
{
	short oblast=0;
	if(F->prichytavat_k_mrizce!=1)kontrolovat_oblast=false;
	if(kontrolovat_oblast)
	{
		if(Objekt->element->predchozi->n>0 && (Objekt->element->predchozi->eID!=300 || (Objekt->element->predchozi->eID==300 && Objekt->element->predchozi->dalsi==Objekt->element)))oblast=oblast_objektu(vrat_objekt(Objekt->element->predchozi->objekt_n),F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y);
		if(oblast==2)oblast==0;//vyřezení oblasti za
		X=F->akt_souradnice_kurzoru.x-Objekt->element->geo.X1; Y=F->akt_souradnice_kurzoru.y-Objekt->element->geo.Y1;
	}
	if(oblast==0)
	{
		////posun kabiny-polygonu
		posun_body(X,Y,Objekt);
		////posun nadpisu
		Objekt->Xt+=X;
		Objekt->Yt+=Y;
		////posun tabulky pohonů
		if(Objekt->Xp>0 && Objekt->Yp>0)
		{
			Objekt->Xp+=X;
			Objekt->Yp+=Y;
		}
		////posun elementů
		TElement *E=Objekt->element;//objekt má vždy element (zarážka)
    T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		while(E!=NULL && E->objekt_n==Objekt->n)
		{
			E->X+=X;E->Y+=Y;//souřadnice elementu
			if(E->Xt!=-100)E->Xt+=X;E->Yt+=Y;//souřadnice tabulky + kontrola zda je vytvořená
			//geometrie elementu
			E->geo.X1+=X;E->geo.X2+=X;E->geo.X3+=X;E->geo.X4+=X;
			E->geo.Y1+=Y;E->geo.Y2+=Y;E->geo.Y3+=Y;E->geo.Y4+=Y;
			E=dalsi_krok(VYHYBKY,E,Objekt);
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E;
	}
	////přilepení objektu na předchozí objekt
	TElement *E_pom=NULL;
	if(Objekt->element->predchozi->n>0)E_pom=Objekt->element->predchozi;
	if(E_pom!=NULL && F->prichytavat_k_mrizce==1 && oblast==1 && (Objekt->element->geo.X1!=E_pom->geo.X4 || Objekt->element->geo.Y1!=E_pom->geo.Y4))
	{
		posun_objekt(E_pom->geo.X4-Objekt->element->geo.X1,E_pom->geo.Y4-Objekt->element->geo.Y1,Objekt,false);
	}
	////změna rotace
	if(E_pom!=NULL && Objekt->n>1 && povolit_rotaci)
	{
		double azimut=0,x=F->akt_souradnice_kurzoru.x,y=F->akt_souradnice_kurzoru.y;
		azimut=m.Rt90(m.azimut(E_pom->geo.X4,E_pom->geo.Y4,x,y));
		if(m.Rt90(azimut+180)!=Objekt->predchozi->orientace || E_pom->eID==300)rotuj_objekt(Objekt,Objekt->orientace-azimut);
	}
	E_pom=NULL;delete E_pom;
}
////---------------------------------------------------------------------------
//orotuje objekt o danou rotaci
void Cvektory::rotuj_objekt(TObjekt *Objekt, double rotace)
{
	if(rotace!=0)
	{
		double azimut=Objekt->orientace-rotace;
		if(azimut<0)azimut=270;//ošetření proti přetotočení
		if(azimut==360)azimut=0;//ošetření proti přetotočení
		TPointD Bod;
		////rotace kabiny-polygonu
		rotuj_body(Objekt->element->geo.X1,Objekt->element->geo.Y1,rotace,Objekt);
		////rotace elementů
		TElement *E=Objekt->element;//objekt má vždy element (zarážka)
		while(E!=NULL && E->objekt_n==Objekt->n)
		{
			if(E->orientace==m.Rt90(Objekt->orientace-90))E->orientace=m.Rt90(azimut-90);//zapsání nové orientace do elementu
			else E->orientace=m.Rt90(azimut+90);
			//souřadnice elementu
			Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->X,E->Y,rotace);
			E->X=Bod.x;E->Y=Bod.y;
			//souřadnice tabulky + kontrola zda je vytvořená
			if(E->Xt!=-100)
			{
				Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->Xt,E->Yt,rotace);
				E->Xt=Bod.x;E->Yt=Bod.y;
			}
			//geometrie elementu
			Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->geo.X1,E->geo.Y1,rotace);
			E->geo.X1=Bod.x;E->geo.Y1=Bod.y;
			Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->geo.X2,E->geo.Y2,rotace);
			E->geo.X2=Bod.x;E->geo.Y2=Bod.y;
			Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->geo.X3,E->geo.Y3,rotace);
			E->geo.X3=Bod.x;E->geo.Y3=Bod.y;
			Bod=m.rotace(Objekt->element->geo.X1,Objekt->element->geo.Y1,E->geo.X4,E->geo.Y4,rotace);
			E->geo.X4=Bod.x;E->geo.Y4=Bod.y;
			E->geo.orientace=m.Rt90(E->geo.orientace-rotace);
			E=E->dalsi;
		}
		E=NULL;delete E;
		//rotace nadpisu
		TPointD bod;
		TRect oblast=F->vrat_max_oblast(Objekt,true);
		switch((int)azimut)
		{
			case 0:bod.x=oblast.left;bod.y=oblast.top-(oblast.top-oblast.bottom)/2.0;break;
			case 90:bod.x=oblast.left+(oblast.right-oblast.left)/2.0;bod.y=oblast.top;break;
			case 180:bod.x=oblast.right;bod.y=oblast.bottom-(oblast.bottom-oblast.top)/2.0;break;
			case 270:bod.x=oblast.right+(oblast.left-oblast.right)/2.0;bod.y=oblast.top;break;
      default:break;
		}
		bod=m.P2L(TPoint(bod.x,bod.y));
		Objekt->Xt=bod.x;Objekt->Yt=bod.y;
		Objekt->orientace=Objekt->orientace_text=azimut;
		F->duvod_validovat=2;//je přímý důvod k validaci
	}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//KOMORY
//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, nastaví velikost dané komory dle proměnné velikost,typ;//1-se sprchou, 0 bez jen okap
void Cvektory::vloz_komoru(TObjekt *Objekt,double velikost,TKomora *ZaKomoru,short typ)
{
	TKomora *Komora=new TKomora;
	Komora->velikost=velikost;
	Komora->typ=typ;
	vloz_komoru(Objekt,Komora,ZaKomoru,typ);
}
//---------------------------------------------------------------------------
//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, není třeba nastavovat ukazatele ani n-pořadí
void Cvektory::vloz_komoru(TObjekt *Objekt,TKomora *Komora,TKomora *ZaKomoru,short typ)
{
	if(Komora!=NULL)
	{
		////pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(Objekt->komora==NULL)
		{
			Objekt->komora=new TKomora;
			Objekt->komora->n=0;
			Objekt->komora->typ=0;
			Objekt->komora->predchozi=Objekt->komora;//hlavička ukazuje sama na sebe
			Objekt->komora->dalsi=NULL;
		}
		////nastavení typu komory
		Komora->typ=typ;
		////vložení nové komory na konec seznamu komor
		if(ZaKomoru==NULL || ZaKomoru!=NULL && ZaKomoru==Objekt->komora->predchozi)//pokud se má vkládat nakonec
		{
			Komora->n=Objekt->komora->predchozi->n+1;//navýšení počítadla
			Komora->predchozi=Objekt->komora->predchozi;//nová komora ukazuje na poslední prvek ve spojaku jako na prvek předchozí
			Komora->dalsi=NULL;//nová komora neukazuje na žádný další prvek, resp. ukazuje na NULL
			Objekt->komora->predchozi->dalsi=Komora;//za poslední prvek vloží novou poslední komoru
			Objekt->komora->predchozi=Komora;//hlavička ukazuje již na novou komoru jako poslední prvek
		}
		else////vložení mezi komory
		{
			//nastavení počítadla u vkládané komory
			Komora->n=ZaKomoru->n+1;
			//nové ukazatelové propojení
			ZaKomoru->dalsi->predchozi=Komora;//následující komoře přídá ukaztel na předchozí na vkladanou
			Komora->dalsi=ZaKomoru->dalsi;//vkládaná ukazuje na původní následují
			Komora->predchozi=ZaKomoru;
			ZaKomoru->dalsi=Komora;//za požadovanou komoru se vloží vkládaná komora
			//navýšení počítadla u následujícíh komor
			TKomora *K=Komora->dalsi;
			while(K!=NULL)
			{
				K->n++;
				K=K->dalsi;
			}
			K=NULL;delete K;
		}
	}
}
//---------------------------------------------------------------------------
//na aktuálních souřadnicích myši hledá komoru, pokud je nalezena vrátí na ni ukazatel
Cvektory::TKomora *Cvektory::najdi_komoru(TObjekt* Objekt)
{
	TKomora *K=Objekt->komora->dalsi;//přeskočení hlavičky
	double vzdalenost=0;
	bool nalezeno=false;
	TRect oblast;
	//zjištění max oblasti
	oblast.left=MaxInt;oblast.right=MaxInt*(-1);
	oblast.top=MaxInt;oblast.bottom=MaxInt*(-1);
	TBod *B=Objekt->body->dalsi;
	while(B!=NULL)
	{
		if(m.L2Px(B->X)<oblast.left)oblast.left=m.L2Px(B->X);
		if(m.L2Px(B->X)>oblast.right)oblast.right=m.L2Px(B->X);
		if(m.L2Py(B->Y)<oblast.top)oblast.top=m.L2Py(B->Y);
		if(m.L2Py(B->Y)>oblast.bottom)oblast.bottom=m.L2Py(B->Y);
		B=B->dalsi;
	}
	delete B;B=NULL;
  //kontrola oblasti komor
	while(K!=NULL)
	{
		switch((int)Objekt->orientace)
		{
			case 0:if(Objekt->element->geo.Y1+vzdalenost<=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y<=Objekt->element->geo.Y1+vzdalenost+K->velikost && oblast.left<F->akt_souradnice_kurzoru_PX.x && oblast.right>F->akt_souradnice_kurzoru_PX.x)nalezeno=true;break;
			case 90:if(Objekt->element->geo.X1+vzdalenost<=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x<=Objekt->element->geo.X1+vzdalenost+K->velikost && oblast.top<F->akt_souradnice_kurzoru_PX.y && oblast.bottom>F->akt_souradnice_kurzoru_PX.y)nalezeno=true;break;
			case 180:if(Objekt->element->geo.Y1-vzdalenost>=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y>=Objekt->element->geo.Y1-vzdalenost-K->velikost && oblast.left<F->akt_souradnice_kurzoru_PX.x && oblast.right>F->akt_souradnice_kurzoru_PX.x)nalezeno=true;break;
			case 270:if(Objekt->element->geo.X1-vzdalenost>=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x>=Objekt->element->geo.X1-vzdalenost-K->velikost && oblast.top<F->akt_souradnice_kurzoru_PX.y && oblast.bottom>F->akt_souradnice_kurzoru_PX.y)nalezeno=true;break;
		}
		vzdalenost+=K->velikost;//přičtení šířky další komory
		if(!nalezeno)K=K->dalsi;
		else break;
	}
	return K;
}
//---------------------------------------------------------------------------
//ověří zda se na daných fyzických souřadnicích nachází kóta komory, pokud neexistují komory vrací -2, pokud se nenachází na kótě vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty, pozn. oblast kóty se testuje až jako poslední
short Cvektory::PtInKota_komory(TObjekt *Objekt,long X,long Y)
{
	short RET=-1;//nic nenalezeno
	double x=F->akt_souradnice_kurzoru_PX.x,y=F->akt_souradnice_kurzoru_PX.y;//souřadnice kurzoru jsou neměnné po celou dobu metody, důležité použít fyzické souřadnice !!
	if(Objekt->komora!=NULL)
	{
  	TKomora *K=Objekt->komora->dalsi;//přeskočení hlavičky, vždy budou minimálně 2 komory
  	while(K!=NULL)
  	{
  		if(K->kota.rect1.PtInRect(TPoint(X,Y))){RET=1;F->pom_komora=K;break;}//hodnoty kóty
  		else
  		{
  			if(K->kota.rect2.PtInRect(TPoint(X,Y))){RET=2;F->pom_komora=K;break;}//jednotky kóty
				else if(m.LeziVblizkostiUsecky(x,y,K->kota.rect0.left,K->kota.rect0.top,K->kota.rect0.right,K->kota.rect0.bottom)<=F->Zoom){RET=0;F->pom_komora=K;break;}//oblast kóty
  		}
  		K=K->dalsi;
  	}
		K=NULL;delete K;
	}
	else RET=-2;//neexistují komory
	return RET;
}
//---------------------------------------------------------------------------
//vrátí součet velikostí všech komor, nebo velikosti do konkrétní komory (včetně)
double Cvektory::vrat_velikosti_komor(TKomora *po)
{
  double ret=0;
	TKomora *K=F->OBJEKT_akt->komora->dalsi;
	while(K!=NULL)
	{
		ret+=K->velikost;
		if(po!=NULL&&po->n==K->n)break;
		K=K->dalsi;
	}
	K=NULL;delete K;
	return ret;
}
//---------------------------------------------------------------------------
//zkopíruje komory včetně jejich velikosti z originálu na kopii bez ukazatelového propojení
void Cvektory::kopiruj_komory(TObjekt *Original,TObjekt *Kopie)
{
	//pokud kopie obsahuje původní komory, tak je smaže
	vymaz_komory(Kopie);
	//samotné nakopírování
	TKomora *K=Original->komora->dalsi;//přeskočí hlavičku
	while(K!=NULL)
	{
		vloz_komoru(Kopie,K->velikost,NULL,K->typ);
		K=K->dalsi;
	}
	K=NULL;delete K;
}
//---------------------------------------------------------------------------
//slouží ke změně pořadí komor
void Cvektory::presun_komoru(TObjekt *Objekt,TKomora *aktKomora,TKomora *za)
{
	if(za!=NULL)if(aktKomora->n!=za->n)//ošetření, parametr za je vrace funkcí najdi_komoru (může být NULL nebo stejná komora)
	{
		//odstranění aktKomory ze seznamu
		if(aktKomora->dalsi==NULL)//poslední komora
		{
			Objekt->komora->predchozi=aktKomora->predchozi;
			aktKomora->predchozi->dalsi=NULL;
		}
		else if(aktKomora->n==1)//první komora
		{
			Objekt->komora->dalsi=aktKomora->dalsi;
			aktKomora->dalsi->predchozi=Objekt->komora;
		}
		else//ostatní komory
		{
			aktKomora->dalsi->predchozi=aktKomora->predchozi;
			aktKomora->predchozi->dalsi=aktKomora->dalsi;
    }
		//vložení na nové místo
    aktKomora->dalsi=za->dalsi;
		aktKomora->predchozi=za;
		if(za->dalsi==NULL)Objekt->komora->predchozi=aktKomora;
		else za->dalsi->predchozi=aktKomora;
		za->dalsi=aktKomora;
		//přeindexování
		TKomora *K=Objekt->komora->dalsi;
		int n=1;
		while(K!=NULL)
		{
			K->n=n;
			n++;
			K=K->dalsi;
		}
		K=NULL;delete K;
	}
}
//---------------------------------------------------------------------------
//smaže konkrétní komoru daného objektu
void Cvektory::smaz_komoru(TObjekt *Objekt,TKomora *Komora)
{
	if(Komora!=NULL && Komora->n!=0)//pokud existuje a zároveň mimo hlavičky, ta nejde smazat
	{
		if(Komora->dalsi==NULL)//jedná se o poslední prvek (múže být i za hlavičkou)
		{
			Objekt->komora->predchozi=Komora->predchozi;
			Komora->predchozi->dalsi=NULL;
			Objekt->komora->predchozi->velikost+=Komora->velikost;//zvětšení poslední komory o odstraněnou komoru
		}
		else//nejedná se o poslední prvek
		{
			Komora->predchozi->dalsi=Komora->dalsi;
			Komora->dalsi->predchozi=Komora->predchozi;
			Komora->dalsi->velikost+=Komora->velikost;//zvětšení další komory o odstraněnou komoru
		}
		//přeindexování komor od počátku na konec, index n slouží k získání počtu komor v objektu
		Komora=Objekt->komora->dalsi;int n=1;
		while(Komora!=NULL)
		{
			Komora->n=n;
			n++;
			Komora=Komora->dalsi;
		}
		Komora=NULL;delete Komora;
	}
}
//---------------------------------------------------------------------------
//vymaže všechny komory daného objektu včetně hlavičky
void Cvektory::vymaz_komory(TObjekt* Objekt)
{
	if(Objekt->komora!=NULL)
	{
		//maže od zadu dokud nezbyde pouze hlavička
		while(Objekt->komora->dalsi==NULL)smaz_komoru(Objekt,Objekt->komora->predchozi);

		//na závěr ještě smaže hlavičku
		Objekt->komora=NULL;delete Objekt->komora;
	}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//ELEMENTY
void Cvektory::hlavicka_ELEMENTY()
{
	TElement *novy=new TElement;
	novy->n=0;
	novy->eID=0;
	novy->short_name="";
	novy->name="";
	novy->orientace=0;
	novy->rotace_jig=0;
	novy->stav=-1;

	novy->X=0;
	novy->Y=0;
	novy->Z=0;
	novy->Xt=0;
	novy->Yt=0;

	novy->PTotoc=0;
	novy->OTOC_delka=0;
	novy->zona_pred=0;
	novy->zona_za=0;

	novy->WT=0;

	novy->citelna_oblast.rect0=TRect(0,0,0,0);
	novy->citelna_oblast.rect1=TRect(0,0,0,0);
	novy->citelna_oblast.rect2=TRect(0,0,0,0);
	novy->citelna_oblast.rect3=TRect(0,0,0,0);
	novy->citelna_oblast.rect4=TRect(0,0,0,0);

	vloz_G_element(novy,0,0,0,0,0,0,0,0,0,0,0,0,0);
	novy->mGrid=NULL;
	novy->data.PD=0;
	novy->data.orientace_jig_pred=0;
	novy->data.LO1=0;
	novy->data.LO2=0;
	novy->data.LO_pozice=0;
	novy->data.PT1=0;
	novy->data.PT2=0;
	novy->data.WTstop=0;
	novy->data.RT=0;
	novy->data.pocet_pozic=0;
	novy->data.pocet_voziku=0;

	novy->objekt_n=0;
	novy->pohon=NULL;

	novy->sparovany=NULL;
	novy->predchozi=novy;
	novy->predchozi2=NULL;
	novy->dalsi=NULL;
	novy->dalsi2=NULL;
	ELEMENTY=novy;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
Cvektory::TElement *Cvektory::vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y,short orientace,TElement *Ep)
{
	//poslední použité ID
	int cislo_mGrid=vrat_nejvetsi_ID_tabulek(Objekt)+1;
	//alokace paměti
	TElement *novy=new TElement;

	//základní atributy elementu potřebné pro řazení
	novy->eID=eID;
  novy->identifikator_vyhybka_spojka=0;
	novy->X=X;
	novy->Y=Y;
  novy->Z=0;
	novy->Xt=-100;
	novy->Yt=-100;
	novy->orientace=orientace;//důležité pro volání makra m.Rxy, bez tohoto by makro vracelo chybné hodnoty
	novy->dalsi=NULL;
	novy->predchozi=NULL;
	novy->dalsi2=NULL;
	novy->predchozi2=NULL;

	//ukazatelové propojení + řazení
	vloz_element(Objekt,novy,Ep);

	//defaultní data
  novy->data.LO1=1.5;
	novy->OTOC_delka=0;
	novy->zona_pred=0;
	novy->zona_za=0;
	novy->data.LO2=0;
	novy->data.LO_pozice=0;
	novy->data.PT1=0;
	novy->PTotoc=0;
	novy->data.PT2=0;
	novy->WT=0;//čekání na palec
	novy->data.WTstop=0;//čekání na stopce
	novy->data.RT=0;//ryzí reserve time
	novy->data.pocet_voziku=0;
	novy->data.pocet_pozic=0;
	novy->rotace_jig=0;
	if(vrat_druh_elementu(novy)==0){novy->data.pocet_pozic=1;novy->data.pocet_voziku=1;}//S&G elementy
	if(eID==0){novy->data.pocet_pozic=2;novy->data.pocet_voziku=1;}//pouze stopky
	novy->stav=1;
	novy->data.PD=-1;//defaultní stav pro S&G roboty
	novy->objekt_n=Objekt->n;//příslušnost elementu k objektu
	novy->pohon=NULL;//pohon na kterém se nachází element
//	if(novy->predchozi->n>0 && novy->predchozi->objekt_n==Objekt->n && (novy->predchozi->eID!=300 || (novy->predchozi->eID==300 && novy->predchozi->dalsi==novy)))novy->pohon=novy->predchozi->pohon;
//	if(novy->predchozi->n>0 && Objekt->element!=NULL && novy->predchozi->objekt_n!=Objekt->n && (Objekt->element->predchozi->n>0 && novy->predchozi->objekt_n!=Objekt->element->predchozi->objekt_n || Objekt->element->predchozi->n==0))novy->pohon=novy->predchozi->pohon;
//	if(novy==Objekt->element && novy->dalsi!=NULL)novy->pohon=novy->dalsi->pohon;
	if(novy->predchozi->n>0 && novy->predchozi->eID!=200 && novy->predchozi->dalsi==novy)novy->pohon=novy->predchozi->pohon;//například při vkládání gemetri na konec linky
	else if(novy->dalsi!=NULL && novy->dalsi->predchozi==novy)novy->pohon=novy->dalsi->pohon;//pokud definuji sek větev nepřiřazovat pohon
	novy->geo.HeightDepp=0;
	novy->geo.delkaPud=0;
	if(novy->dalsi!=NULL)novy->Z=novy->dalsi->Z;
	else if(novy->predchozi->n>0)novy->Z=novy->predchozi->Z+novy->predchozi->geo.HeightDepp;

	//název
	AnsiString T="";
	switch(eID)
	{
		case 0: T="Stop"; break;//stop stanice
		case 1:case 7:case 11:case 15:case 101:case 105:  T="Robot"; 			          novy->data.PD=0;break;//kontinuální robota
		case 2:case 8:case 12:case 16:case 102:case 106:  T="Robot"; 			          novy->data.PT1=60;break;//robot se stopkou
		case 3:case 9:case 13:case 17:case 103:case 107:  T="Robot"; 			          novy->data.PD=0;novy->OTOC_delka=0.450;novy->zona_pred=0.3;novy->zona_za=0.3;novy->data.LO1=(1.5-novy->OTOC_delka)/2.0;novy->data.LO2=novy->data.LO1;novy->rotace_jig=180;break;//kontinuální robot s pasivní otočí
		case 4:case 10:case 14:case 18:case 104:case 108: T="Robot";								novy->data.PT1=60;novy->PTotoc=20;novy->data.PT2=60;novy->rotace_jig=180; break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 5: T=F->ls->Strings[273]; 										 													novy->OTOC_delka=0.450;novy->zona_pred=0.3;novy->zona_za=0.3;novy->rotace_jig=90;break;//pasivní otoč
		case 6: T=F->ls->Strings[273]; 										 													novy->PTotoc=20;novy->rotace_jig=90;break;//aktivní otoč
		case 100: T=F->ls->Strings[270];break;
		case 200: T=F->ls->Strings[271];break;
		case 300: T="Výhybka";break;
		case 301: T="Spojka";break;
		case MaxInt: T="Zarážka";break;
	}
	if(101<=eID && eID<=108)T=F->ls->Strings[272];//"Operátor";
	if(novy->name=="")//přiřazení názvu pouze v případě, že element žádné nemá, při posuvu je novému elementu přiřazeno jméno
	{
		unsigned int nTyp=vrat_poradi_elementu_do(novy)+1;//pokud se jedná o roboty
		if(novy->eID==300 || novy->eID==301)nTyp=novy->identifikator_vyhybka_spojka;
		novy->name=T+" "+AnsiString(nTyp);//číslování a pojmenovávání zarážek pouze v debug
		novy->short_name=T.SubString(1,3)+AnsiString(nTyp);
	}

	//mGrid elementu
  novy->mGrid=NULL;
	if(F->OBJEKT_akt!=NULL && (Objekt->n==F->OBJEKT_akt->n || novy->eID==MaxInt || novy->eID==300 || novy->eID==301))//stačí nastavovat pouze v náhledu při vloz_element, nová strategie, je mgrid, nekopírovat a používat jenom v OBJEKT_akt, zde však podmínka zda se jedná o OBJEKT_akt nebyla z nějakého důvodu možná
	{
		novy->mGrid=new TmGrid(F);
		novy->mGrid->Tag=6;//ID formu
		novy->mGrid->ID=cislo_mGrid;//novy->n;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
	}                  //číslování mGridů zvlášť!!! z důvodu přehazování elementů

  //pokud se jedná o první element v kabině
	if(Objekt->element==NULL)Objekt->element=novy;
	//ukazatel na spárovaný element
	novy->sparovany=NULL;
	prirad_sparovany_element(novy);//metoda již v sobě ošetřuje, že se bude jednat o stopku
	//návrácení ukazatelele na element k dalšímu použití
	return novy;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu
void  Cvektory::vloz_element(TObjekt *Objekt,TElement *Element,TElement *force_razeni)
{
	Element->predchozi2=NULL;
	Element->dalsi2=NULL;
	TElement *posledni=vrat_posledni_element_objektu(Objekt);
	if(posledni!=NULL)Element->n=posledni->n+1;else Element->n=ELEMENTY->predchozi->n+1;//zjištění a uložení n vkládaného elementu
	Element->objekt_n=Objekt->n;//element ještě nemá tento atribut přiřazený, pro přejmenování ho však potřebuje
	//////Zařazení elementu do seznamu + kontrola pořadí
	if(force_razeni==NULL)
	{
		//kontrola zda je element vkládán za předchozí nebo mezi předchozí
		Cvektory::TElement *p=vloz_element_pred(Objekt,Element);//pokud bude vkládaný elment vložen na konec vrází NULL, pokud mezi vrátí ukazatel na další element
		////funkcionalita pro běžné elementy
		if(Element->eID!=300 && Element->eID!=301)
		{
			if(p==NULL)//vkládám na konec
			{
				//ukazatelové propojení
				if(posledni==NULL)//vkládání nového objektu - vkládání zarážky
	  		{
	  			if(ELEMENTY->dalsi==NULL)//vkládání prvního bojektu a první zarážky
	  			{
	  				Element->dalsi=NULL;
	  				Element->predchozi=ELEMENTY;
	  				ELEMENTY->dalsi=Element;
	  				ELEMENTY->predchozi=Element;
	  			}
	  			else//vkládání dalších objektu - tj. zarážek
	  			{
	  				if(Objekt->dalsi==NULL)//vkládání objektu na konec - tzn. zarážka na konec
	  				{
	  					Element->dalsi=NULL;
	  					ELEMENTY->predchozi->dalsi=Element;
	  					Element->predchozi=ELEMENTY->predchozi;
	  					ELEMENTY->predchozi=Element;
	  				}
	  				else//vkládání objektu mezi ostatní - tzn. zarážka mezi elementy
	  	  		{
	  					Element->dalsi=Objekt->dalsi->element;
	  	  			Element->predchozi=Objekt->dalsi->element->predchozi;
	  					Objekt->dalsi->element->predchozi->dalsi=Element;
	  					Objekt->dalsi->element->predchozi=Element;
	  	  			//změna indexů
							Cvektory::TElement *E=Element;
							T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	  					int n=Element->predchozi->n+1;
	  	      	while(E!=NULL)//projetí od aktuálního objektu až do konce
	  					{
	  						//indexy
	  	  				E->n=n;
	  	      		n++;
								E=dalsi_krok(VYHYBKY,E);
							}
							vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	  	  			E=NULL;delete E;
							//změna názvů
	  	  			uprav_popisky_elementu(NULL);//musí být NULL, pokud je poslán Element načte se jeho objekt a z něj první element, Objekt ale v tuto chvíli nemá ukazatel na první element ... pam. chyba
	  	  		}
	  			}
	  	  }
	  	  else//vkládáné elementů do objektu
	  		{
	  			Element->dalsi=NULL;
	  			Element->predchozi=posledni;
	  			posledni->dalsi=Element;
	  			if(ELEMENTY->predchozi->n==posledni->n)ELEMENTY->predchozi=Element;
	  		}
	  		//geometrie + ošetření proti přiřazování geometrie v metodě kopiruj_elementy
	  		if(F->OBJEKT_akt!=NULL && Element->n!=1 && Element->Xt==-100)//nutna podminka, pri nacitani z binarky je OBJEKT_akt=NULL a nactou se hodnoty OK
	  		vloz_G_element(Element,0,Element->predchozi->geo.X4,Element->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,Element->predchozi->geo.orientace);
	  	}
			else /*if(p->n!=Element->n)*///vkládám mezi elementy, vpřípadě, že bylo vloženo před prví prvek vrací Element, přesun je již vyřešen
			{
				//ukazatelové propojení
				Element->dalsi=p;
	  		Element->predchozi=p->predchozi;
	  		if(Objekt->element!=NULL && p==Objekt->element)Objekt->element=Element;//nový první element objetku
				if(p->predchozi->eID==300 && p->predchozi->dalsi2==p)p->predchozi->dalsi2=Element;
				else p->predchozi->dalsi=Element;
	  		p->predchozi=Element;

				//geometrie
				vloz_G_element(Element,0,p->geo.X1,p->geo.Y1,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,p->geo.orientace);
				if(!((Element->X==p->geo.X1 && Element->Y==p->geo.Y1) || (Element->X==p->geo.X4 && Element->Y==p->geo.Y4)) && p->geo.typ==0)vloz_G_element(p,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,0,0,0,0,p->geo.X4,p->geo.Y4,p->geo.orientace);
				//změna indexů
				Cvektory::TElement *E=Element;
	  		//E->n=vrat_poradi_elementu_do(E)+1;
				int n=E->predchozi->n+1;
				//pokud existuje alespoň jedna vyhybka je potřeba přeindexovat celý spoják
				if(pocet_vyhybek!=0)
				{
					E=ELEMENTY->dalsi;
					n=1;
				}
				T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	  		while(E!=NULL)//projetí od aktuálního objektu až do konce
	  		{
	  			//indexy
	  			E->n=n;
	  			n++;
					E=dalsi_krok(VYHYBKY,E);
				}
				vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
				E=NULL;delete E;
				//změna názvů
				if(pocet_vyhybek==0)uprav_popisky_elementu(Element);
				else uprav_popisky_elementu(NULL);//pokud je v elementech výhybka musí dojít k upravení celého spojáku
			}
		}
		//vkládání výhybek a spojek
		else
		{
			if(p!=NULL && Element->eID==300)
			{
				Element->dalsi=p;
				Element->dalsi2=NULL;
				if(p->predchozi->eID==300 && p->predchozi->dalsi2==p)p->predchozi->dalsi2=Element;
				else p->predchozi->dalsi=Element;
				Element->predchozi=p->predchozi;
				Element->predchozi2=NULL;
				p->predchozi=Element;
				vyhybka_pom=Element;
				pocet_vyhybek++;
			}
			//vkládání spojky
			if(p!=NULL && Element->eID==301)
			{
				Element->dalsi=p;
				Element->dalsi2=NULL;
				Element->predchozi=p->predchozi;
				Element->predchozi2=vyhybka_pom;
				if(p->predchozi->eID==300 && p->predchozi->dalsi2==p)p->predchozi->dalsi2=Element;
				else p->predchozi->dalsi=Element;
				p->predchozi=Element;
				Element->dalsi2=vyhybka_pom;
				vyhybka_pom->predchozi2=Element;//uložení rychlého odkazu výhybky na svoji spojku
				vyhybka_pom->dalsi2=Element;//ukazatel na vedlejší větev
				vyhybka_pom=NULL;
			}
			if(Objekt->element!=NULL && p==Objekt->element || Objekt->element==NULL)Objekt->element=Element;
			Element->identifikator_vyhybka_spojka=pocet_vyhybek;
			//geometrie
			vloz_G_element(Element,0,p->geo.X1,p->geo.Y1,0,0,0,0,Element->X,Element->Y,p->geo.orientace);
			if(!((Element->X==p->geo.X1 && Element->Y==p->geo.Y1) || (Element->X==p->geo.X4 && Element->Y==p->geo.Y4)) && p->geo.typ==0)vloz_G_element(p,0,Element->X,Element->Y,0,0,0,0,p->geo.X4,p->geo.Y4,p->geo.orientace);
			//změna indexů
			Cvektory::TElement *E=Element;
			int n=p->n;
			T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
			while(E!=NULL)//projetí od aktuálního objektu až do konce
			{
				//indexy
				E->n=n;
				n++;
				E=dalsi_krok(VYHYBKY,E);
			}
			vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
			E=NULL;delete E;
			//změna názvů
			uprav_popisky_elementu(Element);
    }
		p=NULL; delete p;
	}
	//////Zarazení do seznamu do předm určeného pořadí (používá se při editaci geometrie)
	else//zařazení před element force_razeni
	{
		//vkládání objektu ... zařazení jeho výhybky
		if(F->Akce!=F->GEOMETRIE && F->pom_element!=NULL && F->pom_element==force_razeni)
		{
			//vkládání na vedlejší větev
			if(force_razeni->eID==300)
			{
		  	Element->dalsi=force_razeni->dalsi2;
				if(force_razeni->dalsi2->eID==301)force_razeni->dalsi2->predchozi2=Element;
				else force_razeni->dalsi2->predchozi=Element;
		  	force_razeni->dalsi2=Element;
		  	Element->predchozi=force_razeni;
			}
			//vkládání mezi objekty
			else
			{
				//vkládání před první objekt
				if(force_razeni->objekt_n==1 && OBJEKTY->dalsi->n!=force_razeni->objekt_n)
				{
					ELEMENTY->dalsi->predchozi=Element;
					Element->dalsi=ELEMENTY->dalsi;
					Element->predchozi=ELEMENTY;
					ELEMENTY->dalsi=Element;
				}
				//normální vkládání za objekt případně mezi objekty
				else
				{
			  	Element->dalsi=force_razeni->dalsi;
					if(force_razeni->dalsi!=NULL)
					{
						if(force_razeni->dalsi->eID==301 && force_razeni->dalsi->predchozi2==force_razeni)force_razeni->dalsi->predchozi2=Element;
						else force_razeni->dalsi->predchozi=Element;
					}
			  	else ELEMENTY->predchozi=Element;
			  	Element->predchozi=force_razeni;
					force_razeni->dalsi=Element;
				}
			}
			//změna indexů
			Cvektory::TElement *E=Element;
      T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
			int n=force_razeni->n+1;
			while(E!=NULL)//projetí od aktuálního objektu až do konce
			{
				//indexy
				E->n=n;
				n++;
				E=dalsi_krok(VYHYBKY,E);
			}
			vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
			E=NULL;delete E;
			//změna názvů
			uprav_popisky_elementu(Element);
		}
		//vkládání geometrie
		else
		{
			if(vyhybka_pom!=NULL && force_razeni->eID==301 && force_razeni->predchozi2->eID==300)//force_razeni->objekt_n!=F->OBJEKT_akt->n)//vkládání elementu mezi výhybku a spojku
			{
      	Element->dalsi=force_razeni;
				force_razeni->predchozi2->dalsi2=Element;
				Element->predchozi=force_razeni->predchozi2;
				force_razeni->predchozi2=Element;
			}
			else if(force_razeni->eID==300 && force_razeni->objekt_n!=F->OBJEKT_akt->n)//přidávání geometrie z objektu spojky
			{
				Element->dalsi=force_razeni->dalsi2;
				if(force_razeni->dalsi2==force_razeni->predchozi2)force_razeni->dalsi2->predchozi2=Element;
				else force_razeni->dalsi2->predchozi=Element;
				Element->predchozi=force_razeni;
				force_razeni->dalsi2=Element;
      }
			else//ukazatelové propojení, normální funkce
			{
				if(force_razeni==Objekt->element)Objekt->element=Element;//asi problém při napojování výhybky - sledovat
				Element->dalsi=force_razeni;
				if(force_razeni->eID==301 && vyhybka_pom!=NULL)
				{
					if(force_razeni->predchozi2->eID==300 && force_razeni->predchozi2->dalsi2==force_razeni)force_razeni->predchozi2->dalsi2=Element;
					else force_razeni->predchozi2->dalsi=Element;
					Element->predchozi=force_razeni->predchozi2;
					force_razeni->predchozi2=Element;
				}
				else
				{
          if(force_razeni->predchozi->eID==300 && force_razeni->predchozi->dalsi2==force_razeni)force_razeni->predchozi->dalsi2=Element;
					else force_razeni->predchozi->dalsi=Element;
					Element->predchozi=force_razeni->predchozi;
					force_razeni->predchozi=Element;
				}
			}
			//změna indexů a kontrola popisků elementů přesunuta do ukončneí editace geometrie, tam proběhne jednou na konci, tady probíhala po každém vložení části geometrie
//			Cvektory::TElement *E=ELEMENTY->dalsi;//Element->predchozi;
			//přesun vždy na výhybku
//			if(Element->predchozi->eID==300 && Element->predchozi->dalsi2==Element)E=Element->predchozi;
//			if(Element->dalsi->eID==301 && Element->dalsi->predchozi2==Element)E=Element->dalsi->dalsi2;
//			unsigned long n=E->n;
//			while(E!=NULL)
//			{
//				//indexy
//				E->n=n;
//				n=n+1;
//				E=dalsi_krok(E);
//			}
//			E=NULL;delete E;
			//změna názvů
//			uprav_popisky_elementu(Element);
		}
	}
}
////---------------------------------------------------------------------------
//vkládá elementy za sebe do spojáku ELEMENTY, souží pro načítání ze souboru
void Cvektory::vloz_element(TElement *Element)
{
	if(ELEMENTY==NULL)hlavicka_ELEMENTY();
	Element->dalsi2=NULL;
	Element->predchozi2=NULL;
	//ukazatelové propojení
	if(ELEMENTY->dalsi==NULL)//první element za hlavičku
	{
		Element->n=1;
		Element->predchozi=ELEMENTY;
		ELEMENTY->dalsi=Element;
		ELEMENTY->predchozi=Element;
	}
	else//další elementy
	{
		Element->n=ELEMENTY->predchozi->n+1;
		Element->predchozi=ELEMENTY->predchozi;
		ELEMENTY->predchozi->dalsi=Element;
		ELEMENTY->predchozi=Element;
	}
	Element->dalsi=NULL;
	//kontrola jestli má objekt ukazatel na svůj první element
	TObjekt *O=vrat_objekt(Element->objekt_n);
	if(O->element==NULL)O->element=Element;
	O=NULL;delete O;
}
////---------------------------------------------------------------------------
//kontrola zda vkládaný element bude umístěn na konec nebo mezi jiné elementy
Cvektory::TElement *Cvektory::vloz_element_pred(TObjekt *Objekt,TElement *Element)
{
	Cvektory::TElement *ret=NULL;//návratová proměnná, defaultně prázdn
	if((F->Akce==F->ADD || F->Akce==F->VYH || F->Akce==F->MOVE_ELEMENT || F->editace_textu))// && Objekt!=NULL && Objekt->element!=NULL/* && Objekt->element->dalsi!=NULL*/)//ošetření proti spouštění při zavírání a otvírání náhledu
	{
		TElement *p=ELEMENTY->dalsi;//začnu od prvního elementu v projektu
		if(Objekt!=NULL)p=Objekt->element;//začnu od prvního elementu v objektu
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		while(p!=NULL)
		{
			if(p!=Element)//neřeší se s aktuálním elementem (při posunu)
			{
				//přichytávání na konec začátek a konec oblouku, pouze pro výhybku a spojku
				if((Element->eID==300 || Element->eID==301) && F->Akce==F->Takce::VYH && F->prichytavat_k_mrizce==1 && p->geo.typ!=0)
				{
					if(m.delka(Element->X,Element->Y,p->geo.X1,p->geo.Y1)<1){ret=p->predchozi;break;}//začátek linie
					if(m.delka(Element->X,Element->Y,p->geo.X4,p->geo.Y4)<1){ret=p->dalsi;break;}//konec linie
				}
				//kontrola zda vkládaný element neleží mezi prvním a druhým elementem, druhým až n
				if(p->geo.typ==0 && m.PtInLine(p->geo.X1,p->geo.Y1,p->geo.X4,p->geo.Y4,F->d.Rxy(Element).x,F->d.Rxy(Element).y))
				//if(p->geo.typ==0 && m.LeziVblizkostiUsecky(m.round2double(F->d.Rxy(Element).x,2),m.round2double(F->d.Rxy(Element).y,2),m.round2double(p->geo.X1,2),m.round2double(p->geo.Y1,2),m.round2double(p->geo.X4,2),m.round2double(p->geo.Y4,2))==0)
				{
					ret=p;//uložení elementu, který předcházi vkládanému elementu
					break;
				}
				//předávací místo může být přesně na konci funkčního elementu, i když se jedná o oblouk
				if(Element->eID==200 && Element->X==p->geo.X4 && Element->Y==p->geo.Y4)
				{
					ret=p->dalsi;//uložení elementu, který předcházi vkládanému elementu
					break;
				}
			}
			p=dalsi_krok(VYHYBKY,p,Objekt);//posun na další prvek, buď v objektu (Objekt!=NULL) nebo v celé lince (Objekt==NULL)
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		p=NULL; delete p;
	}
	return ret;
}
////---------------------------------------------------------------------------
//danému elementu přiřadí/naplní geometrickou složku
void Cvektory::vloz_G_element(TElement *Element,short typ,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4,double orientace,double rotacni_uhel,double radius,double delka)
{
	if(Element!=NULL)
	{
		Element->geo.typ=typ;//0 - linie, 1 - oblouk, -1 neidentifikovatelný tvar pomocí bézieru
		Element->geo.delka=delka;
		Element->geo.radius=radius;
		Element->geo.orientace=orientace;
		Element->geo.rotacni_uhel=rotacni_uhel;
		Element->geo.X1=X1;Element->geo.Y1=Y1;
		Element->geo.X4=X4;Element->geo.Y4=Y4;
		Element->geo.X2=X2;Element->geo.Y2=Y2;
		Element->geo.X3=X3;Element->geo.Y3=Y3;
		switch(typ)
		{
			case -1://obecný bézier
			{
				//if(delka==0)Element->geo.delka=m.bezierDelka(X1,Y1,X2,Y2,X3,Y3,X4,Y4);//pokud nebyla délka dodána
				Element->geo.radius=0;
				Element->geo.rotacni_uhel=0;
			}break;
			case 0://linie
			{
				if(delka==0)
				{
					if(Element->geo.HeightDepp==0)Element->geo.delka=m.delka(Element->geo.X1,Element->geo.Y1,Element->geo.X4,Element->geo.Y4);//pokud nebyla délka dodána
					else
					{
						Element->geo.delkaPud=m.delka(Element->geo.X1,Element->geo.Y1,Element->geo.X4,Element->geo.Y4);//pokud nebyla délka dodána
						Element->geo.delka=m.castPrepony(Element->geo.delkaPud,Element->geo.delkaPud,Element->geo.HeightDepp);//změna 3D délky
					}
				}
				Element->geo.radius=0;
				Element->geo.rotacni_uhel=0;
				Element->geo.X2=Element->geo.X1;Element->geo.Y2=Element->geo.Y1;
				Element->geo.X3=Element->geo.X4;Element->geo.Y3=Element->geo.Y4;
			}break;
			case 1://oblouk
			{
				if(delka==0)Element->geo.delka=m.R2Larc(radius,rotacni_uhel);//pokud nebyla délka dodána
				Element->geo.radius=radius;
			}break;
		}
	}
}
////---------------------------------------------------------------------------
//pokud byl nějaký element vložen mezi ostatní a ne na konec, provede přejměnování,dále pokud bylo stisknuto storno vrátí všechny změny
void Cvektory::uprav_popisky_elementu(TElement *Element)
{
	bool rename=false;//proměná sloužící k spouštění přejměnování
	AnsiString t_operator=F->ls->Strings[272],t_ion=F->ls->Strings[270],t_otoc=F->ls->Strings[273],t_PM=F->ls->Strings[271];
	//úprava názvu pro roboty
	TElement *E=ELEMENTY->dalsi;//začíná se od začátku, někdy je potřeba ovlivnit i předchozí elementy
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření seznamu průchodu
	if(Element!=NULL)E=vrat_objekt(Element->objekt_n)->element;//pokud vím který element byl změněn začnu od prvního elementu v tomto objektu
	while (E!=NULL)
	{
		switch(E->eID)
		{
			//stopka
			case 0:if(E->name.SubString(1,5).UpperCase()=="STOP " || E->name=="")rename=true;else rename=false;break;
			//otoče
			case 5:
			case 6:if(E->name.SubString(1,4)==t_otoc || E->name.SubString(1,7)==t_otoc/*"Otoč" || "Turning"*/ || E->name=="")rename=true;else rename=false;break;
			//předavací místo
			case 200:if(E->name.SubString(1,15)==t_PM || E->name.SubString(1,14)==t_PM/*"Předávací místo" || "Transfer point"*/ || E->name=="")rename=true;else rename=false;break;
			//výhybka a spojka
			case 300:case 301:break;//nepřejmenovává se
			//zarážka
			case MaxInt:rename=true;break;
			//roboti, operátoři a ION
			default :if(E->name.SubString(1,5)=="Robot" || E->name.SubString(1,8)==t_operator || E->name.SubString(1,7)==t_ion || E->name=="")rename=true;else rename=false;break;//musí zde být, jinak nějakým způsobem je pro robot rename nastaveno na true
		}
		//změna názvu
		if(rename)//přejmenování elementu ve spojáku + mGridu
		{
			int n=vrat_poradi_elementu_do(E)+1;//zjistí pořadové číslo elementu
			//zapsání nového názvu
			switch(E->eID)
			{
				//stopka
				case 0:E->name="Stop "+AnsiString(n);break;
				//otoče
				case 5:
				case 6:E->name=t_otoc+" "+AnsiString(n);break;
				//předávací místo
				case 200:E->name=t_PM+" "+AnsiString(n);break;
				//výhybka a spojka
				case 300:case 301:break;//nepřejmenovává se
				//zarážka
				case MaxInt:E->name="Zarážka "+AnsiString(n);break;
				//roboti, operátoři a ION
				default :if(E->eID==100)E->name=t_ion+" "+AnsiString(n);else if(101<=E->eID && E->eID<=108)E->name=t_operator+" "+AnsiString(n);else E->name="Robot "+AnsiString(n);break;
			}
			E->short_name=E->name.SubString(1,3)+AnsiString(n);
			//změna názvu v hlavičce mGridu, jako první z důvodu podmínky prázdného názvu
			if(E->name!="" && E->mGrid!=NULL && E->eID!=300 && E->eID!=301)//nutné, přejmenovávám i první element, který nemá vytvořený mGrid
			{
				try
				{
					int col=0;
					if(E->eID==200 || E->eID==300)col=3;
					E->mGrid->Cells[col][0].Text="<a>"+E->name+"</a>";
					E->mGrid->Cells[col][0].Font->Color=clBlack;//z důvodu nasazení odkazu, po přejmenování se text vrátil do modré barvy
					if(col==0 && E->eID!=300 && E->eID!=200)E->mGrid->MergeCells(0,0,E->mGrid->ColCount-1,0);//nutné kvůli správnému zobrazení hlavičky, u PM tabulek se neslučuje hlavička
					if(F->zobrazeni_tabulek)E->mGrid->Update();//musí zde být ošetření proti paměťové chybě
				}catch(...){;}
			}
		}
		//aktualizace ID tabulek
		E=dalsi_krok(VYHYBKY,E);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);
	delete E;E=NULL;
}
////---------------------------------------------------------------------------
//zkopíruje atributy elementu bez ukazatelového propojení, pouze ukazatelové propojení na mGrid je zachováno
void Cvektory::kopiruj_element(TElement *Original, TElement *Kopie)
{
	Kopie->n=Original->n;
	Kopie->eID=Original->eID;
	Kopie->identifikator_vyhybka_spojka=Original->identifikator_vyhybka_spojka;
	Kopie->short_name=Original->short_name;
	Kopie->name=Original->name;
	Kopie->X=Original->X;
	Kopie->Y=Original->Y;
  Kopie->Z=Original->Z;
	Kopie->Xt=Original->Xt;
	Kopie->Yt=Original->Yt;
	Kopie->orientace=Original->orientace;
	Kopie->rotace_jig=Original->rotace_jig;
	Kopie->stav=Original->stav;
	Kopie->data.PD=Original->data.PD;
	Kopie->data.LO1=Original->data.LO1;
	Kopie->OTOC_delka=Original->OTOC_delka;
	Kopie->zona_pred=Original->zona_pred;
	Kopie->zona_za=Original->zona_za;
	Kopie->data.LO2=Original->data.LO2;
	Kopie->data.LO_pozice=Original->data.LO_pozice;
	Kopie->data.PT1=Original->data.PT1;
	Kopie->PTotoc=Original->PTotoc;
	Kopie->data.PT2=Original->data.PT2;
	Kopie->WT=Original->WT;
	Kopie->data.WTstop=Original->data.WTstop;
	Kopie->data.RT=Original->data.RT;
	Kopie->data.pocet_voziku=Original->data.pocet_voziku;
	Kopie->data.pocet_pozic=Original->data.pocet_pozic;
	Kopie->geo=Original->geo;
	Kopie->mGrid=NULL;//nealokovat!!!!!! k alokci pro mGrid dochází v metodě Form1::NP_input() při otevírání náhledu
	Kopie->objekt_n=Original->objekt_n;
	//if(Original->pohon!=NULL)Kopie->pohon=vrat_pohon(Original->pohon->n);
	//else
	Kopie->pohon=Original->pohon;
	Kopie->sparovany=Original->sparovany;
}
////---------------------------------------------------------------------------
//zkopíruje atributy dat, slouží pro zakládání cesty v zakázce, musí být řešeno přes cestu!!!!!! ne Tdata Original, Tdata Kopie
void Cvektory::kopiruj_data_elementu(Tdata Original,TCesta *Cesta)
{
	Cesta->data.PD=Original.PD;
	Cesta->data.orientace_jig_pred=Original.orientace_jig_pred;
	Cesta->data.LO1=Original.LO1;
	Cesta->data.LO2=Original.LO2;
	Cesta->data.LO_pozice=Original.LO_pozice;
	Cesta->data.PT1=Original.PT1;
	Cesta->data.PT2=Original.PT2;
	Cesta->data.WTstop=Original.WTstop;
	Cesta->data.RT=Original.RT;
	Cesta->data.pocet_pozic=Original.pocet_pozic;
	Cesta->data.pocet_voziku=Original.pocet_voziku;
}
////---------------------------------------------------------------------------
//všem elementům, které měly přiřazen pohon s oldN(oldID), přiřadí pohon s newN(newID), podle toho, jak jsou ukládány nově do spojáku, důležité, pokud dojde k narušení pořadí ID resp n pohonů a pořadí jednotlivých řádků ve stringridu, např. kopirováním, smazáním, změnou pořadí řádků atp.
void Cvektory::aktualizace_prirazeni_pohonu_k_elementum(unsigned int oldN,unsigned int newN)
{
	TElement *E=ELEMENTY->dalsi;
	while(E!=NULL)
	{
		if(E->pohon!=NULL && oldN==E->pohon->n)// && E->probehla_aktualizace_prirazeni_pohonu==false)//objekt měl přiřazen pohon a ještě nebyl nově přepřiřazen
		{
			E->pohon=vrat_pohon(newN);//danému objektu přiřadíme nové ID resp. n původního pohonu
			//O->probehla_aktualizace_prirazeni_pohonu=true;//již se s tímto objektem nebude pracovat v dalších přiřazování dané aktualizace, důležitné např. pro situaci 2->3,3->4 (aby prvně nebyl přiřezn pohon s id 2 na 3 a potom všechny pohony s id 3 na pohon 4, protože měly být přiřazený jen některé...)
		}
		E=E->dalsi;//posun na další element
	}
	delete E;E=NULL;//odstranění již nepotřebného ukazatele
}
////---------------------------------------------------------------------------
//připraví vektor provizorní osy pohonu
void Cvektory::vytvor_elementarni_osu(TObjekt *Original, TObjekt  *Kopie)
{                 //smazat
	if(Original==F->pom && Kopie==F->OBJEKT_akt)
	{
		//hlavicka_elementy(Kopie);
	//	Kopie->elementy->geo.rotace=m.Rt90(F->d.trend(F->pom));
	//	Kopie->elementy->geo.typ=0;Kopie->elementy->X=0;Kopie->elementy->Y=0;Kopie->elementy->geo.delka=0;
	//	if(Kopie->elementy->geo.rotace==90 || Kopie->elementy->geo.rotace==270)Kopie->elementy->Y=Kopie->Y-Kopie->rozmer_kabiny.y/2.0;//vodorovná kabina
	//	else Kopie->elementy->X=Kopie->X+Kopie->rozmer_kabiny.x/2.0;//svislá
	}

}
////---------------------------------------------------------------------------
//vratí eID prvního použitého robota, slouží na filtrování, jaké roboty v knihovně robotů zakazazovat, pokud není nic nalezeno vrátí -1
int Cvektory::vrat_eID_prvniho_pouziteho_robota(TObjekt *Objekt)
{
	int RET=-1;
	if(Objekt->element!=NULL)
	{
		TElement *E=Objekt->element;//přeskočí rovnou hlavičku
		while(E!=NULL && E->objekt_n==Objekt->n)
		{
			if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18 || 101<=E->eID && E->eID<=108) RET=E->eID;
			E=E->dalsi;
		}
		E=NULL;delete E;
	}
	if(RET==-1)Objekt->rezim=-1;else{if(RET>0 && RET%2==0)Objekt->rezim=0;else Objekt->rezim=1;}//rovnou nastaví režim objektu
	return RET;
}
////---------------------------------------------------------------------------
//vrátí počet stejných elementů před Element, u robotů v jednom objektu, u otočí a stopek vrátí počet i z předchozích objektů
unsigned int Cvektory::vrat_poradi_elementu_do(TElement *Element)
{
	unsigned int r_pocet=0,s_pocet=0,o_pocet=0,t_pocet=0,pm_pocet=0,z_pocet=0,ret=0;//nastavení všech počtů na nulu
	TElement *E=ELEMENTY->dalsi;
  T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL)
	{
		//pokud narazím na stejný element, ukončím hledání
		if(E==Element)break;
		//zapisování nalezených elementů
		switch(E->eID)
		{
			case 0:s_pocet++;break;
			case 5:
			case 6:o_pocet++;break;
			case 200:pm_pocet++;break;
			case MaxInt:z_pocet++;break;
			default:
			{
				if(E->eID!=0 && E->eID!=5 && E->eID!=6 && E->eID!=100 && E->eID!=200 && vrat_druh_elementu(E)!=-1)r_pocet++;
				if(E->eID==100)t_pocet++;
				if(Element->objekt_n!=E->objekt_n){r_pocet=0;t_pocet=0;}//pokud se stále nenacházím v cílovém objektu, jsem pořad s číslováním na 0, až v stejnem objektu začnu počítat výskyty robotů, atd.
			}break;
		}
		E=dalsi_krok(VYHYBKY,E);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	//podle eID vrátí příslušný počet elementů
	switch(Element->eID)
	{
		case 0: ret=s_pocet;break;
		case 5: case 6: ret=o_pocet;break;
		case 100: ret=t_pocet;break;
		case 200: ret=pm_pocet;break;
		case MaxInt: ret=z_pocet;break;
		default: ret=r_pocet;break;
	}
	return ret;
}
////---------------------------------------------------------------------------
//vrátí největší ID napříč mGridy v objektu, používáno pro přiřazování ID novým tabulkám, řešeno takto z důvodu chyby při odmazávání a následném přidávání elementu (v kabině jsou 3 elementy druhý se odmaže, tabulky v kabině mají nyní ID 1 a 3, po přidání dalšího elementu bylo dříve přidano ID=pocet elementů, což by se v tomto případě rovnalo 3)
unsigned int Cvektory::vrat_nejvetsi_ID_tabulek (TObjekt *Objekt)
{
	unsigned int ret=0;
	if(Objekt->element!=NULL && F->OBJEKT_akt!=NULL)//pouze pro vkládání v editaci
	{
		TElement *E=Objekt->element;
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		while(E!=NULL && E->objekt_n==Objekt->n)
		{
			if(E->n>0/*&&E->eID!=100 && E->mGrid!=NULL*/)//přeskočení hlavičky a elementu bez tabulky
			{
				try//mGrid může být neNULL, ale zároveň nemusí existovat (pouze alokovaná paměť)
				{if(ret<(unsigned)E->mGrid->ID)ret=E->mGrid->ID;}
				catch(...){;}
      }
			E=dalsi_krok(VYHYBKY,E,Objekt);
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E;
	}
	return ret;
}
////---------------------------------------------------------------------------
//ověří zda se jedná S&G linku z pohledu užité cesty, resp. zda obsahuje alespoň jeden S&G element včetně stopky na užité cestě, element musí být na pohonu
bool Cvektory::SGlinka()
{
	bool RET=false;
	if(ZAKAZKA_akt!=NULL && ZAKAZKA_akt->cesta!=NULL)
	{
		TCesta *C=ZAKAZKA_akt->cesta->dalsi;//přeskočí hlavičku
		while (C!=NULL)
		{
			if(/*C->Element->pohon!=NULL && */vrat_druh_elementu(C->Element)==0){RET=true;break;}
			C=C->dalsi;//posun na další prvek cesty
		}
		C=NULL;delete C;//je nutné v tomto místě volat NULL!!!
	}
	return RET;
}
////---------------------------------------------------------------------------
//vrátí typ elementu -1 nenastaven nebo zarážka či předávací místo, 0 - S&G( včetně stopky), 1 - kontinuál
short Cvektory::vrat_druh_elementu(TElement *Element)
{
	short RET=-1;//nenastaven nebo zarážka či předávací místo a další provozní elementy
	if(Element!=NULL)//zarážka předávací místo  výhybka a spojka 				                      //nutné přeskakovat elementarní hlavičku!
	if(Element->eID!=MaxInt && Element->eID!=200 && Element->eID!=300 && Element->eID!=301 && Element->eID!=400 && Element->eID!=401 && Element->eID!=402 && Element->n>0)
	{
		if(Element->eID%2==0 && Element->eID!=100 && ((Element->eID==0 && Element->data.pocet_voziku>0) || Element->eID!=0))RET=0;//S&G elementy, mimo těch průjezdních a mimo ION tyče
		else RET=1;//kontinuální elementy, či S&G elementy průjezdní (tj. kontinuální)
	}
	return RET;
}
////---------------------------------------------------------------------------
//vrátí true, pokud se jedná o funční element
bool Cvektory::funkcni_element(TElement *Element)
{                                                 //nutné přeskakovat elementarní hlavičku!
	if(Element->eID!=MaxInt && Element->eID<199 && Element->n>0)return true;
	else return false;
}
////---------------------------------------------------------------------------
//orotuje všechny elementy daného objektu o danou hodnotu
void Cvektory::rotace_elementu(TObjekt *Objekt,short rotace)
{
	TElement *E=Objekt->element;//přeskočí rovnou hlavičku
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		if(E->orientace+rotace>360)E->orientace=0;//kvůli přetýkání na dvě podmínky
		E->orientace+=rotace;
		if(E->orientace==360)E->orientace=0;
		E=E->dalsi;
	}
	E=NULL;delete E;
}
////---------------------------------------------------------------------------
//zkontroluje, zda se nacházím v oblasti elemetu
bool Cvektory::oblast_elementu(TElement *Element, double X, double Y)
{
	float otoc_sirka=3.5;//ve skutečnosti poloměr
	float otoc_tloustka=0.8;
	bool ret=false;
	short rotace=Element->orientace;
	if(Element->eID==0)//STOPKY
	{
		rotace=m.Rt90(rotace+180);//stopka je o 180° orotovaná
		if(m.PtInStopka(Element->X,Element->Y,X,Y,rotace) || Element->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))ret=true;//testování symbolu včetně popisku,pozn. CreatePolygonRgn i PtInRect - zahrnuje pouze vnitřní tvar, obrys tvaru je z oblasti vyloučen
	}
	if(Element->eID==5 || Element->eID==6)//OTOČE
	{
		if(m.PtInCircle(X,Y,Element->X,Element->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px) || Element->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))ret=true;//testování symbolu včetně popisku,pozn.
	}
	if(Element->eID==100)//ION tyč
	{
		if(m.PtInIon(Element->X,Element->Y,X,Y,rotace) || Element->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))ret=true;
	}
	if(1<=Element->eID && Element->eID<=4 || 7<=Element->eID && Element->eID<=18)//ROBOTI
	{
		//hledání, zda leží v regionu, region se liší dle rotace
		HRGN hreg;
		double DoSkRB=F->d.DoSkRB;
		switch(rotace)
		{
			case 0: hreg=CreateRectRgn(m.L2Px(Element->X-F->d.Robot_delka_zakladny/2.0),m.L2Py(Element->Y+DoSkRB),m.L2Px(Element->X+F->d.Robot_delka_zakladny/2.0),m.L2Py(Element->Y-F->d.Robot_sirka_zakladny/2.0));break;
			case 90:hreg=CreateRectRgn(m.L2Px(Element->X-F->d.Robot_sirka_zakladny/2.0),m.L2Py(Element->Y+F->d.Robot_delka_zakladny/2.0),m.L2Px(Element->X+DoSkRB),m.L2Py(Element->Y-F->d.Robot_delka_zakladny/2.0));break;
			case 180:hreg=CreateRectRgn(m.L2Px(Element->X-F->d.Robot_delka_zakladny/2.0),m.L2Py(Element->Y+F->d.Robot_sirka_zakladny/2.0),m.L2Px(Element->X+F->d.Robot_delka_zakladny/2.0),m.L2Py(Element->Y-DoSkRB));DoSkRB*=-1;break;
			case 270:hreg=CreateRectRgn(m.L2Px(Element->X-DoSkRB),m.L2Py(Element->Y+F->d.Robot_delka_zakladny/2.0),m.L2Px(Element->X+F->d.Robot_sirka_zakladny/2.0),m.L2Py(Element->Y-F->d.Robot_delka_zakladny/2.0));DoSkRB*=-1;break;
		}
		if(PtInRegion(hreg,m.L2Px(X),m.L2Py(Y)))ret=true;
		else//pokud nenalezeno, testuje ještě případně OTOČE ROBOTŮ
		{
			if(Element->eID==3 || Element->eID==4 || Element->eID==9 || Element->eID==10 || Element->eID==13 || Element->eID==14 || Element->eID==17 || Element->eID==18)
			{
				if(rotace==0 || rotace==180)
				{
					if(m.PtInCircle(X,Y,Element->X,Element->Y+DoSkRB,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))ret=true;//ROBOTi s otočemi
				}
				else//90°, 270°
				{
					if(m.PtInCircle(X,Y,Element->X+DoSkRB,Element->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))ret=true;//ROBOTi s otočemi
				}
			}
			else//ani mezi otočemi robotu nenalezeno, hledá mezi STOPKAMI ROBOTŮ
			{
				if(Element->eID==2 || Element->eID==4 || Element->eID==8 || Element->eID==10 || Element->eID==12 || Element->eID==14 || Element->eID==16 || Element->eID==18)
				{
					if(rotace==0 || rotace==180)
					{
						if(m.PtInStopka(Element->X,Element->Y+DoSkRB,X,Y,rotace))ret=true;//ROBOTi se stopkami
					}
					else//90°, 270°
					{
						if(m.PtInStopka(Element->X+DoSkRB,Element->Y,X,Y,rotace))ret=true;//ROBOTi se stopkami
					}
				}
			}
		}
	}
	if(101<=Element->eID && Element->eID<=108)//ani roboti nanelezeny, hledá tedy mezi LIDSKÝMI ROBOTY
	{
		if(m.PtInClovek(Element->X,Element->Y,X,Y,rotace,Element->eID)|| Element->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))ret=true;
		else //pokud nenalezeno, testuje ještě případně otoče lidských robotů
		{
			double DkRB=F->d.DkRB;if(rotace==180 || rotace==270)DkRB*=-1;
			if(Element->eID==103 || Element->eID==104 || Element->eID==107 || Element->eID==108)//s otočemi
			{
				if(rotace==0 || rotace==180)
				{
					if(m.PtInCircle(X,Y,Element->X,Element->Y+DkRB,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))ret=true;
				}
				else//90°, 270°
				{
					if(m.PtInCircle(X,Y,Element->X+DkRB,Element->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))ret=true;
				}
			}
			else//ani mezi otočemi lidských robotu nenalezeno, hledá mezi STOPKAMI LIDSKÝCH ROBOTŮ
			{
				if(Element->eID==102 || Element->eID==104 || Element->eID==106 || Element->eID==108)
				{
					if(rotace==0 || rotace==180)
					{
						if(m.PtInStopka(Element->X,Element->Y+DkRB,X,Y,rotace))ret=true;//ROBOTi se stopkami
					}
					else//90°, 270°
					{
						if(m.PtInStopka(Element->X+DkRB,Element->Y,X,Y,rotace))ret=true;//ROBOTi se stopkami
					}
				}
			}
		}
	}
	if(Element->eID==MaxInt && F->Akce!=F->Takce::GEOMETRIE)//zarážka s S/K
	{
		if(Element->geo.HeightDepp!=0 && m.PtInCircle(X,Y,Element->X,Element->Y,0.3))ret=true;
    else if(Element->dalsi!=NULL && Element->dalsi->geo.HeightDepp!=0 && m.PtInCircle(X,Y,Element->X,Element->Y,0.3))ret=true;
	}
	if(Element->eID==MaxInt && F->Akce==F->Takce::GEOMETRIE)//zarážka při geometrii
	{
		if(m.PtInCircle(X,Y,Element->X,Element->Y,0.3))ret=true;
	}
	if(Element->eID==200 && ((Element->orientace==0 || Element->orientace==180) && m.PtInRectangle(Element->X-0.1,Element->Y-0.25,Element->X+0.1,Element->Y+0.25,X,Y) || (Element->orientace==90 || Element->orientace==270) && m.PtInRectangle(Element->X-0.25,Element->Y-0.1,Element->X+.25,Element->Y+0.1,X,Y)))ret=true;
	if((Element->eID==300 || Element->eID==301) && m.PtInCircle(X,Y,Element->X,Element->Y,0.2))ret=true;
	return ret;
}
////---------------------------------------------------------------------------
//hledá element v místě kurzoru pracuje v logických/metrických souradnicích
Cvektory::TElement *Cvektory::najdi_element(TObjekt *Objekt, double X, double Y)
{
	//vhodno přesunout do globálních proměnných do Cvykresli
	X=F->akt_souradnice_kurzoru.x;Y=F->akt_souradnice_kurzoru.y;
	TElement *E=NULL,*ret=NULL;
  T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku

	//algoritmus prochází jednotlivé elementy a porovnává vůči jejich pozici aktuální pozici kurzoru, aby se zbytečně netestovalo vše (metoda se volá neustále při každém posunu kurzoru), postupuje algoritmus maximálně větveně (šetření strojového času), tedy v případě uspěchu ihned končí, v případě neúspěchu testuje dále
	if(Objekt!=NULL)
	{
		E=Objekt->element;
  	while(E!=NULL && E->objekt_n==Objekt->n)
  	{
			if(E->n!=0)
			{
				if(oblast_elementu(E,X,Y)){ret=E;break;}
			}
			E=dalsi_krok(VYHYBKY,E,Objekt);
		}
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	E=NULL;delete E;
	return ret;
}
////---------------------------------------------------------------------------
//najde poslední element v objektu který odpovídá eID
Cvektory::TElement *Cvektory::najdi_posledni_element_podle_eID(unsigned int eID,TObjekt *Objekt)
{
	//průchod od prvního elementu v objektu
	TElement *ret=NULL;
	if(Objekt!=NULL && Objekt->n>0)//ošetření proti nulovému objektu nebo hlavičce
	{
   	TElement *E=Objekt->element;
   	while(E!=NULL && E->objekt_n==Objekt->n)
   	{
   		if(E->eID==eID)ret=E;//pokud je nalezen zapíše se, nepřeruší se cyklus, bude se přepisovat dokud nezustane v ukazateli poslední element tohoto eID v Objektu
   		E=E->dalsi;
   	}
		E=NULL;delete E;
	}
	return ret;
}
////---------------------------------------------------------------------------
//hledá tabulku elementu pouze pro daný objekt v oblasti definované pomocí šířky a výšky tabulky (která se může nacházet v daném místě kliku), pracuje v logických/metrických souradnicich, vrátí ukazatel na daný element, který tabulku vlastní, pokud se na daných souřadnicích nachází tabulka
Cvektory::TElement *Cvektory::najdi_tabulku(TObjekt *Objekt, double X, double Y)
{
	//deklarace
	TElement *ret=NULL;

	//hledání tabulky teploměru
	if(Objekt==F->OBJEKT_akt && Objekt->teplomery!=NULL)
	{
		TTeplomery *T=vrat_teplomery_podle_zakazky(F->OBJEKT_akt,ZAKAZKA_akt);
		//kontrola zda existuje cesta pro akt zakázku
		if(T!=NULL && T->posledni->mGrid!=NULL && T->posledni->Xt<=X && X<=T->posledni->Xt+T->posledni->mGrid->Width*F->m2px/F->Zoom && T->posledni->Yt>=Y && Y>=T->posledni->Yt-T->posledni->mGrid->Height*F->m2px/F->Zoom)ret=T->posledni;
    T=NULL;delete T;
	}

  //hledání předchozího PM
	if(ret==NULL && F->predchozi_PM!=NULL && F->predchozi_PM->mGrid!=NULL && F->predchozi_PM->Xt<=X && X<=F->predchozi_PM->Xt+F->predchozi_PM->mGrid->Width*F->m2px/F->Zoom && F->predchozi_PM->Yt>=Y && Y>=F->predchozi_PM->Yt-F->predchozi_PM->mGrid->Height*F->m2px/F->Zoom)ret=F->predchozi_PM;

	//hledání tabulky elementu
	TElement *E=Objekt->element;
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(ret==NULL && E!=NULL && E->objekt_n==Objekt->n)
	{
		//pokud mgrid existuje hledá poslední mGrid na pozici X,Y ... nesmí být po nalezení break
		if(E->mGrid!=NULL && E->Xt<=X && X<=E->Xt+E->mGrid->Width*F->m2px/F->Zoom && E->Yt>=Y && Y>=E->Yt-E->mGrid->Height*F->m2px/F->Zoom)ret=E;//bez break, slouží k nalezení poslední tabulky
		E=dalsi_krok(VYHYBKY,E);
	}

	//ukazatelové záležitosti + return
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	E=NULL;delete E;
	return ret;
}
////---------------------------------------------------------------------------
//vratí ukazatel na element dle n elementu umístěného v daném objektu
Cvektory::TElement *Cvektory::vrat_element(TObjekt *Objekt, unsigned int n)
{
	TElement *E=Objekt->element;
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		if(E->n==n) break;
		else E=E->dalsi;
	}
	return E;
}
////---------------------------------------------------------------------------
//vratí ukazatel na element dle n elementu
Cvektory::TElement *Cvektory::vrat_element(unsigned int n)
{
	TElement *E=ELEMENTY->dalsi;
  T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL)
	{
		if(E->n==n)break;
		else E=dalsi_krok(VYHYBKY,E);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	return E;
}
////---------------------------------------------------------------------------
//ověří zda se na daných fyzických souřadnicích nachází kóta elementu, pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty, pozn. oblast kóty se testuje až jako poslední
short Cvektory::PtInKota_elementu(TObjekt *Objekt,long X,long Y)
{
	short RET=-1;//nic nenalezeno
	double x=F->akt_souradnice_kurzoru_PX.x,y=F->akt_souradnice_kurzoru_PX.y;//souřadnice kurzoru jsou neměnné po celou dobu metody, důležité použít fyzické souřadnice !!
	TElement *E=Objekt->element;
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		if(E->citelna_oblast.rect1.PtInRect(TPoint(X,Y)) && (E->pohon!=NULL || E->pohon==NULL && F->DKunit<2)){RET=1;F->pom_element=E;break;}//hodnoty kóty
		else if(E->citelna_oblast.rect4.left!=E->citelna_oblast.rect4.right!=0 && E->citelna_oblast.rect4.PtInRect(TPoint(X,Y))){RET=3;F->pom_element=E;break;}
		else
		{
			if(E->citelna_oblast.rect2.PtInRect(TPoint(X,Y))){RET=2;F->pom_element=E;break;}//jednotky kóty
			else if(m.LeziVblizkostiUsecky(x,y,E->citelna_oblast.rect0.left,E->citelna_oblast.rect0.top,E->citelna_oblast.rect0.right,E->citelna_oblast.rect0.bottom)<=F->Zoom){RET=0;F->pom_element=E;break;}//kóta celá
		}
		E=dalsi_krok(VYHYBKY,E,Objekt);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	E=NULL;delete E;
	return RET;
}
////---------------------------------------------------------------------------
//ověří zda se na dané myších souřadnice nachází v geo. segmentu elemnetu
bool Cvektory::PtInSegment(TElement *E,double Xmys,double Ymys)
{
	if(E->geo.HeightDepp==0)return m.PtInSegment(E->geo.X1,E->geo.Y1,E->geo.typ,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka,Xmys,Ymys);
	else return m.PtInSegment(E->geo.X1,E->geo.Y1,E->geo.typ,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delkaPud,Xmys,Ymys);
}
////---------------------------------------------------------------------------
//posune pouze Element z pomocného spojového seznamu OBJEKT_akt na parametrem uvedenou vzádlenost (v metrech) od elementu předchozího, pokud je implicitní hodnota pusun_dalsich_elementu false změněna na true, jsou o danou změnu posunu přesunuty i elementy následující Elementu (tudíž jejich vzdálenost od Elementu bude zachována, naopak v případě výchozí hodnoty false je následujícím/dalším elementům poloha zachována)
bool Cvektory::posun_element(TElement *Element,double vzdalenost,bool pusun_dalsich_elementu,bool posun_kurzorem,bool kontrola_zmeny_poradi)
{
	Cvektory::TElement *E=NULL;
	bool RET=true;
	if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->element!=NULL/*&&F->Akce!=F->MOVE_ELEMENT*/)//raději ošetření, ač by se metoda měla volat jen v případě existence OBJEKT_akt
	{
		bool posun_povolit=true,error=false;
		TPointD puv_souradnice;
		puv_souradnice.x=Element->X;puv_souradnice.y=Element->Y;
		if(F->OBJEKT_akt->element!=NULL && vzdalenost!=0)//musí existovat alespoň jeden element && nesmí být vzdálenost rovna nule
		{
			//////Načtení délky před posunem
			double vzd=Element->geo.delka;//leze použít pouze délku elementu, nově se nevyskytují zarážky mezi fce. elementy
			if(Element->geo.HeightDepp!=0)vzd=Element->geo.delkaPud;//pokud se jedná o S/K
			//vzd=vzdalenost_od_predchoziho_elementu(Element,false);//musí zde být vzdálenost k předchozímu funkčnímu elementu, tj. velikost kóty
			if((Element->dalsi!=NULL && Element->dalsi->geo.typ!=0 || Element->geo.typ!=0) && kontrola_zmeny_poradi)posun_povolit=false;//pokud by element ovlivnil posunem geometrii
			//////Realizace posunu + validace
			if(posun_kurzorem && posun_povolit)//pokud se jedná o posun kurzorem, třeba jinak určit vzdálenost
			{
				if(Element->geo.orientace==90 || Element->geo.orientace==0)vzdalenost=vzd+vzdalenost;
				else vzdalenost=vzd-vzdalenost;
			}
			//realizace posunu
			/*if(Element->geo.HeightDepp==0)*/vzdalenost=(/*vzd/m.abs_d(vzd))*(*/m.abs_d(vzd)-vzdalenost);
			//else if(Element->geo.HeightDepp/vzdalenost<1)vzdalenost=-vzdalenost*cos(asin(Element->geo.HeightDepp/vzdalenost))+Element->geo.delkaPud;//zjištění posunu ve S/K
			//else {vzdalenost=0;posun_povolit=false;}//nebyly dodrženy pravidla pravoúhlého trojúhelníku, znemožnit editaci
			switch((int)Element->geo.orientace)
			{
				case   0:Element->Y=Element->Y-vzdalenost;break;
				case  90:Element->X=Element->X-vzdalenost;break;
				case 180:Element->Y=Element->Y+vzdalenost;break;
				case 270:Element->X=Element->X+vzdalenost;break;
			}
			//kontrola zda je element stále na linii
			if(F->bod_na_geometrii(0,0,Element) /*|| Element->n==vrat_posledni_element_objektu(F->OBJEKT_akt)->n*/ || !kontrola_zmeny_poradi)
			{
				//kontrola + změna pořadí
				if(kontrola_zmeny_poradi)
				{
					E=vloz_element_pred(NULL,Element);
					if(E!=NULL && Element->dalsi!=NULL && E!=Element->dalsi && Element->geo.HeightDepp==0 && E->geo.HeightDepp==0 && Element->objekt_n==E->objekt_n)zmen_poradi_elementu(Element,E);
					if(E!=NULL && Element->dalsi!=NULL && E!=Element->dalsi && (Element->geo.HeightDepp!=0 || E->geo.HeightDepp!=0))error=true;//pokud by mělo ojít ke změně pořadí, nedovolit
					if(E!=NULL && Element->objekt_n!=E->objekt_n)error=true;//posun mimo kabinu
				}
				//aktualizace posouvaného elementu
				if(!error)
				{
			  	vloz_G_element(Element,0,Element->geo.X1,Element->geo.Y1,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,Element->geo.orientace);
			  	if(kontrola_zmeny_poradi)
			  	{
			  		//aktualizace dalšího elemtnu
			  		if(Element->dalsi!=NULL)vloz_G_element(Element->dalsi,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
			  		//aktualizace RT
			  		if(Element->dalsi!=NULL&&!pusun_dalsich_elementu){reserve_time(Element,NULL,true,true);reserve_time(Element->sparovany,NULL,true,true);}//při změně vzdálenosti je nutno dopočítat znova RT, pokud je za robotem další robot jeho RT musí být také přepočítáno
			  		else reserve_time(Element,NULL,true,true);
			  	}
				}
        else
				{Element->X=puv_souradnice.x;Element->Y=puv_souradnice.y;posun_povolit=false;}
			}
			//pokud ne budou mu navráceny původní souřadnice
			else
			{Element->X=puv_souradnice.x;Element->Y=puv_souradnice.y;posun_povolit=false;}
			//////Posun dalsích elementů
			if(pusun_dalsich_elementu && posun_povolit)
			{
				TElement *E=Element->dalsi;        //F->Memo_testy->Clear();
				while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
				{
					puv_souradnice.x=E->X;puv_souradnice.y=E->Y;
					if(E->geo.typ!=0 || (E->dalsi!=NULL && E->dalsi->geo.typ!=0) || E->geo.delka==0 || (E->dalsi!=NULL && E->dalsi->geo.delka==0))break;//ukončení v případě, že se někde nachází jiná geometrie než linie
					if(E->eID!=MaxInt && E->eID!=200)
					{
			   		switch((int)E->geo.orientace)
			   		{
			   			case   0:E->Y=E->Y-vzdalenost;break;
			   			case  90:E->X=E->X-vzdalenost;break;
			   			case 180:E->Y=E->Y+vzdalenost;break;
							case 270:E->X=E->X+vzdalenost;break;
						}
					}
					//kontrola zda je element stále na linii
					if(F->bod_na_geometrii(0,0,E))//pokud ano
					{
						//aktualizace geometrie
						vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
						if(E->dalsi!=NULL && E->dalsi->geo.typ==0)vloz_G_element(E->dalsi,0,E->geo.X4,E->geo.Y4,0,0,0,0,E->dalsi->geo.X4,E->dalsi->geo.Y4,E->dalsi->geo.orientace);
					}
					//pokud ne budou mu navráceny původní souřadnice
					else {E->X=puv_souradnice.x;E->Y=puv_souradnice.y;}
					E=E->dalsi;
				}
				E=NULL;delete E;
			}
		}
		RET=posun_povolit;
		if(!RET)F->TIP=F->ls->Strings[307];//"Prvek nelze přesunout"
	}
	E=NULL; delete E;
	return RET;
}
////---------------------------------------------------------------------------
//řeší změnu pořadí při posuvu elementů, dojde k novému ukazatelovému propojení, přejmenování a přeindexování elementů
void Cvektory::zmen_poradi_elementu(TElement *E,TElement *Ed)
{             	                          //E = posouvaný element, Ed = další element
	//////přesun elementu před
	if(E->n>Ed->n)
	{
		//ukazatelové záležitosti
		Ed->dalsi=E->dalsi;
		E->predchozi=Ed->predchozi;
		Ed->predchozi->dalsi=E;
		Ed->predchozi=E;
		E->dalsi->predchozi=Ed;
		E->dalsi=Ed;
		if(E->n==ELEMENTY->predchozi->n)ELEMENTY->predchozi=Ed;
		//nahrazení prvního elementu v objektu
		TObjekt *O=vrat_objekt(Ed->objekt_n);
		if(Ed->n==O->element->n)O->element=E;
		O=NULL;delete O;
		//aktualizace geometrie
		vloz_G_element(Ed->dalsi,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,0,0,0,0,F->d.Rxy(Ed->dalsi).x,F->d.Rxy(Ed->dalsi).y,Ed->dalsi->geo.orientace);
		vloz_G_element(E,0,Ed->geo.X1,Ed->geo.Y1,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
		vloz_G_element(Ed,0,F->d.Rxy(E).x,F->d.Rxy(E).y,0,0,0,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,Ed->geo.orientace);
	}
	//////přesun elementu za
	else
	{
		//ukazatelové záležitosti
		E->dalsi->predchozi=E->predchozi;
		E->predchozi->dalsi=E->dalsi;
		Ed->predchozi->dalsi=E;
		E->predchozi=Ed->predchozi;
		E->dalsi=Ed;
		Ed->predchozi=E;
		//nahrazení prvního elementu v objektu
		TObjekt *O=vrat_objekt(E->objekt_n);
		if(E->n==O->element->n)O->element=E->predchozi;
		O=NULL;delete O;
		////aktualizace geometrie
		vloz_G_element(E->predchozi,0,E->geo.X1,E->geo.Y1,0,0,0,0,F->d.Rxy(E->predchozi).x,F->d.Rxy(E->predchozi).y,E->predchozi->geo.orientace);
		vloz_G_element(Ed,0,F->d.Rxy(E).x,F->d.Rxy(E).y,0,0,0,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,Ed->geo.orientace);
		vloz_G_element(E,0,F->d.Rxy(E->predchozi).x,F->d.Rxy(E->predchozi).y,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
	}
	E->pohon=E->dalsi->pohon;//přiřazení správného pohonu
	//////přeindexování (N-hodnoty) v celém seznamu, možno řešit sepáratáně, ale takto to bylo rychleji napsané
	unsigned long n=E->n;
	E->n=Ed->n;Ed->n=n;
	TElement *E_pom=NULL;//ukazatel na první element, přeskočí hlavičku, metoda volaná jen v případě, že existují min. 2 elementy
  T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	if(E->n<Ed->n)E_pom=Ed;//Ed je poslední element z dvojce
	else E_pom=E;
	n=E_pom->n;
	while (E_pom!=NULL)
	{
		E_pom->n=n;
		n++;
		E_pom=dalsi_krok(VYHYBKY,E_pom);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	delete E_pom;E_pom=NULL;
	uprav_popisky_elementu(E);//změna názvů
	if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt || Ed->eID%2==0 && Ed->eID!=100 && Ed->eID!=200 && Ed->eID!=MaxInt)aktualizuj_sparovane_ukazatele();//změna pořadí přičemž alespoň jeden element byl stop-element
}
////---------------------------------------------------------------------------
//vratí vzdálenost od předchozího elementu, pracuje zatím pouze v orotogonalizovaném prostoru (bude nutno vylepšit s příchodem oblouků), pokud se jedná o první element, uvažuje se jako vzdálenost od počátku kabiny (nutno vylepšit ještě pro různé orientace kabiny)
double Cvektory::vzdalenost_od_predchoziho_elementu(TElement *Element,bool pouzeSG)
{
  //deklarace proměnných
	double delka=Element->geo.delka;//délka úseku Elementu, stejná pro oba druhy výpočtů
	TElement *E=Element->predchozi;

	//vzdálenost od předchozího SG elementu, slouží k výpočtům RT, zatím provizorně
	if(pouzeSG)
	{
		while(E!=NULL && E->n>0)//procházení napříč objekty
		{
			if(vrat_druh_elementu(E)==0)break;//pokud je předchozi S&G prěruš cyklus
			else delka+=E->geo.delka;//pokud jdes dál přičti vzdálenost
			E=E->predchozi;
		}
		E=NULL;delete E;
	}

	//vzdálenost k předchozímu funkčnímu elementu
	else
	{
		//pokud se nejedná o první element v kabině hledá vzdálenost k předchozímu funkčnímu elementu, není třeba řešit geometrii - kóty se vykreslují pouze na liniových úsecích
		if(Element->predchozi->n>0 && Element->predchozi->objekt_n==Element->objekt_n)
		{
			while(E!=NULL && E->n>0 && E->objekt_n==Element->objekt_n)//pouze předchozí elementy v objektu
	  	{
				if(E->eID!=MaxInt || E->geo.typ!=0)break;//pokud se jedná o funkční element zastavit průchod
				else delka+=E->geo.delka;//jedná se o zarážku - přičti délku úseku zarážky
	  		E=E->predchozi;
			}
		}
	}

	//ukazatelové záležitosti
	E=NULL;delete E;

	//vrat délku k předchozímu elementu
	return delka;
}
////---------------------------------------------------------------------------
//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje, metoda vrací jednorázový výsledek, ale i zároveň aktulizuje data "Element->data.orientace_jig_pred" již prošlých Elementu, včetně zadávaného
double Cvektory::vrat_rotaci_jigu_po_predchazejicim_elementu(TElement *Element)
{
	double akt_rotace_jigu=0;
	if(ZAKAZKA_akt==NULL)update_akt_zakazky();//pro jistotu, pokud neexistuje akt zakázka vytvoří default
	TCesta *C=ZAKAZKA_akt->cesta->dalsi;
	while(C!=NULL)
	{
		Element->data.orientace_jig_pred=akt_rotace_jigu;//aktulizuje data "Element->data.orientace_jig_pred" již prošlých Elementu, včetně zadávaného tím, že se zadavá do dat a ne do elementu, zohledňuje i vícecestný průchod
		if(Element==C->Element)break;//akcelerátor, skončí cyklus
		else if(C->Element->rotace_jig!=0 && -180<=C->Element->rotace_jig && C->Element->rotace_jig<=180)akt_rotace_jigu+=C->Element->rotace_jig;//stále předcházející elementy, ty mě pro návrátovou hodnotu zajímají, rotace aktuálního elementu se nezohledňuje
		C=C->dalsi;
	}
	C=NULL;delete C;//nutné včetně NULL
	return m.a360(akt_rotace_jigu);//obalené a360 kvůli přetýkání přes 360° součty jednolivých rotací
}
////---------------------------------------------------------------------------
//metoda vrátí ukazatel na poslední rotační element na lince
Cvektory::TElement *Cvektory::vrat_posledni_rotacni_element()
{ //NUTNÝ UPDATE PŘI PŘECHODU NA ZÁKÁZKY RESP. VÝHYBKY!!!
	TElement *RET=NULL,*E=ELEMENTY->predchozi;
	while(E->n>0)
	{
		if(E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180){RET=E;break;}
		E=E->predchozi;
	}
	E=NULL;delete E;
	return RET;
}
////---------------------------------------------------------------------------
//přiřadí Elementu ukazatel na jeho spárovaný element, zároveň aktualizuje tomuto spárovanému elementu spárovaný element + aktualizace první - poslední S&G element
void Cvektory::prirad_sparovany_element(TElement *Element)
{
	if(vrat_druh_elementu(Element)==0)
	{
		TElement *E=Element->predchozi;
		while(E!=NULL && E->n>0)
		{
			if(vrat_druh_elementu(E)==0 && (E->eID!=0 || (E->eID==0 && E->stav>0)))break;//nalezen předchozí S&G
			E=E->predchozi;
		}
		if(E->n!=0)
		{
		 	Element->sparovany=E->sparovany;//nahrazení, vložení za E, takže Element přebere jeho spárovaný
			E->sparovany=Element;
      //vypsání ukazatelů do mgridu
			try
			{
				if(E->eID==0){E->mGrid->Cells[2][1].Text=E->sparovany->name;E->mGrid->Refresh();}
				else if(vrat_druh_elementu(E)==0){E->mGrid->Cells[1][E->mGrid->RowCount-1].Text=E->sparovany->name;E->mGrid->Refresh();}
				if(Element->eID==0){Element->mGrid->Cells[2][1].Text=Element->sparovany->name;Element->mGrid->Refresh();}
				else if(vrat_druh_elementu(Element)==0){Element->mGrid->Cells[1][Element->mGrid->RowCount-1].Text=Element->sparovany->name;Element->mGrid->Refresh();}
			}catch(...){}
		}
		//////první poslední stop element
		if(ELEMENTY->predchozi->n>=2)
		{
			E=ELEMENTY->dalsi;
			while(E!=NULL)
			{
				if(vrat_druh_elementu(E)==0 && (E->eID!=0 || (E->eID==0 && E->stav>0)))break;
				E=E->dalsi;
			}
			if(E!=NULL)
			{
				Cvektory::TElement *prvni_stopka=E;
				E=ELEMENTY->predchozi;
				while(E!=NULL && E->n>0)
				{
					if(vrat_druh_elementu(E)==0 && (E->eID!=0 || (E->eID==0 && E->stav>0)))break;//nalezen předchozí S&G
					E=E->predchozi;
				}
				if(E!=NULL && (E->objekt_n!=prvni_stopka->objekt_n || E->objekt_n==prvni_stopka->objekt_n && E->n!=prvni_stopka->n))//pokud existuje poslední stop elemet a nerovná se mrvnímu
				{
					//bool prvni=false,posledni=false;
					if(prvni_stopka->sparovany==NULL)prvni_stopka->sparovany=E;//prvni=true;}
					if(E->sparovany==NULL)E->sparovany=prvni_stopka;//posledni=true;}
					try //musí být řešeno takto, nelze rozeznat jestli existuje mgird
					{
						if(prvni_stopka->eID==0){prvni_stopka->mGrid->Cells[2][1].Text=E->name;prvni_stopka->mGrid->Refresh();}
						else if(vrat_druh_elementu(prvni_stopka)==0){prvni_stopka->mGrid->Cells[1][prvni_stopka->mGrid->RowCount-1].Text=E->name;prvni_stopka->mGrid->Refresh();}
						if(E->eID==0){E->mGrid->Cells[2][1].Text=prvni_stopka->name;E->mGrid->Refresh();}
						else if(vrat_druh_elementu(E)==0){E->mGrid->Cells[1][E->mGrid->RowCount-1].Text=prvni_stopka->name;E->mGrid->Refresh();}
//						if(prvni && prvni_stopka->eID==0){prvni_stopka->mGrid->Cells[2][1].Text=E->name;prvni_stopka->mGrid->Refresh();}
//						if(posledni && E->eID==0){E->mGrid->Cells[2][1].Text=prvni_stopka->name;E->mGrid->Refresh();}
					}
					catch(...)
					{;}
				}
				prvni_stopka=NULL;delete prvni_stopka;
      }
		}
		E=NULL;delete E;
	}
}
////---------------------------------------------------------------------------
//projde všechny stop-elementy a aktualizuje jim ukazatele na spárované elementy
void Cvektory::aktualizuj_sparovane_ukazatele()
{
	F->aktualizace_RT();//aktualizace RT v tabulkách, vně podmínka OBJEKT_akt!=NULL
	TElement *prvni=NULL,*posledni=NULL;
	//////procházení od prvního elementu a hledání prvního S&G elementu, po nalezení hledání dalšího S&G elementu
	TElement *E=ELEMENTY->dalsi,*E1=NULL;
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL)
	{
		//hledám první S&G element
		if(vrat_druh_elementu(E)==0 && (E->eID!=0 || (E->eID==0 && E->stav>0)))//nalezen první element, nyní musím hledat další S&G element
		{
			if(prvni==NULL)prvni=E;//zapsaní prvního S&G elementu na lince, pouze prvního
			posledni=E;//zapisování každého S&G elementu do ukazatele poslední, na konci zbyde poslední S&G element
			E1=E->dalsi;
			while(E1!=NULL)
			{
				//nalezen další S&G element
				if(vrat_druh_elementu(E1)==0 && (E1->eID!=0 || (E1->eID==0 && E1->stav>0)))break;
				E1=E1->dalsi;
			}
			//////zapsání sparovaného ukazatele
			E->sparovany=E1;
			//uprava tabulek je-li potřeba
			try
			{
				if(F->OBJEKT_akt!=NULL && E->objekt_n==F->OBJEKT_akt->n && E->sparovany!=NULL)
				{
					if(E->eID==0)E->mGrid->Cells[2][1].Text=E->sparovany->name;
					else if(vrat_druh_elementu(E)==0)E->mGrid->Cells[1][E->mGrid->RowCount-1].Text=E->sparovany->name;
				}
			}catch(...){}
			//vymazání pomocných ukazatelů
			E1=NULL;delete E1;
		}
    else E->sparovany==NULL;
		E=dalsi_krok(VYHYBKY,E);
	}
	//ukazatelové záležitosti
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	delete E;E=NULL;

	//////spárovaný ukazatel z posledního elementu na první
	if(posledni!=NULL)posledni->sparovany=prvni;//nutné opodmínkovat!!, aktualizace probíhá i při načítání ze souboru, v souboru může existouvat pouze jeden objekt se zarážkou, tudíž první i poslední ==NULL
	//zapsání do mGridu
	try
	{
		if(F->OBJEKT_akt!=NULL && posledni->objekt_n==F->pom->n)
		{
			if(posledni->eID==0)posledni->mGrid->Cells[2][1].Text=posledni->sparovany->name;
			else posledni->mGrid->Cells[1][posledni->mGrid->RowCount-1].Text=posledni->sparovany->name;
		}
	}catch(...){}
	//ukazatelové záležitosti
	prvni=NULL;delete prvni;
	posledni=NULL;delete posledni;
}
////---------------------------------------------------------------------------
//vypočítá a uloží RT do elementu
void Cvektory::reserve_time(TElement *Element,TCesta *Cesta,bool highlight_bunek,bool refresh_mGrid)
{
	if(Element!=NULL && vrat_druh_elementu(Element)==0)//ošetření proti prázdnému ukazateli a proti KK elementu
	{
		//deklarace
  	double cas=0;
		bool error=false;
		if(Cesta==NULL && ZAKAZKA_akt!=NULL && ZAKAZKA_akt->n!=NULL)//pokud nejsou v datech elementu aktuální informace
		{
			Cesta=vrat_segment_cesty(ZAKAZKA_akt,Element);
			if(Cesta!=NULL)Element->data=Cesta->data;
		}
		//TObjekt *O=vrat_objekt(Element->objekt_n);
		if(Element->pohon!=NULL)cas+=Element->geo.delka/Element->pohon->aRD;//pokud má element pohon výpočet času přejezdu jeho úseku
		else error=true;//nemá pohon = error
		//průchod od Element k předchozím
		TElement *E=Element->predchozi;
		while(E!=NULL && E->n>0)
		{
			if(E->n>0)//kontrola za nejsem na hlavičce až na tomto místě, to dovolí překlopění cyklu na konec pokud dojde na hlaviču (žádoucí)
			{
		  	if(E->eID==200)cas+=E->WT;//wt na předávacím místě
				if((vrat_druh_elementu(E)==0 && (E->eID!=0 || (E->eID==0 && E->data.pocet_voziku>0))) || (E->n==Element->n && E->objekt_n==Element->objekt_n))break;//pokud je předchozi S&G prěruš cyklus
				if(E->pohon!=NULL)cas+=E->geo.delka/E->pohon->aRD;//pokud existuje úsek a má pohon
				else error=true;//jinak error
			}
			E=E->predchozi;
		}
		//výpočet RT a zapsání do dat elemetnu
		double RT=0,WT=Element->WT,WTin=0;
		if(F->scGPCheckBox_meridlo_casy->Checked && Element->sparovany!=NULL)WTin=Element->sparovany->WT;
		if(Element->eID==0/* && Element->data.pocet_voziku>0*/ && cas+Element->WT<PP.TT)WT*=Element->data.pocet_voziku;
		double RD=0;if(Element->pohon!=NULL)RD=Element->pohon->aRD;
		RT=m.RT(Element->data.PT1+Element->data.PT2+Element->PTotoc/*+Element->data.WTstop*/,cas,WT,Element->data.pocet_voziku,RD,WTin);
		Element->data.RT=RT;//ryzí RT

		//vypsání RT do tabulky elementu
		if(F->OBJEKT_akt!=NULL && Element->objekt_n==F->OBJEKT_akt->n)
		{
			switch(Element->eID)
			{
				case 0://stop stanice
				{
					//při posunu stopstanice může dojít ke změně poštu aktuálních vozíku, nový přepočet, pokud k této změně došlo
					if(highlight_bunek)
	  			{
						Element->mGrid->Cells[2][2].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
						Element->data.pocet_pozic=F->max_voziku(Element);
						Element->mGrid->Cells[2][5].Text=Element->data.pocet_pozic;
						if(Element->data.pocet_pozic<Element->data.pocet_voziku)//pokud při posunu akt. počet vozíků přesáhne maximální
	  				{
	  					Element->data.pocet_voziku=Element->data.pocet_pozic;
							Element->data.WTstop=F->m.V2WT(Element->data.pocet_voziku,PP.TT);//uložení do paměti + výpočet
							Element->mGrid->Cells[2][3].Text=F->m.round2double(F->outPT(Element->data.WTstop),3);//OUTPUT
							//nové RT, protože se změnilo WTstop
							WT=Element->WT;
							if(Element->eID==0/* && Element->data.pocet_voziku>1 */&& cas+Element->WT<PP.TT)WT*=Element->data.pocet_voziku;
							RT=m.RT(Element->data.PT1+Element->data.PT2+Element->PTotoc+Element->data.WTstop,cas,WT,Element->data.pocet_voziku,RD);
							Element->data.RT=RT;//ryzí
						}
					}
					//vypsání OK pokud je RT kladné a zároveň má stopka více akt_vozíku
					if(Element->data.RT>0 && Element->data.pocet_voziku>1){Element->mGrid->Cells[2][2].Text="OK";Element->mGrid->Cells[2][2].Hint=F->m.round2double(F->outPT(Element->data.RT),3);Element->mGrid->Cells[2][2].ShowHint=true;}
					else {Element->mGrid->Cells[2][2].Text=F->m.round2double(F->outPT(Element->data.RT),3);Element->mGrid->Cells[2][2].Hint="";Element->mGrid->Cells[2][2].ShowHint=false;}
				}break;
				case 2:case 8:case 12:case 16:case 102:case 106://roboti se stop stanicí
				{
					Element->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(Element->data.RT),3);
	  			if(highlight_bunek)Element->mGrid->Cells[1][2].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
				}break;
    		case 4:case 10:case 14:case 18:case 104:case 108://roboti s aktivní otočí
	  		{
					Element->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(Element->data.RT),3);
					if(highlight_bunek)Element->mGrid->Cells[1][5].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
				}break;
				case 6://aktivní otoč
				{
					Element->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(Element->data.RT),3);
					if(highlight_bunek)Element->mGrid->Cells[1][3].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
				}break;
			}
			//pokud některý z geometrických úseků neměl přiřazený pohon RT nebude správné, vypsat error
			if(error)Element->mGrid->ShowNote(F->ls->Strings[421],F->d.clError,14);//"RT není relevantní, některý z objektů nemá pohon!"
			if(F->OBJEKT_akt->zobrazit_mGrid && refresh_mGrid)Element->mGrid->Refresh();
		}
    //uložení erroru do dat, + 1 000 000 nebo - 1 000 000
		if(error && Element->data.RT>0)Element->data.RT+=1000000;else Element->data.RT-=1000000;
		//narácení dat do segmentu cesty zakázky
		if(Cesta!=NULL)	Cesta->data=Element->data;
	}

	//kontrola RT, pokud je záporné, výpis doporučeného PT
	if(F->OBJEKT_akt!=NULL &&  F->OBJEKT_akt->zobrazit_mGrid && (F->Akce==F->Takce::NIC || F->Akce==F->Takce::ADD))
	{
		TElement *E=F->OBJEKT_akt->element;
    T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		String note="",jednotky=" s";
		if(F->PTunit==F->Tminsec::MIN)jednotky=" min";
		while(E!=NULL)
		{
			try{
				if(vrat_druh_elementu(E)==0 && E->mGrid!=NULL && E->pohon==F->OBJEKT_akt->pohon && (E->eID!=0))// || (E->eID==0 && (E->mGrid->Note.Text=="" || (E->mGrid->Note.Text!="" && E->mGrid->Note.Text.SubString(1,F->ls->Strings[250].Length())!=F->ls->Strings[250] && E->mGrid->Note.Text.SubString(1,F->ls->Strings[251].Length())!=F->ls->Strings[251] && E->mGrid->Note.Text.SubString(1,F->ls->Strings[426].Length())!=F->ls->Strings[426])))))
				{
		  		note="Dop. hodnota PT je maximálně ";
					//kontrola, zda je RT záporné
					if(m.round2double(E->data.RT,5)<0)
					{
						note+=AnsiString(m.round2double(F->outPT(E->data.PT1+E->data.PT2+E->PTotoc+E->data.RT),3))+jednotky;
						E->mGrid->Note.Text=note; //E->mGrid->ShowNote(note);
					}
					else E->mGrid->Note.Text="";
          E->mGrid->Refresh();
				}
			}catch(...){;}//dochází k tomuto při načítání mgridů
			E=dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E;
	}
}
////---------------------------------------------------------------------------
//přepočíta data elementů na danném pohonu
void Cvektory::aktualizuj_data_elementum_na_pohonu(unsigned long pohon_n)
{
	//kontrola existence pohonů a správnosti pohon_n
	if(POHONY!=NULL && POHONY->dalsi!=NULL && 0<pohon_n && pohon_n<=POHONY->predchozi->n && F->OBJEKT_akt!=NULL)
	{
		//deklarace
		TElement *E=ELEMENTY->dalsi;
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();
		//průchod skrze všechny elementy
		while(E!=NULL)
		{
			//kontorla, zda je element na pohon_n a není v akt. editovaném objektu
			if(E->pohon!=NULL && E->pohon->n==pohon_n && E->objekt_n!=F->OBJEKT_akt->n)
			{
				//přepočítání dat
				switch(E->eID)
				{
					//stopstanice
					case 0:
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
            reserve_time(E);
						break;
					}
					//robot (kontinuální)
					case 1:case 7:case 11:case 15:case 101:case 105:
					{
						double CT=m.CT(E->data.LO1,E->pohon->aRD);
            E->data.RT=m.KKRT(CT,E->data.PT1);
						break;
					}
          //robot se stop stanicí
					case 2:case 8:case 12:case 16:case 102:case 106:
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
            reserve_time(E);
						break;
					}
					//robot s pasivní otočí
					case 3:case 9:case 13:case 17:case 103:case 107:
					{
						E->PTotoc=m.PTo(E->OTOC_delka,E->pohon->aRD);
						double CT1=m.CT(E->data.LO1,E->pohon->aRD),CT2=m.CT(E->data.LO2,E->pohon->aRD);
						E->data.RT=m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
						break;
					}
					//robot s aktivní otočí (resp. s otočí a stop stanicí)
					case 4:case 10:case 14:case 18:case 104:case 108:
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
            reserve_time(E);
						break;
					}
					//otoč pasivní
					case 5:
          {
						E->PTotoc=m.PTo(E->OTOC_delka,E->pohon->aRD);
						break;
					}
					//otoč aktivní (resp. otoč se stop stanicí)
					case 6:
          {
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
            reserve_time(E);
						break;
					}
					//PM
					case 200:
					{
						if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)E->WT=m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
						break;
					}
					//výhybky
					case 300:
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
						break;
					}
					//spojka
					case 301:
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
						break;
					}
					default:break;
        }
			}
			//posun na další element
			E=dalsi_krok(VYHYBKY,E);
		}
		//ukazatelové záležistosi
    vymaz_seznam_VYHYBKY(VYHYBKY);
		delete E;E=NULL;
  }
}
////---------------------------------------------------------------------------
//vrátí poslední element v objektu
Cvektory::TElement *Cvektory::vrat_posledni_element_objektu(TObjekt *Objekt)
{
	if(Objekt!=NULL && Objekt->element!=NULL)
	{
		TElement *ret=Objekt->element,*E=Objekt->element;
		while(E!=NULL && E->objekt_n==Objekt->n)
		{
			if(E->dalsi!=NULL && E->dalsi->objekt_n==Objekt->n)E=E->dalsi;
			else break;
		}
		ret=E;
		E=NULL;delete E;
		return ret;
	}
	else return NULL;
}
////---------------------------------------------------------------------------
Cvektory::T2Element *Cvektory::hlavicka_seznam_VYHYBKY()
{
	T2Element *VYHYBKY=new T2Element;
	VYHYBKY->n=0;
	VYHYBKY->vyhybka=NULL;
	VYHYBKY->spojka=NULL;
	VYHYBKY->dalsi=NULL;
	VYHYBKY->predchozi=VYHYBKY;
  return VYHYBKY;
}
////---------------------------------------------------------------------------
void Cvektory::uloz_vyhybku_do_seznamu(TElement *vyhybka,T2Element *VYHYBKY)
{
	T2Element *novy=new T2Element;
	novy->n=VYHYBKY->predchozi->n+1;
	novy->vyhybka=vyhybka;
	novy->spojka=NULL;
	novy->dalsi=NULL;
	novy->predchozi=VYHYBKY->predchozi;
	VYHYBKY->predchozi->dalsi=novy;
	VYHYBKY->predchozi=novy;
	novy=NULL;delete novy;

}
////---------------------------------------------------------------------------
void Cvektory::smaz_vyhybku_ze_seznamu(T2Element *VYHYBKY)
{
	T2Element *smaz=VYHYBKY->predchozi;
	smaz->predchozi->dalsi=NULL;
	VYHYBKY->predchozi=smaz->predchozi;
	delete smaz;smaz=NULL;
}
////---------------------------------------------------------------------------
long Cvektory::vymaz_seznam_VYHYBKY(T2Element *VYHYBKY)
{
	long pocet_smazanych=0;
	while(VYHYBKY!=NULL)
	{

		delete VYHYBKY->predchozi;
		VYHYBKY->predchozi=NULL;
		VYHYBKY=VYHYBKY->dalsi;
	}
	delete VYHYBKY;VYHYBKY=NULL;
	return  pocet_smazanych;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//určí další krok průchodového algoritmu ve spojáku elementů, 2 možností průchod kompletního spojáku ELEMENTY, druhá průchod pouze elementů jednoho objektu
Cvektory::TElement *Cvektory::dalsi_krok(T2Element *VYHYBKY,TElement *E,TObjekt *O)
{
  F->log(__func__);
	////normální průchod kompletním spojákem elementů
	if(O==NULL)
	{
		if(E->eID==300 && E->predchozi2==NULL)//ošetření proti průchodu při vkládání spojky, výhybka nemá jeětě nadefinovanou vedjší větev musím se k ní chovat jako k elementu
		{
			E=E->dalsi;
			if(E!=NULL && E->eID==301 && VYHYBKY->predchozi->n!=0 && E->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka)
			{
				do
				{
					E=VYHYBKY->predchozi->vyhybka->dalsi;//navrácení zpět na výhybku
					smaz_vyhybku_ze_seznamu(VYHYBKY);//smazání výhybky kterou jsem již prošel
				}while(E->eID==301 && VYHYBKY->predchozi->n!=0 && E->dalsi->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka);
			}
		}
  	else
  	{
  		//jsem na výhybce
  		if(E->eID==300)
  		{
				uloz_vyhybku_do_seznamu(E,VYHYBKY);//uloži ji do seznamu, abych věděl kam se mám vrátit
  			E=E->dalsi2;//další krok bude na sekundární větev
  			//kontrola jestli ve vedlejší větvi není pouze spojka
				if(E!=NULL && E->eID==301 && VYHYBKY->predchozi->n!=0 && E->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka)
  			{
  				//navrácení na hlavní větev
  				do
  				{
  					E=VYHYBKY->predchozi->vyhybka->dalsi;//navrácení zpět na výhybku
						smaz_vyhybku_ze_seznamu(VYHYBKY);//smazání výhybky kterou jsem již prošel
  				}while(E->eID==301 && VYHYBKY->predchozi->n!=0 && E->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka);
  			}
  		}
  		//jsem v sekundární vetvi na posledním elementu před spojkou
			else if(E->dalsi!=NULL && E->dalsi->eID==301 && VYHYBKY->predchozi->n!=0 && E->dalsi->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka)
			{
  			//navrácení na hlavní větev + kontrola zda za vyhybkou není hned spojka (výhybka v sekundární větvi)
  			do
    		{
  				E=VYHYBKY->predchozi->vyhybka->dalsi;//navrácení zpět na výhybku
					smaz_vyhybku_ze_seznamu(VYHYBKY);//smazání výhybky kterou jsem již prošel
  			}while(E->eID==301 && VYHYBKY->predchozi->n!=0 && E->dalsi->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka);
      }
  		else E=E->dalsi;
		}
	}

	////průchod pouze v jednom objektu, odlehčený alg. kompletního průchodu k testům
	else
	{
		//jsem na výhybce
		if(E->eID==300 && E->predchozi2!=NULL)
		{
			uloz_vyhybku_do_seznamu(E,VYHYBKY);//uloži ji do seznamu, abych věděl kam se mám vrátit
			E=E->dalsi2;//další krok bude na sekundární větev
			//kontrola jestli ve vedlejší větvi není pouze spojka
			if(E!=NULL && E->eID==301 && VYHYBKY->predchozi->n!=0 && E->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka || E->objekt_n!=O->n)
			{
				//navrácení na hlavní větev
				E=VYHYBKY->predchozi->vyhybka->dalsi;//navrácení zpět na výhybku
				smaz_vyhybku_ze_seznamu(VYHYBKY);//smazání výhybky kterou jsem již prošel
			}
		}
		//jsem v sekundární vetvi na posledním elementu před spojkou
		else if(E->dalsi!=NULL && (VYHYBKY->predchozi->n!=0 && ((E->dalsi->eID==301 && E->dalsi->identifikator_vyhybka_spojka==VYHYBKY->predchozi->vyhybka->identifikator_vyhybka_spojka) || E->dalsi->objekt_n!=O->n)))
		{
			//navrácení na hlavní větev + kontrola zda za vyhybkou není hned spojka (výhybka v sekundární větvi)
			E=VYHYBKY->predchozi->vyhybka->dalsi;//navrácení zpět na výhybku
			smaz_vyhybku_ze_seznamu(VYHYBKY);//smazání výhybky kterou jsem již prošel
		}
		else if(E->eID==301 && E->dalsi2->objekt_n!=O->n && (E!=VYHYBKY->spojka || VYHYBKY->spojka==NULL))//posun na sekundární větev na hranice objektu
		{
			VYHYBKY->spojka=E;//uložení spojky na které jsem již byl
			E=E->predchozi2;
			if(E->objekt_n!=O->n && E->eID!=300)E=E->dalsi->dalsi;//přesun z veldejší větve na element na hlavní větvi za spojkou
			else if(E->objekt_n!=O->n && E->eID==300)E=E->dalsi2->dalsi;
			else if(E->objekt_n==O->n){while(E->objekt_n==O->n){E=E->predchozi;}if(E->eID!=300)E=E->dalsi;else E=E->dalsi2;}
		}
		else E=E->dalsi;
		//if(E!=NULL && E==VYHYBKY->spojka)E=E->dalsi;
		if(E!=NULL && E->objekt_n!=O->n)E=NULL;//pokud jsem již mimo objekt
	}
	//vrat další krok
	return E;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//spíše do zásoby procházení pozadu
Cvektory::TElement *Cvektory::predchozi_krok(TElement *E,TObjekt *O)
{
	//jsem na výhybce
//	if(E->eID==301)
//	{
//		uloz_vyhybku_do_seznamu(E);//uloži ji do seznamu, abych věděl kam se mám vrátit
//		E=E->predchozi2;//další krok bude na sekundární větev
//	}
//	//jsem v sekundární vetvi na posledním elementu před spojkou
//	else if(E->predchozi!=NULL && E->predchozi->eID==300 && VYHYBKY->predchozi->n!=0)
//	{
//		E=VYHYBKY->predchozi->vyhybka->predchozi;//navrácení zpět na výhybku
//		smaz_vyhybku_ze_seznamu();//smazání výhybky kterou jsem již prošel
//		//kontrola zda za vyhybkou není hned spojka (výhybka v sekundární větvi)
//		if(E->eID==300)
//		{
//			E=VYHYBKY->predchozi->vyhybka->predchozi;//pokud ano vrat se na predchozi vyhybku
//			smaz_vyhybku_ze_seznamu();//smazání výhybky kterou jsem již prošel
//		}
//	}
//	else E=E->predchozi;
	//vrat další krok
	return E;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//vytvoří tabulku průchodu pro alg. sekundarni_zapis_cteni()
Cvektory::T2Element *Cvektory::vytvor_tabElm_pruchodu()
{
	T2Element *tab_pruchodu=new T2Element[pocet_vyhybek+1];
	//nulování počtu průchodů přes jednotlivé výhybky a spojky
	for(unsigned int i=0;i<pocet_vyhybek+1;i++)
	{
		tab_pruchodu[i].vyhybka=NULL;
		tab_pruchodu[i].vyhybka_pocet=0;
    tab_pruchodu[i].spojka=NULL;
		tab_pruchodu[i].spojka_pocet=0;
	}
  //nulování pomocného atributu, spíše pro jistotu
	vyhybka_pom=NULL;
  return tab_pruchodu;
}
////---------------------------------------------------------------------------
//slouží k ukládání elementů do binárky a pro jejich čtení a zároveň uložení do spojáku
Cvektory::TElement *Cvektory::sekvencni_zapis_cteni(TElement *E,TPoint *tab_pruchodu_TP,T2Element *tab_pruchodu_T2E)
{
	////zápis pomocí sekvenčního algoritmu
	if(tab_pruchodu_TP!=NULL && tab_pruchodu_T2E==NULL)
	{
		//deklarace proměnných
		unsigned int pruchod=0;

		//aktuálně se ukazatel nachází na výhybce
		if(E->eID==300)
		{
			pruchod=tab_pruchodu_TP[E->identifikator_vyhybka_spojka-1].x;//uložení stavu průchodu před navýšením
			tab_pruchodu_TP[E->identifikator_vyhybka_spojka-1].x+=1;//navýšení stavu průchodu, musí být zde, v dalším řádku ztracím ukazatel na výhybku
			//podle průcohdu (první nebo další) určí následující element
			if(pruchod==0)E=E->dalsi2;//první průchod, následuje element z vedlejší větve
			else E=E->dalsi;//další průchod, následuje element z hlavní větve
		}
		//aktuálně se ukazatel nachází na spojce
		else if(E->eID==301)
		{
			pruchod=tab_pruchodu_TP[E->identifikator_vyhybka_spojka-1].y;//uložení stavu průchodu před navýšením
			tab_pruchodu_TP[E->identifikator_vyhybka_spojka-1].y+=1;//navýšení stavu průchodu, musí být zde, v dalším řádku ztracím ukazatel na spojku
			//podle průcohdu (první nebo další) určí následující element
			if(pruchod==0)E=E->dalsi2;//první průchod, následuje spárovaná výhybka
			else E=E->dalsi;//další průchod následuje další element
		}
		//aktuálně se ukazatel nachází na elementu, který není ani výhybka ani spojka
		else E=E->dalsi;
	}

	////čtení pomocí sekvenčního algoritmu + ukládání
	else
	{
		//pokud neexistuje hlavička vytvoří ji
		if(ELEMENTY==NULL)hlavicka_ELEMENTY();

		//aktualizace počtu průchodů přes elementy, platí pouze pro výhybky a spojky
		if(E->eID==300)tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka_pocet++;
		if(E->eID==301)tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka_pocet++;

		////první průchod skre spojku, výhybku nebo průchod přes ostatní elementy
		if((E->eID==300 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka==NULL) || (E->eID==301 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka==NULL) || (E->eID!=300 && E->eID!=301))
		{
			//nulování šech ukazatelů elementu
			E->dalsi=NULL;E->dalsi2=NULL;
			E->predchozi=NULL;E->predchozi2=NULL;

			//vkládám spojku
			if(E->eID==301)
			{
				if(vyhybka_pom==NULL)//přišel jsem na spojku z hlavní větve
				{
					E->predchozi=ELEMENTY->predchozi;
					ELEMENTY->predchozi->dalsi=E;
					ELEMENTY->predchozi=E;
				}
				else//přišel jsem na spojku z vedlejší větve nebo hlavní větve
				{
					if(tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka!=NULL)E->predchozi2=vyhybka_pom;//pokud má spojka již vloženou výhybku přišel sem z vedlejší větve
					else E->predchozi=vyhybka_pom;//spojka nemá výhybku, přišel sem z hlavní vetve
					if(vyhybka_pom->eID==300 && vyhybka_pom->dalsi2==NULL)vyhybka_pom->dalsi2=E;//předchozí výhybka
					else vyhybka_pom->dalsi=E;//předchozí je běžný element
					vyhybka_pom=E;
				}
				vyhybka_pom=E;//uložení pro průchod vedlejší větve
				tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka=E;//uložení do tabulky průchodů
			}
			//vkládám výhybku
			else if(E->eID==300)
			{
				//případ na výhybku sem narazil jako první, její spojka ještě není vložená
				if(tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka==NULL)
				{
					if(vyhybka_pom==NULL)//přišel jsem na vyhybku z hlavní větve
					{
						E->predchozi=ELEMENTY->predchozi;
						ELEMENTY->predchozi->dalsi=E;
						ELEMENTY->predchozi=E;
					}
					else//přišel jsem na vyhybku z vedlejší větve
					{
						E->predchozi=vyhybka_pom;
						if(vyhybka_pom->eID==300 && tab_pruchodu_T2E[vyhybka_pom->identifikator_vyhybka_spojka].vyhybka_pocet==1)vyhybka_pom->dalsi2=E;//předchozí je výhybka, na která byla v předchozím kroku vložena = teď jsem na dalsi2 (vedlejší větev)
						else vyhybka_pom->dalsi=E;//běžný element, nebo výhybka z hlavní větve
					}
				}
				//předchozí element by spojka pro aktuální výhybku, nedelám nic
				vyhybka_pom=E;//uložení pro průchod vedlejší větve
				tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka=E;//uložení do tabulky průchodů
			}
			//vkládám ostatní elementy
			else
			{
				if(vyhybka_pom==NULL)//vkládání elementu na hlavní větev
				{
					E->predchozi=ELEMENTY->predchozi;
					ELEMENTY->predchozi->dalsi=E;
					ELEMENTY->predchozi=E;
				}
				else//vkládání elementu na vedlejší větev
				{
					E->predchozi=vyhybka_pom;
					if(vyhybka_pom->eID==300 && vyhybka_pom->dalsi2==NULL)vyhybka_pom->dalsi2=E;//předchozí je běžný element
					else vyhybka_pom->dalsi=E;//předchozí výhybka
					vyhybka_pom=E;//uložení pro průchod vedlejší větve
				}
			}
		}

		////další průchod přes spojku, tz. aktualizace ukazatelů
		else if(E->eID==301 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka!=NULL)
		{
			//načtení již vložené výhybky
			E=tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka;
			//aktualizace zbývajících ukazatelů
			if(vyhybka_pom==NULL)//přišel jsem na spojku z hlavní větve
			{                  
				if(E->predchozi==NULL)E->predchozi=ELEMENTY->predchozi;//pokud ještě není nastavena hlavní větev nastaví ji
				if(E->predchozi->eID==300 && E->predchozi->dalsi2==NULL)ELEMENTY->predchozi->dalsi2=E;
				else E->predchozi->dalsi=E;
				ELEMENTY->predchozi=E;
			}
			if(vyhybka_pom!=NULL)//přišel jsem na spojku z vedlejší větve
			{                       
				if(E->predchozi==NULL)E->predchozi=vyhybka_pom;//pokud ještě není nastavena hlavní větev nastaví ji
				if(E->predchozi2==NULL)E->predchozi2=vyhybka_pom;//pokud ještě není nadefinovaná vedlejší větev nastaví ji
				if(vyhybka_pom->eID==300 && vyhybka_pom->dalsi2==NULL)vyhybka_pom->dalsi2=E;//předchozí je běžný element
				else vyhybka_pom->dalsi=E;//předchozí výhybka
				vyhybka_pom=E;//na vyhybce vždy končí vedlejší větev
			}
			//propojení vyhybky a spojky
			TElement *V=tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka;
			V->predchozi2=E;E->dalsi2=V;
			V=NULL;delete V;
		}

		////další průchod přes výhybku, tz. aktualizace ukazatelů
		else if(E->eID==300 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka!=NULL)
		{
			//načtení již vložené výhybky
			E=tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka;
			//aktualizace zbývajících ukazatelů
			if(vyhybka_pom==NULL/* && E->predchozi==NULL*/)//přišel jsem na vyhybku z hlavní větve
			{
				if(E->predchozi==NULL)E->predchozi=ELEMENTY->predchozi;//pokud výhybka má nedokončenou cestu po hlavní větvi nastaví ji
				if(E->predchozi->eID==300 && E->predchozi->dalsi2==NULL)E->predchozi->dalsi2=E;
				else E->predchozi->dalsi=E;
				ELEMENTY->predchozi=E;
			}
			else if(E->predchozi==NULL)//přišel jsem na vyhybku z vedlejší větve
			{
				E->predchozi=vyhybka_pom;
				if(vyhybka_pom->eID==300 && vyhybka_pom->dalsi2==NULL)vyhybka_pom->dalsi2=E;//předchozí je výhybka, která nema nadefinovanou vedlejší větev
				else vyhybka_pom->dalsi=E;//předchozí je běžný element, nebo vyhybka, která nemá nadefinovanou hlavní vetev
			}
			if(E!=ELEMENTY->predchozi)vyhybka_pom=E;//druhý průchod výhybkou v sekundarní větvi, tz. nevrátil sem se na hlavní větev musím tedy udržovat poslední element v vyhybka_pom
		}

		////jsem na vyhybce nebo spojce a mám naplněný ukazatel vyhybka_pom (udržuje poslední element v sek. vetvi), kontrola zda sem uzavřel sekundární větce
		if(vyhybka_pom!=NULL && (E->eID==300 || E->eID==301))
		{
			unsigned int nedokoncene=0;
			//průchod uloženými vyhybkami a spojkami, kontrola na zda mám nějaké neuzavřené větve
			for(unsigned int i=1;i<pocet_vyhybek+1;i++)
			{
				if((tab_pruchodu_T2E[i].vyhybka_pocet>=1 && tab_pruchodu_T2E[i].spojka_pocet>=1 && tab_pruchodu_T2E[i].vyhybka->dalsi2!=NULL && tab_pruchodu_T2E[i].spojka->predchozi2!=NULL) || (tab_pruchodu_T2E[i].vyhybka_pocet==0 && tab_pruchodu_T2E[i].spojka_pocet==0));
				else
				{
					nedokoncene++;
					break;
				}
			}
			//všechny dosavadní větve jsou kompletní, smaž vyhybka_pom
			if(nedokoncene==0)
			{
				ELEMENTY->predchozi=vyhybka_pom;
				vyhybka_pom->dalsi=NULL;
				vyhybka_pom=NULL;
			}
		}

		//kontrola jestli má objekt ukazatel na svůj první element
		TObjekt *O=vrat_objekt(E->objekt_n);
		if(O!=NULL && E->n==O->element_n)O->element=E;//načtení prvního elementu v objektu

		//ukazatelové záležitosti
		O=NULL;delete O;
	}
	return E;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//smaže spojku nebo vyhybku společně s jejich větví
void Cvektory::smaz_vyhybku_spojku(TElement *Element,unsigned long maz_OBJ)
{
	//deklarace proměnných + založení seznamu smazat
	T2Element *smazat=new T2Element,*pom=new T2Element;
	smazat->n=0;smazat->dalsi=NULL;smazat->predchozi=smazat;
	TElement *E=NULL;
	if(Element->eID==301)Element=Element->dalsi2;//pokud je Element spojka přesunu se na výhybku

	//průchod skrze všechny ovlivněné elementy
	T2Element *tab_pruchodu_T2E=vytvor_tabElm_pruchodu();
	TPoint *tab=new TPoint[pocet_vyhybek];
	E=Element;
	while(E!=NULL)
	{
		//aktualizace počtu průchodů přes elementy, platí pouze pro výhybky a spojky
		if(E->eID==300){tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka=E;tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka_pocet++;}
		if(E->eID==301){tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka=E;tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka_pocet++;}

		//uložení elementu do spojáku mazaných
		if((E->eID==300 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].vyhybka_pocet<=1) || (E->eID==301 && tab_pruchodu_T2E[E->identifikator_vyhybka_spojka].spojka_pocet<=1) || (E->eID!=300 && E->eID!=301))
		{
			pom=new T2Element;
			pom->vyhybka=E;
			pom->n=smazat->predchozi->n+1;
			pom->dalsi=NULL;
			pom->predchozi=smazat->predchozi;
			smazat->predchozi->dalsi=pom;
			smazat->predchozi=pom;
			pom=NULL;delete pom;   
		}

		//posun na další element
		E=sekvencni_zapis_cteni(E,tab,NULL);

		////jsem na vyhybce nebo spojce a mám naplněný ukazatel vyhybka_pom (udržuje poslední element v sek. vetvi), kontrola zda sem uzavřel sekundární větce
  	unsigned int nedokoncene=0;
  	//průchod uloženými vyhybkami a spojkami, kontrola na zda mám nějaké neuzavřené větve
		for(unsigned int i=1;i<pocet_vyhybek+1;i++)
		{
			if((tab_pruchodu_T2E[i].vyhybka_pocet>=1 && tab_pruchodu_T2E[i].spojka_pocet>=1 && tab_pruchodu_T2E[i].vyhybka->dalsi2!=NULL && tab_pruchodu_T2E[i].spojka->predchozi2!=NULL) || (tab_pruchodu_T2E[i].vyhybka_pocet==0 && tab_pruchodu_T2E[i].spojka_pocet==0));
			else
			{
  			nedokoncene++;
				break;
  		}
		}
		if(nedokoncene==0)break;
	}
	delete []tab_pruchodu_T2E;
	delete []tab;

	////samotné mazání
	//unsigned long objekt_n=0,O1_n=Element->objekt_n,O2_n=Element->predchozi2->objekt_n;//uchovávání n objektů, kde se nachází výhybka a spojka
	do
	{
		pom=smazat->predchozi;//od konce
		E=pom->vyhybka;//uložení mazaného elementu do pomocného ukazatele
		//if(objekt_n!=0 && objekt_n!=E->objekt_n && objekt_n!=O1_n && objekt_n!=O2_n)smaz_objekt(vrat_objekt(objekt_n));//mazání prázdného objektu
		//objekt_n=E->objekt_n;
		//pokud mažu výhybku nebo spojku je důležité upravit ukazatele na hlavní větvi a geometrii
		if(E->eID==300 || E->eID==301)smaz_element(E,true,maz_OBJ);
		else//mazání ostatních elementů
		{
			if(E->mGrid!=NULL && F->OBJEKT_akt!=NULL && (E->objekt_n==F->OBJEKT_akt->n || E==F->predchozi_PM))
			{
				try
				{
					E->mGrid->Delete();
				}catch(...){;}
			}
			E->mGrid=NULL;
			//odebrání ukazatele na první element
			TObjekt *o=vrat_objekt(E->objekt_n);
			if(o!=NULL && E==o->element)o->element=NULL;
			o=NULL;delete o;
			//samotné smazání elementu
			delete E;E=NULL;
		}
		//vyřazení záznamu se seznamu smazat
		smazat->predchozi=pom->predchozi;
		//smazání záznamu
		delete pom;pom=NULL;
		//pokud jsem na hlavičce seznamu smazat, smažu hlavičku a cyklus se ukončí
		if(smazat->predchozi->n==0)
		{
			delete smazat;
			smazat=NULL;
		}
	}while(smazat!=NULL);

	//upravení počtu výhybek
	aktualizuj_identifikator_vyhybky_spojky();
	//kontrola zda nezbyl nějaký prázdný objekt bez elementů, pokud ano smaže jej
	TObjekt *O=OBJEKTY->dalsi,*o_pom=NULL;
	while(O!=NULL)
	{
		o_pom=O;
		O=O->dalsi;
		if(o_pom->element==NULL)smaz_objekt(o_pom);
	}
	o_pom=NULL;delete o_pom;
	delete O;O=NULL;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//aktualizuje identifikátory výhybkám a spojkám, přejmenuje
void Cvektory::aktualizuj_identifikator_vyhybky_spojky()
{
	//deklarace
  T2Element *smazat=new T2Element,*pom=new T2Element;
	smazat->n=0;smazat->dalsi=NULL;smazat->predchozi=smazat;
	TElement *E=ELEMENTY->dalsi;//průchod spojáku elementů od začátku
	int pocet=0;//počet výhybek, začínám od začátku
						
	//průchod kompletního spojáku
	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL)
	{
		if(E->eID==300)//narazil jsem na výhybku
		{			
      //navýšení počtu
			pocet++;
      //uložení do pomoxného spojáku
	  	pom=new T2Element;
	  	pom->vyhybka=E;
	  	pom->n=smazat->predchozi->n+1;
	  	pom->dalsi=NULL;
	  	pom->predchozi=smazat->predchozi;
	  	smazat->predchozi->dalsi=pom;
	  	smazat->predchozi=pom;
			pom=NULL;delete pom;
		}
		E=dalsi_krok(VYHYBKY,E);
	}

	//ukazatelové záležitosti
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	delete E;E=NULL;

  //zapsání počtu výhybek do proměnné
	pocet_vyhybek=pocet;

	//změna názvu a identifikátoru, musí být řešena extra průchodem, nelze měnit identifikátory při průchodu pomocí alg. dalsi_krok ani sekvencni_cteni_zapis
	do
	{
		pom=smazat->predchozi;//od konce
		if(pom->vyhybka!=NULL && pom->vyhybka->eID==300)
		{
			E=pom->vyhybka;
			E->identifikator_vyhybka_spojka=pocet;//zapsání identifikátoru do výhybky
			E->name="Výhybka "+AnsiString(pocet);//změna názvu
			E->predchozi2->identifikator_vyhybka_spojka=pocet;//zapsání identifikátoru do spojky
			E->predchozi2->name="Spojka "+AnsiString(pocet);//změna názvu
      pocet--;//jdu od poslední výhybky, proto odečítám
		}
		//vyřazení záznamu se seznamu smazat
		smazat->predchozi=pom->predchozi;
		smazat->predchozi->dalsi=NULL;
		//smazání záznamu
		pom->vyhybka=NULL;
		delete pom;pom=NULL;
		//pokud jsem na hlavičce seznamu smazat, smažu hlavičku a cyklus se ukončí
		if(smazat->predchozi->n==0)
		{
			delete smazat;
			smazat=NULL;
		}
	}while(smazat!=NULL);

	//ukazatelové záležitosti
	E=NULL;delete E;
	delete smazat;smazat=NULL;
	delete pom;pom=NULL;

}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//smaže element ze seznamu
void Cvektory::smaz_element(TElement *Element,bool preskocit_kontolu,unsigned long maz_OBJ)
{
	bool povolit=true;
	bool zobrazit_tip=true;
	TObjekt *O=vrat_objekt(Element->objekt_n);
	if((Element->eID==300 || Element->eID==301) && !preskocit_kontolu)
	{
		//zajištění aby nebyla spuštěna následující funkcionalita
		povolit=false;
		preskocit_kontolu=false;
		zobrazit_tip=false;
		smaz_vyhybku_spojku(Element,maz_OBJ);
	}
	if(Element->eID==200 && !preskocit_kontolu && ((Element->dalsi!=NULL && Element->dalsi->pohon!=Element->pohon) || (Element->dalsi==NULL && ELEMENTY->dalsi->pohon!=Element->pohon)))//mazání PM
	{
		//pokud se jedná o PM na konci objektu .. nelze smazat
		//if(Element->dalsi==NULL || Element->dalsi!=NULL && Element->dalsi->objekt_n!=Element->objekt_n)povolit=false;
		if(povolit)//budu matet PM ve středu objektu .. tz. budou min. 2 PM v objektu
		{
			//před mazáním PM je potřeba sjednotit pohon za mazaným PM do dalsího PM
			unsigned int obj_n=Element->objekt_n;
			TElement *dalsi_PM=Element->dalsi;
			while(dalsi_PM!=NULL/* && dalsi_PM->objekt_n==Element->objekt_n*/)
			{
				if(Element->pohon!=NULL)dalsi_PM->pohon=Element->pohon;else dalsi_PM->pohon=NULL;
				if(obj_n!=dalsi_PM->objekt_n)//pokud jsem v jiném objektu než jsem začal, přiřadím mu aktuélně editovaný pohon
				{
					obj_n=dalsi_PM->objekt_n;
					vrat_objekt(obj_n)->pohon=Element->pohon;
        }
				if(dalsi_PM->eID==200)
				{
          //aktualizace comba u dalšího PM
					if(dalsi_PM->objekt_n==F->OBJEKT_akt->n && dalsi_PM->mGrid!=NULL)
					{
						//prohození sloupců
						int prvni=3;
						if(F->prohodit_sloupce_PM(dalsi_PM))prvni=4;
						//aktualizace itemindexu
						try
						{
					  	if(dalsi_PM->mGrid->Cells[prvni][2].Type==TmGrid::COMBO)
					  	{
                int p_n=0;
					  		if(Element->pohon!=NULL)p_n=Element->pohon->n;
					  		TscGPComboBox *Combo=dalsi_PM->mGrid->getCombo(prvni,2);
					  		if(Combo!=NULL)Combo->ItemIndex=p_n;
					  		Combo=NULL;delete Combo;
					  	}
						}catch(...){;}
					}
					//okončneí průchodu
					break;
				}
				dalsi_PM=dalsi_PM->dalsi;
			}
			dalsi_PM=NULL;delete dalsi_PM;
		}
	}
	//hláška uživateli
	if(!povolit && F->OBJEKT_akt!=NULL && zobrazit_tip)F->TIP=F->ls->Strings[315];//"Nelze odstranit předávací místo"
	if(povolit && (Element->eID!=200 || (Element->eID==200 && preskocit_kontolu) || (Element->eID==200 && Element->geo.delka==0) || (Element->eID==200 && Element->dalsi!=NULL && Element->dalsi->objekt_n==Element->objekt_n && Element->dalsi->geo.delka!=0)) && (Element->dalsi==NULL || Element->dalsi!=NULL && ((Element->geo.typ==0 && Element->dalsi->geo.typ==0) || Element->geo.delka==0) || (maz_OBJ>0/* && Element->objekt_n==maz_OBJ*/)))//pokud bude hlavní větev pokračovat obloukem, problém při ukončování projektu kontrola
	{
		if(O!=NULL && O->element->n==Element->n && Element->dalsi!=NULL && Element->dalsi->objekt_n==O->n)O->element=Element->dalsi;
		//nejdříve smazání tabulky Elelementu
		if(Element->mGrid!=NULL)
		{
			Element->mGrid->Delete();
			Element->mGrid=NULL;
		}
		//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
		if(Element->dalsi!=NULL)//ošetření proti poslednímu prvku
		{
			if(Element->predchozi!=NULL)//nutná kontrola, může dojít, že tyto ukazatele neexistují, to vzníká při odstraňování větví
			{
				if(Element->predchozi->eID==300 && Element->predchozi->dalsi2==Element && Element->predchozi->dalsi!=Element)Element->predchozi->dalsi2=Element->dalsi;
				else Element->predchozi->dalsi=Element->dalsi;
			}
			if(Element->dalsi!=NULL && Element->predchozi!=NULL)//nutná kontrola, může dojít, že tyto ukazatele neexistují, to vzníká při odstraňování větví
			{
				if(Element->dalsi->eID==301 && Element->dalsi->predchozi2==Element && Element->dalsi->predchozi!=Element)Element->dalsi->predchozi2=Element->predchozi;
				else Element->dalsi->predchozi=Element->predchozi;
			}
			if(Element->dalsi!=NULL && Element->dalsi->geo.typ==0 && Element->predchozi!=NULL && F->Akce!=F->GEOMETRIE && O!=NULL)//u geo. se upravuje geometrie ostatních elemntů zvlášť v Unit1, pokud je O==NULL mažu elementy smazaného objektu = neupravovat geo. dalších elementů
			{
				if(Element==ELEMENTY->dalsi)vloz_G_element(Element->dalsi,0,Element->predchozi->geo.X4,Element->predchozi->geo.Y4,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
				else vloz_G_element(Element->dalsi,0,Element->geo.X1,Element->geo.Y1,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
			}
		}
		else//poslední prvek                                                                           ¨
		{
			if(Element->n==1)//pokud je mazaný prvek hned za hlavičkou, tj. jedne prvek ve spojáku
			{
				ELEMENTY->dalsi=NULL;
				ELEMENTY->predchozi=ELEMENTY;
			}
			else
			{
				Element->predchozi->dalsi=NULL;
				ELEMENTY->predchozi=Element->predchozi;//zapis do hlavičky poslední prvek seznamu
			}
		}
		//odebrání ukazatele na první element
		TObjekt *o=vrat_objekt(Element->objekt_n);
		if(o!=NULL && Element==o->element)o->element=NULL;
		o=NULL;delete o;
		//odstranění z pěměti
		delete Element;Element=NULL;
	}
	else if(povolit)//změna na zarážku, v případě že mažu element který obsahuje složitejsí geometrii
	{
		//smazání a znovuvytvoření mGridu elementu
		long ID=Element->n;
		if(F->OBJEKT_akt!=NULL && (Element->objekt_n==F->OBJEKT_akt->n || Element==F->predchozi_PM))
		{
      ID=Element->mGrid->ID;
			Element->mGrid->Delete();
			Element->mGrid=NULL;
		}
		//změna na zarážku
		Element->eID=MaxInt;
		//změna názvu a úprava číslování
		Element->name="Zarážka";
		//uprav_popisky_elementu(Element);//přesunuto na úroveň unit1 smazat1click
		//defaultní data
		Element->data.LO1=1.5;
		Element->OTOC_delka=0;
		Element->zona_pred=0;
		Element->zona_za=0;
		Element->data.LO2=0;
		Element->data.LO_pozice=0;
		Element->data.PT1=0;
		Element->PTotoc=0;
		Element->data.PT2=0;
		Element->WT=0;//čekání na palec
		Element->data.WTstop=0;//čekání na stopce
		Element->data.RT=0;//ryzí reserve time
		Element->data.pocet_voziku=0;
		Element->data.pocet_pozic=0;
		Element->rotace_jig=0;
		if(vrat_druh_elementu(Element)==0){Element->data.pocet_pozic=1;Element->data.pocet_voziku=1;}//S&G elementy
		if(Element->eID==0){Element->data.pocet_pozic=2;Element->data.pocet_voziku=1;}//pouze stopky
		Element->stav=1;
		Element->data.PD=-1;//defaultní stav pro S&G roboty
		Element->dalsi2=NULL;
		Element->predchozi2=NULL;
		//smazání a znovuvytvoření mGridu elementu
		if(F->OBJEKT_akt!=NULL && Element->objekt_n==F->OBJEKT_akt->n)// || Element==F->predchozi_PM))
		{
			Element->mGrid=new TmGrid(F);
			Element->mGrid->Tag=6;//ID formu
			Element->mGrid->ID=ID;//předávání nejvetšího možného ID, aktualizace n elementů se provede, až po mazání, proto přiřadit větší ID aby nedošlo ke kolizi s jiným mGridem
			Element->mGrid->Create(2,1);
			F->design_element(Element,false);//nutné!
		}
	}
}
////---------------------------------------------------------------------------
//smaže všechny elementy v daném objektu
void Cvektory::vymaz_elementy(TObjekt *Objekt)
{
	//deklarace pomocného spojáku pro uchování mazaných elementů
	T2Element *smazat=new T2Element,*pom=new T2Element;
	smazat->n=0;smazat->dalsi=NULL;smazat->predchozi=smazat;

	//načtení elementů v objektu, stačí po hlavní větvi, vyhybky a spojky budou mazat celé větve pozdeji
	TElement *E=Objekt->element;
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		//uložení do pomoxného spojáku
		pom=new T2Element;
		pom->vyhybka=E;
		pom->n=smazat->predchozi->n+1;
		pom->dalsi=NULL;
		pom->predchozi=smazat->predchozi;
		smazat->predchozi->dalsi=pom;
		smazat->predchozi=pom;
		pom=NULL;delete pom;
		//posun na další element
		E=E->dalsi;
	}

	//mazání pouze výhybek, musí být zvlášť !!!!!!!!!!!
	pom=smazat->dalsi;
	while(pom!=NULL)
	{
		if(pom->vyhybka!=NULL && pom->vyhybka->name!="" && (pom->vyhybka->eID==300 || pom->vyhybka->eID==301))
		{
			smaz_vyhybku_spojku(pom->vyhybka,pom->vyhybka->objekt_n);
		}
		pom=pom->dalsi;
	}

	//mazání normálních elementů + spojáku smazat
	do
	{
		pom=smazat->predchozi;//od konce
		if(pom->vyhybka!=NULL && pom->vyhybka->name!="")smaz_element(pom->vyhybka,true,Objekt->n);
		//vyřazení záznamu se seznamu smazat
		smazat->predchozi=pom->predchozi;
		smazat->predchozi->dalsi=NULL;
		//smazání záznamu
		delete pom;pom=NULL;
		//pokud jsem na hlavičce seznamu smazat, smažu hlavičku a cyklus se ukončí
		if(smazat->predchozi->n==0)
		{
			delete smazat;
			smazat=NULL;
		}
	}while(smazat!=NULL);

	//ukazatelové záležitosti
  pom=NULL;delete pom;
	Objekt->element=NULL;
	E=NULL;delete E;

}
////---------------------------------------------------------------------------
//vymaže spojový seznam elementů z paměti
long Cvektory::vymaz_seznam_ELEMENTY()
{
	long pocet_smazanych_objektu=0;
	//načtení všech elementu do pomocného spojáku, kvůli výhybkám je nutné mazat nejen po hlavní větvi
	T2Element *smazat=new T2Element,*pom=NULL;
	smazat->dalsi=NULL;smazat->predchozi=smazat;
	TElement *E=ELEMENTY->dalsi;
  T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	while(E!=NULL)
	{
		pom=new T2Element;
		pom->vyhybka=E;
		pom->dalsi=NULL;
		pom->predchozi=smazat->predchozi;
		smazat->predchozi->dalsi=pom;
		smazat->predchozi=pom;
		pom=NULL;delete pom;
		E=dalsi_krok(VYHYBKY,E);
	}
	vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	delete E;E=NULL;
	//mazání elementů a pomocných spojáků
	while (smazat!=NULL)
	{
		pocet_smazanych_objektu++;
		smazat->predchozi->vyhybka=NULL;
		delete smazat->predchozi->vyhybka;
		delete smazat->predchozi;
		smazat->predchozi=NULL;
		smazat=smazat->dalsi;
	}
	delete smazat;smazat=NULL;
	delete pom;pom=NULL;
	delete ELEMENTY;ELEMENTY=NULL;

	return pocet_smazanych_objektu;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//POHONY
////vytvoří novou hlavičku pro pohonů
void Cvektory::hlavicka_POHONY()
{
	//F->log(__func__);//logování  - NELZE
	TPohon *novy=new TPohon;
	novy->n=0;
	novy->name="";
	novy->rychlost_od=0;
	novy->rychlost_do=0;
	novy->aRD=0;
	novy->roztec=0;
	novy->roztec_ID=0;
	novy->Rz=0;
	novy->Rx=0;
	novy->rezim=-1;
	novy->retez=NULL;
	novy->palec=NULL;

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	POHONY=novy;
}
////---------------------------------------------------------------------------
//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_pohon(TPohon *pohon)
{
	F->log(__func__);//logování
	TPohon *novy=new TPohon;
	novy->retez=NULL;
	novy->palec=NULL;

	novy=pohon;//novy bude ukazovat tam kam prvek Objekt
	novy->n=POHONY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	POHONY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=POHONY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	POHONY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
////---------------------------------------------------------------------------
//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double aRD,double R,double Rz,double Rx,unsigned int roztec_ID)
{
	F->log(__func__);//logování
	TPohon *novy=new TPohon;
	novy->name=name;
	novy->rychlost_od=rychlost_od;
	novy->rychlost_do=rychlost_do;
	novy->aRD=aRD;
	novy->roztec=R;
  novy->roztec_ID=roztec_ID;
	novy->Rz=Rz;
	novy->Rx=Rx;
	novy->rezim=-1;
	novy->retez=NULL;
	novy->palec=NULL;
	vloz_pohon(novy);
}
////---------------------------------------------------------------------------
//vrátí ukazatel na pohon dle n pohonu
Cvektory::TPohon *Cvektory::vrat_pohon(unsigned long n)
{
	F->log(__func__);//logování
	TPohon *p=POHONY->dalsi;//přeskočí hlavičku
	while (p!=NULL)
	{
		if(p->n==n)break;//akce s ukazatelem
		else p=p->dalsi;//posun na další prvek v seznamu
	}
	return p;
}
////---------------------------------------------------------------------------
//bez ukazatelového propojení zkopíruje atributu pohonu do pohonu požadovaného objektu, neobsahuje-li tento objekt alokovanou paměť pro pohon, naalokuje jí
void Cvektory::kopiruj_pohon(TPohon *Pohon,TObjekt *Objekt)
{
	F->log(__func__);//logování
	if(Pohon!=NULL)
	{
		if(Objekt->pohon==NULL)Objekt->pohon=new TPohon;
		if(Objekt==F->pom)
		{
			Objekt->pohon=vrat_pohon(Pohon->n);//přiřazení zvoleného pohonu k objektu
			*vrat_pohon(Pohon->n)=*Pohon;//situace překopírování z pomocného do ostrého (ukládání náhledu), aby bylo zachováno spojové propojení seznamu pohonů i pro případ nepřiřazeno
		}
		else
		{
			*Objekt->pohon=*Pohon;//překopírování hodnot pohonů bez spojového propojení s originálem, ale i přes další a předchozí není zoohledněno propojení se daným spojovým seznamem pohonů, daný pohon je pouze datovou součástí OBJEKT_akt, není samostatným objektem či objektem zařazeným ve spojáku pohonů
		}
	}
	else Objekt->pohon=NULL;
}
////---------------------------------------------------------------------------
//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
bool Cvektory::pohon_je_pouzivan(unsigned long n,bool po_obektech,bool vsechny_elementy)
{
	F->log(__func__);//logování
	TObjekt *O=OBJEKTY->dalsi;
	bool nalezen=false;
	if(po_obektech)
	{
  	while (O!=NULL)
  	{
  		if(O->pohon!=NULL)
  		{
  			if(O->pohon->n==n)
  			{
					nalezen=true;
  				break;//přeruší další vyhledávání
  			}
  		}
  		O=O->dalsi;
		}
	}
	else
	{
		TElement *E=ELEMENTY->dalsi;
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		while(E!=NULL)
		{
			if(F->OBJEKT_akt!=NULL && F->OBJEKT_akt->n!=E->objekt_n || F->OBJEKT_akt==NULL)//kontrolovat pouze mimo aktuálně editované elementy
			{
				if((vsechny_elementy || (!vsechny_elementy && vrat_druh_elementu(E)!=-1)) && E->pohon!=NULL && E->pohon->n==n)
				{nalezen=true;break;}
			}
			E=dalsi_krok(VYHYBKY,E);
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E;
	}
	O=NULL;delete O;
	return nalezen;
}
////---------------------------------------------------------------------------
//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv, ten vrátí formou ukazatale na první nalezený používáný, druhý vstupní parametr metody TObjekt mimo_objekt je ukazatel na objekt, který se bude při vyhledávání ignorovat, nenajde-li vrací NULL, třetí parametr, pokud je náchán na implicitní -1 řeší se pro všechny režimy, pokud je v rozmezí 0 až 2 řeší se pro konkrétní režim
Cvektory::TObjekt *Cvektory::pohon_je_pouzivan(unsigned long n,TObjekt *mimo_objekt,short rezim)
{
	F->log(__func__);//logování
	TObjekt *O=OBJEKTY->dalsi;
	while (O!=NULL)
	{
		if(O->pohon!=NULL && O!=mimo_objekt)//byl objekt s pohonem nalezen a nejedná se o vynechávaný objekt
		{
			if(O->pohon->n==n)//byl objekt s daným pohonem nalezen
			{
				if(rezim==-1)break;//pokud se neřeší konkrétní režim, tak přeruší další vyhledávání, objekt nalezen
				else//řesí se konkrétní režim
				{
					if(O->rezim==rezim)break;//přeruší další vyhledávání, objekt v požadovaném režimu nalezen
				}
			}
		}
		O=O->dalsi;
	}
	return O;
}
////---------------------------------------------------------------------------
//aktualizuje Rx a Rz pohonů, které jsou používané, provádí se pro přepsání pohonů na PL
void Cvektory::aktualizuj_parametry_pouzivanych_pohonu()
{
  //kontrola, zda existují pohony
	if(POHONY!=NULL)
	{
  	TPohon *p=POHONY->dalsi;
  	while(p!=NULL)
		{
  		//kontrola zda je pohon používán
			if(pohon_je_pouzivan(p->n,false,true))
			{
  			//aktualizace Rx a Rz
  			p->Rz=m.Rz(p->aRD);
  			p->Rx=m.Rx(p->aRD,p->roztec);
			}
  		//posun na další pohon
  		p=p->dalsi;
  	}
  	//ukazatelové záležitosti
  	delete p;p=NULL;
	}
}
////---------------------------------------------------------------------------
//aktualizuje režim všem pohonům, použito například při načtení z binárky
void Cvektory::aktualizuj_rezim_pohonu()
{
	TPohon *p=POHONY->dalsi;
	while(p!=NULL)
	{
    aktualizuj_rezim_pohonu(p);//zjistí a nastaví režim pohonu
		p=p->dalsi;
	}
  delete p;p=NULL;
}
////---------------------------------------------------------------------------
//zapíše stav konkrétnímu pohon, nebo zjistí stav pohonu a zapíše ho
void Cvektory::aktualizuj_rezim_pohonu(TPohon *pohon,short rezim)
{
	//kontrola, zda existuje pohon
	if(pohon!=NULL)
	{
		//uložení původního režimu pohonu
		short puv_rezim=pohon->rezim;

		//pokud byl zaslán režim uloží ho
		if(rezim>-1)
		{
			//pokud není pohon v řežimu KK zapsat poslaný režim
			if(rezim==1 || (rezim!=1 && puv_rezim!=1))pohon->rezim=rezim;
		}
		//pokud nebyl zaslán zjistí ho
		else
		{
	  	//průchod skrze všechny elementy, hledání počtu S&G a KK elementů na pohonu
	  	unsigned int pSG=0,pKK=0;
	  	bool pokracovat=true;
	  	TElement *E=ELEMENTY->dalsi;
	  	T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
	  	while(E!=NULL)
	  	{
	  		//pokud je elemenent na stejném pohonu, kontrola
	  		if(E->pohon==pohon && E->eID!=5 && E->eID!=6 && E->eID!=100 && E->eID!=200 && E->eID!=300 && E->eID!=301)//přeskakovat otoče, ion. tyče, PM, výhybky a spojky, neudávají režim
	  		{
	  			//kontrola typu elementu
	  			switch(vrat_druh_elementu(E))
	  			{
	  				case 0:pSG++;break;//na pohonu se nachází S&G element
	  				case 1:{pKK++;pokracovat=false;}break;//na pohonu se nachází KK element, pokud byl nalezen jeden KK element, ukončení průchodu
	  				default:break;
	  			}
	  		}
	  		//přesun na další element
	  		if(pokracovat)E=dalsi_krok(VYHYBKY,E);
	  		//pokud jsem narazil na S&G element dojde k ukončení průchodu
	  		else break;
	  	}
			vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
	  	E=NULL;delete E;

	  	//nastavení režimu
	  	pohon->rezim=-1;
	  	if(pKK>0)pohon->rezim=1;
	  	else if(pSG>0)pohon->rezim=0;
	  	//aktualizace parametrů
	  	if(pohon->rezim!=0)
	  	{
	  		pohon->Rz=m.Rz(pohon->aRD);
	  		pohon->Rx=m.Rx(pohon->aRD,pohon->roztec);
	  	}
	  	else
	  	{
	  		pohon->Rx=0;
	  		pohon->Rz=0;
			}
		}

		//pokud došlo ke změně režimu a je otevřena editace, aktualizuje zobrazení v tabulkách
		if(F->OBJEKT_akt!=NULL && puv_rezim!=pohon->rezim)FormX->zmena_rezimu_pohonu(pohon);
	}
}
////---------------------------------------------------------------------------
//dle n pohonu vráti objekty, které pohon používají, pokud je short_name na true, vrátí kratký název objektu jinak dlouhý
AnsiString Cvektory::vypis_objekty_vyuzivajici_pohon(unsigned long n,bool short_name)
{
	TObjekt *O=OBJEKTY->dalsi;
	AnsiString nalezen="";
	while (O!=NULL)
	{
		if(O==F->pom && F->pom!=NULL)//pokud je voláno z editovaného PO
		{   //R  21.1.2020 - ZAKOMENTOVANA CAST - VAZBA NA STARE PO
			 //a pokud se jedná o stejný objekt, jako právě projížděný cyklemmusí být samostatně
//				if(Form_parametry->scComboBox_pohon->ItemIndex>0  && Form_parametry->scComboBox_pohon->ItemIndex==(long)n)//pokud má pohon přiřazen a jedná se o stejný pohon
//				{
//					if(short_name)nalezen+=O->short_name+", ";
//					else nalezen+=O->name+", ";
//				}
		}
		else//pro ostatní objekty mimo editaci na PO
		{
			if(O->pohon!=NULL && O->pohon->n==n)//pokud má pohon přiřazen a jedná se o stejný pohon
			{
				if(short_name)nalezen+=O->short_name+", ";
				else nalezen+=O->name+", ";
			}
		}
		O=O->dalsi;
	}
	O=NULL;delete O;
	if(nalezen!="")nalezen=nalezen.SubString(1,nalezen.Length()-2);//ještě odebere poslední čárku a mezeru
	return nalezen;
}
////---------------------------------------------------------------------------
//vratí počet objektů přiřazených k danému pohonu, parametr režim, ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
unsigned long Cvektory::vrat_pocet_objektu_vyuzivajici_pohon(unsigned long n, short rezim)//vratí počet objektů přiřazených k danému pohonu, parametr režim, ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
{
	TObjekt *O=OBJEKTY->dalsi;
	unsigned long RET=0;
	while (O!=NULL)
	{
		if((short)O->rezim==rezim && O->pohon!=NULL && O->pohon->n==n)RET++;//pokud má pohon přiřazen a jedná se o stejný pohon a je v libovolném režimu
		if(rezim==-1 && O->pohon!=NULL && O->pohon->n==n)RET++;//pokud má pohon přiřazen a jedná se o stejný pohon a je v daném režimu
		O=O->dalsi;
	}
	O=NULL;delete O;
	return RET;
}
////---------------------------------------------------------------------------
//vratí formou ukazatele na pole objekty přiřazené k danému pohonu, parametr režim, ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
Cvektory::TObjekt *Cvektory::vrat_objekty_vyuzivajici_pohon(unsigned long n, short rezim)
{
	TObjekt *O=OBJEKTY->dalsi;
	TObjekt *RET=new TObjekt[vrat_pocet_objektu_vyuzivajici_pohon(n)];
	unsigned long i=0;//zde oproti další metodě alokujeme od 0
	while (O!=NULL)
	{
		if(rezim==-1 && O->pohon!=NULL && O->pohon->n==n)RET[i++]=*O;//pokud má pohon přiřazen a jedná se o stejný pohon a je v libovolném režimu
		if((short)O->rezim==rezim && O->pohon!=NULL && O->pohon->n==n)RET[i++]=*O;//pokud má pohon přiřazen a jedná se o stejný pohon a je v daném režimu
		O=O->dalsi;
	}
	O=NULL;delete O;
	return RET;
}
////---------------------------------------------------------------------------
//vratí formou ukazatele na pole objekty bez pohonů
Cvektory::TObjekt *Cvektory::vrat_objekty_bez_pohonu()
{
	TObjekt *O=OBJEKTY->dalsi;
	TObjekt *RET=new TObjekt[vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false)+1];//nutno alokovat o jeden prvek více
	unsigned long i=1;
	while (O!=NULL)
	{
		if(O->pohon==NULL)RET[i++]=*O;//pokud má objekt pohon nepřiřazen zařídí tento objekt do pole
		O=O->dalsi;
	}
	O=NULL;delete O;
	return RET;
}
////---------------------------------------------------------------------------
//vrátí počet nepoužívaných pohonů
unsigned long Cvektory::vrat_pocet_nepouzivanych_pohonu()
{
	unsigned long RET=0;
	TPohon *p=POHONY->dalsi;//přeskočí hlavičku
	while (p!=NULL)//projde všechny pohony
	{
		if(pohon_je_pouzivan(p->n)==false)RET++;//pokud je nepoužíván, navýší počítadlo počtu nepoužívaných pohonů
		p=p->dalsi;
	}
	p=NULL;delete p;
	return RET;
}
////---------------------------------------------------------------------------
//vrátí nejnižší možnou rychlost ze všech objektů, které jsou přiřazené k danému pohonu (využívá se pro S&G a PP, u KK musí být RD v souladu s TT)
//dalo by se ještě pro určtité účely i zefektivnit, že pokud je pohon přiřazen k nějakém KK objektu, není třeba dále hledat, protože je již zajištěno minRD...
//pokud vrátí 0, znamená, že pohon není využíván
double Cvektory::minRD(TPohon *pohon)
{
//	TObjekt *O=OBJEKTY->dalsi;
	double min=123567.0;//jen náhodně velké číslo
//	while (O!=NULL)
//	{                                    //mohl bych ještě odfiltrovávat, zda se nejedná o KK, ale je to víceméně zbytečné
//		if(O->pohon!=NULL && O->pohon==pohon)//pokud má pohon přiřazen a jedná se o stejný pohon
//		{
//			if(O->delka_dopravniku/O->CT<min)min=O->delka_dopravniku/O->CT;//tak z těchto objektů najde nejmenší možné RD
//		}
//		O=O->dalsi;
//	}
//	O=NULL;delete O;
	if(min==123567.0)min=0;//pokud vrátí 0, znamená, že pohon není využíván
	return min;
}
////---------------------------------------------------------------------------
//vypíše objekty přiřazené k danému pohonu, kterým neodpovídá rachlost přejezdu při navrhovaném testRD      //0 - S&G, 1-KK, 2-PP, 20-S&G+PP
AnsiString Cvektory::vypis_objekty_nestihajici_prejezd(TPohon *pohon,double testRD,short rezim)
{
	TObjekt *O=OBJEKTY->dalsi;
	AnsiString objekty="";
	while (O!=NULL)
	{
		if(O->pohon!=NULL && O->pohon==pohon)//pokud má pohon přiřazen a jedná se o stejný pohon
		{                         //pro S&G a PP                         //pro konkrétní režim
			if(rezim==-1 || (rezim==20 && (O->rezim==0 || O->rezim==2)) || O->rezim==rezim)//filtr režimu
			{
				double rRD=0;//4.12.2018 zdm, m.kontrola_rychlosti_prejezdu(O->CT,O->MT1+O->MT2,O->PT,O->WT1+O->WT2,O->delka_dopravniku,testRD);
				if(rRD!=0)
				{
					objekty=O->short_name+", ";
				}
			}
		}
		O=O->dalsi;
	}
	O=NULL;delete O;
	if(objekty!="")objekty=objekty.SubString(1,objekty.Length()-2);//ještě odebere poslední čárku a mezeru
	return objekty;
}
////---------------------------------------------------------------------------
//zvážit integraci metody do výše uvedené!!!!
//zkontroluje objekt zda daná rychlost pohonu odpovídá požadované rychlosti pohonu, pokud ne vrátí popis včetně hodnoty, lze poslat externí testovací parametry nebo nechat ověřit dle uložených ve spojáku objekty
//řeší buď dotazy přímo nad ostrými daty nebo zadáním externích parametrů napr. pro účely prediktivního testování
AnsiString Cvektory::kontrola_rychlosti_prejezdu(TObjekt *O,short rezim,double CT,double MT,double WT,double aRD,double DD,short aRDunit,unsigned short precision,AnsiString mark,bool add_decimal,AnsiString separator_aRD)
{
	//jednotky a definice výstupního textu
	AnsiString aRDunitT="m/s";if(aRDunit)aRDunitT="m/min";
	AnsiString error_text="";

	//pokud jsou dodané parametry objektu dodané v nulových hodnotách,tzn. použíjí se hodnoty přímo daného objektu ze spojového seznamu, nikoliv externě dodané parametry funkce (ty slouží např. v případě testování na GAPO, kde se počítají parametry potenciálně nové)
	if(CT==0 || aRD==0 || DD==0)
	{
//		CT=O->CT;
//		DD=O->delka_dopravniku;
		if(O->pohon!=NULL)
		{
			aRD=O->pohon->aRD;
			MT=DD/aRD;
		}
		else
		{
			aRD=0;
			MT=0;
		}
		//4.12.2018 zdm, if(WT==0)WT=O->WT1+O->WT2;//pokud není dodáno WT, tak zkusí vzít z uložení
	}

	//vrátí rozdíl aktuální rychlosti pohonu a potřebné k uskuteční přejezdu, pokud je hodnota 0 je v pořádku, je-li záporná, přejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
	double rRD=0;//4.12.2018 zdm m.kontrola_rychlosti_prejezdu(CT,MT,O->PT,WT,DD,aRD);//pokud není MT dodáno bude spočítáno, pokud nebude vyčísleno PT a WT, bude MT totožné s CT, bude tedy splněna alespoň minumální nutná (nikoliv dostatčující) podmínka, kdy DD/CT>=aRD
	if(rezim==0 && m.null(rRD)!=0 || rezim==2 && rRD<0)//problém nastane pokud rRD tzn. rozdíl od aRD pro S&G, pro PP se pouze řeší pokud je rRD menší než aRD
	{
		error_text=O->short_name;
		if(aRDunit>=0)//pokud jsou dodány jednotky aRD, tzn. bude požadován výpis o rozídílu o kolik se nestíhá
		error_text+=separator_aRD+F->m.round2double((rRD)*(1+59.0*aRDunit),precision,mark,add_decimal)+" ["+aRDunitT+"]";
	}

	//návratová hodnota
	return error_text;
}
////---------------------------------------------------------------------------
//ověří zda je stejná rychlost pohonu na lince používána, pokud není vratí NULL, jinak ukazatel na daný pohon
Cvektory::TPohon *Cvektory::najdi_pohon_dle_RD(double RD)
{
	TPohon *p=POHONY->dalsi;//přeskočí hlavičku
	while (p!=NULL)
	{
		if(p->aRD==RD)break;//akce s ukazatelem popř. pokud by se nepoužívalo aktivně aRD (nyní v Form1 v NP())by se musela hledat RD v objektech a z nalezeného objektu by se vrátil přiřazený pohon, pokud by obsahoval, pokud ne, hledalo by se dál
		else p=p->dalsi;//posun na další prvek v seznamu
	}
	return p;
}
////---------------------------------------------------------------------------
//danému pohonu vytvoří řetěz dle geometrie všech elementů, co spadají pod daný pohon
void Cvektory::vytvor_retez(TPohon *Pohon)
{
	//////odstranění původních dat
	smazat_retez(Pohon);
	smazat_palce(Pohon);

	//////vytvoření nových dat
	if(ELEMENTY!=NULL)
	{
		double delka=0;//celková délka řetězu zatím uvažovaná bez vychylení na předávacích místech
		//if(F->Poffset*0.0083333333*3>=Pohon->roztec)F->Poffset=0;//pro testy úvodní simulace
		//double umisteni=F->Poffset*0.0083333333*3/*pro testy úvodní simulace jinak 0*/;
		double umisteni=0;
		TElement *E=ELEMENTY->dalsi;//přeskočí hlavičku
		T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		while(E!=NULL)//procházení jednotlivých elementů pro tvorbů řetězu
		{
			if(E->pohon!=NULL && Pohon!=NULL && E->pohon->n==Pohon->n && E->geo.delka>0)//řetěz tvořen pouze z geometrie všech elementů, co spadají pod daný pohon
			{
				////DELKA - celková délka řetězu zatím uvažovaná bez vychylení na předávacích místech, proto vystrčená provizorně zde
				delka+=E->geo.delka;

				////PALCE - nutné do samostané metody kvůli vykreslování
				while(m.round2double(umisteni,3)<=E->geo.delka)//m.round2double(umisteni,3) je nasazeno kvůli nepřesnosti v číselném řádu, procházení jednoho segmentu a umísťování palce na tento segment
				{
					TPalec *P=new TPalec;
					vloz_palec(P,Pohon);
					//výpočet souřadnic a rotace jigu dle aktuálního umístění
					TPointD_3D Pt=m.getPt(E->geo.radius,E->geo.orientace,E->geo.rotacni_uhel,E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4,umisteni/E->geo.delka,umisteni/E->geo.delka);
					P->X=Pt.x;P->Y=Pt.y;P->orientace=Pt.z;
					//navýšení umístění dle rozteče
					umisteni+=Pohon->roztec;
					P=NULL;delete P;
				}

				////ŘETĚZ, resp. zapouzdření řetězu
				TRetez *R=new TRetez;//segment řetězu
				vloz_segment_retezu(R,Pohon);
				////všechy elementy včetně předávacího místa, to se ještě následně případně ovlivňuje níže (dotváří)
				R->eID=E->eID;
				R->geo=E->geo;
				//dokonalé předávací místo, zatím odstaveno, stačí odkomentovat, ale má návaznost na palce řetězu ty by ještě bylo nutné měnit v místě předávacího místa (resp. jejich souřadnice)
//				////Předávací místo, výpočet souřadnic předávacího místa - ubrání z existujícho segmentu, přepsání výše uvedených souřadnic
//				//vstupní parametry dimenzující geometrii předávacího místa
//				float  D=1;//delká vyosování - 1 metr na každou stranu
//				float RetezWidth=1;//šířka řetezu kvůli vyosení v předávacím místě
//				TPointD V=m.rotace(RetezWidth*F->m2px,180-E->geo.orientace+90,0);//vyosení, mezera mezi pohony v předávacím místě
//				TPointD Z=m.rotace(D,180-E->geo.orientace,0);//začátek vyosování
//				////situace před předávacím místem
//				if(E->eID==200)
//				{
//					//segment řetězu před předávacím místem konkrétně před vyosení
//					R->eID=E->eID-1;//eID 199 poslední segment před vysováním tj. před předávacím místem
//					R->geo.X1=E->geo.X1; 							 			 R->geo.Y1=E->geo.Y1;
//					R->geo.X2=(E->geo.X4-Z.x+E->geo.X1)/2.0; R->geo.Y2=(E->geo.Y4-Z.y+E->geo.Y1)/2.0;
//					R->geo.X3=R->geo.X2; 										 R->geo.Y3=R->geo.Y2;
//					R->geo.X4=E->geo.X4-Z.x; 								 R->geo.Y4=E->geo.Y4-Z.y;
//
//					//segment řetězu předávací místo konkrétně od vyosení
//					TRetez *R1=new TRetez;
//					vloz_segment_retezu(R1,Pohon);
//					R1->eID=E->eID;//eID 200
//					R1->geo.X1=R->geo.X4; 						R1->geo.Y1=R->geo.Y4;
//					R1->geo.X2=E->geo.X4-Z.x/2.0;			R1->geo.Y2=E->geo.Y4-Z.y/2.0;
//					R1->geo.X3=E->geo.X4-Z.x/2.0+V.x;	R1->geo.Y3=E->geo.Y4-Z.y/2.0+V.y;
//					R1->geo.X4=E->geo.X4+V.x;					R1->geo.Y4=E->geo.Y4+V.y;
//				}
//				///situace za předávacím místem
//				TElement *Ep=E->predchozi;
//				if(Ep!=NULL && Ep->eID==200)
//				{
//					//segment řetězu za předávacím místem konkrétně v největším místě vyosení za předávacím místem
//					R->eID=E->eID+1;//eID 201 segment řetězu za předávacím místem konkrétně v největším místě vyosení za předávacím místem
//					R->geo.X1=E->geo.X1+Z.x; 						 R->geo.Y1=E->geo.Y1+Z.y;
//					R->geo.X2=(R->geo.X1+E->geo.X4)/2.0; R->geo.Y2=(R->geo.Y1+E->geo.Y4)/2.0;
//					R->geo.X3=R->geo.X2; 								 R->geo.Y3=R->geo.Y2;
//					R->geo.X4=E->geo.X4; 								 R->geo.Y4=E->geo.Y4;
//
//					//segment řetězu předávací místo konkrétně ke konci vyosení
//					TRetez *R1=new TRetez;
//					vloz_segment_retezu(R1,Pohon);
//					R1->eID=E->eID+2;//eID 202
//					R1->geo.X1=E->geo.X1-V.x;					R1->geo.Y1=E->geo.Y1-V.y;
//					R1->geo.X2=E->geo.X1+Z.x/2.0-V.x;	R1->geo.Y2=E->geo.Y1+Z.y/2.0-V.y;
//					R1->geo.X3=E->geo.X1+Z.x/2.0;			R1->geo.Y3=E->geo.Y1+Z.y/2.0;
//					R1->geo.X4=R->geo.X1; 						R1->geo.Y4=R->geo.Y1;
//				}

				////kalkulace s umístěním palce pro další segment
				umisteni-=E->geo.delka;//zbytek z předchzejícího geometrického úseku, který nestihl být zohledněn převeden na další geometrický úsek, resp. element = výchozí umístění v dalším elementu
			}
			E=dalsi_krok(VYHYBKY,E);//posun na další element nahrazuje při použití výhybek dřívější E=E->dalsi;
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		delete E;E=NULL;

		////doplnění případného posledního (momentálně virtuálního) palce, který není vidět (je mimo řetěz), pozor řeší jenom předpoklad, že rozteč palců je menší než délka řetězu
		if(delka>0 && ceil(delka/Pohon->roztec)+1>Pohon->palec->predchozi->n)//pokud je potřebná délka (pro celočíselnou dělitelnost roztečí) řetězu větší než aktuální, přídá palec (momentálně virutální)
		{
			//F->Memo("____________"+Pohon->name);
			//F->Memo(Pohon->palec->predchozi->n);
			TPalec *P=new TPalec;
			vloz_palec(P,Pohon);
			P->X=DOUBLE_MIN;P->Y=DOUBLE_MIN;P->orientace=DOUBLE_MIN;//zneplatní souřadnice, což zajistí, že tento palec nebude zatím vidět (jedná se o "poznávací znamení")
			P=NULL;delete P;
			//F->Memo(Pohon->palec->predchozi->n);
		}
	}
}
////---------------------------------------------------------------------------
//danému řetězu vloží jeden geometrický segment
void Cvektory::vloz_segment_retezu(TRetez *Retez,TPohon *Pohon)
{
	if(Pohon->retez==NULL)//pokud nebyla vytvořena hlavička, tak vytvoří
	{
		Pohon->retez=new TRetez;
		Pohon->retez->n=0;
		Pohon->retez->eID=0;
		Pohon->retez->predchozi=Pohon->retez;
		Pohon->retez->dalsi=NULL;
	}
	Retez->n=Pohon->retez->predchozi->n+1;//navýšení počítadla
	Retez->predchozi=Pohon->retez->predchozi;//nový prvek ukazuje na poslední prvek ve spojaku jako na prvek předchozí
	Retez->dalsi=NULL; //nový prvek neukazuje na žádný další prvek, resp. ukazuje na NULL
	Pohon->retez->predchozi->dalsi=Retez;//za poslední aktuální prvek vloží nový poslední
	Pohon->retez->predchozi=Retez;//hlavička nově ukazuje již na nový bod jako poslední prvek
}
////---------------------------------------------------------------------------
//od daného pohonu resp. řetězu posune palce o daný časový úsek
void Cvektory::posun_palce_retezu()
{
	TPohon *Pohon=POHONY->dalsi;//přeskočí hlavičku
	while(Pohon!=NULL)
	{
		if(Pohon->retez!=NULL && Pohon->palec!=NULL)
		{
			////vstupní proměnné či ukazatele
			TRetez *R=Pohon->retez->dalsi;//přeskočí hlavičku ŘETĚZ
			TPalec *P=Pohon->palec->dalsi;//přeskočí hlavičku PALCE
			//Pohon->umisteni+=fmod(F->sTIME*Pohon->aRD,Pohon->roztec);//aktuální čas simulace * rychlost pohonu=umisteni, umístení prvního palce (nejedná se vždy o n==1, ale vizuálně prvně vykreslovaného a ve spojáku palců umístěného aktuálně na první pozici)
      Pohon->umisteni+=m.px2m(1);
			//if(Pohon->n==1)F->Memo(String(Pohon->umisteni)+" "+String(Pohon->roztec));
			////vynulování umístění, objevil se nový palec
			if(Pohon->umisteni>=Pohon->roztec)
			{
				///objevil se nový palec, tzn. posune se zpět o celý úsek rozteče (tj. jak kdyby neustále rozteč byla odečítáná při přetečení)
				Pohon->umisteni-=Pohon->roztec;
				///přeskládání spojáku pálců a to tak, že poslední palec bude první (hned za hlavičkou), původně první bude druhý, původně předposlední bude poslední atd
				TPalec *P1=Pohon->palec->predchozi;//uložení původně posledního, následně prvního
				//hlavička
				Pohon->palec->dalsi=P1;//na první místo za hlavičku
				Pohon->palec->predchozi=P1->predchozi;//hlavička ukazuje na poslední prvek na původní předposlední
				//nový první, původně poslední
				P1->predchozi->dalsi=NULL;//původní předposlední bude uzavírat spojový seznam
				P1->predchozi=Pohon->palec;//je první, tedy předchůdce je hlavička
				P1->dalsi=P;//nový první ukazuje na další na původní první nyní druhý
				//nyní druhý, původní první
				P->predchozi=P1;//ukazuje na předchůdce místo na hlavičku na nyní první (původně poslední)
				//pro další použítí nahraje do globálního ukazatele prního palce, nový ukazatel na první palec
				P=P1;
				P1=NULL;delete P1;
			}

			////vychozí umístení prvního palce (nejedná se vždy o n==1, ale vizuálně prvně vykreslovaného a ve spojáku palců umístěného aktuálně na první pozici)
			double umisteni=Pohon->umisteni;//následně vždy umístění aktuálně zpracovávaného palce

			////procházení jednotlivých elementů pro tvorbů řetězu
			while(R!=NULL)
			{
				while(m.round2double(umisteni,3)<=R->geo.delka)//m.round2double(umisteni,3) je nasazeno kvůli nepřesnosti v číselném řádu
				{
					//výpočet souřadnic a rotace jigu dle aktuálního umístění
					TPointD_3D Pt=m.getPt(R->geo.radius,R->geo.orientace,R->geo.rotacni_uhel,R->geo.X1,R->geo.Y1,R->geo.X4,R->geo.Y4,umisteni/R->geo.delka,umisteni/R->geo.delka);
					P->X=Pt.x;P->Y=Pt.y;P->orientace=Pt.z;
					P=P->dalsi;
					//navýšení umístění dle rozteče
					umisteni+=Pohon->roztec;
				}
				//kalkulace s umístěním palce pro další segment
				umisteni-=R->geo.delka;//zbytek z předchzejícího geometrického úseku, který nestihl být zohledněn převeden na další geometrický úsek, resp. element = výchozí umístění v dalším elementu
				R=R->dalsi;
			}
			////pokud existují ještě nějaké palce, které nejsou vidět (jsou mimo viditelnou část řetězu), tak jim zneplatní souřadnice,
			while(P!=NULL)
			{
				P->X=DOUBLE_MIN;P->Y=DOUBLE_MIN;P->orientace=DOUBLE_MIN;//zneplatní souřadnice, což zajistí, že tento palec nebude zatím vidět (jedná se o "poznávací znamení")
				P=P->dalsi;
			}
			////odstranění již nepotřebných ukazatelů
			R=NULL;delete R;
			P=NULL;delete P;
		}
		Pohon=Pohon->dalsi;
	}
	Pohon=NULL;delete Pohon;
}
////---------------------------------------------------------------------------
//danému pohonu vloží jeden palec
void Cvektory::vloz_palec(TPalec *Palec,TPohon *Pohon)
{
	if(Pohon->palec==NULL)//pokud nebyla vytvořena hlavička, tak vytvoří
	{
		Pohon->palec=new TPalec;
		Pohon->palec->n=0;
		Pohon->palec->X=0;
		Pohon->palec->Y=0;
		Pohon->palec->orientace=0;
		Pohon->palec->stav=-1;
		Pohon->palec->predchozi=Pohon->palec;
		Pohon->palec->dalsi=NULL;
	}
	Palec->n=Pohon->palec->predchozi->n+1;//navýšení počítadla
	Palec->predchozi=Pohon->palec->predchozi;//nový prvek ukazuje na poslední prvek ve spojaku jako na prvek předchozí
	Palec->dalsi=NULL; //nový prvek neukazuje na žádný další prvek, resp. ukazuje na NULL
	Pohon->palec->predchozi->dalsi=Palec;//za poslední aktuální prvek vloží nový poslední
	Pohon->palec->predchozi=Palec;//hlavička nově ukazuje již na nový bod jako poslední prvek
}
////---------------------------------------------------------------------------
//všem objektům s n pohonem zruší přiřazení k tomuto pohonu a nahradí hodnotu ukazatele na přiřazený pohon za NULL, nově i všem elementům
void Cvektory::zrusit_prirazeni_pohunu_k_objektum_elementum(unsigned long n)
{
	//průchod všemi objekty, testuje je daný pohon objektu přiřazen a pokud ano, tak mu nastaví přiřazený pohon na NULL
	TObjekt *O=OBJEKTY->dalsi;
	while(O!=NULL)
	{
    //kontrola objektu
		if(O->pohon!=NULL && O->pohon->n==n)//pokud má pohon přiřazen a jedná se o stejný pohon
		{
			O->pohon=NULL;//pohon již nepřiřazen
		}
		O=O->dalsi;
	}
	delete O;O=NULL;
	//procházení objektů
	TElement *E=ELEMENTY->dalsi;
	while(E!=NULL)
	{
  	if(E->pohon!=NULL && E->pohon->n==n)//pokud má element pohon přiřazen a jedná se o stejný pohon
		{
			E->pohon=NULL;//pohon již není přiřazen
		}
		E=E->dalsi;
	}
	delete E;E=NULL;
}
////---------------------------------------------------------------------------
//smaze z pameti
long Cvektory::vymaz_seznam_POHONY()
{
	long pocet_smazanych_pohonu=0;
	while (POHONY!=NULL)
	{
		pocet_smazanych_pohonu++;
		smazat_retez(POHONY->predchozi);
		smazat_palce(POHONY->predchozi);
		POHONY->predchozi=NULL;
		delete POHONY->predchozi;
		POHONY=POHONY->dalsi;
	};
	return pocet_smazanych_pohonu;
}
////---------------------------------------------------------------------------
//danému pohonu smaže jeho řetěz
void Cvektory::smazat_retez(TPohon *pohon)
{
	if(pohon!=NULL)
	while (pohon->retez!=NULL)
	{
		pohon->retez->predchozi=NULL;
		delete pohon->retez->predchozi;
		pohon->retez=pohon->retez->dalsi;
	};
}
////---------------------------------------------------------------------------
//danému pohonu smaže jeho palce řetězu
void Cvektory::smazat_palce(TPohon *pohon)
{
	if(pohon!=NULL)
	while (pohon->palec!=NULL)
	{
		pohon->palec->predchozi=NULL;
		delete pohon->palec->predchozi;
		pohon->palec=pohon->palec->dalsi;
	};
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvektory::vloz_do_BUFFERU()
{
	//pokud ještě nebyla založena hlavička, založí ji
	if(sBUFFER==NULL)
	{
		//alokace
		T_buffer_simulace *novy=new T_buffer_simulace;

		//atributy
		novy->n=0;
		novy->raster=NULL;

		//ukazatelové záležitosti
		novy->predchozi=novy;//ukazuje sam na sebe
		novy->dalsi=NULL;
		sBUFFER=novy;
		novy=NULL;delete novy;//toto opravdu možno?
	}


	//již standardní vložení
	T_buffer_simulace *novy;
	try
	{
		//alokace
		novy=new T_buffer_simulace;
		//atributy
		novy->n=sBUFFER->predchozi->n+1;//navýším počítadlo prvku o jedničku
		//novy->raster=new Graphics::TBitmap();
		//novy->raster=new TPngImage;
		//novy->raster->Assign(F->Staticka_scena);
		//novy->raster->Canvas->Font->Size=50;
		//novy->raster->Canvas->TextOutW(100,100,novy->n);
		novy->raster;

		//ukazatelové záležitosti
		sBUFFER->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
		novy->predchozi=sBUFFER->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		novy->dalsi=NULL;//poslední prvek se na žádny dalsí prvek neodkazuje (neexistuje)
		sBUFFER->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
		if(novy->n==1)sIterator=novy;//ukazovátko aktuálně načítaného rastru,výchozí nastavení pro první prvek
		novy=NULL;delete novy;//toto opravdu možno?
	}catch(...)
	{
		//novy->raster->FreeImage();//asi netřeba
		delete novy->raster;
		delete novy;
		F->Memo("Buffer naplněn na maximum");
		//zde zvážit utnutí timeru, buď totální nebo částečné
	}
}
////---------------------------------------------------------------------------
Cvektory::TmyPx *Cvektory::komprese(Graphics::TBitmap *bmp_in,TRect oblast,bool resize)
{
	//vstupní bitmapy
	bmp_in->PixelFormat=pf24bit;//nutné v případě, že by nebylo nastavovan
	if(oblast==TRect(0,0,0,0))//pokud nebyla dodána oblast, použije se ta nativní z bmp
	{
		oblast.Left=0;
		oblast.Top=0;
		oblast.Right=bmp_in->Width-1;
		oblast.Bottom=bmp_in->Height-1;
	}

	TmyPx *RET=NULL;//ukazatel na první bod v mračnu bodů/spojáku
	TmyPx *Iterator=NULL;//ukaztel na poslední bod v mračnu bodů/spojáku
	TmyPx *Rt=NULL;//tempová hodnota pro slučování buněk

	//průchod přes jednotlivé řádky
	for(unsigned short Y=oblast.Top;Y<=oblast.Bottom;Y++)
	{
		//načtení řádků
		PRGBTriple R=(PRGBTriple)(bmp_in->ScanLine[Y]);// vezmu ukazatel na řádek y z výsledné bitmapy

		//práce se jednolivými pixely (jednoprvkovými sloupci) řádků
		for(unsigned short X=oblast.Left;X<=oblast.Right;X++)
		{
			if(R[X].rgbtRed!=255 || R[X].rgbtGreen!=255 || R[X].rgbtBlue!=255)//když NEBUDE BÍLÁ, pokud ano, nemá cenu řešit
			{
				if(Rt==NULL || R[X].rgbtRed!=Rt->R || R[X].rgbtGreen!=Rt->G || R[X].rgbtBlue!=Rt->B)//KOMPRESE pokud nebude aktuální buňka hodnotami stejná jako předchozí(resp. množina buněk) buňka na daném řádku, nebo se jedná o první výskyt aktivní (nebílé) buňky na řádku Rt==NULL
				{
					//ukazatelové záležitosti
					if(RET==NULL)//jedná se o první zápis, à la zápis do hlavičky, ale zde se začíná rovnou zápisem do výchozího bodu
					{
						RET=new TmyPx;
						RET->dalsi=NULL;
						Iterator=RET;
					}
					else//zápis dalších prvků
					{
						Iterator->dalsi=new TmyPx;//alokace nového bodu
						Iterator=Iterator->dalsi;//posun na něj
					}
					Iterator->dalsi=NULL;
					//hodnoty
					Iterator->X=X;Iterator->Y=Y;
					if(resize){Iterator->X-=oblast.Left;Iterator->Y-=oblast.Top;}//pokud je požadováno zmenšení výstupního rastru
					Iterator->o=0;
					Iterator->R=R[X].rgbtRed;
					Iterator->G=R[X].rgbtGreen;
					Iterator->B=R[X].rgbtBlue;
					//tempový vzor pro slučování ukazuje na iterator
					Rt=Iterator;
				}
				else//buňka je totožná jako předchozí(resp. množina buněk), lze sloučit a tudíž nutno udělat zápis do první ze slučovaných tj. do poslední přidané do mračna bodů/spojového seznamu, tempová se nemění
				{
					Rt->o++;//zápis do posledního přidaného bodu z mračna bodů/spojového seznamu informaci o sloučených buňkách, že následují další totožné
				}
			}
			else//byla BÍLA, temp je tedy nutné "vymazat"
			Rt=NULL;
		}
	}
	Iterator=NULL;delete Iterator;//prvně musí být NULL
	delete Rt;
	return RET;//vrátí ukazatel na první bod v mračnu bodů/spojáku
}
////---------------------------------------------------------------------------
Graphics::TBitmap *Cvektory::dekomprese(TmyPx *Raster,unsigned short Width,unsigned short Height)
{
	Graphics::TBitmap *bmp_out=new Graphics::TBitmap;  //je sice rychlejší mít bmp_out připravenou, ale nelze do ni připisovat, stejně se musí vymazat či vytvořit nová
	bmp_out->Width=Width;
	bmp_out->Height=Height;
	bmp_out->PixelFormat=pf24bit;
	TmyPx *Iterator=Raster;
	PRGBTriple R;
	unsigned short Y=0;//aktuálně zpracovávaný řádek
	while(Iterator!=NULL)
	{
		//jedná-li se o přechod na jiný řádek nebo výchozí řádek, tak získá přístup k tomuto řádku, jinak ne - tím šetří zbytečné průchody přes řádky
		if(Y!=Iterator->Y || Iterator==Raster)
		{
			Y=Iterator->Y;//index nově zpracovávaného řádku
			R=(PRGBTriple)bmp_out->ScanLine[Y];
		}

		//zápis získaných hodnot a DEKOMPRESE (znovuzápis) opakujících se hodnot
		unsigned short pocet_slouceni=0;
		do
		{
			R[Iterator->X+pocet_slouceni].rgbtRed=Iterator->R;
			R[Iterator->X+pocet_slouceni].rgbtGreen=Iterator->G;
			R[Iterator->X+pocet_slouceni].rgbtBlue=Iterator->B;
		}while(++pocet_slouceni<=Iterator->o);

		//posun na další prvek a smazání z paměti stávajícího
		//TmyPx *Iterator_old=Iterator; --pro rychlostní testy mazání odstaveno (kvůli opakování dat)
		Iterator=Iterator->dalsi;
		//delete Iterator_old;Iterator_old=NULL; --pro rychlostní testy mazání odstaveno (kvůli opakování dat)
	}
	delete Iterator;
	return bmp_out;
}
//---------------------------------------------------------------------------
//Uloží rastrová data do souboru
void Cvektory::rast_do_souboru(TmyPx *Raster,String FileName)
{
	//vytvoření streamu pro zápis do souboru
	TFileStream *FileStream=new TFileStream(FileName,fmOpenWrite|fmCreate);

	TmyPx *Iterator=Raster;
	while(Iterator!=NULL)
	{
		FileStream->Write(&Iterator,sizeof(TmyPx));//zapiše do souboru jeden prvek
		Iterator=Iterator->dalsi;//posun na další prvek
	}
	delete Iterator;
	delete FileStream;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void Cvektory::vloz_SIM(double T1,double T2,short A,TElement *S,TVozik *V)
{
	////pokud ještě nebyla založena hlavička, založí ji
	if(SIM==NULL)
	{
		//alokace - musí být samostatná
		TSIM *novy=new TSIM;

		//atributy
		novy->n=0;
		novy->T1=0;
		novy->T2=0;
		novy->A=0;
		novy->S=NULL;
		novy->V=NULL;

		//ukazatelové záležitosti
		novy->predchozi=novy;//ukazuje sam na sebe
		novy->dalsi=NULL;
		SIM=novy;
		//odstranění již nepotřebného ukazatale
		novy=NULL;delete novy;//toto opravdu možno?
	}

	////(pokračuje) již standardní vložení
	//alokace  - musí být další
	TSIM *novy=new TSIM;

	//atributy
	novy->n=SIM->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->T1=T1;//čas vstup
	novy->T2=T2;//čas vstup
	novy->A=A;//akce
	novy->S=S;//stopka
	novy->V=V;//vozík

	//ukazatelové záležitosti
	SIM->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=SIM->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na žádny dalsí prvek neodkazuje (neexistuje)
	SIM->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí

	//odstranění již nepotřebného ukazatale
	novy=NULL;delete novy;//toto opravdu možno?
}
////---------------------------------------------------------------------------
//přichytí X souřadnici na nejbližší čas akci v grafu pro vozík V
int Cvektory::prichytit_cas_SIM(unsigned int V)
{
	//deklarace
	int X,ret=0,dif=MaxInt;
	X=F->akt_souradnice_kurzoru_PX.x/F->d.PX2SEC*3;
	Cvektory::TSIM *sim=SIM->dalsi;

	//průchod skrze simulační data
	while(sim)
	{
    //kontrola zda se jedná o správný vozík a o správný interval
		if(V==sim->V->n && sim->T1<=X && X<=sim->T2)
    {
			if(X-sim->T1<dif){dif=X-sim->T1;ret=sim->T1;}
			if(sim->T2-X<dif){dif=sim->T2-X;ret=sim->T2;}
		}
		sim=sim->dalsi;
	}
	delete sim;sim=NULL;

	//vracení přichycené souřadnice
	return ret*F->d.PX2SEC/3.0;
}
////---------------------------------------------------------------------------
//smaže z paměti
void Cvektory::vymaz_seznam_SIM()
{
	while(SIM!=NULL)
	{
		SIM->predchozi=NULL;
		delete SIM->predchozi;
		SIM=SIM->dalsi;
	};
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//zkontroluje aplikovatelnost uvažovaného hodnodty dle VID parametru, resp. čísla sloupce (aRD=4,R=5,Rz=6,Rx=7) na všech objektech, přiřazených k danému pohonu označeným parametrem PID, vratí doporučenou hodnotu dle VID a vrátí text chybouvé hlášku s problémem a doporučenou hodnotou, pokud vrátí prázdné uvozovky, je vše v pořádku
//vstupy aRD,R,Rz,Rx a výstupní číselná hodnota jsou v SI jednotkách, naopak textový řetězec problému resp. doporučení, obsahuje hodnotu již převedenou dle aRDunit, Runit, Rzunit
TTextNumber Cvektory::rVALIDACE(short VID,unsigned long PID,double aRD,double R,double Rz,double Rx,short aRDunit,short Runit,short Rzunit)
{
	TTextNumber RET;
	RET.text="";RET.number1=0.0;RET.number2=0;
	TPohon *p=vrat_pohon(PID);
	if(p!=NULL)
	{
	 //validace pouze vstupu
	 TTextNumber VaRD=validace_aRD(aRD,p);
	 TTextNumber VRz=validace_Rz(Rz,PID);
	 TTextNumber VRx=validace_Rx(Rx);

	 //výstupní text o chybách, oddělený entry, pokud se daný text vyskytuje
	 if(VaRD.text!="" && (VRz.text!="" || VRx.text!=""))VaRD.text+="<br>";if(VRz.text!="" && VRx.text!="")VRz.text+="<br>";
	 RET.text=VaRD.text+VRz.text+VRx.text;

	 //chybový kód dané kombinace problémů
	 RET.number2=VaRD.number2+VRz.number2+VRx.number2;

	 //hledání doporučené hodnoty pro daný vstup (dle VID, resp. čísla sloupce), konzolidace problémů do společné hodnoty
	 switch(VID)
	 {
		//aRD
		case 4:
		{
			RET.number1=ceil(validace_Rz(VaRD.number1*PP.TT,PID).number1/R)*R/PP.TT;
			if(RET.number2)
			{
				RET.text+="<br><b>Navržená hodnota rychlosti: <u>"+AnsiString(RET.number1*(1+59.0*aRDunit))+"</u> [m/";
				if(aRDunit)RET.text+="min";else RET.text+="s";
				RET.text+="].</b>";
			}
		}break;
		//R
		case 5:
		{
			RET.number1=VaRD.number1*PP.TT/(validace_Rx(validace_Rz(validace_aRD(Rx*R/PP.TT,p).number1*PP.TT,PID).number1/R).number1);
			if(RET.number2)
			{
				RET.text+="<br><b>Navržená hodnota rozteče: <u>"+AnsiString(RET.number1*(1+999*Runit))+"</u> [";
				if(Runit)RET.text+="mm";else RET.text+="m";
				RET.text+="].</b>";
			}
		}break;
		//Rz
		case 6:
		{
			RET.number1=validace_Rx(validace_Rz(validace_aRD(Rz/PP.TT,p).number1*PP.TT,PID).number1/R).number1*R;
			if(RET.number2)
			{
				RET.text+="<br><b>Navržená hodnota rozestupu: <u>"+AnsiString(RET.number1*(1+999*Rzunit))+"</u> [";
				if(Rzunit)RET.text+="mm";else RET.text+="m";
				RET.text+="].</b>";
      }
		}break;
		//Rx
		case 7:
		{
			RET.number1=validace_Rx(validace_Rz(validace_aRD(Rx*R/PP.TT,p).number1*PP.TT,PID).number1/R).number1;
			if(RET.number2)RET.text+="<br><b>Navržená hodnota počtu palců rozestupu: <u>"+AnsiString(RET.number1)+"</u> [počet palců].</b>";
		}break;
	 }
	}
	p=NULL;delete p;
	return RET;
}
////---------------------------------------------------------------------------
TTextNumber Cvektory::validace_aRD(double aRD,TPohon *p)
{
	TTextNumber RET;
	RET.text="";RET.number1=aRD;RET.number2=0;
//	double mRD=minRD(p);//vrátí nejnižší možnou rychlost ze všech objektů, které jsou přiřazené k danému pohonu (využívá se pro S&G a PP, u KK musí být RD v souladu s TT)//pokud vrátí 0, znamená, že pohon není využíván
//	//if(mRD>aRD)
//  if(vypis_objekty_nestihajici_prejezd(p,aRD,20)!="")
//	{          //musí být rozšířeno o hlídání dostatečného mezerového fondu!!! minM(aRD,RD2,R2)
//		RET.text=AnsiString("Nedostatečná rychlost! Objekt(y) nestíhající přejezd: "+vypis_objekty_nestihajici_prejezd(p,aRD,20));
//		RET.number1=mRD;
//		RET.number2=4000;//číslo chyby
//	}
	return RET;
}
////---------------------------------------------------------------------------
TTextNumber Cvektory::validace_Rz(double Rz,unsigned long PID)
{
	TTextNumber RET;
	RET.text="";RET.number1=Rz;RET.number2=0;
	TObjekt *O=vrat_objekty_vyuzivajici_pohon(PID);
	double minRz=0.0;
	for (unsigned long i=0;i<vrat_pocet_objektu_vyuzivajici_pohon(PID);i++)
	{
		 if(m.UDV(O[i].orientace)>minRz)minRz=m.UDV(O[i].orientace);//najde minimální nutný rozestup
		 if(Rz<m.UDV(O[i].orientace))//pokud platí, nastal problém, vozíky se nevejdeou
		 {
			RET.text=AnsiString("Nedostatečný rozestup!");
			RET.number1=minRz;
			RET.number2=60;//číslo chyby
		 	break;//není třeba dalšího vyhledávání
		 }
	}
	return RET;
}
////---------------------------------------------------------------------------
TTextNumber Cvektory::validace_Rx(double Rx)
{
	TTextNumber RET;
	RET.text="";RET.number1=Rx;RET.number2=0;
	if(!m.cele_cislo(Rx))
	{
		RET.text=AnsiString("Neceločíselná hodnota počtu palců rozestupu!");
		//RET.number1=m.round(Rx);//tady je otázka zda ceil nebo round
		RET.number1=ceil(Rx);//tady je otázka zda ceil nebo round
		RET.number2=7;//číslo chyby
	}
	return RET;
}
////---------------------------------------------------------------------------
//double Cvektory::delka_dopravniku(Cvektory::TObjekt *ukaz)
//{
//		Cvektory::TObjekt *ukaz_prvni=ukaz;
//		double delka=0;
//		ukaz=ukaz->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			if(ukaz->dalsi!=NULL)//mimo poslední linie
//				delka+=Form1->m.delka(ukaz->X,ukaz->Y,ukaz->dalsi->X,ukaz->dalsi->Y);
//			else//pro poslední linii
//				delka+=Form1->m.delka(ukaz->X,ukaz->Y,ukaz_prvni->X,ukaz_prvni->Y);
//			//posun na další prvek v seznamu
//			ukaz=ukaz->dalsi;
//		}
//		return delka;
//}
////---------------------------------------------------------------------------
//void Cvektory::aktualizace_indexu_uzitych_dopravniku(short item_index)
//{
//	Cvektory::TObjekt *ukaz;
//	ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//
//	while (ukaz!=NULL)
//	{
//			if(ukaz->typ_dopravniku>item_index)
//			{
//					ukaz->typ_dopravniku--;
//			}
//			ukaz=ukaz->dalsi;//posun na další prvek
//	}
//}
////---------------------------------------------------------------------------
//bool Cvektory::kontrola_existence_dopravniku(short item_index)
//{
//	bool RET=false;
//	Cvektory::TObjekt *ukaz;
//	ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//
//	while (ukaz!=NULL)
//	{
//			if(ukaz->typ_dopravniku==item_index)
//			{
//					RET=true;break;
//			}
//			ukaz=ukaz->dalsi;//posun na další prvek
//	}
//	return RET;
//}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//DÁVKY
//vytvoří novou hlavičku pro spojový seznam dávky v zakázce
void Cvektory::hlavicka_davky_zakazky(TZakazka *zakazka)
{
	TDavka *nova=new TDavka;

	nova->n=0;
	nova->pocet_voziku=0;
	nova->pocet_prazdnych=0;
	nova->pocet_voziku=0;

	nova->dalsi=NULL;
	nova->predchozi=nova;
	zakazka->davky=nova;

	nova=NULL;delete nova;
}
//---------------------------------------------------------------------------
//vymaže předchozí dávky a vytvoří novou hlavičku, použito pro opakovanou tvorbu default zakázky
void Cvektory::inicializace_davek(TZakazka *zakazka)
{
	vymaz_davky_zakazky(zakazka);
	hlavicka_davky_zakazky(zakazka);
}
//---------------------------------------------------------------------------
//vytvoří novou dávku a vloží ji do zakázky
void Cvektory::vloz_davku(TZakazka *zakazka,unsigned long pocet_voziku,unsigned long pocet_prazdnych,unsigned long pocet_celkem)
{
	TDavka *nova=new TDavka;

	nova->pocet_voziku=pocet_voziku;
	nova->pocet_prazdnych=pocet_prazdnych;
	nova->pocet_celkem=pocet_celkem;

	vloz_davku(zakazka,nova);

	nova=NULL;delete nova;
}
//---------------------------------------------------------------------------
//vloží vytvořenou dávku do zakázky
void Cvektory::vloz_davku(TZakazka *zakazka,TDavka *davka)
{
	davka->n=zakazka->davky->predchozi->n+1;
	zakazka->davky->predchozi->dalsi=davka;
	davka->predchozi=zakazka->davky->predchozi;
	zakazka->davky->predchozi=davka;
	davka->dalsi=NULL;
}
//---------------------------------------------------------------------------
//vrátí ukazatel na dávku podle její zakázky a n
Cvektory::TDavka *Cvektory::vrat_davku(TZakazka *zakazka,unsigned long n)
{
	TDavka *ret=zakazka->davky->dalsi;
	while(ret!=NULL)
	{
		if(ret->n==n)break;
		else ret=ret->dalsi;
	}
	return ret;
}
//---------------------------------------------------------------------------
//smaže konkrétní dávku
void Cvektory::smaz_davku(TZakazka *zakazka,unsigned long n)
{
	TDavka *smaz=vrat_davku(zakazka,n),*pom=smaz->dalsi;
  //úprava indexů
	unsigned long index=smaz->n;
	while(pom!=NULL)
	{
		pom->n=index;
		index++;
		pom=pom->dalsi;
	}
	delete pom;pom=NULL;
	//vyřezení ze spojáku
	if(smaz->dalsi!=NULL)smaz->dalsi->predchozi=smaz->predchozi;
	else zakazka->davky->predchozi=smaz->predchozi;
	smaz->predchozi->dalsi=smaz->dalsi;
	//samotné mazání
	delete smaz;smaz=NULL;
}
//---------------------------------------------------------------------------
//smaze všechny dávky v zakázce
void Cvektory::vymaz_davky_zakazky(TZakazka *zakazka)
{
	while(zakazka!=NULL && zakazka->davky!=NULL)
	{
		zakazka->davky->predchozi=NULL;
		delete zakazka->davky->predchozi;
		zakazka->davky=zakazka->davky->dalsi;
	};
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//ZAKÁZKY
//vytvoří novou hlavičku pro spojový seznam ZAKAZKY
void Cvektory::hlavicka_ZAKAZKY()
{
	TZakazka *nova=new TZakazka;
	nova->n=0;
	nova->id="hlavička";
	nova->typ=1;
	nova->name="hlavička";
	nova->barva=clWhite;
	nova->pomer=0;
	nova->TT=0;
	nova->jig.sirka=0;nova->jig.delka=0;nova->jig.vyska=0;nova->jig.ks=0;
	nova->pocet_voziku=0;
	nova->serv_vozik_pocet=0;
	nova->opakov_servis=0;
	nova->cesta=NULL;//new TCesta;
	nova->davky=NULL;

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;//další prvek zatím není ukazuje na nul
	ZAKAZKY=nova;//nahraje ukazatel na hlavičku spojového seznamu na ukazatel CESTY
}
//---------------------------------------------------------------------------
//vytvoří novou hlavičku pro spojový seznam ZAKAZKY_temp
void Cvektory::hlavicka_ZAKAZKY_temp()
{
	TZakazka *nova=new TZakazka;
	nova->n=0;
	nova->id="hlavička_temp";
	nova->typ=1;
	nova->name="hlavička_temp";
	nova->barva=clWhite;
	nova->pomer=0;
	nova->TT=0;
	nova->jig.sirka=0;nova->jig.delka=0;nova->jig.vyska=0;nova->jig.ks=0;
	nova->pocet_voziku=0;
	nova->serv_vozik_pocet=0;
	nova->opakov_servis=0;
	nova->cesta=NULL;//new TCesta;
	nova->davky=NULL;

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;//další prvek zatím není ukazuje na nul
	ZAKAZKY_temp=nova;//nahraje ukazatel na hlavičku spojového seznamu na ukazatel CESTY
}
//---------------------------------------------------------------------------
//vytvoří zakázku dle zadaných parametru do spojového seznamu ZAKÁZKY
void Cvektory::vloz_temp_zakazku(UnicodeString id,unsigned short typ, UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis)
{
	TZakazka *nova=new TZakazka;
	nova->id=id;
	nova->typ=typ;
	nova->name=name;
	nova->barva=barva;
	nova->pomer=pomer;
	nova->TT=TT;
	nova->jig=jig;
	nova->pocet_voziku=pocet_voziku;
	nova->serv_vozik_pocet=serv_vozik_pocet;
	nova->opakov_servis=opakov_servis;
	nova->cesta=NULL;//new TCesta;
	hlavicka_davky_zakazky(nova);

	vloz_temp_zakazku(nova);
}
//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
void Cvektory::vloz_temp_zakazku(TZakazka *Zakazka_temp)
{
	if(ZAKAZKY_temp==NULL)hlavicka_ZAKAZKY_temp();//POKUD HLAVIČKA NEEXISTUJE, TAK JI ZALOŽÍ

	TZakazka *nova=new TZakazka;

	nova=Zakazka_temp;//novy bude ukazovat tam kam prvek Zakazka
	nova->n=ZAKAZKY_temp->predchozi->n+1;//navýším počítadlo prvku o jedničku
	ZAKAZKY_temp->predchozi->dalsi=nova;//poslednímu prvku přiřadím ukazatel na nový prvek
	nova->predchozi=ZAKAZKY_temp->predchozi;//nova prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	nova->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	ZAKAZKY_temp->predchozi=nova;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
void Cvektory::vloz_zakazku(TZakazka *Zakazka)
{
	TZakazka *nova=new TZakazka;

	nova=Zakazka;//novy bude ukazovat tam kam prvek Zakazka
	nova->n=ZAKAZKY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	ZAKAZKY->predchozi->dalsi=nova;//poslednímu prvku přiřadím ukazatel na nový prvek
	nova->predchozi=ZAKAZKY->predchozi;//nova prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	nova->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	ZAKAZKY->predchozi=nova;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
// vrátí ukazatel (resp. data) na zakázku, nejčastěji editovanou
Cvektory::TZakazka *Cvektory::vrat_zakazku(unsigned long n_zakazky)
{
	TZakazka *ukaz=NULL;
	if(ZAKAZKY!=NULL)ukaz=ZAKAZKY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		//akce s ukazatelem
		if(ukaz->n==n_zakazky)break;
		else ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	}
	return ukaz;
}
//---------------------------------------------------------------------------
// vrátí ukazatel (resp. data) na temp zakázku, nejčastěji editovanou
Cvektory::TZakazka *Cvektory::vrat_temp_zakazku(unsigned long n_zakazky)
{
	TZakazka *ukaz=NULL;
	if(ZAKAZKY_temp!=NULL)ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		//akce s ukazatelem
		if(ukaz->n==n_zakazky)break;
		else ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	}
	return ukaz;
}
//---------------------------------------------------------------------------
//vrátí ukazatel (resp. data) na editovanou zakázku, podle jejiho mGridu
Cvektory::TZakazka *Cvektory::vrat_temp_zakazku_z_mGridu(unsigned long ID)
{
	 TZakazka *ukaz=NULL;
	 if(ZAKAZKY_temp!=NULL)ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	 while (ukaz!=NULL)
	 {
		//akce s ukazatelem
		if(ukaz->mGrid!=NULL && (unsigned)ukaz->mGrid->ID==ID)break;
		else ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	 }
	 return ukaz;
}
//---------------------------------------------------------------------------
//provede editaci zakázky s uvedeným “n” ze spojového seznamu ZAKAZKY_temp
void Cvektory::edituj_temp_zakazku(unsigned long n,UnicodeString id, unsigned short typ, UnicodeString name,TColor barva,double pomer,double TT,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis)
{
	if(ZAKAZKY_temp->dalsi!=NULL && n>0)
	{
			TZakazka *ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
			while (ukaz!=NULL)
			{
				if(ukaz->n==n)//zakázka nalezena
				{
					//editace parametrů
					ukaz->id=id;
					ukaz->typ=typ;
					ukaz->name=name;
					ukaz->barva=barva;
					ukaz->pomer=pomer;
					ukaz->TT=TT;
					//ukaz->jig=jig;//JIG se už jako parametr nepožaduje, stejně jako cesta, jedná se o předávání ukazatelem při zavírání patřičného formuláře
					ukaz->pocet_voziku=pocet_voziku;
					ukaz->serv_vozik_pocet=serv_vozik_pocet;
					ukaz->opakov_servis=opakov_servis;
					break;
				}
				else ukaz=ukaz->dalsi;//posun na další prvek v seznamu
			}
	}
}
//---------------------------------------------------------------------------
//smaže zakázku s uvedeným “n” ze spojového seznamu ZAKAZKY_temp včetně přidružených cest
void Cvektory::smaz_temp_zakazku(unsigned long n)
{
	if(ZAKAZKY_temp!=NULL)
	{
			TZakazka *ukaz=ZAKAZKY_temp;//ukazatel na hlavičku
			while (ukaz!=NULL)
			{
				//akce s ukazatelem
				if(ukaz->n==n)break;
				else ukaz=ukaz->dalsi;//posun na další prvek v seznamu
			}

			//samotné mazání
			//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
			if(ukaz->dalsi!=NULL)//ošetření proti poslednímu prvku
			{
				ukaz->predchozi->dalsi=ukaz->dalsi;
				ukaz->dalsi->predchozi=ukaz->predchozi;
				//mazání a tvorba bmp zakázek
				for(int i=ZAKAZKY_temp->predchozi->n-1;i>=int(ukaz->n-1);i--)
				{
					ukaz->mGrid->scGPImageCollection->Images->Delete(i);
				}
				TZakazka *Z=ukaz->dalsi;
				while(Z!=NULL)
				{
					Z->n-=1;
					Graphics::TBitmap *bmp=F->d.nacti_nahled_cesty(Z);
					ukaz->mGrid->scGPImageCollection->Images->Add()->Bitmap=bmp;
					delete(bmp);
					Z->n+=1;
					Z=Z->dalsi;
				}
				delete Z;Z=NULL;
				//uprava indexů a posun tabulek
				Z=ukaz->dalsi;
				long top=ukaz->mGrid->Top,pom=0;
				unsigned long n=ukaz->n;
				while(Z!=NULL)
				{
					Z->n=n;
					Z->mGrid->Cells[0][1].ImageIndex=n-1;
					n++;
					//posun ostatních tabulek
					pom=Z->mGrid->Top;
					Z->mGrid->Top=top;
					top=pom;
					//posun na další zakázku
					Z=Z->dalsi;
				}
				delete Z;Z=NULL;
			}
			else//poslední prvek
			{
				if(ukaz->n==1)//pokud je mazaný prvek hned za hlavičkou
				{
					ZAKAZKY_temp->predchozi=ZAKAZKY_temp; //popř hlavička bude ukazovat sama na sebe
					ZAKAZKY_temp->dalsi=NULL;
				}
				else
				{
					ukaz->predchozi->dalsi=NULL;
					ZAKAZKY_temp->predchozi=ukaz->predchozi;//zapis do hlavičky poslední prvek seznamu
				}
				ukaz->mGrid->scGPImageCollection->Images->Delete(ukaz->n-1);
			}
			vymaz_cestu_zakazky(ukaz);
			vymaz_davky_zakazky(ukaz);
			ukaz->mGrid->Delete();
			ukaz->mGrid=NULL;
			delete ukaz;ukaz=NULL;//smaže mazaný prvek
	}
}
//---------------------------------------------------------------------------
//změní zařazení zakázky ve spojovém seznamu
void Cvektory::zmen_poradi_temp_zakazky(unsigned long aktualni_poradi,unsigned long nove_poradi)
{
	if(aktualni_poradi!=nove_poradi && nove_poradi<=ZAKAZKY_temp->predchozi->n)//pokud jsou zadaná pořadí rozdílná a zároveň není zadáváno větší číslo než n posledního prvku
	{
		TZakazka *ukaz_ap=vrat_temp_zakazku(aktualni_poradi);
		TZakazka *ukaz_np=vrat_temp_zakazku(nove_poradi);//musí být tu, aby byl uložen aktuální seznam, ne po vyjmutí
		if(ukaz_ap!=NULL && ukaz_np!=NULL)
		{
			//vyjmutí ze spojáku
			if(ukaz_ap->dalsi!=NULL)//pokud se nejedná o poslední prvek
			{
				ukaz_ap->predchozi->dalsi=ukaz_ap->dalsi;//předchozí bude ukazovat na následující
				ukaz_ap->dalsi->predchozi=ukaz_ap->predchozi; //následující bude ukazovat na další
			}
			else//pokud se jedná o poslední prvek
			{
				ZAKAZKY_temp->predchozi=ukaz_ap->predchozi;
				ukaz_ap->predchozi->dalsi=NULL;
			}
			//vložení na novou pozici
			if(aktualni_poradi<nove_poradi)//vložení za ukaz_np
			{
				ukaz_ap->predchozi=ukaz_np;
				if(ukaz_np->dalsi!=NULL)//pokud se nejedná o poslední prvek
				{
					ukaz_ap->dalsi=ukaz_np->dalsi;
					ukaz_np->dalsi->predchozi=ukaz_ap;
				}
				else//pokud se jedná o poslední prvek
				{
					ukaz_ap->dalsi=NULL;
					ZAKAZKY_temp->predchozi=ukaz_ap;
				}
				ukaz_np->dalsi=ukaz_ap;
			}
			else//vložení před ukaz_np
			{
				ukaz_np->predchozi->dalsi=ukaz_ap;//přechozí budu ukazovat na vkládaný
				ukaz_ap->predchozi=ukaz_np->predchozi; //vkládaný bude mít jako předchozí původní předchozí
				ukaz_ap->dalsi=ukaz_np;//vkládaný bude ukazovat na následující na původní
				ukaz_np->predchozi=ukaz_ap;//původní bude mít před sebou vkládaný
			}
		}
		//přeindexování (N-hodnoty) v celém seznamu, možno řešit sepáratáně, ale takto to bylo rychleji napsané
		TZakazka *ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
		unsigned long i=1;
		while (ukaz!=NULL)
		{
			ukaz->n=i++;
			ukaz=ukaz->dalsi;
		}
	}
}
//---------------------------------------------------------------------------
//po stisku OK v superformu zkopíruje data z ZAKAZKY_temp do ZAKAZKY
void Cvektory::kopirujZAKAZKY_temp2ZAKAZKY(bool mazat_ZAKAZKY_temp)
{
	vymaz_seznam_ZAKAZKY();//vymazání původního seznamu
	//hlavicka_ZAKAZKY();//založení hlavičky - není potřeba vzhledem k následující kontrukci, kdy se předává hlavička z ZAKAZKY_temp
	ZAKAZKY=ZAKAZKY_temp;//zkopírování ukazatele na hlavičku spojaku ZAKAZKY_temp, touto konstrukcí ZAKAZKY ukazují tam, kam ZAKAZKY_temp

  //znovuvytvoření default zakázky
	vytvor_default_zakazku();//po kopírování zakázek dojkde ke smazání hlavičky
	//přiřazení spárovaných ukazatelů k cestám
	TZakazka *zakazka=ZAKAZKY;
	while(zakazka!=NULL)
	{
		najdi_sparovane_elementy_ceste(zakazka);
		zakazka=zakazka->dalsi;
	}
	delete zakazka;zakazka=NULL;

	//mazání ZAKAZKY_temp
	if(mazat_ZAKAZKY_temp)//smazání již nepotřebné hlavičky spojaku ZAKAZKY_temp, nepoužívat metodu vymaz_temp_zakazky(), protože ta maže i jednotlivé objekty a ty jsou již v ostrém spojáku ZAKAZKY
	{
		ZAKAZKY_temp=NULL;
		delete ZAKAZKY_temp;
	}
}
//---------------------------------------------------------------------------
//zkopíruje data objekt ze ZAKAZEK do nezávislého ZAKAZKY_temp, není jen o předání ukazatelů slouží v momentu načítání SF
void Cvektory::kopirujZAKAZKY2ZAKAZKY_temp()
{
	//nelze: ZAKAZKY_temp=ZAKAZKY;//toto je pouze zkopírování ukazatele na hlavičku spojaku ZAKAZKY_temp, touto konstrukcí ZAKAZKY ukazují tam, kam ZAKAZKY_temp tudíž na stejná data
	//není třeba, už by měl být smazaný: vymaz_seznam_ZAKAZKY_temp();
	//hlavička se zakládá pokud neexistuje, tak v konstrukci metody vloz_temp_zakazku se poprvé vytvoří
	TZakazka *Z=ZAKAZKY->dalsi;//přeskakuje hlavičku
	while(Z!=NULL)
	{
		 //alokace paměti
		 TZakazka *T=new TZakazka;
		 //kopírování dat
		 T->id=Z->id;
		 T->typ=Z->typ;
		 T->name=Z->name;
		 T->barva=Z->barva;
		 T->pomer=Z->pomer;
		 T->TT=Z->TT;
		 T->jig=Z->jig;
		 T->pocet_voziku=Z->pocet_voziku;
		 T->serv_vozik_pocet=Z->serv_vozik_pocet;
		 T->opakov_servis=Z->opakov_servis;
		 vloz_temp_zakazku(T);//uloží zakazku
		 //kopírování cesty
		 hlavicka_cesta_zakazky(T);//volá hlavičku cesty k dané temp zakazkce
		 TCesta *C=Z->cesta->dalsi;
		 while(C!=NULL)//projíždí cestu dané zakázky
		 {
			 vloz_segment_cesty(T,C->Element,C->sparovany,C->data);
			 C=C->dalsi;//posun na další segment cesty dané zakázky
		 }
		 C=NULL;delete C;
		 //kopirování dávek
		 hlavicka_davky_zakazky(T);
		 TDavka *D=Z->davky->dalsi;
		 while(D!=NULL)//projíždí cestu dané zakázky
		 {
			 vloz_davku(T,D->pocet_voziku,D->pocet_prazdnych,D->pocet_celkem);
			 D=D->dalsi;//posun na další segment cesty dané zakázky
		 }
		 D=NULL;delete D;
		 //posun na další zakázku
		 Z=Z->dalsi;
	}
	Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
//aktualizuje cestu aktuální zakázy, pokud je zvolena, pokud ne aktualizuje defaultní zakázku
void Cvektory::update_akt_zakazky()
{
 //kontrola zda je zvolená uživatelská zakázka nebo jde o defaultní zakázku
 if(ZAKAZKA_akt==NULL || ZAKAZKA_akt!=NULL && ZAKAZKA_akt->n==0)
 {
	 vytvor_default_zakazku();//jde o default zakázku, proběhne její aktualizace
   ZAKAZKA_akt=ZAKAZKY;
 }
 else
 {
	 //aktualizace dat aktuální uživatelské zakázky
 }
 //aktualizace spárovaných ukazatelů
 najdi_sparovane_elementy_ceste(ZAKAZKA_akt);
}
//---------------------------------------------------------------------------
//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji smaže a nahradí novou
void Cvektory::vytvor_default_zakazku()
{
	////ZAKAZKA
	TZakazka *Z=ZAKAZKY;																																		//počet vozíků vygeneruje dle hodnoty WIP+jeden navíc kvůli přehlednosti, kdy začíná náběh
	Z->id=1;Z->typ=1;Z->name="Nová zakázka";Z->barva=clPurple;Z->pomer=100;Z->TT=PP.TT;Z->pocet_voziku=WIP(1)+1;Z->serv_vozik_pocet=0;Z->opakov_servis=0;
	Z->n=0;
	Z->cesta=NULL;
	Cvektory::TJig j;
	j.sirka=F->d.v.PP.sirka_jig;j.delka=F->d.v.PP.delka_jig;j.vyska=F->d.v.PP.vyska_jig;j.ks=1;//defaultní hodnoty jigu
	Z->jig=j;

	////CESTA
	inicializace_cesty(Z);//vymaže předchozí cestu a zavolá hlavičku cesty nové
	vloz_cestu_po_hlavni_vetvi(Z);//vloží cestu po hlavní větvi
	////Dávky
	inicializace_davek(Z);//vymaže předchozí dávky a zavolá hlavičku cesty nové
	Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
//smaze seznam ZAKAZKY z paměti v četně přidružených cest
long Cvektory::vymaz_seznam_ZAKAZKY()
{
	long pocet_smazanych_objektu=0;
	while (ZAKAZKY!=NULL)
	{
		vymaz_cestu_zakazky(ZAKAZKY);//mazání jednotlivých cest
		vymaz_davky_zakazky(ZAKAZKY);
		ZAKAZKY->predchozi=NULL;
		delete ZAKAZKY->predchozi;
		ZAKAZKY=ZAKAZKY->dalsi;
		pocet_smazanych_objektu++;
	};
	return pocet_smazanych_objektu;//vrací hodnotu včetně hlavičky
}
//pro temp
long Cvektory::vymaz_seznam_ZAKAZKY_temp()
{
	long pocet_smazanych_objektu=0;
	while (ZAKAZKY_temp!=NULL)
	{
		vymaz_cestu_zakazky(ZAKAZKY_temp);//mazání jednotlivých cest
		ZAKAZKY_temp->predchozi=NULL;
		delete ZAKAZKY_temp->predchozi;
		ZAKAZKY_temp=ZAKAZKY_temp->dalsi;
		pocet_smazanych_objektu++;
	};

	return pocet_smazanych_objektu;//vrací hodnotu včetně hlavičky
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//cesta dané zakázky
//vytvoří novou hlavičku pro spojový seznam konkrétní cesty dané zakázky
void Cvektory::hlavicka_cesta_zakazky(TZakazka *zakazka)
{
	zakazka->cesta=new TCesta;
	TCesta *nova=zakazka->cesta;

	nova->n=0;
	nova->Element=NULL;
	nova->sparovany=NULL;

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;
}
//---------------------------------------------------------------------------
//vymaže předchozí cestu a zavolá hlavičku cesty nové
void Cvektory::inicializace_cesty(TZakazka *zakazka)
{
	vymaz_cestu_zakazky(zakazka);
	hlavicka_cesta_zakazky(zakazka);
}
//---------------------------------------------------------------------------
//vytvoří cestu po hlavní vetvi projektu, pokud je nastaveno po vyhybku vytvoří cestu po první výhybku (včetně)
void Cvektory::vloz_cestu_po_hlavni_vetvi(TZakazka *zakazka,bool po_vyhybku)
{
	TElement *E=ELEMENTY->dalsi;
	while(E!=NULL)
	{
		vloz_segment_cesty(zakazka,E);
		if(po_vyhybku && E->eID==300)break;
		E=E->dalsi;//po hlavní vetvi
	}
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//do konkrétní zakázky vloží segment cesty
void Cvektory::vloz_segment_cesty(TZakazka *zakazka,TElement *element)
{
	TCesta *segment=new TCesta;
	kopiruj_data_elementu(element->data,segment);//kopiruj data
	segment->Element=element;
	segment->sparovany=NULL;

	vloz_segment_cesty(zakazka,segment);
}
//---------------------------------------------------------------------------
//do konkrétní zakázky vloží segment cesty, slouží pro kopírování zakázek
void Cvektory::vloz_segment_cesty(TZakazka *zakazka,TElement *element,TElement *sparovany,Tdata data)
{
	TCesta *segment=new TCesta;
	kopiruj_data_elementu(data,segment);//kopiruj data
	segment->Element=element;
	segment->sparovany=sparovany;

	vloz_segment_cesty(zakazka,segment);
}
//---------------------------------------------------------------------------
//přetížená funkce, zajišťuje samotné uložení
void Cvektory::vloz_segment_cesty(TZakazka *zakazka,TCesta *segment_cesty)
{
	TCesta *segment=new TCesta;
	segment=segment_cesty;
	segment->n=zakazka->cesta->predchozi->n+1;//navýším počítadlo prvku o jedničku
	zakazka->cesta->predchozi->dalsi=segment;//poslednímu prvku přiřadím ukazatel na nový prvek
	segment->predchozi=zakazka->cesta->predchozi;//nova prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	segment->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	zakazka->cesta->predchozi=segment;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//kopíruje cestu zakázky do jiné zakázky
void Cvektory::kopiruj_cestu_zakazky(TZakazka *original,TZakazka *kopie)
{
	inicializace_cesty(kopie);//smazání staré a vložení hlavičky
	TCesta *C=original->cesta->dalsi,*C_kop=NULL;
	while(C!=NULL)
	{
		//vytvoření kopie cesty
		C_kop=new TCesta;
		C_kop->n=C->n;
		kopiruj_data_elementu(C->data,C_kop);//kopiruj data
		C_kop->Element=C->Element;
		C_kop->sparovany=C->sparovany;
		//zařazení kopie cesty do cílové zakázky
		kopie->cesta->predchozi->dalsi=C_kop;
		C_kop->predchozi=kopie->cesta->predchozi;
		C_kop->dalsi=NULL;
		kopie->cesta->predchozi=C_kop;
    C_kop=NULL;delete C_kop;
		C=C->dalsi;
	}
	delete C;C=NULL;
}
//---------------------------------------------------------------------------
//ověří zda daný element je součástí cesty dané zakázky či nikoliv, pokud ano vrací ukazatel na daný segment
Cvektory::TCesta *Cvektory::obsahuje_segment_cesty_element(TElement *element,TZakazka *zakazka)
{
	 TCesta *RET=NULL;
	 TCesta *C=zakazka->cesta->dalsi;
	 while(C!=NULL)
	 {
		 if(C->Element==element){RET=C;break;}
		 C=C->dalsi;
	 }
	 C=NULL;delete C;
	 return RET;
}
//---------------------------------------------------------------------------
//vrátí počet výskytů elementu v cestě zakázky
unsigned int Cvektory::kolikrat_obsahuje_segment_cesty_element(TElement *element,TZakazka *zakazka)
{
	 unsigned int RET=0;
	 TCesta *C=zakazka->cesta->dalsi;
	 while(C!=NULL)
	 {
		 if(C->Element==element)RET++;
		 C=C->dalsi;
	 }
	 delete C;C=NULL;
	 return RET;
}
//---------------------------------------------------------------------------
//ověří zda daný element je součástí cesty nějaké zakázky či nikoliv, pokud ano vrací ukazatel
Cvektory::TZakazka *Cvektory::obsahuje_segment_cesty_element(TElement *element)
{
	 TZakazka *Z=ZAKAZKY->dalsi;
	 while(Z!=NULL)//prochází jednotlivé zakázky
	 {
		 if(obsahuje_segment_cesty_element(element,Z)!=NULL)
		 {return Z;}//nalezen
		 else
		 Z=Z->dalsi;
	 }
	 return NULL;
}
//---------------------------------------------------------------------------
//vrátí konkrétí segment cesty v zakázce, který obsahuje element
Cvektory::TCesta *Cvektory::vrat_segment_cesty(TZakazka *zakazka,TElement *element)
{
	TCesta *c=zakazka->cesta->dalsi;
	while(c!=NULL)
	{
		if(c->Element==element)break;
		else c=c->dalsi;
	}
	return c;
}
//---------------------------------------------------------------------------
//najde a uloží všem segmentům cesty spárované elementy
void Cvektory::najdi_sparovane_elementy_ceste(TZakazka *zakazka)
{
	TCesta *c=zakazka->cesta,*c_pom=NULL;
	//mazání starých ukazatelů
	while(c!=NULL)
	{
		c->sparovany=NULL;
		c=c->dalsi;
	}
	//přiřazení nových
	c=zakazka->cesta;
	while(c!=NULL)
	{
		if(c->n>0)//přeskočení hlavičky
		{
			if(vrat_druh_elementu(c->Element)==0)//pokud je segment cesty S&G hledám mu spárovaný element
			{
				//hledání spárovaného
				c_pom=c->dalsi;//začnu od dalšího segmentu cesty
				while(c_pom!=NULL)
				{
					if(vrat_druh_elementu(c_pom->Element)==0)//pokud je element v segmentu je také S&G uložím ho
					{
						c->sparovany=c_pom->Element;
						c=c_pom->predchozi;//skok v průchodu na předchozí element dalšího S&G elementu, předchozí z důvodu, že na konci cyklu c=c->dalsi
						break;
					}
					else c_pom=c_pom->dalsi;//nenašel, pokračuj v hledání
				}
        //nenašel jsem, hledám od začátku
				if(c->sparovany==NULL && c_pom==NULL)//došel jsem na konec seznamu a nenašel žádný S&G element, průchod od začátku
				{
					c_pom=zakazka->cesta->dalsi;
					while(c_pom!=NULL)
					{
						if(vrat_druh_elementu(c_pom->Element)==0)//pokud je element v segmentu je také S&G uložím ho
						{
				  		c->sparovany=c_pom->Element;
							break;
						}
			  		else c_pom=c_pom->dalsi;
			  	}
				}
				//ukazatelové záležitosti
				c_pom=NULL;delete c_pom;
      }
		}
		c=c->dalsi;
	}
	//ukazatelové záležitosti
	delete c;c=NULL;
	c_pom=NULL;delete c_pom;
}
//---------------------------------------------------------------------------
//vymaže celou cestu dané zakázky
void Cvektory::vymaz_cestu_zakazky(TZakazka *zakazka)
{
		while (zakazka->cesta!=NULL)
		{
			zakazka->cesta->predchozi=NULL;
			delete zakazka->cesta->predchozi;
			zakazka->cesta=zakazka->cesta->dalsi;
		};
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//VOZIKY
void Cvektory::hlavicka_VOZIKY()
{
	//alokace
	TVozik *novy=new TVozik;

	//atributy
	novy->n=0;
	novy->X=0;
	novy->Y=0;
	novy->typ=0;
	novy->stav=-1;
	novy->zakazka=NULL;
	novy->element=NULL;
	novy->palec=NULL;

	//ukazatelové záležitosti
	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	VOZIKY=novy;
}
////---------------------------------------------------------------------------
//vygeneruje podle zadané zakázky seznam vozíků v úvodním rozložení
void Cvektory::generuj_VOZIKY()
{
	////příprava seznamu a update zakázky
	vymaz_seznam_VOZIKY();
	hlavicka_VOZIKY();
	update_akt_zakazky();

	if(ZAKAZKA_akt!=NULL && ZAKAZKA_akt->cesta!=NULL)//možná nadbytečné
	{
		////definice globálních proměnných
		double umisteniCas=0;/*pro testy úvodní simulaceF->Poffset*/;//umístění vozíku z časového hlediska
		double akt_rotace_jigu=0;//výchozí rotace jigu na lince
		bool rotacni_zbytek=false;//indikátor, pokud předcházela kontinuální/pasivní otoč
		bool SG=SGlinka();//typ linky, pokud obsahuje alespoň jeden SG elemement je již SG, kontinuální má význám hlavně pro prvotní zakreslování
		int pocet_voziku_z_prejezdu_na_bufferu=0;//počet vozíků přicházejících z přejezdu, které již "narazily" do bufferu
		bool prvni_rotace_jigu=true;//pro případy SG linky, kde je dříve otoč než stopka

		////PROVIZORNÍ vytvoření pole pro již prošlé elementy, kvůli  bufferu na vedlejší větvi
		TElement *E=ELEMENTY->dalsi;
    T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
		unsigned long pocet_elementu=0;
		while(E!=NULL)
		{
			pocet_elementu++;
			E=dalsi_krok(VYHYBKY,E);
		}
		vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
		E=NULL;delete E; //pokud nedojde algoritmus na konec seznamu
		delete E;E=NULL; //pokud dojde na konec seznamu
		bool *POLE_element_pouzit=new bool[pocet_elementu];

		////procházení seznamu cesty dané zakázky
		TCesta *C=ZAKAZKA_akt->cesta->dalsi;//přeskočí hlavičku
		while (C!=NULL)
		{
			////pro linku s alespoň s jedním S&G elementem, postupuje se, resp.
			////taktuje se od prvního S&G elementu na lince, generuje se dopředně (co následuje elementu, proto Et=E->dalsi aj., např. od dané stop po další stop), mimo bufferu, ten se generuje zpětně
			if(SG)
			{
				if(prvni_rotace_jigu && C->Element->rotace_jig!=0 && -180<=C->Element->rotace_jig && C->Element->rotace_jig<=180){akt_rotace_jigu+=C->Element->rotace_jig;}//pro případy SG linky, kde je dříve otoč než stopka
				if(vrat_druh_elementu(C->Element)==0 && C->Element->sparovany!=NULL)//pro S&G který má spárovaný objekt,resp. od S&G elementu až po jeho spárovaný, mezi tím segmenty neřeší
				{
					///inicializace
					//vymazání předchozího umístění, protože u S&G se vykresluje vždy od dané stopky bez zohlednění "zbytku z minula" tj. umisteni
					umisteniCas=0;
					bool predchazi_stop=true;
					prvni_rotace_jigu=false;
					//ukazatelové záležitosti
					TCesta *Ct=C->dalsi;if(Ct==NULL)Ct=ZAKAZKA_akt->cesta->dalsi;//další, protože ten spravuje geometrii před sebou, tzn. od daného stop elementu, případně další kolo spojáku, musí se brát datové atributy z dat segmentu cesty nikoliv dat elementu
					TElement *Esd=C->sparovany->dalsi;if(Esd==NULL)Esd=ZAKAZKA_akt->cesta->dalsi->Element;//případně další kolo spojáku
					if(C->Element==C->sparovany)Esd=C->sparovany;//pouze pro situace, kdy je na lince zatím jenom jeden stop-element tak, aby se zobrazovaly vůbec vozíky
					///samotné generování
					//vozíky v BUFFRu ale pozor ještě v předchozím (před-po stop segement), vozíky stojící v bufferu, včetně toho na stopce, generuje se zpětně
					if(vrat_druh_elementu(C->Element)==0/* && C->Element->data.pocet_voziku>0*/)generuj_voziky_stop_a_bufferu(C->Element,akt_rotace_jigu,pocet_voziku_z_prejezdu_na_bufferu);
					pocet_voziku_z_prejezdu_na_bufferu=0;//vynulování již zohledněno výše
					//vozíky na PŘEJEZDECH mimo stop elementů (nicméně i přes stop se musí kvůli umístění projít) - procházení cyklem od dalšího elementu daného stop elementů (z dané stopky) až po jeho spárovaný stop element, generuje se dopředně
					while(Ct->Element!=Esd)
					{
						//výpočetní část
						TPointD_3Dbool RET=generuj_voziky_segementu_mimo_stop_a_buffer(Ct->Element,Ct->Element->data.pocet_voziku,umisteniCas,akt_rotace_jigu,rotacni_zbytek,predchazi_stop);//pokud má či nemá pohon, řeší uvnitř metody
						umisteniCas=RET.x;akt_rotace_jigu=RET.y;pocet_voziku_z_prejezdu_na_bufferu=RET.z;rotacni_zbytek=RET.b;predchazi_stop=false;//za účely použítí v dalším kole
						//ukazatelové záležitost pro další krok
						if(Ct->dalsi==NULL)Ct=ZAKAZKA_akt->cesta->dalsi;//další kolo spojáku
						else {Ct=Ct->dalsi;}//další element
						if(C->Element==C->sparovany){Esd=C->sparovany->dalsi;if(Esd==NULL)Esd=ZAKAZKA_akt->cesta->dalsi->Element;/*případně další kolo spojáku*/}//pouze pro situace, kdy je na lince zatím jenom jeden stop-element tak, aby se zobrazovaly vůbec vozíky
					}
					///odstranění již nepotřebných ukazatelů, zde prvně nutné NULL!!!
					Ct=NULL;delete Ct;
					Esd=NULL;delete Esd;
				}
				///PROVIZORNĚ pro buffer na vedlejší větvi
				POLE_element_pouzit[C->Element->n-1]=true;//nutné odečíst do n elementu jedna, elementy číslované od 1, pole číslováno od 0
			}
			////pro KONTINUÁLNÍ linku tj. bez S&G elementu, např. ale i pro úvodní zakreslování
			////taktuje se od začátku prvního geometrického elementu prvního vloženého objektu, generuje se se zpětně (co předchází elementu)
			else
			{
				TPointD_3Dbool RET=generuj_voziky_segementu_mimo_stop_a_buffer(C->Element,C->data.pocet_voziku,umisteniCas,akt_rotace_jigu,rotacni_zbytek,false);//pokud má či nemá pohon, řeší uvnitř metody
				umisteniCas=RET.x;akt_rotace_jigu=RET.y;pocet_voziku_z_prejezdu_na_bufferu=0;rotacni_zbytek=RET.b;//za účely použítí v dalším kole
			}
			//ukazatelové záležitosti
			C=C->dalsi;//posun na další prvek cesty
		}
		delete C;//osdranění již nepotřebného pomocného ukazatele

		///PROVIZORNĚ vytvoření bufferu na vedlejší větvi
//		E=ELEMENTY->dalsi;
//		while(E!=NULL)
//		{                        //nutné odečíst do n elementu jedna, elementy číslované od 1, pole číslováno od 0
//			if(POLE_element_pouzit[E->n-1]==false && vrat_druh_elementu(E)==0 && E->data.pocet_voziku>0)//pokud se jedná o nepoužitý element a je to zároveň S&G element a obsahuje více vozíků
//			generuj_voziky_stop_a_bufferu(E,90/*nutné ještě dodat dle skutečného stavu*/,0);
//			E=dalsi_krok(E);
//		}
		E=NULL;delete E;
		delete[] POLE_element_pouzit;

		////obrácení přeindexování číslování vozíků, aby byl první vozík první najetý na lince
		TVozik *V=VOZIKY->dalsi;
		while(V!=NULL)
		{
			V->n=VOZIKY->predchozi->n-(V->n-1);//inverzní přepočítání indexu
			V=V->dalsi;//posun na další vozík
		}
		delete V;
	}
}
////---------------------------------------------------------------------------
//podpůrná metoda metody výše uvedené, řeší vždy jen daný geometrický segment daného elementu, mimo bufferu s vozíkem na stopce (i mimo jednovozíkový buffer)
TPointD_3Dbool Cvektory::generuj_voziky_segementu_mimo_stop_a_buffer(TElement *E,unsigned int pocet_voziku,double umisteniCas,double akt_rotace_jigu,bool rotacni_zbytek,bool predchazi_stop)
{
	TPointD_3Dbool RET;//návratová hodnota
	if(E->pohon!=NULL && E->pohon->aRD>0)//pokud je element resp. jeho geometrie relevantní k zobrazení vozíků, bez pohonu či uvedené rychlosti to nejde
	{
		double umisteni=umisteniCas*E->pohon->aRD;//umístění vozíku z prostorového hlediska, přepočet z časoveho
		double Rz=m.Rz(E->pohon->aRD);//stanovený rozestup dle RD pohonu
		double buffer_zona=0; if(pocet_voziku>0)buffer_zona=pocet_voziku*PP.delka_podvozek/*toto ne, jde jen o délku mezi uchyty-PP.uchyt_pozice*/;//délka [v metrech] buffrovácí zony, pokud je obsažena na daném elementu
		if(E->eID==200 && E->dalsi!=NULL && E->dalsi->pohon!=NULL)buffer_zona=buffer_pres_predavaci_misto(E->dalsi);//pokud se generují vozíky v segmentu předávacího místa, které je součástí bufferu
		RET.z=0;//počet vozíků z přejezdu, které už přesahuji buffer, a ten o danou hodnotu navyšují
		//cyklické navýšení umístění dle rozestup Rz, včetně řešení těch, co se do segmentu nevejdou kvůli bufferu
		while(umisteni<=E->geo.delka)
		{
			if(umisteni<=E->geo.delka-buffer_zona)//vozíky pouze na přejezdu
			{
				short stav=1;//stav uchycení vozíku
				double umisteni_temp=umisteni;if(umisteni<0){umisteni=0;stav=0;}//zatím čeká na uchycení na palec
				//aplikace náhodného čekání na palce problem while, problém musí se vejít na palce musí mít vozíky rozestup dle R atd... umisteni+=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,2)*E->pohon->aRD;
				//výpočet souřadnic a rotace jigu dle aktuálního umístění
				TPointD_3D Pt=m.getPt(E->geo.radius,E->geo.orientace,E->geo.rotacni_uhel,E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4,umisteni/E->geo.delka,(umisteni+PP.uchyt_pozice-PP.delka_podvozek/2.0)/E->geo.delka);
				//pasivní otoče - POZOR pouze pro kontinuální/pasivní otoč pro aktavní bude sice na místě, ale řešit otáčením dle umisteniČas
				double R=0;
				double umisteniJIG=umisteni+PP.uchyt_pozice-PP.delka_podvozek/2.0;//pokud není uchytávání podvozku v polovině vozíku (pokud je řeší i toto), je třeba rotovat JIG dle jiné polohy tj. dle středu jigu (vyplývá z kontstrukce otoče) nikoliv od polohy palce (která je pouze totožná se středem jigu resp. i podvozku v situaci, kdy je uchytávání nastaveno na střed vozíku
				if(E->OTOC_delka>0 && E->geo.delka<=umisteniJIG+E->OTOC_delka/2.0 && E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180)
				{
					R=E->rotace_jig/2.0*(E->OTOC_delka/2.0-(E->geo.delka-umisteniJIG))/(E->OTOC_delka/2.0);//pozice vozíku v zoně otáčení, v počátku až do středu otoče, princip výpočtu zde funguje jako PŘIČTENÍ rotace k orientaci jigu při vstupu do zóny otáčení
				}
				if(rotacni_zbytek)//dokončení rotace jigu na elementu následujícím otoči (který zajišťuje na svém geometrickém počátku, který začíná otočí)
				{
					if(umisteniJIG<=E->predchozi->OTOC_delka/2.0)R=-E->predchozi->rotace_jig/2.0*(E->predchozi->OTOC_delka/2.0-umisteniJIG)/(E->predchozi->OTOC_delka/2.0);//pozice vozíku v zoně otáčení, od středu otoče až do konce zóny otáčení, princip výpočtu zde funguje jako ODEČTENÍ rotace od FINÁLNÍ orientaci jigu při vÝstupu ze zóny otáčení
					else rotacni_zbytek=false;//dokončena ilustrace otáčení JIGu
				}
				//finální vložení vozíku s vypočítanými parametry do spojáku VOZIKY
				if(!predchazi_stop)vloz_vozik(ZAKAZKA_akt,E,Pt.x,Pt.y,Pt.z,akt_rotace_jigu+R,stav);//vkládá se pouze pokud se nejedná o stopku, vozíky stopky se vkládájí v separátní metodě na tvorbu bufferu, (nicméně i přes stop se musí kvůli umístění zde projít, a proto se tato metoda celá volá)
				else predchazi_stop=false;//zdisablování pro další stavy, pokud bylo zrovna aktivní/tj. stop element
				//vrácení původní hodnoty
				umisteni=umisteni_temp;
			}
			else//počet vozíků přicházejících z přejezdu, které již "narazily" do bufferu a navyšují ho
			{
				if(E->geo.delka-buffer_zona+PP.delka_podvozek>umisteni)RET.z++;//ale řeší se pouze pro ty, kterou by byly v buffru z části, tzn. by přijížděly, ty které vyplývájí z umístnění dle Rz a jsou celýrm rozměrem umístěny v bufferu, jsou již součástí bufferu a tím pádem již buffer nanavyšují
			}
			//navýšení umístění dle rozestup Rz
			//ShowMessage(String(umisteni)+" +Rz");
			umisteni+=Rz;
		}
		//kalkulace s umístěním pro další segment
		//ShowMessage(String(umisteni));
		umisteni-=E->geo.delka;//zbytek z předchzejícího geometrického úseku, který nestihl být zohledněn převeden na další geometrický úsek, resp. element = výchozí umístění v dalším elementu, případně zohlední i přechod na nový pohon (díky práci v jednotkách délky), pouze je následně nutné odečíst případné WT při přechodu
		//ShowMessage(String(umisteni));
		umisteniCas=umisteni/E->pohon->aRD;//z praktického univerzálního hlediska dané zpoždění resp. časový fond vrací v časé (není díky tomu následně nutné hledat hodnotu rychlosti předchozího pohonu)
		if(E->eID==200)umisteniCas-=E->WT;//čekání na předávacím místě způsobí následné zpoždění/rozsunutí mezi vozíků, upozornění čekání se počítá až z rychlosti dalšího pohonu
		//zajištění aktuální rotace pro následující úsek
		if(E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180)
		{
			akt_rotace_jigu+=E->rotace_jig;//pro uchování následujícího stavu
			rotacni_zbytek=true;//pro zoohlednění v dalším kole
		}
	}
	RET.x=umisteniCas;
	RET.y=akt_rotace_jigu;
	RET.b=rotacni_zbytek;
	return RET;
}
////---------------------------------------------------------------------------
//vygeneruje vozíky v bufferu včetně aktuálního vozíku na stopce, počet vozíků přicházejících z přejezdu, které již "narazily" do bufferu
void Cvektory::generuj_voziky_stop_a_bufferu(TElement *E,double akt_rotace_jigu,int pocet_voziku_z_prejezdu_na_bufferu)
{
	short stav=0;//stav uchycení vozíku, první vozík čeká na palec, další vyháknuto
	double X1=E->geo.X1;
	double Y1=E->geo.Y1;
	double delka=E->geo.delka;
	double delka_BpPM=buffer_pres_predavaci_misto(E);
	if(delka_BpPM>0)
	{
		delka+=delka_BpPM;
		X1=E->predchozi->geo.X1;
		Y1=E->predchozi->geo.Y1;
	}
	double umisteni=delka-(E->data.pocet_voziku+pocet_voziku_z_prejezdu_na_bufferu-1)*PP.delka_podvozek;//delka bufferu (-1, protože druhý vozík má až pozici 1xdelka podvozku), nepouživat -PP.uchyt_pozice, protože se to počítá právě pro úchyt nikoliv začátek vozíku
	while(umisteni<=delka+0.0000000001)//generuje vozíky bufferu včetně toho na stopce, nutno s <=,  0.0000000001 nutný WA kvůli řádové chybě při porovnání dvou totožných double hodnot
	{
		TPointD_3D Pt=m.getPt(E->geo.radius,E->geo.orientace,E->geo.rotacni_uhel,X1,Y1,E->geo.X4,E->geo.Y4,umisteni/delka,(umisteni+PP.uchyt_pozice-PP.delka_podvozek/2.0)/delka);//zjištění aktuálních souřadnic vozíků
		//TZakazka *Z=new TZakazka;//pouze jen kvůli testům
		if(pocet_voziku_z_prejezdu_na_bufferu>0){/*Z->n=1;Z->barva=clRed;*/stav=-2;pocet_voziku_z_prejezdu_na_bufferu--;}//barevné odlišení pouze jen kvůli testům, vozík na přejezdu, který narazil do bufferu nad rámec jeho nastaveného počtu
		else {/*Z->n=2;Z->barva=clBlue;*/stav=-1;}//pasivní stojí - vyháknutý (v bufferu)barevné odlišení pouze jen kvůli testům
		if(umisteni+PP.delka_podvozek-0.001>delka)stav=0;//vozík přímo na stopce - barevné odlišení pouze jen kvůli testům, 0.001 pouze WA aby nenastavalo stav i druhému vozíku
		//vloz_vozik(Z,E,Pt.x,Pt.y,Pt.z,akt_rotace_jigu,stav);//finální vložení vozíku s vypočítanými parametry do spojáku VOZIKY, se stave odháknuto
		vloz_vozik(ZAKAZKA_akt,E,Pt.x,Pt.y,Pt.z,akt_rotace_jigu,stav);//finální vložení vozíku s vypočítanými parametry do spojáku VOZIKY, se stave odháknuto
		stav=-1;//další již vyháknut
		umisteni+=PP.delka_podvozek;//posun o další vozík
	}
}
////---------------------------------------------------------------------------
//pokud je buffer přes předávací místo, vrátí délku bufferu, co předchází předávacímu místu, pokud ne vrátí se 0, založeno na úvaze, že v bufferu nemůže být výhybka
double Cvektory::buffer_pres_predavaci_misto(TElement *E)
{                        //pozn. zda jed před PM linie se řešit na úrovni editace, v zjišťování velikosti bufferu
	if(E->pohon!=NULL && vrat_druh_elementu(E)==0 && E->predchozi!=NULL && E->predchozi->pohon!=NULL && E->predchozi->eID==200 && E->geo.delka<E->data.pocet_pozic*PP.delka_podvozek-PP.uchyt_pozice)//pokud je buffer od dané stop přes předávací místo
	{
		//F->Memo("Delka: "+String(E->geo.delka)+" výp.:"+String(E->data.pocet_pozic*PP.delka_podvozek-PP.uchyt_pozice));
		return E->data.pocet_pozic*PP.delka_podvozek-PP.uchyt_pozice-E->geo.delka;
	}
	else return 0;
}
////---------------------------------------------------------------------------
//uloží ukazatel na vozík do spojového seznamu voziků
void Cvektory::vloz_vozik(TZakazka *zakazka,TElement *element,double X,double Y,double orientaceP,double rotaceJ,short stav)
{
	//alokace
	TVozik *novy=new TVozik;

	//atributy
	novy->n=0;
	novy->X=X;
	novy->Y=Y;
	novy->orientace_podvozek=orientaceP;
	novy->rotace_jig=rotaceJ;
	novy->typ=0;
	novy->stav=stav;
	novy->zakazka=zakazka;
	novy->element=element;
	novy->palec=NULL;

	//ukazatelové záležitosti
	novy->n=VOZIKY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	VOZIKY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=VOZIKY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje)
	VOZIKY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
};
//---------------------------------------------------------------------------
//dle n resp. ID vozíku vrátí ukazatel na daný vozík
Cvektory::TVozik *Cvektory::vrat_vozik(unsigned int n)
{
	TVozik *V=VOZIKY->dalsi;
	while (V!=NULL)
	{
		if(V->n==n)break;//nalezen přeruší se další vyhledávání
		V=V->dalsi;
	};
	return V;
}
////---------------------------------------------------------------------------
void Cvektory::vymaz_seznam_VOZIKY()
{    //F->Memo("_________________________",true);
	while (VOZIKY!=NULL)
	{
		delete VOZIKY->predchozi;
		VOZIKY->predchozi=NULL;

		VOZIKY=VOZIKY->dalsi;
	};
};
//---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvektory::hlavicka_PROCESY()
{
	TProces *novy=new TProces;
	novy->n=0;
	novy->n_v_zakazce=0;
	novy->Tpoc=0;
	novy->Tkon=0;
	novy->Tdor=0;
	novy->Tpre=0;
	novy->Tcek=0;
	novy->Trand=0;
	novy->vozik=NULL;

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;//další je zatim NULL
	PROCESY=novy;
}
//---------------------------------------------------------------------------
//uloží ukazatel na proces do spojového seznamu procesů přetížená fce
void Cvektory::vloz_proces(TProces *Proces)
{
	TProces *novy=new TProces;
	novy=Proces;//novy bude ukazovat tam kam prvek data

	novy->n=PROCESY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	PROCESY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=PROCESY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	PROCESY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//hledá proces mezi procesy
Cvektory::TProces *Cvektory::najdi_proces(double cas, double vozik)
{
	TProces *RET=NULL;
	TProces *P=PROCESY->dalsi;
	while (P!=NULL)
	{
		if(P->vozik->n==vozik && P->Tpoc<=cas && cas<P->Tcek)//pokud se myš nachází nad právě cyklem procházeným procesem
		{  //ShowMessage(AnsiString(cas)+","+AnsiString(P->Tpoc)+","+AnsiString(P->Tcek));
			 RET=P;//proces nalezen
			 break;//ukončí předčasně while cyklus-zbytečně by se hledalo dál, proces byl již nalezen
		}
		P=P->dalsi;
	};
	return RET;
}
//---------------------------------------------------------------------------
//vratí následující proces na stejném objektu jako proces zadaný
Cvektory::TProces *Cvektory::vrat_nasledujici_proces_objektu(TProces *Proces)
{
		TProces *P=NULL;//Proces->dalsi;
//		while (P!=NULL)
//		{
//			if(P->segment_cesty->objekt==Proces->segment_cesty->objekt)
//			{
//				break;
//			}
//			P=P->dalsi;
//		};
		return P;
}
//---------------------------------------------------------------------------
long Cvektory::vymaz_seznam_PROCESY()
{
	long pocet_smazanych_objektu=0;
	while (PROCESY!=NULL)
	{
		pocet_smazanych_objektu++;
		delete PROCESY->predchozi;
		PROCESY->predchozi=NULL;
		PROCESY=PROCESY->dalsi;
	};

	return pocet_smazanych_objektu;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//////vytvoří novou hlavičku pro řetězy
//void Cvektory::hlavicka_RETEZY()
//{
//	TRetez *novy=new TRetez;
//	novy->n=0;
//	novy->name="";//celý název objektu
//
//	novy->predchozi=novy;//ukazuje sam na sebe
//	novy->dalsi=NULL;
//	RETEZY=novy;//RETEZY
//}
//////---------------------------------------------------------------------------
//////uloží retez a jeho parametry do spojového seznamu
//void Cvektory::vloz_retez(AnsiString name, double roztec)
//{
//	TRetez *novy=new TRetez;
//
//	novy->n=RETEZY->predchozi->n+1;//navýším počítadlo prvku o jedničku
//	novy->name=name;
//	novy->roztec=roztec;
//
//	RETEZY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=RETEZY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;
//	RETEZY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//}
////---------------------------------------------------------------------------
////z položky (předpoklad vybrané) v comboboxů řetězů vrátí pouze hodnotu rozteče
//double Cvektory::vrat_roztec_retezu_z_item(AnsiString item,AnsiString separator)
//{
//	return Form1->ms.MyToDouble(Form1->ms.TrimLeftFromText(item,separator));
//}
////---------------------------------------------------------------------------
////vypíše všechny použitelné řetezy použitelné pro zadané rozmezí dle užité rozteče, separátor odděluje název řetězu od rozteče, totál separátor jednotlivé řetězy, pokud je Rz zadané nulové vrátí hodnotu nula, pokud chci vypsat všechny načtené řetězy ze souboru retezy.csv použiji parametr Rz=-1, pokud není požadován výpis názvu řetězu použiji prázdné uvozovky
//AnsiString Cvektory::vypis_retezy_s_pouzitelnou_rozteci(double Rz,AnsiString separator,AnsiString total_separator,bool mm)
//{
//	AnsiString RET="";
//	short J=1.0;if(mm)J=1000.0;
//	if(Rz)
//	{
//			TRetez *CH=RETEZY->dalsi;
//			while(CH!=NULL)
//			{                                 //pokud chci vypsat vše
//				if(m.mod_d(Rz,CH->roztec)==0 || Rz==-1)//zbytek po dělení je nula, tzn. vhodný řetěz s roztečí vhodnou pro požadovaný rozestup nalezen nebo -1 pokud chci vypsat všechny načtené řetězy ze souboru retezy.csv
//				{
//					if(separator=="")RET+=AnsiString(J*CH->roztec)+total_separator;//pokud není požadován výpis názvu řetězu
//					else RET+=CH->name+separator+AnsiString(J*CH->roztec)+total_separator;
//				}
//				CH=CH->dalsi;
//			}
//			CH=NULL;delete CH;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
////smaze RETEZY z pameti
//long Cvektory::vymaz_seznam_RETEZY()
//{
//	long pocet_smazanych_objektu=0;
//	while (RETEZY!=NULL)
//	{
//		pocet_smazanych_objektu++;
//		RETEZY->predchozi=NULL;
//		delete RETEZY->predchozi;
//		RETEZY=RETEZY->dalsi;
//	};
//
//	return pocet_smazanych_objektu;
//};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vloží nový typ dopravníku do KATALOGu dopravníků, pokud neexistuje hlavička vytvoří ji, druh: 0 - podlahový, 1 - podvěsný
void Cvektory::vloz_typ_dopravniku(AnsiString name,UnicodeString link,short druh)
{
	////hlavička
	if(KATALOG==NULL)//pokud neexistuje hlavička ještě ji před vložením standardního prvku vytvoří
	{
		//alokace paměti
		Ttyp_dopravniku *novy=new Ttyp_dopravniku;
		//atributy
		novy->n=0;
		novy->druh=0;
		novy->name="";
		novy->link="";
		novy->roztec=NULL;
		novy->hOblouk=NULL;
		novy->hRadius=NULL;
		novy->vOblouk=NULL;
		novy->vRadius=NULL;
		//ukazatelové propojení
		novy->predchozi=novy;//ukazuje sam na sebe
		novy->dalsi=NULL;
		KATALOG=novy;
	}

	////standardní vložení nakonec
	//alokace paměti
	Ttyp_dopravniku *novy=new Ttyp_dopravniku;
	//atributy
	novy->n=KATALOG->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->druh=druh;
	novy->name=name;
	novy->link=link;
	novy->roztec=NULL;
	novy->hOblouk=NULL;
	novy->hRadius=NULL;
	novy->vOblouk=NULL;
	novy->vRadius=NULL;
	//ukazatelové propojení
	KATALOG->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=KATALOG->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;
	KATALOG->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//vloží dle typu hodnoty hodnotu do typuDopravniku, pokud je NULL, vloží nakonec (do posledního typu dopravníku uloženém ve spojovém seznamu v KaTALOGu)
void Cvektory::vloz_do_typu_dopravniku(TtypHodnoty typHodnoty,double hodnota,Ttyp_dopravniku *typDopravniku)
{
	if(KATALOG!=NULL)
	{
		if(typDopravniku==NULL)typDopravniku=KATALOG->predchozi;//pokud je NULL, vloží nakonec (do posledního typu dopravníku uloženém ve spojovém seznamu v KaTALOGu)
		////typ hodnoty
		TDoubleHodnota *TH=NULL;
		switch(typHodnoty)
		{
			case R:	 TH=typDopravniku->roztec; break;
			case hO: TH=typDopravniku->hOblouk;break;
			case vO: TH=typDopravniku->vOblouk;break;
			case vR: TH=typDopravniku->vRadius;break;
			case hR: TH=typDopravniku->hRadius;break;
		}
		////hlavička
		if(TH==NULL)//pokud neexistuje hlavička ještě ji před vložením standardního prvku vytvoří
		{
			//alokace paměti
			TDoubleHodnota *novy=new TDoubleHodnota;
			//atributy
			novy->n=0;
			novy->hodnota=0;
			//ukazatelové propojení
			novy->predchozi=novy;//ukazuje sam na sebe
			novy->dalsi=NULL;
			TH=novy;
			switch(typHodnoty)
			{
				case R:	 typDopravniku->roztec= TH;break;
				case hO: typDopravniku->hOblouk=TH;break;
				case vO: typDopravniku->vOblouk=TH;break;
				case vR: typDopravniku->vRadius=TH;break;
				case hR: typDopravniku->hRadius=TH;break;
			}
		}
		////standardní vložení nakonec
		//alokace paměti
		TDoubleHodnota *novy=new TDoubleHodnota;
		//atributy
		novy->n=TH->predchozi->n+1;//navýším počítadlo prvku o jedničku
		novy->hodnota=hodnota;
		//ukazatelové propojení
		TH->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
		novy->predchozi=TH->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		novy->dalsi=NULL;
		TH->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
	}
}
//---------------------------------------------------------------------------
//vytvoří katalog typů dopravníku za pomocí volání nasledujících dvou metod
void Cvektory::vytvor_KATALOG()
{
	////CALDAN VLD - S150
	vloz_typ_dopravniku("VLD S150","http://caldan.dk/sites/default/files/TechSpecsPDF/VLD_2018_uk.pdf",0);
	//rozteč
	vloz_do_typu_dopravniku(R,150);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,200);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,315);
	vloz_do_typu_dopravniku(hR,500);
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,90);
	vloz_do_typu_dopravniku(vO,45);
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,700);
	vloz_do_typu_dopravniku(vR,1000);

	////CALDAN VLD - S180
	vloz_typ_dopravniku("VLD S180","http://caldan.dk/sites/default/files/TechSpecsPDF/VLD_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,150);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,200);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,315);
	vloz_do_typu_dopravniku(hR,500);
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,90);
	vloz_do_typu_dopravniku(vO,45);
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,700);
	vloz_do_typu_dopravniku(vR,1000);

  ////CALDAN VLD - S200
	vloz_typ_dopravniku("VLD S200","http://caldan.dk/sites/default/files/TechSpecsPDF/VLD_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,150);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,200);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,315);
	vloz_do_typu_dopravniku(hR,500);
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,90);
	vloz_do_typu_dopravniku(vO,45);
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,700);
	vloz_do_typu_dopravniku(vR,1000);

	////CALDAN VLD - S270
	vloz_typ_dopravniku("VLD S270","http://caldan.dk/sites/default/files/TechSpecsPDF/VLD_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,270);
	vloz_do_typu_dopravniku(R,300);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,90);
	vloz_do_typu_dopravniku(vO,45);
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,1000);

	////CALDAN VLD - S300
	vloz_typ_dopravniku("VLD S300","http://caldan.dk/sites/default/files/TechSpecsPDF/VLD_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,270);
	vloz_do_typu_dopravniku(R,300);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,90);
	vloz_do_typu_dopravniku(vO,45);
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,1000);

  ////CALDAN SD
	vloz_typ_dopravniku("SD","http://caldan.dk/sites/default/files/TechSpecsPDF/SD_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,315);
	vloz_do_typu_dopravniku(hR,500);
  vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,1000);

	////CALDAN HD100
	vloz_typ_dopravniku("HD100","http://caldan.dk/sites/default/files/TechSpecsPDF/HD100_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
  vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,1500);
  vloz_do_typu_dopravniku(vR,2500);

	////CALDAN HD100S
	vloz_typ_dopravniku("HD100S","http://caldan.dk/sites/default/files/TechSpecsPDF/HD100S_2019_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
  vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,1500);
  vloz_do_typu_dopravniku(vR,2500);

	////CALDAN HD140
	vloz_typ_dopravniku("HD140","http://caldan.dk/sites/default/files/TechSpecsPDF/HD140_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,1440);
	vloz_do_typu_dopravniku(R,270);
	vloz_do_typu_dopravniku(R,540);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,1250);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
  vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
  vloz_do_typu_dopravniku(vR,2000);

	////CALDAN PF100
	vloz_typ_dopravniku("PF100","http://caldan.dk/sites/default/files/TechSpecsPDF/PF100_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
	vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,2500);
	vloz_do_typu_dopravniku(vR,1500);

	////CALDAN PF100S
	vloz_typ_dopravniku("PF100S","http://caldan.dk/sites/default/files/TechSpecsPDF/PF100S_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
  vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
  vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
  vloz_do_typu_dopravniku(vR,2500);
  vloz_do_typu_dopravniku(vR,1500);

  ////CALDAN PF120
	vloz_typ_dopravniku("PF120","http://caldan.dk/sites/default/files/TechSpecsPDF/PF120_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
  vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
  vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
  vloz_do_typu_dopravniku(vR,1000);

	////CALDAN PF140
	vloz_typ_dopravniku("PF140","http://caldan.dk/sites/default/files/TechSpecsPDF/PF140_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
  vloz_do_typu_dopravniku(R,300);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
  vloz_do_typu_dopravniku(hR,1250);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
  vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
  vloz_do_typu_dopravniku(vR,1000);
  vloz_do_typu_dopravniku(vR,2500);

	////CALDAN PF160
	vloz_typ_dopravniku("PF160","http://caldan.dk/sites/default/files/TechSpecsPDF/PF160_2018_uk.pdf",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,270);
  vloz_do_typu_dopravniku(R,300);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
  vloz_do_typu_dopravniku(hR,1250);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,30);
  vloz_do_typu_dopravniku(vO,15);
	//vertikální radiusy
  vloz_do_typu_dopravniku(vR,1500);
  vloz_do_typu_dopravniku(vR,2500);

	////Standa VaV
	vloz_typ_dopravniku("Standa_VaV","http://galatek.cz",0);
	vloz_do_typu_dopravniku(R,1800);
	vloz_do_typu_dopravniku(R,1805);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,700);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,0);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,0);

	////Testy
	vloz_typ_dopravniku("TESTY","nic.cz",0);
	vloz_do_typu_dopravniku(R,180);
	vloz_do_typu_dopravniku(R,500);
	vloz_do_typu_dopravniku(R,1800);
	//horizontální oblouky
	vloz_do_typu_dopravniku(hO,90);
	vloz_do_typu_dopravniku(hO,45);
	vloz_do_typu_dopravniku(hO,30);
	vloz_do_typu_dopravniku(hO,15);
	//horizontální radiusy
	vloz_do_typu_dopravniku(hR,1000);
	//vertikální oblouky
	vloz_do_typu_dopravniku(vO,0);
	//vertikální radiusy
	vloz_do_typu_dopravniku(vR,0);
}
//---------------------------------------------------------------------------
//dla zadaného n vrátí daný typ dopravníku formou ukazatatele
Cvektory::Ttyp_dopravniku *Cvektory::vrat_typ_dopravniku(unsigned long n)
{
	Ttyp_dopravniku *K=KATALOG->dalsi;//přeskočí hlavičku
	while(K!=NULL)
	{
		if(K->n==n)break;//pokud byl prvek nalezen vrátí na něj ukazatel
		else K=K->dalsi;
	}
	return K;
}
//---------------------------------------------------------------------------
//vrátí od zadaného typu dopravníku dle zadaného n a typu hodnoty hodnotu - přetížená následující
double Cvektory::vrat_hodnotu_typu_dopravniku(unsigned long nDopravniku,TtypHodnoty typHodnoty,unsigned long n)
{
	return vrat_hodnotu_typu_dopravniku(vrat_typ_dopravniku(nDopravniku),typHodnoty,n);
}
//---------------------------------------------------------------------------
//vrátí od zadaného typu dopravníku dle zadaného n a typu hodnoty hodnotu
double Cvektory::vrat_hodnotu_typu_dopravniku(Ttyp_dopravniku *typDopravniku,TtypHodnoty typHodnoty,unsigned long n)
{
	if(typDopravniku!=NULL)
	{
		TDoubleHodnota *TH=NULL;
		switch(typHodnoty)
		{
			case R:	 TH=typDopravniku->roztec; break;
			case hO: TH=typDopravniku->hOblouk;break;
			case vO: TH=typDopravniku->vOblouk;break;
			case vR: TH=typDopravniku->vRadius;break;
			case hR: TH=typDopravniku->hRadius;break;
		}
		if(TH!=NULL)
		{
			double RET=0;
			TH=TH->dalsi;//přeskakuje hlavičku
			while(TH!=NULL)
			{
				if(TH->n==n){RET=TH->hodnota;break;}//pokud byl prvek nalezen vrátí na něj ukazatel
				else TH=TH->dalsi;
			}
			return RET;
		}
		else return -1;
	}
	else return -2;
}
//---------------------------------------------------------------------------
//zkontroluje zda se hodnota nachází v dopravníku, například hodnota oblouku v hobloucích dopravníku
bool Cvektory::hodnota_v_katalogu(unsigned long nDopravniku,double hodnota,TtypHodnoty typHodnoty)
{
	//deklarace
	Ttyp_dopravniku *K=vrat_typ_dopravniku(nDopravniku);
	TDoubleHodnota *TH=NULL;
	bool ret=false;

	//kontrola existence doprvníku + fce
	if(K!=NULL && hodnota>0)
	{
		//načtení DoubleHodnota
		switch(typHodnoty)
		{
			//horizontální oblouky
			case hO:TH=K->hOblouk->dalsi;break;
			//vertikální oblouky
			case vO:TH=K->vOblouk->dalsi;break;
			//nedefinované
			default:break;
		}
		//načtení konkrétních hodnot
		while(TH!=NULL)
		{
			if(TH->hodnota==hodnota){ret=true;break;}
			TH=TH->dalsi;
    }
	}

  //ukazatelové záležitosti
	K=NULL;delete K;
	TH=NULL;delete TH;
  return ret;
}
//---------------------------------------------------------------------------
//smaže celý katalog, včetně přidružených spojových seznamů
void Cvektory::vymaz_seznam_KATALOG()
{
	if(KATALOG!=NULL)//pokud existuje katalog
	{
		Ttyp_dopravniku *K=KATALOG->dalsi;
		delete KATALOG;KATALOG==NULL;//smazání hlavičky
		while(K!=NULL)
		{
			////atributy
			//rozteč
			if(K->roztec!=NULL)
			{
				TDoubleHodnota *H=K->roztec->dalsi;
				delete K->roztec;K->roztec==NULL;//smazání hlavičky
				while(H!=NULL)
				{
					TDoubleHodnota *Ht=H;//ukazatel na následně mazaný objekt
					H=H->dalsi;//posun na další
					delete Ht;Ht=NULL;//odstranění již nebotřebného objektu
				}
				delete H;H=NULL;//závěrečné odstranění
			}
			//hOblouk
			if(K->hOblouk!=NULL)
			{
				TDoubleHodnota *H=K->hOblouk->dalsi;
				delete K->roztec;K->hOblouk==NULL;//smazání hlavičky
				while(H!=NULL)
				{
					TDoubleHodnota *Ht=H;//ukazatel na následně mazaný objekt
					H=H->dalsi;//posun na další
					delete Ht;Ht=NULL;//odstranění již nebotřebného objektu
				}
				delete H;H=NULL;//závěrečné odstranění
			}
			//hRadius
			if(K->hRadius!=NULL)
			{
				TDoubleHodnota *H=K->hRadius->dalsi;
				delete K->roztec;K->hRadius==NULL;//smazání hlavičky
				while(H!=NULL)
				{
					TDoubleHodnota *Ht=H;//ukazatel na následně mazaný objekt
					H=H->dalsi;//posun na další
					delete Ht;Ht=NULL;//odstranění již nebotřebného objektu
				}
				delete H;H=NULL;//závěrečné odstranění
			}
			//vOblouk
			if(K->vOblouk!=NULL)
			{
				TDoubleHodnota *H=K->vOblouk->dalsi;
				delete K->roztec;K->vOblouk==NULL;//smazání hlavičky
				while(H!=NULL)
				{
					TDoubleHodnota *Ht=H;//ukazatel na následně mazaný objekt
					H=H->dalsi;//posun na další
					delete Ht;Ht=NULL;//odstranění již nebotřebného objektu
				}
				delete H;H=NULL;//závěrečné odstranění
			}
			//vRadius
			if(K->vRadius!=NULL)
			{
				TDoubleHodnota *H=K->vRadius->dalsi;
				delete K->roztec;K->vRadius==NULL;//smazání hlavičky
				while(H!=NULL)
				{
					TDoubleHodnota *Ht=H;//ukazatel na následně mazaný objekt
					H=H->dalsi;//posun na další
					delete Ht;Ht=NULL;//odstranění již nebotřebného objektu
				}
				delete H;H=NULL;//závěrečné odstranění
			}
			////samotný typ dopravniku
			Ttyp_dopravniku *Kt=K;//ukazatel na následně mazaný objekt
			K=K->dalsi; //posun na další
			delete Kt;Kt=NULL;//odstranění již nebotřebného objektu
		}
		delete K;K=NULL;//závěrečné odstranění
	}
}
//---------------------------------------------------------------------------
//metody pro ZPRÁVY resp. VALIDACE
//---------------------------------------------------------------------------
//zkontroluje buď všechny elementy (je-li vstupní parametr NULL), smaže všechny zprávy, a kde najde problém, uloží do zpráv, v případě, že není NULL
void Cvektory::VALIDACE(TElement *Element)//zatím neoživáná varianta s parametrem pro kontrolu jenom pouze změněného elementu!!!
{
	if(F->duvod_validovat==2)//validace probíhá jenom při editaci (pokud je důvod a až byla dokončena editační akce), nebo když nejsou zprávy po spuštění aplikace
	{
	 //	TDateTime T;F->Memo("/popř. otáčení, buffry/ + generuj_VOZIKY + VALIDACE: "+T.CurrentTime().TimeString());//PROVIZORNÍ VÝPIS VOLÁNÍ TÉTO METODY

    //ZVÁŽIT ZDE VOLÁNÍ vrat_rotaci_jigu_po_predchazejicim_elementu(POSLEDNÍ ELEMENT)

		//provizorní umístnění, ač funkčně velice vhodné
		generuj_VOZIKY();

		//inicializační proměnné
		F->duvod_validovat=0;//již bude zvalidováno a dále by se zbytečně volalo
		long pocet_erroru=0,pocet_warningu=0;

		//pokud se budou testovat všechny elementy, je nutné vymazat všechny zprávy
		long tzEx,tzEy,tVID=0;if(F->d.zobrazit_celou_zpravu>0){TZprava *Z=vrat_zpravu(F->d.zobrazit_celou_zpravu);tzEx=Z->X;tzEy=Z->Y;tVID=Z->VID;Z=NULL;delete Z;}//pokud je zobrazen výpis zprávy, zazálohuje jeho ukazatel, kvůli znovuzobrazení
		if(Element==NULL){F->d.zobrazit_celou_zpravu=0;vymazat_ZPRAVY();}

		//průchod přes všechny elementy
		TElement *E=ELEMENTY;//nepřeskakovat raději hlavičku
		while(E!=NULL)
		{
			if(E->n!=0)//přeskočení hlavičky až zde
			{
				////výchozí hodnoty
				unsigned int pocet_voziku=E->data.pocet_voziku;
				double rotaceJ=vrat_rotaci_jigu_po_predchazejicim_elementu(E);
				double orientaceP=m.Rt90(E->geo.orientace-180);
				double X=F->d.Rxy(E).x;
				double Y=F->d.Rxy(E).y;
				//double dJ=PP.delka_jig;//později nahradit ze zakázky
				//double sJ=PP.sirka_jig;//později nahradit ze zakázky
				////určení směru vykreslování pozic
				short x=0,y=0;
				switch(m.Rt90(orientaceP))
				{
					case 0:   y=1;  x=0;  break;
					case 90:  y=0;  x=1;  break;
					case 180: y=-1; x=0;  break;
					case 270: y=0;  x=-1; break;
				}

				////testování jednotlivých problémů na elementech - řadit od nejdůležitějšího
				////////////Pohon nepřiřazen!
				if(funkcni_element(E) && E->pohon==NULL)
				{
					if(E->eID==300 || E->eID==301)vloz_zpravu(X+0.3,Y+0.3,-1,219,E);//výhybky a spojky mírné (nutné) odsazení od daného umístění
					else vloz_zpravu(X,Y,-1,219,E);//ostatní elementy přímo v daném umístění elementu
					pocet_erroru++;
				}
				////////////Rotace neodpovídá orientaci JIGů na začátku linky!
				if(E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180)
				{
					Cvektory::TElement *Ep=vrat_posledni_rotacni_element();
					if(Ep!=NULL)
					{
						double aR=m.a360(rotaceJ+Ep->rotace_jig);//výstupní rotace jigu z posledního rotačního elementu
						if(Ep->n==E->n &&  Ep->objekt_n==E->objekt_n && aR!=0 && aR!=180)//předposlení podmínka při novém DM zbytečná!
						{vloz_zpravu(X,Y,-1,401,Ep);pocet_erroru++;}
					}
					Ep=NULL;delete Ep;
				}
				////////////RT záporné nebo bez rezervy
				if(funkcni_element(E) && E->eID!=5  && E->eID!=100)//používáme (+testujeme) RT u všech funkčních elementů, mimo otoče a předávacího místaf
				{
					if(fabs(E->data.RT)>=1000000){vloz_zpravu(X,Y,-1,450,E);pocet_erroru++;}
					else
					{
						if(E->data.RT<0){vloz_zpravu(X,Y,-1,406,E);pocet_erroru++;}
						if(E->data.RT==0){vloz_zpravu(X,Y,1,407,E);pocet_warningu++;}
						//dodělat pokud RT==0 a aRD==0 vypsat, že není zadaná platná rychlost
					}
				}
				////////////RT záporné nebo bez rezervy na předávacích místech - NEDOKONALÉ DODĚLAT
				if(E->eID==200 && E->pohon!=NULL && E->dalsi!=NULL && E->dalsi->geo.delka>=E->dalsi->data.pocet_pozic*PP.delka_podvozek-PP.uchyt_pozice)//neřeší, pokud je předávací místo součástí bufferu, CHTĚLO BY ALE ZKONTROLOVAT, KDYBY PŘEDÁVACÍ MÍSTO NESTÍHALO ZÁSOBIT BUFFER
				{                                          //chtělo by nahradit přímo z rotace jigu z elementu
					if(E->pohon->Rx>0 && (PP.TT-E->WT)<m.UDV(vrat_rotaci_jigu_po_predchazejicim_elementu(E))/E->pohon->aRD
					&& !(E->dalsi!=NULL && E->dalsi->pohon!=NULL && vrat_druh_elementu(E->dalsi)==0 && E->dalsi->geo.delka<E->data.pocet_pozic*PP.delka_podvozek-PP.uchyt_pozice))//pro situace, kdy není předávací místo v bufferu (viz situace magna po chlazení)
					{vloz_zpravu(X,Y,-1,402,E);pocet_erroru++;}//pokud čekání na palec z kontinuálního pohonu bude větší nebo rovno času přejezdu
				}
				////////////Pozor, překrytí JIGů! - musí být umístěno na konci (popř. na začátku)
				if(PP.delka_podvozek<m.UDJ(rotaceJ) && pocet_voziku>1/*pocet_voziku>=1*/){vloz_zpravu(X+x*(PP.delka_podvozek*pocet_voziku/2.0-PP.uchyt_pozice),Y+y*(PP.delka_podvozek*pocet_voziku/2.0-PP.uchyt_pozice),-1,402,E);pocet_erroru++;}//pro buffer (výpis ve středu bufferu)
//				if(E->pohon!=NULL && m.UDV(rotaceJ)>m.Rz(E->pohon->aRD)){vloz_zpravu(X+x*PP.delka_podvozek/2.0,Y+y*PP.delka_podvozek/2.0,-1,403,E);pocet_erroru++;}//pro libovolný přejezd (výpis ve středu přejezdu)
				if(E->pohon!=NULL && m.UDV(rotaceJ)>m.Rz(E->pohon->aRD)){vloz_zpravu(X,Y,-1,403,E);pocet_erroru++;}//pro libovolný přejezd (výpis ve středu přejezdu)
			}
			////posun na další elementy
			E=E->dalsi;
		}
		delete E;E=NULL;

		//testování vozíků
		if(VOZIKY!=NULL)
		{
			TVozik *V=VOZIKY->dalsi;
			while(V!=NULL)
			{
				if(V->stav==-2)
				{
					if(PP.delka_podvozek<m.UDJ(V->rotace_jig)){vloz_zpravu(V->X,V->Y,-1,402,V->element);pocet_erroru++;}//případně ještě, //Pozor, překrytí JIGů!
					vloz_zpravu(V->X,V->Y,1,451,V->element);pocet_warningu++;//"Upozornění,v bufferu je vyšší počet vozíku, než je nastaveno."; -- MUSÍ BÝT AŽ DRUHÉ V POŘADÍ
				}
				V=V->dalsi;
			}
			delete V;
		}

		//zakutalizuje zprávy v miniformu zpráv
		Form_zpravy->update_zpravy(pocet_erroru,pocet_warningu);
		if(tVID>0)//pokud byl zobrazen popis zprávy,vyhledá se nové umístění případné zprávy (má jiné n i jiný ukazatel) a zobrazí se popis znovu
		{
			TZprava *Z=ZPRAVY->dalsi;
			while(Z!=NULL)
			{
				if(Z->VID==tVID && Z->X==tzEx && Z->Y==tzEx)//pokud se jedná o stejnou zprávu jako původní (před přegenerováním seznamu zpráv) se zobrazeným rozšiřujícím textem
				{
					F->d.zobrazit_celou_zpravu=Z->n;
				}
				Z=Z->dalsi;
			}
			Z=NULL; delete Z;
		}
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//z čísla VIDu vrátí jeho textový popis
UnicodeString Cvektory::getVID(long VID)
{
	UnicodeString Text="";
	switch(VID) //řadit od nejdůležitějšího
	{
		case 111: Text=F->ls->Strings[467];break;//"Počátek a konec linky byl automaticky spojen."
		case 219: Text=F->ls->Strings[219]+"!";break;//Pohon nepřiřazen!
		case 401: Text=F->ls->Strings[401];break;//Rotace neodpovídá orientaci JIGů na začátku linky!
		case 402: Text=F->ls->Strings[402];break;//Pozor, překrytí JIGů!
		case 403: Text=F->ls->Strings[468];break;//Pozor, na následujícím úseku přejezdu dochází k překrytí JIGů!
		case 406: Text=F->ls->Strings[406];break;//Nestíhá se přejezd, záporná časová rezerva!
		case 407: Text=F->ls->Strings[407];break;//Nulová časová rezerva.
		case 450: Text=F->ls->Strings[469];break;//Nerelevantní hodnota časové rezervy, na některém objektu není přiřazen pohon!
		case 451: Text="Upozornění,v bufferu je vyšší počet vozíku, než je nastaveno.";break;//přeložit
		default:  Text="Error or warning!";break;//obecná chyba či varování
	}
	return Text;
}
//---------------------------------------------------------------------------
//vytvoří hlavičku zprav
void Cvektory::hlavicka_ZPRAVY()
{
	//F->log(__func__);//logování  - NELZE

	//alokace paměti
	TZprava *nova=new TZprava;

	//atributy
	nova->n=0;
	nova->X=0;
	nova->Y=0;
	nova->zID=0;
	nova->VID=0;
	nova->VIDvalue=-1;
	nova->citelna_oblast=NULL;

	//ukazatelové záležitosti
	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;
	ZPRAVY=nova;
}
//---------------------------------------------------------------------------
//vloží jeden prvek na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_zpravu(TZprava *zprava)
{
	F->log(__func__);//logování

	//pokud ještě neexistuje hlavička
	if(ZPRAVY==NULL)hlavicka_ZPRAVY();

	//vložení nového bodu na konec seznamu bodů
	zprava->n=ZPRAVY->predchozi->n+1;//navýšení počítadla
	zprava->predchozi=ZPRAVY->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
	zprava->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
	ZPRAVY->predchozi->dalsi=zprava;//za poslední aktuální prvek vloží nový poslední
	ZPRAVY->predchozi=zprava;//hlavička ukazuje již na nový bod jako poslední prvek
}
//---------------------------------------------------------------------------
//vloží jeden prvek na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_zpravu(double X, double Y, short zID, int VID, TElement *Element,double VIDvalue)
{
	F->log(__func__);//logování

	//alokace paměti
	TZprava *Z=new TZprava;

	//atributy
	Z->X=X;
	Z->Y=Y;
	Z->Element=Element;
	Z->zID=zID;
	Z->VID=VID;
	Z->VIDvalue=VIDvalue;

	//samotné vložení do spojového seznamu ZPRAVY
	vloz_zpravu(Z);
}//---------------------------------------------------------------------------
//dle N (id) zprávy vrátí ukazatel na danou zprávu
Cvektory::TZprava *Cvektory::vrat_zpravu(unsigned long n)
{
	F->log(__func__);//logování

	if(n==0 || ZPRAVY==NULL)return NULL;
	else
	{
		Cvektory::TZprava *Z=ZPRAVY->dalsi;
		while(Z!=NULL)
		{
			if(Z->n==n)break;
			Z=Z->dalsi;
		}
		return Z;
	}
}

//---------------------------------------------------------------------------
//vrátí počet errorů a warningů
TPoint Cvektory::vrat_pocet_zprav()
{
	F->log(__func__);//logování

	TPoint RET=TPoint(0,0);
	if(ZPRAVY==NULL)return RET;
	else
	{
		Cvektory::TZprava *Z=ZPRAVY->dalsi;
		while(Z!=NULL)
		{
			switch(Z->zID)
			{
				case -1:RET.x++;break;//zvýšit počet errorů
				case 1: RET.y++;break;//zvýšit počet warningů
			}
			Z=Z->dalsi;
		}
		return RET;
	}
}
//---------------------------------------------------------------------------
//ověří, zda se na daných souřadních myši nachází nějaká (libovolná) ze zpráv
long Cvektory::PtInZpravy()
{
	long RET=0;
	if(ZPRAVY!=NULL)
	{
		Cvektory::TZprava *Z=ZPRAVY->dalsi;
		while(Z!=NULL)
		{                                                                    //*3 kvůli AA
			if(PtInRegion(Z->citelna_oblast,F->akt_souradnice_kurzoru_PX.x*3,F->akt_souradnice_kurzoru_PX.y*3)  || m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,Z->X,Z->Y,m.px2m(m.round(3*F->Zoom)))){RET=Z->n;break;}
			Z=Z->dalsi;
		}
		Z=NULL;delete Z;
	}
	return RET;
}
//---------------------------------------------------------------------------
//vše odstraní včetně hlavičky
void Cvektory::vymazat_ZPRAVY()
{
	F->log(__func__);//logování
	while (ZPRAVY!=NULL)
	{
		delete ZPRAVY->predchozi;
		ZPRAVY->predchozi=NULL;
		ZPRAVY=ZPRAVY->dalsi;
	};
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PRÁCE ZE SOUBORY
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zapis hlavičky souboru
void Cvektory::vytvor_hlavicku_souboru()
{
  File_hlavicka.FileVersion=Form1->ms.MyToDouble(Form1->FileVersion);
	File_hlavicka.ProductVersion=Form1->ms.MyToDouble(Form1->ProductVersion);
	File_hlavicka.Mod=Form1->MOD;
	File_hlavicka.Zoom=Form1->Zoom;
	File_hlavicka.PosunutiX=Form1->Posun.x;
	File_hlavicka.PosunutiY=Form1->Posun.y;
	File_hlavicka.cas_start=PP.cas_start;
	File_hlavicka.vytvoril_Sdelka=PP.vytvoril.Length()+1;
	File_hlavicka.cas_posledni_upravy=PP.cas_posledni_upravy;
	File_hlavicka.upravil_Sdelka=PP.upravil.Length()+1;
	File_hlavicka.mnozstvi=PP.mnozstvi;
	File_hlavicka.hod_den=PP.hod_den;
	File_hlavicka.dni_rok=PP.dni_rok;
	File_hlavicka.efektivita=PP.efektivita;
	File_hlavicka.TT=PP.TT;
	File_hlavicka.typ_linky=PP.typ_linky;
	File_hlavicka.delka_jig=PP.delka_jig;
	File_hlavicka.sirka_jig=PP.sirka_jig;
	File_hlavicka.vyska_jig=PP.vyska_jig;
	File_hlavicka.delka_podvozek=PP.delka_podvozek;
	File_hlavicka.uchyt_pozice=PP.uchyt_pozice;
	File_hlavicka.zamek_layoutu=PP.zamek_layoutu;
	//objektové záležitosti
	File_hlavicka.pocet_pohonu=POHONY->predchozi->n;
	File_hlavicka.pocet_objektu=OBJEKTY->predchozi->n;

	//nutnost průchodu, skrze výhybky nelze určít počet elementů z posledního elementu
	TElement *E=ELEMENTY->dalsi;
	TPoint *tab_pruchodu=new TPoint[pocet_vyhybek];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
	File_hlavicka.pocet_elementu=0;
	while(E!=NULL)
	{
		File_hlavicka.pocet_elementu++;
		E=sekvencni_zapis_cteni(E,tab_pruchodu,NULL);//použití sekvenčního algoritmu, podle stejného bude soubor načítán, tz. stejný počet elementů v sekvenčním řazení
	}
	delete E;E=NULL;
	delete []tab_pruchodu;

	File_hlavicka.pocet_zakazek=ZAKAZKY->predchozi->n;
	File_hlavicka.pocet_voziku=VOZIKY->predchozi->n;
	File_hlavicka.pocet_vyhybek=pocet_vyhybek;
	//katalogové záležitosti
	File_hlavicka.katalog=PP.katalog;
	File_hlavicka.typ_linky=PP.typ_linky;
	File_hlavicka.radius=PP.radius;
	//stav ikony  TODO ROSTA dodelat
	//File_hlavicka.objekt_posunout_vse
	if(ZAKAZKA_akt!=NULL)File_hlavicka.zakazka_akt=ZAKAZKA_akt->n;
	else File_hlavicka.zakazka_akt=0;
}
//---------------------------------------------------------------------------
//Uloží vektorová data do souboru
short int Cvektory::uloz_do_souboru(UnicodeString FileName)
{
	try
	{
		//vytvoření streamu pro zápis do souboru
		TFileStream *FileStream=new TFileStream(FileName,fmOpenWrite|fmCreate);

		//zapiše hlavičku do souboru //už neplatí:+ zbylé atributy a PP se do hlavičky zapisují v unit1
		vytvor_hlavicku_souboru();
		FileStream->Write(&File_hlavicka,sizeof(TFile_hlavicka));
    //uložení autorů
		wchar_t *autor=new wchar_t [File_hlavicka.vytvoril_Sdelka];
		autor=PP.vytvoril.c_str();
		FileStream->Write(autor,File_hlavicka.vytvoril_Sdelka*sizeof(wchar_t));//
		autor=NULL; delete[] autor;
		autor=new wchar_t [File_hlavicka.upravil_Sdelka];
		autor=PP.upravil.c_str();
		FileStream->Write(autor,File_hlavicka.upravil_Sdelka*sizeof(wchar_t));//
		autor=NULL; delete[] autor;

		//uložení parametrů RASTRU
		C_raster *R=new C_raster;
		R->text_length=PP.raster.filename.Length()+1;
		R->resolution=PP.raster.resolution;
		R->X=PP.raster.X;
		R->Y=PP.raster.Y;
		R->show=PP.raster.show;
		R->grayscale=PP.raster.grayscale;
		R->dim=PP.raster.dim;
		FileStream->Write(R,sizeof(C_raster));//zapiše jeden prvek do souboru
		//text - adresa rastru
		wchar_t *name=new wchar_t[R->text_length];
		name=PP.raster.filename.c_str();
		FileStream->Write(name,R->text_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
		name=NULL; delete[] name;
		R=NULL;delete R;

		//uložení POHONŮ
		TPohon *ukaz1=POHONY->dalsi;
		while (ukaz1!=NULL)
		{
			////překopírování dat do pomocného objektu uložitelného do bináru
			C_pohon *c_ukaz1=new C_pohon;
			// ShowMessage("uloz pohon n"+AnsiString(ukaz1->n));
			//samotná data
			c_ukaz1->n=ukaz1->n;
			c_ukaz1->text_length=ukaz1->name.Length()+1;
			c_ukaz1->rychlost_od=ukaz1->rychlost_od;
			c_ukaz1->rychlost_do=ukaz1->rychlost_do;
			c_ukaz1->aRD=ukaz1->aRD;
			c_ukaz1->roztec=ukaz1->roztec;
			c_ukaz1->roztec_ID=ukaz1->roztec_ID;
			c_ukaz1->Rz=ukaz1->Rz;
			c_ukaz1->Rx=ukaz1->Rx;
			FileStream->Write(c_ukaz1,sizeof(C_pohon));//zapiše jeden prvek do souboru
			//text - name
			wchar_t *name=new wchar_t [c_ukaz1->text_length];
			name=ukaz1->name.c_str();
			FileStream->Write(name,c_ukaz1->text_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
			name=NULL; delete[] name;

			c_ukaz1=NULL;delete c_ukaz1;
			ukaz1=ukaz1->dalsi;//posunutí na další pozici v seznamu
		};
		ukaz1=NULL;delete ukaz1;

		////OBJEKTů
		TObjekt *ukaz=OBJEKTY->dalsi;
		while (ukaz!=NULL)
		{
			////překopírování dat do pomocného objektu uložitelného do bináru
			C_objekt *c_ukaz=new C_objekt;

			if(ukaz->n>0 && File_hlavicka.pocet_objektu>=ukaz->n)//mimo hlavičky či shitu
			{
				//samotná data
				c_ukaz->n=ukaz->n;
				c_ukaz->id=ukaz->id;
				c_ukaz->X=ukaz->X;
				c_ukaz->Y=ukaz->Y;
				c_ukaz->Xt=ukaz->Xt;
				c_ukaz->Yt=ukaz->Yt;
				c_ukaz->Xp=ukaz->Xp;
				c_ukaz->Yp=ukaz->Yp;
				c_ukaz->orientace_text=ukaz->orientace_text;
				c_ukaz->sirka_steny=ukaz->sirka_steny;
				c_ukaz->orientace=ukaz->orientace;
				c_ukaz->rezim=ukaz->rezim;
        c_ukaz->stavPM=ukaz->stavPM;
				if(ukaz->body!=NULL)c_ukaz->pocet_bodu=ukaz->body->predchozi->n;
				else c_ukaz->pocet_bodu=0;
				if(ukaz->element!=NULL) c_ukaz->element_n=ukaz->element->n;
				else c_ukaz->element_n=0;
				if(ukaz->komora!=NULL) c_ukaz->pocet_komor=ukaz->komora->predchozi->n;
				else c_ukaz->pocet_komor=0;
				if(ukaz->teplomery!=NULL) c_ukaz->pocet_teplomeru=ukaz->teplomery->predchozi->n;
				else c_ukaz->pocet_teplomeru=0;
				//c_ukaz->rotace=ukaz->rotace;
				if(ukaz->pohon!=NULL)c_ukaz->pohon=ukaz->pohon->n;
				else c_ukaz->pohon=0;
				c_ukaz->koty_elementu_offset=ukaz->koty_elementu_offset;
				c_ukaz->zobrazit_koty=ukaz->zobrazit_koty;
				c_ukaz->zobrazit_mGrid=ukaz->zobrazit_mGrid;
				c_ukaz->uzamknout_nahled=ukaz->uzamknout_nahled;
				c_ukaz->text_length=ukaz->name.Length()+1;
				FileStream->Write(c_ukaz,sizeof(C_objekt));//zapiše jeden prvek do souboru
				//text - short name
				wchar_t *short_name=new wchar_t[5];//max 4 znaky
				short_name=ukaz->short_name.c_str();
				FileStream->Write(short_name,5*sizeof(wchar_t));//zapiše jeden řetězec za prvek bod
				short_name=NULL; delete[] short_name;
				//text - name
				wchar_t *name=new wchar_t [c_ukaz->text_length];
				name=ukaz->name.c_str();
				FileStream->Write(name,c_ukaz->text_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
				name=NULL; delete[] name;
				//zapis bodu objektu
				if(c_ukaz->n>0 && c_ukaz->pocet_bodu>0)
				{
					TBod *B=ukaz->body->dalsi;
					while(B!=NULL)
					{
						//překopírování dat do pomocného objektu uložitelného do bináru
						C_bod *cB=new C_bod;
						//plněný - kopírování dat jednotlivých atributů
						cB->n=B->n;
						cB->X=B->X;
						cB->Y=B->Y;
						cB->kota_offset=B->kota_offset;
						FileStream->Write(cB,sizeof(C_bod));//zapiše jeden prvek do souboru
						//posun na další bod
						B=B->dalsi;
						delete cB; cB=NULL;
					}
					delete B; B=NULL;
				}
				//zapis komor do objektu
				if(c_ukaz->n>0 && c_ukaz->pocet_komor>0)
				{
					TKomora *K=ukaz->komora->dalsi;
					while(K!=NULL)
					{
						//překopírování dat do pomocného objektu uložitelného do bináru
						 C_komora *cK=new C_komora;
						//plněný - kopírování dat jednotlivých atributů
						cK->n=K->n;
						cK->velikost=K->velikost;
						cK->typ=K->typ;
						FileStream->Write(cK,sizeof(C_komora));//zapiše jeden prvek do souboru
						//posun na další bod
						K=K->dalsi;
						delete cK; cK=NULL;
					}
					delete K; K=NULL;
				}
				//zápis teploměrů do objektu
				if(c_ukaz->n>0 && c_ukaz->pocet_teplomeru>0)
				{
					TTeplomery *T=ukaz->teplomery->dalsi;
					C_teplomery *cT=NULL;
					C_element *cE=NULL;
          TCesta *C=NULL;
					C_cesta *cC=NULL;
					while(T!=NULL)
					{
						//zapsání segmentu teploměry
						cT=new C_teplomery;
						cT->n=T->n;
						cT->Z_n=T->Z_n;
						cT->pocet_sehmentu_cesty=T->cesta->predchozi->n;
						FileStream->Write(cT,sizeof(C_teplomery));//zapiše jeden prvek do souboru
						//zapsání elementu první teploměr
						cE=new C_element;
						cE->n=T->prvni->n;
						cE->eID=T->prvni->eID;
						cE->name_delka=T->prvni->name.Length()+1;
						cE->X=T->prvni->X;
						cE->Y=T->prvni->Y;
            cE->Z=T->prvni->Z;
						cE->Xt=T->prvni->Xt;
						cE->Yt=T->prvni->Yt;
						cE->orientace=T->prvni->orientace;
						cE->objekt_n=T->prvni->objekt_n;
						cE->sparovany_n=T->prvni->sparovany->n;
						cE->geo=T->prvni->geo;
						FileStream->Write(cE,sizeof(C_element));//zapiše jeden prvek do souboru
						wchar_t *name=new wchar_t [cE->name_delka];
						name=T->prvni->name.c_str();
						FileStream->Write(name,cE->name_delka*sizeof(wchar_t));
						name=NULL; delete[] name;
						delete cE;cE=NULL;
						//zapsání elementu druhý teploměr
						cE=new C_element;
						cE->n=T->posledni->n;
						cE->eID=T->posledni->eID;
						cE->name_delka=T->posledni->name.Length()+1;
						cE->X=T->posledni->X;
						cE->Y=T->posledni->Y;
            cE->Z=T->posledni->Z;
						cE->Xt=T->posledni->Xt;
						cE->Yt=T->posledni->Yt;
						cE->orientace=T->posledni->orientace;
						cE->objekt_n=T->posledni->objekt_n;
						cE->sparovany_n=T->posledni->sparovany->n;
						cE->geo=T->posledni->geo;
						FileStream->Write(cE,sizeof(C_element));//zapiše jeden prvek do souboru
						name=new wchar_t [cE->name_delka];
						name=T->posledni->name.c_str();
						FileStream->Write(name,cE->name_delka*sizeof(wchar_t));
						name=NULL; delete[] name;
						//zapsání cesty
						if(cT->pocet_sehmentu_cesty>0)
						{
							C=T->cesta->dalsi;
							while(C!=NULL)
							{
								cC=new C_cesta;
								cC->n=C->n;
								cC->n_element=C->Element->n;
								if(C->sparovany!=NULL)cC->n_sparovany=C->sparovany->n;
								else cC->n_sparovany=0;
								cC->data=C->data;
								FileStream->Write(cC,sizeof(C_cesta));//zapiše jeden prvek do souboru
                //ukazatelové záležitosti
								delete cC;cC=NULL;
								C=C->dalsi;
							}
							delete C;C=NULL;
            }
            //ukazatelové záležitosti
						T=T->dalsi;
						delete cT;cT=NULL;
						delete cE;cE=NULL;
						delete cC;cC=NULL;
					}
          //ukazatelové záležitosti
					delete T;T=NULL;
				}
			}
			c_ukaz=NULL;delete c_ukaz;
			ukaz=ukaz->dalsi;//posunutí na další pozici v seznamu
		};
		ukaz=NULL;delete ukaz;

		////ELEMENTY
		TPoint *tab_pruchodu=new TPoint[pocet_vyhybek];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
		TElement *E=ELEMENTY->dalsi;
		while(E!=NULL)
		{
			//překopírování dat do pomocného objektu uložitelného do bináru
			C_element *cE=new C_element;
			if(E->n>0)// && File_hlavicka.pocet_elementu>=E->n)
			{
				//plněný - kopírování dat jednotlivých atributů
				cE->n=E->n;
				//  ShowMessage("uloz element n"+AnsiString(E->n));
				cE->eID=E->eID;
				cE->identifikator_vyhybka_spojka=E->identifikator_vyhybka_spojka;
				cE->name_delka=E->name.Length()+1;
				cE->X=E->X;
				cE->Y=E->Y;
        cE->Z=E->Z;
				cE->Xt=E->Xt;
				cE->Yt=E->Yt;
				cE->orientace=E->orientace;
				cE->rotace_jig=E->rotace_jig;
				cE->stav=E->stav;
				cE->OTOC_delka=E->OTOC_delka;
				cE->zona_pred=E->zona_pred;
				cE->zona_za=E->zona_za;
				cE->PTotoc=E->PTotoc;
				cE->WT=E->WT;
				cE->data=E->data;
				cE->objekt_n=E->objekt_n;
				//  ShowMessage("E->pohony->n");
				if(E->pohon==NULL) cE->pohon_n=0;
				else cE->pohon_n=E->pohon->n;
				cE->geo=E->geo;
				//uložení do binárního filu
				FileStream->Write(cE,sizeof(C_element));//zapiše jeden prvek do souboru

				//text - short_name
				wchar_t *short_name=new wchar_t[5];
				short_name=E->short_name.c_str();
				FileStream->Write(short_name,5*sizeof(wchar_t));// fixni pocet pozic
				short_name=NULL; delete[] short_name;

				//text - name
				wchar_t *name=new wchar_t [cE->name_delka];
				name=E->name.c_str();
				FileStream->Write(name,cE->name_delka*sizeof(wchar_t));//
				//   ShowMessage(AnsiString(name)+" uloz pohon n:"+AnsiString(cE->pohon_n));
				name=NULL; delete[] name;
			}
			//posun na další segment cesty
			E=sekvencni_zapis_cteni(E,tab_pruchodu,NULL);
			delete cE; cE=NULL;
		}
		delete []tab_pruchodu;

		////uložení HALy do_souboru, překopírování dat do pomocného objektu uložitelného do bináru
		//alokace
		C_hala *cH=new C_hala;
		//samotná data
		cH->X=HALA.X;
		cH->Y=HALA.Y;
		if(HALA.body!=NULL)cH->pocet_bodu=HALA.body->predchozi->n;
		else cH->pocet_bodu=0;
		cH->name_delka=HALA.name.Length()+1;
		FileStream->Write(cH,sizeof(C_hala));//zapiše jeden prvek do souboru
		//text - name
		wchar_t *name_hala=new wchar_t [cH->name_delka];
		name_hala=HALA.name.c_str();
		FileStream->Write(name_hala,cH->name_delka*sizeof(wchar_t));//zápis názvu do souboru
		name_hala=NULL; delete[] name_hala;
		//odstranění ukazatele na Halu, již není třeba
		delete cH; cH=NULL;
		//zápis jednotlivých bodů
		if(HALA.body!=NULL)
		{
			TBod *B=HALA.body->dalsi;//přeskočí hlavičku
			while(B!=NULL)
			{
				//překopírování dat do pomocného objektu uložitelného do bináru
				C_bod *cB=new C_bod;
				//plněný - kopírování dat jednotlivých atributů
				cB->n=B->n;
				cB->X=B->X;
				cB->Y=B->Y;
				cB->kota_offset=B->kota_offset;
				FileStream->Write(cB,sizeof(C_bod));//zapiše jeden prvek do souboru
				delete cB; cB=NULL;
				//posun na další bod
				B=B->dalsi;
			}
			delete B; B=NULL;
		}

		////uložení ZAKÁZEK
		TZakazka *ukaz2=ZAKAZKY->dalsi;
		while (ukaz2!=NULL)
		{
			//překopírování dat do pomocného objektu uložitelného do bináru
			C_zakazka *c_ukaz2=new C_zakazka;

			//samotná data
			c_ukaz2->n=ukaz2->n;
			c_ukaz2->id_length=ukaz2->id.Length()+1;
			c_ukaz2->typ=ukaz2->typ;
			c_ukaz2->name_length=ukaz2->name.Length()+1;
			c_ukaz2->barva=ukaz2->barva;
			c_ukaz2->pomer=ukaz2->pomer;
			c_ukaz2->TT=ukaz2->TT;
			c_ukaz2->jig=ukaz2->jig;
			c_ukaz2->pocet_segmentu_cesty=0;
			if(ukaz2->cesta!=NULL) if(ukaz2->cesta->predchozi->n>=0) //raději na dva if
			c_ukaz2->pocet_segmentu_cesty=ukaz2->cesta->predchozi->n;
			c_ukaz2->pocet_davek=ukaz2->davky->predchozi->n;
			//ShowMessage("Cvectory 1014: "+AnsiString(c_ukaz2->pocet_segmentu_cesty));
			c_ukaz2->pocet_voziku=ukaz2->pocet_voziku;
			c_ukaz2->serv_vozik_pocet=ukaz2->serv_vozik_pocet;
			c_ukaz2->opakov_servis=ukaz2->opakov_servis;
			FileStream->Write(c_ukaz2,sizeof(C_zakazka));//zapiše jeden prvek do souboru
			//text - id
			wchar_t *id=new wchar_t [c_ukaz2->id_length];
			id=ukaz2->id.c_str();
			FileStream->Write(id,c_ukaz2->id_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
			id=NULL; delete[] id;
			//text - name
			wchar_t *name=new wchar_t [c_ukaz2->name_length];
			name=ukaz2->name.c_str();
			FileStream->Write(name,c_ukaz2->name_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
			name=NULL; delete[] name;
			//zápis cesty resp. jednotlivých segmentů
			if(c_ukaz2->pocet_segmentu_cesty>0)
			{
				TCesta *c=ukaz2->cesta->dalsi;//ukazatel na cestu dané zakázky, přeskočí hlavičku
				while(c!=NULL)
				{
					//překopírování dat do pomocného objektu uložitelného do bináru
					C_cesta *c_c=new C_cesta;
					//plněný - kopírování dat
					c_c->n=c->n;
					c_c->n_element=c->Element->n;
					c_c->n_sparovany=0;
					if(c->sparovany!=NULL)c_c->n_sparovany=c->sparovany->n;
					c_c->data=c->data;
					//uložení do binárního filu
					FileStream->Write(c_c,sizeof(C_cesta));//zapiše jeden prvek do souboru
					//posun na další segment cesty
					c=c->dalsi;
					c_c=NULL; delete c_c;
				}
			}
			//zápis dávek
			if(c_ukaz2->pocet_davek>0)
			{
				TDavka *d=ukaz2->davky->dalsi;//ukazatel na cestu dané zakázky, přeskočí hlavičku
				while(d!=NULL)
				{
					//překopírování dat do pomocného objektu uložitelného do bináru
					C_davka *c_d=new C_davka;
					//plněný - kopírování dat
					c_d->n=d->n;
					c_d->pocet_voziku=d->pocet_voziku;
					c_d->pocet_prazdnych=d->pocet_prazdnych;
					c_d->pocet_celkem=d->pocet_celkem;
					//uložení do binárního filu
					FileStream->Write(c_d,sizeof(C_davka));//zapiše jeden prvek do souboru
					//posun na další segment cesty
					d=d->dalsi;
					c_d=NULL; delete c_d;
				}
			}
			//c=NULL; delete c;
			c_ukaz2=NULL;delete c_ukaz2;
			ukaz2=ukaz2->dalsi;//posunutí na další pozici v seznamu
		};
		ukaz2=NULL;delete ukaz2;

		delete FileStream;
		return 1;
	}
	catch(...){return 2;}
}
//---------------------------------------------------------------------------
//načte vektorová data ze souboru
short int Cvektory::nacti_ze_souboru(UnicodeString FileName)
{
	if(!FileExists(FileName)){return 0;}
	else
	{
		try
		{
		 // ShowMessage("nacitam data");
			TFileStream *FileStream=new TFileStream(FileName,fmOpenRead);

			//načte hlavičku ze souboru
			FileStream->Read(&File_hlavicka,sizeof(TFile_hlavicka));//načte hlavičku ze souboru

			//kontrola, zda se shoduje verze projektu a verze souboru, pokud ne vyhodí chybovou hlášku
			if(File_hlavicka.FileVersion!=F->ms.MyToDouble(F->FileVersion))throw new Exception("Verze souboru a projektu se neshoduje");//std::invalid_argument("Verze souboru a projektu se neshoduje");
			//načtení autorů
			wchar_t *autor=new wchar_t[File_hlavicka.vytvoril_Sdelka];
			FileStream->Read(autor,File_hlavicka.vytvoril_Sdelka*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
			PP.vytvoril=autor;
			autor=NULL; delete[] autor;
			autor=new wchar_t[File_hlavicka.upravil_Sdelka];
			FileStream->Read(autor,File_hlavicka.upravil_Sdelka*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
			PP.upravil=autor;
			autor=NULL; delete[] autor;

			//uložení parametrů RASTRu
			C_raster *R=new C_raster;
			FileStream->Read(R,sizeof(C_raster));//načte jeden prvek ze souboru
			PP.raster.resolution=R->resolution;
			PP.raster.X=R->X;
			PP.raster.Y=R->Y;
			PP.raster.show=R->show;
			PP.raster.grayscale=R->grayscale;
			PP.raster.dim=R->dim;
			//adresa rastru
			wchar_t *name=new wchar_t[R->text_length];
			FileStream->Read(name,R->text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
			PP.raster.filename=name;
			name=NULL; delete[] name;
			R=NULL;delete R;

			//vytvoří nové hlavičky pro spojové seznamy
			hlavicka_POHONY();
			hlavicka_OBJEKTY();
			hlavicka_ELEMENTY();

			//pohony - musí být nad objekty!
			for(unsigned int i=1;i<=File_hlavicka.pocet_pohonu;i++)//možno řešit sice while, po strukturách, ale toto je připravené pro případ, kdy budu načítat i objekty jiného typu než objekt
			{
				TPohon *ukaz1=new TPohon;
				C_pohon *c_ukaz1=new C_pohon;
				FileStream->Read(c_ukaz1,sizeof(C_pohon));//načte jeden prvek ze souboru
				if(c_ukaz1->n!=0 && File_hlavicka.pocet_pohonu>=c_ukaz1->n)//pokud nenačte hlavičku či nějaký shit
				{
					//samotná data
					ukaz1->n=c_ukaz1->n;
					ukaz1->rychlost_od=c_ukaz1->rychlost_od;
					ukaz1->rychlost_do=c_ukaz1->rychlost_do;
					ukaz1->aRD=c_ukaz1->aRD;
					ukaz1->roztec=c_ukaz1->roztec;
					ukaz1->roztec_ID=c_ukaz1->roztec_ID;
					ukaz1->Rz=c_ukaz1->Rz;
					ukaz1->Rx=c_ukaz1->Rx;
          ukaz1->rezim=-1;//nenastaveno, následně dojde k aktualizaci
					ukaz1->retez=NULL;//možná prozatím jinak načítat buď z přidruženého spojáku nebo volat metodu po načtení elementů
					ukaz1->palec=NULL;//možná prozatím jinak načítat buď z přidruženého spojáku nebo volat metodu po načtení elementů

					//popisek
					wchar_t *name=new wchar_t[c_ukaz1->text_length];
					FileStream->Read(name,c_ukaz1->text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
					ukaz1->name=name;
					name=NULL; delete[] name;

					//vloží finální prvek do spojového seznamu
					vloz_pohon(ukaz1);
				}
				ukaz1=NULL; delete ukaz1;
				c_ukaz1=NULL; delete c_ukaz1;
			};

			////OBJEKTY
			for(unsigned int i=1;i<=File_hlavicka.pocet_objektu;i++)//možno řešit sice while, po strukturách, ale toto je připravené pro případ, kdy budu načítat i objekty jiného typu než objekt
			{
				TObjekt *ukaz=new TObjekt;
				C_objekt *c_ukaz=new C_objekt;
				FileStream->Read(c_ukaz,sizeof(C_objekt));//načte jeden prvek ze souboru
				if(c_ukaz->n!=0 && File_hlavicka.pocet_objektu>=c_ukaz->n)//pokud nenačte hlavičku či nějaký shit
				{
			//    ShowMessage("n objektu: "+AnsiString(c_ukaz->n));
					//samotná data
					ukaz->n=c_ukaz->n;
					ukaz->id=c_ukaz->id;
					ukaz->X=c_ukaz->X;
					ukaz->Y=c_ukaz->Y;
          ukaz->Xt=c_ukaz->Xt;
					ukaz->Yt=c_ukaz->Yt;
					ukaz->Xp=c_ukaz->Xp;
					ukaz->Yp=c_ukaz->Yp;
					ukaz->orientace_text=c_ukaz->orientace_text;
					ukaz->sirka_steny=c_ukaz->sirka_steny;
          ukaz->body=NULL;  //NUTNOST PRO AUTO VYTVARENI HLAVICKY
					ukaz->rezim=c_ukaz->rezim;
					ukaz->stavPM=c_ukaz->stavPM;
					ukaz->orientace=c_ukaz->orientace;
					ukaz->pohon=vrat_pohon(c_ukaz->pohon);
					ukaz->koty_elementu_offset=c_ukaz->koty_elementu_offset;
					ukaz->element=NULL;
					ukaz->element_n=c_ukaz->element_n;
					ukaz->komora=NULL;    //NUTNOST PRO AUTO VYTVARENI HLAVICKY
					ukaz->teplomery=NULL;
					ukaz->zobrazit_koty=c_ukaz->zobrazit_koty;
					ukaz->zobrazit_mGrid=c_ukaz->zobrazit_mGrid;
					ukaz->uzamknout_nahled=c_ukaz->uzamknout_nahled;
					ukaz->probehla_aktualizace_prirazeni_pohonu=false;//neukládá se do binárky
					//zkratku
					wchar_t *short_name=new wchar_t [5];
					FileStream->Read(short_name,5*sizeof(wchar_t));//načte popisek umístěný za strukturou bod
					ukaz->short_name=short_name;
					short_name=NULL; delete[] short_name;
					//popisek
					wchar_t *name=new wchar_t[c_ukaz->text_length];
					FileStream->Read(name,c_ukaz->text_length*sizeof(wchar_t));
					ukaz->name=name;
					name=NULL; delete[] name;

					//přiřazení bodů objektu
					for(unsigned int j=1;j<=c_ukaz->pocet_bodu;j++)
					{
						C_bod cB;//=new C_cesta;
						FileStream->Read(&cB,sizeof(C_bod));//načte jeden prvek ze souboru
						TBod *B=new TBod;
						B->n=cB.n;
						B->kota_offset=cB.kota_offset;
						B->X=cB.X;
						B->Y=cB.Y;
						vloz_bod(B,ukaz);
					 //delete B; B=NULL; nesmí být
					}

					//přiřazení komora
					for(unsigned int j=1;j<=c_ukaz->pocet_komor;j++)
					{
						C_komora cK;//=new C_cesta;
						FileStream->Read(&cK,sizeof(C_komora));//načte jeden prvek ze souboru
						TKomora *K=new TKomora;
						K->n=cK.n;
						K->velikost=cK.velikost;
						K->typ=cK.typ;
						vloz_komoru(ukaz,K,NULL,K->typ);
						//delete K; K=NULL; - nesmí být
					}

          //přiřazení teploměrů
					for(unsigned int j=1;j<=c_ukaz->pocet_teplomeru;j++)
					{
			  		//načtení záznamu teploměrů
			  		C_teplomery cT;
			  		FileStream->Read(&cT,sizeof(C_teplomery));//načte jeden prvek ze souboru
						TTeplomery *T=new TTeplomery;
			  		T->n=cT.n;
						T->Z_n=cT.Z_n;
            T->cesta=NULL;
						hlavicka_cesty_teplomery(T);//vytvoří hlavičku pro cesty
						//načtení prvního teplomeru
						TElement *E_pom=new TElement;//slouží pro uchování n elementu, který ještě neexistuje
						C_element *cE=new C_element;
						TElement *E=new TElement;
						FileStream->Read(cE,sizeof(C_element));//načte jeden prvek ze souboru
						E->n=cE->n;
						E->eID=cE->eID;
						E->X=cE->X;
						E->Y=cE->Y;
						E->Z=cE->Z;
						E->Xt=cE->Xt;
						E->Yt=cE->Yt;
						E->orientace=cE->orientace;
						E->objekt_n=cE->objekt_n;
						E_pom->n=cE->sparovany_n;
						E->sparovany=E_pom;
						E->mGrid=NULL;
            E->pohon=NULL;
						E_pom=NULL;delete E_pom;
						E->geo=cE->geo;
						wchar_t *name=new wchar_t[cE->name_delka];
						FileStream->Read(name,cE->name_delka*sizeof(wchar_t));
						E->name=name;
						delete[] name; name=NULL;
						T->prvni=E;
						E=NULL;delete E;
						delete cE;cE=NULL;
						//načtení posledního teplomeru
						cE=new C_element;
						E=new TElement;
						FileStream->Read(cE,sizeof(C_element));//načte jeden prvek ze souboru
						E->n=cE->n;
						E->eID=cE->eID;
						E->X=cE->X;
						E->Y=cE->Y;
            E->Z=cE->Z;
						E->Xt=cE->Xt;
						E->Yt=cE->Yt;
						E->orientace=cE->orientace;
						E->objekt_n=cE->objekt_n;
						E_pom=new TElement;
						E_pom->n=cE->sparovany_n;
						E->sparovany=E_pom;
						E->mGrid=NULL;
						E->pohon=NULL;
						E_pom=NULL;delete E_pom;
						E->geo=cE->geo;
			  		name=new wchar_t[cE->name_delka];
			  		FileStream->Read(name,cE->name_delka*sizeof(wchar_t));
						E->name=name;
						delete[] name; name=NULL;
						T->posledni=E;
            E=NULL;delete E;
			  		//načtení cesty teploměrů
						for(unsigned int i=1;i<=cT.pocet_sehmentu_cesty;i++)
						{
							C_cesta cC;
							FileStream->Read(&cC,sizeof(C_cesta));//načte jeden prvek ze souboru
							E_pom=new TElement;
							E_pom->n=cC.n_element;
							vloz_segment_cesty_do_seznamu_cesty(T,E_pom);
							E_pom=NULL;delete E_pom;
						}
						//zařezení záznamu do objektu
						if(ukaz->teplomery==NULL)hlavicka_teplomery(ukaz);
						ukaz->teplomery->predchozi->dalsi=T;
						T->predchozi=ukaz->teplomery->predchozi;
						T->dalsi=NULL;
						ukaz->teplomery->predchozi=T;
            T=NULL;delete T;
					}

					//vložení finálního objektu do spojáku
					vloz_objekt(ukaz);
				}
				ukaz=NULL; delete ukaz;
				c_ukaz=NULL; delete c_ukaz;
			};
			////ELEMENTY
			pocet_vyhybek=File_hlavicka.pocet_vyhybek;//musí být předáván, v tuto chvíli je počet pouze v hlavičce, je potřeba pro vytvoření tab průchodů
			T2Element *tab_pruchodu=vytvor_tabElm_pruchodu();
			for(unsigned int j=1;j<=File_hlavicka.pocet_elementu;j++)
			{
				TElement *E=new TElement;
				C_element *cE=new C_element;
				FileStream->Read(cE,sizeof(C_element));//načte jeden prvek ze souboru
				if(cE->n!=0 && File_hlavicka.pocet_elementu>=cE->n)//pokud nenačte hlavičku či nějaký shit
				{
					E->n=cE->n;
					//ShowMessage("element n:"+AnsiString(cE.n));
					E->eID=cE->eID;
					E->identifikator_vyhybka_spojka=cE->identifikator_vyhybka_spojka;
					E->X=cE->X;
					E->Y=cE->Y;
					E->Z=cE->Z;
					E->Xt=cE->Xt;
					E->Yt=cE->Yt;
					E->orientace=cE->orientace;
					E->rotace_jig=cE->rotace_jig;
					E->stav=1;//cE->stav;
					E->OTOC_delka=cE->OTOC_delka;
					E->zona_pred=cE->zona_pred;
					E->zona_za=cE->zona_za;
					E->PTotoc=cE->PTotoc;
					E->WT=cE->WT;
					E->data=cE->data;
					E->objekt_n=cE->objekt_n;
					E->pohon=vrat_pohon(cE->pohon_n);
					E->geo=cE->geo;
					E->sparovany=NULL;
					//shortname
					wchar_t *short_name=new wchar_t [5];
					FileStream->Read(short_name,5*sizeof(wchar_t));//načte popisek umístěný za strukturou bod
					E->short_name=short_name;
					delete[] short_name; short_name=NULL;

					 //name
					wchar_t *name=new wchar_t[cE->name_delka];
					FileStream->Read(name,cE->name_delka*sizeof(wchar_t));
					E->name=name;
					//ShowMessage(AnsiString(name)+", nacti pohon n:"+AnsiString(cE.pohon_n));
					delete[] name; name=NULL;

					//stav
					E->stav=1;
					if(E->eID==0 && E->data.pocet_voziku==0)E->stav=0;//pokud se jedná o průjezdnou stop stanici nastavit korektní stav

					sekvencni_zapis_cteni(E,NULL,tab_pruchodu);//vloží element do spojáku elementů, hlavní vedlejší větve
				}
				E=NULL;delete E;
				cE=NULL;delete cE;
			}
			delete []tab_pruchodu;tab_pruchodu=NULL;vyhybka_pom=NULL;

			////HALA
			//alokace pomocného ukazatele
			C_hala *cH=new C_hala;
			//načtení atributů haly ze souboru
			FileStream->Read(cH,sizeof(C_hala));
			//samotná data
			HALA.body=NULL;
			HALA.X=cH->X;
			HALA.Y=cH->Y;
			//name
			wchar_t *nameH=new wchar_t[cH->name_delka];
			FileStream->Read(nameH,cH->name_delka*sizeof(wchar_t));
			HALA.name=nameH;
			delete[] nameH; nameH=NULL;
			//načtení bodu haly
			for(unsigned int j=1;j<=cH->pocet_bodu;j++)
			{
				C_bod cB;
				FileStream->Read(&cB,sizeof(C_bod));//načte jeden prvek ze souboru
				TBod *B=new TBod;
				B->n=cB.n;
				B->kota_offset=cB.kota_offset;
				B->X=cB.X;
				B->Y=cB.Y;
				vloz_bod(B);
				//delete B; B=NULL; nesmí být
			}

			//ZAKÁZKY
			for(unsigned int i=1;i<=File_hlavicka.pocet_zakazek;i++)//možno řešit sice while, po strukturách, ale toto je připravené pro případ, kdy budu načítat i objekty jiného typu než objekt
			{
				TZakazka *ukaz2=new TZakazka;
				C_zakazka *c_ukaz2=new C_zakazka;
				FileStream->Read(c_ukaz2,sizeof(C_zakazka));//načte jeden prvek ze souboru
				if(c_ukaz2->n!=0 && File_hlavicka.pocet_zakazek>=c_ukaz2->n)//pokud nenačte hlavičku či nějaký shit
				{
					//samotná data
					ukaz2->n=c_ukaz2->n;
					ukaz2->typ=c_ukaz2->typ;
					ukaz2->barva=c_ukaz2->barva;
					ukaz2->pomer=c_ukaz2->pomer;
					ukaz2->TT=c_ukaz2->TT;
					ukaz2->jig=c_ukaz2->jig;
					ukaz2->pocet_voziku=c_ukaz2->pocet_voziku;
					ukaz2->serv_vozik_pocet=c_ukaz2->serv_vozik_pocet;
					ukaz2->opakov_servis=c_ukaz2->opakov_servis;

					//id
					wchar_t *id=new wchar_t[c_ukaz2->id_length];
					FileStream->Read(id,c_ukaz2->id_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
					ukaz2->id=id;
					id=NULL; delete[] id;

					//popisek
					wchar_t *name=new wchar_t[c_ukaz2->name_length];
					FileStream->Read(name,c_ukaz2->name_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
					ukaz2->name=name;
					name=NULL; delete[] name;

					//přiřazení segmenty cesty
					hlavicka_cesta_zakazky(ukaz2);
					for(unsigned int j=1;j<=c_ukaz2->pocet_segmentu_cesty;j++)
					{
						C_cesta c_c;//=new C_cesta;
						FileStream->Read(&c_c,sizeof(C_cesta));//načte jeden prvek ze souboru
						vloz_segment_cesty(ukaz2,vrat_element(c_c.n_element),vrat_element(c_c.n_sparovany),c_c.data);
					}

					//načítání dávek zakázky
					hlavicka_davky_zakazky(ukaz2);
					for(unsigned int k=1;k<=c_ukaz2->pocet_davek;k++)
					{
						C_davka c_d;
						FileStream->Read(&c_d,sizeof(C_davka));//načte jeden prvek ze souboru
						vloz_davku(ukaz2,c_d.pocet_voziku,c_d.pocet_prazdnych,c_d.pocet_celkem);
					}

					//vloží zakazku do spojového seznamu ZAKAZKY
					vloz_zakazku(ukaz2);
				}
				//ukaz2=NULL; delete ukaz2;
				c_ukaz2=NULL; delete c_ukaz2;
			};

			//aktualizace Objekt->element ukazatele, a aktualizace ukazatelů teploměrů
			TObjekt *O=OBJEKTY->dalsi;
			while(O!=NULL)
			{
				O->element=vrat_element(O->element_n);
				//teploměry
				if(O->teplomery!=NULL)
				{
					unsigned long n;
					TTeplomery *T=O->teplomery->dalsi;
					while(T!=NULL)
					{
						//spárovaný element prvního teploměru
						n=T->prvni->sparovany->n;
						delete T->prvni->sparovany;T->prvni->sparovany=NULL;
						T->prvni->sparovany=vrat_element(n);
						//spárovaný element posledního teploměru
						n=T->posledni->sparovany->n;
						delete T->posledni->sparovany;T->posledni->sparovany=NULL;
						T->posledni->sparovany=vrat_element(n);
						//elementy v cestě
						TCesta *C=T->cesta->dalsi;
						while(C!=NULL)
						{
              //vrácení ukazatele na element
							n=C->Element->n;
							delete C->Element;C->Element=NULL;
							C->Element=vrat_element(n);
							C=C->dalsi;
						}
						delete C;C=NULL;
            //posun na další záznam teploměrů
						T=T->dalsi;
					}
          delete T;T=NULL;
        }
        //posun na další objekt
				O=O->dalsi;
			}
			delete O;O=NULL;
			//aktualizuje spárovné ukazatele po načtení
			aktualizuj_sparovane_ukazatele();
      //aktualizace režimu pohonů
			aktualizuj_rezim_pohonu();

			delete FileStream;
			return 1;
		}
		//catch(std::invalid_argument){;return 3;}
		//catch(...){;return 2;}//jiná chyba, např. špatný formát souboru
		catch(Exception &exeption)
		{
			if(exeption.Message=="Verze souboru a projektu se neshoduje")return 3;
      else return 2;
		}
	}
}
////---------------------------------------------------------------------------
void Cvektory::nacti_CSV_retezy(AnsiString FileName)
{   //FUNKČNÍ ALE ODSTAVENÁ METODA
//	try
//	{
//		AnsiString DATA=ReadFromTextFile(FileName);
//
//		//parsování a ukládání do spojového seznamu
//		if(DATA.Pos("název") || DATA.Pos("name"))DATA=Form1->ms.delete_repeat(DATA,"\r\n",1);//smaže případnou hlavičku csv souboru
//		while(DATA.Pos(";"))//bude parsovat, dokud budou data obsahovat středník
//		{
//			//pársování
//			AnsiString name=DATA.SubString(1,DATA.Pos(";")-1);DATA=Form1->ms.delete_repeat(DATA,";",1);
//			AnsiString roztec=DATA.SubString(1,DATA.Pos("\r\n")-1);DATA=Form1->ms.delete_repeat(DATA,"\r\n",1);//smaže konec řádku
//			//uložení do spojáku pro další využítí
//			vloz_retez(name,Form1->ms.MyToDouble(roztec));
//		}
//	}
//	catch(...)//např. soubor nenalezen
//	{
//		; //nic se neděje
//	}
}
////---------------------------------------------------------------------------
AnsiString Cvektory::ReadFromTextFile(AnsiString FileName)
{
	///////////////////////////načtení dat ze souboru
	TMemoryStream* MemoryStream=new TMemoryStream();
	MemoryStream->LoadFromFile(FileName);
	char *data_ch=new char[MemoryStream->Size];
	MemoryStream->Read(data_ch,MemoryStream->Size);
	return AnsiString(data_ch);
}
//---------------------------------------------------------------------------
void Cvektory::SaveText2File(AnsiString Text,AnsiString FileName)
{
  ///////////////////////////převod dat do UTF8
	/*WideString WData=Text;
	char *CHData=new char[Text.Length()+1];
	WideCharToMultiByte(/*CP_ACP*//*CP_UTF8,0,WData,Text.Length()+1,CHData,Text.Length()+1,NULL,NULL);
	*/

	///////////////////////////zapis data do souboru
	TMemoryStream* MemoryStream=new TMemoryStream();
	MemoryStream->Clear();
	MemoryStream->Write(Text.c_str(),Text.Length());//Win
	//MemoryStream->Write(Text.w_str(),Text.Length()*2);//UTF8
	MemoryStream->SaveToFile(FileName);
	delete MemoryStream;
}
////---------------------------------------------------------------------------
//short int Cvektory::ulozit_report(UnicodeString FileName)
//{
//	//
//}

//ZDM
//---------------------------------------------------------------------------
//AnsiString Cvektory::get_csv_xls(AnsiString S)//S=separator
//{
//	AnsiString data="";
//
//	//tabulka parametry linky
//
//	data+="Předpokládáný celkový výrobní čas [hodin]:"+S+UnicodeString(Form1->PP.hodin*Form1->PP.smen*Form1->PP.dni)+"\n";
//	data+="TAKT TIME linky [min/vozík]"+S+UnicodeString(Form1->PP.TT)+"\n";
//	data+="Propustnost linky [vozíků/min]"+S+UnicodeString(1/Form1->PP.TT)+"\n";
//	data+="LEAD TIME linky [vozíků/min]"+S+UnicodeString(LT)+"\n";
//	data+="WIP [vozíků]"+S+UnicodeString((1/Form1->PP.TT)*LT)+"\n";
//	data+="PCE [%]"+S+UnicodeString(sum_WT()/LT*100)+"\n";//zokrouhleno na dvě desetinná místa
//	data+="Produktů na vozík [ks]:"+S+UnicodeString(Form1->PP.produktu_vozik)+"\n";
//	data+="Délka vozíku [m]:"+S+UnicodeString(Form1->PP.delka_voziku)+"\n";
//	data+="Šířka vozíku [m]:"+S+UnicodeString(Form1->PP.sirka_voziku)+"\n";
//	data+="\n";//mezera,odřádkování
//
//	//hlavička dat objektů
//	data+="id"+S+"typ"+S+"zkratka"+S+"název"+S+"souřadnice X"+S+"souřadnice Y"+S+"režim"+S+"takt time objektu"+S+"cykle time objektu\n";
//
//	Cvektory::TObjekt *ukaz;
//	ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//		data+=UnicodeString(ukaz->n)+S+UnicodeString(ukaz->id)+S+UnicodeString(ukaz->short_name)+S+UnicodeString(ukaz->name)+S+UnicodeString(ukaz->X)+S+UnicodeString(ukaz->Y)+S;
//		switch(ukaz->rezim)
//		{
//			case 0:data+="STOP & GO"+S;break;
//			case 1:data+="KONTINUÁLNÍ"+S;break;
//			case 2:data+="POSTPROCESNÍ"+S;break;
//		}
//		data+=
//		UnicodeString(ukaz->TTo)+S+UnicodeString(ukaz->CT)+S+UnicodeString(ukaz->kapacita_objektu)+S+UnicodeString(ukaz->dop_kapacita_objektu);
//		data+="\n";//odřádkování
//		ukaz=ukaz->dalsi;//posun na další prvek
//	}
//	return data;
//}
////---------------------------------------------------------------------------
//AnsiString Cvektory::get_html()
//{
//		AnsiString data="";
//		data="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=windows-1250\"><meta name=\"tispl\" content=\"TISPL - Eltep s.r.o\"><title>REPORT z programu TISPL - Eltep</title></head><body>";//hlavička
//		data+="<style>th{color:white;font-family:Arial;background-color:gray;}td{text-align:center;color:gray;font-family:Arial;}table {border-collapse:collapse;border-color:gray;}</style>";
//		//tabulka parametry linky
//		data+="<table border=\"1\">";//tělo začátek tabulky parametry projektu
//		data+="<tr><th align=\"right\">Předpokládáný celkový výrobní čas [hodin]</th><td>"+UnicodeString(Form1->PP.hodin*Form1->PP.smen*Form1->PP.dni)+"</td></tr>";
//		data+="<tr><th align=\"right\">TAKT TIME linky [min/vozík]</th><td>"+UnicodeString(Form1->PP.TT)+"</td></tr>";
//		data+="<tr><th align=\"right\">Propustnost linky [vozíků/min]</th><td>"+UnicodeString(1/Form1->PP.TT)+"</td></tr>";
//		data+="<tr><th align=\"right\">LEAD TIME linky [vozíků/min]</th><td>"+UnicodeString(LT)+"</td></tr>";
//		data+="<tr><th align=\"right\">WIP [vozíků]</th><td>"+UnicodeString((1/Form1->PP.TT)*LT)+"</td></tr>";
//		data+="<tr><th align=\"right\">PCE [%]</th><td>"+UnicodeString(sum_WT()/LT*100)+"</td></tr>";//zokrouhleno na dvě desetinná místa
//		data+="<tr><th align=\"right\">Produktů na vozík [ks]</th><td>"+UnicodeString(Form1->PP.produktu_vozik)+"</td></tr>";
//		data+="<tr><th align=\"right\">Délka vozíku [m]</th><td>"+UnicodeString(Form1->PP.delka_voziku)+"</td></tr>";
//		data+="<tr><th align=\"right\">Šířka vozíku [m]</th><td>"+UnicodeString(Form1->PP.sirka_voziku)+"</td></tr>";
//		data+="</table>";//tělo konec tabulky parametry projektu
//		data+="<hr>";
//		//tělo začátek tabulky data
//		data+="<table border=\"1\" width=\"100%\">";
//		data+="<tr><th>id</th><th>typ</th><th>zkratka</th><th>název</th><!--<th>souřadnice X</th><th>souřadnice Y--></th><th>režim</th><th>takt time objektu</th><th>cykle time objektu</th><th>kapacita objektu</th><th>doporučená kapacita objektu</th></tr>";//tělo hlavička tabulky
//		//samotná data
//		Cvektory::TObjekt *ukaz;
//		ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			data+="<tr><td>"+UnicodeString(ukaz->n)+"</td><td>"+UnicodeString(ukaz->id)+"</td><td>"+UnicodeString(ukaz->short_name)+"</td><td>"+UnicodeString(ukaz->name)+"</td><!--<td>"+UnicodeString(ukaz->X)+"</td><td>"+UnicodeString(ukaz->Y)+"</td>--><td>";
//			switch(ukaz->rezim)
//			{
//				case 0:data+="STOP & GO";break;
//				case 1:data+="KONTINUÁLNÍ";break;
//				case 2:data+="POSTPROCESNÍ";break;
//			}
//			data+="</td><td>"+UnicodeString(ukaz->TTo)+"</td><td>"+UnicodeString(ukaz->CT)+"</td><td>"+UnicodeString(ukaz->kapacita_objektu)+"</td><td>"+UnicodeString(ukaz->dop_kapacita_objektu)+"</td></tr>";//tělo hlavička tabulky
//			ukaz=ukaz->dalsi;//posun na další prvek
//		}
//		data+="</table>";//tělo konec tabulky
//		data+="</body></html>";//patička
//		return data;
//}

////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//void Cvektory::hlavicka_palce()
//{
//	TPalec *novy_uzel=new TPalec;
//	novy_uzel->n=0;
//	novy_uzel->pozice=0;
//	novy_uzel->segment=NULL;
//
//	novy_uzel->predchozi=novy_uzel;//ukazuje sam na sebe
//	novy_uzel->dalsi=NULL;
//	PALCE=novy_uzel;
//}
////---------------------------------------------------------------------------
//void Cvektory::vloz_palec()//přidá nový vozík do seznamu PALCE
//{
//	TPalec *novy=new TPalec;
//
//	novy->n=PALCE->predchozi->n+1;//navýším počítadlo prvku o jedničku
//
//	PALCE->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=PALCE->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;
//	PALCE->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//}
////---------------------------------------------------------------------------
//long Cvektory::vymaz_seznam_palcu()
//{
//	long pocet_smazanych_objektu=0;
//	while (PALCE!=NULL)
//	{
//		pocet_smazanych_objektu++;
//		PALCE->predchozi=NULL;
//		delete PALCE->predchozi;
//		PALCE=PALCE->dalsi;
//	};
//
//	return pocet_smazanych_objektu;
//};
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
TPointD Cvektory::vrat_zacatek_a_konec_zakazky(TZakazka *jaka)//ukazatel na cestu resp, zakázku
{
	TPointD RET; RET.x=0;RET.y=0;//bool prvni=true;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		//odstaveno kvůli zDM if(vozik->zakazka->n==jaka->n && prvni){RET.x=vozik->start/Form1->d.PX2MIN*60.0;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
		//odstaveno kvůli zDM if(vozik->zakazka->n==jaka->n){RET.y=vozik->pozice/Form1->d.PX2MIN*60.0;}//uloží koncovou pozici posledního vozíku na zakázce
		vozik=vozik->dalsi;
	}
	return RET;
}
TPointD Cvektory::vrat_zacatek_a_konec_zakazky(unsigned int ID_zakazky)//ukazatel na cestu resp, zakázku
{
	TPointD RET; RET.x=0;RET.y=0;//bool prvni=true;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		//odstaveno kvůli zDM if(vozik->zakazka->n==ID_zakazky && prvni){RET.x=vozik->start/Form1->d.PX2MIN*60.0;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
		//odstaveno kvůli zDM if(vozik->zakazka->n==ID_zakazky)RET.y=vozik->pozice/Form1->d.PX2MIN*60.0;//uloží koncovou pozici posledního vozíku na zakázce
		vozik=vozik->dalsi;
	}
	return RET;
}
////---------------------------------------------------------------------------
//nemusí se vždy jednat o poslední zakázku
double Cvektory::vrat_nejpozdejsi_konec_zakazek()
{
	double MAX=0;
	TZakazka *Z=ZAKAZKY->dalsi;
	while(Z!=NULL)
	{
		double DO=vrat_zacatek_a_konec_zakazky(Z).y;//konec zakazky v min
		if(DO>MAX)MAX=DO;
		Z=Z->dalsi;
	}
	return MAX;
}
////---------------------------------------------------------------------------
//vratí součet CT časů objektů ze statusu návrh/architekt
double Cvektory::vrat_LT()
{
		double LT=0.0;
		TObjekt *ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
		while (ukaz!=NULL)
		{
			//LT+=ukaz->CT;
			ukaz=ukaz->dalsi;//posun na další prvek
		}
		return LT;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_LT_voziku(TVozik *jaky)//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
{
	//odstaveno kvůli zDM if(jaky!=NULL) return (jaky->pozice-jaky->start)/Form1->d.PX2MIN*60.0;
	//odstaveno kvůli zDM else
	return 0;
}
double Cvektory::vrat_LT_voziku(unsigned int n_voziku)//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
{
	double RET=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(n_voziku==vozik->n)//pokud byl nalezen
		{
			RET=vrat_LT_voziku(vozik);
			break;
		}
		vozik=vozik->dalsi;
	}
	return RET;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_sumPT_voziku(TVozik *jaky)//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
{
	double SUM=0;
//	Cvektory::TCesta *C=jaky->zakazka->cesta->dalsi;
//	while(C!=NULL)//jde po konkrétní cestě
//	{
//		SUM+=C->CT;
//		C=C->dalsi;
//	}
	return SUM;
}
double Cvektory::vrat_sumPT_voziku(unsigned int n_voziku)//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
{
	double SUM=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(n_voziku==vozik->n)//pokud byl nalezen
		{
			SUM=vrat_sumPT_voziku(vozik);
			break;
		}
		vozik=vozik->dalsi;
	}
	return SUM;
}
//---------------------------------------------------------------------------
double Cvektory::vrat_AVGsumPT_zakazky(TZakazka *jaka)//vrátí čistý čas, který strávily vozík ve výrobě bez čekání pro danou zakazku
{
	double SUM=0.0; double n=0.0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(jaka==vozik->zakazka)//pokud se jedna o zadanou cestu navýší počítadlo
		{
			SUM+=vrat_sumPT_voziku(vozik);
			n++;
		}
		vozik=vozik->dalsi;
	}
	if(n==0)return 0;//pokud je pouze jenom jeden vozík není takt
	else
	return SUM/n;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_WT_voziku(TVozik *jaky)//vrátí čas čeká vozíku během výroby
{
	return vrat_LT_voziku(jaky)-vrat_sumPT_voziku(jaky);
}
double Cvektory::vrat_WT_voziku(unsigned int n_voziku)//vrátí čas čeká vozíku během výroby
{
	double RET=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(n_voziku==vozik->n)//pokud byl nalezen
		{
			RET=vrat_WT_voziku(vozik);
			break;
		}
		vozik=vozik->dalsi;
	}
	return RET;
}
//---------------------------------------------------------------------------
double Cvektory::vrat_AVGsumWT_zakazky(TZakazka *jaka)//vrátí čistý čas, který vozíky čekaly pro danou zakazku
{
	double SUM=0.0; double n=0.0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(jaka==vozik->zakazka)//pokud se jedna o zadanou cestu navýší počítadlo
		{
			SUM+=vrat_WT_voziku(vozik);
			n++;
		}
		vozik=vozik->dalsi;
	}
	if(n==0)return 0;//pokud je pouze jenom jeden vozík není takt
	else
	return SUM/n;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_TT_voziku(TVozik *jaky)//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
{
	return 0;//odstaveno kvůli zDM (jaky->pozice-jaky->predchozi->pozice)/Form1->d.PX2MIN*60.0;//ošetřuje i případ prvního prvku a hlavičky, hlavička má pozici nastavenou na nula
}
double Cvektory::vrat_TT_voziku(unsigned int n_voziku)//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
{
	double RET=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(n_voziku==vozik->n)//pokud byl nalezen
		{
			RET=vrat_TT_voziku(vozik);
			break;
		}
		vozik=vozik->dalsi;
	}
	return RET;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_AVG_TT_zakazky(TZakazka *jaka)//vrátí hodnotu průměrného TT mezi vozíky v rámci dané zakázky/cesty
{
	unsigned int i=0;
	bool prvni_vozik_zakazky=true;//ignorace taktu prvního vozíku
	double TT=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka==jaka)//pokud byl nalezen v rámci dané cesty a zároveň se nejedná o první vozík v zakázce, tak aby se nezohledňoval takt z vyplývající z náběhu link či přechodu mezi zakázkami
		{
			if(prvni_vozik_zakazky)prvni_vozik_zakazky=false;//ignorace taktu prvního vozíku
			else
			{
				TT+=vrat_TT_voziku(vozik);//zatím se jedná pouze o součet TT
				i++;//navýší počet pro výpočet průměru
			}
		}
		vozik=vozik->dalsi;
	}
	if(i==0)return 0;//pokud je pouze jenom jeden vozík není takt
	else return floor(TT/i*1000000.0)/1000000.0;//vrátí průměrné TT
}                      //kvůli chybě s přesnosti uříznutí na 6 reaálných míst
double Cvektory::vrat_AVG_TT_zakazky(unsigned int n_zakazky)//vrátí hodnotu průměrného TT mezi vozíky v rámci
{
	unsigned int i=0;
	bool prvni_vozik_zakazky=true;//ignorace taktu prvního vozíku
	double TT=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka->n==n_zakazky)//pokud byl nalezen v rámci dané cesty a zároveň se nejedná o první vozík v zakázce, tak aby se nezohledňoval takt z vyplývající z náběhu link či přechodu mezi zakázkami
		{
			if(prvni_vozik_zakazky)prvni_vozik_zakazky=false;//ignorace taktu prvního vozíku
			else
			{
				TT+=vrat_TT_voziku(vozik);//zatím se jedná pouze o součet TT
				i++;//navýší počet pro výpočet průměru
			}
		}
		vozik=vozik->dalsi;
	}
	if(i==0)return 0;//pokud je pouze jenom jeden vozík není takt
	else return floor(TT/i*1000000.0)/1000000.0;//vrátí průměrné TT
}                      //kvůli chybě s přesnosti uříznutí na 6 reálných míst
////---------------------------------------------------------------------------
unsigned int Cvektory::vrat_pocet_voziku_zakazky(TZakazka *jaka)
{
	unsigned int RET=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka==jaka)RET++;
		vozik=vozik->dalsi;
	}
	return RET;
}
unsigned int Cvektory::vrat_pocet_voziku_zakazky(unsigned int n_zakazky)
{
	unsigned int RET=0;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka->n==n_zakazky)RET++;
		vozik=vozik->dalsi;
	}
	return RET;
}
////---------------------------------------------------------------------------
//vrátí začátek a konec vozíku na časové ose v PX
TPoint Cvektory::vrat_start_a_pozici_vozikuPX(unsigned int n_voziku)
{
	 TPoint RET; RET.x=0;RET.y=0;
	 if(n_voziku==0)n_voziku=1;//ošetření proti zásahu do hlavičky
	 TVozik *V=VOZIKY->dalsi;
	 while(V!=NULL)
	 {
			//odstaveno kvůli zDM if(V->n==n_voziku)RET.x=V->start;
			//odstaveno kvůli zDM if(V->n==n_voziku)RET.y=V->pozice;
			V=V->dalsi;
	 }
	 return RET;
}
////---------------------------------------------------------------------------
//vrátí max. počet vozíků na lince, kde parametr s implicitní hodnotou 0 je volaný výpočet z překrytí vozíků na časových osách, hodnot 1 - součtem kapacit zadaných (resp. v návrháru/architektovi vypočítaných), hodnota 2 - součtem kapacit vypočtených v časových osách,  3 - tradiční výpočet WIP=1/TT*LT
double Cvektory::WIP(short typ_vypoctu)
{
	double pocet_final=0.0;
	switch(typ_vypoctu)
	{
		//z vozíků na časových osách
		case 0:
		{
			//srovnává všechny kombinace, možná by šlo zjednodušit, uvídíme v průběhu
			Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
			while (vozik!=NULL)
			{
				unsigned int pocet=0;
				Cvektory::TVozik *vozik2=vozik->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
				while (vozik2!=NULL)
				{

					//odstaveno kvůli zDM if(vozik->pozice>=vozik2->start)//pokud nastane situace že vozík skončil před začátkem vozíku, není nutné navyšovat počítadlo vozíků
					pocet++;
					vozik2=vozik2->dalsi;
				}
				if(pocet_final<pocet)pocet_final=pocet;
				vozik=vozik->dalsi;
			}
		}break;
		//součtem kapacit zadaných (resp. v návrháru/architektovi vypočítaných)
		case 1:
		{
			Cvektory::TObjekt *ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
			while (ukaz!=NULL)
			{
				pocet_final+=ukaz->kapacita;
				ukaz=ukaz->dalsi;//posun na další prvek
			}
		}break;
		//součtem kapacit vypočtených v časových osách
		case 2:
		{
			Cvektory::TObjekt *ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
			while (ukaz!=NULL)
			{
				pocet_final+=ukaz->kapacita_dop;
				ukaz=ukaz->dalsi;//posun na další prvek
			}
		}break;
		//tradiční výpočet
		case 3:
		pocet_final=(1/PP.TT)*vrat_LT();
		break;
		//součtem pozic zadaných (resp. v návrháru/architektovi vypočítaných)
		case 4:
		{
			Cvektory::TObjekt *ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
			while (ukaz!=NULL)
			{
				//pocet_final+=ukaz->pozice;
				ukaz=ukaz->dalsi;//posun na další prvek
			}
		}break;
	}
	return pocet_final;
}
//---------------------------------------------------------------------------
//srovnává všechny kombinace
unsigned int Cvektory::vrat_kapacitu_objektu(TObjekt *O)
{
	 unsigned int pocet_final=0;
//	 if(O->rezim!=0)//pokud se nejedná o S&G, ten má kapacitu vždy 1, pokud ano algoritmus se přeskočí
//	 {
//			 TProces *P=PROCESY->dalsi;
//			 while (P!=NULL)
//			 {
//				 unsigned int pocet=1;
//				 if(P->segment_cesty->objekt->n==O->n)//pokud se jedná o hledaný objekt
//				 {
//						TProces *P2=P->dalsi;
//						while (P2!=NULL)
//						{
//							if(P2->segment_cesty->objekt->n==O->n && P->Tcek>P2->Tpoc)//pokud se jedná o spoluhledaný objekt a objekty se v čase zároveň překrývají (tudíž se navyšuje jejich kapacita)
//							pocet++;
//							P2=P2->dalsi;
//						}
//				 }
//				 if(pocet_final<pocet)pocet_final=pocet;
//				 P=P->dalsi;
//			 };
//	 }
//	 else pocet_final=1;
	 if(O!=NULL && PROCESY!=NULL && PROCESY->predchozi->n>0) return pocet_final/*+1*/;
	 else return 0;
}
//---------------------------------------------------------------------------
void Cvektory::uloz_doporucene_kapacity_objetku()
{
	TObjekt *ukaz=OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		ukaz->kapacita_dop=vrat_kapacitu_objektu(ukaz);
		ukaz=ukaz->dalsi;
	}
}
//---------------------------------------------------------------------------
//vrátí minimální, střední, průměrnou a maximální dobu čekání na palec v sec pro daný objekt (segment cesty) tak, jak bylo vypočteno v analýze/na časových osách, musí být tedy zde zvolena nějaká (libovolná) volba čekání na palec, mimo "žádná"Struktura TMinMedAvgMax_d vrací 4 hodnoty, min, med, agv, max datového typu double, volání výsledků probíhá přes “tečkový selektor”
TMinMedAvgMax_d Cvektory::vrat_statisticke_doby_cekani_na_palec(TCesta *segment_cesty)
{
	 //definice
	 double min=9999999.9;//pouze velká hodnota
	 double max=0;
	 long pocet=0.0;
	 double soucet=0.0;
	 TMinMedAvgMax_d RET; RET.Min=0.0;RET.Med=0.0;RET.Avg=0.0;RET.Max=0.0;
	 //výpočet
	 TProces *P=PROCESY->dalsi;
	 while(P!=NULL)
	 {
		 if(P->segment_cesty==segment_cesty)
		 {
			 double T=P->Tcek-P->Tpre;
			 if(T>max)max=T;
			 if(T<min)min=T;
			 pocet++;
		 }
		 P=P->dalsi;
	 }
	 delete P;
	 //návratové hodnoty
	 if(min!=9999999.9)RET.Min=min;
	 RET.Med=(max+min)/2.0;
	 if(pocet>0)RET.Avg=soucet/pocet;
	 RET.Max=max;
	 //navrácení hodnoty
   return RET;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//provede dotaz dle zadaného parametru/dotazu metdody v syntaxi SQL, pokud dotazaz vrací data, metoda vratí data pomocí AnsiString, zatím umí jen základní úroveň
AnsiString Cvektory::QUERY(AnsiString query)
{
	query=query.UpperCase();//vše na velká písmena
	AnsiString command=query.SubString(1,query.Pos(" ")-1);
	query=query.SubString(query.Pos(" ")+1,query.Length());
	//SELECT
	if(command=="SELECT")
	{
		AnsiString command=query.SubString(1,query.Pos(" ")-1);
		query=query.SubString(query.Pos(" ")+1,query.Length());
		AnsiString from=query.SubString(query.Pos("="),query.Pos(" ")-1);
		query=query.SubString(query.Pos(" ")+1,query.Length());
		if(command.Pos("COUNT"));
		if(command.Pos("MAX"));
		if(command.Pos("MIN"));
		if(command.Pos("AVG"));
		if(command==("*"));
	}
	return command;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vytovoří a otevře CSV, je-li požadováno
void Cvektory::Text2CSV(AnsiString text,AnsiString FileName,AnsiString Title,AnsiString DefaultExt,AnsiString Filter,bool openDialog,bool open)
{
	//příprava SaveDialogu
	F->SaveDialog->Title=Title;
	F->SaveDialog->DefaultExt=DefaultExt;
	F->SaveDialog->Filter=Filter;

	//předvyplnění názvem stejnojmeným souboru
	UnicodeString FN=FileName;
	if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
	F->SaveDialog->FileName=FN;

	bool OPN=true;
	if(openDialog){if(F->SaveDialog->Execute())OPN=true;else OPN=false;}

	if(OPN)
	{
		Screen->Cursor=crHourGlass;//změní kurzor na přesýpací hodiny
		//nastavení formátu
		UnicodeString export_format="csv";

		//samotné uložení

		//zapis data do souboru
		TMemoryStream* MemoryStream=new TMemoryStream();
		MemoryStream->Clear();
		MemoryStream->Write(text.c_str(),text.Length());//Win kodování
		if(openDialog)MemoryStream->SaveToFile(F->SaveDialog->FileName);
		else MemoryStream->SaveToFile(FileName);

		//postprocesní záležitost
		Screen->Cursor=crDefault;//změní kurzor na default
		if(openDialog)FileName=Form1->SaveDialog->FileName;
		if(open)ShellExecute(0,L"open",UnicodeString(FileName).c_str(),0,0,SW_SHOWNORMAL);;//otevře výstup
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void Cvektory::vse_odstranit()
{
 	//HALA
 	vymaz_body();

 	//VOZÍKY
 	if(VOZIKY!=NULL && VOZIKY->predchozi->n>0)//pokud je více objektů
 	{
 		vymaz_seznam_VOZIKY();//vymaze vozíky z paměti
 		VOZIKY=NULL; delete VOZIKY;
 	}
 	hlavicka_VOZIKY();//nutnost

 	//ZAKÁZKY
 	if(ZAKAZKY!=NULL && ZAKAZKY->predchozi->n>0)//pokud je více objektů
 	{
 		vymaz_seznam_ZAKAZKY();//byla zde poznámka, že před zdm padalo
 		delete ZAKAZKY; ZAKAZKY=NULL;
 	}
 	hlavicka_ZAKAZKY();//nutnost

 	//OBJEKTY včetně elementů
 	if(OBJEKTY!=NULL && OBJEKTY->predchozi->n>0)//pokud je více objektů
 	{
 		vymaz_seznam_OBJEKTY();//vymaze objekty z paměti
 		delete OBJEKTY; OBJEKTY=NULL;
 	}
	hlavicka_OBJEKTY();//nutnost

	//ELEMENTY
	if(ELEMENTY!=NULL && ELEMENTY->predchozi->n>0)//pokud je více elementů
 	{
		vymaz_seznam_ELEMENTY();//vymaze elementy z paměti
		delete ELEMENTY; ELEMENTY=NULL;
 	}
	hlavicka_ELEMENTY();//nutnost

 	//POHONY
 	if(POHONY!=NULL && POHONY->predchozi->n>0)//pokud je více objektů
 	{
 		vymaz_seznam_POHONY();//vymaze pohony z paměti
 		delete POHONY; POHONY=NULL;
	}
	hlavicka_POHONY();//nutnost

	//ZPRÁVY
	bool puvodni_zpravy=false;if(ZPRAVY!=NULL && ZPRAVY->predchozi->n)puvodni_zpravy=true;
	vymazat_ZPRAVY();if(puvodni_zpravy){Form_zpravy->update_zpravy(0,0);}

	//DATA
	if(DATA!=NULL && DATA->predchozi->n>0)//pokud je více objektů
 	{
		vymaz_seznam_DATA();//vymaze pohony z paměti
		delete DATA; DATA=NULL;
	}
	hlavicka_DATA();//nutnost

	//MAG_LASO
	if(MAG_LASO!=NULL && MAG_LASO->predchozi->n>0)//pokud je více elementů
 	{
		vymaz_seznam_MAG_LASO();//vymaze elementy z paměti
		delete MAG_LASO; MAG_LASO=NULL;
 	}
	hlavicka_MAG_LASO();//nutnost

//		//palce
//		if(PALCE->predchozi->n>0)//pokud je více objektů
//		{
//			vymaz_seznam();//vymaze body z paměti
//			delete PALCE; PALCE=NULL;
//		}

  //SIM
	vymaz_seznam_SIM();
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////práce s DATA, obrazem projektu
//vytvoření hlavičky DATA
void Cvektory::hlavicka_DATA()
{
	////alokace paměti + vytvoření hlavičky
	DATA=vytvor_prazdny_obraz();

	////ukazatelové zalezitosti DAT
	DATA->dalsi=NULL;
	DATA->predchozi=DATA;
}
//---------------------------------------------------------------------------
//vytvoří prázdný obraz projektu, slouží pro vytváření hlavičky nebo pro nový obraz (první prázdný obraz, poté se plní)
Cvektory::TDATA *Cvektory::vytvor_prazdny_obraz()
{
	////alokace paměti
	TDATA *obraz=new TDATA;

	////nastavení parametrů na default hodnoty
	obraz->n=0;
	obraz->edit_Objekt=0;
	obraz->posledni_element_n=0;
	obraz->pocet_vyhybek=0;

	////Vytvoření hlavičky Objekty
	obraz->Objekty=new TObjekt;
	obraz->Objekty->n=0;
	obraz->Objekty->id=0;
	obraz->Objekty->X=0;
	obraz->Objekty->Y=0;
	obraz->Objekty->body=NULL;
	obraz->Objekty->sirka_steny=0;
	obraz->Objekty->short_name="";
	obraz->Objekty->name="";
	obraz->Objekty->rezim=0;
	obraz->Objekty->pohon=NULL;
	obraz->Objekty->element=NULL;
	obraz->Objekty->min_prujezdni_profil.x=0;
	obraz->Objekty->min_prujezdni_profil.y=0;
	obraz->Objekty->koty_elementu_offset.x=0;
	obraz->Objekty->koty_elementu_offset.y=0;
	obraz->Objekty->komora=NULL;
	obraz->Objekty->teplomery=NULL;
	obraz->Objekty->probehla_aktualizace_prirazeni_pohonu=false;
	obraz->Objekty->zobrazit_koty=true;
	obraz->Objekty->zobrazit_mGrid=true;
	obraz->Objekty->uzamknout_nahled=false;
	obraz->Objekty->dalsi=NULL;
	obraz->Objekty->predchozi=obraz->Objekty;

	////Vytvoření hlavičky Elementy
	obraz->Elementy=new TElement;
	obraz->Elementy->n=0;
	obraz->Elementy->eID=0;
	obraz->Elementy->short_name="";
	obraz->Elementy->name="";
	obraz->Elementy->orientace=0;
	obraz->Elementy->rotace_jig=0;
	obraz->Elementy->stav=-1;
	obraz->Elementy->X=0;
	obraz->Elementy->Y=0;
	obraz->Elementy->Z=0;
	obraz->Elementy->Xt=0;
	obraz->Elementy->Yt=0;
	obraz->Elementy->PTotoc=0;
	obraz->Elementy->OTOC_delka=0;
	obraz->Elementy->zona_pred=0;
	obraz->Elementy->zona_za=0;
	obraz->Elementy->WT=0;
	obraz->Elementy->citelna_oblast.rect0=TRect(0,0,0,0);
	obraz->Elementy->citelna_oblast.rect1=TRect(0,0,0,0);
	obraz->Elementy->citelna_oblast.rect2=TRect(0,0,0,0);
	obraz->Elementy->citelna_oblast.rect3=TRect(0,0,0,0);
	obraz->Elementy->citelna_oblast.rect4=TRect(0,0,0,0);
	vloz_G_element(obraz->Elementy,0,0,0,0,0,0,0,0,0,0,0,0,0);
	obraz->Elementy->mGrid=NULL;
	obraz->Elementy->data.PD=0;
	obraz->Elementy->data.orientace_jig_pred=0;
	obraz->Elementy->data.LO1=0;
	obraz->Elementy->data.LO2=0;
	obraz->Elementy->data.LO_pozice=0;
	obraz->Elementy->data.PT1=0;
	obraz->Elementy->data.PT2=0;
	obraz->Elementy->data.WTstop=0;
	obraz->Elementy->data.RT=0;
	obraz->Elementy->data.pocet_pozic=0;
	obraz->Elementy->data.pocet_voziku=0;
	obraz->Elementy->objekt_n=0;
	obraz->Elementy->pohon=NULL;
	obraz->Elementy->sparovany=NULL;
	obraz->Elementy->dalsi=NULL;
	obraz->Elementy->predchozi=obraz->Elementy;

	////Vytvoření hlavičky Pohon
	obraz->Pohony=new TPohon;
	obraz->Pohony->n=0;
	obraz->Pohony->name="";
	obraz->Pohony->rychlost_od=0;
	obraz->Pohony->rychlost_do=0;
	obraz->Pohony->aRD=0;
	obraz->Pohony->roztec=0;
  obraz->Pohony->roztec_ID=0;
	obraz->Pohony->Rz=0;
	obraz->Pohony->Rx=0;
	obraz->Pohony->retez=NULL;
	obraz->Pohony->dalsi=NULL;
	obraz->Pohony->predchozi=obraz->Pohony;

	////vytvoření hlavičky pro Z_cesty
	obraz->Z_cesty=new TZakazka_uloz;
	obraz->Z_cesty->n=0;
	obraz->Z_cesty->dalsi=NULL;
	obraz->Z_cesty->predchozi=obraz->Z_cesty;
  obraz->Z_cesty->cesta=NULL;

	////vracení prázdného obrazu
	return obraz;
}
//---------------------------------------------------------------------------
//vytvoří obraz projektu v závislosti zda se jedná o storno funkcionalitu, layout nebo editaci objektu
void Cvektory::vytvor_obraz_DATA(bool storno)
{
	////vytvoření nebo vybrání obrazu do kterého budu ukladát projekt
	TDATA *obraz=DATA;//pro storno funkcionalitu nevytvařím prázdný obraz, ale ukládám do hlavičky
	if(!storno)//pokud se nejedná o storno funkcionalitu vytvořím nový prázdný obraz do kterého budu projetk ukládat
	{
		//detekování přechodu z layoutu do editace nebo z editace do editace, editace a layout nebo editace a editace jsou nezávislé v Undo, tz. nelze se pomocí Undo dostat z editace zpět do layout, proto je nutné při přechodu smazat všechny obrazy
		if(DATA->predchozi->n>0 && F->OBJEKT_akt!=NULL && (DATA->predchozi->edit_Objekt==0 || DATA->predchozi->edit_Objekt>0 && DATA->predchozi->edit_Objekt!=F->OBJEKT_akt->n))
		{
			//mazání všech jíž nepotřebných obrazů
			while(DATA->dalsi!=NULL)
			{
				smaz_obraz_DATA(DATA->predchozi->n);
			}
			pozice_data=0;//vrácení pozice na default hodnotu
			if(F->scGPGlyphButton_redo->Enabled)F->scGPGlyphButton_redo->Enabled=false;//pokud je povolen button na redo ... zakázat, bude přidán nový obraz, přepíše následující
		}
		//pokud jsem používal funkcionalitu Undo a jsem například o 2 kroky zpět a vytvořím nový obraz, dojde ke smazání těchto 2 krouku (funkcionalita viz. MS Word abc -> ad)
		if(pozice_data!=0)
		{
			//mazání od posledního do pozice_data + 1
			for(unsigned long i=DATA->predchozi->n;i>pozice_data;i--)
			{
				smaz_obraz_DATA(i);
			}
			pozice_data=0;//vrácení pozice na default hodnotu
			if(F->scGPGlyphButton_redo->Enabled)F->scGPGlyphButton_redo->Enabled=false;//pokud je povolen button na redo ... zakázat, bude přidán nový obraz, přepíše následující
		}
		if(!F->scGPGlyphButton_undo->Enabled && DATA->predchozi->n>=1)F->scGPGlyphButton_undo->Enabled=true;//pokud je zakázán btn pro undo ... povolit, bude přidán nový datový obraz
		//vytvoření prázdného obrazu (hlavičky objektu, elementů a pohonů)
		obraz=vytvor_prazdny_obraz();
		//kontrola zda je místo v bufferu, případné smazání nejstaršího obrazu, tj. vždy první
		if(DATA->predchozi->n>=pocet_kroku)smaz_obraz_DATA(1);
		//zařazení do spojáku DATA, vždy na konec
		obraz->n=DATA->predchozi->n+1;
		obraz->predchozi=DATA->predchozi;
		DATA->predchozi->dalsi=obraz;
		DATA->predchozi=obraz;
		obraz->dalsi=NULL;
		//kontrola zda je otevřená editace, pokud ano doplní do obrazu n editovaného objektu (slouží k rozpoznání zda šlo o editaci nebo layout)
		if(F->OBJEKT_akt!=NULL)obraz->edit_Objekt=F->OBJEKT_akt->n;
	}
	if(storno && DATA->Objekty->predchozi->n!=0)obraz=NULL;//pokud se spouští otevírání náhledu znova (přechod z editace do editace) v průběhu manipulace z daty nepřepisovat hlavičku,pří přechodu z editace do editace nenačítat z hlavičky, důležité!!!!

  ////vytvoření obrazu projektu
	if(obraz!=NULL)
	{
		//kopírování všech objektů
		TObjekt *O=OBJEKTY->dalsi,*o_kop=NULL;
		while(O!=NULL)
		{
			//vytvoření kopie
			o_kop=new TObjekt;
			kopiruj_objekt(O,o_kop);
      o_kop->element_n=O->element->n;
			//ukazatelové propojení kopie s obraz->Objekty
			if(obraz->Objekty->dalsi==NULL)obraz->Objekty->dalsi=o_kop;
			else obraz->Objekty->predchozi->dalsi=o_kop;
			o_kop->predchozi=obraz->Objekty->predchozi;
			obraz->Objekty->predchozi=o_kop;
			o_kop->dalsi=NULL;
			//ukazatelové záleřitosti
			o_kop=NULL;delete o_kop;
			O=O->dalsi;
		}
		delete O;O=NULL;

		//kopírování všech elementů
		TElement *E=ELEMENTY->dalsi,*e_kop=NULL;
		TPoint *tab_pruchodu=new TPoint[pocet_vyhybek];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
		while(E!=NULL)
		{
			//vytvoření kopie
			e_kop=new TElement;
			kopiruj_element(E,e_kop);
			//ukazatelové propojení kopie s obraz->Elementy
			if(obraz->Elementy->dalsi==NULL)obraz->Elementy->dalsi=e_kop;
			else obraz->Elementy->predchozi->dalsi=e_kop;
			e_kop->predchozi=obraz->Elementy->predchozi;
			obraz->Elementy->predchozi=e_kop;
			e_kop->dalsi=NULL;
			//ukazatelové záležitosti
			e_kop=NULL;delete e_kop;
			E=sekvencni_zapis_cteni(E,tab_pruchodu,NULL);
		}
		delete E;E=NULL;
		delete []tab_pruchodu;

		//uložení počtu výhybek
		obraz->pocet_vyhybek=pocet_vyhybek;

		//kopie všech pohonů
		TPohon *p=POHONY->dalsi,*p_kop=NULL;
		while(p!=NULL)
		{
			//vytvoření kopie
			p_kop=new TPohon;
			p_kop->n=p->n;
			p_kop->name=p->name;
			p_kop->rychlost_od=p->rychlost_od;
			p_kop->rychlost_do=p->rychlost_do;
			p_kop->aRD=p->aRD;
			p_kop->roztec=p->roztec;
      p_kop->roztec_ID=p->roztec_ID;
			p_kop->Rz=p->Rz;
			p_kop->Rx=p->Rx;
			p_kop->retez=p->retez;
			//ukazatelové propojení kopie s obraz->Pohony
			if(obraz->Pohony->dalsi==NULL)obraz->Pohony->dalsi=p_kop;
			else obraz->Pohony->predchozi->dalsi=p_kop;
			p_kop->predchozi=obraz->Pohony->predchozi;
			obraz->Pohony->predchozi=p_kop;
			p_kop->dalsi=NULL;
  		//ukazatelové záležitosti
			p_kop=NULL;delete p_kop;
  		p=p->dalsi;
  	}
		delete p;p=NULL;

		//záloha cesty aktuální zakázky
		if(ZAKAZKY!=NULL && ZAKAZKY->dalsi!=NULL)
		{
			TZakazka *Z=ZAKAZKY->dalsi;
			TZakazka_uloz *Z_u=NULL;
			while(Z!=NULL)
			{
				Z_u=new TZakazka_uloz;
				//vytvoření hlavičky pro cestu zakázky
				Z_u->cesta=new TCesta_uloz;
				Z_u->cesta->n=0;
				Z_u->cesta->dalsi=NULL;
				Z_u->cesta->predchozi=Z_u->cesta;
				//průchod skrze segmenty cesty zakázky
				TCesta *c=Z->cesta->dalsi;
		  	TCesta_uloz *c_u=NULL;
		  	while(c!=NULL)
		  	{
		  		c_u=new TCesta_uloz;
					c_u->n=Z_u->cesta->predchozi->n+1;
					c_u->element_n=c->Element->n;
		  		if(c->sparovany!=NULL)c_u->sparovany_n=c->sparovany->n;
		  		else c_u->sparovany_n=0;
		  		c_u->data.PD=c->data.PD;
		  		c_u->data.orientace_jig_pred=c->data.orientace_jig_pred;
		  		c_u->data.LO1=c->data.LO1;
		  		c_u->data.LO2=c->data.LO2;
		  		c_u->data.LO_pozice=c->data.LO_pozice;
		  		c_u->data.PT1=c->data.PT1;
		  		c_u->data.PT2=c->data.PT2;
		  		c_u->data.WTstop=c->data.WTstop;
					c_u->data.RT=c->data.RT;
		  		c_u->data.pocet_pozic=c->data.pocet_pozic;
		  		c_u->data.pocet_voziku=c->data.pocet_voziku;
		  		c_u->dalsi=NULL;
					c_u->predchozi=Z_u->cesta->predchozi;
					Z_u->cesta->predchozi->dalsi=c_u;
		  		Z_u->cesta->predchozi=c_u;
		  		c_u=NULL;delete c_u;
		  		c=c->dalsi;
		  	}
				delete c;c=NULL;
				//uložení Z_u do obrazu
				Z_u->dalsi=NULL;
				Z_u->predchozi=obraz->Z_cesty->predchozi;
				obraz->Z_cesty->predchozi->dalsi=Z_u;
				obraz->Z_cesty->predchozi=Z_u;
				Z_u=NULL;delete Z_u;
				Z=Z->dalsi;
			}
			delete Z;Z=NULL;
		}
	}
}
//---------------------------------------------------------------------------
//načtení z obrazu projektu v závislosti zda se jedná o storno funkcionalitu, layout nebo editaci objektu
void Cvektory::nacti_z_obrazu_DATA(bool storno)
{        
	////určení obrazu z kterého budou data načítány
	TDATA *obraz=DATA;
	if(!storno)obraz=vrat_obraz_DATA(pozice_data);

	////načzení dat z obrazu
	if(obraz!=NULL && (obraz->n==0 && storno || obraz->n!=0 && !storno))//kontrola zda sen nenarazil na poslední obraz
	{
		F->Timer_backup->Enabled=false;//vypnutí timeru pro backup, nesmí spustit během této metody!! (uložení nesmyslů do backup)
		bool exituje_tab_poh=false;
		////Layout nebo Storno
		//if(obraz->edit_Objekt==0)
		{
			if(F->OBJEKT_akt!=NULL && !storno)
			{
				//mazání mGridů elementů
				F->nastav_focus();
				TElement *E=F->OBJEKT_akt->element;
				T2Element *VYHYBKY=hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
				while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
				{
					E->mGrid->Delete();
					E->mGrid=NULL;
					E=dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
				}
				vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
				E=NULL;delete E;
				//mazání PmG
				if(F->PmG!=NULL)
				{
					F->PmG->Delete();
					F->PmG=NULL;
				}
				//mazání predchozi_PM
				if(F->predchozi_PM!=NULL)
				{
					F->predchozi_PM->mGrid->Delete();
					F->predchozi_PM->mGrid=NULL;
				}
				//mazání mGridu pro teploměry
				if(F->OBJEKT_akt->teplomery!=NULL)
				{
					TTeplomery *T=vrat_teplomery_podle_zakazky(F->OBJEKT_akt,ZAKAZKA_akt);
					if(T!=NULL && T->posledni->mGrid!=NULL)
					{
						T->posledni->mGrid->Delete();
						T->posledni->mGrid=NULL;
          }
        }
			}  
			////mazání dat starého projektu, které budou nahrazeny
			vymaz_seznam_OBJEKTY();
			hlavicka_OBJEKTY();//nutné po mazání!!!
			vymaz_seznam_ELEMENTY();
			hlavicka_ELEMENTY();//nutné po mazání!!!
			F->predchozi_PM=NULL;
			ZAKAZKA_akt=NULL;

			////mazání ukazatelů
      F->pom=NULL;//musí zde být, může působit problémy při vykreslení

			////načtení Objektů
			TObjekt *dO=obraz->Objekty->dalsi,*O=NULL;
			while(dO!=NULL)
			{
				//vytvoření nového objektu
				O=new TObjekt;   
				kopiruj_objekt(dO,O);    
				O->element=NULL;//slouží pro následnou aktualizaci v metodě vloz_element();
				//vložení nového objektu do spojáku
				O=vloz_objekt(O);
				O->n=dO->n;
	  		//ukazatelové záležitosti
	  		O=NULL;delete O;
				dO=dO->dalsi;
			}
			delete dO;dO=NULL;
			if(obraz->edit_Objekt!=0 && !storno)
			{
				F->OBJEKT_akt=vrat_objekt(obraz->edit_Objekt);
        F->pom=F->OBJEKT_akt;
			}

			////načtení Elementů
			pocet_vyhybek=obraz->pocet_vyhybek;//navrácení počtu výhybek,musí být před vytvářením tabulky průchodu
			TElement *dE=obraz->Elementy->dalsi,*E=NULL;
			T2Element *tab_pruchodu=vytvor_tabElm_pruchodu();
			while(dE!=NULL)
			{
				//kopírování atributů
				E=new TElement;
				kopiruj_element(dE,E);
				//vložení do seznamu ELEMENTY
				sekvencni_zapis_cteni(E,NULL,tab_pruchodu);
				if(F->OBJEKT_akt !=NULL && E->objekt_n==F->OBJEKT_akt->n && !storno)
				{
					E->mGrid=new TmGrid(F);
					E->mGrid->Tag=6;//ID formu
					E->mGrid->ID=E->n;
					F->design_element(E,false,true);
					if(E->eID==200 || E->eID==300)exituje_tab_poh=true;
				}
				//vloz_element(E);
				//ukazatelové záležitosi
				E=NULL;delete E;
				dE=dE->dalsi;
			}
			delete dE;dE=NULL;
			delete []tab_pruchodu;tab_pruchodu=NULL;vyhybka_pom=NULL;

			////elementy vloženy, aktualizace ukazatelů Objekt->element
			O=OBJEKTY->dalsi;
			while(O!=NULL)
			{
				O->element=vrat_element(O->element_n);
				O=O->dalsi;
			}
      delete O;O=NULL;
		}

		////aktualizace pohonů
		TPohon *p=POHONY->dalsi,*dp=obraz->Pohony->dalsi;
		while(p!=NULL && dp!=NULL)
		{
			//načtení atributů
			p->n=dp->n;
			p->name=dp->name;
			p->rychlost_od=dp->rychlost_od;
			p->rychlost_do=dp->rychlost_do;
			p->aRD=dp->aRD;
			p->roztec=dp->roztec;
      p->roztec_ID=dp->roztec_ID;
			p->Rz=dp->Rz;
			p->Rx=dp->Rx;
			p->retez=dp->retez;
			//ukazatelové záležitosti
			p=p->dalsi;
			dp=dp->dalsi;
		}
		delete p;p=NULL;
		delete dp;dp=NULL;
    //aktualizace režimu pohonů
		aktualizuj_rezim_pohonu();

		//načtení cest do zakázek, nnutno aktualizovat
		if(ZAKAZKY!=NULL && ZAKAZKY->dalsi!=NULL && obraz->Z_cesty->dalsi!=NULL)
		{
			TZakazka *Z=ZAKAZKY->dalsi;
			TZakazka_uloz *Z_u=obraz->Z_cesty->dalsi;
			while(Z!=NULL)
			{
				inicializace_cesty(Z);
				TCesta *c=NULL;
		  	TCesta_uloz *c_u=Z_u->cesta->dalsi;
				while(c_u!=NULL)
		  	{
		  		c=new TCesta;
					c->n=c_u->n;
		  		c->Element=NULL;
		  		if(c_u->element_n!=0)c->Element=vrat_element(c_u->element_n);
					c->sparovany=NULL;
		  		if(c_u->sparovany_n!=0)c->sparovany=vrat_element(c_u->sparovany_n);
		  		kopiruj_data_elementu(c_u->data,c);

		  		c->dalsi=NULL;
		  		c->predchozi=Z->cesta->predchozi;
					Z->cesta->predchozi->dalsi=c;
					Z->cesta->predchozi=c;
		  		c=NULL;delete c;
					c_u=c_u->dalsi;
		  	}
				delete c_u;c_u=NULL;
				Z=Z->dalsi;
				Z_u=Z_u->dalsi;
			}
			delete Z;Z=NULL;
			delete Z_u;Z_u=NULL;
		}
    update_akt_zakazky();//případný update zakázky v hlavičce

		//aktualizace dat
    //aktualizace Objekt->element ukazatele, a aktualizace ukazatelů teploměrů
		TObjekt *O=OBJEKTY->dalsi;
		while(O!=NULL)
		{
			if(O->teplomery!=NULL)
			{
				unsigned long n;
				TTeplomery *T=O->teplomery->dalsi;
				while(T!=NULL)
				{
					//spárovaný element prvního teploměru
					n=T->prvni->sparovany->n;
					delete T->prvni->sparovany;T->prvni->sparovany=NULL;
					T->prvni->sparovany=vrat_element(n);
					//spárovaný element posledního teploměru
					n=T->posledni->sparovany->n;
					delete T->posledni->sparovany;T->posledni->sparovany=NULL;
					T->posledni->sparovany=vrat_element(n);
					//elementy v cestě
					TCesta *C=T->cesta->dalsi;
					while(C!=NULL)
					{
						//vrácení ukazatele na element
						n=C->Element->n;
						delete C->Element;C->Element=NULL;
						C->Element=vrat_element(n);
						C=C->dalsi;
					}
					delete C;C=NULL;
					//posun na další záznam teploměrů
					T=T->dalsi;
				}
				delete T;T=NULL;
			}
			//posun na další objekt
			O=O->dalsi;
		}
		delete O;O=NULL;

		//sparovaných ukazatelu a RT
		aktualizuj_sparovane_ukazatele();//vždy!!!!
		if(F->OBJEKT_akt!=NULL && !storno)
		{
			//aktualizace předchozího PM
			if(F->OBJEKT_akt->element->predchozi->n>0)F->predchozi_PM=najdi_posledni_element_podle_eID(200,vrat_objekt(F->OBJEKT_akt->element->predchozi->objekt_n));
			if(F->predchozi_PM!=NULL)
			{
				F->predchozi_PM->mGrid=new TmGrid(F);
				F->predchozi_PM->mGrid->Tag=6;//ID formu
				F->predchozi_PM->mGrid->ID=F->predchozi_PM->n;
				F->design_element(F->predchozi_PM,false);//znovuvytvoření tabulek
				exituje_tab_poh=true;//pohonová tabulka v editaci bude exitovat
			}
			//znovu vytvoření tabulky pohonů pokud je třeba
			F->vytvoreni_tab_pohon(exituje_tab_poh);
      F->vytvor_aktualizuj_tab_teplomeru();//znovuvytvoření tabulky teploměrů
		}

		F->Timer_backup->Enabled=true;//obnovení timeru pro backup, nespouští se!
		if(storno)
		{
			pozice_data=0;//vrácení pozice na default hodnotu
			vymaz_seznam_DATA();//smazání nepotřebných dat
			hlavicka_DATA();
		}
	}  
}
//---------------------------------------------------------------------------
//vrátí obraz podle jeho n
Cvektory::TDATA *Cvektory::vrat_obraz_DATA(unsigned long n)
{
	TDATA *D=DATA;
	while(D!=NULL)
	{
		if(D->n==n)break;
		D=D->dalsi;
	}
	return D;
}
//---------------------------------------------------------------------------
//smaže konkrétní obraz
void Cvektory::smaz_obraz_DATA(unsigned long n)
{
	////vyhledání obrazu odpovídajícímu n
	TDATA *obraz=vrat_obraz_DATA(n);

	//vytovoření prázdného na místo hlavičky
	if(n==0)
	{
		TDATA *prazdny=vytvor_prazdny_obraz();
		prazdny->n=0;
		prazdny->dalsi=DATA->dalsi;
		if(DATA->predchozi->n>0)prazdny->predchozi=DATA->predchozi;
		else prazdny->predchozi=prazdny;
		DATA=prazdny;
		//aktualizace n
		prazdny=DATA->dalsi;
		unsigned int n=1;
		while(prazdny!=NULL)
		{
			prazdny->n=n;
			n++;
			prazdny=prazdny->dalsi;
    }
		//smazání ukazatele
		prazdny=NULL;delete prazdny;
	}
	//mazání ostatních
	else
	{
		//deklarace pomocné proměnné
		bool preindexovat=false;
		if(obraz->n<DATA->predchozi->n)preindexovat=true;
		//nové ukazatelové propojení
		obraz->predchozi->dalsi=obraz->dalsi;
		if(obraz->dalsi!=NULL)obraz->dalsi->predchozi=obraz->predchozi;
		else DATA->predchozi=obraz->predchozi;
		//pokud bude odmazán obraz, který není na konci -> nové indexy
		if(preindexovat)
		{
			TDATA *D=DATA->dalsi;
			unsigned long index=1;
	  	while(D!=NULL)
			{
	  		D->n=index;
				index++;
	  		D=D->dalsi;
	  	}
			delete D;D=NULL;
		}
	}

	////mazání dat v obrazu, objekty, elementy, ...
	smaz_data_obrazu(obraz);
	
	////smazání obrazu
	delete obraz;obraz=NULL;
}
//---------------------------------------------------------------------------
//mazání dat v obrazu, objekty, elementy, ...
void Cvektory::smaz_data_obrazu(TDATA *obraz)
{
	////smazání objektů
	while(obraz->Objekty!=NULL)
	{
		vymaz_body(obraz->Objekty->predchozi);
		vymaz_komory(obraz->Objekty->predchozi);
		vymaz_seznam_teplomery(obraz->Objekty->predchozi,false);//vymazat i s elementy
		delete obraz->Objekty->predchozi;
		obraz->Objekty->predchozi=NULL;
		obraz->Objekty=obraz->Objekty->dalsi;
	};
	////smazání elementů
	while(obraz->Elementy!=NULL)
	{
		obraz->Elementy->predchozi=NULL;
		delete obraz->Elementy->predchozi;
		obraz->Elementy=obraz->Elementy->dalsi;
	};
	////smazání pohonů
	while(obraz->Pohony!=NULL)
	{
		delete obraz->Pohony->predchozi;
		obraz->Pohony->predchozi=NULL;
		obraz->Pohony=obraz->Pohony->dalsi;
	};
	////smazání Z_cesta
	while(obraz->Z_cesty!=NULL)
	{
		while(obraz->Z_cesty->predchozi->cesta!=NULL)
		{
			delete obraz->Z_cesty->predchozi->cesta->predchozi;
			obraz->Z_cesty->predchozi->cesta->predchozi=NULL;
			obraz->Z_cesty->predchozi->cesta=obraz->Z_cesty->predchozi->cesta->dalsi;
		}
		delete obraz->Z_cesty->predchozi;
		obraz->Z_cesty->predchozi=NULL;
		obraz->Z_cesty=obraz->Z_cesty->dalsi;
	};
}
//---------------------------------------------------------------------------
//smaže kompletní seznam DATA
long Cvektory::vymaz_seznam_DATA()
{
	long pocet_smazanych_DAT=0;
	while (DATA!=NULL)
	{
		pocet_smazanych_DAT++;
		if(DATA->predchozi->n>0)smaz_data_obrazu(DATA->predchozi);
		delete DATA->predchozi;
		DATA->predchozi=NULL;
		DATA=DATA->dalsi;
	};

	//zakázání tlačítek pro undo a redo, pokud josu povolena
	if(F->scGPGlyphButton_undo->Enabled)F->scGPGlyphButton_undo->Enabled=false;
	if(F->scGPGlyphButton_redo->Enabled)F->scGPGlyphButton_redo->Enabled=false;

	return pocet_smazanych_DAT;
}
////---------------------------------------------------------------------------
//provádí operace s obrazem na vlákně
void Cvektory::vlakno_obraz()
{
	switch(F->vlakno_akce)
	{
		//vytvoření obrazu pro UNDO
		case 1:
		{
			vytvor_obraz_DATA();
			F->vlakno_obraz->Terminate();
			F->vlakno_obraz->Free();
			F->vlakno_obraz=NULL;delete F->vlakno_obraz;
			break;
		}
		//vytvoření obrazu pro UNDO + storno
		case 2:
		{
			vytvor_obraz_DATA(true);
			vytvor_obraz_DATA();
      F->vlakno_obraz->Terminate();
			F->vlakno_obraz->Free();
			F->vlakno_obraz=NULL;delete F->vlakno_obraz;
			break;
		}
		//vymazání seznamu obrazů
		case 3:
		{
			vymaz_seznam_DATA();
			hlavicka_DATA();
			break;
		}
		default:break;
	}
	F->vlakno_akce=0;//navrácení do default stavu
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//MAGNETICKÉ LASO
//vytvoří novou hlavičku pro magnetické laso
void Cvektory::hlavicka_MAG_LASO()
{
	TCesta *novy=new TCesta;
	novy->n=0;
	novy->Element=NULL;
	novy->sparovany=NULL;
	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	MAG_LASO=novy;

	//ukazatelové záležitosti
	novy=NULL;delete novy;
}
////---------------------------------------------------------------------------
//smaže kompletní seznam pro magnetické laso
long Cvektory::vymaz_seznam_MAG_LASO()
{
	long pocet_smazanych_segmentu=0;
	while (MAG_LASO!=NULL)
	{
		pocet_smazanych_segmentu++;
		if(MAG_LASO->predchozi->Element!=NULL && MAG_LASO->predchozi->Element->n==MaxInt)//vymaže virtuální elementy
		{
			delete MAG_LASO->predchozi->Element;
			MAG_LASO->predchozi->Element=NULL;
		}
		MAG_LASO->predchozi=NULL;
		delete MAG_LASO->predchozi;
		MAG_LASO=MAG_LASO->dalsi;
	};

	return pocet_smazanych_segmentu;
}
////---------------------------------------------------------------------------
//provede kontrolu zda může být element vložen do seznamu mag. laso, pokud ano vloží ho
void Cvektory::kontrola_vlozeni_do_mag_lasa(TElement *E)
{
	//pokud byla zapčata cesta ukládání již pročlých segmentů
	if(MAG_LASO->Element!=NULL && E!=NULL)
	{
		//zjištění zda se element již nevyskytuje v mag- lasu
		short segment=obsahuje_MAG_LASO_element(E);

		/////////měření po trendu
		//element není obsažen v magnetickém lasu a je to následující element v cestě, nepořítam se spojkou v mag. lasu
		if(segment==0 && (MAG_LASO->predchozi->sparovany==E->predchozi || (E->predchozi!=NULL && E->predchozi->n>0 && E->predchozi->predchozi==MAG_LASO->predchozi->Element)))
			vloz_segment_MAG_LASA(E->predchozi);

		//kontrola zda není předchozí spojka, vedlejší větev
		if(segment==0 && E->predchozi->eID==301 && E->predchozi->predchozi2->predchozi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->predchozi->predchozi2);

		//kontrola zda není předchozí spojka, hlavní větev
		if(segment==0 && E->predchozi->eID==301 && E->predchozi->predchozi->predchozi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->predchozi->predchozi);

		//kontrola zde předchozí předchozí není spojka, vedlejší větev
		if(segment==0 && E->predchozi->n>0 && E->predchozi->predchozi->eID==301 && E->predchozi->predchozi->predchozi2==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->predchozi);

		//kontrola zde předchozí předchozí není spojka, hlavní větev
		if(segment==0 && E->predchozi->n>0 && E->predchozi->predchozi->eID==301 && E->predchozi->predchozi->predchozi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->predchozi);

		//kontrola zda se nenechází element bez geometrie uprostřed měření
		if(segment==0 && E->predchozi->geo.delka==0 && E->predchozi->n>2 && E->predchozi->predchozi->predchozi==MAG_LASO->predchozi->Element)
		{
			vloz_segment_MAG_LASA(E->predchozi);
			vloz_segment_MAG_LASA(E->predchozi->predchozi);
		}

		//kontrola zda se nenechází element bez geometrie na začátku měření
		if(segment==0 && E->predchozi->geo.delka==0 && E->predchozi->n>1 && E->predchozi->predchozi==MAG_LASO->predchozi->sparovany)
		{
			vloz_segment_MAG_LASA(MAG_LASO->predchozi->sparovany);
			vloz_segment_MAG_LASA(E->predchozi);
		}

		/////////měření proti trendu
		//začátek + obecně na hlavní větvi
		if(segment==0 && ((MAG_LASO->predchozi->sparovany==E->dalsi && E->dalsi!=NULL) || (E->dalsi!=NULL && E->dalsi->n>0 && E->dalsi->eID!=301 && E->dalsi->dalsi==MAG_LASO->predchozi->Element)))
			vloz_segment_MAG_LASA(E->dalsi);

		//začátek měření přes spojku
		if(segment==0 && MAG_LASO->dalsi==NULL && E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->dalsi==MAG_LASO->predchozi->sparovany)
			vloz_segment_MAG_LASA(E->dalsi);

		//z hlavnní větve na vedlejší přes spojku
		if(segment==0 && E->dalsi!=NULL && E->dalsi->dalsi!=NULL && E->dalsi->dalsi->eID==301 && E->dalsi->dalsi->dalsi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->dalsi);

		//návrat z vedlejší větve na hlavní přes výhybku
		if(segment==0 && E->eID==300 && E->dalsi2->dalsi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->dalsi2);

		//předchozí výhybka, předchozí v ceste vedlejší větev
		if(segment==0 && E->dalsi!=NULL && E->dalsi->eID==300 && E->dalsi->dalsi2==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->dalsi);

		//z hlavní větve na hlavní přes spojku
		if(segment==0 && E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->dalsi!=NULL && E->dalsi->dalsi->dalsi==MAG_LASO->predchozi->Element)
			vloz_segment_MAG_LASA(E->dalsi->dalsi);

		//začátek na vedlejší větvi, za spojkou
		if(segment==0 && E->eID==300 && E->dalsi2!=NULL && E->dalsi2==MAG_LASO->predchozi->sparovany)
			vloz_segment_MAG_LASA(E->dalsi2);

		 //kontrola zda se nenechází element bez geometrie uprostřed měření
		if(segment==0 && E->dalsi!=NULL && E->dalsi->geo.delka==0 && E->dalsi->dalsi!=NULL && E->dalsi->dalsi->dalsi==MAG_LASO->predchozi->Element)
		{
			vloz_segment_MAG_LASA(E->dalsi);
			vloz_segment_MAG_LASA(E->dalsi->dalsi);
		}

		//kontrola zda se nenechází element bez geometrie na začátku měření
		if(segment==0 && E->dalsi!=NULL && E->dalsi->geo.delka==0 && E->dalsi->dalsi==MAG_LASO->predchozi->sparovany)
			vloz_segment_MAG_LASA(E->dalsi);

		/////////mazání obsaženého
		//element je již obsazen v seznamu magnetického lasa, bude smazán segment cesty obsahující E, taktéž budou smazány následující segmenty cesty, pokud existují další segmenty
		if(segment>0)
			smaz_segment_MAG_LASA(E);
	}
}
////---------------------------------------------------------------------------
//vloží nový segment do magnetického lasa
void Cvektory::vloz_segment_MAG_LASA(TElement *E)
{
	//ukazatelové vytvoření a vložení segmentu
	TCesta *novy=new TCesta;
	novy->n=MAG_LASO->predchozi->n+1;
	novy->dalsi=NULL;
	novy->predchozi=MAG_LASO->predchozi;
	MAG_LASO->predchozi->dalsi=novy;
	MAG_LASO->predchozi=novy;

	//parametry segmentu
	novy->Element=E;
	novy->sparovany=NULL;

  //ukazatelové záležitosti
	novy=NULL;delete novy;
}
////---------------------------------------------------------------------------
//smaže segmenty z mag. lasa
void Cvektory::smaz_segment_MAG_LASA(TElement *E)
{
	//hledání mazaného segmentu
  bool ukoncit=false;
	TCesta *smazat=NULL;
	while(MAG_LASO->dalsi!=NULL)//nemazat až na hlavičku, ošrtření
	{
    //nastavení vstupních atributů
		smazat=MAG_LASO->predchozi;

		//kontrola zda nejsem na E, pokud ano ukoncit po jeho smazání
		if(smazat->Element==E)ukoncit=true;

		//odstranění ze spojáku
		smazat->predchozi->dalsi=NULL;
		MAG_LASO->predchozi=smazat->predchozi;

		//smazání segmentu
		delete smazat;smazat=NULL;

		//ukončení po smazání segmentu, který obsahuje E
		if(ukoncit)break;
	}
}
////---------------------------------------------------------------------------
//"přilepování" souřadnic na gaometrii linky, linie i oblouky
TPointD Cvektory::bod_na_geometrii(TElement *E)
{
	//deklarace
	TPointD ret;
	ret.x=F->akt_souradnice_kurzoru.x;ret.y=F->akt_souradnice_kurzoru.y;

  //ošetření proti prázdnému ukazateli
	if(E!=NULL)
	{
  	//přichytávání bodu na linii
  	if(E->geo.typ==0 && (E->geo.orientace==m.Rt90(E->geo.orientace) || E->geo.orientace==360))//jen pro přímky 0,90,180,270°
		{
  		//přiřazení souřadnic pro vložení
  		if(E->geo.orientace==90 || E->geo.orientace==270){ret.x=F->akt_souradnice_kurzoru.x;ret.y=E->geo.Y1;}
  		else {ret.x=E->geo.X1;ret.y=F->akt_souradnice_kurzoru.y;}
		}

  	//přichytávání bodu na oblouk
  	if(E->geo.typ!=0)
		{
  		double uhel=m.uhelObloukuVsMys(E->geo.X1,E->geo.Y1,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
  		TPointD *souradnice=m.getArcLine(E->geo.X1,E->geo.Y1,E->geo.orientace,uhel,E->geo.radius);
  		ret=souradnice[3];
		}
	}

	//navracení souřadnic
	return ret;
}
////---------------------------------------------------------------------------
//kontrola zda spoják magnetického lasa obsahuje segment s danným elementem
short Cvektory::obsahuje_MAG_LASO_element(TElement *E)
{
	//deklarace
	short ret=0;

  //ošetření proti prázdnému ukazateli
	if(E!=NULL)
	{
  	//hledání zda je element v mag. lasu
  	TCesta *segment=MAG_LASO->dalsi;
  	while(segment!=NULL)
  	{
  		if(segment->Element==E)
  		{
  			ret=segment->n;
  			break;
  		}
  		segment=segment->dalsi;
  	}

  	//ukazatelové záležitosti
		segment=NULL;delete segment;
	}

  //návrat zda byl nalezen nebo ne
	return ret;
}
////---------------------------------------------------------------------------
//zkontroloje, zda existují vrátka objektu a jestli jsem kurzorem v jejich oblasti, pokud ano vrátí bod, pokud ne vrátí [-MaxInt,-MaxInt]
TPointD Cvektory::InVrata(TElement *E,bool kontrola_kurzoru)
{
	//deklarace
	TPointD ret;
	ret.x=ret.y=-1*MaxInt;//defaultní stav nenalezeno
	TObjekt *O=vrat_objekt(E->objekt_n);
	//pouze kontrolovat u objektů se stěnami a má body, pouze pro liniovou geometrii objektu
	if(E->geo.typ==0 && O!=NULL/* && O->sirka_steny>0*/ && O->body!=NULL && O->body->dalsi!=NULL)
	{
		//deklarace parametrů úseček
		TPointD P;
		double xs1=E->geo.X1;double ys1=E->geo.Y1;double xk1=E->geo.X4;double yk1=E->geo.Y4;//úsečka z geometrie kontrolovaného elementu
		double xs2,ys2,xk2,yk2;
		//průchod skrze hrany objektu
		TBod *A=O->body->dalsi,*B=NULL;
		while(A!=NULL)
		{
			//nastavení druhé úsečky
			B=A->dalsi;
			if(B==NULL)B=O->body->dalsi;
			xs2=A->X;ys2=A->Y;xk2=B->X;yk2=B->Y;
			//kontrola zda existuje průsečík
			P=m.PrusecikPrimek(xs1,ys1,xk1,yk1,xs2,ys2,xk2,yk2);
			if(!IsNan(P.x) && !IsNan(P.y) && !IsInfinite(P.x) && !IsInfinite(P.y))//prusečík existuje
			{
				//kontrola zda jsem kurzorem v této oblasti, pokud je navoleno, když ne kontrola zda jsem v segmentu elementu, přidána kontrola koncových bodů, ptinsegment měl problém s počátečním bodem
				if((!kontrola_kurzoru && ((P.x==xs1 && P.y==ys1) || (P.x==xk1 && P.y==yk1) || m.LeziVblizkostiUsecky(P.x,P.y,xs1,ys1,xk1,yk1)==0)) || m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,P.x,P.y,F->velikost_citelne_oblasti_elementu))
				{
					ret=P;
					break;
        }
      }
			A=A->dalsi;
		}
		//ukazatelové záležitosti
    A=NULL;delete A;
		B=NULL;delete B;
	}
	//ukazatelové záležitosti
	O=NULL;delete O;
	//vracení výsledku
  return ret;
}
////---------------------------------------------------------------------------
//vytvoří hlavičku v objektu pro teploměru
void Cvektory::hlavicka_teplomery(TObjekt *Objekt)
{
	//vytvoření struktury teploměrů
	TTeplomery *novy=new TTeplomery;

	//efaultní parametry
	novy->n=0;
	novy->Z_n=0;
	novy->prvni=NULL;
	novy->posledni=NULL;
	novy->cesta=NULL;
	novy->dalsi=NULL;
	novy->predchozi=novy;

	//zařazení
	Objekt->teplomery=novy;

  //ukazatelové záležitosti
	novy=NULL;delete novy;
}
////---------------------------------------------------------------------------
//vytvoří v objektu hlavičku pro cestu teploměrů
void Cvektory::hlavicka_cesty_teplomery(TTeplomery *teplomery)
{
	//kontrola a mazání stávající cesty
	if(teplomery->cesta!=NULL)vymaz_seznam_cest(teplomery);

	//vytvoření segmentu
	TCesta *novy=new TCesta;

  //definice atriburů
	novy->n=0;
	novy->Element=NULL;
	novy->sparovany=NULL;
	novy->dalsi=NULL;
	novy->predchozi=novy;

	//zařazení do seznamu
	teplomery->cesta=novy;

  //ukazatelové záležitosti
	novy=NULL;delete novy;
}
////---------------------------------------------------------------------------
//vymaže konkrétní záznam teploměrů, s elementy nebo bez
void Cvektory::vymaz_teplomery(TObjekt *Objekt,TTeplomery *teplomery,bool ponechat_elementy)
{
	//kontrola zda je co mazat
	if(teplomery!=NULL)
	{
		//mazání segmentů cesty
		vymaz_seznam_cest(teplomery,ponechat_elementy);

		//mazání tabulky oblasti, pokud existuje
		if(teplomery->posledni!=NULL && teplomery->posledni->mGrid!=NULL)
		{
			teplomery->posledni->mGrid->Delete();
			teplomery->posledni->mGrid=NULL;
		}

		//mazání teploměrů
		if(!ponechat_elementy)//mazání spárovaných, pokud mají být mazány
		{
			if(teplomery->prvni!=NULL){delete teplomery->prvni->sparovany;teplomery->prvni->sparovany=NULL;}
			if(teplomery->posledni!=NULL){delete teplomery->posledni->sparovany;teplomery->posledni->sparovany=NULL;}
		}
		delete teplomery->prvni;teplomery->prvni=NULL;
		delete teplomery->posledni;teplomery->posledni=NULL;

		//vyřezení záznamu z objektu
		if(teplomery->n!=0)
		{
			if(teplomery->dalsi!=NULL)teplomery->dalsi->predchozi=teplomery->predchozi;
			else Objekt->teplomery->predchozi=teplomery->predchozi;
			teplomery->predchozi->dalsi=teplomery->dalsi;
		}
		else Objekt->teplomery=NULL;

		//smazání záznamu
		delete teplomery;teplomery=NULL;
	}
}
////---------------------------------------------------------------------------
//vymaže seznam cest z teplomerů, s elementy nebo bez
void Cvektory::vymaz_seznam_cest(TTeplomery *teplomery,bool ponechat_elementy)
{
  //mazání segmentů cesty
	while(teplomery->cesta!=NULL)
	{
		//mazání elementů pokud mají být mazány
		if(!ponechat_elementy)
		{
			delete teplomery->cesta->predchozi->Element;
      teplomery->cesta->predchozi->Element=NULL;
    }
		//mazání segmentu
		delete teplomery->cesta->predchozi;
		teplomery->cesta->predchozi=NULL;
		teplomery->cesta=teplomery->cesta->dalsi;
	}
	//mazání hlavičky a nulování ukazatele
	delete teplomery->cesta;teplomery->cesta=NULL;
}
////---------------------------------------------------------------------------
//vymaže seznam teploměrů z objektu, s elementy nebo bez
void Cvektory::vymaz_seznam_teplomery(TObjekt *Objekt,bool ponechat_elementy)
{
	//kontrola zda je co mazat
	if(Objekt!=NULL && Objekt->teplomery!=NULL)
	{
		//mazání záznamů teploměru
		while(Objekt->teplomery!=NULL)
		{
			vymaz_teplomery(Objekt,Objekt->teplomery->predchozi,ponechat_elementy);
		}
		//smazání hlavičky a nulování ukazatele
		delete Objekt->teplomery;Objekt->teplomery=NULL;
	}
}
////---------------------------------------------------------------------------
//vrátí ukazatel na záznam teploměrů pro konkrétní zakázku
Cvektory::TTeplomery *Cvektory::vrat_teplomery_podle_zakazky(TObjekt *Objekt,TZakazka *Zakazka)
{
	//deklarace
	TTeplomery *ret=NULL,*T=NULL;

	//kontrola zda existují teploměry a Zakázka
	if(Objekt->teplomery!=NULL && Zakazka!=NULL)
	{
    //hledání zda existují teploměry pro Zakázku
		T=Objekt->teplomery->dalsi;
		while(T!=NULL)
		{
			//pokud ano, ulož uukazatel a zastav průchod
			if(T->Z_n==Zakazka->n){ret=T;break;}
			T=T->dalsi;
		}
		//ukazatelové záležitosti
		T=NULL;delete T;
  }

  //vracení výsledku
  return ret;
}
////---------------------------------------------------------------------------
//vytvoří záznam teploměrů pro zakázku
Cvektory::TTeplomery *Cvektory::vytvor_zaznam_teplomeru_pro_zakazku(TObjekt *Objekt,TZakazka *Zakazka)
{
  //vytvoření záznamu
	TTeplomery *novy=new TTeplomery;

  //kontrola zda existuje hlavička teploměrů
	if(Objekt->teplomery==NULL)hlavicka_teplomery(Objekt);

	//nastavení defaultních dat
	novy->n=Objekt->teplomery->predchozi->n+1;
	novy->Z_n=Zakazka->n;
	novy->prvni=NULL;
	novy->posledni=NULL;
  novy->cesta=NULL;
	hlavicka_cesty_teplomery(novy);

  //zařazení do spojáku teploměrů v objektu
	Objekt->teplomery->predchozi->dalsi=novy;
	novy->predchozi=Objekt->teplomery->predchozi;
	novy->dalsi=NULL;
	Objekt->teplomery->predchozi=novy;

	//vracení vytvořeného záznamu
  return novy;
}
////---------------------------------------------------------------------------
//vloží segment cesty do cest teploměrů v objektu, nebo vložení teploměru, doplnění bodu vložení
void Cvektory::vloz_segment_cesty_teplomery(TObjekt *Objekt,TElement *Element,bool prvni,bool posledni,double X,double Y)
{
	//pokud objekt nemá cestu vytvoří jí
	if(Objekt->teplomery==NULL)hlavicka_teplomery(Objekt);
	TTeplomery *teplomery=vrat_teplomery_podle_zakazky(Objekt,ZAKAZKA_akt);
	if(teplomery==NULL)teplomery=vytvor_zaznam_teplomeru_pro_zakazku(Objekt,ZAKAZKA_akt);

	//vytvoření nového vkládáného záznamu a vložení
	unsigned int eID;
	switch(Objekt->id)
	{
		case 6:eID=402;break;//"vytěkání"
		case 7:eID=400;break;//"sušení"
		case 8:eID=401;break;//"chlazení"
		default:eID==400;break;
	}
	vloz_segment_cesty_do_seznamu_cesty(teplomery,Element,prvni,posledni,eID,X,Y);
}
////---------------------------------------------------------------------------
//vloží segment cesty do jakéhokoliv seznamu
void Cvektory::vloz_segment_cesty_do_seznamu_cesty(TTeplomery *teplomery,TElement *Element,bool prvni,bool posledni,unsigned int eID,double X,double Y)
{
	////vkládání teploměru
	if(prvni || posledni)
	{
		//nastavení default teploty
		String name="";
		//vytvoření elementu a načtení atributů
		TElement *E=new TElement;
		E->n=MaxInt;
		E->name=name;
		E->objekt_n=Element->objekt_n;
		E->mGrid=NULL;
		E->pohon=NULL;
		E->geo=Element->geo;//kopírování všech parametrů geometrie
		E->Xt=E->X=E->geo.X1=E->geo.X4=X;
		E->Yt=E->Y=E->geo.Y1=E->geo.Y4=Y;
		E->eID=eID;
		E->sparovany=Element;
		E->dalsi=NULL;
		E->dalsi2=NULL;
		E->predchozi==NULL;
		E->predchozi2=NULL;
		//uložení do struktury teploměrů
		if(prvni)teplomery->prvni=E;
		if(posledni)teplomery->posledni=E;
    //ukazatelové záležitosti
		E=NULL;delete E;
	}

  ////vkládání segmentu cesty
	else
	{
    //vytvoření nove vkládáného záznamu
  	TCesta *novy=new TCesta;

  	//definice atributů
  	novy->n=teplomery->cesta->predchozi->n+1;
  	novy->Element=Element;
  	novy->sparovany=NULL;
		novy->dalsi=NULL;

    //zařazení do seznamu
  	novy->predchozi=teplomery->cesta->predchozi;
  	teplomery->cesta->predchozi->dalsi=novy;
  	teplomery->cesta->predchozi=novy;

  	//ukazatelové záležitosti
		novy=NULL;delete novy;
  }
}
////---------------------------------------------------------------------------
//vytvoří 2 teploměry a defaultní cestu mezi nimi
void Cvektory::vytvor_default_c_teplomery(TObjekt *Objekt)
{
	//v případě že již existuje vymazat
	TTeplomery *T=vrat_teplomery_podle_zakazky(Objekt,ZAKAZKA_akt);
	if(T!=NULL)vymaz_teplomery(Objekt,T);
  update_akt_zakazky();//důležité mít aktuální data

	//hledání vrátek
	TCesta *CE=vrat_segment_cesty(ZAKAZKA_akt,Objekt->element),*CprvniE=NULL,*CposledniE=NULL;
	//pokud není první element oběktu v cestě, kontrola ostatních zda jsou v cestě
	if(CE==NULL && Objekt->element->dalsi!=NULL && Objekt->element->dalsi->objekt_n==Objekt->n)
	{
		TElement *E=Objekt->element->dalsi;
		while(CE==NULL && E!=NULL && E->objekt_n==Objekt->n)
		{
			CE=vrat_segment_cesty(ZAKAZKA_akt,E);
      E=E->dalsi;
		}
    E=NULL;delete E;
  }
  //kontrola, zda existuje první element v cestě, pokud ne nic nevytvářet
	if(CE!=NULL)
	{
  	TPointD P,prvni,posledni;
  	int nalezeno=0;
  	while(CE!=NULL && CE->Element->objekt_n==Objekt->n)
  	{
  		//hledání průsečíku s kabinou objektu
  		P=InVrata(CE->Element,false);
  		if(P.x!=-1*MaxInt && P.y!=-1*MaxInt)//pokud byl nalezen
  		{
  			nalezeno++;//počet nalezených bodů
  			if(nalezeno==1)//pokud se jedná o první nalezený bod, zapíše ho do prvního
  			{
  				prvni=P;
  				CprvniE=CE;//ukládání ukazatele
  				//pokud je v objektu pouze jeden element, dochází k problémům při hledání vrátek, work
  				if(CE->dalsi==NULL || (CE->dalsi!=NULL && CE->dalsi->Element->objekt_n!=Objekt->n)){prvni.x=CE->Element->geo.X1;prvni.y=CE->Element->geo.Y1;}
  			}
  			else//každý další se ukládá do posledního
  			{
  				posledni=P;//zapsání každého nalezeného bodu do posledního
  				CposledniE=CE;//ukládání ukazatele
  			}
  		}

  		//ukazatelové záležitosti
  		if(CE->dalsi==NULL || (CE->dalsi!=NULL && CE->dalsi->Element->objekt_n!=Objekt->n))break;//přerušení průchodu na posledním elementu
  		CE=CE->dalsi;//procházení pouze po hlavní větvi, "hlavní" vrátka
  	}

  	//kontrola zda existují vrátka + řešení
  	if(CprvniE==NULL)//pokud nebyl nalezen
		{
  		CprvniE=vrat_segment_cesty(ZAKAZKA_akt,Objekt->element);//defaultně vezmu první element v kabině
  		prvni.x=CprvniE->Element->geo.X1;prvni.y=CprvniE->Element->geo.Y1;//první bod geometrie tohoto elementu
  	}
  	if(CposledniE==NULL)//pokud nebyl nalezen
		{
			CposledniE=CE;//defaultně poslední element v kabině
			posledni.x=CposledniE->Element->geo.X4;posledni.y=CposledniE->Element->geo.Y4;//první bod geometrie tohoto elementu
		}

		//vkládání teploměrů a cesty mezi nimi
		vloz_segment_cesty_teplomery(Objekt,CprvniE->Element,true,false,prvni.x,prvni.y);
		//kontrola zda se první a poslední element rovná
		if(CprvniE==CposledniE)vloz_segment_cesty_teplomery(Objekt,CposledniE->Element,false,true,posledni.x,posledni.y);
		//uložení cesty
  	else
		{
			CE=CprvniE->dalsi;
  		while(CE!=NULL)
    	{
  			//vkládání segmentů cesty a druhého teploměru
				if(CE==CposledniE){vloz_segment_cesty_teplomery(Objekt,CposledniE->Element,false,true,posledni.x,posledni.y);break;}
				else vloz_segment_cesty_teplomery(Objekt,CE->Element);
    		CE=CE->dalsi;
  		}
  	}
	}

	//ukazatelové záležitosti
	CE=NULL;delete CE;
	CposledniE=NULL;delete CposledniE;
	CprvniE=NULL;delete CprvniE;
}
////---------------------------------------------------------------------------
//hledá zda není uživatel kurzorem nad teploměrem, pokud ano zapíše ukazatel na něj do F->pom_element a vrátí 1 ... teplomer, 2 ... popisek, nebo 0 ... nenalezeno
short Cvektory::najdi_teplomer()
{
	//deklarace
	short ret=0;

	//hledání zda jsem v oblasti nějakého teploměru, pokud existuje cesta a teploměry
	if(F->OBJEKT_akt!=NULL)
	{
    //kontrola těla teploměrů
		TTeplomery *teplomery=vrat_teplomery_podle_zakazky(F->OBJEKT_akt,ZAKAZKA_akt);
		if(teplomery!=NULL && teplomery->prvni->sparovany!=NULL && teplomery->posledni->sparovany!=NULL)
		{
			//kontrola prvního teploměru
			if(m.PtInTeplomer(teplomery->prvni->geo.X1,teplomery->prvni->geo.Y1,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,m.Rt90(teplomery->prvni->sparovany->geo.orientace-teplomery->prvni->sparovany->geo.rotacni_uhel-90)))
			{
				F->pom_element=teplomery->prvni;//nalezeno
				ret=1;
			}
	  	//kontrola druhého teploměru
	  	if(m.PtInTeplomer(teplomery->posledni->geo.X1,teplomery->posledni->geo.Y1,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,m.Rt90(teplomery->posledni->sparovany->geo.orientace-teplomery->posledni->sparovany->geo.rotacni_uhel-90)))
	  	{
				F->pom_element=teplomery->posledni;//nalezeno
				ret=1;
			}

      //kontrola popisků teploměrů
	  	if(ret==0 && F->scGPCheckBox1_popisky->Checked)
	  	{
	  		//pom_element->citelna_oblast.rect3.PtInRect(TPoint(X,Y))
				if(teplomery->prvni->citelna_oblast.rect3.PtInRect(TPoint(F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)) || teplomery->prvni->citelna_oblast.rect4.PtInRect(TPoint(F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)))
	  		{
	  			F->pom_element=teplomery->prvni;//nalezeno
	  			ret=2;
	  		}
				if(teplomery->posledni->citelna_oblast.rect3.PtInRect(TPoint(F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)) || teplomery->posledni->citelna_oblast.rect4.PtInRect(TPoint(F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)))
				{
	  			F->pom_element=teplomery->posledni;//nalezeno
	  			ret=2;
	  		}
			}
		}

    //ukazatelové záležitosti
		teplomery=NULL;delete teplomery;
	}

	//vracení parametru
	return ret;
}
////---------------------------------------------------------------------------
//posunem teploměru dochází k editaci jeho oblasti
void Cvektory::posun_teplomeru(TElement *teplomer)
{
	//kontrola zda existuje teploměr
	if(teplomer!=NULL && F->OBJEKT_akt!=NULL && F->OBJEKT_akt->teplomery!=NULL)
	{
		//načtení elementu v jehož segmentu se nachází kurzor
		TCesta *CE=ZAKAZKA_akt->cesta->dalsi;
		while(CE!=NULL)
		{
      //kontrola přichicení na element
			if(F->prichytavat_k_mrizce==1 && (CE->Element->eID!=MaxInt || (CE->Element->eID==MaxInt && CE->Element->dalsi==NULL)) && m.PtInCircle(F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y,CE->Element->geo.X4,CE->Element->geo.Y4,F->velikost_citelne_oblasti_elementu))
			{
				teplomer->X=CE->Element->geo.X4;
				teplomer->Y=CE->Element->geo.Y4;
        break;
      }
			//kontrola zda nejsme v segmentu aktuálně kontrolovaného elementu
			if(PtInSegment(CE->Element,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))
			{
        //upravení souřadnic, "přilepení" přesně na pohon
				TPointD P=bod_na_geometrii(CE->Element);
				teplomer->X=P.x;
        teplomer->Y=P.y;
				break;
			}
			//posun na další segment cesty
			CE=CE->dalsi;
		}

		//načtení struktury teploměru
		TTeplomery *T=vrat_teplomery_podle_zakazky(F->OBJEKT_akt,ZAKAZKA_akt);

		//kontrola zda sem přesunul teploměr na pohon
		if(CE!=NULL && T!=NULL)
		{
			//hledání přímé cesty
			TElement *prvni=T->prvni->sparovany,*posledni=T->posledni->sparovany;
			if(teplomer==T->prvni)prvni=CE->Element;else posledni=CE->Element;
			bool error=true;
			CE=vrat_segment_cesty(ZAKAZKA_akt,prvni);
			while(CE!=NULL)
			{
				//pokud sem došel na poslední element v cestě ukončit cyklus, zapsat, že cyklus proběhl
				if(CE->Element==posledni){error=false;break;}
        //posun na další element
				CE=CE->dalsi;
			}

			//v cestě se nachází výhybka
			if(error || CE==NULL)
			{
				teplomer->X=F->puv_souradnice.x;
				teplomer->Y=F->puv_souradnice.y;
			}

			//existuje pouze jedna cesta, uložit
			else
			{
				//vytvoření kopii teploměrů
				TElement *t1=NULL,*t2=NULL;
				t1=new TElement;
				t1->n=MaxInt;
				t1->name=T->prvni->name;
				t1->objekt_n=prvni->objekt_n;
				t1->mGrid=NULL;
				t1->pohon=NULL;
				t1->geo=prvni->geo;//kopírování všech parametrů geometrie
				t1->Xt=t1->X=t1->geo.X1=t1->geo.X4=T->prvni->X;
				t1->Yt=t1->Y=t1->geo.Y1=t1->geo.Y4=T->prvni->Y;
				t1->eID=T->prvni->eID;
				t1->sparovany=prvni;
				t1->dalsi=NULL;
				t1->dalsi2=NULL;
				t1->predchozi==NULL;
				t1->predchozi2=NULL;
				//druhý
				t2=new TElement;
				t2->n=MaxInt;
        t2->name=T->posledni->name;
				t2->objekt_n=posledni->objekt_n;
				t2->mGrid=NULL;
				t2->pohon=NULL;
				t2->geo=prvni->geo;//kopírování všech parametrů geometrie
				t2->Xt=t2->X=t2->geo.X1=t2->geo.X4=T->posledni->X;
				t2->Yt=t2->Y=t2->geo.Y1=t2->geo.Y4=T->posledni->Y;
				t2->eID=T->posledni->eID;
				t2->sparovany=posledni;
				t2->dalsi=NULL;
				t2->dalsi2=NULL;
				t2->predchozi==NULL;
				t2->predchozi2=NULL;

				//vymazání původní struktury teploměrů
				vymaz_teplomery(F->OBJEKT_akt,T);
				T=vytvor_zaznam_teplomeru_pro_zakazku(F->OBJEKT_akt,ZAKAZKA_akt);

				//vložení teploměru do struktury
				T->prvni=t1;
				T->posledni=t2;
				t1=NULL;delete t1;
				t2=NULL;delete t2;


				//vkládání cesty
				if(prvni!=posledni)//pokud existuje cesta mezi dvěma elementy
				{
					CE=vrat_segment_cesty(ZAKAZKA_akt,prvni)->dalsi;
					while(CE!=NULL && CE->Element!=posledni)
					{
						//uložení segmentu cesty
						vloz_segment_cesty_do_seznamu_cesty(T,CE->Element);
						CE=CE->dalsi;
					}
				}

				//mGrid posledního teploměru
				F->vytvor_aktualizuj_tab_teplomeru();
			}

			//ukazatelové záležitosti
			prvni=NULL;delete prvni;
			posledni=NULL;delete posledni;
		}
		//vloženo mimo pohon
		else
		{
			teplomer->X=F->puv_souradnice.x;
			teplomer->Y=F->puv_souradnice.y;
    }

		//ukazatelové záležitosti
		T=NULL;delete T;
		CE=NULL;delete CE;
  }
}
////---------------------------------------------------------------------------
//provede kontrolu, zda existuje cesta pro akt zakázku pokud ano, zkontroluje jestli existuje cesta i na nové zakázce, pokud ne vytvoří default cestu od vrátek k vrátkům
void Cvektory::zmena_zakazky_vytvoreni_teplomeru(TObjekt *Objekt,TZakazka *Zakt,TZakazka *Znova)
{
	//kontrola zda mám všechny parametry
	if(Objekt!=NULL && Zakt!=NULL && Znova!=NULL)
	{
		bool vytvor_default=false;
		//kontrola zda existuje cesta pro Zakt
		TTeplomery *T=vrat_teplomery_podle_zakazky(Objekt,Zakt);
		TTeplomery *T2=vrat_teplomery_podle_zakazky(Objekt,Znova);
		if(T!=NULL && T2==NULL)
		{      
			TCesta *CE=NULL,*prvni=NULL,*posledni=NULL;
			prvni=vrat_segment_cesty(Znova,T->prvni->sparovany);
			posledni=vrat_segment_cesty(Znova,T->posledni->sparovany); //if(prvni==NULL)F->Memo("prvni error");if(posledni==NULL)F->Memo("posledni error");
			//kontrola zda existují teploměry v nové zakázkce
			if(prvni!=NULL && posledni!=NULL)
			{        
				//teploměry existují v nové zakázce, hledám cestu mezi nimi
				vytvor_default=true;
				CE=prvni->dalsi;
				while(CE!=NULL)
				{
          //nalezení posledního
					if(posledni==CE){vytvor_default=false;break;}
					CE=CE->dalsi;
				}

				//pokud existuje cesta vytvoří ji
				if(!vytvor_default)
				{
					////vytvoření kopie teploměrů
					T2=vytvor_zaznam_teplomeru_pro_zakazku(Objekt,Znova);					
					//vytvoření kopii teploměrů
					TElement *t1=NULL,*t2=NULL;
					t1=new TElement;
					t1->n=MaxInt;
					t1->name=T->prvni->name;
					t1->objekt_n=prvni->Element->objekt_n;
			  	t1->mGrid=NULL;
			  	t1->pohon=NULL;
					t1->geo=prvni->Element->geo;//kopírování všech parametrů geometrie
					t1->Xt=t1->X=t1->geo.X1=t1->geo.X4=T->prvni->X;
					t1->Yt=t1->Y=t1->geo.Y1=t1->geo.Y4=T->prvni->Y;
					t1->eID=T->prvni->eID;
					t1->sparovany=prvni->Element;
					t1->dalsi=NULL;
					t1->dalsi2=NULL;
					t1->predchozi==NULL;
			  	t1->predchozi2=NULL;
			  	//druhý
					t2=new TElement;
					t2->n=MaxInt;
          t2->name=T->posledni->name;
			  	t2->objekt_n=posledni->Element->objekt_n;
			  	t2->mGrid=NULL;
					t2->pohon=NULL;
					t2->geo=prvni->Element->geo;//kopírování všech parametrů geometrie
					t2->Xt=t2->X=t2->geo.X1=t2->geo.X4=T->posledni->X;
					t2->Yt=t2->Y=t2->geo.Y1=t2->geo.Y4=T->posledni->Y;
					t2->eID=T->posledni->eID;
					t2->sparovany=posledni->Element;
			  	t2->dalsi=NULL;
					t2->dalsi2=NULL;
					t2->predchozi==NULL;
					t2->predchozi2=NULL;
			  	//vložení teploměru do struktury
					T2->prvni=t1;
					T2->posledni=t2;
					t1=NULL;delete t1;
					t2=NULL;delete t2;    
					
					//uložení segmentu cesty
					if(prvni!=posledni)
					{         
						CE=prvni->dalsi;
						while(CE!=NULL)
						{
							//pokud jsem došel na poselední element, ukončít 
							if(CE==posledni)break;
							//vkládání segmentu cesty
							vloz_segment_cesty_do_seznamu_cesty(T2,CE->Element);
							//posun na další 
							CE=CE->dalsi;
						}
					}
				}
			}
			else vytvor_default=true;

			//ukazatelové záležitosti
			CE=NULL;delete CE;
			prvni=NULL;delete prvni;
			posledni=NULL;delete posledni;
		}
		else if(T2==NULL)vytvor_default=true;
		
		//pokud neexistuje vytvořit default
		if(vytvor_default)vytvor_default_c_teplomery(Objekt);

    //ukazatelové záležitosti
		T=NULL;delete T;
		T2=NULL;delete T2;
	}
}
////---------------------------------------------------------------------------
//kopíruje záznamy teploměrů do jiného objektu, pro účely obrazu objektu
void Cvektory::kopiruj_seznam_teplomery(TObjekt *zdroj,TObjekt *cil)
{
	//pokud je cíl plný vymazat a vynulovat
	if(cil->teplomery!=NULL)vymaz_seznam_teplomery(cil);
	cil->teplomery=NULL;

  //pokud jsou ve zdroji teploměry, budou nakopírovány do cíle
	if(zdroj->teplomery!=NULL)
	{
   	hlavicka_teplomery(cil);

   	TTeplomery *T=zdroj->teplomery->dalsi,*Tnovy=NULL;
   	while(T!=NULL)
   	{
   		//vytvoření kopie
   		Tnovy=kopiruj_teplomer(T);
   		//zařazení do cílového objektu
   		cil->teplomery->predchozi->dalsi=Tnovy;
   		Tnovy->predchozi=cil->teplomery->predchozi;
   		cil->teplomery->predchozi=Tnovy;
   		//posun na další teploměry
   		T=T->dalsi;
   		Tnovy=NULL;
   	}

   	//ukazatelové záležitosti
   	T=NULL;delete T;
   	Tnovy=NULL;delete Tnovy;
	}
}
////---------------------------------------------------------------------------
//vytvoří kopii z originálního záznamu teploměrů
Cvektory::TTeplomery *Cvektory::kopiruj_teplomer(TTeplomery *original)
{
	TTeplomery *novy=new TTeplomery;

	//základní parametry
	novy->n=original->n;
	novy->Z_n=original->Z_n;
	novy->dalsi=NULL;
	novy->predchozi=NULL;
  novy->cesta=NULL;
	hlavicka_cesty_teplomery(novy);
								 
	//kopírování segmentů cesty
	TCesta *CE=original->cesta->dalsi;
	TElement *Ekop=NULL;
	while(CE!=NULL)
	{
		Ekop=new TElement;
		kopiruj_element(CE->Element,Ekop);
		vloz_segment_cesty_do_seznamu_cesty(novy,Ekop);
		Ekop=NULL;delete Ekop;
		CE=CE->dalsi;
	}

	//kopírování prvního a posledního teploměru
	TElement *t1=NULL,*t2=NULL;
	t1=new TElement;
	t1->n=MaxInt;
	t1->name=original->prvni->name;
	t1->objekt_n=original->prvni->sparovany->objekt_n;
	t1->mGrid=NULL;
	t1->pohon=NULL;
	t1->geo=original->prvni->sparovany->geo;//kopírování všech parametrů geometrie
	t1->Xt=t1->X=t1->geo.X1=t1->geo.X4=original->prvni->X;
	t1->Yt=t1->Y=t1->geo.Y1=t1->geo.Y4=original->prvni->Y;
	t1->eID=original->prvni->eID;
	Ekop=new TElement;
	kopiruj_element(original->prvni->sparovany,Ekop);
	t1->sparovany=Ekop;
	Ekop=NULL;delete Ekop;
	t1->dalsi=NULL;
	t1->dalsi2=NULL;
	t1->predchozi==NULL;
	t1->predchozi2=NULL;
	//druhý
	t2=new TElement;
	t2->n=MaxInt;
	t2->name=original->posledni->name;
	t2->objekt_n=original->posledni->sparovany->objekt_n;
	t2->mGrid=NULL;
	t2->pohon=NULL;
	t2->geo=original->prvni->sparovany->geo;//kopírování všech parametrů geometrie
	t2->Xt=t2->X=t2->geo.X1=t2->geo.X4=original->posledni->X;
	t2->Yt=t2->Y=t2->geo.Y1=t2->geo.Y4=original->posledni->Y;
	t2->eID=original->posledni->eID;
  Ekop=new TElement;
	kopiruj_element(original->posledni->sparovany,Ekop);
	t2->sparovany=Ekop;
	Ekop=NULL;delete Ekop;
	t2->dalsi=NULL;
	t2->dalsi2=NULL;
	t2->predchozi==NULL;
	t2->predchozi2=NULL;
	//vložení teploměru do struktury
	novy->prvni=t1;
	novy->posledni=t2;
	t1=NULL;delete t1;
	t2=NULL;delete t2;
  Ekop=NULL;delete Ekop;
	
	return novy;
}
////---------------------------------------------------------------------------
//pokud došlo ke změně, která může ovlivnit cestu teploměru, zkontroluje, zda je možné aktualizovat a pokud ano, aktualizuje
void Cvektory::aktualizuj_cestu_teplomeru()
{
	TObjekt *Objekt=OBJEKTY->dalsi;
	while(Objekt!=NULL)
	{
  	if(Objekt->teplomery!=NULL)
  	{
  		TTeplomery *T=vrat_teplomery_podle_zakazky(Objekt,ZAKAZKA_akt);
  		if(T!=NULL)
  		{
  			update_akt_zakazky();//důležité mít aktuální data
  			//deklarace
  			bool nalezen_prvni=false,nalezen_posledni=false;
  			double X1,Y1,X2,Y2;
  			X1=T->prvni->geo.X1;Y1=T->prvni->geo.Y1;
  			X2=T->posledni->geo.X4;Y2=T->posledni->geo.Y4;
  			TCesta *CE=ZAKAZKA_akt->cesta->dalsi;
  			//nulování spárovaných elementů teplomerů
  			TElement *E1=T->prvni->sparovany,*E2=T->posledni->sparovany;
  			T->prvni->sparovany=NULL;
  			T->posledni->sparovany=NULL;

  			//hledání nových spárovaných elementů
				while(CE!=NULL)
				{
					//kontrola, zda je první element stále na geometrii
					if(!nalezen_prvni && (PtInSegment(CE->Element,X1,Y1) || (CE->Element->geo.X4==X1 && CE->Element->geo.Y4==Y1)))
					{
						nalezen_prvni=true;
						T->prvni->sparovany=CE->Element;
					}
					//kontrola, zda je posledni element stále na geometrii
					if(!nalezen_posledni && (PtInSegment(CE->Element,X2,Y2) || (CE->Element->geo.X4==X2 && CE->Element->geo.Y4==Y2)))
					{
						nalezen_posledni=true;
						T->posledni->sparovany=CE->Element;
  				}
     			//posun na další segnemt cesty, případně konec průchodu
  				if(nalezen_prvni && nalezen_posledni)break;
  				CE=CE->dalsi;
  			}

  			//aktualizace cesty
  			if(T->prvni->sparovany!=NULL && T->posledni->sparovany!=NULL)
				{
					//mazání stare cesty
					hlavicka_cesty_teplomery(T);//smaže a vytvoří hlavičku
  				//pokud je cesta mezi teploměry aktualizuje, pokud ne nedelá nic
					if(T->prvni->sparovany!=T->posledni->sparovany)
					{
  					CE=vrat_segment_cesty(ZAKAZKA_akt,T->prvni->sparovany)->dalsi;
						while(CE!=NULL)
  					{
							if(CE->Element==T->posledni->sparovany)break;
							vloz_segment_cesty_do_seznamu_cesty(T,CE->Element);
							CE=CE->dalsi;
						}
					}
          //aktualizace tabulky
					if(Objekt==F->OBJEKT_akt)F->vytvor_aktualizuj_tab_teplomeru();
				}

				//došlo k chybe vytvoření default cesty, obsahuje aktualizaci tabulky
				else F->reset_teplomeru(Objekt);

				//ukazatelové záležitosti
				CE=NULL;delete CE;
				E1=NULL;delete E1;
				E2=NULL;delete E2;
			}
			//uakazatelové záležitosti
			T=NULL;delete T;
		}
		//posun na další objekt
		Objekt=Objekt->dalsi;
	}
	//ukazatelové záležitosti
	delete Objekt;Objekt=NULL;
}
////---------------------------------------------------------------------------


