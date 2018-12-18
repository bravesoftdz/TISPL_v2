//---------------------------------------------------------------------------
#ifndef vektoryH
#define vektoryH
#include <vcl.h>
#include "knihovna_objektu.h"
#include "my.h"
#include "TmGrid.h"
//#define TITULEK "Omap editor"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class Cvektory
{
  private:
	Cmy m;//instance na my knihovnu

	public:

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
			struct TPohon *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TPohon *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	TPohon *POHONY;//spojový seznam pohonů

	struct TElement
	{
			unsigned long n; //pořadí ve spoj.seznamu
			unsigned int eID; //id typu elementu: 0 - stop stanice, 1 - robot, 2 - robot se stop stanicí, 3 - robot s pasivní otočí, 4 - robot s aktivní otočí (resp. s otočí a stop stanicí), 5 - otoč pasivní, 6 - otoč aktivní (resp. otoč se stop stanicí)
			UnicodeString short_name;//krátký název max. 4 znaky
			UnicodeString name;//celý název objektu
			double X, Y;//umístění
			double Xt,Yt;//umístění tabulky, resp. mGridu
			short rotace_symbolu;//v jaké orientaci je element na obrazovce vykreslen 0,90,180,270
			double rotace_jigu;//úhel rotace jigu vůči podvozku
			bool stav;

			double LO1;
			double OTOC_delka;
			double LO2;
			double LO_pozice;

			double PT1;
			double PTotoc;
			double PT2;
			double TIME;//CT, PT, WT, RT,...

			unsigned int akt_pocet_voziku;
			unsigned int max_pocet_voziku;

			//Gobjekt, Gelement doplnit
			TmGrid *mGrid;

			UnicodeString poznamka;//uloží poznámku


			struct TElement *sparovany;//ukazatel na následující spárovaný element ve spojovém seznamu (nemusí být totožný s dalším)
			struct TElement *predchozi;//ukazatel na předchozí element ve spojovém seznamu
			struct TElement *dalsi;//ukazatel na  další element ve spojovém seznamu
	};

	struct TObjekt
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			unsigned int id; //id typu objektu
			UnicodeString short_name;//krátký název max. 4 znaky
			UnicodeString name;//celý název objektu
			double X, Y;//umístění objektu
			unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking)KK,2-Postprocesní (PP),3-stopka
			double CT;//pro status návrh
			double RD;//pro status návrh v m/s, jenom pomocná proměnná získaná jako DD/CT, stežejní je většinou aRD (aktuální rychlost), která se váže přímo (i datově) k pohonu
			double delka_dopravniku;//delka dopravníku v rámci objektu
			double kapacita;//uživatelsky zadaná kapacita
			double kapacita_dop;//doporučená, vypočítáná
			double pozice;//počet vozíků v kabině
			double rotace;//rotace jigu v objektu
			double mezera;//mezera mezi vozíky (kritická mezera)
			double mezera_jig;//mezera mezi jigy
			double mezera_podvozek;//mezera mezi podvozky
			TPohon *pohon;//ukazatel na použitý pohon
			TElement *elementy;
			TPointD min_prujezdni_profil;//výška a šířka minimálního průjezdního profilu v objektu
			unsigned short cekat_na_palce;//0-ne,1-ano,2-automaticky
			unsigned short stopka;//zda následuje na konci objektu stopka //0-ne,1-ano,2-automaticky
			double odchylka;//povolená odchylka u PP z CT
			double obsazenost;//slouží pro uchování času obsazenosti pro vykreslování na časových osách
			short CT_zamek;
			short RD_zamek;
			short DD_zamek;
			short K_zamek;
			UnicodeString poznamka;//uloží poznámku ke vzniku CT
			bool probehla_aktualizace_prirazeni_pohonu;//pouze pomocná proměnná využitá v momentu, kdy probíhá nové ukládání pohonů na PL a probíhá aktualizace n, tak ošetření proti situaci např. "2->3 a 3->4"//neukládá se do binárky
			struct TObjekt *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
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
			double Rotace;// úhel natočení jigu v objektu pro danou zakázku
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

	struct T_vector
	{
			int vector;//úroveň ztlumení vektorů - zatím nebude využito
			bool transparent;//zda bude vektorová vrstva transparentní - zatím nebude využito
	};


	struct T_parametry_projektu //(Parametry výroby + Parametry linky (vozíky)
	{
			TDateTime cas_start;//začátek výroby v SEČ (resp. LSEČ)
			unsigned long  mnozstvi;//požadované množství
			double hod_den;//počet hodin za den
			double dni_rok;//počet hodin za den
			double efektivita;//přepokládaná výrobní efektivina
			double TT;//pro návrháře
			double delka_jig;
			double sirka_jig;
			double vyska_jig;
			double delka_podvozek;
			short typ_voziku;//0 - podlahový, 1 - podvěsný
			T_raster raster;
			T_vector vector;
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
	struct TText *TEXTY;

	struct TLinie
	{
			unsigned long n; //pořadí objektu ve spoj.seznamu
			//unsigned int vrstva;//ID vrstvy
			double X1,Y1,X2,Y2;//umístění objektu levý horní okraj
			unsigned short int width;//šířka v px
			TColor barva;
			unsigned short int typ;//typ čáry
			struct TLinie *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
			struct TLinie *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TLinie *LINIE;//seznam linií sloužicích jako poznámky

	struct TRetez
	{
		unsigned int n; //pořadí objektu ve spoj.seznamu
		AnsiString name;//název řetězu
		double roztec;//rozteč palců na řetězů v metrech
		struct TRetez *predchozi;//ukazatel na předchozí objekt ve spojovém seznamu
		struct TRetez *dalsi;//ukazatel na  další objekt ve spojovém seznamu
	};
	struct TRetez *RETEZY;


		struct TFile_hlavicka
		{
					unsigned short int Verze;
					unsigned short int Mod;
					double Zoom;
					long PosunutiX;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
					long PosunutiY;//proměnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souřadnicích zařízení
					unsigned int pocet_objektu;
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
					double typ_vozik;
		};
		TFile_hlavicka File_hlavicka;

//konstruktor
		Cvektory();

//metody pro OBJEKTY
		void hlavicka_OBJEKTY();
		TObjekt *vloz_objekt(unsigned int id, double X, double Y);//vloží prvek do seznamu + vrátí ukazatel na vložený prvek
		TObjekt *vloz_objekt(unsigned int id, double X, double Y,TObjekt *p);//přetížená fce vkládá objekt za objekt p + vrátí ukazatel na vložený prvek
		void vloz_objekt(TObjekt *Objekt);//přetížená fce
		TObjekt *kopiruj_objekt(TObjekt *Objekt,short offsetX=0,short offsetY=0,AnsiString index_name="",bool remove_pre_index=false,TObjekt *p=NULL);//zkopíruje objekt Objekt na konec spojového seznamu Objektů, za předpokladu že p==NULL, pokud p není NULL je objekt za tento objekt p ve spojovém seznamů objektů zařazen, hodnota offsetu je hodnota odsazení zkopírovoaného objektu od objektu vzorového,index_name slouží pro rozlišení např. LAK, LAK1, LAK2...,zároveň vrací ukazatel na právě zkopírovaný objekt např. pro další použití
		TObjekt *najdi_objekt(double X, double Y,double offsetX, double offsetY,short typ=-1);//hledá bod v dané oblasti
		TObjekt *vrat_objekt(unsigned int n);//dle zadaného n vrátí ukazatel na hledaný objekt
		TObjekt *vrat_objekt_z_roma(int X);//dle X kurzoru myši vrátí z modu procesy (ROMA) ukazatel na aktuální objekt
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
		short smaz_objekt(TObjekt *Objekt);//smaže prvek ze seznamu
		void zmen_poradi_objektu(TObjekt *aktualni_poradi,TObjekt *nove_poradi);
		void zmen_poradi_objektu(unsigned long aktualni_poradi,unsigned long nove_poradi);
		void sniz_indexy(TObjekt *Objekt);
		void zvys_indexy(TObjekt *Objekt);
		void ortogonalizovat();//ortogonalizuje schéma
		long vymaz_seznam_OBJEKTY();

//metody pro ELEMENTY
		void hlavicka_elementy(TObjekt *Objekt);//danému objektu vytvoří hlavičku elementů
		TElement *vloz_element(TObjekt *Objekt,unsigned int eID, double X, double Y);//vloží element do spojového seznamu elementů daného technologického objektu a zároveň na něj vrátí ukazatel
		int vrat_eID_prvniho_pouziteho_robota(TObjekt *Objekt);//vratí eID prvního použitého robota, slouží na filtrování, jaké roboty v knihovně robotů zakazazovat, pokud není nic nalezeno vrátí -1
		long vymaz_elementy(TObjekt *Objekt);//vymaže všechny elementy daného objektu včetně hlavičky a vrátí počet smazaných elementů (počítáno bez hlavičky)
//metody pro POHONY
		void hlavicka_POHONY();
		void vloz_pohon(TPohon *pohon);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
		void vloz_pohon(UnicodeString name,double rychlost_od,double rychlost_do,double aRD,double R,double Rz,double Rx);//vloží jeden pohon na konec seznamu, přiřadí automaticky poslední N (id).
		TPohon *vrat_pohon(unsigned long n);//vrátí ukazatel na pohon dle n pohonu
		bool pohon_je_pouzivan(unsigned long n);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv
		TObjekt *pohon_je_pouzivan(unsigned long n,TObjekt *mimo_objekt,short rezim=-1);//dle n pohonu ověří zda je pohon používán nějakým objektem či nikoliv, ten vrátí formou ukazatale na první nalezený používáný, druhý vstupní parametr metody TObjekt mimo_objekt je ukazatel na objekt, který se bude při vyhledávání ignorovat, nenajde-li vrací NULL, třetí parametr, pokud je náchán na implicitní -1 řeší se pro všechny režim, pokud je v rozmezí 0 až 2 řeší se pro konkrétní režim
unsigned long vrat_pocet_nepouzivanych_pohonu();//vrátí počet nepoužívaných pohonů
		double minRD(TPohon *pohon);//vrátí nejnižší možnou rychlost ze všech objektů, které jsou přiřazené k danému pohonu (využívá se pro S&G a PP, u KK musí být RD v souladu s TT)//pokud vrátí 0, znamená, že pohon není využíván
AnsiString vypis_objekty_nestihajici_prejezd(TPohon *pohon,double testRD,short rezim=-1);//vypíše objekty přiřazené k danému pohonu nestíhající přejezd při navrhovaném testRD, možno nastavit režim, pro S&G + PP hodnota režim 20
AnsiString kontrola_rychlosti_prejezdu(TObjekt *O,short rezim,double CT=0,double MT=0,double WT=0,double aRD=0,double DD=0,short aRDunit=-1,unsigned short precision=3,AnsiString mark="..",bool add_decimal=false,AnsiString separator_aRD=" o ");//zkontroluje objekt zda daná rychlost pohonu odpovídá požadované rychlosti pohonu, pokud ne vrátí popis včetně hodnoty, lze poslat externí testovací parametry nebo nechat ověřit dle uložených ve spojáku objekty
		TPohon *najdi_pohon_dle_RD(double RD);//ověří zda je stejná rychlost pohonu na lince používána, pokud není vratí NULL, jinak ukazatel na daný pohon
		void zrusit_prirazeni_pohunu_k_objektum(unsigned long n);//všem objektům s n pohonem zruší přiřazení k tomuto pohonu a nahradí hodnotu ukazatele na přiřazený pohon za NULL
		void generuj_POHONY();//vygeneruje ve statusu NÁVRH seznam doprvníků dle použitého CT objektu a zároveň tomuto objektu tento pohon přiřadí, obsahuje ošetření proti duplicitě
		AnsiString navrhni_POHONY(AnsiString separator="</br>",short m_min=1);//navrhne pohony zobrazené v parametrech linky, vrátí řetězec oddělený seperátorem, pouze jako seznam unikátních použitých rychlostí, lze nastavit jednotky zobrazení rychlosti pohonu, implicintě m/min
		long vymaz_seznam_POHONY();//smaže jednotlivé prvky seznamu, včetně hlavičky, pokud následuje další práce se seznamem, je nutné založit nejdříve hlavičku pomocí hlavicka_pohony()
//		double delka_dopravniku(Cvektory::TObjekt *ukaz);
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

//metody pro RETEZY
	void hlavicka_RETEZY();//vytvoří novou hlavičku pro řetězy
	void vloz_retez(AnsiString name, double roztec);//uloží retez a jeho parametry do spojového seznamu
	double vrat_roztec_retezu_z_item(AnsiString item,AnsiString separator=",");//z položky (předpoklad vybrané) v comboboxů řetězů vrátí pouze hodnotu rozteče
	AnsiString vypis_retezy_s_pouzitelnou_rozteci(double Rz,AnsiString separator=",",AnsiString total_separator=";",bool mm=false);//vypíše všechny použitelné řetezy použitelné pro zadané rozmezí dle užité rozteče, separátor odděluje název řetězu od rozteče, totál separátor jednotlivé řetězy, pokud je Rz zadané nulové vrátí hodnotu nula, //vypíše všechny použitelné řetezy použitelné pro zadané rozmezí dle užité rozteče, separátor odděluje název řetězu od rozteče, totál separátor jednotlivé řetězy, pokud je Rz zadané nulové vrátí hodnotu nula, pokud chci vypsat všechny načtené řetězy ze souboru retezy.csv použiji parametr Rz=-1, pokud není požadován výpis názvu řetězu použiji prázdné uvozovky
	long vymaz_seznam_RETEZY();//smaze RETEZY z pameti

//metody pro PALCE
//		void hlavicka_palce();
//		void vloz_palec();//přidá nový vozík do seznamu PALCE
//		long vymaz_seznam_palcu();

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

//		//technické, statistické a ekonomické ukazatele
//		void get_LT_a_max_min_TT();
//		double sum_WT();
//		double LT;
//		double MAX_TT;
//		double MIN_TT;
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
				//UnicodeString short_name; nelze
				//UnicodeString name; nelze
				unsigned int text_length;
				double X, Y;
				unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní
				double CT;//kvůli návrháři
				double RD;//kvůli návrháři
				double kapacita;
				double kapacita_dop;
				double pozice;//počet vozíků v kabině
				double rotace;//rotace jigu v objektu
				double mezera;//mezera mezi vozíky (kritická mezera)
				double mezera_jig;//mezera mezi jigy
				double mezera_podvozek;//mezera mezi podvozky
				unsigned int pohon;//"id" resp. n přidruženého - roletkou vybraného pohonu
				double delka_dopravniku;//delka dopravníku v rámci objektu
				unsigned short cekat_na_palce;//0-ne,1-ano,2-automaticky
				unsigned short  stopka;//zda následuje na konci objektu stopka//0-ne,1-ano,2-automaticky
				double odchylka;//povolená odchylka z CT (užito u PP)
				short CT_zamek;
				short RD_zamek;
				short DD_zamek;
				short K_zamek;
				//UnicodeString poznamka; nelze
				unsigned long poznamka_length;
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

		unsigned int vrat_kapacitu_objektu(TObjekt *O);//stačí v sekci private, protože ukládám přímo přímo do atributů objektu pomocí uloz_doporucene_kapacity_objetku();
//
//	protected:

};
//---------------------------------------------------------------------------
#endif

