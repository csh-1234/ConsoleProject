#include <iostream>
#include <Windows.h>
#include "Manager.h"
#include <conio.h>
#include "Player.h"
#include "Knight.h"
#include "Monster.h"
#include <string>
#include <cwchar>
#include <locale.h>


using namespace std;
#pragma warning(disable:4996)
enum class EGameObject
{
    EMPTY = 0,
    // 1 ~ 10 漁
    WALL = 1,
    PLAYER = 2,
    TREE1 = 3,
    TREE2 = 4,
    MONSTER = 5,
    BOSSMONSTER,
    NPC,
    PORTAL = 9,
};

class ConsoleBuffer {
private:
    std::vector<std::vector<wchar_t>> buffer;  // 諦檜萄 僥濠翮 餌辨
    int width, height;
    HANDLE hConsole;

public:
    ConsoleBuffer(int w, int h) : width(w), height(h), buffer(h, std::vector<wchar_t>(w, L' ')) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleScreenBufferSize(hConsole, { (SHORT)w, (SHORT)h });
    }

    void Clear() {
        for (auto& row : buffer) {
            std::fill(row.begin(), row.end(), L' ');  // 諦檜萄 奢寥 餌辨
        }
    }

    void DrawAt(int x, int y, const std::wstring& str) {  // 諦檜萄 僥濠翮 餌辨
        for (size_t i = 0; i < str.length() && x + i < width; ++i) {
            if (y >= 0 && y < height && x + i >= 0) {
                buffer[y][x + i] = str[i];
            }
        }
    }

    void ChangeAt(int x, int y, const std::wstring& str) {  // 諦檜萄 僥濠翮 餌辨
        for (size_t i = 0; i < str.length() && x + i < width; ++i) {
            if (y >= 0 && y < height && x + i >= 0) {
                buffer[y][x + i] = str[i];
            }
        }
    }

    void Render() {
        COORD topLeft = { 0, 0 };
        DWORD written;

        for (const auto& row : buffer) {
            WriteConsoleOutputCharacterW(hConsole, row.data(), width, topLeft, &written);  // 嶸棲囀萄 幗瞪 餌辨
            topLeft.Y++;
        }
    }
};

const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 50;

const int VIEWPORT_WIDTH = 20;  // �飛橦� ル衛腎朝 傘綠
const int VIEWPORT_HEIGHT = 10; // �飛橦� ル衛腎朝 堪檜

Knight* player = new Knight("�垮瘚�");
Slime* slime;
Monster* monster;
int playerX = player->GetPosX();
int playerY = player->GetPosY();

string buffer[MAP_HEIGHT] = {};
int buffer2[50][50] = {};

const int ViewSizeX = 100; // 18 12
const int ViewSizeY = 100;

