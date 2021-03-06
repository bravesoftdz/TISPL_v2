﻿//---------------------------------------------------------------------------
#ifndef vektoryH
#define vektoryH
#include <vcl.h>
#include "knihovna_objektu.h"
#include "my.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class Cvektory
{
  private:
	Cmy m;//instance na my knihovnu

	public:

	struct TBod
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double X, Y;//umístění v logických (metrických) souřadnicích
		T3Rect kota;//citelná oblast hodnoty kóty od aktuálního bodu k bodu předchozímu ve fyzických souřadnicích (px), uchovávájící oblast celé kóty(rect0), popisku kóty (rect1), jednotek kóty (rect2), neukládat do binárky, slouží jenom jako temp data při vykreslování
		double kota_offset;//odsazení kót v metrech
		struct TBod *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TBod *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct THala
	{
		UnicodeString name;//název
		double X, Y;//umístění názvu v logických (metrických) souřadnicích
		TBod *body;//definice obrysu haly
	};THala HALA;

	struct TGeometrie//pouze struktura
	{
		short  typ;//0 - linie, 1 - oblouk, -1 neidentifikovatelný tvar pomocí bézieru
		double delka;//skutečná délka dopravník
		double delkaPud;//půdorysní délka, použita pouze pokud je odlišná od délky, tj. je obsaženo stoupání
		double HeightDepp;//výška stoupání čí hloubka klesání
		double radius;
		double orientace;
		double rotacni_uhel;
		double X1,Y1,X2,Y2,X3,Y3,X4,Y4;//body bézieru, v případě linie X1==X2, Y1==Y2, X3==X4, Y3==Y4
	};

	struct TRetez//resp. geometrie pouzdra řetězu
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		TGeometrie geo;
		unsigned int eID; //id typu elementu viz. tabulka elementů https://docs.google.com/spreadsheets/d/1_S7yp1L25tov0mKqP3Rl_0Y2cx-e3UwDXb102hDvRlA/edit?usp=sharing
		struct TRetez *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TRetez *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct TPalec//jednotlivé palce, v podstatě skutečný řetěz
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double X,Y;//umístění
		double orientace;
		short stav;//stav palce aktivní - 1 má vozík, pasivní
		//double pozice;
		struct TPalec *predchozi;
		struct TPalec *dalsi;
	};
	//TPalec *PALCE;//spojový seznam palců na řetězu

	struct TPohon
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		UnicodeString name;//název
		double rychlost_od;//minimální pracovní rychlost dopravníku
		double rychlost_do;//maximální pracovní rychlost dopravníku
		double aRD;//aktuální rychlost dopravníku m/s
		double roztec;//rozteč palců v m
    unsigned int roztec_ID;//uchovává v sobě ItemIndex rozteče
		double Rz;//rozestup aktivních palců v m
		double Rx;//rozestup aktivních palců (počet aktivních palců)
		double umisteni;//umisteni retezu resp. palců, tempová proměnná pro simulaci
    short rezim;////rezim objektu 0-S&G,1-Kontin., -1 nenastaven
		TRetez *retez;//geometrie pohonu, řetězu resp. pouzdro řetězu
		TPalec *palec;//jednotlivé palce v podstatě ve skutečnosti řetěz
		struct TPohon *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TPohon *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TPohon *POHONY;//spojový seznam pohonů

	struct TKomora// - NEW + dodat do CObjekt v rámci rodičovských spojáků!!!!
	{
		unsigned long n; //pořadí ve spoj.seznam
		double velikost;//v metrech
		short typ;//1-se sprchou, 0 bez jen okap
		T3Rect kota;//citelná oblast hodnoty kóty ve fyzických souřadnicích (px), uchovávájící oblast celé kóty(rect0), popisku kóty (rect1), jednotek kóty (rect2), neukládat do binárky, slouží jenom jako temp data při vykreslování
		struct TKomora *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TKomora *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct Tdata//data elementu, která se mění v závislosti na zakázce
	{
		short PD;//part detect:  -1 = nic, 0 = začátek jigu, 1 = střed jigu, 2 = celý jig
		short orientace_jig_pred;//v jaké orientaci je JIG před elementem na obrazovce vykreslen 0,90,180,270 (orientace dle světových stran)
		double LO1;//lakovací okno - pracovní oblast
		double LO2;
		double LO_pozice;//vyosení lakovacího okna - pracovní oblasti
		double PT1;//procesní čas
		double PT2;
		double WTstop;//čekání na stopce
		double RT;//reserve time (ryzí,pokrácený)
		unsigned int pocet_pozic;//daný geometrií před stopstanicí (maximální možný počet vozíků, které lze nabufrovat)
		unsigned int pocet_voziku;//počet vozíků v buffru
	};

	struct TElement
	{
		unsigned long n; //pořadí ve spoj.seznamu
		unsigned int eID; //id typu elementu viz. tabulka elementů https://docs.google.com/spreadsheets/d/1_S7yp1L25tov0mKqP3Rl_0Y2cx-e3UwDXb102hDvRlA/edit?usp=sharing
		unsigned int identifikator_vyhybka_spojka;
		UnicodeString short_name;//krátký název max. 4 znaky
		UnicodeString name;//celý název objektu
		short orientace;//v jaké orientaci je element na obrazovce vykreslen 0,90,180,270 (orientace dle světových stran)
		double rotace_jig;//úhel o který element orotuje jig vzhledem k jeho aktuální rotaci jigu vůči podvozku, např. rotace_jig=90°, aktuální rotace jigu 90°, výsledek 180° - REVIZE!!!
		short stav;//stav elementu: -1 = disabled, nebo nepoužito např. element v knihovně, nebo nepoužitý robot v kabině 0 = stopka otevřeno, v případě elementů bez stopky pouze pasivní element 1 = stopka zavřeno, v případě elementů bez stopky pouze aktivní element 2 = highlight, element zvýrazněn, nikoliv jeho název (předpokladá se při klasickém zobrazení tj. typ==1) 3 = highlight pouze názvu elementu

		double X, Y, Z;//umístění v logických (metrických) souřadnicích
		double Xt,Yt;//umístění tabulky, resp. mGridu v logických (metrických) souřadnicích

		double PTotoc;//čas otáčení na otoči
		double OTOC_delka;//fyzycká délka otoče
		double zona_pred;//zóna od počátku otáčení do středu otoče
		double zona_za;//zona od středu otoče do konce otáčení

		double WT;//čekání na palec pro hlavní větev
		double WT_index;//index, určuje pro který pohon je WT určeno, 1 - prvni pohon (pohon před PM, V), 2 - druhý pohon (sekundární větev, pohon za PM), defaultně 0 nepřiřazeno

		unsigned int VID;//validační ID, uchovává ID validace v mGridu, např. dopRD
    double VID_value;//uchovává v sobě hodnotu pro validaci, např. hodnotu dopRD

		T8Rect citelna_oblast;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast celé kóty(rect0), popisku kóty (rect1), jednotek kóty (rect2) a samotného názvu např. Robot 3 (rect3) elementu, ohodnoty koty mezi LO (rect4), textu na kótě mezi LO (rect5) - popisek kóty mezi LO, v případě teploměrů, uložena oblast popisku °C, Z - souřadnice aktualní element (počátek stoupání či klesání) rect6, Z - souřadnice další element (konec stoupání či klesání) - rect 7, HeightDeep hodnota - rect 8 --- strukturu nedovávat  do CObjekt

		TGeometrie geo;//geometrie elementu, ale ta která předchází, tzn. X==X4,Y==Y4
		TmGrid *mGrid;
    Tdata data;//data elementu, která se mění v závislosti na zakázce

		unsigned long objekt_n;//příslušnost elementu k objektu
		TPohon *pohon;//přiřazený pohon k elementu

		double temp,temp2;//provizorní záležitost pro testovací účely, není ukládáno do binárky

		struct TElement *sparovany;//ukazatel na následující spárovaný element ve spojovém seznamu (nemusí být totožný s dalším)
		struct TElement *predchozi;//ukazatel na předchozí element ve spojovém seznamu
		struct TElement *predchozi2;//ukazatel na předchozí element ve spojovém seznamu
		struct TElement *dalsi;//ukazatel na  další element ve spojovém seznamu
		struct TElement *dalsi2;//ukazatel na  další element ve spojovém seznamu
	};
	TElement *ELEMENTY;//seznam elementů

	struct T2Element
	{
		unsigned long n;
		TElement *vyhybka;
		unsigned long vyhybka_pocet;//uchovává informaci kolikrát jsem byl na výhybce
		TElement *spojka;
    unsigned long spojka_pocet;//uchovává informaci kolikrát jsem byl na spojce
		T2Element *dalsi;
		T2Element *predchozi;
	};

  struct TCesta//pouze přidružený spoják, který je součástí zakázky, jeden objekt spojáku je jeden segment cesty
	{
		unsigned long n;
		Tdata data;//data elementu pro konkrétní zakázku
		struct TElement *Element;//element v segmentu cesty
		struct TElement *sparovany;//spárovaný element k elementu, opět unikátní pro zakázku

		struct TCesta *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TCesta *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TCesta *MAG_LASO;

	struct TTeplomery//uchovává v sobě teploměry a jejich cestu pro různé zakázky
	{
		unsigned long n;
		unsigned long Z_n;
		TElement *prvni;
		TElement *posledni;
		TCesta *cesta;
		TTeplomery *dalsi;
		TTeplomery *predchozi;
	};

	struct TObjekt
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		unsigned int id; //id typu objektu
		UnicodeString short_name;//krátký název max. 4 znaky         //DOPRYC
		UnicodeString name;//celý název objektu
		double X,Y;//umístění objektu ve schématu - prozatím, bude ale sloužit na umístění popisku objektu!!!
		TBod *body;//definice obrysu objektu
		double sirka_steny;//šířka stěny kabiny objektu v metrech  - NEW + dodat do CObjekt!!!!
		double Xk,Yk;//bude ke smazání - umístění levého horního rohu kabiny v layoutu a náhledu kabiny  //DOPRYC
		double Xt,Yt;// souřadnice popisku objektu pro možnost posouvání dodat!!!   //PRIDAT
		double orientace_text;//orientace textu (názvu objektu) dle světových stran
		double Xp,Yp;// souřadnice tab pohonu pro možnost posouvání dodat!!!   //PRIDAT
		short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking)KK,2-Postprocesní (PP), -1 nenastaven, pouzíva tab.pohonu
		short stavPM;//obsahuje stav pohonových tabulek v objektu 0-normal (zobrazeny rychlosti a comba, dafaultní stav), 1-minimized (pouze hlavička tabulek a comba), 2-maximized (vše zobrazeno)
		double CT;//pro status návrh   //DOPRYC
		double RD;//pro status návrh v m/s, jenom pomocná proměnná získaná jako DD/CT, stežejní je většinou aRD (aktuální rychlost), která se váže přímo (i datově) k pohonu  //DOPRYC
		double delka_dopravniku;//delka dopravníku v rámci objektu   //DOPRYC
		double kapacita;//uživatelsky zadaná kapacita  //DOPRYC
		double kapacita_dop;//doporučená, vypočítáná  //DOPRYC
		double pozice;//počet vozíků v kabině  //DOPRYC
		double rotace;//rotace jigu v objektu - nově spíše rotace náhledu  - //DOPRYC
		double orientace;//0,90,180,270 (orientace dle světových stran) - používat toto
		double mezera;//mezera mezi vozíky (kritická mezera) //DOPRYC
		double mezera_jig;//mezera mezi jigy //DOPRYC
		double mezera_podvozek;//mezera mezi podvozky  //DOPRYC
		TPohon *pohon;//ukazatel na použitý pohon
		TElement *element;//ukazatel na první element objektu
		unsigned long element_n;//slouží pro uchování n elementu, při načítání z binárky jsou objekty načtené jako první, poté elementy, naposledy je třeba aktualizovat objektům ukazatel na element pomocí tohoto n
		TPointD min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
//		TPointD rozmer_kabiny;//délka a šířka obvodových zdí kabiny   //DOPRYC
		T2Rect kabinaKotaX_oblastHodnotaAJednotky;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast popisku a jednotek kóty kabiny -//DOPRYC
		T2Rect kabinaKotaY_oblastHodnotaAJednotky;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast popisku a jednotek kóty kabiny -//DOPRYC
		TPointD koty_elementu_offset;//.x=odsazení kót elementů v metrech normální stav, .y=odsazení kót elementův metrech editace geometrie  - NEW + dodat do CObjekt!!!!
		TKomora *komora;//ukazatel na případné komory objektu - NEW + dodat do CObjekt
    TTeplomery *teplomery;
		unsigned short cekat_na_palce;//0-ne,1-ano,2-automaticky   //DOPRYC
		unsigned short stopka;//zda následuje na konci objektu stopka //0-ne,1-ano,2-automaticky   //DOPRYC
		double odchylka;//povolená odchylka u PP z CT  //DOPRYC
		double obsazenost;//slouží pro uchování času obsazenosti pro vykreslování na časových osách //DOPRYC
		short CT_zamek; //DOPRYC
		short RD_zamek;  //DOPRYC
		short DD_zamek;   //DOPRYC
		short K_zamek;   //DOPRYC
		UnicodeString poznamka;//uloží poznámku ke vzniku CT   //DOPRYC
		bool probehla_aktualizace_prirazeni_pohonu;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
		bool zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty
		bool zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy
		bool uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu
		struct TObjekt *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		//struct TObjekt *predchozi2;//ukazatel na předchozí2 objekt ve spojovém seznamu
		struct TObjekt *dalsi;//ukazatel na  další objekt ve spojovém seznamu
		//struct TObjekt *dalsi2;//ukazatel na další2 objekt ve spojovém seznamu
	};
	TObjekt *OBJEKTY;//seznam objektů

	struct TJig//pouze složený datový typ
	{
		double sirka;
		double delka;
		double vyska;
		double ks;//kusů
	};

	struct TCesta_uloz//ukladání cesty
	{
		unsigned long n;
		unsigned long element_n;
		unsigned long sparovany_n;
		Tdata data;

		struct TCesta_uloz *predchozi;
		struct TCesta_uloz *dalsi;
	};

	struct TDavka
	{
		unsigned long n;
		unsigned long pocet_voziku;
		unsigned long pocet_prazdnych;
		unsigned long pocet_celkem;
		TDavka *dalsi;
		TDavka *predchozi;
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
		TmGrid *mGrid;
		TDavka *davky;
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
	TZakazka *ZAKAZKA_akt;//udržuje aktuální zakázku (uživatelskou nebo defaultní)

	struct TZakazka_uloz
	{
		unsigned long n;//pořadí objektu ve spoj.seznamu
		struct TCesta_uloz *cesta;
		struct TZakazka_uloz *predchozi;
		struct TZakazka_uloz *dalsi;
	};

	struct TVozik
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double X,Y;//umístění
		double orientace_podvozek;//orientace podvozku dle pohybu na lince
		double rotace_jig;//rotace jigu vuči podvozku (nehledí na skutečnou orientaci jigu)
		short typ;//1-normální, 0 - prazdný
		short stav;//narazil do vozíku před ním vyhákává či již je po nárazu vyháknutý -2, vyháknutý -1, čeká na palec 0, jede 1
		struct TElement *element;//na jekém segmentu elementu se momentálně nachází
		struct TPalec *palec;//pokud je uchycen k palci, tak k jakému
		struct TZakazka *zakazka;//ukazatel na přidruženou zakázku
		struct TVozik *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TVozik *dalsi;//ukazatel na další objekt ve spojovém seznamu
	};
	TVozik *VOZIKY;//spojový seznam vozíků

	struct TVozik_parametry//paremtry vozíků - stejné pro všechny vozíky
	{
		double delka;
		unsigned short typ;//0 - podlahový, 1 - závěsný
	};

	struct T_raster
	{
		UnicodeString filename;//adresa umístění podkladového rastru na disku
		long double resolution;//rozlišení metrů na jeden pixel rastrového podkladu
		double X,Y;//logické souřadnice (v metrech) umístění rastrového podkladu v projektu
		bool show;//indikace zda je raster zobrazen
		bool grayscale;//zda bude či nebude rastrový podklad v odstínech šedi či nikoliv
		int dim;//úroveň ztlumení
	};

	struct T_vector //DOPRYC //zakomentovat
	{
		int vector;//úroveň ztlumení vektorů - zatím nebude využito
		bool transparent;//zda bude vektorová vrstva transparentní - zatím nebude využito
	};

	struct TmyPx
	{
		unsigned short X;
		unsigned short Y;
		unsigned short o/*opakování*/;
		unsigned char R;
		unsigned char G;
		unsigned char B;
		TmyPx *dalsi;
	};

	struct T_buffer_simulace//slouží k nabuffrování jednotlivých bmp obrazů kroků animované simulace
	{
		unsigned long n;//pořadí prvku
		//Graphics::TBitmap *raster;//ukazatel na nabuffrovanou bitmapu
		//TPngImage* raster;
		TmyPx *raster;
		struct T_buffer_simulace *predchozi;//ukazatel na předchozí prvek ve spojovém seznamu
		struct T_buffer_simulace *dalsi;//ukazatel na  další prvek ve spojovém seznamu
	};
	T_buffer_simulace *sBUFFER;//slouží k nabuffrování jednotlivých bmp obrazů kroků animované simulace
	T_buffer_simulace *sIterator;//ukazovátko aktuálně načítaného rastru


	struct T_parametry_projektu //(Parametry výroby + Parametry linky (vozíky)
	{
    int katalog;//ID katalogových komponent zvolených pro geometrické elementy linky
    short typ_linky; //0 - podlahový, 1 - podvěsný
		double radius;//nastavený radius komponent linky z katalogu
		TDateTime cas_start;//začátek výroby v SEČ (resp. LSEČ)
		UnicodeString vytvoril;//jmeno autora
		TDateTime cas_posledni_upravy;//čas a datum poslední úpravy v SEČ (resp. LSEČ)
		UnicodeString upravil;//jmeno autora poslední úpravy
		unsigned long  mnozstvi;//požadované množství
		double hod_den;//počet hodin za den
		double dni_rok;//počet hodin za den
		double efektivita;//přepokládaná výrobní efektivina
		double TT;//takt time linky
		double delka_jig;//přejmenovat na max_...
		double sirka_jig;//přejmenovat na max_...
		double vyska_jig;//přejmenovat na max_...
		double delka_podvozek;//délka podvozku všech vozíků
		double sirka_podvozek;//šířka podvozku všech vozíků - zatím není ukládáno do binárky, protože se uživatelsky nenastavuje
		double uchyt_pozice;//pozice uchycení vozíků k nosnému palci
		bool zamek_layoutu;//zámek editace layoutu
		T_raster raster;
		T_vector vector;//DOPRYC - zakomentovat
    UnicodeString autor;//nazev autora projektu
	};
	T_parametry_projektu PP;

	struct TProces //možno odstranit
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		unsigned long n_v_zakazce;//pořadí objektu v rámci zakázky
		double Tpoc;//X-počateční   //absolutní časy (i dále)
		double Tkon;//X-koncové
		double Tdor;//X-dorovnání předchozího vozíku
		double Tpre;//X- nutná doba přejezdu, zpoždění za předchozím vozíkem
		double Tcek;//X- nutná doba cekani na palec
		double Trand;//hodnota vypočitán dle funkce náhodná doba čekání na palec
		struct TCesta *segment_cesty;//příslušnost procesu k vyjmutému (ze spojáku) segmentu cesty (tedy objektu)
		struct TVozik *vozik;//ukazatel na vozík jehož proces vyjadřuje
		struct TProces *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TProces *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TProces *PROCESY;//seznam vygenerovaných procesů  v rámci časových os

	struct TOdstavka//možno odstranit
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double datum_od;
		double datum_do;
		struct TOdstavka *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TOdstavka *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TOdstavka ODSTAVKY;//seznam plánovaných odstávek linky

	struct TDoubleHodnota//pomocná struktura níže uvedené
	{
		unsigned int n; //pořadí objektu ve spoj.seznamu
		double hodnota;//double hodnota
		struct TDoubleHodnota *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TDoubleHodnota *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct Ttyp_dopravniku// - NEW + dodat do CObjekt
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		short druh;//0 - podlahový, 1 - podvěsný
		UnicodeString name;//název
		UnicodeString link;//hypertextový odkaz na katalog
		TDoubleHodnota *roztec;//přidružený spojový seznam možných roztečí
		TDoubleHodnota *hOblouk;//přidružený spojový seznam horizontálních oblouků
		TDoubleHodnota *hRadius;//přidružený spojový seznam horizontálních radiusů
		TDoubleHodnota *vOblouk;//přidružený spojový seznam vertikálních oblouků
		TDoubleHodnota *vRadius;//přidružený spojový seznam vertiálních radiusů
		struct Ttyp_dopravniku *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct Ttyp_dopravniku *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};Ttyp_dopravniku *KATALOG;

	struct TText
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		int vrstva;//ID vrstvy, slouží na filtr vykreslení (např. layout vs. editace)
		double X, Y;//umístění objektu levý horní okraj
		short zarvonani;//typ zarovnání textu vůči X,Y
		UnicodeString text;//samotný text
		TFont *Font;//vlastnosti font (název, velikost, rotace, barva, styl)
		//použít na CFont:unsigned short int size;
		//použít na CFont:unsigned short int styl;//0-nic, 1-bold,2-italic,3-oboje
		//použít na CFont:TColor barva;
		TColor barva_pozadi;
		struct TText *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TText *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TText *TEXTY;

	struct TSpojnice
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		int vrstva;//ID vrstvy (např. layout vs. editace)
		short zakonceni;//0 - nebude zakončeno, 1- bude zakončeno šipkou, popř. další možné typy doplnit
		short typ;//0-liniová spojnice, 1-bézierová spojnice
		TBod *body;//souřadnice vedení šipky
		unsigned short int width;//šířka v px
		TColor barva;
		short cara_typ;//typ čáry
		struct TSpojnice *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TSpojnice *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TSpojnice *SPOJNICE;//seznam linií sloužicích pro účely poznámek

	struct TZprava
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double X,Y;//pozice zprávy ve fyzických souřadnicích
		TElement *Element;//ukazatel na element, ke kterému se zpráva váže
		short zID;//typ zprávy -1 - error, 1 - warning
		long VID;//typ validačního výpisu
		double VIDvalue;//případná doporučená hodnota
		HRGN citelna_oblast;//pouze pomocná proměnná ve fyzických souřadnicích (px) uchovávájící oblast textu zprávy
		struct TZprava *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TZprava *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TZprava *ZPRAVY;//seznam ZPRÁV

	struct TFile_hlavicka
	{
		double FileVersion;
		double ProductVersion;
		unsigned short int Mod;
		double Zoom;
		long PosunutiX;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
		long PosunutiY;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
		unsigned int pocet_objektu;
		unsigned int pocet_elementu;
		unsigned int pocet_vyhybek;//uchovává počet výhybek a spojek pro tvoření pole průchodu při rekonstrukci elementů
		unsigned int pocet_pohonu;
		unsigned int pocet_zakazek;
		unsigned int pocet_voziku;
		//parametry projektu (PP):
		TDateTime cas_start;//začátek výroby v SEČ (resp. LSEČ)
		unsigned int vytvoril_Sdelka;//délka jmena autora
		TDateTime cas_posledni_upravy;//čas a datum poslední úpravy
		unsigned int upravil_Sdelka;//délka jména autora poslední úpravy
		unsigned long  mnozstvi;//požadované množství
		double hod_den;//počet hodin za den
		double dni_rok;//počet hodin za den
		double efektivita;//přepokládaná výrobní efektivina
		double TT;//globální TT linky
		double delka_jig;
		double sirka_jig;
		double vyska_jig;
		double delka_podvozek;
		double sirka_podvozek;
		double uchyt_pozice;
		bool zamek_layoutu;//zámek editace layoutu
    int katalog;
		short typ_linky;
    double radius;
		short objekt_posunout_vse;
		unsigned long zakazka_akt;//uchovává n aktuální zakázky
		//UnicodeString autor;//nazev autora projektu
	};
	TFile_hlavicka File_hlavicka;

	struct TDATA//obraz datového modelu projektu
	{
		unsigned long n;
		unsigned long edit_Objekt;//uchovává informaci zda je otevřena editace a jakého objektu
		unsigned long posledni_element_n;//uchovává n posledního elementu v editovaném objektu, pro porovnání zda se změnil počet v projektu vs. obraz
		unsigned int pocet_vyhybek;
		TObjekt *Objekty;
		TElement *Elementy;
		TPohon *Pohony;
		TZakazka_uloz *Z_cesty;//uchovává v sobě data cest k jednotlivým zakázkám

		TDATA *dalsi;
		TDATA *predchozi;
	};
	TDATA *DATA;

	struct TSIM
	{
		unsigned long n;
		double T1;//čas vstup
		double T2;//čas výstup
		short A;//akce
		TElement *S;//stopka
		TVozik *V;//vozík
		TSIM *dalsi;
		TSIM *predchozi;
	};
	TSIM *SIM;


