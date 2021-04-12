#include"stdafx.h"
#include"Homework.h"
#include"HomeworkDlg.h"


void CHomeworkDlg::Hanoi() {
	if (ready_state&&start_state) {
		int delay = 0;
		if (exception != 0) {
			if (Q_state == true) {
				if (exception == 1) {
					if (puck_chk == true) {
						exception = 0;
						Motion_cmd = 10;
					}
					else {
						if (hanoi_num == 1) {
							hanoi_num = 0;
							exception = 0;
						}
						else {
							hanoi_num = 0;
							exception = 0;
							UD_NeckAngle = 101;
							neck_angle_chg_UD = true;
						}
					}
				}
				else if (exception == 2) {
					hanoi_num_save++;
					hanoi_num = hanoi_num_save;
					exception = 0;
					UD_NeckAngle = 140;
					neck_angle_chg_UD = true;
				}
			}
		}
		else if (UD_NeckAngle == 100 && RL_NeckAngle == 180) {
			if (puck_chk == false && goal_chk == false) {
				control_motion = 10;
				neck_angle_chg_max = 30;
				Motion_cmd = M_C_FORWARD;
			}
			else if (goal_chk == true) {
				if (abs(goal_x - target_center_x) <= 30 && abs(goal_y - target_center_y) <= 30) {
					control_motion = 75;
					neck_angle_chg_max = 5;
				}
				if (abs(goal_x - target_center_x) < adjust_goal && abs(goal_y - target_center_y) < adjust_goal) {
					// ������ �ֱ� -> 140
					Motion_cmd = M_HANOI_PUT1;
					delay = 5000;
					exception = 2;
				}
				else if (abs(goal_x - target_center_x) > abs(goal_y - target_center_y)) {
					if (goal_x - target_center_x > 0) {
						Motion_cmd = M_LEFTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Left");
					}
					else {
						Motion_cmd = M_RIGHTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Right");
					}
				}
				else {
					if (goal_y - target_center_y > 0) {
						Motion_cmd = M_FORWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Forward");
					}
					else {
						Motion_cmd = M_BACKWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Backward");
					}
				}
			}
			else if (puck_chk == true) {
				// ������ ���� -> 101
				if (abs(puck_x[hanoi_num] - target_center_x) <= 30 && abs(puck_y[hanoi_num] - target_center_y) <= 30) {
					control_motion = 75;
					neck_angle_chg_max = 5;
				}
				if (abs(puck_x[hanoi_num] - target_center_x) < adjust_puck[hanoi_num] && abs(puck_y[hanoi_num] - target_center_y) < adjust_puck[hanoi_num]) {
					Motion_cmd = M_HANOI_GET1;
					delay = 5000;
					exception = 1;
				}
				else if (abs(puck_x[hanoi_num] - target_center_x) > abs(puck_y[hanoi_num] - target_center_y)) {
					if (puck_x[hanoi_num] - target_center_x > 0) {
						Motion_cmd = M_LEFTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"101-Left");
					}
					else {
						Motion_cmd = M_RIGHTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"101-Right");
					}
				}
				else {
					if (puck_y[hanoi_num] - target_center_y > 0) {
						Motion_cmd = M_FORWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"101-Forward");
					}
					else {
						Motion_cmd = M_BACKWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"101-Backward");
					}
				}
			}
		}
		else if (UD_NeckAngle == 140 && RL_NeckAngle == 180) {
			if (rotation_right_or_left != 0) {
				// 141��
			}
			else if (detect_result[hanoi_num] == true) {
				double x = detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width;
				double y = detected_obj[hanoi_num][0].y + detected_obj[hanoi_num][0].height;
				if (x - img_w / 2 > 0) {
					rotation_right_or_left = 1;
					// ���� ȸ�� -> 141��
				}
				else {
					rotation_right_or_left = 2;
					// ������ ȸ�� -> 141��
				}
			}
			else {
				//RL ������
			}
		}
		Sleep(delay);
		delay = 0;
	}
}

