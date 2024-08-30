unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Menus;

type
  TForm1 = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    ComboBox1: TComboBox;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Edit6: TEdit;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Edit7: TEdit;
    Edit8: TEdit;
    Edit9: TEdit;
    MainMenu1: TMainMenu;
    CreateReport1: TMenuItem;
    Help1: TMenuItem;
    procedure calculate(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure keypress(Sender: TObject; var Key: Char);
    procedure Frequencychange(Sender: TObject);
    procedure fieldexit(Sender: TObject);
    procedure Format;
    procedure Help1Click(Sender: TObject);
    procedure CreateReport1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  AmountBorrowed, PeriodPayment, TotalInterest, TotalFees, TotalAmountPaid,
    AnnualInterestRate, PeriodFees: Double;
  PaymentFrequency, NumPayments, PaymentTermYears, Periods: Integer;
  PeriodInterest, RemainingAmount: Double;
  NumPaymentsLeft, PaymentCounter: Integer;
implementation

uses Unit2;

function Power(Base, Exponent: Double): Double;
begin
  Result := Exp(Exponent * Ln(Base));
end;

procedure CalculateLoan;
begin
  // Convert annual interest rate to period
  PeriodInterest := AnnualInterestRate / Periods / 100;

  // Calculate period payment using simple amortization formula
  if PeriodInterest = 0 then
    PeriodPayment := AmountBorrowed / NumPayments + PeriodFees
  else
    PeriodPayment := (AmountBorrowed * PeriodInterest) /
      (1 - Power(1 + PeriodInterest, -NumPayments)) + PeriodFees;

  // Calculate total fees paid
  TotalFees := (PeriodFees * NumPayments);

  // Initialize variables
  RemainingAmount := AmountBorrowed;
  NumPaymentsLeft := NumPayments;
  TotalInterest := 0;
  TotalAmountPaid := 0;

  // Calculate total interest paid and total amount paid
  for PaymentCounter := 1 to NumPayments do
  begin
    TotalInterest := TotalInterest + (RemainingAmount * PeriodInterest);
    TotalAmountPaid := TotalAmountPaid + PeriodPayment;

    // Update remaining amount
    RemainingAmount := RemainingAmount - (PeriodPayment - PeriodFees - (RemainingAmount *
      PeriodInterest));
  end;
end;

{$R *.DFM}

procedure TForm1.calculate(Sender: TObject);
begin
  if (Edit1.Text = '') or (Edit2.Text = '') or (Edit3.Text = '') or (Edit4.Text =
    '') then
  begin
    Edit6.Text := '-';
    Edit7.Text := '-';
    Edit8.Text := '-';
    Edit9.Text := '-';
    Exit;
  end;
  AmountBorrowed := StrToFloat(StringReplace(Edit1.Text, ',', '',
    [rfReplaceAll]));
  NumPayments := StrToInt(Edit2.Text);
  AnnualInterestRate := StrToFloat(StringReplace(Edit3.Text, ',', '',
    [rfReplaceAll]));
  PeriodFees := StrToFloat(StringReplace(Edit4.Text, ',', '', [rfReplaceAll]));
  CalculateLoan;
  Label8.Caption := 'The term of this loan is '
    + floattostrf(NumPayments / Periods, ffNumber, 6, 2) + ' years';
  Edit6.Text := FloatToStrF(PeriodPayment, ffNumber, 18, 2);
  Edit7.Text := FloattoStrF(TotalInterest, ffNumber, 18, 2);
  Edit8.Text := FloattoStrF(TotalFees, ffNumber, 18, 2);
  Edit9.Text := FloattoStrF(TotalAmountPaid, ffNumber, 18, 2);
end;

procedure TForm1.FormCreate(Sender: TObject);

begin
  Form1.ClientWidth := Edit1.Left + Edit1.Width + 20;
  Form1.ClientHeight := Edit9.Top + Edit9.Height + 20;
  Periods := 12;
  Edit1.Text := '1,000,000.00';
  with Edit1 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit2 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit3 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit4 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit6 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit7 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit8 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
  with Edit9 do
    SetWindowlong(
      Handle, GWL_STYLE, GetWindowlong(Handle, GWL_STYLE) or ES_Right);
end;

procedure TForm1.keypress(Sender: TObject; var Key: Char);
begin
  // Allow only numeric input
  if Key = ',' then Key := '.';
  if not (Key in ['0'..'9', '.']) and not (Ord(Key) = VK_BACK) then
    Key := #0; // Suppress the key
  Form1.Calculate(Self);
end;

procedure TForm1.Frequencychange(Sender: TObject);
begin
  case ComboBox1.ItemIndex of
    0: periods := 365;
    1: periods := 360;
    2: periods := 52;
    3: periods := 26;
    4: periods := 24;
    5: periods := 12;
    6: periods := 4;
    7: periods := 2;
    8: periods := 1;
  else
    periods := 12;
  end;
  Form1.Calculate(Self);
end;

procedure TForm1.Format;
begin
  AmountBorrowed := StrToFloat(StringReplace(Edit1.Text, ',', '',
    [rfReplaceAll]));
  Edit1.Text := FloattoStrF(AmountBorrowed, ffNumber, 18, 2);
  AnnualInterestRate := StrToFloat(StringReplace(Edit3.Text, ',', '',
    [rfReplaceAll]));
  Edit3.Text := FloattoStrF(AnnualInterestRate, ffNumber, 18, 2);
  PeriodFees := StrToFloat(StringReplace(Edit4.Text, ',', '', [rfReplaceAll]));
  Edit4.Text := FloattoStrF(PeriodFees, ffNumber, 18, 2);
end;

procedure TForm1.fieldexit(Sender: TObject);
begin
  if Edit1.Text = '' then Edit1.Text := '0.00';
  if Edit2.Text = '' then Edit2.Text := '12';
  if Edit3.Text = '' then Edit3.Text := '0.00';
  if Edit4.Text = '' then Edit4.Text := '0.00';
  Format;
end;

procedure TForm1.Help1Click(Sender: TObject);
begin
  ShowMessage('LoanCalc 0.1.1 by Shpati Koleka, MIT License');
end;

procedure TForm1.CreateReport1Click(Sender: TObject);
begin
  if Form2.Showing then Form2.Close;
  Form1.Calculate(Self);
  Form2.Show;
end;

end.
