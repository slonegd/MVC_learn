#include <string>
#include <iostream>

using DataChangeHandler = void (*) (const std::string& newData);
// typedef void (*DataChangeHandler)(const std::string& newData);

// Model is responsible for data get and set
class Model
{
    std::string data;
    DataChangeHandler event {nullptr};
public:
    Model(const std::string& data) { this->SetData(data); }
    Model() = default;

    const std::string Data() { return this->data; }

    void SetData (const std::string& data)
    {
        this->data = data;
        if (event != nullptr)
            event (data);
    }

    void RegisterDataChangeHandler (DataChangeHandler handler)
    {
        event = handler;
    }
};

// View is responsible to present data to users
class View
{
    Model& model;
public:
    View (Model& model) : model(model) { }
    void Render() { std::cout << "Model Data = " << model.Data() << std::endl; }
};

// Controller combines Model and View
class Controller
{
    Model& model;
    View& view;
public:
    Controller (Model& model, View& view) : model(model), view(view) { }
    // when application starts
    void OnLoad() { view.Render(); }
};


void DataChange (const std::string& data)
{
   std::cout << "Data Changes: " << data << std::endl;
}
 
int main()
{
    Model model ("Model");
    View view (model);    
    // register the data-change event
    model.RegisterDataChangeHandler (DataChange);
    // binds model and view.
    Controller controller (model, view);
    // when application starts or button is clicked or form is shown...
    controller.OnLoad();
    model.SetData ("Changes"); // this should trigger View to render (НЕПРАВДА!!!!)
    model.SetData ("My");
    return 0;
}