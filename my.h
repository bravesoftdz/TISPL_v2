//---------------------------------------------------------------------------
#ifndef myH
#define myH
#include <vcl.h>
#include "CTpointD.h"
#include "scGPControls.hpp"//knihovna kvùli designování tlaèítek
const double DOUBLE_MIN=-1.7*pow(10,-308.0);
const double DOUBLE_MAX=1.7*pow(10,308.0);
//---------------------------------------------------------------------------
class Cmy
{
  private:

  public:
	long round(double number);//fce pro zaokrouhlení realného èísla na nejblížší celé èíslo
	double round2double(double number,unsigned short precision);//zaokrouhlí na poèet desetinných míst dle precison
	AnsiString round2double(double number,unsigned short precision,AnsiString mark,bool add_decimal=false);//zaokrouhlí na poèet desetinných míst dle precison a vratí hodnotu pomocí øetezce, za èíslem následuje znak, dle posledního parametru (napø dvì teèky .. jakože èíslo pokraèuje), pokud èíslo obsahuje reálnou èást nezobrazenou v rámci precision, pokud je nastaven poslední parametr add_decimal na true a je-li reálná èást kratší než poèet reaálných míst decimál, jsou do tohototo poètu doplnìny nuly
	//double round2double(double number,unsigned short from_min,double ifcontent);//zaokrouhlí pokud reálná èást obsahuje hodnotu, zohledòuje požadavek na minimální desetinné místo dle from
	short Rt90(double number);//zaokrouhlí na násobky 90tky, 360° pøevede na 0°, vìtší než 360° vratí za 0°
	double abs_d(double number);//fce vrátí absolutní hodnotu z double èísla, možno nahradit mocí fabs z maht
	double ToRad(double number);
	double ToDeg(double number);
	TPointD P2L(TPoint fyzicke);
	TPointD P2L(long fyzickaX,long fyzickaY);
	double P2Lx(long fyzicka);
	double P2Ly(long fyzicka);
	TPoint L2P(double logickaX, double logickaY);//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
	TPoint L2P(TPointD logicke);
	long L2Px(double logicka);
	long L2Py(double logicka);
	long m2px(double meter);//pøevede vzdálenost v metrech na vzdálenost monitoru dle konstanty m2px (metrù na pixel) a aktuální hodnoty promìnné Zoom
	double px2m(long px);//pøevede logické jednotky (metry) na pixely monitoru dle konstanty m2px (metrù na pixel) a aktuální hodnoty promìnné Zoom
	long double getResolution(int puvX,int puvY,int aktX,int aktY,double metry);//dle zmìøeného úseku na monitoru pøevede tuto pixelovou vzádlenost a hodnotu v podobì uživatelského vstupu v metrech na hodnotu rozlišení podkladu
	long px_offset(unsigned int X,unsigned int Y,long Distance);//vráti souøadnice pixelù posunuté o Distance METRù
	bool cele_cislo(double number);
	double mod_d(double number1,double number2);//možno nahradit fci fmod z math.h  - to si už tak nejsem jistý viz paramatry této funkce - ukazatel?
	bool isFinite(double number1,double number2);
	TColor clIntensive(TColor C,short A); //A + míra zesvìtlení,  - míra ztmaveni
	double delka(double X1,double Y1,double X2,double Y2);
	double azimut(double X1,double Y1,double X2,double Y2);
	double uhel(double X1,double Y1,double X2,double Y2);
	TPointD rotace(double delka, double akt_uhel, double rotace);
	TPointDbool zkratit_polygon_na_roztec(double d, double r,double xp, double yp, double x0, double y0, double x1, double y1);//d - delka linky,r - roztec palcuxp, yp - souradnice oznaceneho bodu x0, y0, x1, y1- souradnice sousedu k oznacenemu bodu
	double cekani_na_palec(double cas, double roztec_palcu, double rychlost_dopravniku,int funkce);//vrátí dobu èekání na palec v sec, zadání je u èas (výstupu vozíku z objektu) v min, rozteèe je v m resp. v m za z minu u rychlosti dopravniku
	double mezera_mezi_voziky(double dJ,double sJ,double rotace,double roztec,double mezera=0,unsigned short typ=0);//metoda vratí minimální možnou mezeru mezi vozíky (promìnná vozíku prezentuje šíøku èí délku vozíku dle aktuální rotace v objektu), za pøedpokladu, že je parametr mezera roven 0, v pøípadì nenulového parametru mezery vrací vhodnou nejmenší hodnotu této mezery vùèi rozmìrùm rozteè a rozmìr vozíku, lze parametrizovat vracený výsledek 0 (implicitnì) - kritická mezera, 1 èi 281 - mezera mezi JIG, 2 èi 282 mezera mezi PODVOZKY
	double mezera(double dJ,double sJ,double rotace,double Rx,double R);//vrátí mezeru dle rozestupu v palcích a rozteèe a velikosti vozíku dle rotace
	double mezera(double rotace,double Rz);//vrátí mezeru dle rozestupu a rotace (resp. velikosti vozíku spoèítané dle rotace)
	double minM(double RD1,double RD2,double R2);//rozpracovano
	double Rz(double dV,double sV,double rotace,double M);//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správnì mezera
	double Rz(double RD);//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k RD (a resp. TT)
	double Rz(double Rx,double R);//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k Rx a R
	double Rx(double dV,double sV,double rotace,double M,double R);//vrátí rozestup v poètech palcù mezi aktivními palci, byla-li zadáná správnì mezera
	double Rx(double RD,double R);//vrátí rozestup v poètech palcù mezi aktivními palci z RD a R (a resp. TT)
	double Rx2(double Rz,double R);//vrátí rozestup v poètech palcù mezi aktivními palci z Rz a R
	double R(double Rz,double Rx);//vrátí rozteèe dle rozestupu a poètu palcù
	double RD(double Rz);//vrátí RD dle rozestupu v metrech mezi aktivními palci v souvstažnosti s TT
	double dopRD(double dJ,double sJ,double rotace,double R,double TT,double RD);//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
	bool kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove);//vrací true pokud nová rozteè (R) ovlivní Rz resp RD
	double UDV(double dJ,double sJ,double rotace);//vratí užitnou délku vozíku
	double UDV(double rotace);//vratí užitnou délku vozíku, parametry bere z PP
	double UDJ(double dJ,double sJ,double rotace);//vrátí užitnou délku jigu
	double UDJ(double rotace);//vrátí užitnou délku jigu, parametry bere z PP
	bool lze_rotovat_jig_bez_zmeny_RzRxRD(double mezera,double akt_rotace);//vrátí, zda je možné orotovat jig tak, aby nemìlo vliv na zmìnu Rz, Rx, RD
	double prejezd_voziku(double delka, double rychlost_dopravniku);//vrátí èas pøejezdu vozíku
	double prejezd_voziku_rychlost(double CT,double MT,double PT,double WT,double DD);//vrátí požadovanou rychlost pøejezdu, umí si dopoèítat MT, není-li dodáno, pokud vyjde záporná rychlost tzn. nestíhá
	double kontrola_rychlosti_prejezdu(double CT,double MT,double PT,double WT,double DD,double aRD);//vrátí rozdíl aktuální rychlosti pohonu a potøebné k uskuteèní pøejezdu, pokud je hodnota 0 je v poøádku, je-li záporná, pøejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
	long LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2);
	void designButton(TscGPButton *button,TForm *form, short rank,short sum,short horizontal_space=22,short vertikal_space=11);//nastaví horizontální a vertikální pozici tlaèítka a také designové vlasnosti podle tlaèítkek Ano, Uložit, OK, Storno dle MyMessageBox
	void frameForm(TForm *form,TColor color,short width=1);//vykreslí danému oknu transparentní (kvùli možnému smazání - pøemaskování) dle zadané barvy a šíøky, nutno volat pøi formactive (lépe však pøi formpaint), pøi šíøce 1px (ta je zároveò implicitní) staèí volat, jenom pøi formactive, jinak i pøi formsize, formresize,formclose, pøíklad použití: frameForm(Form_parametry,clWebOrange,1);
	void frameRect(TRect Rect,TColor color=clWebOrange,short width=1);//nakresli na libovolném místì na monitoru obdélník dle zadaných souøadnic ve formátu TRect, je možné nastavit barvu a šíøku èáry tohoto obdélníku
	double get_timePERpx(double speed,double A=1,double speed_min=0);//z rychlosti v m/s vratí èas milisekundách potøebný na pøekreslení jednoho pixelu pøi daném zoomu, parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se vhodný èas na pøehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer  nehledì na rychlost, pokud je (i implicitní) parametr speed_min==0, tzn. pøevezme se hodnota aktuálního poèítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálnì (netrhnanì), v pøípadì nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace, pokud by byl paremetr speed nižší, nebude se jednat kontinuální (netrhnanou) simulaci
	bool between(double value,double from, double to, bool left_closed=true, bool right_closed=true);//vrací true èi falce zda se daná hodnota nachází èí nenachází v intervalu, interval mùže být uzavøený (tzn. vèetnì hodnoty hranice intervalu) nebo otevøený a to i rozdílnì pro obì meze, implicitnì jsou hranice nastaveny na uzavøený interval z obou stran, tzn. do podmínky se zahrnuje vèetnì obou hodnot
	double null(double number,double tolerance=0.0000000001);//zkontroluje èíslo, zda se nejadná o hodnout okolo nuly dle možné odchylky dle toleration, pøedchází zvláštnímu chování výpoètù okolo nuly

	protected:

};
#endif

