#include<Windows.h>
#include"White.h"

#define CELL_PIXEL			100
#define COLOR_WHITE          RGB(255,255,255)
#define COLOR_BLACK          RGB(0,0,0)
#define COLOR_BOUNDARY		 RGB(139, 134, 130)
#define COLOR_TEXT			 RGB(173,216,230)

#define INIT_TIMER_ELAPSE	1000//��Ϸ��ʼ�ٶ�
#define SPEEDUP_RATIO 0.7//����������ٶȱ���
#define MAX_X		9// ��Ϸ�����С
#define MAX_Y		5
#define NOTE_X      450
#define NOTE_Y      200
#define NOTE_WIDTH  450
#define NOTE_HIGH   300
// ȫ�ֱ���
HINSTANCE hinst; /// HINSTANCE��������ʾ��������ʵ���ľ����ĳЩAPI������ʹ�õ����������
RECT rectBoundary;

// ��������

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//LONG NoteCreate(HWND hwnd);



int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{


	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,///����ͼ��
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);///���IDC_HAND;
				   // GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
				   // ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = "MainMenu";
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))///
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
		"HeiBaiKuai Game!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�
	//
	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);
    
	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwnd);
    //Sleep(5000);


	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);//����MainWndProc
	}
	return msg.wParam;
}
void GamePaint(HWND hwnd)
{

	HPEN hpen;
	HDC hdc,hdcmem, hdcBitmapSrc;
	HBITMAP hbmMem, hBitmap;
	BITMAP bmp;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hbrushWhite;
	HBRUSH hBrushBlack;
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	POINT pBlack;
	POINT lpWhite;
	
	int i, j;
	
	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);		  
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);
	
	SelectObject(hdcmem, hbmMem);


	// ������Ҫ�õ���PEN��BRUSH
	hbrushWhite = CreateSolidBrush(COLOR_WHITE); // RGB��ɫ��ʵ��BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�
	hBrushBlack = CreateSolidBrush(COLOR_BLACK);
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);//�߿�����


												   /*******************************************************************************
												   * #############  ������  ################
												   *
												   *******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	/*******************************************************************************
	* #############  ���׿� ################
	*
	*******************************************************************************/

	// ����ͼ��Ҫ�õ�PEN��BRUSHѡ��DC��
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushWhite);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	for (i = 0; i < COL; i++)
	{
		for (j = 0; j < ROW; j++)
		{
			if (*(GetK() + i*ROW + j) == 1)
			{
				lpWhite = GetPosit(i, j);
				Rectangle(hdcmem,
					lpWhite.x * CELL_PIXEL + rectBoundary.left,
					lpWhite.y * CELL_PIXEL + rectBoundary.top,
					(lpWhite.x + 1)*CELL_PIXEL + rectBoundary.left,
					(lpWhite.y + 1)*CELL_PIXEL + rectBoundary.top);
				
		  }
		}
	}
	
	/*******************************************************************************
	* #############  ���ڿ� ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hBrushBlack);
	for (i = 0; i < COL; i++)
	{
		for (j = 0; j < ROW; j++)
		{
			if (*(GetK() + i*ROW + j) == 0)
			{
				pBlack = GetPosit(i, j);
				Rectangle(hdcmem,
					pBlack.x * CELL_PIXEL + rectBoundary.left,
					pBlack.y * CELL_PIXEL + rectBoundary.top,
					(pBlack.x + 1)*CELL_PIXEL + rectBoundary.left,
					(pBlack.y + 1)*CELL_PIXEL + rectBoundary.top);
				
		  }
		}
	}

	

	/*******************************************************************************
	* #############  ��ǽ  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  дһ����  ################
	*
	*******************************************************************************/

	// ������һ���������
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Sorce %d  Level %d", GetScore(),GetLevel());
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	//����
	hdcBitmapSrc = CreateCompatibleDC(hdc);
	SelectObject(hdcBitmapSrc, getNote1(hwnd));
	GetObject(getNote1(hwnd), sizeof(BITMAP), &bmp);

	StretchBlt(hdcmem,
		NOTE_X, NOTE_Y,
		NOTE_WIDTH, NOTE_HIGH,
		hdcBitmapSrc,
		0, 0, bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);	

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  ���պ��ͷ���Դ  ################
	*
	*******************************************************************************/
	// ������Դ
	// DeleteObject �ͷ�GDI����
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hdcBitmapSrc);
	DeleteObject(hbrushWhite);
	DeleteObject(hBrushBlack);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC ����  ################
	* �ͷ�ռ�õ�DC��ϵͳ��Դ��
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}




void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom + 120, //��������Ϣ������ʾ�ռ䡣
		TRUE);
}
LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
	case WM_CREATE:
		CreateGame(hwnd,
			INIT_TIMER_ELAPSE,
			SPEEDUP_RATIO,
			MAX_X, MAX_Y);
		ReSizeGameWnd(hwnd);


		//Sleep(1000);
		break;

		// ��ϵͳ��Ϊ�����ϵ�GDI����Ӧ�ñ��ػ�ʱ�����򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ��ȻӦ�ó���Ҳ����ͨ������ UpateWindow�������򴰿ڷ���һ��WM_PAINT��Ϣ��
		// ����ʹ��GDI�ڴ����ϻ���ͼ�εĳ��� �����롱 д�����
		// ���������WM_PAINT��Ϣ�Ĵ�������л���GDIͼ�Σ���ô�ڴ���ˢ��ʱ�ͻᱻ�±�Ĩ���͸���
	case WM_PAINT:

		GamePaint(hwnd);
		
		break;

	case WM_KEYDOWN:

		OnKeyDown(wParam,hwnd);
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN:
		//OnTimer(hwnd, wParam);
		OnTimer(hwnd);
		GamePaint(hwnd);
		break;
	
	case WM_TIMER:

		OnTimer(hwnd);
		GamePaint(hwnd);
 		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

