#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <iostream>
#include <vector>
#include <algorithm> 
#include "TEAREADER.H"
#include "TEASHUFFLER.H"

enum AppState { STATE_START, STATE_MODE, STATE_RANDOM, STATE_THEME };

struct ImageButton {
    sf::Sprite sprite;
    void setup(sf::Texture& tex, float x, float y) {
        sprite.setTexture(tex);
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin(b.width / 2.0f, b.height / 2.0f);
        sprite.setPosition(x, y);
    }
    void setup(sf::Texture& tex, float x, float y, float width, float height) {
        sprite.setTexture(tex);
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin(b.width / 2.0f, b.height / 2.0f);
        sprite.setPosition(x, y);
        sprite.setScale(width / b.width, height / b.height);
    }
    bool isClicked(sf::Vector2f mousePos) { return sprite.getGlobalBounds().contains(mousePos); }
    void draw(sf::RenderWindow& window) { window.draw(sprite); }
};

void wrapText(sf::Text& text, float maxWidth) {
    sf::String originalStr = text.getString(); sf::String wrappedStr = "";
    sf::Text tempText = text; tempText.setString("");
    size_t lastSpace = 0; size_t lineStartIndex = 0;
    for (size_t i = 0; i < originalStr.getSize(); ++i) {
        sf::Uint32 charCode = originalStr[i];
        if (charCode == ' ') lastSpace = i;
        tempText.setString(originalStr.substring(lineStartIndex, i - lineStartIndex + 1));
        if (tempText.getLocalBounds().width > maxWidth) {
            if (lastSpace > lineStartIndex) {
                wrappedStr += originalStr.substring(lineStartIndex, lastSpace - lineStartIndex); wrappedStr += "\n";
                lineStartIndex = lastSpace + 1; i = lineStartIndex - 1;
            }
            else {
                wrappedStr += originalStr.substring(lineStartIndex, i - lineStartIndex); wrappedStr += "\n";
                lineStartIndex = i; i = i - 1;
            }
        }
    }
    wrappedStr += originalStr.substring(lineStartIndex, originalStr.getSize() - lineStartIndex);
    text.setString(wrappedStr);
}

void centerText(sf::Text& text, float x, float y) {
    sf::FloatRect r = text.getLocalBounds();
    text.setOrigin(r.left + r.width / 2.0f, r.top + r.height / 2.0f);
    text.setPosition(x, y);
}

