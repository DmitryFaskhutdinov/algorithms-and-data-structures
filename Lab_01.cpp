// Общими требованиями к лабораторной работе являются:
// 1) указывать  имена входного  и  выходного   файлов   через командную строку или с клавиатуры в процессе работы программы;
// 2) обрабатывать ошибки открытия файлов;
// 3) ориентироваться на кодировку ANSI (1251);
// 3) не использовать  стандартные функции работы со  строками и символами
//  COPY, POS, Str, Val и т. п. из ПАСКАЛя; 
//  stoi, to_string, substr, strstr, find, regex и т. п.  из C++;
// 4) иметь возможность работы с текстами на русском языке;
// 5) не переписывать файл в оперативную память целиком.


// 19. Строки текстового  файла  содержат  фамилии  студентов. 
// Определить   порядковый   номер   байта,  начиная  с  которого располагается первая  по  алфавиту  фамилия.  
// Заменить  данную фамилию  в исходном файле символами '*',  не переписывая этого файла (8).

#include <iostream>
#include <fstream>

const int max_length{100};
const char repSymbol = '*';

void readFileName(std::string& userFile);
bool checkFile(std::fstream& fs);
bool isLess( char currName[], char minName[], int currLen, int minLen);
void updateMinSurename(char currSurename[], int currSurLen, long currSurPosition, char minSurename[], int& minSurLen, long& minSurPosition);
void replaceWithSymbol(std::fstream& fs, long position, int length, char symbol);

int main() {
    std::string fileName;
    readFileName(fileName);

    std::fstream fs;
    fs.open(fileName, std::ios::in | std::ios::out);
    if (!checkFile(fs)) {
        return 1;
    }

    char minSurename[max_length] = {0};
    int minSurLen = 0;
    long minSurPosition = 0;

    char currSurename[max_length] = {0};
    int currSurLen = 0;
    long currSurPosition = fs.tellg();

    char ch;

    while (fs.get(ch)) {
        if (ch != '\n' && currSurLen < max_length - 1) {
            //
            if (currSurLen == 0) {
                currSurPosition = fs.tellg();
            }
            //
            currSurename[currSurLen++] = ch;
        } else {
            currSurename[currSurLen] = '\0';

            if (minSurLen == 0 || isLess(currSurename, minSurename, currSurLen, minSurLen)) {
                updateMinSurename(currSurename, currSurLen, currSurPosition, minSurename, minSurLen, minSurPosition);
            }
            
            currSurLen = 0;
            //currSurPosition = fs.tellg();
        }
    }

    if (currSurLen > 0) {
        currSurename[currSurLen] = '\0';
        if (minSurLen == 0 || isLess(currSurename, minSurename, currSurLen, minSurLen)) {
            updateMinSurename(currSurename, currSurLen, currSurPosition, minSurename, minSurLen, minSurPosition);
            std::cout << "Текущая минимальная фамилия: " << minSurename << ", длина: " << minSurLen << ", позиция: " << minSurPosition << std::endl;

        }
    }

    replaceWithSymbol(fs, minSurPosition, minSurLen, repSymbol);
    std::cout << "Фамилию " << minSurename << " заменили на " << repSymbol << " на позиции " << minSurPosition << std::endl;

    fs.close();
}

void readFileName(std::string& userFile) {
    std::cout << "Введите название файла для работы (без пробелов): ";
    std::cin >> userFile; 
    std::cout << "Выбран файл: " << userFile << std::endl; 
}

bool checkFile(std::fstream& fs) {
    if (!fs.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл. Проверте верно ли введено название и повторите попытку." << std::endl; 
        return false;
    } 
    if (fs.peek() == EOF) {
        std::cout << "Ошибка: файл пуст.\n";
        return false;
    }
    return true;
}

bool isLess( char currName[], char minName[], int currLen, int minLen) {
    for (int i = 0; i < currLen && i < minLen; i++) {
        if (currName[i] < minName[i]) {
            return true;
        }
        if (currName[i] > minName[i]) {
            return false;
        } 
    }

    if (currLen < minLen) {
        return true;
    } else {
        return false;
    }
}

void replaceWithSymbol(std::fstream& fs, long position, int length, char symbol) {
    fs.seekp(position);
    for (int i = 0; i < length; i++) { 
        fs.put(symbol);
    }
}

void updateMinSurename(char currSurename[], int currSurLen, long currSurPosition, char minSurename[], int& minSurLen, long& minSurPosition) {
    for (int i = 0; i < currSurLen; ++i) {
        minSurename[i] = currSurename[i];
    }
    minSurLen = currSurLen;
    minSurename[minSurLen] = '\0';
    minSurPosition = currSurPosition;
}