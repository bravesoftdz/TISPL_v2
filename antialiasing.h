//---------------------------------------------------------------------------

#ifndef antialiasingH
#define antialiasingH
#include <vcl.h>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class Cantialising
{
  private:

	public:
	Graphics::TBitmap *antialiasing(Graphics::TBitmap *bmp_grid, Graphics::TBitmap *bmp_in);//vr�t� bitmapu p�esampovanou pomoc� algoritmu antialiasingu
	Graphics::TBitmap *antialiasing(Graphics::TBitmap *bmp_in);//vr�t� bitmapu p�esampovanou pomoc� algoritmu antialiasingu

	protected:
};
#endif

