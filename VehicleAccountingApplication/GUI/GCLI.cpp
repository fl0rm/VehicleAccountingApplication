#include "GCLI.h"

#include <windows.h>
#include <math.h>
#include <sstream>

#include "../ExternalDependencies.h"

GCLI::GCLI() : choice_{}, visible_ {}, width_{}, height_{}, lang_pack_{} {
    //------------------------------
    // Параметры консольного окна
    hconsole_ = GetStdHandle(STD_OUTPUT_HANDLE);

    is_validity_descriptor_ = true;

    if (hconsole_ == INVALID_HANDLE_VALUE) is_validity_descriptor_ = false;

    if (!GetConsoleScreenBufferInfo(hconsole_, &csbi_)) is_validity_descriptor_ = false;

    if (is_validity_descriptor_) {

        COORD new_screen_buffer_size{};
        SMALL_RECT WinSize;

        WinSize = csbi_.srWindow;
        WinSize.Right += 2;

        new_screen_buffer_size.X = csbi_.srWindow.Right - csbi_.srWindow.Left + 1; 
        new_screen_buffer_size.Y = csbi_.srWindow.Bottom - csbi_.srWindow.Top + 1; 

        SetConsoleScreenBufferSize(hconsole_, new_screen_buffer_size);
        SetConsoleWindowInfo(hconsole_, true, &WinSize);
        SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

        width_ = new_screen_buffer_size.X;
        height_ = 10;
    }

    key_[ActiveWindow::Table] = 'A';
    key_[ActiveWindow::Edit] = 'E';
    key_[ActiveWindow::Delete] = 'D';
    key_[ActiveWindow::Select] = 'C';
    key_[ActiveWindow::Sorting] = 'S';
    
    menu_points_.first.resize(3);
    menu_points_.second.resize(5);
    menu_points_.first[0] = "New Table";
    menu_points_.first[1] = "Open File";
    menu_points_.first[2] = "Exit";
    menu_points_.second[0] = "New Table";
    menu_points_.second[1] = "Open File";
    menu_points_.second[2] = "Save File";
    menu_points_.second[3] = "Back";
    menu_points_.second[4] = "Exit";

    //------------------------------
    // Загрузка ресурсов 
    
    if (!file_processor_.LoadLangPack(lang_pack_)) {
        table_.SetLang(lang_pack_);
        key_ = lang_pack_.key_;
        menu_points_ = lang_pack_.menu_;
    }
        

    file_processor_.SetSize(width_, 15);

    //------------------------------
    /*/ Установка отступов для колонок
    
    
    id_size_ = 7;
    brand_size_ = 14;
    model_size_ = 14;
    weight_size_ = 14;


    /**
    id_size_ = log10(fabs(table_.back().GetId())) + 1;

    brand_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (brand_size_ < table_[i].GetBrand().length()) brand_size_ = table_[i].GetBrand().length();
    }

    model_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (model_size_ < table_[i].GetModel().length()) model_size_ = table_[i].GetModel().length();
    }

    weight_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (weight_size_ < table_[i].GetWeight()) weight_size_ = table_[i].GetWeight();
    }
    weight_size_ = log10(weight_size_) + 1;
    */
}


//------------------------------
// Очистка консоли 
//------------------------------
void GCLI::ClearConsole() const{
    
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };
    
    cellCount = csbi_.dwSize.X * csbi_.dwSize.Y;

    if (!FillConsoleOutputCharacter(hconsole_, ' ', cellCount, homeCoords, &count)) return;
    if (!FillConsoleOutputAttribute(hconsole_, csbi_.wAttributes, cellCount, homeCoords, &count)) return;

    SetConsoleCursorPosition(hconsole_, homeCoords);
}
 

//------------------------------
// Главное меню
//------------------------------
void GCLI::MainMenu() {
    std::ostringstream os;
    std::string separation(width_ - 1, '-');
    std::vector<std::string> menu_points;

    os << separation << std::endl;

    if (table_.size_of_pure_table() > 0) menu_points = menu_points_.second;
    else menu_points = menu_points_.first;
    
    for (int8_t i = 0; i < menu_points.size(); i++) {
        if (i == choice_) os << "-> ";
        else os << "- ";
        os << menu_points[i] << std::endl;
    }
    os << separation << std::endl;

    std::cout << os.str();
}


