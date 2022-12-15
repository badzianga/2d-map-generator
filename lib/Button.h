#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Button {
    public:
        Button() {  }
        Button(sf::Vector2f buttonSize, std::string text, int charSize) {
            this->text.setString(text);
            this->text.setFillColor(sf::Color::White);
            this->text.setOutlineColor(sf::Color::Black);
            this->text.setOutlineThickness(1.f);
            this->text.setCharacterSize(charSize);
            button.setSize(buttonSize);
            button.setFillColor(sf::Color(38, 42, 51));
            button.setOutlineColor(sf::Color(20, 22, 27));
            button.setOutlineThickness(1.f);
        }

        void setFont(sf::Font &font) {
            text.setFont(font);
        }

        void setBgColor(sf::Color color) {
            button.setFillColor(color);
        }

        void setTextColor(sf::Color color) {
            text.setFillColor(color);
        }

        void setPosition(sf::Vector2f position) {
            button.setPosition(position);

            float xPos = (position.x + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
            float yPos = (position.y + button.getLocalBounds().height / 2) - (text.getLocalBounds().height / 2) - 6;
            text.setPosition(sf::Vector2f(xPos, yPos));
        }

        void drawTo(sf::RenderWindow *window) {          
            window->draw(this->button);
            window->draw(this->text);
        }

        bool isMouseOver(sf::RenderWindow *window) {
            int mouseX = sf::Mouse::getPosition(*window).x;
            int mouseY = sf::Mouse::getPosition(*window).y;

            float btnPosX = button.getPosition().x;
            float btnPosY = button.getPosition().y;

            float btnXPosWidth = button.getPosition().x + button.getLocalBounds().width;
            float btnYPosHeight = button.getPosition().y + button.getLocalBounds().height;

            return (mouseX <= btnXPosWidth && mouseX >= btnPosX && mouseY <= btnYPosHeight && mouseY >= btnPosY);
        }

    private:
        sf::RectangleShape button;
        sf::Text text;
};