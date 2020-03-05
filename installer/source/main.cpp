#include "filesystem.h"
#include "syscalls.h"
#include "graphics.h"
#include <io/pad.h>

#define MAX_OPTIONS 20

//global vars
string menu1[MAX_OPTIONS];
string menu2[MAX_OPTIONS][MAX_OPTIONS];
string menu2_path[MAX_OPTIONS][MAX_OPTIONS];

//headers
int restore(string appfolder);
int install(string appfolder, string firmware_folder, string app_choice);
int make_menu_to_array(string appfolder, int whatmenu, string vers, string type);
void bitmap_menu(int menu_id, int msize, int selected, int choosed, int menu1_pos);

s32 main(s32 argc, char* argv[]);
int string_array_size(string *arr)
{
	int size=0;
	while (strcmp(arr[size].c_str(),"") != 0)
	{
		size++;
	}
	return size;
}

int restore(string appfolder)
{
	string ret="";
	ret=copy_prepare(appfolder, "restore", "", "");
	if (ret == "") //restore success
	{
		if (is_dev_blind_mounted()==0)
		{
			unmount_dev_blind();
			Mess.Dialog(MSG_OK, ("Backup has restored with success."));
			return 1;
		}
		Mess.Dialog(MSG_OK,("Backup has restored with success.\nPress OK to continue."));
		return 1;
	}
	else //problem in the restore process so emit a warning
	{
		Mess.Dialog(MSG_ERROR,("Backup has not restored! An error occured while restoring the backup!\n\nTry to restore again manually, if the error persists, your system may be corrupted, please check all files and if needed reinstall firmare from XMB or recovery menu."));
	}
	return 0;
}

int install(string appfolder, string firmware_folder, string app_choice)
{
	string ret="";
	string problems="\n\nThis process change dev_flash files so DON'T TURN OFF YOUR PS3 while the process in running.\n\nIf you have some corruption after copying the files or the installer quits unexpectly check all files before restarting and if possible reinstall the firmware from XMB or Recovery Menu.";

	Mess.Dialog(MSG_YESNO,("Are you sure you want to install?"+problems).c_str());
	if (Mess.GetResponse(MSG_DIALOG_BTN_YES)==1)
	{
		ret=copy_prepare(appfolder, "backup", firmware_folder, app_choice);
		if (ret == "") //backup success
		{
			ret=copy_prepare(appfolder, "install", firmware_folder, app_choice);
			if (ret == "") //copy success
			{
				if (is_dev_blind_mounted()==0)
				{
					unmount_dev_blind();
					Mess.Dialog(MSG_OK, ("Installed with success.\nPress OK to continue"));
					recursiveDelete(appfolder+"/backup/");
					return 1;
				}
				return 1;
			}
			else //problem in the copy process so rollback by restoring the backup
			{
				Mess.Dialog(MSG_ERROR,("An error occured while copying files!\n\nBackup will be restored."));
				return restore(appfolder);
			}
		}
		else //problem in the backup process so rollback by deleting the backup
		{
			Mess.Dialog(MSG_ERROR,("An error occured while doing backuping the files!\n\nIncomplete backup will be deleted."));
			if (recursiveDelete(appfolder+"/backup/") != "") Mess.Dialog(MSG_ERROR,("Problem while deleting the backup!\n\nError: "+ret+"\n\nTry to delete with a file manager.").c_str());
		}
	}

	return 0;
}

