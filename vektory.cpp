﻿////---------------------------------------------------------------------------
#pragma hdrstop
#include "vektory.h"
#include "unit1.h"
////---------------------------------------------------------------------------
#pragma package(smart_init)
////---------------------------------------------------------------------------
////konstruktor
Cvektory::Cvektory()
{
	hlavicka_OBJEKTY();//vytvoří novou hlavičku pro objekty
	hlavicka_POHONY();//vytvoří novou hlavičku pro pohony
	//	hlavicka_voziky();
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
	novy->kapacita=0;
	novy->kapacita_dop=0;
	novy->pohon=NULL;//ukazatel na použitý pohon
	novy->delka_dopravniku=0;//delka dopravníku v rámci objektu
	novy->cekat_na_palce=0;//0-ne,1-ano,2-automaticky
	novy->stopka=false;//zda následuje na konci objektu stopka
	//novy->obsazenost=0;

	novy->predchozi=novy;//ukazuje sam na sebe
	novy->dalsi=NULL;
	OBJEKTY=novy;//OBJEKTY
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////uloží objekt a jeho parametry do seznamu
short Cvektory::vloz_objekt(unsigned int id, double X, double Y)
{
	TObjekt *novy=new TObjekt;

	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	novy->id=id;
	novy->short_name=knihovna_objektu[id].short_name;
	novy->name=knihovna_objektu[id].name;
	novy->rezim=0;if(id==4 || id==5 || id==6)novy->rezim=2;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
	novy->X=X;//přiřadím X osu,pozice objektu
	novy->Y=Y;//přiřadím Y osu,pozice objektu
	novy->kapacita=0;
	novy->kapacita_dop=0;
	novy->pohon=new TPohon;//ukazatel na použitý pohon
	novy->delka_dopravniku=0;//delka dopravníku v rámci objektu
	novy->cekat_na_palce=0;//0-ne,1-ano,2-automaticky
	novy->stopka=false;//zda následuje na konci objektu stopka
	//novy->obsazenost=0;

	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí

	return 0;
};
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu mezi objekty                //p předchozí
short Cvektory::vloz_objekt(unsigned int id, double X, double Y,TObjekt *p)
{
	TObjekt *novy=new TObjekt;
	novy->id=id;
	novy->short_name=knihovna_objektu[id].short_name;
	novy->name=knihovna_objektu[id].name;
	novy->X=X;//přiřadím X osu
	novy->Y=Y;//přiřadím Y osu
	novy->kapacita=0;
	novy->kapacita_dop=0;
	novy->pohon=new TPohon;//ukazatel na použitý pohon
	novy->delka_dopravniku=0;//delka dopravníku v rámci objektu
	novy->cekat_na_palce=0;//0-ne,1-ano,2-automaticky
	novy->stopka=false;//zda následuje na konci objektu stopka

	//novy->obsazenost=0;

	novy->predchozi=p;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=p->dalsi;
	p->dalsi->predchozi=novy;
	p->dalsi=novy;
	novy->n=p->n;//přiřadím počítadlo prvku ze současného prvku, v dalším kroku se totiž navýší
	//indexy zvýšit separátně
	return 0;
};
//---------------------------------------------------------------------------
//uloží objekt a jeho parametry do seznamu - přetížená fce
short Cvektory::vloz_objekt(TObjekt *Objekt)
{
	TObjekt *novy=new TObjekt;

	novy=Objekt;//novy bude ukazovat tam kam prvek Objekt
	novy->n=OBJEKTY->predchozi->n+1;//navýším počítadlo prvku o jedničku
	OBJEKTY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
	novy->predchozi=OBJEKTY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	OBJEKTY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí

	return 0;
};
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
////---------------------------------------------------------------------------
////smaze body z pameti
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
	novy->roztec=0;

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
void Cvektory::vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double roztec)
{
	TPohon *novy=new TPohon;
	novy->name=name;
	novy->rychlost_od=rychlost_od;
	novy->rychlost_do=rychlost_do;
	novy->roztec=roztec;
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
	nova->name="hlavička";
	nova->barva=clWhite;
	nova->pomer=0;
	nova->TT=0;
	nova->jig.sirka=0;nova->jig.delka=0;nova->jig.vyska=0;nova->jig.ks=0;
	nova->pocet_voziku=0;
	nova->serv_vozik_pocet=0;
	nova->opakov_servis=0;
	nova->cesta=new TCesta;

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;//další prvek zatím není ukazuje na nul
	ZAKAZKY=nova;//nahraje ukazatel na hlavičku spojového seznamu na ukazatel CESTY
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
//vytvoří zakázku dle zadaných parametru do spojového seznamu ZAKÁZKY
void Cvektory::vloz_zakazku(UnicodeString id,UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis,TCesta *Cesta)
{
	TZakazka *nova=new TZakazka;
	nova->id=id;
	nova->name=name;
	nova->barva=barva;
	nova->pomer=pomer;
	nova->TT=TT;
	nova->jig=jig;
	nova->pocet_voziku=pocet_voziku;
	nova->serv_vozik_pocet=serv_vozik_pocet;
	nova->opakov_servis=opakov_servis;
	nova->cesta=Cesta;

	vloz_zakazku(nova);
}
//---------------------------------------------------------------------------
//smaze seznam ZAKAZKY z paměti v četně přidružených cest
long Cvektory::vymaz_seznam_ZAKAZKY()
{
	long pocet_smazanych_objektu=0;
	while (ZAKAZKY!=NULL)
	{
		//mazání jednotlivých cest
		while (ZAKAZKY->cesta!=NULL)
		{
			ZAKAZKY->cesta->predchozi=NULL;
			delete ZAKAZKY->cesta->predchozi;
			ZAKAZKY->cesta=ZAKAZKY->cesta->dalsi;
		};
		ZAKAZKY->predchozi=NULL;
		delete ZAKAZKY->predchozi;
		ZAKAZKY=ZAKAZKY->dalsi;
		pocet_smazanych_objektu++;
	};

	return pocet_smazanych_objektu;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vytvoří novou hlavičku pro spojový seznam konkrétní cesty dané zakázky
void Cvektory::hlavicka_cesta_zakazky(TZakazka *Zakazka)
{
	Zakazka->cesta=new TCesta;
	TCesta *nova=Zakazka->cesta;
	nova->n=0;
	nova->objekt=NULL;
	nova->CT=0;
	nova->Tc=0;
	nova->Tv=0;
	nova->RD=0;

	nova->predchozi=nova;//ukazuje sam na sebe
	nova->dalsi=NULL;
}
//---------------------------------------------------------------------------
//do konkrétní cesty vloží segmenty cesty
void Cvektory::vloz_segment_cesty(TZakazka *Zakazka,TCesta *Segment_cesty)
{
	TCesta *segment=new TCesta;
	segment=Segment_cesty;
	segment->n=Zakazka->cesta->predchozi->n+1;//navýším počítadlo prvku o jedničku

	Zakazka->cesta->predchozi->dalsi=segment;//poslednímu prvku přiřadím ukazatel na nový prvek
	segment->predchozi=Zakazka->cesta->predchozi;//nova prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
	segment->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
	Zakazka->cesta->predchozi=segment;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
}
void Cvektory::vloz_segment_cesty(TZakazka *Zakazka,TObjekt *Objekt,double CT,double Tc,double Tv,double RD)
{
	TCesta *segment=new TCesta;

	segment->n=Zakazka->cesta->predchozi->n+1;//navýším počítadlo prvku o jedničku
	segment->objekt=Objekt;
	segment->CT=CT;
	segment->Tc=Tc;
	segment->Tv=Tv;
	segment->RD;

	vloz_segment_cesty(Zakazka,segment);
}
//---------------------------------------------------------------------------
//Cvektory::TSeznam_cest *Cvektory::vrat_cestu(unsigned int ID_cesty)
//{
//	Cvektory::TSeznam_cest *ukaz=CESTY->dalsi;
//	TSeznam_cest *ret=NULL;
//	while (ukaz!=NULL)
//	{
//		if(ukaz->n==ID_cesty)
//		{
//			ret=ukaz;
//			break;
//		}
//		ukaz=ukaz->dalsi;
//	}
//	return ret;
//}

////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//void Cvektory::hlavicka_procesy()
//{
//	TProces *novy=new TProces;
//	novy->n=0;
//	novy->n_v_zakazce=0;
//	novy->Tpoc=0;
//	novy->Tkon=0;
//	novy->Tdor=0;
//	novy->Tpre=0;
//	novy->Tcek=0;
//	novy->cesta=NULL;
//	novy->vozik=NULL;
//
//	novy->predchozi=novy;//ukazuje sam na sebe
//	novy->dalsi=NULL;//další je zatim NULL
//	PROCESY=novy;
//}
////---------------------------------------------------------------------------
////uloží ukazatel na vozík do spojového seznamu voziku přetížená fce
//void Cvektory::vloz_proces(TProces *Proces)
//{
//	TProces *novy=new TProces;
//	novy=Proces;//novy bude ukazovat tam kam prvek data
//
//	novy->n=PROCESY->predchozi->n+1;//navýším počítadlo prvku o jedničku
//	PROCESY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=PROCESY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
//	PROCESY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//}
////---------------------------------------------------------------------------
////hledá bod mezi procesy
//Cvektory::TProces *Cvektory::najdi_proces(double cas, double vozik)
//{
//	TProces *RET=NULL;
//	TProces *P=PROCESY->dalsi;
//	while (P!=NULL)
//	{
//		if(P->vozik->n==vozik && P->Tpoc<=cas && cas<P->Tcek)//pokud se myš nachází nad právě cyklem procházeným procesem
//		{
//			 RET=P;//proces nalezen
//			 break;//ukončí předčasně while cyklus-zbytečně by se hledalo dál, proces byl již nalezen
//		}
//		P=P->dalsi;
//	};
//	return RET;
//}
////---------------------------------------------------------------------------
////vratí následující proces na stejném objektu jako proces zadaný
//Cvektory::TProces *Cvektory::vrat_nasledujici_proces_objektu(TProces *Proces)
//{
//		TProces *RET=NULL;
//		TProces *P=Proces->dalsi;
//		while (P!=NULL)
//		{
//			if(P->cesta->objekt==Proces->cesta->objekt)
//			{
//				RET=P;
//				break;
//			}
//			P=P->dalsi;
//		};
//		return P;
//}
////---------------------------------------------------------------------------
//long Cvektory::vymaz_seznam_procesu()
//{
//	long pocet_smazanych_objektu=0;
//	while (PROCESY!=NULL)
//	{
//		pocet_smazanych_objektu++;
//		delete PROCESY->predchozi;
//		PROCESY->predchozi=NULL;
//		PROCESY=PROCESY->dalsi;
//	};
//
//	return pocet_smazanych_objektu;
//}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PRÁCE ZE SOUBORY
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Uloží vektorová data do souboru
short int Cvektory::uloz_do_souboru(UnicodeString FileName)
{
     TFileStream *FileStream=new TFileStream(FileName,fmOpenWrite|fmCreate);
		 //optimalizace_seznamu();  asi možno smazat, myslím, že nepoužívám

		 //zapiše hlavičku do souboru
		 File_hlavicka.pocet_pohonu=POHONY->predchozi->n;
		 File_hlavicka.pocet_objektu=OBJEKTY->predchozi->n;
		 //ZDM File_hlavicka.pocet_voziku=VOZIKY->predchozi->n;
		 //ZDM File_hlavicka.delka_textu_prepravniky=seznam_dopravniku.Length()+1;
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
			 c_ukaz1->roztec=ukaz1->roztec;
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
					c_ukaz->kapacita_dop=ukaz->kapacita_dop;
					c_ukaz->kapacita=ukaz->kapacita;
					c_ukaz->pohon=ukaz->pohon->n;
					c_ukaz->delka_dopravniku=ukaz->delka_dopravniku;
					c_ukaz->cekat_na_palce=ukaz->cekat_na_palce;
					c_ukaz->stopka=ukaz->stopka;
					c_ukaz->text_length=ukaz->name.Length()+1;
					//ZDM c_ukaz->paremetry_text_length=ukaz->techn_parametry.Length()+1;
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
					//ZDM //text - parametry
					//ZDM wchar_t *parametry=new wchar_t [c_ukaz->paremetry_text_length];
					//ZDM parametry=ukaz->techn_parametry.c_str();
					//ZDM FileStream->Write(parametry,c_ukaz->paremetry_text_length*sizeof(wchar_t));//zapiše třetí řetězec za prvek bod
					//ZDM parametry=NULL; delete[] parametry;
			 }
			 c_ukaz=NULL;delete c_ukaz;
			 ukaz=ukaz->dalsi;//posunutí na další pozici v seznamu
		 };
		 ukaz=NULL;delete ukaz;

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
//---------------------------------------------------------------------------
//načte vektorová data ze souboru
short int Cvektory::nacti_ze_souboru(UnicodeString FileName)
{
	if(!FileExists(FileName))return 0;
	else
	{
			try
			{
			TFileStream *FileStream=new TFileStream(FileName,fmOpenRead);

			//zapiše hlavičku do souboru
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
					ukaz1->roztec=c_ukaz1->roztec;

					//popisek
					wchar_t *name=new wchar_t[c_ukaz1->text_length];
					FileStream->Read(name,c_ukaz1->text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
					ukaz1->name=name;
					name=NULL; delete[] name;

					//vloží finální prvek do spojového seznamu
					vloz_pohon(ukaz1);
					(ukaz1);
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
						ukaz->kapacita=c_ukaz->kapacita;
						ukaz->kapacita_dop=c_ukaz->kapacita_dop;
						ukaz->pohon=vrat_pohon(c_ukaz->pohon);
						ukaz->delka_dopravniku=c_ukaz->delka_dopravniku;
						ukaz->cekat_na_palce=c_ukaz->cekat_na_palce;
						ukaz->stopka=c_ukaz->stopka;

						//zkratku
						wchar_t *short_name=new wchar_t [5];
						FileStream->Read(short_name,5*sizeof(wchar_t));//načte popisek umístěný za strukturou bod
						ukaz->short_name=short_name;
						short_name=NULL; delete[] short_name;
						//popisek
						wchar_t *name=new wchar_t[c_ukaz->text_length];
						FileStream->Read(name,c_ukaz->text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
						ukaz->name=name;
						name=NULL; delete[] name;

						//ZDM //data pro Valuelisteditor, paremetry
						//ZDM wchar_t *parametry=new wchar_t[c_ukaz->paremetry_text_length];
						//ZDM FileStream->Read(parametry,c_ukaz->paremetry_text_length*sizeof(wchar_t));//načte jeden nazev fontu za prvekem bod a popisek bodu
						//ZDM ukaz->techn_parametry=parametry;
						//ZDM parametry=NULL; delete[] parametry;

						//vloží prvek do spojového seznamu
						vloz_objekt(ukaz);
				}
				ukaz=NULL; delete ukaz;
				c_ukaz=NULL; delete c_ukaz;
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
			catch(...){return 2;}//jiná chyba, např. špatný formát souboru
	}
}
////---------------------------------------------------------------------------
short int Cvektory::ulozit_report(UnicodeString FileName)
{
		//ZDM get_LT_a_max_min_TT();//zajistí hodnoty

		AnsiString data="";//celková textová data k exportu

		//zjištění exportovaného formátu
		unsigned short export_format=1;
		if(FileName.SubString(FileName.Length()-2,3).LowerCase() =="xls")export_format=2;
		if(FileName.SubString(FileName.Length()-3,4).LowerCase() =="html")export_format=3;

		//formátování textu do jednotlivých formátů
		UnicodeString S=";";//SEPARATOR pro CSV
		switch(export_format)//1-csv, 2 - xls,3 - html
		{
			//ZDM case 1:data=get_csv_xls(";");break;//CSV
			//ZDM case 2:data=get_csv_xls("\t");break;//XLS
			//ZDM case 3:data=get_html();break;//html
		}

		//poznámka, provizorní záležitost - potom smazat
		if(export_format==3)data+="<font color=\"Red\">";
		data+="\n*pozn.:další paramatry objektů budou doplněny, jedná se provizorní podobu výstupu ...";

		///////////////////////////zapis data do souboru
		TMemoryStream* MemoryStream=new TMemoryStream();
		MemoryStream->Clear();
		MemoryStream->Write(data.c_str(),data.Length());//Win kodování
		MemoryStream->SaveToFile(FileName);
		delete MemoryStream;
		return 1;
}
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
//void Cvektory::hlavicka_voziky()
//{
//	TVozik *novy=new TVozik;
//	novy->n=0;
//	novy->id=0;
//	novy->delka=0;
//	novy->sirka=0;
//	novy->vyska=0;
//	novy->rotace=0;
//	novy->nazev_vyrobku="hlavicka";
//	novy->max_vyrobku=0;
//	novy->akt_vyrobku=0;
//	novy->delka_vcetne_vyrobku=0;
//	novy->sirka_vcetne_vyrobku=0;
//	novy->vyska_vcetne_vyrobku=0;
//	novy->stav=-1;
//	novy->pozice=0;
//	novy->start=0;
//	novy->X=0;novy->Y=0;
//	novy->timer=0;
//	novy->segment=NULL;
//	novy->cesta=NULL;
//
//	novy->predchozi=novy;//ukazuje sam na sebe
//	novy->dalsi=NULL;
//	VOZIKY=novy;
//}
////---------------------------------------------------------------------------
//void Cvektory::vloz_vozik()//přidá nový vozík do seznamu VOZIKY
//{
//	TVozik *novy=new TVozik;
//
//	novy->n=VOZIKY->predchozi->n+1;//navýším počítadlo prvku o jedničku
//	novy->segment=NULL;novy->pozice=-1;novy->stav=-1;
//	novy->X=0;novy->Y=0;novy->timer=0;novy->start=0;
//
//	VOZIKY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=VOZIKY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;
//	VOZIKY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//}
////---------------------------------------------------------------------------
////uloží ukazatel na vozík do spojového seznamu voziku přetížená fce
//void Cvektory::vloz_vozik(TVozik *Vozik)
//{
//	TVozik *novy=new TVozik;
//	novy=Vozik;//novy bude ukazovat tam kam prvek data
//
//	//pozor v případě načítání existujícího stavu ze souboru změnitm toto je výchozí pozice na lince
//	novy->segment=NULL;novy->pozice=-1;novy->stav=-1;
//	novy->X=0;novy->Y=0;novy->timer=0;novy->start=0;
//
//	novy->n=VOZIKY->predchozi->n+1;//navýším počítadlo prvku o jedničku
//	VOZIKY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=VOZIKY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;//poslední prvek se na zadny dalsí prvek neodkazuje (neexistuje
//	VOZIKY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//};
////---------------------------------------------------------------------------
//void Cvektory::vloz_vozik(unsigned long n,UnicodeString id,double delka,double sirka,double vyska,double rotace,UnicodeString nazev_vyrobku,double max_vyrobku,double akt_vyrobku,double delka_vcetne_vyrobku,double sirka_vcetne_vyrobku,double vyska_vcetne_vyrobku,TColor barva,TSeznam_cest *cesta)
//{
// 	TVozik *novy=new TVozik;
//
//	novy->n=n;//navýším počítadlo prvku o jedničku
//	novy->id=id;
//	novy->delka=delka;
//	novy->sirka=sirka;
//	novy->vyska=vyska;
//	novy->rotace=rotace;
//	novy->nazev_vyrobku=nazev_vyrobku;
//	novy->max_vyrobku=max_vyrobku;
//	novy->akt_vyrobku=akt_vyrobku;
//	novy->delka_vcetne_vyrobku=delka_vcetne_vyrobku;
//	novy->sirka_vcetne_vyrobku=sirka_vcetne_vyrobku;
//	novy->vyska_vcetne_vyrobku=vyska_vcetne_vyrobku;
//	novy->barva=barva;
//	novy->pozice=-1;novy->stav=-1;
//	novy->start=0;
//	novy->X=0;novy->Y=0;
//	novy->timer=0;
//	novy->segment=NULL;
//	novy->cesta=cesta;
//
//
//	VOZIKY->predchozi->dalsi=novy;//poslednímu prvku přiřadím ukazatel na nový prvek
//	novy->predchozi=VOZIKY->predchozi;//novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi=NULL;
//	VOZIKY->predchozi=novy;//nový poslední prvek zápis do hlavičky,body->predchozi zápis do hlavičky odkaz na poslední prvek seznamu "predchozi" v tomto případě zavádějicí
//
//}
////---------------------------------------------------------------------------
//long Cvektory::vymaz_seznam_voziku()
//{
//	long pocet_smazanych_objektu=0;
//	while (VOZIKY!=NULL)
//	{
//		pocet_smazanych_objektu++;
//		VOZIKY->predchozi=NULL;
//		delete VOZIKY->predchozi;
//		VOZIKY=VOZIKY->dalsi;
//	};
//
//	return pocet_smazanych_objektu;
//};
////---------------------------------------------------------------------------
//void Cvektory::vymazat_casovou_obsazenost_objektu_a_pozice_voziku(TObjekt *Objekt,TVozik *Vozik)
//{
//	TObjekt *ukaz=Objekt->dalsi;
//	while (ukaz!=NULL)
//	{
//		ukaz->obsazenost=0;
//		ukaz=ukaz->dalsi;
//	};
//	TVozik *ukaz1=Vozik->dalsi;
//	while (ukaz1!=NULL)
//	{
//		ukaz1->pozice=-1;
//		ukaz1=ukaz1->dalsi;
//	};
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
//TPointD Cvektory::vrat_zacatek_a_konec_zakazky(TSeznam_cest *jaka)//ukazatel na cestu resp, zakázku
//{
//	TPointD RET; RET.x=0;RET.y=0;bool prvni=true;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta->n==jaka->n && prvni){RET.x=vozik->start/Form1->d.PX2MIN;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
//		if(vozik->cesta->n==jaka->n){RET.y=vozik->pozice/Form1->d.PX2MIN;}//uloží koncovou pozici posledního vozíku na zakázce
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
//TPointD Cvektory::vrat_zacatek_a_konec_zakazky(unsigned int ID_zakazky)//ukazatel na cestu resp, zakázku
//{
//	TPointD RET; RET.x=0;RET.y=0;bool prvni=true;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta->n==ID_zakazky){RET.x=vozik->start/Form1->d.PX2MIN;prvni=false;}//uloží výchozí pozici prvního vozíku na zakázce
//		if(vozik->cesta->n==ID_zakazky)RET.y=vozik->pozice/Form1->d.PX2MIN;//uloží koncovou pozici posledního vozíku na zakázce
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
////nemusí se vždy jednat o poslední zakázku
//double Cvektory::vrat_nejpozdejsi_konec_zakazek()
//{
//	double MAX=0;
//	TSeznam_cest *C=CESTY->dalsi;
//	while(C!=NULL)
//	{
//		double DO=vrat_zacatek_a_konec_zakazky(C).y;//konec zakazky v min
//		if(DO>MAX)MAX=DO;
//		C=C->dalsi;
//	}
//	return MAX;;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_LT_voziku(TVozik *jaky)//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
//{
//	if(jaky!=NULL) return (jaky->pozice-jaky->start)/Form1->d.PX2MIN;
//	else return 0;
//}
//double Cvektory::vrat_LT_voziku(unsigned int n_voziku)//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
//{
//	double RET=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(n_voziku==vozik->n)//pokud byl nalezen
//		{
//			RET=vrat_LT_voziku(vozik);
//			break;
//		}
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_sumPT_voziku(TVozik *jaky)//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
//{
//	double SUM=0;
//	Cvektory::TCesta *C=jaky->cesta->cesta->dalsi;
//	while(C!=NULL)//jde po konkrétní cestě
//	{
//		SUM+=C->CT;
//		C=C->dalsi;
//	}
//	return SUM;
//}
//double Cvektory::vrat_sumPT_voziku(unsigned int n_voziku)//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
//{
//	double SUM=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(n_voziku==vozik->n)//pokud byl nalezen
//		{
//			SUM=vrat_sumPT_voziku(vozik);
//			break;
//		}
//		vozik=vozik->dalsi;
//	}
//	return SUM;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_AVGsumPT_zakazky(TSeznam_cest *jaka)//vrátí čistý čas, který strávily vozík ve výrobě bez čekání pro danou zakazku
//{
//	double SUM=0.0; double n=0.0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(jaka==vozik->cesta)//pokud se jedna o zadanou cestu navýší počítadlo
//		{
//			SUM+=vrat_sumPT_voziku(vozik);
//			n++;
//		}
//		vozik=vozik->dalsi;
//	}
//	if(n==0)return 0;//pokud je pouze jenom jeden vozík není takt
//	else
//	return SUM/n;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_WT_voziku(TVozik *jaky)//vrátí čas čeká vozíku během výroby
//{
//	return vrat_LT_voziku(jaky)-vrat_sumPT_voziku(jaky);
//}
//double Cvektory::vrat_WT_voziku(unsigned int n_voziku)//vrátí čas čeká vozíku během výroby
//{
//	double RET=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(n_voziku==vozik->n)//pokud byl nalezen
//		{
//			RET=vrat_WT_voziku(vozik);
//			break;
//		}
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_AVGsumWT_zakazky(TSeznam_cest *jaka)//vrátí čistý čas, který vozíky čekaly pro danou zakazku
//{
//	double SUM=0.0; double n=0.0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(jaka==vozik->cesta)//pokud se jedna o zadanou cestu navýší počítadlo
//		{
//			SUM+=vrat_WT_voziku(vozik);
//			n++;
//		}
//		vozik=vozik->dalsi;
//	}
//	if(n==0)return 0;//pokud je pouze jenom jeden vozík není takt
//	else
//	return SUM/n;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_TT_voziku(TVozik *jaky)//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
//{
//	return (jaky->pozice-jaky->predchozi->pozice)/Form1->d.PX2MIN;//ošetřuje i případ prvního prvku a hlavičky, hlavička má pozici nastavenou na nula
//}
//double Cvektory::vrat_TT_voziku(unsigned int n_voziku)//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
//{
//	double RET=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(n_voziku==vozik->n)//pokud byl nalezen
//		{
//			RET=vrat_TT_voziku(vozik);
//			break;
//		}
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
//double Cvektory::vrat_AVG_TT_zakazky(TSeznam_cest *jaka)//vrátí hodnotu průměrného TT mezi vozíky v rámci dané zakázky/cesty
//{
//	unsigned int i=0;
//	bool prvni_vozik_zakazky=true;//ignorace taktu prvního vozíku
//	double TT=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta==jaka)//pokud byl nalezen v rámci dané cesty a zároveň se nejedná o první vozík v zakázce, tak aby se nezohledňoval takt z vyplývající z náběhu link či přechodu mezi zakázkami
//		{
//			if(prvni_vozik_zakazky)prvni_vozik_zakazky=false;//ignorace taktu prvního vozíku
//			else
//			{
//				TT+=vrat_TT_voziku(vozik);//zatím se jedná pouze o součet TT
//				i++;//navýší počet pro výpočet průměru
//			}
//		}
//		vozik=vozik->dalsi;
//	}
//	if(i==0)return 0;//pokud je pouze jenom jeden vozík není takt
//	else return floor(TT/i*1000000.0)/1000000.0;//vrátí průměrné TT
//}                      //kvůli chybě s přesnosti uříznutí na 6 reaálných míst
//double Cvektory::vrat_AVG_TT_zakazky(unsigned int n_zakazky)//vrátí hodnotu průměrného TT mezi vozíky v rámci
//{
//	unsigned int i=0;
//	bool prvni_vozik_zakazky=true;//ignorace taktu prvního vozíku
//	double TT=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta->n==n_zakazky)//pokud byl nalezen v rámci dané cesty a zároveň se nejedná o první vozík v zakázce, tak aby se nezohledňoval takt z vyplývající z náběhu link či přechodu mezi zakázkami
//		{
//			if(prvni_vozik_zakazky)prvni_vozik_zakazky=false;//ignorace taktu prvního vozíku
//			else
//			{
//				TT+=vrat_TT_voziku(vozik);//zatím se jedná pouze o součet TT
//				i++;//navýší počet pro výpočet průměru
//			}
//		}
//		vozik=vozik->dalsi;
//	}
//	if(i==0)return 0;//pokud je pouze jenom jeden vozík není takt
//	else return floor(TT/i*1000000.0)/1000000.0;//vrátí průměrné TT
//}                      //kvůli chybě s přesnosti uříznutí na 6 reaálných míst
////---------------------------------------------------------------------------
//unsigned int Cvektory::vrat_pocet_voziku_zakazky(TSeznam_cest *jaka)
//{
//	unsigned int RET=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta==jaka)RET++;
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
//unsigned int Cvektory::vrat_pocet_voziku_zakazky(unsigned int n_zakazky)
//{
//	unsigned int RET=0;
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		if(vozik->cesta->n==n_zakazky)RET++;
//		vozik=vozik->dalsi;
//	}
//	return RET;
//}
////---------------------------------------------------------------------------
//unsigned int Cvektory::WIP()//vrátí max. počet vozíků na lince
//{
//	unsigned int pocet_final=0;
//	//srovnává všechny kombinace, možná by šlo zjednodušit, uvídíme v průběhu
//	Cvektory::TVozik *vozik=VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//	while (vozik!=NULL)
//	{
//		unsigned int pocet=0;
//		Cvektory::TVozik *vozik2=vozik->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
//		while (vozik2!=NULL)
//		{
//
//			if(vozik->pozice>=vozik2->start)//pokud nastane situace že vozík skončil před začátkem vozíku, není nutné navyšovat počítadlo vozíků
//			pocet++;
//			vozik2=vozik2->dalsi;
//		}
//		if(pocet_final<pocet)pocet_final=pocet;
//		vozik=vozik->dalsi;
//	}
//	if(VOZIKY->dalsi!=NULL)//pokud existuje nějaký vozík
//	return pocet_final/*+1*/;
//	else
//	return 0;
//}
////---------------------------------------------------------------------------
////srovnává všechny kombinace
//unsigned int Cvektory::vrat_kapacitu_objektu(TObjekt *O)
//{
//	 unsigned int pocet_final=0;
//	 if(O->rezim!=0)//pokud se nejedná o S&G, ten má kapacitu vždy 1, pokud ano algoritmus se přeskočí
//	 {
//			 TProces *P=PROCESY->dalsi;
//			 while (P!=NULL)
//			 {
//				 unsigned int pocet=0;
//				 if(P->cesta->objekt->n==O->n)//pokud se jedná o hledaný objekt
//				 {
//						TProces *P2=P->dalsi;
//						while (P2!=NULL)
//						{
//							if(P2->cesta->objekt->n==O->n && P->Tcek>P2->Tpoc)//pokud se jedná o spoluhledaný objekt a objekty se v čase zároveň překrývají (tudíž se navyšuje jejich kapacita)
//							pocet++;
//							P2=P2->dalsi;
//						}
//				 }
//				 if(pocet_final<pocet)pocet_final=pocet;
//				 P=P->dalsi;
//			 };
//	 }
//	 else pocet_final=1;
//	 if(O!=NULL && PROCESY!=NULL && PROCESY->predchozi->n>0) return pocet_final/*+1*/;
//	 else return 0;
//}
////---------------------------------------------------------------------------
//void Cvektory::uloz_doporucene_kapacity_objetku()
//{
//	TObjekt *ukaz=OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//		ukaz->dop_kapacita_objektu=vrat_kapacitu_objektu(ukaz);
//		ukaz=ukaz->dalsi;
//	}
//}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvektory::vse_odstranit()
{
		//objekty
		if(OBJEKTY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_OBJEKTY();//vymaze objekty z paměti
			delete OBJEKTY; OBJEKTY=NULL;
		}

		//pohony
		if(POHONY->predchozi->n>0)//pokud je více objektů
		{
			vymaz_seznam_POHONY();//vymaze pohony z paměti
			delete POHONY; POHONY=NULL;
		}

//		//voziky
//		if(VOZIKY->predchozi->n>0)//pokud je více objektů
//		{
//			vymaz_seznam_voziku();//vymaze body z paměti
//			delete VOZIKY; VOZIKY=NULL;
//		}
//
//		//palce
//		if(PALCE->predchozi->n>0)//pokud je více objektů
//		{
//			vymaz_seznam();//vymaze body z paměti
//			delete PALCE; PALCE=NULL;
//		}
//
//		//cesty
//		if(CESTY!=NULL && CESTY->predchozi->n>0)//pokud je více objektů
//		{
//			//vymaz_cesty();   padá dodělat
//			delete CESTY; CESTY=NULL;
//		}
//
//		//procesy
//		if(PROCESY!=NULL && PROCESY->predchozi->n>0)//pokud je více objektů
//		{
//			vymaz_seznam_procesu();
//			delete PROCESY; PROCESY=NULL;
//		}



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

