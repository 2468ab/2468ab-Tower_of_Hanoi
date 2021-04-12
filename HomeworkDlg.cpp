
// HomeworkDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Homework.h"
#include "HomeworkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinThread* CHomeworkDlg::m_p_img_thread = NULL;
CWinThread* CHomeworkDlg::m_receive_thread = NULL;
CWinThread* CHomeworkDlg::m_send_thread = NULL;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CHomeworkDlg 대화 상자



CHomeworkDlg::CHomeworkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOMEWORK_DIALOG, pParent)
	, m_Radio(0)
	, m_str_edit_senddata2(_T(""))
	, m_str_comport(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHomeworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMG1, m_st_img1);
	DDX_Control(pDX, IDC_STATIC_IMG2, m_st_img2);
	DDX_Control(pDX, IDC_SLIDER_H_MIN, m_slider_h_min);
	DDX_Control(pDX, IDC_SLIDER_H_MAX, m_slider_h_max);
	DDX_Control(pDX, IDC_SLIDER_S_MIIN, m_slider_s_min);
	DDX_Control(pDX, IDC_SLIDER_S_MAX, m_slider_s_max);
	DDX_Control(pDX, IDC_SLIDER_V_MIN, m_slider_v_min);
	DDX_Control(pDX, IDC_SLIDER_V_MAX, m_slider_v_max);
	DDX_Radio(pDX, IDC_RADIO_COLOR1, (int&)m_Radio);
	DDX_Text(pDX, IDC_EDIT_SENDDATA2, m_str_edit_senddata2);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_str_comport);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_port);
	DDX_Control(pDX, IDC_EDIT_SENDDATA, m_edit_send_view);
	DDX_Control(pDX, IDC_EDIT_SENDDATA, m_edit_send_view);
	DDX_Control(pDX, IDC_EDIT_RECEIVEDATA, m_edit_rcv_view);
	DDX_Control(pDX, IDC_EDIT_SENDDATA2, m_edit_send_data2);
}

BEGIN_MESSAGE_MAP(CHomeworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_COLOR1, IDC_RADIO_COLOR6, CHomeworkDlg::SetRadioStatus)
	ON_BN_CLICKED(IDC_BUTTON1, &CHomeworkDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CHomeworkDlg::OnBnClickedButtonLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CHomeworkDlg::OnCbnSelchangeComboPort)
	ON_BN_CLICKED(IDC_RADIO_BPS1, &CHomeworkDlg::OnBnClickedRadioBps1)
	ON_BN_CLICKED(IDC_RADIO_BPS2, &CHomeworkDlg::OnBnClickedRadioBps2)
	ON_BN_CLICKED(IDC_BUTTON_COMOPEN, &CHomeworkDlg::OnBnClickedButtonComopen)
	ON_EN_CHANGE(IDC_EDIT_SENDDATA2, &CHomeworkDlg::OnEnChangeEditSenddata2)
	ON_EN_CHANGE(IDC_EDIT_RECEIVEDATA, &CHomeworkDlg::OnEnChangeEditReceivedata)

	ON_MESSAGE(WM_DSP_CLOSE, &CHomeworkDlg::OnThreadClosed)
	ON_MESSAGE(WM_DSP_RECEIVE, &CHomeworkDlg::OnReceive)


	ON_BN_CLICKED(IDC_BUTTON_READY, &CHomeworkDlg::OnBnClickedButtonReady)
	ON_BN_CLICKED(IDC_BUTTON_START, &CHomeworkDlg::OnBnClickedButtonStart)
	ON_EN_CHANGE(IDC_EDIT_SENDDATA, &CHomeworkDlg::OnEnChangeEditSenddata)
	ON_EN_CHANGE(IDC_EDIT_UD, &CHomeworkDlg::OnEnChangeEditUd)
	ON_STN_CLICKED(IDC_STATIC_IMG1, &CHomeworkDlg::OnStnClickedStaticImg1)
	ON_EN_CHANGE(IDC_EDIT_B, &CHomeworkDlg::OnEnChangeEditB)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_ERASE, &CHomeworkDlg::OnBnClickedButtonErase)
	ON_BN_CLICKED(IDC_BUTTON_SAVE1, &CHomeworkDlg::OnBnClickedButtonSave1)
	ON_BN_CLICKED(IDC_BUTTON_LOAD1, &CHomeworkDlg::OnBnClickedButtonLoad1)
