#include <windows.h>
#include <msctf.h>
#include <oleauto.h>
#include <atomic>


#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")


static std::atomic<int> g_state(0);



int ReadTSFState()
{
    HRESULT hr;

    bool init = false;


    hr = CoInitializeEx(
        nullptr,
        COINIT_APARTMENTTHREADED
    );


    if(SUCCEEDED(hr))
        init = true;



    ITfThreadMgr* tm = nullptr;


    hr = CoCreateInstance(
        CLSID_TF_ThreadMgr,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_ITfThreadMgr,
        (void**)&tm
    );


    if(FAILED(hr) || !tm)
    {
        if(init)
            CoUninitialize();

        return 0;
    }



    TfClientId id;


    tm->Activate(
        &id
    );



    ITfDocumentMgr* dm = nullptr;


    tm->GetFocus(
        &dm
    );



    if(!dm)
    {
        tm->Deactivate();
        tm->Release();

        if(init)
            CoUninitialize();

        return 0;
    }



    ITfContext* ctx=nullptr;


    dm->GetTop(
        &ctx
    );


    dm->Release();



    if(!ctx)
    {
        tm->Deactivate();
        tm->Release();

        if(init)
            CoUninitialize();

        return 0;
    }



    ITfCompartmentMgr* cm=nullptr;


    ctx->QueryInterface(
        IID_ITfCompartmentMgr,
        (void**)&cm
    );


    ctx->Release();



    if(!cm)
    {
        tm->Deactivate();
        tm->Release();

        if(init)
            CoUninitialize();

        return 0;
    }



    ITfCompartment* cp=nullptr;


    cm->GetCompartment(
        GUID_COMPARTMENT_KEYBOARD_INPUTMODE_CONVERSION,
        &cp
    );


    cm->Release();



    int result=0;


    if(cp)
    {
        VARIANT v;

        VariantInit(&v);


        cp->GetValue(
            &v
        );


        if(v.vt==VT_I4)
        {
            if(v.lVal & TF_CONVERSIONMODE_NATIVE)
                result=1;
        }


        VariantClear(
            &v
        );


        cp->Release();
    }



    tm->Deactivate();
    tm->Release();



    if(init)
        CoUninitialize();



    return result;
}





extern "C"
__declspec(dllexport)
int GetIMEState()
{
    g_state =
        ReadTSFState();

    return g_state.load();
}