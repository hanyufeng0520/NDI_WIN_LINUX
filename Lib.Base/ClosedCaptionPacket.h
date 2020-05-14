#pragma once
#define NB_VBI_PACKET_CNT	5

struct VBIPacket
{
#define VBI_ID_LENGTH 40
	int		idLineNumber;
	char	id[VBI_ID_LENGTH];

	VBIPacket()
	{
		clear();
	}
	void clear()
	{
		idLineNumber = -1;
		memset(id, 0, VBI_ID_LENGTH);
	}
};

struct ClosedCaptionPacket
{
	VBIPacket packetEven[NB_VBI_PACKET_CNT];
	VBIPacket packetOdd[NB_VBI_PACKET_CNT];
	ClosedCaptionPacket()
	{
		clear();
	}
	void clear()
	{
		for (int n = 0; n < NB_VBI_PACKET_CNT; ++n)
		{
			packetEven[n].clear();
			packetOdd[n].clear();
		}
	}
	bool getHasClosedCaption() const
	{
		bool bRes = false;
		for (int n = 0; n < NB_VBI_PACKET_CNT; ++n)
		{
			if (packetEven[n].idLineNumber != -1 || packetOdd[1].idLineNumber != -1)
			{
				bRes = true;
				break;
			}
		}
		return bRes;
	}
};
