

#ifndef APP_H_
#define APP_H_

#include "Os.h"
#if defined(WIN32)
#define SetEvent SetEvent2
#endif
#include "debug.h"

#include <stdio.h>
#include <string.h>

#include "Can.h"
#include "CanIf.h"
#include "CanTp.h"
#include "PduR.h"
#include "Com.h"
#include "Dcm.h"
#include "Dio.h"
#include "Port.h"
#include "Ea.h"
#include "Fee.h"
#include "NvM.h"
#if defined(WIN32)
#include "Stmo.h"
#endif
#if defined(USE_GUI)
#include "Gui.h"
#endif

#include "Diag.h"

// ========= APP Module
#include "app_led.h"
#include "app_gauge.h"
#include "app_time.h"
#include "app_nvm.h"

#endif /* APP_H_ */

