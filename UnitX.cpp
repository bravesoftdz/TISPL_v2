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
{ //v�choz� nastaven� - p�i zobrazen� tab.element� je toti� v�dy vol�n Onchange, p�i na��t�n� hodnot do bu�ek
  // proto je nastaven input_state=NO, aby v tento moment neprob�hal ��dn� v�po�et v Onchange ud�losti
 input_state=NO;
}
//---------------------------------------------------------------------------


void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
input_state=NOTHING;
}

void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
//
}

//zpracov�n� onchange ud�losti - INPUT, v�po�et a OUTPUT zp�t do ovlivn�n� bu�ky
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


                case 1: //robot (kontinu�ln�)
                {
                     if(Col==1 && Row==1) //vstup PT -> vystup LO
                     {
                      input_state=PT; //nastaveni stavu
                      E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
                      E->LO1 = /*F->pom_temp->pohon->aRD*/5.0 * E->PT1; //nahradit aRD                                      //vypocet
                      E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //OUTPUT
//                    E->mGrid->getEdit(Col,Row+1)->Text=E->mGrid->Cells[Col][Row+1].Text;
                      }
                     if(Col==1 && Row==2) //vstup LO -> vystup PT
                     {
                      input_state=LO; //nastaveni stavu
                      E->LO1 = F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
                      E->PT1 = E->LO1 / 5.0/*F->pom_temp->pohon->aRD*/;       //nahradit aRD             //vypocet
                      E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT1);  //OUTPUT
                     }
                } break;

                 case 2: //robot se stop stanic�
                {

                } break;

                 case 3: //robot s pasivn� oto��
                {

                } break;

           }

         }
       E->mGrid->Refresh();
       //V Unit1 je obsluha ud�losti p�i povolen� Timeru, kde je vol�n i REFRESH();
       F->Timer2->Enabled=true;


       E=NULL;delete E;
			 input_state=NOTHING;
    }
}
