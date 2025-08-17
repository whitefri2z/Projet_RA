// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/AllStoragePermissionHelper.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

void FStoragePermissionHelper::RequestManageExternalStorage()
{
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        jclass ActivityClass = Env->GetObjectClass(FJavaWrapper::GameActivityThis);

        jmethodID StartActivityMethod = Env->GetMethodID(ActivityClass, "startActivity", "(Landroid/content/Intent;)V");

        jclass SettingsClass = Env->FindClass("android/provider/Settings");
        jfieldID ActionID = Env->GetStaticFieldID(SettingsClass, "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION", "Ljava/lang/String;");
        jobject ActionString = Env->GetStaticObjectField(SettingsClass, ActionID);

        jclass IntentClass = Env->FindClass("android/content/Intent");
        jmethodID IntentCtor = Env->GetMethodID(IntentClass, "<init>", "(Ljava/lang/String;)V");
        jobject IntentObject = Env->NewObject(IntentClass, IntentCtor, ActionString);

        jclass UriClass = Env->FindClass("android/net/Uri");
        jmethodID ParseMethod = Env->GetStaticMethodID(UriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
        FString Uri = FString::Printf(TEXT("package:%s"), TEXT("com.Cime.ARProject"));
        jstring UriJString = Env->NewStringUTF(TCHAR_TO_UTF8(*Uri));
        jobject UriObject = Env->CallStaticObjectMethod(UriClass, ParseMethod, UriJString);

        jmethodID SetDataMethod = Env->GetMethodID(IntentClass, "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");
        Env->CallObjectMethod(IntentObject, SetDataMethod, UriObject);

        // Lancer l'activité depuis notre Activity
        Env->CallVoidMethod(FJavaWrapper::GameActivityThis, StartActivityMethod, IntentObject);

        // Nettoyage
        Env->DeleteLocalRef(ActivityClass);
        Env->DeleteLocalRef(SettingsClass);
        Env->DeleteLocalRef(IntentClass);
        Env->DeleteLocalRef(UriClass);
        Env->DeleteLocalRef(ActionString);
        Env->DeleteLocalRef(IntentObject);
        Env->DeleteLocalRef(UriObject);
        Env->DeleteLocalRef(UriJString);
    }
}
#else
void FStoragePermissionHelper::RequestManageExternalStorage()
{
    
}
#endif

