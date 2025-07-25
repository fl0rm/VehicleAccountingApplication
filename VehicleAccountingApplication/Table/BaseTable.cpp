
#include <iostream>
#include <sstream>

#include "BaseTable.h"


//------------------------------
// Конструктор по умолчанию
//------------------------------
BaseTable::BaseTable() {
	
    width_ = 120;
    row_count_ = 10;

    page_ = 0;
    select_element_ = 0;
    select_point_ = TitlePoints::Brand;
    order_ = SortOrder::Asc;

    vehicle_[TypeVehicle::Car] = "Car";
    vehicle_[TypeVehicle::Boat] = "Boat";
    vehicle_[TypeVehicle::Plane] = "Plane";
    vehicle_[TypeVehicle::SpaceShip] = "SpaceShip";

    additionals_field_names_[TypeVehicle::Car].resize(2);
    additionals_field_names_[TypeVehicle::Car][0] = "PTSD";
    additionals_field_names_[TypeVehicle::Car][1] = "Mileage";
    additionals_field_names_[TypeVehicle::Boat].resize(2);
    additionals_field_names_[TypeVehicle::Boat][0] = "Displacement";
    additionals_field_names_[TypeVehicle::Boat][1] = "Depth of immersion of the screw";
    additionals_field_names_[TypeVehicle::Plane].resize(2);
    additionals_field_names_[TypeVehicle::Plane][0] = "Load capacity";
    additionals_field_names_[TypeVehicle::Plane][1] = "Wingspan";
    additionals_field_names_[TypeVehicle::SpaceShip].resize(7);
    additionals_field_names_[TypeVehicle::SpaceShip][0] = "Fuel type";
    additionals_field_names_[TypeVehicle::SpaceShip][1] = "Hyperjump range";
    additionals_field_names_[TypeVehicle::SpaceShip][2] = "Kerosene with liquid oxygen";
    additionals_field_names_[TypeVehicle::SpaceShip][3] = "Liquid hidrogen with liquid oxygen";
    additionals_field_names_[TypeVehicle::SpaceShip][4] = "Solid fuel";
    additionals_field_names_[TypeVehicle::SpaceShip][5] = "Nuclear fuel";
    additionals_field_names_[TypeVehicle::SpaceShip][6] = "Proton fuel";

    hints_[ActiveWindow::Table].resize(4);
    hints_[ActiveWindow::Edit].resize(2);
    hints_[ActiveWindow::Select].resize(4);
    hints_[ActiveWindow::Sorting].resize(2);
    hints_[ActiveWindow::Table][0] = "A - Add";
    hints_[ActiveWindow::Table][1] = "C - Choice";
    hints_[ActiveWindow::Table][2] = "S - Sort";
    hints_[ActiveWindow::Table][3] = "esc - return to viewing";
    hints_[ActiveWindow::Edit][0] = "Enter - confirm";
    hints_[ActiveWindow::Edit][1] = "esc - return to viewing";
    hints_[ActiveWindow::Select][0] = "D - Delete";
    hints_[ActiveWindow::Select][1] = "E - Edit";
    hints_[ActiveWindow::Select][2] = "Enter - view";
    hints_[ActiveWindow::Select][3] = "esc - return to viewing";
    hints_[ActiveWindow::Sorting][0] = "Enter - sort";
    hints_[ActiveWindow::Sorting][1] = "esc - return to viewing";

    //------------------------------
    // Установка отступов для колонок
    

    column_ = title_.size_;

}


//------------------------------
// Деструктор
//------------------------------
BaseTable::~BaseTable() {
    for (auto i : pure_table_) delete i;
}


