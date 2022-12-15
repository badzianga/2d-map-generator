#pragma once

#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

#define DELETE_KEY 8

class Textbox {
    private:
        sf::RectangleShape box;
        sf::Text textbox;
        std::ostringstream text;
        bool isSelected = false;
        bool hasLimit = false;
        int limit;

        void inputLogic(int charTyped) {
            /*
                Insert typed character to the textbox or delete last character.
            */
            if (charTyped != DELETE_KEY) {
                text << static_cast<char>(charTyped);
            } else {
                if (text.str().length() > 0) {
                    deleteLastChar();
                }
            }
            textbox.setString(text.str());
        }

        void deleteLastChar() {
            /*
                Delete last character from the textbox.
            */
            std::string t = text.str();
            std::string newText = "";
            for (int i = 0; i < t.length() - 1; i++) {
                newText += t[i];
            }
            text.str(newText);
        }

    public:
        Textbox(sf::Vector2f boxSize, int charSize, bool selected, int defaultValue) {
            box.setSize(boxSize);
            box.setFillColor(sf::Color(49, 52, 65));
            box.setOutlineThickness(1.f);

            textbox.setCharacterSize(charSize);
            textbox.setFillColor(sf::Color::White);
            textbox.setOutlineColor(sf::Color::Black);
            textbox.setOutlineThickness(1.f);
            text.str(std::to_string(defaultValue));
            textbox.setString(std::to_string(defaultValue));
            isSelected = selected;
            if (selected) {
                box.setOutlineColor(sf::Color(51, 142, 203));
            } else {
                box.setOutlineColor(sf::Color(22, 25, 31));
            }
        }
        void setFont(sf::Font &font) {
            textbox.setFont(font);
        }

        void setPosition(sf::Vector2f position) {
            textbox.setPosition({position.x + 2.f, position.y});
            box.setPosition({position.x, position.y + 2.f});
        }

        void setLimit(bool trueOrFalse) {
            hasLimit = trueOrFalse;
        }

        void setLimit(bool trueOrFalse, int lim) {
            hasLimit = trueOrFalse;
            limit = lim - 1;
        }

        void setSelected(bool selected) {
            isSelected = selected;
            if (isSelected) {
                box.setOutlineColor(sf::Color(51, 142, 203));
            } else {
                box.setOutlineColor(sf::Color(22, 25, 31));
            }
        }

        std::string getText() {
            return text.str();
        }

        void drawTo(sf::RenderWindow *window) {
            window->draw(box);
            window->draw(textbox);
        }

        bool isMouseOver(sf::RenderWindow *window) {
            int mouseX = sf::Mouse::getPosition(*window).x;
            int mouseY = sf::Mouse::getPosition(*window).y;

            float btnPosX = box.getPosition().x;
            float btnPosY = box.getPosition().y;

            float btnXPosWidth = box.getPosition().x + box.getLocalBounds().width;
            float btnYPosHeight = box.getPosition().y + box.getLocalBounds().height;

            return (mouseX <= btnXPosWidth && mouseX >= btnPosX && mouseY <= btnYPosHeight && mouseY >= btnPosY);
        }

        void typedOn(sf::Event input) {
            /*
                Handle writing to textbox.
            */
            if (isSelected) {
                int charTyped = input.text.unicode;
                if ((charTyped > 47 && charTyped < 58) || charTyped == DELETE_KEY) {
                    if (hasLimit) {
                        if (text.str().length() <= limit) {
                            inputLogic(charTyped);
                        } else if (text.str().length() >= limit && charTyped == DELETE_KEY) {
                            deleteLastChar();
                            textbox.setString(text.str());
                        }
                    }
                    else {
                        inputLogic(charTyped);
                    }
                }
            }
        }
};