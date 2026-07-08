#include <windows.h>
#include <msctf.h>
#include <imm.h>
#include <oleauto.h>

#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")
#pragma comment(lib,"imm32.lib")


extern "C"
__declspec(dllexport)
int GetIMEState()
{
    HWND hwnd =
        GetForegroundWindow();


    if(!hwnd)
        return 0;



    DWORD pid = 0;


    DWORD tid =
        GetWindowThreadProcessId(
            hwnd,
            &pid
        );


    if(!tid)
        return 0;



    DWORD currentTid =
        GetCurrentThreadId();



    bool attached = false;



    if(currentTid != tid)
    {
        attached =
            AttachThreadInput(
                currentTid,
                tid,
                TRUE
            );
    }



    int result = 0;



    HRESULT hr =
        CoInitializeEx(
            nullptr,
            COINIT_APARTMENTTHREADED
        );



    ITfThreadMgr* tm = nullptr;



    hr =
        CoCreateInstance(
            CLSID_TF_ThreadMgr,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_ITfThreadMgr,
            (void**)&tm
        );



    if(SUCCEEDED(hr) && tm)
    {

        TfClientId clientId = 0;


        tm->Activate(
            &clientId
        );



        ITfDocumentMgr* docMgr=nullptr;



        tm->GetFocus(
            &docMgr
        );



        if(docMgr)
        {

            ITfContext* ctx=nullptr;


            docMgr->GetTop(
                &ctx
            );


            docMgr->Release();



            if(ctx)
            {

                ITfCompartmentMgr* mgr=nullptr;



                ctx->QueryInterface(
                    IID_ITfCompartmentMgr,
                    (void**)&mgr
                );



                if(mgr)
                {

                    ITfCompartment* comp=nullptr;



                    mgr->GetCompartment(
                        GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION,
                        &comp
                    );



                    if(comp)
                    {

                        VARIANT var;

                        VariantInit(&var);



                        comp->GetValue(
                            &var
                        );



                        if(var.vt == VT_I4)
                        {

                            LONG mode =
                                var.lVal;



                            if(mode &
                              TF_CONVERSIONMODE_NATIVE)
                            {
                                result = 1;
                            }
                        }



                        VariantClear(
                            &var
                        );


                        comp->Release();
                    }



                    mgr->Release();
                }


                ctx->Release();
            }
        }


        tm->Deactivate();

        tm->Release();
    }



    if(attached)
    {
        AttachThreadInput(
            currentTid,
            tid,
            FALSE
        );
    }



    if(SUCCEEDED(hr))
        CoUninitialize();



    return result;
}