//konstruktor
	Cvektory();

//společné metody pro HALU a OBJEKT, pokud je ukazatel na Objekt NULL, jedná se o metody pro HALU
	void hlavicka_body(TObjekt *Objekt=NULL);//pomocná metoda, není třeba volat, volá se automaticky při vkládání prvního bodu při vloz_bod, pokud je ukazatel na Objekt NULL, jedná se o metody pro HALU
	void vloz_bod(TBod *Bod,TObjekt *Objekt=NULL);//vloží nový bod na konec seznamu bodů, pokud je ukazatel na Objekt NULL, jedná se o metody pro HALU
	void vloz_bod(double X, double Y,TObjekt *Objekt=NULL,TBod *ZaBod=NULL, bool ortogonalizovat=true,bool konec=false);//vloží nový bod na konec seznamu bodů pokud je Za=NULL, jinak vloží za tento bod, ošetřuje bod vložený na stejný místo jako předchozí, či jako první, pokud se jedná o poslední vložení při uzavírání polygonu a je zapnuta ortogonalizace, je zajištěno, aby byl první poslední a předposlední bod v ortogonalizovaném vztahu, zajištění poslední spojnice zajištuje vykreslovací metoda, pokud jsou vloženy pouze 3 body a ukončeno vkládání je dopočítán 4 bod do rozměrů obdélníku
	void posun_bod(double X, double Y,TBod* Bod);//posune bod HALy nebo Objektu, záleží, kam bod náleží
	void posun_hranu(double OffsetX,double OffsetY,TBod* Bod1,TBod* Bod2);//posune hranu tvořenou danými body o zadaný offset
	void posun_body(double OffsetX,double OffsetY,TObjekt* Objekt=NULL);//posune všechny body polygonu objektu či haly o daný offset
	void rotuj_body(double X, double Y,double uhel,TObjekt* Objekt=NULL);//orotuje celý polygonu objektu či haly proti směru hodinových ručiček okolo osy dle bodu o souřadnicích X,Y, dle hodnoty rotace uhel
	TBod *najdi_bod(TObjekt* Objekt=NULL);//na aktuálních souřadnicích myši hledá bod, pokud je nalezen vrátí na něj ukazatel, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
	TBod *najdi_usecku(TObjekt* Objekt=NULL,long presnost=3);//na aktuálních souřadnicích myši hledá úsečku, pokud je nalezena je vracen ukazatel na druhý bod, pokd nebylo nalezeno nic vrátí NULL, parametr Objekt implicitně NULLL, rozlišuje hledání úsečky v HALE nebo v Objektu, parametr přesnost určuje vzdálenost od přímky, která je nutná k její detekci
	short PtInKota_bod(TObjekt *Objekt=NULL);//ověří zda se na daných fyzických souřadnicích myši nachází kóta hrany/stěny HALy či Objektu (Objektu pokud Objekt!=NULL), pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty , pozn. oblast kóty se testuje až jako poslední
	bool PtInBody(TObjekt *Objekt=NULL,bool vcetne_poloviny_sirky_steny=false);//ověří zda se souřadnicích myši nachází ve vnitř polygonu, pokud je Objekt==NULL, hledá se v polygonu HALy
	void kopiruj_body(TObjekt *Original,TObjekt *Kopie);//zkopíruje body včetně z originálu na kopii bez ukazatelového propojení, funguje jenom pro body objektů nikoliv HALY!!!
	void smaz_bod(TBod* Bod,TObjekt* Objekt=NULL);//smaže konkrétní bod, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
	void vymaz_body(TObjekt* Objekt=NULL);//vymaže všechny body včetně hlavičky, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU

