// zSubtitlePlayer.h
// Author: Václav Maroušek
// Copyright (c) 2026 The Chronicles of Myrtana Team

namespace GOTHIC_ENGINE {

#define SUBTITLE_OFFSET_Y 7424 // from top

class zCSubtitlePlayer
{
public:

	~zCSubtitlePlayer();

	void ParseSubtitles(const zSTRING& filename);
	void RenderSubtitles(unsigned int time);

private:

	struct SubtitleItem
	{
		unsigned int startTime;	// ms
		unsigned int endTime;	// ms
		unsigned int lineIndex;
		unsigned int lineCount;
	};

	zCArray<SubtitleItem>	m_items;
	zCArray<zSTRING>		m_lines;
	zCView*					m_view;

};

}