int make_menu_to_array(string appfolder, int whatmenu, string vers, string type)
{
	int ifw=0, iapp=0;
	DIR *dp, *dp2;
	struct dirent *dirp, *dirp2;
	string direct, direct2;

	if (whatmenu==1 || whatmenu==2 || whatmenu==0)
	{
		iapp=0;
		direct=appfolder+"/content";
		dp = opendir (direct.c_str());
		if (dp == NULL) return -1;
		while ( (dirp = readdir(dp) ) )
		{
			if ( strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0 && strcmp(dirp->d_name, "") != 0 && dirp->d_type == DT_DIR)
			{
				//second menu
				ifw=0;
				direct2=direct+"/"+dirp->d_name;
				dp2 = opendir (direct2.c_str());
				if (dp2 == NULL) return -1;
				while ( (dirp2 = readdir(dp2) ) )
				{
					if ( strcmp(dirp2->d_name, ".") != 0 && strcmp(dirp2->d_name, "..") != 0 && strcmp(dirp2->d_name, "") != 0 && dirp2->d_type == DT_DIR)
					{
						string fwfolder=(string)dirp2->d_name;
						string app_fwv=fwfolder.substr(0,fwfolder.find("-"));
						string app_fwt=fwfolder.substr(app_fwv.size()+1,fwfolder.rfind("-")-app_fwv.size()-1);
						string app_fwc=fwfolder.substr(app_fwv.size()+1+app_fwt.size()+1);
						if ((strcmp(app_fwv.c_str(), vers.c_str())==0 || strcmp(app_fwv.c_str(), "All")==0) && (strcmp(app_fwt.c_str(), type.c_str())==0 || strcmp(app_fwt.c_str(), "All")==0))
						{
							menu2[iapp][ifw]=app_fwc;
							menu2_path[iapp][ifw]=dirp2->d_name;
							ifw++;
						}
					}
				}
				closedir(dp2);
				if (ifw>0) //has apps for the current firmware version
				{
					menu2[iapp][ifw]="Back to main menu";
					ifw++;
					menu2[iapp][ifw]="\0";
					menu1[iapp]=dirp->d_name;
					iapp++;
				}
			}
		}
		closedir(dp);

		if (iapp>0)
		{
			menu1[iapp]="Back to XMB";
			iapp++;
			menu1[iapp]="\0";
		}


	}
	return 0;
}


void bitmap_menu(int menu_id, int msize, int selected, int choosed, int menu1_pos)
{
	int j, tposy=png_logo.height+ypos(30)+ypos(30), posy=tposy+ypos(30), sizeFont = ypos(30), spacing=ypos(4);
	int start_at=0, end_at=0, roll_at=0, dynamic_menu_end=0;
	string menu1_text;

	//dynamic menu calculations
	if (menu_id==1) dynamic_menu_end=msize-2;
	else if (menu_id==2) dynamic_menu_end=msize-2;
	if (menu_id==1) roll_at=MENU_ROLL_OPTIONS;
	else roll_at=SUBMENU_ROLL_OPTIONS;

	if (dynamic_menu_end+1>roll_at)
	{
		if (selected<=1) start_at=0; //two first options
		else if (selected>dynamic_menu_end-roll_at+1) start_at=dynamic_menu_end-roll_at+1; //two last options
		else start_at=selected-1;
		end_at=start_at+roll_at-1;
		if (end_at>=dynamic_menu_end) end_at=dynamic_menu_end;
		if (start_at!=0) ISUp.AlphaDrawIMGtoBitmap(xpos(950),tposy+sizeFont+spacing,&png_scroll_up,&Menu_Layer);
		if (end_at!=dynamic_menu_end) ISDown.AlphaDrawIMGtoBitmap(xpos(950),tposy+roll_at*(sizeFont+spacing),&png_scroll_down,&Menu_Layer);
	}
	else end_at=dynamic_menu_end;

	if (menu_id==1)
	{
    //dynamic menu
		for(j=start_at;j<=end_at;j++)
		{
			posy=posy+sizeFont+spacing;
			F2.PrintfToBitmap(center_text_x(sizeFont, menu1[j].c_str()),posy,&Menu_Layer,menu_text_color(j, selected, choosed,-1,-1),sizeFont, "%s",menu1[j].c_str());
		}
		//static menu
		F2.PrintfToBitmap(center_text_x(sizeFont, menu1[msize-1].c_str()),ypos(550),&Menu_Layer,menu_text_color(msize-1, selected, choosed,-1,-1),sizeFont, "%s",menu1[msize-1].c_str());
		//buttons
		if (selected<msize-1)
		{
			IBCross.AlphaDrawIMGtoBitmap(xpos(90),ypos(650),&png_button_cross,&Menu_Layer);
			F2.PrintfToBitmap(xpos(90)+png_button_cross.width+xpos(10),ypos(650)+sizeFont-ypos(5),&Menu_Layer, COLOR_WHITE, sizeFont, "Select");
		}
		else
		{
			IBCross.AlphaDrawIMGtoBitmap(xpos(90),ypos(650),&png_button_cross,&Menu_Layer);
			F2.PrintfToBitmap(xpos(90)+png_button_cross.width+xpos(10),ypos(650)+sizeFont-ypos(5),&Menu_Layer, COLOR_WHITE, sizeFont, "Exit");
		}
	}
	else if (menu_id==2)
	{
		//dynamic menu
		for(j=start_at;j<=end_at;j++)
		{
			posy=posy+sizeFont+spacing;
			F2.PrintfToBitmap(center_text_x(sizeFont, menu2[menu1_pos][j].c_str()),posy,&Menu_Layer,menu_text_color(j, selected, choosed,-1,-1),sizeFont, "%s",menu2[menu1_pos][j].c_str());
		}
		//static menu
		F2.PrintfToBitmap(center_text_x(sizeFont, menu2[menu1_pos][msize-1].c_str()),ypos(550),&Menu_Layer,menu_text_color(msize-1, selected, choosed,-1,-1),sizeFont, "%s",menu2[menu1_pos][msize-1].c_str());
		//buttons
		IBCross.AlphaDrawIMGtoBitmap(xpos(90),ypos(610),&png_button_cross,&Menu_Layer);
		if (selected<msize-1) F2.PrintfToBitmap(xpos(90)+png_button_cross.width+xpos(10),ypos(610)+sizeFont-ypos(5),&Menu_Layer, COLOR_WHITE, sizeFont, "Install");
		else F2.PrintfToBitmap(xpos(90)+png_button_cross.width+xpos(10),ypos(610)+sizeFont-ypos(5),&Menu_Layer, COLOR_WHITE, sizeFont, "Select");
		IBCircle.AlphaDrawIMGtoBitmap(xpos(90),ypos(650),&png_button_circle,&Menu_Layer);
		F2.PrintfToBitmap(xpos(90)+png_button_circle.width+xpos(10),ypos(650)+sizeFont-ypos(5),&Menu_Layer, COLOR_WHITE, sizeFont, "Back");
	}
}

