package cn.qssq666.fileredirect;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;
import java.util.Random;

import cn.qssq666.QSSQHook;
import cn.qssq666.fileredirect.tool.MyTool;

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

//        QSSQHook.launchEngine();
        QSSQHook.startDexOverride();
        findViewById(R.id.btn_reload).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                QSSQHook.enableIORedirect(MainActivity.this);
                testRedirect();
            }
        });
//        testRedirect();
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


        StringBuffer sb = new StringBuffer();
        sb.append("型号:" + Build.PRODUCT + "\n");
        sb.append("SDk:" + Build.VERSION.SDK_INT + "\n");
        String content_a = "a_file_content" + new Random().nextInt();
        String content_b = "b_file_content_" + new Random().nextInt();


        String content = null;
        File filesDir = this.getFilesDir();
        Log.w(TAG, "FILESDIR" + filesDir + ",");
        File a_file = new File(filesDir, "afile.txt");
        MyTool.writeFile(a_file, content_a);
        File b_file = new File(filesDir, "bfile.txt");

        MyTool.writeFile(b_file, content_b);
        sb.append("写入内容完成\n");

        try {
            content = MyTool.readFile(a_file.getCanonicalPath());
            sb.append("a文件内容:"+content);
            content = MyTool.readFile(b_file.getCanonicalPath());
            sb.append("b文件内容:"+content);
        }catch (Throwable e){

        }

        sb.append("把a文件指向到b文件\n");

        try {
            QSSQHook.redirectFile(a_file.getCanonicalPath(), b_file.getCanonicalPath());//把路径a,指向路径b
        } catch (IOException e) {
            Log.e(TAG, "redirectFile fail ", e);
        }


        String resultA="A读取失败";
        String resultB="B读取失败";
        try {
            content =resultA= MyTool.readFile(a_file.getCanonicalPath());
            sb.append("重定向后读取"+a_file.getName()+"的内容:" + content + "");
        } catch (IOException e) {
            e.printStackTrace();
        }

        sb.append("\n");
        try {
            content = resultB=MyTool.readFile(b_file.getCanonicalPath());
            sb.append("重定向后读取"+b_file.getName()+"的内容:" + content + "");
        } catch (IOException e) {
            e.printStackTrace();
        }
      /*  try {
            WhaleRuntime.forbid(fileb.getCanonicalPath());
        } catch (IOException e) {
            e.printStackTrace();
        }*/
        String result = "";
        if (resultA.equals(resultB)) {
            result = "伪造成功!，内容都是:" + resultB;
            tv.setTextColor(Color.GREEN);
        } else {
            result = "伪造失败!，"+a_file.getName()+"内容是:" + resultA + ","+a_file.getName()+"内容是:" + resultB;
            tv.setTextColor(Color.RED);

        }
        tv.setText(result);
        Log.w(TAG, "" + result);


        String redirectedPath = QSSQHook.getRedirectedPath(a_file.getAbsolutePath());
        sb.append("\n获取"+a_file+"重定向路径:\n" + redirectedPath); //获取的是重定向的路径
        sb.append("\n");
        redirectedPath = QSSQHook.getRedirectedPath(b_file.getAbsolutePath());
        sb.append("\n获取"+b_file+"重定向路径:\n" + redirectedPath);//依然是原来
        sb.append("\n");
        sb.append("\n");

        redirectedPath = QSSQHook.resverseRedirectedPath(a_file.getAbsolutePath());
        sb.append("\n获取"+a_file.getAbsolutePath()+"原始路径\n" + redirectedPath);
        sb.append("\n");
        redirectedPath = QSSQHook.resverseRedirectedPath(b_file.getAbsolutePath());//根据被重定向的路径逆推回去真实原来路径。
        sb.append("\n获取"+b_file.getAbsolutePath()+"原始路径\n" + redirectedPath);
        tv2.setText(sb.toString());
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

    }
}
