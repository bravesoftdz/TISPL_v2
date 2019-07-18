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

	Cvektory::TProces *Pom_proces;//pomocn� ukazatel na proces, vyu��v� se v p�i na��t�n� p�vodn� vytvo�en�ch n�hodn�ch hodnot �ek�n� na palec
	short oY;//ofset na ose Y, 5 pouze grafick� korekce
	float sizeP;//velikost text� popisk� element� v knihovn�
	TColor clStenaKabiny,clStenaHaly;

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

	void vykresli_halu(TCanvas *canv,int typ=-2);//stav: -3 kurzor, -2 normal (implicitn�), -1-highlight bez editace, 0-editace zv�razn� v�echny body, 1-a� po�et bod� zv�razn�n� dan�ho bodu,po�et bod�+1 zv�razn�n� dan� hrany v�etn� soused�c�c�h �chop� (nap�. pro polygono o 6 bodech) bude hodnota stavu 7 zv�razn�n� prvn� hrany (od bodu 1 do bodu 2)
	void vykresli_objekty(TCanvas *canv);
	void vykresli_rectangle(TCanvas *canv, Cvektory::TObjekt *ukaz);
	void vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O);
	void prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N);
	void vykresli_vektory(TCanvas *canv);
	void vykresli_kabinu(TCanvas *canv);//zaji��uje kompletn� vykreslen� s vol�n�m n�sleduj�c� metody
	void vykresli_kabinu(TCanvas *canv,Cvektory::TObjekt *O,int stav=-2,bool zobrazit_koty=true);//zaji��uje vykreslen� pouze obrysu dle typu objektu
	void vykresli_pow_sprchu(TCanvas *canv,long X1,long X2,long Y1,long Y2,unsigned int velikost_komory_px,TColor color,short sirka,short pmpp,short typ=0);//symbolika tekouc� kapaliny u POW
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
	unsigned int  vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset=0,bool animace=false);////metoda pou��van� ve star�m n�hledu objektu, mo�no odstranit///zajist� vykreslen� n�hledu objektu, XY -um�st�n� L za��tek (st�ed dopravn�ku) objektu v metrech, Poffset - pozi�n� poloha, v�choz� poloha prvn�ho voz�ku/pozice v objektu (a v��i tomuto objektu),m��e slou�it na animaci �i n�vaznost v p��pad� layoutu, za zm�n�n� stoj� lok�ln� prom�nn� t�to metody KR, co� je kalibrace �et�zu v��i podvozku nap�. 0 - st�ed, -DP/2 - za��tek, DP/2 - konec, �i libovoln� v m od za��tku podvozku
	unsigned int vykresli_pozice(TCanvas *canv, int i, TPointD OD, TPointD DO,double delka, double delkaV,double sirkaV,double delkaP,double mezera,double akt_pozice=0);//zaji��uje vykreslen� pozic v layoutu + p��prava konstrukce kdy� nebudu cht�t vykreslovat objekt vodorovn�, pouze bude nutn� zajistit je�t� rotaci pozic a podvozk�
	void vykresli_retez(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset=0,bool animace=false);///zajist� vykreslen� �et�zz, XY -um�st�n� L za��tek (st�ed dopravn�ku) objektu v metrech, Poffset - pozi�n� poloha, v�choz� poloha prvn�ho voz�ku/pozice v objektu (a v��i tomuto objektu),m��e slou�it na animaci �i n�vaznost v p��pad� layoutu, za zm�n�n� stoj� lok�ln� prom�nn� t�to metody KR, co� je kalibrace �et�zu v��i podvozku nap�. 0 - st�ed, -DP/2 - za��tek, DP/2 - konec, �i libovoln� v m od za��tku podvozku
	void vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dP,double dJ,double sJ,double rotaceP=0,double rotaceJ=0,TColor clChassis=(TColor) RGB(50,50,50), TColor clJig=clPurple);//vykreslen� jednoho komplexn�ho voz�ku (podvozek v�etn� jigu)
	void nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv,unsigned short typ);//short typ = zda se jedn� o zadav�n� name==1 nebo short_name==2 nebo nic==0
