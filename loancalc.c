// Compile with: tcc LoanCalc.c -lcomctl32 -lcomdlg32 -mwindows

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IDI_MYICON 101

#define ID_EDIT1 1001
#define ID_COMBOBOX 1002
#define ID_EDIT3 1003
#define ID_EDIT4 1004
#define ID_EDIT5 1005

#define ID_MENU_ITEM1 2001
#define ID_MENU_ITEM2 2002

#define ID_FILE_SAVE 3001

// Modern color scheme
#define BACKGROUND_COLOR RGB(240, 240, 240)
#define TEXT_COLOR RGB(51, 51, 51)

const FWIDTH = 180;
const FHEIGHT = 23;
const HSPACER = 12;

const char* FONT = "Arial"; 
const char* FONT2 = "Courier New"; 

HICON hIcon;

HFONT hFont, hFont2, hFontBold, hFontBoldBig;
HBRUSH hBackgroundBrush;
HWND hEdit0, hEdit1, hEdit2, hEdit3, hEdit4, hEdit5, hEdit6, hEdit7, hEdit8, hLabel, mainForm;

// Global variables for the memo form
HWND hMemoForm = NULL;
HWND hMemoEdit = NULL;
HFONT hMemoFont = NULL;

// Add these with your other function prototypes
LRESULT CALLBACK MemoFormProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowMemoForm(HWND hParent);

double principal, numberOfPayments, annualInterestRate, periodfees, periodpayment, totalinterest, totalfees, totalpayment, yearsofloan;
int paymentsperyear = 12, loaded = 0;
char lbl[40];

// Function declaration
double GetNr(HWND hEdit) {
    char buffer[50]; // Buffer to hold the edit box text
    GetWindowText(hEdit, buffer, sizeof(buffer)); // Get text from the edit box

    // Remove spaces from the string
    char noSpaces[50];
    int j = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] != ' ') {
            noSpaces[j++] = buffer[i]; // Copy non-space characters
        }
    }
    noSpaces[j] = '\0'; // Null-terminate the string

    // Convert the cleaned string to a double
    double number = atof(noSpaces); // Use atof or sscanf depending on your needs

    return number; // Return the numeric value
}

char* formatNumber(double num, int decimalPlaces) {
    char buffer[50];
    
    // Create the format string dynamically based on decimalPlaces
    char format[10];
    snprintf(format, sizeof(format), "%%.%df", decimalPlaces); // E.g., "%.2f" for 2 decimal places

    // Format the number with the specified decimal places
    snprintf(buffer, sizeof(buffer), format, num);

    int len = strlen(buffer);
    int decimalPoint = len - (decimalPlaces + 1);  // Position of the decimal point
    int spaceCount = (decimalPoint - 1) / 3;       // Number of spaces to add
    int newLen = len + spaceCount;

    // Allocate memory for the new formatted string
    char *output = (char*)malloc(newLen + 1);  // +1 for null terminator
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    output[newLen] = '\0';  // Null-terminate the new output string

    // Start copying the string backwards, adding spaces where appropriate
    int j = newLen - 1;
    for (int i = len - 1, count = 0; i >= 0; i--, j--) {
        output[j] = buffer[i];
        if (i < decimalPoint && ++count == 3) {
            j--;
            output[j] = ' ';  // Add space as thousand separator
            count = 0;
        }
    }

    return output;  // Return the formatted string
}

// Function to calculate mortgage payment
double calculateMortgage(double principal, double annualInterestRate, int totalPeriods, int periodsPerYear) {
    double monthlyInterestRate = annualInterestRate / 100 / periodsPerYear;
    double denominator = pow(1 + monthlyInterestRate, totalPeriods) - 1;
    
    // Check if denominator is zero (handles case when interest rate is 0)
    if (denominator == 0) {
        return principal / totalPeriods; // Equal payments if no interest
    }

    // Mortgage payment formula
    double mortgagePayment = (principal * monthlyInterestRate * pow(1 + monthlyInterestRate, totalPeriods)) / denominator;
    return mortgagePayment;
}

