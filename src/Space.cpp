//
// Created by Данил on 04.03.2021.
//

#include "../include/Space.h"
#include "../include/Objects.h"


Cell::~Cell() {
    this->object = nullptr;
}

char Cell::getFiller() {
    return this->filler;
}

void Cell::setFiller(char c) {
    this->filler = c;
}

Object *Cell::getObject() {
    return this->object;
}

void Cell::setObject(Object *object1) {
    this->object = object1;
}

int Cell::getSymbolColor() const {
    return symbolColor;
}

void Cell::setSymbolColor(int color) {
    this->symbolColor = color;
}

int Cell::getBackgroundColor() const {
    return backgroundColor;
}

void Cell::setBackgroundColor(int color) {
    this->backgroundColor = color;
}

Cell::Cell(int X,
           int Y,
           char filler1,
           int backgroundColor1,
           int symbolColor1) {
    this->posX = X;
    this->posY = Y;
    this->filler = filler1;
    this->backgroundColor = backgroundColor1;
    this->symbolColor = symbolColor1;
    this->object = nullptr;
    this->priority = LOW_PRIORITY;
}

Space::Space() {
    this->prevCondition.resize(displayHeight);
    for (int i = 0; i < displayHeight; ++i) {
        for (int j = 0; j < displayWidth; ++j) {
            this->prevCondition[i].push_back(startSpacePicture[i][j]);
        }
    }
    this->nextCondition.resize(displayHeight);
    for (int i = 0; i < displayHeight; ++i) {
        for (int j = 0; j < displayWidth; ++j) {
            this->nextCondition[i].push_back(startSpacePicture[i][j]);
        }
    }
    for (int i = 0; i < displayHeight; ++i) {
        for (int j = 0; j < displayWidth; ++j) {
            int bcolor = black;
            int scolor = black;
            if (startSpacePicture[i][j] == '*') {
                scolor = white;
            }
            Cell *cell = new Cell(j, i, this->prevCondition[i][j], bcolor, scolor);
            this->space[i][j] = cell;
        }
    }
}

void Space::setCell(int x, int y, Cell *cell) {
    this->space[y][x] = cell;
}

Cell *Space::getCell(int x, int y) {
    return this->space[y][x];
}

Scene::Scene(int sceneNumber) {
    this->prevCondition.resize(displayHeight);
    this->nextCondition.resize(displayHeight);
    this->prevConditionColor.resize(displayHeight);
    this->nextConditionColor.resize(displayHeight);
    for (int i = 0; i < displayHeight; ++i) {
        for (int j = 0; j < displayWidth; ++j) {
            this->prevCondition[i].push_back(display[i][j]);
            this->prevConditionColor[i].push_back(displayColor[i][j]);
            this->nextCondition[i].push_back(display[i][j]);
            this->nextConditionColor[i].push_back(displayColor[i][j]);
        }

    }
    if (sceneNumber == TEST) {
        this->scene_.framesCount = __s_test::framesCount;
        this->scene_.frames.push_back(__s_test::__s_test_1());
        this->scene_.frames.push_back(__s_test::__s_test_2());
        this->scene_.frames.push_back(__s_test::__s_test_3());
        this->scene_.frames.push_back(__s_test::__s_test_4());
    }
}

void Scene::rawRender() {
    for (int i = 0; i < this->scene_.framesCount; ++i) {
        if (i > 0) {
            for (int j = 0; j < this->scene_.frames[i - 1].sizeY; ++j) {
                for (int k = 0; k < this->scene_.frames[i - 1].sizeX; ++k) {
                    setCursorPosition(this->scene_.frames[i - 1].xPadding + k, this->scene_.frames[i - 1].yPadding + j);
                    setConsoleColour(cc(black, black));
                    std::cout << ' ';
                }
            }
        }
        for (int j = 0; j < this->scene_.frames[i].sizeY; ++j) {
            for (int k = 0; k < this->scene_.frames[i].sizeX; ++k) {
                setCursorPosition(this->scene_.frames[i].xPadding + k, this->scene_.frames[i].yPadding + j);
                setConsoleColour(this->scene_.frames[i].color[j][k]);
                std::cout << this->scene_.frames[i].image[j][k];
            }
        }
        Sleep(1000);
    }
}

