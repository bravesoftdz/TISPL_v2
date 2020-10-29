//---------------------------------------------------------------------------
#pragma hdrstop
#include "my.h"
#include "Unit1.h"
#include "MyMessageBox.h"//kvůli referenci na tlačítka v MyMessageBox
//---------------------------------------------------------------------------
#pragma package(smart_init)
/////////////////////////////////////////////////////////////////////////////
//fce pro zaokrouhlení realného čísla na nejblížší celé číslo
long Cmy::round(double number)
{
	if(number>0)
	return (long)(number+0.5);//pro kladné hodnoty
	else
	return (long)(number-0.5);//pro záporné hodnoty
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhlí na počet desetinných míst dle precison
double Cmy::round2double(double number,unsigned short precision)
{
	double decimal=number-floor(number);//nutno takto rozkládat, protože jinak by v případě většího celého základu nestačil double
	return number=floor(number)+round(decimal*pow(10.0,precision))/pow(10.0,precision);
}
/////////////////////////////////////////////////////////////////////////////
//zaokrouhlí na počet desetinných míst dle precison a vratí hodnotu pomocí řetezce, za číslem následuje znak, dle posledního parametru (např. dvě tečky .. jakože číslo pokračuje), pokud číslo obsahuje reálnou část nezobrazenou v rámci precision, pokud je nastaven poslední parametr add_decimal na true a je-li reálná část kratší než počet reaálných míst decimál, jsou do tohototo počtu doplněny nuly
AnsiString Cmy::round2double(double number,unsigned short precision,AnsiString mark,bool add_decimal)
{
	double RET=round2double(number,precision);
	AnsiString RETT="0"; //floor musí být z důvodu porovnávání kladných i záporných čísel a také z důvodu že není možné porovnávat double a int, ale je nutné porovnat int vs. int
	if(floor(RET*pow(10.0,precision))!=ceil(number*pow(10.0,precision)))RETT=AnsiString(RET)+mark;//pokud číslo obsahuje reálnou část vrátí i se znakem pokračování
	else
	{
		if(add_decimal)RETT=F->ms.addDecimal(RET,precision);//pokud je požadavek na doplnění reálných míst do stanoveného počtu
		else RETT=AnsiString(RET);
	}
	return RETT;
}
/////////////////////////////////////////////////////////////////////////////
//ověří, zda dané číslo je celé číslo
bool Cmy::cele_cislo(double number)
{
  return number==floor(number);
}
/////////////////////////////////////////////////////////////////////////////
//modulo pro double hodnoty
double Cmy::mod_d(double number1,double number2)
{
	//nedotažené: return number1-floor(number1/number2)*number2;
	return fmod(number1,number2);//toto ale také nefunguje pro druhou reálnou hodnotu dobře
}
/////////////////////////////////////////////////////////////////////////////
//fce vrátí absolutní hodnotu z double čísla, možno nahradit mocí fabs z math
double Cmy::abs_d(double number)
{
  return fabs(number);
}
/////////////////////////////////////////////////////////////////////////////
//převede stupně na radiány
double Cmy::ToRad(double number)
{
	return number*M_PI/180.0;
}
/////////////////////////////////////////////////////////////////////////////
//převede radiány na stupně
double Cmy::ToDeg(double number)
{
	return number*180/M_PI;
}
/////////////////////////////////////////////////////////////////////////////
//převede na násobky 90tky, 360° převede na 0°, větší než 360° vratí za 0°
short Cmy::Rt90(double number)
{
	short RET=round(number/90.0)*90;
	if(RET>=360)RET-=360;     //případně: RET=fmod(number,360.0);
	if(RET>=360)RET=Rt90(RET);//rekurzce pro případy, že se bude jednát o několikanásobnou rotaci či kvůlip převodu z 360° na 0°
	if(RET<0)RET+=360;//pro záporné hodnoty
	if(RET<0)RET=Rt90(RET);//rekurzce pro případy, že se bude jednát o několikanásobnou rotaci
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//záporné stupně převede do kladných v rámci 360°
double Cmy::a360(double number)
{
	number=fmod(number,360.0);//ošetření přetečení přes 360 stupňů
	if(number<0)number+=360;//pro záporné hodnoty
	//if(number<0)number=a360(number);//rekurzce pro případy, že se bude jednát o několikanásobnou rotaci - již není třeba a nahrazuje i pro kladné přetečení přes 360 první řádek
	return number;
}
/////////////////////////////////////////////////////////////////////////////
//převede hodnotu rotace (proti směru hodinových ručiček a vpravo naležato jako výchozí pozice) na orientaci dle světových stran
double Cmy::r2o(double number)
{
	return a360(360-number+90);
}
//převede hodnotu orientace dle světových stran na hodnotu rotace (proti směru hodinových ručiček a vpravo naležato jako výchozí pozice)
double Cmy::o2r(double number)
{
	return r2o(number);
}
/////////////////////////////////////////////////////////////////////////////
//ze zadaného radiusu vrátí obvod kruhové výseče o velikosti definované úhlem výseče ve stupních
double Cmy::R2Larc(double radius,double angle)
{
	return radius*2*M_PI*a360(fabs(angle))/360.0;
}
/////////////////////////////////////////////////////////////////////////////
//ze zadaného radiusu a délky tětivy vrátí úhel kruhové výseče ve stupních
double Cmy::T2Aarc(double radius,double t_lenght)
{
	return ToDeg(2*ArcSin(t_lenght/(radius*2)));
}
//////////////////////////////////////////////////////////////////////////////
//ze tří zadaných stran trojúhlelníku vrátí úhel ve stupních dle parametru p 1-alfa,2-beta,3-gama
double Cmy::AngleFromTriangle(double a,double b,double c,short p)
{
	double RET=0;
	switch(p)
	{
		case 1:RET=ToDeg(ArcCos((b*b+c*c-a*a)/(2*b*c)));break;//ALFA
		case 2:RET=ToDeg(ArcCos((a*a+c*c-b*b)/(2*a*c)));break;//BETA
		case 3:RET=ToDeg(ArcCos((a*a+b*b-c*c)/(2*a*b)));break;//GAMA
	}
	return RET;
}
//////////////////////////////////////////////////////////////////////////////
//vrátí hodnotu úhlu části oblouku, který svírá výchozí bod oblouku, střed pomyslné kružnice oblouku a přímka procházejí tímto středem a souřadnicemi myši, pozor, neřeší přetečení přes oblast, nebo kurzor myši zcela mimo oblast
double Cmy::uhelObloukuVsMys(double Xoblouk,double Yoblouk,double orientace,double rotacni_uhel,double radius,double Xmys,double Ymys)
{
	TPointD S=getArcCenter(Xoblouk,Yoblouk,orientace,rotacni_uhel,radius);
	return rotacni_uhel/fabs(rotacni_uhel)*AngleFromTriangle(radius,delka(Xmys,Ymys,S.x,S.y),delka(Xmys,Ymys,Xoblouk,Yoblouk),3);//rotacni_uhel/fabs(rotacni_uhel) zajišťuje pouze znaménko úhlu
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//Převede logické souřadnice na fyzické (displej zařízení) , vraci fyzické souřadnice
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
	return round(F->Zoom*(logicka/F->m2px-F->Posun.x));
}
long Cmy::L2Py(double logicka)
{
	return round(F->Zoom*(-1*logicka/F->m2px-F->Posun.y));
}
TPoint *Cmy::L2P(TPointD *POLE,long posledni_prvek)
{
	//long velikost=sizeof(POLE)/sizeof(POLE[0]); nefunguje pro TPointD
	TPoint *POLE_px=new TPoint[posledni_prvek+1];
	for(int i=0;i<posledni_prvek+1;i++){POLE_px[i].x=L2Px(POLE[i].x);POLE_px[i].y=L2Py(POLE[i].y);}
	return POLE_px;
}
void Cmy::L2P(TPointD *POLE,TPoint *POLEpx)
{
	//POLEpx=L2P(POLE);
}
/////////////////////////////////////////////////////////////////////////////
//Převede  fyzické na logické souřadnice (displej zařízení) , vraci logické souřadnice
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
		return (fyzicka/F->Zoom+F->Posun.x)*F->m2px;
	}
	catch(...)
	{return 0;}
}
double Cmy::P2Ly(long fyzicka)
{
	try
	{
		return -1.0*(fyzicka/F->Zoom+F->Posun.y)*F->m2px;
	}
	catch(...)
	{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
//převede vzdálenost v metrech na vzdálenost monitoru dle konstanty m2px (metrů na pixel) a aktuální hodnoty proměnné Zoom
long Cmy::m2px(double meter)
{
	return round(meter*F->Zoom/F->m2px);
}
/////////////////////////////////////////////////////////////////////////////
//převede logické jednotky (metry) na pixely monitoru dle konstanty m2px (metrů na pixel) a aktuální hodnoty proměnné Zoom
double Cmy::px2m(long px)
{
	return px/F->Zoom*F->m2px;
}
/////////////////////////////////////////////////////////////////////////////
//dle změřeného úseku na monitoru převede tuto pixelovou vzádlenost a hodnotu v podobě uživatelského vstupu v metrech na hodnotu rozlišení podkladu
long double Cmy::getResolution(int puvX,int puvY,int aktX,int aktY,double metry)
{
	return metry/delka(P2Lx(puvX),P2Ly(puvY),P2Lx(aktX),P2Ly(aktY))*F->m2px;//výpočet metry děleno počet PX, výchozí zobrazení v nativním rozlišení (bez usazení do metrického měřítka) je 0.1
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//vráti střed pomyslné kružnice, z které je oblouk tvořen
TPointD Cmy::getArcCenter(double X,double Y,double orientace,double rotacni_uhel,double radius)
{
	double vychozi_uhel=90*rotacni_uhel/fabs(rotacni_uhel);//pro všechny oblouky je výchozí pro výpočet středu 90tkový oblouk, pouze záleží na směru rotace
	TPointD ret=rotace(radius,180-orientace,vychozi_uhel);//výpočet středu
	ret.x+=X;ret.y+=Y;
	return ret;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí vzdálenost od výchozího a koncového bodu k řídícímu bodu oblouku realizovaného bézierovou křivkou, vstupním parametrem je rotační úhel a radius, připraveno pouze pro některé úhly, výpočet není sice zcela exaktní, ale v rámci požadované tolerance výborný
double Cmy::getL(double RA,double R)
{
	double L=0.55191502449;//L - je vzdálenost od výchozího a koncového bodu vs. řídícího bodu, 0.552284749798297 resp. 0.55191502449 (přesnější), je tato hodnota na oblouku s poloměrem 1, viz http://spencermortensen.com/articles/bezier-circle/
	switch(round(abs(RA)))
	{             //L - hodnoty spočítané pro menší úhly
		case 15:L=R*0.0873912837529813;break;
		case 30:L=R*0.175536663479836;break;
		case 45:L=R*0.26521649;break;
		case 90:L*=R;break;
		default: L*=R*fabs(RA)/90.0;//už se značnou nepřesností
	}
	return L;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí souřadnice (4 místné pole TPointD tj. 8 hodnot) bézierovy křivky oblouku či linie dle zadaných souřadnic, X,Y jsou logické souřadnice výchozího vykreslování, parametry: orientace oblouku - dle světových stran (umí i jiné než 90° násobky), rotační úhel - pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90 (je-li nastaven na 0° jedná se o linii), radius - je radius oblouku v metrech nebo pokud je rotační úhel nastaven na 0° tedy se jedná o linii, je radius délkou linie
TPointD *Cmy::getArcLine(double X,double Y,double orientace,double rotacni_uhel,double radius)
{
	//parametry pro výchozí výpočetní model oblouku s OR 90, ten se následně dle skutečné OR resp. Orientace přerotuje a včetně znaménka přezrcadlí (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru)
	double OR=orientace;//orientace oblouku, dle světových stran
	double RA=rotacni_uhel;if(RA>90)RA=90;if(RA<-90)RA=-90;//rotační úhel, pod kterým je oblouk rotován, může být záporný (znaménko určuje směr rotace, + proti směru hodinových ručiček, - po směru), max. hodnota +90 a min. hodnota -90
	double R=radius;//Radius resp. délka u linie v metrech
	double L=getL(RA,R);//L - je vzdálenost od výchozího a koncového bodu vs. řídícího bodu
	double X1=X;//výchozí bod oblouku
	double Y1=Y;//výchozí bod oblouku
	double a=R*cos(ToRad(90-fabs(RA)));//výpočet polohy koncového bodu na ose X - výpočet dle goniometrické funkce v pravoúhlém trojúhelníku  - JE OK
	double b=R-sqrt(R*R-a*a);//výpočet polohy koncového bodu na ose Y - odečtení hodnoty dle pythagorovy věty od radiusu - JE OK
	if(RA==0){a=R;b=0;L=0;}//pokud se jedná o linii nastaví takovéto parametry
	double X2=X1+a;//koncový bod oblouku
	double Y2=Y1+b;//koncový bod oblouku
	TPointD K=rotace(X2,Y2,X2,Y2-L,-90+fabs(RA));//nový koncový řídící po rotaci
	TPointD *PL=new TPointD[4]; PL[0].x=X1;PL[0].y=Y1;PL[1].x=X1+L;PL[1].y=Y1;PL[2].x=K.x;PL[2].y=K.y;PL[3].x=X2;PL[3].y=Y2;
	if(RA<0)zrcadli_polygon(PL,3,180);//v případě záporné hodnoty přerotuje okolo osy
	rotace_polygon(X1,Y1,PL,3,90-OR);//orotuje se dle skutečné orientace
	return PL;//navrácení hodnoty
}
///------------------------------------------------------------------------------------------------------------------------------------------------------
//vratí bod včetně akutálního azimutu bodu z bézierovy křivky dle zadaných procent z její délky, perc jsou procenta/100 pozice na křivce v intervalu <0,1>
TPointD_3D Cmy::bezierPt(double orientace,double rotacni_uhel,double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double perc)
{
		//kvadratický bézier - p = (1-t)^2 *P0 + 2*(1-t)*t*P1 + t*t*P2 t is usually on 0-1 but that's not an essential - in fact the curves extend to infinity. P0, P1, etc are the control points. The curve goes through the two end points but not usually through the other points.
//	int x1=100,y1=100,x2=500,y2=100,x3=500,y3=500;
//	line(canv,x1,y1,x2,y2); line(canv,x2,y2,x3,y3);
//	for( float i = 0 ; i < 1 ; i += 0.01 )
//	{
//		// The Green Line
//		int xa = getPt( x1 , x2 , i );
//		int ya = getPt( y1 , y2 , i );
//		int xb = getPt( x2 , x3 , i );
//		int yb = getPt( y2 , y3 , i );
//
//		// The Black Dot
//		int x = getPt( xa , xb , i );
//		int y = getPt( ya , yb , i );
//
//		canv->Pixels[x][y]=clRed;
//	}

	//kubický bézier  //p = (1-t)^3 *P0 + 3*t*(1-t)^2*P1 + 3*t^2*(1-t)*P2 + t^3*P3 t is usually on 0-1 but that's not an essential - in fact the curves extend to infinity. P0, P1, etc are the control points. The curve goes through the two end points but not usually through the other points.
	//The Green Lines
	if(0<perc && perc<1)
	{
		double xa=getPt(x1,x2,perc);
		double ya=getPt(y1,y2,perc);
		double xb=getPt(x2,x3,perc);
		double yb=getPt(y2,y3,perc);
		double xc=getPt(x3,x4,perc);
		double yc=getPt(y3,y4,perc);

		//The Blue Line
		double xm=getPt(xa,xb,perc);
		double ym=getPt(ya,yb,perc);
		double xn=getPt(xb,xc,perc);
		double yn=getPt(yb,yc,perc);

		//return
		TPointD_3D RET;
		RET.x=getPt(xm,xn,perc);
		RET.y=getPt(ym,yn,perc);
		RET.z=azimut(xm,ym,xn,yn);
		return RET;
	}
	else//vychozí čí koncová pozice
	{
		TPointD_3D RET;
		if(perc==0){RET.x=x1;RET.y=y1;RET.z=orientace;}
		else {RET.x=x4;RET.y=y4;RET.z=orientace-rotacni_uhel;}//pro perc==1
		return RET;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//podpůrná metoda výše uvedené
double Cmy::getPt(double n1,double n2,double perc)
{
	return n1+((n2-n1)*perc);
}
///------------------------------------------------------------------------------------------------------------------------------------------------------
//vratí bod z linie čí z oblouku dle poměřu (perc) k celkové délce dané křivky, perc2 slouží na poměr v místě středu vozíku, který nemusí být díky rozdílnému místu uchycení totožný s parametrem perc
TPointD_3D Cmy::getPt(double radius,double orientace,double rotacni_uhel,double X1,double Y1,double X4,double Y4,double perc,double perc2,double rotacni_uhel_predchozi, double rotacni_uhel_nasledujici)
{
	TPointD_3D RET;
	if(0<perc && perc<1)
	{
		if(rotacni_uhel==0)//linie
		{
			RET.x=getPt(X1,X4,perc);
			RET.y=getPt(Y1,Y4,perc);
			RET.z=orientace;
		}
		else
		{
			double vychozi_uhel=90*rotacni_uhel/fabs(rotacni_uhel);//pro všechny oblouky je výchozí pro výpočet středu 90tkový oblouk, pouze záleží na směru rotace
			TPointD s=rotace(radius,180-orientace,vychozi_uhel);//výpočet středu
			TPointD ret=rotace(radius,180-orientace,rotacni_uhel*perc-vychozi_uhel);//výpočet ofsetu cílového bodu //dále v komentáři záloha výpočtu velikosti posunu (nepoužito) tj. trojúhelník o stranach Radius,Radius, Vypočítaná +radius,Y1+2*radius*sin(ToRad(rotacni_uhel*perc/2.0)
			RET.x=X1+s.x+ret.x;
			RET.y=Y1+s.y+ret.y;
			RET.z=orientace-rotacni_uhel*perc2;//navrácení aktuální orientace nosného palce (nerelevantně vozíku)
		}
	}
	else//vychozí čí koncová pozice
	{
		if(perc==0){RET.x=X1;RET.y=Y1;}
		else {RET.x=X4;RET.y=Y4;}//pro perc==1
		RET.z=orientace-rotacni_uhel*perc2;//pokud je střed podvozku uvnitř oblouku či linie nebo předchozí či následuje oblouku lini a střed je mimo aktuální oblouk, vždy nehledě na uchycení na palec
	}
	if(perc2<0 && rotacni_uhel_predchozi==0)RET.z=orientace; if(perc2>1 && rotacni_uhel_nasledujici==0)RET.z=orientace-rotacni_uhel;//pro situace, kdy předchází či navazuje linie, ne zcela dokonale, problém by mohl být v případě rozdílného radiusu či příliš krátkých segmentů atp.
	return RET;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vrátí délku bézierovy křivky, prec=preciznost výpočtu - asi nevyužito
double Cmy::bezierDelka(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,double prec)
{       //domnívám se, že by to chtělo převést do logických souřadnic kvůli přesnosti
	double /*delka_px=0,*/delka_RET=0, x_pre=x1,y_pre=y2;
	for( double i=0;i<=1;i+=prec)
	{
		TPointD_3D P=bezierPt(0,0,x1,y1,x2,y2,x3,y3,x4,y4,i);double x=P.x;double y=P.y;
		//delka_px+=m.delka(x_pre,y_pre,x,y);
		delka_RET+=delka(P2Lx(x_pre),P2Ly(y_pre),P2Lx(x),P2Ly(y));
		x_pre=x;y_pre=y;
	}
	return delka_RET;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí délku mezi body v absolutní hodnotě
double Cmy::delka(double X1,double Y1,double X2,double Y2)
{
	return sqrt(pow(X2-X1,2)+ pow(Y2-Y1,2));
}
/////////////////////////////////////////////////////////////////////////////,
//vrátí délku sklonu, může být zadáno i klesání
double Cmy::delkaSklon(double delka,double vyska)
{
	if(vyska!=0)return sqrt(pow(delka,2)+ pow(vyska,2));
  else return delka;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí délku části přepony podle části délky půdorysny
double Cmy::castPrepony(double castPud,double delkaPud,double vyska)
{
	if(vyska==0)return castPud;
	else return castPud/cos(atan(vyska/delkaPud));
}
/////////////////////////////////////////////////////////////////////////////
//vrátí půdorysnou délku z části přepony
double Cmy::vrat_delku_z_casti_prepony(double cast_prepony,double delkaPud,double vyska)
{
	if(vyska==0)return cast_prepony;
	else return cast_prepony*cos(atan(vyska/delkaPud));
}
/////////////////////////////////////////////////////////////////////////////
//prec=preciznost výpočtu
//double Cvykresli::bezierDelka(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double prec)
//{           DODĚLAT
//	double delka=0, x_pre=x1,y_pre=y1;
//	for( double i=0;i<=1;i+=prec)
//	{
//		TPointD_3D P=bezierPt(x1,y1,x2,y2,x3,y3,x4,y4,i);double x=P.x;double y=P.y;
//		//delka_px+=m.delka(x_pre,y_pre,x,y);
//		delka+=m.delka(m.P2Lx(x_pre),m.P2Ly(y_pre),m.P2Lx(x),m.P2Ly(y));
//		x_pre=x;y_pre=y;
//	}
//	return delka;
//}
/////////////////////////////////////////////////////////////////////////////
double Cmy::azimut(double X1,double Y1,double X2,double Y2)
{
	try
	{
		double ac=(Y2-Y1)/(delka(X1,Y1,X2,Y2)*1.0);
		//if(ac<-1 || 1<ac)F->Memo("azimut acos problém: "+String(X1)+" "+String(Y1)+" "+String(X2)+" "+String(Y2));
		if(delka(X1,Y1,X2,Y2)!=0 && -1<=ac && ac<=1)
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
		double ac=(Y2-Y1)/(delka(X1,Y1,X2,Y2)*1.0);
		//if(ac<-1 || 1<ac)F->Memo("uhel acos problém: "+String(ac));
		if(delka(X1,Y1,X2,Y2)!=0 && -1<=ac && ac<=1)
		{
			return acos((Y2-Y1)/(delka(X1,Y1,X2,Y2)*1.0))*180/M_PI;
		}
		else return 0;
	}
	catch(...)
	{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
//rotace                         //pozor, akt_uhel neni azimut, nutno používat akt_uhel=180-dodaný azimut (potom odpovídá orientaci tzn. např. 270 je na "vpravo, na západě"), příklad rotace(10,180-270,90); orotuje 10 metrovou linii směrující vodorovně zleva doprava o 90° proti směru hodinových ručiček tj. na 180° orientace
TPointD Cmy::rotace(double delka, double akt_uhel, double rotace)//rotace - pozor, naopak rotuje proti směru hodinových ručiček
{
	double Uhel=fmod(akt_uhel+rotace,360.0);// včetně ošetření přetečení přes 360 stupňů
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
//vrátí souřadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y1 proti směru hodinových ručiček, úhel zadaván jako azimut asi? (rozdíl proti výše uvedené stejnojmenné metodě)
TPointD Cmy::rotace(double X1,double Y1,double X2,double Y2,double uhel)
{
	TPointD RET=rotace(delka(X1,Y1,X2,Y2),180-azimut(X1,Y1,X2,Y2),uhel);
	RET.x+=X1;RET.y+=Y1;
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí souřadnice X2,Y2 po daném úhlu rotace, rotuje okolo X1,Y1 proti směru hodinových ručiček
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
//zajistí přezrcadlení polygonů zadaného v logických souřadnicích pomocí pole dle požadovaného parametru uhel zrcadlení
void Cmy::zrcadli_polygon(TPointD *POLE,long posledni_prvek,double uhel)
{
	uhel=Rt90(uhel);
	switch((int)uhel)
	{
		case 0:
		{
			double MaxY=POLE[0].y;
			for(long i=0;i<=posledni_prvek;i++)if(POLE[i].y>MaxY)MaxY=POLE[i].y;//najde co nejvíce vpravo bod a okolo toho proběhne mirroring
			for(long i=0;i<=posledni_prvek;i++)POLE[i].y=MaxY+MaxY-POLE[i].y;
		}break;
		case 90:
		{
			double MaxX=POLE[0].x;
			for(long i=0;i<=posledni_prvek;i++)if(POLE[i].x>MaxX)MaxX=POLE[i].x;//najde co nejvíce vpravo bod a okolo toho proběhne mirroring
			for(long i=0;i<=posledni_prvek;i++)POLE[i].x=MaxX+MaxX-POLE[i].x;
		}break;
		case 180:
		{
			double MinY=POLE[0].y;
			for(long i=0;i<=posledni_prvek;i++)if(POLE[i].y<MinY)MinY=POLE[i].y;//najde co nejvíce vpravo bod a okolo toho proběhne mirroring
			for(long i=0;i<=posledni_prvek;i++)POLE[i].y=MinY-(POLE[i].y-MinY);
		}break;
		case 270:
		{
			double MinX=POLE[0].x;
			for(long i=0;i<=posledni_prvek;i++)if(POLE[i].x<MinX)MinX=POLE[i].x;//najde co nejvíce vpravo bod a okolo toho proběhne mirroring
			for(long i=0;i<=posledni_prvek;i++)POLE[i].x=MinX-(POLE[i].x-MinX);
		}break;
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
//		if(ukaz->n>0)//vynechá hlavičku
//		{
//			pocet++;
//			Cvektory::TBod_LP *p;
//			p=ukaz->prvky;
//			while (p->dalsi!=NULL)//pro konkretni linii
//			{
//				TPointD P=w.WebMercator2UTM(p->X,p->Y);
//				TPointD P_dalsi=w.WebMercator2UTM(p->dalsi->X,p->dalsi->Y);
//				TPointD P_pred=w.WebMercator2UTM(p->predchozi->X,p->predchozi->Y);
//				if(p->n>0)//vynechá hlavičku
//				{
//					delka+=my.round(sqrt(pow(P_dalsi.x - P.x, 2)+ pow(P_dalsi.y - P.y, 2)));
//					if(p->n!=1)obsah+=P.x*(P_dalsi.y - P_pred.y); //Výpočet_plochy_pomocí_L´Huillierových_vzorců včetně nedokončených ploch či linii, alternativa Mascheroniho vzorec:http://klobouk.fsv.cvut.cz/~hanek/K154/PDF/Vypocet_vymer.pdf
////					if(pouze_usecka)//spočítá azimut
////					{
////						azimut=acos((P_dalsi.y-P.y)/(delka*1.0))*180/M_PI;
////						if(P_dalsi.x<P.x)azimut=360-azimut;
////					}
//				}
////				else //zjistí, jestli obsahuje pouze dva body, tj. jestli se jedná o úsečku a je důvod změřit i azimut
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
//	ret.x=delka;//v m - reálné číslo
//	ret.y=obsah;//v m2 - reálné číslo
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
			return 2147483647;//vrací nesmyslně vysoké číslo jako neúspěch (zřejmě leží na přímce ale ne na úsečce)
	}
	else if(Y2==Y1)//pokud jsou v jedné "linii" tj. je abs. vodorovná
	{
		if((X1<=x && x<=X2 && X1<X2) || (X2<=x && x<=X1 && X2<X1))
			return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
		else
			return 2147483647;//vrací nesmyslně vysoké číslo jako neúspěch (zřejmě leží na přímce ale ne na úsečce)
	}
	else//pro ostatní situace
  {
		if(((X1<=x && x<=X2 && X1<X2) || (X2<=x && x<=X1 && X2<X1)) && ((Y1<=y && y<=Y2 && Y1<Y2) || (Y2<=y && y<=Y1 && Y2<Y1)))
			return round(fabs((a*x + b*y + c) / sqrt(fabs(a*a + b*b))));
		else
			return 2147483647;//vrací nesmyslně vysoké číslo jako neúspěch (zřejmě leží na přímce ale ne na úsečce)
	}
}
/////////////////////////////////////////////////////////////////////////////
//funkce ověřující, zda kurzor myši, leží v obdelníku obsaném danému oblouku, souřadnice kurzoru myši se zadávají v logických souřadnicích, ostatní v logických, pro ověření zda leží na oblouku (s určitým perimetrem okolo obloukové linie) řešení níže metoda PtInSegment
bool Cmy::LeziVoblouku(double X,double Y,double orientace,double RA,double R,double Xmys,double Ymys)
{
	bool RET=false;
	TPointD *PL=getArcLine(X,Y,orientace,RA,R);
	double KorekceX=0,KorekceY=0;//pro případy, kdy počáteční i koncový bod je v jedné linii (vodorovné či svislé)
	if(round2double(PL[0].x,2)==round2double(PL[3].x,2))KorekceX=getArcLine(X,Y,orientace,RA/2.0,R)[3].x-PL[3].x;//round2double nasazeno z důvodu divného chování při porovnání dvou totožných čísel, kdy bylo vraceno false, bylo zaznamenáno u korekceY
	if(round2double(PL[0].y,2)==round2double(PL[3].y,2))KorekceY=getArcLine(X,Y,orientace,RA/2.0,R)[3].y-PL[3].y;//round2double nasazeno z důvodu divného chování při porovnání dvou totožných čísel, kdy bylo vraceno false, bylo zaznamenáno u korekceY
	if(PtInRectangle(X+KorekceX,Y+KorekceY,PL[3].x,PL[3].y,Xmys,Ymys))RET=true;
	delete []PL;PL=NULL;//smazání již nepotřebných ukazatelů
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//funkce vrátí průsečík dvou přímek či úseček, daná přímka musí být definována dvěma body na přímce (úsečkou, ale průsečík řesí mimo rozsah úsečky)
//pozor nutné ošetřit výstupní stavy, pokud jsou úsečky totožné, vrací pro každou souřadnici NAN (lze otestovat pomocí IsNan(vrácená_hodnota),nebo rovnoběžné -INF, či rovnoběžné protisměrné INF (lze otestovat pomocí IsInfinite(vrácená_hodnota))
TPointD Cmy::PrusecikPrimek(double xs1,double ys1,double xk1,double yk1,double xs2,double ys2,double xk2,double yk2)
{
	//ošetření, pokud je svislá linie jako první, zamění pořadí zadávaných úseček
	if(xs1==xk1)
	{
		double xs1temp=xs1;double ys1temp=ys1;double xk1temp=xk1;double yk1temp=yk1;
		xs1=xs2;ys1=ys2;xk1=xk2;yk1=yk2;
		xs2=xs1temp;ys2=ys1temp;xk2=xk1temp;yk2=yk1temp;
	}
	//samotný výpočet
	TPointD RET;
	RET.x=((yk1-ys1)*(xk2-xs2)*xs1+(xk1-xs1)*(xk2-xs2)*(ys2-ys1)-(yk2-ys2)*(xk1-xs1)*xs2)/((yk1-ys1)*(xk2-xs2)-(xk1-xs1)*(yk2-ys2));
	RET.y=(RET.x*(yk1-ys1)+ys1*(xk1-xs1)-xs1*(yk1-ys1))/(xk1-xs1);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//kontroluje zda se bod nachází na linii, linie mohou být jen svoslé nebo vodorovné, nikoliv pod sklonem (vrací false)
bool Cmy::PtInLine(double X1,double Y1,double X2,double Y2,double Xmys,double Ymys)
{
	//kontrola pořadí bodů přímky + přehození
	double pom;
	if(X1>X2){pom=X1;X1=X2;X2=pom;}
	if(Y1>Y2){pom=Y1;Y1=Y2;Y2=pom;}
	//kontrola zda je přímka vodorovná nebo svislá a zároveň, zda se bod nachází na přimce
	if(round2double(X1,5)==round2double(X2,5) && Y1<=Ymys && Ymys<=Y2 && round2double(X1,5)==round2double(Xmys,5))return true;
	else if(round2double(Y1,5)==round2double(Y2,5) && X1<=Xmys && Xmys<=X2 && round2double(Y1,5)==round2double(Ymys,5))return true;
	//přímka je pod úhlem nebo se bod nennachází na přímce
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//funkce ověří, zda se bod nachází v zadaném kruhu
bool Cmy::PtInCircle(double point_X,double point_Y,double center_X,double center_Y,double radius)
{
	return delka(point_X,point_Y,center_X,center_Y)<=radius;
}
/////////////////////////////////////////////////////////////////////////////
//funkce ověří, zda se bod nachází v obdelníku, zadání je v logických souřadnicích
bool Cmy::PtInRectangle(double X1,double Y1,double X2,double Y2,double Xmys,double Ymys)
{
	if(X1>X2){double Xt=X1;X1=X2;X2=Xt;}//v případě opačného pořadí, záměna pořadí
	if(Y1>Y2){double Yt=Y1;Y1=Y2;Y2=Yt;}//v případě opačného pořadí, záměna pořadí
	return (X1<=Xmys && Xmys<=X2 && Y1<=Ymys && Ymys<=Y2);
}
/////////////////////////////////////////////////////////////////////////////
//metoda ověří, zda se bod nachází ve stopce
bool Cmy::PtInStopka(double Ex,double Ey,double X,double Y,short uhel)
{
  //vhodno přesunout do globálních proměnných do Cvykresli
	float size=7*F->Zoom;//stopka
	short sklon=50;//stopka
	//referenční bode ve špičce, špička je směrem dolu (při nulové rotaci)
	float sizeX=size;float sizeY=size;
	if(uhel==0 || uhel==180)sizeX/=2.0;
	if(uhel==90 || uhel==270)sizeY/=2.0;
	POINT body[3]={{round(L2Px(Ex)+rotace(1,sklon,uhel).x*sizeX),round(L2Py(Ey)+rotace(1,sklon,uhel).y*sizeY)},{L2Px(Ex),L2Py(Ey)},{round(L2Px(Ex)+rotace(1,360-sklon,uhel).x*sizeX),round(L2Py(Ey)+rotace(1,360-sklon,uhel).y*sizeY)}};
	//F->Canvas->Pen->Color=clBlue;F->Canvas->Polygon((TPoint*)body,2);//pro testovací zákres
	return PtInRegion(CreatePolygonRgn(body,3,WINDING),L2Px(X),L2Py(Y));
}
/////////////////////////////////////////////////////////////////////////////
//metoda ověří zda je bod ve vnitř obrysu člověka, který se nachází na daných souřadnicích
bool Cmy::PtInClovek(double X,double Y,double Xmys,double Ymys,double rotace,short eID)
{
	//změna orientace - pozor
	short rotace90=Rt90(rotace);
	if(rotace90==0 || rotace90==180)//kvůli převrácené kresbě ve výchozí pozici tj. 0°
	{
		rotace+=180;
		rotace=fmod(rotace,360.0);// včetně ošetření přetečení přes 360 stupňů
		if(rotace<0){rotace+=360;}//pro záporné hodnoty
		rotace90=Rt90(rotace);
	}
	rotace=rotace90-1*(rotace-rotace90);//kvůli převrácené orientaci rotace, kdy rotační metody rotují proti hodinovým ručičkám a naopak rotace elementu je po směru hodinových ručiček

	//funkcionalita člověk - vyvoření cesty pera
	double oX=0.5665; double oY=-0.33425;//souřadnice těžiště hlavy - uchopovací bod, offset v souřadnicích, nemusí být středem vypočítaným z min a max hodnot níže...
	int posledni_prvek=63;TPoint PF[64];TPointD PL[64];
	if(F->Zoom>=9){PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.782;PL[37].y=-0.5;PL[38].x=0.694;PL[38].y=-0.654;PL[39].x=0.656;PL[39].y=-0.458;PL[40].x=0.64;PL[40].y=-0.474;PL[41].x=0.63;PL[41].y=-0.478;PL[42].x=0.586;PL[42].y=-0.48;PL[43].x=0.576;PL[43].y=-0.484;PL[44].x=0.582;PL[44].y=-0.532;PL[45].x=0.538;PL[45].y=-0.48;PL[46].x=0.507;PL[46].y=-0.469;PL[47].x=0.506;PL[47].y=-0.474;PL[48].x=0.476;PL[48].y=-0.458;PL[49].x=0.476;PL[49].y=-0.56;PL[50].x=0.38;PL[50].y=-0.62;PL[51].x=0.372;PL[51].y=-0.388;PL[52].x=0.322;PL[52].y=-0.36;PL[53].x=0.326;PL[53].y=-0.368;PL[54].x=0.272;PL[54].y=-0.332;PL[55].x=0.218;PL[55].y=-0.36;PL[56].x=0.18;PL[56].y=-0.392;PL[57].x=0.122;PL[57].y=-0.378;PL[58].x=0.148;PL[58].y=-0.456;PL[59].x=0.152;PL[59].y=-0.478;PL[60].x=0.214;PL[60].y=-0.584;PL[61].x=0.242;PL[61].y=-0.63;PL[62].x=0.29;PL[62].y=-0.626;PL[63].x=0.316;PL[63].y=-0.704;}//včetně noh
	else {posledni_prvek=60;PL[0].x=0.318;PL[0].y=-0.71;PL[1].x=0.276;PL[1].y=-0.718;PL[2].x=0.262;PL[2].y=-0.718;PL[3].x=0.198;PL[3].y=-0.692;PL[4].x=0.12;PL[4].y=-0.572;PL[5].x=0.036;PL[5].y=-0.55;PL[6].x=0.008;PL[6].y=-0.342;PL[7].x=0.024;PL[7].y=-0.26;PL[8].x=0.09;PL[8].y=-0.274;PL[9].x=0.228;PL[9].y=-0.204;PL[10].x=0.336;PL[10].y=-0.156;PL[11].x=0.366;PL[11].y=-0.11;PL[12].x=0.574;PL[12].y=-0.106;PL[13].x=0.716;PL[13].y=-0.108;PL[14].x=0.776;PL[14].y=-0.132;PL[15].x=0.878;PL[15].y=-0.188;PL[16].x=0.987;PL[16].y=-0.235;PL[17].x=1.166;PL[17].y=-0.282;PL[18].x=1.128;PL[18].y=-0.366;PL[19].x=1.116;PL[19].y=-0.468;PL[20].x=1.092;PL[20].y=-0.512;PL[21].x=0.946;PL[21].y=-0.692;PL[22].x=0.898;PL[22].y=-0.706;PL[23].x=0.89;PL[23].y=-0.716;PL[24].x=0.826;PL[24].y=-0.712;PL[25].x=0.828;PL[25].y=-0.7;PL[26].x=0.834;PL[26].y=-0.66;PL[27].x=0.928;PL[27].y=-0.584;PL[28].x=0.978;PL[28].y=-0.488;PL[29].x=1.002;PL[29].y=-0.46;PL[30].x=1.018;PL[30].y=-0.378;PL[31].x=0.956;PL[31].y=-0.384;PL[32].x=0.956;PL[32].y=-0.378;PL[33].x=0.868;PL[33].y=-0.33;PL[34].x=0.814;PL[34].y=-0.361;PL[35].x=0.814;PL[35].y=-0.37;PL[36].x=0.76;PL[36].y=-0.392;PL[37].x=0.731;PL[37].y=-0.397;PL[38].x=0.709;PL[38].y=-0.404;PL[39].x=0.691;PL[39].y=-0.407;PL[40].x=0.679;PL[40].y=-0.457;PL[41].x=0.633;PL[41].y=-0.479;PL[42].x=0.589;PL[42].y=-0.481;PL[43].x=0.579;PL[43].y=-0.485;PL[44].x=0.579;PL[44].y=-0.54;PL[45].x=0.541;PL[45].y=-0.48;PL[46].x=0.492;PL[46].y=-0.465;PL[47].x=0.439;PL[47].y=-0.452;PL[48].x=0.441;PL[48].y=-0.405;PL[49].x=0.372;PL[49].y=-0.389;PL[50].x=0.326;PL[50].y=-0.368;PL[51].x=0.272;PL[51].y=-0.332;PL[52].x=0.218;PL[52].y=-0.36;PL[53].x=0.18;PL[53].y=-0.392;PL[54].x=0.122;PL[54].y=-0.378;PL[55].x=0.148;PL[55].y=-0.456;PL[56].x=0.152;PL[56].y=-0.478;PL[57].x=0.214;PL[57].y=-0.584;PL[58].x=0.242;PL[58].y=-0.63;PL[59].x=0.29;PL[59].y=-0.626;PL[60].x=0.316;PL[60].y=-0.704;}
	rotace_polygon(oX,oY,X,Y,PL,PF,posledni_prvek,rotace);//orotuje a rovnou převede do fyzických souřadnic nahrazuje původní: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=L2PxX+m2px(PL[i].x-oX);PF2[i].y=L2PyY+m2px(oY-PL[i].y);}

	//pistole obrys v případě ION
	TPoint PF1[9];
	if(105<=eID && eID<=108)
	{
		float PS=25;
		PL[0].x=0.28875;PL[0].y=-0.68;PL[1].x=0.34;PL[1].y=-0.68;PL[2].x=0.34;PL[2].y=-0.9475;PL[3].x=0.32;PL[3].y=-0.975;PL[4].x=0.32;PL[4].y=-1.04375;PL[5].x=0.27;PL[5].y=-1.04375;PL[6].x=0.27;PL[6].y=-0.975;PL[7].x=0.25;PL[7].y=-0.9475;PL[8].x=0.25;PL[8].y=-0.7225;
		rotace_polygon(oX,oY,PL,8,rotace);
		if(PS!=0)rotace_polygon(PL[0].x,PL[0].y,PL,8,PS);//jen samotná pistole, pozn. optimalizaci takto jsem nerozchodil: if(PS!=0)m.rotace_polygon(oX,oY,PL[0].x,PL[0].y,PL,PF,8,PS);//jen samotná pistole
		for(int i=0;i<=8;i++){PF1[i].x=L2Px(X+PL[i].x-oX);PF1[i].y=L2Py(Y+PL[i].y-oY);}
	}

	//uzavření do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF,posledni_prvek);
	if(105<=eID && eID<=108)F->Canvas->Polygon(PF1,8);//v případě ION ještě pistole
	EndPath(F->Canvas->Handle);

	//testování finální citelné oblasti
//	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
//	else return false;
	//nové řešení
	bool RET=false;
	HRGN h=PathToRegion(F->Canvas->Handle);
	if(PtInRegion(h,L2Px(Xmys),L2Py(Ymys)))RET=true;else RET=false;
	DeleteObject(h);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//metoda ověří zda je bod ve vnitř obrysu ION tyčí, který se nachází na daných souřadnicích
bool Cmy::PtInIon(double X,double Y,double Xmys,double Ymys,double rotace)
{
	double oX=0.5;double oY=-1;//střed offset
	int posledni_prvek=21;TPointD PL[22];

	//jedna kružnice
	TPoint PF1[22];
	PL[0].x=0.5;PL[0].y=-1.86333333333333;PL[1].x=0.65;PL[1].y=-1.88666666666667;PL[2].x=0.846666666666667;PL[2].y=-1.62666666666667;PL[3].x=0.596666666666667;PL[3].y=-1.45666666666667;PL[4].x=0.58;PL[4].y=-1.38333333333333;PL[5].x=0.576666666666667;PL[5].y=-1.37333333333333;PL[6].x=0.56;PL[6].y=-1.3;PL[7].x=0.628333333333333;PL[7].y=-1.22;PL[8].x=0.628333333333333;PL[8].y=-1.22333333333333;PL[9].x=0.696666666666667;PL[9].y=-1.14333333333333;PL[10].x=0.498333333333333;PL[10].y=-1.14166666666667;PL[11].x=0.501666666666667;PL[11].y=-1.14833333333333;PL[12].x=0.3;PL[12].y=-1.14333333333333;PL[13].x=0.366666666666667;PL[13].y=-1.22;PL[14].x=0.366666666666667;PL[14].y=-1.22;PL[15].x=0.433333333333333;PL[15].y=-1.29666666666667;PL[16].x=0.416666666666667;PL[16].y=-1.37333333333333;PL[17].x=0.42;PL[17].y=-1.38;PL[18].x=0.403333333333333;PL[18].y=-1.45666666666667;PL[19].x=0.26;PL[19].y=-1.53;PL[20].x=0.2;PL[20].y=-1.83333333333333;PL[21].x=0.493333333333333;PL[21].y=-1.86;
	rotace_polygon(oX,oY,X,Y,PL,PF1,posledni_prvek,rotace);//orotuje a rovnou převede do fyzických souřadnic nahrazuje původní: for(int i=0;i<=posledni_prvek;i++){PF2[i].x=X+m2px(PL[i].x-oX);PF2[i].y=Y+m2px(oY-PL[i].y);}

	//druhá kružnice
	TPoint PF2[22];
	PL[0].x=0.5;PL[0].y=-0.14;PL[1].x=0.65;PL[1].y=-0.116666666666666;PL[2].x=0.846666666666667;PL[2].y=-0.376666666666666;PL[3].x=0.596666666666667;PL[3].y=-0.546666666666666;PL[4].x=0.58;PL[4].y=-0.62;PL[5].x=0.576666666666667;PL[5].y=-0.63;PL[6].x=0.56;PL[6].y=-0.703333333333333;PL[7].x=0.628333333333333;PL[7].y=-0.783333333333333;PL[8].x=0.628333333333333;PL[8].y=-0.78;PL[9].x=0.696666666666667;PL[9].y=-0.86;PL[10].x=0.498333333333333;PL[10].y=-0.861666666666666;PL[11].x=0.501666666666667;PL[11].y=-0.855;PL[12].x=0.3;PL[12].y=-0.86;PL[13].x=0.366666666666667;PL[13].y=-0.783333333333333;PL[14].x=0.366666666666667;PL[14].y=-0.783333333333333;PL[15].x=0.433333333333333;PL[15].y=-0.706666666666667;PL[16].x=0.416666666666667;PL[16].y=-0.63;PL[17].x=0.42;PL[17].y=-0.623333333333333;PL[18].x=0.403333333333333;PL[18].y=-0.546666666666666;PL[19].x=0.26;PL[19].y=-0.473333333333333;PL[20].x=0.21;PL[20].y=-0.156666666666667;PL[21].x=0.493333333333333;PL[21].y=-0.143333333333333;
	rotace_polygon(oX,oY,X,Y,PL,PF2,posledni_prvek,rotace);//orotuje a rovnou převede do fyzických souřadnic nahrazuje původní:

	//uzavření do cesty
	BeginPath(F->Canvas->Handle);
	F->Canvas->PolyBezier(PF1,posledni_prvek);
	F->Canvas->PolyBezier(PF2,posledni_prvek);
	EndPath(F->Canvas->Handle);

	//testování finální citelné oblasti
//	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
//	else return false;
	//nové řešení
	bool RET=false;
	HRGN h=PathToRegion(F->Canvas->Handle);
	if(PtInRegion(h,L2Px(Xmys),L2Py(Ymys)))RET=true;else RET=false;
	DeleteObject(h);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//metoda ověří zda je bod ve vnitř obrysu teploměru, který se nachází na daných souřadnicích
bool Cmy::PtInTeplomer(double X,double Y,double Xmys,double Ymys,double rotace)
{
	////vstupní proměnné či konstanty
	X=L2Px(X);Y=L2Py(Y);
	//double Z=F->Zoom;//zoom, pouze zkrácení zápisu
	float polomer=0.2;//poloměr kružnic zadaná v metrech
	int polomer1=m2px(polomer);//poloměr kružnic
	int polomer2=m2px(polomer/2.0);//poloměr kružnic
	int polomer8=m2px(polomer/8.0);//poloměr kružnic
	short DT=4;//násobek poloměru z kterého vznikné délka teploměru
	int vzdalenostY=m2px(F->d.v.PP.sirka_podvozek/2.0+polomer*2);//vzdálenost kružnice od středu v metrech (vzádlenost kružnic podělená dvěmi), pro kurzor a normální zobrazení dle šířky kolejí linky (šířky vozíku), jinak fixní šířka pro ikonu v galerii

	////rotace
	if(rotace==90 || rotace==270)
	{
		X+=vzdalenostY;
		Y+=vzdalenostY;
	}

	////uzavření do cesty
	BeginPath(F->Canvas->Handle);
	//obrys elementu
	F->Canvas->Ellipse(X-polomer1,Y-polomer1-vzdalenostY,X+polomer1,Y+polomer1-vzdalenostY);//baňka
	F->Canvas->RoundRect(X-polomer2,Y-polomer1*DT-vzdalenostY,X+polomer2,Y-vzdalenostY,polomer1,polomer1);//tyčka
	//stupnice opsaný obdelník
	F->Canvas->Rectangle(TRect(X+polomer2,Y-polomer1*3-polomer8*2-vzdalenostY,X+polomer1+polomer2+polomer8,Y-polomer1-vzdalenostY));//pouze empiricky nasázené hodnoty, aby oblast vyšla nějak "rozumně"
	EndPath(F->Canvas->Handle);

	////testování finální citelné oblasti
//	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
//	else return false;
	//nové řešení - výše nepoužívat
	bool RET=false;
	HRGN h=PathToRegion(F->Canvas->Handle);
	//InvertRgn(F->Canvas->Handle,h); test výpisu oblasti
	if(PtInRegion(h,L2Px(Xmys),L2Py(Ymys)))RET=true;else RET=false;
	DeleteObject(h);
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
//otestuje zda v daném geometrickém segmetnu (o velikosti pásma aktuální šířky kolejí) se nachází souřadnice kurzor myši, pokud ano, vrátí true, jinak false
bool Cmy::PtInSegment(double X,double Y,short typ,double orientace,double rotacni_uhel,double radius,double delka,double Xmys,double Ymys)
{
	//vytvoření testovací oblasti
	BeginPath(F->Canvas->Handle);
	F->d.vytvor_oblast_koleje(F->Canvas,X,Y,typ,orientace,rotacni_uhel,radius,delka);
	EndPath(F->Canvas->Handle);

	//testování finální citelné oblasti
//	if(PtInRegion(PathToRegion(F->Canvas->Handle),L2Px(Xmys),L2Py(Ymys)))return true;
//	else return false;
	//nové řešení
	bool RET=false;
	HRGN h=PathToRegion(F->Canvas->Handle);
	if(PtInRegion(h,L2Px(Xmys),L2Py(Ymys)))RET=true;else RET=false;
	DeleteObject(h);
	return RET;
}

/////////////////////////////////////////////////////////////////////////////
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
		double v=sqrt(fabs(pow(d0n,2)-pow(m,2)));// dela se to trojuhelnikama  <--DOPLNĚNA ABSOLUTNÍ HODNOTA PRO REÁLNÁ ČÍSLA

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
double Cmy::cekani_na_palec(double cas, double roztec_palcu,double rychlost_dopravniku,int funkce)//vrátí dobu čekání na palec v sec, rozteč je v m, rychlost dopravníku v m/s
{
	//if(zohlednit && rezim!=1)//pokud se jedná o kontinuální režim neřeší se, předpokládá se, že jede na stejném dopravníku
	{
		//exaktní výpočet je použitelný jenom v případ známe goemetrie, navíc obsahuje chybu
		//double cas_presunu_mezi_palci=(60*roztec_palcu)/(rychlost_dopravniku*100);  //to 100 je převod na cm z m
		//nn, neceločíselný zbyte double zbytek_po_deleni=(cas*60/cas_presunu_mezi_palci)-floor(cas*60/cas_presunu_mezi_palci);//tzn. kde se nachází
		//return cas_presunu_mezi_palci*zbytek_po_deleni;
		//jako střední hodnota vyplývající z normálního pravděpodonostního rozdělení hodnot
		//(cas_presunu_mezi_palci-0)/2 resp. (max-min)/2
		//return (roztec_palcu/(rychlost_dopravniku*1000.0))/2.0; //vrátí dobu čekání na palec v min, rozteče je v mm resp. v m za z minu u rychlosti dopravniku
	}
	//else return 0;

	//vrátí dobu čekání na palec v sec, rozteč je v m, rychlost dopravníku v m/s
	double RET=0.0;
	double MIN=0.0;double MAX=0.0; if(rychlost_dopravniku!=0)MAX=roztec_palcu/rychlost_dopravniku;
	double ZOI=0.1;if(MAX==0 || MAX<=ZOI)ZOI=0.0;//korekce pro zajištění zprava otevřeného intervalu (nemůže být uzavřený, protože to není reálné, dochází v takové situaci ještě k uchopení původním palcem), řád kokekce zvolen neexaktně, pouze dle úvahy, pokud není MAX hodnota (například z důvodu 0 hodnoty rozteče) kladné číslo větší než nula, tak se korekce neuvažuje, aby se nešlo do mínusu s výpočtem (tedy čekáním)
	switch(funkce)
	{
			case 0:RET=MIN;break;//nic resp minimum=0, nečeká na palec vůbec buď vyšel přesně nebo se nezohledňuje
			case 1:RET=(MAX-ZOI+MIN)/2.0;break;//střední hodnota (v tomto případě i průměr) dle normálního rozdělení pro hodnoty <0,max)
			case 2:RET=RAND(MIN,MAX,1,true,false);/*fmod(rand(),MAX*10)/10.0+MIN;*/break;//náhodná hodnota v rozmezí <0,max) čekání na palce, zde ZOI není nutné zohledňovat, protože již vyplývá z použitého algoritmu
			case 3:RET=MAX-ZOI;break;//max.možná hodnota čekání na pale
			case 4:/*RET=tady bude exaktní výpočet pro geometrii*/break;
	}
	return RET;
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::latence_mezi_stopkami(double RD)//vratí v jednotkách času [s] zpoždění mezí reakcí stopek, RD - rychlost pohonu, na které je stopka z které je vyslána informace o možnosti otevření stopky předchozí
{
	return 0.1/RD;//aktuálně nastaveno na 10 cm délka patle
}
/////////////////////////////////////////////////////////////////////////////
//metoda vratí minimální možnou mezeru mezi vozíky, pokud je parametr mezera roven 0, v případě nenulového parametru mezery vrací vhodnou nejbližší hodnotu této mezery vůči rozměrům rozteč a rozměr vozíku, pokud nebude zadaná rozteč tj. bude 0, vrací hodnotu 0, lze parametrizovat vracený výsledek 0 (implicitně) - kritická mezera, 1 či 281 - mezera mezi JIG, 2 či 282 mezera mezi PODVOZKY
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
			if(min_mezera>=mezera || mezera<0)RET=min_mezera;//pokud je minimální možná mezera menší než nebo stejná jak zadaná, případně chybně zadaná v podobě záporného čísla tak vrátí minimální možnou resp. zadanou v případě ==
			else {RET=min_mezera+round((mezera-min_mezera)/roztec)*roztec;}//vratí nejbližší možnou mezeru mezi vozíky
		}
		//pokud je požadovaná mezi jigy či vozíky
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
	else return 0;//pokud nebude známa rozteč
}
////////////////////////
//vrátí mezeru dle rozestupu v palcích a rozteče a velikosti vozíku dle rotace, typ==-1 vrátí velikost mezery automaticky co je kritištější, zda podovzek či jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double dJ,double sJ,double rotace,double Rx,double R,short typ)
{
	switch(typ)
	{
		case  0: return (Rx*R)-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return (Rx*R)-UDJ(dJ,sJ,rotace);//mezi JIGy
		default: return (Rx*R)-UDV(dJ,sJ,rotace);//automaticky co je kritičtější
	}
}
////////////////////////
//vrátí mezeru dle rozestupu a rotace (resp. velikosti vozíku spočítané dle rotace), typ==-1 vrátí velikost mezery automaticky co je kritištější, zda podovzek či jig, 0 - podvozek, 1 - jig
double Cmy::mezera(double rotace,double Rz,short typ)
{
	switch(typ)
	{
		case  0: return Rz-F->d.v.PP.delka_podvozek;//mezi podvozky
		case  1: return Rz-UDJ(rotace);//mezi JIGy
		default: return Rz-UDV(rotace);//automaticky co je kritičtější
	}
}
/////////////////////////////////////////////////////////////////////////////
//vrátí velikokost minimální mezery tak, aby se stíhálo čekání na palce a vozíky se nesrazily
double Cmy::minM(double RD1,double RD2,double R2)
{
	double M=0.0;
	double maxTcek=cekani_na_palec(0,R2,RD2,3);//R2/RD2-ZOI;//maximální doba čekání na uchycení na palec
	if(RD2>=RD1)M=maxTcek*RD1;
	else M=maxTcek/(2*(1/RD1-1/RD2));
	return M;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správně mezera
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
//vrátí rozestup v počtech palců mezi aktivními palci, byla-li zadáná správně mezera
double Cmy::Rx(double dJ,double sJ,double rotace,double M,double R)
{
	if(R==0)return 0;//nebyla definováná rozteč palců, tudíž se nepracuje s palci
	else return (M+UDV(dJ,sJ,rotace))/R;
}
////////////////////////
//vrátí rozestup v počtech palců mezi aktivními palci z RD a R (a resp. TT)
double Cmy::Rx(double RD,double R)
{
	return Rx2(Rz(RD),R);
}
////////////////////////
//vrátí rozestup v počtech palců mezi aktivními palci z Rz a R
double Cmy::Rx2(double Rz,double R)
{
	if(R==0) return 0;//nebyla definováná rozteč palců, tudíž se nepracuje s palci
	else return Rz/R;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozteče dle rozestupu a počtu palců
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
//vratí RD dle velikosti lakovacího okna a procesního času
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
//dle aktuálního RD a délky otoč vrátí PTo
double Cmy::PTo(double Dotoc,double RD)
{
	if(RD==0)return 0; else return Dotoc/RD;
}
////////////////////////
//vypočítá velikost zóny otáčení před a za otočí
TPointD Cmy::zona_otaceni(double orientace_jig_vstup,double rotace_otoce,double delka_otoc)
{
	TPointD ret;ret.x=0;ret.y=0;
	//////inputs JIG
	double sirka=F->d.v.PP.sirka_jig,delka=F->d.v.PP.delka_jig;

	//////konstanty pro výpočet
	double diam=sqrt(pow(delka/2.0,2)+pow(sirka/2.0,2));//poloměr kružnic
	double c1=RadToDeg(ArcTan(sirka/delka));
	double alpha[4];alpha[0]=c1+orientace_jig_vstup,alpha[1]=180-alpha[0],alpha[2]=180+alpha[0],alpha[3]=360-alpha[0];
	double max=MaxInt*(-1),min=MaxInt;

	//////virtuální pole
	double mezi_vypocet=0;
	for(int col=0;col<=3;col++)
	{
		//přiřazení alpha podle sloupce
		double a=alpha[col];
		int c2=0;
		for(int row=0;row<=7;row++)
		{
			//výpočet prvního pole
			if(row==2)c2=1;if(row==4)c2=-1;
			if(row%2==0 && row!=6)mezi_vypocet=-delka_otoc/DegToRad(rotace_otoce)*(DegToRad(a)+ArcSin(delka_otoc/(DegToRad(rotace_otoce)*diam))-2*M_PI*c2-M_PI);
			if(row%2!=0 && row!=7)mezi_vypocet=delka_otoc/DegToRad(rotace_otoce)*(-DegToRad(a)+ArcSin(delka_otoc/(DegToRad(rotace_otoce)*diam))-2*M_PI*c2);
			if(row==6)mezi_vypocet=0;
			if(row==7)mezi_vypocet=delka_otoc;
			//výpočet druhého pole
			if(row<=5)
			{
				if(mezi_vypocet>0 && mezi_vypocet<delka_otoc)mezi_vypocet=diam*cos(DegToRad(a)+DegToRad(rotace_otoce)*mezi_vypocet/delka_otoc)+mezi_vypocet;
				else mezi_vypocet=delka_otoc/2.0;
			}
			else
			{
				mezi_vypocet=diam*cos(DegToRad(a)+DegToRad(rotace_otoce)*mezi_vypocet/delka_otoc)+mezi_vypocet;
			}
			//hledání extrémů
			if(mezi_vypocet>max)max=mezi_vypocet;
			if(mezi_vypocet<min)min=mezi_vypocet;
		}
	}
	//////výsledek
	ret.x=min*(-1);
	ret.y=max-delka_otoc;
	return ret;
}
////////////////////////
//dle aktuálního RD a času otáčení otoče vrátí délku otáčení
double Cmy::Dotoc(double PTo,double RD)
{
	return PTo*RD;
}
/////////////////////////////////////////////////////////////////////////////
//vratí hodnotu RT (reserve time), ta může být i záporná, WT čekání na palac si dopočítává metoda sama, pokud WT==-1, pokud je dosazena kladná hodnota větší než 0, tak je ta uvažovaná jako WT, 0 hodnota znamena WT čekání na palec neuvažovat
//metoda aktuálně nevyžitá připadně možno smazat
//double Cmy::RT(double PT,double delka_prejezdu,double RD,double R,double WT)
//{
//	if(RD!=0)
//	{
//		if(WT==-1)WT=cekani_na_palec(0,R,RD,3);
//		return F->d.v.PP.TT-(delka_prejezdu/RD+PT+WT);
//	}
//	else return 0;
//}double Cmy::RT(double PT,double doba_prejezdu,double WT,unsigned int pocet_voziku_v_bufferu,double RD,double WT0)
{
  //	if(RD==0 || pocet_voziku==0) return 0;//pokud není přiřazen pohon nebo se jedná o průjezdní stopku
//	else
//	{                                                             //ubraná časová čast přejezdu o buffer
//		double pocet_voziku_prejezd=(doba_prejezdu-pocet_voziku/*nastavených - stojicích v bufferu*/*F->d.v.PP.delka_podvozek/RD)/F->d.v.PP.TT;//doplní včetně skutečného počtu vozíků (tzn. vozíky v bufferu a v pohybu) na přejezdu
//		return (pocet_voziku_prejezd+pocet_voziku)*F->d.v.PP.TT-(doba_prejezdu+PT+WT);
//	}
//	F->Memo(F->d.v.PP.TT);
//	F->Memo(doba_prejezdu);
//	F->Memo(pocet_voziku_v_bufferu);
//	F->Memo(F->d.v.PP.delka_podvozek);
//	F->Memo(RD);
//
//	60
//100,536980351773
//8
//0,6
//0,081
//
//	60-(fmod(100.536980351773-(8-1)*0,6/0.081,60)+0+6.7)+(8-1)*60

	if(RD==0 || pocet_voziku_v_bufferu==0)return 0;//pokud není přiřazen pohon nebo se jedná o průjezdní stopku
	else
	{
																													//ubraná časová čast přejezdu o buffer, jeden vozík již zoohledněn (proto -1)
		if(pocet_voziku_v_bufferu==1)                                                                         //tj. 0
		return F->d.v.PP.TT+/*(pocet_voziku_v_bufferu-1)*F->d.v.PP.TT*/-(fmod(doba_prejezdu-(pocet_voziku_v_bufferu-1/*nastavených - stojicích v bufferu*/)*F->d.v.PP.delka_podvozek/RD,F->d.v.PP.TT)+PT+WT+WT0);
		else return F->d.v.PP.TT-(WT+F->d.v.PP.delka_podvozek/RD+WT);//WT opravdu 2x při uchycení pře mikropřejezdem a potom na stopce//F->d.v.PP.TT*pocet_voziku_v_bufferu - čas mezi stopkami, nikoliv rezerva na stopce;

	}

	//doba_prejezdu-
}
/////////////////////////////////////////////////////////////////////////////
//výpočet časové rezervy pro KK roboty / operátory
double Cmy::CT(double LO,double aRD)
{
	double CT=0;
	if(aRD>0)
	{
		CT=LO/aRD;
		if(CT>F->d.v.PP.TT)CT=F->d.v.PP.TT;
	}
	return CT;
}
/////////////////////////////////////////////////////////////////////////////
//výpočet RT pro KK roboty
double Cmy::KKRT(double CT,double PT)
{
	return CT-PT;
}
/////////////////////////////////////////////////////////////////////////////
//výpočet RT pro KK roboty s otočí
double Cmy::KKRT(double CT1,double PT1,double CT2,double PT2)
{
	double RT1,RT2;
	RT1=CT1-PT1;
	RT2=CT2-PT2;
	if(RT1<RT2)return RT1;
  else return RT2;
}
/////////////////////////////////////////////////////////////////////////////
//vratí RD dle délky otoče a času otáčení
double Cmy::RDo(double Dotoc,double PTo)
{
	if(PTo==0)return 0; else return Dotoc/PTo;
}
////////////////////////
//vrátí doporučenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteč mezi palci i takt
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
//vrací true pokud nová rozteč (R) ovlivní Rz resp RD
bool Cmy::kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove)
{
	if(fmod(R_puvodni,R_nove))return true;
	else return false;
}
/////////////////////////////////////////////////////////////////////////////
//dle požadovaného času čekání a požadovaného taktu vrátí počet vozíku, pokud je druhý parametr true osekne vrácený výsledek na celé číslo zaokrouhlené směrem dolu
double Cmy::WT2V(double WT,double TT,double Floor)
{
	if(Floor)return floor(WT/TT);
	else return WT/TT;
}
/////////////////////////////////////////////////////////////////////////////
//dle počtu vozíků a požadovaného taktu vrátí dobu čekání
double Cmy::V2WT(double V,double TT)
{
	return V*TT;
}
/////////////////////////////////////////////////////////////////////////////
//dle celkového WT a počtu použitých pozic vrátí WT na aktuální pozici, NEPOUŽÍVÁ SE!!!
double Cmy::casVbufferu(double WTstop,unsigned int pocet_pouzitych_pozic,unsigned int akt_pozice)
{
	return (WTstop/(double)pocet_pouzitych_pozic)*akt_pozice;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku vozíku
double Cmy::UDV(double dJ,double sJ,double rotace)
{
	//postupně rozšířit o výpočet dle zadaných stupňů nejenom 0 vs. 90
	double DV=UDJ(dJ,sJ,rotace);
	if(DV<F->d.v.PP.delka_podvozek)DV=F->d.v.PP.delka_podvozek;
	return DV;
}
/////////////////////////////////////////////////////////////////////////////
//vratí užitnou délku vozíku, parametry bere z PP
double Cmy::UDV(double rotace)
{
	return UDV(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);//je to takto v pořádku
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku jigu
double Cmy::UDJ(double dJ,double sJ,double rotace)
{
	//postupně rozšířit o výpočet (obsanou oblast) dle zadaných stupňů nejenom 0 vs. 90, např. nutno dořešit např. typickou 45°
	if(rotace==0 || rotace==360 || rotace==180  || rotace==-180)return dJ;//delka voziku
	else return sJ;// šířka vozíku
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku jigu, parametry bere z PP
double Cmy::UDJ(double rotace)
{
	return UDJ(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,rotace);
}
/////////////////////////////////////////////////////////////////////////////
//vrátí, zda je možné orotovat jig tak, aby nemělo vliv na změnu Rz, Rx, RD
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
//vrátí čas přejezdu vozíku
double Cmy::prejezd_voziku(double delka, double rychlost_dopravniku)
{
	if(rychlost_dopravniku==0 || delka==0)return 0;//pozor může být zavadějící
	else return delka/rychlost_dopravniku;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí požadovanou rychlost přejezdu, umí si dopočítat MT, není-li dodáno, pokud vyjde záporná rychlost tzn. nestíhá
double Cmy::prejezd_voziku_rychlost(double CT,double MT,double PT,double WT,double DD)
{
	if(MT==0)//pokud není MT dodáno je nutné jej spočítat, pokud nebude vyčísleno PT a WT (což může být vhodné na objekty v režimu PP), bude MT totožné s CT, bude tedy splněna alespoň minumální nutná (nikoliv dostatčující) podmínka, kdy DD/CT>=aRD
	{
		MT=CT-PT-WT;
	}
	if(MT==0) return 0;//pouze ošetření dělení nulou
	else return DD/MT;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozdíl aktuální rychlosti pohonu a potřebné k uskuteční přejezdu, pokud je hodnota 0 je v pořádku, je-li záporná, přejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
double Cmy::kontrola_rychlosti_prejezdu(double CT,double MT,double PT,double WT,double DD,double aRD)
{
	return aRD-prejezd_voziku_rychlost(CT,MT,PT,WT,DD);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//nastaví horizontální a vertikální pozici tlačítka a také designové vlasnosti podle tlačítkek Ano, Uložit, OK, Storno dle MyMessageBox
void Cmy::designButton(TscGPButton *button,TForm *form,short rank,short sum,short horizontal_space,short vertikal_space)
{
	//horizontální pozice
	switch(sum)
	{
		//celkem jedno tlačítko
		case 1:button->Left=form->Width/2-button->Width/2;break;//na střed
		//celkem dvě tlačítka
		case 2:
		{
			if(rank==1)button->Left=form->Width/2-button->Width-horizontal_space/2; //první tlačítko
			else button->Left=form->Width/2+horizontal_space/2;//druhé tlačítko
		}
		break;
		//celkem tři tlačítka
		case 3:
		{
			switch(rank)
			{
				case 1:button->Left=form->Width/2-button->Width/2-horizontal_space-button->Width; break;//první
				case 2:button->Left=form->Width/2-button->Width/2;break;//druhé na střed
				case 3:button->Left=form->Width/2+button->Width/2+horizontal_space-button->Width;break;//třetí
			}
		}
		break;
	}

	//vertikální pozice
	button->Top=form->Height-button->Height-vertikal_space;

	//přebírání designu z referenčních tlačítek v MyMessageBox
	if(button->ModalResult==mrYes){*button->Options=*myMessageBox->Button_Yes->Options;button->Layout=myMessageBox->Button_Yes->Layout;}
	if(button->ModalResult==mrOk){*button->Options=*myMessageBox->Button_OK->Options;button->Layout=myMessageBox->Button_OK->Layout;}
	if(button->ModalResult==mrCancel){*button->Options=*myMessageBox->Button_Cancel->Options;button->Layout=myMessageBox->Button_Cancel->Layout;}
	if(button->ModalResult==mrNo){*button->Options=*myMessageBox->Button_No->Options;button->Layout=myMessageBox->Button_No->Layout;}
}
/////////////////////////////////////////////////////////////////////////////
//vykreslí danému oknu transparentní (kvůli možnému smazání - přemaskování) dle zadané barvy a šířky, nutno volat při formactive (lépe však při formpaint), při šířce 1px (ta je zároveň implicitní) stačí volat, jenom při formactive, jinak i při formsize, formresize,formclose, příklad použití: frameForm(Form_parametry,clWebOrange,1);
void Cmy::frameForm(TForm *form,TColor color,short width)
{
	short o=floor(width/2.0);
	frameRect(TRect(form->Left-o,form->Top-o,form->Left+form->Width+o,form->Top+form->Height+o),color,width);
}
/////////////////////////////////////////////////////////////////////////////
//nakresli na libovolném místě na monitoru obdélník dle zadaných souřadnic ve formátu TRect, je možné nastavit barvu a šířku čáry tohoto obdélníku
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
//z rychlosti v m/s vratí čas v milisekundách (proto *1000) potřebný na překreslení jednoho pixelu při daném zoomu
//parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se minimální vhodný čas na přehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer nehledě na rychlost
//pokud je (i implicitní) parametr speed_min==0, tzn. převezme se hodnota aktuálního počítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálně (netrhnaně), v případě nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace,
//pokud by byl paremetr speed nižší než nenulový speed_min, nebude se jednat kontinuální (netrhnanou) simulaci
double Cmy::get_timePERpx(double speed,double A,double speed_min)//A je akcelerace
{
	double Z=F->Zoom;if(F->antialiasing)Z/=3.0;//pokud je spuštěn antialiasing, stačí 1/3 rychlost (vyplývá z principu algoritmu AA)
	if(A==0)//v případě implicitního zrychlení je uvažováno spočítat zrychlení vhodné k tomu, aby se animace zobrazovala kontinuálně (netrhnaně)
	{                           //bacha pokud není speed_min 0, stejně při animaci se nechovalo korektně
		if(speed_min==0)speed_min=speed;//pokud je (i implicitní) parametr speed_min==0, tzn. převezme se hodnota aktuálního počítaného RD, tzn. všechny animace se promítnou se stejným afps dle fps, tj. všechny animace se zobrazí kontinuálně (netrhnaně), v případě nenulové hodnoty je speed_min stanovane jako minimální možná rychlost pro zobrazení kontinuální (netrhnané) simulace
		F->afps=F->fps/(1/F->m2px*speed_min);//korekce, nepokrátím výrazy v níže uvedeném vzorci, protože afps se používá na více místech a zásah by byl příliš komplikovaný
		return F->m2px/Z/speed*1000.0/F->afps;//původně chybná úvaha (proto korekce výše), (bez korekce) koncipováno pouze pro rychlosti 0,1 m/s (hodnota m2px) a vyšší//vrátí čas, tak aby se jednalo o kontinální animaci
	}
	else return F->m2px/Z/speed*1000.0/A;//vrátí čas na posun o jeden pixel
}
/////////////////////////////////////////////////////////////////////////////
//vrací true či false zda se daná hodnota nachází čí nenachází v intervalu, interval může být uzavřený (tzn. včetně hodnoty hranice intervalu) nebo otevřený a to i rozdílně pro obě meze, implicitně jsou hranice nastaveny na uzavřený interval z obou stran, tzn. do podmínky se zahrnuje včetně obou hodnot
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
//zkotroluje číslo, zda se nejadná o hodnotu okolo nuly dle možné odchylky dle toleration, předchází zvláštnímu chování výpočtů okolo nuly
double Cmy::null(double number,double tolerance)
{
	if(between(number,(-1)*tolerance,tolerance))return 0.0;
	else return number;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//zesvětlí nebo ztmaví barvu
TColor Cmy::clIntensive(TColor C,short A)//+A  míra zesvětlení,  -A  míra ztmaveni, max hodnota 255, min hodnota -255
{
	BYTE R=GetRValue(C); BYTE G=GetGValue(C); BYTE B=GetBValue(C);
	if(A>0)//zesvětlení
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
//přepošítá hodnotu posuvníku intenzity na změnu intenzity při vykreslění elementů
short Cmy::get_intensity()
{
	return(-3.6*F->scGPTrackBar_intenzita->Value+360);
}
/////////////////////////////////////////////////////////////////////////////
//vrátí černou pro světlé barvy a bílou pro tmavé barvy, používá se např. na volbu barvy textu s barevným pozadím
TColor Cmy::getBlackOrWhiteInvert(TColor color)
{
	if(color>(TColor)RGB(128,128,128))return clBlack; else return clWhite; //ShowMessage("černá");else ShowMessage("bílá");
}
/////////////////////////////////////////////////////////////////////////////
//vratí dle hodnoty input barvu z nadefinované barevné palety, v případě "přetečení barvy opakuje
TColor Cmy::getColorOfPalette(unsigned int input)
{
	switch(input%5)
	{
		case 0:return TColor RGB(0,0,0);;break;
		case 1:return TColor RGB(4,139,168);break;
		case 2:return TColor RGB(246,249,48);break;
		case 3:return TColor RGB(22,166,55);break;
		case 4:return TColor RGB(94,58,38);break;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
short Cmy::getValueFromPosition(long GlobalValue,short Position)
{
	double base=GlobalValue/pow(10.0,Position);
	return floor(base*10)-floor(base)*10;//nutné takto (jinak problémy s 1->0.999998 atd.
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//vrátí náhodné číslo v daném rozsahu o požadované přesnosti, MIN a MAX mohou být součástí navrácených hodnot (jedná se o oboustranně uzavřený interval - implicitně), pokud není nastaveno jinak posledními dvěma parametry, které umožňují nastavení z jedne či obou stran otevřený interval
//srand(time(NULL)); v knihovně ktéré se tato metoda volá je nutno volat jako úplně první toto (nejvýše v kódu knihovny)
double Cmy::RAND(double MIN, double MAX,unsigned short precision,bool MINin,bool MAXin)
{
	double P=pow(10.0,precision);
	if(!MINin)MIN+=1/P;if(!MAXin)MAX-=1/P;//pokud se jedná z jedné strany o otevřený interval
	return fmod(rand(),(MAX+1/P-MIN)*P)/P+MIN;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//vrátí Zoom upravený na používané stupnici
double Cmy::getZoomValue(double Zoom)
{
	if(Zoom<=0.25 || (Zoom>=0.25 && Zoom<0.5))Zoom=0.25;
	if(Zoom>=0.5 && Zoom<1)Zoom=0.5;
	if(Zoom>=1 && Zoom<1.5)Zoom=1;
	if(Zoom>=1.5 && Zoom<2)Zoom=1.5;
	if(Zoom>=2 && Zoom<2.5)Zoom=2;
	if(Zoom>=2.5 && Zoom<3)Zoom=2.5;
	if(Zoom>=3 && Zoom<3.5)Zoom=3;
	if(Zoom>=3.5 && Zoom<4)Zoom=3.5;
	if(Zoom>=4 && Zoom<4.5)Zoom=4;
	if(Zoom>=4.5 && Zoom<5)Zoom=4.5;
	if(Zoom>=5 && Zoom<6)Zoom=5;
	if(Zoom>=6 && Zoom<7)Zoom=6;
	if(Zoom>=7 && Zoom<8)Zoom=7;
	if(Zoom>=8 && Zoom<9)Zoom=8;
	if(Zoom>=9 && Zoom<10)Zoom=9;
	if(Zoom>=10 && Zoom<15)Zoom=10;
	if(Zoom>=15 && Zoom<20)Zoom=15;
	if(Zoom>=20 && Zoom<25)Zoom=20;
	if(Zoom>=25 && Zoom<30)Zoom=25;
	if(Zoom>=30)Zoom=30;
	return Zoom;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//Zoom=0.25;
//Zoom=0.5;
//Zoom=1;
//Zoom=1.5;
//Zoom=2;
//Zoom=2.5;
//Zoom=3;
//Zoom=3.5;
//Zoom=4;
//Zoom=4.5;
//Zoom=5;
//Zoom=6;
//Zoom=7;
//Zoom=8;
//Zoom=9;
//Zoom=10;
//Zoom=15;
//Zoom=20;
//Zoom=25;
//Zoom=30;
