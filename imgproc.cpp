#include"stdafx.h"
#include"Homework.h"
#include"HomeworkDlg.h"

UINT img_thread(LPVOID arg) {
	CHomeworkDlg* pDlg = (CHomeworkDlg*)arg;
	
	while (true) {
		pDlg->img_proc();
		pDlg->Hanoi();
		WaitForSingleObject(pDlg->m_p_img_thread->m_hThread, 1);
	}
}

void CHomeworkDlg::img_proc() {
	if (vcap.isOpened()) {
		while (!vcap.read(Origin_img));
		
		if (Origin_img.data) {
			int tmp_img_width = Origin_img.cols;
			if (tmp_img_width != img_w) {
				resize(Origin_img, Origin_img, cv::Size(img_w, img_h));
			}
			Show_img = Origin_img.clone();
		}
		else {
			vcap.release();
			vcap = VideoCapture(0);
			if (!vcap.isOpened()) {
				AfxMessageBox(_T("CAM_ERROR_E00!"));
			}
			return;
		}
	}
	else {
		vcap.release();
		vcap = VideoCapture(0);
		if (!vcap.isOpened()) {
			AfxMessageBox(_T("CAM_ERROR_E01!"));
		}
		return;
	}
	int64 start_point = getTickCount();
	vector<vector<Rect>>().swap(detected_obj);
	detected_obj = vector<vector<Rect>>(6);
	if (image_change) {
		double up = 0.3;
		double down = 0.3;
		int i = hanoi_num;
		int color_size = Color_RGB[i].size();
		if (color_size > 0) {
			Mat tmpp_bin_img;
			tmp_extracted_img[i] = Origin_img.clone();
			cvtColor(tmp_extracted_img[i], tmp_extracted_img[i], COLOR_BGR2HSV);
			Scalar lower, upper;
			for (int j = 0; j < color_size; j++) {
				Mat tmp_bin_img;
				lower = Scalar(Color_RGB[i][j][0], Color_RGB[i][j][1] - up * Color_RGB[i][j][1], Color_RGB[i][j][2] - up * Color_RGB[i][j][2]);
				upper = Scalar(Color_RGB[i][j][0], Color_RGB[i][j][1] + down * Color_RGB[i][j][1], Color_RGB[i][j][2] + down * Color_RGB[i][j][2]);
				inRange(tmp_extracted_img[i], lower, upper, tmp_bin_img);
				if (j == 0) {
					tmpp_bin_img = tmp_bin_img;
				}
				else {
					tmpp_bin_img |= tmp_bin_img;
				}
			}
			tmpp_bin_img *= 0.00392156;
			tmp_ext_bin_img[i] = tmpp_bin_img.clone();
			cvtColor(tmpp_bin_img, tmpp_bin_img, COLOR_GRAY2BGR);
			tmp_extracted_img[i] = tmp_extracted_img[i].mul(tmpp_bin_img);

			cvtColor(tmp_extracted_img[i], tmp_extracted_img[i], COLOR_HSV2BGR);

			Mat label, stats, centroids;
			int label_num = connectedComponentsWithStats(tmp_ext_bin_img[i], label, stats, centroids, 8, CV_32S);
			int* stats_pointer = (int*)stats.data;
			double* centroids_pointer = (double*)centroids.data;

			for (int j = 1; j < label_num; j++) {
				int stats_idx = j * 5;
				int centroids_idx = j * 2;

				int left = *(stats_pointer + stats_idx + CC_STAT_LEFT);
				int top = *(stats_pointer + stats_idx + CC_STAT_TOP);
				int width = *(stats_pointer + stats_idx + CC_STAT_WIDTH);
				int height = *(stats_pointer + stats_idx + CC_STAT_HEIGHT);
				int area = *(stats_pointer + stats_idx + CC_STAT_AREA);

				int centroids_x = (int)round(*(centroids_pointer + centroids_idx + 0));
				int centroids_y = (int)round(*(centroids_pointer + centroids_idx + 1));

				Rect tmp_rectangle = Rect(left, top, width, height);

				if (area > 200) {
					if (hanoi_num == i) {
						SetDlgItemInt(IDC_EDIT_G, centroids_x);
						SetDlgItemInt(IDC_EDIT_R, centroids_y);
					}
					target_center_x = centroids_x;
					target_center_y = centroids_y;
					target_width = width;
					target_height = height;
					detected_obj[i].push_back(tmp_rectangle);
					rectangle(Show_img, tmp_rectangle, rec_color[i], 2, 8);
					circle(Show_img, Point(centroids_x, centroids_y), 3, cir_color[i], 2, 8);
				}
			}
		}
	}
	else {
		int i = hanoi_num;
		cvtColor(Origin_img, tmp_extracted_img[i], COLOR_BGR2HSV);
		Scalar lower, upper;
		if (H_max[i] >= H_min[i])
		{
			Mat tmp_bin_img;
			lower = Scalar(H_min[i], S_min[i], V_min[i]);
			upper = Scalar(H_max[i], S_max[i], V_max[i]);
			inRange(tmp_extracted_img[i], lower, upper, tmp_bin_img);
			tmp_bin_img *= 0.00392156;
			tmp_ext_bin_img[i] = tmp_bin_img.clone();
			cvtColor(tmp_bin_img, tmp_bin_img, COLOR_GRAY2BGR);
			tmp_extracted_img[i] = tmp_extracted_img[i].mul(tmp_bin_img);
		}
		else
		{
			Mat tmp_bin_img1, tmp_bin_img2;
			lower = Scalar(0, S_min[i], V_min[i]);
			upper = Scalar(H_max[i], S_max[i], V_max[i]);
			inRange(tmp_extracted_img[i], lower, upper, tmp_bin_img1);
			lower = Scalar(H_min[i], S_min[i], V_min[i]);
			upper = Scalar(180, S_max[i], V_max[i]);
			inRange(tmp_extracted_img[i], lower, upper, tmp_bin_img2);
			tmp_ext_bin_img[i] = tmp_bin_img1.clone();
			tmp_bin_img1 |= tmp_bin_img2;
			tmp_bin_img1 *= 0.00392156;
			cvtColor(tmp_bin_img1, tmp_bin_img1, COLOR_GRAY2BGR);
			tmp_extracted_img[i] = tmp_extracted_img[i].mul(tmp_bin_img1);
		}
		cvtColor(tmp_extracted_img[i], tmp_extracted_img[i], COLOR_HSV2BGR);

		Mat label, stats, centroids;
		int label_num = connectedComponentsWithStats(tmp_ext_bin_img[i], label, stats, centroids, 8, CV_32S);
		int* stats_pointer = (int*)stats.data;
		double* centroids_pointer = (double*)centroids.data;

		for (int j = 1; j < label_num; j++) {
			int stats_idx = j * 5;
			int centroids_idx = j * 2;

			int left = *(stats_pointer + stats_idx + CC_STAT_LEFT);
			int top = *(stats_pointer + stats_idx + CC_STAT_TOP);
			int width = *(stats_pointer + stats_idx + CC_STAT_WIDTH);
			int height = *(stats_pointer + stats_idx + CC_STAT_HEIGHT);
			int area = *(stats_pointer + stats_idx + CC_STAT_AREA);

			int centroids_x = (int)round(*(centroids_pointer + centroids_idx + 0));
			int centroids_y = (int)round(*(centroids_pointer + centroids_idx + 1));

			Rect tmp_rectangle = Rect(left, top, width, height);

			if (area > 100) {
				if (hanoi_num == i) {
					SetDlgItemInt(IDC_EDIT_G, centroids_x);
					SetDlgItemInt(IDC_EDIT_R, centroids_y);
				}
				target_center_x = centroids_x;
				target_center_y = centroids_y;
				target_width = width;
				target_height = height;
				detected_obj[i].push_back(tmp_rectangle);
				rectangle(Show_img, tmp_rectangle, rec_color[i], 2, 8);
				circle(Show_img, Point(centroids_x, centroids_y), 3, cir_color[i], 2, 8);
			}
		}
	}

	if ((image_change && Color_RGB[hanoi_num].size() > 0) || image_change == false) {
		img_view(&tmp_extracted_img[hanoi_num], 1);
	}

	for (int i = 0; i < 6; i++) {
		if (detected_obj[i].size() > 0) {
			detect_result[i] = true;
		}
		else {
			detect_result[i] = false;
		}
	}

	if (detect_result[0] == true && hanoi_num == 0) {
		goal_chk = true;
		puck_chk = false;
	}
	else if (detect_result[hanoi_num] == true) {
		goal_chk = false;
		puck_chk = true;
	}
	else {
		goal_chk = false;
		puck_chk = false;
	}
	
	int64 end_point = getTickCount();
	double freq = getTickFrequency();
	double process_time = double(end_point - start_point) / freq * 1000; //√ ¥‹¿ß∂Ûº≠ 1000∞ˆ«ÿ¡‡º≠ ¥‹¿Ã§ÃπŸ≤ﬁ
	CString tmp_process_time;
	tmp_process_time.Format(L"%.2f ms", process_time);
	CString tmp;
	tmp.Format(L"%d, %d", goal_chk, puck_chk);
	SetDlgItemText(IDC_EDIT_PROCESS_TIME, tmp); //tmp_process_time

	tmp.Format(L"H : %d", hanoi_num);
	SetDlgItemText(IDC_EDIT_B, tmp); //tmp_process_time
	img_view(&Show_img, 0);
}

void CHomeworkDlg::img_view(Mat * img, int order)
{
	int bpp = 8 * (int)(*img).elemSize();
	CStatic* tmp_st;
	switch (order)
	{
	case 0:
	default:
		tmp_st = &m_st_img1;
		break;
	case 1:
		tmp_st = &m_st_img2;
		break;
	}
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	if (bpp < 32)
		padding = 4 - ((*img).cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	if (bpp < 32)
	{
		border = 4 - ((*img).cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || (*img).isContinuous() == false)
	{
		cv::copyMakeBorder((*img), mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = (*img);
	}


	RECT r;
	(*tmp_st).GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);

	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;

	if (mat_temp.cols == winSize.width  && mat_temp.rows == winSize.height)
	{

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	HDC dc = ::GetDC((*tmp_st).m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);

	::ReleaseDC((*tmp_st).m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();

}