void Scene::render(int frequency) {
    for (int i = 0; i < this->scene_.framesCount; ++i) {
        if (i > 0) {

            // TODO: оптимизировать затирание прошлой сцены

            for (int j = 0; j < this->scene_.frames[i - 1].sizeY; ++j) {
                for (int k = 0; k < this->scene_.frames[i - 1].sizeX; ++k) {
                    setCursorPosition(this->scene_.frames[i - 1].xPadding + k, this->scene_.frames[i - 1].yPadding + j);
                    setConsoleColour(
                        displayColor[this->scene_.frames[i - 1].yPadding + j][this->scene_.frames[i - 1].xPadding + k]);
                    std::cout
                        << display[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding + j];
                }
            }
        }
        for (int j = 0; j < this->scene_.frames[i].sizeY; ++j) {
            for (int k = 0; k < this->scene_.frames[i].sizeX; ++k) {
                nextCondition[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                       j] = this->scene_.frames[i].image[j][k];
                nextConditionColor[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                            j] = this->scene_.frames[i].color[j][k];
                if (nextCondition[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding + j] !=
                    prevCondition[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding + j] ||
                    nextConditionColor[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                                j] !=
                    prevConditionColor[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                                j]) {
                    setCursorPosition(this->scene_.frames[i].xPadding + k, this->scene_.frames[i].yPadding + j);
                    setConsoleColour(this->scene_.frames[i].color[j][k]);

                    if (this->scene_.frames[i].image[j][k] != ' ' && this->scene_.frames[i].color[j][k] != -1) {
                        std::cout << this->scene_.frames[i].image[j][k];

                        prevCondition[this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                               j] = nextCondition[
                            this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding + j];
                        prevConditionColor[this->scene_.frames[i - 1].xPadding + k][
                            this->scene_.frames[i - 1].yPadding +
                            j] = nextConditionColor[
                            this->scene_.frames[i - 1].xPadding + k][this->scene_.frames[i - 1].yPadding +
                                                                     j];
                    }
                }
            }
        }
        Sleep(frequency);
    }
    for (int j = 0; j < this->scene_.frames[this->scene_.framesCount - 1].sizeY; ++j) {
        for (int k = 0; k < this->scene_.frames[this->scene_.framesCount - 1].sizeX; ++k) {
            setCursorPosition(this->scene_.frames[this->scene_.framesCount - 1].xPadding + k,
                              this->scene_.frames[this->scene_.framesCount - 1].yPadding + j);
            setConsoleColour(displayColor[this->scene_.frames[this->scene_.framesCount - 1].xPadding + k][
                                 this->scene_.frames[this->scene_.framesCount - 1].yPadding + j]);
            std::cout << display[this->scene_.frames[this->scene_.framesCount - 1].xPadding + k][
                this->scene_.frames[this->scene_.framesCount - 1].yPadding + j];
        }
    }

    // TODO: Вернуть вектора в стартовое положение
}

void Movie::setStartDisplayCondition() {
    for (auto &i : display) {
        for (int j = 0; j < displayWidth; ++j) {
            i[j] = ' ';
        }
    }
}

void Movie::setStartDisplayColors() {
    std::random_device gen;
    for (auto &i : displayColor) {
        for (int j = 0; j < displayWidth; ++j) {
//            i[j] = cc(black, black);
            i[j] = 28;// static_cast<signed>(gen() % 256);
        }
    }
}

void Movie::renderStartDisplay() {
    std::random_device gen;
    for (int i = 0; i < displayHeight; ++i) {
        for (int j = 0; j < displayWidth; ++j) {
            setCursorPosition(j, i);
            setConsoleColour(displayColor[i][j]);
//            setConsoleColour(gen() % 256);
            std::cout << display[i][j];
        }
    }
}

void Movie::wait(int seconds) {
    Sleep(seconds * 1000);
}

void Movie::setAreaToNormalCondition(int startX, int startY, int finishX, int finishY) {
    for (int i = startY; i < finishY; ++i) {
        for (int j = startX; j < finishX; ++j) {
            setCursorPosition(j, i);
            setConsoleColour(displayColor[i][j]);
            std::cout << display[i][j];
        }
    }
}

