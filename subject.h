#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>

template <typename InfoType, typename StateType> class Observer;

template <typename InfoType, typename StateType> class Subject {
    StateType state;
protected:
    void setState(StateType newS);

    std::vector<Observer<InfoType, StateType>*> observers;
public:
    void attach(Observer<InfoType, StateType> *o);
    void notifyObservers();
    virtual InfoType getInfo() const = 0;
    StateType getState() const;
};

template <typename InfoType, typename StateType>
void Subject<InfoType, StateType>::attach(Observer<InfoType, StateType> *o) {
    observers.emplace_back(o);
}

template <typename InfoType, typename StateType>
void Subject<InfoType, StateType>::notifyObservers() {
    for (auto &ob : observers) ob->notify(*this);
}

template <typename InfoType, typename StateType>
void Subject<InfoType, StateType>::setState(StateType newS) { state = newS; }

template <typename InfoType, typename StateType>
StateType Subject<InfoType, StateType>::getState() const { return state; }

#endif
