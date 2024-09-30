#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

class ConsoleBuffer {
private:
    std::vector<std::vector<wchar_t>> buffer;  // 諦檜萄 僥濠翮 餌辨
    int width, height;
    HANDLE hConsole;

public:
    ConsoleBuffer(int w, int h) : width(w), height(h), buffer(h, std::vector<wchar_t>(w, L' ')) 
    {
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

    void Render() {
        COORD topLeft = { 0, 0 };
        DWORD written;

        for (const auto& row : buffer) {
            WriteConsoleOutputCharacterW(hConsole, row.data(), width, topLeft, &written);  // 嶸棲囀萄 幗瞪 餌辨
            topLeft.Y++;
        }
    }
};

void DrawMessageBox(ConsoleBuffer& buffer, const std::wstring& message) {
    buffer.DrawAt(4, 35, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    for (int i = 36; i < 47; ++i) {
        buffer.DrawAt(4, i, L"早                                                                                         早");
    }
    buffer.DrawAt(4, 47, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");

    buffer.DrawAt(16, 41, message);
}

void DrawSkillList(ConsoleBuffer& buffer) 
{
    buffer.DrawAt(30, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");
    buffer.DrawAt(30, 31, L"早                                                                   早");
    buffer.DrawAt(26, 32, L"早        だ檜橫獐                                                    早");
    buffer.DrawAt(30, 33, L"早                                                                   早");
    buffer.DrawAt(30, 34, L"早        ��                                                         早");
    buffer.DrawAt(30, 35, L"早                                                                   早");
    buffer.DrawAt(30, 36, L"早        詭纔螃                                                      早");
    buffer.DrawAt(30, 37, L"早                                                                   早");
    buffer.DrawAt(30, 38, L"早        菴煎陛晦                                                    早");
    buffer.DrawAt(30, 39, L"早                                                                   早");
    buffer.DrawAt(30, 40, L"早                                                                   早");
    buffer.DrawAt(30, 41, L"早                                                                   早");
    buffer.DrawAt(30, 42, L"早                                                                   早");
    buffer.DrawAt(30, 43, L"早                                                                   早");
    buffer.DrawAt(30, 44, L"早                                                                   早");
    buffer.DrawAt(30, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭");
}

void DrawBattleChoiceUI(ConsoleBuffer& buffer) {
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬 旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 ");
    buffer.DrawAt(5, 31, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 32, L"早       ч翕 摹鷗      早 早                                                                   早");
    buffer.DrawAt(5, 33, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 34, L"早 收收收收收收收收收收收收收收收收收收收收 早 早                                                                   早 ");
    buffer.DrawAt(5, 35, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 36, L"早 Ⅱ 奢問              早 早                                                                   早");
    buffer.DrawAt(5, 37, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 38, L"早    蝶鑒              早 早                                                                   早 ");
    buffer.DrawAt(5, 39, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 40, L"早    檣漸饜葬          早 早                                                                   早");
    buffer.DrawAt(5, 41, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 42, L"早    紫蜂纂晦          早 早                                                                   早");
    buffer.DrawAt(5, 43, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 44, L"早                      早 早                                                                   早 ");
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭 曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 ");
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

    //buffer.DrawAt(13, 32, L"ч翕 摹鷗");
    //buffer.DrawAt(6, 34, L" 收收收收收收收收收收收收收收收收收收收收 ");
    //buffer.DrawAt(10, 36, L"奢問");
    //buffer.DrawAt(10, 38, L"蝶鑒");
    //buffer.DrawAt(10, 40, L"檣漸饜葬");
    //buffer.DrawAt(10, 42, L"紫蜂纂晦");
    
    buffer.DrawAt(5, 30, L"旨收收收收收收收收收收收收收收收收收收收收收收旬");
    for (int i = 31; i < 45; ++i) {
        buffer.DrawAt(5, i, L"早                      早");
    }
    buffer.DrawAt(5, 45, L"曲收收收收收收收收收收收收收收收收收收收收收收旭");
}

int main() {
    ConsoleBuffer buffer(100, 50);  // 夔樂 觼晦蒂 撲薑ц蝗棲棻

    bool showBattleUI = true;
    bool showMessage = false;
    std::wstring currentMessage = L"";

    bool bKeyDown = false; // 'B' 酈 殮溘 鼻鷓
    bool mKeyDown = false; // 'M' 酈 殮溘 鼻鷓

    while (true) {
        buffer.Clear();
        DrawSlimeImage(buffer);
        DrawBattleChoiceUI(buffer);
        //DrawSkillList(buffer);
        buffer.Render();
        //if (showBattleUI) {
        //    DrawBattleUI(buffer);
        //}

        //if (showMessage) {
        //    DrawMessageBox(buffer, currentMessage);
        //}

        //buffer.Render();

        //// 餌辨濠 殮溘 籀葬
        //if (GetAsyncKeyState('B') & 0x8000) {
        //    if (!bKeyDown) {
        //        showBattleUI = !showBattleUI;
        //        bKeyDown = true;  // 酈陛 揚溜擠擊 晦煙
        //    }
        //}
        //else {
        //    bKeyDown = false; // 酈陛 飲橫螺擊 陽 鼻鷓 蟾晦��
        //}

        //if (GetAsyncKeyState('M') & 0x8000) {
        //    if (!mKeyDown) {
        //        showMessage = !showMessage;
        //        //if (showMessage) {
        //        //    currentMessage = L"嬴檜蠱擊(蒂) �僱磈牳抻懂炴�.";  // 嶸棲囀萄 僥濠翮煎 滲唳
        //        //}
        //        mKeyDown = true;  // 酈陛 揚溜擠擊 晦煙
        //    }
        //}
        //else {
        //    mKeyDown = false;  // 酈陛 飲橫螺擊 陽 鼻鷓 蟾晦��
        //}

        //Sleep(100);  // CPU 餌辨榆 馬模蒂 嬪и 穠擎 渠晦 衛除
    }

    return 0;
}