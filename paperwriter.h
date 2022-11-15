#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "imgui.h"
#include <SDL.h>
#include <SDL_image.h>
#include <tuple>
#define CHAR_WIDTH 80
#define CHAR_HEIGHT 240
#define PAPER_WIDTH 5100
#define PAPER_HEIGHT 7020
namespace PaperWriter
{
	struct Text
	{
		std::string text;
		ImVec2 pos;
		float size;
		bool operator==(const std::tuple<std::string, ImVec2, float>& o) const
		{
			return (text == std::get<0>(o) && pos.x == std::get<1>(o).x && pos.y == std::get<1>(o).y && size == std::get<2>(o));
		}
		std::tuple<std::string, ImVec2, float> lastState;
	};

	static std::vector<PaperWriter::Text> vTexts;
	static ImVec2 vecPaperSize = { PAPER_WIDTH,PAPER_HEIGHT };
	static SDL_Surface* surfPaper;
	void Init();
	void RenderUI();
	void AddText(const std::string& text, ImVec2 pos, float textSize = 1.f);
	void AddText(const std::string& text, float x = 0, float y = 0, float textSize = 1.f);
	void DrawText(const Text& text);
	SDL_Surface* CreateSurfaceFromText(const Text& text);
	SDL_Surface* CreateEmptyPaper();
	void RedrawPaper();
	void DetectChanges();
}