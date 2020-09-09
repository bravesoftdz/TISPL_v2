//---------------------------------------------------------------------------

#ifndef Tvlakno_soundH
#define Tvlakno_soundH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TSound : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TSound(bool CreateSuspended);
  short A;

};
//---------------------------------------------------------------------------
#endif
