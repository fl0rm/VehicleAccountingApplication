
#include "FileProcessorInterface.h"

FileProcessorInterface::FileProcessorInterface() : choice_{}, page_{} {
	width_ = 120;
	row_count_ = 15;

    status_[ActiveWindow::Menu] = "Main menu";
    status_[ActiveWindow::Table] = "Viewing";
    status_[ActiveWindow::Save] = "Save menu";
    status_[ActiveWindow::Open] = "Open menu";
    status_[ActiveWindow::Select] = "Select";
    status_[ActiveWindow::Edit] = "Edit";
}

void FileProcessorInterface::SetSize(uint16_t width, uint16_t row_count) {
    width_ = width;
    row_count_ = row_count;
}

void FileProcessorInterface::SetTitle(ActiveWindow vis) {
    SetConsoleTitleA((file_processor_ + status_[vis]).c_str());
}

bool FileProcessorInterface::LoadLangPack(LangStruct& pack) {
    bool answer = file_processor_.LoadLangPack(pack);
    if (!answer)
        this->status_ = pack.status_;
    
    return answer;
}

void FileProcessorInterface::RenderingFileSelector() {
    std::ostringstream os;
    std::string path;
    std::string separation(width_, '-');
    std::string page_offset((width_ - std::to_string(page_).length()) / 2, ' ');


    file_processor_.GetFilesInDir(all_dirs_);
    file_processor_.GetFilesInDir(all_files_, ".tbl");
    path = file_processor_.GetPath();

    os << separation << std::endl;

    // Путь к файлу
    if (choice_ == 1)
        os << "->..." << std::endl;
    else
        os << "..." << std::endl;

    if (choice_ == 2)
        path = "->" + path;

    if (path.length() > width_)
        os << path.substr(0, width_ - 3) << "..." << std::endl;
    else
        os << path << std::endl;

    os << separation << std::endl;

    // Директория
    for (size_t i = page_ * row_count_; (i < (page_ + 1) * row_count_ && i < (all_dirs_.size() + all_files_.size()) ); i++) {
        os << "\t";
        if (page_ * row_count_ + choice_ - 3 == i)
            os << "->";

        if (i < all_dirs_.size())  
            // Папки в директории
            os << all_dirs_[i] << std::endl;
        else
            // Файлы в директории
            os << " - " << all_files_[i - all_dirs_.size()] << std::endl;
    }

    os << separation << std::endl << '\t';

    // Вывод имени файла
    if ((choice_ == row_count_ + 3 && row_count_ * (page_ + 1) <= all_dirs_.size() + all_files_.size()) ||
        (choice_ == (all_dirs_.size() + all_files_.size()) % row_count_ + 3 && row_count_ * (page_ + 1) > all_dirs_.size() + all_files_.size()))
        os << "->";

    os << "../" << file_processor_.GetName() << ".tbl" << std::endl;

    // Нумирация страницы

    os << std::endl << separation << std::endl;

    if (page_ > 0) {
        page_offset.pop_back();
        page_offset.pop_back();
        os << page_offset << "< ";
    }
    else {
        os << page_offset;
    }
    os << (page_ + 1);

    if ((page_ + 1) * row_count_ < all_dirs_.size() + all_files_.size()) os << " >";

    os << std::endl << separation << std::endl;

    std::cout << os.str();
}

void FileProcessorInterface::MoveCursorUp() {
    if (choice_ > 1)
        choice_--;
}

void FileProcessorInterface::MoveCursorDown() {
    if (choice_ + row_count_ * page_ < all_dirs_.size() + all_files_.size() + 3)
        if (row_count_ * page_ + choice_ < (page_ + 1) * row_count_ + 3)
            choice_++;
}


//------------------------------
// Переход на следующую страницу
//------------------------------
void FileProcessorInterface::NextPage() {
    if ((page_ + 1) * row_count_ < all_dirs_.size() + all_files_.size()) {
        page_++;
        choice_ = 0;
    }
}


//------------------------------
// Переход на преидущую страницу
//------------------------------
void FileProcessorInterface::PreviosPage() {
    if (page_ > 0) {
        page_--;
        choice_ = 0;
    } 
}



