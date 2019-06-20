//---------------------------------------------------------------------------
#pragma hdrstop
#include "my.h"
#include "Unit1.h"
#include "MyMessageBox.h"//kvùli referenci na tlaèítka v MyMessageBox
//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////////////////////////////////////
//fce pro zaokrouhlení realného èísla na nejblížší celé èíslo
long Cmy::round(double number)
{
	if(number>0)
	return (long)(number+0.5);//pro kladné hodnoty
	else
	return (long)(number-0.5);//pro záporné hodnoty
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhlí na poèet desetinných míst dle precison
double Cmy::round2double(double number,unsigned short precision)
{
	double decimal=number-floor(number);//nutno takto rozkládat, protože jinak by v pøípadì vìtšího celého základu nestaèil double
	return number=floor(number)+round(decimal*pow(10.0,precision))/pow(10.0,precision);
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhlí na poèet desetinných míst dle precison a vratí hodnotu pomocí øetezce, za èíslem následuje znak, dle posledního parametru (napø. dvì teèky .. jakože èíslo pokraèuje), pokud èíslo obsahuje reálnou èást nezobrazenou v rámci precision, pokud je nastaven poslední parametr add_decimal na true a je-li reálná èást kratší než poèet reaálných míst decimál, jsou do tohototo poètu doplnìny nuly
AnsiString Cmy::round2double(double number,unsigned short precision,AnsiString mark,bool add_decimal)
{
	double RET=round2double(number,precision);
	AnsiString RETT="0"; //floor musí být z dùvodu porovnávání kladných i záporných èísel a také z dùvodu že není možné porovnávat double a int, ale je nutné porovnat int vs. int
	if(floor(RET*pow(10.0,precision))!=ceil(number*pow(10.0,precision)))RETT=AnsiString(RET)+mark;//pokud èíslo obsahuje reálnou èást vrátí i se znakem pokraèování
	else
	{
		if(add_decimal)RETT=F->ms.addDecimal(RET,precision);//pokud je požadavek na doplnìní reálných míst do stanoveného poètu
		else RETT=AnsiString(RET);
	}
	return RETT;
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhlí na násobky 90tky, 360° pøevede na 0°, vìtší než 360° vratí za 0°
short Cmy::Rt90(double number)
{
	short RET=round(number/90.0)*90;
	if(RET>=360)RET-=360;     //pøípadnì: RET=fmod(number,360.0);
	if(RET>=360)RET=Rt90(RET);//rekurzce pro pøípady, že se bude jednát o nìkolikanásobnou rotaci èi kvùlip pøevodu z 360° na 0°
	if(RET<0)RET+=360;//pro záporné hodnoty
	if(RET<0)RET=Rt90(RET);//rekurzce pro pøípady, že se bude jednát o nìkolikanásobnou rotaci
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//ovìøí, zda dané èíslo je celé èíslo
bool Cmy::cele_cislo(double number)
{
  return number==floor(number);
}
/////////////////////////////////////////////////////////////////////////////
//modulo pro double hodnoty
double Cmy::mod_d(double number1,double number2)
{
	//nedotažené: return number1-floor(number1/number2)*number2;
	return fmod(number1,number2);//toto ale také nefunguje pro druhou reálnou hodnotu dobøe
}
/////////////////////////////////////////////////////////////////////////////
//fce vrátí absolutní hodnotu z double èísla, možno nahradit mocí fabs z math
double Cmy::abs_d(double number)
{
  return fabs(number);
}
/////////////////////////////////////////////////////////////////////////////
//pøevede stupnì na radiány
double Cmy::ToRad(double number)
{
	return number*M_PI/180.0;
}
/////////////////////////////////////////////////////////////////////////////
//pøevede radiány na stupnì
double Cmy::ToDeg(double number)
{
	return number*180/M_PI;
}
//////////////////////////////////////////////////////////////////////////////
//Pøevede logické souøadnice na fyzické (displej zaøízení) , vraci fyzické souøadnice
TPoint Cmy::L2P(double logickaX, double logickaY)
{
	return TPoint(L2Px(logickaX),L2Py(logickaY));
}
TPoint Cmy::L2P(TPointD logicke)
{
	return L2P(logicke.x,logicke.y);
}
long Cmy::L2Px(double logicka)
{
	//return round(Form1->Posun.X+logicka*Form1->Zoom);
	return round(Form1->Zoom*(logicka/Form1->m2px-Form1->Posun.x));
}
long Cmy::L2Py(double logicka)
{
	//return round(Form1->Posun.Y+logicka*Form1->Zoom);
	return round(Form1->Zoom*(-1*logicka/Form1->m2px-Form1->Posun.y));
}
TPoint *Cmy::L2P(TPointD *POLE,long posledni_prvek)
{
	//long velikost=sizeof(POLE)/sizeof(POLE[0]); nefunguje pro TPointD
	long velikost=posledni_prvek+1;
	TPoint *POLE_px=new TPoint [velikost];
	for(int i=0;i<velikost;i++){POLE_px[i].x=L2Px(POLE[i].x);POLE_px[i].y=L2Py(POLE[i].y);
	F->Memo1->Visible=true;
	F->Memo1->Lines->Add(POLE_px[i].x);
	F->Memo1->Lines->Add(POLE_px[i].y);
	F->Memo1->Lines->Add("______________");

	}
}
void Cmy::L2P(TPointD *POLE,TPoint *POLEpx)
{
	//POLEpx=L2P(POLE);
}
/////////////////////////////////////////////////////////////////////////////
//Pøevede  fyzické na logické souøadnice (displej zaøízení) , vraci logické souøadnice
TPointD Cmy::P2L(TPoint fyzicke)
{
	TPointD logicke;logicke.x=P2Lx(fyzicke.X);logicke.y=P2Ly(fyzicke.Y);
	return logicke;
}
TPointD Cmy::P2L(long fyzickaX,long fyzickaY)
{
	TPointD logicke;logicke.x=P2Lx(fyzickaX);logicke.y=P2Ly(fyzickaY);
	return logicke;
}
double Cmy::P2Lx(long fyzicka)
{
	try
	{
		return (fyzicka/Form1->Zoom+Form1->Posun.x)*Form1->m2px;
	}
	catch(...)
	{return 0;}
}
double Cmy::P2Ly(long fyzicka)
{
	try
	{
		return -1.0*(fyzicka/Form1->Zoom+Form1->Posun.y)*Form1->m2px;
	}
	catch(...)
	{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
//pøevede vzdálenost v metrech na vzdálenost monitoru dle konstanty m2px (metrù na pixel) a aktuální hodnoty promìnné Zoom
long Cmy::m2px(double meter)
{
	return round(meter*F->Zoom/F->m2px);
}
/////////////////////////////////////////////////////////////////////////////
//pøevede logické jednotky (metry) na pixely monitoru dle konstanty m2px (metrù na pixel) a aktuální hodnoty promìnné Zoom
double Cmy::px2m(long px)
{
	return px/F->Zoom*F->m2px;
}
/////////////////////////////////////////////////////////////////////////////
//dle zmìøeného úseku na monitoru pøevede tuto pixelovou vzádlenost a hodnotu v podobì uživatelského vstupu v metrech na hodnotu rozlišení podkladu
long double Cmy::getResolution(int puvX,int puvY,int aktX,int aktY,double metry)
{
	return metry/delka(P2Lx(puvX),P2Ly(puvY),P2Lx(aktX),P2Ly(aktY))*F->m2px;//výpoèet metry dìleno poèet PX, výchozí zobrazení v nativním rozlišení (bez usazení do metrického mìøítka) je 0.1
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::delka(double X1,double Y1,double X2,double Y2)
{
	return sqrt(pow(X2-X1,2)+ pow(Y2-Y1,2));
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::azimut(double X1,double Y1,double X2,double Y2)
{
	try
	{
		if(delka(X1,Y1,X2,Y2)!=0)
		{
			double A=acos((Y2-Y1)/(delka(X1,Y1,X2,Y2)*1.0))*180/M_PI;
			if(X2<X1)A=360-A;
			return A;
		}
		else return 0;
	}
	catch(...)
	{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::uhel(double X1,double Y1,double X2,double Y2)
{
	try
	{
		if(delka(X1,Y1,X2,Y2)!=0)
		{
			return acos((Y2-Y1)/(delka(X1,Y1,X2,Y2)*1.0))*180/M_PI;
		}
		else return 0;
	}
	catch(...)
	{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
//rotace
TPointD Cmy::rotace(double delka, double akt_uhel, double rotace)
{
	double Uhel=fmod(akt_uhel+rotace,360.0);// vèetnì ošetøení pøeteèení pøes 360 stupòù
	if(Uhel<0){Uhel+=360;}//pro záporné hodnoty

	short ZnamenkoX,ZnamenkoY;

	if (Uhel>=0 && Uhel< 90)
	{
		ZnamenkoX=1;ZnamenkoY=-1;
	}
	if (Uhel>=90 && Uhel<= 180)
	{
		Uhel=180-Uhel;
		ZnamenkoX=1;ZnamenkoY=1;
	}
	if (Uhel>180 && Uhel<= 270)
	{
		Uhel=Uhel-180;
		ZnamenkoX=-1;ZnamenkoY=1;
	}
	if (Uhel>270 && Uhel<= 360)
	{
		Uhel=360-Uhel;
		ZnamenkoX=-1;ZnamenkoY=-1;
	}

	Uhel=Uhel*(M_PI/180);//pro prevod ze stupnu na radiany
	TPointD ret;
	ret.x=sin(Uhel)*delka*ZnamenkoX;//posun na Xove ose
	ret.y=cos(Uhel)*delka*ZnamenkoY;//posun na Yove ose

	return ret;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí souøadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y1 proti smìru hodinových ruèièek
TPointD Cmy::rotace(double X1,double Y1,double X2,double Y2,double uhel)
{
	TPointD RET=rotace(delka(X1,Y1,X2,Y2),180-azimut(X1,Y1,X2,Y2),uhel);
	RET.x+=X1;RET.y+=Y1;
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí souøadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y1 proti smìru hodinových ruèièek
TPoint Cmy::rotace_px(long X1,long Y1,long X2,long Y2,double uhel)
{
	TPointD RETd=rotace(P2Lx(X1),P2Ly(Y1),P2Lx(X2),P2Ly(Y2),uhel);
	TPoint RET;RET.x=L2Px(RETd.x);RET.y=L2Py(RETd.y);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//orotuje okolo daného bodu polygon
void Cmy::rotace_polygon(double X,double Y,TPointD *POLE,long posledni_prvek,double uhel)
{
	if(fmod(uhel,360)!=0)//pouze akcelerátor
	{
		for(long i=0;i<=posledni_prvek;i++)POLE[i]=rotace(X,Y,POLE[i].x,POLE[i].y,uhel);
	}
}
/////////////////////////////////////////////////////////////////////////////
//orotuje okolo daného bodu polygon a vrátí do POLE i do POLE_px
void Cmy::rotace_polygon(double oX,double oY,double X,double Y,TPointD *POLE,TPoint *POLE_px,long posledni_prvek,double uhel)
{
	for(long i=0;i<=posledni_prvek;i++)
	{
		POLE[i]=rotace(oX,oY,POLE[i].x,POLE[i].y,uhel);
		POLE_px[i].x=L2Px(X+POLE[i].x-oX);POLE_px[i].y=L2Py(Y+POLE[i].y-oY);
	}
}
/////////////////////////////////////////////////////////////////////////////
//TPointD Cvektory::Get_AreaSize(TPolygon *ukaz)
//{
//	unsigned int delka=0;
//	float azimut=0;
//	float obsah=0;
//	unsigned int pocet=0;
//	bool pouze_usecka=false;
//
//	while (ukaz!=NULL)//pro všechny linie
//	{
//		if(ukaz->n>0)//vynechá hlavièku
//		{
//			pocet++;
//			Cvektory::TBod_LP *p;
//			p=ukaz->prvky;
//			while (p->dalsi!=NULL)//pro konkretni linii
//			{
//				TPointD P=w.WebMercator2UTM(p->X,p->Y);
//				TPointD P_dalsi=w.WebMercator2UTM(p->dalsi->X,p->dalsi->Y);
//				TPointD P_pred=w.WebMercator2UTM(p->predchozi->X,p->predchozi->Y);
//				if(p->n>0)//vynechá hlavièku
//				{
//					delka+=my.round(sqrt(pow(P_dalsi.x - P.x, 2)+ pow(P_dalsi.y - P.y, 2)));
//					if(p->n!=1)obsah+=P.x*(P_dalsi.y - P_pred.y); //Výpoèet_plochy_pomocí_L´Huillierových_vzorcù vèetnì nedokonèených ploch èi linii, alternativa Mascheroniho vzorec:http://klobouk.fsv.cvut.cz/~hanek/K154/PDF/Vypocet_vymer.pdf
////					if(pouze_usecka)//spoèítá azimut
////					{
////						azimut=acos((P_dalsi.y-P.y)/(delka*1.0))*180/M_PI;
////						if(P_dalsi.x<P.x)azimut=360-azimut;
////					}
//				}
////				else //zjistí, jestli obsahuje pouze dva body, tj. jestli se jedná o úseèku a je dùvod zmìøit i azimut
////				{
////					if(p->predchozi->n==2)pouze_usecka=true;
////					TPointD P_pred_pred=w.WebMercator2UTM(p->predchozi->predchozi->X,p->predchozi->predchozi->Y);
////					TPointD P_dalsi_dalsi=w.WebMercator2UTM(p->dalsi->dalsi->X,p->dalsi->dalsi->Y);
////					obsah+=P_pred.x*(P_dalsi.y - P_pred_pred.y); //pro posledni
////					obsah+=P_dalsi.x*(P_dalsi_dalsi.y - P_pred.y); //pro n=1
////				}
//				p=p->dalsi;//posun na další prvek
//			}
//			p=NULL;delete p;
//		}
//		ukaz=ukaz->dalsi;//posun na další prvek
//		obsah=fabs(obsah);
//	}
//	ukaz=NULL;delete ukaz;
//	obsah=fabs(obsah/2);
//
//	TPointD ret;
//	ret.x=delka;//v m - reálné èíslo
//	ret.y=obsah;//v m2 - reálné èíslo
//	return ret;
// }
/////////////////////////////////////////////////////////////////////////////
long Cmy::LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2)
{
	double a=Y2-Y1;
	double b=X1-X2;
	double c=-a*X1-b*Y1;

  if(X2==X1)//pokud jsou v jedné "linii" tj. je abs. svislá
  {
	if((Y1<=y && y<=Y2 && Y1<Y2) || (Y2<=y && y<=Y1 && Y2<Y1))
		return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
    else
	  return 2147483647;//vrací nesmyslnì vysoké èíslo jako neúspìch (zøejmì leží na pøímce ale ne na úseèce)
  }
  else//pro ostatní situace
  {
	if(((X1<=x && x<=X2 && X1<X2) || (X2<=x && x<=X1 && X2<X1)) && ((Y1<=y && y<=Y2 && Y1<Y2) || (Y2<=y && y<=Y1 && Y2<Y1)))
		return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
	else
		return 2147483647;//vrací nesmyslnì vysoké èíslo jako neúspìch (zøejmì leží na pøímce ale ne na úseèce)
	}
}
/////////////////////////////////////////////////////////////////////////////
//funkce ovìøí, zda se bod nachází v zadaném kruhu
bool Cmy::PtInCircle(double point_X,double point_Y,double center_X,double center_Y,double radius)
{
	return delka(point_X,point_Y,center_X,center_Y)<=radius;
}
/////////////////////////////////////////////////////////////////////////////
//funkce ovìøí, zda se bod nachází v obdelníku
bool Cmy::PtInRectangle(double X1,double Y1,double X2,double Y2,double Xmys,double Ymys)
{
	X1=abs_d(X1);Y1=abs_d(Y1);X2=abs_d(X2);Y2=abs_d(Y2);Xmys=abs_d(Xmys);Ymys=abs_d(Ymys);
	return (X1<=Xmys&&Xmys<=X2&&Y1<=Ymys&&Ymys<=Y2);
}
/////////////////////////////////////////////////////////////////////////////
//metoda ovìøí zda je bod ve vnitø obrysu èlovìka, který se nachází na daných souøadnicích
bool Cmy::PtInClovek(double X,double Y,double Xmys,double Ymys,double rotace)
{
	//funkcionalita èlovìk - vyvoøení cesty pera
	double oX=0.5665; double oY=-0.33425;//souøadnice tìžištì hlavy - uchopovací bod, offset v souøadnicích, nemusí být støedem vypoèítaným z min a max hodnot níže...
	int posledni_prvek=63;TPoint PF[64];TPointD PL[64];
	if(F->Zoom>=9){PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.782;PL[37].y=-0.5;PL[38].x=0.694;PL[38].y=-0.654;PL[39].x=0.656;PL[39].y=-0.458;PL[40].x=0.64;PL[40].y=-0.474;PL[41].x=0.63;PL[41].y=-0.478;PL[42].x=0.586;PL[42].y=-0.48;PL[43].x=0.576;PL[43].y=-0.484;PL[44].x=0.582;PL[44].y=-0.532;PL[45].x=0.538;PL[45].y=-0.48;PL[46].x=0.507;PL[46].y=-0.469;PL[47].x=0.506;PL[47].y=-0.474;PL[48].x=0.476;PL[48].y=-0.458;PL[49].x=0.476;PL[49].y=-0.56;PL[50].x=0.38;PL[50].y=-0.62;PL[51].x=0.372;PL[51].y=-0.388;PL[52].x=0.322;PL[52].y=-0.36;PL[53].x=0.326;PL[53].y=-0.368;PL[54].x=0.272;PL[54].y=-0.332;PL[55].x=0.218;PL[55].y=-0.36;PL[56].x=0.18;PL[56].y=-0.392;PL[57].x=0.122;PL[57].y=-0.378;PL[58].x=0.148;PL[58].y=-0.456;PL[59].x=0.152;PL[59].y=-0.478;PL[60].x=0.214;PL[60].y=-0.584;PL[61].x=0.242;PL[61].y=-0.63;PL[62].x=0.29;PL[62].y=-0.626;PL[63].x=0.316;PL[63].y=-0.704;}//vèetnì noh
	else {posledni_prvek=60;PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.731;PL[37].y=-0.397;PL[38].x=0.709;PL[38].y=-0.404;PL[39].x=0.691;PL[39].y=-0.407;PL[40].x=0.679;PL[40].y=-0.457;PL[41].x=0.633;PL[41].y=-0.479;PL[42].x=0.589;PL[42].y=-0.481;PL[43].x=0.579;PL[43].y=-0.485;PL[44].x=0.579;PL[44].y=-0.54;PL[45].x=0.541;PL[45].y=-0.48;PL[46].x=0.492;PL[46].y=-0.465;PL[47].x=0.439;PL[47].y=-0.452;PL[48].x=0.441;PL[48].y=-0.405;PL[49].x=0.372;PL[49].y=-0.389;PL[50].x=0.326;PL[50].y=-0.368;PL[51].x=0.272;PL[51].y=-0.332;PL[52].x=0.218;PL[52].y=-0.36;PL[53].x=0.18;PL[53].y=-0.392;PL[54].x=0.122;PL[54].y=-0.378;PL[55].x=0.148;PL[55].y=-0.456;PL[56].x=0.152;PL[56].y=-0.478;PL[57].x=0.214;PL[57].y=-0.584;PL[58].x=0.242;PL[58].y=-0.63;PL[59].x=0.29;PL[59].y=-0.626;PL[60].x=0.316;PL[60].y=-0.704;}
	rotace_polygon(oX,oY,X,Y,PL,PF,posledni_prvek,rotace);//orotuje a rovnou pøevede do fyzických souøadnic nahrazuje pùvodní: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=L2PxX+m2px(PL[i].x-oX);PF2[i].y=L2PyY+m2px(oY-PL[i].y);}
	//uzavøení do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF,posledni_prvek);
	EndPath(F->Canvas->Handle);
	//testování finální citelné oblasti
	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//metoda ovìøí zda je bod ve vnitø obrysu ION tyèí, který se nachází na daných souøadnicích
bool Cmy::PtInIon(double X,double Y,double Xmys,double Ymys,double rotace)
{
	double oX=0.5;double oY=-1;//støed offset
	int posledni_prvek=21;TPointD PL[22];
	//jedna kružnice
	TPoint PF1[22];
	PL[0].x=0.5;PL[0].y=-1.86333333333333;PL[1].x=0.65;PL[1].y=-1.88666666666667;PL[2].x=0.846666666666667;PL[2].y=-1.62666666666667;PL[3].x=0.596666666666667;PL[3].y=-1.45666666666667;PL[4].x=0.58;PL[4].y=-1.38333333333333;PL[5].x=0.576666666666667;PL[5].y=-1.37333333333333;PL[6].x=0.56;PL[6].y=-1.3;PL[7].x=0.628333333333333;PL[7].y=-1.22;PL[8].x=0.628333333333333;PL[8].y=-1.22333333333333;PL[9].x=0.696666666666667;PL[9].y=-1.14333333333333;PL[10].x=0.498333333333333;PL[10].y=-1.14166666666667;PL[11].x=0.501666666666667;PL[11].y=-1.14833333333333;PL[12].x=0.3;PL[12].y=-1.14333333333333;PL[13].x=0.366666666666667;PL[13].y=-1.22;PL[14].x=0.366666666666667;PL[14].y=-1.22;PL[15].x=0.433333333333333;PL[15].y=-1.29666666666667;PL[16].x=0.416666666666667;PL[16].y=-1.37333333333333;PL[17].x=0.42;PL[17].y=-1.38;PL[18].x=0.403333333333333;PL[18].y=-1.45666666666667;PL[19].x=0.26;PL[19].y=-1.53;PL[20].x=0.2;PL[20].y=-1.83333333333333;PL[21].x=0.493333333333333;PL[21].y=-1.86;
	rotace_polygon(oX,oY,X,Y,PL,PF1,posledni_prvek,rotace);//orotuje a rovnou pøevede do fyzických souøadnic nahrazuje pùvodní: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=X+m2px(PL[i].x-oX);PF2[i].y=Y+m2px(oY-PL[i].y);}
	//druhá kružnice
	TPoint PF2[22];
	PL[0].x=0.5;PL[0].y=-0.14;PL[1].x=0.65;PL[1].y=-0.116666666666666;PL[2].x=0.846666666666667;PL[2].y=-0.376666666666666;PL[3].x=0.596666666666667;PL[3].y=-0.546666666666666;PL[4].x=0.58;PL[4].y=-0.62;PL[5].x=0.576666666666667;PL[5].y=-0.63;PL[6].x=0.56;PL[6].y=-0.703333333333333;PL[7].x=0.628333333333333;PL[7].y=-0.783333333333333;PL[8].x=0.628333333333333;PL[8].y=-0.78;PL[9].x=0.696666666666667;PL[9].y=-0.86;PL[10].x=0.498333333333333;PL[10].y=-0.861666666666666;PL[11].x=0.501666666666667;PL[11].y=-0.855;PL[12].x=0.3;PL[12].y=-0.86;PL[13].x=0.366666666666667;PL[13].y=-0.783333333333333;PL[14].x=0.366666666666667;PL[14].y=-0.783333333333333;PL[15].x=0.433333333333333;PL[15].y=-0.706666666666667;PL[16].x=0.416666666666667;PL[16].y=-0.63;PL[17].x=0.42;PL[17].y=-0.623333333333333;PL[18].x=0.403333333333333;PL[18].y=-0.546666666666666;PL[19].x=0.26;PL[19].y=-0.473333333333333;PL[20].x=0.21;PL[20].y=-0.156666666666667;PL[21].x=0.493333333333333;PL[21].y=-0.143333333333333;
	rotace_polygon(oX,oY,X,Y,PL,PF2,posledni_prvek,rotace);//orotuje a rovnou pøevede do fyzických souøadnic nahrazuje pùvodní:
	//uzavøení do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF1,posledni_prvek);
	F->Canvas->PolyBezier(PF2,posledni_prvek);
	EndPath(F->Canvas->Handle);
	//testování finální citelné oblasti
	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
TPointDbool Cmy::zkratit_polygon_na_roztec(double d, double r,double xp, double yp, double x0, double y0, double x1, double y1)//d - delka linky,r - roztec palcuxp, yp - souradnice oznaceneho bodu x0, y0, x1, y1- souradnice sousedu k oznacenemu bodu
{
		TPointDbool RET;
		// zkratit linku o z, d je delka linky, r je roztec zubu
		double z=fmod(d,r);

		// kontrola vhodnosti vybraneho bodu
		double d0=sqrt(pow((x0-xp),2)+pow((y0-yp),2));
		double d1=sqrt(pow((x1-xp),2)+pow((y1-yp),2));
		double dt=sqrt(pow((x0-x1),2)+pow((y0-y1),2));
		double sq=d0+d1-dt-z;

		if (sq<0)
			RET.b=false; //pokud nebude splnena trojuhelnikova nerovnost, nemuze se dany bod pouzit
		else
		{
			double d0n=((d0+d1-z)*d0)/(d0+d1); // prepocitani ocekavanych delek
			double d1n=((d0+d1-z)*d1)/(d0+d1);

			double m=((pow(d0n,2)-pow(d1n,2))/2*dt)+(dt/2); // prusecik pocitan metodikou popsanou na wikibooks(prusecik dvou kruznic)
			ShowMessage(pow(d0n,2)); ShowMessage(pow(m,2));
			double v=sqrt(fabs(pow(d0n,2)-pow(m,2)));// dela se to trojuhelnikama  <--DOPLNÌNA ABSOLUTNÍ HODNOTA PRO REÁLNÁ ÈÍSLA

			double xs=x0+(m/dt)*(x1-x0); // vypocet pruseciku spojnice bodu s osou
			double ys=y0+(m/dt)*(y1-y0);

			double xn1=xs-(v/dt)*(y0-y1); // mozne souradnice novych bodu
			double yn1=ys+(v/dt)*(x0-x1);
			double xn2=xs+(v/dt)*(y0-y1);
			double yn2=ys-(v/dt)*(x0-x1);

			double dist1=sqrt(pow((xn1-xp),2)+pow((yn1-yp),2)); // nalezeni blizsiho bodu
			double dist2=sqrt(pow((xn2-xp),2)+pow((yn2-yp),2));

      double xn,yn;

			if (dist1<dist2)
			{
				xn=xn1;
				yn=yn1;
			}
			else
			{
				xn=xn2;
				yn=yn2;
      }

			RET.b=true; RET.x=xn; RET.y=yn;
		}

		return RET;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
double Cmy::cekani_na_palec(double cas, double roztec_palcu,double rychlost_dopravniku,int funkce)//vrátí dobu èekání na palec v sec, rozteè je v m, rychlost dopravníku v m/s
{
		//if(zohlednit && rezim!=1)//pokud se jedná o kontinuální režim neøeší se, pøedpokládá se, že jede na stejném dopravníku
		{
			//exaktní výpoèet je použitelný jenom v pøípad známe goemetrie, navíc obsahuje chybu
			//double cas_presunu_mezi_palci=(60*roztec_palcu)/(rychlost_dopravniku*100);  //to 100 je pøevod na cm z m
			//nn, neceloèíselný zbyte double zbytek_po_deleni=(cas*60/cas_presunu_mezi_palci)-floor(cas*60/cas_presunu_mezi_palci);//tzn. kde se nachází
			//return cas_presunu_mezi_palci*zbytek_po_deleni;
			//jako støední hodnota vyplývající z normálního pravdìpodonostního rozdìlení hodnot
			//(cas_presunu_mezi_palci-0)/2 resp. (max-min)/2
			//return (roztec_palcu/(rychlost_dopravniku*1000.0))/2.0; //vrátí dobu èekání na palec v min, rozteèe je v mm resp. v m za z minu u rychlosti dopravniku
		}
		//else return 0;

		//vrátí dobu èekání na palec v sec, rozteè je v m, rychlost dopravníku v m/s
		double RET=0.0;
		double MIN=0.0;double MAX=0.0; if(rychlost_dopravniku!=0)MAX=roztec_palcu/rychlost_dopravniku;
		double ZOI=0.1;if(MAX==0 || MAX<=ZOI)ZOI=0.0;//korekce pro zajištìní zprava otevøeného intervalu (nemùže být uzavøený, protože to není reálné, dochází v takové situaci ještì k uchopení pùvodním palcem), øád kokekce zvolen neexaktnì, pouze dle úvahy, pokud není MAX hodnota (napøíklad z dùvodu 0 hodnoty rozteèe) kladné èíslo vìtší než nula, tak se korekce neuvažuje, aby se nešlo do mínusu s výpoètem (tedy èekáním)
		switch(funkce)
		{
				case 0:RET=MIN;break;//nic resp minimum=0, neèeká na palec vùbec buï vyšel pøesnì nebo se nezohledòuje
				case 1:RET=(MAX-ZOI+MIN)/2.0;break;//støední hodnota (v tomto pøípadì i prùmìr) dle normálního rozdìlení pro hodnoty <0,max)
				case 2:RET=fmod(rand(),MAX*10)/10.0+MIN;break;//náhodná hodnota v rozmezí <0,max) èekání na palce, zde ZOI není nutné zohledòovat, protože již vyplývá z použitého algoritmu
				case 3:RET=MAX-ZOI;break;//max.možná hodnota èekání na pale
				case 4:/*RET=tady bude exaktní výpoèet pro geometrii*/break;
		}
		return RET;
}
/////////////////////////////////////////////////////////////////////////////
//metoda vratí minimální možnou mezeru mezi vozíky, pokud je parametr mezera roven 0, v pøípadì nenulového parametru mezery vrací vhodnou nejbližší hodnotu této mezery vùèi rozmìrùm rozteè a rozmìr vozíku, pokud nebude zadaná rozteè tj. bude 0, vrací hodnotu 0, lze parametrizovat vracený výsledek 0 (implicitnì) - kritická mezera, 1 èi 281 - mezera mezi JIG, 2 èi 282 mezera mezi PODVOZKY
double Cmy::mezera_mezi_voziky(double dJ,double sJ,double rotace,double roztec,double mezera,unsigned short typ)
{
	//požadovaná hodnota
	double DV=UDV(dJ,sJ,rotace);//kritická mezera
	double RET=0.0;

	if(roztec!=0)
	{
		double min_mezera=ceil(DV/roztec)*roztec-DV;//vrátí minimální možnou mezi vozíky, nepokrácené: ceil((vozik/2+vozik/2)/roztec]*roztec-vozik
		if(mezera==0)
		{
			RET=min_mezera;
		}
		if(mezera!=0)//pokud by bylo nula, tak by vrátilo hodnotu min_mezera
		{
			if(min_mezera>=mezera || mezera<0)RET=min_mezera;//pokud je minimální možná mezera menší než nebo stejná jak zadaná, pøípadnì chybnì zadaná v podobì záporného èísla tak vrátí minimální možnou resp. zadanou v pøípadì ==
			else {RET=min_mezera+round((mezera-min_mezera)/roztec)*roztec;}//vratí nejbližší možnou mezeru mezi vozíky
		}
		//pokud je požadovaná mezi jigy èi vozíky
		bool JMK=false; if(UDV(dJ,sJ,rotace)==UDJ(dJ,sJ,rotace))JMK=true;//mezera mezi JIGy je kritická
		switch(typ)
		{
			//jig
			case 1:
			case 251: if(JMK)RET=min_mezera;else RET=min_mezera+F->d.v.PP.delka_podvozek-UDJ(dJ,sJ,rotace);break;
			//podvozek
			case 2:
			case 252:	if(!JMK)RET=min_mezera;else RET=min_mezera+F->d.v.PP.delka_podvozek-UDJ(dJ,sJ,rotace);break;
		}
		return RET;
	}
	else return 0;//pokud nebude známa rozteè
}
////////////////////////
//vrátí mezeru dle rozestupu v palcích a rozteèe a velikosti vozíku dle rotace, typ==-1 vrátí velikost mezery automaticky co je kritištìjší, zda podovzek èi jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double dJ,double sJ,double rotace,double Rx,double R,short typ)
{
	switch(typ)
	{
		case  0: return (Rx*R)-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return (Rx*R)-UDJ(dJ,sJ,rotace);//mezi JIGy
		default: return (Rx*R)-UDV(dJ,sJ,rotace);//automaticky co je kritiètìjší
	}
}
////////////////////////
//vrátí mezeru dle rozestupu a rotace (resp. velikosti vozíku spoèítané dle rotace), typ==-1 vrátí velikost mezery automaticky co je kritištìjší, zda podovzek èi jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double rotace,double Rz,short typ)
{
	switch(typ)
	{
		case  0: return Rz-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return Rz-UDJ(rotace);//mezi JIGy
		default: return Rz-UDV(rotace);//automaticky co je kritiètìjší
	}
}
/////////////////////////////////////////////////////////////////////////////
//vrátí velikokost minimální mezery tak, aby se stíhálo èekání na palce a vozíky se nesrazily
double Cmy::minM(double RD1,double RD2,double R2)
{
	double M=0.0;
	double maxTcek=cekani_na_palec(0,R2,RD2,3);//R2/RD2-ZOI;//maximální doba èekání na uchycení na palec
	if(RD2>=RD1)M=maxTcek*RD1;
	else M=maxTcek/(2*(1/RD1-1/RD2));
	return M;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správnì mezera
double Cmy::Rz(double dJ,double sJ,double rotace,double M)
{
	return M+UDV(dJ,sJ,rotace);
}
////////////////////////
//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k RD (a resp. TT)
double Cmy::Rz(double RD)
{
	return RD*F->d.v.PP.TT;
}
////////////////////////
//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k Rx a R
double Cmy::Rz(double Rx, double R)
{
	return Rx*R;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozestup v poètech palcù mezi aktivními palci, byla-li zadáná správnì mezera
double Cmy::Rx(double dJ,double sJ,double rotace,double M,double R)
{
	if(R==0)return 0;//nebyla definováná rozteè palcù, tudíž se nepracuje s palci
	else return (M+UDV(dJ,sJ,rotace))/R;
}
////////////////////////
//vrátí rozestup v poètech palcù mezi aktivními palci z RD a R (a resp. TT)
double Cmy::Rx(double RD,double R)
{
	return Rx2(Rz(RD),R);
}
////////////////////////
//vrátí rozestup v poètech palcù mezi aktivními palci z Rz a R
double Cmy::Rx2(double Rz,double R)
{
	if(R==0) 0;//nebyla definováná rozteè palcù, tudíž se nepracuje s palci
	else return Rz/R;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozteèe dle rozestupu a poètu palcù
double Cmy::R(double Rz,double Rx)
{
	return Rz/Rx;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí RD dle rozestupu v metrech mezi aktivními palci v souvstažnosti s TT
double Cmy::RD(double Rz)
{
	return Rz/Form1->d.v.PP.TT;
}
/////////////////////////////////////////////////////////////////////////////
//vratí RD dle velikosti lakovacího okna a procesního èasu
double Cmy::RD(double LO,double PT)
{
	if(PT==0)return 0; else return LO/PT;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí PT dle velikosti lakovacícho okna a aktuální rychlosti pohonu
double Cmy::PT(double LO,double RD)
{
	if(RD==0)return 0; else return LO/RD;
}
////////////////////////
//dle aktuálního RD a délky otoè vrátí PTo
double Cmy::PTo(double Dotoc,double RD)
{
	if(RD==0)return 0; else return Dotoc/RD;
}
////////////////////////
//dle aktuálního RD a èasu otáèení otoèe vrátí délku otáèení
double Cmy::Dotoc(double PTo,double RD)
{
	return PTo*RD;
}
////////////////////////
//vratí hodnotu RT (reserve time), ta mùže být i záporná, WT èekání na palac si dopoèítává metoda sama, pokud WT==-1, pokud je dosazena kladná hodnota vìtší než 0, tak je ta uvažovaná jako WT, 0 hodnota znamena WT èekání na palec neuvažovat
double Cmy::RT(double PT,double delka_prejezdu,double RD,double R,double WT)
{
	if(RD!=0)
	{
		if(WT==-1)WT=cekani_na_palec(0,R,RD,3);
		return F->d.v.PP.TT-(delka_prejezdu/RD+PT+WT);
	}
	else return 0;
}
/////////////////////////////////////////////////////////////////////////////
//vratí RD dle délky otoèe a èasu otáèení
double Cmy::RDo(double Dotoc,double PTo)
{
	if(PTo==0)return 0; else return Dotoc/PTo;
}
////////////////////////
//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
double Cmy::dopRD(double dJ,double sJ,double rotace,double R,double TT, double RD)
{
	if(TT==0)return 0;
	else
	{
		double DV=UDV(dJ,sJ,rotace);
		return (DV+mezera_mezi_voziky(dJ,sJ,rotace,R,TT*RD-DV))/TT;
	}
}
/////////////////////////////////////////////////////////////////////////////
//vrací true pokud nová rozteè (R) ovlivní Rz resp RD
bool Cmy::kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove)
{
	if(fmod(R_puvodni,R_nove))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku vozíku
double Cmy::UDV(double dJ,double sJ,double rotace)
{
	//postupnì rozšíøit o výpoèet dle zadaných stupòù nejenom 0 vs. 90
	double DV=UDJ(dJ,sJ,rotace);
	if(DV<F->d.v.PP.delka_podvozek)DV=F->d.v.PP.delka_podvozek;
	return DV;
}
/////////////////////////////////////////////////////////////////////////////
//vratí užitnou délku vozíku, parametry bere z PP
double Cmy::UDV(double rotace)
{
	return UDV(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);//je to takto v poøádku
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku jigu
double Cmy::UDJ(double dJ,double sJ,double rotace)
{
	//postupnì rozšíøit o výpoèet dle zadaných stupòù nejenom 0 vs. 90
	if(rotace==0)return dJ;//delka voziku
	else return sJ;// šíøka vozíku
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku jigu, parametry bere z PP
double Cmy::UDJ(double rotace)
{
	return UDJ(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);
}
/////////////////////////////////////////////////////////////////////////////
//vrátí, zda je možné orotovat jig tak, aby nemìlo vliv na zmìnu Rz, Rx, RD
bool Cmy::lze_rotovat_jig_bez_zmeny_RzRxRD(double mezera,double akt_rotace)
{
	bool RET=false;
	if(akt_rotace==0)
	{
		if(UDV(90)-UDV(0)<=mezera)RET=true;
	}
	else
	{
		if(UDV(0)-UDV(90)<=mezera)RET=true;
	}
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí èas pøejezdu vozíku
double Cmy::prejezd_voziku(double delka, double rychlost_dopravniku)
{
	if(rychlost_dopravniku==0 || delka==0)return 0;//pozor mùže být zavadìjící
	else return delka/rychlost_dopravniku;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí požadovanou rychlost pøejezdu, umí si dopoèítat MT, není-li dodáno, pokud vyjde záporná rychlost tzn. nestíhá
double Cmy::prejezd_voziku_rychlost(double CT,double MT,double PT,double WT,double DD)
{
	if(MT==0)//pokud není MT dodáno je nutné jej spoèítat, pokud nebude vyèísleno PT a WT (což mùže být vhodné na objekty v režimu PP), bude MT totožné s CT, bude tedy splnìna alespoò minumální nutná (nikoliv dostatèující) podmínka, kdy DD/CT>=aRD
	{
		MT=CT-PT-WT;
	}
	if(MT==0) return 0;//pouze ošetøení dìlení nulou
	else return DD/MT;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozdíl aktuální rychlosti pohonu a potøebné k uskuteèní pøejezdu, pokud je hodnota 0 je v poøádku, je-li záporná, pøejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
double Cmy::kontrola_rychlosti_prejezdu(double CT,double MT,double PT,double WT,double DD,double aRD)
{
	return aRD-prejezd_voziku_rychlost(CT,MT,PT,WT,DD);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//zesvìtlí nebo ztmaví barvu
TColor Cmy::clIntensive(TColor C,short A)//+A - míra zesvìtlení,-A míra ztmavení
{
	BYTE R=GetRValue(C); BYTE G=GetGValue(C); BYTE B=GetBValue(C);
	if(A>0)//zesvìtlení
	{
		if(A>255-R)R=255;else R+=A;
		if(A>255-G)G=255;else G+=A;
		if(A>255-B)B=255;else B+=A;
	}
	if(A<0)//ztmavení
	{
		if((-1)*A>R)R=0;else R+=A;
		if((-1)*A>G)G=0;else G+=A;
		if((-1)*A>B)B=0;else B+=A;
	}
	return (TColor)RGB(R,G,B);
}
/////////////////////////////////////////////////////////////////////////////
//nastaví horizontální a vertikální pozici tlaèítka a také designové vlasnosti podle tlaèítkek Ano, Uložit, OK, Storno dle MyMessageBox
void Cmy::designButton(TscGPButton *button,TForm *form,short rank,short sum,short horizontal_space,short vertikal_space)
{
	//horizontální pozice
	switch(sum)
	{
		//celkem jedno tlaèítko
		case 1:button->Left=form->Width/2-button->Width/2;break;//na støed
		//celkem dvì tlaèítka
		case 2:
		{
			if(rank==1)button->Left=form->Width/2-button->Width-horizontal_space/2; //první tlaèítko
			else button->Left=form->Width/2+horizontal_space/2;//druhé tlaèítko
		}
		break;
		//celkem tøi tlaèítka
		case 3:
		{
			switch(rank)
			{
				case 1:button->Left=form->Width/2-button->Width/2-horizontal_space-button->Width; break;//první
				case 2:button->Left=form->Width/2-button->Width/2;break;//druhé na støed
				case 3:button->Left=form->Width/2+button->Width/2+horizontal_space-button->Width;break;//tøetí
			}
		}
		break;
	}

	//vertikální pozice
	button->Top=form->Height-button->Height-vertikal_space;

	//pøebírání designu z referenèních tlaèítek v MyMessageBox
	if(button->ModalResult==mrYes){*button->Options=*myMessageBox->Button_Yes->Options;button->Layout=myMessageBox->Button_Yes->Layout;}
	if(button->ModalResult==mrOk){*button->Options=*myMessageBox->Button_OK->Options;button->Layout=myMessageBox->Button_OK->Layout;}
	if(button->ModalResult==mrCancel){*button->Options=*myMessageBox->Button_Cancel->Options;button->Layout=myMessageBox->Button_Cancel->Layout;}
	if(button->ModalResult==mrNo){*button->Options=*myMessageBox->Button_No->Options;button->Layout=myMessageBox->Button_No->Layout;}
}
/////////////////////////////////////////////////////////////////////////////
//vykreslí danému oknu transparentní (kvùli možnému smazání - pøemaskování) dle zadané barvy a šíøky, nutno volat pøi formactive (lépe však pøi formpaint), pøi šíøce 1px (ta je zároveò implicitní) staèí volat, jenom pøi formactive, jinak i pøi formsize, formresize,formclose, pøíklad použití: frameForm(Form_parametry,clWebOrange,1);
void Cmy::frameForm(TForm *form,TColor color,short width)
{
	short o=floor(width/2.0);
	frameRect(TRect(form->Left-o,form->Top-o,form->Left+form->Width+o,form->Top+form->Height+o),color,width);
}
/////////////////////////////////////////////////////////////////////////////
//nakresli na libovolném místì na monitoru obdélník dle zadaných souøadnic ve formátu TRect, je možné nastavit barvu a šíøku èáry tohoto obdélníku
void Cmy::frameRect(TRect Rect,TColor color,short width)
{
	TCanvas *C=new(TCanvas);
	C->Handle=GetWindowDC(HWND_DESKTOP);
	C->Pen->Color=color;
	C->Pen->Mode=pmCopy;//pmNotXor;
	C->Pen->Width=width;
	C->Brush->Style=bsClear;
	C->Rectangle(Rect);
}
/////////////////////////////////////////////////////////////////////////////
//z rychlosti v m/s vratí èas v milisekundách (proto *1000) potøebný na pøekreslení jednoho pixelu pøi daném zoomu
//parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se minimální vhodný èas na pøehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer nehledì na rychlost
//pokud je (i implicitní) parametr speed_min==0, tzn. pøevezme se hodnota aktuálního poèítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálnì (netrhnanì), v pøípadì nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace,
//pokud by byl paremetr speed nižší než nenulový speed_min, nebude se jednat kontinuální (netrhnanou) simulaci
double Cmy::get_timePERpx(double speed,double A,double speed_min)//A je akcelerace
{
	double Z=F->Zoom;if(F->antialiasing)Z/=3.0;//pokud je spuštìn antialiasing, staèí 1/3 rychlost (vyplývá z principu algoritmu AA)
	if(A==0)//v pøípadì implicitního zrychlení je uvažováno spoèítat zrychlení vhodné k tomu, aby se animace zobrazovala kontinuálnì (netrhnanì)
	{                           //bacha pokud není speed_min 0, stejnì pøi animaci se nechovalo korektnì
		if(speed_min==0)speed_min=speed;//pokud je (i implicitní) parametr speed_min==0, tzn. pøevezme se hodnota aktuálního poèítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálnì (netrhnanì), v pøípadì nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace
		F->afps=F->fps/(1/F->m2px*speed_min);//korekce, nepokrátím výrazy v níže uvedeném vzorci, protože afps se používá na více místech a zásah by byl pøíliš komplikovaný
		return F->m2px/Z/speed*1000.0/F->afps;//pùvodnì chybná úvaha (proto korekce výše), (bez korekce) koncipováno pouze pro rychlosti 0,1 m/s (hodnota m2px) a vyšší//vrátí èas, tak aby se jednalo o kontinální animaci
	}
	else return F->m2px/Z/speed*1000.0/A;//vrátí èas na posun o jeden pixel
}
/////////////////////////////////////////////////////////////////////////////
//vrací true èi false zda se daná hodnota nachází èí nenachází v intervalu, interval mùže být uzavøený (tzn. vèetnì hodnoty hranice intervalu) nebo otevøený a to i rozdílnì pro obì meze, implicitnì jsou hranice nastaveny na uzavøený interval z obou stran, tzn. do podmínky se zahrnuje vèetnì obou hodnot
bool Cmy::between(double value,double from, double to, bool left_closed, bool right_closed)
{
	bool RET=false;
	if(left_closed && right_closed && from<=to && value>=from && value<=to)RET=true;
	if(!left_closed && right_closed && from<to && value>from && value<=to)RET=true;
	if(left_closed && !right_closed && from<to && value>=from && value<to)RET=true;
	if(!left_closed && !right_closed && from<to && value>from && value<to)RET=true;
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//zkotroluje èíslo, zda se nejadná o hodnotu okolo nuly dle možné odchylky dle toleration, pøedchází zvláštnímu chování výpoètù okolo nuly
double Cmy::null(double number,double tolerance)
{
	if(between(number,(-1)*tolerance,tolerance))return 0.0;
	else return number;
}

