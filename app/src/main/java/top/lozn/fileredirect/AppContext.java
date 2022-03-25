package top.lozn.fileredirect;

import android.app.Application;
import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


public class AppContext extends Application {


    private static final String TAG = "AppContext";

    static {

        try {


            Class<?> aClass = Class.forName("android.app.ActivityThread");
            Method currentActivityThread = aClass.getDeclaredMethod("currentActivityThread");
            currentActivityThread.setAccessible(true);
            //mInstrumentationPackageName
            Object currentActivityThreadObj = currentActivityThread.invoke(null);
            Field mBoundApplication = aClass.getDeclaredField("mBoundApplication");
            mBoundApplication.setAccessible(true);
            Object mBoundApplicationObj = mBoundApplication.get(currentActivityThreadObj);
            Field appInfoField = mBoundApplicationObj.getClass().getDeclaredField("appInfo");
            appInfoField.setAccessible(true);
            Object appInfoObj = appInfoField.get(mBoundApplicationObj);
            Field packgeName = appInfoObj.getClass().getField("packageName");
            packgeName.setAccessible(true);
            Object o = packgeName.get(appInfoObj);
            Log.w(TAG,"PACKAGEnAME:"+o);
            /*
            Field mInstrumentationPackageName = aClass.getDeclaredField("mInstrumentationPackageName");
            mInstrumentationPackageName.setAccessible(true);
            Object o = mInstrumentationPackageName.get(currentActivityThreadObj);
*/

            Log.w(TAG, "PACKAGENAME:" + o);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
//        System.loadLibrary("prncon");
    }

    @Override
    public void onCreate() {

        super.onCreate();

        Log.w(TAG, "LOAD SUCC");

//        WhaleRuntime.fixAndroidO();

//        testRedirect();
//        QSSQHook.fixAndroidO();
    }

}
