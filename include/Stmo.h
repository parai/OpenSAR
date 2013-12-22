
#ifndef STMO_H_
#define STMO_H_
#include "Std_Types.h"
typedef enum
{
	eStmoId_Speed,
	eStmoId_Tacho,
	eStmoCount
}Stmo_IdType;

typedef uint16 Stmo_DegreeType;	// uint in 0.01 degree, [0,36000]

extern void Stmo_Init(void);
extern Std_ReturnType Stmo_SetPosDegree(Stmo_IdType Id,Stmo_DegreeType Degree);


#endif /* STMO_H_ */