//metody pro OBJEKTY
	void hlavicka_OBJEKTY();
	TObjekt *vloz_objekt(unsigned int id, double X, double Y);//vloží prvek do seznamu + vrátí ukazatel na vložený prvek
	TObjekt *vloz_objekt(unsigned int id, double X, double Y,TObjekt *pred,TObjekt *po);//přetížená fce vkládá objekt za objekt p + vrátí ukazatel na vložený prvek
	TObjekt *vloz_objekt(TObjekt *Objekt);//přetížená fce
	void nastav_atributy_objektu(TObjekt *novy,unsigned int id, double X, double Y);//alokuje paměť pro objekt, nastavý atriuty objektu, vrátí ukazatel na nově vytvořený prvek
	TObjekt *kopiruj_objekt(TObjekt *Objekt,short offsetX=0,short offsetY=0,AnsiString index_name="",bool remove_pre_index=false,TObjekt *p=NULL);//zkopíruje objekt Objekt na konec spojového seznamu Objektů, za předpokladu že p==NULL, pokud p není NULL je objekt za tento objekt p ve spojovém seznamů objektů zařazen, hodnota offsetu je hodnota odsazení zkopírovoaného objektu od objektu vzorového,index_name slouží pro rozlišení např. LAK, LAK1, LAK2...,zároveň vrací ukazatel na právě zkopírovaný objekt např. pro další použití
	void kopiruj_objekt(TObjekt *Original,TObjekt *Kopie);//zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze ukazatel na mGrid originálu zůstané propojený
	TObjekt *PtInObjekt();//ověří, zda se na souřadnicích myši nachází nějaký objekt, pokud ano, vrátí na něj ukazatel, jinak vrátí NULL
	TObjekt *najdi_objekt(double X, double Y,double offsetX, double offsetY,short typ=-1);//hledá bod v dané oblasti
	short oblast_objektu(TObjekt *O,double X, double Y);//vrátí ret podle toho v jaké jsem oblasti objektu, slouží k přilepování objektu při vkládání, ret = 0 - mimo oblasti, 1 - oblast za objektem, 2 - oblast před objektem
	TObjekt *vrat_objekt(unsigned int n);//dle zadaného n vrátí ukazatel na hledaný objekt
	TObjekt *vrat_objekt_z_roma(int X);//SMAZAT??dle X kurzoru myši vrátí z modu procesy (ROMA) ukazatel na aktuální objekt - MOŽNO ODSTRANIT
	TObjekt *vrat_objekt(TElement *Element,bool In_pom_temp=false);//DOPRYČ????    vrátí rodičovský Objekt daného elementu, In_pom_temp - zda bude hledat místo daného ostrého objektu v něm - PO ZMĚNĚ DM BUDE TŘEBA?
	void aktualizace_objektu(short typ);//SMAZAT??dle zadaného TT  či případně dalších hodnot zaktualizuje paramametry všech objektů//typ -2://zaktualizuje přiřazení pohonu k objektu, nutné pokud proběhla změna v pohonech, protože původní jsou smazané //typ -1://dle zamčených a odemčených hodnot při změně TT//typ 0://dle zamčených a odemčených hodnot při změně parametrů vozíku//typ 1://při změně TT změna CT a RD, K a DD zůstává//typ 2://při změně TT změna K,DD,RD zůstává CT//typ 3://při změně parametrů vozíku změna DD, RD zůstává K, CT//typ 4://při změně parametrů vozíku změna u DD, CT zůstává K,RD//typ 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
  void aktualizace_rezimu_objektu(TObjekt *Objekt,bool aktualizovat_sta_mGridu=true);//aktualizuje stav objektu, například po odmazání posledního KK robota objekt nezůstane v KK režimu
	void aktualizace_prirazeni_pohonu_k_objektum(unsigned int oldN,unsigned int newN);//všem objektům, které měly přiřazen pohon s oldN(oldID), přiřadí pohon s newN(newID), podle toho, jak jsou ukládány nově do spojáku, důležité, pokud dojde k narušení pořadí ID resp n pohonů a pořadí jednotlivých řádků ve stringridu, např. kopirováním, smazáním, změnou pořadí řádků atp.
	void aktualizace_prirazeni_pohonu_dokoncena();//po dokončení aktualizace přiřazení pohonu (při ukládání pohonu na PL) vrátí atribut probehla_aktualizace_prirazeni_pohonu všech objektů na false, aby bylo připraveno k dalšímu opětovnému užítí, nepřímo spolupracuje s metodou výše uvedenou aktualizace_prirazeni_pohonu_k_objektum
	double vrat_soucet_delek_vsech_objektu();//SMAZAT??sečte délky jednotlivých objektů
	unsigned int pocet_objektu(short typ=-1);//vrátí počet objektů v požadovaném režimu, nebo v případě implicitního parametru -1 objekty ve všech režimech
	unsigned int pocet_objektu_zakazky(TZakazka *Zakazka,short typ=-1);//vrátí počet objektů v požadovaném režimu pro danou zakázku, nebo v případě implicitního parametru -1 objekty ve všech režimech pro danou zakázku
	AnsiString vypsat_objekty_bez_prirazenych_pohonu(bool shortname=true,AnsiString separator=", ");//vrátí AnsiString řetezec shortname či name (dle prvního parametru, který je implicitně na shortname=true) seznam objektů, které nemají přiřazený pohon, jednotlivé názvy objektů oddělí dle paramaterů seperátor, implicitně ", " tj. čárka a mezera,, v případě že žádný objekt nenajde, vrátí prázdný řetězec
	unsigned long vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(bool s,short rezim=-1);//vrátí počet objektů bez či s přiřazenými pohony (dle vstupního parametru), parametr režim ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	AnsiString vypis_objekty_vyuzivajici_pohon(unsigned long n,bool short_name=true);//dle n pohonu vráti objekty, které pohon používají, pokud je short_name na true, vrátí kratký název objektu jinak dlouhý
	unsigned long vrat_pocet_objektu_vyuzivajici_pohon(unsigned long n, short rezim=-1);//vratí počet objektů přiřazených k danému pohonu, parametr režim ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	TObjekt *vrat_objekty_vyuzivajici_pohon(unsigned long n, short rezim=-1);//vratí formou ukazatele na pole objekty přiřazené k danému pohonu, parametr režim, ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	TObjekt *vrat_objekty_bez_pohonu();//vratí formou ukazatele na pole objekty bez pohonů
	AnsiString vypis_objekty_s_pohony_bez_roztece(bool shortname=true);//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů, které mají přiřazený pohon bez uvedené rozteče jednotlivé názvy objektů oddělí  ", " tj. čárkou a mezerou, v případě že žádný objekt nenajde, vrátí prázdný řetězec, pozor pohony bez přiřazení k objektům nevypisuje
	AnsiString vypis_objekty_mimo_100vytizeni(bool shortname=true, bool vypsat_procetna=true, AnsiString separator=", ");//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů podle zakázek, které nemají 100% vytížení/// SMAZAT??
	double vrat_min_rychlost_prejezdu();//najde minimální rychlost pohonu (tj. totál min RD) ze všech použitých pohonů, pomůcka pro stanovení referenční rychlosti animace
	short smaz_objekt(TObjekt *Objekt);//smaže objekt ze seznamu
	void zmen_poradi_objektu(TObjekt *aktualni_poradi,TObjekt *nove_poradi);
	void zmen_poradi_objektu(unsigned long aktualni_poradi,unsigned long nove_poradi);
	void nove_indexy(bool nasledne_zmena_nazvu=false);//projde všechny objekty a nastavý nové indexy podle aktuálního pořadí objektů
	void ortogonalizovat();//ortogonalizuje schéma
	void posun_objekt(double X,double Y,TObjekt *Objekt,bool kontrolovat_oblast=true,bool povolit_rotaci=true);//slouží k posunu objektu jako celku o X a Y, posun kabiny, pohonu, elementů, tabulek, nadpisu, kontrolovat_oblast slouží k nucenému přesunutí
	void rotuj_objekt(TObjekt *Objekt, double rotace);//orotuje objekt o danou rotaci
	//přidružené metody pro KOMORY
	void vloz_komoru(TObjekt *Objekt,double velikost,TKomora *ZaKomoru=NULL,short typ=1);//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, nastaví velikost dané komory dle proměnné velikost,short typ;//1-se sprchou, 0 bez jen okap
	void vloz_komoru(TObjekt* Objekt,TKomora *Komora,TKomora *ZaKomoru=NULL,short typ=1);//vloží novou komoru na konec seznamu komor, pokud je ZaKomoru=NULL, jinak vloží za tento objekt, není třeba nastavovat ukazatele ani n-pořadí,short typ;//1-se sprchou, 0 bez jen okap
	TKomora *najdi_komoru(TObjekt* Objekt);//na aktuálních souřadnicích myši hledá komoru, pokud je nalezena vrátí na ni ukazatel
	short PtInKota_komory(TObjekt *Objekt,long X,long Y);//ověří zda se na daných fyzických souřadnicích nachází kóta komory, pokud neexistují komory vrací -2, pokud se nenachází na kótě vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty, pozn. oblast kóty se testuje až jako poslední
	double vrat_velikosti_komor(TKomora *po=NULL);//vrátí součet velikostí komor, nebo velikosti do konkrétní komory (včetně)
	void kopiruj_komory(TObjekt *Original,TObjekt *Kopie);//zkopíruje komory včetně jejich velikosti z originálu na kopii bez ukazatelového propojení
	void presun_komoru(TObjekt *Objekt,TKomora *aktKomora,TKomora *za);//slouží ke změně pořadí komor
	void smaz_komoru(TObjekt* Objekt,TKomora* Komora);//smaže konkrétní komoru daného objektu
	void vymaz_komory(TObjekt* Objekt);//vymaže všechny komory daného objektu včetně hlavičky
	long vymaz_seznam_OBJEKTY();//vymaže spojový seznam technologických objektů včetně přidružených elementů a případných komor z paměti

