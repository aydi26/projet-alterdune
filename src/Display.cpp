#include "../include/Display.h"
#include "../include/Colors.h"
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void Display::initConsole() {
#ifdef _WIN32
    // active le mode UTF-8 pour les caracteres box-drawing Unicode
    SetConsoleOutputCP(65001);
    // active le traitement des sequences ANSI sur Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if(GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
}

int Display::visibleLength(const string& s) {
    int len = 0;
    bool inEscape = false;
    for(int i = 0; i < (int)s.size(); i++) {
        unsigned char c = s[i];
        if(c == '\033') {
            inEscape = true;
            continue;
        }
        if(inEscape) {
            if(c == 'm') inEscape = false;
            continue;
        }
        // sauter les octets de continuation UTF-8
        if((c & 0xC0) == 0x80) continue;
        len++;
    }
    return len;
}

// repete un caractere (string) n fois
static string repeatStr(const string& s, int n) {
    string result;
    for(int i = 0; i < n; i++) result += s;
    return result;
}

void Display::drawTopBorder(int width, const string& color) {
    // ╔ + (width-2) fois ═ + ╗
    cout << color << "\xe2\x95\x94" << repeatStr("\xe2\x95\x90", width - 2) << "\xe2\x95\x97" << RESET << endl;
}

void Display::drawBottomBorder(int width, const string& color) {
    // ╚ + (width-2) fois ═ + ╝
    cout << color << "\xe2\x95\x9a" << repeatStr("\xe2\x95\x90", width - 2) << "\xe2\x95\x9d" << RESET << endl;
}

void Display::drawSeparator(int width, const string& color) {
    // ╠ + (width-2) fois ═ + ╣
    cout << color << "\xe2\x95\xa0" << repeatStr("\xe2\x95\x90", width - 2) << "\xe2\x95\xa3" << RESET << endl;
}

void Display::drawThinSeparator(int width, const string& color) {
    // ╠ + (width-2) fois ─ + ╣
    cout << color << "\xe2\x95\xa0" << repeatStr("\xe2\x94\x80", width - 2) << "\xe2\x95\xa3" << RESET << endl;
}

void Display::drawCenteredLine(const string& text, int width, const string& color) {
    int textLen = visibleLength(text);
    int innerWidth = width - 2;     // espace entre les deux ║
    int totalPad = innerWidth - textLen;
    if(totalPad < 0) totalPad = 0;
    int leftPad = totalPad / 2;
    int rightPad = totalPad - leftPad;

    cout << color << "\xe2\x95\x91" << RESET;
    cout << string(leftPad, ' ') << text << string(rightPad, ' ');
    cout << color << "\xe2\x95\x91" << RESET << endl;
}

void Display::drawLeftLine(const string& text, int width, const string& color) {
    int textLen = visibleLength(text);
    int innerWidth = width - 2;
    int pad = innerWidth - 2 - textLen;     // 2 espaces de marge gauche
    if(pad < 0) pad = 0;

    cout << color << "\xe2\x95\x91" << RESET;
    cout << "  " << text << string(pad, ' ');
    cout << color << "\xe2\x95\x91" << RESET << endl;
}

void Display::drawEmptyLine(int width, const string& color) {
    int innerWidth = width - 2;
    cout << color << "\xe2\x95\x91" << RESET;
    cout << string(innerWidth, ' ');
    cout << color << "\xe2\x95\x91" << RESET << endl;
}

void Display::drawTitleBox(const string& title, int width, const string& color) {
    drawTopBorder(width, color);
    drawCenteredLine(color + title + RESET, width, color);
    drawSeparator(width, color);
}

string Display::progressBar(int current, int max, int barWidth) {
    if(max <= 0) max = 1;
    if(current < 0) current = 0;
    if(current > max) current = max;

    int filled = (current * barWidth) / max;
    int empty = barWidth - filled;

    string bar = "[";
    // \xe2\x96\x88 = █ (bloc plein)
    for(int i = 0; i < filled; i++) bar += "\xe2\x96\x88";
    // \xe2\x96\x91 = ░ (bloc vide)
    for(int i = 0; i < empty; i++) bar += "\xe2\x96\x91";
    bar += "]";
    return bar;
}

string Display::hpBar(const string& label, int current, int max, int barWidth) {
    // couleur auto selon le pourcentage de HP
    string color;
    if(max > 0) {
        int pct = (current * 100) / max;
        if(pct > 50) color = GREEN;
        else if(pct > 25) color = YELLOW;
        else color = RED;
    } else {
        color = RED;
    }

    string bar = progressBar(current, max, barWidth);
    ostringstream oss;
    oss << label << " " << color << bar << RESET << " " << current << "/" << max;
    return oss.str();
}

string Display::mercyBar(const string& label, int current, int max, int barWidth) {
    string bar = progressBar(current, max, barWidth);
    ostringstream oss;
    oss << label << " " << YELLOW << bar << RESET << " " << current << "/" << max;
    return oss.str();
}

void Display::drawMenuOption(int num, const string& text, int width, const string& color) {
    ostringstream oss;
    oss << color << "> " << num << ". " << text << RESET;
    drawLeftLine(oss.str(), width, color);
}
