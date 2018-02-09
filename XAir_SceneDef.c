/*
 * XAir_SceneDef.c
 *
 *  Created on: Jan 27, 2018
 *
 *  Definition of XAir path commands to generate a full backup. scnlen and xdump functions.
 *
 *      Author: Ted Rippert
 */

#include <string.h>
#include <stdio.h>

char *scncmds[] = { "/config/chlink", "/config/buslink", "/config/linkcfg", "/config/solo", "/config/amixenable", "/config/amixlock", "/config/mute", "/ch/01/config", "/ch/01/preamp", "/ch/01/gate", "/ch/01/gate/filter", "/ch/01/dyn", "/ch/01/dyn/filter", "/ch/01/insert", "/ch/01/eq", "/ch/01/eq/1", "/ch/01/eq/2", "/ch/01/eq/3", "/ch/01/eq/4", "/ch/01/mix", "/ch/01/mix/01", "/ch/01/mix/02", "/ch/01/mix/03", "/ch/01/mix/04", "/ch/01/mix/05", "/ch/01/mix/06", "/ch/01/mix/07", "/ch/01/mix/08", "/ch/01/mix/09", "/ch/01/mix/10", "/ch/01/grp", "/ch/01/automix", "/ch/02/config", "/ch/02/preamp", "/ch/02/gate", "/ch/02/gate/filter", "/ch/02/dyn", "/ch/02/dyn/filter", "/ch/02/insert", "/ch/02/eq", "/ch/02/eq/1", "/ch/02/eq/2", "/ch/02/eq/3", "/ch/02/eq/4", "/ch/02/mix", "/ch/02/mix/01", "/ch/02/mix/02", "/ch/02/mix/03", "/ch/02/mix/04", "/ch/02/mix/05", "/ch/02/mix/06", "/ch/02/mix/07", "/ch/02/mix/08", "/ch/02/mix/09", "/ch/02/mix/10", "/ch/02/grp", "/ch/02/automix", "/ch/03/config", "/ch/03/preamp", "/ch/03/gate", "/ch/03/gate/filter", "/ch/03/dyn", "/ch/03/dyn/filter", "/ch/03/insert", "/ch/03/eq", "/ch/03/eq/1", "/ch/03/eq/2", "/ch/03/eq/3", "/ch/03/eq/4", "/ch/03/mix", "/ch/03/mix/01", "/ch/03/mix/02", "/ch/03/mix/03", "/ch/03/mix/04", "/ch/03/mix/05", "/ch/03/mix/06", "/ch/03/mix/07", "/ch/03/mix/08", "/ch/03/mix/09", "/ch/03/mix/10", "/ch/03/grp", "/ch/03/automix", "/ch/04/config", "/ch/04/preamp", "/ch/04/gate", "/ch/04/gate/filter", "/ch/04/dyn", "/ch/04/dyn/filter", "/ch/04/insert", "/ch/04/eq", "/ch/04/eq/1", "/ch/04/eq/2", "/ch/04/eq/3", "/ch/04/eq/4", "/ch/04/mix", "/ch/04/mix/01", "/ch/04/mix/02", "/ch/04/mix/03", "/ch/04/mix/04", "/ch/04/mix/05", "/ch/04/mix/06", "/ch/04/mix/07", "/ch/04/mix/08", "/ch/04/mix/09", "/ch/04/mix/10", "/ch/04/grp", "/ch/04/automix", "/ch/05/config", "/ch/05/preamp", "/ch/05/gate", "/ch/05/gate/filter", "/ch/05/dyn", "/ch/05/dyn/filter", "/ch/05/insert", "/ch/05/eq", "/ch/05/eq/1", "/ch/05/eq/2", "/ch/05/eq/3", "/ch/05/eq/4", "/ch/05/mix", "/ch/05/mix/01", "/ch/05/mix/02", "/ch/05/mix/03", "/ch/05/mix/04", "/ch/05/mix/05", "/ch/05/mix/06", "/ch/05/mix/07", "/ch/05/mix/08", "/ch/05/mix/09", "/ch/05/mix/10", "/ch/05/grp", "/ch/05/automix", "/ch/06/config", "/ch/06/preamp", "/ch/06/gate", "/ch/06/gate/filter", "/ch/06/dyn", "/ch/06/dyn/filter", "/ch/06/insert", "/ch/06/eq", "/ch/06/eq/1", "/ch/06/eq/2", "/ch/06/eq/3", "/ch/06/eq/4", "/ch/06/mix", "/ch/06/mix/01", "/ch/06/mix/02", "/ch/06/mix/03", "/ch/06/mix/04", "/ch/06/mix/05", "/ch/06/mix/06", "/ch/06/mix/07", "/ch/06/mix/08", "/ch/06/mix/09", "/ch/06/mix/10", "/ch/06/grp", "/ch/06/automix", "/ch/07/config", "/ch/07/preamp", "/ch/07/gate", "/ch/07/gate/filter", "/ch/07/dyn", "/ch/07/dyn/filter", "/ch/07/insert", "/ch/07/eq", "/ch/07/eq/1", "/ch/07/eq/2", "/ch/07/eq/3", "/ch/07/eq/4", "/ch/07/mix", "/ch/07/mix/01", "/ch/07/mix/02", "/ch/07/mix/03", "/ch/07/mix/04", "/ch/07/mix/05", "/ch/07/mix/06", "/ch/07/mix/07", "/ch/07/mix/08", "/ch/07/mix/09", "/ch/07/mix/10", "/ch/07/grp", "/ch/07/automix", "/ch/08/config", "/ch/08/preamp", "/ch/08/gate", "/ch/08/gate/filter", "/ch/08/dyn", "/ch/08/dyn/filter", "/ch/08/insert", "/ch/08/eq", "/ch/08/eq/1", "/ch/08/eq/2", "/ch/08/eq/3", "/ch/08/eq/4", "/ch/08/mix", "/ch/08/mix/01", "/ch/08/mix/02", "/ch/08/mix/03", "/ch/08/mix/04", "/ch/08/mix/05", "/ch/08/mix/06", "/ch/08/mix/07", "/ch/08/mix/08", "/ch/08/mix/09", "/ch/08/mix/10", "/ch/08/grp", "/ch/08/automix", "/ch/09/config", "/ch/09/preamp", "/ch/09/gate", "/ch/09/gate/filter", "/ch/09/dyn", "/ch/09/dyn/filter", "/ch/09/insert", "/ch/09/eq", "/ch/09/eq/1", "/ch/09/eq/2", "/ch/09/eq/3", "/ch/09/eq/4", "/ch/09/mix", "/ch/09/mix/01", "/ch/09/mix/02", "/ch/09/mix/03", "/ch/09/mix/04", "/ch/09/mix/05", "/ch/09/mix/06", "/ch/09/mix/07", "/ch/09/mix/08", "/ch/09/mix/09", "/ch/09/mix/10", "/ch/09/grp", "/ch/09/automix", "/ch/10/config", "/ch/10/preamp", "/ch/10/gate", "/ch/10/gate/filter", "/ch/10/dyn", "/ch/10/dyn/filter", "/ch/10/insert", "/ch/10/eq", "/ch/10/eq/1", "/ch/10/eq/2", "/ch/10/eq/3", "/ch/10/eq/4", "/ch/10/mix", "/ch/10/mix/01", "/ch/10/mix/02", "/ch/10/mix/03", "/ch/10/mix/04", "/ch/10/mix/05", "/ch/10/mix/06", "/ch/10/mix/07", "/ch/10/mix/08", "/ch/10/mix/09", "/ch/10/mix/10", "/ch/10/grp", "/ch/10/automix", "/ch/11/config", "/ch/11/preamp", "/ch/11/gate", "/ch/11/gate/filter", "/ch/11/dyn", "/ch/11/dyn/filter", "/ch/11/insert", "/ch/11/eq", "/ch/11/eq/1", "/ch/11/eq/2", "/ch/11/eq/3", "/ch/11/eq/4", "/ch/11/mix", "/ch/11/mix/01", "/ch/11/mix/02", "/ch/11/mix/03", "/ch/11/mix/04", "/ch/11/mix/05", "/ch/11/mix/06", "/ch/11/mix/07", "/ch/11/mix/08", "/ch/11/mix/09", "/ch/11/mix/10", "/ch/11/grp", "/ch/11/automix", "/ch/12/config", "/ch/12/preamp", "/ch/12/gate", "/ch/12/gate/filter", "/ch/12/dyn", "/ch/12/dyn/filter", "/ch/12/insert", "/ch/12/eq", "/ch/12/eq/1", "/ch/12/eq/2", "/ch/12/eq/3", "/ch/12/eq/4", "/ch/12/mix", "/ch/12/mix/01", "/ch/12/mix/02", "/ch/12/mix/03", "/ch/12/mix/04", "/ch/12/mix/05", "/ch/12/mix/06", "/ch/12/mix/07", "/ch/12/mix/08", "/ch/12/mix/09", "/ch/12/mix/10", "/ch/12/grp", "/ch/12/automix", "/ch/13/config", "/ch/13/preamp", "/ch/13/gate", "/ch/13/gate/filter", "/ch/13/dyn", "/ch/13/dyn/filter", "/ch/13/insert", "/ch/13/eq", "/ch/13/eq/1", "/ch/13/eq/2", "/ch/13/eq/3", "/ch/13/eq/4", "/ch/13/mix", "/ch/13/mix/01", "/ch/13/mix/02", "/ch/13/mix/03", "/ch/13/mix/04", "/ch/13/mix/05", "/ch/13/mix/06", "/ch/13/mix/07", "/ch/13/mix/08", "/ch/13/mix/09", "/ch/13/mix/10", "/ch/13/grp", "/ch/13/automix", "/ch/14/config", "/ch/14/preamp", "/ch/14/gate", "/ch/14/gate/filter", "/ch/14/dyn", "/ch/14/dyn/filter", "/ch/14/insert", "/ch/14/eq", "/ch/14/eq/1", "/ch/14/eq/2", "/ch/14/eq/3", "/ch/14/eq/4", "/ch/14/mix", "/ch/14/mix/01", "/ch/14/mix/02", "/ch/14/mix/03", "/ch/14/mix/04", "/ch/14/mix/05", "/ch/14/mix/06", "/ch/14/mix/07", "/ch/14/mix/08", "/ch/14/mix/09", "/ch/14/mix/10", "/ch/14/grp", "/ch/14/automix", "/ch/15/config", "/ch/15/preamp", "/ch/15/gate", "/ch/15/gate/filter", "/ch/15/dyn", "/ch/15/dyn/filter", "/ch/15/insert", "/ch/15/eq", "/ch/15/eq/1", "/ch/15/eq/2", "/ch/15/eq/3", "/ch/15/eq/4", "/ch/15/mix", "/ch/15/mix/01", "/ch/15/mix/02", "/ch/15/mix/03", "/ch/15/mix/04", "/ch/15/mix/05", "/ch/15/mix/06", "/ch/15/mix/07", "/ch/15/mix/08", "/ch/15/mix/09", "/ch/15/mix/10", "/ch/15/grp", "/ch/15/automix", "/ch/16/config", "/ch/16/preamp", "/ch/16/gate", "/ch/16/gate/filter", "/ch/16/dyn", "/ch/16/dyn/filter", "/ch/16/insert", "/ch/16/eq", "/ch/16/eq/1", "/ch/16/eq/2", "/ch/16/eq/3", "/ch/16/eq/4", "/ch/16/mix", "/ch/16/mix/01", "/ch/16/mix/02", "/ch/16/mix/03", "/ch/16/mix/04", "/ch/16/mix/05", "/ch/16/mix/06", "/ch/16/mix/07", "/ch/16/mix/08", "/ch/16/mix/09", "/ch/16/mix/10", "/ch/16/grp", "/ch/16/automix", "/rtn/aux/config", "/rtn/aux/preamp", "/rtn/aux/eq", "/rtn/aux/eq/1", "/rtn/aux/eq/2", "/rtn/aux/eq/3", "/rtn/aux/eq/4", "/rtn/aux/mix", "/rtn/aux/mix/01", "/rtn/aux/mix/02", "/rtn/aux/mix/03", "/rtn/aux/mix/04", "/rtn/aux/mix/05", "/rtn/aux/mix/06", "/rtn/aux/mix/07", "/rtn/aux/mix/08", "/rtn/aux/mix/09", "/rtn/aux/mix/10", "/rtn/aux/grp", "/rtn/1/config", "/rtn/1/preamp", "/rtn/1/eq", "/rtn/1/eq/1", "/rtn/1/eq/2", "/rtn/1/eq/3", "/rtn/1/eq/4", "/rtn/1/mix", "/rtn/1/mix/01", "/rtn/1/mix/02", "/rtn/1/mix/03", "/rtn/1/mix/04", "/rtn/1/mix/05", "/rtn/1/mix/06", "/rtn/1/mix/07", "/rtn/1/mix/08", "/rtn/1/mix/09", "/rtn/1/mix/10", "/rtn/1/grp", "/rtn/2/config", "/rtn/2/preamp", "/rtn/2/eq", "/rtn/2/eq/1", "/rtn/2/eq/2", "/rtn/2/eq/3", "/rtn/2/eq/4", "/rtn/2/mix", "/rtn/2/mix/01", "/rtn/2/mix/02", "/rtn/2/mix/03", "/rtn/2/mix/04", "/rtn/2/mix/05", "/rtn/2/mix/06", "/rtn/2/mix/07", "/rtn/2/mix/08", "/rtn/2/mix/09", "/rtn/2/mix/10", "/rtn/2/grp", "/rtn/3/config", "/rtn/3/preamp", "/rtn/3/eq", "/rtn/3/eq/1", "/rtn/3/eq/2", "/rtn/3/eq/3", "/rtn/3/eq/4", "/rtn/3/mix", "/rtn/3/mix/01", "/rtn/3/mix/02", "/rtn/3/mix/03", "/rtn/3/mix/04", "/rtn/3/mix/05", "/rtn/3/mix/06", "/rtn/3/mix/07", "/rtn/3/mix/08", "/rtn/3/mix/09", "/rtn/3/mix/10", "/rtn/3/grp", "/rtn/4/config", "/rtn/4/preamp", "/rtn/4/eq", "/rtn/4/eq/1", "/rtn/4/eq/2", "/rtn/4/eq/3", "/rtn/4/eq/4", "/rtn/4/mix", "/rtn/4/mix/01", "/rtn/4/mix/02", "/rtn/4/mix/03", "/rtn/4/mix/04", "/rtn/4/mix/05", "/rtn/4/mix/06", "/rtn/4/mix/07", "/rtn/4/mix/08", "/rtn/4/mix/09", "/rtn/4/mix/10", "/rtn/4/grp", "/bus/1/config", "/bus/1/dyn", "/bus/1/dyn/filter", "/bus/1/insert", "/bus/1/eq", "/bus/1/eq/1", "/bus/1/eq/2", "/bus/1/eq/3", "/bus/1/eq/4", "/bus/1/eq/5", "/bus/1/eq/6", "/bus/1/geq", "/bus/1/mix", "/bus/1/grp", "/bus/2/config", "/bus/2/dyn", "/bus/2/dyn/filter", "/bus/2/insert", "/bus/2/eq", "/bus/2/eq/1", "/bus/2/eq/2", "/bus/2/eq/3", "/bus/2/eq/4", "/bus/2/eq/5", "/bus/2/eq/6", "/bus/2/geq", "/bus/2/mix", "/bus/2/grp", "/bus/3/config", "/bus/3/dyn", "/bus/3/dyn/filter", "/bus/3/insert", "/bus/3/eq", "/bus/3/eq/1", "/bus/3/eq/2", "/bus/3/eq/3", "/bus/3/eq/4", "/bus/3/eq/5", "/bus/3/eq/6", "/bus/3/geq", "/bus/3/mix", "/bus/3/grp", "/bus/4/config", "/bus/4/dyn", "/bus/4/dyn/filter", "/bus/4/insert", "/bus/4/eq", "/bus/4/eq/1", "/bus/4/eq/2", "/bus/4/eq/3", "/bus/4/eq/4", "/bus/4/eq/5", "/bus/4/eq/6", "/bus/4/geq", "/bus/4/mix", "/bus/4/grp", "/bus/5/config", "/bus/5/dyn", "/bus/5/dyn/filter", "/bus/5/insert", "/bus/5/eq", "/bus/5/eq/1", "/bus/5/eq/2", "/bus/5/eq/3", "/bus/5/eq/4", "/bus/5/eq/5", "/bus/5/eq/6", "/bus/5/geq", "/bus/5/mix", "/bus/5/grp", "/bus/6/config", "/bus/6/dyn", "/bus/6/dyn/filter", "/bus/6/insert", "/bus/6/eq", "/bus/6/eq/1", "/bus/6/eq/2", "/bus/6/eq/3", "/bus/6/eq/4", "/bus/6/eq/5", "/bus/6/eq/6", "/bus/6/geq", "/bus/6/mix", "/bus/6/grp", "/fxsend/1/config", "/fxsend/1/mix", "/fxsend/1/grp", "/fxsend/2/config", "/fxsend/2/mix", "/fxsend/2/grp", "/fxsend/3/config", "/fxsend/3/mix", "/fxsend/3/grp", "/fxsend/4/config", "/fxsend/4/mix", "/fxsend/4/grp", "/lr/config", "/lr/dyn", "/lr/dyn/filter", "/lr/insert", "/lr/eq", "/lr/eq/1", "/lr/eq/2", "/lr/eq/3", "/lr/eq/4", "/lr/eq/5", "/lr/eq/6", "/lr/geq", "/lr/mix", "/dca/1", "/dca/1/config", "/dca/2", "/dca/2/config", "/dca/3", "/dca/3/config", "/dca/4", "/dca/4/config", "/fx/1", "/fx/1/par", "/fx/2", "/fx/2/par", "/fx/3", "/fx/3/par", "/fx/4", "/fx/4/par", "/routing/main/01", "/routing/main/02", "/routing/aux/01", "/routing/aux/02", "/routing/aux/03", "/routing/aux/04", "/routing/aux/05", "/routing/aux/06", "/routing/p16/01", "/routing/p16/02", "/routing/p16/03", "/routing/p16/04", "/routing/p16/05", "/routing/p16/06", "/routing/p16/07", "/routing/p16/08", "/routing/p16/09", "/routing/p16/10", "/routing/p16/11", "/routing/p16/12", "/routing/p16/13", "/routing/p16/14", "/routing/p16/15", "/routing/p16/16", "/routing/usb/01", "/routing/usb/02", "/routing/usb/03", "/routing/usb/04", "/routing/usb/05", "/routing/usb/06", "/routing/usb/07", "/routing/usb/08", "/routing/usb/09", "/routing/usb/10", "/routing/usb/11", "/routing/usb/12", "/routing/usb/13", "/routing/usb/14", "/routing/usb/15", "/routing/usb/16", "/routing/usb/17", "/routing/usb/18", "/headamp/01", "/headamp/02", "/headamp/03", "/headamp/04", "/headamp/05", "/headamp/06", "/headamp/07", "/headamp/08", "/headamp/09", "/headamp/10", "/headamp/11", "/headamp/12", "/headamp/13", "/headamp/14", "/headamp/15", "/headamp/16", "/headamp/17", "/headamp/18", "/headamp/19", "/headamp/20", "/headamp/21", "/headamp/22", "/headamp/23", "/headamp/24", NULL };


int scnlen(char *arr[]){   //function to return number of elements in NULL terminated string array
 int index;
    for (index=0; arr[index] != NULL; index++);
return(index);
}


// specific to XR series: xdump and xfdump are copied/included here as meters are different (more consistent)
// than on X32. Also, changed so that XAir reply print format is compatible with XAir OSC input when blank
// pathis given as a command (for use in XAir_SnapBackup).
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
							printf("%d rta: \n", n);
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
	if (debug) for (i = 0; i < (strlen(header) + 10); i++) printf(" ");
//	else       printf("%s, %4d B: ", header, len);
	Xdump(buf, len, debug);
}
