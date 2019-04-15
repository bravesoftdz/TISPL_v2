//---------------------------------------------------------------------------

#ifndef UnitXH
#define UnitXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormX : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
	void zmena_aRD ();
	void zmena_R ();
	void zmena_Rx ();
	void aktualizace_tab_elementu ();

public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
  void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);

	//prom�nn� pro vy�azen� OnChange metody p�i vykreslov�n� tabulek
  enum Tinput_state{NO,NOTHING,PT,LO,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif
