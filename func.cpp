#include "func.h"
#include <iostream>
using namespace std;

LimitedSkiPass::LimitedSkiPass(int trips)
    : totalTrips(trips), remainingTrips(trips) {}

bool LimitedSkiPass::usePass(const string& mountain) {
    if (remainingTrips <= 0) {
        cout << "Немає залишкових поїздок." << endl;
        return false;
    }
    remainingTrips--;
    cout << "Поїздка на горі " << mountain 
         << " успішна. Залишилося поїздок: " << remainingTrips << endl;
    return true;
}

int LimitedSkiPass::getRemainingTrips() const {
    return remainingTrips;
}

int LimitedSkiPass::getTotalTrips() const {
    return totalTrips;
}

UnlimitedSkiPass::UnlimitedSkiPass() {
}

bool UnlimitedSkiPass::usePass(const string& mountain) {
    cout << "Поїздка на горі " << mountain << " успішна (безлімітний скіпас)." << endl;
    return true;
}

int UnlimitedSkiPass::getRemainingTrips() const {
    return -1;
}

Turnstile::Turnstile(const string& id)
    : turnstileId(id) {}

bool Turnstile::allowEntry(SkiPass& pass, const string& mountain) {
    if(mountain == "Goverla") {
        LimitedSkiPass* lpass = dynamic_cast<LimitedSkiPass*>(&pass);
        if(lpass != nullptr) {
            cout << "Гора " << mountain << " доступна лише для безлімітного скіпасу." << endl;
            return false;
        }
    }
    cout << "Турнікет " << turnstileId 
         << " перевіряє скіпас для гори " << mountain << "..." << endl;
    return pass.usePass(mountain);
}
