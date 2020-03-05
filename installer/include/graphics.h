#include <NoRSX.h>
#include <string>

#define MENU_ROLL_OPTIONS 5
#define SUBMENU_ROLL_OPTIONS 7

using namespace std;

extern NoRSX *Graphics;
extern Bitmap BMap;
extern NoRSX_Bitmap Menu_Layer;
extern Background B1;
extern Font F1, F2;
extern Image IBackground, ILogo, IBCross, IBCircle, ISDown, ISUp;
extern pngData png_logo, png_background, png_button_cross, png_button_circle, png_scroll_up, png_scroll_down;
extern MsgDialog Mess;

extern msgType MSG_OK, MSG_ERROR, MSG_YESNO;

int ypos(int y);
int xpos(int x);
s32 center_text_x(int fsize, const char* message);
u32 menu_text_color(int current, int selected, int choosed, int backups, int menu1_restore);
void bitmap_intitalize(string screensize, string folder);
void bitmap_background(string version, string type);
void draw_menu(int choosed);