//------------------------------
// Отрисовка таблицы
//------------------------------
void BaseTable::RenderingTheTable( ActiveWindow visible_) {
    std::ostringstream os;
    std::string separation(width_ - 1, '-');
    int16_t offset = (width_ - column_.sym()) / 7;
    int16_t tmp_counter;

    // Оформление титульника
    os << separation << std::endl;
    os << title_ << std::endl;
    os << separation << std::endl;

    // Форматирование таблицы
    for (unsigned int i = page_ * row_count_; (i < row_count_ * (page_ + 1)) && (i < conditions_table_.size()); i++) {
        for (int8_t j = 0; j < 7; j++)
            os << RederingTheCell(i, static_cast<TitlePoints>(j), visible_).str();            
        os << std::endl;
    }


    // Дорисовка остаточного места
    if ((page_ + 1) * row_count_ > conditions_table_.size()) for (int8_t i = conditions_table_.size() % row_count_; i < row_count_; i++) {

        for (int8_t j = 0; j < 6; j++) {
            for (int16_t y = 0; y < offset + column_.size_.at(static_cast<TitlePoints>(j)); y++) os << ' ';
            os << '|';
        }
        
        os << std::endl;
    }

    os << separation << std::endl;

    // Нумирация страницы
    if (page_ > 0) {
        tmp_counter = log10(page_) - 1;
        for (int8_t j = 0; j < width_ - tmp_counter; j += 2) os << ' ';
        os << "< ";
    }
    else {
        tmp_counter = log10(page_) + 1;
        for (int8_t j = 0; j < width_ - tmp_counter; j += 2) os << ' ';
    }
    os << (page_ + 1);

    if ((page_ + 1) * row_count_ < conditions_table_.size()) os << " >";

    // Подсказки

    os << std::endl << separation << std::endl;

    tmp_counter = 0;
    for (const auto& i : hints_[visible_])
        tmp_counter += i.length() + 2;

    offset = (width_ - tmp_counter);

    for (uint8_t j = 0; j < offset; j += 2) os << ' ';

    for (const auto& i : hints_[visible_])
        os << i << "  ";

    os << std::endl << separation << std::endl;

    // Вывод содержимого в консоль
    std::cout << os.str();
}


//------------------------------
// Отрисовка ячейки
//------------------------------
std::ostringstream BaseTable::RederingTheCell(unsigned int i, TitlePoints cell, ActiveWindow visible_) {
    std::ostringstream os;
    int16_t offset = (width_ - column_.sym()) / 7;
    int16_t shift = 0;

    switch (cell)
    {
    case TitlePoints::Id:
        if (conditions_table_[i].first->GetId() == 0) shift = 1;
        else shift = log10(conditions_table_[i].first->GetId()) + 1;

        if ((visible_ == ActiveWindow::Select || visible_ == ActiveWindow::Edit) && (select_element_ == i)) { os << "->"; shift += 2; }

        break;
    case TitlePoints::Type:
        shift = vehicle_[conditions_table_[i].first->GetType()].length();
        break;
    case TitlePoints::Brand:
        shift = conditions_table_[i].first->brand_.length();
        break;
    case TitlePoints::Model:
        shift = conditions_table_[i].first->model_.length();
        break;
    case TitlePoints::Date:
        shift = conditions_table_[i].first->production_date_.length();
        break;
    case TitlePoints::Weight:
        if (conditions_table_[i].first->weight_ == 0) shift = 1;
        else shift = log10(conditions_table_[i].first->weight_) + 1;
        break;
    case TitlePoints::Additions:
        shift = conditions_table_[i].first->PrintAdditions(additionals_field_names_).length();
        break;
    default:
        break;
    }

    shift = offset + column_.size_.at(cell) - shift;

    if (cell != TitlePoints::Additions || shift >= 0)
        for (int16_t j = 0; j < shift / 2; j++) os << ' ';

    switch (cell)
    {
    case TitlePoints::Id:
        os << conditions_table_[i].first->GetId();
        break;
    case TitlePoints::Type:
        os << vehicle_[conditions_table_[i].first->GetType()];
        break;
    case TitlePoints::Brand:
        os << conditions_table_[i].first->brand_;
        break;
    case TitlePoints::Model:
        os << conditions_table_[i].first->model_;
        break;
    case TitlePoints::Date:
        os << conditions_table_[i].first->production_date_;
        break;
    case TitlePoints::Weight:
        os << conditions_table_[i].first->weight_;
        break;
    case TitlePoints::Additions:
        if (shift < 0) {
            shift = 0;
            for (int8_t i = 0; i < 6; i++) {
                shift += offset + column_.size_.at(static_cast<TitlePoints>(i)) + 1;
            }
            shift = width_ - shift - 3;
            os << conditions_table_[i].first->PrintAdditions(additionals_field_names_).substr(0, shift - 1) << "...";
        }
        else
            os << conditions_table_[i].first->PrintAdditions(additionals_field_names_);
        break;
    default:
        break;
    }

    if (cell != TitlePoints::Additions) {
        for (int16_t j = 0; j < shift / 2; j++) os << ' ';
        if (shift % 2 == 1) os << ' ';
        os << '|';
    }

    return os;
}