/*
void CHomeworkDlg::Hanoi() {
	if (ready_state&&start_state&&Thread_chk==false) {
		if (UD_NeckAngle == 100 && RL_NeckAngle == 180) {
			if (puck_chk == false && goal_chk == false) {
				Motion_cmd = M_C_FORWARD;
			}
			else if (goal_chk == true) {
				if (abs(goal_x_110 - target_center_x) < adjust_goal_110 && abs(goal_y_110 - target_center_y) < adjust_goal_110) {
					UD_NeckAngle = 140;
					neck_angle_chg_UD = true;
					// ������ �ֱ� -> 140
				}
				else if (abs(goal_x_110 - target_center_x) > abs(goal_y_110 - target_center_y)) {
					if (goal_y_110 - target_center_x > 0) {
						Motion_cmd = M_LEFTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Left");
					}
					else {
						Motion_cmd = M_RIGHTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Right");
					}
				}
				else {
					if (goal_y_110 - target_center_y > 0) {
						Motion_cmd = M_FORWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Forward");
					}
					else {
						Motion_cmd = M_BACKWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Backward");
					}
				}
				hanoi_num++;
			}
			else if (puck_chk == true) {
				// ������ ���� -> 101
				if (abs(puck_x_110 - target_center_x) < adjust_goal_110 && abs(puck_y_110 - target_center_y) < adjust_puck_110) {
					UD_NeckAngle = 101;
					neck_angle_chg_UD = true;
					hanoi_num = 0;
				}
				else if (abs(puck_x_110 - target_center_x) > abs(puck_y_110 - target_center_y)) {
					if (puck_y_110 - target_center_x > 0) {
						Motion_cmd = M_LEFTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Left");
					}
					else {
						Motion_cmd = M_RIGHTSTEP;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Right");
					}
				}
				else {
					if (puck_y_110 - target_center_y > 0) {
						Motion_cmd = M_FORWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Forward");
					}
					else {
						Motion_cmd = M_BACKWARD;
						SetDlgItemText(IDC_EDIT_ANGLE, L"140-Backward");
					}
				}
			}
		}
		else if (UD_NeckAngle == 101 && RL_NeckAngle == 180) {
			Motion_Q_state_prv = Motion_Q_state;
			Motion_Q_state = Q_state;
			if (rotation_cnt < 2) {
				if (Motion_Q_state == true) {
					// ȸ��
				}
				if (Motion_Q_state == true && Motion_Q_state_prv == false) {
					rotation_cnt++;
				}
			}
			else {
				//100��
			}
		}
		else if (UD_NeckAngle == 140 && RL_NeckAngle == 180) {
			if (rotation_right_or_left != 0) {
				// 141��
			}
			else if (detect_result[hanoi_num] == true) {
				double x = detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width;
				double y = detected_obj[hanoi_num][0].y + detected_obj[hanoi_num][0].height;
				if (x - img_w / 2 > 0) {
					rotation_right_or_left = 1;
					// ���� ȸ�� -> 141��
				}
				else {
					rotation_right_or_left = 2;
					// ������ ȸ�� -> 141��
				}
			}
			else {
				//RL ������
			}
		}
		else if (UD_NeckAngle == 140 && RL_NeckAngle == 100) {
			if (detect_result[hanoi_num] == true) {
			}
			else {
			}
		}
		else if (UD_NeckAngle == 140 && RL_NeckAngle == 260) {
			if (detect_result[hanoi_num] == true) {
			}
			else {
			}
		}
		else if (UD_NeckAngle == 141 && RL_NeckAngle == 180) {
			// ���ڸ� 180�� ȸ��
			if (rotation_right_or_left == 0) {
				// 139��
			}
			else {
				// 142��
			}
		}
		else if (UD_NeckAngle == 139 && RL_NeckAngle == 180) {
			double x = detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width;
			double y = detected_obj[hanoi_num][0].y + detected_obj[hanoi_num][0].height;
			if (x - img_w / 2 > 0) {
				rotation_right_or_left = 2;
				// ������ ȸ�� -> 142��
			}
			else {
				rotation_right_or_left = 1;
				// ������ ȸ�� -> 142��
			}
		}
		else if (UD_NeckAngle == 142 && RL_NeckAngle == 180) {
			double x = detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width;
			double y = detected_obj[hanoi_num][0].y + detected_obj[hanoi_num][0].height;
			// ���߰� �밡�� �ڱ�
		}
	}
}
*/
/*
void CHomeworkDlg::Hanoi() {
	if (hanoi_num == 0) {
		if (navy == false) {
			if (detect_result[1] == false) {
				// ����
			}
			else {
				// �̼������� ����
				navy = true;
				hanoi_num = 2;
			}
		}
		else {

		}
	}
	else if (hanoi_num == 1) {
		if (detect_result[1] == false) {
			// ����
		}
		else {
			// �̼����� �� ���
			hanoi_num = 0;
		}
	}
	else {
		if (find_hanoi == false && determine_rotation <= 3) {
			if (detect_result[hanoi_num] == true) {
				find_hanoi = true;
				if (determine_rotation == 0) {
					if (detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width > img_w / 2) {
						// �����ʿ� ���� -> �������� ȸ��
					}
					else {
						// ���ʿ� ���� -> ���������� ȸ��
					}
				}
				else if (determine_rotation >= 1 && detect_result[hanoi_num] == true) {
					find_hanoi = true;
					// �Ӹ� ���ڸ�
				}
			}
			else {
				determine_rotation++;
				if (determine_rotation == 1) {
					// �Ӹ� ���� ȸ��
				}
				else if(determine_rotation == 1) {
					// �Ӹ� ������ ȸ��
				}
			}
		}
		else {
			// ���ڸ� �ѹ��� ����
		    if (determine_rotation >= 3) {
				find_hanoi = true;
				if (detected_obj[hanoi_num][0].x + detected_obj[hanoi_num][0].width > img_w / 2) {
					// �����ʿ� ���� -> ������ ȸ��
				}
				else {
					// ���ʿ� ���� -> ������ ȸ��
				}
			}
			else if (determine_rotation <= 2) {
				// ���� ���߱�
			}
		}
	}
}
*/