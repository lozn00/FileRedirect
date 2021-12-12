package cn.qssq666;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Process;
import android.support.annotation.Keep;
import android.support.v4.os.BuildCompat;
import android.util.Log;
import android.util.Pair;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

public class QSSQHook {
    private static final String VESCAPE = "/6decacfa7aad11e8a718985aebe4663a";
    private static final String TAG = "QSSQHook";




//    private static final List<DexOverride> sDexOverrides = new ArrayList<>();

    private static boolean sFlag = false;
    private static boolean sEnabled = false;
    private static boolean sBypassedP = false;

    private static final String LIB_NAME = "prncon";
    private static final String LIB_NAME_64 = "prncon_64";

    static {
        try {
//            if (VirtualRuntime.is64bit()) {
//                System.loadLibrary(LIB_NAME_64);
//            } else {
                System.loadLibrary(LIB_NAME);
//            }
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }


    public static void startDexOverride() {
    /*    List<InstalledAppInfo> installedApps = VirtualCore.get().getInstalledApps(0);
        for (InstalledAppInfo info : installedApps) {
            if (info.appMode != InstalledAppInfo.MODE_APP_USE_OUTSIDE_APK) {
                String originDexPath = getCanonicalPath(info.getApkPath());
                DexOverride override = new DexOverride(originDexPath, null, null, info.getOdexPath());
                sDexOverrides.add(override);
            }
        }
        for (String framework : StubManifest.REQUIRED_FRAMEWORK) {
            File zipFile = VEnvironment.getFrameworkFile32(framework);
            File odexFile = VEnvironment.getOptimizedFrameworkFile32(framework);
            if (zipFile.exists() && odexFile.exists()) {
                String systemFilePath = "/system/framework/" + framework + ".jar";
                sDexOverrides.add(new DexOverride(systemFilePath, zipFile.getPath(), null, odexFile.getPath()));
            }
        }*/
    }

    public static String getRedirectedPath(String origPath) {
        try {
            return nativeGetRedirectedPath(origPath);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
        return origPath;
    }

    public static String resverseRedirectedPath(String origPath) {
        try {
            return nativeReverseRedirectedPath(origPath);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
        return origPath;
    }

    private static final List<Pair<String, String>> REDIRECT_LISTS = new LinkedList<>();


    public static void redirectDirectory(String origPath, String newPath) {
        if (!origPath.endsWith("/")) {
            origPath = origPath + "/";
        }
        if (!newPath.endsWith("/")) {
            newPath = newPath + "/";
        }
        REDIRECT_LISTS.add(new Pair<>(origPath, newPath));
    }

    public static void redirectFile(String origPath, String newPath) {
        if (origPath.endsWith("/")) {
            origPath = origPath.substring(0, origPath.length() - 1);
        }
        if (newPath.endsWith("/")) {
            newPath = newPath.substring(0, newPath.length() - 1);
        }
        REDIRECT_LISTS.add(new Pair<>(origPath, newPath));
    }

    public static void readOnlyFile(String path) {
        try {
            nativeIOReadOnly(path);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public static void readOnly(String path) {
        if (!path.endsWith("/")) {
            path = path + "/";
        }
        try {
            nativeIOReadOnly(path);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public static void whitelistFile(String path) {
        try {
            nativeIOWhitelist(path);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public static void whitelist(String path) {
        if (!path.endsWith("/")) {
            path = path + "/";
        }
        try {
            nativeIOWhitelist(path);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public static void forbid(String path, boolean file) {
        if (!file && !path.endsWith("/")) {
            path = path + "/";
        }
        try {
            nativeIOForbid(path);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public static void enableIORedirect(Context context) {
        if (sEnabled) {
            Log.w(TAG,"已经重定向了");
            return;
        }
        ApplicationInfo coreAppInfo=context.getApplicationInfo();
        Collections.sort(REDIRECT_LISTS, new Comparator<Pair<String, String>>() {
            @Override
            public int compare(Pair<String, String> o1, Pair<String, String> o2) {
                String a = o1.first;
                String b = o2.first;
                return compare(b.length(), a.length());
            }

            private int compare(int x, int y) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                    return Integer.compare(x, y);
                }
                return (x < y) ? -1 : ((x == y) ? 0 : 1);
            }
        });
        for (Pair<String, String> pair : REDIRECT_LISTS) {
            try {
                nativeIORedirect(pair.first, pair.second);
            } catch (Throwable e) {
                Log.e(TAG, Log.getStackTraceString(e));
            }
        }
        try {
            String soPath = new File(coreAppInfo.nativeLibraryDir, "lib" + LIB_NAME + ".so").getAbsolutePath();
            String soPath64 = new File(coreAppInfo.nativeLibraryDir, "lib" + LIB_NAME + ".so").getAbsolutePath();
//            String nativePath = VEnvironment.getNativeCacheDir(VirtualCore.get().isPluginEngine()).getPath();//当前进程可读，可写的目录
            nativeEnableIORedirect(soPath, soPath64,context.getCacheDir().getAbsolutePath(), Build.VERSION.SDK_INT, Build.VERSION.PREVIEW_SDK_INT);

        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
        sEnabled = true;
    }

    public static void launchEngine() {
        if (sFlag) {
            return;
        }
     /*   Object[] methods = {NativeMethods.gOpenDexFileNative,
                NativeMethods.gCameraNativeSetup,
                NativeMethods.gAudioRecordNativeCheckPermission,
                NativeMethods.gMediaRecorderNativeSetup,
                NativeMethods.gAudioRecordNativeSetup,
                NativeMethods.gCameraStartPreview,
                NativeMethods.gCameraNativeTakePicture,
                NativeMethods.gAudioRecordStart,
                NativeMethods.gMediaRecordPrepare};
        try {
            nativeLaunchEngine(methods, VirtualCore.get().getHostPkg(), VirtualRuntime.isArt(), Build.VERSION.SDK_INT, NativeMethods.gCameraMethodType, NativeMethods.gAudioRecordMethodType);
        } catch (Throwable e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }*/
        sFlag = true;
    }

    public static void bypassHiddenAPI(){
        if (Build.VERSION.SDK_INT > 27 || (Build.VERSION.SDK_INT == 27 && Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)) {
            try {
                Method forNameMethod = Class.class.getDeclaredMethod("forName", String.class);
                Class<?> clazz = (Class<?>) forNameMethod.invoke(null, "dalvik.system.VMRuntime");
                Method getMethodMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
                Method getRuntime = (Method) getMethodMethod.invoke(clazz, "getRuntime", new Class[0]);
                Method setHiddenApiExemptions = (Method) getMethodMethod.invoke(clazz, "setHiddenApiExemptions", new Class[]{String[].class});
                Object runtime = getRuntime.invoke(null);
              /*  if (BuildCompat.isEMUI()) {
                    setHiddenApiExemptions.invoke(runtime, new Object[]{
                            new String[]{
                                    "Landroid/",
                                    "Lcom/android/",
                                    "Ljava/lang/",
                                    "Ldalvik/system/",
                                    "Llibcore/io/",
                                    "Lhuawei/"
                            }
                    });
                } else {*/
                    setHiddenApiExemptions.invoke(runtime, new Object[]{
                            new String[]{
                                    "Landroid/",
                                    "Lcom/android/",
                                    "Ljava/lang/",
                                    "Ldalvik/system/",
                                    "Llibcore/io/",
                            }
                    });
//                }
            } catch (Throwable e) {
                e.printStackTrace();
            }
        }
    }

    public static boolean onKillProcess(int pid, int signal) {
        return true;
    }

    public static int onGetCallingUid(int originUid) {
//        Process.getGidForName()
        return 0;
    }
/*

    private static DexOverride findDexOverride(String originDexPath) {
        for (DexOverride dexOverride : sDexOverrides) {
            if (dexOverride.originDexPath.equals(originDexPath)) {
                return dexOverride;
            }
        }
        return null;
    }
*/

    public static void onOpenDexFileNative(String[] params) {
    /*    String dexPath = params[0];
        String odexPath = params[1];
        if (dexPath != null) {
            String dexCanonicalPath = getCanonicalPath(dexPath);
            DexOverride override = findDexOverride(dexCanonicalPath);
            if (override != null) {
                if (override.newDexPath != null) {
                    params[0] = override.newDexPath;
                }
                odexPath = override.newDexPath;
                if (override.originOdexPath != null) {
                    String odexCanonicalPath = getCanonicalPath(odexPath);
                    if (odexCanonicalPath.equals(override.originOdexPath)) {
                        params[1] = override.newOdexPath;
                    }
                } else {
                    params[1] = override.newOdexPath;
                }
            }
        }
        Log.i(TAG, "OpenDexFileNative(\"%s\", \"%s\")", dexPath, odexPath);*/
    }

    private static final String getCanonicalPath(String path) {
        File file = new File(path);
        try {
            return file.getCanonicalPath();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return file.getAbsolutePath();
    }

    private static native void nativeLaunchEngine(Object[] method, String hostPackageName, boolean isArt, int apiLevel, int cameraMethodType, int audioRecordMethodType);

    private static native void nativeMark();

    private static native String nativeReverseRedirectedPath(String redirectedPath);

    private static native String nativeGetRedirectedPath(String origPath);

    private static native void nativeIORedirect(String origPath, String newPath);

    private static native void nativeIOWhitelist(String path);

    private static native void nativeIOForbid(String path);

    public static native boolean nativeCloseAllSocket();

    public static native void nativeChangeDecryptState(boolean state);

    public static native boolean nativeConfigEncryptPkgName(String[] name);

    public static native void nativeAddEncryptPkgName(String name);

    public static native void nativeDelEncryptPkgName(String name);

    public static native boolean nativeGetDecryptState();

    private static native void nativeIOReadOnly(String path);

    private static native void nativeEnableIORedirect(String soPath, String soPath64, String cachePath, int apiLevel, int previewApiLevel);

    private static native void nativeBypassHiddenAPIEnforcementPolicy(int apiLevel, int previewApiLevel);

    public static native boolean nativeConfigNetStrategy(String[] netStrategy,int type);

    public static native void nativeConfigNetworkState(boolean netonOroff);

    public static native void nativeConfigWhiteOrBlack(boolean isWhiteOrBlack);

    public static native void nativeConfigDomainToIp();


 /*   @Keep
    public static int onGetUid(int uid) {
        return 0;
    }*/

    @Keep
    public static void onSystemExit(int code) {
    }

    @Keep
    public static void onSendSignal(int pid, int sig, int quiet) {
    }
/*
    public static String getRedirectedPath(String origPath) {
        try {
            return nativeGetRedirectedPath(origPath);
        } catch (Throwable e) {
            Log.e(TAG, "getRedirectedPath", e);
        }
        return origPath;
    }

    public static String resverseRedirectedPath(String origPath) {
        try {
            return nativeReverseRedirectedPath(origPath);
        } catch (Throwable e) {
            Log.e(TAG, "resverseRedirectedPath", e);
        }
        return origPath;
    }

    public static void redirectDirectory(String origPath, String newPath) {
        if (!origPath.endsWith("/")) {
            origPath = origPath + "/";
        }
        if (!newPath.endsWith("/")) {
            newPath = newPath + "/";
        }
        try {
            nativeIORedirect(origPath, newPath);
        } catch (Throwable e) {
            Log.e(TAG, "redirectDirectory", e);
        }
    }

    public static String getEscapePath(String path) {
        if (path == null) {
            return null;
        }
        File file = new File(path);
        if (file.exists()) {
            return file.getAbsolutePath();
        }
        return new File(VESCAPE, path).getAbsolutePath();
    }

    public static void redirectFile(String origPath, String newPath) {
        if (origPath.endsWith("/")) {
            origPath = origPath.substring(0, origPath.length() - 1);
        }
        if (newPath.endsWith("/")) {
            newPath = newPath.substring(0, newPath.length() - 1);
        }

        try {
            nativeIORedirect(origPath, newPath);
        } catch (Throwable e) {
            Log.e(TAG, "redirectFile", e);
        }
    }

    public static void whitelist(String path) {
        try {
            nativeIOWhitelist(path);
        } catch (Throwable e) {
            Log.e(TAG, "whitelist", e);
        }
    }

    public static void forbid(String path) {
        if (!path.endsWith("/")) {
            path = path + "/";
        }
        try {
            nativeIOForbid(path);
        } catch (Throwable e) {
            Log.e(TAG, "forbid", e);
        }
    }
    public static void enableIORedirect( String packageName) {
        String soPath = String.format("/data/data/%s/lib/xlibprncon.so", packageName);
        if(!new File(soPath).exists()){
        }
        enableIORedirect(soPath, packageName);
    }

    public static void enableIORedirect(String soPath, String packageName) {
        try {

           *//* if (!new File(soPath).exists()) {
                throw new RuntimeException("Unable to find the so ." + soPath);
            }*//*
            nativeEnableIORedirect(soPath, Build.VERSION.SDK_INT, getPreviewSDKInt());
            redirectDirectory(VESCAPE, "/");
        } catch (Throwable e) {
            Log.e(TAG, "enableIORedirect", e);
        }
    }

    public static int getPreviewSDKInt() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            try {
                return Build.VERSION.PREVIEW_SDK_INT;
            } catch (Throwable e) {
                // ignore
            }
        }
        return 0;
    }


//    private static native void nativeLaunchEngine(Object[] method, String hostPackageName, boolean isArt, int apiLevel, int cameraMethodType);

//    private static native void nativeMark();

    private static native String nativeReverseRedirectedPath(String redirectedPath);

    private static native String nativeGetRedirectedPath(String orgPath);

    private static native void nativeIORedirect(String origPath, String newPath);

    private static native void nativeIOWhitelist(String path);

    private static native void nativeIOForbid(String path);

    private static native void nativeEnableIORedirect(String selfSoPath, int apiLevel, int previewApiLevel);

    public static native void fixAndroidO();


    public static native void bwn6a6b();*/
}
