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
	double mezera_mezi_voziky(double vozik,double roztec,double mezera=0);//metoda vrat� minim�ln� mo�nou mezeru mezi voz�ky (prom�nn� voz�ku prezentuje ���ku �� d�lku voz�ku dle aktu�ln� rotace v objektu), za p�edpokladu, �e je parametr mezera roven 0, v p��pad� nenulov�ho parametru mezery vrac� vhodnou nejmen�� hodnotu t�to mezery v��i rozm�r�m rozte� a rozm�r voz�ku
	double Rz(double M,double DV);//vr�t� rozestup v metrech mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera, ta mus� b�t z�rove� zad�n� dle u�itn�ho rozm�ru (vrac� metoda UDV() z PM)
	double Rx(double M,double DV,double R);//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera, ta mus� b�t z�rove� zad�n� dle u�itn�ho rozm�ru (vrac� metoda UDV() z PM)
	double mezera(double Rx,double R,double DV);//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera, ta mus� b�t z�rove� zad�n� dle u�itn�ho rozm�ru (vrac� metoda UDV() z PM)
	double prejezd_voziku(double delka, double rychlost_dopravniku);
	long LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2);
	void designButton(TscGPButton *button,TForm *form, short rank,short sum,short horizontal_space=22,short vertikal_space=11);//nastav� horizont�ln� a vertik�ln� pozici tla��tka a tak� designov� vlasnosti podle tla��tkek Ano, Ulo�it, OK, Storno dle MyMessageBox

	protected:

};
#endif