void update_result() {
    loaded = 0;
    char buffer[24];
    char* endPtr;

    // Get the text from the edit box
    principal = GetNr(hEdit0);
    numberOfPayments = GetNr(hEdit2);
    annualInterestRate = GetNr(hEdit3);
    periodfees = GetNr(hEdit4);
    
    yearsofloan = numberOfPayments / paymentsperyear;
    periodpayment = calculateMortgage(principal, annualInterestRate, numberOfPayments, paymentsperyear) + periodfees;

    totalfees = periodfees * numberOfPayments;
    totalpayment = periodpayment * numberOfPayments;
    totalinterest = totalpayment - principal - totalfees;

    SetWindowText(hEdit5, formatNumber(periodpayment, 2));
    SetWindowText(hEdit6, formatNumber(totalinterest, 2));
    SetWindowText(hEdit7, formatNumber(totalfees, 2));
    SetWindowText(hEdit8, formatNumber(totalpayment, 2));

    char str[40];
    sprintf(str, "The term of this loan in years is %.2f", yearsofloan);
    SetWindowText(hLabel, str);

    loaded = 1;
}

// Function to add menu items
void AddMenus(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    
    // Create the first menu item
    AppendMenu(hMenu, MF_STRING, ID_MENU_ITEM1, "Amortization Schedule");
    // Create the second menu item
    AppendMenu(hMenu, MF_STRING, ID_MENU_ITEM2, "About");
    
    // Attach the menu to the window
    SetMenu(hwnd, hMenu);
}

// Function to create the amortization report
void CreateReport(HWND hMemoEdit) {
    char buffer[1024];
    char line[256];
    double balance, payment, totalInterest, totalPrincipal;
    int numPayments;

    // Clear the memo
    SetWindowText(hMemoEdit, "");

    // Get values from the main form (you'll need to implement a way to access these)
    balance = GetNr(hEdit0);
    payment = GetNr(hEdit5);
    numPayments = (int)GetNr(hEdit2);

    // Create the report header
    sprintf(buffer, "LOAN AMORTIZATION SCHEDULE - %d payment(s) per year\r\n\r\n", paymentsperyear);
    SendMessage(hMemoEdit, EM_REPLACESEL, 0, (LPARAM)buffer);

    sprintf(buffer, "Loan Amount: %s   Interest rate: %.2f%%   Number of payments: %d   Payment Amount: %s\r\n\r\n",
            formatNumber(balance, 2), annualInterestRate , numPayments, formatNumber(payment,2));
    SendMessage(hMemoEdit, EM_REPLACESEL, 0, (LPARAM)buffer);

    // Create the table header
    sprintf(buffer, "| Prd. |        Payment |      Principal |       Interest | acc. Principal |  acc. Interest |   Fees |        Balance |\r\n");
    SendMessage(hMemoEdit, EM_REPLACESEL, 0, (LPARAM)buffer);
    sprintf(buffer, "=======================================================================================================================\r\n");
    SendMessage(hMemoEdit, EM_REPLACESEL, 0, (LPARAM)buffer);

    // Calculate and add amortization data
    totalInterest = 0;
    totalPrincipal = 0;

    for (int period = 1; period <= numPayments; period++) {
        double interest = balance * (annualInterestRate / 100 / paymentsperyear);
        interest = round(interest * 100) / 100.0f;
        double principal = payment - interest - periodfees;
        principal = round(principal * 100) / 100.0f;
        if (period == numPayments && balance != principal) {
            principal = balance;
        }
        payment = principal + interest + periodfees;
        totalInterest += interest;
        totalPrincipal += principal;
        balance -= principal;
        
        //sprintf(line, "| %4d | %7.2f | %9.2f | %10.2f | %14.2f | %13.2f | %5.2f | %11.2f |\r\n",
        sprintf(line, "| %4d | %14s | %14s | %14s | %14s | %14s | %6s | %14s |\r\n",
                period, formatNumber(payment, 2), formatNumber(principal, 2), formatNumber(interest, 2), formatNumber(totalPrincipal, 2), formatNumber(totalInterest, 2), formatNumber(periodfees, 2), formatNumber(balance, 2));
        SendMessage(hMemoEdit, EM_REPLACESEL, 0, (LPARAM)line);
    }
}