END_MESSAGE_MAP()


// CHomeworkDlg 메시지 처리기

BOOL CHomeworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//vcap = VideoCapture(0);
	vcap = VideoCapture("http://192.168.1.254/?custom=1&cmd=3001&par=0");  // 외장캠 사용시 사용할 내용
    vcap = VideoCapture("http://192.168.1.254:8192/");
	if (!vcap.isOpened()){
		MessageBox(_T("캠을 열수 없습니다. \n"));
	}

	vcap.set(CAP_PROP_FRAME_WIDTH, img_w);//img_w = 320
	vcap.set(CAP_PROP_FRAME_HEIGHT, img_h); //img_h = 240

	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;

	RGBQUAD* palette = bitInfo->bmiColors;
	for (int i = 0; i < 256; i++){
		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
		palette[i].rgbReserved = 0;
	}

	m_slider_h_min.SetRange(0, 180); m_slider_h_min.SetPos(H_min[0]);
	m_slider_h_max.SetRange(0, 180); m_slider_h_max.SetPos(H_max[0]);
	m_slider_s_min.SetRange(0, 255); m_slider_s_min.SetPos(S_min[0]);
	m_slider_s_max.SetRange(0, 255); m_slider_s_max.SetPos(S_max[0]);
	m_slider_v_min.SetRange(0, 255); m_slider_v_min.SetPos(V_min[0]);
	m_slider_v_max.SetRange(0, 255); m_slider_v_max.SetPos(V_max[0]);

	SetDlgItemInt(IDC_EDIT_H_MIN, H_min[0]);
	SetDlgItemInt(IDC_EDIT_H_MAX, H_max[0]);
	SetDlgItemInt(IDC_EDIT_S_MIN, S_min[0]);
	SetDlgItemInt(IDC_EDIT_S_MAX, S_max[0]);
	SetDlgItemInt(IDC_EDIT_V_MIN, V_min[0]);
	SetDlgItemInt(IDC_EDIT_V_MAX, V_max[0]);

	m_combo_port.AddString(L"COM1");
	m_combo_port.AddString(L"COM2");
	m_combo_port.AddString(L"COM3");
	m_combo_port.AddString(L"COM4");
	m_combo_port.AddString(L"COM5");
	m_combo_port.AddString(L"COM6");
	m_combo_port.AddString(L"COM7");

	m_str_comport = L"COM1";

	UpdateData(FALSE);

	m_st_img1.GetWindowRect(&rect_img1);
	ScreenToClient(&rect_img1);
	//SetDlgItemInt(IDC_EDIT_B, rect_img1.Height());
	//SetDlgItemInt(IDC_EDIT_G, rect_img1.Width());

	if (m_receive_thread == NULL)
		m_receive_thread = AfxBeginThread(CommThread, this);
	if (m_send_thread == NULL)
		m_send_thread = AfxBeginThread(CommSendThread, this);

	//Button* pButton = (CButton*)GetDlgItem(IDC_RADIO_COLOR1);
	//pButton->SetCheck(true);

	if (m_p_img_thread == NULL) {
		m_p_img_thread = AfxBeginThread(img_thread, this);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHomeworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHomeworkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHomeworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHomeworkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (vcap.isOpened()){
		vcap.release();
	}
	DWORD nExitCode = NULL;
	GetExitCodeThread(m_p_img_thread->m_hThread, &nExitCode);
	TerminateThread(m_p_img_thread->m_hThread, nExitCode);
	m_p_img_thread->Delete();
	m_p_img_thread = NULL;

	GetExitCodeThread(m_receive_thread->m_hThread, &nExitCode);
	TerminateThread(m_receive_thread->m_hThread, nExitCode);
	m_receive_thread->Delete();
	m_receive_thread = NULL;

	GetExitCodeThread(m_send_thread->m_hThread, &nExitCode);
	TerminateThread(m_send_thread->m_hThread, nExitCode);
	m_send_thread->Delete();
	m_send_thread = NULL;
}

void CHomeworkDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	H_min[m_Radio] = m_slider_h_min.GetPos();
	H_max[m_Radio] = m_slider_h_max.GetPos();
	S_min[m_Radio] = m_slider_s_min.GetPos();
	S_max[m_Radio] = m_slider_s_max.GetPos();
	V_min[m_Radio] = m_slider_v_min.GetPos();
	V_max[m_Radio] = m_slider_v_max.GetPos();
	SetDlgItemInt(IDC_EDIT_H_MIN, H_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_H_MAX, H_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MIN, S_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MAX, S_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MIN, V_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MAX, V_max[m_Radio]);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHomeworkDlg::SetRadioStatus(UINT value) {
	UpdateData(TRUE);
	m_slider_h_min.SetPos(H_min[m_Radio]);
	m_slider_h_max.SetPos(H_max[m_Radio]);
	m_slider_s_min.SetPos(S_min[m_Radio]);
	m_slider_s_max.SetPos(S_max[m_Radio]);
	m_slider_v_min.SetPos(V_min[m_Radio]);
	m_slider_v_max.SetPos(V_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_H_MIN, H_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_H_MAX, H_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MIN, S_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MAX, S_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MIN, V_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MAX, V_max[m_Radio]);
}



void CHomeworkDlg::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	erase_color();

	FILE* fpt;
	fopen_s(&fpt, "param.txt", "w");

	for (int i = 0; i < 6; i++){
		for (int j = 0; j < Color_RGB[i].size(); j++) {
			fprintf(fpt, "Color_%d_", i + 1);
			fprintf(fpt, "B_%d", Color_RGB[i][j][0]);
			fprintf(fpt, "G_%d", Color_RGB[i][j][1]);
			fprintf(fpt, "R_%d\n", Color_RGB[i][j][2]);
		}
	}
	fclose(fpt);
	AfxMessageBox(_T("SAVE"));
}

void CHomeworkDlg::OnBnClickedButtonLoad() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 6; i++) {
		Color_RGB[i].clear();
	}
	FILE* fpt;
	fopen_s(&fpt, "param.txt", "r");
	char cstr[255];
	string str;
	string tmp_str;
	while (fgets(cstr, 255, fpt))
	{
		str = cstr;
		str = str.substr(str.find_first_of("_") + 1, 1);
		int tmp_color_num = atoi(str.c_str());
		str = cstr;
		str = str.substr(str.find_first_of("_") + 5, str.size());
		tmp_str = str.substr(0, str.find_first_of("G"));
		int b = atoi(tmp_str.c_str());

		str = str.substr(str.find_first_of("_") + 1, str.size());
		tmp_str = str.substr(0, str.find_first_of("R"));
		int g = atoi(tmp_str.c_str());

		str = str.substr(str.find_first_of("_") + 1, str.size());
		int r = atoi(str.c_str());

		Vec3b tmp_color(b, g, r);

		Color_RGB[tmp_color_num - 1].push_back(tmp_color);
	}
	fclose(fpt);
}


void CHomeworkDlg::OnCbnSelchangeComboPort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


void CHomeworkDlg::OnBnClickedRadioBps1()
{
	m_bps = 115200;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnBnClickedRadioBps2()
{
	m_bps = 38400;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CHomeworkDlg::OnBnClickedButtonComopen()
{
	if (comm_1_state)
	{
		if (comm_1)
		{
			comm_1->Close();
			comm_1 = NULL;
			comm_1_state = false;
			GetDlgItem(IDC_BUTTON_COMOPEN)->SetWindowText(_T("OPEN"));
		}
	}
	else
	{
		CString m_str_baudrate = _T("115200");

		if (m_bps == 115200)
		{
			m_str_baudrate = _T("115200");
		}
		else if (m_bps == 38400)
		{
			m_str_baudrate = _T("38400");
		}
		else
		{
			m_str_baudrate = _T("115200");
		}

		comm_1 = new CSerialComm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));   // initial Comm port
		if (comm_1->Create(GetSafeHwnd()) != 0) // 통신포트를 열고윈도우의핸들을넘긴다.
		{
			comm_1_state = true;
			GetDlgItem(IDC_BUTTON_COMOPEN)->SetWindowText(_T("CLOSE"));
		}
		else
		{
			comm_1_state = false;
			AfxMessageBox(_T("ERROR!"));
		}
		m_str_baudrate.ReleaseBuffer();
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnEnChangeEditSenddata2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (m_str_edit_senddata2.Find(L";") != -1) {
		data_from_typing = true;
	}
}


void CHomeworkDlg::OnEnChangeEditReceivedata()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnBnClickedButtonReady()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RL_NeckAngle = 180;
	UD_NeckAngle = 100;
	neck_angle_chg_UD = true;
	neck_angle_chg_RL = true;
	ready_state = true;
}


void CHomeworkDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (start_state) {
		start_state = false;
		SetDlgItemText(IDC_BUTTON_START, L"START");
	}
	else {
		start_state = true;
		SetDlgItemText(IDC_BUTTON_START, L"STOP");
	}
}


void CHomeworkDlg::OnEnChangeEditSenddata()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnEnChangeEditUd()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UD_NeckAngle = GetDlgItemInt(IDC_EDIT_UD);
}


void CHomeworkDlg::OnStnClickedStaticImg1()
{
	
	GetCursorPos(&pos);
	CWnd::ScreenToClient(&pos);

	double y = double(pos.y - rect_img1.top) / rect_img1.Height()*img_h;
	double x = double(pos.x - rect_img1.left) / rect_img1.Width()*img_w;
	Mat tmp_img = Origin_img.clone();
	Color_RGB[m_Radio].push_back(tmp_img.at<Vec3b>(int(y),int(x)));
	int b = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][0];
	int g = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][1];
	int r = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][2];
	SetDlgItemInt(IDC_EDIT_B, b);
	SetDlgItemInt(IDC_EDIT_G, g);
	SetDlgItemInt(IDC_EDIT_R, r);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnEnChangeEditB()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CHomeworkDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (save_data > 0) {
		GetCursorPos(&pos);
		CWnd::ScreenToClient(&pos);
		if (0 <= pos.x - rect_img1.left&&pos.x - rect_img1.left <= rect_img1.Width() && 0 <= pos.y - rect_img1.top&&pos.y - rect_img1.top <= rect_img1.Height()) {
			double y = double(pos.y - rect_img1.top) / rect_img1.Height()*img_h;
			double x = double(pos.x - rect_img1.left) / rect_img1.Width()*img_w;
			Mat tmp_img = Origin_img.clone();
			cvtColor(tmp_img, tmp_img, COLOR_BGR2HSV);
			if (save_data == 1) {
				init_b = tmp_img.at<Vec3b>(int(y), int(x))[0];
				init_g = tmp_img.at<Vec3b>(int(y), int(x))[1];
				init_r = tmp_img.at<Vec3b>(int(y), int(x))[2];
			}
			else if (init_b - init_ra * init_b <= tmp_img.at<Vec3b>(int(y), int(x))[0] && tmp_img.at<Vec3b>(int(y), int(x))[0] <= init_b + init_ra * init_b&&init_g - init_ra * init_g <= tmp_img.at<Vec3b>(int(y), int(x))[1] && tmp_img.at<Vec3b>(int(y), int(x))[1] <= init_g + init_ra * init_g&&init_r - init_ra * init_r <= tmp_img.at<Vec3b>(int(y), int(x))[2] && tmp_img.at<Vec3b>(int(y), int(x))[2] <= init_r + init_ra * init_r) {
				Color_RGB[m_Radio].push_back(tmp_img.at<Vec3b>(int(y), int(x)));
				int b = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][0];
				int g = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][1];
				int r = Color_RGB[m_Radio][Color_RGB[m_Radio].size() - 1][2];
				SetDlgItemInt(IDC_EDIT_B, b);
				SetDlgItemInt(IDC_EDIT_G, g);
				SetDlgItemInt(IDC_EDIT_R, r);
			}
			save_data++;
			SetDlgItemInt(IDC_EDIT_DISTANCE, Color_RGB[0].size());
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}



BOOL CHomeworkDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		// 키가 눌렸을때
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
			// 리턴키\tab
		case VK_TAB:
			if (save_data > 0) {
				save_data = 0;
			}
			else {
				save_data = 1;
			}
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}



