#ifndef PORTABLE_H
#define PORTABLE_H

#include "portmacro.h"

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );

#endif	/* PORTABLE_H */