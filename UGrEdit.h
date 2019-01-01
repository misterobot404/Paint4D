//---------------------------------------------------------------------------
#ifndef UGrEditH
#define UGrEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <clipbrd.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TImage *Image4;
	TSpeedButton *SBBrush;
	TSpeedButton *SBColor;
	TSpeedButton *SBRect;
	TSpeedButton *SBRectang;
	TSpeedButton *SBFillRec;
	TSpeedButton *SBErase;
	TSpeedButton *SBPen;
	TSpeedButton *SBLine;
	TMainMenu *MainMenu1;
	TMenuItem *MFile;
	TMenuItem *MOpen;
	TMenuItem *MSave;
	TMenuItem *N1;
	TMenuItem *Undo;
	TMenuItem *MCut;
	TMenuItem *MCopy;
	TMenuItem *MPaste;
	TOpenPictureDialog *OpenPictureDialog1;
	TSavePictureDialog *SavePictureDialog1;
	void __fastcall MOpenClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Image3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall UndoClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SBBrushClick(TObject *Sender);
	void __fastcall Image3MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Image3MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall MCopyClick(TObject *Sender);
	void __fastcall MPasteClick(TObject *Sender);
	void __fastcall MSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
Graphics::TBitmap *BitMap = new Graphics::TBitmap;
TRect R,R0;
int X0,Y0,X1,Y1;
bool RBegin = false, REnd = false, RDrag = false;
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