s32 main(s32 argc, char* argv[])
{
	padInfo2 padinfo2;
	padData paddata;
	int menu1_position=0, menu2_position=0, mpos=0, temp=0, current_menu=1, msize=0, choosed=0;
	string fw_version, ttype, mainfolder, rtype="hard";
	int oldmsize=msize, oldcurrentmenu=current_menu, oldmpos=mpos;

	ioPadInit(MAX_PORT_NUM); //this will initialize the controller (7= seven controllers)
	mainfolder=get_app_folder(argv[0]);
	if (check_terms(mainfolder)!=0) goto end;
	fw_version=get_firmware_info("version");
	ttype=get_firmware_info("type");
	if (make_menu_to_array(mainfolder, 0,fw_version, ttype)!=0) { Mess.Dialog(MSG_ERROR,"Problem reading folder!"); goto end; }
	if (string_array_size(menu1)==0) { Mess.Dialog(MSG_ERROR,"Your firmware version is not supported."); goto end; }
	bitmap_intitalize(int_to_string(Graphics->height)+"p",mainfolder);
	bitmap_background(fw_version, ttype);
	bitmap_menu(current_menu, string_array_size(menu1), mpos, 0, menu1_position);
	Graphics->AppStart();
	while (Graphics->GetAppStatus())
	{
		oldmsize=msize;
		oldcurrentmenu=current_menu;
		oldmpos=mpos;
		if (current_menu==1)
		{
			msize=string_array_size(menu1);
			mpos=menu1_position;
		}
		else if (current_menu==2)
		{
			msize=string_array_size(menu2[menu1_position]);
			mpos=menu2_position;
		}

		if (msize!=oldmsize || current_menu!=oldcurrentmenu || mpos!=oldmpos || choosed==1)
		{
			bitmap_background(fw_version, ttype);
			bitmap_menu(current_menu, msize, mpos, 0, menu1_position);
			choosed=0;
		}
		draw_menu(choosed);
		if (ioPadGetInfo2(&padinfo2)==0)
		{
			for(int i=0;i<MAX_PORT_NUM;i++)
			{
				if (padinfo2.port_status[i])
				{
					ioPadGetData(i, &paddata);
					if (current_menu==1)
					{
						if (paddata.BTN_DOWN || paddata.ANA_L_V == 0x00FF || paddata.ANA_R_V == 0x00FF)
						{
							if (menu1_position<msize-1)
							{
								menu1_position++;
							}
							else menu1_position=0;
						}
						else if (paddata.BTN_UP || paddata.ANA_L_V == 0x0000 || paddata.ANA_R_V == 0x0000)
						{
							if (menu1_position>0)
							{
								menu1_position--;
							}
							else menu1_position=msize-1;
						}
						else if (paddata.BTN_CROSS) //Install an app
						{
							choosed=1;
							bitmap_background(fw_version, ttype);
							bitmap_menu(current_menu, msize, mpos, choosed, menu1_position);
							draw_menu(choosed);
							if (menu1_position<msize-1)
							{
								if (menu2[menu1_position][0]=="All" && string_array_size(menu2[menu1_position])==2)
								{
									draw_menu(choosed);
									temp=install(mainfolder,menu2_path[menu1_position][0], menu1[menu1_position]);
									if (temp==2)
									{
										Graphics->AppExit();
									}
									else if (temp==1)
									{
										if (make_menu_to_array(mainfolder, 3,fw_version, ttype)!=0)
										{
											Mess.Dialog(MSG_ERROR,"Problem reading folder!");
											Graphics->AppExit();
										}
									}
								}
								else current_menu=2;
							}
							else if (menu1_position<msize) Graphics->AppExit();
						}
					}
					else if (current_menu==2)
					{
						if (paddata.BTN_CIRCLE) { current_menu=1; }
						else if (paddata.BTN_DOWN || paddata.ANA_L_V == 0x00FF || paddata.ANA_R_V == 0x00FF)
						{
							if (menu2_position<msize-1) { menu2_position++; }
							else menu2_position=0;
						}
						else if (paddata.BTN_UP || paddata.ANA_L_V == 0x0000 || paddata.ANA_R_V == 0x0000)
						{
							if (menu2_position>0) { menu2_position--; }
							else menu2_position=msize-1;
						}
						else if (paddata.BTN_CROSS) //Install an app
						{
							choosed=1;
							bitmap_background(fw_version, ttype);
							bitmap_menu(current_menu, msize, mpos, choosed, menu1_position);
							draw_menu(choosed);
							if (menu2_position<msize-1)
							{
								draw_menu(choosed);
								temp=install(mainfolder, menu2_path[menu1_position][menu2_position], menu1[menu1_position]);
								if (temp==2)
								{
									Graphics->AppExit();
								}
								else if (temp==1)
								{
									if (make_menu_to_array(mainfolder, 3,fw_version, ttype)!=0)
									{
										Mess.Dialog(MSG_ERROR,"Problem reading folder!");
										Graphics->AppExit();
									}
									current_menu=1;
								}
							}
							else current_menu=1;
						}
					}
				}
			}
		}
	}
	goto end;

	end:
	{
		BMap.ClearBitmap(&Menu_Layer);
		ioPadEnd(); //this will uninitialize the controllers
		if (is_dev_blind_mounted()==0) unmount_dev_blind();
	}
	return 0;
}
