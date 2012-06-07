/* 
 * File:   common.h
 * Author: matt
 *
 * Created on 27 May 2012, 6:00 PM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <string.h>
#include <vector>
#include <map>
#include <iostream>

#define DEBUG_LOG std::cout << "DEBUG: "
void friendlyExit(int code);

#define exit(code) friendlyExit(code);
#define _exit(code) friendlyExit(code);

#endif	/* COMMON_H */
