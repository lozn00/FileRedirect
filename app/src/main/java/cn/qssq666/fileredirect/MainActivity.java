package cn.qssq666.fileredirect;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;
import java.util.Random;

import cn.qssq666.QSSQHook;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private TextView tv;
    private TextView tv2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView) findViewById(R.id.tv_text);
        tv2 = (TextView) findViewById(R.id.tv_text2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                this.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0);
            } else {

            }
        }
        QSSQHook.enableIORedirect(this,this.getPackageName());
        testRedirect();
    }

    private void testRedirect() {

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
        File filesDir = this.getFilesDir();
        Log.w(TAG, "FILESDIR" + filesDir + ",");
        File filea = new File(filesDir, "mya.txt");
        AppContext.writeFile(filea, "aaaaa" + new Random().nextInt());
        File fileb = new File(filesDir, "myb.txt");

        AppContext.writeFile(fileb, "bbbb" + new Random().nextInt());
        try {
            QSSQHook.redirectFile(filea.getCanonicalPath(), fileb.getCanonicalPath());//把路径a,指向路径b
        } catch (IOException e) {
            Log.e(TAG, "redirectFile fail ", e);
        }

        StringBuffer sb=new StringBuffer();
      sb.append("型号:" + Build.PRODUCT+"\n");
      sb.append("SDk:" + Build.VERSION.SDK_INT+"\n");
        String stra = "aa";
        String strb = "bb";
        try {
            s = AppContext.readFile(filea.getCanonicalPath());
            stra = s;
        } catch (IOException e) {
            e.printStackTrace();
        }
      sb.append("重定向后读取的内容/data/data_a:" + s+"");
        sb.append("\n");
        try {
            s = AppContext.readFile(fileb.getCanonicalPath());
            strb = s;
        } catch (IOException e) {
            e.printStackTrace();
        }
     sb.append("重定向后读取的内容/data/data_b:" + s);
      /*  try {
            WhaleRuntime.forbid(fileb.getCanonicalPath());
        } catch (IOException e) {
            e.printStackTrace();
        }*/
        String result = "";
        if (stra.equals(strb)) {
            result = "伪造成功!，内容都是:" + stra;
        tv.setTextColor(Color.GREEN);
        } else {
            result = "伪造失败!，a内容是:" + stra + ",b内容是:" + strb;
        tv.setText(result);
        tv.setTextColor(Color.RED);

        }
        Log.w(TAG, ""+result);


        String redirectedPath = QSSQHook.getRedirectedPath(filea.getAbsolutePath());
        sb.append("\n获取重定向a路径:" + redirectedPath); //获取的是重定向的路径
        redirectedPath = QSSQHook.getRedirectedPath(fileb.getAbsolutePath());
        sb.append("\n获取重定向b路径:" + redirectedPath);//依然是原来

        redirectedPath = QSSQHook.resverseRedirectedPath(filea.getAbsolutePath());
        sb.append("\n获取反向重定向a路径:" + redirectedPath);
        redirectedPath = QSSQHook.resverseRedirectedPath(fileb.getAbsolutePath());//根据被重定向的路径逆推回去真实原来路径。
        sb.append("\n获取反向重定向b路径:" + redirectedPath);
tv2.setText(sb.toString());
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

    }
}
