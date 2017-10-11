//---------------------------------------------------------------------------
#ifndef vektoryH
#define vektoryH
#include <vcl.h>
#include "knihovna_objektu.h"
//#include "math.h"
#include "my.h"
//#define TITULEK "Omap editor"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class Cvektory
{
  private:

	public:

	struct TPohon
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		UnicodeString name;//název
		double rychlost_od;//minimální pracovní rychlost dopravníku
		double rychlost_do;//maximální pracovní rychlost dopravníku
		double roztec;//rozteč palců v mm
		struct TPohon *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TPohon *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TPohon *POHONY;//spojový seznam pohonů

	struct TObjekt
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			unsigned int id; //id typu objektu
			UnicodeString short_name;//krátký název max. 4 znaky
			UnicodeString name;//celý název objektu
			double X, Y;//umístění objektu
			unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní,3-stopka
			double kapacita;//uživatelsky zadaná kapacita
			double kapacita_dop;//doporučená, vypočítáná
			TPohon *pohon;//ukazatel na použitý pohon
			double delka_dopravniku;//delka dopravníku v rámci objektu
			unsigned short cekat_na_palce;//0-ne,1-ano,2-automaticky
			bool stopka;//zda následuje na konci objektu stopka
			double odchylka;
			struct TObjekt *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TObjekt *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TObjekt *OBJEKTY;//seznam objektů

	struct TJig//pouze složený datový typ
	{
		 double sirka;
		 double delka;
		 double vyska;
		 double ks;//kusů
	};

	struct TCesta//pouze přidružený spoják, který je součástí zakázky, jeden objekt spojáku je jeden segment cesty
	{
		 unsigned long n;
		 TObjekt *objekt;
		 double CT; //cycle time
		 double Tc;//čaš čištění v rámci zakázky resp. stejné barvy, vztahuje se na konkrétní objekt a a zároveň zakázku, musí být tady, pokud není použito, tak 0
		 double Tv;//čas čištění a výměny barev, vztahuje se na konkrétní objekt a a zároveň zakázku, musí být tady, pokud není použito, tak 0
		 double RD;//rychlost dopravníku
		 struct TCesta *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		 struct TCesta *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct TZakazka
	{
		 unsigned long n;//pořadí objektu ve spoj.seznamu
		 UnicodeString id;//uživatelské ID objektu
		 unsigned short typ;//0- realná,1-servisní
		 UnicodeString name;//název zakázky
		 TColor barva;//barva zakáky
		 double pomer;//poměr z celkového množství výrobků
		 double TT;
		 TJig jig;//šířka délka, výška, rotace a  ks připadajících na jig/rám vozzíku
		 unsigned long pocet_voziku;//počet vozíků v zakázce
		 unsigned long serv_vozik_pocet;//počet servisních vozíků v zakázce
		 unsigned long opakov_servis;//cyklus opakování servisních vozíku
		 struct TCesta *cesta;//ukazatel na první segment cesty
		 struct TZakazka *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		 struct TZakazka *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TZakazka *ZAKAZKY;//spojový seznam zakázek
	TZakazka *ZAKAZKY_temp;//spojový seznam zakázek

	struct TVozik
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			struct TZakazka *zakazka;
			struct TVozik *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TVozik *dalsi;//ukazatel na další objekt ve spojovém seznamu
	};
	TVozik *VOZIKY;//spojový seznam vozíků

	struct TVozik_parametry//paremtry vozíků - stejné pro všechny vozíky
	{
			double delka;
			unsigned short typ;//0 - normální, 1 - závěsný
	};


	struct T_parametry_projektu //(Parametry výroby + Parametry linky (vozíky)
	{
		double cas_start;//začátek výroby v SEČ (resp. LSEČ)
		unsigned long  mnozstvi;//požadované množství
		double hod_den;//počet hodin za den
		double dni_rok;//počet hodin za den
		double efektivita;//přepokládaná výrobní efektivina
		double delka_voziku;
		double typ_voziku;//0 - normální, 1 - závěsný
	};
	T_parametry_projektu PP;


	struct TPalec
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			//double pozice;
			//struct TObjekt *segment;
			//přiřazení k dopravníku???
			struct TPalec *predchozi;
			struct TPalec *dalsi;
	};
	TPalec *PALCE;//spojový seznam palců na řetězu

	struct TG_objekt
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double X1,Y1,X2,Y2;//umístění objektu
		UnicodeString name;//název G_objektu
		double radius;//0==linie, 0> oblouk na jednu stranu, 0< oblouk na druhou stranu
		struct TG_Objekt *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TG_Objekt *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TG_objekt *G_OBJEKTY;//spojový seznam

	struct TDopravnik
	{
		//toto zv86it TPohonz* pohon;
		TG_objekt *g_objekt; //spojový seznam G_objektů, ze kterých se dopravní skládá
		TPalec *palec; //spojový seznam palců, příslušných k dopravníku - zvážit příslušnot v palcích
		struct TDopravnik *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TDopravnik *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TDopravnik *DOPRAVNIKY;

//	znovu zvážit
//	objekt1	->G_objekt1->dopravnik1
//					->G_objekt2->dopravnik1
//					->G_objekt3->dopravnik...

	struct TProces
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			unsigned long n_v_zakazce;//pořadí objektu v rámci zakázky
			double Tpoc;//X-počateční   //absolutní časy (i dále)
			double Tkon;//X-koncové
			double Tdor;//X-dorovnání předchozího vozíku
			double Tpre;//X- nutná doba přejezdu, zpoždění za předchozím vozíkem
			double Tcek;//X- nutná doba cekani na palec
			struct TZakazka *zakazka;//příslušnost procesu k zakázce
			struct TVozik *vozik;//ukazatel na vozík jehož proces vyjadřuje
			struct TProces *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TProces *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TProces *PROCESY;//seznam vygenerovaných procesů  v rámci časových os

	struct TOdstavka
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			double datum_od;
			double datum_do;
			struct TOdstavka *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TOdstavka *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TOdstavka ODSTAVKY;//seznam plánovaných odstávek linky

	struct TText
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			//unsigned int vrstva;//ID vrstvy
			UnicodeString text;//samotný text
			UnicodeString font;//název fontu
			double X, Y;//umístění objektu levý horní okraj
			unsigned short int size;
			unsigned short int styl;//0-nic, 1-bold,2-italic,3-oboje
			TColor barva;
			TColor barva_pozadi;
			struct TText *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TText *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TText TEXTY;

	struct TLinie
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			//unsigned int vrstva;//ID vrstvy
			double X1,Y1,X2,Y2;//umístění objektu levý horní okraj
			unsigned short int width;//šířka v px
			TColor barva;
			unsigned short int typ;//typ čáry
			struct TLinie *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TLinieText *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TLinie LINIE;//seznam linií sloužicích jako poznámky

///starý datový model
//		struct TObjekt
//		{
//			unsigned int n; //pořadí objektu ve spoj.seznamu
//			unsigned int id;   //id typu objektu
//			UnicodeString short_name;  //krátký název
//			UnicodeString name;  //celý název objektu
//			double X, Y;
//			unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní,3-stopka
//			double TTo; //takt time objektu
//			double CT; //cycle time
//			short stav;//0 - stop 2 - vozík může jet
//			unsigned short orientace_voziku; //0-na délku, 1- na šířku
//			double vzdalenost; //mezera mezi vozíky
//			short typ_dopravniku;
//			double delka_dopravniku;
//			double kapacita_objektu;
//			double dop_kapacita_objektu;//doporučená, vypočítáná
//			double obsazenost;//čas obsazenosti
//			UnicodeString techn_parametry;//obsah valuestringlistu, nelze ukládát pouze jednotlivá data, protože se seznam může u S&G měnit
//			struct TObjekt *predchozi;
//			struct TObjekt *dalsi;
//		};
//		TObjekt *OBJEKTY;
//
//		struct TCesta//seznam technologických objektů na cestě - definice cesty
//		{
//			unsigned int n; //pořadí + ID objektu ve spoj.seznamu
//			TObjekt *objekt;
//			double CT; //cycle time
//			double RD;//rychlost dopravníku
//			double R;//roztec
//			struct TCesta *predchozi;
//			struct TCesta *dalsi;
//		};
//		struct TSeznam_cest //seznam cest
//		{
//			unsigned int n; //pořadí cesty ve spoj.seznamu všech cest
//			TCesta *cesta;
//			TColor barva;
//			struct TSeznam_cest *predchozi;
//			struct TSeznam_cest *dalsi;
//		}; TSeznam_cest *CESTY;
//
//		struct TVozik
//		{
//			unsigned int n; //pořadí objektu ve spoj.seznamu
//			UnicodeString id;   //id typu objektu
//			double delka;
//			double sirka;
//			double vyska;
//			double rotace;
//			UnicodeString nazev_vyrobku;
//			double max_vyrobku;
//			double akt_vyrobku;
//			double delka_vcetne_vyrobku;
//			double sirka_vcetne_vyrobku;
//			double vyska_vcetne_vyrobku;
//			short stav;//-1 - není na lince, 0 - stop, 1 - čeká, 2 - jede
//			TColor barva;
//			double start;//výchozí pozice vozíku v grafu časové osy
//			double pozice;//pozice na dopravniku či v grafu
//			double X;
//			double Y;
//			double timer;
//			struct TObjekt *segment;
//			struct TSeznam_cest *cesta;
//			struct TVozik *predchozi;
//			struct TVozik *dalsi;
//		};
//		TVozik *VOZIKY;
//
//		struct TProces
//		{
//			unsigned int n; //pořadí objektu ve spoj.seznamu
//			unsigned int n_v_zakazce;//pořadí objektu v rámci zakázky
//			double Tpoc;//X-počateční
//			double Tkon;//X-koncové
//			double Tdor;//X-dorovnání předchozího vozíku
//			double Tpre;//X- nutná doba přejezdu, zpoždění za předchozím vozíkem
//			double Tcek;//X- nutná doba cekani na palec
//			struct TCesta *cesta; //při změna datového modelu spíše zakazka
//			struct TVozik *vozik;
//			struct TProces *predchozi;
//			struct TProces *dalsi;
//		};
//		TProces *PROCESY; //zadaný CT - obsažen v cestě, objekt obsažen v cestě
//
//		struct TPalec
//		{
//			unsigned long n; //pořadí objektu ve spoj.seznamu
//			double pozice;
//			struct TObjekt *segment;
//			struct TPalec *predchozi;
//			struct TPalec *dalsi;
//		};
//		TPalec *PALCE;


		struct TFile_hlavicka
		{
					unsigned short int Verze;
					unsigned short int Mod;
					double Zoom;
					long PosunutiX;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
					long PosunutiY;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
					unsigned int pocet_objektu;
					unsigned int pocet_pohonu;
					//unsigned int pocet_voziku;
					//parametry projektu:
					double cas_start;//začátek výroby v SEČ (resp. LSEČ)
					unsigned long  mnozstvi;//požadované množství
					double hod_den;//počet hodin za den
					double dni_rok;//počet hodin za den
					double efektivita;//přepokládaná výrobní efektivina
					double delka_voziku;
					double typ_voziku;
		};
		TFile_hlavicka File_hlavicka;

//konstruktor
		Cvektory();

//metody pro OBJEKTY
		void hlavicka_OBJEKTY();
		short vloz_objekt(unsigned int id, double X, double Y);//vloží prvek do seznamu
		short vloz_objekt(TObjekt *Objekt);//přetížená fce
		short vloz_objekt(unsigned int id, double X, double Y,TObjekt *p);//přetížená fce vkládá mezi objekty
		TObjekt *najdi_objekt(double X, double Y,double offsetX, double offsetY);//hledá bod v dané oblasti
		short smaz_objekt(TObjekt *Objekt);//smaže prvek ze seznamu
		void sniz_indexy(TObjekt *Objekt);
		void zvys_indexy(TObjekt *Objekt);
		long vymaz_seznam_OBJEKTY();

//metody pro POHONY
		void hlavicka_POHONY();
		void vloz_pohon(TPohon *pohon);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
		void vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double roztec);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
		TPohon *vrat_pohon(unsigned long n);//vrátí ukazatel na pohon dle n pohonu
		long vymaz_seznam_POHONY();//smaže jednotlivé prvky seznamu, včetně hlavičky, pokud následuje další práce se seznamem, je nutné založit nejdříve hlavičku pomocí hlavicka_pohony()
//		void vymazat_casovou_obsazenost_objektu_a_pozice_voziku(TObjekt *Objekt,TVozik *Vozik);
//		double delka_dopravniku(Cvektory::TObjekt *ukaz);

//metody pro ZAKAZKY
public:
		void vloz_temp_zakazku(UnicodeString id,unsigned short typ, UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//vytvoří zakázku dle zadaných parametru do spojového seznamu ZAKÁZKY
		void edituj_temp_zakazku(unsigned long n,UnicodeString id,unsigned short typ,UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//provede editaci zakázky s uvedeným “n” ze spojového seznamu ZAKAZKY_temp
		void smaz_temp_zakazku(unsigned long n);//smaže zakázku s uvedeným “n” ze spojového seznamu ZAKAZKY_temp včetně přidružených cest
		void zmen_poradi_temp_zakazky(unsigned long aktualni_poradi,unsigned long nove_poradi);//změní zařazení zakázky ve spojovém seznamu
		void kopirujZAKAZKY_temp2ZAKAZKY();//po stisku OK v superformu zkopíruje data z ZAKAZKY_temp do ZAKAZKY
private:
		void hlavicka_ZAKAZKY();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY
		void hlavicka_ZAKAZKY_temp();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY_temp, nutná volat při on_show superformuláře
		void vloz_temp_zakazku(TZakazka *Zakazka_temp);//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
		long vymaz_seznam_ZAKAZKY();//smaze seznam ZAKAZKY z paměti v četně přidružených cest, pokud následuje další práce se seznamem (např. nové vkládání), je nutné založit nejdříve hlavičku pomocí hlavicka_ZAKAZKY()
		long vymaz_seznam_ZAKAZKY_temp();//smaze seznam ZAKAZKY_temp z paměti včetně přidružených cest, nutno implementovat při close() superformu (ať už při OK, storna, či křížku formu)
//metody pro cesta konkrétní zakázky
public:
		void hlavicka_cesta_zakazky(TZakazka *Zakazka);//vytvoří novou hlavičku pro spojový seznam konkrétní cesty dané zakázky
		TZakazka *vrat_temp_zakazku(unsigned long n_zakazky);// vrátí ukazatel (resp. data) na editovanou zakázku
		inicializace_cesty(TZakazka *Editovana_zakazka);//volat pouze jednou v počátku metody při stisku OK, vymaže předchozí cestu - pokud existuje, vytvoří hlavičku cesty. Příklad použití metody: inicializace_cesty(Editovana_zakazka);
		void vloz_segment_cesty(TZakazka *Editovana_zakazka,unsigned long n_vybraneho_objektu/*z comboboxu*/,double CT,double Tc,double Tv,double RD);//do konkrétní cesty vloží segmenty cesty,  bude užito v metodě při stisku OK, při vkládání každého řádku stringgridu v daném for cyklu.
		void vloz_segment_cesty(TZakazka *Zakazka,TCesta *Segment_cesty);//do konkrétní cesty vloží segmenty cesty

//		TSeznam_cest *vrat_cestu(unsigned int ID_cesty);

			//metody pro PROCESY
//		void hlavicka_procesy();
//		void vloz_proces(TProces *Proces);
//		TProces *najdi_proces(double cas, double vozik);//hledá bod mezi procesy
//		TProces *vrat_nasledujici_proces_objektu(TProces *Proces);//vratí následující proces na stejném objektu jako proces zadaný
//		long vymaz_seznam_procesu();

			//metody pro VOZIKY
//		void hlavicka_voziky();
//		void vloz_vozik();//přidá nový prázdný - nedefinovaný vozík do seznamu VOZIKY
//		void vloz_vozik(TVozik *Vozik);
//		void vloz_vozik(unsigned long n,UnicodeString id,double delka,double sirka,double vyska,double rotace,UnicodeString nazev_vyrobku,double max_vyrobku,double akt_vyrobku,double delka_vcetne_vyrobku,double sirka_vcetne_vyrobku,double vyska_vcetne_vyrobku,TColor barva,TSeznam_cest *cesta);
//    long vymaz_seznam_voziku();
//		void hlavicka_palce();
//		void vloz_palec();//přidá nový vozík do seznamu PALCE
//		long vymaz_seznam_palcu();

    //odstraní všechny vektory (všechny globální spojáky)
		void vse_odstranit();

		//souborové operace
		short int uloz_do_souboru(UnicodeString FileName);
		short int nacti_ze_souboru(UnicodeString FileName);
		short int ulozit_report(UnicodeString FileName);

//		//technické, statistické a ekonomické ukazatele
//		void get_LT_a_max_min_TT();
//		double sum_WT();
//		double LT;
//		double MAX_TT;
//		double MIN_TT;
//		TPointD vrat_zacatek_a_konec_zakazky(TSeznam_cest *jaka);//ukazatel na cestu resp, zakázku
//		TPointD vrat_zacatek_a_konec_zakazky(unsigned int n_zakazky);//n resp. ID cestu resp, zakázku
//		double vrat_nejpozdejsi_konec_zakazek();//nemusí se vždy jednat o poslední zakázku
//		double vrat_LT_voziku(TVozik *jaky);//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
//		double vrat_LT_voziku(unsigned int n_voziku);//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
//		double vrat_sumPT_voziku(TVozik *jaky);//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
//		double vrat_sumPT_voziku(unsigned int n_voziku);//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
//		double vrat_AVGsumPT_zakazky(TSeznam_cest *jaka);//vrátí čistý čas, který strávily vozík ve výrobě bez čekání pro danou zakazku
//		double vrat_WT_voziku(TVozik *jaky);//vrátí čas čeká vozíku během výroby
//		double vrat_WT_voziku(unsigned int n_voziku);//vrátí čas čeká vozíku během výroby
//		double vrat_AVGsumWT_zakazky(TSeznam_cest *jaka);//vrátí čistý čas, který vozíky čekaly pro danou zakazku
//		double vrat_TT_voziku(TVozik *jaky);//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
//		double vrat_TT_voziku(unsigned int n_voziku);//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
//		double vrat_AVG_TT_zakazky(TSeznam_cest *jaka);//vrátí hodnotu průměrného TT mezi vozíky v rámci dané zakázky/cesty
//		double vrat_AVG_TT_zakazky(unsigned int n_zakazky);//vrátí hodnotu průměrného TT mezi vozíky v rámci
//		unsigned int vrat_pocet_voziku_zakazky(TSeznam_cest *jaka);
//		unsigned int vrat_pocet_voziku_zakazky(unsigned int n_zakazky);
//		unsigned int WIP();//vrátí max. počet vozíků na lince
//		void uloz_doporucene_kapacity_objetku();

//pomocné struktury pro ukládání do bináru
private:
		struct C_pohon//pro konverzi do bináru
		{
				unsigned int n;
				unsigned int text_length;
				double rychlost_od;//minimální pracovní rychlost dopravníku
				double rychlost_do;//maximální pracovní rychlost dopravníku
				double roztec;//rozteč palců v mm
		};
		struct C_objekt//pro konverzi do bináru
		{
				unsigned int n;
				unsigned int id;
				//UnicodeString short_name; nelze
				//UnicodeString name; nelze
				unsigned int text_length;
				double X, Y;
				unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
				double kapacita;
				double kapacita_dop;
				unsigned int pohon;//"id" resp. n přidruženého - roletkou vybraného pohonu
				double delka_dopravniku;//delka dopravníku v rámci objektu
				unsigned short cekat_na_palce;//0-ne,1-ano,2-automaticky
				bool stopka;//zda následuje na konci objektu stopka
				double odchylka;//povolená odchylka z CT (hlavně užito u PP)
		};
//		struct C_vozik//pro konverzi do bináru
//		{
//			unsigned int n; //pořadí objektu ve spoj.seznamu
//			unsigned int id_length;   //délka řetězce id
//			double delka;
//			double sirka;
//			double vyska;
//			double rotace;
//			unsigned int text_length;
//			double max_vyrobku;
//			double akt_vyrobku;
//			double delka_vcetne_vyrobku;
//			double sirka_vcetne_vyrobku;
//			double vyska_vcetne_vyrobku;
//			short stav;//-1 - není na lince, 0 - stop, 1 - čeká, 2 - jede
//			TColor barva;
//		};
//
//		AnsiString get_csv_xls(AnsiString S);
//		AnsiString get_html();
//		unsigned int vrat_kapacitu_objektu(TObjekt *O);//stačí v sekci private, protože ukládám přímo přímo do atributů objektu pomocí uloz_doporucene_kapacity_objetku();
//
//	protected:

};
//---------------------------------------------------------------------------
#endif
