
// PhaseShiftDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PhaseShift.h"
#include "PhaseShiftDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define PI 3.14159265;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPhaseShiftDlg dialog



CPhaseShiftDlg::CPhaseShiftDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHASESHIFT_DIALOG, pParent)
	, ampl1(18)
	, ampl2(15)
	, ampl3(22)
	, ampl4(22)
	, ampl5(25)
	, center1(100)
	, center2(300)
	, center3(500)
	, center4(700)
	, center5(900)
	, width1(7)
	, width2(8)
	, width3(7)
	, width4(5)
	, width5(7)
	, length(1024)
	, epsilon(1.e-3)
	, invert(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhaseShiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AMPL1, ampl1);
	DDX_Text(pDX, IDC_AMPL2, ampl2);
	DDX_Text(pDX, IDC_AMPL3, ampl3);
	DDX_Text(pDX, IDC_AMPL4, ampl4);
	DDX_Text(pDX, IDC_AMPL5, ampl5);
	DDX_Text(pDX, IDC_CENTER1, center1);
	DDX_Text(pDX, IDC_CENTER2, center2);
	DDX_Text(pDX, IDC_CENTER3, center3);
	DDX_Text(pDX, IDC_CENTER4, center4);
	DDX_Text(pDX, IDC_CENTER5, center5);
	DDX_Text(pDX, IDC_WIDTH1, width1);
	DDX_Text(pDX, IDC_WIDTH2, width2);
	DDX_Text(pDX, IDC_WIDTH3, width3);
	DDX_Text(pDX, IDC_WIDTH4, width4);
	DDX_Text(pDX, IDC_WIDTH5, width5);
	DDX_Text(pDX, IDC_LENGTH7, length);
	DDX_Text(pDX, IDC_EPSILON8, epsilon);
}

BEGIN_MESSAGE_MAP(CPhaseShiftDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHIFT, &CPhaseShiftDlg::OnBnClickedShift)
	ON_BN_CLICKED(IDC_RUN, &CPhaseShiftDlg::OnBnClickedRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RUN2, &CPhaseShiftDlg::OnBnClickedRun2)
	ON_BN_CLICKED(IDC_RUN3, &CPhaseShiftDlg::OnBnClickedRun3)
	ON_BN_CLICKED(IDC_INVERT, &CPhaseShiftDlg::OnBnClickedInvert)
END_MESSAGE_MAP()


// CPhaseShiftDlg message handlers

BOOL CPhaseShiftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	frameWnd1 = GetDlgItem(IDC_STATIC1);
	frameDC1 = frameWnd1->GetDC();
	frameWnd1->GetClientRect(&rect1);

	frameWnd2 = GetDlgItem(IDC_STATIC2);
	frameDC2 = frameWnd2->GetDC();
	frameWnd2->GetClientRect(&rect2);

	frameWnd3 = GetDlgItem(IDC_STATIC3);
	frameDC3 = frameWnd3->GetDC();
	frameWnd3->GetClientRect(&rect3);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhaseShiftDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhaseShiftDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		CDC* MDc1 = new CDC();
		MDc1->CreateCompatibleDC(frameDC1);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(frameDC1, rect1.Width(), rect1.Height());
		CBitmap* tmp = MDc1->SelectObject(&bmp);

		CDC* MDc2 = new CDC();
		MDc2->CreateCompatibleDC(frameDC2);
		CBitmap bmp2;
		bmp2.CreateCompatibleBitmap(frameDC2, rect2.Width(), rect2.Height());
		CBitmap* tmp2 = MDc2->SelectObject(&bmp2);

		CDC* MDc3 = new CDC();
		MDc3->CreateCompatibleDC(frameDC3);
		CBitmap bmp3;
		bmp3.CreateCompatibleBitmap(frameDC3, rect3.Width(), rect3.Height());
		CBitmap* tmp3 = MDc3->SelectObject(&bmp3);

		if (isSignReady == true)//Отрисовывает 
		{
			picture(MDc1, rect1, signal, 0, length);
			Draw(MDc1, rect1, signal, 255, 0, 0);
			
		}

		if (isSpectrReady == true)//Отрисовывает 
		{
			picture(MDc2, rect2, spectr, 0, 1);
			Draw(MDc2, rect2, spectr, 255, 0, 0);
		}

		if (isShiftReady == true)
		{
			picture(MDc3, rect3, shift, 0, length);
			Draw(MDc3, rect3, shift, 0, 255, 0);
		}

		frameDC1->BitBlt(0, 0, rect1.Width(), rect1.Height(), MDc1, 0, 0, SRCCOPY);
		delete MDc1;
		frameDC2->BitBlt(0, 0, rect2.Width(), rect2.Height(), MDc2, 0, 0, SRCCOPY);
		delete MDc2;
		frameDC3->BitBlt(0, 0, rect3.Width(), rect3.Height(), MDc3, 0, 0, SRCCOPY);
		delete MDc3;
	}
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPhaseShiftDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPhaseShiftDlg::OnBnClickedShift()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	KillTimer(1);
	sqrto.clear(); sqrto.resize(length);
	shift.clear(); shift.resize(length);
	recoveredTemp.clear(); recoveredTemp.resize(length);
	signalRecovered.clear(); signalRecovered.resize(2 * length);


	if (invert == 1)
		for (int i = 0; i < length; i++)
		{
			recoveredTemp[i] = recovered[length - i - 1];
		}
	if (invert == 0)
		for (int i = 0; i < length; i++)
		{
			recoveredTemp[i] = recovered[i];
		}
	for (int i = 0; i < length; i++)
	{
		sqrto[i] = 0;
		for (int j = 0; j < length; j++)
		{
			if ((j - i) >= 0)
				sqrto[i] += (signal[j] - recoveredTemp[j - i]) * (signal[j] - recoveredTemp[j - i]);
			else
				sqrto[i] += (signal[j] - recoveredTemp[j - i + length]) * (signal[j] - recoveredTemp[j - i + length]);
		}
	}

	double min = sqrto[0];

	for (int i = 1; i < length; i++)
	{
		if (sqrto[i] < min)
		{
			min = sqrto[i];
			minIndex = i;
		}
	}

	difference = 0;

	if (minIndex > length / 2)
	{
		timer = SetTimer(2, 1, NULL);
	}
	else
	{
		timer = SetTimer(3, 1, NULL);
	}

	UpdateData(false);
}



