/*
    gpio-toggle-pin.cpp - Simple C++ program to toggle a BBB GPIO pin.
    Copyright (C) 2020 Geoffrey G. Messier

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define GPIO_PIN_SET_DIR(x,y) { \
	char fileStr[50]; \
	sprintf(fileStr,"/sys/class/gpio/gpio%d/direction",(x)); \
	FILE *fpDir = fopen(fileStr,"w"); \
	fprintf(fpDir,(y)); \
	fclose(fpDir); \
}

#define GPIO_PIN_OPEN(x,y) { \
	char fileStr[50]; \
	sprintf(fileStr,"/sys/class/gpio/gpio%d/value",(x)); \
	(y) = fopen(fileStr,"w"); \
}

#define GPIO_PIN_CLOSE(y) { \
	fclose(y); \
}

#define GPIO_PIN_SET_HIGH(y){ \
	fprintf(y,"1"); \
	fflush(y); \
}

#define GPIO_PIN_SET_LOW(y){ \
	fprintf(y,"0"); \
	fflush(y); \
}

int main() {

	GPIO_PIN_SET_DIR(112,"out");

	FILE *pin112;
	GPIO_PIN_OPEN(112,pin112);

	while(1){
		GPIO_PIN_SET_HIGH(pin112);
		//usleep(1000);
		GPIO_PIN_SET_LOW(pin112);
		//usleep(1000);
	}

	return 0;
}
