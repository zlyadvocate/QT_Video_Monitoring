#include <QApplication>
#include "myhelper.h"
#include "frmmain.h"
#include "myapp.h"
//zly minidump
#include <Windows.h>
#include <DbgHelp.h>
#include <winnt.h>
#include <time.h>
#include <locale>
#include <codecvt>
LONG __stdcall ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    //程式异常捕获
    //创建 Dump 文件
    char str_time[100];
    memset(str_time, 0, 100);
    struct tm *local_time = NULL;
    time_t utc_time;
    utc_time = time(NULL);
    local_time = localtime(&utc_time);
    strftime(str_time, 100, "%Y%m%d%H%M%S", local_time);

    std::string timeStr(str_time);
    std::string dumpFilePath = timeStr + ".dmp";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(dumpFilePath);
    HANDLE hDumpFile = CreateFile(wide.c_str(),GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    fprintf(stderr, "record->ExceptionCode:%lu\n", record->ExceptionCode);
    return EXCEPTION_EXECUTE_HANDLER;
}
#include<tchar.h>
void DisableSetUnhandledExceptionFilter()
{
    HMODULE h_hand = LoadLibrary(_T("kernel32.dll"));

    void *addr = (void*)GetProcAddress(h_hand,"SetUnhandledExceptionFilter");
    if (addr)
    {
        unsigned char code[16];

        int size = 0;

        code[size++] = 0x33;

        code[size++] = 0xC0;

        code[size++] = 0xC2;

        code[size++] = 0x04;

        code[size++] = 0x00;

        DWORD dwOldFlag, dwTempFlag;

        VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);

        WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);

        VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
    }
}

void RunCrashHandlerLocal()
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注册异常捕获函数
}

//endminidump

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RunCrashHandlerLocal();//zly minidump

    a.setApplicationName("VM");         //设置应用程序名称
    a.setApplicationVersion("V201412"); //设置应用程序版本
    myHelper::SetUTF8Code();            //设置程序编码为UTF-8
    myHelper::SetChinese();             //设置程序为中文字库

    //赋值当前应用程序路径和桌面宽度高度
    myApp::AppPath = QApplication::applicationDirPath() + "/";
    //qDebug() << myApp::AppPath;
    myApp::DeskWidth = qApp->desktop()->availableGeometry().width();
    myApp::DeskHeight = qApp->desktop()->availableGeometry().height();

    //程序加载时先加载所有配置信息
    myApp::ReadConfig();

    //加载和应用样式
    myHelper::SetStyle(myApp::AppStyle);

    //创建共享内存,判断是否已经运行程序
    //    QSharedMemory mem("VM");
    //    if (!mem.create(1)) {
    //        myHelper::ShowMessageBoxError("程序已运行,软件将自动关闭!");
    //        return 1;
    //    }

    //判断当前数据库文件是否存在(如果数据库打开失败则终止应用程序)
    if (!myHelper::FileIsExist(myApp::AppPath + "VM.db")) {
        myHelper::ShowMessageBoxError("数据库文件不存在,程序将自动关闭!");
        return 1;
    }

    QSqlDatabase DbConn;
    DbConn = QSqlDatabase::addDatabase("QSQLITE");
    DbConn.setDatabaseName(myApp::AppPath + "VM.db");

    //创建数据库连接并打开(如果数据库打开失败则终止应用程序)
    if (!DbConn.open()) {
        myHelper::ShowMessageBoxError("打开数据库失败,程序将自动关闭！");
        return 1;
    }

    frmMain w;
    w.show();
    w.setGeometry(qApp->desktop()->availableGeometry());

    return a.exec();
}
