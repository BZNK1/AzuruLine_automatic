// Azuruline_map.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
using namespace std;
using namespace cv;

struct posi{
	int x, y;
};

Mat pics[21] = {};
HWND window;
int cc,a;


//获得bmp截图
inline HBITMAP GetCaptureBmp()
{
	HDC     hDC;
	HDC     MemDC;
	BYTE*   Data;
	HBITMAP   hBmp;
	BITMAPINFO   bi;

	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bi.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
	bi.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;

	hDC = GetDC(NULL);
	MemDC = CreateCompatibleDC(hDC);
	hBmp = CreateDIBSection(MemDC, &bi, DIB_RGB_COLORS, (void**)&Data, NULL, 0);
	SelectObject(MemDC, hBmp);
	BitBlt(MemDC, 0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, hDC, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hDC);
	DeleteDC(MemDC);
	return   hBmp;
}


//bmp转mat
inline BOOL HBitmapToMat(HBITMAP& _hBmp, Mat& _mat)
{
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	Mat v_mat;
	v_mat.create(Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data);
	_mat = v_mat;
	return TRUE;
}


//截图调用函数
inline Mat GetCurrentScreen() {
	HBITMAP	screen;
	screen = GetCaptureBmp();
	Mat src;
	HBitmapToMat(screen, src);
	imshow("screen",src);
	waitKey(0);
	return src;
}


inline void mouse(int oper,int x,int y) {
	x += rand() % 7;
	y += rand() % 7;
	//处理鼠标按键消息
	switch(oper){
	case 0:
		//处理鼠标悬浮事件，这里的MAKELPARAM(x,y)表示鼠标位置
		::SetCursorPos(x, y);
		break;
	case 1:
		//处理鼠标左键按下事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		break;
	case 2:
		//处理鼠标左键抬起事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	case 3:
		//处理鼠标左键双击事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(100);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	case 4:
		//处理鼠标右键按下事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		break;
	case 5:
		//处理鼠标右键抬起事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		break;
	case 6:
		//处理鼠标右键双击事件
		::PostMessage(window,WM_RBUTTONDBLCLK,0,MAKELPARAM(x,y));
		break;
	case 7:
		//处理鼠标中键按下事件
		::PostMessage(window,WM_MBUTTONDOWN,MK_MBUTTON,MAKELPARAM(x,y));
		break;
	case 8:
		//处理鼠标中键抬起事件
		::PostMessage(window,WM_MBUTTONUP, MK_MBUTTON, MAKELPARAM(x,y));
		break;
	case 9:
		//处理鼠标中键双击事件
		::PostMessage(window,WM_MBUTTONDBLCLK,0,MAKELPARAM(x,y));
		break;
	case 10:
		//处理滚轮向前滚动事件，这里的MAKEWPARAM(0,120)，0表示没有任何附加按键按住，120表示滚轮滚动方向
		::PostMessage(window,WM_MOUSEWHEEL,MAKEWPARAM(0,120),MAKELPARAM(x,y));
		break;
	case 11:
		//处理滚轮向后滚动事件
		::PostMessage(window,WM_MOUSEWHEEL,MAKEWPARAM(0,-120),MAKELPARAM(x,y));
		break;
	case 12:
		//处理按住鼠标左键移动事件
		int delx, dely; POINT p;
		GetCursorPos(&p);
		delx = p.x - x; dely = p.y - y;
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		if (delx < 0) {
			for (int i = p.x; i <= x; i += 10) {
				::SetCursorPos(x, y);
				Sleep(10);
			}
		}
		else {
			for (int i = p.x; i >= x; i -= 10) {
				::SetCursorPos(x, y);
				Sleep(10);
			}
		}if (dely < 0) {
			for (int i = p.y; i <= y; i += 10) {
				::SetCursorPos(x, y);
				Sleep(10);
			}
		}
		else {
			for (int i = p.y; i >= y; i -= 10) {
				::SetCursorPos(x, y);
				Sleep(10);
			}
		}
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	case 13:
		//处理按住鼠标右键移动事件
		::PostMessage(window,WM_MOUSEMOVE,MK_RBUTTON,MAKELPARAM(x,y));
		break;
	case 14:
		//处理按住鼠标中键移动事件
		::PostMessage(window,WM_MOUSEMOVE,MK_MBUTTON,MAKELPARAM(x,y));
		break;
	case 15:
		//处理鼠标左键单击事件
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	default: 
		break;
	}
}


//坐标寻找
inline posi find_object(Mat src, Mat target) {
	cvtColor(src, src, COLOR_BGR2GRAY);
	cvtColor(target, target, COLOR_BGR2GRAY);
	Mat image_matched;
	//imshow("screen", src);
	//waitKey(0);
	imshow("target", target);
	waitKey(0);
	matchTemplate(src, target, image_matched, TM_CCOEFF_NORMED);
	double minVal, maxVal;
	Point minLoc, maxLoc;
	//寻找最佳匹配位置
	minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);
	Mat image_color;
	cvtColor(src, image_color, COLOR_GRAY2BGR);
	circle(image_color,
		Point(maxLoc.x + target.cols / 2, maxLoc.y + target.rows / 2),
		20,
		Scalar(0, 0, 255),
		2,
		8,
		0);

	//imshow("source image", src);
	//imshow("match result", image_matched);
	imshow("target", image_color);
	waitKey(0);
	
	//maxVal为匹配度,一般在0.7以上
	if (maxVal >= 0.7) {
		printf("position:(%d,%d),value:%f\n", maxLoc.x + target.cols / 2, maxLoc.y + target.rows / 2, maxVal);
		return{ maxLoc.x + target.cols / 2, maxLoc.y + target.rows / 2 };
	}
	else {
		printf("position:(%d,%d),value:%f;nagitive\n", maxLoc.x + target.cols / 2, maxLoc.y + target.rows / 2, maxVal);
		return { 0,0 };
	}
}


void inline position_adjust() {
	posi curloc = find_object(GetCurrentScreen(), pics[8]);
	posi stdloc = find_object(pics[14], pics[8]);
	mouse(0, curloc.x, curloc.y);
	mouse(12, stdloc.x, stdloc.y);

	printf("moving from (%d,%d) to (%d,%d)\n", curloc.x, curloc.y, stdloc.x, stdloc.y);

	return;
}


inline posi is_target_appeared() {
	Mat target = pics[3];
	Mat src = GetCurrentScreen();
	posi res = find_object(src, target);
	if (res.x != 0)return res;
	else {
		printf("boss hasn't appeared yet\n");
		return { 0,0 };
	}
}


inline void click_once(int index) {
	Mat target = pics[index];
	Mat src = GetCurrentScreen();
	posi locat;
	locat = find_object(src, target);
	if (locat.x == 0) {
		printf("no index:%d found\n", index);
		return;
	}
	printf("click index:%d position:(%d,%d)\n", index, locat.x, locat.y);
	mouse(16, locat.x, locat.y);

	return;
}


inline void init() {
	pics[1] = imread("./Data/attack.jpg");
	pics[2] = imread("./Data/autofight.jpg");
	pics[3] = imread("./Data/boss.jpg");
	pics[4] = imread("./Data/click2continue.jpg");
	pics[5] = imread("./Data/confirm.jpg");
	pics[6] = imread("./Data/go_rightnow.jpg");
	pics[7] = imread("./Data/mapA1.jpg");
	pics[8] = imread("./Data/mark.jpg");
	pics[9] = imread("./Data/queding.jpg");
	pics[10] = imread("./Data/ship1.jpg");
	pics[11] = imread("./Data/ship2.jpg");
	pics[12] = imread("./Data/ship3.jpg");
	pics[13] = imread("./Data/ship4.jpg");
	pics[14] = imread("./Data/standard.jpg");
	pics[15] = imread("./Data/switch.jpg");
	window = FindWindow("Qt5QWindowIcon", NULL);
	ShowWindow(window, 5);
	cc = 0;
	//Sleep(10000);
}


void test() {
	for (int i = 1; i <= 15; i++) {
		find_object(pics[14], pics[i]);
	}
	scanf("%d", &a);
}


void loop() {
	click_once(7);
	Sleep(1000);
	click_once()
}


int main(){
	init();
	loop();
	
	return 0;
	
}