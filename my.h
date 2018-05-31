//---------------------------------------------------------------------------
#ifndef myH
#define myH
#include <vcl.h>
#include "CTpointD.h"
#include "scGPControls.hpp"//knihovna kv�li designov�n� tla��tek

//---------------------------------------------------------------------------
class Cmy
{
  private:

  public:
  long round(double number);//fce pro zaokrouhlen� realn�ho ��sla na nejbl힚� cel� ��slo
  double round2double(double number,unsigned short precision);//zaokrouhl� na po�et desetinn�ch m�st dle precison
  double round2double(double number,unsigned short from_min,double ifcontent);//zaokrouhl� pokud re�ln� ��st obsahuje hodnotu, zohled�uje po�adavek na minim�ln� desetinn� m�sto dle from
  double abs_d(double number);//fce vr�t� absolutn� hodnotu z double ��sla, mo�no nahradit moc� fabs z maht
  double ToRad(double number);
  double ToDeg(double number);
  //TPoint* P2L(TPoint *Physical, float Zoom, long PosunutiX, long PosunutiY);//P�evede Fyzick� sou�adnice na logicke, vraci logicke sou�adnice
	//double P2Lx(long fyzicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientWidth);//P�evede fyzick� sou�adnice (displej za��zen�) na logicke, vraci logicke sou�adnice
	TPointD P2L(TPoint fyzicke);
	TPointD P2L(long fyzickaX,long fyzickaY);
	double P2Lx(long fyzicka);
	//double P2Ly(long fyzicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//P�evede fyzick� sou�adnice (displej za��zen�) na logicke, vraci logicke sou�adnice
	double P2Ly(long fyzicka);
	//double P2LyTEST(long fyzicka);
	TPoint L2P(double logickaX, double logickaY);//P�evede logick� sou�adnice na fyzick� (displej za��zen�) , vraci fyzick� sou�adnice
	TPoint L2P(TPointD logicke);
	long L2Px(double logicka);
	long L2Py(double logicka);
	//long L2Px(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientWidth);//P�evede logick� sou�adnice na fyzick� (displej za��zen�) , vraci fyzick� sou�adnice
	//long L2Py(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//P�evede logick� sou�adnice na fyzick� (displej za��zen�) , vraci fyzick� sou�adnice
	//long L2PyTEST(double logicka, unsigned short int Zoom, double Posunuti, unsigned short int ClientHeight);//P�evede logick� sou�adnice na fyzick� (displej za��zen�) , vraci fyzick� sou�adnice
	long m2px(unsigned int meter,unsigned int Scale=1);//p�evede vzd�lenost v metrech na vzd�lenost monitoru
  float px2m(unsigned int px,unsigned int Scale=1);//P�evede logick� jednotky (udajn� metry) na pixely monitoru
  long px_offset(unsigned int X,unsigned int Y,long Distance);//vr�ti sou�adnice pixel� posunut� o Distance METR�
	bool cele_cislo(double number);
	double mod_d(double number1,double number2);//mo�no nahradit fci fmod z math.h  - to si u� tak nejsem jist� viz paramatry t�to funkce - ukazatel?
	bool isFinite(double number1,double number2);
	TColor clIntensive(TColor C,short A); //A + m�ra zesv�tlen�,  - m�ra ztmaveni
	double azimut(double X1,double Y1,double X2,double Y2);
	double uhel(double X1,double Y1,double X2,double Y2);
	double delka(double X1,double Y1,double X2,double Y2);
	TPointD rotace(double delka, double akt_uhel, double rotace);
	TPointDbool zkratit_polygon_na_roztec(double d, double r,double xp, double yp, double x0, double y0, double x1, double y1);//d - delka linky,r - roztec palcuxp, yp - souradnice oznaceneho bodu x0, y0, x1, y1- souradnice sousedu k oznacenemu bodu
	double cekani_na_palec(double cas, double roztec_palcu, double rychlost_dopravniku,int funkce);//vr�t� dobu �ek�n� na palec v sec, zad�n� je u �as (v�stupu voz�ku z objektu) v min, rozte�e je v m resp. v m za z minu u rychlosti dopravniku
	double mezera_mezi_voziky(double dV,double sV,double rotace,double roztec,double mezera=0);//metoda vrat� minim�ln� mo�nou mezeru mezi voz�ky (prom�nn� voz�ku prezentuje ���ku �� d�lku voz�ku dle aktu�ln� rotace v objektu), za p�edpokladu, �e je parametr mezera roven 0, v p��pad� nenulov�ho parametru mezery vrac� vhodnou nejmen�� hodnotu t�to mezery v��i rozm�r�m rozte� a rozm�r voz�ku
	double mezera(double dV,double sV,double rotace,double Rx,double R);//vr�t� mezeru dle rozestupu v palc�ch a rozte�e a velikosti voz�ku dle rotace
	double mezera(double rotace,double Rz);//vr�t� mezeru dle rozestupu a rotace (resp. velikosti voz�ku spo��tan� dle rotace)
	double Rz(double dV,double sV,double rotace,double M);//vr�t� rozestup v metrech mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
	double Rz(double RD);//vr�t� rozestup v metrech mezi aktivn�mi palci v souvsta�nosti k RD (a resp. TT)
	double Rx(double dV,double sV,double rotace,double M,double R);//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
	double Rx(double RD,double R);//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci z RD a R (a resp. TT)
	double Rx2(double Rz,double R);//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci z Rx a R
	double RD(double Rz);//vr�t� RD dle rozestupu v metrech mezi aktivn�mi palci v souvsta�nosti s TT
	double dopRD(double dV,double sV,double rotace,double R,double TT,double RD);//vr�t� doporu�enou nejbli��� rychlost pohonu, k rychlosti zadan� tak, aby se reflektovala rozte� mezi palci i takt
	double UDV(double dV,double sV,double rotace);//vrat� u�itnou d�lku voz�ku
	double UDV(double rotace);//vrat� u�itnou d�lku voz�ku, parametry bere z PP
	double UDJ(double dJ,double sJ,double rotace);//vr�t� u�itnou d�lku jigu
	bool lze_rotovat_jig_bez_zmeny_RzRxRD(double mezera,double akt_rotace);//vr�t�, zda je mo�n� orotovat jig tak, aby nem�lo vliv na zm�nu Rz, Rx, RD
	double prejezd_voziku(double delka, double rychlost_dopravniku);
	long LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2);
	void designButton(TscGPButton *button,TForm *form, short rank,short sum,short horizontal_space=22,short vertikal_space=11);//nastav� horizont�ln� a vertik�ln� pozici tla��tka a tak� designov� vlasnosti podle tla��tkek Ano, Ulo�it, OK, Storno dle MyMessageBox
	void frameForm(TForm *form,TColor color,short width=1);//vykresl� dan�mu oknu transparentn� (kv�li mo�n�mu smaz�n� - p�emaskov�n�) dle zadan� barvy a ���ky, nutno volat p�i formactive (l�pe v�ak p�i formpaint), p�i ���ce 1px (ta je z�rove� implicitn�) sta�� volat, jenom p�i formactive, jinak i p�i formsize, formresize,formclose, p��klad pou�it�: frameForm(Form_parametry,clWebOrange,1);
	double get_timePERpx(double speed,double A=1,double speed_min=0);//z rychlosti v m/s vrat� �as milisekund�ch pot�ebn� na p�ekreslen� jednoho pixelu p�i dan�m zoomu, parametr A=je rychlost animace, kdy implicitn� 1 origin�ln� rychlost - tedy 100%, pokud je parametr A=0, vr�t� se vhodn� �as na p�ehr�n� kontinu�ln� animace, metoda je vhodn� na animace a simulace pro timer  nehled� na rychlost, pokud je (i implicitn�) parametr speed_min==0, tzn. p�evezme se hodnota aktu�ln�ho po��tan�ho RD, tzn. v�echny animace se prom�tnou se stejn�m afps dle fps, tj. v�echny animace se zobraz� kontinu�ln� (netrhnan�), v p��pad� nenulov� hodnoty je speed_min stanovane jako minim�ln� mo�n� rychlost pro zobrazen� kontinu�ln� (netrhnan�) simulace, pokud by byl paremetr speed ni���, nebude se jednat kontinu�ln� (netrhnanou) simulaci
	bool between(double value,double from, double to, bool left_closed=true, bool right_closed=true);//vrac� true �i falce zda se dan� hodnota nach�z� �� nenach�z� v intervalu, interval m��e b�t uzav�en� (tzn. v�etn� hodnoty hranice intervalu) nebo otev�en� a to i rozd�ln� pro ob� meze, implicitn� jsou hranice nastaveny na uzav�en� interval z obou stran, tzn. do podm�nky se zahrnuje v�etn� obou hodnot

	protected:

};
#endif

