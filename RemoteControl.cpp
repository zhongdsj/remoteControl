#include "Server/Client.h"
#include "Server/User.h"
#include "WindowView/MyWindow.h"
#include "WindowView/Handle/MessageHandle.h"
#pragma comment(lib, "ws2_32.lib")


bool init()
{
    WSADATA  wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup error:" << GetLastError() << std::endl;
        return false;
    }
    return true;
}

void close(Client* client)
{
    client->close();
    WSACleanup();
}

// TODO 处理消息放入window类中, 完善Element类型如button, text, input。

LRESULT CALLBACK WndProc(HWND hwnd, UINT message/*窗口消息*/, WPARAM wParam, LPARAM lParam)
{
	//处理消息
	switch (message)
	{
	case WM_CREATE:
		/*窗口在创建时，会接收到该消息，通常在这里进行初始化操作*/	
		return 0;
	case WM_SIZE:
		/*窗口被改变大小时，会接收到该消息，在窗口被创建时也会收到一次*/
		return 0;
	case WM_PAINT:
		/*窗口有绘图操作更新时,会收到这个消息*/
		return 0;
	case WM_DESTROY:
		/*关不窗口时，会收到该消息，PostQuitMessage()像系统表明终止当前线程，没有这个函数的话，窗口不会关闭*/
		PostQuitMessage(0);
		return 0;
	}
	//将不需要处理的消息传递给系统作默认处理
	return DefWindowProc(hwnd, message, wParam, lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
{
    
    LPCWSTR szAppName = TEXT("HelloWin"); //创建窗口的时候需要一个窗口类名
	HWND hwnd;    //创建窗口函数CreateWindow()会返回一个HWND句柄，这里定义下，用来接收这个句柄
	MSG msg;      //消息结构体，在消息循环的时候需要
	WNDCLASS wndclass; //创建窗口类对象
 
 
    //对窗口类的各属性进行初始化
	wndclass.style = CS_HREDRAW | CS_VREDRAW; /*窗口类的风格，CS前缀,C表示Class,S表示
                    Style，这里使用了水平和垂直风格*/
	wndclass.lpfnWndProc = WndProc;  /*这里将回到函数的名字赋值用以windows后面回调*/
	wndclass.cbClsExtra = 0;  //附加参数，通常情况下为0
	wndclass.cbWndExtra = 0;  //附加参数，通常情况下为0
	wndclass.hInstance = hInstance;  //窗口句柄，这里将WinMain中的hInstance句柄赋值就可
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); /*窗口图标，LoadIcon()是加载图标，这里是加载一个系统资源图标，LoadIcon()的原型是HICON LoadIcon(HINSTANCE, LPCSTR);*/
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  /*加载鼠标，同上相似*/
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  /*窗口画刷，这里是使用的白色画刷，所以创建出来的窗口的背景颜色则是白色的*/
	wndclass.lpszMenuName = NULL;  //窗口菜单名称，这里没有菜单，设为NULL
	wndclass.lpszClassName = szAppName;  //窗口类名称，这个窗口类名称可作为这个窗口的唯一标识
 
    /*注册窗口类*/
	if (!RegisterClass(&wndclass))
	{
        //注册窗口类失败时，弹出提示
		MessageBox(NULL, TEXT("This program requires Window NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	MyWindow* win = new MyWindow(0, szAppName, TEXT("The Hello Program"), WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance);
 
    /*显示窗口，显示方式使用WinMain的参数*/
	win->showWindow(nCmdShow);
    /*更新窗口*/
	win->updateWindow();

	BOOL bRet;
    /*消息循环，获取消息*/
	while (bRet = GetMessage(&msg, NULL, NULL, 0) != 0)
	{
		if(bRet != -1)
		{
			//翻译消息
			TranslateMessage(&msg);
			//派发消息
			DispatchMessage(&msg);
		}
	}
    //返回消息的wParam参数给系统
	return msg.wParam;
}

int main1(int argc, char* argv[])
{
    if(!init()) return -1;
    Client* client = new Client((User*)0, 9001, 120);
    // 调用此方法进行连接
    client->connect(222, "127.0.0.1", 9002, true);
    client->setHandle(new MessageHandle());
    // char buffer[1000];
    std::string buffer;
    std::string key("message");
    client->start();
    while (buffer != "exit")
    {
        std::cout << "输入消息: ";
        std::cin >> buffer;
        if(buffer == "exit")
        {
            
            break;
        }
        client->sendMessage(222, key, buffer);
    }
    
    close(client);
    delete client;
    return 0;
}
