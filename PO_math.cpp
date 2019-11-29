//---------------------------------------------------------------------------
#pragma hdrstop
#include "PO_math.h"
#include "parametry_linky.h"
#include "MyString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT
void TPO_math::input_CT(bool prepocet_K)
{
	switch (rezim)
	{
		case 0:
    RD=DD/CT;//test min.RD
		break;//S&G
		case 1://Kontinu�l
			if(RD_locked)DD=RD*CT;//pro p��pad RD zam�eno, CT odem�eno (zaji�t�no v PO)
			if(DD_locked)//pro p��pad DD zam�eno, CT odem�eno (zaji�t�no v PO)
			{
				RD=DD/CT;
				M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice a za v�po�tem RD
			}
			if(prepocet_K)K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2://PP
			if(prepocet_K)K=CT/TT;//v�po�et kapacity
			DD=K*(UDV()+M);//d�lky kabiny
			RD=DD/CT;//test min.RD
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
void TPO_math::input_RD(bool prepocet_M)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(CT_locked)DD=RD*CT;//pro p��pad CT zam�eno, RD odem�eno (zaji�t�no v PO)
			if(DD_locked)CT=DD/RD;//pro p��pad DD zam�eno, RD odem�eno (zaji�t�no v PO)
			if(prepocet_M)M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			K=CT/TT;//v�po�et kapacity, v p��pad� zam�en�ho CT se nic nem�n�
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2://ROSTA EDIT
    	if(CT_locked)DD=RD*CT;//pro p��pad CT zam�eno, RD odem�eno (zaji�t�no v PO)
			if(DD_locked)CT=DD/RD;//pro p��pad DD zam�eno, RD odem�eno (zaji�t�no v PO)
			if(prepocet_M)M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			K=CT/TT;//v�po�et kapacity, v p��pad� zam�en�ho CT se nic nem�n�
			P=Pozice();//v�po�et po�tu pozic
    break;//PP
	}
}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
void TPO_math::input_DD()
{
	switch (rezim)
	{
		case 0:RD=DD/CT;//test min.RD
		break;//S&G
		case 1://Kontinu�l
			if(RD_locked)CT=DD/RD;//pro p��pad RD zam�eno, DD odem�eno (zaji�t�no v PO)
			if(CT_locked)
			{
				RD=DD/CT;//pro p��pad CT zam�eno, DD odem�eno (zaji�t�no v PO)
				M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			}
			K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2:
			K=DD/(UDV()+M);//v�po�et kapacity
			CT=TT*K;//v�po�et CT
			RD=DD/CT;//test min.RD
			P=Pozice();//v�po�et po�tu pozic
		break;//PP
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
void TPO_math::input_K(bool prepocet_CT)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			input_CT(false);//+ zakazan� zp�tn� p�epo�et K
			break;
		case 2://PP
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			DD=K*(UDV()+M);//d�lky kabiny
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
void TPO_math::input_P(bool prepocet_CT)//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny P
{
	K=P2K();//prvn� si vr�t�m zji�t�nou kapacitu
	input_K(prepocet_CT);//potom pracuji jako p�i vkl�d�n� kapacity
	//mo�n� zde bude nutn� zak�zat zp�tn� p�epo�et P
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny kritick� voz�kov� mezery
void TPO_math::input_M()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			RD=(UDV()+M)/TT;//dle mezery spo��t�m novou rychlost
			input_RD();//d��ve se pou��valo input_RD(false) znovu nep�epo��t�v� mezeru
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
//vr�t� velikost mezery dle aktu�ln� rychlosti RD, nehled� na rozte�, ale rovnou po��t�.
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
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery jigu
void TPO_math::input_MJ()
{
	M=MJ-fabs(m.UDJ(dJ,sJ,Rotace)-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery podvozku
void TPO_math::input_MP()
{
	M=MP-fabs(dP-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny rozestupu v metrech
void TPO_math::input_Rz(bool prepocet_Rx)
{
	RD=Rz/TT;
	if(prepocet_Rx)Rx=Rz/R;//prepocet_Rx - pro p�epo�et z input_Rx
	input_RD(false);//vypo��t� CT,DD,K,P dle nastaven�ch z�mk�
	M=Mezera(false);//vypo��t� mezery
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny rozestupu v po�tu palc�
void TPO_math::input_Rx()
{
	Rz=Rx*R;
	input_Rz(false);//false aby se znovu nep�epo��tavlo Rx zp�tn� z Rz
}
//---------------------------------------------------------------------------
//vr�t� po�et pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double TPO_math::Pozice()
{
	return K2P(K);
}
//---------------------------------------------------------------------------
//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double TPO_math::K2P(double K)
{
	double P=floor(K);//celo��seln� kapacita
	double DV=UDV();
	double DVM=(DV+M)*(K-P);//d�lka ��sti posledn� voz�ko-mezery v kabin�
	if(DVM>=DV)P++;//nav��� o cel� voz�k, proto�e je minim�ln� cel� voz�k v kabin�
	else P+=DVM/DV;//nav��� o ��st voz�ku, proto�e je jenom ��st voz�ku v kabin�
	return P;
}
//---------------------------------------------------------------------------
double TPO_math::P2K(double P)//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
{
	if(P-floor(P)>0)//pokud je zad�n necelo��seln� po�et pozic
	{
		return (floor(P)*(UDV()+M)+(UDV()*(P-floor(P))))/(UDV()+M);
	}
	else//celo��seln�
	{
		return (P*UDV()+(P-1)*M)/(UDV()+M);
	}
}
//---------------------------------------------------------------------------
//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. situace, �e K==P
double TPO_math::P2K()
{
	return P2K(P);
}
//---------------------------------------------------------------------------
//vr�t� u�itnou d�lku voz�ku dle hodnoty rotace
double TPO_math::UDV()
{
	return m.UDV(dJ,sJ,Rotace);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//alokuje ErrorList o velikosti RowCount (typicky mGrid->RowCount)
void TPO_math::createErrorList(long RowCount)
{
	ErrorList=new AnsiString[RowCount];//0 ��dek z�stane nevyu�it
}
//---------------------------------------------------------------------------
//vr�t� slou�en� ErrorText z ErrorListu (resp. jednotliv�ch ��dk�)
AnsiString TPO_math::getErrorText(long RowCount,AnsiString seperator)
{
	AnsiString RET="";
	bool jiz_byl_zaznam=false;

	for(long i=1;i<RowCount;i++)
	{
		if(!ErrorList[i].IsEmpty())
		{ //mimo prvn�ho ��dku
			if(jiz_byl_zaznam)RET+=seperator;//pokud n�sleduje chybov� text, tak od��dkuje
			RET+=ErrorList[i];
			jiz_byl_zaznam=true;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
//odstran� ErrorList z pam�ti
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

	////VALIDACE aRD rozsahu od-do, pro objekty s pohony a pro v�echny pohony
	if(objekty[Row].pohon!=NULL)//tato validace se ne�e�� pro objekty bez p�i�azen�ch pohon�
	{
	 	long 	plRow  = 0;//n ��dku pohonu na PL
		if(objekty[Row].pohon!=NULL && rezim!=100) plRow =	Form_parametry_linky->getROW(objekty[Row].pohon->n);//pro v�echny objekty s pohony
		if(objekty[Row].id>=100) plRow = Form_parametry_linky->getROW(objekty[Row].id-100);//pro nepou��van� pohony (bez p�i�azen�)
		double aRD_od=0;//ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[2][plRow])/(1+59.0*aRDunit);
		double aRD_do=0;//ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[3][plRow])/(1+59.0*aRDunit);
		if(!Form1->m.between(RD,aRD_od,aRD_do))
		{
			T="";//"Rozsah rychlosti "+m.round2double(aRD_od*(1+59.0*aRDunit),2,"..")+" a� "+m.round2double(aRD_do*(1+59.0*aRDunit),2,"..")+" "+aRDunitT+" pohonu "+Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[1][plRow]+" neopov�d� nastavovan� rychlosti "+F->m.round2double(RD*(1+59.0*aRDunit),2,"..")+" "+aRDunitT+"!";
		}
	}

	////testov�n� pouze pro objekty s pohonem
	if(objekty[Row].pohon!=NULL)//testov�n�, zda objekt m� p�i�azen pohon, pokud by byl jen pohon bez p�i�azen�, jedn� se o "objekt 100 v re�imu 100"
	{
		 ////VALIDACE necelo��seln� Rx
		 if(!m.cele_cislo(Rx))
		 {
			if(T!="")T+="<br>";//pokud existuje ji� p�edchoz� chybov� z�znam a bude n�sledovat dal�� chybov� je nutn� od��dkovat
			T+="Hodnota rozestupu (Rx) nen� celo��seln�!";
		 }

		 ////VALIDACE hl�d�n� p�ejezdu
		 if(objekty[Row].rezim<100)//testovan� objekt m� p�i�azen pohon (�vodn� podm�nka v���e) a z�rove� se nejedn� o pohon bez p�i�azen� (nepou��van�) k objekt�m (tato podm�nka)
		 {
			 AnsiString error_text="";
			 if(objekty[Row].rezim==0 || objekty[Row].rezim==2)//situace 1 - p��pad testov�n� zda dan� objekt (v S&G �i PP), kter� se m�n� (objekty[Row]) je co se t��e p�ejezdu OK
			 {
				 //4.12.2018 zdm,
				 /*
				 double MT=objekty[Row].MT1+objekty[Row].MT2; //MT by m�lo b�t zaktualizovan� dle gapo zm�ny
				 double WT=objekty[Row].WT1-objekty[Row].WT2;//otzka je jak dodat WT pop�. PT, m�ly by b�t zaktualizovan�
				 //vr�t� rozd�l aktu�ln� rychlosti pohonu a pot�ebn� k uskute�n� p�ejezdu, pokud je hodnota 0 je v po��dku, je-li z�porn�, p�ejezd se nest�h� o danou hodnotu v m/s, je-li kladn�, je aktu�ln� rychlost o danou hodnoutu hodnotu v m/s vy���
				 error_text=F->d.v.kontrola_rychlosti_prejezdu(F->d.v.vrat_objekt(objekty[Row].n),CT,MT,WT,RD,DD,aRDunit);
				 */
			 }
			 else//situace 2 - testov�n�, zda zm�na u dan�ho KK objektu nezp�sob� probl�m u jin�ho PP �i SG objektu (objekty[i].pohon), projede v�echny dot�en� pp a sg z dan� skupiny, kde se kliklo
			 {
				 for (unsigned long i=1;i<(unsigned)RowCount;i++)//projde v�echny zobrazen� objekty
				 {
					 if(objekty[i].pohon!=NULL)//testovan� objekt mus� m�t pohon
					 {                           //odfiltrov�n� situace 1 tzn. objekty[Row]!=objekty[i] tj. stejn� objekt - to nyn� prob�h� automaticky, proto�e se porovn�vaj� re�imy KK a proti tomu S&G �i PP, tj. nem��e se porovn�vat toto�n� objekt
						 if(objekty[Row].pohon->n==objekty[i].pohon->n && (rezim==0 || rezim==2))//nalezen objekt ze stejn� skupiny (stejn� pohon) v re�imu S&G �i PP, nutno tedy testovat mo�nost p�ejezdu
						 {
						   //4.12.2018 zdm,
							 /*
							 double MT=objekty[i].MT1+objekty[i].MT2; //MT by m�lo b�t zaktualizovan� dle gapo zm�ny
							 double WT=objekty[i].WT1+objekty[i].WT2;//otzka je jak dodat WT pop�. PT, m�ly by b�t zaktualizovan�
							 //vr�t� rozd�l aktu�ln� rychlosti pohonu a pot�ebn� k uskute�n� p�ejezdu, pokud je hodnota 0 je v po��dku, je-li z�porn�, p�ejezd se nest�h� o danou hodnotu v m/s, je-li kladn�, je aktu�ln� rychlost o danou hodnoutu hodnotu v m/s vy���
							 AnsiString error_text2=F->d.v.kontrola_rychlosti_prejezdu(F->d.v.vrat_objekt(objekty[i].n),CT,MT,WT,RD,DD,aRDunit);
							 if(error_text!="")error_text+="<br>"+error_text2;else error_text+=error_text2;//pokud existuje ji� p�edchoz� chybov� z�znam a bude n�sledovat chybov� je nutn� od��dkovat
							 */
						 }
					 }
				 }
			 }
			 //v�pis probl�m s rychlost� p�ejezdu
			 if(error_text!="")error_text="N�sleduj�c�m objekt�m neodpov�d� rychlost p�ejezdu:<br>"+error_text;//pokud je chybov� text, tak p�id� popis probl�mu
			 //vr�cen� celkov�ho v�pisu
			 if(error_text!="" && T!="")T+="<br>";//pokud existuje ji� p�edchoz� chybov� z�znam (o rozmez� �i Rx) a bude n�sledovat chybov� o p�ejezdu je nutn� od��dkovat
			 T+=error_text;//pokud je chybov� text i ohledn� p�ejezdu, tak p�id�/vr�t� popis probl�mu
		 }
	}

	////nespr�vn� hodnoty
	AnsiString error_text="";
	if(F->m.null(CT)<=0 && rezim!=100)error_text+="CT ("+AnsiString(CT)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(RD)<=0 && rezim!=100)error_text+="RP ("+AnsiString(RD)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(DD)<=0 && rezim!=100)error_text+="DD ("+AnsiString(DD)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(K)<=0 && rezim!=100) error_text+="K ("+AnsiString(K)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(P)<=0 && rezim!=100) error_text+="P ("+AnsiString(P)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(M)<0 && rezim!=100)  error_text+="kritick� mezera ("+AnsiString(M)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(MJ)<0 && rezim!=100) error_text+="mezera jig ("+AnsiString(MJ)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(MP)<0 && rezim!=100) error_text+="mezera podvozek ("+AnsiString(MP)+") ";//ne�e�� se pro nepou�it� pohony
	if(F->m.null(R)<=0 && objekty[Row].pohon!=NULL) error_text+="R ("+AnsiString(R)+") "; //ne�e�� se pro objekty bez pohon�
	if(F->m.null(Rz)<=0 && objekty[Row].pohon!=NULL)error_text+="Rz ("+AnsiString(Rz)+") ";//ne�e�� se pro objekty bez pohon�
	if(F->m.null(Rx)<=0 && objekty[Row].pohon!=NULL)error_text+="Rx ("+AnsiString(Rx)+") ";//ne�e�� se pro objekty bez pohon�
	if(error_text!="" && T!="")T+="<br>";//pokud existuje ji� p�edchoz� chybov� z�znam (o rozmez� �i Rx) a bude n�sledovat chybov� o p�ejezdu je nutn� od��dkovat
	if(error_text!="")T+="Neplatn� hodnota: "+error_text;//pokud existuje ji� p�edchoz� chybov� z�znam dod� je�t� p�ed n�j popisek

	//nadpis a finalizace textu
	AnsiString p="nep�i�azen";if(objekty[Row].pohon!=NULL)p=objekty[Row].pohon->name;if(objekty[Row].id>=100)p=F->d.v.vrat_pohon(objekty[Row].id-100)->name;
	AnsiString o="nep�i�azen";if(objekty[Row].id<100)o=objekty[Row].name;
	if(T!="")T="<b>Volba pro pohon "+p.UpperCase()+" objekt "+o.UpperCase()+" nen� mo�n� z d�vodu:</b><br>"+T;//pokud existuje chybov� z�znam
	ErrorList[Row]=T;//ulo�� do celkov�ho ErrorListu na pozici dan�ho ��dku, p�ep�e star� text
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
