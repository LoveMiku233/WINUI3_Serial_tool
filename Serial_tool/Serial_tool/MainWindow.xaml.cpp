#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Serial_tool::implementation
{
    TempHum DHT11;
    Serial_Miku com1;
    bool open = false;
    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::tick1(IInspectable const& sender, Windows::Foundation::IUnknown const& from) {
        char dat[20];
        com1.readData(dat, 20);
        getTempHum(dat, &DHT11);
        std::cout << dat << std::endl;
        std::cout << DHT11.temp << std::endl;
        std::cout << DHT11.hum << std::endl;
        temp().Text(to_hstring(DHT11.temp) + L"℃");
        hum().Text(to_hstring(DHT11.hum) + L"%");
    }

    void MainWindow::proc(){
        std::chrono::milliseconds sec(100);
        auto timer=DispatcherTimer();
        timer.Interval(sec);
        timer.Tick({ this,&MainWindow::tick1});
        timer.Start();
    }

    void MainWindow::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        if (!open) {
            if (!com1.openSpy("COM3", CBR_115200, NOPARITY, 8, ONESTOPBIT))
            {
                std::cout << "打开串口" << std::endl;
            }
            proc();
            open = true;
        }
    }
}



