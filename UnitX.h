//---------------------------------------------------------------------------

#ifndef UnitXH
#define UnitXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormX : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
  enum Tinput_state{NO,NOTHING,PT,LO,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...

	void zmena_aRD (double aRD);
	void zmena_R (double R);
	void zmena_Rx (double Rx);

public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
	bool vstoupeno_poh, vstoupeno_elm;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif
