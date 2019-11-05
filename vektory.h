//---------------------------------------------------------------------------
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
		double delka;
		double radius;
		double orientace;
		double rotacni_uhel;
		double X1,Y1,X2,Y2,X3,Y3,X4,Y4;//body bézieru
	};

	struct TRetez
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		TGeometrie geo;
		unsigned int eID; //id typu elementu viz. tabulka elementů https://docs.google.com/spreadsheets/d/1_S7yp1L25tov0mKqP3Rl_0Y2cx-e3UwDXb102hDvRlA/edit?usp=sharing
		struct TRetez *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TRetez *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};

	struct TPohon
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		UnicodeString name;//název
		double rychlost_od;//minimální pracovní rychlost dopravníku
		double rychlost_do;//maximální pracovní rychlost dopravníku
		double aRD;//aktuální rychlost dopravníku m/s
		double roztec;//rozteč palců v m
		double Rz;//rozestup aktivních palců v m
		double Rx;//rozestup aktivních palců (počet aktivních palců)
		TRetez *retez;//geometrie pohonu, řetězu
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

	struct TElement
	{
		unsigned long n; //pořadí ve spoj.seznamu
		unsigned int eID; //id typu elementu viz. tabulka elementů https://docs.google.com/spreadsheets/d/1_S7yp1L25tov0mKqP3Rl_0Y2cx-e3UwDXb102hDvRlA/edit?usp=sharing
		UnicodeString short_name;//krátký název max. 4 znaky
		UnicodeString name;//celý název objektu
		double X, Y;//umístění v logických (metrických) souřadnicích
		double Xt,Yt;//umístění tabulky, resp. mGridu v logických (metrických) souřadnicích
		short orientace;//v jaké orientaci je element na obrazovce vykreslen 0,90,180,270 (orientace dle světových stran)
		double rotace_jig;//úhel o který element orotuje jig vzhledem k jeho aktuální rotaci jigu vůči podvozku, např. rotace_jig=90°, aktuální rotace jigu 90°, výsledek 180° - REVIZE!!!
		short stav;//stav elementu: -1 = disabled, nebo nepoužito např. element v knihovně, nebo nepoužitý robot v kabině 0 = stopka otevřeno, v případě elementů bez stopky pouze pasivní element 1 = stopka zavřeno, v případě elementů bez stopky pouze aktivní element 2 = highlight, element zvýrazněn, nikoliv jeho název (předpokladá se při klasickém zobrazení tj. typ==1) 3 = highlight pouze názvu elementu

		short PD;//part detect:  -1 = nic, 0 = začátek jigu, 1 = střed jigu, 2 = celý jig
		double LO1;
		double OTOC_delka;
		double zona_pred;
		double zona_za;
		double LO2;
		double LO_pozice;

		double PT1;
		double PTotoc;
		double PT2;

		double WT;//čekání na palec
		double WTstop;//čekání na stopce
		double RT;//reserve time

		T5Rect citelna_oblast;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast celé kóty(rect0), popisku kóty (rect1), jednotek kóty (rect2) a samotného názvu např. Robot 3 (rect3) elementu, hodnota koty mezi LO (rect4) - nedovávat  do CObjekt

		unsigned int akt_pocet_voziku;
		unsigned int max_pocet_voziku;

		TGeometrie geo;
		TmGrid *mGrid;
		unsigned long objekt_n;//příslušnost elementu k objektu
		TPohon *pohon;//přiřazený pohon k elementu

		//UnicodeString poznamka;//uloží poznámku   //DOPRYC

		struct TElement *sparovany;//ukazatel na následující spárovaný element ve spojovém seznamu (nemusí být totožný s dalším)
		struct TElement *predchozi;//ukazatel na předchozí element ve spojovém seznamu
		struct TElement *dalsi;//ukazatel na  další element ve spojovém seznamu
	};

	struct TObjekt
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		unsigned int id; //id typu objektu
	 	UnicodeString short_name;//krátký název max. 4 znaky         //DOPRYC
		UnicodeString name;//celý název objektu
		double X,Y;//umístění objektu ve schématu - prozatím, bude ale sloužit na umístění popisku objektu!!!
		TBod *body;//definice obrysu haly - NEW + dodat do CObjekt!!!!
    double sirka_steny;//šířka stěny kabiny objektu v metrech  - NEW + dodat do CObjekt!!!!
		double Xk,Yk;//bude ke smazání - umístění levého horního rohu kabiny v layoutu a náhledu kabiny  //DOPRYC
		double Xt,Yt;// souřadnice popisku objektu pro možnost posouvání dodat!!!   //PRIDAT
    double orientace_text;//orientace textu (názvu objektu) dle světových stran
		double Xp,Yp;// souřadnice tab pohonu pro možnost posouvání dodat!!!   //PRIDAT
		short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking)KK,2-Postprocesní (PP), -1 nenastaven, pouzíva tab.pohonu
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
		TElement *elementy;//ukazatel na hlavičku elementů
		TPointD min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
		TPointD rozmer_kabiny;//délka a šířka obvodových zdí kabiny   //DOPRYC
		T2Rect kabinaKotaX_oblastHodnotaAJednotky;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast popisku a jednotek kóty kabiny -//DOPRYC
		T2Rect kabinaKotaY_oblastHodnotaAJednotky;//pouze pomocná proměnná ve fyzických souřadnicích (px), uchovávájící oblast popisku a jednotek kóty kabiny -//DOPRYC
		TPointD koty_elementu_offset;//.x=odsazení kót elementů v metrech normální stav, .y=odsazení kót elementův metrech editace geometrie  - NEW + dodat do CObjekt!!!!
		TKomora *komora;//ukazatel na případné komory objektu - NEW + dodat do CObjekt
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
		struct TObjekt *predchozi2;//ukazatel na předchozí2 objekt ve spojovém seznamu
		struct TObjekt *dalsi;//ukazatel na  další objekt ve spojovém seznamu
		struct TObjekt *dalsi2;//ukazatel na další2 objekt ve spojovém seznamu
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
		unsigned int Opak;//počet opakování jak často se čištění opakuje
		double RD;//rychlost dopravníku
		double Rotace;// úhel natočení jigu v objektu pro danou zakázku   - odstranit
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
		struct TZakazka *zakazka;//ukazatel na přidruženou zakázku
		short typ;//0-normální, 1 - servisní
		double start;//výchozí pozice v grafu časových os
		double pozice;//aktuální pozice na dopravniku či v grafu časových os
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

	struct T_parametry_projektu //(Parametry výroby + Parametry linky (vozíky)
	{
    int katalog;//ID katalogových komponent zvolených pro geometrické elementy linky
    short typ_linky; //0 - podlahový, 1 - podvěsný
		double radius;//nastavený radius komponent linky z katalogu
		TDateTime cas_start;//začátek výroby v SEČ (resp. LSEČ)
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
		T_raster raster;
		T_vector vector;//DOPRYC - zakomentovat
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

	struct TProces
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

	struct TOdstavka
	{
		unsigned long n; //pořadí objektu ve spoj.seznamu
		double datum_od;
		double datum_do;
		struct TOdstavka *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TOdstavka *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TOdstavka ODSTAVKY;//seznam plánovaných odstávek linky

	struct TDoubleHodnota
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

	struct TFile_hlavicka
	{
		unsigned short int Verze;
		unsigned short int Mod;
		double Zoom;
		long PosunutiX;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
		long PosunutiY;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
		unsigned int pocet_objektu;
		unsigned int pocet_vyhybek;
		unsigned int pocet_pohonu;
		unsigned int pocet_zakazek;
		unsigned int pocet_voziku;
		//parametry projektu (PP):
		TDateTime cas_start;//začátek výroby v SEČ (resp. LSEČ)
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
    int katalog;
		short typ_linky;
    double radius;
    short objekt_posunout_vse;
	};
	TFile_hlavicka File_hlavicka;

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
	bool PtInBody(TObjekt *Objekt=NULL);//ověří zda se souřadnicích myši nachází ve vnitř polygonu, pokud je Objekt==NULL, hledá se v polygonu HALy
	void kopiruj_body(TObjekt *Original,TObjekt *Kopie);//zkopíruje body včetně z originálu na kopii bez ukazatelového propojení, funguje jenom pro body objektů nikoliv HALY!!!
	void smaz_bod(TBod* Bod,TObjekt* Objekt=NULL);//smaže konkrétní bod, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU
	void vymaz_body(TObjekt* Objekt=NULL);//vymaže všechny body včetně hlavičky, pokud je ukazatel na Objekt NULL, jedná se o metodu pro HALU

//výhybky
	unsigned int pocet_vyhybek;//uchovává počet přidaných vyhybek, - NEW + dodat do CObjekt a do souborové hlavičky včetně souvisejícího!!!
	bool akt_vetev;//nese informaci o jakou větev ve schématu se jedná, true = primární (dalsi) false = sekundarní (dalsi2)

//metody pro OBJEKTY
	void hlavicka_OBJEKTY();
	TObjekt *vloz_objekt(unsigned int id, double X, double Y);//vloží prvek do seznamu + vrátí ukazatel na vložený prvek
	TObjekt *vloz_objekt(unsigned int id, double X, double Y,TObjekt *pred,TObjekt *po);//přetížená fce vkládá objekt za objekt p + vrátí ukazatel na vložený prvek
	TObjekt *vloz_objekt(unsigned int id, double X, double Y,TObjekt *vyhybka,TObjekt *pred,TObjekt *po);//přetížená fce vkládá spojku + vrátí ukazatel na vložený prvek
	void vloz_objekt(TObjekt *Objekt);//přetížená fce
	TObjekt *Cvektory::nastav_atributy_objektu(unsigned int id, double X, double Y);//alokuje paměť pro objekt, nastavý atriuty objektu, vrátí ukazatel na nově vytvořený prvek
	TObjekt *kopiruj_objekt(TObjekt *Objekt,short offsetX=0,short offsetY=0,AnsiString index_name="",bool remove_pre_index=false,TObjekt *p=NULL);//zkopíruje objekt Objekt na konec spojového seznamu Objektů, za předpokladu že p==NULL, pokud p není NULL je objekt za tento objekt p ve spojovém seznamů objektů zařazen, hodnota offsetu je hodnota odsazení zkopírovoaného objektu od objektu vzorového,index_name slouží pro rozlišení např. LAK, LAK1, LAK2...,zároveň vrací ukazatel na právě zkopírovaný objekt např. pro další použití
	void kopiruj_objekt(TObjekt *Original,TObjekt *Kopie);//zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze ukazatel na mGrid originálu zůstané propojený
	TObjekt *PtInObjekt();//ověří, zda se na souřadnicích myši nachází nějaký objekt, pokud ano, vrátí na něj ukazatel, jinak vrátí NULL
	TObjekt *najdi_objekt(double X, double Y,double offsetX, double offsetY,short typ=-1);//hledá bod v dané oblasti
	short oblast_objektu(TObjekt *O,double X, double Y);//vrátí ret podle toho v jaké jsem oblasti objektu, slouží k přilepování objektu při vkládání, ret = 0 - mimo oblasti, 1 - oblast za objektem, 2 - oblast před objektem
	TObjekt *vrat_objekt(unsigned int n);//dle zadaného n vrátí ukazatel na hledaný objekt
	TObjekt *vrat_objekt_z_roma(int X);//dle X kurzoru myši vrátí z modu procesy (ROMA) ukazatel na aktuální objekt
	TObjekt *vrat_objekt(TElement *Element,bool In_pom_temp=false);//vrátí rodičovský Objekt daného elementu, In_pom_temp - zda bude hledat místo daného ostrého objektu v něm
	void aktualizace_objektu(short typ);//dle zadaného TT  či případně dalších hodnot zaktualizuje paramametry všech objektů//typ -2://zaktualizuje přiřazení pohonu k objektu, nutné pokud proběhla změna v pohonech, protože původní jsou smazané //typ -1://dle zamčených a odemčených hodnot při změně TT//typ 0://dle zamčených a odemčených hodnot při změně parametrů vozíku//typ 1://při změně TT změna CT a RD, K a DD zůstává//typ 2://při změně TT změna K,DD,RD zůstává CT//typ 3://při změně parametrů vozíku změna DD, RD zůstává K, CT//typ 4://při změně parametrů vozíku změna u DD, CT zůstává K,RD//typ 5://při změně parametrů vozíku změna u K,CT,RD zůstává DD
	void aktualizace_prirazeni_pohonu_k_objektum(unsigned int oldN,unsigned int newN);//všem objektům, které měly přiřazen pohon s oldN(oldID), přiřadí pohon s newN(newID), podle toho, jak jsou ukládány nově do spojáku, důležité, pokud dojde k narušení pořadí ID resp n pohonů a pořadí jednotlivých řádků ve stringridu, např. kopirováním, smazáním, změnou pořadí řádků atp.
	void aktualizace_prirazeni_pohonu_dokoncena();//po dokončení aktualizace přiřazení pohonu (při ukládání pohonu na PL) vrátí atribut probehla_aktualizace_prirazeni_pohonu všech objektů na false, aby bylo připraveno k dalšímu opětovnému užítí, nepřímo spolupracuje s metodou výše uvedenou aktualizace_prirazeni_pohonu_k_objektum
	double vrat_soucet_delek_vsech_objektu();//sečte délky jednotlivých objektů
	unsigned int pocet_objektu(short typ=-1);//vrátí počet objektů v požadovaném režimu, nebo v případě implicitního parametru -1 objekty ve všech režimech
	unsigned int pocet_objektu_zakazky(TZakazka *Zakazka,short typ=-1);//vrátí počet objektů v požadovaném režimu pro danou zakázku, nebo v případě implicitního parametru -1 objekty ve všech režimech pro danou zakázku
	AnsiString vypsat_objekty_bez_prirazenych_pohonu(bool shortname=true,AnsiString separator=", ");//vrátí AnsiString řetezec shortname či name (dle prvního parametru, který je implicitně na shortname=true) seznam objektů, které nemají přiřazený pohon, jednotlivé názvy objektů oddělí dle paramaterů seperátor, implicitně ", " tj. čárka a mezera,, v případě že žádný objekt nenajde, vrátí prázdný řetězec
	unsigned long vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(bool s,short rezim=-1);//vrátí počet objektů bez či s přiřazenými pohony (dle vstupního parametru), parametr režim ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	AnsiString vypis_objekty_vyuzivajici_pohon(unsigned long n,bool short_name=true);//dle n pohonu vráti objekty, které pohon používají, pokud je short_name na true, vrátí kratký název objektu jinak dlouhý
	unsigned long vrat_pocet_objektu_vyuzivajici_pohon(unsigned long n, short rezim=-1);//vratí počet objektů přiřazených k danému pohonu, parametr režim ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	TObjekt *vrat_objekty_vyuzivajici_pohon(unsigned long n, short rezim=-1);//vratí formou ukazatele na pole objekty přiřazené k danému pohonu, parametr režim, ve všech režimech -1, 0 - S&G, 1-KK, 2 - PP
	TObjekt *vrat_objekty_bez_pohonu();//vratí formou ukazatele na pole objekty bez pohonů
	AnsiString vypis_objekty_s_pohony_bez_roztece(bool shortname=true);//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů, které mají přiřazený pohon bez uvedené rozteče jednotlivé názvy objektů oddělí  ", " tj. čárkou a mezerou, v případě že žádný objekt nenajde, vrátí prázdný řetězec, pozor pohony bez přiřazení k objektům nevypisuje
	AnsiString vypis_objekty_mimo_100vytizeni(bool shortname=true, bool vypsat_procetna=true, AnsiString separator=", ");//vrátí AnsiString řetezec shortname či name (dle parametru, který je implicitně na shortname=true) seznam objektů podle zakázek, které nemají 100% vytížení
	double vrat_min_rychlost_prejezdu();//najde ze všech objektů nejnižší rychlost přejezdu (tj. totál min RD), neřeší přiřazení k pohonům, pomůcka pro stanovení referenční rychlosti animace
	short smaz_objekt(TObjekt *Objekt,bool opakovani=false);//smaže prvek ze seznamu, opakování určuje zda se smazání provedlou jednou či 2x, v případě odstranění vyhybky je potřeba odstranit i její spojku
	void zmen_poradi_objektu(TObjekt *aktualni_poradi,TObjekt *nove_poradi);
	void zmen_poradi_objektu(unsigned long aktualni_poradi,unsigned long nove_poradi);
	void nove_nazvy();//projde všechny objekty, výhybkám a spojkám upravý návez podle jejich n
	void nove_indexy(bool nasledne_zmena_nazvu=false);//projde všechny objekty a nastavý nové indexy podle aktuálního pořadí objektů
	void ortogonalizovat();//ortogonalizuje schéma
	TObjekt *dalsi_krok(TObjekt *Objekt,TPoint *tab_pruchodu);//určuje další krok cyklu při procházení objektů
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
	void hlavicka_elementy(TObjekt *Objekt);//danému objektu vytvoří hlavičku elementů
	TElement *vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y,short rotace_symbolu,TElement *Ep=NULL);//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
	void vloz_element(TObjekt *Objekt,TElement *Element,TElement *force_razeni=NULL);//vloží element do spojového seznamu elementů daného technologického objektu
	Cvektory::TElement *vloz_element_za(TObjekt *Objekt,TElement *Element);//kontrola zda vkládaný element bude umístěn na konec nebo mezi jiné elementy, pokud bude vkládán na konec return=NULL, pokud mezi elementy return=ukazatel na předchozí element
	void vloz_G_element(TElement *Element,short typ,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4,double orientace=0,double rotacni_uhel=0,double radius=0,double delka=0);//danému elementu přiřadí/naplní geometrickou složku
	void uprav_popisky_elementu(TObjekt *Objekt, TElement *Element);//upraví indexy a popisky elementů po vloženém elementu (parametr Element), pokud dostane parametrem Element NULL přejmenuje a přeindexuje všechny ovlovněné elementy do původního stavu (tlačítko storno)
	void kopiruj_element(TElement *Original, TElement *Kopie);//zkopíruje atributy elementu bez ukazatelového propojení, pouze ukazatelové propojení na mGrid je zachováno
	void kopiruj_elementy(TObjekt *Original, TObjekt  *Kopie);//zkopíruje elementy a jejich atributy bez ukazatelového propojení z objektu do objektu, pouze ukazatelové propojení na mGrid je zachováno spojuje dvě metody vloz_element(TObjekt *Objekt,TElement *Element) a kopiruj_element(TElement *Original, TElement *Kopie);
	void vytvor_elementarni_osu(TObjekt *Original, TObjekt  *Kopie);//připraví vektor provizorní osy pohonu
	int vrat_eID_prvniho_pouziteho_robota(TObjekt *Objekt);//vratí eID prvního použitého robota, slouží na filtrování, jaké roboty v knihovně robotů zakazazovat, pokud není nic nalezeno vrátí -1
	unsigned int vrat_poradi_elementu(TObjekt *Objekt,unsigned int eID);//vratí pořádí stopek, robotů a otočí zatím pouze v elementu, bude na zvážení rozšíření na všechny objekty
	unsigned int vrat_poradi_elementu_do (TObjekt *Objekt, TElement *Element);//vrátí pořadí robotů v objektu, stopek a otočí ve všech předchozích objektech, to všd do Elementu
	unsigned int vrat_nejvetsi_ID_tabulek (TObjekt *Objekt);//vrátí největší ID napříč mGridy v objektu, používáno pro přiřazování ID novým tabulkám, řešeno takto z důvodu chyby při odmazávání a následném přidávání elementu (v kabině jsou 3 elementy druhý se odmaže, tabulky v kabině mají nyní ID 1 a 3, po přidání dalšího elementu bylo dříve přidano ID=pocet elementů, což by se v tomto případě rovnalo 3)
	void rotace_elementu(TObjekt *Objekt,short rotace);//orotuje všechny elementy daného objektu o danou hodnotu
	TElement *najdi_element(TObjekt *Objekt, double X, double Y);//hledá element v místě kurzoru pracuje v logických/metrických souradnicích
	TElement *najdi_tabulku(TObjekt *Objekt, double X, double Y);//hledá tabulku elementu pouze pro daný objekt v oblasti definované pomocí šířky a výšky tabulky (která se může nacházet v daném místě kliku), pracuje v logických/metrických souradnicich, vrátí ukazatel na daný element, který tabulku vlastní, pokud se na daných souřadnicích nachází tabulka
	TElement *vrat_element(TObjekt *Objekt, unsigned int n);//vraťí ukazatel na element dle n elementu umístěného v daném objektu
	short PtInKota_elementu(TObjekt *Objekt,long X,long Y);//ověří zda se na daných fyzických souřadnicích nachází kóta elementu, pokud ne vrací -1, pokud ano 0 v celé kótě, 1 - na hodnotě kóty, 2 - na jednotkách kóty , 3 - na hodnotě LO kóty , pozn. oblast kóty se testuje až jako poslední
	bool posun_element(TElement *Element,double vzdalenost,bool pusun_dalsich_elementu=false,bool posun_kurzorem=false,bool kontrola_zmeny_poradi=true);//posune pouze Element z pomocného spojového seznamu pom_temp na parametrem uvedenou vzádlenost (v metrech) od elementu předchozího, pokud je implicitní hodnota pusun_dalsich_elementu false změněna na true, jsou o danou změnu posunu přesunuty i elementy následující Elementu (tudíž jejich vzdálenost od Elementu bude zachována, naopak v případě výchozí hodnoty false je následujícím/dalším elementům poloha zachována). Nutá rozdílná funkce při posunu z kót a při posunu korzorem, proto parametr posun_kurzorem
	void posuv_aktualizace_RT(TElement *Element);//posunem elementu tj. změnou vzdálenosti od předchozího se ovlivní hodnota RT, nutno přepočítat
	void zmen_poradi_elementu(TElement *E,TElement *Ed);//řeší změnu pořadí při posuvu elementů, dojde k novému ukazatelovému propojení, přejmenování a přeindexování elementů
	double vzdalenost_od_predchoziho_elementu(TElement *Element,bool pouzeSG=false);//vratí vzdálenost od předchozího elementu, pracuje zatím pouze v orotogonalizovaném prostoru (bude nutno vylepšit s příchodem oblouků), pokud se jedná o první element, uvažuje se jako vzdálenost od počátku kabiny (nutno vylepšit ještě pro různé orientace kabiny)
	double vrat_rotaci_jigu_po_predchazejicim_elementu(TObjekt *Objekt,TElement *Element);//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje
	double vrat_rotaci_jigu_po_predchazejicim_elementu(TElement *Element);//zadávám aktuální element, je zjištěna rotace před tímto zadávaným elementem, rotace aktuálního elementu se nezohledňuje
	TElement *vrat_posledni_rotacni_element();//metoda vrátí ukazatel na poslední rotační element na lince
	//dodělat void zmen_poradi_Elementu(TObjekt *Objekt, TElement Element,TObjekt *zaElement);
	//dodělat void zmen_poradi_Elementu(TObjekt *Objekt unsigned long aktualni_poradi,unsigned long nove_poradi);
	void napln_comba_stopek();//obsah všech comboboxu všech stopek nejdříve smaže a následně naplní combobox stopky ostatními elementy, které mohou být s danou stopkou spárované, nevypisuje danou stopku, vybere v combu stop-element spárovaný či předchozí, buď navržený nebo uživatelsky vybraný
	void napln_combo_stopky(TElement *Stopka);//nejdříve smaže obsah comboboxu a následně naplní combobox stopky ostatními elementy, které mohou být s danou stopkou spárované, nevypisuje danou stopku, vybere v combu stop-element spárovaný či předchozí, buď navržený nebo uživatelsky vybraný
	void uloz_sparovany_element(TElement *Stopka);//uloží dané stopce ukazatel na sparovaný stop element, který byl vybraný v Combu dané stopky, ošetřuje zda se jedná o stopku
	void vrat_predchozi_stop_element(TElement *Element,TObjekt *Objekt);//dané stopce najde předchozí stop-element na lince, je možno, že nebude reflektovat danou zakázku//nově se podívá na předchozí stop-element a přiřadí mu ukazatel na Element
	void aktualizuj_sparovane_ukazatele();//projde všechny stop-elementy a aktualizuje jim ukazatele na spárované elementy
	TElement *Cvektory::vrat_predchozi_element(TElement *Element);//vrátí předchozí element k Element, který byl do metody poslán jako parametr, přeskočí geometrické zarážky
	void smaz_element(TObjekt *Objekt, unsigned int n);//smaže element ze seznamu
	void smaz_element(TElement *Element);//smaže element ze seznamu
	long vymaz_elementy(TObjekt *Objekt,bool mGridSmazat=true);//vymaže všechny elementy daného objektu včetně hlavičky a vrátí počet smazaných elementů (počítáno bez hlavičky), automaticky, pokud posledním parametreme není nastaveno jinak, smaže přidružený mGrid

//metody pro POHONY
	void hlavicka_POHONY();
	void vloz_pohon(TPohon *pohon);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
	void vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double aRD,double R,double Rz,double Rx);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
	TPohon *vrat_pohon(unsigned long n);//vrátí ukazatel na pohon dle n pohonu
	void kopiruj_pohon(TPohon *Pohon,TObjekt *Objekt);//bez ukazatelového propojení zkopíruje atributu pohonu do pohonu požadovaného objektu, neobsahuje-li tento objekt alokovanou paměť pro pohon, naalokuje jí
	bool pohon_je_pouzivan(unsigned long n,TElement *mimo_element=NULL);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
	TObjekt *pohon_je_pouzivan(unsigned long n,TObjekt *mimo_objekt,short rezim=-1);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv, ten vrátí formou ukazatale na první nalezený používáný, druhý vstupní parametr metody TObjekt mimo_objekt je ukazatel na objekt, který se bude při vyhledávání ignorovat, nenajde-li vrací NULL, třetí parametr, pokud je náchán na implicitní -1 řeší se pro všechny režim, pokud je v rozmezí 0 až 2 řeší se pro konkrétní režim
	unsigned long vrat_pocet_nepouzivanych_pohonu();//vrátí počet nepoužívaných pohonů
	double minRD(TPohon *pohon);//vrátí nejnižší možnou rychlost ze všech objektů, které jsou přiřazené k danému pohonu (využívá se pro S&G a PP, u KK musí být RD v souladu s TT)//pokud vrátí 0, znamená, že pohon není využíván
	AnsiString vypis_objekty_nestihajici_prejezd(TPohon *pohon,double testRD,short rezim=-1);//vypíše objekty přiřazené k danému pohonu nestíhající přejezd při navrhovaném testRD, možno nastavit režim, pro S&G + PP hodnota režim 20
	AnsiString kontrola_rychlosti_prejezdu(TObjekt *O,short rezim,double CT=0,double MT=0,double WT=0,double aRD=0,double DD=0,short aRDunit=-1,unsigned short precision=3,AnsiString mark="..",bool add_decimal=false,AnsiString separator_aRD=" o ");//zkontroluje objekt zda daná rychlost pohonu odpovídá požadované rychlosti pohonu, pokud ne vrátí popis včetně hodnoty, lze poslat externí testovací parametry nebo nechat ověřit dle uložených ve spojáku objekty
	TPohon *najdi_pohon_dle_RD(double RD);//ověří zda je stejná rychlost pohonu na lince používána, pokud není vratí NULL, jinak ukazatel na daný pohon
	void vytvor_retez(TPohon *Pohon);//danému pohonu vytvoří řetěz dle geometrie všech elementů, co spadají pod daný pohon
	void vloz_segment_retezu(TRetez *Retez,TPohon *Pohon);//danému řetězu vloží jeden geometrický segment
	void zrusit_prirazeni_pohunu_k_objektum(unsigned long n);//všem objektům s n pohonem zruší přiřazení k tomuto pohonu a nahradí hodnotu ukazatele na přiřazený pohon za NULL
	long vymaz_seznam_POHONY();//smaže jednotlivé prvky seznamu, včetně hlavičky, pokud následuje další práce se seznamem, je nutné založit nejdříve hlavičku pomocí hlavicka_pohony()
	void smazat_retez(TPohon *pohon);//danému pohonu smaže jeho řetěz
	//	double delka_dopravniku(Cvektory::TObjekt *ukaz);
	//následují 4 možno odstanit:
	TTextNumber rVALIDACE(short VID,unsigned long PID,double aRD,double R,double Rz,double Rx,short aRDunit,short Runit,short Rzunit);//zkontroluje aplikovatelnost uvažovaného hodnodty dle VID parametru, resp. čísla sloupce (aRD=4,R=5,Rz=6,Rx=7 na všech objektech, přiřazených k danému pohonu označeným parametrem PID, pokud je zadán parametr getValueOrMessage 0 (který je zároveň implicitní), vratí doporučenou hodnotu dle VID, pokud je zvoleno 1, vrátí text chybouvé hlášku s problémem a doporučenou hodnotou, pokud vrátí prázdné uvozovky, je vše v pořádku, //vstupy aRD,R,Rz,Rx a výstupní číselná hodnota jsou v SI jednotkách, naopak textový řetězec problému resp. doporučení, obsahuje hodnotu již převedenou dle aRDunit, Runit, Rzunit
