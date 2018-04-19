package org.minijvm.activity;

import android.app.NativeActivity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

/**
 * Created by gust on 2018/4/19.
 */

public class JvmNativeActivity extends NativeActivity {
    static {
        System.loadLibrary("glfm_example");
    }

    private final static String TAG = "JvmNativeActivity";

    // android:name="android.app.NativeActivity"
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        View v = getWindow().getDecorView();
        super.onCreate(savedInstanceState);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        String str = event.getCharacters();
        if (str != null) {
            if (onStringInput(str)) return true;
        }
        return super.dispatchKeyEvent(event);
    }


    // Need this for screen rotation to send configuration changed callbacks to native
    @Override
    public void onConfigurationChanged(android.content.res.Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

    public void showKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(this.getWindow().getDecorView(), InputMethodManager.SHOW_FORCED);
    }


    public void hideKeyboard() {
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(this.getWindow().getDecorView().getWindowToken(), 0);
    }

    native boolean onStringInput(String str);
}
