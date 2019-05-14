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
	void aktualizace_tab_elementu_pOdebran ();
	void korelace_tab_pohonu(int Row);
	void korelace_tab_pohonu_elementy();
	void korelace_v_elementech(long ID,long Row);

	//promìnné pro UnitX
	double dopRD;

public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void odstranit_korelaci();
	void validace();
	void naplneni_dopRD();

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif

