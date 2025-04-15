#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("WinnieThePooh");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Winnie the Pooh with Honey Pot"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    HPEN hPen;
    HBRUSH hBrush;
    POINT potPoints[4]; // Объявляем массив здесь, вне блока case

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        // Устанавливаем режим отображения сглаженных линий
        SetGraphicsMode(hdc, GM_ADVANCED);

        // Рисуем фон (небо и траву)
        hBrush = CreateSolidBrush(RGB(135, 206, 235)); // Цвет неба
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 0, 600, 300);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(34, 139, 34)); // Цвет травы
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 300, 600, 600);
        DeleteObject(hBrush);

        // Рисуем солнце
        hBrush = CreateSolidBrush(RGB(255, 255, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 450, 50, 550, 150);
        DeleteObject(hBrush);

        // Рисуем Винни-Пуха (тело)
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, hPen);
        hBrush = CreateSolidBrush(RGB(255, 200, 150)); // Цвет меха
        SelectObject(hdc, hBrush);

        // Тело (овал)
        Ellipse(hdc, 200, 200, 400, 400);

        // Голова (круг)
        Ellipse(hdc, 225, 125, 375, 275);

        // Уши
        Ellipse(hdc, 225, 100, 275, 175);
        Ellipse(hdc, 325, 100, 375, 175);

        // Лапы
        Ellipse(hdc, 175, 250, 225, 300); // Левая верхняя
        Ellipse(hdc, 375, 250, 425, 300); // Правая верхняя
        Ellipse(hdc, 225, 375, 275, 425); // Левая нижняя
        Ellipse(hdc, 325, 375, 375, 425); // Правая нижняя

        DeleteObject(hBrush);

        // Рисуем лицо (глаза, нос, рот)
        // Глаза
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 260, 175, 280, 195); // Левый глаз
        Ellipse(hdc, 320, 175, 340, 195); // Правый глаз

        // Нос
        hBrush = CreateSolidBrush(RGB(255, 165, 0)); // Оранжевый
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 290, 195, 310, 215);
        DeleteObject(hBrush);

        // Рот (улыбка)
        Arc(hdc, 280, 210, 320, 250, 280, 220, 320, 220);

        // Рисуем горшок с медом
        hPen = CreatePen(PS_SOLID, 2, RGB(139, 69, 19)); // Коричневый
        SelectObject(hdc, hPen);
        hBrush = CreateSolidBrush(RGB(210, 105, 30)); // Шоколадный
        SelectObject(hdc, hBrush);

        // Основание горшка
        Ellipse(hdc, 350, 350, 450, 380);

        // Верхняя часть горшка
        potPoints[0] = { 360, 330 };
        potPoints[1] = { 360, 350 };
        potPoints[2] = { 440, 350 };
        potPoints[3] = { 440, 330 };
        Polygon(hdc, potPoints, 4);

        // Мед (желтая часть)
        hBrush = CreateSolidBrush(RGB(255, 215, 0)); // Золотой
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 370, 340, 430, 370);

        // Надпись
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 380, 355, TEXT("HUNNY"), 5);

        DeleteObject(hBrush);
        DeleteObject(hPen);

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}