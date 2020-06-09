//---------------------------------------------------------------------------
#ifndef myH
#define myH
#include <vcl.h>
#include "CTpointD.h"
#include "scGPControls.hpp"//knihovna kvůli designování tlačítek
const double DOUBLE_MIN=-1.7*pow(10,-308.0);
const double DOUBLE_MAX=1.7*pow(10,308.0);
//---------------------------------------------------------------------------
class Cmy
{
  private:

  public:
	long round(double number);//fce pro zaokrouhlení realného čísla na nejblížší celé číslo
	double round2double(double number,unsigned short precision);//zaokrouhlí na počet desetinných míst dle precison
	AnsiString round2double(double number,unsigned short precision,AnsiString mark,bool add_decimal=false);//zaokrouhlí na počet desetinných míst dle precison a vratí hodnotu pomocí řetezce, za číslem následuje znak, dle posledního parametru (např dvě tečky .. jakože číslo pokračuje), pokud číslo obsahuje reálnou část nezobrazenou v rámci precision, pokud je nastaven poslední parametr add_decimal na true a je-li reálná část kratší než počet reaálných míst decimál, jsou do tohototo počtu doplněny nuly
	double abs_d(double number);//fce vrátí absolutní hodnotu z double čísla, možno nahradit mocí fabs z math
	double ToRad(double number);
	double ToDeg(double number);
	short Rt90(double number);//zaokrouhlí na násobky 90tky, 360° převede na 0°, větší než 360° vratí za 0°
	double a360(double number);//záporné stupně převede do kladných v rámci 360°
	double r2o(double number);//převede hodnotu rotace (proti směru hodinových ručiček a vpravo naležato jako výchozí pozice) na orientaci dle světových stran
	double o2r(double number);//převede hodnotu orientace dle světových stran na hodnotu rotace (proti směru hodinových ručiček a vpravo naležato jako výchozí pozice)
	double R2Larc(double radius,double angle);//ze zadaného radiusu vrátí obvod kruhové výseče o velikosti definované úhlem výseče ve stupních
	double T2Aarc(double radius,double t_lenght);//ze zadaného radiusu a délky tetivy vrátí úhel kruhové výseče ve stupních
	double getAngleFromTriangle(double a,double b,double c,short p);//ze tří zadaných stran trojúhlelníku vrátí úhel dle parametru p 1-alfa,2-beta,3-gama
	TPointD P2L(TPoint fyzicke);
	TPointD P2L(long fyzickaX,long fyzickaY);
	double P2Lx(long fyzicka);
	double P2Ly(long fyzicka);
	TPoint L2P(double logickaX, double logickaY);//Převede logické souřadnice na fyzické (displej zařízení) , vraci fyzické souřadnice
	TPoint L2P(TPointD logicke);
	TPoint *L2P(TPointD *POLE,long posledni_prvek);
	void L2P(TPointD *POLE,TPoint *POLEpx);
	long L2Px(double logicka);
	long L2Py(double logicka);
	long m2px(double meter);//převede vzdálenost v metrech na vzdálenost monitoru dle konstanty m2px (metrů na pixel) a aktuální hodnoty proměnné Zoom
	double px2m(long px);//převede logické jednotky (metry) na pixely monitoru dle konstanty m2px (metrů na pixel) a aktuální hodnoty proměnné Zoom
	long double getResolution(int puvX,int puvY,int aktX,int aktY,double metry);//dle změřeného úseku na monitoru převede tuto pixelovou vzádlenost a hodnotu v podobě uživatelského vstupu v metrech na hodnotu rozlišení podkladu
	long px_offset(unsigned int X,unsigned int Y,long Distance);//vráti souřadnice pixelů posunuté o Distance METRů
	bool cele_cislo(double number);
	double mod_d(double number1,double number2);//možno nahradit fci fmod z math.h  - to si už tak nejsem jistý viz paramatry této funkce - ukazatel?
	bool isFinite(double number1,double number2);
	double getL(double RA,double R);//vrátí vzdálenost od výchozího a koncového bodu k řídícímu bodu oblouku realizovaného bézierovou křivkou, vstupním parametrem je rotační úhel a radius, připraveno pouze pro některé úhly, výpočet není sice zcela exaktní, ale v rámci požadované tolerance výborný
	TPointD *getArcLine(double X,double Y,double orientace,double rotacni_uhel,double radius);//vrátí souřadnice (4 místné pole TPointD tj. 8 hodnot) bézierovy křivky oblouku či linie dle zadaných souřadnic, X,Y jsou logické souřadnice výchozího vykreslování, parametry: orientace oblouku - dle světových stran (umí i jiné než 90° násobky), rotační úhel - pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0° jedná se o linii), radius - je radius oblouku v metrech nebo pokud je rotační úhel nastaven na 0° tedy se jedná o linii, je radius délkou linie
	TPointD_3D bezierPt(double orientace,double rotacni_uhel,double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double perc);//vratí bod včetně akutálního azimutu bodu z bézierovy křivky dle zadaných procent z její délky, perc jsou procenta/100 pozice na křivce v intervalu <0,1>
	TPointD_3D getPt(double radius,double orientace,double rotacni_uhel,double X1,double Y1,double X4,double Y4,double perc,double perc2,double rotacni_uhel_predchozi=0, double rotacni_uhel_nasledujici=0);//vratí bod z linie čí z oblouku dle poměřu (perc) k celkové délce dané křivky, perc2 slouží na poměr v místě středu vozíku, který nemusí být díky rozdílnému místu uchycení totožný s parametrem perc
	double getPt(double n1,double n2,double perc);//podpůrná metoda výše uvedené
	double bezierDelka(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,double prec=0.001);//vrátí délku bézierovy křivky, prec=preciznost výpočtu
	double delka(double X1,double Y1,double X2,double Y2);//vrátí délku mezi body v absolutní hodnotě
	double azimut(double X1,double Y1,double X2,double Y2);
	double uhel(double X1,double Y1,double X2,double Y2);
	TPointD rotace(double delka, double akt_uhel, double rotace); //pozor, akt_uhel neni azimut, nutno používat akt_uhel=180-dodaný azimut (potom odpovídá orientaci tzn. např. 270 je na "vpravo, na západě"), příklad rotace(10,180-270,90); orotuje 10 metrovou linii směrující vodorovně zleva doprava o 90° proti směru hodinových ručiček tj. na 180° orientace, rotace - naopak rotuje proti směru hodinových ručiček
	TPointD rotace(double X1,double Y1,double X2,double Y2,double uhel);//vrátí souřadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y2 proti směru hodinových ručiček
	TPoint rotace_px(long X1,long Y1,long X2,long Y2,double uhel);//vrátí souřadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y2 proti směru hodinových ručiček
	void rotace_polygon(double X,double Y,TPointD *POLE,long posledni_prvek,double uhel);//orotuje okolo daného bodu polygon
	void rotace_polygon(double X,double Y,double oX,double oY,TPointD *POLE,TPoint *POLE_px,long posledni_prvek,double uhel);//orotuje okolo daného bodu polygon a vrátí do POLE i do POLE_px
	void zrcadli_polygon(TPointD *POLE,long posledni_prvek,double uhel);//zajistí přezrcadlení polygonů zadaného v logických souřadnicích pomocí pole dle požadovaného parametru uhel zrcadlení
	TPointDbool zkratit_polygon_na_roztec(double d, double r,double xp, double yp, double x0, double y0, double x1, double y1);//d - delka linky,r - roztec palcuxp, yp - souradnice oznaceneho bodu x0, y0, x1, y1- souradnice sousedu k oznacenemu bodu
	double cekani_na_palec(double cas, double roztec_palcu, double rychlost_dopravniku,int funkce);//vrátí dobu čekání na palec v sec, zadání je u čas (výstupu vozíku z objektu) v min, rozteče je v m resp. v m za z minu u rychlosti dopravniku
	double mezera_mezi_voziky(double dJ,double sJ,double rotace,double roztec,double mezera=0,unsigned short typ=0);//metoda vratí minimální možnou mezeru mezi vozíky (proměnná vozíku prezentuje šířku čí délku vozíku dle aktuální rotace v objektu), za předpokladu, že je parametr mezera roven 0, v případě nenulového parametru mezery vrací vhodnou nejmenší hodnotu této mezery vůči rozměrům rozteč a rozměr vozíku, lze parametrizovat vracený výsledek 0 (implicitně) - kritická mezera, 1 či 281 - mezera mezi JIG, 2 či 282 mezera mezi PODVOZKY
	double mezera(double dJ,double sJ,double rotace,double Rx,double R,short typ=-1);//vrátí mezeru dle rozestupu v palcích a rozteče a velikosti vozíku dle rotace, typ==-1 vrátí velikost mezery automaticky co je kritištější, zda podovzek či jig, 0 - podvozek, 1 - jig
	double mezera(double rotace,double Rz,short typ=-1);//vrátí mezeru dle rozestupu a rotace (resp. velikosti vozíku spočítané dle rotace), typ==-1 vrátí velikost mezery automaticky co je kritištější, zda podovzek či jig, 0 - podvozek, 1 - jig
	double minM(double RD1,double RD2,double R2);//rozpracovano
	double Rz(double dV,double sV,double rotace,double M);//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správně mezera
	double Rz(double RD);//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k RD (a resp. TT)
	double Rz(double Rx,double R);//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k Rx a R
	double Rx(double dV,double sV,double rotace,double M,double R);//vrátí rozestup v počtech palců mezi aktivními palci, byla-li zadáná správně mezera
	double Rx(double RD,double R);//vrátí rozestup v počtech palců mezi aktivními palci z RD a R (a resp. TT)
	double Rx2(double Rz,double R);//vrátí rozestup v počtech palců mezi aktivními palci z Rz a R
	double R(double Rz,double Rx);//vrátí rozteče dle rozestupu a počtu palců
	double RD(double Rz);//vrátí RD dle rozestupu v metrech mezi aktivními palci v souvstažnosti s TT
	double RD(double LO,double PT);//vratí RD dle velikosti lakovacího okna a procesního času
	double PT(double LO,double RD);//vrátí PT dle velikosti lakovacícho okna a aktuální rychlosti pohonu
	double PTo(double Dotoc,double RD);//dle aktuálního RD a délky otoč vrátí PTo
	double Dotoc(double PTo,double RD);//dle aktuálního RD a času otáčení otoče vrátí délku otáčení
	TPointD zona_otaceni(double rotace_pred,double rotace_otoce,double delka_otoc);//vypočítá velikost zóny otáčení před a za otočí
	//double RT(double PT,double delka_prejezdu,double RD,double R,double WT=-1);//vratí hodnotu RT (reserve time), ta může být i záporná, WT čekání na palac si dopočítává metoda sama, pokud WT==-1, pokud je dosazena kladná hodnota větší než 0, tak je ta uvažovaná jako WT, 0 hodnota znamena WT čekání na palec neuvažovat
	double RT(double PT,double doba_prejezdu,double WT,unsigned int pocet_voziku,double RD);//přetížená metoda
	double RDo(double Dotoc,double PTo);//vratí RD dle délky otoče a času otáčení
	double dopRD(double dJ,double sJ,double rotace,double R,double TT,double RD);//vrátí doporučenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteč mezi palci i takt
	bool kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove);//vrací true pokud nová rozteč (R) ovlivní Rz resp RD
	double WT2V(double WT,double TT,double Floor=false);//dle požadovaného času čekání a požadovaného taktu vrátí počet vozíku, pokud je druhý parametr true osekne vrácený výsledek na celé číslo zaokrouhlené směrem dolu
	double V2WT(double V,double TT);//dle počtu vozíků a požadovaného taktu vrátí dobu čekání
	double UDV(double dJ,double sJ,double rotace);//vratí užitnou délku vozíku
	double UDV(double rotace);//vratí užitnou délku vozíku, parametry bere z PP
	double UDJ(double dJ,double sJ,double rotace);//vrátí užitnou délku jigu
	double UDJ(double rotace);//vrátí užitnou délku jigu, parametry bere z PP
	bool lze_rotovat_jig_bez_zmeny_RzRxRD(double mezera,double akt_rotace);//vrátí, zda je možné orotovat jig tak, aby nemělo vliv na změnu Rz, Rx, RD
	double prejezd_voziku(double delka, double rychlost_dopravniku);//vrátí čas přejezdu vozíku
	double prejezd_voziku_rychlost(double CT,double MT,double PT,double WT,double DD);//vrátí požadovanou rychlost přejezdu, umí si dopočítat MT, není-li dodáno, pokud vyjde záporná rychlost tzn. nestíhá
	double kontrola_rychlosti_prejezdu(double CT,double MT,double PT,double WT,double DD,double aRD);//vrátí rozdíl aktuální rychlosti pohonu a potřebné k uskuteční přejezdu, pokud je hodnota 0 je v pořádku, je-li záporná, přejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
	long LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2);
	bool LeziVoblouku(double X,double Y,double orientace,double RA,double R,double Xmys,double Ymys);//funkce ověřující, zda kurzor myši, leží v obdelníku obsaném danému oblouku, souřadnice kurzoru myši se zadávají v logických souřadnicích, ostatní v logických, pro ověření zda leží na oblouku (s určitým perimetrem okolo obloukové linie) řešení níže metoda PtInSegment
	bool PtInCircle(double point_X,double point_Y,double center_X,double center_Y,double radius);//metoda ověří, zda se bod nachází v zadaném kruhu
	bool PtInRectangle(double X1,double Y1,double X2,double Y2,double Xmys,double Ymys);//metoda ověří, zda se bod nachází v obdelníku
	bool PtInStopka(double Ex,double Ey,double X,double Y,short uhel);//metoda ověří, zda se bod nachází ve stopce
	bool PtInClovek(double X,double Y,double Xmys,double Ymys,double rotace,short eID);//metoda ověří zda je bod ve vnitř obrysu člověka, který se nachází na daných souřadnicích
	bool PtInIon(double X,double Y,double Xmys,double Ymys,double rotace);//metoda ověří zda je bod ve vnitř obrysu ION tyčí, který se nachází na daných souřadnicích
	bool PtInSegment(double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka,double Xmys,double Ymys);//otestuje zda v daném geometrickém segmetnu (o velikosti pásma aktuální šířky kolejí) se nachází souřadnice kurzor myši, pokud ano, vrátí true, jinak false
	void designButton(TscGPButton *button,TForm *form, short rank,short sum,short horizontal_space=22,short vertikal_space=11);//nastaví horizontální a vertikální pozici tlačítka a také designové vlasnosti podle tlačítkek Ano, Uložit, OK, Storno dle MyMessageBox
	void frameForm(TForm *form,TColor color,short width=1);//vykreslí danému oknu transparentní (kvůli možnému smazání - přemaskování) dle zadané barvy a šířky, nutno volat při formactive (lépe však při formpaint), při šířce 1px (ta je zároveň implicitní) stačí volat, jenom při formactive, jinak i při formsize, formresize,formclose, příklad použití: frameForm(Form_parametry,clWebOrange,1);
	void frameRect(TRect Rect,TColor color=clWebOrange,short width=1);//nakresli na libovolném místě na monitoru obdélník dle zadaných souřadnic ve formátu TRect, je možné nastavit barvu a šířku čáry tohoto obdélníku
	double get_timePERpx(double speed,double A=1,double speed_min=0);//z rychlosti v m/s vratí čas milisekundách potřebný na překreslení jednoho pixelu při daném zoomu, parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se vhodný čas na přehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer  nehledě na rychlost, pokud je (i implicitní) parametr speed_min==0, tzn. převezme se hodnota aktuálního počítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálně (netrhnaně), v případě nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace, pokud by byl paremetr speed nižší, nebude se jednat kontinuální (netrhnanou) simulaci
	bool between(double value,double from, double to, bool left_closed=true, bool right_closed=true);//vrací true či falce zda se daná hodnota nachází čí nenachází v intervalu, interval může být uzavřený (tzn. včetně hodnoty hranice intervalu) nebo otevřený a to i rozdílně pro obě meze, implicitně jsou hranice nastaveny na uzavřený interval z obou stran, tzn. do podmínky se zahrnuje včetně obou hodnot
	double null(double number,double tolerance=0.0000000001);//zkontroluje číslo, zda se nejadná o hodnout okolo nuly dle možné odchylky dle tolerance, předchází zvláštnímu chování výpočtů okolo nuly
	TColor clIntensive(TColor C,short A);//+A  míra zesvětlení,  -A  míra ztmaveni, max hodnota 255, min hodnota -255
	short get_intensity();//přepočítá hodnotu posuvníku intenzity z menu nastavení na změnu barevné intenzity při vykreslění objektů či elementů
	TColor getBlackOrWhiteInvert(TColor color);//vrátí černou pro světlé barvy a bílou pro tmavé barvy, používá se např. na volbu barvy textu s barevným pozadím
	TColor getColorOfPalette(unsigned int input);//vratí dle hodnoty input barvu z nadefinované barevné palety, v případě "přetečení barvy opakuje
	short getValueFromPosition(long GlobalValue,short Position);

	protected:
};
#endif

