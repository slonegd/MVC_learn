/**
 * Вы можете модифицировать написанную программу, добавив режим ввода температуры в шкале
 * Фаренгейта: для этого понадобится дополнительный метод setMode() в классе ConsoleView,
 * который будет устанавливать текущий режим отображения (их должно быть два: с предложением
 * ввести градусы в цельсиях и в фаренгейтах), а сам режим будет устанавливать контроллер. 
 * 
 * НО: как, если нет прямой связи View-Controller?
 **/

#include <iostream>
#include <vector>

class Observer
{
public:
    virtual void update() = 0;
};

class Observable
{
   std::vector<Observer*> _observers;
public:
   void addObserver (Observer *observer) { _observers.push_back(observer); }
   void notifyUpdate()
   {
      int size = _observers.size();
      for (int i = 0; i < size; i++)
         _observers[i]->update();
   }
};

class TemperatureModel : public Observable
{
   float _temperatureF {0};
public:
   TemperatureModel (float tempF = 0) : _temperatureF(tempF) { }
   float getF ()            { return _temperatureF;                                      }
   float getC ()            { return (_temperatureF - 32.0) * 5.0 / 9.0;                 }
   void  setF (float tempF) { _temperatureF = tempF;                    notifyUpdate();  } 
   void  setC (float tempC) { _temperatureF = tempC * 9.0 / 5.0 + 32.0; notifyUpdate();  }
};


class Controller
{
   TemperatureModel *_model;
   bool _f_mode {false};
public:
   Controller(TemperatureModel *model) : _model(model) { }
   void start()
   {
      float temp;
      while (1) {
         std::cin >> temp;
         if (temp == 0)
            _f_mode = not _f_mode;
         if (_f_mode)
            _model->setF(temp);
         else
            _model->setC(temp);
      }
   }
   bool f_mode() { return _f_mode; }

};


class ConsoleView : public Observer
{
   TemperatureModel *_model;
   Controller *      _controller;
public:
   ConsoleView(TemperatureModel* model, Controller* controller)
      : _model(model)
      , _controller(controller)
   {
      _model->addObserver(this);
   }
   void update() override
   {
      std::cout << "Temperature in Celsius: "   <<  _model->getC() << std::endl;
      std::cout << "Temperature in Farenheit: " <<  _model->getF() << std::endl;
      if (_controller->f_mode())
         std::cout << "Input temperature in Farenheit: ";
      else
         std::cout << "Input temperature in Celsius: ";
   }
};



int main()
{
   TemperatureModel model;
   Controller controller(&model);
   ConsoleView view(&model, &controller);
   controller.start();
   return 0;
}