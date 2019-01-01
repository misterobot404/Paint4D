//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UGrEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
// константы курсоров
const int crFill  = 1;
const int crPlus  = 2;
const int crDraw  = 3;
const int crErase = 4;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MOpenClick(TObject *Sender)
{
 if (OpenPictureDialog1->Execute()) {
  Image3->Picture->LoadFromFile(OpenPictureDialog1->FileName);
  BitMap->Assign(Image3->Picture);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{

// задание свойств кисти основного и вспомогательного цветов
 Image1->Canvas->Brush->Color = clBlack;
 Image2->Canvas->Brush->Color = clWhite;
// заполнение окон основного и вспомогательного цветов
 Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
 Image2->Canvas->FillRect(Rect(0,0,Image2->Width,Image2->Height));
// задание ширины элемента палитры цветов
 int HW = Image4->Width / 10;
// закраска элементов палитры цветов
 for(int i = 1; i <=10; i++)
  {
   switch (i)
   {
    case 1:Image4->Canvas->Brush->Color = clBlack;
           break;
    case 2:Image4->Canvas->Brush->Color = clAqua;
           break;
    case 3:Image4->Canvas->Brush->Color = clBlue;
           break;
    case 4:Image4->Canvas->Brush->Color = clFuchsia;
           break;
    case 5:Image4->Canvas->Brush->Color = clGreen;
           break;
    case 6:Image4->Canvas->Brush->Color = clLime;
           break;
    case 7:Image4->Canvas->Brush->Color = clMaroon;
           break;
    case 8:Image4->Canvas->Brush->Color = clRed;
           break;
    case 9:Image4->Canvas->Brush->Color = clYellow;
           break;
    case 10:Image4->Canvas->Brush->Color = clWhite;
   }
   Image4->Canvas->Rectangle((i-1)*HW,0,i*HW,Image4->Height);
  }
// рисование креста на холсте - только для тестирования
  Image3->Canvas->MoveTo(0,0);
  Image3->Canvas->LineTo(Image3->Width,Image3->Height);
  Image3->Canvas->MoveTo(0,Image3->Height);
  Image3->Canvas->LineTo(Image3->Width,0);
  BitMap->Assign(Image3->Picture);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if((Sender == Image4) || SBColor->Down)
// режим установки основного и вспомогательного цветов
 {
  if(Button == mbLeft)
  {
// установка основного цвета
    Image1->Canvas->Brush->Color = ((TImage *)Sender)->Canvas->Pixels[X][Y];
    Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
  }
  else
  {
// установка вспомогательного цвета
    Image2->Canvas->Brush->Color = ((TImage *)Sender)->Canvas->Pixels[X][Y];
    Image2->Canvas->FillRect(Rect(0,0,Image2->Width,Image2->Height));
  }
 }
else
{
  X0 = X;
  Y0 = Y;
  if (SBPen->Down)
  {
// режим карандаша
    Image3->Canvas->MoveTo(X,Y);
    Image3->Canvas->Pen->Color = Image1->Canvas->Brush->Color;
  }
  else if (SBLine->Down)
// режим линии
  {
   X1 = X;
   Y1 = Y;
   Image3->Canvas->Pen->Mode = pmNotXor;
   Image3->Canvas->Pen->Color = Image1->Canvas->Brush->Color;
  }
  else if (SBBrush->Down)
// режим закраски указанной области холста
  {
   if (Button==mbLeft)
    Image3->Canvas->Brush->Color = Image1->Canvas->Brush->Color;
   else Image3->Canvas->Brush->Color = Image2->Canvas->Brush->Color;
   Image3->Canvas->FloodFill(X,Y,Image3->Canvas->Pixels[X][Y],fsSurface);
  }
  else if (SBErase->Down)
  {
// режим ластика
    R = Rect(X-6,Y-6,X+6,Y+6);
    Image3->Canvas->DrawFocusRect(R);
    Image3->Canvas->Brush->Color = Image2->Canvas->Brush->Color;
    Image3->Canvas->FillRect(Rect(X-5,Y-5,X+5,Y+5));
   }
  else if (SBRect->Down || SBRectang->Down || SBFillRec->Down)
  {
// режим работы с рамкой
    if (REnd)
// стирание прежней рамки
    {
    Image3->Canvas->DrawFocusRect(R);
    if ((X < R.Right) && (X > R.Left) && (Y > R.Top) && (Y < R.Bottom))
// режим начала перетаскивания фрагмента
    {
// установка флагов
      RDrag = true;
      REnd = false;
// запоминание начального положения перетаскиваемого фрагмента
      R0 = R;
// запоминание изображения
      BitMap->Assign(Image3->Picture);
// установка цвета кисти
      Image3->Canvas->Brush->Color = Image2->Canvas->Brush->Color;
      MCopy->Enabled = false;
      MCut->Enabled = false;
    }
    }
    else
// режим начала рисования рамки фрагмента
    {
      RBegin = true;
      REnd = false;
      R.Top = X;
      R.Bottom = X;
      R.Left = Y;
      R.Right = Y;
      Image3->Canvas->DrawFocusRect(R);
     }
   }
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UndoClick(TObject *Sender)
{
 Image3->Picture->Assign(BitMap);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
 BitMap->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SBBrushClick(TObject *Sender)
{
 if (((TSpeedButton *)Sender)->Down)
  BitMap->Assign(Image3->Picture);
 RBegin = false;
 RDrag = false;
 REnd = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image3MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
if (! Shift.Contains(ssLeft))  return;
// режим линии
if (SBLine->Down)
{
// стирание прежней линии
  Image3->Canvas->MoveTo(X0,Y0);
  Image3->Canvas->LineTo(X1,Y1);
// рисование новой линии
  Image3->Canvas->MoveTo(X0,Y0);
  Image3->Canvas->LineTo(X,Y);
// запоминание новых координат конца линии
  X1 = X;
  Y1 = Y;
}
else if (SBPen->Down)
 Image3->Canvas->LineTo(X,Y);
 else if (SBErase->Down)
   {
// режим ластика
    Image3->Canvas->DrawFocusRect(R);
    R = Rect(X-6,Y-6,X+6,Y+6);
    Image3->Canvas->DrawFocusRect(R);
    Image3->Canvas->FillRect(Rect(X-5,Y-5,X+5,Y+5));
   }
 else if ((SBRect->Down && (RBegin || RDrag))
          || SBRectang->Down || SBFillRec->Down)
 {
  if (RBegin)
   {
// Режим рисования рамки фрагмента
    Image3->Canvas->DrawFocusRect(R);
    if (X0 < X) { R.Left = X0; R.Right = X; }
    else { R.Left = X; R.Right = X0; }
    if (Y0 < Y) { R.Top = Y0; R.Bottom = Y; }
    else { R.Top = Y; R.Bottom = Y0; }
    Image3->Canvas->DrawFocusRect(R);
   }
  else if (SBRect->Down)
  {
// Режим перетаскивания фрагмента
// восстановление изображения под перетаскиваемым фрагментом
   Image3->Canvas->CopyRect(R,BitMap->Canvas,R);
// если не нажата клавиша Ctrl - стирание изображения в R0
   if (! Shift.Contains(ssCtrl))
    Image3->Canvas->FillRect(R0);
// формирование нового положения фрагмента
   R.Left = R.Left + X - X0;
   R.Right = R.Right + X - X0;
   R.Top = R.Top + Y - Y0;
   R.Bottom = R.Bottom + Y - Y0;
// запоминание положения курсора мыши
   X0 = X;
   Y0 = Y;
// рисование фрагмента в новом положении
   Image3->Canvas->CopyRect(R,BitMap->Canvas,R0);
// рисование рамки
   Image3->Canvas->DrawFocusRect(R);
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image3MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if (SBLine->Down)
{
 Image3->Canvas->MoveTo(X0,Y0);
 Image3->Canvas->LineTo(X1,Y1);
 Image3->Canvas->Pen->Mode = pmCopy;
 Image3->Canvas->MoveTo(X0,Y0);
 Image3->Canvas->LineTo(X,Y);
}
else if (SBRect->Down)
{
 if (RDrag) Image3->Canvas->DrawFocusRect(R);
 if (RBegin && ! REnd)
  {
   REnd = true;
   MCopy->Enabled = true;
   MCut->Enabled = true;
  }
}
 else if (SBRectang->Down)
 {
  Image3->Canvas->Brush->Color = Image1->Canvas->Brush->Color;
  Image3->Canvas->FrameRect(R);
 }
 else if (SBFillRec->Down)
 {
  Image3->Canvas->Brush->Color = Image2->Canvas->Brush->Color;
  Image3->Canvas->Pen->Color = Image1->Canvas->Brush->Color;
  Image3->Canvas->Rectangle(R.Left,R.Top,R.Right,R.Bottom);
 }
 else if (SBErase->Down) Image3->Canvas->DrawFocusRect(R);
 RBegin = false;
 RDrag = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MCopyClick(TObject *Sender)
{
/*
 word MyFormat;
 THandle AData;
 HPALETTE APalette*/
 Graphics::TBitmap *BMCopy = new Graphics::TBitmap;
 Image3->Canvas->DrawFocusRect(R);
 BMCopy->Width = R.Right - R.Left;
 BMCopy->Height = R.Bottom - R.Top;
 try
 {
  BMCopy->Canvas->CopyRect(Rect(0,0,BMCopy->Width,BMCopy->Height),Image3->Canvas,R);
  Image3->Canvas->DrawFocusRect(R);
/*  BMCopy->SaveToClipBoardFormat(MyFormat,AData,APalette);
  Clipboard->SetAsHandle(MyFormat,AData); */
  Clipboard()->Assign(BMCopy);
  if (Sender == MCut)
   {
    Image3->Canvas->Brush->Color = clWhite;
    Image3->Canvas->FillRect(R);
   }
 }
__finally
{
  BMCopy->Free();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MPasteClick(TObject *Sender)
{
 Graphics::TBitmap *BMCopy = new Graphics::TBitmap;
 try
 {
  try
  {
   BMCopy->LoadFromClipboardFormat(CF_BITMAP,
                  Clipboard()->GetAsHandle(CF_BITMAP),0);
//   BMCopy->Assign(Clipboard());
   Image3->Canvas->CopyRect(Rect(0,0,BMCopy->Width,BMCopy->Height),
         BMCopy->Canvas,Rect(0,0,BMCopy->Width,BMCopy->Height));
  }
  __finally
  {
   BMCopy->Free();
  }
 }
  catch (EInvalidGraphic&)
 {
  ShowMessage("Ошибочный формат графики");
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MSaveClick(TObject *Sender)
{
 if (SavePictureDialog1->Execute())
  {
    BitMap->Assign(Image3->Picture);
    BitMap->SaveToFile(SavePictureDialog1->FileName);
  }
}
//---------------------------------------------------------------------------


