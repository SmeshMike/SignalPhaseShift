
// PhaseShiftDlg.h : header file
//
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#pragma once

using namespace std;

class cmplx
{
public:
	double re;
	double im;

	cmplx() { re = im = 0.; }
	cmplx(double x, double y) { re = x; im = y; }
	cmplx& operator = (cmplx&);
	friend cmplx operator * (cmplx& x, cmplx& y);
	friend cmplx operator / (cmplx& x, cmplx& y);
	friend cmplx operator / (cmplx& x, double y);
	friend cmplx operator + (cmplx& x, cmplx& y);
	friend cmplx operator - (cmplx& x, cmplx& y);
};

// CPhaseShiftDlg dialog
class CPhaseShiftDlg : public CDialogEx
{
// Construction
public:
	CPhaseShiftDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHASESHIFT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void Draw(CDC* pDC, CRect rect, vector <double> A, int R, int G, int B);// Отрисовка графика
	void setka(CDC* pDC, CRect rect, vector <double> A, double x0, double x1);// Сетки
	void zoom(vector <double> A);// Функция для автоподстройки границ графика по оси у
	void picture(CDC* pDC, CRect rect, vector <double> A, double x0, double x1);// Функция вызывающая методы сетка и зум


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedShift();
	afx_msg void OnBnClickedRun();

	double ampl1;
	double ampl2;
	double ampl3;
	double ampl4;
	double ampl5;

	double center1;
	double center2;
	double center3;
	double center4;
	double center5;

	double width1;
	double width2;
	double width3;
	double width4;
	double width5;

	int length;
	double epsilon;

	bool isSignReady;
	bool isSpectrReady;
	bool isRecoverReady;
	bool isShiftReady;
	int timer;
	int minIndex;
	int difference;

	vector <double> recovered;
	vector <double> signal;
	vector <double> spectr;
	vector <double> shift;
	vector <double> recoveredTemp;
	vector <double> signalRecovered;
	vector <double> sqrto;
	cmplx* foureaRecovered;

	CDC* frameDC1;
	CWnd* frameWnd1;
	CRect frameRect1, rect1;

	CDC* frameDC2;
	CWnd* frameWnd2;
	CRect frameRect2, rect2;

	CDC* frameDC3;
	CWnd* frameWnd3;
	CRect frameRect3, rect3;

	int xCenter1, yCenter1;// Для задания границ отрисовки сетки и графика, определяют ширину и высоту в пикселях
	double xScale1, yScale1;
	double xstart1, ystart1;
	double y_min1, y_max1;// Для отыскания наименьшего и наибольшего элементов по амплитуде

	double precision;
	int invert;

	void fourea(int n, cmplx* sig, int s)// функция для Фурье- преобразований, s=1 - обратное, s=-1 -прямое
	{
		int i, j, istep;
		int m, mmax;
		double r, r1, theta, w_r, w_i, temp_r, temp_i;
		double pi = 3.1415926f;

		r = pi * s;
		j = 0;
		for (i = 0; i < n; i++)
		{
			if (i < j)
			{

				temp_r = sig[j].re;
				temp_i = sig[j].im;
				sig[j].re = sig[i].re;
				sig[j].im = sig[i].im;
				sig[i].re = temp_r;
				sig[i].im = temp_i;
			}
			m = n >> 1;
			while (j >= m) { j -= m; m = (m + 1) / 2; }
			j += m;
		}
		mmax = 1;
		while (mmax < n)
		{
			istep = mmax << 1;
			r1 = r / (double)mmax;
			for (m = 0; m < mmax; m++)
			{
				theta = r1 * m;
				w_r = (double)cos((double)theta);
				w_i = (double)sin((double)theta);
				for (i = m; i < n; i += istep)
				{

					j = i + mmax;
					temp_r = w_r * sig[j].re - w_i * sig[j].im;
					temp_i = w_r * sig[j].im + w_i * sig[j].re;
					sig[j].re = sig[i].re - temp_r;
					sig[j].im = sig[i].im - temp_i;
					sig[i].re += temp_r;
					sig[i].im += temp_i;

				}
			}
			mmax = istep;
		}
		if (s > 0)
			for (i = 0; i < n; i++)
			{
				sig[i].re /= (double)n;
				sig[i].im /= (double)n;
			}

	}


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRun2();
	afx_msg void OnBnClickedRun3();
	afx_msg void OnBnClickedInvert();
};
