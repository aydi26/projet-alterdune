#ifndef COLORS_H
#define COLORS_H

#include <string>

using namespace std;

// codes d'échappement ANSI pour la coloration du terminal
// le rendu peut être incomplet sur certains terminaux Windows non compatibles ANSI
const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
const string GRAY    = "\033[90m";
const string BOLD      = "\033[1m";
const string DIM       = "\033[2m";
const string UNDERLINE = "\033[4m";

#endif
