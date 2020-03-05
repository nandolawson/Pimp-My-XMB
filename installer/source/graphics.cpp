#include "graphics.h"

NoRSX *Graphics = new NoRSX();
Background B1(Graphics);
Bitmap BMap(Graphics);
NoRSX_Bitmap Menu_Layer;
Font F1("/dev_flash/data/font/SCE-PS3-RD-B-LATIN2.TTF", Graphics), F2(LATIN2, Graphics);
Image IBackground(Graphics), ILogo(Graphics), IBCross(Graphics), IBCircle(Graphics), ISDown(Graphics), ISUp(Graphics);
pngData png_logo, png_background, png_button_cross, png_button_circle, png_scroll_up, png_scroll_down;
MsgDialog Mess(Graphics);

msgType MSG_OK = (msgType)(MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_OK | MSG_DIALOG_DISABLE_CANCEL_ON); //| MSG_DIALOG_BKG_INVISIBLE
msgType MSG_ERROR = (msgType)(MSG_DIALOG_ERROR | MSG_DIALOG_BTN_TYPE_OK | MSG_DIALOG_DISABLE_CANCEL_ON); //| MSG_DIALOG_BKG_INVISIBLE
msgType MSG_YESNO = (msgType)(MSG_DIALOG_NORMAL | MSG_DIALOG_BTN_TYPE_YESNO | MSG_DIALOG_DEFAULT_CURSOR_NO); //| MSG_DIALOG_BKG_INVISIBLE

int ypos(int y)
{
	return (int)(y*Graphics->height)/720;
}

int xpos(int x)
{
	return (int)(x*Graphics->width)/1280;
}

s32 center_text_x(int fsize, const char* message)
{
	return (Graphics->width-(strlen(message)*fsize/2))/2;
}

u32 menu_text_color(int current, int selected, int choosed, int backups, int menu1_restore)
{
	if (backups==0 && menu1_restore!=0) return COLOR_GREY;
	else if (current==selected)
	{
		if (choosed==1) return COLOR_RED;
		else return COLOR_YELLOW;
	}
	else return COLOR_WHITE;
}

void bitmap_intitalize(string screensize, string folder)
{
	BMap.GenerateBitmap(&Menu_Layer);
    IBackground.LoadPNG((folder+"/images/"+screensize+"/background.png").c_str(), &png_background);
    ILogo.LoadPNG((folder+"/images/"+screensize+"/logo.png").c_str(), &png_logo);
    IBCross.LoadPNG((folder+"/images/"+screensize+"/cross.png").c_str(), &png_button_cross);
    IBCircle.LoadPNG((folder+"/images/"+screensize+"/circle.png").c_str(), &png_button_circle);
    ISUp.LoadPNG((folder+"/images/"+screensize+"/scrollup.png").c_str(), &png_scroll_up);
    ISDown.LoadPNG((folder+"/images/"+screensize+"/scrolldown.png").c_str(), &png_scroll_down);
}

void bitmap_background(string version, string type)
{
	B1.MonoBitmap(COLOR_BLACK,&Menu_Layer);
	IBackground.AlphaDrawIMGtoBitmap(0,0,&png_background,&Menu_Layer);
	u32 imgX =(Graphics->width/2)-(png_logo.width/2), imgY = ypos(30);
	ILogo.AlphaDrawIMGtoBitmap(imgX,imgY,&png_logo,&Menu_Layer);
}

void draw_menu(int choosed)
{
	BMap.DrawBitmap(&Menu_Layer);
	Graphics->Flip();
	if (choosed==1) usleep(25*1000);
	else usleep(5*1000);
}
