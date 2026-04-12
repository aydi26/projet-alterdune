#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iostream>

using namespace std;

// namespace de fonctions utilitaires d'affichage TUI
// repose sur cout + ANSI + caracteres Unicode box-drawing
namespace Display {

    // largeur par defaut des cadres (en caracteres visibles)
    const int DEFAULT_WIDTH = 52;

    // initialise la console Windows pour le support UTF-8
    void initConsole();

    // calcule la largeur visible d'une chaine (sans les codes ANSI)
    int visibleLength(const string& s);

    // bordure superieure : ╔══════════╗
    void drawTopBorder(int width = DEFAULT_WIDTH, const string& color = "");

    // bordure inferieure : ╚══════════╝
    void drawBottomBorder(int width = DEFAULT_WIDTH, const string& color = "");

    // separateur epais : ╠══════════╣
    void drawSeparator(int width = DEFAULT_WIDTH, const string& color = "");

    // separateur fin : ╠──────────╣
    void drawThinSeparator(int width = DEFAULT_WIDTH, const string& color = "");

    // ligne de texte centree dans un cadre : ║   texte   ║
    void drawCenteredLine(const string& text, int width = DEFAULT_WIDTH, const string& color = "");

    // ligne de texte alignee a gauche : ║ texte          ║
    void drawLeftLine(const string& text, int width = DEFAULT_WIDTH, const string& color = "");

    // ligne vide dans le cadre : ║                ║
    void drawEmptyLine(int width = DEFAULT_WIDTH, const string& color = "");

    // cadre titre complet : top + titre centre + separateur
    void drawTitleBox(const string& title, int width = DEFAULT_WIDTH, const string& color = "");

    // barre de progression : [████████░░░░] current/max
    string progressBar(int current, int max, int barWidth = 20);

    // barre HP avec couleur auto (vert > jaune > rouge)
    string hpBar(const string& label, int current, int max, int barWidth = 20);

    // barre mercy en jaune
    string mercyBar(const string& label, int current, int max, int barWidth = 20);

    // option de menu avec fleche : ║  > 1. texte     ║
    void drawMenuOption(int num, const string& text, int width = DEFAULT_WIDTH, const string& color = "");
}

#endif
