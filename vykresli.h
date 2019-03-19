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
//	void SG(Cvektory::TVozik *ukaz);//zaji�tuje z�kladn� funkcionalitu technologick�ho objektu v re�imu S&G p�i vykreslov�n� simulaci
//	void STOPKA(Cvektory::TVozik *ukaz);//zaji�tuje z�kladn� funkcionalitu technologick�ho objektu STOPKA p�i vykreslov�n� simulaci
//	bool KOLIZE(Cvektory::TVozik *V1,Cvektory::TVozik *V2);//vrac� logickou hodnotu zda do�lo �i nedo�lo ke kolizi s jin�m voz�kem
	void vykresli_proces(TCanvas *canv, AnsiString shortname, TColor color,short typ, long X1, long X2,long Y,bool legenda=false);//vykresl� jeden d�l�� �asov� proces (obdeln��ek procesu objektu) pro jeden voz�k, vyta�eno pouze kv�li p�ehlednosti
	void vykresli_legendu_casovych_os(TCanvas *canv);//vykresl� a napozicuje legendu pro jednotliv� procesy na �asov�ch os�ch, pokud je skryt� pozicuje alespo� samotn� tla��tko podle zobrazen�ch �i skryt�ch graf�
	void vypis_mezivozikovy_takt(TCanvas *canv,Cvektory::TVozik *vozik, double X,long Y,bool index=false);//pouze pro zp�ehledn�n� zapisu, textov� v�pis a k�ta mezivoz�kov�ho taktu
	void vytizenost_procesu(TCanvas *canv, Cvektory::TProces *P,double X,int Y);
	void vykresli_Xosy(TCanvas *canv);//vykresl� statick� svislice na �asov� osy
	int CorEx(Cvektory::TObjekt *O);//vr�t� sou�adnice dle typu bu� st�edov� nebo excentrick� v podob� lev�ho horn�ho rohu objektu
	int CorEy(Cvektory::TObjekt *O);//vr�t� sou�adnice dle typu bu� st�edov� nebo excentrick� v podob� lev�ho horn�ho rohu objektu

	short oY;//ofset na ose Y, 5 pouze grafick� korekce
	Cvektory::TProces *Pom_proces;//pomocn� ukazatel na proces, vyu��v� se v p�i na��t�n� p�vodn� vytvo�en�ch n�hodn�ch hodnot �ek�n� na palec

	public:
	Cvykresli();//konstruktor
	Cvektory v;
	Cmy m;

	//--prom�nn� k ��elu filtrace
	struct TTP
	{
		double K;//Krok po kolika minutach se bude zobrazovat
		double OD;//od kter� min se proces za�ne vypisovat
		double DO;//do ktere se bude vypisovat
		double KZ;//kosntatn� konec zakazky v min
		unsigned int Nod;//rozmez� jak� se vyp�e vozik,
		unsigned int Ndo;//rozmez� jak� se vyp�e vozik, pokud bude 0 vypisuj� se v�echny
		bool A;//animace true nebo false
	};TTP TP;//nastaven� modu technologick� procesy

	void vykresli_vektory(TCanvas *canv);
	void vykresli_objekty(TCanvas *canv);
	void vykresli_rectangle(TCanvas *canv, Cvektory::TObjekt *ukaz);
	void vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O);
	void prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N);
	void vykresli_grid(TCanvas *canv,int size_grid=10);
	void vykresli_meridlo(TCanvas *canv,int X,int Y,bool kalibracni_sipka=false);//v p��pad� m��en� vzd�lenosti vykresl� spojnici a pop�. vypisuje hodnotu vzd�lenosti
	//	void vykresli_graf_rezervy(TCanvas *canv);//m�d graf rezerv
	void vykresli_casove_osy(TCanvas *canv);//MARO metoda, celkov� vykreslen� m�du �asov� osy
	void vykresli_vytizenost_objektu(TCanvas *canv);
	double proces(TCanvas *canv, unsigned int n, double X_predchozi, double X, int Y, Cvektory::TCesta *C/*segment cesty*/, Cvektory::TVozik *vozik);
	void vykresli_svislici_na_casove_osy(TCanvas *canv,int X,int Y);//vykresl� pohyblivou svislici yna �asov� osy dle um�st�n� kurzoru my�i
	void zobrazit_label_zamerovac(int X,int Y);//vypi�e labal zam��ova�
	void vykresli_technologicke_procesy(TCanvas *canv);//ROMA metoda, vykresl� graf technologick�ch proces� v��i jednotliv�m t-objekt�m v �ase
	void vykresli_layout(TCanvas *canv);//zaji��uje vykreslen� layout
	unsigned int  vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset=0,bool animace=false);///zajist� vykreslen� n�hledu objektu, XY -um�st�n� L za��tek (st�ed dopravn�ku) objektu v metrech, Poffset - pozi�n� poloha, v�choz� poloha prvn�ho voz�ku/pozice v objektu (a v��i tomuto objektu),m��e slou�it na animaci �i n�vaznost v p��pad� layoutu, za zm�n�n� stoj� lok�ln� prom�nn� t�to metody KR, co� je kalibrace �et�zu v��i podvozku nap�. 0 - st�ed, -DP/2 - za��tek, DP/2 - konec, �i libovoln� v m od za��tku podvozku
	unsigned int vykresli_pozice(TCanvas *canv, int i, TPointD OD, TPointD DO,double delka, double delkaV,double sirkaV,double delkaP,double mezera,double akt_pozice=0);//zaji��uje vykreslen� pozic v layoutu + p��prava konstrukce kdy� nebudu cht�t vykreslovat objekt vodorovn�, pouze bude nutn� zajistit je�t� rotaci pozic a podvozk�
	void vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dP,double dJ,double sJ,double rotaceP=0,double rotaceJ=0,TColor clChassis=(TColor) RGB(50,50,50), TColor clJig=clPurple);//vykreslen� jednoho komplexn�ho voz�ku (podvozek v�etn� jigu)
	void nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv);
