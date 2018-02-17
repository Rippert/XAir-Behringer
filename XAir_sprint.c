/* 
  XR_sprint.c
 
     This file is part of the XAir-Behringer utilities.

    XAir-Behringer utilities is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2018 Patrick-Gilles Maillot
  
 
   Created on: Oct 1, 2014
       Author: Patrick-Gilles Maillot
 
       copies the content of data to XR buffer, respecting big-endian
 */


#include <string.h>



int
Xsprint(char *bd, int index, char format, void *bs)
{
	int i;
// check format
	switch (format) {
	case 's':
	// string : copy characters one at a time until a 0 is found
		if (bs) {
			strcpy (bd+index, bs);
			index += (int)strlen(bs) + 1;
		} else {
			bd[index++] = 0;
		}
	// align to 4 bytes boundary if needed
		while (index & 3) bd[index++] = 0;
		break;
	case 'f':
	case 'i':
	// float or int : copy the 4 bytes of float or int in big-endian order
		i = 4;
		while (i > 0)
			bd[index++] = (char)(((char*)bs)[--i]);
		break;
	default:
	// don't copy anything
		break;
	}
	return index;
}


int
Xfprint(char *bd, int index, char* text, char format, void *bs)
{
// first copy text
	strcpy (bd+index, text);
	index += (int)strlen(text) + 1;
// align to 4 bytes boundary if needed
	while (index & 3) bd[index++] = 0;
// then set format, keeping #4 alignment
	bd[index++] = ',';
	bd[index++] = format;
	bd[index++] = 0;
	bd[index++] = 0;
// based on format, set value
	return Xsprint(bd, index, format, bs);
}
