//---------------------------------------------------------------------------
#pragma hdrstop
#include "PO_math.h"
#include "parametry_linky.h"
#include "MyString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT
void TPO_math::input_CT(bool prepocet_K)
{
	switch (rezim)
	{
		case 0:
    RD=DD/CT;//test min.RD
		break;//S&G
		case 1://Kontinuál
			if(RD_locked)DD=RD*CT;//pro pøípad RD zamèeno, CT odemèeno (zajištìno v PO)
			if(DD_locked)//pro pøípad DD zamèeno, CT odemèeno (zajištìno v PO)
			{
				RD=DD/CT;
				M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice a za výpoètem RD
			}
			if(prepocet_K)K=CT/TT;//výpoèet kapacity
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2://PP
			if(prepocet_K)K=CT/TT;//výpoèet kapacity
			DD=K*(UDV()+M);//délky kabiny
			RD=DD/CT;//test min.RD
			P=Pozice();//výpoèet poètu pozic
		break;
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD
void TPO_math::input_RD(bool prepocet_M)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(CT_locked)DD=RD*CT;//pro pøípad CT zamèeno, RD odemèeno (zajištìno v PO)
			if(DD_locked)CT=DD/RD;//pro pøípad DD zamèeno, RD odemèeno (zajištìno v PO)
			if(prepocet_M)M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice
			K=CT/TT;//výpoèet kapacity, v pøípadì zamèeného CT se nic nemìní
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2://ROSTA EDIT
    	if(CT_locked)DD=RD*CT;//pro pøípad CT zamèeno, RD odemèeno (zajištìno v PO)
			if(DD_locked)CT=DD/RD;//pro pøípad DD zamèeno, RD odemèeno (zajištìno v PO)
			if(prepocet_M)M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice
			K=CT/TT;//výpoèet kapacity, v pøípadì zamèeného CT se nic nemìní
			P=Pozice();//výpoèet poètu pozic
    break;//PP
	}
}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny DD
void TPO_math::input_DD()
{
	switch (rezim)
	{
		case 0:RD=DD/CT;//test min.RD
		break;//S&G
		case 1://Kontinuál
			if(RD_locked)CT=DD/RD;//pro pøípad RD zamèeno, DD odemèeno (zajištìno v PO)
			if(CT_locked)
			{
				RD=DD/CT;//pro pøípad CT zamèeno, DD odemèeno (zajištìno v PO)
				M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice
			}
			K=CT/TT;//výpoèet kapacity
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2:
			K=DD/(UDV()+M);//výpoèet kapacity
			CT=TT*K;//výpoèet CT
			RD=DD/CT;//test min.RD
			P=Pozice();//výpoèet poètu pozic
		break;//PP
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny K
void TPO_math::input_K(bool prepocet_CT)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(prepocet_CT)CT=TT*K;//výpoèet CT
			input_CT(false);//+ zakazaný zpìtný pøepoèet K
			break;
		case 2://PP
			if(prepocet_CT)CT=TT*K;//výpoèet CT
			DD=K*(UDV()+M);//délky kabiny
			P=Pozice();//výpoèet poètu pozic
		break;
	}
}
//---------------------------------------------------------------------------
void TPO_math::input_P(bool prepocet_CT)//pøepoèet souvisejících hodnot vyplývajících ze zmìny P
{
	K=P2K();//prvnì si vrátím zjištìnou kapacitu
	input_K(prepocet_CT);//potom pracuji jako pøi vkládání kapacity
	//možná zde bude nutné zakázat zpìtný pøepoèet P
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny kritické vozíkové mezery
void TPO_math::input_M()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			RD=(UDV()+M)/TT;//dle mezery spoèítám novou rychlost
			input_RD();//døíve se používalo input_RD(false) znovu nepøepoèítává mezeru
			break;
		case 2://PP
			if(DD_locked)input_DD();
			else input_K();
			break;
	}
	MJ=M+fabs(m.UDJ(dJ,sJ,Rotace)-m.UDV(dJ,sJ,Rotace));
	MP=M+fabs(dP-m.UDV(dJ,sJ,Rotace));
}
//---------------------------------------------------------------------------
//vrátí velikost mezery dle aktuální rychlosti RD, nehledí na rozteè, ale rovnou poèítá.
double TPO_math::Mezera(bool prepocet_Rzalezitosti)
{
	double mezera=RD*TT-m.UDV(dJ,sJ,Rotace);
	if(prepocet_Rzalezitosti)Rx=m.Rx(dJ,sJ,Rotace,mezera,R);
	if(prepocet_Rzalezitosti)Rz=m.Rz(dJ,sJ,Rotace,mezera);
	MJ=RD*TT-m.UDJ(dJ,sJ,Rotace);
	MP=RD*TT-dP;
	return mezera;
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny mezery jigu
void TPO_math::input_MJ()
{
	M=MJ-fabs(m.UDJ(dJ,sJ,Rotace)-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny mezery podvozku
void TPO_math::input_MP()
{
	M=MP-fabs(dP-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny rozestupu v metrech
void TPO_math::input_Rz(bool prepocet_Rx)
{
	RD=Rz/TT;
	if(prepocet_Rx)Rx=Rz/R;//prepocet_Rx - pro pøepoèet z input_Rx
	input_RD(false);//vypoèítá CT,DD,K,P dle nastavených zámkù
	M=Mezera(false);//vypoèítá mezery
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny rozestupu v poètu palcù
void TPO_math::input_Rx()
{
	Rz=Rx*R;
	input_Rz(false);//false aby se znovu nepøepoèítavlo Rx zpìtnì z Rz
}
//---------------------------------------------------------------------------
//vrátí poèet pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
double TPO_math::Pozice()
{
	return K2P(K);
}
//---------------------------------------------------------------------------
//vrátí poèet pozic z kapacity, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
double TPO_math::K2P(double K)
{
	double P=floor(K);//celoèíselná kapacita
	double DV=UDV();
	double DVM=(DV+M)*(K-P);//délka èásti poslední vozíko-mezery v kabinì
	if(DVM>=DV)P++;//navýší o celý vozík, protože je minimálnì celý vozík v kabinì
	else P+=DVM/DV;//navýší o èást vozíku, protože je jenom èást vozíku v kabinì
	return P;
}
//---------------------------------------------------------------------------
double TPO_math::P2K(double P)//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
{
	if(P-floor(P)>0)//pokud je zadán neceloèíselný poèet pozic
	{
		return (floor(P)*(UDV()+M)+(UDV()*(P-floor(P))))/(UDV()+M);
	}
	else//celoèíselný
	{
		return (P*UDV()+(P-1)*M)/(UDV()+M);
	}
}
//---------------------------------------------------------------------------
//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. situace, že K==P
double TPO_math::P2K()
{
	return P2K(P);
}
//---------------------------------------------------------------------------
//vrátí užitnou délku vozíku dle hodnoty rotace
double TPO_math::UDV()
{
	return m.UDV(dJ,sJ,Rotace);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//alokuje ErrorList o velikosti RowCount (typicky mGrid->RowCount)
void TPO_math::createErrorList(long RowCount)
{
	ErrorList=new AnsiString[RowCount];//0 øádek zùstane nevyužit
}
//---------------------------------------------------------------------------
//vrátí slouèený ErrorText z ErrorListu (resp. jednotlivých øádkù)
AnsiString TPO_math::getErrorText(long RowCount,AnsiString seperator)
{
	AnsiString RET="";
	bool jiz_byl_zaznam=false;

	for(long i=1;i<RowCount;i++)
	{
		if(!ErrorList[i].IsEmpty())
		{ //mimo prvního øádku
			if(jiz_byl_zaznam)RET+=seperator;//pokud následuje chybový text, tak odøádkuje
			RET+=ErrorList[i];
			jiz_byl_zaznam=true;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
//odstraní ErrorList z pamìti
void TPO_math::deleteErrorList()
{
	//delete[]ErrorList;
}
//---------------------------------------------------------------------------
void TPO_math::gapoVALIDACE(Cvektory::TObjekt *objekty,long Row,long RowCount,short aRDunit)
{
	////instance
	TMyString ms;

	////jednotky
	AnsiString aRDunitT="m/s";if(aRDunit)aRDunitT="m/min";
	AnsiString T="";

	////VALIDACE aRD rozsahu od-do, pro objekty s pohony a pro všechny pohony
	if(objekty[Row].pohon!=NULL)//tato validace se neøeší pro objekty bez pøiøazených pohonù
	{
	 	long 	plRow  = 0;//n øádku pohonu na PL
		if(objekty[Row].pohon!=NULL && rezim!=100) plRow =	Form_parametry_linky->getROW(objekty[Row].pohon->n);//pro všechny objekty s pohony
		if(objekty[Row].id>=100) plRow = Form_parametry_linky->getROW(objekty[Row].id-100);//pro nepoužívané pohony (bez pøiøazení)
		double aRD_od=0;//ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[2][plRow])/(1+59.0*aRDunit);
		double aRD_do=0;//ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[3][plRow])/(1+59.0*aRDunit);
		if(!Form1->m.between(RD,aRD_od,aRD_do))
		{
			T="";//"Rozsah rychlosti "+m.round2double(aRD_od*(1+59.0*aRDunit),2,"..")+" až "+m.round2double(aRD_do*(1+59.0*aRDunit),2,"..")+" "+aRDunitT+" pohonu "+Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[1][plRow]+" neopovídá nastavované rychlosti "+F->m.round2double(RD*(1+59.0*aRDunit),2,"..")+" "+aRDunitT+"!";
		}
	}

	////testování pouze pro objekty s pohonem
	if(objekty[Row].pohon!=NULL)//testování, zda objekt má pøiøazen pohon, pokud by byl jen pohon bez pøiøazení, jedná se o "objekt 100 v režimu 100"
	{
		 ////VALIDACE neceloèíselné Rx
		 if(!m.cele_cislo(Rx))
		 {
			if(T!="")T+="<br>";//pokud existuje již pøedchozí chybový záznam a bude následovat další chybový je nutné odøádkovat
			T+="Hodnota rozestupu (Rx) není celoèíselná!";
		 }

		 ////VALIDACE hlídání pøejezdu
		 if(objekty[Row].rezim<100)//testovaný objekt má pøiøazen pohon (úvodní podmínka výšše) a zároveò se nejedná o pohon bez pøiøazení (nepoužívaný) k objektùm (tato podmínka)
		 {
			 AnsiString error_text="";
			 if(objekty[Row].rezim==0 || objekty[Row].rezim==2)//situace 1 - pøípad testování zda daný objekt (v S&G èi PP), který se mìní (objekty[Row]) je co se týèe pøejezdu OK
			 {
				 //4.12.2018 zdm,
				 /*
				 double MT=objekty[Row].MT1+objekty[Row].MT2; //MT by mìlo být zaktualizované dle gapo zmìny
				 double WT=objekty[Row].WT1-objekty[Row].WT2;//otzka je jak dodat WT popø. PT, mìly by být zaktualizované
				 //vrátí rozdíl aktuální rychlosti pohonu a potøebné k uskuteèní pøejezdu, pokud je hodnota 0 je v poøádku, je-li záporná, pøejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
				 error_text=F->d.v.kontrola_rychlosti_prejezdu(F->d.v.vrat_objekt(objekty[Row].n),CT,MT,WT,RD,DD,aRDunit);
				 */
			 }
			 else//situace 2 - testování, zda zmìna u daného KK objektu nezpùsobí problém u jiného PP èi SG objektu (objekty[i].pohon), projede všechny dotèené pp a sg z dané skupiny, kde se kliklo
			 {
				 for (unsigned long i=1;i<(unsigned)RowCount;i++)//projde všechny zobrazené objekty
				 {
					 if(objekty[i].pohon!=NULL)//testovaný objekt musí mít pohon
					 {                           //odfiltrování situace 1 tzn. objekty[Row]!=objekty[i] tj. stejný objekt - to nyní probíhá automaticky, protože se porovnávají režimy KK a proti tomu S&G èi PP, tj. nemùže se porovnávat totožný objekt
						 if(objekty[Row].pohon->n==objekty[i].pohon->n && (rezim==0 || rezim==2))//nalezen objekt ze stejné skupiny (stejný pohon) v režimu S&G èi PP, nutno tedy testovat možnost pøejezdu
						 {
						   //4.12.2018 zdm,
							 /*
							 double MT=objekty[i].MT1+objekty[i].MT2; //MT by mìlo být zaktualizované dle gapo zmìny
							 double WT=objekty[i].WT1+objekty[i].WT2;//otzka je jak dodat WT popø. PT, mìly by být zaktualizované
							 //vrátí rozdíl aktuální rychlosti pohonu a potøebné k uskuteèní pøejezdu, pokud je hodnota 0 je v poøádku, je-li záporná, pøejezd se nestíhá o danou hodnotu v m/s, je-li kladná, je aktuální rychlost o danou hodnoutu hodnotu v m/s vyšší
							 AnsiString error_text2=F->d.v.kontrola_rychlosti_prejezdu(F->d.v.vrat_objekt(objekty[i].n),CT,MT,WT,RD,DD,aRDunit);
							 if(error_text!="")error_text+="<br>"+error_text2;else error_text+=error_text2;//pokud existuje již pøedchozí chybový záznam a bude následovat chybový je nutné odøádkovat
							 */
						 }
					 }
				 }
			 }
			 //výpis problém s rychlostí pøejezdu
			 if(error_text!="")error_text="Následujícím objektùm neodpovídá rychlost pøejezdu:<br>"+error_text;//pokud je chybový text, tak pøidá popis problému
			 //vrácení celkového výpisu
			 if(error_text!="" && T!="")T+="<br>";//pokud existuje již pøedchozí chybový záznam (o rozmezí èi Rx) a bude následovat chybový o pøejezdu je nutné odøádkovat
			 T+=error_text;//pokud je chybový text i ohlednì pøejezdu, tak pøidá/vrátí popis problému
		 }
	}

	////nesprávné hodnoty
	AnsiString error_text="";
	if(F->m.null(CT)<=0 && rezim!=100)error_text+="CT ("+AnsiString(CT)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(RD)<=0 && rezim!=100)error_text+="RP ("+AnsiString(RD)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(DD)<=0 && rezim!=100)error_text+="DD ("+AnsiString(DD)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(K)<=0 && rezim!=100) error_text+="K ("+AnsiString(K)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(P)<=0 && rezim!=100) error_text+="P ("+AnsiString(P)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(M)<0 && rezim!=100)  error_text+="kritická mezera ("+AnsiString(M)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(MJ)<0 && rezim!=100) error_text+="mezera jig ("+AnsiString(MJ)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(MP)<0 && rezim!=100) error_text+="mezera podvozek ("+AnsiString(MP)+") ";//neøeší se pro nepoužité pohony
	if(F->m.null(R)<=0 && objekty[Row].pohon!=NULL) error_text+="R ("+AnsiString(R)+") "; //neøeší se pro objekty bez pohonù
	if(F->m.null(Rz)<=0 && objekty[Row].pohon!=NULL)error_text+="Rz ("+AnsiString(Rz)+") ";//neøeší se pro objekty bez pohonù
	if(F->m.null(Rx)<=0 && objekty[Row].pohon!=NULL)error_text+="Rx ("+AnsiString(Rx)+") ";//neøeší se pro objekty bez pohonù
	if(error_text!="" && T!="")T+="<br>";//pokud existuje již pøedchozí chybový záznam (o rozmezí èi Rx) a bude následovat chybový o pøejezdu je nutné odøádkovat
	if(error_text!="")T+="Neplatná hodnota: "+error_text;//pokud existuje již pøedchozí chybový záznam dodá ještì pøed nìj popisek

	//nadpis a finalizace textu
	AnsiString p="nepøiøazen";if(objekty[Row].pohon!=NULL)p=objekty[Row].pohon->name;if(objekty[Row].id>=100)p=F->d.v.vrat_pohon(objekty[Row].id-100)->name;
	AnsiString o="nepøiøazen";if(objekty[Row].id<100)o=objekty[Row].name;
	if(T!="")T="<b>Volba pro pohon "+p.UpperCase()+" objekt "+o.UpperCase()+" není možná z dùvodu:</b><br>"+T;//pokud existuje chybový záznam
	ErrorList[Row]=T;//uloží do celkového ErrorListu na pozici daného øádku, pøepíše starý text
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