//	void vykresli_simulaci(TCanvas *canv);//zaji��uje vykreslen� simulace
//	void vykresli_linku(TCanvas *canv);//zaji��uje vykreslen� osy linky
//	void umisti_vozik(TCanvas *canv,Cvektory::TVozik *ukaz);//zaji��uje um�st�n� voz�ku na lince
//	void vykresli_vozik(TCanvas *canv,Cvektory::TVozik *ukaz,long X,long Y,bool NEW);//zaji��uje vykreslen� voz�ku p�i simulaci, pokud je NEW==1, tak se vykresl� nov�, p�i 0 se sma�e star�
//	void priprav_palce();//p�id� nov� palec do seznamu PALCE s um�st�n�m p��mo na linku dle stanoven� rozte�e
//	void umisti_palec(TCanvas *canv,Cvektory::TPalec *ukaz);//zaji��uje aktu�ln� um�st�n� voz�ku na lince v��i animaci
	void vykresli_palec(TCanvas *canv,double X,double Y,bool NEW,bool ACTIVE);//zaji��uje samotn� vykreslen� palce, parametr NEW rozli�uje nov� palec a palace star� ji� ke smaz�n� (to slou�� pro simulaci), posledn� parametr zna��, zda palec ozna�it jako aktivn�
	void vykresli_element(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=0,short typ=0,double rotace=0,short stav=1);//celkov� vykreslovac� metoda, vykresl� bu� stopku, robota nebo oto�
	void vykresli_robota(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=1,short typ=0,double rotace=0,short stav=1,double LO=1.5,double aP=0,float TS=0);
	void vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ=0, double rotace=0, short stav=1);
	void vykresli_otoc(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=5,short typ=0,double rotace=0,short stav=1);
	void vykresli_lakovaci_okno(TCanvas *canv,long X,long Y,double LO,double delka_ramena,double rotace=0);
	TPoint polygonDleOsy(TCanvas *canv,long X,long Y,float delka, float sirka1, float sirka2, double sklon, double rotace);//vykresl� polygon dle osy, um� i k�nick� tvar, vrat� sou�adnice konce osy polygonu
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
	void rotace_textu(TCanvas *canv, long rotace);//�hel rotace je desetin�ch stupn�
	void set_pen(TCanvas *canv, TColor color, int width, int style=PS_ENDCAP_SQUARE);//vr�t� HANDLE na nastaven� pera,//pop�.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matou�ek III str. 179 �i http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
	TColor set_color(TCanvas *canv, Cvektory::TObjekt *O);
	void drawRectText(TCanvas *canv,TRect Rect, UnicodeString Text);
	void editacni_okno(TCanvas *canv, unsigned int X1, unsigned int Y1,unsigned  int X2,unsigned  int Y2, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void sipka(TCanvas *canv,int X, int Y,float azimut,bool bez_vyplne=false, float=3,COLORREF color=clBlack,COLORREF color_brush=clGray,TPenMode PenMode=pmCopy,TPenStyle PenStyle=psSolid);//zajist� vykreslen� �ipky - orientace spojovac� linie
	void odznac_oznac_objekt(TCanvas *canv, Cvektory::TObjekt *p,int posunX=0, int posunY=0, COLORREF color=clBlack);
	void odznac_oznac_objekt_novy(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);
	void odznac_oznac_objekt_novy_posledni(TCanvas *canv, int X, int Y);
void odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p, bool posun=false);//p�� umistiv�n� �i posouv�n� vyh�bky
void odznac_oznac_vetev(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);//vykresl� �i odzna�� potenci�ln� um�st�n� v�tve, dal�� f�ze po um�st�n� v�hybky
	bool lezi_v_pasmu(TCanvas *c,long X,long Y,Cvektory::TObjekt *p, bool odecti_region=true);
	bool lezi_v_pasmu_poslednim(TCanvas *c,long X,long Y,bool odecti_region=true);
