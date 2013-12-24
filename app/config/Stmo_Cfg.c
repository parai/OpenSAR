
#include "Stmo.h"

static const Stmo_ChannelType StmoChannels[] =
{
	{ // Speed
		.SoftwareZero = 1000
	},
	{ // Tacho
		.SoftwareZero = 1000
	}
};

const Stmo_ConfigType Stmo_ConfigData =
{
	.Channels = StmoChannels
};