//------------------------------
// Отрисовка одного элемента
//------------------------------
void BaseTable::RenderingTheLine() {
    std::ostringstream os;
    std::string separation(width_, '-');

    os << separation << std::endl;

    os << title_.GetTitle().name_.at(TitlePoints::Id) << " - " << conditions_table_[select_element_].first->GetId() << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Type) << " - " << vehicle_.at(conditions_table_[select_element_].first->GetType()) << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Brand) << " - " << conditions_table_[select_element_].first->brand_ << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Model) << " - " << conditions_table_[select_element_].first->model_ << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Date) << " - " << conditions_table_[select_element_].first->production_date_ << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Weight) << " - " << conditions_table_[select_element_].first->weight_ << std::endl;
    os << title_.GetTitle().name_.at(TitlePoints::Additions) << ":" << std::endl;
    os << conditions_table_[select_element_].first->PrintAdditions(additionals_field_names_);

    os << std::endl << separation << std::endl;

    std::cout << os.str();
}


//------------------------------
// Установка новой таблицы
//------------------------------
void BaseTable::SetTable(const vec_Vehicle& tb) {
    clear();
    pure_table_.resize(tb.size());
    
    for (size_t i = 0; i < tb.size(); i++) {
        switch (tb[i]->GetType())
        {
        case TypeVehicle::Car:
            pure_table_[i] = new VehicleCar(dynamic_cast<VehicleCar*>(tb[i]));
            break;
        case TypeVehicle::Boat:
            pure_table_[i] = new VehicleBoat(dynamic_cast<VehicleBoat*>(tb[i]));
            break;
        case TypeVehicle::Plane:
            pure_table_[i] = new VehiclePlane(dynamic_cast<VehiclePlane*>(tb[i]));
            break;
        case TypeVehicle::SpaceShip:
            pure_table_[i] = new VehicleSpaceShip(dynamic_cast<VehicleSpaceShip*>(tb[i]));
            break;
        default:
            break;
        }
    }

    Rebalance();

}


//------------------------------
// Установка курсора на первый столбец
//------------------------------
void BaseTable::SetColumn() {
    select_point_ = TitlePoints::Id;
}


//------------------------------
// Установка курсора на начало страницы
//------------------------------
void BaseTable::SetCursor() {
    select_element_ = page_ * row_count_;

    select_point_ = TitlePoints::Brand;
}


//------------------------------
// Установка языкового пакета
//------------------------------
void BaseTable::SetLang(const LangStruct& pack) {

    title_ = pack.title_;
    column_ = title_.size_;

    vehicle_ = pack.vehicle_;

    additionals_field_names_ = pack.additionals_field_names_;

    hints_ = pack.hints_;

}