double trend(Cvektory::TObjekt *Objekt);//vrat� trend sch�matu pro objekt z parametru,pro jeden prvek automaticky bude trend sch�matu 0�, pokud se jedn� o prvn� prvek, tak v�jimka, �e�� trend podle prvku n�sleduj�c�ho, pro dal�� se hled� na trend podle azimutu k p�edchoz�mu prvku
	void sound();
	void meritko(TCanvas *canv,long X=-1,long Y=-1);
	void vykresli_packu(TCanvas *canv, int X1,int Y1,int X2,int Y2,TColor color=/*clBtnShadow*/(TColor)RGB(170,170,170),short Width=1,short OffsetX=-5,short OffsetY=0);//vykresl� packu jako spojnici mezi komponentami
	//void vykresli_packy_PL(TCanvas *canv,short typ,short zamek_aRD,short zamek_R,short zamek_Rz,short zamek_Rx);
	void vykresli_packy_PL(TCanvas *canv,TscGPButton *zamek_aRD,TscGPButton *zamek_R,TscGPButton *zamek_Rz,TscGPButton *zamek_Rx);

	short O_width,O_height,V_width;//logick� parametry, nezoomovat
	float Robot_sirka_zakladny;
	float Robot_delka_zakladny;
	double DoSkRB;//delka od st�edu (X,Y bodu) robota k referen�n�mu bodu robota (tj. k trysce) v metrech
	unsigned long cas;//ms
	double PX2MIN;//m���tko PX na MIN
	short KrokY;//rozte� na �asov�ch os�ch
	int WidthCanvasCasoveOsy,HeightCanvasCasoveOsy;
	TPointD PosunT; //v�choz� posunut� obrazu Posunu
	bool mod_vytizenost_objektu;
	short NOLIEX;//NO - 0, LINEAR - 1, EXPO - 2
	bool JIZPOCITANO;//�e�en� aby se zbyte�n� a opakovan� neukl�dalo do PROCES� pokud to nen� treba
	bool RANDOM;
	bool grafickeDilema;//provizorn� prom�nn� na p�ep�n�n� stavu, zda se p�i p�id�v�n� objektu a p�esouv�n� objektu bude zmen�ovat p�smo nebo nep�ekreslovat objekt
	short legenda_polozky[8];//zobrazen� jednotliv�ch polo�ek, 0 index je v�ak po�et zobrazen�ch (ulo�en�ch) polo�ek v poli, -1 stav je u dan�ho atributu norm�l
	short Xofset;//zajist� spr�vn� po��tek prvn�ho objektu dle ���ky nejdel�� vypisovan� minuty v ROMA
	short precision;//po�et desetinn�ch m�st ��sel na �asov� ose

	protected:



};
#endif
