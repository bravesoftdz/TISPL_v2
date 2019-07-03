////---------------------------------------------------------------------------
#pragma hdrstop
#include "vektory.h"
#include "unit1.h"
#include "parametry.h"
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
	hlavicka_RETEZY();//vytvoří novou hlavičku pro řetězy
	//	hlavicka_palce();
	HALA.body=NULL;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////společné metody pro HALU a objekty
//vloží nový bod na konec seznamu bodů pokud je Za=NULL, jinak vloží za tento bod
void Cvektory::vloz_bod(double X, double Y,TObjekt *Objekt,TBod *ZaBod, bool ortogonalizovat)
{
	////alokace paměti
	TBod *Bod=new TBod;
					 ortogonalizovat=false;
	////data
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
		if(m.abs_d(B->X-X)<m.abs_d(B->Y-Y)){Bod->X=B->X;Bod->Y=Y;}//zarovnat dle X
		else {Bod->X=X;Bod->Y=B->Y;}//zarovnat dle Y
		B=NULL;delete B;
	}
	else//nikoliv vložení bez ortogonalizace
	{
		Bod->X=X;
		Bod->Y=Y;
  }

	////vkládání do bodů objektu
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
		//vložení nového boud na konec seznamu bodů
		if(ZaBod==NULL || ZaBod!=NULL && ZaBod==Objekt->body->predchozi)//pokud se má vkládat nakonec
		{
			Bod->n=Objekt->body->predchozi->n+1;//navýšení počítadla
			Bod->predchozi=Objekt->body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
			Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
			Objekt->body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
			Objekt->body->predchozi=Bod;//hlavička ukazuje již na novou komoru jako poslední prvek
		}
		else////vložení mezi komory
		{
			//nastavení počítadla u vkládané komory
			Bod->n=ZaBod->n+1;
			//navýšení počítadla u následujícíh komor
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
			ZaBod->dalsi=Bod;//za požadovanou komoru se vloží vkládaná komora
		}
	}
	else//pro HALU
	{
		//pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(HALA.body==NULL)
		{
			HALA.body=new TBod;
			HALA.body->n=0;
			HALA.body->predchozi=HALA.body;//hlavička ukazuje sama na sebe
			HALA.body->dalsi=NULL;
		}
		//vložení nového boud na konec seznamu bodů
		if(ZaBod==NULL || ZaBod!=NULL && ZaBod==HALA.body->predchozi)//pokud se má vkládat nakonec
		{
			Bod->n=HALA.body->predchozi->n+1;//navýšení počítadla
			Bod->predchozi=HALA.body->predchozi;//nový bod ukazuje na poslední prvek ve spojaku jako na prvek předchozí
			Bod->dalsi=NULL;//nový bod neukazuje na žádný další prvek, resp. ukazuje na NULL
			HALA.body->predchozi->dalsi=Bod;//za poslední aktuální prvek vloží nový poslední
			HALA.body->predchozi=Bod;//hlavička ukazuje již na novou komoru jako poslední prvek
		}
		else////vložení mezi komory
		{
			//nastavení počítadla u vkládané komory
			Bod->n=ZaBod->n+1;
			//navýšení počítadla u následujícíh komor
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
			ZaBod->dalsi=Bod;//za požadovanou komoru se vloží vkládaná komora
		}
  }
}
////---------------------------------------------------------------------------
//na aktuálních souřadnicích myši hledá bod, pokud je nalezen vrátí na něj ukazatel, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
Cvektory::TBod *Cvektory::najdi_bod(TObjekt* Objekt)
{
	float o=0.2;//citelná oblast v metrech, pokud nebudu chtít zvětšovat se zoomem, tak zde podělit zoomem
	TBod *B=Objekt->body->dalsi;//jedná se o body objektu + přeskočí hlavičku
	if(Objekt==NULL)HALA.body->dalsi;//jedná se bod haly + přeskočí hlavičku
	while(B!=NULL)
	{
		if(m.PtInRectangle(B->X-o,B->Y+o,B->X+o,B->Y+o,F->akt_souradnice_kurzoru.x,F->akt_souradnice_kurzoru.y))break;
		B=B->dalsi;
	}
	return B;
}
////---------------------------------------------------------------------------
//zkopíruje body včetně z originálu na kopii bez ukazatelového propojení, funguje jenom pro body objektů nikoliv HALY!!!
void Cvektory::kopiruj_body(TObjekt *Original,TObjekt *Kopie)
{
	//pokud kopie obsahuje původní body, tak je smaže
	vymaz_body(Kopie);
	//samotné nakopírování
	TBod *B=Original->body->dalsi;//přeskočí hlavičku
	while(B!=NULL)
	{
		vloz_bod(B->X,B->Y,Kopie);
		B=B->dalsi;
	}
	B=NULL;delete B;
}
////---------------------------------------------------------------------------
//smaže konkrétní bod, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
void Cvektory::smaz_bod(TBod* Bod,TObjekt* Objekt)
{
	if(Bod!=NULL && Bod->n!=0)//pokud existuje a zároveň mimo hlavičky, ta nejde smazat
	{
		if(Bod->dalsi==NULL)//jedná se o poslední prvek (múže být i za hlavičkou)
		{
			if(Objekt->body!=NULL)Objekt->body->predchozi=Bod->predchozi;//pro body
			else HALA.body->predchozi=Bod->predchozi;//pro HALU
			Bod->predchozi=NULL;
		}
		else//nejedná se o poslední prvek
		{
			Bod->predchozi->dalsi=Bod->dalsi;
			Bod->dalsi->predchozi=Bod->predchozi;
		}
		Bod=NULL;delete Bod;
	}
}
////---------------------------------------------------------------------------
//vymaže všechny body včetně hlavičky, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
void Cvektory::vymaz_body(TObjekt* Objekt)
{
	if(Objekt->body!=NULL)//pro body
	{
		//maže od zadu dokud nezbyde pouze hlavička
		while(Objekt->body->dalsi==NULL)smaz_bod(Objekt->body->predchozi,Objekt);
		//na závěr ještě smaže hlavičku
		Objekt->body=NULL;delete Objekt->body;
	}
	else//pro HALU
	{
		//maže od zadu dokud nezbyde pouze hlavička
		while(HALA.body->dalsi==NULL)smaz_bod(HALA.body->predchozi);
		//na závěr ještě smaže hlavičku
		HALA.body=NULL;delete HALA.body;
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
	novy->Xk=0;
	novy->Yk=0;
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
	novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//velikost mezery mezi vozíky
	novy->pohon=NULL;//ukazatel na použitý pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->rozmer_kabiny.x=0;
	novy->rozmer_kabiny.y=0;
	novy->koty_elementu_offset=0;//odsazení kót elementů v metrech
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
	AnsiString name,short_name;//dočasná konstrukce pro přiřazování spráných názvů objektům
	if(id==F->VyID){name=knihovna_objektu[id].name+" "+AnsiString(pocet_vyhybek);short_name=knihovna_objektu[id].short_name+AnsiString(pocet_vyhybek);}
	else if(id<=pocet_objektu_knihovny) {name=knihovna_objektu[id].name;short_name=knihovna_objektu[id].short_name;}else {name="Spojka "+AnsiString(pocet_vyhybek);short_name="S"+AnsiString(pocet_vyhybek);}

	TObjekt *novy=new TObjekt;

	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->id=id;
	novy->short_name=short_name;
	novy->name=name;
	novy->rezim=0;if(id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu,pozice objektu
	novy->Y=Y;//přiřadím Y osu,pozice objektu
	novy->Xk=X;//výchozí pozice kabiny
	novy->Yk=Y;//výchozí pozice kabiny
	novy->sirka_steny=0.12;//šířka stěny kabiny objektu v metrech
	novy->CT=PP.TT;//pro status návrh
	novy->RD=m.UDV(0)/novy->CT;//pro status návrh
	novy->delka_dopravniku=m.UDV(0);//delka dopravníku v rámci objektu
	novy->kapacita=1;
	novy->kapacita_dop=0;
	novy->pozice=1;
	novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//mezera mezi vozíky (kritická mezera)
	novy->mezera_jig=0;//mezera mezi jigy
	novy->mezera_podvozek=0;//mezera mezi podvozky
	novy->pohon=NULL;//při vložení nemá vložen žádný pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->rozmer_kabiny.x=10;//výchozí rozměr kabiny
	novy->rozmer_kabiny.y=6;//výchozí rozměr kabiny
	novy->koty_elementu_offset=4;//odsazení kót elementů v metrech
	novy->komora=NULL;//ukazatel na komory
	if(id==3)for(short i=1;i<=4;i++)vloz_komoru(novy,novy->rozmer_kabiny.x/4.0);//pokud se jedná o POWash,nastaví defaultně 4 stejné komory
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
	AnsiString name,short_name;//dočasná konstrukce pro přiřazování spráných názvů objektům
	if(id==F->VyID){name=knihovna_objektu[id].name+" "+AnsiString(pocet_vyhybek);short_name="V"+AnsiString(pocet_vyhybek);}
	else if(id<=pocet_objektu_knihovny) {name=knihovna_objektu[id].name;short_name=knihovna_objektu[id].short_name;}else {name="Spojka "+AnsiString(pocet_vyhybek);short_name="S"+AnsiString(pocet_vyhybek);}

	TObjekt *novy=new TObjekt;
	novy->id=id;
	novy->short_name=short_name;
	novy->name=name;
	novy->rezim=0;if(id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu
	novy->Y=Y;//přiřadím Y osu
	novy->Xk=X;//výchozí pozice kabiny
	novy->Yk=Y;//výchozí pozice kabiny
	novy->sirka_steny=0.12;//šířka stěny kabiny objektu v metrech
	novy->CT=PP.TT;//pro status návrh
	novy->RD=m.UDV(0)/novy->CT;//pro status návrh
	novy->delka_dopravniku=m.UDV(0);//delka dopravníku v rámci objektu
	novy->kapacita=1;
	novy->kapacita_dop=0;
	novy->pozice=1;
	novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//velikost mezery mezi vozíky  (kritická mezera)
	novy->mezera_jig=0;//mezera mezi jigy
	novy->mezera_podvozek=0;//mezera mezi podvozky
	novy->pohon=NULL;//při vložení nemá vložen žádný pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->rozmer_kabiny.x=10;//výchozí rozměr kabiny
	novy->rozmer_kabiny.y=6;//výchozí rozměr kabiny
	novy->koty_elementu_offset=4;//odsazení kót elementů v metrech
	novy->komora=NULL;//ukazatel na komory
	if(id==3)for(short i=1;i<=4;i++)vloz_komoru(novy,novy->rozmer_kabiny.x/4.0);//pokud se jedná o POWash,nastaví defaultně 4 stejné komory
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

//	novy->predchozi=pred;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->predchozi2=NULL;
//	novy->dalsi=pred->dalsi;
//	pred->dalsi->predchozi=novy;
//	pred->dalsi=novy;
//	novy->n=pred->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší

	//////testovací konstrukce
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
  AnsiString name,short_name;//dočasná konstrukce pro přiřazování spráných názvů objektům
	if(id==F->VyID){name=knihovna_objektu[id].name+" "+AnsiString(pocet_vyhybek);short_name=knihovna_objektu[id].short_name+AnsiString(pocet_vyhybek);}
	else if(id<=pocet_objektu_knihovny) {name=knihovna_objektu[id].name;short_name=knihovna_objektu[id].short_name;}else {name="Spojka "+AnsiString(pocet_vyhybek);short_name="S"+AnsiString(pocet_vyhybek);}

	TObjekt *novy=new TObjekt;
	novy->id=id;
	novy->short_name=short_name;
	novy->name=name;
	novy->rezim=0;if(id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu
	novy->Y=Y;//přiřadím Y osu
	novy->Xk=X;//výchozí pozice kabiny
	novy->Yk=Y;//výchozí pozice kabiny
	novy->sirka_steny=0.12;//šířka stěny kabiny objektu v metrech
	novy->CT=PP.TT;//pro status návrh
	novy->RD=m.UDV(0)/novy->CT;//pro status návrh
	novy->delka_dopravniku=m.UDV(0);//delka dopravníku v rámci objektu
	novy->kapacita=1;
	novy->kapacita_dop=0;
	novy->pozice=1;
	novy->rotace=0;//rotace jigu v objektu
	novy->mezera=0;//velikost mezery mezi vozíky  (kritická mezera)
	novy->mezera_jig=0;//mezera mezi jigy
	novy->mezera_podvozek=0;//mezera mezi podvozky
	novy->pohon=NULL;//při vložení nemá vložen žádný pohon
	novy->elementy=NULL;//ukazatel na přidružené elementy
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->rozmer_kabiny.x=10;//výchozí rozměr kabiny
	novy->rozmer_kabiny.y=6;//výchozí rozměr kabiny
	novy->koty_elementu_offset=4;//odsazení kót elementů v metrech
	novy->komora=NULL;//ukazatel na komory
	if(id==3)for(short i=1;i<=4;i++)vloz_komoru(novy,novy->rozmer_kabiny.x/4.0);//pokud se jedná o POWash,nastaví defaultně 4 stejné komory
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
	{
		vloz_objekt(Objekt);
		OBJEKTY->predchozi->short_name=novy->short_name;
		OBJEKTY->predchozi->name=novy->name;
		OBJEKTY->predchozi->X=novy->X;
		OBJEKTY->predchozi->Y=novy->Y;
		OBJEKTY->predchozi->Xk=novy->Xk;
		OBJEKTY->predchozi->Yk=novy->Yk;
		return OBJEKTY->predchozi;//vrátí poslední ukazatel na prvek seznamu
	}
	else//vkládání mezi objekty
	{
		novy->rezim=Objekt->rezim;
		novy->sirka_steny=Objekt->sirka_steny;//šířka stěny kabiny objektu v metrech
		novy->CT=Objekt->CT;//pro status návrh převezme původní hodnoty
		novy->RD=Objekt->RD;//pro status návrh převezme původní hodnoty
		novy->delka_dopravniku=Objekt->delka_dopravniku;
		novy->kapacita=Objekt->kapacita;
		novy->kapacita_dop=Objekt->kapacita_dop;
		novy->pozice=Objekt->pozice;
		novy->rotace=Objekt->rotace;
		novy->mezera=Objekt->mezera;//velikost mezery mezi vozíky (kritická mezera)
		novy->mezera_jig=Objekt->mezera_jig;//mezera mezi jigy
		novy->mezera_podvozek=Objekt->mezera_podvozek;//mezera mezi podvozky
		novy->pohon=Objekt->pohon;
		kopiruj_elementy(Objekt,novy);
		novy->min_prujezdni_profil=Objekt->min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
		novy->rozmer_kabiny=Objekt->rozmer_kabiny;//výchozí rozměr kabiny
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
	Kopie->n=Original->n;
	Kopie->id=Original->id;
	Kopie->short_name=Original->short_name;
	Kopie->name=Original->name;
	Kopie->X=Original->X;
	Kopie->Y=Original->Y;
	Kopie->Xk=Original->Xk;
	Kopie->Yk=Original->Yk;
	Kopie->sirka_steny=Original->sirka_steny;
	Kopie->rezim=Original->rezim;
	Kopie->CT=Original->CT;
	Kopie->RD=Original->RD;
	Kopie->delka_dopravniku=Original->delka_dopravniku;
	Kopie->kapacita=Original->kapacita;
	Kopie->kapacita_dop=Original->kapacita_dop;
	Kopie->pozice=Original->pozice;
	Kopie->rotace=Original->rotace;
	Kopie->mezera=Original->mezera;
	Kopie->mezera_jig=Original->mezera_jig;
	Kopie->mezera_podvozek=Original->mezera_podvozek;
	kopiruj_pohon(Original->pohon,Kopie);//POHON
	Kopie->min_prujezdni_profil=Original->min_prujezdni_profil;
	Kopie->rozmer_kabiny=Original->rozmer_kabiny;
	Kopie->koty_elementu_offset=Original->koty_elementu_offset;
	Kopie->komora=NULL;
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
	//ELEMENTY - musí být ke konci
	kopiruj_elementy(Original,Kopie);
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
	if(Objekt->id==F->VyID&&!opakovani)spojka_vyh=Objekt->predchozi2;
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
					O->delka_dopravniku=O->kapacita*m.UDV(O->rotace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
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
					O->delka_dopravniku=O->kapacita*m.UDV(O->rotace)*O->mezera;//ošetřeno i pro stav kdy je stejný počet mezer jako vozíku
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
					O->delka_dopravniku=O->kapacita*m.UDV(O->rotace)*O->mezera;//DD
					//CT
					if(O->rezim==1)O->CT=O->delka_dopravniku/O->RD;//pro kontinual
					else//pro PP
					{
						O->kapacita=O->delka_dopravniku/(m.UDV(O->rotace)+O->mezera);
					}
				}
			}
			break;
			case 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
			{
				if(O->rezim!=0)//pro kontinuál a PP
				{
					//K
					O->kapacita=O->delka_dopravniku/(m.UDV(O->rotace)+O->mezera);//K
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
		double DV=m.UDV(O->rotace);
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
	TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];
	int i=1,vyhybek=1;//i uchovává na kolikátém objektu se nacházím, vyhybek = počet vyhybek na které jsem narazil (jen jednou)
	while(O!=NULL)
	{
		int n=F->ms.MyToDouble(O->short_name.SubString(2,1));//extrakce čísla výhybky / spojky
		//přiřazení nového n
		if((long)O->id==pocet_objektu_knihovny+1)if(tab_pruchodu[n].y==0){O->n=i;i++;}
		if((long)O->id==F->VyID)if(tab_pruchodu[n].x==0){O->n=i;i++;}
		if((long)O->id!=F->VyID&&(long)O->id!=pocet_objektu_knihovny+1){O->n=i;i++;}
		//pokud došlo k události kvůli které bude nutné přejmenovat, uloží nová čísla výhybek a spojek do name
		if(nasledne_zmena_nazvu)
		{
    	if((long)O->id==pocet_objektu_knihovny+1)if(tab_pruchodu[n].x==0&&tab_pruchodu[n].y==0){O->name="Spojka "+AnsiString(vyhybek);O->dalsi2->name="Výhybka "+AnsiString(vyhybek);vyhybek++;}
			if((long)O->id==F->VyID)if(tab_pruchodu[n].x==0&&tab_pruchodu[n].y==0){O->name="Výhybka "+AnsiString(vyhybek);O->predchozi2->name="Spojka "+AnsiString(vyhybek);vyhybek++;}
    }
		O=dalsi_krok(O,tab_pruchodu);
	}
	O=NULL;tab_pruchodu=NULL;delete O;delete tab_pruchodu;
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
//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, nastaví velikost dané komory dle proměnné velikost
void Cvektory::vloz_komoru(TObjekt *Objekt,double velikost,TKomora *ZaKomoru)
{
	TKomora *Komora=new TKomora;
	Komora->velikost=velikost;
	vloz_komoru(Objekt,Komora,ZaKomoru);
}
//---------------------------------------------------------------------------
//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, není třeba nastavovat ukazatele ani n-pořadí
void Cvektory::vloz_komoru(TObjekt *Objekt,TKomora *Komora,TKomora *ZaKomoru)
{
	if(Komora!=NULL)
	{
		////pokud ještě HLAVIČKA neexistuje, tak ji založí
		if(Objekt->komora==NULL)
		{
			Objekt->komora=new TKomora;
			Objekt->komora->n=0;
			Objekt->komora->predchozi=Objekt->komora;//hlavička ukazuje sama na sebe
			Objekt->komora->dalsi=NULL;
		}
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
			//navýšení počítadla u následujícíh komor
			TKomora *K=ZaKomoru->dalsi;
			while(K!=NULL)
			{
				K->n++;
				K=K->dalsi;
			}
			K=NULL;delete K;
			//nové ukazatelové propojení
			ZaKomoru->dalsi->predchozi=Komora;//následující komoře přídá ukaztel na předchozí na vkladanou
			Komora->dalsi=ZaKomoru->dalsi;//vkládaná ukazuje na původní následují
			ZaKomoru->dalsi=Komora;//za požadovanou komoru se vloží vkládaná komora
		}
	}
}
//---------------------------------------------------------------------------
//na aktuálních souřadnicích myši hledá komoru, pokud je nalezena vrátí na ni ukazatel
Cvektory::TKomora *Cvektory::najdi_komoru(TObjekt* Objekt)
{
	TKomora *K=Objekt->komora->dalsi;//přeskočení hlavičky
	double vzdalenost=0;
	while(K!=NULL)
	{
		if(Objekt->rotace==0 || Objekt->rotace==180)
		{
			if(Objekt->Xk+vzdalenost<=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x<=Objekt->Xk+vzdalenost+K->velikost && Objekt->Yk>=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y>Objekt->Yk-Objekt->rozmer_kabiny.y)
			break;
		}
		else
		{
			if(Objekt->Yk+vzdalenost>=F->akt_souradnice_kurzoru.y && F->akt_souradnice_kurzoru.y<=Objekt->Yk+vzdalenost+K->velikost && Objekt->Xk<=F->akt_souradnice_kurzoru.x && F->akt_souradnice_kurzoru.x<Objekt->Yk+Objekt->rozmer_kabiny.x)
			break;
		}
		vzdalenost+=K->velikost;//přičtení šířky další komory
		K=K->dalsi;
	}
	return K;
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
		vloz_komoru(Kopie,K->velikost);
		K=K->dalsi;
	}
	K=NULL;delete K;
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
			Komora->predchozi->dalsi=NULL;   //Původní: Komora->predchozi=NULL;
		}
		else//nejedná se o poslední prvek
		{
			Komora->predchozi->dalsi=Komora->dalsi;
			Komora->dalsi->predchozi=Komora->predchozi;
		}
		Objekt->komora->predchozi->velikost+=Komora->velikost;//zvětšení poslední komory o odstraněnou komoru
		//přeindexování komor, index n souží k získání počtu komor v objektu
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
//---------------------------------------------------------------------------
//vymaže spojový seznam technologických objektů včetně přidružených elementů a případných komor z paměti
long Cvektory::vymaz_seznam_OBJEKTY()
{
	long pocet_smazanych_objektu=0;
	while (OBJEKTY!=NULL)
	{
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
	if(Objekt->id==F->VyID)//výhybka
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
	Objekt->elementy->rotace_symbolu=0;
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
	Objekt->elementy->RT=0;

	Objekt->elementy->mGrid=NULL;

	Objekt->elementy->sparovany=NULL;
	Objekt->elementy->predchozi=Objekt->elementy;//ukazuje sam na sebe
	Objekt->elementy->dalsi=NULL;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
Cvektory::TElement *Cvektory::vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y,short rotace_symbolu)
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
	novy->rotace_symbolu=rotace_symbolu;//důležité pro volání makra m.Rxy, bez tohoto by makro vracelo chybné hodnoty

  //ukazatelové propojení - bylo původně poslední, ale nemohlo fungovat správně
//	vloz_element(Objekt,novy);
	vloz_element(Objekt,novy);

	//defaultní data
	novy->LO1=1.5;
	novy->OTOC_delka=0;
	novy->LO2=0;
	novy->LO_pozice=0;
	novy->PT1=0;
	novy->PTotoc=0;
	novy->PT2=0;
	novy->WT=0;//čekání na palec
	novy->WTstop=0;//čekání na stopce
	novy->RT=0;//reserve time
	novy->stav=1;

	//název
	AnsiString T="";
	switch(eID)
	{
		case 0: T="Stop"; break;//stop stanice
		case 1:case 7:case 11:case 15:case 101:case 105:  T="Robot"; 				break;//kontinuální robota
		case 2:case 8:case 12:case 16:case 102:case 106:  T="Robot"; 				novy->PT1=60;break;//robot se stopkou
		case 3:case 9:case 13:case 17:case 103:case 107:  T="Robot"; 				novy->OTOC_delka=0.450;novy->LO1=(1.5-novy->OTOC_delka)/2.0;novy->LO2=novy->LO1;novy->rotace_jig=180;break;//kontinuální robot s pasivní otočí
		case 4:case 10:case 14:case 18:case 104:case 108: T="Robot";				novy->PT1=60;novy->PTotoc=20;novy->PT2=60;novy->rotace_jig=180; break;//robot s aktivní otočí (tj. s otočí a se stopkou)
		case 5: T="Otoč"; 																									novy->OTOC_delka=0.450;novy->rotace_jig=90;break;//pasivní otoč
		case 6: T="Otoč"; 																									novy->PTotoc=20;novy->rotace_jig=90;break;//aktivní otoč
		case 100: T="ION tyč";
	}
	if(novy->name=="")//přiřazení názvu pouze v případě, že element žádné nemá, při posuvu je novému elementu přiřazeno jméno
	{
		unsigned int nTyp=vrat_poradi_elementu_do(Objekt,novy)+1;//pokud se jedná o roboty
		novy->name=T+" "+AnsiString(nTyp);
		novy->short_name=T.SubString(1,3)+AnsiString(nTyp);
	}

	//mGrid elementu
	if(Objekt->n==F->pom_temp->n)//stačí nastavovat pouze v náhledu při vloz_element, nová strategie, je mgrid, nekopírovat a používat jenom v pom_temp, zde však podmínka zda se jedná o pom_temp nebyla z nějakého důvodu možná
	{
		novy->mGrid=new TmGrid(F);
		novy->mGrid->Tag=6;//ID formu
		novy->mGrid->ID=cislo_mGrid;//novy->n;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
	}                  //číslování mGridů zvlášť!!! z důvodu přehazování elementů

	//ukazatel na spárovaný element
	novy->sparovany=vrat_predchozi_stop_element(novy);//metoda již v sobě ošetřuje, že se bude jednat o stopku

	//návrácení ukazatelele na element k dalšímu použití
	return novy;
}
////---------------------------------------------------------------------------
//vloží element do spojového seznamu elementů daného technologického objektu
void  Cvektory::vloz_element(TObjekt *Objekt,TElement *Element)
{
	if(Objekt->elementy==NULL)hlavicka_elementy(Objekt);//pokud by ještě nebyla založena hlavička, tak ji založí

	Element->n=Objekt->elementy->predchozi->n+1;//navýším počítadlo prvku o jedničku

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
	}
	else if(p->n!=Element->n)//vkládám mezi elementy, vpřípadě, že bylo vloženo před prví prvek vrací Element, přesun je již vyřešen
	{
		//ukazatelové propojení
		Element->dalsi=p->dalsi;
		Element->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		p->dalsi->predchozi=Element;
		p->dalsi=Element;
		//změna indexů
		int n=1;
		Cvektory::TElement *E=Objekt->elementy->dalsi;
		while(E!=NULL)
		{
			E->n=n;
			n++;
			E=E->dalsi;
		}
		E=NULL;delete E;
		//změna názvů
		uprav_popisky_elementu(Objekt,Element);
	}
	p=NULL; delete p;
}
////---------------------------------------------------------------------------
//kontrola zda vkládaný element bude umístěn na konec nebo mezi jiné elementy
Cvektory::TElement *Cvektory::vloz_element_za(TObjekt *Objekt,TElement *Element)
{
	Cvektory::TElement *ret=NULL;//návratová proměnná, defaultně prázdn
	bool pred1=false,zaposlednim=false;//proměnné pro 2 speciální případy které budou řešeny kompletně v této metodě
	if((F->Akce==F->ADD||F->Akce==F->MOVE_ELEMENT)&&Objekt->elementy->dalsi!=NULL)//ošetření proti spouštění při zavírání a otvírání náhledu
	{
		Cvektory::TElement *p=Objekt->elementy->dalsi;//přeskočí hlavičku
		while (p!=NULL)
		{
			if(p->dalsi!=NULL&&p->n!=Element->n&&p->dalsi->n!=Element->n)//aby se neřešila situace poslední-prní prvek,řešeno separátně
			{   
				//kontrola zda vkládaný element neleží mezi prvním a druhým elementem, druhým až n
				if(F->m.PtInRectangle(F->d.Rxy(p).x,F->d.Rxy(p).y,F->d.Rxy(p->dalsi).x,F->d.Rxy(p->dalsi).y,F->d.Rxy(Element).x,F->d.Rxy(Element).y))
				{
					ret=p;//uložení elementu, který předcházi vkládanému elementu
					break;
				}
			}
			p=p->dalsi;//posun na další prvek
		}
		p=NULL; delete p;
		//případ přidání+posun před první a posun za poslední budou řešeny tady, metoda vrátí NULL v těchto případech
		//a to z důvodu rozdílné funkcionality
		//řešeno přes minimální a maximální souřadnice elementů v kabině, při použití podobného hledání jako u hledání v oblasti problém
		//s řazením elementů, pokud nejsou v řadě za sebou, což v tuto chvíli nejsou nedává hledání reálné výsledky
		if(ret==NULL&&(Element->rotace_symbolu==0||Element->rotace_symbolu==180))//mezi 2 elementy nic nenalezeno = kontrola před prvním a za posledním
		{
			if(F->d.Rxy(Element).x<=F->d.Rxy(Objekt->elementy->dalsi).x&&Element->n!=1)pred1=true;
			if(F->d.Rxy(Element).x>=F->d.Rxy(Objekt->elementy->predchozi).x&&Element->n!=Objekt->elementy->predchozi->n&&Element->name!="")zaposlednim=true;
		}
		else if(ret==NULL)
    {
			if(F->d.Rxy(Element).y>=F->d.Rxy(Objekt->elementy->dalsi).y&&Element->n!=1)pred1=true;
			if(F->d.Rxy(Element).y<=F->d.Rxy(Objekt->elementy->predchozi).y&&Element->n!=Objekt->elementy->predchozi->n&&Element->name!="")zaposlednim=true;
		}
		if(pred1||zaposlednim)//výjmutí ze spojáku, pro oba případy stejné
		{
			if(Element->name!="")//nutné výjmout element ze spojáku
			{
				if(Element->dalsi!=NULL)//pokud se nejedná o poslední prvek
				{
					Element->predchozi->dalsi=Element->dalsi;//předchozí bude ukazovat na následující
					Element->dalsi->predchozi=Element->predchozi; //následující bude ukazovat na další
				}
				else//pokud se jedná o poslední prvek
				{
					Objekt->elementy->predchozi=Element->predchozi;
					Element->predchozi->dalsi=NULL;
				}
			}
		}
		if(pred1)//ukazatelové propojení pro případ vložení před první element
		{
			Element->dalsi=Objekt->elementy->dalsi;//za elementem první element
			Objekt->elementy->dalsi->predchozi=Element;//za předchozým prvním prvkem nový Element
			Objekt->elementy->dalsi=Element;//za hlavičkou nový element
			Element->predchozi=Objekt->elementy;//před elementem hlavička
		}
		if(zaposlednim)//ukazatelové propojení pro případ vložení za poseední element
		{
			Element->dalsi=NULL;//element na hlavičku
			Objekt->elementy->predchozi->dalsi=Element;//poslední na element
			Element->predchozi=Objekt->elementy->predchozi;//element za posledním
			Objekt->elementy->predchozi=Element;//element poslední
		}
		if(pred1||zaposlednim)//přeindexování a přejmenování, pro oba případy stejné
		{
			//přeindexování
			int n=1;
			Cvektory::TElement *E=Objekt->elementy->dalsi;//možno přeskočit hlavičku
			while(E!=NULL)
			{
				E->n=n;
				n++;
				E=E->dalsi;
			}
			E=NULL; delete E;
			//změna popisků
			uprav_popisky_elementu(Objekt,Element);
			ret=Element;//nastaveno aby v ostatních metodách již nedocházelo k přesunu elementu ve spojáku a přejmenování
		}
	}
	return ret;
}
////---------------------------------------------------------------------------
//pokud byl nějaký element vložen mezi ostatní a ne na konec, provede přejměnování,dále pokud bylo stisknuto storno vrátí všechny změny
void	Cvektory::uprav_popisky_elementu(TObjekt *Objekt, TElement *Element)
{
	bool rename=false;//proměná sloužící k spouštění přejměnování
	if(Element!=NULL)//funkčnost při vložení elementu mezi ostatní, pouze název pořadové čísla byly již změněny
	{
		//úprava názvu pro roboty
//		if(1<=Element->eID && Element->eID<=4 || 7<=Element->eID && Element->eID<=18 || 101<=Element->eID && Element->eID<=108)
//		{
			Cvektory::TElement *E=Objekt->elementy->dalsi;//začíná se od začátku, někdy je potřeba ovlivnit i předchozí elementy
 			while (E!=NULL)
			{
				if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18 || 101<=E->eID && E->eID<=108)
				{
					if(E->name.SubString(1,6)=="Robot "/*&&E->name.Length()<=7*/||E->name=="")rename=true;else rename=false;
					//změna názvu
			  	if(rename)//přejmenování elementu ve spojáku + mGridu
					{
						int n=vrat_poradi_elementu_do(Objekt,E)+1;//zjistí pořadové číslo elementu
			  		//změna názvu v hlavičce mGridu, jako první z důvodu podmínky prázdného názvu
			  		if(E->name!=""&&E->mGrid!=NULL)//nutné, přejmenovávám i první element, který nemá vytvořený mGrid
			  		{
			  			E->mGrid->Cells[0][0].Text="<a>Robot "+AnsiString(n)+"</a>";
			  			E->mGrid->Cells[0][0].Font->Color=clBlack;//z důvodu nasazení odkazu, po přejmenování se text vrátil do modré barvy
			  			E->mGrid->MergeCells(0,0,1,0);//nutné kvůli správnému zobrazení hlavičky
			  			E->mGrid->Update();//musí zde být ošetření proti paměťové chybě
			  		}
			  		E->name="Robot "+AnsiString(n);
			  		E->short_name="Rob"+AnsiString(n);
					}
				}
				E=E->dalsi;//posun na další prvek
 			}
			E=NULL; delete E;
//  	}
//		else
//		{
			Cvektory::TObjekt *O=OBJEKTY->dalsi;//prěskočí hlavičku
			while(O!=NULL)//!!!!!!!!!!!!!dochází k zacyklení při přidání druhé stopky před první v druhé kabině!!!!!!!!!!!!!
			{
				if(O->n>=Objekt->n)//přeskakování objektů před aktuálním
				{
		 			Cvektory::TElement *E=O->elementy;//nepřeskakovat hlavičku
		 			if(O->n==Objekt->n)E=F->pom_temp->elementy;//při procházení aktuálního objektu nahradit pom_temp
		 			while(E!=NULL)
					{
		 				if(E->n>0)//přeskočí hlavičku
						{
		 					//kontrola zda můžu název změnit
							switch(E->eID)
		 					{
		 						case 0:if(E->name.SubString(1,5)=="Stop "&&E->name.Length()<=7||E->name=="")rename=true;else rename=false;break;
								default :rename=false;break;//musí zde být, jinak nějakým způsobem je pro robot rename nastaveno na true
								case 5:
								case 6:if(E->name.SubString(1,5)=="Otoč "&&E->name.Length()<=7||E->name=="")rename=true;else rename=false;break;
							}
		 					//nezměněn nebo nemá název -> mohu změnit
		 					if(rename)
							{
								int n=vrat_poradi_elementu_do(Objekt,E)+1;//zjistí pořadové číslo elementu
								//změna názvu v mGridu
								if(E->name!=""&&O->n==Objekt->n&&E->mGrid!=NULL)//nelze přistupovat k mGridu v případech nového elementu (nemá vytvořený), v neaktivní kabině (elementy nemají vytvořene mGridy)
								{
									if(E->eID==0)E->mGrid->Cells[0][0].Text="<a>Stop "+AnsiString(n)+"</a>";
									else E->mGrid->Cells[0][0].Text="<a>Otoč "+AnsiString(n)+"</a>";
									E->mGrid->Cells[0][0].Font->Color=clBlack;//z důvodu nasazení odkazu, po přejmenování se text vrátil do modré barvy
		 							E->mGrid->MergeCells(0,0,1,0);//nutné kvůli správnému zobrazení hlavičky
									E->mGrid->Update();//musí zde být ošetření proti paměťové chybě
//	   							if(E->eID==0)napln_combo_stopky(E);//pro budoucí použití
		 						}
		   					//změna názvu
		 						switch(E->eID)
		 						{
									case 0:E->name="Stop "+AnsiString(n);E->short_name="Sto"+AnsiString(n);break;
		 							case 5:
									case 6:E->name="Otoč "+AnsiString(n);E->short_name="Oto"+AnsiString(n);break;
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
//		}
	}  //storno funkcionalitu nutno podrobit dalším testům
	else//spuštěno při stisku tlačítka storno, musí dojít k přejmenování na původní, mění se název i pořadová čísla
	{
		Cvektory::TObjekt *O=OBJEKTY->dalsi;
		while(O!=NULL)
		{
			if(O->n>=Objekt->n)//přeskakování objektů před aktuálním
			{
				Cvektory::TElement *E=O->elementy;//nepřeskakovat hlavičku
				if(O->n==Objekt->n)E=F->pom->elementy;//když se jedná o aktuální objekt sáhnout do ostrého spojáku
	   		int n_spojak=1;//hodnota n pro seznam (element)
	   		while(E!=NULL)
	   		{
					if(E->n>0)
					{
						//kontrola zda má element původní název
	   				switch(E->eID)
	   				{
	   					case 0:if(E->name.SubString(1,5)=="Stop "&&E->name.Length()<=6)rename=true;else rename=false;break;
	   					case 1:
							case 2:
	   					case 3:
	   					case 4:if(E->name.SubString(1,6)=="Robot "&&E->name.Length()<=7)rename=true;else rename=false;break;
	   					case 5:
	   					case 6:if(E->name.SubString(1,5)=="Otoč "&&E->name.Length()<=6)rename=true;else rename=false;break;
	   				}
						//pokud má původní název -> přejmenovat
						if(rename)
						{
							int n_nazev=0;//n sloužící pro název, kolikáty element
							if(O->n==Objekt->n)n_nazev=vrat_poradi_elementu_do(Objekt,E)+1;
							else n_nazev=vrat_poradi_elementu_do(O,E)+1;//předávání ukazatele na plný objekt
							//změna názvu
							switch(E->eID)
							{
	   						case 0:E->name="Stop "+AnsiString(n_nazev);break;
								case 1:
								case 2:
								case 3:
	   						case 4:E->name="Robot "+AnsiString(n_nazev);break;
								case 5:
								case 6:E->name="Otoč "+AnsiString(n_nazev);break;
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
void  Cvektory::kopiruj_element(TElement *Original, TElement *Kopie)
{
	Kopie->n=Original->n;
	Kopie->eID=Original->eID;
	Kopie->short_name=Original->short_name;
	Kopie->name=Original->name;
	Kopie->X=Original->X;
	Kopie->Y=Original->Y;
	Kopie->Xt=Original->Xt;
	Kopie->Yt=Original->Yt;
	Kopie->rotace_symbolu=Original->rotace_symbolu;
	Kopie->rotace_jig=Original->rotace_jig;
	Kopie->stav=Original->stav;
	Kopie->LO1=Original->LO1;
	Kopie->OTOC_delka=Original->OTOC_delka;
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
	Kopie->poznamka=Original->poznamka;
	Kopie->sparovany=Original->sparovany;
}
////---------------------------------------------------------------------------
void Cvektory::kopiruj_elementy(TObjekt *Original, TObjekt  *Kopie)//zkopíruje elementy a jejich atributy bez ukazatelového propojení z objektu do objektu, pouze ukazatelové propojení na mGrid je zachováno spojuje dvě metody vloz_element(TObjekt *Objekt,TElement *Element) a kopiruj_element(TElement *Original, TElement *Kopie)
{
	TElement *E=Original->elementy;
	vytvor_elementarni_osu(Original,Kopie);//asi spíše provizorně
	if(E!=NULL)//pokud elementy existují nakopíruje je do pomocného spojáku pomocného objektu
	{
		E=E->dalsi;//přeskočí hlavičku
		while(E!=NULL)
		{
			TElement *Et=new TElement;
			kopiruj_element(E,Et);
			vloz_element(Kopie,Et);
			E=E->dalsi;//posun na další element
		}
	}
	//else vytvor_elementarni_osu(Original,Kopie);//pokud neexistují a jedná se o kopírování z pom do pom_temp, založí hlavičku, resp. v hlavičce vytvoří provizorní osu pohonu
	E=NULL;delete E;
}
////---------------------------------------------------------------------------
//připraví vektor provizorní osy pohonu
void Cvektory::vytvor_elementarni_osu(TObjekt *Original, TObjekt  *Kopie)
{
	if(Original==F->pom && Kopie==F->pom_temp)
	{
		hlavicka_elementy(Kopie);
		Kopie->elementy->geo.rotace=m.Rt90(F->d.trend(F->pom));
		Kopie->elementy->geo.typ=0;Kopie->elementy->X=0;Kopie->elementy->Y=0;Kopie->elementy->geo.delka=0;
		if(Kopie->elementy->geo.rotace==90 || Kopie->elementy->geo.rotace==270)Kopie->elementy->Y=Kopie->Y-Kopie->rozmer_kabiny.y/2.0;//vodorovná kabina
		else Kopie->elementy->X=Kopie->X+Kopie->rozmer_kabiny.x/2.0;//svislá
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
	unsigned int r_pocet=0,s_pocet=0,o_pocet=0,t_pocet=0;//nastavení všech počtů na nulu
	if(Element->eID!=6&&Element->eID!=5&&Element->eID!=0)//pokud je Element robot projdi roboty v Objektu
	{
		Cvektory::TElement *E=Objekt->elementy->dalsi;//přeskočí hlavičku
		while(E->n!=Element->n)
		{
			if(1<=E->eID && E->eID<=4 || 7<=E->eID && E->eID<=18 || 101<=E->eID && E->eID<=108)r_pocet++;
			if(E->eID==100)t_pocet++;
			E=E->dalsi;
		}
		E=NULL; delete E;
	}
	else//když se nejedná o robota projde všechny elementy ve všech objektech
	{
		//procházení předchozích objektů
		Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
		while(O!=NULL)
		{
			Cvektory::TElement *E=O->elementy;//nastaveno na hlavičku, ošetřeno níže
			if(F->pom_temp->n==O->n)if(F->pom_temp!=NULL)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
			while(E!=NULL)
			{
				if(O->n==Objekt->n&&E==Element)break;//ukončení prohledávání když jsem na aktuálním elmentu
				if(E->n>0)//ošetření pro hlavičku
				{
					if(E->eID==0)s_pocet++;
					if(E->eID==5||E->eID==6)o_pocet++;
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
		case 0: return s_pocet;break;
		case 5: case 6: return o_pocet;break;
		case 100: return t_pocet;break;
		default: return r_pocet;break;
	}
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
				if(ret<E->mGrid->ID)ret=E->mGrid->ID;
      }
			E=E->dalsi;
		}
		E=NULL;delete E;
  }
	return ret;
}
////---------------------------------------------------------------------------
//orotuje všechny elementy daného objektu o danou hodnotu
void Cvektory::rotace_elementu(TObjekt *Objekt,short rotace)
{
	TElement *E=Objekt->elementy->dalsi;//přeskočí rovnou hlavičku
	while(E!=NULL)
	{
		if(E->rotace_symbolu+rotace>360)E->rotace_symbolu=0;//kvůli přetýkání na dvě podmínky
		E->rotace_symbolu+=rotace;
		if(E->rotace_symbolu==360)E->rotace_symbolu=0;
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
			short rotace=E->rotace_symbolu;
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
	TElement *E=Objekt->elementy;//NEPŘESKAKOVAT hlavičku!!! kvůli ošetření ohledně existence elementu v objektu
	while(E!=NULL)
	{
		if(E->mGrid!=NULL)//ošetření proti neexistující tabulce
		{
			if(E->Xt<=X && X<=E->Xt+E->mGrid->Width*F->m2px/F->Zoom && E->Yt>=Y && Y>=E->Yt-E->mGrid->Height*F->m2px/F->Zoom)break;
			else E=E->dalsi;
		}
		else E=E->dalsi;
	}
	return E;
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
	TElement *E=Objekt->elementy;//NEPŘESKAKOVAT hlavičku!!!
	while(E!=NULL)
	{
		if(E->citelna_oblast.rect1.PtInRect(TPoint(X,Y))){RET=1;F->pom_element=E;break;}//hodnoty kóty
		else
		{
			if(E->citelna_oblast.rect2.PtInRect(TPoint(X,Y))){RET=2;F->pom_element=E;break;}//jednotky kóty
			else if(E->citelna_oblast.rect0.PtInRect(TPoint(X,Y))){RET=0;F->pom_element=E;break;}//kóta celá
		}
		E=E->dalsi;
	}
	E=NULL;delete E;
	return RET;
}
////---------------------------------------------------------------------------
//posune pouze Element z pomocného spojového seznamu pom_temp na parametrem uvedenou vzádlenost (v metrech) od elementu předchozího, pokud je implicitní hodnota pusun_dalsich_elementu false změněna na true, jsou o danou změnu posunu přesunuty i elementy následující Elementu (tudíž jejich vzdálenost od Elementu bude zachována, naopak v případě výchozí hodnoty false je následujícím/dalším elementům poloha zachována)
bool Cvektory::posun_element(TElement *Element,double vzdalenost,bool pusun_dalsich_elementu,bool posun_kurzorem)
{ //!!!!!!po nasazení geometrie nutno zdokonalit, nebude se pracovát pouze se vzdálenosti na linii buď vodorvné či svislé, ale i v oblouku
	bool RET=false;
	if(F->pom_temp!=NULL && F->pom_temp->elementy!=NULL/*&&F->Akce!=F->MOVE_ELEMENT*/)//raději ošetření, ač by se metoda měla volat jen v případě existence pom_temp
	{
	//použít makro F->d.Rxy(uvažovaný element), m.delka() vrací pouze abs. hodnotu

//		if(F->pom_temp->elementy->dalsi!=NULL)//musí existovat alespoň jeden element
//		{
//			TPointD vzd;
//			if(F->pom_temp->elementy->predchozi->n==1)//pokud existuje jenom jeden element
//			{               ///ještě vylepšít, provizorně jen pro vodorovnou levopravou kabinu
//				vzd.x=Element->X-F->pom_temp->Xk;
//				vzd.y=Element->Y-F->pom_temp->Yk;
//			}
//			else//více elementů
//			{                     použít makro F->d.Rxy(uvažovaný element), m.delka() vrací pouze abs. hodnotu
//				vzd.x=Element->X-Element->predchozi->X;
//				vzd.y=Element->Y-Element->predchozi->Y;
//			}
//
//			if(vzd.x!=0)Element->X-=-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
//			if(vzd.y!=0)Element->Y-=-(vzd.y/m.abs_d(vzd.y))*(m.abs_d(vzd.y)-vzdalenost);
//
//			//kontrola zda se vejdou stále všechny elementy do objektu - dodělat
//			//RET=
//		}
						 //dodělat MaVl
		//provizorně jen pro vodorovnou levopravou kabinu
		if(F->pom_temp->elementy->dalsi!=NULL&&vzdalenost!=0)//musí existovat alespoň jeden element&&nesmí být vzdálenost rovna nule
		{
			TPointD vzd;
			if(Element->n==1)//pro první element, od počátku kabiny
			{
				if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)vzd.x=Element->X-F->pom_temp->Xk;
				else vzd.x=Element->Y-F->pom_temp->Yk;
			}
			else//více elementů
			{
				if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)vzd.x=Element->X-Element->predchozi->X;
				else	vzd.x=Element->Y-Element->predchozi->Y;
			}//odstavil MaKr F->Sv(vzd.x);
			if(vzd.x!=0&&!posun_kurzorem)//posun z kót
			{
				if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)Element->X=Element->X-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
				else Element->Y=Element->Y-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
				if(Element->dalsi!=NULL&&!pusun_dalsich_elementu){posuv_aktualizace_RT(Element);posuv_aktualizace_RT(Element->dalsi);}//při změně vzdálenosti je nutno dopočítat znova RT, pokud je za robotem další robot jeho RT musí být také přepočítáno
				else posuv_aktualizace_RT(Element);
			}
			else if(posun_kurzorem)//posun kurozem
			{
				if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)Element->X=Element->X+vzdalenost;
				else	Element->Y=Element->Y+vzdalenost;
				if(Element->dalsi!=NULL&&!pusun_dalsich_elementu){posuv_aktualizace_RT(Element);posuv_aktualizace_RT(Element->dalsi);}//při změně vzdálenosti je nutno dopočítat znova RT, pokud je za robotem další robot jeho RT musí být také přepočítáno
				else posuv_aktualizace_RT(Element);
			}

			//v případě požadavku na posun i následujících elementů
			if(pusun_dalsich_elementu)
			{
				TElement *E=Element->dalsi;
				while(E!=NULL)
				{
					if(vzd.x!=0&&!posun_kurzorem)
					{
						if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)E->X=E->X-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);//výpočet pro posuv z kót
						else E->Y=E->Y-(vzd.x/m.abs_d(vzd.x))*(m.abs_d(vzd.x)-vzdalenost);
					}
					if(posun_kurzorem)
					{
						if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)E->X=E->X+vzdalenost;//výpočet pro posun kurzorem
						else E->Y=E->Y+vzdalenost;
					}
					E=E->dalsi;
				}
				E=NULL;delete E;
			}
			// případně ještě kontrola zda se vejdou stále všechny elementy do objektu možno použít MaVl metodu z Unit1
			//RET=
		}
	}
	Cvektory::TElement *E=vloz_element_za(F->pom_temp,Element);
	if(E!=NULL&&E->n!=Element->n)zmen_poradi_elementu(Element,E);
	E=NULL; delete E;
	return RET;
}
////---------------------------------------------------------------------------
//posunem elementu tj. změnou vzdálenosti od předchozího se ovlivní hodnota RT, nutno přepočítat
void Cvektory::posuv_aktualizace_RT(TElement *Element)
{
	switch(Element->eID)
	{
		case 2:case 8:case 12:case 16:case 102:case 106://roboti se stop stanicí
		{
			Element->RT=F->m.RT(Element->PT1,vzdalenost_od_predchoziho_elementu(Element,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,Element->WT);
			Element->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(Element->RT),3);
			Element->mGrid->Cells[1][2].Highlight=true;
			Element->mGrid->Refresh();
		}break;
		case 4:case 10:case 14:case 18:case 104:case 108://roboti s aktivní otočí
		{
			Element->RT=m.RT(Element->PT1+Element->PT2+Element->PTotoc,vzdalenost_od_predchoziho_elementu(Element,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,Element->WT);
			Element->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(Element->RT),3);
			Element->mGrid->Cells[1][5].Highlight=true;
			Element->mGrid->Refresh();
		}break;
		case 6://aktivní otoč
		{
			Element->RT=m.RT(Element->PTotoc,vzdalenost_od_predchoziho_elementu(Element,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,Element->WT);
			Element->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(Element->RT),3);
			Element->mGrid->Cells[1][4].Highlight=true;
			Element->mGrid->Refresh();
		}break;
		case 0://stop stanice
		{
			Element->RT=m.RT(0,vzdalenost_od_predchoziho_elementu(Element,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,Element->WT+Element->WTstop);
			Element->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(Element->RT),3);
			Element->mGrid->Cells[1][2].Highlight=true;
			Element->mGrid->Refresh();
		}break;
	}
}
////---------------------------------------------------------------------------
//řeší změnu pořadí při posuvu elementů, dojde k novému ukazatelovému propojení, přejmenování a přeindexování elementů
void Cvektory::zmen_poradi_elementu(TElement *aktualni_poradi,TElement *nove_poradi)
{
	TElement *ukaz_ap=aktualni_poradi;
	TElement *ukaz_np=nove_poradi;//musí být tu, aby byl uložen aktuální seznam, ne po vyjmutí
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
			F->pom_temp->elementy->predchozi=ukaz_ap->predchozi;
			ukaz_ap->predchozi->dalsi=NULL;
		}
		//ukazatelové propojení
		ukaz_ap->dalsi=ukaz_np->dalsi;
		ukaz_ap->predchozi=ukaz_np;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		ukaz_np->dalsi->predchozi=ukaz_ap;
		ukaz_np->dalsi=ukaz_ap;
		/////konec testu
	}
	//přeindexování (N-hodnoty) v celém seznamu, možno řešit sepáratáně, ale takto to bylo rychleji napsané
	TElement *E=F->pom_temp->elementy->dalsi;//ukazatel na první element, přeskočí hlavičku, metoda volaná jen v případě, že existují min. 2 elementy
	int n=1;
	while (E!=NULL)
	{
		E->n=n;
//		if(E->name!="")//nutné, přeindexovává se i první element, který nemá vytvořený mGrid
//		{
//			E->mGrid->ID=E->n;//F->Sv(E->name);
//			E->mGrid->Show(NULL);//musí zde být Show, Update() dělal problémy
//		}
		n++;
		E=E->dalsi;
	}
	E=NULL; delete E;
	ukaz_ap=NULL; delete ukaz_ap;
	ukaz_np=NULL; delete ukaz_np;
	uprav_popisky_elementu(F->pom_temp,aktualni_poradi);//změna názvů
}
////---------------------------------------------------------------------------
//vratí vzdálenost od předchozího elementu, pracuje zatím pouze v orotogonalizovaném prostoru (bude nutno vylepšit s příchodem oblouků), pokud se jedná o první element, uvažuje se jako vzdálenost od počátku kabiny (nutno vylepšit ještě pro různé orientace kabiny)
double Cvektory::vzdalenost_od_predchoziho_elementu(TElement *Element,bool pouzeSG)
{                  //dodělat MaVl
	if(pouzeSG)//vzdálenost od předchozího SG elementu, slouží k výpočtům RT, zatím provizorně, v budoucnu nutno rozšířit na předchozí kabiny + vyhybky!!!!
	{
		double celkem=0;
		//pokud je element první v kabině
		if(Element->n==1)
		{
			if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)return m.delka(F->pom_temp->Xk,F->pom_temp->elementy->Y,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
			else return m.delka(F->pom_temp->elementy->X,F->pom_temp->Yk,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
		}
		else//pokud je v kabině více elementů
		{
	  	//procházení objektu a hledání předchozího SG elementu
	  	Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//provizorně může být použito pom_temp, volání metody pouze když je pom_temp naplněné
	  	while(E->n!=Element->n&&E!=NULL)
			{
				//procházím kabinu od začátku, pokud je element SG uložím jeho vzdálenost k elementu pro kterého hledám vzdálenost k předchozímu
				if(E->eID==0||E->eID%2==0)celkem=m.delka(F->d.Rxy(Element).x,F->d.Rxy(Element).y,F->d.Rxy(E).x,F->d.Rxy(E).y);
	  		E=E->dalsi;
			}
			E=NULL; delete E;
			//pokud byla nalezena alespoň jedna vzdálenost
			if(celkem!=0)return celkem;
			else return m.delka(F->pom_temp->Xk,F->pom_temp->Yk-F->pom_temp->rozmer_kabiny.y/2.0,F->d.Rxy(Element).x,F->d.Rxy(Element).y);
		}
	}
	else//////Původní funkce
	{
  	TPointD E=F->d.Rxy(Element);
  	if(Element->n==1)//pro první element od hrany kabiny
  	{                ///ještě vylepšít, provizorně jen pro vodorovnou levopravou kabinu
			if(Element->rotace_symbolu==0||Element->rotace_symbolu==180)
				return m.delka(F->pom_temp->Xk,F->pom_temp->elementy->Y,E.x,E.y);//(bude nutno ještě vylepšit s příchodem oblouků)!!!
			else
				return m.delka(F->pom_temp->elementy->X,F->pom_temp->Yk,E.x,E.y);//(bude nutno ještě vylepšit s příchodem oblouků)!!!
  	}
  	else//mezi elementy
  	{
  		TPointD Ep=F->d.Rxy(Element->predchozi);
  		return m.delka(E.x,E.y,Ep.x,Ep.y); //(bude nutno vylepšit s příchodem oblouků)!!!
		}
  }
}
////---------------------------------------------------------------------------
//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem
double Cvektory::vrat_rotaci_jigu_po_predchazejicim_elementu(TObjekt *Objekt,TElement *Element)
{
	bool nalezeno=false;
	double akt_rotoce_jigu=0;
	//provizorní (průchod po objektech) do doby než budou spuštěny zakázky resp. výhybky
	Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		TElement *E=Element->dalsi;
		while(E!=NULL)
		{
			if(Element->n==E->n && Objekt->n==O->n)//pozor nelze porovnávat jen ukazatele, může docházet k porování nepřímých kopii (viz pom_temp)
			{
				nalezeno=true;break;//akcelerátor, skončí cyklus
			}
			else//stále předcházející elementy, ty mě pro návrátovou hodnotu zajímají, rotace aktuálního elementu se nezohledňuje
			{
				if(3<=E->eID && E->eID<=6)akt_rotoce_jigu+=E->rotace_jig;
      }
			E=E->dalsi;
		}
		E=NULL;delete E;
		if(nalezeno)break;//akcelerátor, skončí cyklus
		O=O->dalsi;//posun na další prvek
	}
	O=NULL;delete O;

  //ošetření přetečení přes 360°
	akt_rotoce_jigu=fmod(akt_rotoce_jigu,360.0);// včetně ošetření přetečení přes 360 stupňů
	if(akt_rotoce_jigu<0){akt_rotoce_jigu+=360;}//pro záporné hodnoty

	return akt_rotoce_jigu;
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
					if((E->eID%2==0) && E!=Stopka && E->n!=0)
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
Cvektory::TElement *Cvektory::vrat_predchozi_stop_element(TElement *Stopka)
{
	TElement *RET=NULL;
	if(Stopka->eID==0)
	{
		bool konec=false;
		Cvektory::TObjekt *O=OBJEKTY->dalsi;//přeskočí hlavičku
		while (O!=NULL)//prochází všechny objekty
		{
			TElement *E=O->elementy;//nepřeskakovat hlavičku
			if(F->pom_temp->n==O->n)E=F->pom_temp->elementy;//pokud se prochází objekt aktuálně editovaný, tak se vezme z pom_temp, kde jsou aktuální hodnoty
			while(E!=NULL)//a jejich elementy
			{
				////pokud je aktuální element stopka či robot se stopkou a nejedná se o hlavičku
				if((E->eID==0 || E->eID==4 || E->eID==6) && E->n!=0)
				{                  //toto je otazník
					 if(E==Stopka && RET!=NULL){konec=true;break;}//pokud se došlu až k aktuální stopce a již byla stop element nalezen,vyhledávání se ukončí a vrací se to, co je uložené v E resp. RET
					 else RET=E;//pokud ne, uloží
				}
				E=E->dalsi;
			}
			E=NULL;delete E;
			if(konec)break;
			O=O->dalsi;//posun na další prvek
		}
		O=NULL;delete O;
	}
	return RET;
}
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
	Element->mGrid->Delete();

	//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
	if(Element->dalsi!=NULL)//ošetření proti poslednímu prvku
	{
		Element->predchozi->dalsi=Element->dalsi;
		Element->dalsi->predchozi=Element->predchozi;
	}
	else//poslední prvek
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
	TPohon *novy=new TPohon;
	novy->n=0;
	novy->name="";
	novy->rychlost_od=0;
	novy->rychlost_do=0;
	novy->aRD=0;
	novy->roztec=0;
	novy->Rz=0;
	novy->Rx=0;

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	POHONY=novy;
}
////---------------------------------------------------------------------------
//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
void Cvektory::vloz_pohon(TPohon *pohon)
{
	TPohon *novy=new TPohon;

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
	TPohon *novy=new TPohon;
	novy->name=name;
	novy->rychlost_od=rychlost_od;
	novy->rychlost_do=rychlost_do;
	novy->aRD=aRD;
	novy->roztec=R;
	novy->Rz=Rz;
	novy->Rx=Rx;
	vloz_pohon(novy);
}
////---------------------------------------------------------------------------
//vrátí ukazatel na pohon dle n pohonu
Cvektory::TPohon *Cvektory::vrat_pohon(unsigned long n)
{
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
	if(Pohon!=NULL)
	{
		if(Objekt->pohon==NULL)Objekt->pohon=new TPohon;
		*Objekt->pohon=*Pohon;//překopírování hodnot pohonů bez spojového propojení s originálem, ale i přes další a předchozí není zoohledněno propojení se daným spojovým seznamem pohonů, daný pohon je pouze datovou součástí pom_temp, není samostatným objektem či objektem zařazeným ve spojáku pohonů
		if(Objekt==F->pom)*vrat_pohon(Pohon->n)=*Pohon;//situace překopírování z pomocného do ostrého (ukládání náhledu), aby bylo zachováno spojové propojení seznamu pohonů i pro případ nepřiřazeno
	}
	else Objekt->pohon=NULL;
}
////---------------------------------------------------------------------------
//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
bool Cvektory::pohon_je_pouzivan(unsigned long n)
{
	TObjekt *O=OBJEKTY->dalsi;
	bool nalezen=false;
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
	O=NULL;delete O;
	return nalezen;
}
////---------------------------------------------------------------------------
//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv, ten vrátí formou ukazatale na první nalezený používáný, druhý vstupní parametr metody TObjekt mimo_objekt je ukazatel na objekt, který se bude při vyhledávání ignorovat, nenajde-li vrací NULL, třetí parametr, pokud je náchán na implicitní -1 řeší se pro všechny režimy, pokud je v rozmezí 0 až 2 řeší se pro konkrétní režim
Cvektory::TObjekt *Cvektory::pohon_je_pouzivan(unsigned long n,TObjekt *mimo_objekt,short rezim)
{
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
//metodu nepoužíváme, proto je zakomentovaná
//vygeneruje ve statusu NÁVRH seznam doprvníků dle použitého CT objektu a /zároveň tomuto objektu tento pohon přiřadí - nepoužíváme/, obsahuje ošetření proti duplicitě
void Cvektory::generuj_POHONY()
{
//	TObjekt *O=OBJEKTY->dalsi;
//	int i=0;//i vygenerovaného pohonu
//	//prvně najde "i" nejvýššího dříve navrženého pohonu (který se generoval v jiném zobrazení formuláře)
//	TPohon *P=POHONY->dalsi;
//	while(P!=NULL)
//	{
//		 if(P->name.Pos("Navržený pohon "))
//		 {
//			unsigned int i_potencial=Form1->ms.a2i(Form1->ms.TrimLeftFromText(P->name,"ý pohon "));
//			if(i_potencial>i)i=i_potencial;
//		 }
//		 P=P->dalsi;//posun na další prvek
//	}
//
//	while (O!=NULL)
//	{
//		if(O->RD>0)//vypisuje pouze pokud je rychlost dopravníku nenulová,nulové pohony (tj. z režimu S&G a post-procesní) nezohledňuje
//		{
//			TPohon *P=POHONY->dalsi;
//			bool pohon_nenalezen=true;
//			while(P!=NULL)
//			{
//				 if(P->rychlost_od==O->RD && P->rychlost_do==O->RD && P->roztec==1620.0)//byl-li pohon se stejnými parametry nalezen
//				 {
//						pohon_nenalezen=false;//tzn. že již neplatí, že nebyl nenelezen, byl naopak nalezen se stejnými parametry, takže se nebude přidávat, protože by se jednalo o duplicitu
//						//již nepoužíváme O->pohon=P;//přiřazení pohonu k danému objektu
//						break;//nalezen tak se může jít ověřovat další objekt
//				 }
//				 P=P->dalsi;//posun na další prvek
//			}
//			//byl-li předchozí konstrukcí pohon nenanlezen přidá, musí být až po dokončení while(P
//			if(pohon_nenalezen)
//			{
//				vloz_pohon("Navržený pohon "+AnsiString(++i),O->RD,O->RD,1620.0);
//				//již nepoužíváme O->pohon=POHONY->predchozi;
//			}
//		}
//		O=O->dalsi;//posun na další prvek
//	}
}
////---------------------------------------------------------------------------
//navrhne pohony zobrazené v parametrech linky, vrátí řetězec oddělený seperátorem, pouze jako seznam unikátních použitých rychlostí, lze nastavit jednotky zobrazení rychlosti pohonu, implicintě m/min
//řeší pouze pro objekty bez přiřazených pohonů
//umí řešit i pro aktuální PO parametry
AnsiString Cvektory::navrhni_POHONY(AnsiString separator,short m_min)
{
	AnsiString data="";
	TObjekt *O=OBJEKTY->dalsi;
	double *pole_rychlosti=new double[OBJEKTY->predchozi->n];//dynamické pole unikátních rychlostí, pole je o max. velikosti počtu objektů
	for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)pole_rychlosti[j]=0;//vynulování pole
	AnsiString *pole_pohonu=new AnsiString[OBJEKTY->predchozi->n];//dynamické pole unikátních pohonu, pole je o max. velikosti počtu objektů
	for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)pole_pohonu[j]="";//vynulování pole
	double *pole_rozteci=new double[OBJEKTY->predchozi->n];//dynamické pole unikátních rychlostí, pole je o max. velikosti počtu objektů
	for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)pole_rozteci[j]=0;//vynulování pole

	TPohon *P=POHONY->dalsi;
	//projíždí jedntolivé objekty, které nemají přiřařezen pohon, tak jim doporučí, s tím, že navrhuje sloučit se stejnou rychlostí
	while (O!=NULL)
	{
		//hodnoty z procházeného objektu
		double RD=O->RD;
		bool pohon_prirazen=false;if(O->pohon!=NULL)pohon_prirazen=true;
		AnsiString short_name=O->short_name;

		//pokud dochází k volání z PO, tzn. je třeba zohlednti aktulně zadaná data z jednotlivých editů
		//tak se převezmetou tyto hodnoty
		if(F->pom!=NULL)
		{   //a pokud se jedná o stejný objekt, jako právě projížděný cyklemmusí být samostatně
				if(O==F->pom && Form_parametry->scComboBox_pohon->ItemIndex==0)//a nemá přiřazen pohon
				{
					pohon_prirazen=false;
					short_name=Form_parametry->scGPEdit_shortname->Text;
					RD=Form_parametry->scGPNumericEdit_RD->Value; // RD	od uživatele
					if(Form_parametry->RDunitT == Form_parametry->MIN)RD /= 60.0;//převod jednotek
				}
		}

		//řeší pouze pro objekty bez přiřazených pohonů (ty jsou již definovatelné) a zároveň pokud je rychlost dopravníku nenulová
    //zvažit zda nepřeskakovat již navržené stažené do správy pohonů, takto se duplikují ve správě pohonu (ve stringridu po přidání)
		if(pohon_prirazen==false && RD>0)
		{
			for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)
			{
				if(pole_rychlosti[j]==RD)//RD je již v poli, užívá ho jiný objekt
				{
					pole_pohonu[j]+=", "+short_name;
					break;
				}
				if(pole_rychlosti[j]==0)//neni, přídání nově do všech třech polí
				{
					if(m_min)pole_pohonu[j]="Navržený pohon s rychlostí "+AnsiString(RD*60)+" [m/min] pro objekt(y): "+short_name;//v m/min
					else pole_pohonu[j]="Navržený pohon s rychlostí "+AnsiString(RD)+" [m/s] pro objekt(y): "+short_name;//v m/s
					pole_rychlosti[j]=RD;
					pole_rozteci[j]=m.Rz(RD);
					break;
				}
			}
		}
		O=O->dalsi;//posun na další prvek
	}

	//překopíruje pole_pohonu do dat k navrácení, pokud není záznam prázdný
	for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)
	{
		if(pole_pohonu[j]!="")data+=pole_pohonu[j]+".";
		if(pole_rozteci[j]!=0)
		{
		 AnsiString CH=vypis_retezy_s_pouzitelnou_rozteci(pole_rozteci[j],"",",");
		 if(CH!="")data+=" Použ. řetezy s roztečí [m]:"+CH;
		}
		if(pole_pohonu[j]!="")data+=separator;
	}

	//odstranění již nepotřebných dat z paměti
	delete [] pole_rychlosti;
	delete [] pole_pohonu;
	delete [] pole_rozteci;
	O=NULL;delete O;
	P=NULL;delete P;

	return data;
}
//zaloha old provedení
//AnsiString Cvektory::navrhni_POHONY(AnsiString separator)
//{
//	AnsiString data="";
//	TObjekt *O=OBJEKTY->dalsi;
//	double *pole_rychlosti=new double[OBJEKTY->predchozi->n];//dynamické pole unikátních rychlostí, pole je  o max. velikosti počtu objektů
//	for(unsigned int j=0;j<OBJEKTY->predchozi->n;j++)pole_rychlosti[j]=0;//vynulování pole
//	unsigned int i=0;//i vygenerovaného pohonu
//	//prvně najde "i" v názvu nejvýššího dříve navrženého pohonu (který se generoval v jiném zobrazení formuláře)
//	TPohon *P=POHONY->dalsi;
//	while(P!=NULL)
//	{
//		 if(P->name.Pos("Navržený pohon "))
//		 {
//			unsigned int i_potencial=Form1->ms.a2i(Form1->ms.TrimLeftFromText(P->name,"ý pohon "));
//			if(i_potencial>i)i=i_potencial;
//		 }
//		 P=P->dalsi;//posun na další prvek
//	}
//
//	while (O!=NULL)
//	{
//		if(O->RD>0)//vypisuje pouze pokud je rychlost dopravníku nenulová,nulové pohony (tj. z režimu S&G a post-procesní - to je již zavádějící, i v těchto režimech) nezohledňuje
//		{
//			bool nalezen=false;
//			for(unsigned int j=0;j<O->n;j++)//zajištění UNIKATNOSTI, kontroluje pole unikátních rychlosti
//			{
//				if(pole_rychlosti[j]==O->RD)//shodný nalezen
//				{
//					nalezen=true;
//					//break;//přeruší další zbytečné vyhledávání ve for, může se přejít na další objekt a tedy potenciální rychlost
//				}
//			}
//			if(!nalezen)//pokud nebyla rychlost nalezena, tak vypíše a uloží ji do pole_rychlostí kvůli kontrole dalšího prvku//zajištění UNIKATNOSTI
//			{
//				P=POHONY->dalsi;
//				while(P!=NULL)//ještě kontroluje zda již dříve nebyl uložen stejný pohon v navržených pohonech
//				{
//						 if(P->name.Pos("Navržený pohon ") && P->rychlost_od==O->RD && P->rychlost_do==O->RD && P->aRD==O->RD)//byl-li pohon se stejnými parametry nalezen
//						 nalezen=true;
//						 P=P->dalsi;//posun na další prvek
//				}
//				if(!nalezen)//pokud stále platí, že nebyl nalezen
//				{ //libovolný			  //html tabulka                   //csv
//					AnsiString mS=", ";if(separator=="</tr>")mS="</td>";if(separator!="</tr>" && separator!="</br>")mS=";";
//					//název, rychlost, RZ
//					if(mS==", ")		data+="Navržený pohon "+AnsiString(++i)+mS+AnsiString(O->RD*60)+" [m/min]"+mS+AnsiString(Form1->m.Rz(O->RD))+" [m]";//LIBOVOLNÉ
//					if(mS==";")			data+="Navržený pohon "+AnsiString(++i)+mS+AnsiString(O->RD*60)+mS+AnsiString(Form1->m.Rz(O->RD));//CSV
//					if(mS=="</td>")	data+="<tr><th scope=\"row\">Navržený pohon "+AnsiString(++i)+"</th><td>"+AnsiString(O->RD*60)+mS+"<td>"+AnsiString(Form1->m.Rz(O->RD))+mS;//HTML TABLE
//					data+=separator;
//					pole_rychlosti[O->n-1]=O->RD;
//				}                //indexuje se v poly od nuly ale objekty jsou indexované od 1
//			}
//		}
//		O=O->dalsi;//posun na další prvek
//	}
//	delete [] pole_rychlosti;
//	delete O;
//	delete P;
//	return data;
//}

////---------------------------------------------------------------------------
//smaze body z pameti
long Cvektory::vymaz_seznam_POHONY()
{
	long pocet_smazanych_pohonu=0;
	while (POHONY!=NULL)
	{
		pocet_smazanych_pohonu++;
		POHONY->predchozi=NULL;
		delete POHONY->predchozi;
		POHONY=POHONY->dalsi;
	};

	return pocet_smazanych_pohonu;
};
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
		 if(m.UDV(O[i].rotace)>minRz)minRz=m.UDV(O[i].rotace);//najde minimální nutný rozestup
		 if(Rz<m.UDV(O[i].rotace))//pokud platí, nastal problém, vozíky se nevejdeou
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
		S->objekt=O;S->CT=O->CT;S->RD=O->RD;S->Rotace=O->rotace;S->Tc=0;S->Tv=0;S->Opak=0;
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
////vytvoří novou hlavičku pro řetězy
void Cvektory::hlavicka_RETEZY()
{
	TRetez *novy=new TRetez;
	novy->n=0;
	novy->name="";//celý název objektu

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	RETEZY=novy;//RETEZY
}
////---------------------------------------------------------------------------
////uloží retez a jeho parametry do spojového seznamu
void Cvektory::vloz_retez(AnsiString name, double roztec)
{
	TRetez *novy=new TRetez;

	novy->n=RETEZY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->name=name;
	novy->roztec=roztec;

	RETEZY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=RETEZY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;
	RETEZY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//z položky (předpoklad vybrané) v comboboxů řetězů vrátí pouze hodnotu rozteče
double Cvektory::vrat_roztec_retezu_z_item(AnsiString item,AnsiString separator)
{
	return Form1->ms.MyToDouble(Form1->ms.TrimLeftFromText(item,separator));
}
//---------------------------------------------------------------------------
//vypíše všechny použitelné řetezy použitelné pro zadané rozmezí dle užité rozteče, separátor odděluje název řetězu od rozteče, totál separátor jednotlivé řetězy, pokud je Rz zadané nulové vrátí hodnotu nula, pokud chci vypsat všechny načtené řetězy ze souboru retezy.csv použiji parametr Rz=-1, pokud není požadován výpis názvu řetězu použiji prázdné uvozovky
AnsiString Cvektory::vypis_retezy_s_pouzitelnou_rozteci(double Rz,AnsiString separator,AnsiString total_separator,bool mm)
{
	AnsiString RET="";
	short J=1.0;if(mm)J=1000.0;
	if(Rz)
	{
			TRetez *CH=RETEZY->dalsi;
			while(CH!=NULL)
			{                                 //pokud chci vypsat vše
				if(m.mod_d(Rz,CH->roztec)==0 || Rz==-1)//zbytek po dělení je nula, tzn. vhodný řetěz s roztečí vhodnou pro požadovaný rozestup nalezen nebo -1 pokud chci vypsat všechny načtené řetězy ze souboru retezy.csv
				{
					if(separator=="")RET+=AnsiString(J*CH->roztec)+total_separator;//pokud není požadován výpis názvu řetězu
					else RET+=CH->name+separator+AnsiString(J*CH->roztec)+total_separator;
				}
				CH=CH->dalsi;
			}
			CH=NULL;delete CH;
	}
	return RET;
}
//---------------------------------------------------------------------------
//smaze RETEZY z pameti
long Cvektory::vymaz_seznam_RETEZY()
{
	long pocet_smazanych_objektu=0;
	while (RETEZY!=NULL)
	{
		pocet_smazanych_objektu++;
		RETEZY->predchozi=NULL;
		delete RETEZY->predchozi;
		RETEZY=RETEZY->dalsi;
	};

	return pocet_smazanych_objektu;
};
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
		File_hlavicka.typ_vozik=PP.typ_voziku;
		File_hlavicka.delka_jig=PP.delka_jig;
		File_hlavicka.sirka_jig=PP.sirka_jig;
		File_hlavicka.vyska_jig=PP.vyska_jig;
		File_hlavicka.delka_podvozek=PP.delka_podvozek;
		//objektové záležitosti
		File_hlavicka.pocet_pohonu=POHONY->predchozi->n;
		File_hlavicka.pocet_objektu=OBJEKTY->predchozi->n;
		File_hlavicka.pocet_zakazek=ZAKAZKY->predchozi->n;
		File_hlavicka.pocet_voziku=VOZIKY->predchozi->n;
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

		 //uložení parametrů rastru
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

		 //uložení pohonu
		 TPohon *ukaz1=POHONY->dalsi;
		 while (ukaz1!=NULL)
		 {
			 ////překopírování dat do pomocného objektu uložitelného do bináru
			 C_pohon *c_ukaz1=new C_pohon;

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

		 //uložení seznamu technologických objektů
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
					c_ukaz->rezim=ukaz->rezim;
					c_ukaz->CT=ukaz->CT;
					c_ukaz->RD=ukaz->RD;
					c_ukaz->kapacita_dop=ukaz->kapacita_dop;
					c_ukaz->kapacita=ukaz->kapacita;
					c_ukaz->pozice=ukaz->pozice;
					c_ukaz->rotace=ukaz->rotace;
					c_ukaz->mezera=ukaz->mezera;
					c_ukaz->mezera_jig=ukaz->mezera_jig;
					c_ukaz->mezera_podvozek=ukaz->mezera_podvozek;
					if(ukaz->pohon!=NULL)c_ukaz->pohon=ukaz->pohon->n;
					else c_ukaz->pohon=0;
					c_ukaz->delka_dopravniku=ukaz->delka_dopravniku;
					c_ukaz->cekat_na_palce=ukaz->cekat_na_palce;
					c_ukaz->stopka=ukaz->stopka;
					c_ukaz->odchylka=ukaz->odchylka;
					c_ukaz->CT_zamek=ukaz->CT_zamek;
					c_ukaz->RD_zamek=ukaz->RD_zamek;
					c_ukaz->DD_zamek=ukaz->DD_zamek;
					c_ukaz->K_zamek=ukaz->K_zamek;
					c_ukaz->text_length=ukaz->name.Length()+1;
					c_ukaz->poznamka_length=ukaz->poznamka.Length()+1;
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
					//text - poznámka
					wchar_t *poznamka=new wchar_t [c_ukaz->poznamka_length];
					poznamka=ukaz->poznamka.c_str();
					FileStream->Write(poznamka,c_ukaz->poznamka_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
					poznamka=NULL; delete[] poznamka;
			 }
			 c_ukaz=NULL;delete c_ukaz;
			 ukaz=ukaz->dalsi;//posunutí na další pozici v seznamu
		 };
		 ukaz=NULL;delete ukaz;

		 //uložení zakázek
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

//ZDM
//		 //uložení vozíků
//		 TVozik *ukaz1=VOZIKY->dalsi;
//
//		 while (ukaz1!=NULL)
//		 {
//			 ////překopírování dat do pomocného objektu uložitelného do bináru
//			 C_vozik *c_ukaz1=new C_vozik;
//
//			 //samotná data
//			 c_ukaz1->n=ukaz1->n;
//			 c_ukaz1->id_length=ukaz1->id.Length()+1;
//			 c_ukaz1->delka=ukaz1->delka;
//			 c_ukaz1->sirka=ukaz1->sirka;
//			 c_ukaz1->vyska=ukaz1->vyska;
//			 c_ukaz1->rotace=ukaz1->rotace;
//			 c_ukaz1->max_vyrobku=ukaz1->max_vyrobku;
//			 c_ukaz1->akt_vyrobku=ukaz1->akt_vyrobku;
//			 c_ukaz1->delka_vcetne_vyrobku=ukaz1->delka_vcetne_vyrobku;
//			 c_ukaz1->sirka_vcetne_vyrobku=ukaz1->sirka_vcetne_vyrobku;
//			 c_ukaz1->vyska_vcetne_vyrobku=ukaz1->vyska_vcetne_vyrobku;
//			 c_ukaz1->stav=ukaz1->stav;
//			 c_ukaz1->barva=ukaz1->barva;
//			 c_ukaz1->text_length=ukaz1->nazev_vyrobku.Length()+1;
//			 FileStream->Write(c_ukaz1,sizeof(C_vozik));//zapiše jeden prvek do souboru
//			 //text - ID
//			 wchar_t *id=new wchar_t [c_ukaz1->id_length];
//			 id=ukaz1->id.c_str();
//			 FileStream->Write(id,c_ukaz1->id_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
//			 id=NULL; delete[] id;
//			 //text - name
//			 wchar_t *name=new wchar_t [c_ukaz1->text_length];
//			 name=ukaz1->nazev_vyrobku.c_str();
//			 FileStream->Write(name,c_ukaz1->text_length*sizeof(wchar_t));//zapiše druhý řetězec za prvek bod
//			 name=NULL; delete[] name;
//
//			 c_ukaz1=NULL;delete c_ukaz1;
//			 ukaz1=ukaz1->dalsi;//posunutí na další pozici v seznamu
//		 };
//		 ukaz1=NULL;delete ukaz1;

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
			TFileStream *FileStream=new TFileStream(FileName,fmOpenRead);

			//načte hlavičku ze souboru
			FileStream->Read(&File_hlavicka,sizeof(TFile_hlavicka));//načte hlavičku ze souboru

			//uložení parametrů rastru
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

			//technologické objekty
			for(unsigned int i=1;i<=File_hlavicka.pocet_objektu;i++)//možno řešit sice while, po strukturách, ale toto je připravené pro případ, kdy budu načítat i objekty jiného typu než objekt
			{
				TObjekt *ukaz=new TObjekt;
				C_objekt *c_ukaz=new C_objekt;
				FileStream->Read(c_ukaz,sizeof(C_objekt));//načte jeden prvek ze souboru

				if(c_ukaz->n!=0 && File_hlavicka.pocet_objektu>=c_ukaz->n)//pokud nenačte hlavičku či nějaký shit
				{
						//ShowMessage(c_ukaz->n);
						//samotná data
						ukaz->n=c_ukaz->n;
						ukaz->id=c_ukaz->id;
						ukaz->X=c_ukaz->X;
						ukaz->Y=c_ukaz->Y;
						ukaz->rezim=c_ukaz->rezim;
						ukaz->CT=c_ukaz->CT;
						ukaz->RD=c_ukaz->RD;
						ukaz->kapacita=c_ukaz->kapacita;
						ukaz->kapacita_dop=c_ukaz->kapacita_dop;
						ukaz->pozice=c_ukaz->pozice;
						ukaz->rotace=c_ukaz->rotace;
						ukaz->mezera=c_ukaz->mezera;
						ukaz->mezera_jig=c_ukaz->mezera_jig;
						ukaz->mezera_podvozek=c_ukaz->mezera_podvozek;
						ukaz->pohon=vrat_pohon(c_ukaz->pohon);
						ukaz->delka_dopravniku=c_ukaz->delka_dopravniku;
						ukaz->cekat_na_palce=c_ukaz->cekat_na_palce;
						ukaz->stopka=c_ukaz->stopka;
						ukaz->odchylka=c_ukaz->odchylka;
						ukaz->CT_zamek=c_ukaz->CT_zamek;
						ukaz->RD_zamek=c_ukaz->RD_zamek;
						ukaz->DD_zamek=c_ukaz->DD_zamek;
						ukaz->K_zamek=c_ukaz->K_zamek;
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
						//poznámku
						wchar_t *poznamka=new wchar_t[c_ukaz->poznamka_length];
						FileStream->Read(poznamka,c_ukaz->poznamka_length*sizeof(wchar_t));
						ukaz->poznamka=poznamka;
						poznamka=NULL; delete[] poznamka;

						//vloží prvek do spojového seznamu
						vloz_objekt(ukaz);
				}
				ukaz=NULL; delete ukaz;
				c_ukaz=NULL; delete c_ukaz;
			};

			//zakázky
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


			//ZDM
//			for(unsigned int i=1;i<=File_hlavicka.pocet_voziku;i++)//možno řešit sice while, po strukturách, ale toto je připravené pro případ, kdy budu načítat i objekty jiného typu než objekt
//			{
//				TVozik *ukaz1=new TVozik;
//				C_vozik *c_ukaz1=new C_vozik;
//				FileStream->Read(c_ukaz1,sizeof(C_vozik));//načte jeden prvek ze souboru
//				if(c_ukaz1->n!=0 && File_hlavicka.pocet_voziku>=c_ukaz1->n)//pokud nenačte hlavičku či nějaký shit
//				{
//						//samotná data
//					ukaz1->n=c_ukaz1->n;
//					ukaz1->delka=c_ukaz1->delka;
//					ukaz1->sirka=c_ukaz1->sirka;
//					ukaz1->vyska=c_ukaz1->vyska;
//					ukaz1->rotace=c_ukaz1->rotace;
//					ukaz1->max_vyrobku=c_ukaz1->max_vyrobku;
//					ukaz1->akt_vyrobku=c_ukaz1->akt_vyrobku;
//					ukaz1->delka_vcetne_vyrobku=c_ukaz1->delka_vcetne_vyrobku;
//					ukaz1->sirka_vcetne_vyrobku=c_ukaz1->sirka_vcetne_vyrobku;
//					ukaz1->vyska_vcetne_vyrobku=c_ukaz1->vyska_vcetne_vyrobku;
//					ukaz1->stav=c_ukaz1->stav;
//					ukaz1->barva=c_ukaz1->barva;
//
//
//					//id
//					wchar_t *id=new wchar_t[c_ukaz1->id_length];
//					FileStream->Read(id,c_ukaz1->id_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
//					ukaz1->id=id;
//					id=NULL; delete[] id;
//
//					//popisek
//					wchar_t *name=new wchar_t[c_ukaz1->text_length];
//					FileStream->Read(name,c_ukaz1->text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
//					ukaz1->nazev_vyrobku=name;
//					name=NULL; delete[] name;
//
//					//vloží finální prvek do spojového seznamu
//					vloz_vozik(ukaz1);
//				}
//				ukaz1=NULL; delete ukaz1;
//				c_ukaz1=NULL; delete c_ukaz1;
//			};

			delete FileStream;
			return 1;
			}
			catch(...){;return 2;}//jiná chyba, např. špatný formát souboru
	}
}
////---------------------------------------------------------------------------
void Cvektory::nacti_CSV_retezy(AnsiString FileName)
{
	try
	{
		AnsiString DATA=ReadFromTextFile(FileName);

		//parsování a ukládání do spojového seznamu
		if(DATA.Pos("název") || DATA.Pos("name"))DATA=Form1->ms.delete_repeat(DATA,"\r\n",1);//smaže případnou hlavičku csv souboru
		while(DATA.Pos(";"))//bude parsovat, dokud budou data obsahovat středník
		{
			//pársování
			AnsiString name=DATA.SubString(1,DATA.Pos(";")-1);DATA=Form1->ms.delete_repeat(DATA,";",1);
			AnsiString roztec=DATA.SubString(1,DATA.Pos("\r\n")-1);DATA=Form1->ms.delete_repeat(DATA,"\r\n",1);//smaže konec řádku
			//uložení do spojáku pro další využítí
			vloz_retez(name,Form1->ms.MyToDouble(roztec));
		}
	}
	catch(...)//např. soubor nenalezen
	{
		; //nic se neděje
	}
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
//void Cvektory::get_LT_a_max_min_TT()
//{
//		LT=0.0;
//		MAX_TT=-100.0;
//		MIN_TT=+1000.0;
//		Cvektory::TObjekt *ukaz;
//		ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			LT+=ukaz->CT;
//			if(MAX_TT<ukaz->TTo)MAX_TT=ukaz->TTo;
//			if(MIN_TT>ukaz->TTo)MIN_TT=ukaz->TTo;
//			ukaz=ukaz->dalsi;//posun na další prvek
//		}
//}
////---------------------------------------------------------------------------
//double Cvektory::sum_WT()
//{
//	 double SUM=0;
//	 Cvektory::TObjekt *ukaz;
//	 ukaz=OBJEKTY->dalsi;//přeskočí hlavičku
//	 while (ukaz!=NULL)
//	 {
//			switch(ukaz->rezim)
//			{
//					case 0:SUM+=ukaz->CT-Form1->ms.MyToDouble(Form1->ms.EP(Form1->ms.EP(ukaz->techn_parametry,"PT","\n")+"|","=",+"|"));break;//S&G
//					case 1:SUM+=0;break;//Kontinual - je efektivní bez prostojů prostě kontinuální
//					case 2:SUM+=ukaz->CT;break;//PP - je jeden velký prostoj
//      }
//			ukaz=ukaz->dalsi;//posun na další prvek
//	 }
//	 return SUM;
//}
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
		//vozíky
		if(VOZIKY!=NULL && VOZIKY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_VOZIKY();//vymaze vozíky z paměti
			VOZIKY=NULL; delete VOZIKY;
		}
		hlavicka_VOZIKY();//nutnost

		//zakazky
		if(ZAKAZKY!=NULL && ZAKAZKY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_ZAKAZKY();//byla zde poznámka, že před zdm padalo
			delete ZAKAZKY; ZAKAZKY=NULL;
		}
		hlavicka_ZAKAZKY();//nutnost

		//objekty
		if(OBJEKTY!=NULL && OBJEKTY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_OBJEKTY();//vymaze objekty z paměti
			delete OBJEKTY; OBJEKTY=NULL;
		}
		hlavicka_OBJEKTY();//nutnost

		//pohony
		if(POHONY!=NULL && POHONY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_POHONY();//vymaze pohony z paměti
			delete POHONY; POHONY=NULL;
		}
		hlavicka_POHONY();//nutnost
//
//
//
//
////
////		//palce
////		if(PALCE->predchozi->n>0)//pokud je více objektů
////		{
////			vymaz_seznam();//vymaze body z paměti
////			delete PALCE; PALCE=NULL;
////		}
////
////
		//procesy
		if(PROCESY!=NULL && PROCESY->predchozi->n>0)//pokud je více objektů
		{
			//vymaz_seznam_procesu();
			delete PROCESY; PROCESY=NULL;
		}
		hlavicka_PROCESY();

		//retezy
		if(RETEZY!=NULL && RETEZY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_RETEZY();//vymaze objekty z paměti
			delete RETEZY; RETEZY=NULL;
		}
		hlavicka_RETEZY();//nutnost



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


