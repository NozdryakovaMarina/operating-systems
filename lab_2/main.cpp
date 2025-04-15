#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("Винни-Пух");
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
        MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName,
        TEXT("Винни-Пух с горшочком меда"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        600,
        NULL,
        NULL,
        hInstance,
        NULL);

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
    HFONT hFont; // Объявляем переменную вне блока case

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        // Устанавливаем режим отображения сглаженных линий
        SetGraphicsMode(hdc, GM_ADVANCED);

        // Рисуем фон (небо и траву)
        hBrush = CreateSolidBrush(RGB(135, 206, 235)); // Цвет неба
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 0, 600, 400);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(34, 139, 34)); // Цвет травы
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 400, 600, 600);
        DeleteObject(hBrush);

        // Рисуем Винни-Пуха (упрощенная версия)

        // Тело (желтый овал)
        hBrush = CreateSolidBrush(RGB(255, 215, 0)); // Золотистый цвет
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 200, 200, 400, 450);
        DeleteObject(hBrush);

        // Голова (желтый круг)
        hBrush = CreateSolidBrush(RGB(255, 215, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 225, 100, 375, 250);
        DeleteObject(hBrush);

        // Уши (два маленьких круга)
        hBrush = CreateSolidBrush(RGB(255, 215, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 235, 80, 275, 120);  // Левое ухо
        Ellipse(hdc, 325, 80, 365, 120);  // Правое ухо
        DeleteObject(hBrush);

        // Глаза (два маленьких черных круга)
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 270, 150, 290, 170);  // Левый глаз
        Ellipse(hdc, 310, 150, 330, 170);  // Правый глаз
        DeleteObject(hBrush);

        // Нос (черный овал)
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 280, 180, 320, 200);
        DeleteObject(hBrush);

        // Рот (дуга)
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, hPen);
        Arc(hdc, 280, 180, 320, 220, 320, 190, 280, 190);
        DeleteObject(hPen);

        // Лапы (4 овала)
        hBrush = CreateSolidBrush(RGB(255, 215, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 180, 300, 220, 350);  // Левая передняя
        Ellipse(hdc, 380, 300, 420, 350);  // Правая передняя
        Ellipse(hdc, 200, 400, 250, 450);  // Левая задняя
        Ellipse(hdc, 350, 400, 400, 450);  // Правая задняя
        DeleteObject(hBrush);

        // Горшок меда (коричневый)
        hBrush = CreateSolidBrush(RGB(139, 69, 19));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, 350, 250, 450, 300);  // Верх горшка
        Rectangle(hdc, 350, 275, 450, 350);  // Основа горшка
        Ellipse(hdc, 350, 325, 450, 375);  // Низ горшка

        // Надпись "МЁД"
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 0));
        hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
        SelectObject(hdc, hFont);
        TextOut(hdc, 370, 290, TEXT("МЁД"), 3);
        DeleteObject(hFont);
        DeleteObject(hBrush);

        // Лапа с горшочком (часть руки держащая горшок)
        hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, 380, 300, NULL);
        LineTo(hdc, 400, 280);
        LineTo(hdc, 420, 290);
        DeleteObject(hPen);

        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}