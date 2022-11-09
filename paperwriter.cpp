#include "paperwriter.h"
#include "imgui_stdlib.h"
#include <algorithm>
#define CHAR_WIDTH 80
#define CHAR_HEIGHT 240
extern SDL_Renderer* renderer;
void PaperWriter::RenderUI()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	{
		ImGui::Begin("Properties");

		static std::string sText;
		ImGui::InputText("text", &sText);

		if (ImGui::Button("Add text"))
			PaperWriter::AddText(sText);
		ImGui::End();
	}
	{
		ImGui::Begin("Hierarchy");
		for (size_t i = 0; i < vTexts.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::InputText("Text", &vTexts[i].text);
			ImGui::DragFloat2("Position", (float*)&vTexts[i].pos, 1.f, 0, +FLT_MAX, "%.f");
			ImGui::InputFloat("Text scale", &vTexts[i].size, .05f, 1.f, "%.2f");
			if (ImGui::Button("Remove"))
				vTexts.erase(vTexts.begin() + i--);
			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::End();
	}
	{
		ImGui::Begin("Paper");
		for (size_t i = 0; i < vTexts.size(); i++)
		{
			if (!vTexts[i].texture)
			{
				vTexts[i].texture = PaperWriter::CreateTexture(vTexts[i]);
			}
			SDL_Point size;
			SDL_QueryTexture(vTexts[i].texture, NULL, NULL, &size.x, &size.y);
			ImGui::Image((void*)vTexts[i].texture, { (float)size.x,(float)size.y });
		}
		ImGui::End();
	}
}
void PaperWriter::AddText(const std::string& text, ImVec2 pos, float textSize)
{
	PaperWriter::vTexts.push_back({ text, pos, textSize });
}
void PaperWriter::AddText(const std::string& text, float x, float y, float textSize)
{
	PaperWriter::vTexts.push_back({ text, {x, y}, textSize });
}
void PaperWriter::EditText(int index, std::string newText)
{
	PaperWriter::vTexts[index].text = newText;
}
SDL_Texture* PaperWriter::CreateTexture(const Text& text)
{
	//TODO: get max width of line
	int lines = std::count(text.text.begin(), text.text.end(), '\n') + 1;
	SDL_Surface* surfText = SDL_CreateRGBSurface(0, (int)vecPaperSize.y, int(lines * CHAR_HEIGHT * text.size), 32, 0, 0, 0, 0);
	int line = 0;
	int col = 0;
	for (char c : text.text)
	{
		if (c == '\n')
		{
			line++;
			col = 0;
		}
		else
		{
			char path[32];
			sprintf(path, "characters\\%c.jpg", c);
			SDL_Surface* surfChar = IMG_Load(path);
			std::cout << SDL_GetError();
			SDL_Rect rect = { col, line * CHAR_HEIGHT * text.size };
			SDL_BlitSurface(surfChar, 0, surfText, &rect);
			col += surfChar->w * text.size;
			SDL_FreeSurface(surfChar);
		}
	}
	auto texture = SDL_CreateTextureFromSurface(renderer, surfText);
	SDL_FreeSurface(surfText);
	return texture;
}