// Function to clear the memo
void ClearFormData(HWND hMemoEdit) {
    SetWindowText(hMemoEdit, "");
}

// Function to save the report to a file
void SaveToFile(HWND hMemoEdit) {
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hMemoForm;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "txt";

    if (GetSaveFileName(&ofn)) {
        FILE *file = fopen(szFileName, "w");
        if (file) {
            int textLength = GetWindowTextLength(hMemoEdit);
            char *text = (char*)malloc(textLength + 1);
            GetWindowText(hMemoEdit, text, textLength + 1);
            // Replace \r\n with \n before writing to the file
            for (int i = 0; i < textLength; i++) {
                if (text[i] == '\r' && text[i + 1] == '\n') {
                    // Shift the rest of the string left to remove the '\r'
                    memmove(&text[i], &text[i + 1], textLength - i);
                    textLength--;  // Adjust the length after removing '\r'
                }
            }
            fprintf(file, "%s", text);
            fclose(file);
            free(text);
            MessageBox(hMemoForm, "The loan amortization schedule was saved successfully.", "Save Complete", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(hMemoForm, "Failed to save the loan amortization schedule.", "Save Error", MB_OK | MB_ICONERROR);
        }
    }
}

// Window procedure for the memo form
LRESULT CALLBACK MemoFormProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            EnableWindow(mainForm, FALSE);
            // Create a multi-line edit control (memo)
            hMemoEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
                10, 10, 380, 280, hwnd, NULL, NULL, NULL);
            SendMessage(hMemoEdit, EM_SETLIMITTEXT, (WPARAM)0xFFFFFFFF, 0);
            
            // Create a new font
            hMemoFont = CreateFont(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New");
            
            // Set the new font for the memo
            SendMessage(hMemoEdit, WM_SETFONT, (WPARAM)hMemoFont, TRUE);

            // Create a menu
            HMENU hMenu = CreateMenu();
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, "Save to File...");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
            SetMenu(hwnd, hMenu);

            // Generate the report
            CreateReport(hMemoEdit);
            break;
        }
        case WM_SIZE: {
            // Resize the memo control to fit the window
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            SetWindowPos(hMemoEdit, NULL, 4, 4, rcClient.right - 8, rcClient.bottom - 8, SWP_NOZORDER);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_FILE_SAVE) {
                // Save menu item clicked
                SaveToFile(hMemoEdit);
            }
            break;
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                DestroyWindow(hMemoEdit);
                return 0;
            }
        }
        case WM_CLOSE:
            ClearFormData(hMemoEdit);
            DestroyWindow(hwnd);
            hMemoForm = NULL;  // Reset the global handle
            break;
        case WM_DESTROY:
            DeleteObject(hMemoFont);  // Clean up the font object
            EnableWindow(mainForm, TRUE);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Function to create and show the memo form
