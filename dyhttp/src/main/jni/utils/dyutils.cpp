/*
 * dyutils.cpp
 *
 *  Created on: 2016��8��10��
 *      Author: Yongpan Song
 */
#include "dyutils.h"
#include <sys/time.h>

unsigned long getTimestamp() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	return ((tv.tv_sec * 1000000) + (tv.tv_usec));
}
