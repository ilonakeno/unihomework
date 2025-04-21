#ifndef FUNC_H
#define FUNC_H

#include <string>
using namespace std;

class SkiPass {
public:
 
    virtual bool usePass(const string& mountain) = 0;
    
    
    virtual int getRemainingTrips() const = 0;
    
    virtual ~SkiPass() {}
};

class LimitedSkiPass : public SkiPass {
public:
    LimitedSkiPass(int trips);

    
    virtual bool usePass(const string& mountain) override;

    virtual int getRemainingTrips() const override;
    
    int getTotalTrips() const;
    
private:
    int totalTrips;         
    int remainingTrips;    
};

class UnlimitedSkiPass : public SkiPass {
public:
    UnlimitedSkiPass();

   
    virtual bool usePass(const string& mountain) override;
    
    virtual int getRemainingTrips() const override;
};

class Turnstile {
public:
    Turnstile(const string& id);

    bool allowEntry(SkiPass& pass, const string& mountain);
    
private:
    string turnstileId;
};

#endif // FUNC_H