void ShowMemoForm(HWND hParent) {

    if (hMemoForm != NULL) {
        // If the form already exists, just bring it to the front
        SetForegroundWindow(hMemoForm);
        return;
    }

    // Register the window class for the memo form
    WNDCLASSEX wcMemo = { 
        sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MemoFormProc, 
        0, 0, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), 
        (HBRUSH)(COLOR_WINDOW+1), NULL, "MemoFormClass", NULL 
    };
    RegisterClassEx(&wcMemo);

    // Create the memo form
    hMemoForm = CreateWindowEx(WS_EX_CLIENTEDGE, "MemoFormClass", "Loan Amortization Schedule", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 
        hParent, NULL, GetModuleHandle(NULL), NULL);

    // Set the icon for the window
    SendMessage(hMemoForm, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hMemoForm, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    if (hMemoForm != NULL) {
        ShowWindow(hMemoForm, SW_SHOW);
        UpdateWindow(hMemoForm);
    }

    SetForegroundWindow(hMemoForm); 

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            // Create fonts and background brush
            hFont = CreateFont(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT);
            hFont2 = CreateFont(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT2);
            hFontBold = CreateFont(-14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT);
            hBackgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);

            // Create top label
            CreateWindow("STATIC", "Input the loan parameters", WS_VISIBLE | WS_CHILD | SS_LEFT, 20, 29, 360, FHEIGHT, hwnd, NULL, NULL, NULL);
            SendMessage(GetDlgItem(hwnd, 0), WM_SETFONT, (WPARAM)hFontBold, TRUE);

            // Create labels and edit boxes
            const char* labels[] = {"Amount Borrowed", "Payment Frequency", "Number of Payments", "Annual Interest Rate %", "Fees per Period"};
            for (int i = 0; i < 5; i++) {
                HWND hLabel = CreateWindow("STATIC", labels[i], WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 29 + (i + 1) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, NULL, NULL, NULL);
                SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
            }
            hEdit0 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT, FWIDTH + 40, 25 + 1 * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 0), NULL, NULL);
            SendMessage(hEdit0, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SetWindowText(hEdit0, formatNumber(1000000, 2));
            hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT, FWIDTH + 40, 25 + 3 * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 2), NULL, NULL);
            SendMessage(hEdit2, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SetWindowText(hEdit2, formatNumber(300, 0));
            hEdit3 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT, FWIDTH + 40, 25 + 4 * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 3), NULL, NULL);
            SendMessage(hEdit3, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SetWindowText(hEdit3, formatNumber(3.5, 2));
            hEdit4 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT, FWIDTH + 40, 25 + 5 * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 4), NULL, NULL);
            SendMessage(hEdit4, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SetWindowText(hEdit4, formatNumber(0, 2));

            // Create combobox
            HWND hComboBox = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | SS_RIGHT, FWIDTH + 40, 25 + 2 * (FHEIGHT + HSPACER), FWIDTH, 200, hwnd, (HMENU)ID_COMBOBOX, NULL, NULL);
            SendMessage(hComboBox, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Daily     365/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Daily     360/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Weekly     52/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Biweekly   26/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Bimonthly  24/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Monthly    12/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Quarterly   4/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Biannually  2/year");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Anually     1/year");

            // Selecting 12 payments per year
            SendMessage(hComboBox, CB_SETCURSEL, 5, 0);

            // Create term calculation label
            hLabel = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_LEFT, 20, 64 + 6 * (FHEIGHT + HSPACER), 360, FHEIGHT, hwnd, NULL, NULL, NULL);
            SendMessage(hLabel, WM_SETFONT, (WPARAM)hFontBold, TRUE);

            // Create result labels and edit boxes
            const char* labels2[] = {"Payment Amount per Period", "Total Interest Paid", "Total Fees Paid", "Total Amount Paid"};
            for (int i = 0; i < 4; i++) {
                HWND hLabel = CreateWindow("STATIC", labels2[i], WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 29 + (8 + i) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, NULL, NULL, NULL);
                SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
            }
            hEdit5 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT | ES_READONLY, FWIDTH + 40, 25 + (7 + 1) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 0), NULL, NULL);
            SendMessage(hEdit5, WM_SETFONT, (WPARAM)hFont2, TRUE);
            hEdit6 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT | ES_READONLY, FWIDTH + 40, 25 + (7 + 2) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 1), NULL, NULL);
            SendMessage(hEdit6, WM_SETFONT, (WPARAM)hFont2, TRUE);
            hEdit7 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT | ES_READONLY, FWIDTH + 40, 25 + (7 + 3) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 2), NULL, NULL);
            SendMessage(hEdit7, WM_SETFONT, (WPARAM)hFont2, TRUE);
            hEdit8 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | SS_RIGHT | ES_READONLY, FWIDTH + 40, 25 + (7 + 4) * (FHEIGHT + HSPACER), FWIDTH, FHEIGHT, hwnd, (HMENU)(ID_EDIT1 + 3), NULL, NULL);
            SendMessage(hEdit8, WM_SETFONT, (WPARAM)hFont2, TRUE);

            // Add the menu
            AddMenus(hwnd);
            update_result();
            loaded = 1;
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, TEXT_COLOR);
            SetBkColor(hdcStatic, BACKGROUND_COLOR);
            return (LRESULT)hBackgroundBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, TEXT_COLOR);
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_COMMAND:
            if (HIWORD(wParam) == 0) {  // Menu item clicked
                switch (LOWORD(wParam)) {
                    case ID_MENU_ITEM1:
                        ShowMemoForm(hwnd);
                        break;
                    case ID_MENU_ITEM2:
                        MessageBox(NULL, "LoanCalc 1.0 by Shpati Koleka, MIT License", "LoanCalc", MB_OK);
                        break;
                }
            }
            if (HIWORD(wParam) == EN_CHANGE && loaded == 1) {
                // This is triggered when the text in the edit box changes
                update_result();
            }
            if (HIWORD(wParam) == EN_KILLFOCUS) {
                // Handle notifications from the edit box
                SetWindowText(hEdit0, formatNumber(principal, 2));
                SetWindowText(hEdit2, formatNumber(numberOfPayments, 0));
                SetWindowText(hEdit3, formatNumber(annualInterestRate, 2));
                SetWindowText(hEdit4, formatNumber(periodfees, 2));

            }
            if (HIWORD(wParam) == CBN_SELCHANGE) { // Check for selection change
                HWND hComboBox = (HWND)lParam; // Get ComboBox handle
                int selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0); // Get selected index
                
                // Assign a value based on the selected index
                switch (selectedIndex) {
                    case 0:
                        paymentsperyear = 365; // Value for Option 1
                        update_result();
                        break;
                    case 1:
                        paymentsperyear = 360; // Value for Option 2
                        update_result();
                        break;
                    case 2:
                        paymentsperyear = 52; // Value for Option 3
                        update_result();
                        break;
                    case 3:
                        paymentsperyear = 26; // Value for Option 4
                        update_result();
                        break;
                    case 4:
                        paymentsperyear = 24; // Value for Option 5
                        update_result();
                        break;
                    case 5:
                        paymentsperyear = 12; // Value for Option 6
                        update_result();
                        break;
                    case 6:
                        paymentsperyear = 4; // Value for Option 7
                        update_result();
                        break;
                    case 7:
                        paymentsperyear = 2; // Value for Option 8
                        update_result();
                        break;
                    case 8:
                        paymentsperyear = 1; // Value for Option 9
                        update_result();
                        break;
                }
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            DeleteObject(hFont);
            DeleteObject(hFontBold);
            DeleteObject(hBackgroundBrush);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Load the icon
    hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    
    // Initialize Common Controls
    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES };
    InitCommonControlsEx(&icc);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), CreateSolidBrush(BACKGROUND_COLOR), NULL, "ModernWindowClass", NULL };
    RegisterClassEx(&wc);

    // Calculate center position
    int posX = (GetSystemMetrics(SM_CXSCREEN) - 400) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - 320) / 2;

    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_COMPOSITED, "ModernWindowClass", "LoanCalc", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, posX, posY, 2 * FWIDTH + 70, 60 + 13 * (FHEIGHT + HSPACER) + 10, NULL, NULL, hInstance, NULL);
    mainForm = hwnd;

    // Load the icon
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
