UTFT_utilities
==============

Utilities for UTFT library. Font file makers, image to code generators, etc...<br />
Head to the [Wiki](https://github.com/ozracing/UTFT_utilities/wiki) to find out how to use them<br />


UTFT_Font_maker:
----------------
NOTE: This version has graphical interface to select fWidth - Font Width, fHeight - Font Height, Divider - Character divider<br />
and check box to define if it is an numeric font, also ability to choose between manual and automatic font generating. Have fun! :)<br />
<br />
NEW: Added ability (Modded lib button) to add font divider byte to font header (4-5 first bytes) use only with modded library which has ability to use the divider!<br />
else it won't work or you will have anomalies with font!<br />
<br />
[UTFT_Font_maker](UTFT_Font_maker/UTFT_Font_maker.jar):<br />
This app generates Font code file. Font character width can be selected! Default is 8<br />
Current features:<br />
<ul><li>Max Font character width is limited to 64 pixels</li>
<li>Ability to choose the divider between 4 and 8 (Default: 8)</li>
<li>Font size auto detection with ability to manualy set font size (Default: Auto)</li>
<li>Numeric type support</li>
<li>Font divider can be added to the header</li>
<li>Saves code file to the same dir as picture is</li>
<li>only .png file type is supported</li></ul>
<br />
<br />
UTFT_Font_maker_Lite:
---------------------
NOTE: you must use the right converter for your library, stock UTFT divider is 8 so you must use Font maker 8,<br />
if you have modified UTFT library divider to lets say 4 you must use Font maker 4 and so on. Have fun! :)<br />
<br />
[UTFT_Font_maker_Lite_8](UTFT_Font_maker_Lite/UTFT_Font_maker_Lite_8.jar):<br />
This app generates Font code file. Font character width must be dividable by 8.<br />
Current features:<br />
<ul><li>Max Font character width is limited to 64 pixels</li>
<li>Font size auto detection</li>
<li>Numeric type support</li>
<li>Saves code file to the same dir as picture is</li>
<li>only .png file type is supported</li></ul>
<br />
<br />
[UTFT_Font_maker_Lite_4](UTFT_Font_maker_Lite/UTFT_Font_maker_Lite_4.jar):<br />
This app generates Font code file. Font character width must be dividable by 4.<br />
Current features:<br />
<ul><li>Max Font character width is limited to 64 pixels</li>
<li>Font size auto detection</li>
<li>Numeric type support</li>
<li>Saves code file to the same dir as picture is</li>
<li>only .png file type is supported</li></ul>
<br />
<br />
If you want to convert larger fonts than 64 pixel character width e-mail me (drugrulezz@gmail.com) or add an enhancement issue and specify what size you require also what divider you use(4,8)<br />
<br />
<br />
UTFT_RGB565_converter_C:
------------------------
NOTE: there are 2 flavours of this app first one is only for avr and the second is only for Due&PicKit<br />
These apps do not have GUI, it works just like font maker lite. Have fun! :)<br />
<br />
[UTFT_RGB565_converter_C_avr](UTFT_RGB565_converter_C/UTFT_RGB565_converter_C_avr.jar):<br />
This app generates RGB565 type code from png pictures for avr micros.<br />
<br />
[UTFT_RGB565_converter_C_due_pickit](UTFT_RGB565_converter_C/UTFT_RGB565_converter_C_due_pickit.jar):<br />
This app generates RGB565 type code from png pictures for Due and PicKit.<br />
<br />
<br />
<center><a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">UTFT Utilities</span> by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Aldas</span> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US">Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License</a></center>