void DrawMessageBox(ConsoleBuffer& buffer, const std::wstring& message) {
    buffer.DrawAt(4, 35, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(4, 36, L"早                                                                                         早");
    buffer.DrawAt(4, 37, L"早                                                                                         早");
    buffer.DrawAt(4, 38, L"早                                                                                         早");
    buffer.DrawAt(4, 39, L"早                                                                                         早");
    buffer.DrawAt(4, 40, L"早                                                                                         早");
    buffer.DrawAt(4, 41, L"                                                                                           ");
    buffer.DrawAt(4, 42, L"早                                                                                         早");
    buffer.DrawAt(4, 43, L"早                                                                                         早");
    buffer.DrawAt(4, 44, L"早                                                                                         早");
    buffer.DrawAt(4, 45, L"早                                                                                         早");
    buffer.DrawAt(4, 46, L"早                                                                                         早");
    buffer.DrawAt(4, 47, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
    buffer.DrawAt(16, 41, message);
}

void DrawMessageBox(ConsoleBuffer& buffer) {
    buffer.DrawAt(4, 35, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(4, 36, L"早                                                                                         早");
    buffer.DrawAt(4, 37, L"早                                                                                         早");
    buffer.DrawAt(4, 38, L"早                                                                                         早");
    buffer.DrawAt(4, 39, L"早                                                                                         早");
    buffer.DrawAt(4, 40, L"早                                                                                         早");
    buffer.DrawAt(4, 41, L"                                                                                           ");
    buffer.DrawAt(4, 42, L"早                                                                                         早");
    buffer.DrawAt(4, 43, L"早                                                                                         早");
    buffer.DrawAt(4, 44, L"早                                                                                         早");
    buffer.DrawAt(4, 45, L"早                                                                                         早");
    buffer.DrawAt(4, 46, L"早                                                                                         早");
    buffer.DrawAt(4, 47, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawBattleChoiceAttack(ConsoleBuffer& buffer) {
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(5, 31, L"早                      早");
    buffer.DrawAt(5, 32, L"早       ч翕 摹鷗      早");
    buffer.DrawAt(5, 33, L"早                      早");
    buffer.DrawAt(5, 34, L"早 收收收收收收收收收收收收收收收收收收收收 早");
    buffer.DrawAt(5, 35, L"早                      早");
    buffer.DrawAt(5, 36, L"早   Ⅱ 奢問            早");
    buffer.DrawAt(5, 37, L"早                      早");
    buffer.DrawAt(5, 38, L"早    蝶鑒              早");
    buffer.DrawAt(5, 39, L"早                      早");
    buffer.DrawAt(5, 40, L"早    檣漸饜葬          早");
    buffer.DrawAt(5, 41, L"早                      早");
    buffer.DrawAt(5, 42, L"早    紫蜂纂晦          早");
    buffer.DrawAt(5, 43, L"早                      早");
    buffer.DrawAt(5, 44, L"早                      早");
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawBattleChoiceSkill(ConsoleBuffer& buffer) {
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(5, 31, L"早                      早");
    buffer.DrawAt(5, 32, L"早       ч翕 摹鷗      早");
    buffer.DrawAt(5, 33, L"早                      早");
    buffer.DrawAt(5, 34, L"早 收收收收收收收收收收收收收收收收收收收收 早");
    buffer.DrawAt(5, 35, L"早                      早");
    buffer.DrawAt(5, 36, L"早    奢問              早");
    buffer.DrawAt(5, 37, L"早                      早");
    buffer.DrawAt(5, 38, L"早   Ⅱ 蝶鑒            早");
    buffer.DrawAt(5, 39, L"早                      早");
    buffer.DrawAt(5, 40, L"早    檣漸饜葬          早");
    buffer.DrawAt(5, 41, L"早                      早");
    buffer.DrawAt(5, 42, L"早    紫蜂纂晦          早");
    buffer.DrawAt(5, 43, L"早                      早");
    buffer.DrawAt(5, 44, L"早                      早");
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawBattleChoiceInven(ConsoleBuffer& buffer) {
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(5, 31, L"早                      早");
    buffer.DrawAt(5, 32, L"早       ч翕 摹鷗      早");
    buffer.DrawAt(5, 33, L"早                      早");
    buffer.DrawAt(5, 34, L"早 收收收收收收收收收收收收收收收收收收收收 早");
    buffer.DrawAt(5, 35, L"早                      早");
    buffer.DrawAt(5, 36, L"早    奢問              早");
    buffer.DrawAt(5, 37, L"早                      早");
    buffer.DrawAt(5, 38, L"早    蝶鑒              早");
    buffer.DrawAt(5, 39, L"早                      早");
    buffer.DrawAt(5, 40, L"早   Ⅱ 檣漸饜葬        早");
    buffer.DrawAt(5, 41, L"早                      早");
    buffer.DrawAt(5, 42, L"早    紫蜂纂晦          早");
    buffer.DrawAt(5, 43, L"早                      早");
    buffer.DrawAt(5, 44, L"早                      早");
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭");
}

void DrawBattleChoiceRunAway(ConsoleBuffer& buffer) {
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(5, 31, L"早                      早");
    buffer.DrawAt(5, 32, L"早       ч翕 摹鷗      早");
    buffer.DrawAt(5, 33, L"早                      早");
    buffer.DrawAt(5, 34, L"早 收收收收收收收收收收收收收收收收收收收收 早");
    buffer.DrawAt(5, 35, L"早                      早");
    buffer.DrawAt(5, 36, L"早    奢問              早");
    buffer.DrawAt(5, 37, L"早                      早");
    buffer.DrawAt(5, 38, L"早    蝶鑒              早");
    buffer.DrawAt(5, 39, L"早                      早");
    buffer.DrawAt(5, 40, L"早    檣漸饜葬          早");
    buffer.DrawAt(5, 41, L"早                      早");
    buffer.DrawAt(5, 42, L"早   Ⅱ 紫蜂纂晦        早");
    buffer.DrawAt(5, 43, L"早                      早");
    buffer.DrawAt(5, 44, L"早                      早");
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭");
}

void DrawEmptyImage(ConsoleBuffer& buffer) {
    buffer.DrawAt(30, 10, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 12, L"早                                     早");
    buffer.DrawAt(30, 13, L"早                                     早");
    buffer.DrawAt(30, 14, L"早                                     早");
    buffer.DrawAt(30, 15, L"早                                     早");
    buffer.DrawAt(30, 16, L"早                                     早");
    buffer.DrawAt(30, 17, L"早                                     早");
    buffer.DrawAt(30, 18, L"早                                     早");
    buffer.DrawAt(30, 19, L"早                                     早");
    buffer.DrawAt(30, 20, L"早                                     早");
    buffer.DrawAt(30, 21, L"早                                     早");
    buffer.DrawAt(30, 22, L"早                                     早");
    buffer.DrawAt(30, 23, L"早                                     早");
    buffer.DrawAt(30, 24, L"早                                     早");
    buffer.DrawAt(30, 25, L"早                                     早");
    buffer.DrawAt(30, 26, L"早                                     早");
    buffer.DrawAt(30, 27, L"早                                     早");
    buffer.DrawAt(30, 28, L"早                                     早");
    buffer.DrawAt(30, 29, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawSlimeImage(ConsoleBuffer& buffer)
{
    buffer.DrawAt(30, 10, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 11, L"早                                     早");
    buffer.DrawAt(30, 12, L"早                                     早");
    buffer.DrawAt(30, 13, L"早                                     早");
    buffer.DrawAt(30, 14, L"早                                     早");
    buffer.DrawAt(30, 15, L"早                                     早");
    buffer.DrawAt(30, 16, L"早                                     早");
    buffer.DrawAt(30, 17, L"早                 ∞                  早");
    buffer.DrawAt(30, 18, L"早             ∞       ∞             早");
    buffer.DrawAt(30, 19, L"早         ∞               ∞         早");
    buffer.DrawAt(30, 20, L"早       ∞                   ∞       早");
    buffer.DrawAt(30, 21, L"早     ∞                       ∞     早");
    buffer.DrawAt(30, 22, L"早    ∞       ≒         ≒     ∞    早");
    buffer.DrawAt(30, 23, L"早    ∞       ≒         ≒     ∞    早");
    buffer.DrawAt(30, 24, L"早     ∞                       ∞     早");
    buffer.DrawAt(30, 25, L"早      ∞                     ∞      早");
    buffer.DrawAt(30, 26, L"早        ∞                 ∞        早");
    buffer.DrawAt(30, 27, L"早          ∞ ∞ ∞ ∞ ∞ ∞          早");
    buffer.DrawAt(30, 28, L"早                                     早");
    buffer.DrawAt(30, 29, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawSkillList(ConsoleBuffer& buffer)
{
    buffer.DrawAt(30, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 31, L"早                                                                   早");
    buffer.DrawAt(26, 32, L"早        だ檜橫獐                                                   早");
    buffer.DrawAt(30, 33, L"早                                                                   早");
    buffer.DrawAt(30, 34, L"早        ��                                                         早");
    buffer.DrawAt(30, 35, L"早                                                                   早");
    buffer.DrawAt(28, 36, L"早        詭纔螃                                                     早");
    buffer.DrawAt(30, 37, L"早                                                                   早");
    buffer.DrawAt(27, 38, L"早        菴煎陛晦                                                   早");
    buffer.DrawAt(30, 39, L"早                                                                   早");
    buffer.DrawAt(26, 40, L"早                                                                   早");
    buffer.DrawAt(30, 41, L"早                                                                   早");
    buffer.DrawAt(26, 42, L"早                                                                   早");
    buffer.DrawAt(30, 43, L"早                                                                   早");
    buffer.DrawAt(30, 44, L"早                                                                   早");
    buffer.DrawAt(30, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawInvenList(ConsoleBuffer& buffer)
{
    buffer.DrawAt(30, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 31, L"早                                                                   早");
    buffer.DrawAt(26, 32, L"早        羹溘僭擒                                                   早");
    buffer.DrawAt(30, 33, L"早                                                                   早");
    buffer.DrawAt(30, 34, L"早        葆釭僭擒                                                   早");
    buffer.DrawAt(30, 35, L"早                                                                   早");
    buffer.DrawAt(28, 36, L"早        縣葛憮                                                     早");
    buffer.DrawAt(30, 37, L"早                                                                   早");
    buffer.DrawAt(28, 38, L"早        菴煎陛晦                                                   早");
    buffer.DrawAt(30, 39, L"早                                                                   早");
    buffer.DrawAt(25, 40, L"早                                                                   早");
    buffer.DrawAt(30, 41, L"早                                                                   早");
    buffer.DrawAt(26, 42, L"早                                                                   早");
    buffer.DrawAt(30, 43, L"早                                                                   早");
    buffer.DrawAt(30, 44, L"早                                                                   早");
    buffer.DrawAt(30, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void DrawBattleUI(ConsoleBuffer& buffer) {
    buffer.DrawAt(30, 10, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 12, L"早                                     早");
    buffer.DrawAt(30, 13, L"早                                     早");
    buffer.DrawAt(30, 14, L"早                                     早");
    buffer.DrawAt(30, 15, L"早                                     早");
    buffer.DrawAt(30, 16, L"早                                     早");
    buffer.DrawAt(30, 17, L"早                                     早");
    buffer.DrawAt(30, 18, L"早                                     早");
    buffer.DrawAt(30, 19, L"早                                     早");
    buffer.DrawAt(30, 20, L"早                                     早");
    buffer.DrawAt(30, 21, L"早                                     早");
    buffer.DrawAt(30, 22, L"早                                     早");
    buffer.DrawAt(30, 23, L"早                                     早");
    buffer.DrawAt(30, 24, L"早                                     早");
    buffer.DrawAt(30, 25, L"早                                     早");
    buffer.DrawAt(30, 26, L"早                                     早");
    buffer.DrawAt(30, 27, L"早                                     早");
    buffer.DrawAt(30, 28, L"早                                     早");
    buffer.DrawAt(30, 29, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

#pragma region 裘р
//void DrawBuffer()
//{
//    for (size_t y = 0; y < MAP_HEIGHT; y++)
//    {
//        buffer[y] = "";
//        for (size_t x = 0; x < MAP_WIDTH; x++)
//        {
//            if (x == playerX && y == playerY)
//            {
//                buffer[y] += "≠";
//            }
//            else if (map1[y][x] == 1)
//            {
//                buffer[y] += "﹥";
//            }
//            else if (map1[y][x] == 3)
//            {
//                buffer[y] += "Ⅷ";
//            }
//            else if (map1[y][x] == 4)
//            {
//                buffer[y] += "Ⅳ";
//            }
//            else
//            {
//                buffer[y] += "  ";
//            }
//        }
//    }
//}
//void renderBuffer()
//{
//    for (size_t y = 0; y < MAP_WIDTH; y++)
//    {
//        std::cout << buffer[y] << '\n';
//    }
//}
#pragma endregion
void PrintBattleUI();
void PrintBattleUI2();
void PrintKeyboardState();
void PrintMessage();
void printUserInfo();

void gotoxy(int x, int y);

void inputAnyKey()
{
    Manager& manager = manager.getInstance();
    while (true)
    {
        manager.input.Update();
        if (manager.input.GetKeyDown(eKeyCode::Space)) return;
    }
}
void printRightUI2(ConsoleBuffer& buffer)
{
    buffer.DrawAt(104, 0, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    for (size_t i = 1; i < 49; i++)
    {
        buffer.DrawAt(104, i, L"早                                               早");
    }
    buffer.DrawAt(104, 49, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}
void printUserInfo2(ConsoleBuffer& buffer)
{
    Manager& manager = Manager::getInstance();
    buffer.DrawAt(70, 1, L"         [Ы溯檜橫  薑爾]");
    buffer.DrawAt(70, 2, L"收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收");

    wstring wstr(player->GetName().size(), L'\0');
    std::mbstowcs(&wstr[0], player->GetName().c_str(), player->GetName().size());
    buffer.DrawAt(70, 3, L"檜葷 : " + wstr);
    buffer.DrawAt(70, 4, L"羹溘 / 譆渠羹溘 : " + to_wstring(player->GetHp()) + L" / " + to_wstring(player->GetMaxHp()));
    buffer.DrawAt(70, 5, L"葆釭 / 譆渠葆釭 : " + to_wstring(player->GetMp()) + L" / " + to_wstring(player->GetMaxMp()));
    buffer.DrawAt(70, 6, L"奢問溘 : " + to_wstring(player->GetAtk()));
    buffer.DrawAt(70, 7, L"寞橫溘 : " + to_wstring(player->GetDef()));
    buffer.DrawAt(70, 8, L"觼葬じ鏽 �捕� : " + to_wstring(player->GetCriRate()) + L"%");
    buffer.DrawAt(70, 9, L"觼葬じ鏽 等嘐雖 :" + to_wstring(player->GetCriDamage()) + L"%");
    /*buffer.DrawAt(70, 9, L"�裔Ш� :" + to_wstring(player->GetAvoidRate()) + L"%");
    buffer.DrawAt(70, 10, L"樓紫 :" + to_wstring(player->GetSpeed()));*/
    buffer.DrawAt(70, 10, L"模雖旎 :" + to_wstring(player->GetMoney()) + L"埤萄");

    string mapName = manager.map.GetMapName(manager.map.GetcurrenteMap());
    wstring wstr2(mapName.size(), L'\0');
    std::mbstowcs(&wstr2[0], mapName.c_str(),mapName.size());
    buffer.DrawAt(70, 11, L"⑷營 嬪纂 :" + wstr2);
}

void DrawBuffer2(Map* currentMap)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            buffer2[y][x] = currentMap->at(y, x);
        }
    }
}
void renderBuffer2(Map* currentMap)
{
    int startX = playerX - (ViewSizeX / 2);
    int endX = playerX + (ViewSizeX / 2);
    int startY = playerY - (ViewSizeY / 2);
    int endY = playerY + (ViewSizeY / 2);

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // y陛 starty爾棻 觼剪釭 偽堅 endy 爾棻 濛剪釭 偽棻
            if ((startY <= y && y <= endY) && (startX <= x && x <= endX))
            {
                if (x == playerX && y == playerY)
                {
                    std::cout << "≠";
                }
                else if (currentMap->at(y, x) == 1)
                {
                    std::cout << "﹥";
                }
                else if (currentMap->at(y, x) == 3)
                {
                    std::cout << "Ⅷ";
                }
                else if (currentMap->at(y, x) == 4)
                {
                    std::cout << "Ⅳ";
                }
                else if (currentMap->at(y, x) == 5)
                {
                    std::cout << "∪";
                }
                else if (currentMap->at(y, x) == 9)
                {
                    std::cout << "十";
                }
                else
                {
                    std::cout << "  ";
                }
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << endl;
    }
}
void MovePlayer(Map* currentMap)

{
    // (y,x)  up : (-1, +0), down (+1, +0), left(0,+1), right(0,-1)
    int newPosX = playerX;
    int newPosY = playerY;
    Manager& manager = Manager::getInstance();

    if (manager.input.GetKeyDown(eKeyCode::Up)) { newPosY -= 1; }
    else if (manager.input.GetKeyDown(eKeyCode::Down)) { newPosY += 1; }
    else if (manager.input.GetKeyDown(eKeyCode::Left)) { newPosX -= 1; }
    else if (manager.input.GetKeyDown(eKeyCode::Right)) { newPosX += 1; }


    switch (currentMap->at(newPosY, newPosX))
    {
    case (int)EGameObject::PORTAL:
    {
        for (int i = 0; i < manager.map.PortalList.size(); i++)
        {
            if (manager.map.PortalList[i].CurrentMap == manager.map.GetcurrenteMap() &&
                manager.map.PortalList[i].PortalPos == make_pair(newPosY, newPosX))
            {
                manager.map.UnloadMap(manager.map.GetcurrenteMap());
                manager.map.LoadMap(manager.map.PortalList[i].NextMap);
                playerX = manager.map.PortalList[i].PlayerStartPos.first;
                playerY = manager.map.PortalList[i].PlayerStartPos.second;
                break;
            }
        }
        break;
    }
    case (int)EGameObject::MONSTER:
    {
        //檜 跨蝶攪陛 橫雯 跨蝶攪檣雖 橫飩啪 憲嬴撿 ж朝陛?
        //憲 в蹂陛 橈紫煙 虜萇棻. 楠渾戲煎 儅撩ж賊脹棻.
        //避艘擊 陽朝 斜傖 裘 鼻縑憮 雖錶幗萼棻.

     /*   switch (rand() % 3)
        {
        case 0:
            monster = new Monster(MonsterType::SLIME);
            break;
        case 1:
            monster = new Monster(MonsterType::GOBLIN);
            break;
        case 2:
            monster = new Monster(MonsterType::SKELETON);
            break;
        default:
            monster = new Monster(MonsterType::SLIME);
            break;
        }*/
        slime = new Slime();

        ConsoleBuffer buffer(200, 51);  // 夔樂 觼晦蒂 撲薑ц蝗棲棻

        std::wstring currentMessage = L"";
        bool showBattleUI = false;
        bool showMessage = false;

        bool IsdrawSkillList = false;
        bool IsdrawInvenList = false;

        bool bKeyDown = false; // 'B' 酈 殮溘 鼻鷓
        bool mKeyDown = false; // 'M' 酈 殮溘 鼻鷓
        int menuSelect = 1;
        // 1 = attack; 2 = skill 3 = inven; 4 = runawy
        //bool showBattleUI = true;
        while (true)
        {
            manager.input.Update();
            //clearScreen();
            buffer.Clear();
            //DrawSlimeImage(buffer);
            printUserInfo2(buffer);
            DrawSlimeImage(buffer);
            switch (menuSelect)
            {
            case 1:
                DrawBattleChoiceAttack(buffer);
                break;
            case 2:
                DrawBattleChoiceSkill(buffer);
                break;
            case 3:
                DrawBattleChoiceInven(buffer);
                break;
            case 4:
                DrawBattleChoiceRunAway(buffer);
                break;
            default:
                DrawBattleChoiceAttack(buffer);
                break;
            }

            if (manager.input.GetKeyDown(eKeyCode::Down))
            {
                menuSelect++;
                IsdrawSkillList = false;
                IsdrawInvenList = false;
                if (menuSelect == 5)
                {
                    menuSelect = 4;
                }
            }
            if (manager.input.GetKeyDown(eKeyCode::Up))
            {
                menuSelect--;
                IsdrawSkillList = false;
                IsdrawInvenList = false;
                if (menuSelect == 0)
                {
                    menuSelect = 1;
                }
            }

            bool death = false;
            if (manager.input.GetKeyDown(eKeyCode::Space))
            {
                if (menuSelect == 1)
                {
                    int damage;

                    while (true)
                    {
                        manager.input.Initialize();
                        manager.input.Update();
                        damage = player->GetAtk() - slime->GetDef();
                        if (damage <= 0) damage = 0;
                        DrawMessageBox(buffer, L"Ы溯檜橫曖 奢問!                                                               ");
                        buffer.Render();
                        inputAnyKey();

                        DrawMessageBox(buffer, L"跨蝶攪縑啪 " + to_wstring(damage) + L"等嘐雖蒂 殮��!                                                    ");
                        slime->SetHp(slime->GetHp() - damage);
                        buffer.Render();
                        inputAnyKey();

                        if (slime->GetHp() <= 0 || player->GetHp() <= 0)
                        {
                            DrawMessageBox(buffer, L"跨蝶攪蒂 噙楝お葡                                                    ");
                            buffer.Render();
                            inputAnyKey();
                            death = true;
                            break;
                        }

                        DrawMessageBox(buffer, L"                                                                                           ");
                        DrawMessageBox(buffer, L"跨蝶攪 羹溘 " + to_wstring(slime->GetHp()));
                        buffer.Render();
                        inputAnyKey();

                        damage = slime->GetAtk() - player->GetDef();
                        if (damage <= 0) damage = 1;
                        DrawMessageBox(buffer, L"跨蝶攪曖 奢問!                                                                 ");
                        buffer.Render();
                        inputAnyKey();

                        DrawMessageBox(buffer, L"Ы溯檜橫縑啪 " + to_wstring(damage) + L"等嘐雖蒂 殮��!                                                    ");
                        player->SetHp(player ->GetHp() - damage);
                        buffer.Render();
                        inputAnyKey();

                        DrawMessageBox(buffer, L"早                                                                                          ");
                        DrawMessageBox(buffer, L"Ы溯檜橫 羹溘 " + to_wstring(player->GetHp()));
                        buffer.Render();
                        inputAnyKey();
                        break;


                    }
                }
                else if (menuSelect == 2)
                {
                    IsdrawSkillList = true;
                    int skillNum = 0;
                    while (true)
                    {
                        manager.input.Update();
                        if (manager.input.GetKeyDown(eKeyCode::Down))
                        {
                            skillNum++;
                            if (skillNum == 3)
                            {
                                skillNum = 3;
                            }
                        }
                        if (manager.input.GetKeyDown(eKeyCode::Up))
                        {
                            skillNum--;
                            if (skillNum == -1)
                            {
                                skillNum = 0;
                            }
                        }
                        DrawSkillList(buffer);
                        switch (skillNum)
                        {
                        case 0:
                            buffer.DrawAt(33, 32, L"Ⅱ だ檜橫獐");
                            break;
                        case 1:
                            buffer.DrawAt(37, 34, L"Ⅱ ��");
                            break;
                        case 2:
                            buffer.DrawAt(35, 36, L"Ⅱ 詭纔螃");
                            break;
                        case 3:
                            buffer.DrawAt(35, 38, L"Ⅱ 菴煎陛晦");
                            break;
                        default:
                            break;
                        }
                        if (manager.input.GetKeyDown(eKeyCode::Space))
                        {
                            if (skillNum == 0)
                            {

                            }
                            else if (skillNum == 3)
                            {
                                IsdrawSkillList = false;
                                break;
                            }
                        }

                        buffer.Render();
                    }
                }
                else if (menuSelect == 3)
                {
                    IsdrawInvenList = true;
                    int itemNum = 0;
                    while (true)
                    {
                        manager.input.Update();
                        if (manager.input.GetKeyDown(eKeyCode::Down))
                        {
                            itemNum++;
                            if (itemNum == 4)
                            {
                                itemNum = 3;
                            }
                        }
                        if (manager.input.GetKeyDown(eKeyCode::Up))
                        {
                            itemNum--;
                            if (itemNum == -1)
                            {
                                itemNum = 0;
                            }
                        }
                        DrawInvenList(buffer);
                        switch (itemNum)
                        {
                        case 0:
                            buffer.DrawAt(33, 32, L"Ⅱ 羹溘僭擒");
                            break;
                        case 1:
                            buffer.DrawAt(37, 34, L"Ⅱ 葆釭僭擒");
                            break;
                        case 2:
                            buffer.DrawAt(35, 36, L"Ⅱ 縣葛憮");
                            break;
                        case 3:
                            buffer.DrawAt(35, 38, L"Ⅱ 菴煎陛晦");
                            break;
                        default:
                            break;
                        }
                        if (manager.input.GetKeyDown(eKeyCode::Space))
                        {
                            if (itemNum == 3)
                            {
                                IsdrawInvenList = false;
                                break;
                            }
                        }


                        buffer.Render();
                    }
                }
                else if (menuSelect == 4)
                {
                    //TODO 紫蜂, 褒ぬ衛 跨蝶攪縑啪 и欐 蜃堅, 濠褐欐 陳嬴馬
                    break;
                }
                if (death == true)
                {
                    currentMap->at(newPosY, newPosX) = 0;
                    break;
                }
            }

            if (IsdrawSkillList == true)
            {
                DrawSkillList(buffer);
            }

            if (IsdrawInvenList == true)
            {
                DrawInvenList(buffer);
            }


            buffer.Render();

#pragma region MyRegion
            //buffer.Clear();

            //DrawBattleUI(buffer);
            ////DrawMessageBox(buffer, currentMessage);

            //buffer.Render();

            ////// 餌辨濠 殮溘 籀葬
            ////if (GetAsyncKeyState('B') & 0x8000) {
            ////    if (!bKeyDown) {
            ////        showBattleUI = !showBattleUI;
            ////        bKeyDown = true;  // 酈陛 揚溜擠擊 晦煙
            ////    }
            ////}
            ////else {
            ////    bKeyDown = false; // 酈陛 飲橫螺擊 陽 鼻鷓 蟾晦��
            ////}

            ////if (GetAsyncKeyState('M') & 0x8000) {
            ////    if (!mKeyDown) {
            ////        showMessage = !showMessage;
            ////        //if (showMessage) {
            ////        //    currentMessage = L"嬴檜蠱擊(蒂) �僱磈牳抻懂炴�.";  // 嶸棲囀萄 僥濠翮煎 滲唳
            ////        //}
            ////        mKeyDown = true;  // 酈陛 揚溜擠擊 晦煙
            ////    }
            ////}
            ////else {
            ////    mKeyDown = false;  // 酈陛 飲橫螺擊 陽 鼻鷓 蟾晦��
            ////}

            ////Sleep(100);  // CPU 餌辨榆 馬模蒂 嬪и 穠擎 渠晦 衛除
            //if (GetAsyncKeyState('M') & 0x8000)
            //{
            //    break;
            //}
#pragma endregion

        }


        break;
    }
    case (int)EGameObject::EMPTY:
    {
        playerX = newPosX;
        playerY = newPosY;
        break;
    }

    default:
        break;
    }
}

void CursorView()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1; //醴憮 掃晦 (1 ~ 100)
    cursorInfo.bVisible = FALSE; //醴憮 Visible TRUE(爾歜) FALSE(獗梯)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void clearScreen()
{
    // 醴憮蒂 裔 嬪煎 檜翕
    std::cout << "\033[H";
}

void gotoxy(int x, int y)
{
    HANDLE consolHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(consolHandle, pos);
}


void printRightUI()
{
    gotoxy(104, 0);
    std::cout << "旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬";
    for (size_t i = 1; i < 49; i++)
    {
        gotoxy(104, i);
        std::cout << "早                                               早";
    }
    gotoxy(104, 49);
    std::cout << "曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭";
    //clearScreen();
}
void printUserInfo()
{
    Manager& manager = Manager::getInstance();
    gotoxy(106, 5);
    std::cout << "              [Ы溯檜橫  薑爾]" << endl;
    gotoxy(110, 6);
    std::cout << "收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收" << endl;
    
    gotoxy(110, 7);
    std::cout << "檜葷 : " << player->GetName() << std::endl;
    gotoxy(110, 8);
    std::cout << "羹溘 / 譆渠羹溘 : " << player->GetHp() << " / " << player->GetMaxHp() << std::endl;
    gotoxy(110, 9);
    std::cout << "葆釭 / 譆渠葆釭 : " << player->GetMp() << " / " << player->GetMaxMp() << std::endl;
    gotoxy(110, 10);
    std::cout << "奢問溘 : " << player->GetAtk() << std::endl;
    gotoxy(110, 11);
    std::cout << "寞橫溘 : " << player->GetDef() << std::endl;
    gotoxy(110, 12);
    std::cout << "觼葬じ鏽 �捕� : " << player->GetCriRate() << std::endl;
    gotoxy(110, 13);
    std::cout << "觼葬じ鏽 等嘐雖 : " << player->GetCriDamage() << std::endl;
    gotoxy(110, 14);
    std::cout << "�裔Ш� : " << player->GetAvoidRate() << std::endl;
    gotoxy(110, 15);
    std::cout << "樓紫 : " << player->GetSpeed() << std::endl;
    gotoxy(110, 16);
    std::cout << "模雖旎 : " << player->GetMoney() << std::endl;
    gotoxy(110, 17);
    std::cout << "⑷營 嬪纂 : " << manager.map.GetMapName(manager.map.GetcurrenteMap()) << std::endl;
    gotoxy(110, 18);
    std::cout << "蝶鑒 : " << "蝶鑒" << std::endl;
}
void PrintKeyboardState()
{
    Manager& manager = Manager::getInstance();
    
    gotoxy(106, 35);
    std::cout << "                [酈爾萄  殮溘]";
    gotoxy(110, 36);
    std::cout << "收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收" << endl;
#pragma region DirectionKeys
    gotoxy(110, 38);
    std::cout << "寞щ酈 : " << endl;
    gotoxy(122, 37);
    if (manager.input.GetKeyDown(eKeyCode::Up) || manager.input.GetKey(eKeyCode::Up))
    {
        std::cout << "﹥" << endl;
    }
    else
    {
        std::cout << "﹤" << endl;
    }
    gotoxy(120, 38);
    if (manager.input.GetKeyDown(eKeyCode::Left) || manager.input.GetKey(eKeyCode::Left))
    {
        std::cout << "﹥" << endl;
    }
    else
    {
        std::cout << "﹤" << endl;
    }
    gotoxy(122, 38);
    if (manager.input.GetKeyDown(eKeyCode::Down) || manager.input.GetKey(eKeyCode::Down))
    {
        std::cout << "﹥" << endl;
    }
    else
    {
        std::cout << "﹤" << endl;
    }
    gotoxy(124, 38);
    if (manager.input.GetKeyDown(eKeyCode::Right) || manager.input.GetKey(eKeyCode::Right))
    {
        std::cout << "﹥" << endl;
    }
    else
    {
        std::cout << "﹤" << endl;
    }

    gotoxy(128, 38);
    std::cout << "蝶む檜蝶 : " << endl;
    gotoxy(140, 38);
    if (manager.input.GetKeyDown(eKeyCode::Space) || manager.input.GetKey(eKeyCode::Space))
    {
        std::cout << "﹥﹥﹥﹥" << endl;
    }
    else
    {
        std::cout << "﹤﹤﹤﹤" << endl;
    }
#pragma endregion

#pragma region KeyBoardKeys
    gotoxy(110, 40);
    if (manager.input.GetKeyDown(eKeyCode::Q) || manager.input.GetKey(eKeyCode::Q)) std::cout << "≒";
    else std::cout << "侑";
    gotoxy(112, 40);
    if (manager.input.GetKeyDown(eKeyCode::W) || manager.input.GetKey(eKeyCode::W)) std::cout << "≒";
    else std::cout << "具";
    gotoxy(114, 40);
    if (manager.input.GetKeyDown(eKeyCode::E) || manager.input.GetKey(eKeyCode::E)) std::cout << "≒";
    else std::cout << "供";
    gotoxy(116, 40);
    if (manager.input.GetKeyDown(eKeyCode::R) || manager.input.GetKey(eKeyCode::R)) std::cout << "≒";
    else std::cout << "佺";
    gotoxy(118, 40);
    if (manager.input.GetKeyDown(eKeyCode::T) || manager.input.GetKey(eKeyCode::T)) std::cout << "≒";
    else std::cout << "兒";
    gotoxy(120, 40);
    if (manager.input.GetKeyDown(eKeyCode::Y) || manager.input.GetKey(eKeyCode::Y)) std::cout << "≒";
    else std::cout << "典";
    gotoxy(122, 40);
    if (manager.input.GetKeyDown(eKeyCode::U) || manager.input.GetKey(eKeyCode::U)) std::cout << "≒";
    else std::cout << "兕";
    gotoxy(124, 40);
    if (manager.input.GetKeyDown(eKeyCode::I) || manager.input.GetKey(eKeyCode::I)) std::cout << "≒";
    else std::cout << "佰";
    gotoxy(126, 40);
    if (manager.input.GetKeyDown(eKeyCode::O) || manager.input.GetKey(eKeyCode::O)) std::cout << "≒";
    else std::cout << "侑";
    gotoxy(128, 40);
    if (manager.input.GetKeyDown(eKeyCode::P) || manager.input.GetKey(eKeyCode::P)) std::cout << "≒";
    else std::cout << "侏";
    
    gotoxy(110, 41);
    if (manager.input.GetKeyDown(eKeyCode::A) || manager.input.GetKey(eKeyCode::A)) std::cout << "≒";
    else std::cout << "侍";
    gotoxy(112, 41);
    if (manager.input.GetKeyDown(eKeyCode::S) || manager.input.GetKey(eKeyCode::S)) std::cout << "≒";
    else std::cout << "兔";
    gotoxy(114, 41);
    if (manager.input.GetKeyDown(eKeyCode::D) || manager.input.GetKey(eKeyCode::D)) std::cout << "≒";
    else std::cout << "佬";
    gotoxy(116, 41);
    if (manager.input.GetKeyDown(eKeyCode::F) || manager.input.GetKey(eKeyCode::F)) std::cout << "≒";
    else std::cout << "例";
    gotoxy(118, 41);
    if (manager.input.GetKeyDown(eKeyCode::G) || manager.input.GetKey(eKeyCode::G)) std::cout << "≒";
    else std::cout << "來";
    gotoxy(120, 41);
    if (manager.input.GetKeyDown(eKeyCode::H) || manager.input.GetKey(eKeyCode::H)) std::cout << "≒";
    else std::cout << "侃";
    gotoxy(122, 41);
    if (manager.input.GetKeyDown(eKeyCode::J) || manager.input.GetKey(eKeyCode::J)) std::cout << "≒";
    else std::cout << "併";
    gotoxy(124, 41);
    if (manager.input.GetKeyDown(eKeyCode::K) || manager.input.GetKey(eKeyCode::K)) std::cout << "≒";
    else std::cout << "侈";
    gotoxy(126, 41);
    if (manager.input.GetKeyDown(eKeyCode::L) || manager.input.GetKey(eKeyCode::L)) std::cout << "≒";
    else std::cout << "佩";

    gotoxy(110, 42);
    if (manager.input.GetKeyDown(eKeyCode::Z) || manager.input.GetKey(eKeyCode::Z)) std::cout << "≒";
    else std::cout << "冽";
    gotoxy(112, 42);
    if (manager.input.GetKeyDown(eKeyCode::X) || manager.input.GetKey(eKeyCode::X)) std::cout << "≒";
    else std::cout << "其";
    gotoxy(114, 42);
    if (manager.input.GetKeyDown(eKeyCode::C) || manager.input.GetKey(eKeyCode::C)) std::cout << "≒";
    else std::cout << "使";
    gotoxy(116, 42);
    if (manager.input.GetKeyDown(eKeyCode::V) || manager.input.GetKey(eKeyCode::V)) std::cout << "≒";
    else std::cout << "兩";
    gotoxy(118, 42);
    if (manager.input.GetKeyDown(eKeyCode::B) || manager.input.GetKey(eKeyCode::B)) std::cout << "≒";
    else std::cout << "佳";
    gotoxy(120, 42);
    if (manager.input.GetKeyDown(eKeyCode::N) || manager.input.GetKey(eKeyCode::N)) std::cout << "≒";
    else std::cout << "侖";
    gotoxy(122, 42);
    if (manager.input.GetKeyDown(eKeyCode::M) || manager.input.GetKey(eKeyCode::M)) std::cout << "≒";
    else std::cout << "佻";
    


#pragma endregion
}
void PrintMessage() //TODO
{
    
    gotoxy(4, 35);
    std::cout << "旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬";
    for (size_t i = 36; i < 47; i++)
    {
        gotoxy(4, i);
        std::cout << "早                                                                                         早";
    }
    gotoxy(4, 47);
    std::cout << "曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭";

    gotoxy(16, 41);
    std::cout << "嬴檜蠱 擊(蒂) �僱磈牳抻懂炴�.";
}
void PrintBattleUI()
{
    gotoxy(30, 10); 
    std::cout << "旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬";
    for (size_t i = 11; i < 29; i++)
    {
        gotoxy(30, i);
        std::cout << "早                                     早";
    }
    gotoxy(30, 29);
    std::cout << "曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭";


    gotoxy(5, 30);
    std::cout << "旨收收收收收收收收收收收收收收收收收收收收收收旬";
    for (size_t i = 31; i < 45; i++)
    {
        gotoxy(5, i);
        std::cout << "早                      早";
    }
    gotoxy(13, 32);
    std::cout << "ч翕 摹鷗";
    gotoxy(6, 34);
    std::cout << " 收收收收收收收收收收收收收收收收收收收收 ";
    gotoxy(8, 36);
    std::cout << "Ⅱ奢問";
    gotoxy(10, 38);
    std::cout << "蝶鑒";
    gotoxy(10, 40);
    std::cout << "檣漸饜葬";
    gotoxy(10, 42);
    std::cout << "紫蜂纂晦";
    gotoxy(5, 45);
    std::cout << "曲收收收收收收收收收收收收收收收收收收收收收收旭";
}
void PrintBattleUI2()
{
    gotoxy(5, 30);
    std::cout << "旨收收收收收收收收收收收收收收收收收收收收收收旬";
    for (size_t i = 31; i < 45; i++)
    {
        gotoxy(5, i);
        std::cout << "早                      早";
    }
    gotoxy(13, 32);
    std::cout << "ч翕 摹鷗";
    gotoxy(6, 34);
    std::cout << " 收收收收收收收收收收收收收收收收收收收收 ";
    gotoxy(10, 36);
    std::cout << "奢問";
    gotoxy(10, 38);
    std::cout << "蝶鑒";
    gotoxy(10, 40);
    std::cout << "檣漸饜葬";
    gotoxy(10, 42);
    std::cout << "紫蜂纂晦";
    gotoxy(5, 45);
    std::cout << "曲收收收收收收收收收收收收收收收收收收收收收收旭";
}

int main()
{
    setlocale(LC_ALL, "korean");
    _wsetlocale(LC_ALL, L"korean");
    srand((unsigned)time(NULL));
    system("mode con:cols=160 lines=51");
    CursorView();
    Manager& manager = Manager::getInstance();
    manager.input.Initialize();
    manager.map.Initialize();
    manager.map.LoadMap(eMaps::Village);

    //ConsoleBuffer buffer(200, 200);


    Map* currentmap;
    while (true)
    {
        clearScreen();
        manager.input.Update();
        currentmap = manager.map.GetCurrentMap();
        DrawBuffer2(currentmap);
        renderBuffer2(currentmap);
        MovePlayer(currentmap);
        
        printRightUI();
        printUserInfo();
        PrintKeyboardState();
        clearScreen();
    }
}