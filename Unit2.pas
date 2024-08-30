unit Unit2;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, StdCtrls, Unit1, Math;

type
  TForm2 = class(TForm)
    Memo1: TMemo;
    MainMenu1: TMainMenu;
    SaveAs1: TMenuItem;
    SaveAs2: TMenuItem;
    SaveDialog1: TSaveDialog;
    procedure createreport(Sender: TObject);
    procedure ClearFormData(Sender: TObject; var Action: TCloseAction);
    procedure btnSaveToFileClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

{$R *.dfm}

procedure TForm2.createreport(Sender: TObject);
var
  Period, i, len: Integer;
  Payment, Principal, Interest, TotalPrincipal, TotalInterest, Balance: Double;
  Line, txt: string;
begin
  len := Length(Form1.Edit1.Text);
  Form1.Format;
  Balance := StrToFloat(StringReplace(Form1.Edit1.Text, ',', '',
    [rfReplaceAll]));
  Payment := StrToFloat(StringReplace(Form1.Edit6.Text, ',', '',
    [rfReplaceAll]));
  TotalInterest := 0;
  TotalPrincipal := 0;
  Memo1.Text := 'LOAN AMORTIZATION SCHEDULE - ' + Copy(Form1.ComboBox1.Text, 1,
    10);
  Memo1.Lines.Add('');
  Memo1.Lines.Add('');
  Line := 'Loan Amount: ' + Form1.Edit1.Text + '   Interest rate: ' +
    Form1.Edit3.Text + '%   Number of payments: ' + Form1.Edit2.Text +
    '   Payment Amount: ' + Form1.Edit6.Text;
  Memo1.Lines.Add(Line);
  Memo1.Lines.Add('');
  Line := '| Period |';
  txt := ' Payment';
  for i := 1 to (Max(len, 8) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Principal';
  for i := 1 to (Max(len, 10) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Interest';
  for i := 1 to (Max(len, 9) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Total Principal';
  for i := 1 to (Max(len, 16) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Total Interest';
  for i := 1 to (Max(len, 15) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Fees';
  for i := 1 to (Max(Length(Form1.Edit4.Text), 6) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := ' Balance';
  for i := 1 to (Max(len, 15) - Length(txt)) do
    txt := ' ' + txt;
  Line := Line + txt + ' |';
  txt := '';
  for i := 1 to Length(Line) do
    txt := txt + '=';
  Memo1.Lines.Add(txt);
  Memo1.Lines.Add(Line);
  Memo1.Lines.Add(txt);
  txt := floattostrf(Balance, ffNumber, 18, 2);
  for i := 1 to (Length(Line) - Length(txt) - 2) do
    txt := ' ' + txt;
  Memo1.Lines.Add(txt);
  for Period := 1 to NumPayments do
  begin
    Interest := Round(Balance * PeriodInterest * 100) / 100;
    TotalInterest := TotalInterest + Interest;
    if Period = NumPayments then Payment := Balance + Interest + PeriodFees;
    Principal := Round((Payment - PeriodFees - Interest) * 100) / 100;
    TotalPrincipal := TotalPrincipal + Principal;
    Balance := Balance - Principal;
    Line := '';
    txt := inttostr(Period);
    for i := 1 to (7 - Length(txt)) do txt := ' ' + txt;
    Line := '|' + txt + ' |';
    txt := floattostrf(Payment, ffNumber, 18, 2);
    for i := 1 to (Max(len, 8) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := floattostrf(Principal, ffNumber, 18, 2);
    for i := 1 to (Max(len, 9) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := floattostrf(Interest, ffNumber, 18, 2);
    for i := 1 to (Max(len, 10) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := floattostrf(TotalPrincipal, ffNumber, 18, 2);
    for i := 1 to (Max(len, 16) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := floattostrf(TotalInterest, ffNumber, 18, 2);
    for i := 1 to (Max(len, 15) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := Form1.Edit4.Text;
    for i := 1 to (Max(Length(Form1.Edit4.Text), 6) - Length(txt)) do
      txt := ' ' + txt;
    Line := Line + txt + ' |';
    txt := floattostrf(Balance, ffNumber, 18, 2);
    for i := 1 to (Max(len, 15) - Length(txt)) do txt := ' ' + txt;
    Line := Line + txt + ' |';
    Memo1.Lines.Add(Line);
  end;
  Memo1.Height := Form2.ClientHeight - 2;
  Memo1.Width := Form2.Clientwidth - 2;
end;

procedure TForm2.ClearFormData(Sender: TObject; var Action: TCloseAction);
begin
  Memo1.Lines.Clear;
end;

procedure TForm2.btnSaveToFileClick(Sender: TObject);
var
  FileName: string;
begin
  SaveDialog1.Filter := 'Text files (*.txt)|*.txt|All files (*.*)|*.*';
  SaveDialog1.DefaultExt := 'txt';

  if SaveDialog1.Execute then
  begin
    FileName := SaveDialog1.FileName;
    Memo1.Lines.SaveToFile(FileName);
    ShowMessage('The report was saved as ' + FileName + #13#10 +
      ' You can open the file in any text editor for editing or printing it.');
  end;
end;

end.
