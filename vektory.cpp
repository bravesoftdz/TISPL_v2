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
}
////---------------------------------------------------------------------------
////vytvoří novou hlavičku pro objekty
void Cvektory::hlavicka_OBJEKTY()
{
	TObjekt *novy=new TObjekt;
	novy->n=0;
	novy->id=0;
	novy->X=0;
	novy->Y=0;
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
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
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

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	OBJEKTY=novy;//OBJEKTY
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////uloží objekt a jeho parametry do seznamu
void Cvektory::vloz_objekt(unsigned int id, double X, double Y)
{
	TObjekt *novy=new TObjekt;

	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->id=id;
	novy->short_name=knihovna_objektu[id].short_name;
	novy->name=knihovna_objektu[id].name;
	novy->rezim=0;if(id==4 || id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu,pozice objektu
	novy->Y=Y;//přiřadím Y osu,pozice objektu
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
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
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

	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu za objekt p        //p předchozí
void Cvektory::vloz_objekt(unsigned int id, double X, double Y,TObjekt *p)
{
	TObjekt *novy=new TObjekt;
	novy->id=id;
	novy->short_name=knihovna_objektu[id].short_name;
	novy->name=knihovna_objektu[id].name;
	//režim tu před 14.11.2017 nebyl, nevím proč
	novy->rezim=0;if(id==4 || id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu
	novy->Y=Y;//přiřadím Y osu
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
	novy->min_prujezdni_profil.x=0;//výška a šířka minimálního průjezdního profilu v objektu
	novy->min_prujezdni_profil.y=0;//výška a šířka minimálního průjezdního profilu v objektu
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

	novy->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=p->dalsi;
	p->dalsi->predchozi=novy;
	p->dalsi=novy;
	novy->n=p->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
	//indexy zvýšit separátně
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
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
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
		return OBJEKTY->predchozi;//vrátí poslední ukazatel na prvek seznamu
	}
	else//vkládání mezi objekty
	{
		novy->rezim=Objekt->rezim;
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
		novy->min_prujezdni_profil.x=Objekt->min_prujezdni_profil.x;//výška a šířka minimálního průjezdního profilu v objektu
		novy->min_prujezdni_profil.y=Objekt->min_prujezdni_profil.y;//výška a šířka minimálního průjezdního profilu v objektu
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

		novy->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
		novy->dalsi=p->dalsi;
		p->dalsi->predchozi=novy;
		p->dalsi=novy;
		novy->n=p->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
		zvys_indexy(p);//indexy zvýšit separátně se tady psalo
		return novy;//vrátí ukazatel na posledně kopírovaný objekt
	}
}
//---------------------------------------------------------------------------
//hledá objekt v dané oblasti                                       //pracuje v logic souradnicich tzn. již nepouživat *Zoom  použít pouze m2px
Cvektory::TObjekt *Cvektory::najdi_objekt(double X, double Y,double offsetX, double offsetY)
{
	Cvektory::TObjekt *ret=NULL;
	Cvektory::TObjekt *p=OBJEKTY->dalsi;//přeskočí hlavičku
	while (p!=NULL)
	{
		if(p->X<=X && X<=p->X+offsetX*Form1->m2px &&  p->Y>=Y && Y>=p->Y-offsetY*Form1->m2px)ret=p;//nalezeno !
		p=p->dalsi;//posun na další prvek
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
//smaze objekt ze seznamu
short int Cvektory::smaz_objekt(TObjekt *Objekt)
{
	//vyřazení prvku ze seznamu a napojení prvku dalšího na prvek předchozí prku mazaného
	if(Objekt->dalsi!=NULL)//ošetření proti poslednímu prvku
	{
		Objekt->predchozi->dalsi=Objekt->dalsi;
		Objekt->dalsi->predchozi=Objekt->predchozi;
	}
	else//poslední prvek
	{
		if(Objekt->n==1)//pokud je mazaný prvek hned za hlavičkou
		{
			OBJEKTY->predchozi=Objekt->predchozi; //popř hlavička bude ukazovat sama na sebe
			OBJEKTY->dalsi=NULL;
		}
		else
		{
			Objekt->predchozi->dalsi=NULL;
			OBJEKTY->predchozi=Objekt->predchozi;//zapis do hlavičky poslední prvek seznamu
		}
	}

	Objekt=NULL;delete Objekt;//smaže mazaný prvek

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
      break;
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
void Cvektory::sniz_indexy(TObjekt *Objekt)
{
	while (Objekt!=NULL)
	{
		Objekt=Objekt->dalsi;//posun na další prvek
		if(Objekt!=NULL)Objekt->n--;//sníží indexy nasledujicích bodů,protože optimalizace seznamu nefungovalo, navíc ušetřím strojový čas
	}
}
//---------------------------------------------------------------------------
void Cvektory::zvys_indexy(TObjekt *Objekt)//zvýší indexy NÁSLEDUJICÍCH bodů
{
	while (Objekt!=NULL)
	{
		Objekt=Objekt->dalsi;//posun na další prvek
		if(Objekt!=NULL)Objekt->n++;//sníží indexy nasledujicích bodů,protože optimalizace seznamu nefungovalo, navíc ušetřím strojový čas
	}
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
//smaze objekty z pameti
long Cvektory::vymaz_seznam_OBJEKTY()
{
	long pocet_smazanych_objektu=0;
	while (OBJEKTY!=NULL)
	{
		pocet_smazanych_objektu++;
		OBJEKTY->predchozi=NULL;
		delete OBJEKTY->predchozi;
		OBJEKTY=OBJEKTY->dalsi;
	};

	return pocet_smazanych_objektu;
};
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
	POHONY=novy;//OBJEKTY
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
				if(Form_parametry->scComboBox_pohon->ItemIndex>0  && Form_parametry->scComboBox_pohon->ItemIndex==n)//pokud má pohon přiřazen a jedná se o stejný pohon
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
//vratí počet objektů přiřazených k danému pohonu
unsigned long Cvektory::vrat_pocet_objektu_vyuzivajici_pohon(unsigned long n)
{
	TObjekt *O=OBJEKTY->dalsi;
	unsigned long RET=0;
	while (O!=NULL)
	{
		if(O->pohon!=NULL && O->pohon->n==n)RET++;//pokud má pohon přiřazen a jedná se o stejný pohon
	}
	O=NULL;delete O;
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
//vypíše objekt přiřazené k danému pohonu nestíhající přejezd při navrhovaném testRD
AnsiString Cvektory::vypis_objekty_nestihajici_prejezd(TPohon *pohon,double testRD)
{
	TObjekt *O=OBJEKTY->dalsi;
	AnsiString objekty="";
	while (O!=NULL)
	{                                    //mohl bych ještě odfiltrovávat, zda se nejedná o KK, ale je to víceméně zbytečné
		if(O->pohon!=NULL && O->pohon==pohon)//pokud má pohon přiřazen a jedná se o stejný pohon
		{
			if(testRD<O->delka_dopravniku/O->CT)
			objekty=O->short_name+", ";
		}
		O=O->dalsi;
	}
	O=NULL;delete O;
	if(objekty!="")objekty=objekty.SubString(1,objekty.Length()-2);//ještě odebere poslední čárku a mezeru
	return objekty;
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
//zkontroluje aplikovatelnost uvažovaného hodnodty dle VID parametru (aRD=4,R=5,Rz=6,Rx=7 na všech objektech, přiřazených k danému pohonu označeným parametrem PID, pokud je zadán parametr 0 (který je zároveň implicitní), vratí doporučené Rz, pokud 1, vrátí seznam objektů, kde je problém, pokud vrátí prázdné uvozovky, je vše v pořádku
AnsiString Cvektory::validaceR(short VID,unsigned long PID,double aRD,double R,double Rz,double Rx,short getValueOrObject)
{
	TPohon *p=vrat_pohon(PID);
	if(p!=NULL)
	{

  }
// switch(VID)
// {
//	 case 4:break;
//	 case 5:break;
//	 case 6:break;
//	 case 7:break;
// }

//<u>"+AnsiString(doporuc_hodnota)+"</u>
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

			//zapiše hlavičku ze souboru
			FileStream->Read(&File_hlavicka,sizeof(TFile_hlavicka));//načte hlavičku ze souboru

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


