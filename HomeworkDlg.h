
// HomeworkDlg.h: 헤더 파일
//

#pragma once

#include"SerialComm2.h"

UINT img_thread(LPVOID arg);
UINT CommThread(LPVOID arg);
UINT CommSendThread(LPVOID arg);

// CHomeworkDlg 대화 상자
class CHomeworkDlg : public CDialogEx
{
// 생성입니다.
public:
	CHomeworkDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOMEWORK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void SetRadioStatus(UINT value);
	CStatic m_st_img1;
	CStatic m_st_img2;
	CSliderCtrl m_slider_h_min;
	CSliderCtrl m_slider_h_max;
	CSliderCtrl m_slider_s_min;
	CSliderCtrl m_slider_s_max;
	CSliderCtrl m_slider_v_min;
	CSliderCtrl m_slider_v_max;

	int H_min[6] = { 0,0,0,0,0,0 }, H_max[6] = { 180,180,180,180,180,180 };
	int S_min[6] = { 0,0,0,0,0,0 }, S_max[6] = { 255,255,255,255,255,255 };
	int V_min[6] = { 0,0,0,0,0,0 }, V_max[6] = { 255,255,255,255,255,255 };
	
	UINT m_Radio;

	int img_w = 320, img_h = 240;
	VideoCapture vcap;

	Mat Origin_img = Mat::zeros(Size(img_w, img_h), CV_8UC3);
	Mat Show_img = Mat::zeros(Size(img_w, img_h), CV_8UC3);
	Mat tmp_extracted_img[6];
	Mat tmp_ext_bin_img[6];
	Scalar rec_color[6] = { Scalar(0, 0, 255) ,Scalar(255, 0, 0) ,Scalar(0, 255, 255),Scalar(0, 0, 0),Scalar(255, 0, 255),Scalar(0, 255, 0) };
	Scalar cir_color[6] = { Scalar(0, 0, 255) ,Scalar(255, 0, 0) ,Scalar(0, 255, 255),Scalar(0, 0, 0),Scalar(255, 0, 255),Scalar(0, 255, 0) };

	CImage cimage_mfc;
	BITMAPINFO *bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));

	static CWinThread* m_p_img_thread;

	void img_proc();
	void img_view(Mat* img, int order);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	

	CSerialComm* comm_1 = NULL;

	bool data_from_typing = false;
	bool SendNReturn = true;
	static CWinThread* m_p_comm;
	static CWinThread* m_p_send;
	CString m_str_edit_senddata;
	CString m_str_edit_senddata2;
	CString m_str_edit_comport;
	afx_msg void OnCbnSelchangeComboPort();
	afx_msg void OnBnClickedRadioBps1();
	afx_msg void OnBnClickedRadioBps2();
	int m_bps = 115200;
	CString m_str_comport;
	CComboBox m_combo_port;
	
	CString remain_data;
	bool comm_1_state = false;
	static CWinThread*  m_receive_thread;
	static CWinThread*  m_send_thread;
	void send_data_to_dsp();
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT OnReceive(WPARAM length, LPARAM lpara);
	afx_msg void OnBnClickedButtonComopen();
	afx_msg void OnEnChangeEditSenddata2();

	int UD_NeckAngle = 180;
	int RL_NeckAngle = 180;

	bool neck_angle_chg_UD = false;
	int neck_angle_chg_UD_cnt = 0;
	bool neck_angle_chg_RL = false;
	int neck_angle_chg_RL_cnt = 0;
	int neck_angle_chg_max = 30;

	bool Q_state_prv = true;
	bool Q_state = true;

	bool Motion_Q_state_prv = true;
	bool Motion_Q_state = true;

	int Motion_cmd = 0;
	CEdit m_edit_send_view;
	afx_msg void OnEnChangeEditReceivedata();
	CEdit m_edit_rcv_view;
	CEdit m_edit_send_data2;

	vector<vector<Rect>> detected_obj = vector<vector<Rect>>(6);
	bool detect_result[6];


	afx_msg void OnBnClickedButtonReady();
	afx_msg void OnBnClickedButtonStart();
	bool ready_state = false;
	bool start_state = false;
	afx_msg void OnEnChangeEditSenddata();
	int Q_check_num = 0;

	int target_center_x;
	int target_center_y;
	int target_width;
	int target_height;

	double target_distance_x;
	double target_distance_y;
	double target_angle;

	int angle_mode = 0;

	afx_msg void OnEnChangeEditUd();

	bool end = true;

	vector<Vec3b> Color_RGB[6];
	POINT pos;
	bool pos_switch = 0;
	CRect rect_img1;
	afx_msg void OnStnClickedStaticImg1();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int save_data = false;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonErase();
	void erase_color();

	double init_ra = 0.1;
	int init_b;
	int init_g;
	int init_r;
	
	int Motion_cnt = 0;

	// 2019-08-25
	bool image_change = false;
	void Hanoi();
	int hanoi_num = 1;
	int hanoi_num_save = 1;
	int rotation_right_or_left = 0;
	bool Thread_chk = true;
	bool puck_chk = false;
	bool goal_chk = false;

	int rotation_cnt = 0;

	const int goal_x = img_w / 2;
	const int goal_y = img_h / 2;
	const int adjust_goal = 0;

	const int puck_x[6] = {0,120};
	const int puck_y[6] = {0, 105};
	const int adjust_puck[6] = {0,5};

	int control_motion = 10;

	int exception = 0;

	afx_msg void OnBnClickedButtonSave1();
	afx_msg void OnBnClickedButtonLoad1();
};