void fitBackground(sf::Sprite& sprite, sf::Vector2u windowSize) {
    sf::FloatRect b = sprite.getLocalBounds();
    sprite.setScale((float)windowSize.x / b.width, (float)windowSize.y / b.height);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tea Leaf Oracle", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("montse.otf")) return -1;

    // --- SETUP AUDIO ---

    // 1. Tieng Magic (Ket qua)
    sf::SoundBuffer magicBuffer; sf::Sound magicSound;
    if (magicBuffer.loadFromFile("magic.wav")) { magicSound.setBuffer(magicBuffer); }

    // 2. Tieng Click (Nut bam)
    sf::SoundBuffer clickBuffer; sf::Sound clickSound;
    if (clickBuffer.loadFromFile("click.wav")) { clickSound.setBuffer(clickBuffer); }

    // 3. Nhac nen
    sf::Music bgMusic;
    if (bgMusic.openFromFile("background.wav")) { bgMusic.setLoop(true); bgMusic.setVolume(30); bgMusic.play(); }

    sf::Texture texStart, texmode, texBgShuffle, texTheme;
    sf::Texture texBtnMode1, texBtnMode2, texBtnExit, texBtnShuffle, texBtnExit2;
    texStart.loadFromFile("start.png"); texmode.loadFromFile("mode.png");
    texBgShuffle.loadFromFile("bgshuffle.png"); texTheme.loadFromFile("theme.png");
    texBtnMode1.loadFromFile("mode1.png"); texBtnMode2.loadFromFile("mode2.png");
    texBtnExit.loadFromFile("exit.png"); texBtnShuffle.loadFromFile("shuffle.png");
    texBtnExit2.loadFromFile("exit2.png");

    sf::Sprite Start(texStart);   fitBackground(Start, window.getSize());
    sf::Sprite mode(texmode);   fitBackground(mode, window.getSize());
    sf::Sprite bgShuffle(texBgShuffle); fitBackground(bgShuffle, window.getSize());
    sf::Sprite Theme(texTheme); fitBackground(Theme, window.getSize());

    ImageButton btnRandomMsg, btnThemeMsg, btnBackToStart;
    btnRandomMsg.setup(texBtnMode1, 420, 310, 440, 130);
    btnThemeMsg.setup(texBtnMode2, 420, 440, 440, 130);
    btnBackToStart.setup(texBtnExit, 420, 570, 440, 130);

    ImageButton btnShuffle, btnBackToMode;
    btnShuffle.setup(texBtnShuffle, 640, 400, 229, 60);
    btnBackToMode.setup(texBtnExit2, 1190, 630, 78, 70);

    sf::FloatRect areaLove(250, 300, 300, 80); sf::FloatRect areaHappiness(250, 420, 300, 80);
    sf::FloatRect areaMarriage(250, 540, 300, 80); sf::FloatRect areaCareer(730, 300, 300, 80);
    sf::FloatRect areaSuccess(730, 420, 300, 80); sf::FloatRect areaWealth(730, 540, 300, 80);

    TeaReader oracle; oracle.loadData("DATA.TXT");
    TeaShuffler shuffler(&oracle);
    AppState currentState = STATE_START;

    TeaSymbol* currentSymbol = nullptr;
    sf::Sprite cardSpriteSingle;
    sf::Text nameText, meaningText;
    nameText.setFont(font); nameText.setCharacterSize(40); nameText.setFillColor(sf::Color(107, 0, 213));
    meaningText.setFont(font); meaningText.setCharacterSize(28); meaningText.setFillColor(sf::Color(0, 32, 96));

    bool isFirstTimeShuffle = true;

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window); sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                // --- STATE START ---
                if (currentState == STATE_START) {
                    clickSound.play(); // Click de vao
                    currentState = STATE_MODE;
                }

                // --- STATE MODE ---
                else if (currentState == STATE_MODE) {
                    if (btnRandomMsg.isClicked(worldPos)) {
                        clickSound.play(); // Click nut che do
                        currentState = STATE_RANDOM;
                        shuffler.stop();
                        if (bgMusic.getStatus() == sf::Music::Paused) bgMusic.play();
                        currentSymbol = nullptr;
                        nameText.setString(""); meaningText.setString("");
                        isFirstTimeShuffle = true;
                    }
                    if (btnThemeMsg.isClicked(worldPos)) {
                        clickSound.play(); // Click nut che do
                        currentState = STATE_THEME;
                    }
                    if (btnBackToStart.isClicked(worldPos)) {
                        clickSound.play(); // Click nut thoat
                        currentState = STATE_START;
                    }
                }

                // --- STATE RANDOM ---
                else if (currentState == STATE_RANDOM) {
                    // Nut BACK
                    if (btnBackToMode.isClicked(worldPos)) {
                        clickSound.play(); // Click nut thoat
                        currentState = STATE_MODE;
                        shuffler.stop();
                        if (bgMusic.getStatus() == sf::Music::Paused) bgMusic.play();
                    }
                    else {
                        if (isFirstTimeShuffle) {
                            // LAN DAU: Bam nut Xao bai
                            if (btnShuffle.isClicked(worldPos)) {
                                clickSound.play(); // Click nut xao
                                isFirstTimeShuffle = false;
                                shuffler.start();
                                bgMusic.pause();
                                currentSymbol = nullptr;
                            }
                        }
                        else {
                            // LAN SAU: Bam man hinh
                            if (shuffler.isRunning()) {
                                // DANG XAO -> DUNG
                                shuffler.stop();
                                magicSound.play(); // Tieng Ting ket qua
                                if (bgMusic.getStatus() == sf::Music::Paused) bgMusic.play();

                                currentSymbol = oracle.getRandomSymbol();
                                if (currentSymbol) {
                                    nameText.setString(currentSymbol->getName());
                                    meaningText.setString(currentSymbol->getMeaning());
                                    wrapText(meaningText, 980.0f);
                                    if (currentSymbol->isImageLoaded()) {
                                        cardSpriteSingle.setTexture(*currentSymbol->getTexture());
                                        sf::FloatRect b = cardSpriteSingle.getLocalBounds();
                                        cardSpriteSingle.setOrigin(b.width / 2, b.height / 2);
                                        float scale = 280.0f / std::max(b.width, b.height);
                                        cardSpriteSingle.setScale(scale, scale);
                                        cardSpriteSingle.setPosition(640, 310);
                                    }
                                    centerText(nameText, 640, 490);
                                    centerText(meaningText, 640, 570);
                                }
                            }
                            else {
                                // DANG DUNG -> XAO LAI
                                // O day bam man hinh de xao lai, co the coi la click
                                // Tuy ban muon keu tieng gi, minh de tieng Click nhe
                                clickSound.play();
                                shuffler.start();
                                bgMusic.pause();
                                currentSymbol = nullptr;
                            }
                        }
                    }
                }

                // --- STATE THEME ---
                else if (currentState == STATE_THEME) {
                    if (btnBackToMode.isClicked(worldPos)) {
                        clickSound.play(); // Click nut thoat
                        currentState = STATE_MODE;
                    }

                    bool themeClicked = false;
                    if (areaLove.contains(worldPos) || areaHappiness.contains(worldPos) || areaMarriage.contains(worldPos) ||
                        areaCareer.contains(worldPos) || areaSuccess.contains(worldPos) || areaWealth.contains(worldPos)) {
                        themeClicked = true;
                    }
                    if (themeClicked) {
                        clickSound.play(); // Click chon chu de
                    }
                }
            }
        }

        if (currentState == STATE_RANDOM) { shuffler.update(cardSpriteSingle); }

        window.clear();
        if (currentState == STATE_START) { window.draw(Start); }
        else if (currentState == STATE_MODE) { window.draw(mode); btnRandomMsg.draw(window); btnThemeMsg.draw(window); btnBackToStart.draw(window); }
        else if (currentState == STATE_RANDOM) {
            window.draw(bgShuffle);

            if (isFirstTimeShuffle) {
                btnShuffle.draw(window);
            }
            else {
                if (shuffler.isRunning()) {
                    window.draw(cardSpriteSingle);
                }
                else if (currentSymbol) {
                    if (currentSymbol->isImageLoaded()) window.draw(cardSpriteSingle);
                    window.draw(nameText);
                    window.draw(meaningText);
                }
            }
            btnBackToMode.draw(window);
        }
        else if (currentState == STATE_THEME) {
            window.draw(Theme);
            btnBackToMode.draw(window);
        }

        window.display();
    }
    return 0;
}