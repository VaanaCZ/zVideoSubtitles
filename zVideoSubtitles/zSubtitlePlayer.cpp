// zSubtitlePlayer.cpp
// Author: Václav Maroušek
// Copyright (c) 2026 The Chronicles of Myrtana Team

namespace GOTHIC_ENGINE {

unsigned int StringToTimeMS(const zSTRING& str)
{
	int hours, mins, seconds, milliseconds;

	int c1 = str.Search(":", 0, false);
	if (c1 == -1) return 0;

	int c2 = str.Search(":", c1 + 1, false);
	if (c2 == -1) return 0;

	int c3 = str.Search(",", c2 + 1, false);
	if (c3 == -1) c3 = str.Search(".", c2 + 1, false);
	if (c3 == -1) return 0;

	hours = atoi(str.ToChar());
	mins = atoi(str.ToChar() + c1 + 1);
	seconds = atoi(str.ToChar() + c2 + 1);
	milliseconds = atoi(str.ToChar() + c3 + 1);
	
	return hours * 3600000 + mins * 60000 + seconds * 1000 + milliseconds;
}

zCSubtitlePlayer::~zCSubtitlePlayer()
{
	if (m_view) zDELETE(m_view);
}

void zCSubtitlePlayer::ParseSubtitles(const zSTRING& filename)
{
	m_items.EmptyList();

	zBOOL enableSubtitles = zoptions->ReadInt("GAME", "subTitles", 1);
	if (!enableSubtitles)
		return;

	if (!m_view)
	{
		m_view = zNEW(zCView(0, 0, 100, 100));
		m_view->InsertBack("DLG_CONVERSATION.TGA");
		m_view->SetFontColor(NPC_COLOR_TALK_NPC);

		m_view->owner = screen;
	}

	zFILE* file = zfactory->CreateZFile(filename);

	if (!file->Exists() || file->Open(false) != 0)
	{
		delete file;
		return;
	}

	//
	// File exists => parse line by line
	//

	unsigned int startTime = 0;
	unsigned int endTime = 0;

	zSTRING	line = "";
	while (!file->Eof())
	{
		file->Read(line);

		if (!line.IsEmpty())
		{
			int separator = line.Search("-->", 0, false);
			if (separator != -1)
			{
				// Have to do it like this because Union strings are stupid
				zSTRING startTimeStr = line; startTimeStr.Cut(separator, 256);
				zSTRING endTimeStr = line; endTimeStr.Cut(separator + 3);

				startTime = StringToTimeMS(startTimeStr);
				endTime = StringToTimeMS(endTimeStr);
			}
			else
			{
				if (endTime <= startTime)
					continue;

				SubtitleItem item;
				item.startTime	= startTime;
				item.endTime	= endTime;
				item.text		= line;
				m_items.Insert(item);

				startTime = 0;
				endTime = 0;
			}
		}
	}

	delete file;
}

void zCSubtitlePlayer::RenderSubtitles(unsigned int time)
{
	for (int i = 0; i < m_items.GetNum(); i++)
	{
		SubtitleItem& item = m_items[i];

		if (time >= item.startTime && time <= item.endTime)
		{
			int fontY = m_view->font->GetFontY();

			// Set up box
			int screenFontY = screen->any(fontY);
			int ySize = screenFontY * 4;
			int yPos = SUBTITLE_OFFSET_Y - (ySize / 2);

			int xSize = screen->FontSize(item.text) + (screenFontY * 2);
			int xPos = (VIEW_VXMAX - xSize) / 2;

			m_view->SetPos(xPos, yPos);
			m_view->SetSize(xSize, ySize);

			// Print text
			int viewFontY = m_view->any(fontY);
			int printY = (VIEW_VYMAX - viewFontY) / 2;
			m_view->PrintCX(printY, item.text);
			m_view->Blit();
			m_view->ClrPrintwin();
		}
	}
}

}