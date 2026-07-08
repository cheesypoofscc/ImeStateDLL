# ImeStateDLL

TSF based Windows IME state DLL.

Return:

0 = English

1 = Chinese


## Build locally

Open:

x64 Native Tools Command Prompt for VS 2022


Run:

build.bat


## Auto build

GitHub:

Actions

↓

Build ImeState DLL

↓

Download artifact


## AutoHotkey v2

Example:

```ahk
state :=
 DllCall(
   "ImeState.dll\GetIMEState",
   "Int"
 )

if state
    MsgBox "Chinese"
else
    MsgBox "English"