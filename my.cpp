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
	return number=round(number*pow(10.0,precision))/pow(10.0,precision);
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
//metoda vrat� minim�ln� mo�nou mezeru mezi voz�ky pokud je parametr mezera roven 0, v p��pad� nenulov�ho parametru mezery vrac� vhodnou nejbli��� hodnotu t�to mezery v��i rozm�r�m rozte� a rozm�r voz�ku, pokud nebude zadan� rozte� tj. bude 0, vrac� hodnotu 0
double Cmy::mezera_mezi_voziky(double dV,double sV,double rotace,double roztec,double mezera)
{
	double DV=UDV(dV,sV,rotace);//u�itn� d�lka voz�ku

	if(roztec!=0)
	{
		double min_mezera=ceil(DV/roztec)*roztec-DV;//vr�t� minim�ln� mo�nou mezi voz�ky, nepokr�cen�: ceil((vozik/2+vozik/2)/roztec]*roztec-vozik
		if(mezera==0)
		{
			return min_mezera;
		}
		else
		{
			if(min_mezera>=mezera || mezera<0)return min_mezera;//pokud je minim�ln� mo�n� mezera men�� ne� nebo stejn� jak zadan�, p��padn� chybn� zadan� v podob� z�porn�ho ��sla tak vr�t� minim�ln� mo�nou resp. zadanou v p��pad� ==
			else {return min_mezera+round((mezera-min_mezera)/roztec)*roztec;}//vrat� nejbli��� mo�nou mezeru mezi voz�ky
		}
	}
	else return 0;//pokud nebude zn�ma rozte�
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozestup v metrech mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
double Cmy::Rz(double dV,double sV,double rotace,double M)
{
	return M+UDV(dV,sV,rotace);
}
////////////////////////
//vr�t� rozestup v metrech mezi aktivn�mi palci v souvsta�nosti k RD (a resp. TT)
double Cmy::Rz(double RD)
{
	return RD*Form1->d.v.PP.TT;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci, byla-li zad�n� spr�vn� mezera
double Cmy::Rx(double dV,double sV,double rotace,double M,double R)
{
	if(R==0)return 0;//nebyla definov�n� rozte� palc�, tud� se nepracuje s palci
	else return (M+UDV(dV,sV,rotace))/R;
}
////////////////////////
//vr�t� rozestup v po�tech palc� mezi aktivn�mi palci z RD a R (a resp. TT)
double Cmy::Rx(double RD,double R)
{
	if(R==0) 0;//nebyla definov�n� rozte� palc�, tud� se nepracuje s palci
	else return Rz(RD)/R;
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� mezeru dle rozestupu a velikosti voz�ku
double Cmy::mezera(double dV,double sV,double rotace,double Rx,double R)
{
	return (Rx*R)-UDV(dV,sV,rotace);
}
/////////////////////////////////////////////////////////////////////////////
//vr�t� doporu�enou nejbli��� rychlost pohonu, k rychlosti zadan� tak, aby se reflektovala rozte� mezi palci i takt
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
//vr�t� u�itnou d�lku voz�ku
double Cmy::UDV(double dV,double sV,double rotace)
{
	//postupn� roz���it o v�po�et dle zadan�ch stup�� nejenom 0 vs. 90
	double UDV=0;
	UDV=UDJ(dV,sV,rotace);
	if(UDV<F->d.v.PP.delka_podvozku)UDV=F->d.v.PP.delka_podvozku;
	return UDV;
}
/////////////////////////////////////////////////////////////////////////////
//vrat� u�itnou d�lku voz�ku, parametry bere z PP
double Cmy::UDV(double rotace)
{
	return UDV(F->d.v.PP.delka_voziku,F->d.v.PP.sirka_voziku,rotace);
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
double Cmy::prejezd_voziku(double delka, double rychlost_dopravniku)
{
	if(rychlost_dopravniku==0 || delka==0)return 0;//pozor m��e b�t zavad�j�c�
	else return delka/rychlost_dopravniku;
}
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
//z rychlosti v m/s vrat� �as milisekund�ch pot�ebn� na p�ekreslen� jednoho pixelu p�i dan�m zoomu, parametr A=je rychlost animace, kdy implicitn� 1 origin�ln� rychlost - tedy 100%, pokud je parametr A=0, vr�t� se vhodn� �as na p�ehr�n� kontinu�ln� animace, metoda je vhodn� na animace a simulace pro timer
double Cmy::get_timePERpx(double speed,double A)//A je akcelerace
{
	double Z=F->Zoom;if(F->antialiasing)Z/=3.0;//pokud je spu�t�n antialiasing, sta�� 1/3 rychlost (vypl�v� z principu algoritmu AA)
	if(A==0)return F->m2px/Z/speed*1000/F->fps;//vr�t� �as, tak aby se jednalo o kontin�ln� animaci
	else return F->m2px/Z/speed*1000/A;//vr�t� �as na posun o jeden pixel
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
