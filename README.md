Pimp My XMB - An easy-to-use XMB Mod Manager for Sony PlayStation 3
==
## Features
* Quick and easy way change to the look of XMB
  * Full custom themes
  * Custom waves, coldboots and gameboots
  * All original assets
  * Automatic backup function
  * File checks while mod installing to achieve a very low brick risk

More features will be added in the future.
## Requirements
A PS3 console with 4.85 Custom Firmware installed is required to use this software. **HEN is not supported.**
## Usage
Install the package file on your PS3 via Package Manager. Launch _Pimp My XMB_ from the Game Category after that.
## Used themes, tools and other resources
### Themes and other resources
* [PS3 Pro](https://www.psx-place.com/resources/ps3-4k-pro.595/) by LuanTeles
* Rebug Original by Team Rebug
* Many other ones where I am not able to find the creator. If anyone knows who the creator of one of them is, please open an issue here on GitHub so I am able to mention the person.
### Tools
* [xmb-manager-plus-installer](https://github.com/XMB-Manager-Plus/xmb-manager-plus-installer) by XMB Manager Plus
## Important information
This software has several brick protections and is heavily tested, but there is always the risk of a brick. Nobody but you is responsible for anything that happens to your console.
## Build Pimp My XMB
Pimp My XMB can be build on pretty much any Linux distro with [ps3toolchain](github.com/ps3dev/ps3toolchain) and [NoRSX Version 0.29](github.com/wargio/NoRSX/tree/a0d1d0c6c47561df70813fcf86df610d982ffbfc) installed including Cygwin and WSL.

Just run [make.sh](make.sh) and the whole package will be build and saved in the project folder. Even cleaning up the enviroment is not needed as this is done automatically.
## Adding themes and other resources
Adding resources is pretty easy, but it is still recommended to read this section.

The needed file structure of resources is unusual. To see how you need to save your work to use it with this software, look into [this folder](content/theme_base).
### Custom themes
Custom themes are located in [content/custom/themes](content/custom/themes).
* Only modified files should be placed in here as every missing one gets automatically added from the original ones while building.
* If a custom theme should be used as the base for another one, simply put a file called **import** into the theme folder that only containts the base custom themes name. If files are still missing from both, the base and the other theme, they'll still be added from the original ones.
### Custom waves
Custom waves are located in [content/custom/waves](content/custom/waves).
* To properly show your wave in the installer, just give your **lines.qrc** file another name like _Red to Green.qrc_.
* Waves from custom themes will also be in this section. There is no need to add them to this folder again.
### Custom coldboots
Custom waves are located in [content/custom/coldboots](content/custom/coldboots).
* To properly show your coldboot in the installer, just give your **coldboot.raf** file another name like _Rebug Alternative.raf_.
* Coldboots from custom themes will also be in this section. There is no need to add them to this folder again.
### Gameboots
Custom Gameboots are located in [content/custom/gameboots](content/custom/gameboots).
* Gameboots need to be in a folder. Following files are needed to add them to the installer:
  * custom_render_plugin.rco
  * game_ext_plugin.sprx
  * gameboot_multi.ac3
  * gameboot_stereo.ac3
* Gameboots from custom themes will also be in this section. There is no need to add them to this folder again.
## Licensing
This software is licensed under the terms of the GNU General Public License v3.0. You can find a copy of the license [here](LICENSE). All included parts of this project that are not made specifically for it could be differently licensed, which belongs to everything listed under [Used themes, tools and other resources](#used-themes-tools-and-other-resources).
