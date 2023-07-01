#include <pspkernel.h>
#include <string.h>

#include "config.h"
#include "blit.h"
#include "local.h"
#include "dvemgr.h"

#define str_e " "
#define str_f "  "

#define XX 160
#define YY 75
#define SZ 8
#define OFS 12

void menu(void);

void plugin_E_D(void);

void speedboost_enable(void);
void speedboost_disable(void);

void switchtoTV_LCD(int mode);

void settings(void);
void adjustscreen(void);
void buttonsassignment(void);
void showbuttons(int key, int y);
