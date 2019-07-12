package com.duoyi.duoyidemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import com.duoyi.dyhttp.DuoyiHttpApi;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private DuoyiHttpApi m_DuoyiHttpApi = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String root_path = getStoragePath();

        m_DuoyiHttpApi = new DuoyiHttpApi();
        m_DuoyiHttpApi.DyHttpInit(root_path + File.separator + "1.amr");

        /*File file = new File(root_path + File.separator + "crash.txt");
        if (file.exists()) {
            Log.i("ALLTAG", "file exist " + file.getAbsolutePath());
        } else {
            Log.e("ALLTAG", "file not exist");
        }*/
    }

    private String getStoragePath() {
        File root_dir = null;
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            root_dir = Environment.getExternalStorageDirectory();
        } else {
            root_dir = Environment.getDataDirectory();
        }

        return  root_dir.getAbsolutePath();
    }
}
