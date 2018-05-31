//---------------------------------------------------------------------------
#ifndef myH
#define myH
#include <vcl.h>
#include "CTpointD.h"
#include "scGPControls.hpp"//knihovna kvùli designování tlaèítek

//---------------------------------------------------------------------------
class Cmy
{
  private:

  public:
  long round(double number);//fce pro zaokrouhlení realného èísla na nejblížší celé èíslo
  double round2double(double number,unsigned short precision);//zaokrouhlí na poèet desetinných míst dle precison
  double round2double(double number,unsigned short from_min,double ifcontent);//zaokrouhlí pokud reálná èást obsahuje hodnotu, zohledòuje požadavek na minimální desetinné místo dle from
  double abs_d(double number);//fce vrátí absolutní hodnotu z double èísla, možno nahradit mocí fabs z maht
  double ToRad(double number);
  double ToDeg(double number);
  //TPoint* P2L(TPoint *Physical, float Zoom, long PosunutiX, long PosunutiY);//Pøevede Fyzické souøadnice na logicke, vraci logicke souøadnice
	//double P2Lx(long fyzicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientWidth);//Pøevede fyzické souøadnice (displej zaøízení) na logicke, vraci logicke souøadnice
	TPointD P2L(TPoint fyzicke);
	TPointD P2L(long fyzickaX,long fyzickaY);
	double P2Lx(long fyzicka);
	//double P2Ly(long fyzicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//Pøevede fyzické souøadnice (displej zaøízení) na logicke, vraci logicke souøadnice
	double P2Ly(long fyzicka);
	//double P2LyTEST(long fyzicka);
	TPoint L2P(double logickaX, double logickaY);//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
	TPoint L2P(TPointD logicke);
	long L2Px(double logicka);
	long L2Py(double logicka);
	//long L2Px(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientWidth);//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
	//long L2Py(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
	//long L2PyTEST(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
	long m2px(unsigned int meter,unsigned int Scale=1);//pøevede vzdálenost v metrech na vzdálenost monitoru
  float px2m(unsigned int px,unsigned int Scale=1);//Pøevede logické jednotky (udajnì metry) na pixely monitoru
  long px_offset(unsigned int X,unsigned int Y,long Distance);//vráti souøadnice pixelù posunuté o Distance METRù
	bool cele_cislo(double number);
	double mod_d(double number1,double number2);//možno nahradit fci fmod z math.h  - to si už tak nejsem jistý viz paramatry této funkce - ukazatel?
	bool isFinite(double number1,double number2);
	TColor clIntensive(TColor C,short A); //A + míra zesvìtlení,  - míra ztmaveni
	double azimut(double X1,double Y1,double X2,double Y2);
	double uhel(double X1,double Y1,double X2,double Y2);
	double delka(double X1,double Y1,double X2,double Y2);
	TPointD rotace(double delka, double akt_uhel, double rotace);
	TPointDbool zkratit_polygon_na_roztec(double d, double r,double xp, double yp, double x0, double y0, double x1, double y1);//d - delka linky,r - roztec palcuxp, yp - souradnice oznaceneho bodu x0, y0, x1, y1- souradnice sousedu k oznacenemu bodu
	double cekani_na_palec(double cas, double roztec_palcu, double rychlost_dopravniku,int funkce);//vrátí dobu èekání na palec v sec, zadání je u èas (výstupu vozíku z objektu) v min, rozteèe je v m resp. v m za z minu u rychlosti dopravniku
	double mezera_mezi_voziky(double dV,double sV,double rotace,double roztec,double mezera=0);//metoda vratí minimální možnou mezeru mezi vozíky (promìnná vozíku prezentuje šíøku èí délku vozíku dle aktuální rotace v objektu), za pøedpokladu, že je parametr mezera roven 0, v pøípadì nenulového parametru mezery vrací vhodnou nejmenší hodnotu této mezery vùèi rozmìrùm rozteè a rozmìr vozíku
	double mezera(double dV,double sV,double rotace,double Rx,double R);//vrátí mezeru dle rozestupu v palcích a rozteèe a velikosti vozíku dle rotace
	double mezera(double rotace,double Rz);//vrátí mezeru dle rozestupu a rotace (resp. velikosti vozíku spoèítané dle rotace)
	double Rz(double dV,double sV,double rotace,double M);//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správnì mezera
	double Rz(double RD);//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k RD (a resp. TT)
	double Rx(double dV,double sV,double rotace,double M,double R);//vrátí rozestup v poètech palcù mezi aktivními palci, byla-li zadáná správnì mezera
	double Rx(double RD,double R);//vrátí rozestup v poètech palcù mezi aktivními palci z RD a R (a resp. TT)
	double Rx2(double Rz,double R);//vrátí rozestup v poètech palcù mezi aktivními palci z Rx a R
	double RD(double Rz);//vrátí RD dle rozestupu v metrech mezi aktivními palci v souvstažnosti s TT
	double dopRD(double dV,double sV,double rotace,double R,double TT,double RD);//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
	double UDV(double dV,double sV,double rotace);//vratí užitnou délku vozíku
	double UDV(double rotace);//vratí užitnou délku vozíku, parametry bere z PP
	double UDJ(double dJ,double sJ,double rotace);//vrátí užitnou délku jigu
	bool lze_rotovat_jig_bez_zmeny_RzRxRD(double mezera,double akt_rotace);//vrátí, zda je možné orotovat jig tak, aby nemìlo vliv na zmìnu Rz, Rx, RD
	double prejezd_voziku(double delka, double rychlost_dopravniku);
	long LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2);
	void designButton(TscGPButton *button,TForm *form, short rank,short sum,short horizontal_space=22,short vertikal_space=11);//nastaví horizontální a vertikální pozici tlaèítka a také designové vlasnosti podle tlaèítkek Ano, Uložit, OK, Storno dle MyMessageBox
	void frameForm(TForm *form,TColor color,short width=1);//vykreslí danému oknu transparentní (kvùli možnému smazání - pøemaskování) dle zadané barvy a šíøky, nutno volat pøi formactive (lépe však pøi formpaint), pøi šíøce 1px (ta je zároveò implicitní) staèí volat, jenom pøi formactive, jinak i pøi formsize, formresize,formclose, pøíklad použití: frameForm(Form_parametry,clWebOrange,1);
	double get_timePERpx(double speed,double A=1,double speed_min=0);//z rychlosti v m/s vratí èas milisekundách potøebný na pøekreslení jednoho pixelu pøi daném zoomu, parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se vhodný èas na pøehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer  nehledì na rychlost, pokud je (i implicitní) parametr speed_min==0, tzn. pøevezme se hodnota aktuálního poèítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálnì (netrhnanì), v pøípadì nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace, pokud by byl paremetr speed nižší, nebude se jednat kontinuální (netrhnanou) simulaci
	bool between(double value,double from, double to, bool left_closed=true, bool right_closed=true);//vrací true èi falce zda se daná hodnota nachází èí nenachází v intervalu, interval mùže být uzavøený (tzn. vèetnì hodnoty hranice intervalu) nebo otevøený a to i rozdílnì pro obì meze, implicitnì jsou hranice nastaveny na uzavøený interval z obou stran, tzn. do podmínky se zahrnuje vèetnì obou hodnot

	protected:

};
#endif

