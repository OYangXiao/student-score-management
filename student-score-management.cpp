// a student score manage system

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

static const char *DATA_FILENAME = "data.csv";
static const char *CURSE_HEADER = "CurseID, CurseName, CursePoint\n";
static const char *STUDENT_HEADER = "StudentID, StudentName, StudentGender, StudentAge\n";
static const char *SCORE_HEADER = "StudentID, CurseID, Score\n";
static pair<nullptr_t, int> NOT_FOUND = make_pair(nullptr, -1);

class Curse
{
public:
    string id;
    string name;
    short point;
    Curse(string id, string name)
    {
        this->id = id;
        this->name = name;
        this->point = 0;
    }
    bool match(string matchType, string idOrName)
    {
        if (matchType == "id")
        {
            return this->id == idOrName;
        }
        else if (matchType == "name")
        {
            return this->name == idOrName;
        }
        return false;
    }
};

class Student
{
public:
    string id;
    string name;
    string gender;
    int age;
    Student(string id, string name)
    {
        this->id = id;
        this->name = name;
        this->gender = "";
        this->age = 0;
    }
    bool match(string matchType, string idOrName)
    {
        if (matchType == "id")
        {
            return this->id == idOrName;
        }
        else if (matchType == "name")
        {
            return this->name == idOrName;
        }
        return false;
    }
};

class Score
{
private:
    string integer;
    string decimal;
    string studentId;
    string curseId;

public:
    Score(string studentId, string curseId)
    {
        this->studentId = studentId;
        this->curseId = curseId;
        this->integer = "";
        this->decimal = "";
    }

    bool match(string studentId, string curseId)
    {
        return this->studentId == studentId && this->curseId == curseId;
    }

    string getScore()
    {
        if (this->integer == "")
        {
            return "未录入成绩";
        }
        else if (this->decimal == "0")
        {
            return this->integer;
        }
        else
        {
            return this->integer + "." + this->decimal;
        }
    }
    string setScore(string scoreStr)
    {
        this->integer = stoi(scoreStr.substr(0, scoreStr.find(".")));
        this->decimal = stoi(scoreStr.substr(scoreStr.find(".") + 1));
    }
};

class StudentScoreManagement
{
private:
    vector<Curse> curses;
    vector<Student> students;
    vector<Score> scores;

    // template函数
    // 1. 根据id或name查找课程
    // 2. 根据id或name查找学生
    // 3. 根据学生id和课程id查找分数
    template <class T>
    pair<T *, int> findItem(vector<T> &items, string condition1, string condition2)
    {
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].match(condition1, condition2))
            {
                return make_pair(&items[i], i);
            }
        }
        return NOT_FOUND
    }

    // template to creat and add item
    template <class T>
    void addItem(vector<T> &items, string info1, string info2)
    {
        items.push_back(T(id, name));
    }

    // template to remove item
    template <class T>
    void removeItem(vector<T> &items, string nameOrId)
    {
        pair<T *, int> itemPair = this->findItem(items, nameOrId);
        if (itemPair.first != nullptr)
        {
            items.erase(items.begin() + itemPair.second);
        }
    }

    // template to update item
    template <class T>
    void updateItem(vector<T> &items, string nameOrId, string newName)
    {
        pair<T *, int> itemPair = this->findItem(items, nameOrId);
        if (itemPair.first != nullptr)
        {
            itemPair.first->name = newName;
        }
    }

