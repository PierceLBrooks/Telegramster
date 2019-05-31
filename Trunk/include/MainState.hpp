#ifndef MAIN_STATE_HPP
#define MAIN_STATE_HPP

#include <vector>
#include "State.hpp"

namespace sfml
{
	class MainState : public State
	{
		public:
			MainState();
			void enter();
			void exit();
			void update();

			sf::Color colorConvert(sf::Vector3f color);
			void drawLine(sf::Vector2f point1, sf::Vector2f point2);
			void draw(sf::Vector2f point);
			bool drawing, brushMode;
			float deltaTime;
			int brushSize, brushSizeMax, drawMode, gray, scale;
			sf::Color brushColor;
			sf::Image* pictureImg;
			sf::Image* brushImg;
			sf::Texture* pictureTex;
			sf::Texture* brushTex;
			sf::Sprite* pictureSpr;
			sf::Sprite* brushSpr;
			sf::Clock clock;
			sf::Vector2f mouse, mousePrev, mouseReal, window, windowRatio, cursor;
			sf::RectangleShape* back;
			sf::RectangleShape* colorSelect;
			sf::RectangleShape* colorUnit;
			sf::RectangleShape* brushSizeSliderBack;
			sf::RectangleShape* brushSizeSlider;
			sf::RenderTexture* paletteColorTex;
			sf::RenderTexture* paletteGrayTex;
			sf::RectangleShape* paletteColorOutline;
			sf::RectangleShape* paletteGrayOutline;
			sf::Sprite* paletteColorSpr;
			sf::Sprite* paletteGraySpr;
			sf::Text* colorSelectTxt;
			sf::Text* brushSizeSliderTxt;
			sf::Font* font;

		private:
			bool checkDraw() const;
	};
}

#endif
