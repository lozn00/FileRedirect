package cn.qssq666;

import android.os.Build;
import android.util.Log;

import java.io.File;

public class QSSQHook {
    private static final String VESCAPE = "/6decacfa7aad11e8a718985aebe4663a";
    private static final String TAG = "QSSQHook";

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

    public static void enableIORedirect(String packageName) {
        try {
            String soPath = String.format("/data/data/%s/lib/libqssqredirect.so", packageName);
       /*     if (!new File(soPath).exists()) {
                throw new RuntimeException("Unable to find the so ." + soPath);
            }*/
            nativeEnableIORedirect(soPath, Build.VERSION.SDK_INT, getPreviewSDKInt());
//            redirectDirectory(VESCAPE, "/");
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


    public static native void bwn6a6b();
}
