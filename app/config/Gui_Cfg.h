
#ifndef GUI_CFG_H_
#define GUI_CFG_H_

#define GUI_MAX_LAYERS  2

// B = background
// P = pointer
enum
{
	GUI_W_SPEED_B = 0,
	GUI_W_TACHO_B,
	GUI_W_SPEED_P,
	GUI_W_TACHO_P,

};
extern const GuiConfig_Type GuiConfigData;
#endif /* GUI_CFG_H_ */