//------------------------------
// Сортировка таблицы по параметрам
//------------------------------
void BaseTable::SortingTable() {

    auto comparison{ [](const std::pair<VehicleInterface*, VehicleInterface*>& key,TitlePoints point, SortOrder order, const map_Type_Name& vehicle) {
        
        switch (point)
        {
        case TitlePoints::Id:
            if ((order == SortOrder::Asc && key.first->GetId() > key.second->GetId()) ||
                (order == SortOrder::Desc && key.first->GetId() < key.second->GetId())) return true;
            break;
        case TitlePoints::Type:
            if ((order == SortOrder::Asc && vehicle.at(key.first->GetType()) > vehicle.at(key.second->GetType())) ||
                (order == SortOrder::Desc && vehicle.at(key.first->GetType()) < vehicle.at(key.second->GetType()))) return true;
            break;
        case TitlePoints::Brand:
            if ((order == SortOrder::Asc && key.first->brand_ > key.second->brand_) ||
                (order == SortOrder::Desc && key.first->brand_ < key.second->brand_)) return true;
            break;
        case TitlePoints::Model:
            if ((order == SortOrder::Asc && key.first->model_ > key.second->model_) ||
                (order == SortOrder::Desc && key.first->model_ < key.second->model_)) return true;
            break;
        case TitlePoints::Date:
            if ((order == SortOrder::Asc && key.first->production_date_ > key.second->production_date_) ||
                (order == SortOrder::Desc && key.first->production_date_ < key.second->production_date_)) return true;
            break;
        case TitlePoints::Weight:
            if ((order == SortOrder::Asc && key.first->weight_ > key.second->weight_) ||
                (order == SortOrder::Desc && key.first->weight_ < key.second->weight_)) return true;
            break;
        default:
            break;
        }

        return false;
    } };

    
    for (size_t i = 1; i < conditions_table_.size(); i++) {

        std::pair<VehicleInterface*, size_t> key = conditions_table_[i];
        size_t j = i;
        while (j > 0 && comparison({conditions_table_[j - 1].first, key.first}, select_point_, order_, vehicle_)) {
            conditions_table_[j] = conditions_table_[j - 1];
            j--;
        }
        conditions_table_[j] = key;
    }

    switch (order_)
    {
    case SortOrder::Asc:
        order_ = SortOrder::Desc;
        break;
    case SortOrder::Desc:
        order_ = SortOrder::Asc;
        break;
    default:
        break;
    }

}


//------------------------------
// Переход на следующую страницу
//------------------------------
void BaseTable::NextPage() {
    if ((page_ + 1) * row_count_ < conditions_table_.size()) page_++;
}


//------------------------------
// Переход на преидущую страницу
//------------------------------
void BaseTable::PreviosPage() {
    if (page_ > 0) page_--;
}


//------------------------------
// Переход к следующую столбцу
//------------------------------
void BaseTable::NextColumn(const ActiveWindow& visible) {
    if ((select_point_ < TitlePoints::Additions && visible == ActiveWindow::Edit) ||
        (select_point_ < TitlePoints::Weight && visible == ActiveWindow::Sorting)) 
    {
        select_point_ = static_cast<TitlePoints>(static_cast<int>(select_point_) + 1);
        order_ = SortOrder::Asc;
    }
        
}


//------------------------------
// Переход к предидущему столбцу
//------------------------------
void BaseTable::PreviosColumn(const ActiveWindow& visible) {
    if ((select_point_ > TitlePoints::Type && visible == ActiveWindow::Edit) ||
        (select_point_ > TitlePoints::Id && visible == ActiveWindow::Sorting))
    {
        select_point_ = static_cast<TitlePoints>(static_cast<int>(select_point_) - 1);
        order_ = SortOrder::Asc;
    }
}


//------------------------------
// Анимация выбора столбца
//------------------------------
void BaseTable::SelectionAnimation(HANDLE hConsole, const ActiveWindow& visible) {
    title_.Animation(hConsole, select_point_, visible);
}


