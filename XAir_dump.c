/*
  Xdump.c
  
   This file is part of the XAir-Behringer utilities.

    XAir-Behringer utilities is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XAir-Behringer utilities is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with XAir-Behringer utilities.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright 2014-2018 Patrick-Gilles Maillot
    Copyright 2018 Ted Rippert
 
   Created on: Nov. 5, 2014
       Authors: Patrick-Gilles Maillot and Ted Rippert
 
   Updated Sep. 1 2016 to support /meters/16 type (X32 fw 3.04)
   Modified Feb 17 2018 to provide space separated format for
   	   XAir_Interface and XAir_SnapBackup
 
 */

#include <stdio.h>
#include <string.h>



void Xdump(char *buf, int len, int debug)
{
	int i, k, n, j, l, comma = 0, data = 0, dtc = 0, isnode = 0;
	unsigned char c;
	union littlebig {
		char	c1[4];
		short	si[2];
		int		i1;
		float	f1;
	} endian;

	if (!strncmp(buf, "node", 4)) isnode = 1;

	for (i = 0; i < len; i++) {
		c = (unsigned char)buf[i];
		if (c < 32 || c == 127 || c == 255 ) c = ' '; // Manage unprintable chars
		if (debug) printf(" %c", c);
		else if (!isnode) printf("%c", c);
		if (c == ',') {
			comma = i;
			dtc = 1;
		}
		if (dtc && (buf[i] == 0)) {
			data = (i + 4) & ~3;
			for (dtc = i + 1; dtc < data ; dtc++) {
				if (dtc < len) {
					if (debug) printf(" ~");
					else if (!isnode) printf(" ");
				}
			}
			dtc = 0;
			l = data;
			while (++comma < l && data < len) {
				switch (buf[comma]) {
				case 's':
					if (!strcmp(&(buf+data)[strlen(buf+data)-1], "\n")) (buf+data)[strlen(buf+data)-1] = 0;
					if (isnode) printf("%s", (char*)(buf+data));
					else printf("\'%s\'", (char*)(buf+data));
					k = (strlen((char*)(buf+data)) + 4) & ~3;
					for (j = 0; j < k; j++) {
						if (data < len) {
							c = (unsigned char)buf[data++];
							if (c < 32 || c == 127 || c == 255 ) printf("%c", ' '); // Manage unprintable chars
							if (debug) printf(" %c", c);
//							else printf("%c", c);
						}
					}
					break;
				case 'i':
					for (k = 4; k > 0; endian.c1[--k] = buf[data++]);
					printf(" %6d ", endian.i1);
					break;
				case 'f':
					for (k = 4; k > 0; endian.c1[--k] = buf[data++]);
					if (endian.f1 < 10.) printf(" %06.4f ", endian.f1);
					else if (endian.f1 < 100.) printf(" %06.3f ", endian.f1);
					else if (endian.f1 < 1000.) printf(" %06.2f ", endian.f1);
					else if (endian.f1 < 10000.) printf(" %06.1f ", endian.f1);
					break;
				case 'b':
					// Get the number of bytes
					for (k = 4; k > 0; endian.c1[--k] = buf[data++]);
					n = endian.i1;
					// Get the number of data (floats or ints ???) in little-endian format
					for (k = 0; k < 4; endian.c1[k++] = buf[data++]);
					if (n == endian.i1) {
						// Display blob as string
						printf("%d chrs: ", n);
						for (j = 0; j < n; j++) printf("%c ", buf[data++]);
					} else {
						n = endian.i1;
						// Display blob depending on command
						if(strncmp(buf, "/meters/", 8) == 0) {
							for (j = 0; j < n; j++) {
								// data as short ints, little-endian format
								for (k = 0; k < 2; endian.c1[k++] = buf[data++]);
								endian.f1 = (float)endian.si[0] / 256.0;
								printf("%07.2f ", endian.f1);
							}
						} else {
							printf("%d flts: ", n);
							for (j = 0; j < n; j++) {
								// floats are little-endian format
								for (k = 0; k < 4; endian.c1[k++] = buf[data++]);
								printf("%06.2f ", endian.f1);
							}
						}
					}
					break;
				default:
					break;
				}
			}
			i = data - 1;
		}
	}
	printf("\n");
}
//
//
void Xfdump(char *header, char *buf, int len, int debug) {
int i;

if (debug) {
		printf("%s, %4d B: ", header, len);
		for (i = 0; i < len; i++) {
			printf("%02x", (unsigned char)buf[i]);
		}
		printf("\n");
	}
	if (debug) for (size_t i = 0; i < (strlen(header) + 10); i++) printf(" ");
	else       printf("%s, %4d B: ", header, len);
	Xdump(buf, len, debug);
}

