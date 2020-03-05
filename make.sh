#!/bin/bash

# Preparing

## Create needed folders if they do not exist
[ ! -d "content/custom" ] && mkdir content/custom
### coldboots
[ ! -d "content/custom/coldboots" ] && mkdir content/custom/coldboots
### gameboots
[ ! -d "content/custom/gameboots" ] && mkdir content/custom/gameboots
### themes
[ ! -d "content/custom/themes" ] && mkdir content/custom/themes
### waves
[ ! -d "content/custom/waves" ] && mkdir content/custom/waves

## Important variables
Title="Pimp My XMB" ## Has to be 8 digits long (excluding spaces) and contain letters and numbers only.
Firmware_Version="4.85"
Custom_Firmware="All"

## Generate variable
### Title ID from Title
Title_ID=$(printf "${Title// }")
Title_ID=$(printf "${Title_ID^^}")

## Set variable
### Working directory
temp_dir="installer/release"

## Delete if exists
### Finished package file
[ -f "$Title.pkg" ] && rm "$Title.pkg"
### Temporary directory
[ -d "$temp_dir" ] && rm -rf "$temp_dir"
### Clean the whole installer environment
cd installer && make clean && cd ..

# Basic package files
## Create folder structure
mkdir "$temp_dir" "$temp_dir/USRDIR" "$temp_dir/USRDIR/content"
## Copy needed files
cp content/package_files/{ICON0.PNG,PARAM.SFO,PS3LOGO.DAT,PIC1.PNG} "$temp_dir"
## Copy images for the installer
cp -rf "content/package_files/images" "$temp_dir/USRDIR"


# Generate themes

## Set themes directory
themes_dir="$temp_dir/USRDIR/content/Themes"

## Create themes directory
mkdir "$themes_dir"

## Copy original theme
cp -rf "content/theme_base" "$themes_dir"
### Rename theme_base
mv "$themes_dir/theme_base" "$themes_dir/$Firmware_Version-$Custom_Firmware-Original"

## Generate custom themes
cd "content/custom/themes"
for D in *; do
    if [ -d "${D}" ]; then
      ### Copy original theme
      cp -rf "../../theme_base" "../../../$themes_dir"
      mv "../../../$themes_dir/theme_base" "../../../$themes_dir/${D}"
      ### Import content from another theme if wanted
      if test -f "${D}/import"; then
        mkdir ../../../$temp_dir/USRDIR/content/TEMP
        import=$(<"${D}/import")
        cp -rf "$import" "../../../$temp_dir/USRDIR/content/TEMP"
        mv "../../../$temp_dir/USRDIR/content/TEMP/$import" "../../../$temp_dir/USRDIR/content/TEMP/${D}"
        rsync -a -I ../../../$temp_dir/USRDIR/content/TEMP/${D} "../../../$themes_dir/"
      fi
      ### Copy custom theme and overwrite unwanted original theme files
      cp -rf "../../custom/themes/${D}" "../../../$themes_dir"
      mv "../../../$themes_dir/${D}" "../../../$themes_dir/$Firmware_Version-$Custom_Firmware-${D}"
      [ -f "../../../$themes_dir/$Firmware_Version-$Custom_Firmware-${D}/import" ] && rm ../../../$themes_dir/$Firmware_Version-$Custom_Firmware-${D}/import
    fi
done
## Delete TEMP folder if exists
[ -d "../../../$temp_dir/USRDIR/content/TEMP" ] && rm -rf "../../../$temp_dir/USRDIR/content/TEMP"
cd ../../..


# Copy waves

## Set waves directory
waves_dir="$temp_dir/USRDIR/content/Waves"

## Create waves directory
mkdir "$waves_dir"

## Copy original wave
mkdir "$waves_dir/$Firmware_Version-$Custom_Firmware-Original" "$waves_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource~qgl" "$waves_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource"
cp "content/theme_base/PS3~dev_blind~vsh~resource~qgl/lines.qrc" "$waves_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource~qgl"

## Copy waves from themes
cd "content/custom/themes"
for D in *; do
    if [ -d "${D}" ]; then
      [ -f "${D}/PS3~dev_blind~vsh~resource~qgl/lines.qrc" ] && mkdir "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-${D}" "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource~qgl"
      [ -f "${D}/PS3~dev_blind~vsh~resource~qgl/lines.qrc" ] && cp "${D}/PS3~dev_blind~vsh~resource~qgl/lines.qrc" "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource~qgl"
    fi
done
cd ../../..

## Copy waves from "waves"
cd content/custom/waves
for f in *; do
  wave_name=$(basename "$f" .qrc)
  mkdir "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-$wave_name" "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-$wave_name/PS3~dev_blind~vsh~resource~qgl"
  cp "$f" "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-$wave_name/PS3~dev_blind~vsh~resource~qgl"
  mv "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-$wave_name/PS3~dev_blind~vsh~resource~qgl/$f" "../../../$waves_dir/$Firmware_Version-$Custom_Firmware-$wave_name/PS3~dev_blind~vsh~resource~qgl/lines.qrc"
done
cd ../../..


# Copy coldboots

## Set coldlboots directory
coldboots_dir="$temp_dir/USRDIR/content/Coldboots"

