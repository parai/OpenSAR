
#ifndef STMO_H_
#define STMO_H_
#include "Std_Types.h"

typedef uint16 Stmo_DegreeType;	// uint in 0.01 degree, [0,36000]

#include "Stmo_Cfg.h"

extern void Stmo_Init(const Stmo_ConfigType *Config);
extern Std_ReturnType Stmo_SetPosDegree(Stmo_IdType Id,Stmo_DegreeType Degree);
extern void Stmo_MainFunction(void);

#endif /* STMO_H_ */
