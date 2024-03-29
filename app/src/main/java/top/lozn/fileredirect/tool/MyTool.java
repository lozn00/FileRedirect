package top.lozn.fileredirect.tool;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

import top.lozn.QSSQHook;

public class MyTool {

    private static final String TAG = "MyTool";

    public static void testRedirect(Context context) {

     /*   Log.w(TAG, "onCreate CALL");
        String s;
        s = readFile(filea);
        Log.w(TAG, "读取的内容:" + s);
        WhaleRuntime.redirectFile(filea, fileb);//把路径a,指向路径b
        s = readFile(filea);
        Log.w(TAG, "重定向后读取的内容:" + s);
        WhaleRuntime.redirectDirectory(patha, pathb);
        //resverseRedirectedPath
        String redirectedPath = null;
        redirectedPath = WhaleRuntime.getRedirectedPath(filea);
        Log.w(TAG, "获取重定向a路径:" + redirectedPath); //获取的是重定向的路径
        redirectedPath = WhaleRuntime.getRedirectedPath(fileb);
        Log.w(TAG, "获取重定向b路径:" + redirectedPath);//依然是原来

        redirectedPath = WhaleRuntime.resverseRedirectedPath(filea);
        Log.w(TAG, "获取反向重定向a路径:" + redirectedPath);
        redirectedPath = WhaleRuntime.resverseRedirectedPath(fileb);//根据被重定向的路径逆推回去真实原来路径。
        Log.w(TAG, "获取反向重定向b路径:" + redirectedPath);
*/
        String s = null;
        File filesDir = context.getFilesDir();
        Log.w(TAG, "FILESDIR" + filesDir + ",");
        File filea = new File(filesDir, "mya.txt");
        writeFile(filea, "aaaaa"+new Random().nextInt());
        File fileb = new File(filesDir, "myb.txt");

        writeFile(fileb, "bbbb"+new Random().nextInt());
        try {
            QSSQHook.redirectFile(filea.getCanonicalPath(), fileb.getCanonicalPath());//把路径a,指向路径b
        } catch (IOException e) {
            Log.e(TAG,"redirectFile fail ",e);
        }

        String stra = "aa";
        String strb = "bb";
        try {
            s = readFile(filea.getCanonicalPath());
            stra = s;
        } catch (IOException e) {
            e.printStackTrace();
        }
        Log.w(TAG, "重定向后读取的内容/data/data_a:" + s);
        try {
            s = readFile(fileb.getCanonicalPath());
            strb = s;
        } catch (IOException e) {
            e.printStackTrace();
        }
        Log.w(TAG, "重定向后读取的内容/data/data_b:" + s);
      /*  try {
            WhaleRuntime.forbid(fileb.getCanonicalPath());
        } catch (IOException e) {
            e.printStackTrace();
        }*/
        if (stra.equals(strb)) {
            Log.w(TAG, "伪造成功!，内容都是:" + stra);
        } else {
            Log.e(TAG, "伪造失败!，a内容是:" + stra + ",b内容是:" + strb);

        }

        String redirectedPath = QSSQHook.getRedirectedPath(filea.getAbsolutePath());
        Log.w(TAG, "获取重定向a路径:" + redirectedPath); //获取的是重定向的路径
        redirectedPath = QSSQHook.getRedirectedPath(fileb.getAbsolutePath());
        Log.w(TAG, "获取重定向b路径:" + redirectedPath);//依然是原来

        redirectedPath = QSSQHook.resverseRedirectedPath(filea.getAbsolutePath());
        Log.w(TAG, "获取反向重定向a路径:" + redirectedPath);
        redirectedPath = QSSQHook.resverseRedirectedPath(fileb.getAbsolutePath());//根据被重定向的路径逆推回去真实原来路径。
        Log.w(TAG, "获取反向重定向b路径:" + redirectedPath);
    }

    public static boolean writeFile(File path, String stro) {
        try {
            FileWriter fileWriter = new FileWriter(path);
            BufferedWriter bufferedReader = new BufferedWriter(fileWriter);
            String s;

            bufferedReader.write(stro);
            bufferedReader.flush();//不进行flush就会提示流关闭了。
            bufferedReader.close();
            fileWriter.close();
            return true;
        } catch (Exception e) {
            Log.e(TAG, "write fail", e);
            return false;
        }


    }

    public static String readFile(String path) {
        try {
            FileReader in = new FileReader(path);
            BufferedReader bufferedReader = new BufferedReader(in);
            String s;
            StringBuffer sb = new StringBuffer();

            while ((s = bufferedReader.readLine()) != null) {
                sb.append(s + "\n");

            }
            in.close();
            bufferedReader.close();
            return sb.toString();
        } catch (Exception e) {
            Log.e(TAG, "read fail", e);
            return null;
        }
    }
}