bool FileProcessorInterface::SelectReadPosition(vec_Vehicle& vec, HANDLE hConsole) {
    if (choice_ == 1) { // Переход к коревой папке
        std::string path = file_processor_.GetPath();
        bool flag = true;

        while (path.size() > 3 && flag)
            if (path.back() != '\\')
                path.pop_back();
            else {
                path.pop_back();
                flag = false;
            }

        file_processor_.SetNewPath(path);
        page_ = 0;

        return true;
    }
    else if (choice_ == 2) { // Ручной ввод директории
        std::string buffer;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD old_coord{}, new_coord{2, 2};
        DWORD count;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputCharacterA(hConsole, ' ', width_ - 2, { 2, 2 }, &count);

        old_coord = csbi.dwCursorPosition;

        SetConsoleCursorPosition(hConsole, new_coord);

        getline(std::cin, buffer);

        SetConsoleCursorPosition(hConsole, old_coord);

        file_processor_.SetNewPath(buffer);
        page_ = 0;

        return true;
    }
    else if (choice_ > 2 && ((choice_ <= row_count_ + 2 && row_count_ * (page_ + 1) <= all_dirs_.size() + all_files_.size()) || (choice_ <= (all_dirs_.size() + all_files_.size()) % row_count_ + 2 && row_count_ * (page_ + 1) > all_dirs_.size() + all_files_.size()))) {

        if (choice_ + row_count_ * page_ - 3 < all_dirs_.size()) {
            // Переход в следущую директорию
            file_processor_.SetNewPath(file_processor_.GetPath() + "\\" + all_dirs_[page_ * row_count_ + choice_ - 3]);

            choice_ = 1;
            page_ = 0;
            return true;
        }
        else {
            // Открытие файла
            file_processor_.SetNewName(all_files_[row_count_ * page_ + choice_ - all_dirs_.size() - 3].substr(0, all_files_[row_count_ * page_ + choice_ - all_dirs_.size() - 3].length() - 4));

            if (file_processor_.ReadFromFile(vec))
                for (auto i : vec)
                    delete i;

            choice_ = 0;
            page_ = 0;
            return false;
        }
        
    }
    else if ((choice_ == row_count_ + 3 && row_count_ * (page_ + 1) <= all_dirs_.size() + all_files_.size()) || (choice_ == (all_dirs_.size() + all_files_.size()) % row_count_ + 3 && row_count_ * (page_ + 1) > all_dirs_.size() + all_files_.size())) {

        std::string buffer;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD old_coord{}, new_coord{ 0, choice_ + 2 };
        DWORD count;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputCharacterA(hConsole, ' ', width_, new_coord, &count);

        old_coord = csbi.dwCursorPosition;

        SetConsoleCursorPosition(hConsole, new_coord);

        std::cout << "\t->../";
        getline(std::cin, buffer);

        SetConsoleCursorPosition(hConsole, old_coord);

        file_processor_.SetNewName(buffer);

        return true;
    }
}

bool FileProcessorInterface::SelectWritePosition(const vec_Vehicle& vec, HANDLE hConsole) {
    if (choice_ == 1) { // Переход к коревой папке
        std::string path = file_processor_.GetPath();
        bool flag = true;

        while (path.size() > 3 && flag)
            if (path.back() != '\\')
                path.pop_back();
            else {
                path.pop_back();
                flag = false;
            }

        file_processor_.SetNewPath(path);
        page_ = 0;

        return true;
    }
    else if (choice_ == 2) { // Ручной ввод директории
        std::string buffer;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD old_coord{}, new_coord{ 2, 2 };
        DWORD count;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputCharacterA(hConsole, ' ', width_ - 2, { 2, 2 }, &count);

        old_coord = csbi.dwCursorPosition;

        SetConsoleCursorPosition(hConsole, new_coord);

        getline(std::cin, buffer);

        SetConsoleCursorPosition(hConsole, old_coord);

        file_processor_.SetNewPath(buffer);
        page_ = 0;

        return true;
    }
    else if (choice_ > 2 && ((choice_ <= row_count_ + 2 && row_count_ * (page_ + 1) <= all_dirs_.size() + all_files_.size()) || (choice_ <= (all_dirs_.size() + all_files_.size()) % row_count_ + 2 && row_count_ * (page_ + 1) > all_dirs_.size() + all_files_.size()))) {

        if (choice_ + row_count_ * page_ - 3 < all_dirs_.size()) {
            // Переход в следущую директорию
            file_processor_.SetNewPath(file_processor_.GetPath() + "\\" + all_dirs_[page_ * row_count_ + choice_ - 3]);

            choice_ = 1;
            page_ = 0;
            return true;
        }
        else {
            // Перезапись файла
            
            file_processor_.SetNewName(all_files_[row_count_ * page_ + choice_ - all_dirs_.size() - 3].substr(0, all_files_[row_count_ * page_ + choice_ - all_dirs_.size() - 3].length() - 4));

            file_processor_.WriteToFile(vec);

            choice_ = 0;
            page_ = 0;
            return false;
        }

    }
    else if ((choice_ == row_count_ + 3 && row_count_ * (page_ + 1) <= all_dirs_.size() + all_files_.size()) || (choice_ == (all_dirs_.size() + all_files_.size()) % row_count_ + 3 && row_count_ * (page_ + 1) > all_dirs_.size() + all_files_.size())) {

        std::string buffer;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD old_coord{}, new_coord{ 0, choice_ + 2 };
        DWORD count;

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        FillConsoleOutputCharacterA(hConsole, ' ', width_, new_coord, &count);

        old_coord = csbi.dwCursorPosition;

        SetConsoleCursorPosition(hConsole, new_coord);

        std::cout << "\t->../";
        getline(std::cin, buffer);

        SetConsoleCursorPosition(hConsole, old_coord);

        file_processor_.SetNewName(buffer);

        file_processor_.WriteToFile(vec);

        return false;
    }
}