//------------------------------
// Выбор элемент выше
//------------------------------
void BaseTable::SelectElemUp() {
    if (select_element_ > 0) {
        if (page_ * row_count_ >= select_element_) { select_element_ = page_ * row_count_ - 1; page_--; }
        else select_element_--;

        select_point_ = TitlePoints::Brand;
    }
}


//------------------------------
// Выбор элемента ниже
//------------------------------
void BaseTable::SelectElemDown() {
    if (select_element_ < conditions_table_.size() - 1) {
        if (select_element_ >= (page_ + 1) * row_count_ - 1) { page_++; select_element_ = page_ * row_count_; }
        else select_element_++;

        select_point_ = TitlePoints::Brand;
    }
}


//------------------------------
// Смена типа для объекта
//------------------------------
TypeVehicle BaseTable::ChangeType(HANDLE hConsole, KeyHandler input, COORD position) {
    TypeVehicle basic_type = conditions_table_[select_element_].first->GetType();
    unsigned short offset = (width_ - column_.sym()) / 7;
    short overall_offset = 0;

    auto showChange{ [](HANDLE hConsole, unsigned short offset, std::string name_of_type, COORD position) {
        DWORD count;
        FillConsoleOutputCharacterA(hConsole, ' ', offset, position, &count);
        position.X += (offset - name_of_type.length()) / 2;
        SetConsoleCursorPosition(hConsole, position);
        WriteConsoleA(hConsole, name_of_type.c_str(), name_of_type.length(), &count, NULL);
        } };

    showChange(hConsole, offset + column_.size_.at(TitlePoints::Type), ('<' + vehicle_.at(basic_type) + '>'), position);

    while (!input.isKeyPressedOnce(VK_RETURN)) {



        if (input.isKeyPressedOnce(VK_LEFT)) {
            if (basic_type <= TypeVehicle::Car) basic_type = TypeVehicle::SpaceShip;
            else basic_type = static_cast<TypeVehicle>(static_cast<int>(basic_type) - 1);
            showChange(hConsole, offset + column_.size_.at(TitlePoints::Type), ('<' + vehicle_.at(basic_type) + '>'), position);
        }
        if (input.isKeyPressedOnce(VK_RIGHT)) {
            if (basic_type >= TypeVehicle::SpaceShip) basic_type = TypeVehicle::Car;
            else basic_type = static_cast<TypeVehicle>(static_cast<int>(basic_type) + 1);
            showChange(hConsole, offset + column_.size_.at(TitlePoints::Type), ('<' + vehicle_.at(basic_type) + '>'), position);
        }
    }
    
    return basic_type;
}


//------------------------------
// Добавить элемент
//------------------------------
void BaseTable::AddElement() {

    pure_table_.push_back({});
    conditions_table_.push_back({});

    if (pure_table_.size() > 1)        
        pure_table_.back() = new VehicleCar((*(pure_table_.end() - 2))->GetId() + 1);
    else
        pure_table_.back() = new VehicleCar(1);

    conditions_table_.back().first = pure_table_.back();
    conditions_table_.back().second = pure_table_.size() - 1;

    if ((page_ + 1) * row_count_ < conditions_table_.size())
        page_ = (conditions_table_.size() - 1) / row_count_;
    select_element_ = pure_table_.size() - 1;
    select_point_ = TitlePoints::Type;

}


