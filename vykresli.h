//---------------------------------------------------------------------------
#ifndef vykresliH
#define vykresliH
#include <vcl.h>
#include "vektory.h"
#include "my.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class Cvykresli
{
	private:

	bool lezi_v_pasmu(TCanvas *c,long X,long Y,long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,bool odecti_region=true);
//	void SG(Cvektory::TVozik *ukaz);//zajištuje základní funkcionalitu technologického objektu v reimu S&G pøi vykreslování simulaci
//	void STOPKA(Cvektory::TVozik *ukaz);//zajištuje základní funkcionalitu technologického objektu STOPKA pøi vykreslování simulaci
//	bool KOLIZE(Cvektory::TVozik *V1,Cvektory::TVozik *V2);//vrací logickou hodnotu zda došlo èi nedošlo ke kolizi s jinım vozíkem
	void vykresli_proces(TCanvas *canv, AnsiString shortname, TColor color,short typ, long X1, long X2,long Y,bool legenda=false);//vykreslí jeden dílèí èasovı proces (obdelníèek procesu objektu) pro jeden vozík, vytaeno pouze kvùli pøehlednosti
	void vykresli_legendu_casovych_os(TCanvas *canv);//vykreslí a napozicuje legendu pro jednotlivé procesy na èasovıch osách, pokud je skrytá pozicuje alespoò samotné tlaèítko podle zobrazenıch èi skrytıch grafù
	void vypis_mezivozikovy_takt(TCanvas *canv,Cvektory::TVozik *vozik, double X,long Y,bool index=false);//pouze pro zpøehlednìní zapisu, textovı vıpis a kóta mezivozíkového taktu
	void vytizenost_procesu(TCanvas *canv, Cvektory::TProces *P,double X,int Y);
	void vykresli_Xosy(TCanvas *canv);//vykreslí statické svislice na èasové osy
	int CorEx(Cvektory::TObjekt *O);//vrátí souøadnice dle typu buï støedové nebo excentrické v podobì levého horního rohu objektu
	int CorEy(Cvektory::TObjekt *O);//vrátí souøadnice dle typu buï støedové nebo excentrické v podobì levého horního rohu objektu

	short oY;//ofset na ose Y, 5 pouze grafická korekce
	Cvektory::TProces *Pom_proces;//pomocnı ukazatel na proces, vyuívá se v pøi naèítání pùvodnì vytvoøenıch náhodnıch hodnot èekání na palec

	public:
	Cvykresli();//konstruktor
	Cvektory v;
	Cmy m;

	//--promìnné k úèelu filtrace
	struct TTP
	{
		double K;//Krok po kolika minutach se bude zobrazovat
		double OD;//od které min se proces zaène vypisovat
		double DO;//do ktere se bude vypisovat
		double KZ;//kosntatní konec zakazky v min
		unsigned int Nod;//rozmezí jakı se vypíše vozik,
		unsigned int Ndo;//rozmezí jakı se vypíše vozik, pokud bude 0 vypisují se všechny
		bool A;//animace true nebo false
	};TTP TP;//nastavení modu technologické procesy

	void vykresli_vektory(TCanvas *canv);
	void vykresli_objekty(TCanvas *canv);
	void vykresli_rectangle(TCanvas *canv, Cvektory::TObjekt *ukaz);
	void vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O);
	void prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N);
	void vykresli_grid(TCanvas *canv,int size_grid=10);
	void vykresli_meridlo(TCanvas *canv,int X,int Y,bool kalibracni_sipka=false);//v pøípadì mìøení vzdálenosti vykreslí spojnici a popø. vypisuje hodnotu vzdálenosti
	//	void vykresli_graf_rezervy(TCanvas *canv);//mód graf rezerv
	void vykresli_casove_osy(TCanvas *canv);//MARO metoda, celkové vykreslení módu èasové osy
	void vykresli_vytizenost_objektu(TCanvas *canv);
	double proces(TCanvas *canv, unsigned int n, double X_predchozi, double X, int Y, Cvektory::TCesta *C/*segment cesty*/, Cvektory::TVozik *vozik);
	void vykresli_svislici_na_casove_osy(TCanvas *canv,int X,int Y);//vykreslí pohyblivou svislici yna èasové osy dle umístìní kurzoru myši
	void zobrazit_label_zamerovac(int X,int Y);//vypiše labal zamìøovaè
	void vykresli_technologicke_procesy(TCanvas *canv);//ROMA metoda, vykreslí graf technologickıch procesù vùèi jednotlivım t-objektùm v èase
	void vykresli_layout(TCanvas *canv);//zajišuje vykreslení layout
	unsigned int  vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset=0,bool animace=false);///zajistí vykreslení náhledu objektu, XY -umístìní L zaèátek (støed dopravníku) objektu v metrech, Poffset - pozièní poloha, vıchozí poloha prvního vozíku/pozice v objektu (a vùèi tomuto objektu),mùe slouit na animaci èi návaznost v pøípadì layoutu, za zmínìní stojí lokální promìnná této metody KR, co je kalibrace øetìzu vùèi podvozku napø. 0 - støed, -DP/2 - zaèátek, DP/2 - konec, èi libovolnı v m od zaèátku podvozku
	unsigned int vykresli_pozice(TCanvas *canv, int i, TPointD OD, TPointD DO,double delka, double delkaV,double sirkaV,double delkaP,double mezera,double akt_pozice=0);//zajišuje vykreslení pozic v layoutu + pøíprava konstrukce kdy nebudu chtít vykreslovat objekt vodorovnì, pouze bude nutné zajistit ještì rotaci pozic a podvozkù
	void vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dP,double dJ,double sJ,double rotaceP=0,double rotaceJ=0,TColor clChassis=(TColor) RGB(50,50,50), TColor clJig=clPurple);//vykreslení jednoho komplexního vozíku (podvozek vèetnì jigu)
	void nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv);
