//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "unix_convertor.h"
#include "ip_convertor.h"
#include <cstdlib>
#include <sstream>
#include <cctype>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

CHAR* curChar = NULL;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );

    ShortcutKey *key1 = new ShortcutKey;
    key1->_isAlt = FALSE;
    key1->_isCtrl = TRUE;
    key1->_isShift = FALSE;
    key1->_key = 'T';
    ShortcutKey *key2 = new ShortcutKey;
    key2->_isAlt = FALSE;
    key2->_isCtrl = TRUE;
    key2->_isShift = FALSE;
    key2->_key = 'R';
    TCHAR menu1Name[] = TEXT("unix-string");
    TCHAR menu2Name[] = TEXT("string-unix");
    TCHAR menu3Name[] = TEXT("ip-num");
    TCHAR menu4Name[] = TEXT("num-ip");
    setCommand(0,menu1Name, unix2string, key1, false);
    setCommand(1, menu2Name, string2unix, key2, false);
    setCommand(2, menu3Name, ip2Num, NULL, false);
    setCommand(3, menu4Name, num2Ip, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
    //释放快捷键
    for(int i=0;i<nbFunc;i++){
        if(funcItem[i]._pShKey){
            delete funcItem[i]._pShKey;
            funcItem[i]._pShKey=NULL;
        }
    }
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//



void unix2string()
{
    // Open a new document
//    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    //获取选中文本，开始和结束
    size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);
    size_t end = ::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0);
    //可能从后往前选，这时候要对换开始和结束点
    if (end < start)
    {
        size_t tmp = start;
        start = end;
        end = tmp;
    }
    size_t asciiTextLen = end - start;

    if(asciiTextLen == 0){
        size_t allTextlength = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONSTART, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONEND, allTextlength, (LPARAM)curChar);
        curChar = new CHAR[allTextlength+1];
    }else{
        curChar = new CHAR[asciiTextLen+1];
    }
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)curChar);



    my_plugin::unix_convertor convertor;
    for(int i=0;*(curChar+i)!='\0';i++){
        if(!isdigit(*(curChar+i))){
            return;
        }
    }
    unsigned long long value = strtoull(curChar,NULL,10);
    if(value){
        if(value > 4070880000){
            value /=1000;
        }
        my_plugin::time time;
        convertor.utc_sec_2_time(value + 8*SEC_PER_HOUR,time,false);
        char* unixStr = new char[100];
        memset(unixStr,0,100);
        time.toString(unixStr);
        ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)unixStr);
        delete[] unixStr;
    }
    delete[] curChar;
}

void string2unix()
{
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    //获取选中文本，开始和结束
    size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);
    size_t end = ::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0);
    //可能从后往前选，这时候要对换开始和结束点
    if (end < start)
    {
        size_t tmp = start;
        start = end;
        end = tmp;
    }
    size_t asciiTextLen = end - start;

    if(asciiTextLen == 0){
        size_t allTextlength = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONSTART, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONEND, allTextlength, (LPARAM)curChar);
        curChar = new CHAR[allTextlength+1];
    }else{
        curChar = new CHAR[asciiTextLen+1];
    }
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)curChar);

    my_plugin::unix_convertor convertor;
    my_plugin::time time;
    bool success = my_plugin::time::readTimeFromString(time,curChar);
    if(success){
        unsigned long utc_sec = convertor.time_2_utc_sec(time,false);
        std::stringstream stream;
        //扣除时区偏差
        stream << (utc_sec - 8*SEC_PER_HOUR);
        ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)(stream.str().c_str()));
    }
    delete[] curChar;
}


void ip2Num()
{
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    //获取选中文本，开始和结束
    size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);
    size_t end = ::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0);
    //可能从后往前选，这时候要对换开始和结束点
    if (end < start)
    {
        size_t tmp = start;
        start = end;
        end = tmp;
    }
    size_t asciiTextLen = end - start;

    if(asciiTextLen == 0){
        size_t allTextlength = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONSTART, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONEND, allTextlength, (LPARAM)curChar);
        curChar = new CHAR[allTextlength+1];
    }else{
        curChar = new CHAR[asciiTextLen+1];
    }
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)curChar);

    unsigned int ipNum  = my_plugin::ip_convertor::ip2Num(curChar);
    std::stringstream stream;
    stream << ipNum;
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)(stream.str().c_str()));
    delete[] curChar;
}


void num2Ip()
{
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
    //获取选中文本，开始和结束
    size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);
    size_t end = ::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0);
    //可能从后往前选，这时候要对换开始和结束点
    if (end < start)
    {
        size_t tmp = start;
        start = end;
        end = tmp;
    }
    size_t asciiTextLen = end - start;

    if(asciiTextLen == 0){
        size_t allTextlength = ::SendMessage(curScintilla, SCI_GETLENGTH, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONSTART, 0, (LPARAM)curChar);
        ::SendMessage(curScintilla, SCI_SETSELECTIONEND, allTextlength, (LPARAM)curChar);
        curChar = new CHAR[allTextlength+1];
    }else{
        curChar = new CHAR[asciiTextLen+1];
    }
    ::SendMessage(curScintilla, SCI_GETSELTEXT, 0, (LPARAM)curChar);

    for(int i=0;*(curChar+i)!='\0';i++){
        if(!isdigit(*(curChar+i))){
            return;
        }
    }
    unsigned long value = strtoull(curChar,NULL,10);
    char* buf = new char[100];
    memset(buf,0,100);
    my_plugin::ip_convertor::num2Ip(buf,value);
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)(buf));
    delete[] buf;
    delete[] curChar;
}
