//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitX.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormX *FormX;
//---------------------------------------------------------------------------
__fastcall TFormX::TFormX(TComponent* Owner)
  : TForm(Owner)
{
 input_state=NO;
}
//---------------------------------------------------------------------------


void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
input_state=NOTHING;
}

void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
//

}

//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
    if(input_state==NOTHING)
    {
         Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
         if(ID>100000)
         {
        //switch pohon - vypocty

         }
         else
         {

          switch(E->eID)
           {
             		case 0: // stop
                {

                } break;


                case 1: //robot (kontinuální)
                {
                     if(Col==1 && Row==1) //vstup PT -> vystup LO
                     {
                      input_state=PT; //nastaveni stavu
                      double PT = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
                      double LO = /*F->pom_temp->pohon->aRD*/5.0 * PT; //nahradit aRD                                      //vypocet
                      E->mGrid->Cells[Col][Row+1].Text = F->outLO(LO); //OUTPUT
                      }
                     if(Col==1 && Row==2) //vstup LO -> vystup PT
                     {
                      input_state=LO; //nastaveni stavu
                      double LO = F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
                      double PT = LO / 5.0/*F->pom_temp->pohon->aRD*/;       //nahradit aRD             //vypocet
                      E->mGrid->Cells[Col][Row-1].Text = F->outPT(PT);  //OUTPUT
                     }
                } break;

                 case 2: //robot se stop stanicí
                {

                } break;

                 case 3: //robot s pasivní otoèí
                {

                } break;

           }

         }
       E=NULL;delete E;
       F->REFRESH();
       input_state=NOTHING;
    }


}