//metody pro ELEMENTY
	void hlavicka_ELEMENTY();//vytvoří hlavičku seznamu elementů
	TElement *vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y,short rotace_symbolu,TElement *Ep=NULL);//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
	void vloz_element(TObjekt *Objekt,TElement *Element,TElement *force_razeni=NULL);//vloží element do spojového seznamu elementů daného technologického objektu
	void vloz_element(TElement *Element);//vkládá elementy za sebe do spojáku ELEMENTY, souží pro načítání ze souboru
	Cvektory::TElement *vloz_element_pred(TObjekt *Objekt,TElement *Element);//kontrola zda vkládaný element bude umístěn na konec nebo mezi jiné elementy, pokud bude vkládán na konec return=NULL, pokud mezi elementy return=ukazatel na předchozí element
	void vloz_G_element(TElement *Element,short typ,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4,double orientace=0,double rotacni_uhel=0,double radius=0,double delka=0);//danému elementu přiřadí/naplní geometrickou složku
	void uprav_popisky_elementu(TElement *Element);//upraví indexy a popisky elementů po vloženém elementu (parametr Element), pokud dostane parametrem Element NULL přejmenuje a přeindexuje všechny ovlovněné elementy do původního stavu (tlačítko storno)
	void kopiruj_element(TElement *Original, TElement *Kopie);//zkopíruje atributy elementu bez ukazatelového propojení, pouze ukazatelové propojení na mGrid je zachováno
	void kopiruj_data_elementu(Tdata Original,TCesta *cesta);//zkopíruje atributy dat, slouží pro zakládání cesty v zakázce, musí být řešeno přes cestu!!!!!! ne Tdata Original, Tdata Kopie
	void aktualizace_prirazeni_pohonu_k_elementum(unsigned int oldN,unsigned int newN);//všem elementům, které měly přiřazen pohon s oldN(oldID), přiřadí pohon s newN(newID), podle toho, jak jsou ukládány nově do spojáku, důležité, pokud dojde k narušení pořadí ID resp n pohonů a pořadí jednotlivých řádků ve stringridu, např. kopirováním, smazáním, změnou pořadí řádků atp.
	void vytvor_elementarni_osu(TObjekt *Original, TObjekt  *Kopie);//SMAZAT??připraví vektor provizorní osy pohonu
	int vrat_eID_prvniho_pouziteho_robota(TObjekt *Objekt);//vratí eID prvního použitého robota, slouží na filtrování, jaké roboty v knihovně robotů zakazazovat, pokud není nic nalezeno vrátí -1
	unsigned int vrat_poradi_elementu_do (TElement *Element=NULL);//vrátí pořadí robotů v objektu, stopek a otočí ve všech předchozích objektech, to všd do Elementu
  unsigned int vrat_pocet_elementu_eID(TElement *Element);//vrátí počet elementu podle eID Elementu
	unsigned int vrat_nejvetsi_ID_tabulek (TObjekt *Objekt);//vrátí největší ID napříč mGridy v objektu, používáno pro přiřazování ID novým tabulkám, řešeno takto z důvodu chyby při odmazávání a následném přidávání elementu (v kabině jsou 3 elementy druhý se odmaže, tabulky v kabině mají nyní ID 1 a 3, po přidání dalšího elementu bylo dříve přidano ID=pocet elementů, což by se v tomto případě rovnalo 3)
	bool SGlinka();//ověří zda se jedná S&G linku z pohledu užité cesty, resp. zda obsahuje alespoň jeden S&G element včetně stopky na užité cestě, element musí být na pohonu
	short vrat_druh_elementu(TElement *Element);//vrátí typ elementu -1 nenastaven nebo zarážka či předávací místo, 0 - S&G (včetně stopky), 1 - kontinuál
	bool funkcni_element(TElement *Element);//vrátí true, pokud se jedná o funční element
	void rotace_elementu(TObjekt *Objekt,short rotace);//orotuje všechny elementy daného objektu o danou hodnotu
	short oblast_elementu(TElement *Element, double X, double Y);//zkontroluje, zda se nacházím v oblasti elemetu - 0 nenachází, 1 v oblasti elementu, 2 v oblasti popisku elementu
	bool obdelnikova_oblast_elementu(TElement *Element, double X, double Y);//zkontroluje, zda se nacházím v oblasti obdelníku opsaného oblasti daného elemetu bez popisu elementu, použito pouze pro stop, roboty, operatory a ion tyč, //akceluruje celkové vyhledávací řešení při přejíždění myší přes layout, předchází přesnému vyhledávání, to nastavá až po navrácen true hodnoty touto metodou
	TElement *najdi_element(TObjekt *Objekt, double X, double Y);//hledá element v místě kurzoru pracuje v logických/metrických souradnicích
	TElement *najdi_posledni_element_podle_eID(unsigned int eID,TObjekt *Objekt);//najde poslední element v objektu který odpovídá eID
	TElement *najdi_tabulku(TObjekt *Objekt, double X, double Y);//hledá tabulku elementu pouze pro daný objekt v oblasti definované pomocí šířky a výšky tabulky (která se může nacházet v daném místě kliku), pracuje v logických/metrických souradnicich, vrátí ukazatel na daný element, který tabulku vlastní, pokud se na daných souřadnicích nachází tabulka
	TElement *vrat_element(TObjekt *Objekt, unsigned int n);//vraťí ukazatel na element dle n elementu umístěného v daném objektu
	TElement *vrat_element(unsigned int n);//vraťí ukazatel na element dle n elementu
	bool je_element_ve_vetvi(TElement *Element,TElement *Vyhybka);//otestuje, zde se Element nachází ve vedlější větvi Výhybky
	short PtInKota_elementu(TObjekt *Objekt,long X,long Y);//ověří zda se na daných fyzických souřadnicích nachází kóta elementu, pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty , 3 - na hodnotě LO kóty , pozn. oblast kóty se testuje až jako poslední
	bool PtInSegment(TElement *E,double Xmys,double Ymys);//ověří zda se na dané myších souřadnice nachází v geo. segmentu elemnetu
	bool posun_element(TElement *Element,double vzdalenost,bool pusun_dalsich_elementu=false,bool posun_kurzorem=false,bool kontrola_zmeny_poradi=true);//posune pouze Element z pomocného spojového seznamu pom_temp na parametrem uvedenou vzádlenost (v metrech) od elementu předchozího, pokud je implicitní hodnota pusun_dalsich_elementu false změněna na true, jsou o danou změnu posunu přesunuty i elementy následující Elementu (tudíž jejich vzdálenost od Elementu bude zachována, naopak v případě výchozí hodnoty false je následujícím/dalším elementům poloha zachována). Nutá rozdílná funkce při posunu z kót a při posunu korzorem, proto parametr posun_kurzorem
	void zmen_poradi_elementu(TElement *E,TElement *Ed);//řeší změnu pořadí při posuvu elementů, dojde k novému ukazatelovému propojení, přejmenování a přeindexování elementů
	double vzdalenost_od_predchoziho_elementu(TElement *Element,bool pouzeSG=false);//vratí vzdálenost od předchozího elementu, pracuje zatím pouze v orotogonalizovaném prostoru (bude nutno vylepšit s příchodem oblouků), pokud se jedná o první element, uvažuje se jako vzdálenost od počátku kabiny (nutno vylepšit ještě pro různé orientace kabiny)
	double vrat_rotaci_jigu_po_predchazejicim_elementu(TElement *Element);//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje, metoda vrací jednorázový výsledek, ale i zároveň aktulizuje data "Element->data.orientace_jig_pred" již prošlých Elementu, včetně zadávaného
	TElement *vrat_posledni_rotacni_element();//metoda vrátí ukazatel na poslední rotační element na lince
	void prirad_sparovany_element(TElement *Element);//přiřadí Elementu ukazatel na jeho spárovaný element, zároveň aktualizuje tomuto spárovanému elementu spárovaný element + aktualizace první - poslední S&G element
	void aktualizuj_sparovane_ukazatele();//projde všechny stop-elementy a aktualizuje jim ukazatele na spárované elementy
	void reserve_time(TElement *Element,TCesta *Cesta=NULL,bool highlight_bunek=false,bool refresh_mGrid=false);//vypočítá a uloží RT do elementu
	void aktualizuj_data_elementum_na_pohonu(unsigned long pohon_n);//přepočíta data elementů na danném pohonu
	bool validace_duplicity_nazvu(TElement *Element);//prověří zda existuje nějaký další element s duplicitním názvem
	TElement *vrat_posledni_element_objektu(TObjekt *Objekt);//vrátí poslední element v objektu
	T2Element *hlavicka_seznam_VYHYBKY();
	void uloz_vyhybku_do_seznamu(TElement *vyhybka,T2Element *VYHYBKY);
	void smaz_vyhybku_ze_seznamu(T2Element *VYHYBKY);
	long vymaz_seznam_VYHYBKY(T2Element *VYHYBKY);
	TElement *dalsi_krok(T2Element *VYHYBKY,TElement *E,TObjekt *O=NULL);//určí další krok průchodového algorytmu ve spojáku elementů, 2 možností průchod kompletního spojáku ELEMENTY, druhá průchod pouze elementů jednoho objektu
	TElement *predchozi_krok(TElement *E,TObjekt *O=NULL);
  T2Element *vytvor_tabElm_pruchodu();//vytvoří tabulku průchodu pro alg. sekundarni_zapis_cteni()
	TElement *Cvektory::sekvencni_zapis_cteni(TElement *E,TPoint *tab_pruchodu_TP,T2Element *tab_pruchodu_T2E);
	void smaz_vyhybku_spojku(TElement *Element,unsigned long maz_OBJ=0);
	void aktualizuj_identifikator_vyhybky_spojky();
	void smaz_element(TElement *Element,bool preskocit_kontolu=false,unsigned long maz_OBJ=0);//smaže element ze seznamu
	void vymaz_elementy(TObjekt *Objekt);//smaže všechny elementy v daném objektu
	long vymaz_seznam_ELEMENTY();//vymaže spojový seznam elementů z paměti

	//proměnné pro elementy
	TElement *vyhybka_pom;
	unsigned int pocet_vyhybek;

//metody pro POHONY
	void hlavicka_POHONY();
	void vloz_pohon(TPohon *pohon);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
	void vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double aRD,double R,double Rz,double Rx,unsigned int roztec_ID=0);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
	TPohon *vrat_pohon(unsigned long n);//vrátí ukazatel na pohon dle n pohonu
	void kopiruj_pohon(TPohon *Pohon,TObjekt *Objekt);//bez ukazatelového propojení zkopíruje atributu pohonu do pohonu požadovaného objektu, neobsahuje-li tento objekt alokovanou paměť pro pohon, naalokuje jí
	bool pohon_je_pouzivan(unsigned long n,bool po_obektech=true,bool vsechny_elementy=false);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
	TObjekt *pohon_je_pouzivan(unsigned long n,TObjekt *mimo_objekt,short rezim=-1);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv, ten vrátí formou ukazatale na první nalezený používáný, druhý vstupní parametr metody TObjekt mimo_objekt je ukazatel na objekt, který se bude při vyhledávání ignorovat, nenajde-li vrací NULL, třetí parametr, pokud je náchán na implicitní -1 řeší se pro všechny režim, pokud je v rozmezí 0 až 2 řeší se pro konkrétní režim
  void aktualizuj_parametry_pouzivanych_pohonu();//aktualizuje Rx a Rz pohonů, které jsou používané, provádí se pro přepsání pohonů na PL
	void aktualizuj_rezim_pohonu();//aktualizuje režim všem pohonům, použito například při načtení z binárky
	void aktualizuj_rezim_pohonu(TPohon *pohon,short rezim=-1);//zapíše stav konkrétnímu pohon, nebo zjistí stav pohonu a zapíše ho
	unsigned long vrat_pocet_nepouzivanych_pohonu();//vrátí počet nepoužívaných pohonů
	double minRD(TPohon *pohon);//SMAZAT??vrátí nejnižší možnou rychlost ze všech objektů, které jsou přiřazené k danému pohonu (využívá se pro S&G a PP, u KK musí být RD v souladu s TT)//pokud vrátí 0, znamená, že pohon není využíván
	AnsiString vypis_objekty_nestihajici_prejezd(TPohon *pohon,double testRD,short rezim=-1);//vypíše objekty přiřazené k danému pohonu nestíhající přejezd při navrhovaném testRD, možno nastavit režim, pro S&G + PP hodnota režim 20
	AnsiString kontrola_rychlosti_prejezdu(TObjekt *O,short rezim,double CT=0,double MT=0,double WT=0,double aRD=0,double DD=0,short aRDunit=-1,unsigned short precision=3,AnsiString mark="..",bool add_decimal=false,AnsiString separator_aRD=" o ");//SMAZAT??zkontroluje objekt zda daná rychlost pohonu odpovídá požadované rychlosti pohonu, pokud ne vrátí popis včetně hodnoty, lze poslat externí testovací parametry nebo nechat ověřit dle uložených ve spojáku objekty
	TPohon *najdi_pohon_dle_RD(double RD);//ověří zda je stejná rychlost pohonu na lince používána, pokud není vratí NULL, jinak ukazatel na daný pohon
	void vytvor_retez(TPohon *Pohon);//danému pohonu vytvoří řetěz a palce dle geometrie všech elementů, co spadají pod daný pohon
	void vloz_segment_retezu(TRetez *Retez,TPohon *Pohon);//danému řetězu vloží jeden geometrický segment
	void posun_palce_retezu();//od pohonům resp. jejich řetězům posune palce o daný časový úsek
	void vloz_palec(TPalec *Palec,TPohon *Pohon);//danému pohonu vloží jeden palec
	void zrusit_prirazeni_pohunu_k_objektum_elementum(unsigned long n);//všem objektům s n pohonem zruší přiřazení k tomuto pohonu a nahradí hodnotu ukazatele na přiřazený pohon za NULL, nově i všem elementům
	long vymaz_seznam_POHONY();//smaže jednotlivé prvky seznamu, včetně hlavičky, pokud následuje další práce se seznamem, je nutné založit nejdříve hlavičku pomocí hlavicka_pohony()
	void smazat_retez(TPohon *pohon);//danému pohonu smaže jeho řetěz
	void smazat_palce(TPohon *pohon);//danému pohonu smaže jeho palce řetězu
	//	double delka_dopravniku(Cvektory::TObjekt *ukaz);
	//následují 4 možno odstanit:
	TTextNumber rVALIDACE(short VID,unsigned long PID,double aRD,double R,double Rz,double Rx,short aRDunit,short Runit,short Rzunit);//zkontroluje aplikovatelnost uvažovaného hodnodty dle VID parametru, resp. čísla sloupce (aRD=4,R=5,Rz=6,Rx=7 na všech objektech, přiřazených k danému pohonu označeným parametrem PID, pokud je zadán parametr getValueOrMessage 0 (který je zároveň implicitní), vratí doporučenou hodnotu dle VID, pokud je zvoleno 1, vrátí text chybouvé hlášku s problémem a doporučenou hodnotou, pokud vrátí prázdné uvozovky, je vše v pořádku, //vstupy aRD,R,Rz,Rx a výstupní číselná hodnota jsou v SI jednotkách, naopak textový řetězec problému resp. doporučení, obsahuje hodnotu již převedenou dle aRDunit, Runit, Rzunit
private:
	TTextNumber validace_aRD(double aRD,TPohon *p);
	TTextNumber validace_Rz(double Rz,unsigned long PID);
	TTextNumber validace_Rx(double Rx);

