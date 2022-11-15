#include "paperwriter.h"
#include "imgui_stdlib.h"
#include <algorithm>

extern SDL_Renderer* renderer;
void PaperWriter::Init()
{
	surfPaper = CreateEmptyPaper();
}
void PaperWriter::RenderUI()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::Begin("Properties");

	static std::string sText;
	ImGui::InputText("text", &sText);

	if (ImGui::Button("Add text"))
		PaperWriter::AddText(sText);
	ImGui::End();

	/*********************************************************************************/

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

	/*********************************************************************************/


	ImGui::Begin("Controls");
	static float fZoom = 1.f;
	static SDL_Texture* texPaper;
	ImGui::SliderFloat("Zoom", &fZoom, 0.05f, 1.f, "%.2f");
	if (ImGui::Button("Refresh Paper"))
	{
		RedrawPaper();
		if (texPaper)
			SDL_DestroyTexture(texPaper);
		texPaper = SDL_CreateTextureFromSurface(renderer, surfPaper);
	}
		ImGui::End();

	/*********************************************************************************/
	ImGui::Begin("Paper");
	ImGui::Image(texPaper, { surfPaper->w * fZoom,surfPaper->h * fZoom });
	ImGui::End();
	/*********************************************************************************/

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
SDL_Surface* PaperWriter::CreateSurfaceFromText(const Text& text)
{
	//TODO: get max width of line
	int rows = std::count(text.text.begin(), text.text.end(), '\n') + 1;
	SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, (int)vecPaperSize.y, int(rows * CHAR_HEIGHT * text.size), 32, NULL, NULL, NULL, NULL);
	SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 255, 255, 255, 255));
	int row = 0;
	int col = 0;
	for (char c : text.text)
	{
		if (c == '\n')
		{
			row++;
			col = 0;
		}
		else
		{
			char path[32];
			sprintf(path, "characters\\%c.jpg", c);
			SDL_Surface* surfChar = IMG_Load(path);
			SDL_Rect rect = { col, row * CHAR_HEIGHT * text.size };
			SDL_BlitSurface(surfChar, 0, surf, &rect);
			col += surfChar->w * text.size;
			SDL_FreeSurface(surfChar);
		}
	}
	return surf;
}

SDL_Surface* PaperWriter::CreateEmptyPaper()
{
	//TODO: load from file
	using namespace PaperWriter;
	SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, vecPaperSize.x, vecPaperSize.y, 32, NULL, NULL, NULL, NULL);
	SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 255, 255, 255, 255));
	return surf;
}

void PaperWriter::RedrawPaper()
{
	if (surfPaper)
		SDL_FreeSurface(surfPaper);
	surfPaper = CreateEmptyPaper();
	for (const Text& text : vTexts)
	{
		auto surfText = CreateSurfaceFromText(text);
		SDL_Rect rect = { text.pos.x,text.pos.y,surfText->w,surfText->h };
		SDL_BlitSurface(surfText, nullptr, surfPaper, &rect);
		SDL_FreeSurface(surfText);
	}
}