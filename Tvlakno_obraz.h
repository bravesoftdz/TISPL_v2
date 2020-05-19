//---------------------------------------------------------------------------

#ifndef Tvlakno_obrazH
#define Tvlakno_obrazH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class Tvlakno_obraz : public TThread
{
private:
	long i;
	void __fastcall test();
protected:
	void __fastcall Execute();

public:
	__fastcall Tvlakno_obraz(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