//metody pro DÁVKY
public:
	void hlavicka_davky_zakazky(TZakazka *zakazka);//vytvoří novou hlavičku pro spojový seznam dávky v zakázce
	void inicializace_davek(TZakazka *zakazka);//vymaže předchozí dávky a vytvoří novou hlavičku, použito pro opakovanou tvorbu default zakázky
	void vloz_davku(TZakazka *zakazka,unsigned long pocet_voziku,unsigned long pocet_prazdnych,unsigned long pocet_celkem);//vytvoří novou dávku a vloží ji do zakázky
	void vloz_davku(TZakazka *zakazka,TDavka *davka);//vloží vytvořenou dávku do zakázky
	TDavka *vrat_davku(TZakazka *zakazka,unsigned long n);//vrátí ukazatel na dávku podle její zakázky a n
	void smaz_davku(TZakazka *zakazka,unsigned long n);//smaže konkrétní dávku
	void vymaz_davky_zakazky(TZakazka *zakazka);//smaze všechny dávky v zakázce

//metody pro ZAKAZKY
public:
	void hlavicka_ZAKAZKY();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY
	void vloz_temp_zakazku(UnicodeString id,unsigned short typ, UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//vytvoří zakázku dle zadaných parametru do spojového seznamu ZAKÁZKY
	void edituj_temp_zakazku(unsigned long n,UnicodeString id,unsigned short typ,UnicodeString name,TColor barva,double pomer,double TT,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//provede editaci zakázky s uvedeným “n” ze spojového seznamu ZAKAZKY_temp
	void smaz_temp_zakazku(unsigned long n);//smaže zakázku s uvedeným “n” ze spojového seznamu ZAKAZKY_temp včetně přidružených cest
	void zmen_poradi_temp_zakazky(unsigned long aktualni_poradi,unsigned long nove_poradi);//změní zařazení zakázky ve spojovém seznamu
	TZakazka *vrat_zakazku(unsigned long n_zakazky);// vrátí ukazatel (resp. data) na editovanou zakázku
	TZakazka *vrat_temp_zakazku(unsigned long n_zakazky);// vrátí ukazatel (resp. data) na editovanou temp zakázku
	TZakazka *vrat_temp_zakazku_z_mGridu(unsigned long ID);//vrátí ukazatel (resp. data) na editovanou zakázku, podle jejiho mGridu
	void kopirujZAKAZKY_temp2ZAKAZKY(bool mazat_ZAKAZKY_temp=true);//po stisku OK v superformu zkopíruje data z ZAKAZKY_temp do ZAKAZKY, implicitně následně smaže ZAKAZKY_temp
	void kopirujZAKAZKY2ZAKAZKY_temp();//zkopíruje ukazatel na ZAKAZEK do ZAKAZKY_temp, slouží v momentu načítání SF
  void update_akt_zakazky();//aktualizuje cestu aktuální zakázy, pokud je zvolena, pokud ne aktualizuje defaultní zakázku
	void vytvor_default_zakazku();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji smaže a nahradí novou
	long vymaz_seznam_ZAKAZKY_temp();//smaze seznam ZAKAZKY_temp z paměti včetně přidružených cest, nutno implementovat při close() superformu (ať už při OK, storna, či křížku formu)

private:
	void hlavicka_ZAKAZKY_temp();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY_temp, nutná volat při on_show superformuláře
	void vloz_temp_zakazku(TZakazka *Zakazka_temp);//vloží vytvořenéu zakázku do spojového seznamu ZAKÁZKY_temp
	void vloz_zakazku(TZakazka *Zakazka);//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
	long vymaz_seznam_ZAKAZKY();//smaze seznam ZAKAZKY z paměti v četně přidružených cest, pokud následuje další práce se seznamem (např. nové vkládání), je nutné založit nejdříve hlavičku pomocí hlavicka_ZAKAZKY()
//metody pro cesta konkrétní zakázky
public:
	void inicializace_cesty(TZakazka *zakazka);//vymaže předchozí cestu a zavolá hlavičku cesty nové
	void vloz_cestu_po_hlavni_vetvi(TZakazka *zakazka,bool po_vyhybku=false);//vytvoří cestu po hlavní vetvi projektu, pokud je nastaveno po vyhybku vytvoří cestu po první výhybku (včetně)
	void vloz_segment_cesty(TZakazka *zakazka,TElement *element);//do konkrétní cesty vloží segmenty cesty
	void vloz_segment_cesty(TZakazka *zakazka,TElement *element,TElement *sparovany,Tdata data);//do konkrétní zakázky vloží segment cesty, slouží pro kopírování zakázek
  void kopiruj_cestu_zakazky(TZakazka *original,TZakazka *kopie);//kopíruje cestu zakázky do jiné zakázky
	TCesta *obsahuje_segment_cesty_element(TElement *element,TZakazka *zakazka);//ověří zda daný element je součástí cesty dané zakázky či nikoliv, pokud ano vrací ukazatel na daný segment cesty
	unsigned int kolikrat_obsahuje_segment_cesty_element(TElement *element,TZakazka *zakazka);//vrátí počet výskytů elementu v cestě zakázky
	TZakazka *obsahuje_segment_cesty_element(TElement *element);//ověří zda daný element je součástí cesty nějaké zakázky či nikoliv, pokud ano vrací ukazatel na danou zakázku
	TCesta *vrat_segment_cesty(TZakazka *zakazka,TElement *element);//vrátí konkrétí segment cesty v zakázce, který obsahuje element
  void najdi_sparovane_elementy_ceste(TZakazka *zakazka);//najde a uloží všem segmentům cesty spárované elementy
	void vymaz_cestu_zakazky(TZakazka *zakazka);//vymaže celou cestu dané zakázky
private:
	void hlavicka_cesta_zakazky(TZakazka *zakazka);//vytvoří novou hlavičku pro spojový seznam konkrétní cesty dané zakázky
	void vloz_segment_cesty(TZakazka *zakazka,TCesta *segment_cesty);//do konkrétní zakázky vloží segmenty cesty

//metody pro VOZIKY
public:
	void hlavicka_VOZIKY();//vytvoří hlavičku spojového seznamu VOZIKY
	void generuj_VOZIKY();//vygeneruje podle zadané zakázky seznam vozíků v úvodním rozložení
private:
	TPointD_3Dbool generuj_voziky_segementu_mimo_stop_a_buffer(TElement *E,unsigned int pocet_voziku,double umisteniCas,double akt_rotace_jigu,bool rotacni_zbytek,bool predchazi_stop);//podpůrná metoda metody výše uvedené, řeší vždy jen daný geometrický segment daného elementu, mimo bufferu s vozíkem na stopce (i mimo jednovozíkový buffer)
	void generuj_voziky_stop_a_bufferu(TElement *E,double akt_rotace_jigu,int pocet_voziku_z_prejezdu_na_bufferu);//vygeneruje vozíky v bufferu včetně aktuálního vozíku na stopce,počet vozíků přicházejících z přejezdu, které již "narazily" do bufferu
	double buffer_pres_predavaci_misto(TElement *E);//pokud je buffer přes předávací místo, vrátí délku bufferu, co předchází předávacímu místu, pokud ne vrátí se 0
	TVozik *vrat_vozik(unsigned int n);//dle n resp. ID vozíku vrátí ukazatel na daný vozík
	void vloz_vozik(TZakazka *zakazka,TElement *element,double X,double Y,double orientaceP,double rotaceJ,short stav);
	void vymaz_seznam_VOZIKY();

public:
//metody pro BUFFER SIMULACE
	TmyPx *komprese(Graphics::TBitmap *bmp_in,TRect oblast=TRect(0,0,0,0),bool resize=false);
	Graphics::TBitmap *dekomprese(TmyPx *Raster,unsigned short Width,unsigned short Height);
	void rast_do_souboru(TmyPx *Raster,String FileName);//Uloží rastrová data do souboru
	void vloz_do_BUFFERU();
	void vloz_SIM(double T1,double T2,short A,TElement *S,TVozik *V);
  int prichytit_cas_SIM(unsigned int V);//přichytí X souřadnici na nejbližší čas akci v grafu pro vozík V
	void vymaz_seznam_SIM();//smaže z paměti

//metody pro PROCESY, konrola metody obsahují již neexistující atributy  - již možno smazat
	void hlavicka_PROCESY();
	void vloz_proces(TProces *Proces);
	TProces *najdi_proces(double cas, double vozik);//hledá bod mezi procesy
	TProces *vrat_nasledujici_proces_objektu(TProces *Proces);//vratí následující proces na stejném objektu jako proces zadaný
	long vymaz_seznam_PROCESY();

//metody pro KATOLOG
	enum TtypHodnoty{R,hO,vO,vR,hR};TtypHodnoty;
	void vloz_typ_dopravniku(AnsiString name,UnicodeString link="",short druh=0);//vloží nový typ dopravníku do KATALOGu dopravníků, pokud neexistuje hlavička vytvoří ji, druh: 0 - podlahový, 1 - podvěsný
	void vloz_do_typu_dopravniku(TtypHodnoty typHodnoty,double hodnota,Ttyp_dopravniku *typDopravniku=NULL);//vloží dle typu hodnoty hodnotu do typuDopravniku, pokud je NULL, vloží nakonec (do posledního typu dopravníku uloženém ve spojovém seznamu v KaTALOGu)
	void vytvor_KATALOG();//vytvoří katalog typů dopravníku za pomocí volání nasledujících dvou metod
	Ttyp_dopravniku *vrat_typ_dopravniku(unsigned long n);//dla zadaného n vrátí daný typ dopravníku formou ukazatatele
	double vrat_hodnotu_typu_dopravniku(unsigned long nDopravniku,TtypHodnoty typHodnoty,unsigned long n);//vrátí od zadaného typu dopravníku dle zadaného n a typu hodnoty hodnotu - přetížená následující
	double vrat_hodnotu_typu_dopravniku(Ttyp_dopravniku *typDopravniku,TtypHodnoty typHodnoty,unsigned long n);//vrátí od zadaného typu dopravníku dle zadaného n a typu hodnoty hodnotu
  bool hodnota_v_katalogu(unsigned long nDopravniku,double hodnota,TtypHodnoty typHodnoty=hO);//zkontroluje zda se hodnota nachází v dopravníku, například hodnota oblouku v hobloucích dopravníku
	void vymaz_seznam_KATALOG();//smaže celý katalog, včetně přidružených spojových seznamů

//metody pro PALCE
// void hlavicka_palce();
// void vloz_palec();//přidá nový palec do seznamu PALCE
// long vymaz_seznam_palcu();

//metody pro TEXTY
	void vloz_text(double X, double Y,AnsiString text,int vrstva=-1);//vloží nový text na konec seznamu
	void posun_text(double X, double Y,TText* Text);//posune datový objekt textu daný ukazatelem na dané nové souřadnice
	TText *najdi_text(TText* Text);//na aktuálních souřadnicích myši hledá bod, pokud je nalezen vrátí na něj ukazatel, jinak vrátí NULL
	void kopiruj_texty(TText *Original,TText *Kopie);//zkopíruje text z originálu na kopii bez ukazatelového propojení
	void smaz_text(TText* Text);//smaže konkrétní Text
	void vymaz_TEXTY();//vymaže všechny texty ze spojového seznamu TEXTY včetně hlavičky

//metody pro SPOJNICE
	void vloz_spojnici(int vrstva=-1,short zakonceni=0);//vytvoří novou spojnici, a tu vloží do spojového seznamu spojnici, pokud ještě nebyla vytvořena hlavička, tak ji nejdříve založí, parametry: vrstva - ID vrstvy (např. layout vs. editace), zakonceni: 0 - nebude zakončeno, 1- bude zakončeno šipkou
	void vloz_bod_spojnice(double X, double Y,TSpojnice *Spojnice=NULL,TBod *ZaBod=NULL,bool ortogonalizovat=true);//vloží nový bod na konec seznamu dané Spojnice pokud je Za=NULL, jinak vloží za tento bod, ošetřuje bod vložený na stejný místo jako předchozí, pokud je Spojnice==NULL vkládání bodů probíhá přímo do vkládání poslední Spojnice ve spojovém seznamu SPOJINICE
	//void posun_bod(double X, double Y,TBod* Bod);//posune bod spojnice - použít již existující metodu
	//void posun_hranu(double OffsetX,double OffsetY,TBod* Bod1,TBod* Bod2);//posune hranu tvořenou danými body o zadaný offset  - použít již existující metodu
	void posun_body_spojnice(double OffsetX,double OffsetY,TSpojnice *Spojnice);//posune všechny body polygonu o daný offset
	//zatím nebude využito: void rotuj_body(double X, double Y,double uhel,TSpojnice *Spojnice);//orotuje celý polygonu proti směru hodinových ručiček okolo osy dle bodu o souřadnicích X,Y, dle hodnoty rotace uhel
	TBod *najdi_bod_spojnice();//na aktuálních souřadnicích myši hledá bod libovolné spojnice, pokud je nalezen vrátí na něj ukazatel, jinak NULL
	TBod *najdi_usecku_spojnice();//na aktuálních souřadnicích myši hledá úsečku libovolné spojnice, pokud je nalezena je vracen ukazatel na druhý bod, pokud nebylo nalezeno nic vrátí NULL
	TSpojnice *najdi_spojnici();//na aktuálních souřadnicích myší hledá spojnici, pokud najde vrací ukazatel na nalezenou spojnici, využívá výše uvedenou metodu
	void kopiruj_spojnici(TObjekt *Original,TObjekt *Kopie);//zkopíruje spojnici včetně bodů spojnice z originálu na kopii bez ukazatelového propojení
	void smaz_bod_spojnice(TBod* Bod,TSpojnice *Spojnice=NULL);//smaže konkrétní bod konkrétní spojnice, pokud je ukazatel Spojnice=NULL, jedná se bod poslední spojnice (je praktické např. při backspacu posledního bodu editované spojnice)
	void vymaz_body_spojnice(TSpojnice *Spojnice=NULL);//vymaže všechny body včetně hlavičky dané spojnice, pokud je ukazatel Spojnice=NULL, jedná se bod poslední spojnice (je praktické např. při ESC editované spojnice) - využívá výše uvedenou metodu
	void smaz_spojnici(TSpojnice *Spojnice=NULL);//vymaže spojnici včetně všech bodů a hlavičky dané spojnice, pokud je ukazatel Spojnice=NULL, jedná se bod poslední spojnice (je praktické např. při ESC editované spojnice) - využívá výše uvedenou metodu
 	void vymaz_SPOJNICE();//vymaže celý spojový seznam SPOJNICE včetně všech bodů a hlaviček - využívá výše uvedenou metodu, použít do vse_odstranit!!!

//metody pro ZPRÁVY
	void VALIDACE(TElement *Element=NULL);//zkontroluje buď všechny elementy (je-li vstupní parametr NULL), smaže všechny zprávy, a kde najde problém, uloží do zpráv, v případě, že není NULL
	UnicodeString getVID(long VID);//z čísla VIDu vrátí jeho textový popis
	void hlavicka_ZPRAVY();//vytvoří hlavičku zprav
	void vloz_zpravu(TZprava *zprava);//vloží jeden prvek na konec seznamu, přiřadí automaticky poslední N (id).
	void vloz_zpravu(double X, double Y,short zID, int VID, TElement *Element=NULL,double VIDvalue=-1);//vloží jeden prvek na konec seznamu, přiřadí automaticky poslední N (id).
	TZprava *vrat_zpravu(unsigned long n);//dle N (id) zprávy vrátí ukazatel na danou zprávu
	TPoint vrat_pocet_zprav();//vrátí počet errorů a warningů
	long PtInZpravy();//ověří, zda se na daných souřadních myši nachází nějaká (libovolná) ze zpráv
	void vymazat_ZPRAVY();//vše odstraní včetně hlavičky

//odstraní všechny vektory (všechny globální spojáky)
	void vse_odstranit();

//práce s DATA, obrazem projektu
	void hlavicka_DATA();//vytvoření hlavičky DATA
	Cvektory::TDATA *vytvor_prazdny_obraz();//vytvoří prázdný obraz projektu, souží pro vytváření hlavičky nebo pro nový obraz (první prázdný obraz, poté se plní)
	void vytvor_obraz_DATA(bool storno=false);//vytvoří obraz projektu v závislosti zda se jedná o storno funkcionalitu, layout nebo editaci objektu
	void nacti_z_obrazu_DATA(bool storno=false);//načtení z obrazu projektu v závislosti zda se jedná o storno funkcionalitu, layout nebo editaci objektu
	Cvektory::TDATA *vrat_obraz_DATA(unsigned long n);//vrátí obraz podle jeho n
	void smaz_obraz_DATA(unsigned long n=0);//smaže konkrétní obraz
  void smaz_data_obrazu(TDATA *obraz);//mazání dat v obrazu, objekty, elementy, ...
	long vymaz_seznam_DATA();//smaže kompletní seznam DATA

	unsigned long pozice_data;//uchovává pozici ve spojáku dat, pro posunování při ctrl+z funkcionalitě
	unsigned long pocet_kroku;//určuje počet uchovávaných obrazů projektu

//magnetické laso
	void hlavicka_MAG_LASO();//vytvoří novou hlavičku pro magnetické laso
	long vymaz_seznam_MAG_LASO();//smaže kompletní seznam pro magnetické laso
	void kontrola_vlozeni_do_mag_lasa(TElement *E);//provede kontrolu zda může být element vložen do seznamu mag. laso, pokud ano vloží ho
	void vloz_segment_MAG_LASA(TElement *E);//vloží nový segment do magnetického lasa
	void smaz_segment_MAG_LASA(TElement *E);//smaže segment z magnetického lasa
	TPointD bod_na_geometrii(TElement *E,double x=MaxInt, double y=MaxInt);//"přilepování" souřadnic na gaometrii linky, linie i oblouky
	short obsahuje_MAG_LASO_element(TElement *E);//kontrola zda spoják magnetického lasa obsahuje segment s danným elementem
	TPointD InVrata(TElement *E,bool kontrola_kurzoru=true);//zkontroloje, zda existují vrátka objektu a jestli jsem kurzorem v jejich oblasti, pokud ano vrátí bod, pokud ne vrátí [-MaxInt,-MaxInt]

//souborové operace
	void vytvor_hlavicku_souboru();
	short int uloz_do_souboru(UnicodeString FileName);
	short int nacti_ze_souboru(UnicodeString FileName);
	short int ulozit_report(UnicodeString FileName);
	void nacti_CSV_retezy(AnsiString FileName);
	AnsiString ReadFromTextFile(AnsiString FileName);
	void SaveText2File(AnsiString Text,AnsiString FileName);

//technické, statistické a ekonomické ukazatele, kontrola některé motody obsahují již neexistující atributy
	TPointD vrat_zacatek_a_konec_zakazky(TZakazka *jaka);//ukazatel na cestu resp, zakázku
	TPointD vrat_zacatek_a_konec_zakazky(unsigned int n_zakazky);//n resp. ID cestu resp, zakázku
	double vrat_nejpozdejsi_konec_zakazek();//nemusí se vždy jednat o poslední zakázku
	double vrat_LT();//vratí součet CT časů objektů ze statusu návrh/architekt
	double vrat_LT_voziku(TVozik *jaky);//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
	double vrat_LT_voziku(unsigned int n_voziku);//vrátí celkový čas, který strávil vozík ve výrobě včetně čekání
	double vrat_sumPT_voziku(TVozik *jaky);//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
	double vrat_sumPT_voziku(unsigned int n_voziku);//vrátí čistý čas, který strávil vozík ve výrobě bez čekání
	double vrat_AVGsumPT_zakazky(TZakazka *jaka);//vrátí čistý čas, který strávily vozík ve výrobě bez čekání pro danou zakazku
	double vrat_WT_voziku(TVozik *jaky);//vrátí čas čeká vozíku během výroby
	double vrat_WT_voziku(unsigned int n_voziku);//vrátí čas čeká vozíku během výroby
	double vrat_AVGsumWT_zakazky(TZakazka *jaka);//vrátí čistý čas, který vozíky čekaly pro danou zakazku
	double vrat_TT_voziku(TVozik *jaky);//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
	double vrat_TT_voziku(unsigned int n_voziku);//vrátí takt, resp. rozdíl čásů mezi dokončením tohoto a předchozího vozíku
	double vrat_AVG_TT_zakazky(TZakazka *jaka);//vrátí hodnotu průměrného TT mezi vozíky v rámci dané zakázky/cesty
	double vrat_AVG_TT_zakazky(unsigned int n_zakazky);//vrátí hodnotu průměrného TT mezi vozíky v rámci
	unsigned int vrat_pocet_voziku_zakazky(TZakazka *jaka);
	unsigned int vrat_pocet_voziku_zakazky(unsigned int n_zakazky);
	TPoint vrat_start_a_pozici_vozikuPX(unsigned int n_voziku);//vrátí začátek a konec vozíku na časové ose v PX
	double WIP(short typ_vypoctu=0);//vrátí max. počet vozíků na lince, kde parametr s implicitní hodnotou 0 je volaný výpočet z překrytí vozíků na časových osách, hodnot 1 - součtem kapacit zadaných (resp. v návrháru/architektovi vypočítaných), hodnota 2 - součtem kapacit vypočtených v časových osách,  3 - tradiční výpočet WIP=1/TT*LT
	void uloz_doporucene_kapacity_objetku();//ukládá vypočtené doporučené kapacity jednotlivým technologickým objektům do jejich atribitu dop_kapacita, která se nezadává uživatelsky, ale jedině v tomto algoritmu
	TMinMedAvgMax_d vrat_statisticke_doby_cekani_na_palec(TCesta *segment_cesty);//vrátí minimální, střední, průměrnou a maximální dobu čekání na palec v sec pro daný objekt (segment cesty) tak, jak bylo vypočteno v analýze/na časových osách, musí být tedy zde zvolena nějaká (libovolná) volba čekání na palec, mimo "žádná", Struktura TMinMedAvgMax_d vrací 4 hodnoty, min, med, agv, max datového typu double, volání výsledků probíhá přes “tečkový selektor”

//teploměry
	void hlavicka_teplomery(TObjekt *Objekt);
	void hlavicka_cesty_teplomery(TTeplomery *teplomery);//vytvoří v objektu hlavičku pro cestu teploměrů
	void vymaz_teplomery(TObjekt *Objekt,TTeplomery *teplomery,bool ponechat_elementy=true);//vymaže konkrétní záznam teploměrů, s elementy nebo bez
  void vymaz_seznam_cest(TTeplomery *teplomery,bool ponechat_elementy=true);//vymaže seznam cest z teplomerů, s elementy nebo bez
	void vymaz_seznam_teplomery(TObjekt *Objekt,bool ponechat_elementy=true);//vymaže seznam teploměrů z objektu s elementy nebo bez
  Cvektory::TTeplomery *vrat_teplomery_podle_zakazky(TObjekt *Objekt,TZakazka *Zakazka);//vrátí ukazatel na záznam teploměrů pro konkrétní zakázku
  Cvektory::TTeplomery *vytvor_zaznam_teplomeru_pro_zakazku(TObjekt *Objekt,TZakazka *Zakazka);//vytvoří záznam teploměrů pro zakázku
	void vloz_segment_cesty_teplomery(TObjekt *Objekt,TElement *Element,bool prvni=false,bool posledni=false,double X=0,double Y=0);//vloží segment cesty do cest teploměrů v objektu, nebo vložení teploměru, doplnění bodu vložení
  void vloz_segment_cesty_do_seznamu_cesty(TTeplomery *teplomery,TElement *Element,bool prvni=false,bool posledni=false,unsigned int eID=400,double X=0,double Y=0);
	void vytvor_default_c_teplomery(TObjekt *Objekt);//vytvoří 2 teploměry a defaultní cestu mezi nimi
	short najdi_teplomer();//hledá zda není uživatel kurzorem nad teploměrem, pokud ano zapíše ukazatel na něj do F->pom_element a vrátí 1 ... teplomer, 2 ... popisek, nebo 0 ... nenalezeno
	void posun_teplomeru(TElement *teplomer);//posunem teploměru dochází k editaci jeho oblasti
	void zmena_zakazky_vytvoreni_teplomeru(TObjekt *Objekt,TZakazka *Zakt,TZakazka *Znova);//provede kontrolu, zda existuje cesta pro akt zakázku pokud ano, zkontroluje jestli existuje cesta i na nové zakázce, pokud ne vytvoří default cestu od vrátek k vrátkům
	void kopiruj_seznam_teplomery(TObjekt *zdroj,TObjekt *cil);//kopíruje záznamy teploměrů do jiného objektu, pro účely obrazu objektu
	Cvektory::TTeplomery *kopiruj_teplomer(TTeplomery *original);//vytvoří kopii z originálního záznamu teploměrů
	void aktualizuj_cestu_teplomeru();//pokud došlo ke změně, která může ovlivnit cestu teploměru, zkontroluje, zda je možné aktualizovat a pokud ano, aktualizuje
  void aktualizuj_cestu_teplomeru(TObjekt *Objekt);//pokud došlo ke změně, která může ovlivnit cestu teploměru, zkontroluje, zda je možné aktualizovat a pokud ano, aktualizuje, konkrétnímu objektu

//SQL
	AnsiString QUERY(AnsiString query);//vratí AnsiString hodnod dle zadaného dotazu v syntaxi SQL, zatím umí jen základní úroveň - asi odstranit

//pomocné metody
	void Text2CSV(AnsiString text,AnsiString FileName="",AnsiString Title="",AnsiString DefaultExt="",AnsiString Filter="",bool openDialog=true,bool open=true);//vytovoří a otevře CSV, je-li požadováno
	void vlakno_obraz();//provádí operace s obrazem na vlákně

//pomocné struktury pro ukládání do bináru
private:
	struct C_raster
	{
		unsigned int text_length;
		long double resolution;//rozlišení metrů na jeden pixel rastrového podkladu
		double X,Y;//logické souřadnice (v metrech) umístění rastrového podkladu v projektu
		bool show;//indikace zda je raster zobrazen
		bool grayscale;//zda bude či nebude rastrový podklad v odstínech šedi či nikoliv
		int dim;//úroveň ztlumení
	};

	struct C_pohon//pro konverzi do bináru
	{
			unsigned int n;
			unsigned int text_length;
			double rychlost_od;//minimální pracovní rychlost dopravníku
			double rychlost_do;//maximální pracovní rychlost dopravníku
			double aRD;//aktuální rychlost dopravníku m/s
			double roztec;//rozteč palců v m
      unsigned int roztec_ID;//uchovává v sobě ItemIndex rozteče
			double Rz;//rozestup aktivních palců v m
			double Rx;//rozestup aktivních palců (počet aktivních palců)
	};
	struct C_objekt//pro konverzi do bináru
	{
			unsigned int n;
			unsigned int id;
			unsigned int text_length;
			double X, Y;
			double Xt,Yt;
			double Xp,Yp;
			double orientace_text;//orientace nadpisu objektu
      unsigned long pocet_bodu;
			unsigned long element_n;
			unsigned long pocet_komor;
      unsigned long pocet_teplomeru;
			unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
			unsigned short stavPM;//obsahuje stav pohonových tabulek v objektu 0-normal (zobrazeny rychlosti a comba, dafaultní stav), 1-minimized (pouze hlavička tabulek a comba), 2-maximized (vše zobrazeno)
      double sirka_steny;//šířka stěny kabiny objektu v metrech
			double orientace;//orientace objektu
			unsigned int pohon;//"id" resp. n přidruženého - roletkou vybraného pohonu
			double delka_dopravniku;//delka dopravníku v rámci objektu
      TPointD koty_elementu_offset;
			bool zobrazit_koty;//proměnná určující, zda se budou zobrzovat kóty - NEW pridat do binarky
			bool zobrazit_mGrid;//proměnná určující, zda budou zobrazeny mGridy - NEW pridat do binarky
			bool uzamknout_nahled;//proměnná určující, zda bude či nebude možné používat interaktivní prvky v náhledu objektu - NEW pridat do binarky
	};

   struct C_bod//pouze pridruzeny spojak
	{
    unsigned long n; //pořadí objektu ve spoj.seznamu
		double X, Y;//umístění v logických (metrických) souřadnicích
    double kota_offset;//odsazení kót v metrech
	};

	struct C_hala
	{
		double name_delka;
    double X, Y;//umístění názvu v logických (metrických) souřadnicích
		unsigned long pocet_bodu;
  };

  struct C_komora
	{
    unsigned long n; //pořadí ve spoj.seznam
		double velikost;//v metrech
    short typ;//1-se sprchou, 0 bez jen okap
	};

	struct C_davka
	{
		unsigned long n;
		unsigned long pocet_voziku;
		unsigned long pocet_prazdnych;
		unsigned long pocet_celkem;
	};

	struct C_zakazka
	{
			unsigned long n;//pořadí objektu ve spoj.seznamu
			//UnicodeString id;//uživatelské ID objektu - nelze UnicodeString
			unsigned int id_length;
			unsigned short typ;//0- realná,1-servisní
			//UnicodeString name;//název zakázky nelze jako UnicodeString
			unsigned int name_length;
			TColor barva;//barva zakáky
			double pomer;//poměr z celkového množství výrobků
			double TT;
			TJig jig;//šířka délka, výška, rotace a  ks připadajících na jig/rám vozíku
			unsigned long pocet_segmentu_cesty;//udržuje počet jednotlivých segmentů cesty, slouží kvůli tomu, aby se vědělo, kolikrát cyklem načítat strukturu která načítá jednotlivé segmenty konkrétně objekty typu TCesta dané zakázky
      unsigned long pocet_davek;
			unsigned long pocet_voziku;//počet vozíků v zakázce
			unsigned long serv_vozik_pocet;//počet servisních vozíků v zakázce
			unsigned long opakov_servis;//cyklus opakování servisních vozíku
	};

	struct C_element//pouze pridruzeny spojak
	{
			unsigned long n; //pořadí ve spoj.seznamu
			unsigned int eID; //id typu elementu: 0 - stop stanice, 1 - robot, 2 - robot se stop stanicí, 3 - robot s pasivní otočí, 4 - robot s aktivní otočí (resp. s otočí a stop stanicí), 5 - otoč pasivní, 6 - otoč aktivní (resp. otoč se stop stanicí), 7 - pouze geometrická zarážka
			unsigned int identifikator_vyhybka_spojka;//uchovává identifikátor spojky a výhybky
		 //	UnicodeString short_name;//krátký název max. 4 znaky
		 //	UnicodeString name;//celý název objektu
			double name_delka;  // celý název objektu
			double X, Y, Z;//umístění v logických (metrických) souřadnicích
			double Xt,Yt;//umístění tabulky, resp. mGridu v logických (metrických) souřadnicích
			short orientace;//v jaké orientaci je element na obrazovce vykreslen 0,90,180,270
			double rotace_jig;//úhel o který element orotuje jig vzhledem k jeho aktuální rotaci jigu vůči podvozku, např. rotace_jig=90°, aktuální rotace jigu 90°, výsledek 180°
			short stav;
			double OTOC_delka;
			double zona_pred;
			double zona_za;
			double PTotoc;
			double WT1;//čekání na palec
			double WT2;//čekání na palec, pro sekundární větev

			Tdata data;

			unsigned long sparovany_n;
			unsigned long objekt_n;//příslušnost elementu k objektu
			unsigned long pohon_n;//příslušnost elementu k pohonu
			TGeometrie geo;
	};

  struct C_cesta//pouze přidružený spoják, který je součástí zakázky, jeden objekt spojáku je jeden segment cesty
	{
			unsigned long n;//n segmentu cesty
			unsigned long n_element;
			unsigned long n_sparovany;
			Tdata data;
	};

	struct C_teplomery//uchovává v sobě teploměry a jejich cestu pro různé zakázky
	{
		unsigned long n;
		unsigned long Z_n;
		unsigned long pocet_sehmentu_cesty;
	};

//	struct C_vozik//pro konverzi do bináru
//	{
//		unsigned int n; //pořadí objektu ve spoj.seznamu
//		unsigned int id_length;   //délka řetězce id
//		double delka;
//		double sirka;
//		double vyska;
//		double rotace;
//		unsigned int text_length;
//		double max_vyrobku;
//		double akt_vyrobku;
//		double delka_vcetne_vyrobku;
//		double sirka_vcetne_vyrobku;
//		double vyska_vcetne_vyrobku;
//		short stav;//-1 - není na lince, 0 - stop, 1 - čeká, 2 - jede
//		TColor barva;
//	};
//

	unsigned int vrat_kapacitu_objektu(TObjekt *O);//SMAZAT????stačí v sekci private, protože ukládám přímo přímo do atributů objektu pomocí uloz_doporucene_kapacity_objetku();
//
//protected:
};
//---------------------------------------------------------------------------
#endif

