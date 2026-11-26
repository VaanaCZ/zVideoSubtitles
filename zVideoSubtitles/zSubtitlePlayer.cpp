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

void ParseLine(zSTRING& str)
{
	int start = 0;
	while ((start = str.Search("$", start, false)) != -1)
	{
		// Get name of symbol
		int nameStart = start + 1;
		int end = nameStart;

		while (end < str.Length())
		{
			unsigned char c = str[(unsigned int)end];
			
			if (!isalnum(c) && c != '_')
				break;

			end++;
		}

		if (nameStart == end)
		{
			start = end;
			continue;
		}

		zSTRING symName = ((const zSTRING&)str).Copy(nameStart, end - nameStart);

		// Try to resolve symbol value
		zSTRING symValue;

		zCPar_Symbol* sym = parser->GetSymbol(symName);
		if (sym && sym->type == zPAR_TYPE_STRING)
		{
			// From scripts
			symValue = sym->stringdata[0];
		}
		else
		{
			// From output units
			int num = ogame->GetCutsceneManager()->LibValidateOU(symName);
			if (num != -1)
			{
				zCCSBlock* block = ogame->GetCutsceneManager()->LibGet(num);
				zCCSAtomicBlock* aBlock = dynamic_cast<zCCSAtomicBlock*>(block->GetChild(0));

				if (aBlock)
				{
					oCMsgConversation* msg = dynamic_cast<oCMsgConversation*>(aBlock->GetCommand());
					if (msg)
					{
						symValue = msg->text;
					}
				}
			}
		}

		// Replace or leave as is
		if (!symValue.IsEmpty())
		{
			str.Cut(start, end - start);
			str.Put(symValue, start);
			start += symValue.Length();
		}
		else
		{
			start = end;
		}
	}
}

zCSubtitlePlayer::~zCSubtitlePlayer()
{
	if (m_view) zDELETE(m_view);
}

void zCSubtitlePlayer::ParseSubtitles(const zSTRING& filename)
{
	m_items.EmptyList();
	m_lines.EmptyList();

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

	SubtitleItem* item = nullptr;
	zSTRING	line = "";
	while (!file->Eof())
	{
		file->Read(line);

		if (line.IsEmpty())
		{
			item = nullptr;
			continue;
		}

		int separator = line.Search("-->", 0, false);
		if (separator != -1)
		{
			item = nullptr;

			// Have to do it like this because Union strings are stupid
			zSTRING startTimeStr = line; startTimeStr.Cut(separator, 256);
			zSTRING endTimeStr = line; endTimeStr.Cut(separator + 3);

			startTime	= StringToTimeMS(startTimeStr);
			endTime		= StringToTimeMS(endTimeStr);

			if (endTime > startTime)
			{
				m_items.Insert(SubtitleItem());
				item = &m_items[m_items.GetNum() - 1];

				item->startTime	= startTime;
				item->endTime	= endTime;
				item->lineIndex	= m_lines.GetNum();
				item->lineCount	= 0;
			}
		}
		else
		{
			if (!item) continue;

			ParseLine(line);

			m_lines.Insert(line);
			item->lineCount++;

			startTime	= 0;
			endTime		= 0;
		}
	}

	file->Close();
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

			int maxLineWidth = 0;
			for (int i = 0; i < item.lineCount; i++)
			{
				int lineIdx = item.lineIndex + i;
				int lineWidth = screen->FontSize(m_lines[lineIdx]);
				if (lineWidth > maxLineWidth)
					maxLineWidth = lineWidth;
			}

			int maxLineCount = 3;
			if (item.lineCount > maxLineCount)
				maxLineCount = item.lineCount;

			int xSize = maxLineWidth + (screenFontY * 2);
			int ySize = screenFontY * (maxLineCount + 1);
			int xPos = (VIEW_VXMAX - xSize) / 2;
			int yPos = SUBTITLE_OFFSET_Y - (ySize / 2);

			m_view->SetPos(xPos, yPos);
			m_view->SetSize(xSize, ySize);

			// Print text
			int viewFontY = m_view->any(fontY);
			int startPrintY = (VIEW_VYMAX - (item.lineCount * viewFontY)) / 2;

			for (int i = 0; i < item.lineCount; i++)
			{
				int lineIdx = item.lineIndex + i;
				m_view->PrintCX(startPrintY + (i * viewFontY), m_lines[lineIdx]);
			}

			m_view->Blit();
			m_view->ClrPrintwin();
			break;
		}
	}
}

}