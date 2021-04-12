#pragma once

#include "stdafx.h"
#include "Homework.h"
#include "HomeworkDlg.h"

void CHomeworkDlg::send_data_to_dsp()
{
	static int init_cnt = 0;
	static int send_cnt_10ms = 0;
	static int Send_wait_cnt = 0;
	static int J_Order_cnt = 0;
	static int J_wait = 0;
	static int Motion_contionuetoone = 0;
	int send_max_cnt = 2;
	if (comm_1 && comm_1->m_bIsOpenned)
	{
		CString str = _T(";");
		if (SendNReturn)
		{
			SendNReturn = false;
			Send_wait_cnt = 0;

			if (send_cnt_10ms == 0)
			{
				if (data_from_typing) {
					str = m_str_edit_senddata2;
					data_from_typing = false;
					m_str_edit_senddata2.Empty();
					SetDlgItemText(IDC_EDIT_SENDDATA2, m_str_edit_senddata2);
				}
				else {
					str = L"Q?;";
					// Q? -> Q?000;(모션이 동작중이 아님) ,  Q?001;(모션이 동작중임)
				}
				;/* 통신으로 DSP에 보낼 명령어를 만드는 부분*/
			}
			else if (send_cnt_10ms == 1)
			{
				if (neck_angle_chg_UD) {
					int tmp_angle = 20000 + cvRound((double)(360 - UD_NeckAngle) * 55.555555);
					str.Format(L"PS00024,%d;", tmp_angle); // 00024 모터 제어
					neck_angle_chg_UD_cnt++;
					if (neck_angle_chg_UD_cnt >= neck_angle_chg_max) {
						neck_angle_chg_UD = false;
						neck_angle_chg_UD_cnt = 0;
					}
				}
				else if (neck_angle_chg_RL) {
					int tmp_angle = 20000 + cvRound((double)RL_NeckAngle * 55.555555);
					str.Format(L"PS00023,%d;", tmp_angle); // 00023 모터 제어
					neck_angle_chg_RL_cnt++;
					if (neck_angle_chg_RL_cnt >= neck_angle_chg_max) {
						neck_angle_chg_RL = false;
						neck_angle_chg_RL_cnt = 0;
					}
				}
				else if (neck_angle_chg_UD == false && neck_angle_chg_RL == false) {
					if (Q_state && Motion_cmd != M_NONE && Motion_cmd != M_C_FORWARD) {
						switch (Motion_cmd) {
						default:
							str = L";";
						case M_FORWARD:
							str = STR_FORWARD;
							break;
						case M_BACKWARD:
							str = STR_BACKWARD;
							break;
						case M_LEFTSTEP:
							str = STR_LEFTSTEP;
							break;
						case M_RIGHTSTEP:
							str = STR_RIGHTSTEP;
							break;
						case M_LEFTROTATION:
							str = STR_LEFTROTATION;
							break;
						case M_RIGHTROTATION:
							str = STR_RIGHTROTATION;
							break;
						case M_C_FORWARD:
							str = STR_C_FORWARD;
							break;
						case M_C_BACKWARD:
							str = STR_C_BACKWARD;
							break;
						case M_C_LEFTSTEP:
							str = STR_C_LEFTSTEP;
							break;
						case M_C_RIGHTSTEP:
							str = STR_C_RIGHTSTEP;
							break;
						case M_C_LEFTROTATION:
							str = STR_C_LEFTROTATION;
							break;
						case M_C_RIGHTROTATION:
							str = STR_C_RIGHTROTATION;
							break;
						case M_L_LEFTSTEP:
							str = STR_L_LEFTSTEP;
						case M_L_RIGHTSTEP:
							str = STR_L_RIGHTSTEP;
							break;
						case M_L_LEFTROTATION:
							str = STR_L_LEFTROTATION;
							break;
						case M_L_RIGHTROTATION:
							str = STR_L_RIGHTROTATION;
							break;
						case M_PUCKLEFTROTATION:
							str = STR_PUCKLEFTROTATION;
							break;
						case M_PUCKRIGHTROTATION:
							str = STR_PUCKRIGHTROTATION;
							break;
						case M_PUCK1:
							str = STR_PUCK1;
							break;
						case M_PUCK2:
							str = STR_PUCK2;
							break;
						case M_T_FORWARD:
							str = STR_T_FORWARD;
							break;
						case M_HANOI_GET1:
							str = STR_HANOI_GET1;
							break;
						case M_HANOI_GET2:
							str = STR_HANOI_GET2;
							break;
						case M_HANOI_GET3:
							str = STR_HANOI_GET3;
							break;
						case M_HANOI_GET4:
							str = STR_HANOI_GET4;
							break;
						case M_HANOI_GET5:
							str = STR_HANOI_GET5;
							break;
						case M_HANOI_PUT1:
							str = STR_HANOI_PUT1;
							break;
						case M_HANOI_PUT2:
							str = STR_HANOI_PUT2;
							break;
						case M_HANOI_PUT3:
							str = STR_HANOI_PUT3;
							break;
						case M_HANOI_PUT4:
							str = STR_HANOI_PUT4;
							break;
						case M_HANOI_PUT5:
							str = STR_HANOI_PUT5;
							break;
						case M_HANOI:
							str = STR_HANOI;
							break;
						}
						Motion_cmd = M_NONE;
					}
					else if (Motion_cmd == M_C_FORWARD) {
						str = STR_C_FORWARD;
						Motion_cmd = M_NONE;
					}
				}
			}
			//SetDlgItemInt(IDC_EDIT_DISTANCE, Thread_chk);
			//SetDlgItemInt(IDC_EDIT_ANGLE, neck_angle_chg_UD_cnt);
			CString ok;
			ok.Format(L"%s", str);
			//SetDlgItemText(IDC_EDIT_MOTION, ok);
			if (++send_cnt_10ms >= send_max_cnt)
				send_cnt_10ms = 0;
			comm_1->Send(str, str.GetLength());   // send data to comm1
			str.ReleaseBuffer();
			str += _T("\r\n");
			m_edit_send_view.ReplaceSel(str); // view state in edit box
			m_edit_send_view.LineScroll(m_edit_send_view.GetLineCount());
		}
		else
		{
			if (Send_wait_cnt++ > control_motion)
			{
				SendNReturn = true;
				Send_wait_cnt = 0;
			}
		}
		SetDlgItemInt(IDC_EDIT_DISTANCE, control_motion);
	}
}

UINT CommThread(LPVOID arg)
{
	CHomeworkDlg* pDlg = (CHomeworkDlg*)arg;

	extern short   g_nRemoteStatus;
	DWORD      ErrorFlags;
	COMSTAT      ComStat1;
	DWORD      EvtMask;
	char      buf[MAXBUF];
	DWORD      Length;
	int   size;
	int   insize = 0;
	static int Comm1_finsh;

	while (1)
	{
		//pDlg->wait(1);   // wait for thread
		WaitForSingleObject(pDlg->m_receive_thread->m_hThread, 1);
		if (pDlg->comm_1 && pDlg->comm_1->m_bIsOpenned) // Get data from DSP
		{
			Comm1_finsh = 1;

			EvtMask = 0;
			Length = 0;
			insize = 0;
			memset(buf, '\0', MAXBUF);

			WaitCommEvent(pDlg->comm_1->m_hComDev, &EvtMask, NULL);

			ClearCommError(pDlg->comm_1->m_hComDev, &ErrorFlags, &ComStat1);

			if ((EvtMask & EV_RXCHAR) && ComStat1.cbInQue)
			{
				if (ComStat1.cbInQue > MAXBUF)
					size = MAXBUF;
				else
					size = ComStat1.cbInQue;

				do
				{
					ClearCommError(pDlg->comm_1->m_hComDev, &ErrorFlags, &ComStat1);
					if (!ReadFile(pDlg->comm_1->m_hComDev, buf + insize, size, &Length, &(pDlg->comm_1->m_OLR)))
					{
						// 에러
						TRACE("Error in ReadFile\n");
						if (GetLastError() == ERROR_IO_PENDING)
						{
							if (WaitForSingleObject(pDlg->comm_1->m_OLR.hEvent, 1000) != WAIT_OBJECT_0)   Length = 0;
							else   GetOverlappedResult(pDlg->comm_1->m_hComDev, &(pDlg->comm_1->m_OLR), &Length, FALSE);
						}
						else
							Length = 0;
					}
					insize += Length;
				} while ((Length != 0) && (insize < size));

				ClearCommError(pDlg->comm_1->m_hComDev, &ErrorFlags, &ComStat1);

				if (pDlg->comm_1->m_nLength + insize > MAXBUF * 2)
					insize = (pDlg->comm_1->m_nLength + insize) - MAXBUF * 2;

				pDlg->comm_1->m_pEvent->ResetEvent();
				memcpy(pDlg->comm_1->m_sInBuf + pDlg->comm_1->m_nLength, buf, insize);
				pDlg->comm_1->m_nLength += insize;
				pDlg->comm_1->m_pEvent->SetEvent();
				LPARAM temp = (LPARAM)pDlg->comm_1;
				SendMessage(pDlg->comm_1->m_hWnd, WM_DSP_RECEIVE, pDlg->comm_1->m_nLength, temp);
				pDlg->comm_1->m_send_return_flg = false;
			}
		}

		else if (Comm1_finsh == 1)   // close
		{
			//pDlg->wait(1);
			WaitForSingleObject(pDlg->m_receive_thread->m_hThread, 1);
			Comm1_finsh = 0;
			PurgeComm(pDlg->comm_1->m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			LPARAM temp = (LPARAM)pDlg->comm_1;
			SendMessage(pDlg->comm_1->m_hWnd, WM_DSP_CLOSE, 0, temp);
		}
		WaitForSingleObject(pDlg->m_receive_thread->m_hThread, 5);
		//pDlg->wait(1);
	}
	return 0;
}

UINT CommSendThread(LPVOID arg)
{
	CHomeworkDlg* pDlg = (CHomeworkDlg*)arg;

	while (1)
	{
		//pDlg->wait(1);
		WaitForSingleObject(pDlg->m_send_thread->m_hThread, 1);
		if (pDlg->comm_1 && pDlg->comm_1->m_bIsOpenned)
		{
			pDlg->send_data_to_dsp();
		}
	}
	return 0;
}

LRESULT CHomeworkDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped close handle
	((CSerialComm*)lpara)->HandleClose();
	delete ((CSerialComm*)lpara);

	return 0;
}

LRESULT CHomeworkDlg::OnReceive(WPARAM length, LPARAM lpara)
{

	char data[1000]; // data buf
	memset(data, 0, sizeof(data)); // set
	if (comm_1)
	{
		if (comm_1->m_send_return_flg)
		{
			int werf = 0;
		}

		if (length > 1000) // 중단점을 걸 때, 수신 버퍼에 계속 쌓이는 현상을 막아줌
		{
			comm_1->m_nLength = 0;
			return 0;
		}
		comm_1->Receive(data, (int)length); // get data

		CString str;
		data[length] = _T('\0');

		int idx = 0;
		for (int i = 0; i < (int)length; i++)   str += data[i];
		if (str.Find(L";") != -1 || str.Find(L":") != -1)
		{
			str = remain_data + str;
			str += _T("\r\n");
			remain_data = _T("");
			SendNReturn = true;
			if (str.Find(L"Q?") != -1) {
				int Q_check_limit = 5;
				if (str.Find(L"Q?0000;") != -1) { // 안움직임
					if (++Q_check_num >= Q_check_limit) {
						Q_state = true;
						Q_check_num = Q_check_limit;
					}
				}
				else if (str.Find(L"Q?0001;") != -1) {
					Q_state = false;
					Q_check_num = 0;

				}
			}
			m_edit_rcv_view.ReplaceSel(str); // view state in edit box
			m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());
			///////   Log Auto Erase   ///
			GetDlgItemText(IDC_EDIT_RECEIVEDATA, str);
			int TMP_length = str.GetLength();
			str.ReleaseBuffer();
			int TMP_Limit = m_edit_rcv_view.GetLimitText() - 100;
			if (TMP_length > TMP_Limit)
			{
				str.Empty();
				SetDlgItemText(IDC_EDIT_RECEIVEDATA, str);
				SetDlgItemText(IDC_EDIT_SENDDATA, str);
			}
		}
		else
		{
			remain_data += str;
		}
	}
	return 0;
}