#include "key_data.h"
#include "keys.h"
#include <Windows.h>
#include <iostream>
#include <string>

std::string key_to_string(DWORD key, int up) {
	std::string returnstr;
	if (up == 0) {
		switch (key) {
		case 0:
			returnstr = "    ";
			break;
		case VK_BACK:
			returnstr = "back";
			break;
		case VK_TAB:
			returnstr = "tab ";
			break;
		case VK_RETURN:
			returnstr = " En ";
			break;
		case VK_LSHIFT:
			returnstr = "Lshi";
			break;
		case VK_RSHIFT:
			returnstr = "Rshi";
			break;
		case VK_LCONTROL:
			returnstr = "Lcon";
			break;
		case VK_RCONTROL:
			returnstr = "Rcon";
			break;
		case VK_RMENU:
			returnstr = "Ralt";
			break;
		case VK_LMENU:
			returnstr = "Lalt";
			break;
		case VK_CAPITAL:
			returnstr = "caps";
			break;
		case VK_HANGUL:
			returnstr = "han ";
			break;
		case VK_ESCAPE:
			returnstr = " esc";
			break;
		case VK_SPACE:
			returnstr = " spa";
			break;
		case VK_PRIOR:
			returnstr = "page";
			break;
		case VK_NEXT:
			returnstr = "page";
			break;
		case VK_END:
			returnstr = " end";
			break;
		case VK_HOME:
			returnstr = "home";
			break;
		case VK_LEFT:
			returnstr = " ก็ ";
			break;
		case VK_RIGHT:
			returnstr = " กๆ ";
			break;
		case VK_UP:
			returnstr = " ก่ ";
			break;
		case VK_DOWN:
			returnstr = " ก้ ";
			break;
		case VK_SELECT:
			returnstr = "sele";
			break;
		case VK_PRINT:
			returnstr = " pri";
			break;
		case VK_SNAPSHOT:
			returnstr = " prt";
			break;
		case VK_INSERT:
			returnstr = " ins";
			break;
		case VK_DELETE:
			returnstr = " del";
			break;
		case VK_HELP:
			returnstr = "help";
			break;
		case key_0:
			returnstr = "  0 ";
			break;
		case key_1:
			returnstr = "  1 ";
			break;
		case key_2:
			returnstr = "  2 ";
			break;
		case key_3:
			returnstr = "  3 ";
			break;
		case key_4:
			returnstr = "  4 ";
			break;
		case key_5:
			returnstr = "  5 ";
			break;
		case key_6:
			returnstr = "  6 ";
			break;
		case key_7:
			returnstr = "  7 ";
			break;
		case key_8:
			returnstr = "  8 ";
			break;
		case key_9:
			returnstr = "  9 ";
			break;
		case key_a:
			returnstr = "  a ";
			break;
		case key_b:
			returnstr = "  b ";
			break;
		case key_c:
			returnstr = "  c ";
			break;
		case key_d:
			returnstr = "  d ";
			break;
		case key_e:
			returnstr = "  e ";
			break;
		case key_f:
			returnstr = "  f ";
			break;
		case key_g:
			returnstr = "  g ";
			break;
		case key_h:
			returnstr = "  h ";
			break;
		case key_i:
			returnstr = "  i ";
			break;
		case key_j:
			returnstr = "  j ";
			break;
		case key_k:
			returnstr = "  k ";
			break;
		case key_l:
			returnstr = "  l ";
			break;
		case key_m:
			returnstr = "  m ";
			break;
		case key_n:
			returnstr = "  n ";
			break;
		case key_o:
			returnstr = "  o ";
			break;
		case key_p:
			returnstr = "  p ";
			break;
		case key_q:
			returnstr = "  q ";
			break;
		case key_r:
			returnstr = "  r ";
			break;
		case key_s:
			returnstr = "  s ";
			break;
		case key_t:
			returnstr = "  t ";
			break;
		case key_u:
			returnstr = "  u ";
			break;
		case key_v:
			returnstr = "  v ";
			break;
		case key_w:
			returnstr = "  w ";
			break;
		case key_x:
			returnstr = "  x ";
			break;
		case key_y:
			returnstr = "  y ";
			break;
		case key_z:
			returnstr = "  z ";
			break;
		case VK_LWIN:
			returnstr = "Lwin";
			break;
		case VK_RWIN:
			returnstr = "Rwin";
			break;
		case VK_NUMPAD0:
			returnstr = "Num0";
			break;
		case VK_NUMPAD1:
			returnstr = "Num1";
			break;
		case VK_NUMPAD2:
			returnstr = "Num2";
			break;
		case VK_NUMPAD3:
			returnstr = "Num3";
			break;
		case VK_NUMPAD4:
			returnstr = "Num4";
			break;
		case VK_NUMPAD5:
			returnstr = "Num5";
			break;
		case VK_NUMPAD6:
			returnstr = "Num6";
			break;
		case VK_NUMPAD7:
			returnstr = "Num7";
			break;
		case VK_NUMPAD8:
			returnstr = "Num8";
			break;
		case VK_NUMPAD9:
			returnstr = "Num9";
			break;
		case VK_MULTIPLY:
			returnstr = "Num*";
			break;
		case VK_ADD:
			returnstr = "Num+";
			break;
		case VK_SEPARATOR:
			returnstr = "Num ";
			break;
		case VK_SUBTRACT:
			returnstr = "Num+";
			break;
		case VK_DECIMAL:
			returnstr = "NUM.";
			break;
		case VK_DIVIDE:
			returnstr = "  / ";
			break;
		case VK_F1:
			returnstr = " F1 ";
			break;
		case VK_F2:
			returnstr = " F2 ";
			break;
		case VK_F3:
			returnstr = " F3 ";
			break;
		case VK_F4:
			returnstr = " F4 ";
			break;
		case VK_F5:
			returnstr = " F5 ";
			break;
		case VK_F6:
			returnstr = " F6 ";
			break;
		case VK_F7:
			returnstr = " F7 ";
			break;
		case VK_F8:
			returnstr = " F8 ";
			break;
		case VK_F9:
			returnstr = " F9 ";
			break;
		case VK_F10:
			returnstr = " F10";
			break;
		case VK_F11:
			returnstr = " F11";
			break;
		case VK_F12:
			returnstr = " F12";
			break;
		case VK_NUMLOCK:
			returnstr = " Num";
			break;
		case VK_SCROLL:
			returnstr = " scr";
			break;
		case VK_OEM_1:
			returnstr = "  ; ";
			break;
		case VK_OEM_2:
			returnstr = "  / ";
			break;
		case VK_OEM_3:
			returnstr = "  ` ";
			break;
		case VK_OEM_4:
			returnstr = "  [ ";
			break;
		case VK_OEM_5:
			returnstr = "  \\ ";
			break;
		case VK_OEM_6:
			returnstr = "  ] ";
			break;
		case VK_OEM_7:
			returnstr = "  ' ";
			break;
		case VK_OEM_PLUS:
			returnstr = "  = ";
			break;
		case VK_OEM_COMMA:
			returnstr = "  , ";
			break;
		case VK_OEM_MINUS:
			returnstr = "  - ";
			break;
		case VK_OEM_PERIOD:
			returnstr = "  . ";
			break;
		default:
			returnstr = "err ";
			break;
		}
	}
	else {
		switch (key) {
		case 0:
			returnstr = "    ";
			break;
		case VK_BACK:
			returnstr = "spce";
			break;
		case VK_RETURN:
			returnstr = " ter";
			break;
		case VK_LSHIFT:
			returnstr = " ft ";
			break;
		case VK_RSHIFT:
			returnstr = " ft ";
			break;
		case VK_LCONTROL:
			returnstr = "trol";
			break;
		case VK_RCONTROL:
			returnstr = "trol";
			break;
		case VK_HANGUL:
			returnstr = "gul ";
			break;
		case VK_CAPITAL:
			returnstr = "lock";
			break;
		case VK_SPACE:
			returnstr = " ce ";
			break;
		case VK_PRIOR:
			returnstr = " up ";
			break;
		case VK_NEXT:
			returnstr = "down";
			break;
		case VK_SELECT:
			returnstr = " ct ";
			break;
		case VK_PRINT:
			returnstr = " ni ";
			break;
		case VK_SNAPSHOT:
			returnstr = " sc ";
			break;
		case VK_INSERT:
			returnstr = " ert";
			break;
		case VK_SEPARATOR:
			returnstr = "Entr";
			break;
		case VK_NUMLOCK:
			returnstr = "lock";
			break;
		case VK_SCROLL:
			returnstr = "lock";
			break;
		case VK_OEM_1:
			returnstr = "  : ";
			break;
		case VK_OEM_2:
			returnstr = "  ? ";
			break;
		case VK_OEM_3:
			returnstr = "  ~ ";
			break;
		case VK_OEM_4:
			returnstr = "  { ";
			break;
		case VK_OEM_5:
			returnstr = "  | ";
			break;
		case VK_OEM_6:
			returnstr = "  } ";
			break;
		case VK_OEM_7:
			returnstr = "  \" ";
			break;
		case VK_OEM_PLUS:
			returnstr = "  + ";
			break;
		case VK_OEM_COMMA:
			returnstr = "  < ";
			break;
		case VK_OEM_MINUS:
			returnstr = "  > ";
			break;
		case VK_OEM_PERIOD:
			returnstr = "  > ";
			break;
		default:
			returnstr = "    ";
			break;
		}
	}
	return returnstr;
}