//초기값

#define M_NONE 0

//전진

#define M_FORWARD 10
#define STR_FORWARD L"JL0200;"
#define M_C_FORWARD 16
#define STR_C_FORWARD L"JK0200;"
#define M_T_FORWARD 50
#define STR_T_FORWARD L"JR0008;"

//후진

#define M_BACKWARD 11
#define STR_BACKWARD L"JL2000;"
#define M_C_BACKWARD 17
#define STR_C_BACKWARD L"JK2000;"

//좌보행

#define M_LEFTSTEP 12
#define STR_LEFTSTEP L"JL8000;"
#define M_C_LEFTSTEP 18
#define STR_C_LEFTSTEP L"JK8000;"
#define M_L_LEFTSTEP 22
#define STR_L_LEFTSTEP L"JR8000;"

//우보행

#define M_RIGHTSTEP 13
#define STR_RIGHTSTEP L"JL0800;"
#define M_C_RIGHTSTEP 19
#define STR_C_RIGHTSTEP L"JK0800;"
#define M_L_RIGHTSTEP 23
#define STR_L_RIGHTSTEP L"JR0800;"

//좌회전

#define M_LEFTROTATION 14
#define STR_LEFTROTATION L"JL4444;"
#define M_C_LEFTROTATION 20
#define STR_C_LEFTROTATION L"JK4444;"
#define M_L_LEFTROTATION 24
#define STR_L_LEFTROTATION L"JR4444;"


//우회전

#define M_RIGHTROTATION 15
#define STR_RIGHTROTATION L"JL8888;"
#define M_C_RIGHTROTATION 21
#define STR_C_RIGHTROTATION L"JK8888;"
#define M_L_RIGHTROTATION 25
#define STR_L_RIGHTROTATION L"JR8888;"

//컬링 이벤트 모션

#define M_PUCKLEFTROTATION 100
#define STR_PUCKLEFTROTATION L"JL0100;"
#define M_PUCKRIGHTROTATION 101
#define STR_PUCKRIGHTROTATION L"JL0400;"

#define M_PUCK1 102
#define STR_PUCK1 L"JL0001;"
#define M_PUCK2 103
#define STR_PUCK2 L"JL0002;"

#define M_HANOI_GET1 104
#define STR_HANOI_GET1 L"JR0001;"

#define M_HANOI_GET2 105
#define STR_HANOI_GET2 L"JR0001;"

#define M_HANOI_GET3 106
#define STR_HANOI_GET3 L"JR0001;"

#define M_HANOI_GET4 107
#define STR_HANOI_GET4 L"JR0001;"

#define M_HANOI_GET5 108
#define STR_HANOI_GET5 L"JR0001;"

#define M_HANOI_PUT1 109
#define STR_HANOI_PUT1 L"JR0007;"

#define M_HANOI_PUT2 110
#define STR_HANOI_PUT2 L"JR0007;"

#define M_HANOI_PUT3 111
#define STR_HANOI_PUT3 L"JR0007;"

#define M_HANOI_PUT4 112
#define STR_HANOI_PUT4 L"JR0007;"

#define M_HANOI_PUT5 113
#define STR_HANOI_PUT5 L"JR0007;"

#define M_HANOI 114
#define STR_HANOI "JR4444;"

#define PI 3.141592
#define UD_100 100
#define UD_140 140