//------------------------------
//  Обработка событий
//------------------------------
void GCLI::Start() {
    bool live = true;

    file_processor_.SetTitle(ActiveWindow::Menu);
    MainMenu();

    while (live) {

        switch (visible_)
        {
        case ActiveWindow::Menu:

            // Навигация в меню с помощью стрелок
            if ( input_.isKeyPressedOnce(VK_DOWN) && (choice_ < 4) && (choice_ < 2 || table_.size_of_pure_table() > 0)) { choice_++; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce(VK_UP) && (choice_ > 0)) { choice_--; ClearConsole(); MainMenu(); }

            // Навигация в меню с помощью цифр
            if ( input_.isKeyPressedOnce('1')) { choice_ = 0; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('2')) { choice_ = 1; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('3')) { choice_ = 2; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('4') && (table_.size_of_pure_table() > 0)) { choice_ = 3; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('5') && (table_.size_of_pure_table() > 0)) { choice_ = 4; ClearConsole(); MainMenu(); }

            // Подтверждение выбора
            if ( input_.isKeyPressedOnce(VK_RETURN)) {

                switch (choice_)
                {
                case 0:
                    visible_ = ActiveWindow::Table;
                    ClearConsole();

                    file_processor_.SetTitle(ActiveWindow::Table);
                    table_.clear();
                    table_.RenderingTheTable(visible_);
                    break;
                    
                case 1:
                    visible_ = ActiveWindow::Open;
                    ClearConsole();
                    
                    file_processor_.SetTitle(ActiveWindow::Open);
                    file_processor_.RenderingFileSelector();
                    break;
                case 2:
                    if (table_.size_of_pure_table() > 0) { visible_ = ActiveWindow::Save; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Save); file_processor_.RenderingFileSelector(); }
                    else live = false;
                    break;
                case 3:
                    visible_ = ActiveWindow::Table;
                    ClearConsole();

                    file_processor_.SetTitle(ActiveWindow::Table);
                    table_.RenderingTheTable(visible_);
                    break;
                case 4:
                    live = false;
                    break;
                default:
                    break;
                }
            }
            break;
        case ActiveWindow::Table:

            // Переключение страниц таблицы
            if (input_.isKeyPressedOnce(VK_LEFT)) { table_.PreviosPage(); ClearConsole(); table_.RenderingTheTable(visible_); }
            if (input_.isKeyPressedOnce(VK_RIGHT)) { table_.NextPage(); ClearConsole(); table_.RenderingTheTable(visible_); }

            // Добавление эллемента
            if (input_.isKeyPressedOnce(key_[ActiveWindow::Table])) { table_.AddElement(); visible_ = ActiveWindow::Edit; ClearConsole(); file_processor_.FileChanged(); file_processor_.SetTitle(ActiveWindow::Edit); table_.RenderingTheTable(ActiveWindow::Edit); }

            // Переход в режим выбора
            if (input_.isKeyPressedOnce(key_[ActiveWindow::Select])) { table_.SetCursor(); visible_ = ActiveWindow::Select; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Select); table_.RenderingTheTable(visible_); }

            // Сортировка элементов
            if (input_.isKeyPressedOnce(key_[ActiveWindow::Sorting])) { table_.SetColumn(); visible_ = ActiveWindow::Sorting; ClearConsole(); table_.RenderingTheTable(ActiveWindow::Table); }
            
            // Выход в главное меню
            if ( input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Menu; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Menu); MainMenu(); }

            break;
        case ActiveWindow::Save:

            // Навигация в меню с помощью стрелок
            if (input_.isKeyPressedOnce(VK_DOWN)) { file_processor_.MoveCursorDown(); ClearConsole(); file_processor_.RenderingFileSelector(); }
            if (input_.isKeyPressedOnce(VK_UP)) { file_processor_.MoveCursorUp(); ClearConsole(); file_processor_.RenderingFileSelector(); }

            // Подтверждение выбора
            if (input_.isKeyPressedOnce(VK_RETURN)) {
                if (file_processor_.SelectWritePosition(table_.GetTable(), hconsole_)) {
                    ClearConsole();

                    file_processor_.RenderingFileSelector();
                }
                else {
                    visible_ = ActiveWindow::Table;
                    ClearConsole();

                    file_processor_.SetTitle(ActiveWindow::Table);
                    table_.RenderingTheTable(visible_);
                }
            }

            // Выход в главное меню
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Menu; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Menu); MainMenu(); }

            break;
        case ActiveWindow::Open:

            // Переключение страниц
            if (input_.isKeyPressedOnce(VK_LEFT)) { file_processor_.PreviosPage(); ClearConsole(); file_processor_.RenderingFileSelector(); }
            if (input_.isKeyPressedOnce(VK_RIGHT)) { file_processor_.NextPage(); ClearConsole(); file_processor_.RenderingFileSelector(); }

            // Навигация в меню с помощью стрелок
            if (input_.isKeyPressedOnce(VK_DOWN) ) { file_processor_.MoveCursorDown(); ClearConsole(); file_processor_.RenderingFileSelector(); }
            if (input_.isKeyPressedOnce(VK_UP) ) { file_processor_.MoveCursorUp(); ClearConsole(); file_processor_.RenderingFileSelector(); }

            // Подтверждение выбора
            if (input_.isKeyPressedOnce(VK_RETURN)) { 
                vec_Vehicle vec;
                

                if (file_processor_.SelectReadPosition(vec, hconsole_)) {
                    ClearConsole();

                    file_processor_.RenderingFileSelector();
                }
                else{
                    visible_ = ActiveWindow::Table;
                    ClearConsole();

                    table_.SetTable(vec);

                    file_processor_.SetTitle(ActiveWindow::Table);
                    table_.RenderingTheTable(visible_);
                }

                for (auto i : vec)
                    delete i;
            }

            // Выход в главное меню
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Menu; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Menu); MainMenu(); }

            break;
        case ActiveWindow::Select:

            // Навигация по таблице с помощью стрелок
            if (input_.isKeyPressedOnce(VK_DOWN)) { table_.SelectElemDown(); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Select); }
            if (input_.isKeyPressedOnce(VK_UP) ) { table_.SelectElemUp(); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Select ); }

            if (input_.isKeyPressedOnce(VK_RETURN)) { ClearConsole(); table_.RenderingTheLine(); }

            // Редактирование эллемента
            if (input_.isKeyPressedOnce(key_[ActiveWindow::Edit])) { visible_ = ActiveWindow::Edit; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Edit); table_.RenderingTheTable(ActiveWindow::Edit); }

            // Удаление элемента
            if (input_.isKeyPressedOnce(key_[ActiveWindow::Delete])) { table_.DeleteElement(); visible_ = ActiveWindow::Table; ClearConsole(); file_processor_.FileChanged(); file_processor_.SetTitle(ActiveWindow::Table); table_.RenderingTheTable(visible_); }

            // Выход в главное меню
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Table; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Table); table_.RenderingTheTable(visible_); }

            break;
        case ActiveWindow::Edit:

            table_.SelectionAnimation(hconsole_, visible_);

            // Навигация по таблице с помощью стрелок
            if (input_.isKeyPressedOnce(VK_LEFT)) { table_.PreviosColumn(visible_); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Edit); }
            if (input_.isKeyPressedOnce(VK_RIGHT)) { table_.NextColumn(visible_); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Edit); }

            // Подтверждение выбора
            if (input_.isKeyPressedOnce(VK_RETURN)) { table_.EditElement(hconsole_, input_); ClearConsole(); file_processor_.FileChanged(); table_.RenderingTheTable(ActiveWindow::Edit); }

            // Выход из режима редактирования 
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Table; ClearConsole(); file_processor_.SetTitle(ActiveWindow::Table); table_.RenderingTheTable(visible_); }

            break;
        case ActiveWindow::Sorting:

            table_.SelectionAnimation(hconsole_, visible_);

            // Навигация по таблице с помощью стрелок
            if (input_.isKeyPressedOnce(VK_LEFT)) { table_.PreviosColumn(visible_); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Table); }
            if (input_.isKeyPressedOnce(VK_RIGHT)) { table_.NextColumn(visible_); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Table); }

            // Подтверждение выбора
            if (input_.isKeyPressedOnce(VK_RETURN)) { table_.SortingTable(); ClearConsole(); table_.RenderingTheTable(ActiveWindow::Table); }

            // Выход из режима редактирования 
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = ActiveWindow::Table; ClearConsole(); file_processor_.SetTitle(visible_); table_.RenderingTheTable(visible_); }

            break;
        default:
            break;
        }
    }
}