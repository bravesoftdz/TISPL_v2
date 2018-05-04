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
	return number=round(number*pow(10.0,precision))/pow(10.0,precision);
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
		try{
			return (fyzicka/Form1->Zoom+Form1->Posun.x)*Form1->m2px;
		}
		catch(...)
		{return 0;}
}
double Cmy::P2Ly(long fyzicka)
{
		try{
		return -1.0*(fyzicka/Form1->Zoom+Form1->Posun.y)*Form1->m2px;
    }
		catch(...)
		{return 0;}
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::delka(double X1,double Y1,double X2,double Y2)
{
		return sqrt(pow(X2-X1,2)+ pow(Y2-Y1,2));
}
/////////////////////////////////////////////////////////////////////////////
double Cmy::azimut(double X1,double Y1,double X2,double Y2)
{
		try{
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
double Cmy::uhel(double X1,double Y1,double X2,double Y2)
{
		try{
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
//metoda vratí minimální možnou mezeru mezi vozíky pokud je parametr mezera roven 0, v pøípadì nenulového parametru mezery vrací vhodnou nejbližší hodnotu této mezery vùèi rozmìrùm rozteè a rozmìr vozíku, pokud nebude zadaná rozteè tj. bude 0, vrací hodnotu 0
double Cmy::mezera_mezi_voziky(double dV,double sV,double rotace,double roztec,double mezera)
{
	double DV=UDV(dV,sV,rotace);//užitná délka vozíku

	if(roztec!=0)
	{
		double min_mezera=ceil(DV/roztec)*roztec-DV;//vrátí minimální možnou mezi vozíky, nepokrácené: ceil((vozik/2+vozik/2)/roztec]*roztec-vozik
		if(mezera==0)
		{
			return min_mezera;
		}
		else
		{
			if(min_mezera>=mezera || mezera<0)return min_mezera;//pokud je minimální možná mezera menší než nebo stejná jak zadaná, pøípadnì chybnì zadaná v podobì záporného èísla tak vrátí minimální možnou resp. zadanou v pøípadì ==
			else {return min_mezera+round((mezera-min_mezera)/roztec)*roztec;}//vratí nejbližší možnou mezeru mezi vozíky
		}
	}
	else return 0;//pokud nebude známa rozteè
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozestup v metrech mezi aktivními palci, byla-li zadáná správnì mezera
double Cmy::Rz(double dV,double sV,double rotace,double M)
{
	return M+UDV(dV,sV,rotace);
}
////////////////////////
//vrátí rozestup v metrech mezi aktivními palci v souvstažnosti k RD (a resp. TT)
double Cmy::Rz(double RD)
{
	return RD*Form1->d.v.PP.TT;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí rozestup v poètech palcù mezi aktivními palci, byla-li zadáná správnì mezera
double Cmy::Rx(double dV,double sV,double rotace,double M,double R)
{
	if(R==0)return 0;//nebyla definováná rozteè palcù, tudíž se nepracuje s palci
	else return (M+UDV(dV,sV,rotace))/R;
}
////////////////////////
//vrátí rozestup v poètech palcù mezi aktivními palci z RD a R (a resp. TT)
double Cmy::Rx(double RD,double R)
{
	if(R==0) 0;//nebyla definováná rozteè palcù, tudíž se nepracuje s palci
	else return Rz(RD)/R;
}
/////////////////////////////////////////////////////////////////////////////
//vrátí mezeru dle rozestupu a velikosti vozíku
double Cmy::mezera(double dV,double sV,double rotace,double Rx,double R)
{
	return (Rx*R)-UDV(dV,sV,rotace);
}
/////////////////////////////////////////////////////////////////////////////
//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
double Cmy::dopRD(double dV,double sV,double rotace,double R,double TT, double RD)
{
	if(TT==0)return 0;
	else
	{
		double DV=UDV(dV,sV,rotace);
		return (DV+mezera_mezi_voziky(dV,sV,rotace,R,TT*RD-DV))/TT;
	}
}
/////////////////////////////////////////////////////////////////////////////
//vrátí užitnou délku vozíku
double Cmy::UDV(double dV,double sV,double rotace)
{
	//postupnì rozšíøit o výpoèet dle zadaných stupòù nejenom 0 vs. 90
	double UDV=0;
	UDV=UDJ(dV,sV,rotace);
	if(UDV<F->d.v.PP.delka_podvozku)UDV=F->d.v.PP.delka_podvozku;
	return UDV;
}
/////////////////////////////////////////////////////////////////////////////
//vratí užitnou délku vozíku, parametry bere z PP
double Cmy::UDV(double rotace)
{
	return UDV(F->d.v.PP.delka_voziku,F->d.v.PP.sirka_voziku,rotace);
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
double Cmy::prejezd_voziku(double delka, double rychlost_dopravniku)
{
	if(rychlost_dopravniku==0 || delka==0)return 0;//pozor mùže být zavadìjící
	else return delka/rychlost_dopravniku;
}
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
	TCanvas *C=new(TCanvas);
	C->Handle=GetWindowDC(HWND_DESKTOP);
	C->Pen->Color=color;
	C->Pen->Mode=pmNotXor;
	C->Pen->Width=width;
	C->Brush->Style=bsClear;
	short o=floor(width/2.0);
	C->Rectangle(form->Left-o,form->Top-o,form->Left+form->Width+o,form->Top+form->Height+o);
}
/////////////////////////////////////////////////////////////////////////////
//z rychlosti v m/s vratí èas milisekundách potøebný na pøekreslení jednoho pixelu pøi daném zoomu, parametr A=je rychlost animace, kdy implicitní 1 originální rychlost - tedy 100%, pokud je parametr A=0, vrátí se vhodný èas na pøehrání kontinuální animace, metoda je vhodná na animace a simulace pro timer
double Cmy::get_timePERpx(double speed,double A)//A je akcelerace
{
	double Z=F->Zoom;if(F->antialiasing)Z/=3.0;//pokud je spuštìn antialiasing, staèí 1/3 rychlost (vyplývá z principu algoritmu AA)
	if(A==0)return F->m2px/Z/speed*1000/F->fps;//vrátí èas, tak aby se jednalo o kontinální animaci
	else return F->m2px/Z/speed*1000/A;//vrátí èas na posun o jeden pixel
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
