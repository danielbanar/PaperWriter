#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "imgui.h"
#include <SDL.h>
#include <SDL_image.h>
namespace PaperWriter
{
	struct Text
	{
		std::string text;
		ImVec2 pos;
		float size;
		SDL_Texture* texture;
	};

	static std::vector<PaperWriter::Text> vTexts;
	static ImVec2 vecPaperSize = { 5100,7020 };
	void RenderUI();
	void AddText(const std::string& text, ImVec2 pos, float textSize = 1.f);
	void AddText(const std::string& text, float x = 0, float y = 0, float textSize = 1.f);
	void EditText(int index, std::string newText);
	void DrawText(const Text& text);
	SDL_Texture* CreateTexture(const Text& text);
}