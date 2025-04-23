#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <math.h>

void draw_pooh(Display *display, Window window, GC gc) {
    // Цвета
    XColor sky, grass, sun, pooh, black, orange, pot, honey;
    Colormap colormap = DefaultColormap(display, 0);
    
    XAllocNamedColor(display, colormap, "sky blue", &sky, &sky);
    XAllocNamedColor(display, colormap, "forest green", &grass, &grass);
    XAllocNamedColor(display, colormap, "yellow", &sun, &sun);
    XAllocNamedColor(display, colormap, "sienna", &pooh, &pooh);
    XAllocNamedColor(display, colormap, "black", &black, &black);
    XAllocNamedColor(display, colormap, "orange", &orange, &orange);
    XAllocNamedColor(display, colormap, "brown", &pot, &pot);
    XAllocNamedColor(display, colormap, "gold", &honey, &honey);
    
    // Размеры окна
    Window root;
    int x, y;
    unsigned int width, height, border, depth;
    XGetGeometry(display, window, &root, &x, &y, &width, &height, &border, &depth);
    
    // Центральные координаты для Винни-Пуха
    int center_x = width/2;
    int center_y = height/2;
    
    // Фон (небо и трава)
    XSetForeground(display, gc, sky.pixel);
    XFillRectangle(display, window, gc, 0, 0, width, height/2);
    XSetForeground(display, gc, grass.pixel);
    XFillRectangle(display, window, gc, 0, height/2, width, height/2);
    
    // Солнце
    XSetForeground(display, gc, sun.pixel);
    XFillArc(display, window, gc, width-100, 50, 100, 100, 0, 360*64);
    
    // Винни-Пух
    XSetForeground(display, gc, pooh.pixel);
    
    // Тело (овал)
    XFillArc(display, window, gc, center_x-100, center_y-50, 200, 200, 0, 360*64);
    
    // Голова (круг)
    XFillArc(display, window, gc, center_x-75, center_y-150, 150, 150, 0, 360*64);
    
    // Уши
    XFillArc(display, window, gc, center_x-100, center_y-175, 50, 50, 0, 360*64);
    XFillArc(display, window, gc, center_x+50, center_y-175, 50, 50, 0, 360*64);
    
    // Лапы (4 лапы - передние и задние)
    // Передняя левая лапа
    XFillArc(display, window, gc, center_x-130, center_y-20, 60, 80, 0, 360*64);
    // Передняя правая лапа
    XFillArc(display, window, gc, center_x+70, center_y-20, 60, 80, 0, 360*64);
    // Задняя левая лапа
    XFillArc(display, window, gc, center_x-110, center_y+70, 60, 80, 0, 360*64);
    // Задняя правая лапа
    XFillArc(display, window, gc, center_x+50, center_y+70, 60, 80, 0, 360*64);
    
    // Лицо
    XSetForeground(display, gc, black.pixel);
    // Глаза
    XFillArc(display, window, gc, center_x-50, center_y-100, 20, 20, 0, 360*64);
    XFillArc(display, window, gc, center_x+30, center_y-100, 20, 20, 0, 360*64);
    
    // Нос
    XSetForeground(display, gc, orange.pixel);
    XFillArc(display, window, gc, center_x-15, center_y-70, 30, 30, 0, 360*64);
    
    // Рот (улыбка)
    XSetForeground(display, gc, black.pixel);
    XDrawArc(display, window, gc, center_x-30, center_y-60, 60, 40, 210*64, 120*64);
    
    // Горшок с медом
    XSetForeground(display, gc, pot.pixel);
    // Основание горшка
    XFillArc(display, window, gc, center_x+50, center_y+50, 100, 80, 0, 360*64);
    // Верхняя часть горшка
    XFillRectangle(display, window, gc, center_x+50, center_y+50, 100, 10);
    // Ручка горшка
    XDrawArc(display, window, gc, center_x+90, center_y+30, 20, 40, 90*64, 180*64);
    
    // Мед
    XSetForeground(display, gc, honey.pixel);
    XFillArc(display, window, gc, center_x+70, center_y+70, 60, 40, 0, 360*64);
    
    
    // Надпись "МЁД"
    XSetForeground(display, gc, black.pixel);
    XFontStruct* font = XLoadQueryFont(display, "9x15");
    if (font) {
        XSetFont(display, gc, font->fid);
        XDrawString(display, window, gc, center_x+85, center_y+90, "Honey", 3);
        XFreeFont(display, font);
    }
}

int main() {
    Display *display = XOpenDisplay(NULL);


    
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                      10, 10, 600, 600, 1,
                                      BlackPixel(display, screen),
                                      WhitePixel(display, screen));
    
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);
    
    // Установка названия окна
    XStoreName(display, window, "Winy Pooh");
    
    GC gc = XCreateGC(display, window, 0, NULL);
    
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            draw_pooh(display, window, gc);
        }
        else if (event.type == KeyPress) {
            break; // Выход при нажатии любой клавиши
        }
    }
    
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    
    return 0;
}