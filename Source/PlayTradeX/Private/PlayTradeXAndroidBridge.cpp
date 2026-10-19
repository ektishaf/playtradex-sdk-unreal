#include "PlayTradeXAndroidBridge.h"

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Async/Async.h"

#include <jni.h>

namespace
{
    FCriticalSection GPlayTradeXPickerMutex;

    FPlayTradeXAndroidBridge::FPickerCallback
        GPlayTradeXImportPickerCallback;

    FPlayTradeXAndroidBridge::FPickerCallback
        GPlayTradeXExportPickerCallback;


    // ============================================================
    // GameActivity Helpers
    //
    // Used only for PlayTradeX Android application initialization.
    // ============================================================

    jmethodID FindGameActivityMethod(
        const char* MethodName,
        const char* Signature
    )
    {
        JNIEnv* Env =
            FAndroidApplication::GetJavaEnv();

        if (Env == nullptr)
        {
            return nullptr;
        }

        if (FJavaWrapper::GameActivityClassID == nullptr)
        {
            return nullptr;
        }

        jmethodID Method =
            Env->GetMethodID(
                FJavaWrapper::GameActivityClassID,
                MethodName,
                Signature
            );

        if (Env->ExceptionCheck())
        {
            Env->ExceptionClear();

            return nullptr;
        }

        return Method;
    }


    bool CallGameActivityVoidMethod(
        const char* MethodName
    )
    {
        JNIEnv* Env =
            FAndroidApplication::GetJavaEnv();

        if (Env == nullptr)
        {
            return false;
        }

        if (FJavaWrapper::GameActivityThis == nullptr)
        {
            return false;
        }

        jmethodID Method =
            FindGameActivityMethod(
                MethodName,
                "()V"
            );

        if (Method == nullptr)
        {
            return false;
        }

        Env->CallVoidMethod(
            FJavaWrapper::GameActivityThis,
            Method
        );

        if (Env->ExceptionCheck())
        {
            Env->ExceptionClear();

            return false;
        }

        return true;
    }


    // ============================================================
    // PlayTradeX Java Class Helpers
    // ============================================================

    jclass FindPlayTradeXFilePickerBridgeClass(
        JNIEnv* Env
    )
    {
        if (Env == nullptr)
        {
            return nullptr;
        }

        jobject Activity =
            FJavaWrapper::GameActivityThis;

        if (Activity == nullptr)
        {
            return nullptr;
        }

        jclass ActivityClass =
            Env->GetObjectClass(
                Activity
            );

        if (ActivityClass == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            return nullptr;
        }

        jmethodID GetClassLoaderMethod =
            Env->GetMethodID(
                ActivityClass,
                "getClassLoader",
                "()Ljava/lang/ClassLoader;"
            );

        if (GetClassLoaderMethod == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            Env->DeleteLocalRef(
                ActivityClass
            );

            return nullptr;
        }

        jobject ClassLoader =
            Env->CallObjectMethod(
                Activity,
                GetClassLoaderMethod
            );

        if (ClassLoader == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            Env->DeleteLocalRef(
                ActivityClass
            );

            return nullptr;
        }

        jclass ClassLoaderClass =
            Env->FindClass(
                "java/lang/ClassLoader"
            );

        if (ClassLoaderClass == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            Env->DeleteLocalRef(
                ClassLoader
            );

            Env->DeleteLocalRef(
                ActivityClass
            );

            return nullptr;
        }

        jmethodID LoadClassMethod =
            Env->GetMethodID(
                ClassLoaderClass,
                "loadClass",
                "(Ljava/lang/String;)Ljava/lang/Class;"
            );

        if (LoadClassMethod == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            Env->DeleteLocalRef(
                ClassLoaderClass
            );

            Env->DeleteLocalRef(
                ClassLoader
            );

            Env->DeleteLocalRef(
                ActivityClass
            );

            return nullptr;
        }

        jstring ClassName =
            Env->NewStringUTF(
                "com.playtradex.PlayTradeXFilePickerBridge"
            );

        if (ClassName == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            Env->DeleteLocalRef(
                ClassLoaderClass
            );

            Env->DeleteLocalRef(
                ClassLoader
            );

            Env->DeleteLocalRef(
                ActivityClass
            );

            return nullptr;
        }

        jobject LoadedClass =
            Env->CallObjectMethod(
                ClassLoader,
                LoadClassMethod,
                ClassName
            );

        Env->DeleteLocalRef(
            ClassName
        );

        Env->DeleteLocalRef(
            ClassLoaderClass
        );

        Env->DeleteLocalRef(
            ClassLoader
        );

        Env->DeleteLocalRef(
            ActivityClass
        );

        if (LoadedClass == nullptr ||
            Env->ExceptionCheck())
        {
            if (Env->ExceptionCheck())
            {
                Env->ExceptionDescribe();
                Env->ExceptionClear();
            }

            return nullptr;
        }

        return static_cast<jclass>(
            LoadedClass
            );
    }

bool CallPlayTradeXFilePickerBridge(
    const char* MethodName
)
{
    JNIEnv* Env =
        FAndroidApplication::GetJavaEnv();

    if (Env == nullptr)
    {
        return false;
    }

    if (FJavaWrapper::GameActivityThis == nullptr)
    {
        return false;
    }

    jclass BridgeClass =
        FindPlayTradeXFilePickerBridgeClass(
            Env
        );

    if (BridgeClass == nullptr)
    {
        if (Env->ExceptionCheck())
        {
            Env->ExceptionDescribe();
            Env->ExceptionClear();
        }

        return false;
    }

    jmethodID Method =
        Env->GetStaticMethodID(
            BridgeClass,
            MethodName,
            "(Landroid/app/Activity;)V"
        );

    if (Env->ExceptionCheck())
    {
        Env->ExceptionDescribe();
        Env->ExceptionClear();

        Env->DeleteLocalRef(
            BridgeClass
        );

        return false;
    }

    if (Method == nullptr)
    {
        Env->DeleteLocalRef(
            BridgeClass
        );

        return false;
    }

    Env->CallStaticVoidMethod(
        BridgeClass,
        Method,
        FJavaWrapper::GameActivityThis
    );

    if (Env->ExceptionCheck())
    {
        Env->ExceptionDescribe();
        Env->ExceptionClear();

        Env->DeleteLocalRef(
            BridgeClass
        );

        return false;
    }

    Env->DeleteLocalRef(
        BridgeClass
    );

    return true;
}


