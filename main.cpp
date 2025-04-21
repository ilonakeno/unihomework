#include "func.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <string>
using namespace std;

struct PassSummary {
    string passType;    
    int tripsPurchased; 
    int ridesUsed;      
    double cost;        
};

int main() {
    double balance = 0.0;
    int option = 0;

    while (true) {
        cout << "Введіть суму, якою поповнюєте рахунок: ";
        cin >> balance;
        if(cin.fail() || balance <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Невірна сума." << endl;
            cout << "Виберіть: 1 - Спробувати ще, 2 - Завершити на сьогодні: ";
            cin >> option;
            if(cin.fail() || (option != 1 && option != 2)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Невірний вибір. Спробуйте ще." << endl;
                continue;
            }
            if(option == 2) {
                cout << "Завершення на сьогодні." << endl;
                return 0;
            }
        } else {
            break;
        }
    }
    
    // Пропоновані опції скіпасів:
    // Обмежені скіпаси: 3, 5, 10 поїздок з відповідними цінами
    // Безлімітний скіпас: ціна 200.0 грн.
    const int option1Trips = 3, option2Trips = 5, option3Trips = 10;
    const double option1Price = 50.0, option2Price = 80.0, option3Price = 140.0;
    const double unlimitedPrice = 200.0;
    
    // Використовуємо статичний масив для збереження сесій (максимум 100 сесій)
    const int maxSessions = 100;
    PassSummary summaries[maxSessions];
    int sessionCount = 0;
    
    char buyNewPass = '1'; // 1 - купувати новий пас, 2 - ні
    
    do {
        int passChoice = 0;
        bool validPurchase = false;
        int trips = 0;
        double price = 0.0;
        SkiPass* pass = nullptr;
        string passType;
        
        while (!validPurchase) {
            cout << "\nОберіть скіпас для покупки:" << endl;
            cout << "1. Обмежений скіпас: " << option1Trips << " поїздок за " << option1Price << " грн." << endl;
            cout << "2. Обмежений скіпас: " << option2Trips << " поїздок за " << option2Price << " грн." << endl;
            cout << "3. Обмежений скіпас: " << option3Trips << " поїздок за " << option3Price << " грн." << endl;
            cout << "4. Безлімітний скіпас за " << unlimitedPrice << " грн." << endl;
            cout << "Ваш вибір: ";
            
            cin >> passChoice;
            if(cin.fail() || passChoice < 1 || passChoice > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Невірний вибір. Спробуйте ще." << endl;
                continue;
            }
            
            if(passChoice == 4) {
                price = unlimitedPrice;
                passType = "Безлімітний";
                if (balance < price) {
                    cout << "Недостатньо коштів для покупки безлімітного скіпасу." << endl;
                } else {
                    balance -= price;
                    pass = new UnlimitedSkiPass();
                    validPurchase = true;
                }
            } else {
                switch(passChoice) {
                    case 1:
                        trips = option1Trips;
                        price = option1Price;
                        break;
                    case 2:
                        trips = option2Trips;
                        price = option2Price;
                        break;
                    case 3:
                        trips = option3Trips;
                        price = option3Price;
                        break;
                }
                passType = "Обмежений (" + to_string(trips) + " поїздок)";
                if (balance < price) {
                    cout << "Недостатньо коштів для покупки обраного скіпасу." << endl;
                } else {
                    balance -= price;
                    pass = new LimitedSkiPass(trips);
                    validPurchase = true;
                }
            }
            
            if(!validPurchase) {
                cout << "\nВиберіть: 1 - Обрати інший скіпас, 2 - Поповнити рахунок, 3 - Вийти: ";
                int choice2;
                cin >> choice2;
                if(cin.fail() || (choice2 < 1 || choice2 > 3)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Невірний вибір. Спробуйте ще." << endl;
                    continue;
                }
                if(choice2 == 2) {
                    double addFunds = 0.0;
                    cout << "Введіть суму поповнення: ";
                    cin >> addFunds;
                    if(!cin.fail() && addFunds > 0) {
                        balance += addFunds;
                        cout << "Новий баланс: " << balance << " грн." << endl;
                    } else {
                        cout << "Невірна сума поповнення." << endl;
                    }
                } else if(choice2 == 3) {
                    cout << "Завершення на сьогодні." << endl;
                    return 0;
                }
            }
        }
        
        int ridesUsed = 0;
        Turnstile turnstile("TS1");
        char continueRide = '1'; 
        
        do {
            cout << "\nОберіть гору для поїздки:" << endl;
            cout << "1. Everest" << endl;
            cout << "2. Papaya" << endl;
            cout << "3. Goverla (тільки для безлімітного скіпасу)" << endl;
            cout << "Ваш вибір: ";
            int mountainChoice;
            cin >> mountainChoice;
            string mountain;
            switch(mountainChoice) {
                case 1:
                    mountain = "Everest";
                    break;
                case 2:
                    mountain = "Papaya";
                    break;
                case 3:
                    mountain = "Goverla";
                    break;
                default:
                    cout << "Невірний вибір. Поїздка пропущена." << endl;
                    continue;
            }
            
            if (turnstile.allowEntry(*pass, mountain)) {
                ridesUsed++;
            } else {
                cout << "Неможливо виконати поїздку." << endl;
                break;
            }
            
            if(passChoice != 4) {
                LimitedSkiPass* lpass = dynamic_cast<LimitedSkiPass*>(pass);
                if(lpass && lpass->getRemainingTrips() <= 0) {
                    cout << "Всі поїздки використано для цього скіпасу." << endl;
                    break;
                }
            }
            
            cout << "\nБажаєте здійснити ще одну поїздку з поточним скіпасом? (1 - так, 2 - ні): ";
            cin >> continueRide;
        } while(continueRide == '1');
        
        if(sessionCount < maxSessions) {
            summaries[sessionCount].passType = passType;
            summaries[sessionCount].tripsPurchased = (passChoice == 4) ? 0 : 
                ((passChoice == 1) ? option1Trips : (passChoice == 2) ? option2Trips : option3Trips);
            summaries[sessionCount].ridesUsed = ridesUsed;
            summaries[sessionCount].cost = price;
            sessionCount++;
        } else {
            cout << "Досягнуто максимум сесій." << endl;
        }
        
        delete pass;
        
        cout << "\nБажаєте придбати новий скіпас? (1 - так, 2 - ні): ";
        cin >> buyNewPass;
        
    } while(buyNewPass == '1');
    
    time_t now = time(nullptr);
    char dateBuffer[100];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    ofstream check("check.txt");
    if (!check) {
        cout << "Не вдалося відкрити файл для запису чеку." << endl;
        return 1;
    }
    
    check << "----- ЧЕК -----" << endl;
    check << "Дата: " << dateBuffer << endl << endl;
    
    for (int i = 0; i < sessionCount; i++) {
        check << "Скіпас " << i+1 << ":" << endl;
        check << "  Тип: " << summaries[i].passType << endl;
        if(summaries[i].passType != "Безлімітний")
            check << "  Придбано поїздок: " << summaries[i].tripsPurchased << endl;
        check << "  Використано поїздок: " << summaries[i].ridesUsed << endl;
        check << "  Вартість: " << summaries[i].cost << " грн." << endl << endl;
    }
    
    check << "Залишок на рахунку: " << balance << " грн." << endl;
    check << "----------------" << endl;
    check.close();
    
    cout << "\nЧек сформовано та збережено у файлі check.txt" << endl;
    
    return 0;
}
