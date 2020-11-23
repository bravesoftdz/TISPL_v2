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
	int CorEx(Cvektory::TObjekt *O);//vr�t� sou�adnice dle typu bu� st�edov� nebo excentrick� v podob� lev�ho horn�ho rohu objektu
	int CorEy(Cvektory::TObjekt *O);//vr�t� sou�adnice dle typu bu� st�edov� nebo excentrick� v podob� lev�ho horn�ho rohu objektu

	Cvektory::TProces *Pom_proces;//pomocn� ukazatel na proces, vyu��v� se v p�i na��t�n� p�vodn� vytvo�en�ch n�hodn�ch hodnot �ek�n� na palec
	short oY;//ofset na ose Y, 5 pouze grafick� korekce
	float sizeP;//velikost text� popisk� element� v knihovn�
  bool prichyceno_na_vozik;

	public:
	Cvykresli();//konstruktor
	Cvektory v;
	Cmy m;

	enum Talign{LEFT,CENTER,RIGHT};
	enum Tvalign{TOP,MIDDLE,BOTTOM};

	void vykresli_halu(TCanvas *canv,int typ=-2);//stav: -3 kurzor, -2 normal (implicitn�), -1-highlight bez editace, 0-editace zv�razn� v�echny body, 1-a� po�et bod� zv�razn�n� dan�ho bodu,po�et bod�+1 zv�razn�n� dan� hrany v�etn� soused�c�c�h �chop� (nap�. pro polygono o 6 bodech) bude hodnota stavu 7 zv�razn�n� prvn� hrany (od bodu 1 do bodu 2)
	void vykresli_vektory(TCanvas *canv, short scena=0);//vykresl� zak�zky, cesty, spojnice, kabiny, pohony, elementy atd. //scena 0 - v�e, scena 1 - statick� sc�na, scena 2 - dynamick� sc�na
	void vykresli_objekty(TCanvas *canv);
	void vykresli_objekt(TCanvas *canv, Cvektory::TObjekt *ukaz);
	void vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O);
	void prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N);
	void vykresli_kabinu(TCanvas *canv,Cvektory::TObjekt *O,int stav=-2,bool zobrazit_koty=true);//zaji��uje vykreslen� pouze obrysu dle typu objektu
	void vykresli_pow_sprchu(TCanvas *canv,long X1,long X2,long Y1,long Y2,unsigned int velikost_komory_px,TColor color,double sirka,short pmpp,short typ=0,double orientace=-90);//symbolika tekouc� kapaliny u POW
	void vykresli_grid(TCanvas *canv,int size_grid=10);
	void vykresli_meridlo(TCanvas *canv,int X,int Y,bool kalibracni_sipka=false);//v p��pad� m��en� vzd�lenosti vykresl� spojnici a pop�. vypisuje hodnotu vzd�lenosti, pou��v� se asi i na kalibraci podkladu
	void vykresli_meridlo(TCanvas *canv);//metoda na vykreslen� m���tka � la magnetick� laso nebo vz�dlenost teplom�r� v na pohonu, p�sm�ruje vykreslen� podle trendu
  bool vykresli_cit_oblasti_lasa(TCanvas *canv);//vykresl� citeln� oblasti element� na kter� je mo�n� se p�ichytit, vrac� true, pokud byla oblast vykreslena
	void vykresli_meridlo_po_trendu(TCanvas *canv,bool prichyceno);//vykresl� m��en� po trendu linky
	void vykresli_meridlo_proti_trendu(TCanvas *canv,bool prichyceno);//vykresli m��en� proti trendu linky
  void vykresli_oblast_teplomery(TCanvas *canv,short scena,Cvektory::TObjekt *Objekt);//vykresl� teplom�ry a cestu mezi nimi
  void vykresli_segment_cesty_teplomeru(TCanvas *canv,Cvektory::TElement *Element,TColor barva,short teplomer=0);//vykresl� segment cesty oblasti teplom�r�, parametr teplom�r ud�v� zda se bude vykreslovat prvni nebo posledni teplom�r, 1 ... prvni, 2 ... posledni
	void vykresli_pozice_a_zony(TCanvas *canv,Cvektory::TElement *E);//vykresli pozic a obalov�ch z�n
	void vykresli_dopravnik(TCanvas *canv, Cvektory::TZakazka *zakazka=NULL);//kresl� koleje, pouzdro �et�zu, �et�z samotn�, p�ed�vac� m�sto i popisek pohonu, slou�� i z�rove� na n�hled cest zak�zek
  void vykresli_vetev_dopravniku(TCanvas *canv);//vykreslen� highlightu akt. editovan� v�tve
	void vykresli_retez(TCanvas *canv,Cvektory::TRetez *Retez);
	void vykresli_koleje(TCanvas *canv,Cvektory::TElement *E);//vykreslen� jednoho geometrick�ho segmentu dvou p�r� kolej�
	void vykresli_koleje(TCanvas *canv,double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka,TColor clKolej=(TColor)RGB(255,69,0),short TypZarazky=0,TColor colorZarazka=NULL,bool gdiplus=false);//vykreslen� jednoho geometrick�ho segmentu dvou p�r� kolej�, TypZarazky=0 bez,1=na za��tku,2=na konci,3=na za��tku i na konci, barva colorZarazka nastav� pro p��padn� zobrazovanou zar�ku speci�ln� barvu, pokud nen� parametr pou�it (je NULL), tak dostane p��padn� zar�ka stejnou barvu jako barva parametru color
	void vytvor_oblast_koleje(TCanvas *canv,double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka);//vytvo�en� jednoho geometrick�ho segmentu z dvou p�r� kolej� ur�en�ho k testov�n�, zda se nachaz� v dan� oblasti bod, podru�n� metoda, volan� z matematick� knihovny
	void vykresli_palce(TCanvas *canv,Cvektory::TPohon *pohon);//zaji��uje vykreslen� palc�
	void vykresli_palec(TCanvas *canv,Cvektory::TPalec *P);//zaji��uje samotn� vykreslen� palce
	void vykresli_popisek_pohonu(TCanvas *canv,AnsiString text,TPoint zacatek,TPoint konec,short trend,bool pozice);//vykresl� popisek pohonu ve st�edu zadan� �se�ky, parametr pozice zaji��uje st��d�n� pozice popisku
	void vykresli_voziky(TCanvas *canv);//vykresl� v�echny voz�ky ze seznamu voz�k�
	void vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dJ,double sJ,double orientaceP=0,double rotaceJ=0,TColor clChassis=(TColor)RGB(50,50,50), TColor clJig=clPurple,float Width=2);//vykreslen� jednoho komplexn�ho voz�ku (podvozek v�etn� jigu), , X,Y jsou sou�adnice uchycen� voz�ku k palci, co� nemus� b�t st�ed voz�ku
	void vykresli_jig(TCanvas *canv,double X,double Y,double dJ,double sJ,double orientaceP,double rotaceJ,TColor clJig=clPurple,float Width=2);
	void vykresli_vyrobky(TCanvas *canv,double X,double Y,double dJ,double sJ,double orientaceP,double rotaceJ,TColor clJig,float Width);
	void vykresli_vyrobek(TCanvas *canv,double X,double Y,double Z=5,double rotace=0,TColor color=clRed,double prohloubeni=0.02);
	void vykresli_elementy(TCanvas *canv,short scena);//cyklem proch�z� v�echny elementy a ty n�sledn� "select�" dle: scena 0 - v�e do dynamick�, scena 1 - implicitn� statick� elementy do statick� sc�ny, scena 2 - implicitn� statick� elementy do dynamick� sc�ny, scena 3 - implicitn� dynamick� elementy do statick� sc�ny, scena 4 - implicitn� dynamick� elementy do dynamick� sc�ny, 5 - zobrazit jenom popisky
	void vykresli_element(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,unsigned int eID=0,short typ=0,double rotace=0,short stav=1,double LO1=1.5,double OTOC_delka=0,double LO2=0,double LO_pozice=0,Cvektory::TElement *E=NULL);//celkov� vykreslovac� metoda, vykresl� bu� stopku, robota nebo oto�, scena 0 - v�e do dynamick�, scena 1 - implicitn� statick� elementy do statick� sc�ny, scena 2 - implicitn� statick� elementy do dynamick� sc�ny, scena 3 - implicitn� dynamick� elementy do statick� sc�ny, scena 4 - implicitn� dynamick� elementy do dynamick� sc�ny, 5 - zobrazit jenom popisky
	void vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ=0, double rotace=0, short stav=1,Cvektory::TElement *E=NULL);
	void vykresli_robota(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,short eID=1,short typ=0,double rotace=0,short stav=1,double LO1=1.5,double OTOC_delka=0,double LO2=0,double aP=0,float TS=0,double LO_pozice=0,Cvektory::TElement *E=NULL);
	void vykresli_cloveka(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,double LO1,double OTOC_delka,double LO2,Cvektory::TElement *E=NULL);//vykresli siluetu �lov�k s p��padn� p�idru�en�m elememntem, rotuje po sm�ru hodinov�ch ru�i�ek, pro animaci slou�� okolo hrani�n�ch stup�u 0,90,180,270, v�dy rozsah -45� a� +44�, nap�. 45-134� je maxim�ln� pracovn� rozsah pro �lov�ka rotovan�ho na 90� atd.
	void vykresli_otoc(TCanvas *canv,short scena,long X,long Y,AnsiString name,AnsiString short_name,short eID=5,short typ=0,double rotace=0,short stav=1,Cvektory::TElement *E=NULL);
	void vykresli_ion(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,double rotace,short stav,float TS=0,Cvektory::TElement *E=NULL);
	void vykresli_teplomer(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short eID,short typ,double rotace,short stav,Cvektory::TElement *Element);
	void vykresli_zarazku(TCanvas *canv,long X,long Y,double orientace,unsigned long objekt_n,String name="");
	void vykresli_predavaci_misto(TCanvas *canv,Cvektory::TElement *E,long X,long Y,AnsiString name,short typ=0,double rotace=0,short stav=1);
	void vykresli_lakovaci_okno(TCanvas *canv,long X,long Y,double LO1,double OTOC_delka,double LO2,double delka_ramena,double rotace=0,double LO_pozice=0);
	void vykresli_ikonu_linie(TCanvas *canv,int X,int Y,AnsiString Popisek="linie",short stav=0);
	void vykresli_ikonu_oblouku(TCanvas *canv,int X,int Y,AnsiString Popisek="oblouky",short stav=0);
	void vykresli_ikonu_textu(TCanvas *canv,int X,int Y,AnsiString Popisek="text",short stav=0);
	void vykresli_ikonu_sipky(TCanvas *canv,int X,int Y,AnsiString Popisek="spojnice",short stav=0);
	void vykresli_ikonu_komory(TCanvas *canv,int X,int Y,AnsiString Popisek="komora",short typ=0,short stav=0,double orientace=90);//typ: -2 kurzor se sprchy, -1 kurzor bez sprchy, 0 ikona bez sprchy, 1 ikona se sprchou, stav: -1 disabled, 0 norm�l
  void vykresli_editovane_polozky(TCanvas *canv);//vykresli nad statickou sc�nu aktu�ln� editovan� polo�ky, pou��v� se p�i editaci textu, ale je zde bez AA
	Graphics::TBitmap *nacti_nahled_cesty(Cvektory::TZakazka *zakazka);//vytvo�� bmp pro zak�zku
	void vypis_zpravy(TCanvas *canv);//metoda vyp�e zpr�vy ze seznamu zpr�v a z�rove� ulo�� jejich citeln� oblasti
	void nastavit_text_popisu_objektu_v_nahledu(TCanvas *canv);
	void linie(TCanvas *canv,long X1,long Y1,long X2,long Y2,int Width,TColor Color=clBlack,TPenStyle PenStyle=psSolid,TPenMode PenMode=pmCopy,bool gdiplus=false);
	void line(TCanvas *canv,long X1,long Y1,long X2,long Y2,bool gdiplus=false);
	TPointD obdelnik(TCanvas *canv,double X1,double Y1,double X2,double Y2,double rotace,double Sx=DOUBLE_MIN,double Sy=DOUBLE_MIN);//orototuje obdeln�k, podle posledn�ch parametr�, pokud jsou tyto parametry neuveden�, rotuje okolo st�edu obrazce, pro p��padn� dals� pot�eby vr�t� sou�adnice st�edu orotovan�ho obd�lniku
	void bezier(TCanvas *canv,TPointD *POLE,long posledni_prvek,bool gdiplus=false);
	void bezier(TCanvas *canv,TPointD *POLE,long X,long Y,double oX,double oY,double rotace,long posledni_prvek);
	TPoint polygonDleOsy(TCanvas *canv,long X,long Y,float delka, float sirka1, float sirka2, double sklon, double rotace,TPenMode pmMode=pmCopy,TColor clFillOut=clBlack,TColor lFillIn=clWhite);
	void polygon(TCanvas *canv,Cvektory::TBod *body,TColor barva=clBlack, short sirka=1,int stav=-2,bool zobrazit_koty=true,bool automaticky_spojovat=true);//stav: -3 kurzor, -2 normal (implicitn�), -1-disabled, 0-editace zv�razn� v�echny body, 1-a� po�et bod� zv�razn�n� dan�ho bodu,po�et bod�+1 zv�razn�n� dan� hrany v�etn� soused�c�c�h �chop� (nap�. pro polygono o 6 bodech) bude hodnota stavu 7 zv�razn�n� prvn� hrany (od bodu 1 do bodu 2)
	void uchop(TCanvas *canv,Cvektory::TBod *B,TColor barva);//vykresl� jeden uchop/kole�ko zn�zor�uj�c� bod na polygonu
	void smart_kurzor(TCanvas *canv,Cvektory::TElement *E=NULL,short typElementu=0);//vol�n� smart kurzoru slou��c�ho pro v�b�r geometrick�ho elementu, to dle p�edchoz�ch parametr� resp. typ� geometrick�ch elementu a zadan�ch vstupn�ho paramer� metody, tj. dle posledn�ho existuj�c�ho elementu
	void smart_kurzor(TCanvas *canv,double preXk,double preYk,double preOR=0,double preRA=0,double prepreRA=0,short typElementu=0);//vol�n� smart kurzoru slou��c�ho pro v�b�r geometrick�ho elementu, to dle p�edchoz�ch parametr� resp. typ� geometrick�ch elementu a zadan�ch vstupn�ch paramer� metody, preXk,preYk - koncov� sou�adnice, preOR - orientace, preRA - rota�n� �hel p�edchoz�ho geometrick�ho elementu tj. posledn�ho ji� vytvo�en�ho, prepreRArota�n� �hel p�ed-p�edchoz�ho geometrick�ho elementu - neexistuj�-li tyto pre resp. prepre element(y), zadat nulu
	void vykresli_Gelement_kurzor(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,double delka_linie,double predchozi_rotacni_uhel,double predpredchozi_rotacni_uhel,short typElementu=0);//obloukov� �i liniov� (dle situace) kurzor g-elementu, X,Y jsou fyzick� sou�adnice v�choz�ho vykreslov�n�, parametry: orientace oblouku - dle sv�tov�ch stran (um� i jin� ne� 90� n�sobky), rota�n� �hel, pod kter�m je oblouk rotov�n, m��e b�t z�porn� (znam�nko ur�uje sm�r rotace, + proti sm�ru hodinov�ch ru�i�ek, - po sm�ru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0� jedn� se o linii), radius - je radius oblouku v metrech nebo pokud je rota�n� �hel nastaven na 0� tedy se jedn� o linii, je radius d�lkou linie)
	TPointD *vykresli_potencial_Gelement(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,TColor color,bool popisek);//zajist� jednor�zov� vykreslen� potenci�ln�ho obloukov�ho �i liniov�ho (dle situace) g-elementu, X,Y jsou logick� sou�adnice v�choz�ho vykreslov�n�, parametry: orientace oblouku - dle sv�tov�ch stran (um� i jin� ne� 90� n�sobky), rota�n� �hel - pod kter�m je oblouk rotov�n, m��e b�t z�porn� (znam�nko ur�uje sm�r rotace, + proti sm�ru hodinov�ch ru�i�ek, - po sm�ru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0� jedn� se o linii), radius - je radius oblouku v metrech nebo pokud je rota�n� �hel nastaven na 0� tedy se jedn� o linii, je radius d�lkou linie)
	TPointD *vykresli_Gelement(TCanvas *canv,double X,double Y,double orientace,double rotacni_uhel,double radius,TColor color,float width,String Text="",String Text2="",short typ=-1,TColor colorZarazka=NULL,bool gdiplus=false);//zajist� jednor�zov� vykreslen� libovoln�ho obloukov�ho �i liniov�ho (dle situace) g-elementu, X,Y jsou logick� sou�adnice v�choz�ho vykreslov�n�, parametry: orientace oblouku - dle sv�tov�ch stran (um� i jin� ne� 90� n�sobky), rota�n� �hel - pod kter�m je oblouk rotov�n, m��e b�t z�porn� (znam�nko ur�uje sm�r rotace, + proti sm�ru hodinov�ch ru�i�ek, - po sm�ru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0� jedn� se o linii), radius - je radius oblouku v metrech nebo pokud je rota�n� �hel nastaven na 0� tedy se jedn� o linii, je radius d�lkou linie), typ=-1 jen st�edov� ��ra, typ=0 jen koleje, typ 1=koleje+zar�ka na za��tku, typ 2=koleje+zar�ka na konci, typ 3=na za��tku i na konci, barva colorZarazka nastav� pro p��padn� zobrazovanou zar�ku speci�ln� barvu, pokud nen� parametr pou�it (je NULL), tak dostane p��padn� zar�ka stejnou barvu jako barva parametru color
	void vykresli_mGridy(TCanvas *canv=NULL);
	void nabuffrovat_mGridy(TmGrid *mGrid=NULL);//pomocn� metoda vytv��ej�c� rastrov� obrazy mGrid�, pokud je buffer na NULL, buffruj� se v�echny tabulky, pokud ne, tak pouze ta z parametru
	void vykresli_kotu(TCanvas *canv,Cvektory::TElement *Element_do,bool AAsize=true);
	void vykresli_kotu(TCanvas *canv,double X1,double Y1,double X2,double Y2,Cvektory::TElement * aktElement=NULL,double Offset=0,short highlight=0,float width=0.2,TColor color=clGray,bool LO_kota=false,Cvektory::TKomora *komora=NULL,bool AAsize=true);//v metrick�ch jednotk�ch krom� width, zde v px + automaticky dopo��t�v� d�lku a dosazuje aktu�ln� nastaven� jednotky highlight: 0-ne,1-ano,2-ano+vystoupen� k�ty i pozi�n�, aktElement pokud bude NULL, p�edpokl�d� se, �e je to k�ta kabiny, parametr ukladat_do_elementu p�epos�l�n do vno�en� metody
	void vykresli_kotu(TCanvas *canv,long X1,long Y1,long X2,long Y2,AnsiString Text,Cvektory::TElement *aktElement=NULL,int Offset=0,short highlight=0,float width=0.2,TColor color=clGray,bool LO_kota=false,Cvektory::TKomora *komora=NULL,Cvektory::TBod *bod=NULL,bool AAsize=true);//v px + dosazuje aktu�ln� nastaven� jednotky, highlight: 0-ne,1-ano,2-ano+vystoupen� k�ty i pozi�n�, aktElement pokud bude NULL, p�edpokl�d� se, �e je to k�ta kabiny, p�id�n parametr ukladat_do_elementu a to z d�vodu k�ty mezi lak. okny, nesm� b�t ukl�d�na do elementu ani do objektu
	void vykresli_stoupani_klesani(TCanvas *canv,Cvektory::TElement *Element,float offset=0);//vykresl� troj�heln�k indikuj�c� stoup�n� �i kles�n�
  void vykresli_ikonu_stoupani_klesani(TCanvas *canv,int X,int Y,short typ,String popisek="",short stav=0);
	void rotace_textu(TCanvas *canv, long rotace);//�hel rotace je desetin�ch stupn�
	void set_pen(TCanvas *canv, TColor color, int width, int style=PS_ENDCAP_SQUARE);//vr�t� HANDLE na nastaven� pera,//pop�.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matou�ek III str. 179 �i http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
	void set_pen2(TCanvas *canv, TColor color, int width, int ENDCAP=PS_ENDCAP_SQUARE,int JOIN=PS_JOIN_MITER,bool INSIDEFRAME=true,DWORD *UserLineArray=NULL,unsigned short LenghtArray=0,int TextureWidth=0);//vr�t� HANDLE na nastaven� pera,//pop�.PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE a PS_JOIN_ROUND, PS_JOIN_MITER (lze nastavit p�es SetMiterlimit) PS_JOIN_BEVEL, viz Matou�ek III str. 179 �i http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079, dod�n�m p�edposledn�ho parametru UserLineArray nap�. DWORD pole[]={m.round(3/3.0*F->Zoom),m.round(0.5/3.0*F->Zoom),m.round(0.2/3.0*F->Zoom),m.round(0.5/3.0*F->Zoom)} apod.  lze nadefinovat vlastn� typ linie, posledn� parametr je po�et prvk� onoho pole
	TColor set_color(TCanvas *canv, Cvektory::TObjekt *O);
	void drawRectText(TCanvas *canv,TRect Rect, UnicodeString Text);
	void TextFraming(TCanvas *canv,int X,int Y,UnicodeString Text,TFont *Font=NULL,TColor clFraming=clWhite,unsigned short FramingSize=4);//zajist� vykreslen� dan�ho textu dle nastaven�ho Fontu (pokud je NULL, p�evezme se akut�ln� font canvasu) v�etn� framingu, kter� je baravn� a velikostn� nastaviteln�
	HRGN TextOut(TCanvas *canv,int X,int Y,UnicodeString Text,Talign align=CENTER,Tvalign valign=MIDDLE,float FaktorLineSpacing=1,TColor clFraming=clWhite,unsigned short FramingSize=4,TFont *Font=NULL);//zajist� vykreslen� dan�ho textu dle nastaven�ho Fontu (pokud je NULL, p�evezme se akut�ln� font canvasu), mo�no vertik�ln� i horizont�ln� pozicovat, um� zalamovat ��dky v p��pad� v�skytu \n, velikost od��dkov�n� dle velikosti p�sma 1=100%, v p��pad� z�porn� hodnoty �azen� textu od prvn�ho ��dku dal��m sm�rem nahoru, um� tak� framing, kter� je baravn� a velikostn� nastaviteln�, neum� zat�m rotaci textu!!!
	Graphics::TBitmap *srafura(TColor color);//vytvo�� �rafovan� vzor
	Graphics::TBitmap *rastrovani(TColor color,short width, short distance);//vytvo�� rastrovan� vzor
	void editacni_okno(TCanvas *canv, unsigned int X1, unsigned int Y1,unsigned  int X2,unsigned  int Y2, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size=1, COLORREF color=clBlack);//nakresl� editacni_okno
	void sipka(TCanvas *canv,int X, int Y,float azimut,short typ=0, float size=3, COLORREF color=clBlack,COLORREF color_brush=clGray,TPenMode PenMode=pmCopy,TPenStyle PenStyle=psSolid,bool teziste_stred=true);//zajist� vykreslen� �ipky, typ 0 - barevn� v�pl�, 1 - transparentn� v�pl� (bez v�pln�), 2 - transparentn� v�pl� (bez v�pln�) a bez zn�zorn�n� hrany p�epony troj�hlen�ku tvo��c� �ipku, pozor velikost �ipky ji� nen�sobit zoomem
	void vykresli_kurzor_kabiny (TCanvas *canv, int id, int X, int Y, Cvektory::TObjekt *p=NULL);//vykresluje kurzor kabiny + spojnice
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
	void kurzor_cesta(TCanvas *canv);
	void vykresli_kurzor_cesta(TCanvas *canv,Cvektory::TElement *E);
	void vykresli_potencial_cesty(TCanvas *canv,Cvektory::TElement *E);
	void vykresli_casove_osy(TCanvas *canv,short scena);//vykreslen� �asov�ch os
	void vykresli_proces(TCanvas *canv, AnsiString shortname, TColor color, short typ, long X1, long X2, long Y,bool legenda);//vykresl� jeden d�l�� �asov� proces �asov�ch os (obdeln��ek procesu objektu) pro jeden voz�k
	void vykresli_Xosy(TCanvas *canv);//vykresl� statick� svislice na �asov� osy
	void zobrazit_label_zamerovac(int X,int Y);//vyp�e labal zam��ova� na pozici kurzoru my�i
	void vykresli_zamerovac_casovych_os(TCanvas *canv,int X,int Y);//vykresl� pohyblivou svislici na �asov� osy dle um�st�n� kurzoru my�i
	void vykresli_svislici_na_casove_osy(TCanvas *canv,int X=-1,bool AA=false);//v aktu�ln�m �ase simulace vykresl� svislici aktu�ln� ud�losti na �asov�ch os�ch

	//glob�ln� public prom�nn�
	TColor clStenaKabiny,clStenaHaly,clPasiv,clError,clWarning,clChassis,clJig,clMeridlo;
	short O_width,O_height,V_width;//logick� parametry, nezoomovat
	float Robot_sirka_zakladny;
	float Robot_delka_zakladny;
	double DoSkRB;//delka od st�edu (X,Y bodu) robota k referen�n�mu bodu robota (tj. k trysce) v metrech
	double DkRB;//d�lka k referen�n�mu bodu od uchopovac�ho bodu, respektive odsazen� �lov�ka od linky
	unsigned long cas;//ms
	double PX2SEC;//m���tko PX na sekundy
	short KrokY;//rozte� na �asov�ch os�ch
	int WidthCanvasCasoveOsy,HeightCanvasCasoveOsy;
	TPointD PosunT; //v�choz� posunut� obrazu Posunu
	//bool mod_vytizenost_objektu;
	//short NOLIEX;//NO - 0, LINEAR - 1, EXPO - 2  zn�zorn�n� vyt�en� oobjektu
	//bool JIZPOCITANO;//�e�en� aby se zbyte�n� a opakovan� neukl�dalo do PROCES� pokud to nen� treba
	//bool RANDOM;
	bool grafickeDilema;//provizorn� prom�nn� na p�ep�n�n� stavu, zda se p�i p�id�v�n� objektu a p�esouv�n� objektu bude zmen�ovat p�smo nebo nep�ekreslovat objekt
	//short legenda_polozky[8];//zobrazen� jednotliv�ch polo�ek, 0 index je v�ak po�et zobrazen�ch (ulo�en�ch) polo�ek v poli, -1 stav je u dan�ho atributu norm�l
	//short Xofset;//zajist� spr�vn� po��tek prvn�ho objektu dle ���ky nejdel�� vypisovan� minuty v ROMA
	short precision;//po�et desetinn�ch m�st ��sel na �asov� ose
	//TRect aktOblast;//aktu�ln� citeln� oblast popisku elementu ur�en� k ulo�en� ji� �e�eno p��mo v elementu
	int orientace_objektu;
	short predchozi_oblast;
	Cvektory::TGeometrie geoTemp;//pomocn� struktura slou�ic� na uchov�n� (k pozd�j��mu ulo�en� do geometrick�ch element�) aktu�ln� vracen�ch hodnot ze smart kurzoru
	short pasivni_vektory_intenzita;//intenzita pasivn�ch vektor� p�i editaci objektu
	unsigned long zobrazit_celou_zpravu;//prom�nn� ur�uj�c�, kter� zpr�va bude zobrazena
	unsigned long zprava_highlight;//pomocn� prom�nn� slou��c� na ztu�n�n� dan� zpravy
	long SCENA;//ZprVozEledElesDopObjHal
	bool GDIplus;//povoluje �i zakazuje GDI+ vykreslov�n� tam, kde je implementov�no

	protected:


};
#endif
