//---------------------------------------------------------------------------

#ifndef Tvlakno_panCreateH
#define Tvlakno_panCreateH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class vlakno_panCreate : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall vlakno_panCreate(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