void CHomeworkDlg::OnBnClickedButtonErase()
{
	/*for (int i = 0; i < 6; i++) {
		Color_RGB[i].erase(unique(Color_RGB[i].begin(), Color_RGB[i].end()), Color_RGB[i].end());
	}
	SetDlgItemInt(IDC_EDIT_DISTANCE, Color_RGB[0].size());*/
	

	/*for (int i = 0; i < 6; i++) {
		for (int j = 0; j < Color_RGB[i].size(); j++) {
			CString tmp;
			tmp.Format(L"%d ", Color_RGB[i][j][0]);
			SetDlgItemText(IDC_EDIT_RECEIVEDATA, tmp);
			tmp.Format(L"%d ", Color_RGB[i][j][1]);
			SetDlgItemText(IDC_EDIT_RECEIVEDATA, tmp);
			tmp.Format(L"%d \n", Color_RGB[i][j][2]);
			SetDlgItemText(IDC_EDIT_RECEIVEDATA, tmp);
		}
	}*/
	erase_color();
	SetDlgItemInt(IDC_EDIT_DISTANCE, Color_RGB[m_Radio].size());
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CHomeworkDlg::erase_color() {

	vector<Vec3b> Color_RGB_tmp[6];

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < Color_RGB[i].size(); j++) {
			bool chk = true;
			for (int k = 0; k < Color_RGB_tmp[i].size(); k++) {
				if (Color_RGB[i][j][0] == Color_RGB_tmp[i][k][0] && Color_RGB[i][j][1] == Color_RGB_tmp[i][k][1] && Color_RGB[i][j][2] == Color_RGB_tmp[i][k][2]) {
					chk = false;
					break;
				}
			}
			if (chk == true) {
				Color_RGB_tmp[i].push_back(Color_RGB[i][j]);
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		Color_RGB[i].clear();
		Color_RGB[i] = Color_RGB_tmp[i];
	}
}

void CHomeworkDlg::OnBnClickedButtonSave1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	FILE* fpt;
	fopen_s(&fpt, "param1.txt", "w");

	for (int i = 0; i < 6; i++) {
		fprintf(fpt, "Color_%d_H_max %d\n", i + 1, H_max[i]);
		fprintf(fpt, "Color_%d_H_min %d\n", i + 1, H_min[i]);
		fprintf(fpt, "Color_%d_S_max %d\n", i + 1, S_max[i]);
		fprintf(fpt, "Color_%d_S_min %d\n", i + 1, S_min[i]);
		fprintf(fpt, "Color_%d_V_max %d\n", i + 1, V_max[i]);
		fprintf(fpt, "Color_%d_V_min %d\n", i + 1, V_min[i]);
	}
	fclose(fpt);
}


void CHomeworkDlg::OnBnClickedButtonLoad1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	FILE* fpt;
	fopen_s(&fpt, "param1.txt", "r");
	char cstr[255];
	string str;
	while (fgets(cstr, 255, fpt))
	{
		str = cstr;
		str = str.substr(str.find_first_of(" ") + 1, 3);
		int tmp_val = atoi(str.c_str());
		str = cstr;
		str = str.substr(str.find_first_of("_") + 1, 1);
		int tmp_color_num = atoi(str.c_str());
		str = cstr;
		if (str.find("_H_") != -1)
		{
			if (str.find("_max") != -1)H_max[tmp_color_num - 1] = tmp_val;
			else if (str.find("_min") != -1)H_min[tmp_color_num - 1] = tmp_val;
		}
		else if (str.find("_S_") != -1)
		{
			if (str.find("_max") != -1)S_max[tmp_color_num - 1] = tmp_val;
			else if (str.find("_min") != -1)S_min[tmp_color_num - 1] = tmp_val;
		}
		else if (str.find("_V_") != -1)
		{
			if (str.find("_max") != -1)V_max[tmp_color_num - 1] = tmp_val;
			else if (str.find("_min") != -1)V_min[tmp_color_num - 1] = tmp_val;
		}
	}
	fclose(fpt);

	m_slider_h_min.SetPos(H_min[m_Radio]);
	m_slider_h_max.SetPos(H_max[m_Radio]);
	m_slider_s_min.SetPos(S_min[m_Radio]);
	m_slider_s_max.SetPos(S_max[m_Radio]);
	m_slider_v_min.SetPos(V_min[m_Radio]);
	m_slider_v_max.SetPos(V_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_H_MIN, H_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_H_MAX, H_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MIN, S_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_S_MAX, S_max[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MIN, V_min[m_Radio]);
	SetDlgItemInt(IDC_EDIT_V_MAX, V_max[m_Radio]);
}
