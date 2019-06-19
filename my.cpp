//---------------------------------------------------------------------------
#pragma hdrstop
#include "my.h"
#include "Unit1.h"
#include "MyMessageBox.h"//kv�li referenci na tla��tka v MyMessageBox
//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////////////////////////////////////
//fce pro zaokrouhlen� realn�ho ��sla na nejbl힚� cel� ��slo
long Cmy::round(double number)
{
	if(number>0)
	return (long)(number+0.5);//pro kladn� hodnoty
	else
	return (long)(number-0.5);//pro z�porn� hodnoty
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhl� na po�et desetinn�ch m�st dle precison
double Cmy::round2double(double number,unsigned short precision)
{
	double decimal=number-floor(number);//nutno takto rozkl�dat, proto�e jinak by v p��pad� v�t��ho cel�ho z�kladu nesta�il double
	return number=floor(number)+round(decimal*pow(10.0,precision))/pow(10.0,precision);
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhl� na po�et desetinn�ch m�st dle precison a vrat� hodnotu pomoc� �etezce, za ��slem n�sleduje znak, dle posledn�ho parametru (nap�. dv� te�ky .. jako�e ��slo pokra�uje), pokud ��slo obsahuje re�lnou ��st nezobrazenou v r�mci precision, pokud je nastaven posledn� parametr add_decimal na true a je-li re�ln� ��st krat�� ne� po�et rea�ln�ch m�st decim�l, jsou do tohototo po�tu dopln�ny nuly
AnsiString Cmy::round2double(double number,unsigned short precision,AnsiString mark,bool add_decimal)
{
	double RET=round2double(number,precision);
	AnsiString RETT="0"; //floor mus� b�t z d�vodu porovn�v�n� kladn�ch i z�porn�ch ��sel a tak� z d�vodu �e nen� mo�n� porovn�vat double a int, ale je nutn� porovnat int vs. int
	if(floor(RET*pow(10.0,precision))!=ceil(number*pow(10.0,precision)))RETT=AnsiString(RET)+mark;//pokud ��slo obsahuje re�lnou ��st vr�t� i se znakem pokra�ov�n�
	else
	{
		if(add_decimal)RETT=F->ms.addDecimal(RET,precision);//pokud je po�adavek na dopln�n� re�ln�ch m�st do stanoven�ho po�tu
		else RETT=AnsiString(RET);
	}
	return RETT;
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhl� na n�sobky 90tky, 360� p�evede na 0�, v�t�� ne� 360� vrat� za 0�
short Cmy::Rt90(double number)
{
	short RET=round(number/90.0)*90;
	if(RET>=360)RET-=360;     //p��padn�: RET=fmod(number,360.0);
	if(RET>=360)RET=Rt90(RET);//rekurzce pro p��pady, �e se bude jedn�t o n�kolikan�sobnou rotaci �i kv�lip p�evodu z 360� na 0�
	if(RET<0)RET+=360;//pro z�porn� hodnoty
	if(RET<0)RET=Rt90(RET);//rekurzce pro p��pady, �e se bude jedn�t o n�kolikan�sobnou rotaci
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//ov���, zda dan� ��slo je cel� ��slo
bool Cmy::cele_cislo(double number)
{
  return number==floor(number);
}
/////////////////////////////////////////////////////////////////////////////
//modulo pro double hodnoty
double Cmy::mod_d(double number1,double number2)
{
	//nedota�en�: return number1-floor(number1/number2)*number2;
	return fmod(number1,number2);//toto ale tak� nefunguje pro druhou re�lnou hodnotu dob�e
}
/////////////////////////////////////////////////////////////////////////////
//fce vr�t� absolutn� hodnotu z double ��sla, mo�no nahradit moc� fabs z math
double Cmy::abs_d(double number)
{
  return fabs(number);
}
/////////////////////////////////////////////////////////////////////////////
//p�evede stupn� na radi�ny
double Cmy::ToRad(double number)
{
	return number*M_PI/180.0;
}
/////////////////////////////////////////////////////////////////////////////
//p�evede radi�ny na stupn�
double Cmy::ToDeg(double number)
{
	return number*180/M_PI;
}
//////////////////////////////////////////////////////////////////////////////
//P�evede logick� sou�adnice na fyzick� (displej za��zen�) , vraci fyzick� sou�adnice
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
//P�evede  fyzick� na logick� sou�adnice (displej za��zen�) , vraci logick� sou�adnice
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
//p�evede vzd�lenost v metrech na vzd�lenost monitoru dle konstanty m2px (metr� na pixel) a aktu�ln� hodnoty prom�nn� Zoom
long Cmy::m2px(double meter)
{
	return round(meter*F->Zoom/F->m2px);
}
/////////////////////////////////////////////////////////////////////////////
//p�evede logick� jednotky (metry) na pixely monitoru dle konstanty m2px (metr� na pixel) a aktu�ln� hodnoty prom�nn� Zoom
double Cmy::px2m(long px)
{
	return px/F->Zoom*F->m2px;
}
/////////////////////////////////////////////////////////////////////////////
//dle zm��en�ho �seku na monitoru p�evede tuto pixelovou vz�dlenost a hodnotu v podob� u�ivatelsk�ho vstupu v metrech na hodnotu rozli�en� podkladu
long double Cmy::getResolution(int puvX,int puvY,int aktX,int aktY,double metry)
{
	return metry/delka(P2Lx(puvX),P2Ly(puvY),P2Lx(aktX),P2Ly(aktY))*F->m2px;//v�po�et metry d�leno po�et PX, v�choz� zobrazen� v nativn�m rozli�en� (bez usazen� do metrick�ho m���tka) je 0.1
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
	double Uhel=fmod(akt_uhel+rotace,360.0);// v�etn� o�et�en� p�ete�en� p�es 360 stup��
	if(Uhel<0){Uhel+=360;}//pro z�porn� hodnoty

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
//vr�t� sou�adnice X2,Y2 po dan�m �hlu rotace, rotuje okolo X1,Y1 proti sm�ru hodinov�ch ru�i�ek
TPointD Cmy::rotace(double X1,double Y1,double X2,double Y2,double uhel)
{
	TPointD RET=rotace(delka(X1,Y1,X2,Y2),180-azimut(X1,Y1,X2,Y2),uhel);
	RET.x+=X1;RET.y+=Y1;
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� sou�adnice X2,Y2 po dan�m �hlu rotace, rotuje okolo X1,Y1 proti sm�ru hodinov�ch ru�i�ek
TPoint Cmy::rotace_px(long X1,long Y1,long X2,long Y2,double uhel)
{
	TPointD RETd=rotace(P2Lx(X1),P2Ly(Y1),P2Lx(X2),P2Ly(Y2),uhel);
	TPoint RET;RET.x=L2Px(RETd.x);RET.y=L2Py(RETd.y);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//orotuje okolo dan�ho bodu polygon
void Cmy::rotace_polygon(double X,double Y,TPointD *POLE,long posledni_prvek,double uhel)
{
	if(fmod(uhel,360)!=0)//pouze akceler�tor
	{
		for(long i=0;i<=posledni_prvek;i++)POLE[i]=rotace(X,Y,POLE[i].x,POLE[i].y,uhel);
	}
}
/////////////////////////////////////////////////////////////////////////////
//orotuje okolo dan�ho bodu polygon a vr�t� do POLE i do POLE_px
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
//	while (ukaz!=NULL)//pro v�echny linie
//	{
//		if(ukaz->n>0)//vynech� hlavi�ku
//		{
//			pocet++;
//			Cvektory::TBod_LP *p;
//			p=ukaz->prvky;
//			while (p->dalsi!=NULL)//pro konkretni linii
//			{
//				TPointD P=w.WebMercator2UTM(p->X,p->Y);
//				TPointD P_dalsi=w.WebMercator2UTM(p->dalsi->X,p->dalsi->Y);
//				TPointD P_pred=w.WebMercator2UTM(p->predchozi->X,p->predchozi->Y);
//				if(p->n>0)//vynech� hlavi�ku
//				{
//					delka+=my.round(sqrt(pow(P_dalsi.x - P.x, 2)+ pow(P_dalsi.y - P.y, 2)));
//					if(p->n!=1)obsah+=P.x*(P_dalsi.y - P_pred.y); //V�po�et_plochy_pomoc�_L�Huillierov�ch_vzorc� v�etn� nedokon�en�ch ploch �i linii, alternativa Mascheroniho vzorec:http://klobouk.fsv.cvut.cz/~hanek/K154/PDF/Vypocet_vymer.pdf
////					if(pouze_usecka)//spo��t� azimut
////					{
////						azimut=acos((P_dalsi.y-P.y)/(delka*1.0))*180/M_PI;
////						if(P_dalsi.x<P.x)azimut=360-azimut;
////					}
//				}
////				else //zjist�, jestli obsahuje pouze dva body, tj. jestli se jedn� o �se�ku a je d�vod zm��it i azimut
////				{
////					if(p->predchozi->n==2)pouze_usecka=true;
////					TPointD P_pred_pred=w.WebMercator2UTM(p->predchozi->predchozi->X,p->predchozi->predchozi->Y);
////					TPointD P_dalsi_dalsi=w.WebMercator2UTM(p->dalsi->dalsi->X,p->dalsi->dalsi->Y);
////					obsah+=P_pred.x*(P_dalsi.y - P_pred_pred.y); //pro posledni
////					obsah+=P_dalsi.x*(P_dalsi_dalsi.y - P_pred.y); //pro n=1
////				}
//				p=p->dalsi;//posun na dal�� prvek
//			}
//			p=NULL;delete p;
//		}
//		ukaz=ukaz->dalsi;//posun na dal�� prvek
//		obsah=fabs(obsah);
//	}
//	ukaz=NULL;delete ukaz;
//	obsah=fabs(obsah/2);
//
//	TPointD ret;
//	ret.x=delka;//v m - re�ln� ��slo
//	ret.y=obsah;//v m2 - re�ln� ��slo
//	return ret;
// }
/////////////////////////////////////////////////////////////////////////////
long Cmy::LeziVblizkostiUsecky(double x, double y, double X1, double Y1, double X2, double Y2)
{
	double a=Y2-Y1;
	double b=X1-X2;
	double c=-a*X1-b*Y1;

  if(X2==X1)//pokud jsou v jedn� "linii" tj. je abs. svisl�
  {
	if((Y1<=y && y<=Y2 && Y1<Y2) || (Y2<=y && y<=Y1 && Y2<Y1))
		return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
    else
	  return 2147483647;//vrac� nesmysln� vysok� ��slo jako ne�sp�ch (z�ejm� le�� na p��mce ale ne na �se�ce)
  }
  else//pro ostatn� situace
  {
	if(((X1<=x && x<=X2 && X1<X2) || (X2<=x && x<=X1 && X2<X1)) && ((Y1<=y && y<=Y2 && Y1<Y2) || (Y2<=y && y<=Y1 && Y2<Y1)))
		return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
	else
		return 2147483647;//vrac� nesmysln� vysok� ��slo jako ne�sp�ch (z�ejm� le�� na p��mce ale ne na �se�ce)
	}
}
/////////////////////////////////////////////////////////////////////////////
//funkce ov���, zda se bod nach�z� v zadan�m kruhu
bool Cmy::PtInCircle(double point_X,double point_Y,double center_X,double center_Y,double radius)
{
	return delka(point_X,point_Y,center_X,center_Y)<=radius;
}
/////////////////////////////////////////////////////////////////////////////
//funkce ov���, zda se bod nach�z� v obdeln�ku
bool Cmy::PtInRectangle(double X1,double Y1,double X2,double Y2,double Xmys,double Ymys)
{
	X1=abs_d(X1);Y1=abs_d(Y1);X2=abs_d(X2);Y2=abs_d(Y2);Xmys=abs_d(Xmys);Ymys=abs_d(Ymys);
	return (X1<=Xmys&&Xmys<=X2&&Y1<=Ymys&&Ymys<=Y2);
}
/////////////////////////////////////////////////////////////////////////////
//metoda ov��� zda je bod ve vnit� obrysu �lov�ka, kter� se nach�z� na dan�ch sou�adnic�ch
bool Cmy::PtInClovek(double X,double Y,double Xmys,double Ymys,double rotace)
{
	//funkcionalita �lov�k - vyvo�en� cesty pera
	double oX=0.5665; double oY=-0.33425;//sou�adnice t�i�t� hlavy - uchopovac� bod, offset v sou�adnic�ch, nemus� b�t st�edem vypo��tan�m z min a max hodnot n�e...
	int posledni_prvek=63;TPoint PF[64];TPointD PL[64];
	if(F->Zoom>=9){PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.782;PL[37].y=-0.5;PL[38].x=0.694;PL[38].y=-0.654;PL[39].x=0.656;PL[39].y=-0.458;PL[40].x=0.64;PL[40].y=-0.474;PL[41].x=0.63;PL[41].y=-0.478;PL[42].x=0.586;PL[42].y=-0.48;PL[43].x=0.576;PL[43].y=-0.484;PL[44].x=0.582;PL[44].y=-0.532;PL[45].x=0.538;PL[45].y=-0.48;PL[46].x=0.507;PL[46].y=-0.469;PL[47].x=0.506;PL[47].y=-0.474;PL[48].x=0.476;PL[48].y=-0.458;PL[49].x=0.476;PL[49].y=-0.56;PL[50].x=0.38;PL[50].y=-0.62;PL[51].x=0.372;PL[51].y=-0.388;PL[52].x=0.322;PL[52].y=-0.36;PL[53].x=0.326;PL[53].y=-0.368;PL[54].x=0.272;PL[54].y=-0.332;PL[55].x=0.218;PL[55].y=-0.36;PL[56].x=0.18;PL[56].y=-0.392;PL[57].x=0.122;PL[57].y=-0.378;PL[58].x=0.148;PL[58].y=-0.456;PL[59].x=0.152;PL[59].y=-0.478;PL[60].x=0.214;PL[60].y=-0.584;PL[61].x=0.242;PL[61].y=-0.63;PL[62].x=0.29;PL[62].y=-0.626;PL[63].x=0.316;PL[63].y=-0.704;}//v�etn� noh
	else {posledni_prvek=60;PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.731;PL[37].y=-0.397;PL[38].x=0.709;PL[38].y=-0.404;PL[39].x=0.691;PL[39].y=-0.407;PL[40].x=0.679;PL[40].y=-0.457;PL[41].x=0.633;PL[41].y=-0.479;PL[42].x=0.589;PL[42].y=-0.481;PL[43].x=0.579;PL[43].y=-0.485;PL[44].x=0.579;PL[44].y=-0.54;PL[45].x=0.541;PL[45].y=-0.48;PL[46].x=0.492;PL[46].y=-0.465;PL[47].x=0.439;PL[47].y=-0.452;PL[48].x=0.441;PL[48].y=-0.405;PL[49].x=0.372;PL[49].y=-0.389;PL[50].x=0.326;PL[50].y=-0.368;PL[51].x=0.272;PL[51].y=-0.332;PL[52].x=0.218;PL[52].y=-0.36;PL[53].x=0.18;PL[53].y=-0.392;PL[54].x=0.122;PL[54].y=-0.378;PL[55].x=0.148;PL[55].y=-0.456;PL[56].x=0.152;PL[56].y=-0.478;PL[57].x=0.214;PL[57].y=-0.584;PL[58].x=0.242;PL[58].y=-0.63;PL[59].x=0.29;PL[59].y=-0.626;PL[60].x=0.316;PL[60].y=-0.704;}
	rotace_polygon(oX,oY,X,Y,PL,PF,posledni_prvek,rotace);//orotuje a rovnou p�evede do fyzick�ch sou�adnic nahrazuje p�vodn�: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=L2PxX+m2px(PL[i].x-oX);PF2[i].y=L2PyY+m2px(oY-PL[i].y);}
	//uzav�en� do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF,posledni_prvek);
	EndPath(F->Canvas->Handle);
	//testov�n� fin�ln� citeln� oblasti
	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//metoda ov��� zda je bod ve vnit� obrysu ION ty��, kter� se nach�z� na dan�ch sou�adnic�ch
bool Cmy::PtInIon(double X,double Y,double Xmys,double Ymys,double rotace)
{
	double oX=0.5;double oY=-1;//st�ed offset
	int posledni_prvek=21;TPointD PL[22];
	//jedna kru�nice
	TPoint PF1[22];
	PL[0].x=0.5;PL[0].y=-1.86333333333333;PL[1].x=0.65;PL[1].y=-1.88666666666667;PL[2].x=0.846666666666667;PL[2].y=-1.62666666666667;PL[3].x=0.596666666666667;PL[3].y=-1.45666666666667;PL[4].x=0.58;PL[4].y=-1.38333333333333;PL[5].x=0.576666666666667;PL[5].y=-1.37333333333333;PL[6].x=0.56;PL[6].y=-1.3;PL[7].x=0.628333333333333;PL[7].y=-1.22;PL[8].x=0.628333333333333;PL[8].y=-1.22333333333333;PL[9].x=0.696666666666667;PL[9].y=-1.14333333333333;PL[10].x=0.498333333333333;PL[10].y=-1.14166666666667;PL[11].x=0.501666666666667;PL[11].y=-1.14833333333333;PL[12].x=0.3;PL[12].y=-1.14333333333333;PL[13].x=0.366666666666667;PL[13].y=-1.22;PL[14].x=0.366666666666667;PL[14].y=-1.22;PL[15].x=0.433333333333333;PL[15].y=-1.29666666666667;PL[16].x=0.416666666666667;PL[16].y=-1.37333333333333;PL[17].x=0.42;PL[17].y=-1.38;PL[18].x=0.403333333333333;PL[18].y=-1.45666666666667;PL[19].x=0.26;PL[19].y=-1.53;PL[20].x=0.2;PL[20].y=-1.83333333333333;PL[21].x=0.493333333333333;PL[21].y=-1.86;
	rotace_polygon(oX,oY,X,Y,PL,PF1,posledni_prvek,rotace);//orotuje a rovnou p�evede do fyzick�ch sou�adnic nahrazuje p�vodn�: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=X+m2px(PL[i].x-oX);PF2[i].y=Y+m2px(oY-PL[i].y);}
	//druh� kru�nice
	TPoint PF2[22];
	PL[0].x=0.5;PL[0].y=-0.14;PL[1].x=0.65;PL[1].y=-0.116666666666666;PL[2].x=0.846666666666667;PL[2].y=-0.376666666666666;PL[3].x=0.596666666666667;PL[3].y=-0.546666666666666;PL[4].x=0.58;PL[4].y=-0.62;PL[5].x=0.576666666666667;PL[5].y=-0.63;PL[6].x=0.56;PL[6].y=-0.703333333333333;PL[7].x=0.628333333333333;PL[7].y=-0.783333333333333;PL[8].x=0.628333333333333;PL[8].y=-0.78;PL[9].x=0.696666666666667;PL[9].y=-0.86;PL[10].x=0.498333333333333;PL[10].y=-0.861666666666666;PL[11].x=0.501666666666667;PL[11].y=-0.855;PL[12].x=0.3;PL[12].y=-0.86;PL[13].x=0.366666666666667;PL[13].y=-0.783333333333333;PL[14].x=0.366666666666667;PL[14].y=-0.783333333333333;PL[15].x=0.433333333333333;PL[15].y=-0.706666666666667;PL[16].x=0.416666666666667;PL[16].y=-0.63;PL[17].x=0.42;PL[17].y=-0.623333333333333;PL[18].x=0.403333333333333;PL[18].y=-0.546666666666666;PL[19].x=0.26;PL[19].y=-0.473333333333333;PL[20].x=0.21;PL[20].y=-0.156666666666667;PL[21].x=0.493333333333333;PL[21].y=-0.143333333333333;
	rotace_polygon(oX,oY,X,Y,PL,PF2,posledni_prvek,rotace);//orotuje a rovnou p�evede do fyzick�ch sou�adnic nahrazuje p�vodn�:
	//uzav�en� do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF1,posledni_prvek);
	F->Canvas->PolyBezier(PF2,posledni_prvek);
	EndPath(F->Canvas->Handle);
	//testov�n� fin�ln� citeln� oblasti
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
			double v=sqrt(fabs(pow(d0n,2)-pow(m,2)));// dela se to trojuhelnikama  <--DOPLN�NA ABSOLUTN� HODNOTA PRO RE�LN� ��SLA

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
double Cmy::cekani_na_palec(double cas, double roztec_palcu,double rychlost_dopravniku,int funkce)//vr�t� dobu �ek�n� na palec v sec, rozte� je v m, rychlost dopravn�ku v m/s
{
		//if(zohlednit && rezim!=1)//pokud se jedn� o kontinu�ln� re�im ne�e�� se, p�edpokl�d� se, �e jede na stejn�m dopravn�ku
		{
			//exaktn� v�po�et je pou�iteln� jenom v p��pad zn�me goemetrie, nav�c obsahuje chybu
			//double cas_presunu_mezi_palci=(60*roztec_palcu)/(rychlost_dopravniku*100);  //to 100 je p�evod na cm z m
			//nn, necelo��seln� zbyte double zbytek_po_deleni=(cas*60/cas_presunu_mezi_palci)-floor(cas*60/cas_presunu_mezi_palci);//tzn. kde se nach�z�
			//return cas_presunu_mezi_palci*zbytek_po_deleni;
			//jako st�edn� hodnota vypl�vaj�c� z norm�ln�ho pravd�podonostn�ho rozd�len� hodnot
			//(cas_presunu_mezi_palci-0)/2 resp. (max-min)/2
			//return (roztec_palcu/(rychlost_dopravniku*1000.0))/2.0; //vr�t� dobu �ek�n� na palec v min, rozte�e je v mm resp. v m za z minu u rychlosti dopravniku
		}
		//else return 0;

		//vr�t� dobu �ek�n� na palec v sec, rozte� je v m, rychlost dopravn�ku v m/s
		double RET=0.0;
		double MIN=0.0;double MAX=0.0; if(rychlost_dopravniku!=0)MAX=roztec_palcu/rychlost_dopravniku;
		double ZOI=0.1;if(MAX==0 || MAX<=ZOI)ZOI=0.0;//korekce pro zaji�t�n� zprava otev�en�ho intervalu (nem��e b�t uzav�en�, proto�e to nen� re�ln�, doch�z� v takov� situaci je�t� k uchopen� p�vodn�m palcem), ��d kokekce zvolen neexaktn�, pouze dle �vahy, pokud nen� MAX hodnota (nap��klad z d�vodu 0 hodnoty rozte�e) kladn� ��slo v�t�� ne� nula, tak se korekce neuva�uje, aby se ne�lo do m�nusu s v�po�tem (tedy �ek�n�m)
		switch(funkce)
		{
				case 0:RET=MIN;break;//nic resp minimum=0, ne�ek� na palec v�bec bu� vy�el p�esn� nebo se nezohled�uje
				case 1:RET=(MAX-ZOI+MIN)/2.0;break;//st�edn� hodnota (v tomto p��pad� i pr�m�r) dle norm�ln�ho rozd�len� pro hodnoty <0,max)
				case 2:RET=fmod(rand(),MAX*10)/10.0+MIN;break;//n�hodn� hodnota v rozmez� <0,max) �ek�n� na palce, zde ZOI nen� nutn� zohled�ovat, proto�e ji� vypl�v� z pou�it�ho algoritmu
				case 3:RET=MAX-ZOI;break;//max.mo�n� hodnota �ek�n� na pale
				case 4:/*RET=tady bude exaktn� v�po�et pro geometrii*/break;
		}
		return RET;
}
/////////////////////////////////////////////////////////////////////////////
//metoda vrat� minim�ln� mo�nou mezeru mezi voz�ky, pokud je parametr mezera roven 0, v p��pad� nenulov�ho parametru mezery vrac� vhodnou nejbli��� hodnotu t�to mezery v��i rozm�r�m rozte� a rozm�r voz�ku, pokud nebude zadan� rozte� tj. bude 0, vrac� hodnotu 0, lze parametrizovat vracen� v�sledek 0 (implicitn�) - kritick� mezera, 1 �i 281 - mezera mezi JIG, 2 �i 282 mezera mezi PODVOZKY
double Cmy::mezera_mezi_voziky(double dJ,double sJ,double rotace,double roztec,double mezera,unsigned short typ)
{
	//po�adovan� hodnota
	double DV=UDV(dJ,sJ,rotace);//kritick� mezera
	double RET=0.0;

	if(roztec!=0)
	{
		double min_mezera=ceil(DV/roztec)*roztec-DV;//vr�t� minim�ln� mo�nou mezi voz�ky, nepokr�cen�: ceil((vozik/2+vozik/2)/roztec]*roztec-vozik
		if(mezera==0)
		{
			RET=min_mezera;
		}
		if(mezera!=0)//pokud by bylo nula, tak by vr�tilo hodnotu min_mezera
		{
			if(min_mezera>=mezera || mezera<0)RET=min_mezera;//pokud je minim�ln� mo�n� mezera men�� ne� nebo stejn� jak zadan�, p��padn� chybn� zadan� v podob� z�porn�ho ��sla tak vr�t� minim�ln� mo�nou resp. zadanou v p��pad� ==
			else {RET=min_mezera+round((mezera-min_mezera)/roztec)*roztec;}//vrat� nejbli��� mo�nou mezeru mezi voz�ky
		}
		//pokud je po�adovan� mezi jigy �i voz�ky
		bool JMK=false; if(UDV(dJ,sJ,rotace)==UDJ(dJ,sJ,rotace))JMK=true;//mezera mezi JIGy je kritick�
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
	else return 0;//pokud nebude zn�ma rozte�
}
////////////////////////
//vr�t� mezeru dle rozestupu v palc�ch a rozte�e a velikosti voz�ku dle rotace, typ==-1 vr�t� velikost mezery automaticky co je kriti�t�j��, zda podovzek �i jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double dJ,double sJ,double rotace,double Rx,double R,short typ)
{
	switch(typ)
	{
		case  0: return (Rx*R)-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return (Rx*R)-UDJ(dJ,sJ,rotace);//mezi JIGy
		default: return (Rx*R)-UDV(dJ,sJ,rotace);//automaticky co je kriti�t�j��
	}
}
////////////////////////
//vr�t� mezeru dle rozestupu a rotace (resp. velikosti voz�ku spo��tan� dle rotace), typ==-1 vr�t� velikost mezery automaticky co je kriti�t�j��, zda podovzek �i jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double rotace,double Rz,short typ)
{
	switch(typ)
	{
		case  0: return Rz-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return Rz-UDJ(rotace);//mezi JIGy
		default: return Rz-UDV(rotace);//automaticky co je kriti�t�j��
	}
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� velikokost minim�ln� mezery tak, aby se st�h�lo �ek�n� na palce a voz�ky se nesrazily
double Cmy::minM(double RD1,double RD2,double R2)
{
	double M=0.0;
	double maxTcek=cekani_na_palec(0,R2,RD2,3);//R2/RD2-ZOI;//maxim�ln� doba �ek�n� na uchycen� na palec
	if(RD2>=RD1)M=maxTcek*RD1;
	else M=maxTcek/(2*(1/RD1-1/RD2));
	return M;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozestup v metrech mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
double Cmy::Rz(double dJ,double sJ,double rotace,double M)
{
	return M+UDV(dJ,sJ,rotace);
}
////////////////////////
//vr�t� rozestup v metrech mezi aktivn�mi palci v souvsta�nosti k RD (a resp. TT)
double Cmy::Rz(double RD)
{
	return RD*F->d.v.PP.TT;
}
////////////////////////
//vr�t� rozestup v metrech mezi aktivn�mi palci v souvsta�nosti k Rx a R
double Cmy::Rz(double Rx, double R)
{
	return Rx*R;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
double Cmy::Rx(double dJ,double sJ,double rotace,double M,double R)
{
	if(R==0)return 0;//nebyla definov�n� rozte� palc�, tud� se nepracuje s palci
	else return (M+UDV(dJ,sJ,rotace))/R;
}
////////////////////////
//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci z RD a R (a resp. TT)
double Cmy::Rx(double RD,double R)
{
	return Rx2(Rz(RD),R);
}
////////////////////////
//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci z Rz a R
double Cmy::Rx2(double Rz,double R)
{
	if(R==0) 0;//nebyla definov�n� rozte� palc�, tud� se nepracuje s palci
	else return Rz/R;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozte�e dle rozestupu a po�tu palc�
double Cmy::R(double Rz,double Rx)
{
	return Rz/Rx;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� RD dle rozestupu v metrech mezi aktivn�mi palci v souvsta�nosti s TT
double Cmy::RD(double Rz)
{
	return Rz/Form1->d.v.PP.TT;
}
/////////////////////////////////////////////////////////////////////////////
//vrat� RD dle velikosti lakovac�ho okna a procesn�ho �asu
double Cmy::RD(double LO,double PT)
{
	if(PT==0)return 0; else return LO/PT;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� PT dle velikosti lakovac�cho okna a aktu�ln� rychlosti pohonu
double Cmy::PT(double LO,double RD)
{
	if(RD==0)return 0; else return LO/RD;
}
////////////////////////
//dle aktu�ln�ho RD a d�lky oto� vr�t� PTo
double Cmy::PTo(double Dotoc,double RD)
{
	if(RD==0)return 0; else return Dotoc/RD;
}
////////////////////////
//dle aktu�ln�ho RD a �asu ot��en� oto�e vr�t� d�lku ot��en�
double Cmy::Dotoc(double PTo,double RD)
{
	return PTo*RD;
}
////////////////////////
//vrat� hodnotu RT (reserve time), ta m��e b�t i z�porn�, WT �ek�n� na palac si dopo��t�v� metoda sama, pokud WT==-1, pokud je dosazena kladn� hodnota v�t�� ne� 0, tak je ta uva�ovan� jako WT, 0 hodnota znamena WT �ek�n� na palec neuva�ovat
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
//vrat� RD dle d�lky oto�e a �asu ot��en�
double Cmy::RDo(double Dotoc,double PTo)
{
	if(PTo==0)return 0; else return Dotoc/PTo;
}
////////////////////////
//vr�t� doporu�enou nejbli��� rychlost pohonu, k rychlosti zadan� tak, aby se reflektovala rozte� mezi palci i takt
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
//vrac� true pokud nov� rozte� (R) ovlivn� Rz resp RD
bool Cmy::kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove)
{
	if(fmod(R_puvodni,R_nove))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� u�itnou d�lku voz�ku
double Cmy::UDV(double dJ,double sJ,double rotace)
{
	//postupn� roz���it o v�po�et dle zadan�ch stup�� nejenom 0 vs. 90
	double DV=UDJ(dJ,sJ,rotace);
	if(DV<F->d.v.PP.delka_podvozek)DV=F->d.v.PP.delka_podvozek;
	return DV;
}
/////////////////////////////////////////////////////////////////////////////
//vrat� u�itnou d�lku voz�ku, parametry bere z PP
double Cmy::UDV(double rotace)
{
	return UDV(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);//je to takto v po��dku
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� u�itnou d�lku jigu
double Cmy::UDJ(double dJ,double sJ,double rotace)
{
	//postupn� roz���it o v�po�et dle zadan�ch stup�� nejenom 0 vs. 90
	if(rotace==0)return dJ;//delka voziku
	else return sJ;// ���ka voz�ku
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� u�itnou d�lku jigu, parametry bere z PP
double Cmy::UDJ(double rotace)
{
	return UDJ(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);
}
/////////////////////////////////////////////////////////////////////////////
//vr�t�, zda je mo�n� orotovat jig tak, aby nem�lo vliv na zm�nu Rz, Rx, RD
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
//vr�t� �as p�ejezdu voz�ku
double Cmy::prejezd_voziku(double delka, double rychlost_dopravniku)
{
	if(rychlost_dopravniku==0 || delka==0)return 0;//pozor m��e b�t zavad�j�c�
	else return delka/rychlost_dopravniku;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� po�adovanou rychlost p�ejezdu, um� si dopo��tat MT, nen�-li dod�no, pokud vyjde z�porn� rychlost tzn. nest�h�
double Cmy::prejezd_voziku_rychlost(double CT,double MT,double PT,double WT,double DD)
{
	if(MT==0)//pokud nen� MT dod�no je nutn� jej spo��tat, pokud nebude vy��sleno PT a WT (co� m��e b�t vhodn� na objekty v re�imu PP), bude MT toto�n� s CT, bude tedy spln�na alespo� minum�ln� nutn� (nikoliv dostat�uj�c�) podm�nka, kdy DD/CT>=aRD
	{
		MT=CT-PT-WT;
	}
	if(MT==0) return 0;//pouze o�et�en� d�len� nulou
	else return DD/MT;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozd�l aktu�ln� rychlosti pohonu a pot�ebn� k uskute�n� p�ejezdu, pokud je hodnota 0 je v po��dku, je-li z�porn�, p�ejezd se nest�h� o danou hodnotu v m/s, je-li kladn�, je aktu�ln� rychlost o danou hodnoutu hodnotu v m/s vy���
double Cmy::kontrola_rychlosti_prejezdu(double CT,double MT,double PT,double WT,double DD,double aRD)
{
	return aRD-prejezd_voziku_rychlost(CT,MT,PT,WT,DD);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//zesv�tl� nebo ztmav� barvu
TColor Cmy::clIntensive(TColor C,short A)//+A - m�ra zesv�tlen�,-A m�ra ztmaven�
{
	BYTE R=GetRValue(C); BYTE G=GetGValue(C); BYTE B=GetBValue(C);
	if(A>0)//zesv�tlen�
	{
		if(A>255-R)R=255;else R+=A;
		if(A>255-G)G=255;else G+=A;
		if(A>255-B)B=255;else B+=A;
	}
	if(A<0)//ztmaven�
	{
		if((-1)*A>R)R=0;else R+=A;
		if((-1)*A>G)G=0;else G+=A;
		if((-1)*A>B)B=0;else B+=A;
	}
	return (TColor)RGB(R,G,B);
}
/////////////////////////////////////////////////////////////////////////////
//nastav� horizont�ln� a vertik�ln� pozici tla��tka a tak� designov� vlasnosti podle tla��tkek Ano, Ulo�it, OK, Storno dle MyMessageBox
void Cmy::designButton(TscGPButton *button,TForm *form,short rank,short sum,short horizontal_space,short vertikal_space)
{
	//horizont�ln� pozice
	switch(sum)
	{
		//celkem jedno tla��tko
		case 1:button->Left=form->Width/2-button->Width/2;break;//na st�ed
		//celkem dv� tla��tka
		case 2:
		{
			if(rank==1)button->Left=form->Width/2-button->Width-horizontal_space/2; //prvn� tla��tko
			else button->Left=form->Width/2+horizontal_space/2;//druh� tla��tko
		}
		break;
		//celkem t�i tla��tka
		case 3:
		{
			switch(rank)
			{
				case 1:button->Left=form->Width/2-button->Width/2-horizontal_space-button->Width; break;//prvn�
				case 2:button->Left=form->Width/2-button->Width/2;break;//druh� na st�ed
				case 3:button->Left=form->Width/2+button->Width/2+horizontal_space-button->Width;break;//t�et�
			}
		}
		break;
	}

	//vertik�ln� pozice
	button->Top=form->Height-button->Height-vertikal_space;

	//p�eb�r�n� designu z referen�n�ch tla��tek v MyMessageBox
	if(button->ModalResult==mrYes){*button->Options=*myMessageBox->Button_Yes->Options;button->Layout=myMessageBox->Button_Yes->Layout;}
	if(button->ModalResult==mrOk){*button->Options=*myMessageBox->Button_OK->Options;button->Layout=myMessageBox->Button_OK->Layout;}
	if(button->ModalResult==mrCancel){*button->Options=*myMessageBox->Button_Cancel->Options;button->Layout=myMessageBox->Button_Cancel->Layout;}
	if(button->ModalResult==mrNo){*button->Options=*myMessageBox->Button_No->Options;button->Layout=myMessageBox->Button_No->Layout;}
}
/////////////////////////////////////////////////////////////////////////////
//vykresl� dan�mu oknu transparentn� (kv�li mo�n�mu smaz�n� - p�emaskov�n�) dle zadan� barvy a ���ky, nutno volat p�i formactive (l�pe v�ak p�i formpaint), p�i ���ce 1px (ta je z�rove� implicitn�) sta�� volat, jenom p�i formactive, jinak i p�i formsize, formresize,formclose, p��klad pou�it�: frameForm(Form_parametry,clWebOrange,1);
void Cmy::frameForm(TForm *form,TColor color,short width)
{
	short o=floor(width/2.0);
	frameRect(TRect(form->Left-o,form->Top-o,form->Left+form->Width+o,form->Top+form->Height+o),color,width);
}
/////////////////////////////////////////////////////////////////////////////
//nakresli na libovoln�m m�st� na monitoru obd�ln�k dle zadan�ch sou�adnic ve form�tu TRect, je mo�n� nastavit barvu a ���ku ��ry tohoto obd�ln�ku
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
//z rychlosti v m/s vrat� �as v milisekund�ch (proto *1000) pot�ebn� na p�ekreslen� jednoho pixelu p�i dan�m zoomu
//parametr A=je rychlost animace, kdy implicitn� 1 origin�ln� rychlost - tedy 100%, pokud je parametr A=0, vr�t� se minim�ln� vhodn� �as na p�ehr�n� kontinu�ln� animace, metoda je vhodn� na animace a simulace pro timer nehled� na rychlost
//pokud je (i implicitn�) parametr speed_min==0, tzn. p�evezme se hodnota aktu�ln�ho po��tan�ho RD, tzn. v�echny animace se prom�tnou se stejn�m afps dle fps, tj. v�echny animace se zobraz� kontinu�ln� (netrhnan�), v p��pad� nenulov� hodnoty je speed_min stanovane jako minim�ln� mo�n� rychlost pro zobrazen� kontinu�ln� (netrhnan�) simulace,
//pokud by byl paremetr speed ni��� ne� nenulov� speed_min, nebude se jednat kontinu�ln� (netrhnanou) simulaci
double Cmy::get_timePERpx(double speed,double A,double speed_min)//A je akcelerace
{
	double Z=F->Zoom;if(F->antialiasing)Z/=3.0;//pokud je spu�t�n antialiasing, sta�� 1/3 rychlost (vypl�v� z principu algoritmu AA)
	if(A==0)//v p��pad� implicitn�ho zrychlen� je uva�ov�no spo��tat zrychlen� vhodn� k tomu, aby se animace zobrazovala kontinu�ln� (netrhnan�)
	{                           //bacha pokud nen� speed_min 0, stejn� p�i animaci se nechovalo korektn�
		if(speed_min==0)speed_min=speed;//pokud je (i implicitn�) parametr speed_min==0, tzn. p�evezme se hodnota aktu�ln�ho po��tan�ho RD, tzn. v�echny animace se prom�tnou se stejn�m afps dle fps, tj. v�echny animace se zobraz� kontinu�ln� (netrhnan�), v p��pad� nenulov� hodnoty je speed_min stanovane jako minim�ln� mo�n� rychlost pro zobrazen� kontinu�ln� (netrhnan�) simulace
		F->afps=F->fps/(1/F->m2px*speed_min);//korekce, nepokr�t�m v�razy v n�e uveden�m vzorci, proto�e afps se pou��v� na v�ce m�stech a z�sah by byl p��li� komplikovan�
		return F->m2px/Z/speed*1000.0/F->afps;//p�vodn� chybn� �vaha (proto korekce v��e), (bez korekce) koncipov�no pouze pro rychlosti 0,1 m/s (hodnota m2px) a vy���//vr�t� �as, tak aby se jednalo o kontin�ln� animaci
	}
	else return F->m2px/Z/speed*1000.0/A;//vr�t� �as na posun o jeden pixel
}
/////////////////////////////////////////////////////////////////////////////
//vrac� true �i false zda se dan� hodnota nach�z� �� nenach�z� v intervalu, interval m��e b�t uzav�en� (tzn. v�etn� hodnoty hranice intervalu) nebo otev�en� a to i rozd�ln� pro ob� meze, implicitn� jsou hranice nastaveny na uzav�en� interval z obou stran, tzn. do podm�nky se zahrnuje v�etn� obou hodnot
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
//zkotroluje ��slo, zda se nejadn� o hodnotu okolo nuly dle mo�n� odchylky dle toleration, p�edch�z� zvl�tn�mu chov�n� v�po�t� okolo nuly
double Cmy::null(double number,double tolerance)
{
	if(between(number,(-1)*tolerance,tolerance))return 0.0;
	else return number;
}

