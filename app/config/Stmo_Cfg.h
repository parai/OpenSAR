
#ifndef STMO_CFG_H_
#define STMO_CFG_H_

#define STMO_MAX_DEGREE  (36000)
#define STMO_ONE_DEGREE  (100)

#define STMO_ONE_STEP    (25)  // Degree Change in 1 Stmo_MainFunction call

typedef enum
{
	STMO_ID_SPEED,
	STMO_ID_TACHO,
	STMO_CFG_NUM
}Stmo_IdType;

typedef struct
{
	Stmo_DegreeType SoftwareZero;

}Stmo_ChannelType;

typedef struct
{
	Stmo_ChannelType* Channels;
}Stmo_ConfigType;

extern const Stmo_ConfigType Stmo_ConfigData;

#endif /* STMO_CFG_H_ */