//------------------------------
// Редактировать элемент
//------------------------------
void BaseTable::EditElement(HANDLE hConsole ,KeyHandler& input) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD old_coord;
    COORD new_coord {0, select_element_ % row_count_ + 3};
    DWORD count;
    std::string input_string;

    unsigned short offset = (width_ - column_.sym()) / 7;
    short overall_offset = 0;
    for (int8_t i = 0; i < static_cast<int>(select_point_); i++) overall_offset += offset + column_.size_.at(static_cast<TitlePoints>(i)) + 1;
    new_coord.X = overall_offset;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    old_coord = csbi.dwCursorPosition;

    SetConsoleCursorPosition(hConsole, new_coord);
    
    FillConsoleOutputCharacterA(hConsole, ' ', offset + column_.size_.at(select_point_), new_coord, &count);
    
    switch (select_point_)
    {
    case TitlePoints::Type:  {
        TypeVehicle upload_type = ChangeType(hConsole, input, new_coord);

        if (upload_type != conditions_table_[select_element_].first->GetType()) {
            unsigned int save_old_id = conditions_table_[select_element_].first->GetId();

            switch (upload_type)
            {
            case TypeVehicle::Car:
                delete conditions_table_[select_element_].first;
                conditions_table_[select_element_].first = new VehicleCar(save_old_id);
                pure_table_[conditions_table_[select_element_].second] = conditions_table_[select_element_].first;
                break;
            case TypeVehicle::Boat:
                delete conditions_table_[select_element_].first;
                conditions_table_[select_element_].first = new VehicleBoat(save_old_id);
                pure_table_[conditions_table_[select_element_].second] = conditions_table_[select_element_].first;
                break;
            case TypeVehicle::Plane:
                delete conditions_table_[select_element_].first;
                conditions_table_[select_element_].first = new VehiclePlane(save_old_id);
                pure_table_[conditions_table_[select_element_].second] = conditions_table_[select_element_].first;
                break;
            case TypeVehicle::SpaceShip:
                delete conditions_table_[select_element_].first;
                conditions_table_[select_element_].first = new VehicleSpaceShip(save_old_id);
                pure_table_[conditions_table_[select_element_].second] = conditions_table_[select_element_].first;
                break;
            default:
                break;
            }
        }
        }
        break;
    case TitlePoints::Brand:
        getline(std::cin, input_string);
        conditions_table_[select_element_].first->brand_ = input_string;
        break;
    case TitlePoints::Model:
        getline(std::cin, input_string);
        conditions_table_[select_element_].first->model_ = input_string;
        break;
    case TitlePoints::Date:
        getline(std::cin, input_string);
        conditions_table_[select_element_].first->production_date_ = input_string;
        break;
    case TitlePoints::Weight:
        getline(std::cin, input_string);
        try {
            unsigned long u = std::stoul(input_string);
            if (u > (std::numeric_limits<unsigned int>::max)())
                throw std::out_of_range(input_string);

            conditions_table_[select_element_].first->weight_ = u;
        }
        catch (const std::invalid_argument& e) {
            conditions_table_[select_element_].first->weight_ = 0;
        }
        catch (const std::out_of_range& e) {
            conditions_table_[select_element_].first->weight_ = 0;
        }
        break;
    case TitlePoints::Additions:
        break;
    default:
        break;
    }

    SetConsoleCursorPosition(hConsole, old_coord);
}

//------------------------------
// Удалить элемент
//------------------------------
void BaseTable::DeleteElement() {

    if (select_element_ < conditions_table_.size()) {

        delete conditions_table_[select_element_].first;

        pure_table_.erase(pure_table_.begin() + conditions_table_[select_element_].second);
        conditions_table_.erase(conditions_table_.begin() + select_element_);

        if (page_ * row_count_ + 1 > conditions_table_.size())
            if (conditions_table_.size() == 0) page_ = 0;
            else page_ = (conditions_table_.size() - 1) / row_count_;
    }

    Rebalance();

}


//------------------------------
// Очистка таблицы
//------------------------------
void BaseTable::clear() {
    for (auto i : pure_table_) delete i;
    pure_table_.clear();
    conditions_table_.clear();
}


void BaseTable::Rebalance() {
    conditions_table_.reserve(pure_table_.size());
    conditions_table_.resize(pure_table_.size());

    for (size_t i = 0; i < pure_table_.size(); i++) {
        conditions_table_[i].first = pure_table_[i];
        conditions_table_[i].second = i;
    }
}