void CPhaseShiftDlg::OnBnClickedRun()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	//===========ПОСТРОЕНИЕ СИГНАЛА======================
	
	signal.clear(); signal.resize(length);
	

	for (int i = 0; i < length; i++)
	{
		signal[i] = ampl1 * exp((double)(-(i - center1) * (i - center1)) / (2 * width1 * width1)) +
			ampl2 * exp((double)(-(i - center2) * (i - center2)) / (2 * width2 * width2)) +
			ampl3 * exp((double)(-(i - center3) * (i - center3)) / (2 * width3 * width3)) +
			ampl4 * exp((double)(-(i - center4) * (i - center4)) / (2 * width4 * width4)) +
			ampl5 * exp((double)(-(i - center5) * (i - center5)) / (2 * width5 * width5));
	}




	Invalidate(0);
	isSignReady = true;
	UpdateData(false);
}

void CPhaseShiftDlg::OnBnClickedRun2()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);

	//===========ПОСТРОЕНИЕ СПЕКТРА======================

	spectr.clear(); spectr.resize(length);

	cmplx* four = new cmplx[length];

	for (int i = 0; i < length; i++)
	{
		four[i].re = signal[i];
		four[i].im = 0;
	}

	fourea(length, four, -1);

	for (int i = 0; i < length; i++)
	{
		spectr[i] = four[i].re * four[i].re + four[i].im * four[i].im;
	}
	Invalidate(0);
	isSpectrReady = true;
	UpdateData(false);
}


void CPhaseShiftDlg::OnBnClickedRun3()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);

		//===========ВОССТАНОВЛЕНИЕ======================
	recovered.clear(); recovered.resize(length);
	recoveredTemp.clear(); recoveredTemp.resize(length);
	signalRecovered.clear(); signalRecovered.resize(2 * length);
	foureaRecovered = new cmplx[length];

	double phase;
	for (int i = 0; i < length; i++)
	{
		phase = (2 * ((double)rand() / RAND_MAX) - 1) * PI;

		foureaRecovered[i].re = sqrt(spectr[i]) * cos(phase);
		foureaRecovered[i].im = sqrt(spectr[i]) * sin(phase);
	}

	fourea(length, foureaRecovered, 1);

	for (int i = 0; i < length; i++)
	{
		recovered[i] = foureaRecovered[i].re;
		if (recovered[i] < 0)
			recovered[i] = 0;

	}

	precision = 10;
	timer = SetTimer(1, 1, NULL);

	Invalidate(0);
	isRecoverReady= true;
	UpdateData(false);
}



// комплексное сопряжение
cmplx conjg(cmplx c) { return cmplx(c.re, -c.im); }
cmplx conjg(double re, double im) { return cmplx(re, -im); }

void CPhaseShiftDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	UpdateData(true);

	CDC* MDc3 = new CDC();
	MDc3->CreateCompatibleDC(frameDC3);
	CBitmap bmp3;
	bmp3.CreateCompatibleBitmap(frameDC3, rect3.Width(), rect3.Height());
	CBitmap* tmp3 = MDc3->SelectObject(&bmp3);

	//recoveredTemp.clear(); recoveredTemp.resize(length);
	//signalRecovered.clear(); signalRecovered.resize(2 * length);
	if (nIDEvent == 1)
	{
		if (precision > epsilon)
		{
			//критерий остановки по разнице между двумя соседними 
			//итерациями или по отклонению энергии сигнала на какой-то итерации от энергии исходного сигнала

			for (int i = 0; i < length; i++)
			{
				foureaRecovered[i].re = recovered[i];
				foureaRecovered[i].im = 0;
			}

			fourea(length, foureaRecovered, -1);

			double moduleTemp;

			for (int i = 0; i < length; i++)
			{
				moduleTemp = foureaRecovered[i].re * foureaRecovered[i].re + foureaRecovered[i].im * foureaRecovered[i].im;
				foureaRecovered[i].re = foureaRecovered[i].re * sqrt(spectr[i] / moduleTemp);
				foureaRecovered[i].im = foureaRecovered[i].im * sqrt(spectr[i] / moduleTemp);
			}

			fourea(length, foureaRecovered, 1);

			for (int i = 0; i < length; i++)
			{
				recoveredTemp[i] = foureaRecovered[i].re;
				if (recoveredTemp[i] < 0)//зануляем отр часть
				{
					recoveredTemp[i] = 0;
				}
			}

			precision = 0; //toch1 = 0;
			for (int i = 0; i < length; i++)
			{
				precision += (recovered[i] - recoveredTemp[i]) * (recovered[i] - recoveredTemp[i]);
				//toch1 += vosst1[i];
			}
			//summ_kv_razn = toch;
			for (int i = 0; i < length; i++)
			{
				recovered[i] = recoveredTemp[i];
			}
			
		}
		else
		{
			KillTimer(1);
			MessageBox(L"Готово", L"Text", MB_OK);
		}

		

		for (int i = 0; i < length; i++)
		{
			signalRecovered[i] = signal[i];
			signalRecovered[length - 1 + i] = recovered[i];
		}


		picture(MDc3, rect3, signalRecovered, 0, length);
		Draw(MDc3, rect3, recovered, 255, 0, 0);
		Draw(MDc3, rect3, signal, 0, 0, 255);
	}

	//shift.clear(); shift.resize(length);

	if (nIDEvent == 2)
	{
		if (difference < length - minIndex)
			for (int i = 0; i < length; i++)
				if ((i + difference) < length)
					shift[i] = recoveredTemp[i + difference];
				else
					shift[i] = recoveredTemp[i + difference - length];
		else
		{
			KillTimer(2);
			MessageBox(L"Готово", L"Text", MB_OK);
		}


		for (int i = 0; i < (length); i++)
		{
			signalRecovered[i] = signal[i];
			signalRecovered[length - 1 + i] = shift[i];
		}

		picture(MDc3, rect3, signalRecovered, 0, length);
		Draw(MDc3, rect3, signal, 255, 0, 0);
		Draw(MDc3, rect3, shift, 0, 0, 255);
	}

	if (nIDEvent == 3)
	{
		if (difference < minIndex)
			for (int i = 0; i < length; i++)
				if ((i - difference) >= 0)
					shift[i] = recoveredTemp[i - difference];
				else
					shift[i] = recoveredTemp[i - difference + length];
		else
		{
			KillTimer(3);
			MessageBox(L"Готово", L"Text", MB_OK);
		}

		for (int i = 0; i < length; i++)
		{
			signalRecovered[i] = signal[i];
			signalRecovered[length - 1 + i] = shift[i];
		}


		picture(MDc3, rect3, signalRecovered, 0, length);
		Draw(MDc3, rect3, signal, 255, 0, 0);
		Draw(MDc3, rect3, shift, 0, 0, 255);
	}

	difference++;

	frameDC3->BitBlt(0, 0, rect3.Width(), rect3.Height(), MDc3, 0, 0, SRCCOPY);
	delete MDc3;


	UpdateData(false);

	CDialogEx::OnTimer(nIDEvent);
}







void CPhaseShiftDlg::OnBnClickedInvert()
{
	// TODO: Add your control notification handler code here
	if (invert == 0)
		invert = 1;
	else
		invert = 0;
}
