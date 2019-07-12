package com.duoyi.dyhttp;

/**
 * Created by Yongpan Song on 2019/4/1.
 */
public class DuoyiHttpApi {
    public native void DyHttpInit(String path);

    static {
        System.loadLibrary("dyhttpsdk");
    }
}