//	void vykresli_simulaci(TCanvas *canv);//zajišuje vykreslení simulace
//	void vykresli_linku(TCanvas *canv);//zajišuje vykreslení osy linky
//	void umisti_vozik(TCanvas *canv,Cvektory::TVozik *ukaz);//zajišuje umístìní vozíku na lince
//	void vykresli_vozik(TCanvas *canv,Cvektory::TVozik *ukaz,long X,long Y,bool NEW);//zajišuje vykreslení vozíku pøi simulaci, pokud je NEW==1, tak se vykreslí novı, pøi 0 se smae starı
//	void priprav_palce();//pøidá novı palec do seznamu PALCE s umístìním pøímo na linku dle stanovené rozteèe
//	void umisti_palec(TCanvas *canv,Cvektory::TPalec *ukaz);//zajišuje aktuální umístìní vozíku na lince vùèi animaci
	void vykresli_palec(TCanvas *canv,double X,double Y,bool NEW,bool ACTIVE);//zajišuje samotné vykreslení palce, parametr NEW rozlišuje novı palec a palace starı ji ke smazání (to slouí pro simulaci), poslední parametr znaèí, zda palec oznaèit jako aktivní
	void vykresli_element(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=0,short typ=0,double rotace=0,short stav=1);//celková vykreslovací metoda, vykreslí buï stopku, robota nebo otoè
	void vykresli_robota(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=1,short typ=0,double rotace=0,short stav=1,double LO=1.5,double aP=0,float TS=0);
	void vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ=0, double rotace=0, short stav=1);
	void vykresli_otoc(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=5,short typ=0,double rotace=0,short stav=1);
	void vykresli_lakovaci_okno(TCanvas *canv,long X,long Y,double LO,double delka_ramena,double rotace=0);
	TPoint polygonDleOsy(TCanvas *canv,long X,long Y,float delka, float sirka1, float sirka2, double sklon, double rotace);//vykreslí polygon dle osy, umí i kónickı tvar, vratí souøadnice konce osy polygonu
	void linie(TCanvas *canv,long X1,long Y1,long X2,long Y2,TColor Width,TColor Color=clBlack,TPenStyle PenStyle=psSolid,TPenMode PenMode=pmCopy);
	void line(TCanvas *canv,long X1,long Y1,long X2,long Y2);
	void vykresli_ikonu_linie(TCanvas *canv,int X,int Y,AnsiString Popisek="linie",short stav=0);
	void vykresli_ikonu_oblouku(TCanvas *canv,int X,int Y,AnsiString Popisek="oblouky",short stav=0);
	void vykresli_ikonu_textu(TCanvas *canv,int X,int Y,AnsiString Popisek="text",short stav=0);
	void vykresli_ikonu_sipky(TCanvas *canv,int X,int Y,AnsiString Popisek="spojnice",short stav=0);
	void vykresli_mGridy();
	void vykresli_kotu(TCanvas *canv,Cvektory::TElement *Element_od,Cvektory::TElement *Element_do);
	void vykresli_kotu(TCanvas *canv,double X1,double Y1,double X2,double Y2,AnsiString Text, double Offset=0,unsigned short width=1,TColor color=clGray);
	void vykresli_kotu(TCanvas *canv,long X1,long Y1,long X2,long Y2,AnsiString Text, int Offset=0,unsigned short width=1,TColor color=clGray);
	void rotace_textu(TCanvas *canv, long rotace);//úhel rotace je desetinách stupnì
	void set_pen(TCanvas *canv, TColor color, int width, int style=PS_ENDCAP_SQUARE);//vrátí HANDLE na nastavení pera,//popø.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matoušek III str. 179 èi http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
	TColor set_color(TCanvas *canv, Cvektory::TObjekt *O);
	void drawRectText(TCanvas *canv,TRect Rect, UnicodeString Text);
	void editacni_okno(TCanvas *canv, unsigned int X1, unsigned int Y1,unsigned  int X2,unsigned  int Y2, unsigned short int size=1, COLORREF color=clBlack);//nakreslí editacni_okno
	void editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size=1, COLORREF color=clBlack);//nakreslí editacni_okno
	void sipka(TCanvas *canv,int X, int Y,float azimut,bool bez_vyplne=false, float=3,COLORREF color=clBlack,COLORREF color_brush=clGray,TPenMode PenMode=pmCopy,TPenStyle PenStyle=psSolid);//zajistí vykreslení šipky - orientace spojovací linie
	void odznac_oznac_objekt(TCanvas *canv, Cvektory::TObjekt *p,int posunX=0, int posunY=0, COLORREF color=clBlack);
	void odznac_oznac_objekt_novy(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);
	void odznac_oznac_objekt_novy_posledni(TCanvas *canv, int X, int Y);
void odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p, bool posun=false);//pøí umistivání èi posouvání vyhıbky
void odznac_oznac_vetev(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);//vykreslí èi odznaèí potenciální umístìní vìtve, další fáze po umístìní vıhybky
	bool lezi_v_pasmu(TCanvas *c,long X,long Y,Cvektory::TObjekt *p, bool odecti_region=true);
	bool lezi_v_pasmu_poslednim(TCanvas *c,long X,long Y,bool odecti_region=true);
double trend(Cvektory::TObjekt *Objekt);//vratí trend schématu pro objekt z parametru,pro jeden prvek automaticky bude trend schématu 0°, pokud se jedná o první prvek, tak vıjimka, øeší trend podle prvku následujícího, pro další se hledí na trend podle azimutu k pøedchozímu prvku
	void sound();
	void meritko(TCanvas *canv,long X=-1,long Y=-1);
	void vykresli_packu(TCanvas *canv, int X1,int Y1,int X2,int Y2,TColor color=/*clBtnShadow*/(TColor)RGB(170,170,170),short Width=1,short OffsetX=-5,short OffsetY=0);//vykreslí packu jako spojnici mezi komponentami
	//void vykresli_packy_PL(TCanvas *canv,short typ,short zamek_aRD,short zamek_R,short zamek_Rz,short zamek_Rx);
	void vykresli_packy_PL(TCanvas *canv,TscGPButton *zamek_aRD,TscGPButton *zamek_R,TscGPButton *zamek_Rz,TscGPButton *zamek_Rx);

	short O_width,O_height,V_width;//logické parametry, nezoomovat
	float Robot_sirka_zakladny;
	float Robot_delka_zakladny;
	double DoSkRB;//delka od støedu (X,Y bodu) robota k referenènímu bodu robota (tj. k trysce) v metrech
	unsigned long cas;//ms
	double PX2MIN;//mìøítko PX na MIN
	short KrokY;//rozteè na èasovıch osách
	int WidthCanvasCasoveOsy,HeightCanvasCasoveOsy;
	TPointD PosunT; //vıchozí posunutí obrazu Posunu
	bool mod_vytizenost_objektu;
	short NOLIEX;//NO - 0, LINEAR - 1, EXPO - 2
	bool JIZPOCITANO;//øešení aby se zbyteènì a opakovanì neukládalo do PROCESù pokud to není treba
	bool RANDOM;
	bool grafickeDilema;//provizorní promìnná na pøepínání stavu, zda se pøi pøidávání objektu a pøesouvání objektu bude zmenšovat písmo nebo nepøekreslovat objekt
	short legenda_polozky[8];//zobrazení jednotlivıch poloek, 0 index je však poèet zobrazenıch (uloenıch) poloek v poli, -1 stav je u daného atributu normál
	short Xofset;//zajistí správnı poèátek prvního objektu dle šíøky nejdelší vypisované minuty v ROMA
	short precision;//poèet desetinnıch míst èísel na èasové ose

	protected:



};
#endif
