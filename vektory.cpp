////---------------------------------------------------------------------------
#pragma hdrstop
#include "vektory.h"
#include "unit1.h"
#include "parametry.h"//ODSTRANIT
#include "miniform_zpravy.h"
////---------------------------------------------------------------------------
#pragma package(smart_init)
////---------------------------------------------------------------------------
////konstruktor
Cvektory::Cvektory()
{
	hlavicka_OBJEKTY();//vytvoří novou hlavičku pro objekty
	hlavicka_POHONY();//vytvoří novou hlavičku pro pohony
	hlavicka_ZAKAZKY();//vytvoří novou hlavičku pro zakazky
	hlavicka_VOZIKY();//vytvoří novou hlavičku pro vozíky
	//hlavicka_RETEZY();//vytvoří novou hlavičku pro řetězy - nepoužíváno
	//hlavicka_palce();//vytvoří novou hlavičku pro palce - zatím nepoužíváno
	HALA.body=NULL;
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
			{    //F->Memo("ano",true,true);                           //zkontrolovat či doplnit duplcitu vůči předchozímu
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
			else if(m.LeziVblizkostiUsecky(x,y,B->kota.rect0.left,B->kota.rect0.top,B->kota.rect0.right,B->kota.rect0.bottom)<=5){RET=0;F->pom_bod=B;break;}//oblast kóty
		}
		B=B->dalsi;
	}
	B=NULL;delete B;
	return RET;
}
////---------------------------------------------------------------------------
//ověří zda se souřadnicích myši nachází ve vnitř polygonu, pokud je Objekt==NULL, hledá se v polygonu HALy
bool Cvektory::PtInBody(TObjekt *Objekt)
{
	////vstupní proměnné
	bool RET=false;
	TBod *B=NULL;
	if(Objekt!=NULL && Objekt->body!=NULL)B=Objekt->body;//jedná se o body objektu
	else if(HALA.body!=NULL && HALA.body->predchozi->n>1)B=HALA.body;//jedná se bod haly +

	////vytvoření regionu
	if(B!=NULL)
	{
		unsigned long pocet=B->predchozi->n;//počet prvků pole
		POINT *body=new POINT[pocet];//dynamické pole bodů již ve fyzických souřadnicích, zde jen alokace
		B=B->dalsi;//přeskočí hlavičku
		//plnění do pole
		while(B!=NULL)
		{
			body[B->n-1].x=m.L2Px(B->X);body[B->n-1].y=m.L2Py(B->Y);
			B=B->dalsi;
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
	//novy->Xk=0;
	//novy->Yk=0;
	novy->body=NULL;
	novy->sirka_steny=0;
	novy->short_name="";//krátký název
	novy->name="";//celý název objektu
	novy->rezim=0;
	novy->CT=0;//pro status návrh
	novy->RD=0;//pro status návrh
	novy->delka_dopravniku=0;//delka dopravníku v rámci objektu
	novy->kapacita=0;
	novy->kapacita_dop=0;
	novy->pozice=0;
	//novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//velikost mezery mezi vozíky
	novy->pohon=NULL;//ukazatel na použitý pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	//novy->rozmer_kabiny.x=0;
	//novy->rozmer_kabiny.y=0;
	novy->koty_elementu_offset.x=0;//odsazení kót elementů v metrech
	novy->koty_elementu_offset.y=0;//odsazení kót elementů v metrech
	novy->komora=NULL;//ukazatel na komory
	novy->cekat_na_palce=0;//0-ne,1-ano,2-automaticky
	novy->stopka=0;//zda následuje na konci objektu stopka//0-ne,1-ano,2-automaticky
	novy->odchylka=0;//odchylka z CT, využíváno hlavně u objektů v PP režimu
	novy->obsazenost=0;//slouží pro uchování času obsazenosti pro vykreslování na časových osách
	novy->CT_zamek=0;
	novy->RD_zamek=0;
	novy->DD_zamek=0;
	novy->K_zamek=0;
	novy->poznamka="";
	novy->probehla_aktualizace_prirazeni_pohonu=false;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
	novy->zobrazit_koty=true;//proměnná určující, zda se budou zobrzovat kóty
	novy->zobrazit_mGrid=true;//proměnná určující, zda budou zobrazeny mGridy
	novy->uzamknout_nahled=false;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu
	novy->predchozi=novy;//ukazuje sam na sebe
	novy->predchozi2=NULL;
	novy->dalsi=NULL;
	novy->dalsi2=NULL;
	OBJEKTY=novy;//OBJEKTY
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////uloží objekt a jeho parametry do seznamu
Cvektory::TObjekt *Cvektory::vloz_objekt(unsigned int id, double X, double Y)
{
	TObjekt *novy=nastav_atributy_objektu(id,X,Y);
	//ukazatele
	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->predchozi2=NULL;
	novy->dalsi=NULL;
	novy->dalsi2=NULL;
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí

	return novy;
}
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu za objekt p        //p předchozí
Cvektory::TObjekt *Cvektory::vloz_objekt(unsigned int id, double X, double Y,TObjekt *pred,TObjekt *po)
{
	TObjekt *novy=nastav_atributy_objektu(id,X,Y);
	//ukazatele
	novy->predchozi=pred;
	novy->predchozi2=NULL;
	if(po->predchozi==pred)po->predchozi=novy;
	if(po->predchozi2==pred)po->predchozi2=novy;
	novy->dalsi=po;
	novy->dalsi2=NULL;
	if(pred->dalsi==po)pred->dalsi=novy;
	if(pred->dalsi2==po)pred->dalsi2=novy;
	novy->n=pred->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
	//indexy zvýšit separátně
	return novy;
}
//---------------------------------------------------------------------------
Cvektory::TObjekt *Cvektory::vloz_objekt(unsigned int id, double X, double Y,TObjekt *vyhybka,TObjekt *pred,TObjekt *po)
{

	TObjekt *novy=nastav_atributy_objektu(id,X,Y);
	//spojkové ukazatele
	novy->predchozi2=vyhybka;//pohled zpět na sekundární větev, v momentě vložení totožná s vyhybkou
	novy->predchozi=pred;//pohled na hlavní větev
	novy->dalsi=po;
	novy->dalsi2=vyhybka;//propojení s vyhybkou
	if(po!=NULL)//spojka vkládaná mezi 2 ojekty
	{
		if(po->predchozi==pred)po->predchozi=novy;
		if(po->predchozi2==pred)po->predchozi2=novy;
	}
	else OBJEKTY->predchozi=novy;//spojka vkládaná za poslední objekt
	if(pred->dalsi==po)pred->dalsi=novy;
	if(pred->dalsi2==po) pred->dalsi2=novy;
	//výhybkové ukazatele
	vyhybka->dalsi2=novy;//propojení sekundární větve
	vyhybka->predchozi2=novy;//propojení výhybky a spojky
	//indexy zvýšit separátně
	novy->n=pred->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
	return novy;
}
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu - přetížená fce
void Cvektory::vloz_objekt(TObjekt *Objekt)
{
	TObjekt *novy=new TObjekt;

	*novy=*Objekt;//novy bude ukazovat tam kam prvek Objekt
	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->predchozi2=NULL;
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	novy->dalsi2=NULL;
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//alokuje paměť pro objekt, nastavý atriuty objektu, vrátí ukazatel na nově vytvořený prvek
Cvektory::TObjekt *Cvektory::nastav_atributy_objektu(unsigned int id, double X, double Y)
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

	TObjekt *novy=new TObjekt;

	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->id=id;
	novy->short_name=short_name;
	novy->name=name;
	novy->rezim=0;if(id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu,pozice objektu
	novy->Y=Y;//přiřadím Y osu,pozice objektu
	novy->Xp=-500;
	novy->Yp=-500;
	novy->body=NULL;//spojový seznam definičních bodů obrysu objektu
	novy->sirka_steny=0.15;//0.12;//šířka stěny kabiny objektu v metrech
	novy->CT=PP.TT;//pro status návrh
	novy->RD=m.UDV(0)/novy->CT;//pro status návrh
	novy->delka_dopravniku=m.UDV(0);//delka dopravníku v rámci objektu
	novy->kapacita=1;
	novy->kapacita_dop=0;
	novy->pozice=1;
	//novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//mezera mezi vozíky (kritická mezera)
	novy->mezera_jig=0;//mezera mezi jigy
	novy->mezera_podvozek=0;//mezera mezi podvozky
	novy->pohon=NULL;//při vložení nemá vložen žádný pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	////
	if(id==3)novy->koty_elementu_offset.x=1;else novy->koty_elementu_offset.x=4;//odsazení kót elementů v metrech,v kabině POW se kóty vykroslují od hrany kabiny, ne od pohonu
	novy->koty_elementu_offset.y=0.4;
	novy->komora=NULL;//ukazatel na komory
	if(id==3)for(short i=1;i<=4;i++)vloz_komoru(novy,2.5,NULL,i%2);//pokud se jedná o POWash,nastaví defaultně 4 stejné komory
	novy->cekat_na_palce=2;//0-ne,1-ano,2-automaticky
	novy->stopka=2;//zda následuje na konci objektu stopka //0-ne,1-ano,2-automaticky
	novy->odchylka=0;//odchylka z CT, využíváno hlavně u objektů v PP režimu
	novy->obsazenost=0;//slouží pro uchování času obsazenosti pro vykreslování na časových osách
	novy->CT_zamek=0;
	novy->RD_zamek=1;//defautlně zamčeno
	novy->DD_zamek=0;
	novy->K_zamek=0;
	novy->poznamka="";
	novy->probehla_aktualizace_prirazeni_pohonu=false;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
	novy->zobrazit_koty=true;//proměnná určující, zda se budou zobrzovat kóty
	novy->zobrazit_mGrid=true;//proměnná určující, zda budou zobrazeny mGridy
	novy->uzamknout_nahled=false;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu

	//nově, vkládání bodů + defaultní rozměry různých objektů
	TPointD rozmery_kabiny,konec;
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
	TElement *zarazka=vloz_element(novy,MaxInt,konec.x,konec.y,0);
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
	if(OBJEKTY->predchozi->n==1 && novy->X==OBJEKTY->predchozi->X && novy->Y==OBJEKTY->predchozi->Y && OBJEKTY->predchozi->orientace==90 || F->d.predchozi_oblast==2 && OBJEKTY->predchozi->n==1)//změna trendu linky, pokud nebylo s prvním objektem rotováno
	{
		posun_objekt(OBJEKTY->predchozi->elementy->predchozi->geo.X4-OBJEKTY->predchozi->elementy->dalsi->geo.X1,0,OBJEKTY->predchozi);
		rotuj_objekt(OBJEKTY->predchozi,180);
	}
	return novy;
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
		novy->CT=Objekt->CT;//pro status návrh převezme původní hodnoty
		novy->RD=Objekt->RD;//pro status návrh převezme původní hodnoty
		novy->delka_dopravniku=Objekt->delka_dopravniku;
		novy->kapacita=Objekt->kapacita;
		novy->kapacita_dop=Objekt->kapacita_dop;
		novy->pozice=Objekt->pozice;
		//novy->rotace=Objekt->rotace;
		novy->mezera=Objekt->mezera;//velikost mezery mezi vozíky (kritická mezera)
		novy->mezera_jig=Objekt->mezera_jig;//mezera mezi jigy
		novy->mezera_podvozek=Objekt->mezera_podvozek;//mezera mezi podvozky
		novy->pohon=Objekt->pohon;
		kopiruj_elementy(Objekt,novy);
		novy->min_prujezdni_profil=Objekt->min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
		//novy->rozmer_kabiny=Objekt->rozmer_kabiny;//výchozí rozměr kabiny
		novy->koty_elementu_offset=Objekt->koty_elementu_offset;//odsazení kót elementů v metrech
		novy->komora=NULL;
		if(novy->id==3)kopiruj_komory(Objekt,novy);//pokud se jedná o POWash
		novy->cekat_na_palce=Objekt->cekat_na_palce;//0-ne,1-ano,2-automaticky
		novy->stopka=Objekt->stopka;//zda následuje na konci objektu stopka //0-ne,1-ano,2-automaticky
		novy->odchylka=Objekt->odchylka;//odchylka z CT, využíváno hlavně u objektů v PP režimu
		novy->obsazenost=0;//Objekt->obsazenost;//slouží pro uchování času obsazenosti pro vykreslování na časových osách
		novy->CT_zamek=Objekt->CT_zamek;
		novy->RD_zamek=Objekt->RD_zamek;
		novy->DD_zamek=Objekt->DD_zamek;
		novy->K_zamek=Objekt->K_zamek;
		novy->poznamka=Objekt->poznamka;
		novy->probehla_aktualizace_prirazeni_pohonu=Objekt->probehla_aktualizace_prirazeni_pohonu;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
		novy->zobrazit_koty=Objekt->zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty
		novy->zobrazit_mGrid=Objekt->zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy
		novy->uzamknout_nahled=Objekt->uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu

		novy->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		novy->predchozi2=NULL;
		novy->dalsi=p->dalsi;
		p->dalsi->predchozi=novy;
		p->dalsi=novy;
		p->dalsi2=NULL;
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
	Kopie->orientace_text=Original->orientace_text;
	Kopie->sirka_steny=Original->sirka_steny;
	Kopie->rezim=Original->rezim;
	Kopie->CT=Original->CT;
	Kopie->RD=Original->RD;
	Kopie->delka_dopravniku=Original->delka_dopravniku;
	Kopie->kapacita=Original->kapacita;
	Kopie->kapacita_dop=Original->kapacita_dop;
	Kopie->pozice=Original->pozice;
	//Kopie->rotace=Original->rotace;
	Kopie->orientace=Original->orientace;
	Kopie->mezera=Original->mezera;
	Kopie->mezera_jig=Original->mezera_jig;
	Kopie->mezera_podvozek=Original->mezera_podvozek;
	kopiruj_pohon(Original->pohon,Kopie);//POHON
	Kopie->min_prujezdni_profil=Original->min_prujezdni_profil;
	//Kopie->rozmer_kabiny=Original->rozmer_kabiny;
	Kopie->koty_elementu_offset=Original->koty_elementu_offset;
	Kopie->komora=NULL;//POZOR TOTO NENÍ ZCELA SPRÁVNĚ, MĚLO BY SE NEJDŘÍVE SMAZAT PŘIDRUŽENÝ SPOJÁK, ABY NEZŮSTAL V PAMĚTI
	Kopie->body=NULL;//POZOR TOTO NENÍ ZCELA SPRÁVNĚ, MĚLO BY SE NEJDŘÍVE SMAZAT PŘIDRUŽENÝ SPOJÁK, ABY NEZŮSTAL V PAMĚTI
	if(Kopie->id==3)kopiruj_komory(Original,Kopie);//pokud se jedná o POWash
	Kopie->cekat_na_palce=Original->cekat_na_palce;
	Kopie->stopka=Original->stopka;
	Kopie->odchylka=Original->odchylka;
	Kopie->obsazenost=Original->obsazenost;
	Kopie->CT_zamek=Original->CT_zamek;
	Kopie->RD_zamek=Original->RD_zamek;
	Kopie->DD_zamek=Original->DD_zamek;
	Kopie->K_zamek=Original->K_zamek;
	Kopie->poznamka=Original->poznamka;
	Kopie->probehla_aktualizace_prirazeni_pohonu=Original->probehla_aktualizace_prirazeni_pohonu;
	Kopie->zobrazit_koty=Original->zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty
	Kopie->zobrazit_mGrid=Original->zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy
	Kopie->uzamknout_nahled=Original->uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu
	//obě kopírovací metody musí být ke konci
	kopiruj_elementy(Original,Kopie);
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
	TPoint *tab_pruchodu=new TPoint[pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
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
		O=dalsi_krok(O,tab_pruchodu);
	}
	tab_pruchodu=NULL;delete tab_pruchodu;
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
//		switch((int)O->orientace)
//		{
//			case 0:
//			{
//				double delka_y=O->elementy->predchozi->geo.Y4-O->elementy->dalsi->geo.Y1,polovina_y=O->elementy->dalsi->geo.Y1+delka_y/2.0,delka_x=delka_y;
//				if(O->id==8 || O->id==7){delka_y/=2.0;delka_x/=2.0;}//zmenšení oblasti pro dlouhé objekty
//				if(O->elementy->predchozi->geo.X4-delka_x<=X && X<=O->elementy->predchozi->geo.X4+delka_x && polovina_y<=Y && Y<polovina_y+3*delka_y/2.0)ret=1;//oblast za objektem
//				if(O->elementy->dalsi->geo.X1-delka_x<=X && X<=O->elementy->dalsi->geo.X1+delka_x && polovina_y-3*delka_y/2.0<=Y && Y<polovina_y)ret=2;//oblast před objektem
//			}break;
//			case 90:
//			{
//				double delka_x=O->elementy->predchozi->geo.X4-O->elementy->dalsi->geo.X1,polovina_x=O->elementy->dalsi->geo.X1+delka_x/2.0,delka_y=delka_x;
//				if(O->id==8 || O->id==7){delka_y/=3.0;delka_x/=2.0;}//zmenšení oblasti pro dlouhé objekty
//				if(polovina_x<=X && X<=polovina_x+3*delka_x/2.0 && O->elementy->predchozi->geo.Y4-delka_y<=Y && Y<=O->elementy->predchozi->geo.Y4+delka_y)ret=1;//oblast za objektem
//				if(polovina_x-3*delka_x/2.0<=X && X<polovina_x && O->elementy->predchozi->geo.Y4-delka_y<=Y && Y<=O->elementy->predchozi->geo.Y4+delka_y)ret=2;//oblast před objektem
//			}break;
//			case 180:
//			{
//				double delka_y=O->elementy->dalsi->geo.Y1-O->elementy->predchozi->geo.Y4,polovina_y=O->elementy->dalsi->geo.Y1-delka_y/2.0,delka_x=delka_y;
//				if(O->id==8 || O->id==7){delka_y/=2.0;delka_x/=2.0;}//zmenšení oblasti pro dlouhé objekty
//				if(O->elementy->predchozi->geo.X4-delka_x<=X && X<=O->elementy->predchozi->geo.X4+delka_x && polovina_y-3*delka_y/2.0<=Y && Y<=polovina_y)ret=1;//oblast za objektem
//				if(O->elementy->dalsi->geo.X1-delka_x<=X && X<=O->elementy->dalsi->geo.X1+delka_x && polovina_y<Y && Y<=polovina_y+3*delka_y/2.0)ret=2;//oblast před objektem
//			}break;
//			case 270:
//			{
//				double delka_x=O->elementy->dalsi->geo.X1-O->elementy->predchozi->geo.X4,polovina_x=O->elementy->dalsi->geo.X1-delka_x/2.0,delka_y=delka_x;
//				if(O->id==8 || O->id==7){delka_y/=3.0;delka_x/=2.0;}//zmenšení oblasti pro dlouhé objekty
//				if(polovina_x-3*delka_x/2.0<=X && X<=polovina_x && O->elementy->predchozi->geo.Y4-delka_y<=Y && Y<=O->elementy->predchozi->geo.Y4+delka_y)ret=1;//oblast před objektem
//				if(polovina_x<X && X<=polovina_x+3*delka_x/2.0 && O->elementy->predchozi->geo.Y4-delka_y<=Y && Y<=O->elementy->predchozi->geo.Y4+delka_y)ret=2;//oblast za objektem
//			}break;
//		}
		double delka_x=5,delka_y=delka_x;
		if(O->id==0 || O->id==9 || O->body->predchozi->n>4)delka_x=delka_y=2.5;
		if(O->elementy->predchozi->geo.X4-delka_x<X && X<O->elementy->predchozi->geo.X4+delka_x && O->elementy->predchozi->geo.Y4-delka_y<Y && Y<O->elementy->predchozi->geo.Y4+delka_y)ret=1;//oblast před objektem
		if(O->elementy->dalsi->geo.X1-delka_x<X && X<O->elementy->dalsi->geo.X1+delka_x && O->elementy->dalsi->geo.Y1-delka_y<Y && Y<O->elementy->dalsi->geo.Y1+delka_y)ret=2;//oblast za objektem
	}
	return ret;
}
//---------------------------------------------------------------------------
//vrátí ukazatel na objekt dle n objektu
Cvektory::TObjekt *Cvektory::vrat_objekt(unsigned int n)
{
	TPoint *tab_pruchodu=new TPoint[pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
	TObjekt *p=OBJEKTY->dalsi;//přeskočí hlavičku
	while (p!=NULL)
	{
		if(p->n==n)break;//akce s ukazatelem
		else p=dalsi_krok(p,tab_pruchodu);//posun na další prvek v seznamu
	}
	tab_pruchodu=NULL;delete tab_pruchodu;
	return p;
}
//---------------------------------------------------------------------------
//dle X kurzoru myši vrátí z modu procesy (ROMA) ukazatel na aktuální objekt
Cvektory::TObjekt *Cvektory::vrat_objekt_z_roma(int X)
{
	TObjekt *O=OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (O!=NULL)
	{
		if(O->predchozi->obsazenost<X && X<=O->obsazenost)//akce s ukazatelem
		break;//zastaví další hledání výsledek je O
		O=O->dalsi;//posun na další prvek v seznamu
	}
	return O;
}
//---------------------------------------------------------------------------
//vrátí rodičovský Objekt daného elementu, In_pom_temp - zda bude hledat místo daného ostrého objektu v něm
Cvektory::TObjekt *Cvektory::vrat_objekt(TElement *Element,bool In_pom_temp)
{
	bool RET=false;
	Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)//prochází všechny objekty
	{
		TElement *E=E=O->elementy;//nepřeskakovat hlavičku
		if(F->pom_temp->n==O->n && In_pom_temp)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
		while(E!=NULL)//a jejich elementy
		{
			if(E==Element){RET=true;break;}//ukončí předčasně další vyhledávání
			else E=E->dalsi;
		}
		E=NULL;delete E;
		if(RET)break;//ukončí předčasně další vyhledávání
		else O=O->dalsi;//posun na další prvek
	}
	return O;
}
//---------------------------------------------------------------------------
//smaze objekt ze seznamu
short int Cvektory::smaz_objekt(TObjekt *Objekt,bool opakovani)
{
	TObjekt *spojka_vyh=NULL;
	if(Objekt->id==(unsigned)F->VyID&&!opakovani)spojka_vyh=Objekt->predchozi2;
	if(Objekt->id==pocet_objektu_knihovny+1&&!opakovani)spojka_vyh=Objekt->dalsi2;
	//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
	if(Objekt->dalsi!=NULL)//ošetření proti poslednímu prvku
	{
		if(Objekt->predchozi->dalsi==Objekt)Objekt->predchozi->dalsi=Objekt->dalsi;
		if(Objekt->predchozi->dalsi2==Objekt)Objekt->predchozi->dalsi2=Objekt->dalsi;
		if(Objekt->dalsi->predchozi==Objekt)Objekt->dalsi->predchozi=Objekt->predchozi;
		if(Objekt->dalsi->predchozi2==Objekt)Objekt->dalsi->predchozi2=Objekt->predchozi;
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
			if(Objekt->predchozi->dalsi==Objekt)Objekt->predchozi->dalsi=NULL;
			if(Objekt->predchozi->dalsi2==Objekt)Objekt->predchozi->dalsi2=NULL; //možná jen zbytečně navíc
		}
	}

	vymaz_body(Objekt);
	vymaz_komory(Objekt);
	vymaz_elementy(Objekt);
	if(spojka_vyh!=NULL)smaz_objekt(spojka_vyh,true);
	Objekt=NULL;spojka_vyh=NULL;delete Objekt;delete spojka_vyh;//smaže mazaný prvek

	return 0;

};
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
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		//testovací MB: ShowMessage("Vstup CT:"+AnsiString(O->CT)+" RD:"+AnsiString(O->RD)+" DD:"+AnsiString(O->delka_dopravniku)+" K:"+AnsiString(O->kapacita)+" dV:"+AnsiString(dV)+" mV:"+AnsiString(O->mV));
    //HELP: DLE ZAMČENÝCH ODEMČENÝCH HODNOT (INDIVIDUÁLNÍ NASTAVENÍ) NUTNÁ ZMĚNA DATOVÉHO MODELU - NUTNO ULOŽIT Z FORMULÁŘE PO, KTERÁ POLOŽKA JE ZAMČENÁ, KTERÁ ODEMČENÁ
		if(typ==-1)//dle zamčených a odemčených hodnot při změně TT
		{
			////při zamčeném CT
			//doplnit podmínku
			typ=2;//přesměrování na daný typ
			////při zamčené K a DD
			//doplnit podmínku
			typ=1;//přesměrování na daný typ
			////při zamčeném RD
			//doplnit podmínku
			//doplnit počítání mezer
		}
		if(typ==0)//dle zamčených a odemčených hodnot při změně parametrů vozíku
		{
			//při zamčeném K či CT
			typ=3;
			//při zamčeném K,RD
			typ=4;
			//při zamčeném DD
			typ=5;
		}

		switch(typ)
		{
//asi se již nepoužívá
//			case -2://zaktualizuje přiřazení pohonu k objektu, nutné pokud proběhla změna v pohonech, protože původní jsou smazané
//			{
//				if(O->pohon!=NULL)//přiřazuje pouze pokud byl pohon již přiřazen
//				O->pohon=vrat_pohon(O->pohon->n);
//			}
//      break;
			case 1://při změně TT změna CT a RD, K a DD zůstává
			{
				O->CT=PP.TT*O->kapacita;
				if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
			}
			break;
			case 2://při změně TT změna K,DD,RD zůstává CT mimo S&G
			{
				if(O->rezim==0)O->CT=PP.TT;//pro S&G
				else //pro kontinuál a PP
				{
					//K
					O->kapacita=O->CT/PP.TT;
					//DD
					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
					//RD
					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
				}
			}
			break;
			case 3://při změně parametrů vozíku změna DD, RD zůstává K, CT
			{
				if(O->rezim!=0)//pro kontinuál a PP
				{
					//DD
					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
					//RD
					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
				}
			}
			break;
			case 4://při změně parametrů vozíku změna u DD, CT zůstává K,RD
			{
				if(O->rezim!=0)//pro kontinuál a PP
				{
					//DD
					O->delka_dopravniku=O->kapacita*m.UDV(O->orientace)*O->mezera;//DD
					//CT
					if(O->rezim==1)O->CT=O->delka_dopravniku/O->RD;//pro kontinual
					else//pro PP
					{
						O->kapacita=O->delka_dopravniku/(m.UDV(O->orientace)+O->mezera);
					}
				}
			}
			break;
			case 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
			{
				if(O->rezim!=0)//pro kontinuál a PP
				{
					//K
					O->kapacita=O->delka_dopravniku/(m.UDV(O->orientace)+O->mezera);//K
					//CT
					O->CT=PP.TT*O->kapacita;
					//RD
					if(O->rezim==1)O->RD=O->delka_dopravniku/O->CT;//u kontinuálního
				}
			}
			break;
		}
		//prozatím zde a takto výpočte počet pozic
		double P=floor(O->kapacita);//celočíselná kapacita
		double DV=m.UDV(O->orientace);
		double DVM=(DV+O->mezera)*(O->kapacita-P);//délka části poslední vozíko-mezery v kabině
		if(DVM>=DV)P++;//navýší o celý vozík, protože je minimálně celý vozík v kabině
		else P+=DVM/DV;//navýší o část vozíku, protože je jenom část vozíku v kabině
		O->pozice=P;
		//testovací MB:ShowMessage("Výstup CT:"+AnsiString(O->CT)+" RD:"+AnsiString(O->RD)+" DD:"+AnsiString(O->delka_dopravniku)+" K:"+AnsiString(O->kapacita)+" dV:"+AnsiString(dV));
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
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
	TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	double SUM=0.0;
	while (O!=NULL)
	{
		SUM+=O->delka_dopravniku;
		O=O->dalsi;//posun na další prvek
	}
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
{
	unsigned int pocet=0;
	TCesta *C=Zakazka->cesta->dalsi;//přeskočí hlavičku
	while (C!=NULL)
	{                     //celkový počet
		if(C->objekt->rezim==typ || typ==-1)pocet++;
		C=C->dalsi;//posun na další prvek
	}
	C=NULL;delete C;
	return pocet;
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
{
	 TZakazka *Z=ZAKAZKY->dalsi;
	 AnsiString T="";
	 while (Z!=NULL)
	 {
		TCesta *C=ZAKAZKY->cesta->dalsi;
		while (C!=NULL)
		{
			double vytizeni=C->CT/C->objekt->kapacita/Z->TT*100.0;
			if(vytizeni!=100)
			{
				AnsiString N=C->objekt->name;if(shortname)C->objekt->short_name;
				AnsiString V=""; if(vypsat_procetna)"- "+AnsiString(vytizeni)+" %";
				T=Z->name+"/"+N+V+separator;
			}
			C=C->dalsi;
		}
		delete C;
		Z=Z->dalsi;
	 }
	 delete Z;
	 T=T.SubString(1,T.Length()-separator.Length());//odebere ještě poslední separátor
	 return T;
}
//---------------------------------------------------------------------------
//najde ze všech objektů nejnižší rychlost přejezdu (tj. totál min RD), neřeší přiřazení k pohonům, pomůcka pro stanovení referenční rychlosti animace
double Cvektory::vrat_min_rychlost_prejezdu()
{
	double minRD=0.0;
	if(OBJEKTY->dalsi!=NULL)minRD=OBJEKTY->dalsi->RD;
	TObjekt *O=OBJEKTY->dalsi->dalsi;//přeskočí hlavičku a výše uvedený
	while (O!=NULL)
	{
		if(O->RD<minRD)minRD=O->RD;
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
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
//projde všechny objekty, výhybkám a spojkám upravý návez podle jejich n
void Cvektory::nove_nazvy()
{
	TObjekt *O=OBJEKTY->dalsi;
	TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];
	int n;
	while(O!=NULL)//přejmutí čísla výhybek či spojek z jejich názvu, nutné před tímto spustit nove_indexy(true)!!!
	{
		if((long)O->id==F->VyID)n=F->ms.MyToDouble(O->name.SubString(9,1));else n=F->ms.MyToDouble(O->name.SubString(8,1));
		if((long)O->id==pocet_objektu_knihovny+1)if(tab_pruchodu[n].y==0)O->short_name="S"+AnsiString(n);
		if((long)O->id==F->VyID)if(tab_pruchodu[n].x==0)O->short_name="V"+AnsiString(n);
		O=dalsi_krok(O,tab_pruchodu);
	}
	O=NULL;tab_pruchodu=NULL;delete O;delete tab_pruchodu;
}
//---------------------------------------------------------------------------
//projde všechny objekty a nastavý nové indexy podle aktuálního pořadí objektů
void Cvektory::nove_indexy(bool nasledne_zmena_nazvu)
{
	TObjekt *O=OBJEKTY->dalsi;
	int n=1;
	while(O!=NULL)
	{
		//změna indexu objektu
		O->n=n;
		n++;
		//změna objekt_n v elementech, musí být aktuální!!
		Cvektory::TElement *E=O->elementy->dalsi;
		while(E!=NULL)
		{
      E->objekt_n=O->n;
			E=E->dalsi;
		}
		delete E;E=NULL;
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
		vymaz_elementy(OBJEKTY->predchozi);
		pocet_smazanych_objektu++;
		OBJEKTY->predchozi=NULL;
		delete OBJEKTY->predchozi;
		OBJEKTY=OBJEKTY->dalsi;
	};

	return pocet_smazanych_objektu;
};
////---------------------------------------------------------------------------
//určuje další krok cyklu při procházení objektů
Cvektory::TObjekt *Cvektory::dalsi_krok(TObjekt *Objekt,TPoint *tab_pruchodu)
{
  int krok=1;//rozdílné kroky v procházení objekty, defaultně krok = 1
	if(Objekt->id==(unsigned)F->VyID)//výhybka
	{
		int n=F->ms.MyToDouble(Objekt->short_name.SubString(2,1));//extrakce pořadového čísla výhybky
		tab_pruchodu[n].x++;if(tab_pruchodu[n].x==1)krok=2;else krok=1;//navýšení "buňky", která udržuje počet průchodu přes vyhybky
		if(Objekt->predchozi2==NULL)krok=1;//v případě přidávání výhybky není plně nadefinovaná, nutno pokračovat defaultním krokem
		//pokud se jedná o první průchod je krok nastaven na průchod sekundární větví, pokud druhý = průchod primární vetví
	}else//nejedná se o výhybku
	if(Objekt->id==pocet_objektu_knihovny+1)//spojka, neni přítomná v knihovně objektů, nelze ji z ní vkládat
	{
		int n=F->ms.MyToDouble(Objekt->short_name.SubString(2,1));//extrakce pořadového čísla spojky
		tab_pruchodu[n].y++;if(tab_pruchodu[n].y==1)krok=2;else krok=1;//navýšení "buňky", která udržuje počet průchodu přes spojku
		//při prvním průchodu je krok nastaven tak, aby došlo ke skoku na spárovanou výhybku, při dalším průchodu základní krok (dalsi)
	}else krok=1;//nejdená se o výhybku ani o spojku, krok nastavit na defaultní hodnotu
	switch(krok)//rozdělení přistupů na další element popřípadě skok na spárovanou výhybku
	{
		case 1:Objekt=Objekt->dalsi;break;//defaultně
		case 2:Objekt=Objekt->dalsi2;break;//pri průchodu sekundární vetví, skoku na spárovanou výhybku
	}
	return Objekt;
}
////---------------------------------------------------------------------------
//slouží k posunu objektu jako celku o X a Y, posun kabiny, pohonu, elementů, tabulek, nadpisu, kontrolovat_oblast slouží k nucenému přesunutí
void Cvektory::posun_objekt(double X,double Y,TObjekt *Objekt,bool kontrolovat_oblast,bool povolit_rotaci)
{
	short oblast=0;
	if(F->prichytavat_k_mrizce!=1)kontrolovat_oblast=false;
	if(kontrolovat_oblast && Objekt->predchozi->n>0)
	{
		oblast=oblast_objektu(Objekt->predchozi,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y);
		if(oblast==2)oblast==0;//vyřezení oblasti za
		X=F->akt_souradnice_kurzoru.x-Objekt->elementy->dalsi->geo.X1; Y=F->akt_souradnice_kurzoru.y-Objekt->elementy->dalsi->geo.Y1;
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
		TElement *E=Objekt->elementy->dalsi;//objekt má vždy element (zarážka)
		while(E!=NULL)
		{
			E->X+=X;E->Y+=Y;//souřadnice elementu
			if(E->Xt!=-100)E->Xt+=X;E->Yt+=Y;//souřadnice tabulky + kontrola zda je vytvořená
			//geometrie elementu
			E->geo.X1+=X;E->geo.X2+=X;E->geo.X3+=X;E->geo.X4+=X;
			E->geo.Y1+=Y;E->geo.Y2+=Y;E->geo.Y3+=Y;E->geo.Y4+=Y;
			E=E->dalsi;
		}
		delete E;E=NULL;
	}
	////přilepení objektu na předchozí objekt
	if(F->prichytavat_k_mrizce==1 && oblast==1 && (Objekt->elementy->dalsi->geo.X1!=Objekt->predchozi->elementy->predchozi->geo.X4 || Objekt->elementy->dalsi->geo.Y1!=Objekt->predchozi->elementy->predchozi->geo.Y4))
	{
		posun_objekt(Objekt->predchozi->elementy->predchozi->geo.X4-Objekt->elementy->dalsi->geo.X1,Objekt->predchozi->elementy->predchozi->geo.Y4-Objekt->elementy->dalsi->geo.Y1,Objekt,false);
	}
	////změna pořadí před předchozí
//	if(oblast==2)
//		zmen_poradi_objektu(Objekt,Objekt->predchozi);
	////přilepení na další objekt
//	oblast=0;
//	if(kontrolovat_oblast && Objekt->dalsi!=NULL)oblast=oblast_objektu(Objekt->dalsi,F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y);
//	if(F->prichytavat_k_mrizce==1 && (oblast==2 || oblast1==2) && (Objekt->dalsi->elementy->dalsi->geo.X1!=Objekt->elementy->predchozi->geo.X4 || Objekt->dalsi->elementy->dalsi->geo.Y1!=Objekt->elementy->predchozi->geo.Y4))
//		posun_objekt(Objekt->dalsi->elementy->dalsi->geo.X1-Objekt->elementy->predchozi->geo.X4,Objekt->dalsi->elementy->dalsi->geo.Y1-Objekt->elementy->predchozi->geo.Y4,Objekt,false);
	////změna pořadí za další
//	if(oblast==1)
//		zmen_poradi_objektu(Objekt,Objekt->dalsi);
	////změna rotace
	if(Objekt->n>1 && povolit_rotaci)
	{
		double azimut=0,x=F->akt_souradnice_kurzoru.x,y=F->akt_souradnice_kurzoru.y;
		azimut=m.Rt90(m.azimut(Objekt->predchozi->elementy->predchozi->geo.X4,Objekt->predchozi->elementy->predchozi->geo.Y4,x,y));//Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1));
		if(m.Rt90(azimut+180)!=Objekt->predchozi->orientace)rotuj_objekt(Objekt,Objekt->orientace-azimut);
	}
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
		rotuj_body(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,rotace,Objekt);
		////rotace elementů
		TElement *E=Objekt->elementy->dalsi;//objekt má vždy element (zarážka)
		while(E!=NULL)
		{
			if(E->orientace==m.Rt90(Objekt->orientace-90))E->orientace=m.Rt90(azimut-90);//zapsání nové orientace do elementu
			else E->orientace=m.Rt90(azimut+90);
			//souřadnice elementu
			Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->X,E->Y,rotace);
			E->X=Bod.x;E->Y=Bod.y;
			//souřadnice tabulky + kontrola zda je vytvořená
			if(E->Xt!=-100)
			{
				Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->Xt,E->Yt,rotace);
				E->Xt=Bod.x;E->Yt=Bod.y;
			}
			//geometrie elementu
			Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->geo.X1,E->geo.Y1,rotace);
			E->geo.X1=Bod.x;E->geo.Y1=Bod.y;
			Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->geo.X2,E->geo.Y2,rotace);
			E->geo.X2=Bod.x;E->geo.Y2=Bod.y;
			Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->geo.X3,E->geo.Y3,rotace);
			E->geo.X3=Bod.x;E->geo.Y3=Bod.y;
			Bod=m.rotace(Objekt->elementy->dalsi->geo.X1,Objekt->elementy->dalsi->geo.Y1,E->geo.X4,E->geo.Y4,rotace);
			E->geo.X4=Bod.x;E->geo.Y4=Bod.y;
			E->geo.orientace=m.Rt90(E->geo.orientace-rotace);
			E=E->dalsi;
		}
		delete E;E=NULL;
		//rotace nadpisu
		switch((int)azimut)
		{
			case 0:Objekt->Xt=m.P2Lx(F->vrat_max_oblast(Objekt).left);Objekt->Yt=Objekt->elementy->predchozi->geo.Y4-(Objekt->elementy->predchozi->geo.Y4-Objekt->elementy->dalsi->geo.Y1)/2.0;break;
			case 90:case 270:Objekt->Xt=Objekt->elementy->predchozi->geo.X4-(Objekt->elementy->predchozi->geo.X4-Objekt->elementy->dalsi->geo.X1)/2.0;Objekt->Yt=m.P2Ly(F->vrat_max_oblast(Objekt).top);break;
			case 180:Objekt->Xt=m.P2Lx(F->vrat_max_oblast(Objekt).right);Objekt->Yt=Objekt->elementy->predchozi->geo.Y4-(Objekt->elementy->predchozi->geo.Y4-Objekt->elementy->dalsi->geo.Y1)/2.0;break;
		}
		Objekt->orientace=Objekt->orientace_text=azimut;
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
	TRect oblast=F->vrat_max_oblast(Objekt);
	while(K!=NULL)
	{
		switch((int)Objekt->orientace)
		{
			case 0:if(Objekt->elementy->dalsi->geo.Y1+vzdalenost<=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y<=Objekt->elementy->dalsi->geo.Y1+vzdalenost+K->velikost && oblast.left<F->akt_souradnice_kurzoru_PX.x && oblast.right>F->akt_souradnice_kurzoru_PX.x)nalezeno=true;break;
			case 90:if(Objekt->elementy->dalsi->geo.X1+vzdalenost<=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x<=Objekt->elementy->dalsi->geo.X1+vzdalenost+K->velikost && oblast.top<F->akt_souradnice_kurzoru_PX.y && oblast.bottom>F->akt_souradnice_kurzoru_PX.y)nalezeno=true;break;
			case 180:if(Objekt->elementy->dalsi->geo.Y1-vzdalenost>=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y>=Objekt->elementy->dalsi->geo.Y1-vzdalenost-K->velikost && oblast.left<F->akt_souradnice_kurzoru_PX.x && oblast.right>F->akt_souradnice_kurzoru_PX.x)nalezeno=true;break;
			case 270:if(Objekt->elementy->dalsi->geo.X1-vzdalenost>=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x>=Objekt->elementy->dalsi->geo.X1-vzdalenost-K->velikost && oblast.top<F->akt_souradnice_kurzoru_PX.y && oblast.bottom>F->akt_souradnice_kurzoru_PX.y)nalezeno=true;break;
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
				else if(m.LeziVblizkostiUsecky(x,y,K->kota.rect0.left,K->kota.rect0.top,K->kota.rect0.right,K->kota.rect0.bottom)<=1){RET=0;F->pom_komora=K;break;}//oblast kóty
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
	TKomora *K=F->pom_temp->komora->dalsi;
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
//danému objektu vytvoří hlavičku elementů
void Cvektory::hlavicka_elementy(TObjekt *Objekt)
{
	Objekt->elementy=new TElement;
	Objekt->elementy->n=0;
	Objekt->elementy->eID=0;
	Objekt->elementy->short_name="";
	Objekt->elementy->name="";
	Objekt->elementy->X=0;
	Objekt->elementy->Y=0;
	Objekt->elementy->Xt=0;
	Objekt->elementy->Yt=0;
	Objekt->elementy->orientace=0;
	Objekt->elementy->rotace_jig=0;
	Objekt->elementy->stav=true;

	Objekt->elementy->LO1=0;
	Objekt->elementy->OTOC_delka=0;
	Objekt->elementy->LO2=0;
	Objekt->elementy->LO_pozice=0;

	Objekt->elementy->PT1=0;
	Objekt->elementy->PTotoc=0;
	Objekt->elementy->PT2=0;

	Objekt->elementy->WT=0;
	Objekt->elementy->WTstop=0;
	Objekt->elementy->RT.x=0;
	Objekt->elementy->RT.y=0;

	Objekt->elementy->mGrid=NULL;

	Objekt->elementy->sparovany=NULL;
	Objekt->elementy->predchozi=Objekt->elementy;//ukazuje sam na sebe
	Objekt->elementy->dalsi=NULL;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
Cvektory::TElement *Cvektory::vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y,short orientace,TElement *Ep)
{
	//pokud by ještě nebyla založena hlavička, tak ji založí
	if(Objekt->elementy==NULL)hlavicka_elementy(Objekt);
	int cislo_mGrid=vrat_nejvetsi_ID_tabulek(Objekt)+1;
	//alokace paměti
	TElement *novy=new TElement;

	//atributy elementu
	//novy->n=Objekt->elementy->predchozi->n+1;//navýším počítadlo prvku o jedničku již řešeno v vloz_element(Objekt,novy);
	novy->eID=eID;
	novy->X=X;
	novy->Y=Y;
	novy->Xt=-100;
	novy->Yt=-100;
	novy->orientace=orientace;//důležité pro volání makra m.Rxy, bez tohoto by makro vracelo chybné hodnoty

	//ukazatelové propojení - bylo původně poslední, ale nemohlo fungovat správně
	vloz_element(Objekt,novy,Ep);

	//defaultní data
	novy->LO1=1.5;
	novy->OTOC_delka=0;
	novy->zona_pred=0;
	novy->zona_za=0;
	novy->LO2=0;
	novy->LO_pozice=0;
	novy->PT1=0;
	novy->PTotoc=0;
	novy->PT2=0;
	novy->WT=0;//čekání na palec
	novy->WTstop=0;//čekání na stopce
	novy->RT.x=0;//ryzí reserve time
	novy->RT.y=0;//pokrácený reserve time
	novy->akt_pocet_voziku=0;
	novy->max_pocet_voziku=0;
	novy->rotace_jig=0;
	if(vrat_druh_elementu(novy)==0){novy->max_pocet_voziku=1;novy->akt_pocet_voziku=1;}//S&G elementy
	if(eID==0){novy->max_pocet_voziku=2;novy->akt_pocet_voziku=1;}//pouze stopky
	novy->stav=1;
	novy->PD=-1;//defaultní stav pro S&G roboty
	novy->objekt_n=0;//příslušnost elementu k objektu
	if(F->pom_temp!=NULL)novy->objekt_n=F->pom_temp->n;
	novy->pohon=NULL;//pohon na kterém se nachází element
	if(novy->predchozi->n!=0 && novy->predchozi->eID!=200)novy->pohon=novy->predchozi->pohon;
	else if(novy->dalsi!=NULL)novy->pohon=novy->dalsi->pohon;
	if(novy->predchozi->n!=0 && novy->predchozi->eID!=200 && novy->predchozi->pohon!=NULL && F->pom_temp->pohon->n==novy->predchozi->pohon->n || novy->dalsi!=NULL && novy->dalsi->pohon!=NULL && novy->dalsi->pohon->n==F->pom_temp->pohon->n)novy->pohon=F->pom_temp->pohon;

	//název
	AnsiString T="";
	switch(eID)
	{
		case 0: T="Stop"; break;//stop stanice
		case 1:case 7:case 11:case 15:case 101:case 105:  T="Robot"; 			          novy->PD=0;break;//kontinuální robota
		case 2:case 8:case 12:case 16:case 102:case 106:  T="Robot"; 			          novy->PT1=60;break;//robot se stopkou
		case 3:case 9:case 13:case 17:case 103:case 107:  T="Robot"; 			          novy->PD=0;novy->OTOC_delka=0.450;novy->zona_pred=0.3;novy->zona_za=0.3;novy->LO1=(1.5-novy->OTOC_delka)/2.0;novy->LO2=novy->LO1;novy->rotace_jig=180;break;//kontinuální robot s pasivní otočí
		case 4:case 10:case 14:case 18:case 104:case 108: T="Robot";								novy->PT1=60;novy->PTotoc=20;novy->PT2=60;novy->rotace_jig=180; break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 5: T=F->ls->Strings[273]; 										 													novy->OTOC_delka=0.450;novy->zona_pred=0.3;novy->zona_za=0.3;novy->rotace_jig=90;break;//pasivní otoč
		case 6: T=F->ls->Strings[273]; 										 													novy->PTotoc=20;novy->rotace_jig=90;break;//aktivní otoč
		case 100: T=F->ls->Strings[270];break;
		case 200: T=F->ls->Strings[271];break;
		case MaxInt: T="Zarážka";break;
	}
	if(101<=eID && eID<=108)T=F->ls->Strings[272];//"Operátor";
	if(novy->name=="")//přiřazení názvu pouze v případě, že element žádné nemá, při posuvu je novému elementu přiřazeno jméno
	{
		unsigned int nTyp=vrat_poradi_elementu_do(Objekt,novy)+1;//pokud se jedná o roboty
		novy->name=T+" "+AnsiString(nTyp);
		novy->short_name=T.SubString(1,3)+AnsiString(nTyp);
	}

	//mGrid elementu
	if(F->pom_temp!=NULL&&Objekt->n==F->pom_temp->n||novy->eID==MaxInt)//stačí nastavovat pouze v náhledu při vloz_element, nová strategie, je mgrid, nekopírovat a používat jenom v pom_temp, zde však podmínka zda se jedná o pom_temp nebyla z nějakého důvodu možná
	{
		novy->mGrid=new TmGrid(F);
		novy->mGrid->Tag=6;//ID formu
		novy->mGrid->ID=cislo_mGrid;//novy->n;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
	}                  //číslování mGridů zvlášť!!! z důvodu přehazování elementů

	//ukazatel na spárovaný element
	novy->sparovany=NULL;
	vrat_predchozi_stop_element(novy,Objekt);//metoda již v sobě ošetřuje, že se bude jednat o stopku
	//návrácení ukazatelele na element k dalšímu použití
	return novy;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu
void  Cvektory::vloz_element(TObjekt *Objekt,TElement *Element,TElement *force_razeni)
{
	if(Objekt->elementy==NULL)hlavicka_elementy(Objekt);//pokud by ještě nebyla založena hlavička, tak ji založí
	Element->n=Objekt->elementy->predchozi->n+1;//navýším počítadlo prvku o jedničku
	//////Zařazení elementu do seznamu + kontrola pořadí
	if(force_razeni==NULL)
	{
  	//kontrola zda je element vkládán za předchozí nebo mezi předchozí
		Cvektory::TElement *p=vloz_element_za(Objekt,Element);//pokud bude vkládaný elment vložen na konec vrází NULL, pokud mezi vrátí ukazatel na předchozí element
		if(p==NULL)//vkládám na konec
		{
  		//ukazatelové propojení
  		Objekt->elementy->predchozi->dalsi=Element;//poslednímu prvku přiřadím ukazatel na nový prvek
  		Element->predchozi=Objekt->elementy->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
  		Element->dalsi=NULL;
  		Element->sparovany=NULL;
  		Objekt->elementy->predchozi=Element;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
			//geometrie + ošetření proti přiřazování geometrie v metodě kopiruj_elementy
  		if(F->pom_temp!=NULL && Element->n!=1 && Element->Xt==-100)//nutna podminka, pri nacitani z binarky je pom_temp=NULL a nactou se hodnoty OK
  		vloz_G_element(Element,0,Element->predchozi->geo.X4,Element->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,Element->predchozi->geo.orientace);
  	}
  	else if(p->n!=Element->n)//vkládám mezi elementy, vpřípadě, že bylo vloženo před prví prvek vrací Element, přesun je již vyřešen
		{
			//ukazatelové propojení
			Element->dalsi=p;
			Element->predchozi=p->predchozi;
			if(p->n==1)Objekt->elementy->dalsi=Element;
			else p->predchozi->dalsi=Element;
			p->predchozi=Element;

  		//geometrie
			vloz_G_element(Element,0,p->geo.X1,p->geo.Y1,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,p->geo.orientace);
			vloz_G_element(p,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,0,0,0,0,F->d.Rxy(p).x,F->d.Rxy(p).y,p->geo.orientace);
			//změna indexů
			int n=1;
  		Cvektory::TElement *E=Objekt->elementy->dalsi;
  		while(E!=NULL)
  		{
  			//indexy
  			E->n=n;
  			n++;
  			E=E->dalsi;
			}
  		E=NULL;delete E;
  		//změna názvů
			uprav_popisky_elementu(Objekt,Element);
  		//if(Element->eID%2==0 || p->eID%2==0)aktualizuj_sparovane_ukazatele();//došlo ke změně pořadí přičemž jeden z elementů je stop-element
		}
		p=NULL; delete p;
	}
	//////Zarazení do seznamu do předm určeného pořadí (používá se při editaci geometrie)
	else
	{
		if(force_razeni->n==1){Objekt->elementy->dalsi=Element;Element->predchozi=Objekt->elementy;}else {force_razeni->predchozi->dalsi=Element;Element->predchozi=force_razeni->predchozi;}
		Element->dalsi=force_razeni;
		force_razeni->predchozi=Element;
		//změna indexů
		int n=1;
		Cvektory::TElement *E=Objekt->elementy->dalsi;
		while(E!=NULL)
		{
			//indexy
			E->n=n;
			n++;
			E=E->dalsi;
		}
		E=NULL;delete E;
		//změna názvů
		uprav_popisky_elementu(Objekt,Element);
	}
}
////---------------------------------------------------------------------------
//kontrola zda vkládaný element bude umístěn na konec nebo mezi jiné elementy
Cvektory::TElement *Cvektory::vloz_element_za(TObjekt *Objekt,TElement *Element)
{
	Cvektory::TElement *ret=NULL;//návratová proměnná, defaultně prázdn
	//bool pred1=false;zaposlednim=false;//proměnné pro 2 speciální případy které budou řešeny kompletně v této metodě
	if((F->Akce==F->ADD||F->Akce==F->MOVE_ELEMENT)&&Objekt->elementy->dalsi!=NULL)//ošetření proti spouštění při zavírání a otvírání náhledu
	{
		Cvektory::TElement *p=Objekt->elementy->dalsi;//přeskočí hlavičku
		while (p!=NULL)
		{
			if(p->n!=Element->n)//neřeší se s aktuálním elementem (při posunu)
			{
				//kontrola zda vkládaný element neleží mezi prvním a druhým elementem, druhým až n
				if(p->geo.typ==0 && m.LeziVblizkostiUsecky(F->d.Rxy(Element).x,F->d.Rxy(Element).y,p->geo.X1,m.round2double(p->geo.Y1,2),p->geo.X4,m.round2double(p->geo.Y4,2))==0)
				{
					ret=p;//uložení elementu, který předcházi vkládanému elementu
					break;
				}
			}
			p=p->dalsi;//posun na další prvek
		}
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
				if(delka==0)Element->geo.delka=m.delka(Element->geo.X1,Element->geo.Y1,Element->geo.X4,Element->geo.Y4);//pokud nebyla délka dodána
				Element->geo.radius=0;
				Element->geo.rotacni_uhel=0;
				Element->geo.X3=Element->geo.X2=(Element->geo.X1+Element->geo.X4)/2.0;//v případě linie dopočítá kontrolní body bézierovy křivky do středu linie
				Element->geo.Y3=Element->geo.Y2=(Element->geo.Y1+Element->geo.Y4)/2.0;//v případě linie dopočítá kontrolní body bézierovy křivky do středu linie
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
void Cvektory::uprav_popisky_elementu(TObjekt *Objekt, TElement *Element)
{
	bool rename=false;//proměná sloužící k spouštění přejměnování
	AnsiString t_operator=F->ls->Strings[272],t_ion=F->ls->Strings[270],t_otoc=F->ls->Strings[273],t_PM=F->ls->Strings[271];
	if(Element!=NULL)//funkčnost při vložení elementu mezi ostatní, pouze název pořadové čísla byly již změněny
	{     F->log(__func__," ZAČÁTEK");
		//úprava názvu pro roboty
		Cvektory::TElement *E=Objekt->elementy->dalsi;//začíná se od začátku, někdy je potřeba ovlivnit i předchozí elementy
		while (E!=NULL)
		{
			if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18 || E->eID==100 || 101<=E->eID && E->eID<=108)
			{
				if(E->name.SubString(1,5)=="Robot" || E->name.SubString(1,8)==t_operator || E->name.SubString(1,7)==t_ion || E->name=="")rename=true;else rename=false;
				//změna názvu
				if(rename)//přejmenování elementu ve spojáku + mGridu
				{
					int n=vrat_poradi_elementu_do(Objekt,E)+1;//zjistí pořadové číslo elementu
					//změna názvu v hlavičce mGridu, jako první z důvodu podmínky prázdného názvu
					if(E->name!=""&&E->mGrid!=NULL)//nutné, přejmenovávám i první element, který nemá vytvořený mGrid
		  		{
						try
						{
					  	if(E->eID==100)E->mGrid->Cells[0][0].Text="<a>"+t_ion+" "+AnsiString(n)+"</a>";
					  	else if(101<=E->eID && E->eID<=108)E->mGrid->Cells[0][0].Text="<a>"+t_operator+" "+AnsiString(n)+"</a>";
					  	else E->mGrid->Cells[0][0].Text="<a>Robot "+AnsiString(n)+"</a>";
					  	E->mGrid->Cells[0][0].Font->Color=clBlack;//z důvodu nasazení odkazu, po přejmenování se text vrátil do modré barvy
					  	E->mGrid->MergeCells(0,0,1,0);//nutné kvůli správnému zobrazení hlavičky
							if(F->zobrazeni_tabulek)E->mGrid->Update();//musí zde být ošetření proti paměťové chybě
						}catch(...){}
		  		}
					if(E->eID==100)
					{
						E->name=t_ion+" "+AnsiString(n);
						E->short_name="ION"+AnsiString(n);
					}
					else if(101<=E->eID && E->eID<=108)
					{
						E->name=t_operator+" "+AnsiString(n);
						E->short_name=E->name.SubString(1,3)+AnsiString(n);
					}
					else
					{
						E->name="Robot "+AnsiString(n);
						E->short_name="Rob"+AnsiString(n);
					}
				}
			}
			E=E->dalsi;//posun na další prvek
		}
		E=NULL; delete E;    F->log(__func__," KONEC");
		Cvektory::TObjekt *O=OBJEKTY->dalsi;//prěskočí hlavičku
		while(O!=NULL)//!!!!!!!!!!!!!dochází k zacyklení při přidání druhé stopky před první v druhé kabině!!!!!!!!!!!!!
		{
			if(O->n>=Objekt->n)//přeskakování objektů před aktuálním
			{
				Cvektory::TElement *E=O->elementy;//nepřeskakovat hlavičku
				if(F->pom_temp!=NULL && O->n==F->pom_temp->n)E=F->pom_temp->elementy;//při procházení aktuálního objektu nahradit pom_temp
				while(E!=NULL)
				{
					if(E->n>0)//přeskočí hlavičku
					{
						//kontrola zda můžu název změnit
						switch(E->eID)
						{
							case 0:if(E->name.SubString(1,5)=="Stop " || E->name=="")rename=true;else rename=false;break;
							default :rename=false;break;//musí zde být, jinak nějakým způsobem je pro robot rename nastaveno na true
							case 5:
							case 6:if(E->name.SubString(1,4)==t_otoc || E->name.SubString(1,7)==t_otoc/*"Otoč" || "Turning"*/ || E->name=="")rename=true;else rename=false;break;
							case 200:if(E->name.SubString(1,15)==t_PM || E->name.SubString(1,14)==t_PM/*"Předávací místo" || "Transfer point"*/ || E->name=="")rename=true;else rename=false;break;
						}
						//nezměněn nebo nemá název -> mohu změnit
						if(rename)
						{
							int n=vrat_poradi_elementu_do(O,E)+1;//zjistí pořadové číslo elementu
							//změna názvu v mGridu
							if(E->name!=""&&O->n==Objekt->n&&E->mGrid!=NULL)//nelze přistupovat k mGridu v případech nového elementu (nemá vytvořený), v neaktivní kabině (elementy nemají vytvořene mGridy)
							{
								try//dodatečné ošetření
								{
							  	if(E->eID==0)E->mGrid->Cells[0][0].Text="<a>Stop "+AnsiString(n)+"</a>";
							  	if(E->eID==5 || E->eID==6)E->mGrid->Cells[0][0].Text="<a>"+t_otoc+" "+AnsiString(n)+"</a>";
							  	if(E->eID==200)E->mGrid->Cells[0][0].Text="<a>"+t_PM+" "+AnsiString(n)+"</a>";
							  	E->mGrid->Cells[0][0].Font->Color=clBlack;//z důvodu nasazení odkazu, po přejmenování se text vrátil do modré barvy
							  	E->mGrid->MergeCells(0,0,1,0);//nutné kvůli správnému zobrazení hlavičky
									if(F->zobrazeni_tabulek)E->mGrid->Update();//musí zde být ošetření proti paměťové chybě
								}catch(...){}
//	 							if(E->eID==0)napln_combo_stopky(E);//pro budoucí použití
							}
		 					//změna názvu
							switch(E->eID)
							{
								case 0:E->name="Stop "+AnsiString(n);E->short_name="Sto"+AnsiString(n);break;
								case 5:
								case 6:E->name=t_otoc+" "+AnsiString(n);E->short_name=E->name.SubString(1,3)+AnsiString(n);break;
								case 200:E->name=t_PM+" "+AnsiString(n);E->short_name=E->name.SubString(1,3)+AnsiString(n);break;
							}
						}
					}
					E=E->dalsi;
				}
				E=NULL; delete E;
			}
			O=O->dalsi;
		}
		O=NULL; delete O;
	}  //storno funkcionalitu nutno podrobit dalším testům
	else//spuštěno při stisku tlačítka storno, musí dojít k přejmenování na původní, mění se název i pořadová čísla
	{
		Cvektory::TObjekt *O=OBJEKTY->dalsi;
		while(O!=NULL)
		{
			if(O->n>=Objekt->n)//přeskakování objektů před aktuálním
			{
				Cvektory::TElement *E=O->elementy;//nepřeskakovat hlavičku
	   		int n_spojak=1;//hodnota n pro seznam (element)
	   		while(E!=NULL)
	   		{
					if(E->n>0)
					{
						//kontrola zda má element původní název
	   				switch(E->eID)
						{
							//stopka
							case 0:
							//roboti
							if(E->name.SubString(1,5)=="Stop "/*&&E->name.Length()<=6*/)rename=true;else rename=false;break;
							case 1:case 7:case 11:case 15:
							case 2:case 8:case 12:case 16:
							case 3:case 9:case 13:case 17:
							case 4:case 10:case 14:case 18:
							if(E->name.SubString(1,6)=="Robot "/*&&E->name.Length()<=7*/)rename=true;else rename=false;break;
							//otoče
							case 5:
							case 6:
							if(E->name.SubString(1,4)==t_otoc || E->name.SubString(1,7)==t_otoc)rename=true;else rename=false;break;
							//operátoři
							case 101:case 105:case 102:case 106:case 103:case 107:case 104:case 108:
							if(E->name.SubString(1,8)==t_operator)rename=true;else rename=false;break;
							//předávací místo
							case 200:
							if(E->name.SubString(1,14)==t_PM||E->name.SubString(1,15)==t_PM)rename=true;else rename=false;break;
						}
						//pokud má původní název -> přejmenovat
						if(rename)
						{
							int n_nazev=0;//n sloužící pro název, kolikáty element
							n_nazev=vrat_poradi_elementu_do(O,E)+1;//předávání ukazatele na plný objekt
							//změna názvu
							switch(E->eID)
							{
								//stopka
								case 0:
								E->name="Stop "+AnsiString(n_nazev);break;
								//roboti
								case 1:case 7:case 11:case 15:
								case 2:case 8:case 12:case 16:
								case 3:case 9:case 13:case 17:
								case 4:case 10:case 14:case 18:
								E->name="Robot "+AnsiString(n_nazev);break;
								//otoče
								case 5:
								case 6:
								E->name=t_otoc+" "+AnsiString(n_nazev);break;
								//operátoři
								case 101:case 105:case 102:case 106:case 103:case 107:case 104:case 108:
								E->name=t_operator+" "+AnsiString(n_nazev);break;
								//předávací místo
								case 200:
								E->name=t_PM+" "+AnsiString(n_nazev);break;
	   					}
						}
						//změna n ve spojáku
						E->n=n_spojak;
						n_spojak++;
					}
					E=E->dalsi;
	   		}
				E=NULL; delete E;
			}
			O=O->dalsi;
		}
		O=NULL; delete O;
	}
}
////---------------------------------------------------------------------------
//zkopíruje atributy elementu bez ukazatelového propojení, pouze ukazatelové propojení na mGrid je zachováno
void Cvektory::kopiruj_element(TElement *Original, TElement *Kopie)
{
	Kopie->n=Original->n;
	Kopie->eID=Original->eID;
	Kopie->short_name=Original->short_name;
	Kopie->name=Original->name;
	Kopie->X=Original->X;
	Kopie->Y=Original->Y;
	Kopie->Xt=Original->Xt;
	Kopie->Yt=Original->Yt;
	Kopie->orientace=Original->orientace;
	Kopie->rotace_jig=Original->rotace_jig;
	Kopie->stav=Original->stav;
	Kopie->PD=Original->PD;
	Kopie->LO1=Original->LO1;
	Kopie->OTOC_delka=Original->OTOC_delka;
	Kopie->zona_pred=Original->zona_pred;
	Kopie->zona_za=Original->zona_za;
	Kopie->LO2=Original->LO2;
	Kopie->LO_pozice=Original->LO_pozice;
	Kopie->PT1=Original->PT1;
	Kopie->PTotoc=Original->PTotoc;
	Kopie->PT2=Original->PT2;
	Kopie->WT=Original->WT;
	Kopie->WTstop=Original->WTstop;
	Kopie->RT=Original->RT;
	Kopie->akt_pocet_voziku=Original->akt_pocet_voziku;
	Kopie->max_pocet_voziku=Original->max_pocet_voziku;
	Kopie->geo=Original->geo;
	Kopie->mGrid=new TmGrid(F);//nová strategie, je mgrid, nekopírovat a používat jenom v pom_temp, zde však podmínka zda se jedná o pom_temp nebyla z nějakého důvodu možná
	Kopie->objekt_n=Original->objekt_n;
	if(Original->pohon!=NULL)Kopie->pohon=vrat_pohon(Original->pohon->n);
	else Kopie->pohon=Original->pohon;
	Kopie->sparovany=Original->sparovany;
}
////---------------------------------------------------------------------------
void Cvektory::kopiruj_elementy(TObjekt *Original, TObjekt  *Kopie)//zkopíruje elementy a jejich atributy bez ukazatelového propojení z objektu do objektu, pouze ukazatelové propojení na mGrid je zachováno spojuje dvě metody vloz_element(TObjekt *Objekt,TElement *Element) a kopiruj_element(TElement *Original, TElement *Kopie)
{
  TElement *E=Original->elementy;//nepřeskakuje se hlavička, protože v ní jsou uloženy výchozí souřadnice
	//vytvor_elementarni_osu(Original,Kopie);//smazat
	if(E!=NULL)//pokud elementy existují nakopíruje je do pomocného spojáku pomocného objektu
	{
   	E=E->dalsi;//přeskočí hlavičku //smazat
		while(E!=NULL)
		{
			TElement *Et=new TElement;
			vloz_element(Kopie,Et);//pokus zaměna pořadí
			kopiruj_element(E,Et);//pokus zaměna pořadí - kvůli dodatečnému vložení geometrie
			E=E->dalsi;//posun na další element
		}
	}
	E=NULL;delete E;
}
////---------------------------------------------------------------------------
//připraví vektor provizorní osy pohonu
void Cvektory::vytvor_elementarni_osu(TObjekt *Original, TObjekt  *Kopie)
{                 //smazat
	if(Original==F->pom && Kopie==F->pom_temp)
	{
		hlavicka_elementy(Kopie);
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
	if(Objekt->elementy!=NULL)
	{
		TElement *E=Objekt->elementy->dalsi;//přeskočí rovnou hlavičku
		while(E!=NULL)
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
//vratí pořádí stopek, robotů a otočí, zatím pouze v elementu, bude na zvážení rozšíření na všechny objekty
unsigned int Cvektory::vrat_poradi_elementu(TObjekt *Objekt,unsigned int eID)
{
	unsigned int RET=0;
	if(Objekt->elementy!=NULL)
	{
		TElement *E=Objekt->elementy->dalsi;//přeskočí rovnou hlavičku
		while(E!=NULL)
		{
			switch(eID)
			{
				case 0:if(E->eID==0)RET++;break;
				case 1:case 7:case 11:case 15:case 101:case 105:
				case 2:case 8:case 12:case 16:case 102:case 106:
				case 3:case 9:case 13:case 17:case 103:case 107:
				case 4:case 10:case 14:case 18:case 104:case 108:if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18 || 101<=E->eID && E->eID<=108)RET++;break;
				case 5:
				case 6:if(5<=E->eID && E->eID<=6)RET++;break;
			}
			E=E->dalsi;
		}
		E=NULL;delete E;
	}
	return RET;
}
////---------------------------------------------------------------------------
//vrátí počet stejných elementů před Element, u robotů v jednom objektu, u otočí a stopek vrátí počet i z předchozích objektů
unsigned int Cvektory::vrat_poradi_elementu_do (TObjekt *Objekt, TElement *Element)
{
	unsigned int r_pocet=0,s_pocet=0,o_pocet=0,t_pocet=0,pm_pocet=0,z_pocet=0,ret=0;//nastavení všech počtů na nulu
	if(Element->eID!=0 && Element->eID!=5 && Element->eID!=6 && Element->eID!=200 && Element->eID!=MaxInt)//  Element->eID!=6&&Element->eID!=5&&Element->eID!=0)//pokud je Element robot projdi roboty v Objektu
	{
		Cvektory::TElement *E=Objekt->elementy->dalsi;//přeskočí hlavičku
		while(E->n!=Element->n)
		{
			if(E->eID!=0 && E->eID!=5 && E->eID!=6 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt)r_pocet++;
			if(E->eID==100)t_pocet++;
			E=E->dalsi;
		}
		E=NULL; delete E;
	}
	else//elementy, které jou číslované globálně
	{
		//procházení předchozích objektů
		Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
		while(O!=NULL)
		{
			Cvektory::TElement *E=O->elementy;//nastaveno na hlavičku, ošetřeno níže
			if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
			while(E!=NULL)
			{
				if(O->n==Objekt->n && E->n==Element->n)break;//ukončení prohledávání když jsem na aktuálním elmentu
				if(E->n>0)//ošetření pro hlavičku
				{
					if(E->eID==0)s_pocet++;
					if(E->eID==5||E->eID==6)o_pocet++;
					if(E->eID==200)pm_pocet++;
					if(E->eID==MaxInt)z_pocet++;
				}
				E=E->dalsi;
			}
			E=NULL; delete E;
			if(O->n==Objekt->n)break;
			O=O->dalsi;
		}
		O=NULL; delete O;
	}
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
	if(Objekt->elementy!=NULL)
	{
		TElement *E=Objekt->elementy;
		while(E!=NULL)
		{
			if(E->n>0&&E->eID!=100)//přeskočení hlavičky a elementu bez tabulky
			{
				if(ret<(unsigned)E->mGrid->ID)ret=E->mGrid->ID;
      }
			E=E->dalsi;
		}
		E=NULL;delete E;
  }
	return ret;
}
////---------------------------------------------------------------------------
//vrátí typ elementu -1 nenastaven nebo zarážka či předávací místo, 0 - S&G( včetně stopky), 1 - kontinuál
short Cvektory::vrat_druh_elementu(TElement *Element)
{
	short RET=-1;                                   //nutné přeskakovat elementarní hlavičku!
	if(Element->eID!=MaxInt && Element->eID!=200 && Element->n>0)
	{
		if(Element->eID%2==0 && Element->eID!=100)RET=0;//S&G elementy
		else RET=1;//kontinuální elementy
	}
	return RET;
}
////---------------------------------------------------------------------------
//vrátí true, pokud se jedná o funční element
bool Cvektory::funkcni_element(TElement *Element)
{                                                 //nutné přeskakovat elementarní hlavičku!
	if(Element->eID!=MaxInt && Element->eID!=200 && Element->n>0)return true;
	else return false;
}
////---------------------------------------------------------------------------
//orotuje všechny elementy daného objektu o danou hodnotu
void Cvektory::rotace_elementu(TObjekt *Objekt,short rotace)
{
	TElement *E=Objekt->elementy->dalsi;//přeskočí rovnou hlavičku
	while(E!=NULL)
	{
		if(E->orientace+rotace>360)E->orientace=0;//kvůli přetýkání na dvě podmínky
		E->orientace+=rotace;
		if(E->orientace==360)E->orientace=0;
		E=E->dalsi;
	}
	E=NULL;delete E;
}
////---------------------------------------------------------------------------
//hledá element v místě kurzoru pracuje v logických/metrických souradnicích
Cvektory::TElement *Cvektory::najdi_element(TObjekt *Objekt, double X, double Y)
{
	//vhodno přesunout do globálních proměnných do Cvykresli
	float otoc_sirka=3.5;//ve skutečnosti poloměr
	float otoc_tloustka=0.8;

	//algoritmus prochází jednotlivé elementy a porovnává vůči jejich pozici aktuální pozici kurzoru, aby se zbytečně netestovalo vše (metoda se volá neustále při každém posunu kurzoru), postupuje algoritmus maximálně větveně (šetření strojového času), tedy v případě uspěchu ihned končí, v případě neúspěchu testuje dále
	TElement *E=Objekt->elementy;//NEPŘESKAKOVAT hlavičku!!! kvůli ošetření ohledně existence elementu v objektu
	while(E!=NULL)
	{
		if(E->n!=0)
		{
			short rotace=E->orientace;
			if(E->eID==0)//STOPKY
			{
				rotace=m.Rt90(rotace+180);//stopka je o 180° orotovaná
				if(m.PtInStopka(E->X,E->Y,X,Y,rotace) || E->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))break;//testování symbolu včetně popisku,pozn. CreatePolygonRgn i PtInRect - zahrnuje pouze vnitřní tvar, obrys tvaru je z oblasti vyloučen
				else E=E->dalsi;
			}
			else
			{
				if(E->eID==5 || E->eID==6)//OTOČE
				{
					if(m.PtInCircle(X,Y,E->X,E->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px) || E->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))break;//testování symbolu včetně popisku,pozn.
					else E=E->dalsi;
				}
				else
				{
					if(E->eID==100)//ION tyč
					{
						if(m.PtInIon(E->X,E->Y,X,Y,rotace) || E->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))break;
						else E=E->dalsi;
					}
					else
					{
						if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18)//ROBOTI
						{
							//hledání, zda leží v regionu, region se liší dle rotace
							HRGN hreg;
							double DoSkRB=F->d.DoSkRB;
							switch(rotace)
							{
								case 0: hreg=CreateRectRgn(m.L2Px(E->X-F->d.Robot_delka_zakladny/2.0),m.L2Py(E->Y+DoSkRB),m.L2Px(E->X+F->d.Robot_delka_zakladny/2.0),m.L2Py(E->Y-F->d.Robot_sirka_zakladny/2.0));break;
								case 90:hreg=CreateRectRgn(m.L2Px(E->X-F->d.Robot_sirka_zakladny/2.0),m.L2Py(E->Y+F->d.Robot_delka_zakladny/2.0),m.L2Px(E->X+DoSkRB),m.L2Py(E->Y-F->d.Robot_delka_zakladny/2.0));break;
								case 180:hreg=CreateRectRgn(m.L2Px(E->X-F->d.Robot_delka_zakladny/2.0),m.L2Py(E->Y+F->d.Robot_sirka_zakladny/2.0),m.L2Px(E->X+F->d.Robot_delka_zakladny/2.0),m.L2Py(E->Y-DoSkRB));DoSkRB*=-1;break;
								case 270:hreg=CreateRectRgn(m.L2Px(E->X-DoSkRB),m.L2Py(E->Y+F->d.Robot_delka_zakladny/2.0),m.L2Px(E->X+F->d.Robot_sirka_zakladny/2.0),m.L2Py(E->Y-F->d.Robot_delka_zakladny/2.0));DoSkRB*=-1;break;
							}
							if(PtInRegion(hreg,m.L2Px(X),m.L2Py(Y)))break;
							else//pokud nenalezeno, testuje ještě případně OTOČE ROBOTŮ
							{
								if(E->eID==3 || E->eID==4 || E->eID==9 || E->eID==10 || E->eID==13 || E->eID==14 || E->eID==17 || E->eID==18)
								{
									if(rotace==0 || rotace==180)
									{
										if(m.PtInCircle(X,Y,E->X,E->Y+DoSkRB,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))break;//ROBOTi s otočemi
										else E=E->dalsi;
									}
									else//90°, 270°
									{
										if(m.PtInCircle(X,Y,E->X+DoSkRB,E->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))break;//ROBOTi s otočemi
										else E=E->dalsi;
									}
								}
								else//ani mezi otočemi robotu nenalezeno, hledá mezi STOPKAMI ROBOTŮ
								{
									if(E->eID==2 || E->eID==4 || E->eID==8 || E->eID==10 || E->eID==12 || E->eID==14 || E->eID==16 || E->eID==18)
									{
										if(rotace==0 || rotace==180)
										{
											if(m.PtInStopka(E->X,E->Y+DoSkRB,X,Y,rotace))break;//ROBOTi se stopkami
											else E=E->dalsi;
										}
										else//90°, 270°
										{
											if(m.PtInStopka(E->X+DoSkRB,E->Y,X,Y,rotace))break;//ROBOTi se stopkami
											else E=E->dalsi;
										}
									}
									else E=E->dalsi;//ani zde nenalezeno
								}
							}
						}
						else//ani roboti nanelezeny, hledá tedy mezi LIDSKÝMI ROBOTY
						{
						 if(101<=E->eID && E->eID<=108)
						 {
								if(m.PtInClovek(E->X,E->Y,X,Y,rotace,E->eID)|| E->citelna_oblast.rect3.PtInRect(TPoint(m.L2Px(X),m.L2Py(Y))))break;
								else //pokud nenalezeno, testuje ještě případně otoče lidských robotů
								{
									double DkRB=F->d.DkRB;if(rotace==180 || rotace==270)DkRB*=-1;
									if(E->eID==103 || E->eID==104 || E->eID==107 || E->eID==108)//s otočemi
									{
										if(rotace==0 || rotace==180)
										{
											if(m.PtInCircle(X,Y,E->X,E->Y+DkRB,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))break;
											else E=E->dalsi;
										}
										else//90°, 270°
										{
											if(m.PtInCircle(X,Y,E->X+DkRB,E->Y,(otoc_sirka+otoc_tloustka/2.0)*F->m2px))break;
											else E=E->dalsi;
										}
									}
									else//ani mezi otočemi lidských robotu nenalezeno, hledá mezi STOPKAMI LIDSKÝCH ROBOTŮ
									{
										if(E->eID==102 || E->eID==104 || E->eID==106 || E->eID==108)
										{
											if(rotace==0 || rotace==180)
											{
												if(m.PtInStopka(E->X,E->Y+DkRB,X,Y,rotace))break;//ROBOTi se stopkami
												else E=E->dalsi;
											}
											else//90°, 270°
											{
												if(m.PtInStopka(E->X+DkRB,E->Y,X,Y,rotace))break;//ROBOTi se stopkami
												else E=E->dalsi;
											}
										}
										else E=E->dalsi;//ani zde nenalezeno
									}
								}
						 }
						 else if(E->eID==MaxInt && F->Akce==F->GEOMETRIE)//zarážka
						 {
							if(m.PtInCircle(X,Y,E->X,E->Y,0.3))break;
							else E=E->dalsi;
						 }
						 else if(E->eID==200 && ((E->orientace==0 || E->orientace==180) && m.PtInRectangle(E->X-0.1,E->Y-0.25,E->X+0.1,E->Y+0.25,X,Y) || (E->orientace==90 || E->orientace==270) && m.PtInRectangle(E->X-0.25,E->Y-0.1,E->X+.25,E->Y+0.1,X,Y)))break;
						 else E=E->dalsi;//pokud E neodpovídá žádnému odchytávanému elementu, může se ještě  jednat např. o element zarážka
						}
					}
				}
			}
		}
		else  E=E->dalsi;
	}
	return E;
}
////---------------------------------------------------------------------------
//hledá tabulku elementu pouze pro daný objekt v oblasti definované pomocí šířky a výšky tabulky (která se může nacházet v daném místě kliku), pracuje v logických/metrických souradnicich, vrátí ukazatel na daný element, který tabulku vlastní, pokud se na daných souřadnicích nachází tabulka
Cvektory::TElement *Cvektory::najdi_tabulku(TObjekt *Objekt, double X, double Y)
{
	TElement *E=Objekt->elementy,*ret=NULL;//NEPŘESKAKOVAT hlavičku!!! kvůli ošetření ohledně existence elementu v objektu
	while(E!=NULL)
	{
    //pokud mgrid existuje hledá poslední mGrid na pozici X,Y ... nesmí být po nalezení break
		if(E->mGrid!=NULL && E->Xt<=X && X<=E->Xt+E->mGrid->Width*F->m2px/F->Zoom && E->Yt>=Y && Y>=E->Yt-E->mGrid->Height*F->m2px/F->Zoom)ret=E;
		E=E->dalsi;
	}
	delete E;E=NULL;
	return ret;
}
////---------------------------------------------------------------------------
//vraťí ukazatel na element dle n elementu umístěného v daném objektu
Cvektory::TElement *Cvektory::vrat_element(TObjekt *Objekt, unsigned int n)
{
	TElement *E=Objekt->elementy;//NEPŘESKAKOVAT hlavičku!!!
	while(E!=NULL)
	{
		if(E->n==n) break;
		else E=E->dalsi;
	}
	return E;
}
////---------------------------------------------------------------------------
//ověří zda se na daných fyzických souřadnicích nachází kóta elementu, pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty, pozn. oblast kóty se testuje až jako poslední
short Cvektory::PtInKota_elementu(TObjekt *Objekt,long X,long Y)
{
	short RET=-1;//nic nenalezeno
	double x=F->akt_souradnice_kurzoru_PX.x,y=F->akt_souradnice_kurzoru_PX.y;//souřadnice kurzoru jsou neměnné po celou dobu metody, důležité použít fyzické souřadnice !!
	TElement *E=Objekt->elementy;//NEPŘESKAKOVAT hlavičku!!!
	while(E!=NULL)
	{
		if(E->citelna_oblast.rect1.PtInRect(TPoint(X,Y)) && (E->pohon!=NULL || E->pohon==NULL && F->DKunit<2)){RET=1;F->pom_element=E;break;}//hodnoty kóty
		else if(E->citelna_oblast.rect4.left!=E->citelna_oblast.rect4.right!=0 && E->citelna_oblast.rect4.PtInRect(TPoint(X,Y))){RET=3;F->pom_element=E;break;}
		else
		{
			if(E->citelna_oblast.rect2.PtInRect(TPoint(X,Y))){RET=2;F->pom_element=E;break;}//jednotky kóty
			else if(m.LeziVblizkostiUsecky(x,y,E->citelna_oblast.rect0.left,E->citelna_oblast.rect0.top,E->citelna_oblast.rect0.right,E->citelna_oblast.rect0.bottom)<=1){RET=0;F->pom_element=E;break;}//kóta celá
		}
		E=E->dalsi;
	}
	E=NULL;delete E;
	return RET;
}
////---------------------------------------------------------------------------
//posune pouze Element z pomocného spojového seznamu pom_temp na parametrem uvedenou vzádlenost (v metrech) od elementu předchozího, pokud je implicitní hodnota pusun_dalsich_elementu false změněna na true, jsou o danou změnu posunu přesunuty i elementy následující Elementu (tudíž jejich vzdálenost od Elementu bude zachována, naopak v případě výchozí hodnoty false je následujícím/dalším elementům poloha zachována)
bool Cvektory::posun_element(TElement *Element,double vzdalenost,bool pusun_dalsich_elementu,bool posun_kurzorem,bool kontrola_zmeny_poradi)
{ //!!!!!!po nasazení geometrie nutno zdokonalit, nebude se pracovát pouze se vzdálenosti na linii buď vodorvné či svislé, ale i v oblouku
	Cvektory::TElement *E=NULL;
	bool RET=true;
	if(F->pom_temp!=NULL && F->pom_temp->elementy!=NULL/*&&F->Akce!=F->MOVE_ELEMENT*/)//raději ošetření, ač by se metoda měla volat jen v případě existence pom_temp
	{
		bool posun_povolit=true;
		TPointD puv_souradnice;
		puv_souradnice.x=Element->X;puv_souradnice.y=Element->Y;
		if(F->pom_temp->elementy->dalsi!=NULL&&vzdalenost!=0)//musí existovat alespoň jeden element&&nesmí být vzdálenost rovna nule
		{
			//////Výpočet posunu
			TPointD vzd;
			if(Element->n==1)//pro první element, od počátku kabiny
			{
				if(Element->orientace==0||Element->orientace==180)vzd.x=Element->X-F->pom_temp->elementy->dalsi->geo.X1;
				else vzd.x=Element->Y-F->pom_temp->elementy->dalsi->geo.Y1;
			}
			else//více elementů
			{
				if(Element->orientace==0||Element->orientace==180)vzd.x=Element->X-Element->predchozi->X;
				else	vzd.x=Element->Y-Element->predchozi->Y;
			}
			if((Element->dalsi!=NULL && Element->dalsi->geo.typ!=0 || Element->geo.typ!=0) && kontrola_zmeny_poradi)posun_povolit=false;//pokud by element ovlivnil posunem geometrii
			//////Realizace posunu + validace
			if(vzd.x!=0 && !posun_kurzorem && posun_povolit)//posun z kót
			{
				//realizace posunu
				if(Element->orientace==0||Element->orientace==180)Element->X=Element->X-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
				else Element->Y=Element->Y-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
        //kontrola zda je element stále na linii
				if(F->bod_na_geometrii(0,0,Element) || Element->n==F->pom_temp->elementy->predchozi->n || !kontrola_zmeny_poradi)//pokud ano
				{
					//kontrola + změna pořadí
					if(kontrola_zmeny_poradi)
					{
						E=vloz_element_za(F->pom_temp,Element);
						if(E!=NULL && Element->dalsi!=NULL && E->n!=Element->dalsi->n)zmen_poradi_elementu(Element,E);
					}
					//aktualizace posouvaného elementu
			  	vloz_G_element(Element,0,Element->geo.X1,Element->geo.Y1,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,Element->geo.orientace);
					if(kontrola_zmeny_poradi)
					{
				  	//aktualizace dalšího elemtnu
				  	if(Element->dalsi!=NULL)vloz_G_element(Element->dalsi,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
				  	//aktualizace RT
						if(Element->dalsi!=NULL&&!pusun_dalsich_elementu){reserve_time(Element,true,true);reserve_time(Element->sparovany,true,true);}//při změně vzdálenosti je nutno dopočítat znova RT, pokud je za robotem další robot jeho RT musí být také přepočítáno
						else reserve_time(Element,true,true);
					}
				}
				//pokud ne budou mu navráceny původní souřadnice
				else {Element->X=puv_souradnice.x;Element->Y=puv_souradnice.y;posun_povolit=false;}
			}
			else if(vzd.x!=0 && posun_kurzorem && posun_povolit)//posun kurozem
			{
				//realizace posunu
				if(Element->orientace==0||Element->orientace==180)Element->X=Element->X+vzdalenost;
				else Element->Y=Element->Y+vzdalenost;
				//kontrola zda je element stále na linii
				if(F->bod_na_geometrii(0,0,Element) || Element->n==F->pom_temp->elementy->predchozi->n || !kontrola_zmeny_poradi)//pokud ano
				{
					//kontrola + změna pořadí
					if(kontrola_zmeny_poradi)
					{
						E=vloz_element_za(F->pom_temp,Element);
						if(E!=NULL && Element->dalsi!=NULL && E->n!=Element->dalsi->n)zmen_poradi_elementu(Element,E);
					}
					//aktualizace posouvaného elementu
					vloz_G_element(Element,0,Element->geo.X1,Element->geo.Y1,0,0,0,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,Element->geo.orientace);
					if(kontrola_zmeny_poradi)
					{
				  	//aktualizace dalšího elemtnu
				  	if(Element->dalsi!=NULL)vloz_G_element(Element->dalsi,0,F->d.Rxy(Element).x,F->d.Rxy(Element).y,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
				  	//aktualizace RT
						if(Element->dalsi!=NULL&&!pusun_dalsich_elementu){reserve_time(Element,true,true);reserve_time(Element->sparovany,true,true);}//při změně vzdálenosti je nutno dopočítat znova RT, pokud je za robotem další robot jeho RT musí být také přepočítáno
						else reserve_time(Element,true,true);
					}
				}
				//pokud ne budou mu navráceny původní souřadnice
				else {Element->X=puv_souradnice.x;Element->Y=puv_souradnice.y;posun_povolit=false;}
			}
			//////Posun dalsích elementů
			if(pusun_dalsich_elementu && posun_povolit)
			{
				TElement *E=Element->dalsi;
				while(E!=NULL)
				{
					puv_souradnice.x=E->X;puv_souradnice.y=E->Y;
					if(E->geo.typ!=0)break;//ukončení v případě, že se někde nachází jiná geometrie než linie
					if(vzd.x!=0 && !posun_kurzorem && E->eID!=MaxInt && E->eID!=200)//neposunovat zarážku
					{
						if(Element->orientace==0||Element->orientace==180)E->X=E->X-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);//výpočet pro posuv z kót
						else E->Y=E->Y-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
					}
					if(vzd.x!=0 && posun_kurzorem && E->eID!=MaxInt && E->eID!=200)
					{
						if(Element->orientace==0||Element->orientace==180)E->X=E->X+vzdalenost;//výpočet pro posun kurzorem
						else E->Y=E->Y+vzdalenost;
					}
          //kontrola zda je element stále na linii
					if(F->bod_na_geometrii(0,0,E))//pokud ano
					{
						//aktualizace geometrie
						vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
					}
					//pokud ne budou mu navráceny původní souřadnice
					else {E->X=puv_souradnice.x;E->Y=puv_souradnice.y;}
					E=E->dalsi;
				}
				E=NULL;delete E;
			}
		}
		RET=posun_povolit;
	}
	E=NULL; delete E;
	return RET;
}
////---------------------------------------------------------------------------
//řeší změnu pořadí při posuvu elementů, dojde k novému ukazatelovému propojení, přejmenování a přeindexování elementů
void Cvektory::zmen_poradi_elementu(TElement *E,TElement *Ed)
{               	                          //E = posouvaný element, Ed = další element
	//////přesun elementu před
	if(E->n>Ed->n)
	{
		//ukazatelové záležitosti
		E->dalsi->predchozi=Ed;
		Ed->dalsi=E->dalsi;
		if(Ed->n==1)F->pom_temp->elementy->dalsi=E;
		E->predchozi=Ed->predchozi;
		Ed->predchozi->dalsi=E;
		Ed->predchozi=E;
		E->dalsi=Ed;//musí být na konci!!!!!
		//aktualizace geometrie
		vloz_G_element(Ed->dalsi,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,0,0,0,0,F->d.Rxy(Ed->dalsi).x,F->d.Rxy(Ed->dalsi).y,Ed->dalsi->geo.orientace);
		vloz_G_element(E,0,Ed->geo.X1,Ed->geo.Y1,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
		vloz_G_element(Ed,0,F->d.Rxy(E).x,F->d.Rxy(E).y,0,0,0,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,Ed->geo.orientace);
	}
	//////přesun elementu za
	else
	{
		//ukazatelové záležitosti
		E->dalsi=Ed;
		Ed->predchozi->predchozi=E->predchozi;
		E->predchozi=Ed->predchozi;
		Ed->predchozi->predchozi->dalsi=Ed->predchozi;
		//if(E->n==1)F->pom_temp->elementy->dalsi=Ed->predchozi;
		Ed->predchozi->dalsi=E;
		Ed->predchozi=E;
		////aktualizace geometrie
		vloz_G_element(E->predchozi,0,E->geo.X1,E->geo.Y1,0,0,0,0,F->d.Rxy(E->predchozi).x,F->d.Rxy(E->predchozi).y,E->predchozi->geo.orientace);
		vloz_G_element(Ed,0,F->d.Rxy(E).x,F->d.Rxy(E).y,0,0,0,0,F->d.Rxy(Ed).x,F->d.Rxy(Ed).y,Ed->geo.orientace);
		vloz_G_element(E,0,F->d.Rxy(E->predchozi).x,F->d.Rxy(E->predchozi).y,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
	}
	E->pohon=E->dalsi->pohon;//přiřazení správného pohonu
	//////přeindexování (N-hodnoty) v celém seznamu, možno řešit sepáratáně, ale takto to bylo rychleji napsané
	TElement *E_pom=F->pom_temp->elementy->dalsi;//ukazatel na první element, přeskočí hlavičku, metoda volaná jen v případě, že existují min. 2 elementy
	int n=1;
	while (E_pom!=NULL)
	{
		E_pom->n=n;
		n++;
		E_pom=E_pom->dalsi;
	}
	delete E_pom;E_pom=NULL;
	uprav_popisky_elementu(F->pom_temp,E);//změna názvů
	if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt || Ed->eID%2==0 && Ed->eID!=100 && Ed->eID!=200 && Ed->eID!=MaxInt)aktualizuj_sparovane_ukazatele();//změna pořadí přičemž alespoň jeden element byl stop-element
}
////---------------------------------------------------------------------------
//vratí vzdálenost od předchozího elementu, pracuje zatím pouze v orotogonalizovaném prostoru (bude nutno vylepšit s příchodem oblouků), pokud se jedná o první element, uvažuje se jako vzdálenost od počátku kabiny (nutno vylepšit ještě pro různé orientace kabiny)
double Cvektory::vzdalenost_od_predchoziho_elementu(TElement *Element,bool pouzeSG)
{                  //dodělat MaVl
	if(pouzeSG)//vzdálenost od předchozího SG elementu, slouží k výpočtům RT, zatím provizorně, v budoucnu nutno rozšířit na předchozí kabiny + vyhybky!!!!
	{
		double celkem=0;
		//pokud je element první v kabině
//		if(Element->n==1)
//		{
//			if(Element->orientace==0||Element->orientace==180)return m.delka(F->pom_temp->elementy->dalsi->geo.X1,F->pom_temp->elementy->dalsi->geo.Y1,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
//			else return m.delka(F->pom_temp->elementy->dalsi->geo.X1,F->pom_temp->elementy->dalsi->geo.Y1,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
//		}
//		else//pokud je v kabině více elementů
//		{
//			//procházení objektu a hledání předchozího SG elementu
//			Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//provizorně může být použito pom_temp, volání metody pouze když je pom_temp naplněné
//			while(E->n!=Element->n&&E!=NULL)
//			{
//				//procházím kabinu od začátku, pokud je element SG uložím jeho vzdálenost k elementu pro kterého hledám vzdálenost k předchozímu
//				if(E->eID==0||E->eID%2==0)celkem=m.delka(F->d.Rxy(Element).x,F->d.Rxy(Element).y,F->d.Rxy(E).x,F->d.Rxy(E).y);
//				E=E->dalsi;
//			}
//			E=NULL; delete E;
//			//pokud byla nalezena alespoň jedna vzdálenost
//			if(celkem!=0)return celkem;
//			else return m.delka(F->pom_temp->elementy->dalsi->geo.X1,F->pom_temp->elementy->dalsi->geo.Y1,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
//		}
			////////nová koncepce
			bool pokracovat=true;
			TObjekt *O=F->pom;//musí být použit pom, pom_temp neuchovává ukazatel na předchozí objekt
			celkem=Element->geo.delka;//defaultně bude délka před Elementem
			while(O!=NULL && O->n!=0)
			{
				TElement *E=O->elementy->predchozi;//procházení od zadu
				if(F->pom_temp->n==O->n)E=Element->predchozi;//pokud jsem v pom_temp = začátek, začánám od předchozího elementu Elementu
				while(E!=NULL && E->n!=0)
				{
					if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt){pokracovat=false;break;}//pokud je předchozi S&G prěruš cyklus
					celkem+=E->geo.delka;//pokud jdes dál přičti vzdálenost
					E=E->predchozi;
				}
				E=NULL;delete E;
				if(pokracovat)O=O->predchozi;//ošetření proti přechodu na havičku
				else break;
			}
			O=NULL;delete O;
			return celkem;
	}
	else//////Původní funkce
	{
  	TPointD E=F->d.Rxy(Element);
  	if(Element->n==1)//pro první element od hrany kabiny
  	{                ///ještě vylepšít, provizorně jen pro vodorovnou levopravou kabinu
			return m.delka(F->pom_temp->elementy->dalsi->geo.X1,F->pom_temp->elementy->dalsi->geo.Y1,E.x,E.y);//(bude nutno ještě vylepšit s příchodem oblouků)!!!
		}
		else//mezi elementy
		{
			TPointD Ep=F->d.Rxy(Element->predchozi);
			return m.delka(E.x,E.y,Ep.x,Ep.y); //(bude nutno vylepšit s příchodem oblouků)!!!
		}
	}
}
////---------------------------------------------------------------------------
void Cvektory::reserve_time(TElement *Element,bool highlight_bunek,bool refresh_mGrid)
{
	if(Element!=NULL && vrat_druh_elementu(Element)==0)//ošetření proti prázdnému ukazateli a proti KK elementu
	{
  	//deklarace
  	double cas=0;
  	bool pokracovat=true,error=false;
		unsigned int i=0;//počet průchodu skrze objekt Elementu
		TObjekt *O=vrat_objekt(Element->objekt_n);
		if(Element->pohon!=NULL)cas+=Element->geo.delka/Element->pohon->aRD;//pokud má element pohon výpočet času přejezdu jeho úseku
  	else error=true;//nemá pohon = error
  	//průchod od Element k předchozím
  	while(O!=NULL)
  	{
  		if(O->n==0)O=O->predchozi;
  		TElement *E=O->elementy->predchozi;//procházení od zadu
  		if(F->pom_temp!=NULL && O->n==F->pom_temp->n)E=F->pom_temp->elementy->predchozi;//pokud jsem v editovaném objektu
  		if(Element->objekt_n==O->n && i==0){E=Element->predchozi;i++;}//pokud jsem v pom_temp = začátek, začánám od předchozího elementu Elementu, platí pouze při prvním průchodu
  		while(E!=NULL && E->n!=0)
  		{
        if(E->eID==200)cas+=E->WT;//wt na předávacím místě
				if((E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt) || (E->n==Element->n && E->objekt_n==Element->objekt_n)){pokracovat=false;break;}//pokud je předchozi S&G prěruš cyklus
  			if(E->pohon!=NULL)cas+=E->geo.delka/E->pohon->aRD;//pokud existuje úsek a má pohon
  			else error=true;//jinak error
  			E=E->predchozi;
  		}
  		E=NULL;delete E;
  		if(pokracovat)O=O->predchozi;//ošetření proti přechodu na havičku
  		else break;
  	}
		O=NULL;delete O;

		//výpočet RT a zapsání do dat elemetnu
		double RT=0,WT=Element->WT;
		if(Element->eID==0 && Element->akt_pocet_voziku>1 && cas+Element->WT<PP.TT)WT*=Element->akt_pocet_voziku;
		RT=m.RT(Element->PT1+Element->PT2+Element->PTotoc,cas,WT,Element->akt_pocet_voziku);
		Element->RT.x=RT;//ryzí RT
		if(Element->eID==0 && Element->akt_pocet_voziku>1 || Element->eID==6)RT=fmod(RT,PP.TT);
		Element->RT.y=RT;//přepočítané RT, nebo totožné s ryzím

		//vypsání RT do tabulky elementu
		if(F->pom_temp!=NULL && Element->objekt_n==F->pom_temp->n)
		{
			switch(Element->eID)
	  	{
	  		case 0://stop stanice
				{
					//při posunu stopstanice může dojít ke změně poštu aktuálních vozíku, nový přepočet, pokud k této změně došlo
					if(highlight_bunek)
	  			{
	  				Element->mGrid->Cells[1][2].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
	  				Element->max_pocet_voziku=F->max_voziku(Element);
	  				Element->mGrid->Cells[1][5].Text=Element->max_pocet_voziku=Element->max_pocet_voziku;
	  				if(Element->max_pocet_voziku<Element->akt_pocet_voziku)//pokud při posunu akt. počet vozíků přesáhne maximální
	  				{
	  					Element->akt_pocet_voziku=Element->max_pocet_voziku;
	  					Element->WTstop=F->m.V2WT(Element->akt_pocet_voziku,PP.TT);//uložení do paměti + výpočet
	  					Element->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(Element->WTstop),3);//OUTPUT
	  					//nové RT, protože se změnilo WTstop
	  					WT=Element->WT;
	  					if(Element->eID==0 && Element->akt_pocet_voziku>1 && cas+Element->WT<PP.TT)WT*=Element->akt_pocet_voziku;
							RT=m.RT(Element->PT1+Element->PT2+Element->PTotoc,cas,WT,Element->akt_pocet_voziku);
							Element->RT.x=RT;//ryzí
	  					if(Element->akt_pocet_voziku==1)RT==fmod(RT,PP.TT);
							Element->RT.y=RT;//přepočítané RT, nebo totožné s ryzím
	  				}
	  			}
	  			//vypsání OK pokud je RT kladné a zároveň má stopka více akt_vozíku
					if(Element->RT.y>0 && Element->akt_pocet_voziku>1){Element->mGrid->Cells[1][2].Text="OK";Element->mGrid->Cells[1][2].Hint=Element->RT.y;Element->mGrid->Cells[1][2].ShowHint=true;}
					else {Element->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(Element->RT.y),3);Element->mGrid->Cells[1][2].Hint="";Element->mGrid->Cells[1][2].ShowHint=false;}
	  		}break;
	  		case 2:case 8:case 12:case 16:case 102:case 106://roboti se stop stanicí
    		{
					Element->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(Element->RT.y),3);
	  			if(highlight_bunek)Element->mGrid->Cells[1][2].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
	  		}break;
    		case 4:case 10:case 14:case 18:case 104:case 108://roboti s aktivní otočí
	  		{
					Element->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(Element->RT.y),3);
	  			if(highlight_bunek)Element->mGrid->Cells[1][5].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
	  		}break;
	  		case 6://aktivní otoč
				{
					Element->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(Element->RT.y),3);
					if(highlight_bunek)Element->mGrid->Cells[1][3].Highlight=true;//slouži pro higlightování buňky s RT při posunu elementu
	  		}break;
			}
			//pokud některý z geometrických úseků neměl přiřazený pohon RT nebude správné, vypsat error
			if(error)Element->mGrid->ShowNote(F->ls->Strings[421],F->d.clError,14);//"RT není relevantní, některý z objektů nemá pohon!"
			if(F->pom_temp->zobrazit_mGrid && refresh_mGrid)Element->mGrid->Refresh();
		}
    //uložení erroru do dat, + 1 000 000 nebo - 1 000 000
		if(error)
		{
			if(Element->RT.x>0)Element->RT.x+=1000000;else Element->RT.x-=1000000;
			if(Element->RT.y>0)Element->RT.y+=1000000;else Element->RT.y-=1000000;
    }
	}
}
//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje
double Cvektory::vrat_rotaci_jigu_po_predchazejicim_elementu(TObjekt *Objekt,TElement *Element)
{
	bool nalezeno=false;
	double akt_rotoce_jigu=0;
	//provizorní (průchod po objektech) do doby než budou spuštěny zakázky resp. výhybky
	Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		TElement *E=O->elementy->dalsi; if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->dalsi;//v případě editace načte elementy editovaného objektu
		while(E!=NULL)
		{                                       //toto z důvodu toho že zatím E->n není unikátní (není v jednom spojáku), bude se měnit s DM
			if(Objekt!=NULL && Element->n==E->n && Objekt->n==O->n)//pozor nelze porovnávat jen ukazatele, může docházet k porování nepřímých kopii (viz pom_temp)
			{
				nalezeno=true;
				break;//akcelerátor, skončí cyklus
			}
			else
			{
				if(E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180)akt_rotoce_jigu+=E->rotace_jig;//stále předcházející elementy, ty mě pro návrátovou hodnotu zajímají, rotace aktuálního elementu se nezohledňuje
			}
			E=E->dalsi;
		}
		E=NULL;delete E;
		if(nalezeno)break;//akcelerátor, skončí i hlavní cyklus
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;
	//F->Memo("Pro "+Element->name+" o rotaci: "+Element->rotace_jig+" o celkové"+m.a360(akt_rotoce_jigu));//testovací výpis, časem možno odstranit
	return m.a360(akt_rotoce_jigu);
}
////---------------------------------------------------------------------------
//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje
double Cvektory::vrat_rotaci_jigu_po_predchazejicim_elementu(TElement *Element)
{
	return vrat_rotaci_jigu_po_predchazejicim_elementu(vrat_objekt(Element->objekt_n),Element);
}
////---------------------------------------------------------------------------
//metoda vrátí ukazatel na poslední rotační element na lince
Cvektory::TElement *Cvektory::vrat_posledni_rotacni_element()
{ //NUTNÝ UPDATE PŘI PŘECHODU NA ZÁKÁZKY RESP. VÝHYBKY!!!
	TElement *RET=NULL;
	Cvektory::TObjekt *O=OBJEKTY->predchozi;//přeskočí na poslední objekt
	while(O->n>0)
	{
		TElement *E=O->elementy->predchozi; if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->predchozi;//v případě editace načt posledn9 elementy editovaného objektu
		while(E->n>0)
		{
			if(E->rotace_jig!=0 && -180<=E->rotace_jig && E->rotace_jig<=180){RET=E;break;}
			E=E->predchozi;
		}
		E=NULL;delete E;
		if(RET!=NULL)break;
		O=O->predchozi;
	}
	O=NULL;delete O;
	return RET;
}
////---------------------------------------------------------------------------
//obsah všech comboboxu všech stopek nejdříve smaže a následně naplní combobox stopky ostatními elementy, které mohou být s danou stopkou spárované, nevypisuje danou stopku, vybere v combu stop-element spárovaný či předchozí, buď navržený nebo uživatelsky vybraný
void Cvektory::napln_comba_stopek()
{
	TElement *E=F->pom_temp->elementy;//nepřeskakovat hlavičku
	while(E!=NULL)//a jejich elementy
	{
		if(E->eID==0 && E->n!=0)napln_combo_stopky(E);
		E=E->dalsi;
	}
	E=NULL;delete E;
}
////---------------------------------------------------------------------------
//nejdříve smaže obsah comboboxu a následně naplní combobox stopky ostatními elementy, které mohou být s danou stopkou spárované, nevypisuje danou stopku, vybere v combu stop-element spárovaný či předchozí, buď navržený nebo uživatelsky vybraný
void Cvektory::napln_combo_stopky(TElement *Stopka)
{
	if(Stopka->eID==0)//záložní ošetření, aby se opravdu jednalo o stopku
	{
		bool smazat_combo=true;
		TscGPComboBox *C=Stopka->mGrid->getCombo(1,1);//ukazatel na combobox dané stopky
		if(C!=NULL)
		{
			C->Clear();//nejdříve combo vymaže
			TscGPListBoxItem *t=NULL;t=C->Items->Add(/*tady nelze parametr*/);t->Caption="nedefinován";//předvyplní, pro případ, že nebude existovat patřičný element
			Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
			while (O!=NULL)//prochází všechny objekty
			{
				bool hlavicka_vytvorena=false;
				TElement *E=O->elementy;//nepřeskakovat hlavičku
				if(F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
				while(E!=NULL)//a jejich elementy
				{
					////pokud je aktuální element stopka či robot se stopkou a zároveň nejedná se o danou stopku předávanou parametreme metody a nejedná se o hlavičku, naplní se do comba
					if((E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt) && E!=Stopka && E->n!=0)
					{
						if(smazat_combo){C->Clear();smazat_combo=false;/*t=C->Items->Add(/*tady nelze parametr*//*);t->Caption="nepřiřazen";v případě odkomentování zvýšit index u přidělování Itemindex u bez spárované situace*/}//nejdříve combo vymaže od popisku nedefinovan
						if(!hlavicka_vytvorena)//pokud ještě nebyla vytvoří ji formou názvu
						{
							hlavicka_vytvorena=true;
							t=C->Items->Add(/*tady nelze parametr*/);
							if(F->pom_temp->n==O->n)t->Caption=F->pom_temp->name.UpperCase();else t->Caption=O->name.UpperCase();
							C->Items->operator[](C->Items->Count-1)->Header=true;
						}
						t=C->Items->Add(/*tady nelze parametr*/);
						t->Caption=E->name;
					}
					if(Stopka->sparovany!=NULL)//lépe jako samostatný if
					{
						if(Stopka->sparovany->n==E->n && vrat_objekt(Stopka->sparovany,true)->n==O->n)C->ItemIndex=C->Items->Count-1;//pokud již existuje spárovaný, vrátí jeho itemindex   //samotné označí předchozí stop-element
					}
					E=E->dalsi;
				}
				E=NULL;delete E;
				O=O->dalsi;//posun na další prvek
			}
			O=NULL;delete O;
		}
		C=NULL;delete C;
	}
}
////---------------------------------------------------------------------------
//uloží dané stopce ukazatel na sparovaný stop element, který byl vybraný v Combu dané stopky, ošetřuje zda se jedná o stopku
void Cvektory::uloz_sparovany_element(TElement *Stopka)//algoritmus funguje na principu simualce tvorby comboboxu (viz výše)
{
	if(Stopka->eID==0)
	{
		TscGPComboBox *C=Stopka->mGrid->getCombo(1,1);
		if(C!=NULL)
		{
			Stopka->sparovany=NULL;
			short selEl=-1;//aktuální zpracovávána položka virtuálního - comboboxu
			Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
			while (O!=NULL)//prochází všechny objekty
			{
				bool hlavicka_vytvorena=false;
				TElement *E=O->elementy;//nepřeskakovat hlavičku
				if(F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
				while(E!=NULL)//a jejich elementy
				{
					////pokud je aktuální element stopka či robot se stopkou a zároveň nejedná se o danou stopku předávanou parametreme metody a nejedná se o hlavičku, naplní se do comba
					if((E->eID==0 || E->eID==4 || E->eID==6) && E!=Stopka && E->n!=0)
					{
						if(!hlavicka_vytvorena)//pokud ještě nebyla, vytvoří ji formou názvu
						{
							hlavicka_vytvorena=true;
							selEl++;
						}
						selEl++;
						if(selEl==C->ItemIndex)
						{
							Stopka->sparovany=E;
							break;//ukončí předčasně další vyhledávání
						}
					}
					E=E->dalsi;
				}
				E=NULL;delete E;
				if(Stopka->sparovany!=NULL)break;//pokud byl nalezen, ukončí předčasně vyhledávání
				O=O->dalsi;//posun na další prvek
			}
			O=NULL;delete O;
		}
		C=NULL;delete C;
	}
}
////---------------------------------------------------------------------------
//dané stopce najde předchozí stop-element na lince, je možno, že nebude reflektovat danou zakázku
//nově se podívá na předchozí stop-element a přiřadí mu ukazatel na Element
void Cvektory::vrat_predchozi_stop_element(TElement *Element,TObjekt *Objekt)
{
//	TElement *RET=NULL;
//	if(Stopka->eID==0)
//	{
//		bool konec=false;
//		Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
//		while (O!=NULL)//prochází všechny objekty
//		{
//			TElement *E=O->elementy;//nepřeskakovat hlavičku
//			if(F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
//			while(E!=NULL)//a jejich elementy
//			{
//				////pokud je aktuální element stopka či robot se stopkou a nejedná se o hlavičku
//				if(E->eID!=MaxInt && E->eID%2==0 && E->n!=0)
//				{                  //toto je otazník
//					 if(E==Stopka && RET!=NULL){konec=true;break;}//pokud se došlu až k aktuální stopce a již byla stop element nalezen,vyhledávání se ukončí a vrací se to, co je uložené v E resp. RET
//					 else RET=E;//pokud ne, uloží
//				}
//				E=E->dalsi;
//			}
//			E=NULL;delete E;
//			if(konec)break;
//			O=O->dalsi;//posun na další prvek
//		}
//		O=NULL;delete O;
//	}
//	return RET;

	//////nová koncepce
	if(Element->eID%2==0 && Element->eID!=100 && Element->eID!=200 && Element->eID!=MaxInt)
	{
		bool pokracovat=true;
		TElement *E=NULL;
		TObjekt *O=Objekt;
		if(F->pom!=NULL && F->pom->n==O->n)O=F->pom;//pokud existuje pom a rovná se začátečnímu objektu, musí být nahrazen
		while(O!=NULL && O->n!=0)
		{
			E=O->elementy->predchozi;//procházení od zadu
			if(Objekt->n==O->n)E=Element->predchozi;//pokud jsem v prvním objektu = začátek, začánám od předchozího elementu Elementu
			while(E!=NULL && E->n!=0)
			{
				if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt){pokracovat=false;break;}//nalezen předchozí S&G
				E=E->predchozi;
			}
			if(pokracovat)O=O->predchozi;//ošetření proti přechodu na havičku
			else break;
		}
		if(E->n!=0)
		{
		 	Element->sparovany=E->sparovany;//nahrazení, vložení za E, takže Element přebere jeho spárovaný
			E->sparovany=Element;
			if(E->eID==0)
			{
				try
				{E->mGrid->Cells[1][1].Text=E->sparovany->name;E->mGrid->Refresh();}//pokud se element (=stopka) nachází ve stejném objektu a tento objekt je právě editovaný, přepiš mu sparovaný element do mgridu
				catch(...){/*MessageBeep(0);*/}
			}
			if(Element->eID==0)
			{
				try
				{Element->mGrid->Cells[1][1].Text=Element->sparovany->name;Element->mGrid->Refresh();}//pokud se element (=stopka) nachází ve stejném objektu a tento objekt je právě editovaný, přepiš mu sparovaný element do mgridu
				catch(...){/*MessageBeep(0);*/}
			}
		}
		//////první poslední stop element
		if(OBJEKTY->predchozi->n>=2||true)
		{
			pokracovat=true;
			O=OBJEKTY->dalsi;
			while(O!=NULL)
			{
				E=O->elementy->dalsi;
				if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->dalsi;
				while(E!=NULL)
				{
					if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt){pokracovat=false;break;}
					E=E->dalsi;
				}
				if(pokracovat)O=O->dalsi;
				else break;
			}
			if(E!=NULL)
			{
				Cvektory::TElement *prvni_stopka=E;
				pokracovat=true;
				O=OBJEKTY->predchozi;
				while(O!=NULL && O->n>0)
	    	{
	    		E=O->elementy->predchozi;//procházení od zadu
					if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->predchozi;
					while(E!=NULL && E->n>0)
					{
						if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt){pokracovat=false;break;}//nalezen předchozí S&G
						E=E->predchozi;
					}
					if(pokracovat)O=O->predchozi;//ošetření proti přechodu na havičku
	    		else break;
				}           //E = poslední stopka, prvni_stopka = první stopka
				//if(E!=NULL)F->Memo("první : "+prvni_stopka->name+"; posledni: "+E->name);
				if(E!=NULL && (E->objekt_n!=prvni_stopka->objekt_n || E->objekt_n==prvni_stopka->objekt_n && E->n!=prvni_stopka->n))//pokud existuje poslední stop elemet a nerovná se mrvnímu
				{
					bool prvni=false,posledni=false;
					if(prvni_stopka->sparovany==NULL){prvni_stopka->sparovany=E;prvni=true;}
					if(E->sparovany==NULL){E->sparovany=prvni_stopka;posledni=true;}
					try //musí být řešeno takto, nelze rozeznat jestli existuje mgird
					{
						if(prvni && prvni_stopka->eID==0){prvni_stopka->mGrid->Cells[1][1].Text=E->name;prvni_stopka->mGrid->Refresh();}
						if(posledni && E->eID==0){E->mGrid->Cells[1][1].Text=prvni_stopka->name;E->mGrid->Refresh();}
					}
					catch(...)
					{/*MessageBeep(0);*/}
				}
				prvni_stopka=NULL;delete prvni_stopka;
      }
		}
		O=NULL;delete O;
		E=NULL;delete E;
	}
}
////---------------------------------------------------------------------------
//projde všechny stop-elementy a aktualizuje jim ukazatele na spárované elementy
void Cvektory::aktualizuj_sparovane_ukazatele()
{
	F->aktualizace_RT();//aktualizace RT v tabulkách
	Cvektory::TObjekt *O=OBJEKTY->predchozi;
	//smazání původních ukazatelů
	while(O!=NULL && O->n!=0)
	{
		TElement *E=O->elementy->predchozi;//procházení od zadu
		if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->predchozi;
		while(E!=NULL && E->n!=0)
		{
			if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt)
			{
				E->sparovany=NULL;
				if(E->eID==0 && E->Xt!=-100 && F->pom_temp!=NULL && O->n==F->pom->n)E->mGrid->Cells[1][1].Text="N/A";
			}
			E=E->predchozi;
		}
		E=NULL;delete E;
		O=O->predchozi;
	}
	O=OBJEKTY->predchozi;
	//nové přiřazení
	while(O!=NULL && O->n!=0)
	{
		TElement *E=O->elementy->predchozi;//procházení od zadu
		if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy->predchozi;
		while(E!=NULL && E->n!=0)
		{
			if(E->eID%2==0 && E->eID!=100 && E->eID!=200 && E->eID!=MaxInt)vrat_predchozi_stop_element(E,O);
			E=E->predchozi;
		}
		E=NULL;delete E;
		O=O->predchozi;
	}
	O=NULL;delete O;
}
////---------------------------------------------------------------------------
//vrátí předchozí element k Element, který byl do metody poslán jako parametr, přeskočí geometrické zarážky
Cvektory::TElement *Cvektory::vrat_dalsi_element(TElement *Element)
{
	TElement *ret=NULL;
	unsigned long O_n=Element->objekt_n;
	Element=Element->dalsi;
	while(Element!=NULL && Element->objekt_n==O_n)
	{
		if(Element->eID%2==0 && Element->eID!=100 && Element->eID!=200 && Element->eID!=MaxInt){ret=Element;break;}
		Element=Element->dalsi;
	}
	Element=NULL;delete Element;
	return ret;
}
//vrátí poslední element v objektu
Cvektory::TElement *Cvektory::vrat_posledni_element_objektu(TObjekt *Objekt)
{
	if(Objekt!=NULL && OBJEKTY->dalsi!=NULL)
	{
		TElement *ret=NULL,*E=Objekt->elementy->dalsi;
		while(E!=NULL && Objekt->n==E->objekt_n)
		{
			ret=E;
			E=E->dalsi;
		}
		E=NULL;delete E;
		return ret;
	}
	else return NULL;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//smaže element ze seznamu
void Cvektory::smaz_element(TObjekt *Objekt, unsigned int n)
{
	smaz_element(vrat_element(Objekt,n));
}
////---------------------------------------------------------------------------
//smaže element ze seznamu
void Cvektory::smaz_element(TElement *Element)
{
  //nejdříve smazání tabulky Elelementu
  long ID=Element->mGrid->ID;
	Element->mGrid->Delete();

	//kontrola při pokusu o smazání předávacího místa
	bool povolit_smazani=true;
	if(Element->eID==200)
	{
		//unsigned int pocet_PM=0;
		TElement *E=Element->dalsi,*dalsi_PM=NULL,*predchozi_PM=NULL;//procházení daného objektu
		while(E!=NULL && E->objekt_n==Element->objekt_n)
		{
			if(E->eID==200){dalsi_PM=E;break;}
			E=E->dalsi;
		}
		if(E==NULL)
		{
			E=Element->predchozi;
			while(E->n!=0 && E->objekt_n==Element->objekt_n)
	  	{
				if(E->eID==200){predchozi_PM=E;break;}
				E=E->predchozi;
			}
		}
		TObjekt *O=vrat_objekt(Element->objekt_n);//pomocný ukazatel na objekt
		if(dalsi_PM==NULL && predchozi_PM==NULL && O->dalsi!=NULL)povolit_smazani=false;//jedno PM
		else//2 nebo více předávacích míst
		{
			if(dalsi_PM!=NULL && predchozi_PM!=NULL)//mezané PM má předchozí i další PM
			{
				E=predchozi_PM->dalsi;
				while(E->n<=dalsi_PM->n)
				{
					E->pohon=NULL;//nedefinovaný pohon mezi PM
					E=E->dalsi;
				}
			}
			if(dalsi_PM!=NULL && predchozi_PM==NULL)//existuje pouze dalsi PM
			{
				//získání pohonu
				TPohon *p=dalsi_PM->predchozi->pohon;
				E=O->elementy->dalsi;
				if(F->pom_temp!=NULL)E=F->pom_temp->elementy->dalsi;
				//přiřazení pohonu
				while(E!=NULL && E->n!=dalsi_PM->n)
				{
					E->pohon=p;
					E=E->dalsi;
				}
				p=NULL;delete p;
      }
			if(dalsi_PM==NULL && predchozi_PM!=NULL)//existuje pouze predchozi
			{
				//získání pohonu
				TPohon *p=NULL;
				if(Element->dalsi!=NULL)p=Element->dalsi->pohon;
				if(Element->dalsi==NULL && O->dalsi!=NULL && O->dalsi->elementy->dalsi->pohon!=NULL)p=O->dalsi->elementy->dalsi->pohon;
				predchozi_PM->dalsi;
				//přiřazení pohonu
				while(E!=NULL)
				{
					E->pohon=p;
					E=E->dalsi;
				}
				p=NULL;delete p;
			}
			if(dalsi_PM==NULL && predchozi_PM==NULL)
			{
      	//získání pohonu
				TPohon *p=Element->pohon;
        E=Element->dalsi;
				//přiřazení pohonu
				while(E!=NULL)
				{
					E->pohon=p;
					E=E->dalsi;
				}
				p=NULL;delete p;
      }
		}
		O=NULL;E=NULL;dalsi_PM=NULL;predchozi_PM=NULL;delete O;delete E;delete dalsi_PM;delete predchozi_PM;
	}
	//hláška uživateli
	if(!povolit_smazani && F->pom_temp!=NULL)F->TIP=F->ls->Strings[315];//"Nelze odstranit předávací místo"
	////////////smaz_element
	if(povolit_smazani && (Element->eID==MaxInt || Element->geo.typ==0 && Element->dalsi!=NULL && Element->dalsi->geo.typ==0))//pokud se jedná o zarážku, odstranit ze spojáku nebo pokud se jedná o element mezi 2mi liniemi
	{
		//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
		if(Element->dalsi!=NULL)//ošetření proti poslednímu prvku
		{
			Element->predchozi->dalsi=Element->dalsi;
			Element->dalsi->predchozi=Element->predchozi;
			if(Element->dalsi->geo.typ==0 && F->Akce!=F->GEOMETRIE)//upravovat pouze v případě, že se jedná o linii, a kokud needituju geometrii!!! (u geo. se upravuje geometrie ostatních elemntů zvlášť v Unit1)
			{
				if(Element->n!=1)vloz_G_element(Element->dalsi,0,Element->predchozi->geo.X4,Element->predchozi->geo.Y4,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
				else vloz_G_element(Element->dalsi,0,Element->geo.X1,Element->geo.Y1,0,0,0,0,Element->dalsi->geo.X4,Element->dalsi->geo.Y4,Element->dalsi->geo.orientace);
			}
		}
  	else//poslední prvek                                                                           ¨
		{
  		if(Element->n==1)//pokud je mazaný prvek hned za hlavičkou
  		{
				Element->predchozi->predchozi=Element->predchozi; //popř hlavička bude ukazovat sama na sebe
				Element->predchozi->dalsi=NULL;
			}
  		else
  		{
  			Element->predchozi->dalsi=NULL;
  			TElement *Eh=Element;//Element hlavička
  			while(Eh->n!=0)//postup k hlavičce, jinak by se musel parametrem metody předávat ukazatal na Objekt, který element vlastní
  			{
  				Eh=Eh->predchozi;
  			}
  			Eh->predchozi=Element->predchozi;//zapis do hlavičky poslední prvek seznamu
  			Eh=NULL;delete Eh;
  		}
  	}

  	//přeindexování (n/ID) původního následujícího objektu a prvků všech za nim následujících
		TElement *E=Element->dalsi;
  	while(E!=NULL)
  	{
  		E->n--;
  		E=E->dalsi;//posun na další
  	}

		//odstranění z pěměti
		Element=NULL;delete Element;
	}
	else if(povolit_smazani)//pokud se jedná o element, změnit typ na zarážku, zachování geometrie
	{
		//obnovení původních hodnot
		Element->LO1=1.5;
		Element->OTOC_delka=0;
		Element->LO2=0;
		Element->LO_pozice=0;
		Element->PT1=0;
		Element->PTotoc=0;
		Element->PT2=0;
		Element->WT=0;//čekání na palec
		Element->WTstop=0;//čekání na stopce
		Element->RT.x=0;//reserve time
		Element->RT.y=0;//reserve time

		//názvy výhybek prozatím neřešeny
		unsigned int nTyp=vrat_poradi_elementu_do(F->pom_temp,Element)+1;//pokud se jedná o roboty
		Element->name="Zarážka "+AnsiString(nTyp);
		Element->short_name=Element->name.SubString(1,3)+AnsiString(nTyp);

		//změna elemetnu na zarážku
		Element->eID=MaxInt;

		//znovuvytvoření mGridu elementu
		Element->mGrid=new TmGrid(F);
		Element->mGrid->Tag=6;//ID formu
		Element->mGrid->ID=ID;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
		F->design_element(Element,false);//nutné!
	}
}
////---------------------------------------------------------------------------
//vymaže všechny elementy daného objektu včetně hlavičky a vrátí počet smazaných elementů (počítáno bez hlavičky), automaticky, pokud posledním parametreme není nastaveno jinak, smaže přidružený mGrid
long Cvektory::vymaz_elementy(TObjekt *Objekt,bool mGridSmazat)
{
	long pocet_smazanych_objektu=0;
	while (Objekt->elementy!=NULL)
	{
		//vymaz_gObjekty(Objekt->elementy->predchozi);
		if(mGridSmazat)Objekt->elementy->predchozi->mGrid->Delete();
		Objekt->elementy->predchozi->mGrid==NULL;
		Objekt->elementy->predchozi=NULL;
		delete Objekt->elementy->predchozi;
		Objekt->elementy=Objekt->elementy->dalsi;
		pocet_smazanych_objektu++;
	};
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
	novy->Rz=0;
	novy->Rx=0;
	novy->retez=NULL;

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

	novy=pohon;//novy bude ukazovat tam kam prvek Objekt
	novy->n=POHONY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	POHONY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=POHONY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	POHONY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
////---------------------------------------------------------------------------
//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double aRD,double R,double Rz,double Rx)
{
	F->log(__func__);//logování
	TPohon *novy=new TPohon;
	novy->name=name;
	novy->rychlost_od=rychlost_od;
	novy->rychlost_do=rychlost_do;
	novy->aRD=aRD;
	novy->roztec=R;
	novy->Rz=Rz;
	novy->Rx=Rx;
	novy->retez=NULL;
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
			*Objekt->pohon=*Pohon;//překopírování hodnot pohonů bez spojového propojení s originálem, ale i přes další a předchozí není zoohledněno propojení se daným spojovým seznamem pohonů, daný pohon je pouze datovou součástí pom_temp, není samostatným objektem či objektem zařazeným ve spojáku pohonů
		}
	}
	else Objekt->pohon=NULL;
}
////---------------------------------------------------------------------------
//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
bool Cvektory::pohon_je_pouzivan(unsigned long n,bool po_obektech)
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
  	while (O!=NULL)
  	{
  		if(F->pom_temp==NULL || F->pom_temp!=NULL && F->pom_temp->n!=O->n)
  		{
  			TElement *E=O->elementy->dalsi;
  			while(E!=NULL)
  			{
  				if(E->eID!=0 && E->eID!=200 && E->eID!=MaxInt && E->pohon!=NULL && E->pohon->n==n){nalezen=true;break;}
  				E=E->dalsi;
  			}
  			E=NULL;delete E;
  		}
  		if(!nalezen)O=O->dalsi;
  		else break;
		}
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
//dle n pohonu vráti objekty, které pohon používají, pokud je short_name na true, vrátí kratký název objektu jinak dlouhý
AnsiString Cvektory::vypis_objekty_vyuzivajici_pohon(unsigned long n,bool short_name)
{
	TObjekt *O=OBJEKTY->dalsi;
	AnsiString nalezen="";
	while (O!=NULL)
	{
		if(O==F->pom && F->pom!=NULL)//pokud je voláno z editovaného PO
		{
			 //a pokud se jedná o stejný objekt, jako právě projížděný cyklemmusí být samostatně
				if(Form_parametry->scComboBox_pohon->ItemIndex>0  && Form_parametry->scComboBox_pohon->ItemIndex==(long)n)//pokud má pohon přiřazen a jedná se o stejný pohon
				{
					if(short_name)nalezen+=O->short_name+", ";
					else nalezen+=O->name+", ";
				}
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
	TObjekt *O=OBJEKTY->dalsi;
	double min=123567.0;//jen náhodně velké číslo
	while (O!=NULL)
	{                                    //mohl bych ještě odfiltrovávat, zda se nejedná o KK, ale je to víceméně zbytečné
		if(O->pohon!=NULL && O->pohon==pohon)//pokud má pohon přiřazen a jedná se o stejný pohon
		{
			if(O->delka_dopravniku/O->CT<min)min=O->delka_dopravniku/O->CT;//tak z těchto objektů najde nejmenší možné RD
		}
		O=O->dalsi;
	}
	O=NULL;delete O;
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
		CT=O->CT;
		DD=O->delka_dopravniku;
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
	smazat_retez(Pohon);
	//po změně DM chodit přímo po elementech
	if(OBJEKTY!=NULL)
	{
		TObjekt *O=OBJEKTY->dalsi;
		while(O!=NULL)
		{
			if(O->elementy!=NULL)
			{
				TElement *E=O->elementy->dalsi;//přeskočí hlavičku
				if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezmou objekty z pom_temp, kde jsou aktuální hodnoty
				while(E!=NULL)//a jejich elementy
				{
					if(E->pohon!=NULL && Pohon!=NULL && E->pohon->n==Pohon->n)//řetěz tvořen pouze z geometrie všech elementů, co spadají pod daný pohon
					{
						TRetez *R=new TRetez;//segment řetězu
						vloz_segment_retezu(R,Pohon);
						////pokud se nejedná o předávací místo, ale o většinu geometrických prvků
						R->eID=E->eID;
						R->geo=E->geo;
						////výpočet souřadnic předávacího místa - ubrání z existujícho segmentu, přepsání výše uvedených souřadnic
						//vstupní parametry dimenzující geometrii předávacího místa
						float  D=1;//delká vyosování - 1 metr na každou stranu
						float RetezWidth=1;//šířka řetezu kvůli vyosení v předávacím místě
						TPointD V=m.rotace(RetezWidth*F->m2px,180-E->geo.orientace+90,0);//vyosení, mezera mezi pohony v předávacím místě
						TPointD Z=m.rotace(D,180-E->geo.orientace,0);//začátek vyosování
						////situace předávacím místem
						if(E->eID==200)
						{
							//segment řetězu před předávacím místem konkrétně před vyosení
							R->eID=E->eID-1;//eID 199 poslední segment před vysováním tj. před předávacím místem
							R->geo.X1=E->geo.X1; 							 			 R->geo.Y1=E->geo.Y1;
							R->geo.X2=(E->geo.X4-Z.x+E->geo.X1)/2.0; R->geo.Y2=(E->geo.Y4-Z.y+E->geo.Y1)/2.0;
							R->geo.X3=R->geo.X2; 										 R->geo.Y3=R->geo.Y2;
							R->geo.X4=E->geo.X4-Z.x; 								 R->geo.Y4=E->geo.Y4-Z.y;

							//segment řetězu předávací místo konkrétně od vyosení
							TRetez *R1=new TRetez;
							vloz_segment_retezu(R1,Pohon);
							R1->eID=E->eID;//eID 200
							R1->geo.X1=R->geo.X4; 							R1->geo.Y1=R->geo.Y4;
							R1->geo.X2=E->geo.X4-Z.x/2.0;			R1->geo.Y2=E->geo.Y4-Z.y/2.0;
							R1->geo.X3=E->geo.X4-Z.x/2.0+V.x;	R1->geo.Y3=E->geo.Y4-Z.y/2.0+V.y;
							R1->geo.X4=E->geo.X4+V.x;					R1->geo.Y4=E->geo.Y4+V.y;
						}
						///situace za předávacím místem
						TElement *Ep=E->predchozi;
						//if(Ep->n==0 && O->predchozi!=NULL && O->predchozi->elementy->predchozi!=NULL && O->predchozi->elementy->predchozi->eID==200)Ep=O->predchozi->elementy->predchozi;//nahraje předchozí z předchozího objektu toto bude možné po přechodu na nový DM odstranit
						if(Ep!=NULL && Ep->eID==200)
						{
							//segment řetězu za předávacím místem konkrétně v největším místě vyosení za předávacím místem
							R->eID=E->eID+2;//eID 202 segment řetězu za předávacím místem konkrétně v největším místě vyosení za předávacím místem
							R->geo.X1=E->geo.X1+Z.x; 							 	 R->geo.Y1=E->geo.Y1+Z.y;
							R->geo.X2=(R->geo.X1+E->geo.X4)/2.0; 		 R->geo.Y2=(R->geo.Y1+E->geo.Y4)/2.0;
							R->geo.X3=R->geo.X2; 										 R->geo.Y3=R->geo.Y2;
							R->geo.X4=E->geo.X4; 								 		 R->geo.Y4=E->geo.Y4;

							//segment řetězu předávací místo konkrétně od vyosení
							TRetez *R1=new TRetez;
							vloz_segment_retezu(R1,Pohon);
							R1->eID=E->eID+1;//eID 201
							R1->geo.X1=E->geo.X1-V.x;					R1->geo.Y1=E->geo.Y1-V.y;
							R1->geo.X2=E->geo.X1+Z.x/2.0-V.x;	R1->geo.Y2=E->geo.Y1+Z.y/2.0-V.y;
							R1->geo.X3=E->geo.X1+Z.x/2.0;			R1->geo.Y3=E->geo.Y1+Z.y/2.0;
							R1->geo.X4=R->geo.X1; 						R1->geo.Y4=R->geo.Y1;
						}
					}
					E=E->dalsi;
				}
				E=NULL;delete E;
				O=O->dalsi;
			}
		}
		O=NULL;delete O;
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
//všem objektům s n pohonem zruší přiřazení k tomuto pohonu a nahradí hodnotu ukazatele na přiřazený pohon za NULL
void Cvektory::zrusit_prirazeni_pohunu_k_objektum(unsigned long n)
{
	//průchod všemi objekty, testuje je daný pohon objektu přiřazen a pokud ano, tak mu nastaví přiřazený pohon na NULL
	TObjekt *O=OBJEKTY->dalsi;
	while(O!=NULL)
	{
		if(O->pohon!=NULL && O->pohon->n==n)//pokud má pohon přiřazen a jedná se o stejný pohon
		{
			O->pohon=NULL;//pohon již nepřiřazen
		}
		O=O->dalsi;
	}
	O=NULL;delete O;
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
		POHONY->predchozi=NULL;
		delete POHONY->predchozi;
		POHONY=POHONY->dalsi;
	};
	return pocet_smazanych_pohonu;
};
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

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;//další prvek zatím není ukazuje na nul
	ZAKAZKY=nova;//nahraje ukazatel na hlavičku spojového seznamu na ukazatel CESTY
}
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
// vrátí ukazatel (resp. data) na temp zakázku, nejčastěji editovanou
Cvektory::TZakazka *Cvektory::vrat_temp_zakazku(unsigned long n_zakazky)
{
	 TZakazka *ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	 while (ukaz!=NULL)
	 {
		//akce s ukazatelem
		if(ukaz->n==n_zakazky)break;
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
	if(ZAKAZKY_temp->dalsi!=NULL && n>0)
	{
			TZakazka *ukaz=ZAKAZKY_temp->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
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
			}
			else//poslední prvek
			{
				if(ukaz->n==1)//pokud je mazaný prvek hned za hlavičkou
				{
					ZAKAZKY_temp->predchozi=ZAKAZKY_temp->predchozi; //popř hlavička bude ukazovat sama na sebe
					ZAKAZKY_temp->dalsi=NULL;
				}
				else
				{
					ukaz->predchozi->dalsi=NULL;
					ZAKAZKY_temp->predchozi=ukaz->predchozi;//zapis do hlavičky poslední prvek seznamu
				}
			}

			ukaz=NULL;delete ukaz;//smaže mazaný prvek
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
			 vloz_segment_cesty(T,C->objekt,C->CT,C->Tc,C->Tv,C->RD,C->Opak);
			 C=C->dalsi;//posun na další segment cesty dané zakázky
		 }
		 C=NULL;delete C;
		 //posun na další zakázku
		 Z=Z->dalsi;
	}
	Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
void Cvektory::prvni_zakazka_dle_schematu()
{

	////ZAKAZKA
	TZakazka *Z=ZAKAZKY->dalsi;
	if(ZAKAZKY->dalsi==NULL)//pokud první zakázka neexistuje, založí ji
	{
		Z=new TZakazka;                                                                         //počet vozíků vygeneruje dle hodnoty WIP+jeden navíc kvůli přehlednosti, kdy začíná náběh
		Z->id=1;Z->typ=1;Z->name="Nová zakázka";Z->barva=clRed;Z->pomer=100;Z->TT=PP.TT;Z->pocet_voziku=WIP(1)+1;Z->serv_vozik_pocet=0;Z->opakov_servis=0;
		Z->cesta=NULL;
		Cvektory::TJig j;
		j.sirka=F->d.v.PP.sirka_jig;j.delka=F->d.v.PP.delka_jig;j.vyska=F->d.v.PP.vyska_jig;j.ks=1;//defaultní hodnoty jigu
		Z->jig=j;
		vloz_zakazku(Z);//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
	}//jinak přepíše novou cestou (zajištění aktualizace parametrů z objektu) stávající první zakázku
	////CESTA
	inicializace_cesty(Z);//vymaže předchozí cestu a zavolá hlavičku cesty nové
	//procházení všech objektů ve schématu a přiřaření do cesty
	TObjekt *O=OBJEKTY->dalsi;
	while(O!=NULL)
	{
		TCesta *S=new TCesta;
		S->objekt=O;S->CT=O->CT;S->RD=O->RD;S->Rotace=O->orientace;S->Tc=0;S->Tv=0;S->Opak=0;
		vloz_segment_cesty(Z,S);//do konkrétní zakázky vloží segmenty cesty
		O=O->dalsi;
		//S=NULL;delete S;
	}
	//delete O;Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
//smaze seznam ZAKAZKY z paměti v četně přidružených cest
long Cvektory::vymaz_seznam_ZAKAZKY()
{
	long pocet_smazanych_objektu=0;
	while (ZAKAZKY!=NULL)
	{
		vymaz_cestu_zakazky(ZAKAZKY);//mazání jednotlivých cest
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
	nova->objekt=NULL;
	nova->CT=0;
	nova->Tc=0;
	nova->Tv=0;
	nova->RD=0;
	nova->Opak=0;

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
//do konkrétní zakázky vloží segmenty cesty
void Cvektory::vloz_segment_cesty(TZakazka *zakazka,unsigned int n_vybraneho_objektu,double CT,double Tc,double Tv,double RD,unsigned int Opak)//do konkrétní cesty vloží segmenty cesty,  bude užito v metodě při stisku OK, při vkládání každého řádku stringgridu v daném for cyklu.
{
	vloz_segment_cesty(zakazka,vrat_objekt(n_vybraneho_objektu),CT,Tc,Tv,RD,Opak);
}
//přetížená funkce
void Cvektory::vloz_segment_cesty(TZakazka *zakazka,TObjekt *vybrany_objekt,double CT,double Tc,double Tv,double RD,unsigned int Opak)//do konkrétní cesty vloží segmenty cesty,  bude užito v metodě při stisku OK, při vkládání každého řádku stringgridu v daném for cyklu.
{
	TCesta *segment=new TCesta;

	segment->objekt=vybrany_objekt;
	segment->CT=CT;
	segment->Tc=Tc;
	segment->Tv=Tv;
	segment->RD=RD;
	segment->Opak=Opak;

	vloz_segment_cesty(zakazka,segment);
}
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
}//---------------------------------------------------------------------------
//ověří zda daný objekt je součástí cesty dané zakázky či nikoliv, pokud ano vrací ukazatel na daný segment
Cvektory::TCesta *Cvektory::obsahuje_segment_cesty_objekt(TObjekt *objekt,TZakazka *zakazka)
{
	 TCesta *RET=NULL;
	 TCesta *C=zakazka->cesta->dalsi;
	 while(C!=NULL)
	 {
			 if(C->objekt==objekt){RET=C;break;}
			 C=C->dalsi;
	 }
	 return RET;
}
//ověří zda daný objekt je součástí cesty nějaké zakázky či nikoliv, pokud ano vrací ukazatel
Cvektory::TZakazka *Cvektory::obsahuje_segment_cesty_objekt(TObjekt *objekt)
{
	 TZakazka *Z=ZAKAZKY->dalsi;
	 while(Z!=NULL)//prochází jednotlivé zakázky
	 {
			 if(obsahuje_segment_cesty_objekt(objekt,Z)!=NULL)
			 {return Z;}//nalezen
			 else
			 Z=Z->dalsi;
	 }
	 return NULL;
}
//---------------------------------------------------------------------------
//dle TT z parametru nastaví všem segmentům cesty od dané zakázky odpovídající CT (a line-tracking objektů i RD) dle fixní délky a kapacity, vhodné pro volání před zobrazením cest
void Cvektory::aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(TZakazka *zakazka,double TT)
{
	TCesta *C=zakazka->cesta->dalsi;
	while(C!=NULL)
	{
		 C->CT=TT*C->objekt->kapacita;
		 if(C->objekt->rezim==1)C->RD=C->objekt->delka_dopravniku/C->CT;///u kontinuálního
		 C=C->dalsi;
	}
	C=NULL;delete C;
}
//---------------------------------------------------------------------------
//dle TT zakázky nastaví všem segmentům cesty od dané zakázky odpovídající CT (a line-tracking objektů i RD) dle fixní délky a kapacity, vhodné pro volání před zobrazením cest
void Cvektory::aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(TZakazka *zakazka)
{
	aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(zakazka,zakazka->TT);
}
//---------------------------------------------------------------------------
//to samé co výše ale uskuteční plošně pro všechny zakázky, vhodné pro volání v tlačítku uložit
void Cvektory::aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky()
{
	TZakazka *Z=ZAKAZKY->dalsi;
	while(Z!=NULL)//prochází jednotlivé zakázky
	{
		aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(Z);
		Z=Z->dalsi;
	}
	Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
//dle parametrů JIG přepočítá K (u S&G zanechá 1) a z toho vyplývající změnu CT a RD (u linetracking objektů) jednolivých segmentů cesty dané zakázky
void Cvektory::aktualizace_KaCTaRD_segmentu_cesty_dleJIG(TZakazka *zakazka)
{
  //zatim nepoužíváme rozlišení jig a vozík
	/*TCesta *C=zakazka->cesta->dalsi;
	while(C!=NULL)
	{
		 if(C->objekt->rezim!=0)//u S&G neřeší
		 {
			C->objekt
			C->CT=PP.TT*C->objekt->kapacita;
			if(C->objekt->rezim==1)C->RD=C->objekt->delka_dopravniku/C->CT;///u kontinuálního
			C=C->dalsi;
		 }
	}
	C=NULL;delete C;
	*/
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
	TVozik *novy=new TVozik;
	novy->n=0;
	novy->zakazka=NULL;
	novy->typ=1;
	novy->start=0;//výchozí pozice v grafu časových os
	novy->pozice=-1.0;//akt. pozice na dopravniku či v grafu časových os

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	VOZIKY=novy;
}
////---------------------------------------------------------------------------
//vygeneruje podle zadaných zakázek seznam vozíků (včetně případných servisních), seřazeno dle zakázek
void Cvektory::generuj_VOZIKY()
{
	 if(ZAKAZKY!=NULL)//záměrně do dvou podmínek
	 if(ZAKAZKY->dalsi!=NULL && ZAKAZKY->predchozi->n>0)
	 {
			vymaz_seznam_VOZIKY();
			hlavicka_VOZIKY();
			TZakazka *zakazka=ZAKAZKY->dalsi;//ukazatel na první objekt v seznamu ZAKAZKY, přeskočí hlavičku
			while (zakazka!=NULL)//projíždí jednotlivé zakázky
			{
				long servisu_celkem=0;//počet servisů mezi reálnými vozíky,počet servisních vozíků je servisu_celkem*zakazka->serv_vozik_pocet
				if(zakazka->opakov_servis>0 && zakazka->serv_vozik_pocet>0)
				{
					servisu_celkem=floor((double)(zakazka->pocet_voziku/zakazka->opakov_servis)); //výpočet počtu servisů, počet servisních vozíků je servisu_celkem*zakazka->serv_vozik_pocet
					if(zakazka->pocet_voziku%servisu_celkem==0)servisu_celkem--; //poslední se nebude započítávat
				}
				for(unsigned long i=1;i<=zakazka->pocet_voziku+servisu_celkem;i++)//v rámci zakázky generuje zadaný počet vozíků
				{
					if(servisu_celkem>0)
					{
							if(i%(zakazka->opakov_servis+1))
							vloz_vozik(zakazka,0);//normální
							else
							{
								for(unsigned int j=1;j<=zakazka->serv_vozik_pocet;j++)//vytvoří počet servisních vozíků mezi reálnými v rámci jednoho servisu
								vloz_vozik(zakazka,1);//servisní
							}
					}
					else
					vloz_vozik(zakazka,0);//normální
				}
				zakazka=zakazka->dalsi;//posun na další prvek v seznamu
			}
	 }
}
////---------------------------------------------------------------------------
//uloží ukazatel na vozík do spojového seznamu voziků
void Cvektory::vloz_vozik(TZakazka *zakazka,short typ)//0-normální, 1-servisní
{
	TVozik *novy=new TVozik;
	novy->zakazka=zakazka;//přiřazení zakázky

	//ZDM pozor v případě načítání existujícího stavu ze souboru změnitm toto je výchozí pozice na lince
	//ZDM novy->segment=NULL;novy->stav=-1;
	//ZDM novy->X=0;novy->Y=0;novy->timer=0;;
	novy->typ=typ;
	novy->start=0;//výchozí pozice v grafu časových os
	novy->pozice=-1.0;//pozice na dopravniku či v grafu časových os

	novy->n=VOZIKY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	VOZIKY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=VOZIKY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje)
	VOZIKY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
};
//---------------------------------------------------------------------------
//slouží při úvodním načítání časových os, smaže výchozí a koncovou pozici sloužící pro tvorbu a zobrazení na časových osách
void Cvektory::vymazat_casovou_obsazenost_objektu_a_pozice_voziku(TObjekt *Objekt,TVozik *Vozik)
{
	TObjekt *ukaz=Objekt->dalsi;
	while (ukaz!=NULL)
	{
		ukaz->obsazenost=0;
		ukaz=ukaz->dalsi;
	};
	TVozik *ukaz1=Vozik->dalsi;
	while (ukaz1!=NULL)
	{
		ukaz1->pozice=-1.0;
		ukaz1=ukaz1->dalsi;
	};
}
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
long Cvektory::vymaz_seznam_VOZIKY()
{
	long pocet_smazanych_objektu=0;
	while (VOZIKY!=NULL)
	{
		pocet_smazanych_objektu++;
		VOZIKY->predchozi=NULL;
		delete VOZIKY->predchozi;
		VOZIKY=VOZIKY->dalsi;
	};

	return pocet_smazanych_objektu;
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
		TProces *P=Proces->dalsi;
		while (P!=NULL)
		{
			if(P->segment_cesty->objekt==Proces->segment_cesty->objekt)
			{
				break;
			}
			P=P->dalsi;
		};
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
void Cvektory::VALIDACE(TElement *Element)//zatím neoživáná varianta s parametrem!!!
{
	if(F->duvod_validovat==2)//validace probíhá jenom při editaci (pokud je důvod a až byla dokončena editační akce), nebo když nejsou zprávy po spuštění aplikace
	{
		F->duvod_validovat=0;//již bude zvalidováno a dále by se zbytečně volalo
		long pocet_erroru=0,pocet_warningu=0;

		//pokud se budou testovat všechny elementy, je nutné vymazat všechny zprávy
		if(Element==NULL)vymazat_ZPRAVY();

		//předělat s novým datovým modelem
		TObjekt *O=OBJEKTY->dalsi;
		while(O!=NULL)
		{
			TElement *E=O->elementy;//nepřeskakovat raději hlavičku
			if(F->pom_temp!=NULL && F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se jedná o editovaný objekt
			while(E!=NULL)
			{
				if(E->n!=0)//přeskočení hlavičky až zde
				{
					////výchozí hodnoty
					unsigned int pocet_pozic=E->max_pocet_voziku;   //doporučení rovnou to sbírat zde
					unsigned int pocet_voziku=E->akt_pocet_voziku;
					double rotaceJ=vrat_rotaci_jigu_po_predchazejicim_elementu(E);//metodu po přechodu na nový DM zaktulizovat o průchod přes spoják elementů
					double orientaceP=m.Rt90(E->geo.orientace-180);
					double X=F->d.Rxy(E).x;
					double Y=F->d.Rxy(E).y;
					double dJ=PP.delka_jig;//později nahradit ze zakázky
					double sJ=PP.sirka_jig;//později nahradit ze zakázky
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
						vloz_zpravu(X,Y,-1,219,E);pocet_erroru++;
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
					if(vrat_druh_elementu(E)==0)//pouze pro S&G
					{
						if(fabs(E->RT.y)>=1000000){vloz_zpravu(X,Y,-1,450,E);pocet_erroru++;}
						else
						{
							if(E->RT.y<0){vloz_zpravu(X,Y,-1,406,E);pocet_erroru++;}
							if(E->RT.y==0){vloz_zpravu(X,Y,1,407,E);pocet_warningu++;}
						}
					}
					////////////Pozor, překrytí JIGů! - musí být umístěno na konci (popř. na začátku)
					if(PP.delka_podvozek<m.UDJ(rotaceJ) && (E->rotace_jig==0 || E->rotace_jig==180) && pocet_voziku>1)
					{vloz_zpravu(X+x*PP.delka_podvozek*(pocet_pozic-1)/2.0,Y+y*PP.delka_podvozek*(pocet_pozic-1)/2.0,-1,402,E);pocet_erroru++;}
				}
				////posun na další elementy
				E=E->dalsi;
			}
			delete E;
			O=O->dalsi;
		}
		delete O;

		//zakutalizuje zprávy v miniformu zpráv
		Form_zpravy->update_zpravy(pocet_erroru,pocet_warningu);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//z čísla VIDu vrátí jeho textový popis
UnicodeString Cvektory::getVID(long VID)
{
	UnicodeString Text="";
	switch(VID) //řadit od nejdůležitějšího
	{
		case 219: Text=F->ls->Strings[219]+"!";break;//Pohon nepřiřazen!
		case 401: Text=F->ls->Strings[401];break;//Rotace neodpovídá orientaci JIGů na začátku linky!
		case 402: Text=F->ls->Strings[402];break;//Pozor, překrytí JIGů!
		case 406: Text=F->ls->Strings[406];break;//Nestíhá se přejezd, záporná časová rezerva!
		case 407: Text=F->ls->Strings[407];break;//Nulová časová rezerva.
		case 450: Text="Nerelevantní hodnota časové rezervy, na některém objektu není přiřazen pohon!";break;//Nulová časová rezerva.
		default: Text="Error or warning!";break;//obecná chyba či varování
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
	File_hlavicka.Verze=Form1->ms.MyToDouble(Form1->VERZE);
	File_hlavicka.Mod=Form1->MOD;
	File_hlavicka.Zoom=Form1->Zoom;
	File_hlavicka.PosunutiX=Form1->Posun.x;
	File_hlavicka.PosunutiY=Form1->Posun.y;
	File_hlavicka.cas_start=PP.cas_start;
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
	File_hlavicka.pocet_zakazek=ZAKAZKY->predchozi->n;
	File_hlavicka.pocet_voziku=VOZIKY->predchozi->n;
	//katalogové záležitosti
	File_hlavicka.katalog=PP.katalog;
	File_hlavicka.typ_linky=PP.typ_linky;
	File_hlavicka.radius=PP.radius;
	//stav ikony  TODO ROSTA dodelat
	//File_hlavicka.objekt_posunout_vse
}
//---------------------------------------------------------------------------
//Uloží vektorová data do souboru
short int Cvektory::uloz_do_souboru(UnicodeString FileName)
{
	try
	{
		 TFileStream *FileStream=new TFileStream(FileName,fmOpenWrite|fmCreate);

		 //zapiše hlavičku do souboru //už neplatí:+ zbylé atributy a PP se do hlavičky zapisují v unit1
		 vytvor_hlavicku_souboru();
		 FileStream->Write(&File_hlavicka,sizeof(TFile_hlavicka));

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
				 if(ukaz->body!=NULL)c_ukaz->pocet_bodu=ukaz->body->predchozi->n;
				 else c_ukaz->pocet_bodu=0;
				 if(ukaz->elementy!=NULL) c_ukaz->pocet_elementu=ukaz->elementy->predchozi->n;
				 else c_ukaz->pocet_elementu=0;
				 if(ukaz->komora!=NULL) c_ukaz->pocet_komor=ukaz->komora->predchozi->n;
				 else c_ukaz->pocet_komor=0;
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

				 if(c_ukaz->n>0 && c_ukaz->pocet_elementu>0) //poc elementu
				 {
				 	TElement *E=ukaz->elementy->dalsi;
				 	while(E!=NULL)
				 	{
				 		//překopírování dat do pomocného objektu uložitelného do bináru
				 		C_element *cE=new C_element;
				 		//plněný - kopírování dat jednotlivých atributů
				 		cE->n=E->n;
          //  ShowMessage("uloz element n"+AnsiString(E->n));
				 		cE->eID=E->eID;
				 		cE->name_delka=E->name.Length()+1;
				 		cE->X=E->X;
				 		cE->Y=E->Y;
				 		cE->Xt=E->Xt;
				 		cE->Yt=E->Yt;
				 		cE->orientace=E->orientace;
				 		cE->rotace_jig=E->rotace_jig;
				 		cE->stav=E->stav;
            cE->PD=E->PD;
						cE->LO1=E->LO1;
						cE->OTOC_delka=E->OTOC_delka;
						cE->zona_pred=E->zona_pred;
						cE->zona_za=E->zona_za;
						cE->LO2=E->LO2;
						cE->LO_pozice=E->LO_pozice;
						cE->PT1=E->PT1;
						cE->PTotoc=E->PTotoc;
						cE->PT2=E->PT2;
						cE->WT=E->WT;
						cE->WTstop=E->WTstop;
						cE->RT=E->RT;
						cE->akt_pocet_voziku=E->akt_pocet_voziku;
				 		cE->max_pocet_voziku=E->max_pocet_voziku;
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

						//posun na další segment cesty
						E=E->dalsi;
				 		delete cE; cE=NULL;
           }
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
			 }
			 c_ukaz=NULL;delete c_ukaz;
			 ukaz=ukaz->dalsi;//posunutí na další pozici v seznamu
		 };
		 ukaz=NULL;delete ukaz;

		 ////uložení HALy do_souboru
		 ////překopírování dat do pomocného objektu uložitelného do bináru
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

		 //uložení ZAKÁZEK
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
					 c_c->n_objekt=c->objekt->n;
					 c_c->CT=c->CT;
					 c_c->Tc=c->Tc;
					 c_c->Tv=c->Tv;
					 c_c->RD=c->RD;
					 c_c->Opak=c->Opak;
					 //uložení do binárního filu
					 FileStream->Write(c_c,sizeof(C_cesta));//zapiše jeden prvek do souboru
					 //posun na další segment cesty
					 c=c->dalsi;
					 c_c=NULL; delete c_c;
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
			//ZDM hlavicka_voziky();
			//pohony - musí být nad objekty
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
					ukaz1->Rz=c_ukaz1->Rz;
					ukaz1->Rx=c_ukaz1->Rx;
					ukaz1->retez=NULL;//prozatím jinak načítat buď z přidruženého spojáku nebo volat metodu po načtení elementů

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
					ukaz->orientace=c_ukaz->orientace;
					ukaz->pohon=vrat_pohon(c_ukaz->pohon);
					ukaz->koty_elementu_offset=c_ukaz->koty_elementu_offset;
					ukaz->elementy=NULL;   //NUTNOST PRO AUTO VYTVARENI HLAVICKY
					ukaz->komora=NULL;    //NUTNOST PRO AUTO VYTVARENI HLAVICKY
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
           // ShowMessage("pred nacitanim elem");
          //přiřazení elementů objektu
					for(unsigned int j=1;j<=c_ukaz->pocet_elementu;j++)
					{
					  C_element cE;//=new C_cesta;
					  FileStream->Read(&cE,sizeof(C_element));//načte jeden prvek ze souboru
						TElement *E=new TElement;
            E->n=cE.n;
         //   ShowMessage("element n:"+AnsiString(cE.n));
            E->eID=cE.eID;
						E->X=cE.X;
            E->Y=cE.Y;
            E->Xt=cE.Xt;
						E->Yt=cE.Yt;
            E->orientace=cE.orientace;
            E->rotace_jig=cE.rotace_jig;
						E->stav=cE.stav;
            E->PD=cE.PD;
            E->LO1=cE.LO1;
						E->OTOC_delka=cE.OTOC_delka;
						E->zona_pred=cE.zona_pred;
						E->zona_za=cE.zona_za;
						E->LO2=cE.LO2;
            E->LO_pozice=cE.LO_pozice;
            E->PT1=cE.PT1;
						E->PTotoc=cE.PTotoc;
            E->PT2=cE.PT2;
            E->WT=cE.WT;
						E->WTstop=cE.WTstop;
						E->RT=cE.RT;
            E->akt_pocet_voziku=cE.akt_pocet_voziku;
						E->max_pocet_voziku=cE.max_pocet_voziku;
            E->objekt_n=cE.objekt_n;
						E->pohon=vrat_pohon(cE.pohon_n);
						E->geo=cE.geo;
						//shortname
						wchar_t *short_name=new wchar_t [5];
						FileStream->Read(short_name,5*sizeof(wchar_t));//načte popisek umístěný za strukturou bod
						E->short_name=short_name;
						delete[] short_name; short_name=NULL;

						//name
						wchar_t *name=new wchar_t[cE.name_delka];
						FileStream->Read(name,cE.name_delka*sizeof(wchar_t));
						E->name=name;
      //      ShowMessage(AnsiString(name)+", nacti pohon n:"+AnsiString(cE.pohon_n));
						delete[] name; name=NULL;

						vloz_element(ukaz,E);
						//delete E; E=NULL; - nesmí být
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
					//vložení finálního objektu do spojáku
					vloz_objekt(ukaz);
				}
				ukaz=NULL; delete ukaz;
				c_ukaz=NULL; delete c_ukaz;
			};

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
						vloz_segment_cesty(ukaz2,c_c.n_objekt,c_c.CT,c_c.Tc,c_c.Tv,c_c.RD,c_c.Opak);
					}

					//vloží zakazku do spojového seznamu ZAKAZKY
					vloz_zakazku(ukaz2);
				}
				//ukaz2=NULL; delete ukaz2;
				c_ukaz2=NULL; delete c_ukaz2;
			};

			aktualizuj_sparovane_ukazatele();//prozatimní řešení, aktualizuje spárovné ukazatele po načtení
			delete FileStream;
			return 1;
			}
			catch(...){;return 2;}//jiná chyba, např. špatný formát souboru
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
	TPointD RET; RET.x=0;RET.y=0;bool prvni=true;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka->n==jaka->n && prvni){RET.x=vozik->start/Form1->d.PX2MIN*60.0;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
		if(vozik->zakazka->n==jaka->n){RET.y=vozik->pozice/Form1->d.PX2MIN*60.0;}//uloží koncovou pozici posledního vozíku na zakázce
		vozik=vozik->dalsi;
	}
	return RET;
}
TPointD Cvektory::vrat_zacatek_a_konec_zakazky(unsigned int ID_zakazky)//ukazatel na cestu resp, zakázku
{
	TPointD RET; RET.x=0;RET.y=0;bool prvni=true;
	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
	while (vozik!=NULL)
	{
		if(vozik->zakazka->n==ID_zakazky && prvni){RET.x=vozik->start/Form1->d.PX2MIN*60.0;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
		if(vozik->zakazka->n==ID_zakazky)RET.y=vozik->pozice/Form1->d.PX2MIN*60.0;//uloží koncovou pozici posledního vozíku na zakázce
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
			LT+=ukaz->CT;
			ukaz=ukaz->dalsi;//posun na další prvek
		}
		return LT;
}
////---------------------------------------------------------------------------
double Cvektory::vrat_LT_voziku(TVozik *jaky)//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
{
	if(jaky!=NULL) return (jaky->pozice-jaky->start)/Form1->d.PX2MIN*60.0;
	else return 0;
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
	Cvektory::TCesta *C=jaky->zakazka->cesta->dalsi;
	while(C!=NULL)//jde po konkrétní cestě
	{
		SUM+=C->CT;
		C=C->dalsi;
	}
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
	return (jaky->pozice-jaky->predchozi->pozice)/Form1->d.PX2MIN*60.0;//ošetřuje i případ prvního prvku a hlavičky, hlavička má pozici nastavenou na nula
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
			if(V->n==n_voziku)RET.x=V->start;
			if(V->n==n_voziku)RET.y=V->pozice;
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

					if(vozik->pozice>=vozik2->start)//pokud nastane situace že vozík skončil před začátkem vozíku, není nutné navyšovat počítadlo vozíků
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
				pocet_final+=ukaz->pozice;
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
	 if(O->rezim!=0)//pokud se nejedná o S&G, ten má kapacitu vždy 1, pokud ano algoritmus se přeskočí
	 {
			 TProces *P=PROCESY->dalsi;
			 while (P!=NULL)
			 {
				 unsigned int pocet=1;
				 if(P->segment_cesty->objekt->n==O->n)//pokud se jedná o hledaný objekt
				 {
						TProces *P2=P->dalsi;
						while (P2!=NULL)
						{
							if(P2->segment_cesty->objekt->n==O->n && P->Tcek>P2->Tpoc)//pokud se jedná o spoluhledaný objekt a objekty se v čase zároveň překrývají (tudíž se navyšuje jejich kapacita)
							pocet++;
							P2=P2->dalsi;
						}
				 }
				 if(pocet_final<pocet)pocet_final=pocet;
				 P=P->dalsi;
			 };
	 }
	 else pocet_final=1;
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

//		//palce
//		if(PALCE->predchozi->n>0)//pokud je více objektů
//		{
//			vymaz_seznam();//vymaze body z paměti
//			delete PALCE; PALCE=NULL;
//		}

//	//procesy
//	if(PROCESY!=NULL && PROCESY->predchozi->n>0)//pokud je více objektů
//	{
//		//vymaz_seznam_procesu();
//		delete PROCESY; PROCESY=NULL;
//	}
//	hlavicka_PROCESY();

	/*  až budu pracovat s UNDO a REDO
	v.vymaz_seznam(v.p_redo);
	v.vymaz_seznam(v.p_undo);
	v.vymaz_seznam(v.l_redo);
	v.vymaz_seznam(v.l_undo);

	delete v.p_redo;v.p_redo=NULL;
	delete v.p_undo;v.p_undo=NULL;
	delete v.l_redo;v.l_redo=NULL;
	delete v.l_undo;v.l_undo=NULL;
	*/
}
////---------------------------------------------------------------------------