//	void vykresli_simulaci(TCanvas *canv);//zaji��uje vykreslen� simulace
//	void vykresli_linku(TCanvas *canv);//zaji��uje vykreslen� osy linky
//	void umisti_vozik(TCanvas *canv,Cvektory::TVozik *ukaz);//zaji��uje um�st�n� voz�ku na lince
//	void vykresli_vozik(TCanvas *canv,Cvektory::TVozik *ukaz,long X,long Y,bool NEW);//zaji��uje vykreslen� voz�ku p�i simulaci, pokud je NEW==1, tak se vykresl� nov�, p�i 0 se sma�e star�
//	void priprav_palce();//p�id� nov� palec do seznamu PALCE s um�st�n�m p��mo na linku dle stanoven� rozte�e
//	void umisti_palec(TCanvas *canv,Cvektory::TPalec *ukaz);//zaji��uje aktu�ln� um�st�n� voz�ku na lince v��i animaci
	void vykresli_palec(TCanvas *canv,double X,double Y,bool NEW,bool ACTIVE);//zaji��uje samotn� vykreslen� palce, parametr NEW rozli�uje nov� palec a palace star� ji� ke smaz�n� (to slou�� pro simulaci), posledn� parametr zna��, zda palec ozna�it jako aktivn�
	void vykresli_element(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=0,short typ=0,double rotace=0,short stav=1,double LO1=1.5,double OTOC_delka=0,double LO2=0,double LO_pozice=0);//celkov� vykreslovac� metoda, vykresl� bu� stopku, robota nebo oto�
	void vykresli_robota(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=1,short typ=0,double rotace=0,short stav=1,double LO1=1.5,double OTOC_delka=0,double LO2=0,double aP=0,float TS=0,double LO_pozice=0);
	void vykresli_cloveka(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2);//vykresli siluetu �lov�k s p��padn� p�idru�en�m elememntem, rotuje po sm�ru hodinov�ch ru�i�ek, pro animaci slou�� okolo hrani�n�ch stup�u 0,90,180,270, v�dy rozsah -45� a� +44�, nap�. 45-134� je maxim�ln� pracovn� rozsah pro �lov�ka rotovan�ho na 90� atd.
	void vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ=0, double rotace=0, short stav=1);
	void vykresli_otoc(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID=5,short typ=0,double rotace=0,short stav=1);
	void vykresli_ion(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,double rotace,short stav,float TS=0);
	void vykresli_lakovaci_okno(TCanvas *canv,long X,long Y,double LO1,double OTOC_delka,double LO2,double delka_ramena,double rotace=0,double LO_pozice=0);
	void vykresli_ikonu_linie(TCanvas *canv,int X,int Y,AnsiString Popisek="linie",short stav=0);
	void vykresli_ikonu_oblouku(TCanvas *canv,int X,int Y,AnsiString Popisek="oblouky",short stav=0);
	void vykresli_ikonu_textu(TCanvas *canv,int X,int Y,AnsiString Popisek="text",short stav=0);
	void vykresli_ikonu_sipky(TCanvas *canv,int X,int Y,AnsiString Popisek="spojnice",short stav=0);
	void vykresli_ikonu_komory(TCanvas *canv,int X,int Y,AnsiString Popisek="komora",short typ=0,short stav=0);
	void linie(TCanvas *canv,long X1,long Y1,long X2,long Y2,int Width,TColor Color=clBlack,TPenStyle PenStyle=psSolid,TPenMode PenMode=pmCopy);
	void line(TCanvas *canv,long X1,long Y1,long X2,long Y2);
	void bezier(TCanvas *canv,TPointD *POLE,long posledni_prvek);
	void bezier(TCanvas *canv,TPointD *POLE,long X,long Y,double oX,double oY,double rotace,long posledni_prvek);
	void bezier(TCanvas *canv,TPoint *POLE_px,long posledni_prvek);
	TPoint polygonDleOsy(TCanvas *canv,long X,long Y,float delka, float sirka1, float sirka2, double sklon, double rotace,TPenMode pmMode=pmCopy,TColor clFillOut=clBlack,TColor lFillIn=clWhite);
	void polygon(TCanvas *canv,Cvektory::TBod *body,TColor barva=clBlack, short sirka=1,int stav=-2,bool zobrazit_koty=true,bool automaticky_spojovat=true);//stav: -3 kurzor, -2 normal (implicitn�), -1-disabled, 0-editace zv�razn� v�echny body, 1-a� po�et bod� zv�razn�n� dan�ho bodu,po�et bod�+1 zv�razn�n� dan� hrany v�etn� soused�c�c�h �chop� (nap�. pro polygono o 6 bodech) bude hodnota stavu 7 zv�razn�n� prvn� hrany (od bodu 1 do bodu 2)
	void uchop(TCanvas *canv,Cvektory::TBod *B,TColor barva);//vykresl� jeden uchop/kole�ko zn�zor�uj�c� bod na polygonu
	void vykresli_mGridy(TCanvas *canv=NULL);
	void nabuffrovat_mGridy(TmGrid *mGrid=NULL);//pomocn� metoda vytv��ej�c� rastrov� obrazy mGrid�, pokud je buffer na NULL, buffruj� se v�echny tabulky, pokud ne, tak pouze ta z parametru
	void vykresli_kotu(TCanvas *canv,Cvektory::TElement *Element_od,Cvektory::TElement *Element_do);
	void vykresli_kotu(TCanvas *canv,double X1,double Y1,double X2,double Y2,Cvektory::TElement * aktElement=NULL,double Offset=0,short highlight=0,float width=0.2,TColor color=clGray,bool ukladat_do_elementu=true,Cvektory::TKomora *komora=NULL);//v metrick�ch jednotk�ch krom� width, zde v px + automaticky dopo��t�v� d�lku a dosazuje aktu�ln� nastaven� jednotky highlight: 0-ne,1-ano,2-ano+vystoupen� k�ty i pozi�n�, aktElement pokud bude NULL, p�edpokl�d� se, �e je to k�ta kabiny, parametr ukladat_do_elementu p�epos�l�n do vno�en� metody
	void vykresli_kotu(TCanvas *canv,long X1,long Y1,long X2,long Y2,AnsiString Text,Cvektory::TElement *aktElement=NULL,int Offset=0,short highlight=0,float width=0.2,TColor color=clGray,bool ukladat_do_elementu=true,Cvektory::TKomora *komora=NULL,Cvektory::TBod *bod=NULL);//v px + dosazuje aktu�ln� nastaven� jednotky, highlight: 0-ne,1-ano,2-ano+vystoupen� k�ty i pozi�n�, aktElement pokud bude NULL, p�edpokl�d� se, �e je to k�ta kabiny, p�id�n parametr ukladat_do_elementu a to z d�vodu k�ty mezi lak. okny, nesm� b�t ukl�d�na do elementu ani do objektu
	void rotace_textu(TCanvas *canv, long rotace);//�hel rotace je desetin�ch stupn�
	void set_pen(TCanvas *canv, TColor color, int width, int style=PS_ENDCAP_SQUARE);//vr�t� HANDLE na nastaven� pera,//pop�.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matou�ek III str. 179 �i http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
	TColor set_color(TCanvas *canv, Cvektory::TObjekt *O);
	void drawRectText(TCanvas *canv,TRect Rect, UnicodeString Text);
	Graphics::TBitmap *srafura();
	void editacni_okno(TCanvas *canv, unsigned int X1, unsigned int Y1,unsigned  int X2,unsigned  int Y2, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void sipka(TCanvas *canv,int X, int Y,float azimut,bool bez_vyplne=false, float size=3, COLORREF color=clBlack,COLORREF color_brush=clGray,TPenMode PenMode=pmCopy,TPenStyle PenStyle=psSolid,bool teziste_stred=true);//zajist� vykreslen� �ipky - orientace spojovac� linie
	void odznac_oznac_objekt(TCanvas *canv, Cvektory::TObjekt *p,int posunX=0, int posunY=0, COLORREF color=clBlack);
	void odznac_oznac_objekt_novy(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);
	void odznac_oznac_objekt_novy_posledni(TCanvas *canv, int X, int Y);
	void odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p, bool posun=false);//p�� umistiv�n� �i posouv�n� vyh�bky
	void odznac_oznac_vetev(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p);//vykresl� �i odzna�� potenci�ln� um�st�n� v�tve, dal�� f�ze po um�st�n� v�hybky
	short lezi_v_pasmu(TCanvas *c,long X,long Y,Cvektory::TObjekt *p, bool odecti_region=true);//slou�� k ur�en� zda se kurzor vyskytuje v p�smu mezi dv�ma objekty,0= nic nenalezeno, 1= nalezen dalsi objekt, 2= nalezen dalsi2 objekt
	bool lezi_v_pasmu_poslednim(TCanvas *c,long X,long Y,bool odecti_region=true);
	double trend(Cvektory::TObjekt *Objekt);//vrat� trend sch�matu pro objekt z parametru,pro jeden prvek automaticky bude trend sch�matu 0�, pokud se jedn� o prvn� prvek, tak v�jimka, �e�� trend podle prvku n�sleduj�c�ho, pro dal�� se hled� na trend podle azimutu k p�edchoz�mu prvku
	void sound();
	void meritko(TCanvas *canv,long X=-1,long Y=-1);
	void vykresli_packu(TCanvas *canv, int X1,int Y1,int X2,int Y2,TColor color=/*clBtnShadow*/(TColor)RGB(170,170,170),short Width=1,short OffsetX=-5,short OffsetY=0);//vykresl� packu jako spojnici mezi komponentami
	//void vykresli_packy_PL(TCanvas *canv,short typ,short zamek_aRD,short zamek_R,short zamek_Rz,short zamek_Rx);
	void vykresli_packy_PL(TCanvas *canv,TscGPButton *zamek_aRD,TscGPButton *zamek_R,TscGPButton *zamek_Rz,TscGPButton *zamek_Rx);
	void vykresli_tip(TCanvas *canv);//zaji��uje vykreslov�n�-vypisov�n� tool tipu
	TPointD Rxy(Cvektory::TElement *Element);//vr�t� referen�n� logick� (v metrech) sou�adnice  robota (tzn. bod v m�st� trysky, tj. bod v m�st� um�st�n� ve sch�matu), p�evede dle aktu�ln� rotace symbolu a uchopovac�ch (skute�n�ch) sou�adnic robota, kontroluje, zda se jedn� skute�n� o Robota

	//glob�ln� public prom�nn�
	short O_width,O_height,V_width;//logick� parametry, nezoomovat
	float Robot_sirka_zakladny;
	float Robot_delka_zakladny;
	double DoSkRB;//delka od st�edu (X,Y bodu) robota k referen�n�mu bodu robota (tj. k trysce) v metrech
	double DkRB;//d�lka k referen�n�mu bodu od uchopovac�ho bodu, respektive odsazen� �lov�ka od linky
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
	TRect aktOblast;//aktu�ln� citeln� oblast popisku elementu ur�en� k ulo�en�

	protected:


};
#endif
