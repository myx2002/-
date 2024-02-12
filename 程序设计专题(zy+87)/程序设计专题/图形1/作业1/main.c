#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
void Main()
{
	InitGraphics();
	MovePen(2,0);
	DrawLine(0,4);
	DrawLine(4,0);
	DrawLine(0,-4);
	MovePen(4.5,0);
	DrawLine(0,3);
	DrawLine(1,0);
	DrawLine(0,-3);
	MovePen(2.5,2);
	DrawLine(0,1);
	DrawLine(1,0);
	DrawLine(0,-1);
	DrawLine(-1,0);
	MovePen(2,4);
	DrawLine(2,2);
	DrawLine(2,-2);
	MovePen(2.5,4.5);
	DrawLine(0,1.25);
	DrawLine(1,0);
	DrawLine(0,-0.25);
	MovePen(2.5,5.75);
	DrawLine(-0.2,0);
	DrawLine(0,0.25);
	DrawLine(1.4,0);
	DrawLine(0,-0.25);
	DrawLine(-0.2,0);
}