public:
    // 导出数据到文件
    void exportData()
    {
        this->writeDataToFile();
    }

    // 导入数据
    void importData()
    {
        std::ifstream file("data.csv");
        string line;
        string currentType = "";
        while (std::getline(file, line))
        {
            if (line == CURSE_HEADER)
            {
                currentType = "curse";
                continue;
            }
            else if (line == STUDENT_HEADER)
            {
                currentType = "student";
                continue;
            }
            else if (line == SCORE_HEADER)
            {
                currentType = "score";
                continue;
            }
            else if (currentType == "curse" || currentType == "student")
            {
                // 每行数据格式为：ID, Name
                string id = line.substr(0, line.find(","));
                string name = line.substr(line.find(",") + 2);
                if (currentType == "curse")
                {
                    this->addItem(curses, id, name);
                }
                else if (currentType == "student")
                {
                    this->addItem(students, id, name);
                }
            }
            else if (currentType == "score")
            {
                // 每行数据格式为：StudentID, CurseID, Score
                string studentId = line.substr(0, line.find(","));
                string curseId = line.substr(0, line.find(","));
                string scoreStr = line.substr(line.find(",") + 1);
                short integer = stoi(scoreStr.substr(0, scoreStr.find(".")));
                short decimal = stoi(scoreStr.substr(scoreStr.find(".") + 1));
                scores.push_back(Score(studentId, curseId, integer, decimal));
            }

            string id = line.substr(0, line.find(","));
            string name = line.substr(0, line.find(","));
            string scoreStr = line.substr(line.find(",") + 1);
            int score = stoi(scoreStr);
            students.push_back(Student(id, name, score));
        }
        file.close();
    }

    void printSystemStatus()
    {
        cout << "Total students: " << students.size() << endl;
    }

    void writeDataToFile()
    {
        std::ofstream file;
        file.open("data.csv");
        // 每行数据格式为：Name, Score
        file << "ID, Name, Score\n";
        for (int i = 0; i < students.size(); i++)
        {
            file << students[i].getId() << ", " << students[i].getName() << ", " << students[i].getScore() << "\n";
        }
        file.close();
    }

    // 加入新的学生
    void addStudent(string id, string name, int score)
    {
        students.push_back(Student(id, name, score));
        this->writeDataToFile();
    }

    // 移除学生
    void removeStudent(string nameOrId)
    {
        pair<Student *, int> studentPair = this->findStudent(nameOrId);
        if (studentPair.first != nullptr)
        {
            students.erase(students.begin() + studentPair.second);
        }
        this->writeDataToFile();
    }

    // 更新学生分数
    void updateStudentScore(string nameOrId, int score)
    {
        pair<Student *, int> studentPair = this->findStudent(nameOrId);
        if (studentPair.first != nullptr)
        {
            studentPair.first->setScore(score);
        }
        this->writeDataToFile();
    }

    // 显示指定学生成绩
    void printStudentScore(string nameOrId)
    {
        pair<Student *, int> studentPair = this->findStudent(nameOrId);
        if (studentPair.first != nullptr)
        {
            cout << "ID: " << studentPair.first->getId() << ", Name: " << studentPair.first->getName() << ", Score: " << studentPair.first->getScore() << endl;
        }
    }
};

// StudentScoreManagement studentScoreManagement;

// studentScoreManagement.addStudent("Alice", 90);
// studentScoreManagement.addStudent("Bob", 80);
// studentScoreManagement.addStudent("Charlie", 70);

// studentScoreManagement.printAllStudents();

// studentScoreManagement.removeStudent("Bob");

// studentScoreManagement.printAllStudents();

// studentScoreManagement.updateStudentScore("Alice", 95);

// studentScoreManagement.printAllStudents();

int showMenu()
{
    cout << "1. 导入数据" << endl;
    cout << "2. show system status" << endl;
    cout << "3. add student" << endl;
    cout << "4. remove student" << endl;
    cout << "5. display one student" << endl;
    cout << "6. edit student" << endl;
    cout << "7. dedupe" << endl;
    cout << "8. export data" << endl;
    cout << "9. exit" << endl;
    cout << "Please select a number: ";
    int choice;
    cin >> choice;
    return choice;
}

// 显示提示并等待用户输入
void promptToInput(string prompt, string &input)
{
    cout << prompt;
    cin >> input;
}

int main()
{
    // create a student score management system
    StudentScoreManagement studentScoreManagement;

    // show menu and wait for input
    int choice = showMenu();

    // loop until user choose to exit
    while (choice != 9)
    {
        switch (choice)
        {
        case 1:
            cout << "import data" << endl;
            studentScoreManagement.importData();
            break;
        case 2:
            cout << "show system status" << endl;
            studentScoreManagement.printSystemStatus();
            break;
        case 3:
            cout << "add student" << endl;
            // wait for user input
            string id, name;
            int score;

            break;
        case 4:
            cout << "remove student" << endl;
            break;
        case 5:
            cout << "display one student" << endl;
            break;
        case 6:
            cout << "edit student" << endl;
            break;
        case 7:
            cout << "dedupe" << endl;
            break;
        case 8:
            cout << "export data" << endl;
            break;
        default:
            cout << "Invalid choice" << endl;
            break;
        }

        // show menu and wait for input
        choice = showMenu();
    }
}