private:
	TTextNumber validace_aRD(double aRD,TPohon *p);
	TTextNumber validace_Rz(double Rz,unsigned long PID);
	TTextNumber validace_Rx(double Rx);

//metody pro ZAKAZKY
public:
	void hlavicka_ZAKAZKY();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY
	void vloz_temp_zakazku(UnicodeString id,unsigned short typ, UnicodeString name,TColor barva,double pomer,double TT,TJig jig,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//vytvoří zakázku dle zadaných parametru do spojového seznamu ZAKÁZKY
	void edituj_temp_zakazku(unsigned long n,UnicodeString id,unsigned short typ,UnicodeString name,TColor barva,double pomer,double TT,unsigned long pocet_voziku,unsigned long serv_vozik_pocet,unsigned long opakov_servis);//provede editaci zakázky s uvedeným “n” ze spojového seznamu ZAKAZKY_temp
	void smaz_temp_zakazku(unsigned long n);//smaže zakázku s uvedeným “n” ze spojového seznamu ZAKAZKY_temp včetně přidružených cest
	void zmen_poradi_temp_zakazky(unsigned long aktualni_poradi,unsigned long nove_poradi);//změní zařazení zakázky ve spojovém seznamu
	TZakazka *vrat_temp_zakazku(unsigned long n_zakazky);// vrátí ukazatel (resp. data) na editovanou zakázku
	void kopirujZAKAZKY_temp2ZAKAZKY(bool mazat_ZAKAZKY_temp=true);//po stisku OK v superformu zkopíruje data z ZAKAZKY_temp do ZAKAZKY, implicitně následně smaže ZAKAZKY_temp
	void kopirujZAKAZKY2ZAKAZKY_temp();//zkopíruje ukazatel na ZAKAZEK do ZAKAZKY_temp, slouží v momentu načítání SF
	void prvni_zakazka_dle_schematu();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
	long vymaz_seznam_ZAKAZKY_temp();//smaze seznam ZAKAZKY_temp z paměti včetně přidružených cest, nutno implementovat při close() superformu (ať už při OK, storna, či křížku formu)
private:
	void hlavicka_ZAKAZKY_temp();//vytvoří novou hlavičku pro spojový seznam ZAKAZKY_temp, nutná volat při on_show superformuláře
	void vloz_temp_zakazku(TZakazka *Zakazka_temp);//vloží vytvořenéu zakázku do spojového seznamu ZAKÁZKY_temp
	void vloz_zakazku(TZakazka *Zakazka);//vloží hotovou zakázku do spojového seznamu ZAKÁZKY
	long vymaz_seznam_ZAKAZKY();//smaze seznam ZAKAZKY z paměti v četně přidružených cest, pokud následuje další práce se seznamem (např. nové vkládání), je nutné založit nejdříve hlavičku pomocí hlavicka_ZAKAZKY()
//metody pro cesta konkrétní zakázky
public:
	void inicializace_cesty(TZakazka *zakazka);//vymaže předchozí cestu a zavolá hlavičku cesty nové
	void vloz_segment_cesty(TZakazka *zakazka,unsigned int n_vybraneho_objektu/*z comboboxu*/,double CT,double Tc,double Tv,double RD,unsigned int opak);//do konkrétní cesty vloží segmenty cesty,  bude užito v metodě při stisku OK, při vkládání každého řádku stringgridu v daném for cyklu.
	void vloz_segment_cesty(TZakazka *zakazka,TObjekt *vybrany_objekt,double CT,double Tc,double Tv,double RD,unsigned int opak);//do konkrétní cesty vloží segmenty cesty,  bude užito v metodě při stisku OK, při vkládání každého řádku stringgridu v daném for cyklu.
	TCesta *obsahuje_segment_cesty_objekt(TObjekt *objekt,TZakazka *zakazka);//ověří zda daný objekt je součástí cesty dané zakázky či nikoliv, pokud ano vrací ukazatel na daný segment cesty
	TZakazka *obsahuje_segment_cesty_objekt(TObjekt *objekt);//ověří zda daný objekt je součástí cesty nějaké zakázky či nikoliv, pokud ano vrací ukazatel na danou zakázku
	void aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(TZakazka *zakazka,double TT);//dle TT z parametru nastaví všem segmentům cesty od dané zakázky odpovídající CT (a line-tracking objektů i RD) dle fixní délky a kapacity, vhodné pro volání před zobrazením cest
	void aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky(TZakazka *zakazka);//dle TT zakázky nastaví všem segmentům cesty od dané zakázky odpovídající CT (a line-tracking objektů i RD) dle fixní délky a kapacity, vhodné pro volání před zobrazením cest
	void aktualizace_CTaRD_segmentu_cesty_dleTT_zakazky();//to samé co výše ale uskuteční pro všechny zakázky, vhodné pro volání v tlačítku uložit
	void aktualizace_KaCTaRD_segmentu_cesty_dleJIG(TZakazka *zakazka);//dle parametrů JIG přepočítá K (u S&G zanechá 1) a z toho vyplývající změnu CT a RD (u linetracking objektů) jednolivých segmentů cesty dané zakázky
private:
	void hlavicka_cesta_zakazky(TZakazka *zakazka);//vytvoří novou hlavičku pro spojový seznam konkrétní cesty dané zakázky
	void vymaz_cestu_zakazky(TZakazka *zakazka);//vymaže celou cestu dané zakázky
	void vloz_segment_cesty(TZakazka *zakazka,TCesta *segment_cesty);//do konkrétní zakázky vloží segmenty cesty

//metody pro VOZIKY
public:
	void hlavicka_VOZIKY();//vytvoří hlavičku spojového seznamu VOZIKY
	void generuj_VOZIKY();//vygeneruje podle zadaných zakázek seznam vozíků (včetně případných servisních), seřazeno dle zakázek
	void vymazat_casovou_obsazenost_objektu_a_pozice_voziku(TObjekt *Objekt,TVozik *Vozik);//slouží při úvodním načítání časových os, smaže výchozí a koncovou pozici sloužící pro tvorbu a zobrazení na časových osách
	TVozik *vrat_vozik(unsigned int n);//dle n resp. ID vozíku vrátí ukazatel na daný vozík
private:
	void vloz_vozik(TZakazka *zakazka,short typ);//0-normální, 1-servisní
	long vymaz_seznam_VOZIKY();

//metody pro PROCESY
public:
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
	void vymaz_seznam_KATALOG();//smaže celý katalog, včetně přidružených spojových seznamů

//metody pro PALCE
// void hlavicka_palce();
// void vloz_palec();//přidá nový vozík do seznamu PALCE
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

//odstraní všechny vektory (všechny globální spojáky)
	void vse_odstranit();

//souborové operace
	void vytvor_hlavicku_souboru();
	short int uloz_do_souboru(UnicodeString FileName);
	short int nacti_ze_souboru(UnicodeString FileName);
	short int ulozit_report(UnicodeString FileName);
	void nacti_CSV_retezy(AnsiString FileName);
	AnsiString ReadFromTextFile(AnsiString FileName);
	void SaveText2File(AnsiString Text,AnsiString FileName);

//technické, statistické a ekonomické ukazatele
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

//SQL
	AnsiString QUERY(AnsiString query);//vratí AnsiString hodnod dle zadaného dotazu v syntaxi SQL, zatím umí jen základní úroveň

//pomocné metody
	void Text2CSV(AnsiString text,AnsiString FileName="",AnsiString Title="",AnsiString DefaultExt="",AnsiString Filter="",bool openDialog=true,bool open=true);//vytovoří a otevře CSV, je-li požadováno

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
			double orientace_text;//orientace nadpisu objektu
      unsigned long pocet_bodu;
			unsigned long pocet_elementu;
      unsigned long pocet_komor;
			unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
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
			unsigned long pocet_voziku;//počet vozíků v zakázce
			unsigned long serv_vozik_pocet;//počet servisních vozíků v zakázce
			unsigned long opakov_servis;//cyklus opakování servisních vozíku
	};

		struct C_element//pouze pridruzeny spojak
	{
			unsigned long n; //pořadí ve spoj.seznamu
      unsigned int eID; //id typu elementu: 0 - stop stanice, 1 - robot, 2 - robot se stop stanicí, 3 - robot s pasivní otočí, 4 - robot s aktivní otočí (resp. s otočí a stop stanicí), 5 - otoč pasivní, 6 - otoč aktivní (resp. otoč se stop stanicí), 7 - pouze geometrická zarážka
		 //	UnicodeString short_name;//krátký název max. 4 znaky
     //	UnicodeString name;//celý název objektu
			double name_delka;  // celý název objektu
      double X, Y;//umístění v logických (metrických) souřadnicích
			double Xt,Yt;//umístění tabulky, resp. mGridu v logických (metrických) souřadnicích
      short orientace;//v jaké orientaci je element na obrazovce vykreslen 0,90,180,270
			double rotace_jig;//úhel o který element orotuje jig vzhledem k jeho aktuální rotaci jigu vůči podvozku, např. rotace_jig=90°, aktuální rotace jigu 90°, výsledek 180°
      short stav;
      short PD;//part detect:  -1 = nic, 0 = začátek jigu, 1 = střed jigu, 2 = celý jig

      double LO1;
			double OTOC_delka;
//			double zona_pred;
//			double zona_za;
      double LO2;
			double LO_pozice;

			double PT1;
      double PTotoc;
      double PT2;

      double WT;//čekání na palec
      double WTstop;//čekání na stopce
			double RT;//reserve time

      unsigned int akt_pocet_voziku;
      unsigned int max_pocet_voziku;
      unsigned long objekt_n;//příslušnost elementu k objektu
      unsigned long pohon_n;//příslušnost elementu k pohonu
			TGeometrie geo;
	};

	struct C_cesta//pouze přidružený spoják, který je součástí zakázky, jeden objekt spojáku je jeden segment cesty
	{
			unsigned long n;//n segmentu cesty
			unsigned long n_objekt;//n na vybraný objekt
	  	double CT; //cycle time
			double Tc;//čaš čištění v rámci zakázky resp. stejné barvy, vztahuje se na konkrétní objekt a a zároveň zakázku, musí být tady, pokud není použito, tak 0
			double Tv;//čas čištění a výměny barev, vztahuje se na konkrétní objekt a a zároveň zakázku, musí být tady, pokud není použito, tak 0
			double RD;//rychlost dopravníku
			unsigned int Opak;//počet opakování jak často se čištění opakuje
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

	unsigned int vrat_kapacitu_objektu(TObjekt *O);//stačí v sekci private, protože ukládám přímo přímo do atributů objektu pomocí uloz_doporucene_kapacity_objetku();
//
//protected:
};
//---------------------------------------------------------------------------
#endif