## Create coldboots directory
mkdir "$coldboots_dir"

## Copy original coldboot
mkdir "$coldboots_dir/$Firmware_Version-$Custom_Firmware-Original" "$coldboots_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource"
cp "content/theme_base/PS3~dev_blind~vsh~resource/coldboot.raf" "$coldboots_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource"
cp "content/theme_base/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" "$coldboots_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource"
cp "content/theme_base/PS3~dev_blind~vsh~resource/coldboot_stereo.ac3" "$coldboots_dir/$Firmware_Version-$Custom_Firmware-Original/PS3~dev_blind~vsh~resource"


## Copy coldboots from themes
cd content/custom/themes
for D in *; do
  if [ -d "${D}" ]; then
    [ -f "${D}/PS3~dev_blind~vsh~resource/coldboot.raf" ] && mkdir "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    [ -f "${D}/PS3~dev_blind~vsh~resource/coldboot.raf" ] && cp "${D}/PS3~dev_blind~vsh~resource/coldboot.raf" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    [ ! -f "${D}/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" ] && cp "../../theme_base/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    [ ! -f "${D}/PS3~dev_blind~vsh~resource~qgl/coldboot_stereo.ac3" ] && cp "../../theme_base/PS3~dev_blind~vsh~resource/coldboot_stereo.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    [ -f "${D}/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" ] && cp "${D}/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    [ -f "${D}/PS3~dev_blind~vsh~resource/coldboot_stereo.ac3" ] && cp "${D}/PS3~dev_blind~vsh~resource/coldboot_stereo.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
  fi
done
cd ../../..

## Copy coldboots from "coldboots"
cd "content/custom/coldboots"
for f in *; do
  coldboot_name=$(basename "$f" .raf)
  mkdir "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource"
  cp "$f" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource"
  mv "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource/$f" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource/coldboot.raf"
  cp "../../theme_base/PS3~dev_blind~vsh~resource/coldboot_multi.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource"
  cp "../../theme_base/PS3~dev_blind~vsh~resource/coldboot_stereo.ac3" "../../../$coldboots_dir/$Firmware_Version-$Custom_Firmware-$coldboot_name/PS3~dev_blind~vsh~resource"
done
cd ../../..


# Copy gameboots

## Set gameboots directory
gameboots_dir="$temp_dir/USRDIR/content/Gameboots"

## Create gameboots directory
mkdir "$gameboots_dir"

## Copy no gameboot
mkdir "$gameboots_dir/$Firmware_Version-$Custom_Firmware-None" "$gameboots_dir/$Firmware_Version-$Custom_Firmware-None/PS3~dev_blind~vsh~module" "$gameboots_dir/$Firmware_Version-$Custom_Firmware-None/PS3~dev_blind~vsh~resource"
cp "content/theme_base/PS3~dev_blind~vsh~module/game_ext_plugin.sprx" "$gameboots_dir/$Firmware_Version-$Custom_Firmware-None/PS3~dev_blind~vsh~module"
cp "content/theme_base/PS3~dev_blind~vsh~resource/custom_render_plugin.rco" "$gameboots_dir/$Firmware_Version-$Custom_Firmware-None/PS3~dev_blind~vsh~resource"

## Copy gameboots from themes
cd "content/custom/themes"
for D in *; do
  if [ -d "${D}" ]; then
    if [[ -f "${D}/PS3~dev_blind~vsh~module/game_ext_plugin.sprx" && "${D}/PS3~dev_blind~vsh~resource/custom_render_plugin.rco" && "${D}/PS3~dev_blind~vsh~resource/gameboot_stereo.ac3" && "${D}/PS3~dev_blind~vsh~resource/gameboot_multi.ac3" ]]; then
      mkdir "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~module" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
      cp "${D}/PS3~dev_blind~vsh~module/game_ext_plugin.sprx" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~module"
      cp "${D}/PS3~dev_blind~vsh~resource/custom_render_plugin.rco" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
      cp "${D}/PS3~dev_blind~vsh~resource/gameboot_multi.ac3" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
      cp "${D}/PS3~dev_blind~vsh~resource/gameboot_stereo.ac3" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    fi
  fi
done
cd ../../..

## Copy gameboots from "gameboots"
cd "content/custom/gameboots"
for D in *; do
  if [ -d "${D}" ]; then
    if [[ -f "${D}/custom_render_plugin.rco" && "${D}/game_ext_plugin.sprx" && "${D}/gameboot_multi.ac3" && "${D}/gameboot_stereo.ac3" ]]; then
        mkdir "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~module" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
        cp "${D}/game_ext_plugin.sprx" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~module"
        cp "${D}/custom_render_plugin.rco" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
        cp "${D}/gameboot_multi.ac3" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
        cp "${D}/gameboot_stereo.ac3" "../../../$gameboots_dir/$Firmware_Version-$Custom_Firmware-${D}/PS3~dev_blind~vsh~resource"
    fi
  fi
done
cd "../../.."


# Build package
cd installer
make pkg
mv "installer.pkg" "../$(printf "$Title"".pkg")"

## Clean the environment
make clean
rm -rf "release"
