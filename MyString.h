//---------------------------------------------------------------------------
#ifndef MyStringH
#define MyStringH
#include <vcl.h>
//#include "my.h"
//---------------------------------------------------------------------------
//struktury
//struktury
class TMyString
{
private:

public:

  AnsiString EP(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset=0, unsigned short int stopoffset=0);//parsov�n� textu mezi zadan�mi texty, pop�. mo�no vyu��t offsetu  - ELEMENT PARSER
  UnicodeString TMyString::EP(UnicodeString Text,UnicodeString StartElement, UnicodeString StopElement, unsigned short int startoffset, unsigned short int stopoffset);
  AnsiString EP2(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset=0, unsigned short int stopoffset=0);//parsov�n� textu mezi zadan�mi texty, pop�. mo�no vyu��t offsetu  - ELEMENT PARSER
	AnsiString replace(AnsiString Text,AnsiString Insert,AnsiString Delete);//nahrad� dan� text
	UnicodeString replace(UnicodeString Text,UnicodeString Insert,UnicodeString Delete);//nahrad� dan� text
  unsigned int count(AnsiString Text,AnsiString Find);//vr�ti po�et v�skyt� dan�ho �et�zce
  unsigned int count(UnicodeString Text,UnicodeString Find);//vr�ti po�et v�skyt� dan�ho �et�zce
  AnsiString l2h(AnsiString l);//zm�n� prvn� p�smeno na velk�
  AnsiString delete_repeat(AnsiString Text, AnsiString Text_delete,unsigned int Repeat, unsigned short int Offset=0);//sma�e v�e po zadan� text X-kr�t
  UnicodeString delete_repeat_UTF(UnicodeString Text, UnicodeString Text_delete,unsigned int Repeat, unsigned short int Offset=0);
  AnsiString delete_repeat(AnsiString Text, AnsiString Text_delete);
  UnicodeString delete_repeat(UnicodeString Text, UnicodeString Text_delete);
	AnsiString delete_repeat_all(AnsiString Text, AnsiString Text_delete);
	UnicodeString DeleteSpace(UnicodeString text);
  AnsiString TrimRightFrom(AnsiString Text,AnsiString Trim);//o��zne �etezec po znak, to co p�edch�z� znaku, vr�t�
	UnicodeString TrimRightFrom_UTF(UnicodeString Text,UnicodeString Trim);//o��zne �etezec po znak, to co p�edch�z� znaku, vr�t�
	UnicodeString TrimLeftFrom_UTF(UnicodeString  Text, UnicodeString Trim);//o��zne �etezec po znaku, to co n�sleduje znaku, vr�t�
	UnicodeString TrimLeftFromText(UnicodeString  Text, UnicodeString Trim);//o��zne �etezec od textu, to co n�sleduje textu, vr�t�
	AnsiString Win2UTF(AnsiString Text);//p�evede text v k�dov�n� WINDOWS-1250 na UTF-8
	AnsiString UTF2Win(AnsiString Text);//p�evede text v k�dov�n� UTF-8 na WINDOWS-1250
  double MyToDouble (UnicodeString Text);//p�evede UnicodeString na Double, v p��pad� ��rky p�evede na te�ku
  AnsiString c2t(AnsiString Vstup);
  AnsiString t2c(AnsiString Vstup);
  long a2i(AnsiString Vstup);
  long u2i(UnicodeString Vstup);
  long long a2ll(AnsiString Vstup);
  UnicodeString get_local_decimal();//vr�t� odd�lova� desetinn�ho m�sta na dan�m syst�mu
	UnicodeString get_local_double(double number);//vr�t� re�ln� ��slo s odd�lova� desetinn�ho m�sta na dan�m syst�mu
	double get_decimal(double number);//vr�t� re�lnou ��st ��sla
	unsigned short get_count_decimal(double number);//vr�t� po�et desetinn�ch m�st
	//TPointD HexGes2Dec(UnicodeString Loc);//p�evede ��slo z hexagesim�ln�ho tvaru na dekadick�
protected:

};
//---------------------------------------------------------------------------
#endif