    // ============================================================
    // JNI String Conversion
    // ============================================================

    FString JStringToFString(
        JNIEnv* Env,
        jstring Value
    )
    {
        if (Env == nullptr ||
            Value == nullptr)
        {
            return FString();
        }

        const char* UtfChars =
            Env->GetStringUTFChars(
                Value,
                nullptr
            );

        if (UtfChars == nullptr)
        {
            return FString();
        }

        const FString Result =
            UTF8_TO_TCHAR(
                UtfChars
            );

        Env->ReleaseStringUTFChars(
            Value,
            UtfChars
        );

        return Result;
    }
}


// ============================================================
// Initialize Android Services
// ============================================================

bool FPlayTradeXAndroidBridge::Initialize()
{
#if PLATFORM_ANDROID

    return CallGameActivityVoidMethod(
        "AndroidThunkJava_PlayTradeX_Initialize"
    );

#else

    return false;

#endif
}


// ============================================================
// Open Import Picker
// ============================================================

bool FPlayTradeXAndroidBridge::OpenImportPicker(
    FPickerCallback Callback
)
{
    if (!Callback)
    {
        return false;
    }

    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        if (GPlayTradeXImportPickerCallback)
        {
            return false;
        }

        GPlayTradeXImportPickerCallback =
            MoveTemp(Callback);
    }

    if (!CallPlayTradeXFilePickerBridge(
        "openImportPicker"
    ))
    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        GPlayTradeXImportPickerCallback =
            nullptr;

        return false;
    }

    return true;
}


// ============================================================
// Open Export Picker
// ============================================================

bool FPlayTradeXAndroidBridge::OpenExportPicker(
    FPickerCallback Callback
)
{
    if (!Callback)
    {
        return false;
    }

    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        if (GPlayTradeXExportPickerCallback)
        {
            return false;
        }

        GPlayTradeXExportPickerCallback =
            MoveTemp(Callback);
    }

    if (!CallPlayTradeXFilePickerBridge(
        "openExportPicker"
    ))
    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        GPlayTradeXExportPickerCallback =
            nullptr;

        return false;
    }

    return true;
}


// ============================================================
// Import Result
// ============================================================

void FPlayTradeXAndroidBridge::HandleImportPickerResult(
    const FString& Uri
)
{
    FPickerCallback Callback;

    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        Callback =
            MoveTemp(
                GPlayTradeXImportPickerCallback
            );

        GPlayTradeXImportPickerCallback =
            nullptr;
    }

    if (!Callback)
    {
        return;
    }

    AsyncTask(
        ENamedThreads::GameThread,
        [
            Callback = MoveTemp(Callback),
            Uri
        ]() mutable
        {
            Callback(Uri);
        }
    );
}


// ============================================================
// Export Result
// ============================================================

void FPlayTradeXAndroidBridge::HandleExportPickerResult(
    const FString& Uri
)
{
    FPickerCallback Callback;

    {
        FScopeLock Lock(
            &GPlayTradeXPickerMutex
        );

        Callback =
            MoveTemp(
                GPlayTradeXExportPickerCallback
            );

        GPlayTradeXExportPickerCallback =
            nullptr;
    }

    if (!Callback)
    {
        return;
    }

    AsyncTask(
        ENamedThreads::GameThread,
        [
            Callback = MoveTemp(Callback),
            Uri
        ]() mutable
        {
            Callback(Uri);
        }
    );
}


// ============================================================
// Java → Unreal
//
// Stable JNI boundary owned by com.playtradex.
// ============================================================

extern "C"
JNIEXPORT void JNICALL
Java_com_playtradex_PlayTradeXFilePickerBridge_nativeImportResult(
    JNIEnv* Env,
    jclass,
    jstring Uri
)
{
    const FString Result =
        JStringToFString(
            Env,
            Uri
        );

    FPlayTradeXAndroidBridge::
        HandleImportPickerResult(
            Result
        );
}


extern "C"
JNIEXPORT void JNICALL
Java_com_playtradex_PlayTradeXFilePickerBridge_nativeExportResult(
    JNIEnv* Env,
    jclass,
    jstring Uri
)
{
    const FString Result =
        JStringToFString(
            Env,
            Uri
        );

    FPlayTradeXAndroidBridge::
        HandleExportPickerResult(
            Result
        );
}

#endif